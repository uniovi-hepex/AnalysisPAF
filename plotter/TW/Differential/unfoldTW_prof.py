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
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'F': continue
            if vl.varList[self.var]['var_response'] not in key.GetName(): continue
            if key.GetName() == 'F' + vl.varList[self.var]['var_response']:
                self.bkgs[''] = copy.deepcopy(key.ReadObj())
                self.bkgs[''].Scale(vl.Lumi*1000)
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.bkgs[sysName] = copy.deepcopy(key.ReadObj())
                self.bkgs[sysName].Scale(vl.Lumi*1000)

        
        tfile.Close()

        if '' not in self.responseMatrices:
            raise RuntimeError("Nominal response matrix not there")


                
        
    def getInputs(self,nuis):

        # ttbar only systs dont have a varied response matrix 
        print nuis, vl.varList['Names']['ttbarSysts']+vl.varList['Names']['colorSysts']
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
        self.var  = var
        self.nuis = nuis

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

        self.themax = self.tunfolder.ScanLcurve(10000, 1e-10, 1e-4, self.lCurve, self.logTauX,
                                                self.logTauY, self.logTauCurv)
        
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
        
        #plot = bp.beautifulUnfoldingPlots('LCurve')
        if not hasattr(self,'scanRes'):
            self.logTauX.GetKnot( self.themax, t, x)
            self.logTauY.GetKnot( self.themax, t, y)

            #plot.addHisto(self.lCurve,'AL','',0)


            self.lCurve.SetLineWidth(2)
            self.lCurve.SetLineColor(r.kBlue)
            
            #self.lCurve.GetYaxis().SetDecimals(True)
            self.lCurve.GetYaxis().SetNdivisions(304,False)
            self.lCurve.GetXaxis().SetNdivisions(303,False)

        else:
            plot.addHisto(self.scanRes,'AL','',0)

        c = r.TCanvas()
        self.lCurve.Draw('AL')
        raw_input('check')

        # for i in range( self.lCurve.GetN()) :
        #     x = r.Double(0); y = r.Double(0.)
        #     self.lCurve.GetPoint(i, x, y)
        #     print i, x, y 
        plot.saveCanvas('TR','_%s'%self.var)

        plot = bp.beautifulUnfoldingPlots('LogTauCurv')
        plot.addHisto(self.lCurve, 'AL','',0)
        plot.canvas.cd()
        self.logTauCurv.Draw("AL")
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
        self.systListResp = self.Data.systListResp
        self.theoSysts    = self.Data.theoSysts
        self.allSysts     = list( set().union( self.systListResp, self.theoSysts) )
        self.helpers = { nuis : UnfolderHelper(self.var, nuis) for nuis in self.theoSysts }
        self.helpers[''] = UnfolderHelper(self.var, '')
        self.plotspath  = ""
        self.doColorEnvelope = False
    
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
            plot.addHisto(tru, 'L', 'tW Powheg', 'L')
            plot.addHisto(aMCatNLO, 'L,same', 'tW aMCatNLO', 'L')
            plot.addHisto(data, 'P,E,same', 'Data', 'P')
            plot.saveCanvas('TR')
            tmptfile.Close()
            tmptfile2.Close()
            
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
        regularized  .SetLineColor( bp.colorMap[0])
        unregularized.SetLineColor( bp.colorMap[1])

        
        print regularized  .GetBinContent(1), unregularized.GetBinContent(1)

        plot = bp.beautifulUnfoldingPlots(self.var)
        regularized.Draw()
        regularized.GetYaxis().SetRangeUser(0, 1.1*regularized.GetMaximum())
        plot.addHisto(regularized  ,'hist'     ,'Regularized'  ,'L')
        plot.addHisto(unregularized,'hist,same','UnRegularized','L')
        plot.saveCanvas('TR','comparison')
        


    def doUnfoldingForAllNuis(self):
        self.prepareAllHelpers()
        self.doLCurveScan()
        allHistos   = {}
        nominal     =copy.deepcopy(self.helpers[''].tunfolder.GetOutput(self.var))
        for nuis in self.theoSysts:
            print nuis
            self.helpers[nuis].tunfolder.DoUnfold( self.helpers[''].tunfolder.GetTau() )
            allHistos[nuis] = self.helpers[nuis].tunfolder.GetOutput(self.var + '_' + nuis)
        nominal_withErrors = ep.propagateHistoAsym( nominal, allHistos, self.doColorEnvelope)

        plot = bp.beautifulUnfoldingPlots(self.var)
        plot.plotspath  = self.plotspath
        nominal.SetMarkerStyle(r.kFullCircle)
        nominal.GetXaxis().SetNdivisions(505,True)
        nominal_withErrors[0].SetFillColorAlpha(r.kBlue,0.35)
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
            plot.addHisto(nominal_withErrors,'E2','Syst. unc.','F')
            plot.addHisto(nominal,'P,same','Data','P')
            plot.addHisto(tru,'L,same','tW Powheg','L')
            plot.addHisto(aMCatNLO,'L,same','tW aMCatNLO','L')
            plot.saveCanvas('TR')
            tmptfile.Close()
            tmptfile2.Close()
        else:
            plot.addHisto(nominal_withErrors,'E2','Syst. unc.','F')
            plot.addHisto(nominal,'P,same','Data','P')
            plot.saveCanvas('TR')


        plot2 = bp.beautifulUnfoldingPlots(self.var + 'uncertainties')
        uncList = ep.getUncList(nominal, allHistos, self.doColorEnvelope)[:5]
        uncList[0][1].Draw()

        if uncList[0][1].GetMaximum() < 0.5:

            uncList[0][1].GetYaxis().SetRangeUser(0,0.5)
        else:
            uncList[0][1].GetYaxis().SetRangeUser(0,0.9)
        for i in range(5):
            uncList[i][1].SetLineColor( vl.colorMap[uncList[i][0]] )
            uncList[i][1].SetLineWidth( 2 )
            plot2.addHisto(uncList[i][1], 'H,same' if i else 'H',uncList[i][0],'L')
        
        plot2.plotspath = self.plotspath
        plot2.saveCanvas('TR')
    

    def getConditionNumber(self, nuis):
        return self.helpers[nuis].getConditionNumber()



if __name__=="__main__":
    r.gROOT.SetBatch(True)
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
    #a.prepareNominalHelper()
    #a.doLCurveScan()
    #a.doTauScan()
    #a.doScanPlots()
    #a.doNominalPlot()
    a.doUnfoldingForAllNuis()    # Symmetric uncertainties
    #a.doUnfoldingForAllNuis(True) # Asymmetric uncertainties

    print "> Unfolding done!"
