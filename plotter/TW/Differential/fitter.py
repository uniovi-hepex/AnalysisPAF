import itertools, math
import ROOT
import copy, os
import varList
from warnings import warn,simplefilter

simplefilter("always", UserWarning)

class FittingSuite: 
    def __init__(self, cardFile, doExpect=None, doSmear=False):
        self.cardFile = cardFile
        self.doExpect = doExpect
        self.smear    = doSmear
        self.pmap = {}
        self.procMap  = {'Fakes': 'Bkg',
                         'tW'   : 'Signal',
                         'VVttV': 'Bkg',
                         'DY'   : 'Bkg',
                         'ttbar': 'Bkg'}
        self.readFromCard()
        self.VarName  = 'LeadingJetPt'
        

    def readFromCard(self):
        tf = ROOT.TFile.Open(self.cardFile)

        for key in tf.GetListOfKeys():
            name = key.GetName()
            obj  = key.ReadObj()
            if (obj.InheritsFrom("THStack")): continue
            nuis = '_'.join(name.split('_')[1:])
            proc = name.split('_')[0]

            if 'statbin' in nuis: continue

            if nuis not in self.pmap: 
                self.pmap[nuis] = {proc: copy.deepcopy(obj)} 
            else:
                self.pmap[nuis][proc] = copy.deepcopy(obj)
        # check that we have all the nuisances in all the processes
        nProc = -1
        prevKey = ""
        for key in self.pmap:
            if key =='obs': continue # we will have one category for data
            if (nProc < 0):
                nProc = len(self.pmap[key])
            else:
                if nProc != len(self.pmap[key]): 
                    for proc in self.pmap['']:
                        if proc not in self.pmap[key]: 
                            #print '[I]: Proc not in %s 
                            self.pmap[key][proc] = self.pmap[''][proc]
        
        nProc = -1
        for key in self.pmap:
            if key == 'obs': continue
            if 'statbin' in key: continue
            if (nProc < 0):
                nProc = len(self.pmap[key])
                prevNuis = key
            else:
                if nProc != len(self.pmap[key]): raise RuntimeError("Nuisance %s does not have the same number of processes as one the previous ones (%s). Something has gone wrong..."%(key,prevNuis))
        
        if self.doExpect:
            self.pmap['obs']['data'].Reset()
            for key in self.pmap['']:
                if self.procMap[key]=='Signal':
                    scaledSignal = copy.deepcopy(self.pmap[''][key])
                    scaledSignal.Scale(self.doExpect)
                    self.pmap['obs']['data'].Add(scaledSignal)
                else:
                    self.pmap['obs']['data'].Add(self.pmap[''][key])
            random = ROOT.TRandom3()
            for b in xrange(1,self.pmap['obs']['data'].GetNbinsX()+1):
                med = self.pmap['obs']['data'].GetBinContent(b)
                self.pmap['obs']['data'].SetBinContent(b,int(med) if not self.smear else int(random.Poisson(med)))
                                

                
        #print 'List of nuisances' + '\n '.join([key for key in self.pmap])
        #print 'List of processes' + ', '.join([key for key in self.pmap['']])

    def doAllFits(self):
        self.results = {}
        for key in self.pmap:
            self.results[key] = self.doFit(key)

        for key in self.pmap:
            if self.results[key]==None: continue
            #print key, self.results[key], 100*(self.results[key]-self.results[''])/self.results['']



    def doFit(self, nuis):
        if 'obs' not in self.pmap or 'data' not in self.pmap['obs']: raise RuntimeError("Data not in pmap")
        if 'obs' in nuis: return
        if 'statbin' in nuis: return 

        
        pmap = self.pmap[nuis]

        data = self.pmap['obs']['data']

        gKill = ROOT.RooMsgService.instance().globalKillBelow()
        ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.WARNING)        
        w = ROOT.RooWorkspace("w","w")
        x = w.factory("x[%g,%g]" % (data.GetXaxis().GetXmin(), data.GetXaxis().GetXmax()))
        x.setBins(data.GetNbinsX())
        obs = ROOT.RooArgList(w.var("x"))
        rdhs = {}
        w.imp = getattr(w, 'import')
        for p,h in pmap.iteritems():
            rdhs[p] = ROOT.RooDataHist("hist_"+p,"",obs,h)
            w.imp(rdhs[p])
        rdhs['data'] = ROOT.RooDataHist("hist_data","",obs,data)
        w.imp(rdhs['data'])

        pdfs   = ROOT.RooArgList()
        coeffs = ROOT.RooArgList()
        constraints = ROOT.RooArgList()
        dontDelete = []
        procNormMap = {}


        for p in pmap:
            if pmap[p].Integral() == 0: continue
            hpdf = ROOT.RooHistPdf("pdf_"+p,"",ROOT.RooArgSet(x), rdhs[p])
            pdfs.add(hpdf); dontDelete.append(hpdf)

            if self.procMap[p] == 'Signal':
                #signal is left to float
                normTermName = p
                normterm = w.factory('prod::norm_%s(%g,syst_%s[1,%g,%g])' % (p, pmap[p].Integral(), normTermName, 0.1, 5))
                dontDelete.append((normterm,))
                coeffs.add(normterm)
                procNormMap[p] = normterm


            else:
                normTermName = p
                normterm = w.factory('expr::norm_%s("%g*pow(%g,@0)",syst_%s[-5,5])' % (p, pmap[p].Integral(), 1+self.procMap[p], normTermName))
                if not w.pdf("systpdf_%s" % normTermName): 
                    constterm = w.factory('SimpleGaussianConstraint::systpdf_%s(syst_%s,0,1)' % (normTermName,normTermName))
                    constraints.add(constterm)
                    dontDelete.append((normterm,constterm))
                else:
                    dontDelete.append((normterm))
                coeffs.add(normterm)
                procNormMap[p] = normterm


            # statistical uncertainties of MC
            for b in xrange(1,pmap[p].GetNbinsX()+1):
                key = "%s_%d" % (p,b)
                nuislists = ROOT.RooArgList()
                tlist = ROOT.TList(); tlist.Add(pmap[p])
                y, e = pmap[p].GetBinContent(b), pmap[p].GetBinError(b)
                if not y: continue
                if e/y    < 0.005: continue
                if e < 0.2*math.sqrt(y+1): continue
                hi = pmap[p].Clone(); hi.SetDirectory(None)
                lo = pmap[p].Clone(); lo.SetDirectory(None)
                hi.SetBinContent(b, y+e)
                lo.SetBinContent(b, y*y/(y+e))
                tlist.Add(hi)
                tlist.Add(lo)
                constterm = w.factory("Gaussian::nuis_{0}_shapePdf(nuis_{0}_shape[0,-3,3], 0, 1)".format(key))
                nuislists.add(w.var("nuis_{0}_shape".format(key)))
                coeffs.add(w.var("nuis_{0}_shape".format(key)))
                constraints.add(constterm)
                dontDelete.append((constterm,nuislists))
                statpdf = ROOT.FastVerticalInterpHistPdf2(p + "_stat%d"%b, "", w.var("x"), tlist, nuislists, 1., 1)
                pdfs.add(statpdf)
                dontDelete.append(statpdf)

#        pdfs.Print("V")
#        constraints.Print("V")

        

        addpdf = ROOT.RooAddPdf("tot","",pdfs,coeffs,False)
        model  = addpdf
        if constraints.getSize() > 0:
            constraints.add(addpdf)
            #constraints.Print()
            model = ROOT.RooProdPdf("prod","",constraints)
            #model.Print() 
        getattr(w,'import')(model)


        theModel = ROOT.RooStats.ModelConfig(w)
        theModel.SetPdf('prod')
        theModel.SetObservables('x')
        theModel.SetParametersOfInterest('syst_tW')
        theModel.SetNuisanceParameters('syst_DY,syst_NonWZ,syst_VV,syst_ttV,syst_ttbar')

        result = model.fitTo( rdhs["data"], ROOT.RooFit.Save(1), ROOT.RooFit.PrintLevel(-1) )


        out = ROOT.TFile.Open("postfit_%s.root"%nuis,'recreate')
        for p in procNormMap:
            #print p, procNormMap[p].getVal(),procNormMap[p].getVal()/pmap[p].Integral()
            
            output = copy.deepcopy(pmap[p])
            output.SetName('%s_prefit'%p)
            output.Write()
            
            output = copy.deepcopy(pmap[p])
            output.SetName('%s_postfit'%p)
            output.Scale( procNormMap[p].getVal()/pmap[p].Integral() )
            output.Write()
        data.Write()
        out.Close()
        signal = procNormMap['tW'].getVal()/pmap['tW'].Integral()
        w.IsA().Destructor(w)
        print nuis, signal
        return signal 

    def makeCardFromCard(self, nuis):
        pmap = self.pmap[nuis]
        data = self.pmap['obs']['data']

        #print 'we are coming here:', 'temp/cardFile_%s.root'%nuis
        cardFile = ROOT.TFile.Open('temp/cardFile_%s_%s.root'%(self.VarName, nuis),'recreate')
        data.Write()
        for proc in pmap:
            histo = copy.deepcopy(pmap[proc])
            histo.SetName(proc)
            histo.Write()
        cardFile.Close()
        
        template = open('datacard_template.txt').read()
        yields = {key: histo.Integral() for key,histo in pmap.items()}
        template = template.format(name = self.VarName, nuis=nuis, obs=int(data.Integral()), p=yields)

        card = open('temp/datacard_%s_%s.txt'%(self.VarName, nuis), 'w')
        card.write(template)
        card.close()


    def doCombFit(self, nuis):
        # some good ol' sanity and consistency checks
        if 'obs' not in self.pmap or 'data' not in self.pmap['obs']: raise RuntimeError("Data not in pmap")
        if 'obs' in nuis: return
        if 'statbin' in nuis: return 

        # make the card
        print "\n> Creating card for nuisance", nuis, "\n"
        self.makeCardFromCard(nuis)

        print "> Fitting...\n"
        # do fthe fit (dont correct the typo, it may be there for a resason...)
        if nuis != '': 
            os.system('combine -M MultiDimFit temp/datacard_{name}_{nuis}.txt --name fit_{name}_{nuis} '.format(name = self.VarName, nuis = nuis))
        
            # harvest the results
            result = ROOT.TFile.Open('higgsCombinefit_%s_%s.MultiDimFit.mH120.root'%(self.VarName, nuis))
        
            if result.limit.GetEntries() > 1: raise RuntimeError("More than one entry in the limits tree, something unplanned happened")
        
            for limit in result.limit:
                if limit.r < 0.1 or limit.r > 1.9: 
                    warn('Nuisance fit has probably noy converged. Limit:' + str(limit.r), UserWarning)
                    #raise RuntimeError('Nuisance fit has probably not converged. Limit: ' + str(limit.r))
                    #return 1
                return limit.r
            raise RuntimeError('It shouldnt get here')

        else:
            os.system('combine -M MultiDimFit temp/datacard_{name}_{nuis}.txt --name fit_{name}_{nuis} --algo=singles --robustFit=1'.format(name = self.VarName, nuis = nuis))
            # harvest the results
            result = ROOT.TFile.Open('higgsCombinefit_%s_%s.MultiDimFit.mH120.root'%(self.VarName, nuis))
            if result.limit.GetEntries() != 3: raise RuntimeError("Different number of entries in the limits tree than expected, something unplanned happened")
            rslt = []
            for limit in result.limit:
                if limit.r < 0.1 or limit.r > 1.9:
                    warn('Nominal fit has probably not converged. Limit: ' + str(limit.r), UserWarning)
                    #raise RuntimeError('Nominal fit has probably not converged. Limit: ' + str(limit.r))
                rslt.append(limit.r) # first is nominal, second and third are up and down variations :)
            return rslt

    def doAllCombFits(self):
        self.results = {}
        for key in self.pmap:
            rslt = self.doCombFit(key)
            if key == 'obs': continue
            if key=='':
                # we have also the uncertainties here
                self.results[key] = [rslt[ind]*self.pmap[key]['tW'].Integral() for ind in [0,1,2]]
            else:
                self.results[key] = rslt*self.pmap[key]['tW'].Integral()

        for key in self.pmap:
            if key == 'obs': continue
            if self.results[key]==None or key == '': continue
            print key, self.results[key], 100*(self.results[key]-self.results[''][0])/self.results[''][0]



if __name__=='__main__':

    a = FittingSuite("~/TW_differential/AnalysisPAF/plotter/TW/inputs/forCards_Test_1.root",True)
    #a.doCombFit('')
    a.doAllCombFits()
    #a.doAllFits()
    #a.doFit('')
    #a.doFit('JESUp')
    #a.doFit('JESDown')
    #print 'here'

