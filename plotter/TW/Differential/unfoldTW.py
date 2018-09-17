''' 
Piece of art for doing the unfolding analysis - with a bit of tW flavor.

''' 

import copy, os
import ROOT as r
import beautifulUnfoldingPlots
import errorPropagator
import varList

class DataContainer:
    ''' Class to store all the needed inputs: response matrices 
    and varied input distributions'''

    def __init__(self, var, fileName, fileNameReponse):
        self.var                = var
        self.varresp            = varList.varList[var]['var_response']
        self.fileName           = fileName
        self.fileNameReponse    = fileNameReponse
        self.listOfSysts        = []
        self.responseMatrices   = {}
        self.unfoldingInputs    = {}
        self.bkgs               = {}
        self.readAndStore()

    def readAndStore(self):
        # Getting uncertainties in the input
        tfile = r.TFile.Open(self.fileName)
        for key in tfile.GetListOfKeys():
            if key.GetName() == 'tW':
                self.unfoldingInputs[''] = copy.deepcopy(key.ReadObj())
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                if (sysName in varList.varList['Names']['specialSysts'] ): continue
                self.listOfSysts.append(sysName)
                self.unfoldingInputs[sysName] = copy.deepcopy(key.ReadObj())
        if '' not in self.unfoldingInputs:
            raise RuntimeError("Unfolding inputs for nominal sample is missing")
        tfile.Close()

        # Getting uncertainties in the response matrix
        tfile = r.TFile.Open(self.fileNameReponse)
        systListResp = [] # just for sanity checks
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'R': continue
            if self.varresp != key.GetName().split('_')[0][1:]: continue
            if key.GetName() == 'R' + self.varresp:
                self.responseMatrices[''] = copy.deepcopy(key.ReadObj())
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                systListResp.append(sysName)
                self.responseMatrices[sysName] = copy.deepcopy(key.ReadObj())

        # Getting background (events not passing the fiducial selection, but passing reco)
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'F': continue
            if self.varresp != key.GetName().split('_')[0][1:]: continue
            if key.GetName() == 'F' + self.varresp:
                self.bkgs[''] = copy.deepcopy(key.ReadObj())
                self.bkgs[''].Scale(varList.Lumi*1000)
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.bkgs[sysName] = copy.deepcopy(key.ReadObj())
                self.bkgs[sysName].Scale(varList.Lumi*1000)
        
        tfile.Close()

        if '' not in self.responseMatrices:
            raise RuntimeError("Nominal response matrix not there")

        pleaseraise = False
        if not all(sys in systListResp for sys in self.listOfSysts):
            for sys in self.listOfSysts:
                if sys not in systListResp:
                    if sys not in varList.varList['Names']['ttbarSysts'] + varList.varList['Names']['specialSysts'] + varList.varList['Names']['colorSysts']:
                        print '>', sys, 'is not in the response matrix file!'
                        pleaseraise = True
            if pleaseraise: 
                raise RuntimeError("We dont have reponse matrices for all nuisances")
            else:
                print '\n> TTbar systematic uncertanties considered.\n'
        
        if ((systListResp != self.listOfSysts) and not pleaseraise):
            print RuntimeWarning("Different nuisances in response and inputs")
            #raise RuntimeWarning("Different nuisances in response and inputs")
        pleaseraise = None
        
        
    def getMatrixNInput(self,nuis):
        #if nuis not in self.listOfSysts + [''] + varList.varList['Names']['ttbarSysts']:
        if nuis not in self.listOfSysts + ['']:
            raise RuntimeError("%s is not in the list of response"%nuis)
        if nuis in varList.varList['Names']['ttbarSysts'] + varList.varList['Names']['colorSysts']:
            return self.unfoldingInputs[nuis], self.responseMatrices[''], self.bkgs['']
        else:
            return self.unfoldingInputs[nuis], self.responseMatrices[nuis], self.bkgs[nuis]



class UnfolderHelper:
    ''' Performs unfolding for one specific nuisance'''
    def __init__(self, var, nuis):
        self.var  = var
        self.nuis = nuis

    def makeUnfolderCore(self,unfInputNresponse):
        self.unfInput , self.response, self.bkg = unfInputNresponse
        self.tunfolder = r.TUnfoldDensity(self.response, r.TUnfold.kHistMapOutputHoriz,
                                             r.TUnfold.kRegModeCurvature, r.TUnfold.kEConstraintArea,
                                             r.TUnfoldDensity.kDensityModeeNone)
        self.tunfolder.SetInput( self.unfInput )
        self.tunfolder.SubtractBackground( self.bkg , 'Non fiducial events')

    def doLCurveScan(self):
        if self.nuis != '': 
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX=r.TSpline3() 
        self.logTauY=r.TSpline3() 
        #self.logTauCurv= r.TSpline3() 
        self.lCurve=r.TGraph(0) 

        self.themax = self.tunfolder.ScanLcurve(1000, 1e-9, 1e-5, self.lCurve, self.logTauX,
                                                self.logTauY)

        
    def doTauScan(self):
        if self.nuis != '': 
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX=r.TSpline3() 
        self.logTauY=r.TSpline3() 
        self.scanRes=r.TSpline3() 
        self.lCurve=r.TGraph(0) 

        
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
        
        plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots('LCurve')
        if not hasattr(self,'scanRes'):
            self.logTauX.GetKnot( self.themax, t, x)
            self.logTauY.GetKnot( self.themax, t, y)

            self.lCurve.SetLineWidth(2)
            self.lCurve.SetLineColor(r.kBlue)
            
            #self.lCurve.GetYaxis().SetDecimals(True)
            self.lCurve.GetYaxis().SetNdivisions(304,False)
            self.lCurve.GetXaxis().SetNdivisions(303,False)
            plot.addHisto(self.lCurve,'AL','',0)

        else:
            plot.addHisto(self.scanRes,'AL','',0)

        plot.saveCanvas('TR','_%s'%self.var)


    def getConditionNumber(self):
        # prob = self.response.Clone( self.response.GetName() + '_probMatrix' )
        prob = copy.deepcopy( self.response )
        prob.SetName(self.response.GetName() + '_probMatrix' )
        self.tunfolder.GetProbabilityMatrix(self.response.GetName() + '_probMatrix', '',r.TUnfold.kHistMapOutputHoriz)
        matrx = r.TMatrixD( prob.GetYaxis().GetNbins(), prob.GetXaxis().GetNbins()) # rows are y, x are columns
        for i in range(prob.GetXaxis().GetNbins()):
            for j in range(prob.GetYaxis().GetNbins()):
                matrx[j][i] = prob.GetBinContent( prob.GetBin(i+1,j+1) )
                
        matrx.Print()
        
        decomp = r.TDecompSVD(matrx)

        print 'Matrix condition is', decomp.Condition()
        

class Unfolder():
    def __init__(self, var, fileName, fileNameReponse):
        self.var  = var
        self.doSanityCheck   = True
        self.Data = DataContainer(var,fileName, fileNameReponse)
        self.sysList = self.Data.listOfSysts
        self.helpers = { nuis : UnfolderHelper(self.var, nuis) for nuis in self.sysList }
        self.helpers[''] = UnfolderHelper(self.var, '')
        self.plotspath  = ""
        self.doColorEnvelope = False
        
        print self.sysList

    def prepareAllHelpers(self):
        # maybe protect these boys so they dont get initialized several times
        for nuis in self.sysList:
            self.helpers[nuis].makeUnfolderCore(self.Data.getMatrixNInput(nuis))

    def prepareNominalHelper(self):
        # maybe protect these boys so they dont get initialized several times
        self.helpers[''].makeUnfolderCore(self.Data.getMatrixNInput(''))

    def doLCurveScan(self):
        self.helpers[''].doLCurveScan()

    def doTauScan(self):
        self.helpers[''].doTauScan()

    def doScanPlots(self):
        self.helpers[''].doScanPlots()

    def doClosure(self,tau=None):
        if tau:
            self.helpers[''].DoUnfold(tau)
        
    def doNominalPlot(self, tau=None):
        if tau:
            self.helpers[''].tunfolder.DoUnfold(tau)
        data = self.helpers[''].tunfolder.GetOutput('forPlot')

        print 'Unfolded distribution integral', data.Integral()
        plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots(self.var)
        data.SetMarkerStyle(r.kFullCircle)
        data.GetXaxis().SetNdivisions(505,True)
        plot.plotspath  = self.plotspath
        
        if self.doSanityCheck:
            #tmptfile = r.TFile.Open('/nfs/fanae/user/sscruz/TW_differential/AnalysisPAF/plotter/./Datacards/closuretest_TGenLeadingJet_TGen{var}.root'.format(var=self.var))
            if not os.path.isfile('temp/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist')
            tmptfile = r.TFile.Open('temp/ClosureTest_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW'))
            tru.SetLineWidth(2)
            tru.SetLineColor(beautifulUnfoldingPlots.colorMap[0])
            plot.addHisto(tru, 'L', 'Truth', 'L')
            plot.addHisto(data, 'P,E,same', 'Pseudodata', 'P')
            plot.saveCanvas('TR')
            tmptfile.Close()
        else:
            plot.addHisto(data,'P,E','Data','P')
            plot.saveCanvas('TR')


    def doRegularizationComparison(self):
        self.prepareNominalHelper()
        self.doLCurveScan()
        regularized = self.helpers[''].tunfolder.GetOutput('forPlot_regu')
        self.helpers[''].tunfolder.DoUnfold(0.)
        unregularized = self.helpers[''].tunfolder.GetOutput('forPlot_nuregu')
        
        regularized  .SetLineWidth(2)
        unregularized.SetLineWidth(2)
        regularized  .SetLineColor( beautifulUnfoldingPlots.colorMap[0])
        unregularized.SetLineColor( beautifulUnfoldingPlots.colorMap[1])

        print regularized  .GetBinContent(1), unregularized.GetBinContent(1)

        plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots(self.var)
        regularized.Draw()
        regularized.GetYaxis().SetRangeUser(0, 1.1*regularized.GetMaximum())
        plot.addHisto(regularized  ,'hist'     ,'Regularized'  ,'L')
        plot.addHisto(unregularized,'hist,same','UnRegularized','L')
        plot.saveCanvas('TR','comparison')


    def doUnfoldingForAllNuis(self, asym = False):
        self.prepareAllHelpers()
        allHistos = {}
        nominal   = copy.deepcopy(self.helpers[''].tunfolder.GetOutput(self.var))
        for nuis in self.sysList:
            self.helpers[nuis].tunfolder.DoUnfold( self.helpers[''].tunfolder.GetTau() )
            allHistos[nuis] = self.helpers[nuis].tunfolder.GetOutput(self.var + '_' + nuis)
        
        if asym:
            nominal_withErrors = errorPropagator.propagateHistoAsym(nominal, allHistos, self.doColorEnvelope)
        else:
            nominal_withErrors = errorPropagator.propagateHisto(nominal, allHistos, self.doColorEnvelope)

        plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots(self.var)
        plot.plotspath  = self.plotspath
        nominal.SetMarkerStyle(r.kFullCircle)
        nominal.GetXaxis().SetNdivisions(505, True)
        
        if asym:
            nominal_withErrors[0].SetFillColorAlpha(r.kBlue,0.35)
            nominal_withErrors[0].SetLineColor(r.kBlue)
            nominal_withErrors[0].SetFillStyle(1001)
        else:
            nominal_withErrors.SetFillColorAlpha(r.kBlue,0.35)
            nominal_withErrors.SetLineColor(r.kBlue)
            nominal_withErrors.SetFillStyle(1001)
        
        if self.doSanityCheck:
            #tmptfile = r.TFile.Open('/nfs/fanae/user/sscruz/TW_differential/AnalysisPAF/plotter/./Datacards/closuretest_TGenLeadingJet_TGen{var}.root'.format(var=self.var))
            if not os.path.isfile('temp/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist')
            tmptfile = r.TFile.Open('temp/ClosureTest_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW'))
            tru.SetLineWidth(2)
            tru.SetLineColor(beautifulUnfoldingPlots.colorMap[0])
            if asym:
                plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
            else:
                plot.addHisto(nominal_withErrors, 'E2', 'Syst. unc.', 'F')
            plot.addHisto(nominal, 'P,same', 'Pseudodata', 'P')
            plot.addHisto(tru, 'L,same', 'Truth', 'L')
            plot.saveCanvas('TR')
            tmptfile.Close()
        else:
            if asym:
                plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
            else:
                plot.addHisto(nominal_withErrors, 'E2', 'Syst. unc.', 'F')
            plot.addHisto(nominal, 'P,same', 'Pseudodata', 'P')
            plot.saveCanvas('TR')

        plot2 = beautifulUnfoldingPlots.beautifulUnfoldingPlots(self.var + 'uncertainties')
        uncList = errorPropagator.getUncList( nominal, allHistos, self.doColorEnvelope)[:5]
        uncList[0][1].Draw()

        if uncList[0][1].GetMaximum() < 0.5:

            uncList[0][1].GetYaxis().SetRangeUser(0,0.5)
        else:
            uncList[0][1].GetYaxis().SetRangeUser(0,0.75)
        for i in range(5):
            uncList[i][1].SetLineColor( beautifulUnfoldingPlots.colorMap[i] )
            uncList[i][1].SetLineWidth( 2 )
            plot2.addHisto(uncList[i][1], 'H,same' if i else 'H',uncList[i][0],'L')
        
        plot2.plotspath = self.plotspath
        plot2.saveCanvas('TR')
    

    def getConditionNumber(self, nuis):
        return self.helpers[nuis].getConditionNumber()
        

if __name__=="__main__":
    
    a = Unfolder('LeadingLepEta','temp/cardFile_LeadingLepEta.root','temp/UnfoldingInfo.root')
    #a = Unfolder('LeadingJetEta', 'temp/cardFile_LeadingJetEta.root', 'temp/UnfoldingInfo.root')
    a.doRegularizationComparison()

    #print k

    a.prepareNominalHelper()
    a.doLCurveScan()
    # # #a.doTauScan()
    a.doScanPlots()
    # a.doNominalPlot()
    # #a.doUnfoldingForAllNuis()

    #a.prepareNominalHelper()
    #a.getConditionNumber('')


    ## crap for testing 
    # histo = a.Data.responseMatrices[''].GetXaxis()
    # histo2D = r.TH2D('histo2D','',
    #                  histo.GetNbins(), histo.GetBinLowEdge(1), histo.GetBinLowEdge(histo.GetNbins())+histo.GetBinWidth(histo.GetNbins()),
    #                  histo.GetNbins(), histo.GetBinLowEdge(1), histo.GetBinLowEdge(histo.GetNbins())+histo.GetBinWidth(histo.GetNbins()))
    # print a.helpers[''].tunfolder.GetTau()
    # a.helpers[''].tunfolder.GetEmatrix(histo2D)
    # histo2D.Draw('colz text')
    # c = r.TCanvas()
    # x =copy.deepcopy( a.helpers[''].tunfolder.GetOutput('test'))
    # x.Draw()

    # c2 = r.TCanvas()
    # a.helpers[''].tunfolder.DoUnfold(0.)
    # x2 = copy.deepcopy(a.helpers[''].tunfolder.GetOutput('test2'))
    # x2.Draw()
    # raw_input('meh')
