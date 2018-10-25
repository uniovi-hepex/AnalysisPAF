'''
Piece of art for doing the unfolding analysis - with a bit of tW flavor.

'''

import copy
import ROOT as r
import beautifulUnfoldingPlots as bp
import errorPropagator as ep
import varList as vl
import os,sys

class DataContainer:
    ''' Class to store all the needed inputs: response matrices 
    and varied input distributions'''

    def __init__(self, var, fileName, fileNameReponse):
        self.var             = var
        self.fileName        = fileName
        self.fileNameReponse = fileNameReponse
        self.theoSysts    = []  # theo uncertainties (variations in response, input vector and covariance matrix)
        self.systListResp = []  # all of them (everything has variations in response but ttbar only uncert.)
                                #
                                # + experimental uncertainties -> are propagated in the nominal helper
                                # + theo uncertainties -> have their own helper (as they affect input vector)

        self.responseMatrices = {}
        self.unfoldingInputs  = {}
        self.bkgs             = {}
        self.covarInput       = {}
        self.readAndStore()

    def readAndStore(self):
        # Getting inputs and covariance matrices
        if not os.path.isfile(self.fileName):
            raise RuntimeError('The rootfile with the post fit information of variable {var} does not exist.'.format(var = varName))
        tfile = r.TFile.Open(self.fileName)
        for key in tfile.GetListOfKeys():
            if   'forPlotting' in key.GetName(): continue
            elif 'hFitResult_' in key.GetName():
                if 'hFitResult_' + self.var + '_' not in key.GetName(): continue
                else:
                    sysName = key.GetName().replace('hFitResult_'+self.var+'_','')
                    self.unfoldingInputs[sysName] = copy.deepcopy(key.ReadObj())
                    if sysName is not '':
                        self.theoSysts.append(sysName)

            elif 'hCovar_' in key.GetName():
                if 'hCovar_' + self.var + '_' not in key.GetName():
                    continue
                else:
                    sysName = key.GetName().replace('hCovar_'+self.var+'_','')
                    self.covarInput[sysName] = copy.deepcopy(key.ReadObj())

        if '' not in self.unfoldingInputs:
            print self.unfoldingInputs
            raise RuntimeError("Unfolding inputs for nominal sample is missing")
        if '' not in self.covarInput:
            raise RuntimeError("Covariance input for nominal sample is missing")
        tfile.Close()

        # Getting uncertainties in the response matrix
        tfile = r.TFile.Open(self.fileNameReponse)
        self.systListResp = [] 
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'R': continue
            if vl.varList[self.var]['var_response'] not in key.GetName(): continue
            if key.GetName() == 'R' + vl.varList[self.var]['var_response']:
                self.responseMatrices[''] = copy.deepcopy(key.ReadObj())
            elif 'R'+vl.varList[self.var]['var_response']+'_' in key.GetName():
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.systListResp.append(sysName)
                self.responseMatrices[sysName] = copy.deepcopy(key.ReadObj())

        # Getting background (events not passing the fiducial selection, but passing reco)
        scale = vl.Lumi * 1000
        if vl.doxsec: scale = 1
        
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'F': continue
            if vl.varList[self.var]['var_response'] not in key.GetName(): continue
            if key.GetName() == 'F' + vl.varList[self.var]['var_response']:
                self.bkgs[''] = copy.deepcopy(key.ReadObj())
                self.bkgs[''].Scale(scale)
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.bkgs[sysName] = copy.deepcopy(key.ReadObj())
                self.bkgs[sysName].Scale(scale)

        tfile.Close()

        if '' not in self.responseMatrices:
            raise RuntimeError("Nominal response matrix not there")


    def getInputs(self,nuis):
        # ttbar only systs dont have a varied response matrix
        if verbose: print '> Getting inputs for nuisance', nuis, vl.varList['Names']['ttbarSysts']+vl.varList['Names']['colorSysts']
        if nuis in vl.varList['Names']['ttbarSysts']+vl.varList['Names']['colorSysts']:
            return self.unfoldingInputs[nuis], self.responseMatrices[''], self.bkgs[''], self.covarInput[nuis]
        else:
            if nuis not in self.responseMatrices:
                raise RuntimeError("%s is not in the list of response"%nuis)
            return self.unfoldingInputs[nuis], self.responseMatrices[nuis], self.bkgs[nuis], self.covarInput[nuis]

    def getResponse(self, nuis):
        if nuis not in self.systListResp:
            RuntimeError("%s is not in the list of response"%nuis)
        return self.responseMatrices[nuis]



class UnfolderHelper:
    ''' Performs unfolding for one specific nuisance'''
    def __init__(self, var, nuis):
        self.var        = var
        self.nuis       = nuis
        self.plotspath  = ''
        self.Data       = None
    
    
    def makeUnfolderCore(self,unfInputNresponse):
        self.unfInput , self.response, self.bkg, self.Vyy = unfInputNresponse
        self.tunfolder = r.TUnfoldDensity(self.response, r.TUnfold.kHistMapOutputHoriz,
                                             r.TUnfold.kRegModeCurvature, r.TUnfold.kEConstraintArea,
                                             r.TUnfoldDensity.kDensityModeNone)

        self.tunfolder.SetInput( self.unfInput, 0., 0., self.Vyy )
        self.tunfolder.SubtractBackground( self.bkg , 'Non fiducial events')
    
    
    def doLCurveScan(self):
        if self.nuis != '':
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX    = r.TSpline3()
        self.logTauY    = r.TSpline3()
        self.logTauCurv = r.TSpline3()
        self.lCurve     = r.TGraph(0)

        #self.themax = self.tunfolder.ScanLcurve(10000, 1e-10, 1e-4, self.lCurve, self.logTauX, self.logTauY, self.logTauCurv)

        self.themax = self.tunfolder.ScanLcurve(5000, 1e-20, 1e-1, self.lCurve, self.logTauX,  self.logTauY, self.logTauCurv)

    def doTauScan(self):
        if self.nuis != '':
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX=r.TSpline3()
        self.logTauY=r.TSpline3()
        self.scanRes=r.TSpline3()
        self.lCurve =r.TGraph(0)
        
        # kEScanTauRhoAvg          average global correlation coefficient (from TUnfold::GetRhoI())
        # kEScanTauRhoMax          maximum global correlation coefficient (from TUnfold::GetRhoI())
        # kEScanTauRhoAvgSys       average global correlation coefficient (from TUnfoldSys::GetRhoItotal())
        # kEScanTauRhoMaxSys       maximum global correlation coefficient (from TUnfoldSys::GetRhoItotal())
        # kEScanTauRhoSquareAvg    average global correlation coefficient squared (from TUnfold::GetRhoI())
        # kEScanTauRhoSquareAvgSys average global correlation coefficient squared (from TUnfoldSys::GetRhoItotal())
        r.gInterpreter.LoadMacro('myScanner.C+')
        self.themax = r.myScanner(self.tunfolder, 1000, 1e-9, 1e-5, self.scanRes, r.TUnfoldDensity.kEScanTauRhoAvg,
                                  self.logTauX, self.logTauY)


    def doScanPlots(self):
        # Plot the scan variable and the maximum
        t=r.Double(0)
        x=r.Double(0)
        y=r.Double(0)
        if not hasattr(self, 'logTauX'): self.doLCurveScan()
        
        # First: L-curve plot
        plot = bp.beautifulUnfoldingPlots('{var}_LCurve'.format(var = self.var))
        plot.isLCurve = True
        plot.plotspath  = self.plotspath
        if not hasattr(self,'scanRes'):
            self.logTauX.GetKnot( self.themax, t, x)
            self.logTauY.GetKnot( self.themax, t, y)

            self.lCurve.SetLineWidth(2)
            self.lCurve.SetLineColor(r.kBlue)
            r.TGaxis.SetMaxDigits(3)
            self.lCurve.GetXaxis().SetNoExponent(True)
            self.lCurve.GetYaxis().SetNoExponent(True)
            self.lCurve.GetXaxis().SetTitle('log(#chi^{2}_{R})')
            self.lCurve.GetYaxis().SetTitle('log(#chi^{2}_{reg.})')
            plot.addHisto(self.lCurve,'ALnomin', '', 0)

        else:
            plot.addHisto(self.scanRes,'ALnomin','L curve',0)
        
        plot.addTLatex(0.75, 0.9, "#tau = {taupar}".format(taupar = round(self.tunfolder.GetTau() ,5)))
        plot.saveCanvas('TR', leg = False)
        del plot
        
        # Second: L-curve curvature plot
        plot = bp.beautifulUnfoldingPlots('{var}_LogTauCurv'.format(var = self.var))
        plot.plotspath  = self.plotspath
        plot.initCanvasAndAll()
        #plot.addHisto(self.lCurve, 'AL','',0)
        plot.canvas.cd()
        self.logTauCurv.Draw("AL")
        plot.saveCanvas('TR')
        del plot


    def getConditionNumber(self, nuis = ''):
        # prob = self.response.Clone( self.response.GetName() + '_probMatrix' )
        
        if nuis == '':  prob = copy.deepcopy( self.response )
        else:           prob = copy.deepcopy( self.Data.responseMatrices[nuis] )
        
        prob.SetName(self.response.GetName() + '_' + nuis + '_probMatrix' )
        self.tunfolder.GetProbabilityMatrix(self.response.GetName() + '_' + nuis + '_probMatrix', '', r.TUnfold.kHistMapOutputHoriz)
        matrx = r.TMatrixD( prob.GetYaxis().GetNbins(), prob.GetXaxis().GetNbins()) # rows are y, x are columns
        for i in range(prob.GetXaxis().GetNbins()):
            for j in range(prob.GetYaxis().GetNbins()):
                matrx[j][i] = prob.GetBinContent( prob.GetBin(i+1,j+1) )
                
        if verbose: matrx.Print()
        decomp = r.TDecompSVD(matrx)
        condn  = decomp.Condition()
        del matrx
        del decomp
        del prob
        if verbose: print '> Matrix condition is', condn
        return condn


class Unfolder():
    def __init__(self, var, fileName, fileNameReponse):
        self.var            = var
        self.doSanityCheck  = True
        self.Data           = DataContainer(var,fileName, fileNameReponse)
        self.systListResp   = self.Data.systListResp
        self.theoSysts      = self.Data.theoSysts
        self.allSysts       = list( set().union( self.systListResp, self.theoSysts) )
        self.helpers        = { nuis : UnfolderHelper(self.var, nuis) for nuis in self.theoSysts }
        self.helpers['']    = UnfolderHelper(self.var, '')
        self.helpers[''].Data = self.Data
        self.plotspath      = ""
        self.doColorEnvelope= False
        self.doRegularisation= False
    
    def prepareAllHelpers(self):
        # maybe protect these boys so they dont get initialized several times
        self.prepareNominalHelper()

        for nuis in self.theoSysts:
            if nuis == '': continue
            self.helpers[nuis].makeUnfolderCore(self.Data.getInputs(nuis))
            # no uncertainties are propagated here, only difference wrt nominal is needed

    def prepareNominalHelper(self):
        # maybe protect these boys so they dont get initialized several times
        self.helpers[''].makeUnfolderCore(self.Data.getInputs(''))

        for nuis in self.systListResp:
            if nuis in self.theoSysts: continue # those are handled in a different helper
            self.helpers[''].tunfolder.AddSysError( self.Data.getResponse(nuis), nuis, r.TUnfold.kHistMapOutputHoriz, r.TUnfoldSys.kSysErrModeMatrix )

    def doLCurveScan(self):
        self.helpers[''].doLCurveScan()

    def doTauScan(self):
        self.helpers[''].doTauScan()

    def doScanPlots(self):
        self.helpers[''].plotspath = self.plotspath
        self.helpers[''].doScanPlots()

    def doClosure(self,tau=None):
        if tau:
            self.helpers[''].DoUnfold(tau)
        
    def doNominalPlot(self, tau=None):
        if tau:
            self.helpers[''].tunfolder.DoUnfold(tau)
        data = self.helpers[''].tunfolder.GetOutput('forPlot')

        print 'Unfolded distribution integral', data.Integral()
        plot = bp.beautifulUnfoldingPlots(self.var)
        data.SetMarkerStyle(r.kFullCircle)
        data.GetXaxis().SetNdivisions(505,True)
        plot.plotspath  = self.plotspath
        
        if self.doSanityCheck:
            if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist')
            tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW'))
            tru.SetLineWidth(2)
            tru.SetLineColor(bp.colorMap[0])
            if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist')
            tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var))
            aMCatNLO = copy.deepcopy(tmptfile2.Get('tW'))
            aMCatNLO.SetLineWidth(2)
            aMCatNLO.SetLineColor(bp.colorMap[1])
            for bin in range(1, tru.GetNbinsX()):
                tru.SetBinError(bin, 0.)
                aMCatNLO.SetBinError(bin, 0.)
            plot.addHisto(tru, 'L', 'tW Powheg', 'L')
            plot.addHisto(aMCatNLO, 'L,same', 'tW aMCatNLO', 'L')
            plot.addHisto(data, 'P,E,same', vl.labellegend, 'P')
            plot.saveCanvas('TR')
            tmptfile.Close()
            tmptfile2.Close()
            
        else:
            plot.addHisto(data,'P,E', vl.labellegend,'P')
            plot.saveCanvas('TR')


    def doRegularizationComparison(self):
        self.prepareNominalHelper()
        self.doLCurveScan()
        regularized = self.helpers[''].tunfolder.GetOutput('forPlot_regu')
        self.helpers[''].tunfolder.DoUnfold(0.)
        unregularized = self.helpers[''].tunfolder.GetOutput('forPlot_nuregu')
        
        regularized  .SetLineWidth(2)
        unregularized.SetLineWidth(2)
        regularized  .SetLineColor( bp.colorMap[0])
        unregularized.SetLineColor( bp.colorMap[1])
        
        for bin in range(1, regularized.GetNbinsX()+1):
            regularized.SetBinContent(bin, regularized.GetBinContent(bin)/unregularized.GetBinContent(bin))
        
        plot = bp.beautifulUnfoldingPlots(self.var + '_regcomp')
#        regularized.Draw()
        regularized.GetXaxis().SetTitle(vl.varList[self.var]['xaxis'])
        regularized.GetYaxis().SetTitle('Reg./Unreg.')
        #regularized.GetYaxis().SetRangeUser(0, 1.1*regularized.GetMaximum())
        plot.addHisto(regularized  ,'hist'     ,'Regularized'  ,'L')
#        plot.addHisto(unregularized,'hist,same','UnRegularized','L')
        plot.plotspath = 'results/'
        plot.saveCanvas('BR', '', False)


    def doUnfoldingForAllNuis(self):
        allHistos   = {}
        tauval      = 0.
        self.prepareAllHelpers()
        if self.doRegularisation:
            print '> Performing regularisation...'
            self.doLCurveScan()
            tauval  = self.helpers[''].tunfolder.GetTau()

        if verbose: print '> Unfolding nominal distribution'
        self.helpers[''].tunfolder.DoUnfold(tauval)
        nominal     = copy.deepcopy(self.helpers[''].tunfolder.GetOutput(self.var))
        
        for nuis in self.theoSysts:
            if verbose: print '> Unfolding distribution of {sys} systematic'.format(sys = nuis)
            self.helpers[nuis].tunfolder.DoUnfold( tauval )
            allHistos[nuis] = self.helpers[nuis].tunfolder.GetOutput(self.var + '_' + nuis)
        
        nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, self.doColorEnvelope)
        plot = bp.beautifulUnfoldingPlots(self.var)
        plot.doRatio    = True
        plot.plotspath  = self.plotspath
        
        nominal.SetMarkerStyle(r.kFullCircle)
        nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
        nominal_withErrors[0].SetLineColor(r.kBlue)
        nominal_withErrors[0].SetFillStyle(1001)

        if self.doSanityCheck:
            if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist')
            tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW'))
            tru.SetLineWidth(2)
            tru.SetLineColor(bp.colorMap[0])
            if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist')
            tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var))
            aMCatNLO = copy.deepcopy(tmptfile2.Get('tW'))
            aMCatNLO.SetLineWidth(2)
            aMCatNLO.SetLineColor(r.kAzure)
            
            plot.addHisto(nominal_withErrors, 'hist',   'Total unc.',   'F', 'unc')
            plot.addHisto(tru,                'L,same', 'tW Powheg',    'L', 'mc')
            plot.addHisto(aMCatNLO,           'L,same', 'tW aMCatNLO',  'L', 'mc')
            plot.addHisto(nominal,            'P,E,same', vl.labellegend, 'P', 'data')
            plot.saveCanvas('TR')
            tmptfile.Close()
            tmptfile2.Close()
        else:
            plot.addHisto(nominal_withErrors,'E2','Total unc.','F')
            plot.addHisto(nominal,'P,same', vl.labellegend,'P', 'data')
            plot.saveCanvas('TR')

        del plot
        plot2 = bp.beautifulUnfoldingPlots(self.var + 'uncertainties')
        uncList = ep.getUncList(nominal, allHistos, self.doColorEnvelope)[:5]
        
        incmax  = []
        for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
            if nominal_withErrors[1].GetBinError(bin) > nominal_withErrors[0].GetBinContent(bin):
                incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[0].GetBinContent(bin)]))
            else:
                incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[1].GetBinError(bin)]))

        maxinctot = 0
        hincmax   = nominal_withErrors[0].Clone('hincmax')
        for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
            hincmax.SetBinContent(bin, incmax[bin-1] / hincmax.GetBinContent(bin))
            hincmax.SetBinError(bin, 0)
            if (hincmax.GetBinContent(bin) > maxinctot): maxinctot = hincmax.GetBinContent(bin)

        hincmax.SetLineColor(r.kBlack)
        hincmax.SetLineWidth( 2 )
        hincmax.SetFillColorAlpha(r.kBlue, 0)

        if (maxinctot >= 0.9):
            if maxinctot >= 5:
                uncList[0][1].GetYaxis().SetRangeUser(0, 5)
            else:
                uncList[0][1].GetYaxis().SetRangeUser(0, maxinctot + 0.1)
            
        else:
            uncList[0][1].GetYaxis().SetRangeUser(0, 0.9)

        for i in range(5):
            uncList[i][1].SetLineColor( vl.colorMap[uncList[i][0]] )
            uncList[i][1].SetLineWidth( 2 )
            plot2.addHisto(uncList[i][1], 'H,same' if i else 'H',uncList[i][0],'L')
        
        plot2.addHisto(hincmax, 'H,same', 'Total', 'L')
        plot2.plotspath = self.plotspath
        plot2.saveCanvas('TR')
    

    def getConditionNumber(self, nuis):
        if nuis in vl.varList['Names']['ExpSysts']: return self.helpers[''].getConditionNumber(nuis)
        else: return self.helpers[nuis].getConditionNumber()



if __name__=="__main__":
    vl.SetUpWarnings()
    r.gROOT.SetBatch(True)
    verbose = False
    print "===== Unfolding procedure\n"
    if (len(sys.argv) > 1):
        varName = sys.argv[1]
        print "> Variable to be unfolded:", varName, "\n"
        if (len(sys.argv) > 2):
            pathtothings    = sys.argv[2]
            print "> Special path to things chose:", pathtothings, "\n"
        else:
            pathtothings    = 'temp/{var}_/'.format(var = varName)
        
    else:
        print "> Default variable and path chosen\n"
        varName       = 'LeadingLepEta'
        pathtothings  = 'temp/{var}_/'.format(var = varName)

    print "\n> Beginning unfolding...\n"
    a = Unfolder(varName, pathtothings + 'fitOutput_' + varName + '.root', 'temp/UnfoldingInfo.root')
    a.plotspath       = "results/"
    a.doSanityCheck   = True
    a.doColorEnvelope = True
    a.doRegularisation= vl.doReg
    
    a.doUnfoldingForAllNuis()       # Unfolding
    a.doScanPlots()                 # L-Curve and curvature plots
    a.doRegularizationComparison()  # Comparison plot between regularisation and not
    
                                    # Condition numbers text file
    if not os.path.isdir('results/CondN'):
        os.system('mkdir -p results/CondN')
    fcn = open('results/CondN/{var}'.format(var = varName) + '.txt', 'w')
    out = 'Condition number   Systematic     \n'
    for syst in a.allSysts + ['']:
        if syst in vl.varList['Names']['ttbarSysts'] or syst in vl.varList['Names']['colorSysts']: continue
        out += str(round(a.getConditionNumber(syst), 4)) +  '          ' + syst + '\n'
    fcn.write(out)
    fcn.close
    print "> Unfolding done!"
    print "> Printing yields:"
    ndata = 0
    nmc   = 0
    for i in range(1, len(vl.varList[varName]['recobinning'])):
        tmpdatac = open('temp/{var}_/datacard_{var}_{bin}.txt'.format(var = varName, bin = i), 'r')
        for lin in tmpdatac:
            if 'observation' in lin: ndata += int(lin.split(' ')[1])
            if 'rate'        in lin: nmc   += sum([float(j) for j in lin.split(' ')[1:-1]])
        tmpdatac.close()
    print "\n    - Number of observed data events in the fiducial region:", ndata
    print "\n    - Number of simulated events in the fiducial region:", nmc
