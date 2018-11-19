'''
Piece of art for doing the unfolding analysis - with a bit of tW flavor.

'''

import copy
import ROOT as r
import beautifulUnfoldingPlots as bp
import errorPropagator as ep
import varList as vl
import os,sys,math

class DataContainer:
    ''' Class to store all the needed inputs: response matrices and varied input distributions'''

    def __init__(self, var, fileName, fileNameReponse):
        self.var              = var
        self.fileName         = fileName
        self.fileNameReponse  = fileNameReponse
        self.systListResp     = []
        self.listOfSysts      = []
        self.responseMatrices = {}
        self.unfoldingInputs  = {}
        self.bkgs             = {}
        self.readAndStore()

    def readAndStore(self):
        # Getting inputs
        if not os.path.isfile(self.fileName):
            raise RuntimeError('The rootfile with the post signal extraction information of variable {var} does not exist.'.format(var = varName))
        tfile = r.TFile.Open(self.fileName)
        for key in tfile.GetListOfKeys():
            if 'data_' in key.GetName():
                sysName = key.GetName().replace('data_', '')
                self.unfoldingInputs[sysName] = copy.deepcopy(key.ReadObj())
                if sysName != '': self.listOfSysts.append(sysName)

        if '' not in self.unfoldingInputs:
            raise RuntimeError("Unfolding input for nominal sample is missing.")
        tfile.Close()
        
        # Getting uncertainties in the response matrix
        tfile = r.TFile.Open(self.fileNameReponse)
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'R': continue
            if vl.varList[self.var]['var_response'] not in key.GetName(): continue
            if key.GetName() == 'R' + vl.varList[self.var]['var_response']:
                self.responseMatrices[''] = copy.deepcopy(key.ReadObj())
            elif 'R' + vl.varList[self.var]['var_response'] + '_' in key.GetName():
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.systListResp.append(sysName)
                self.responseMatrices[sysName] = copy.deepcopy(key.ReadObj())

        # Getting background (events not passing the fiducial selection, but passing reco)
        scalevalone = vl.Lumi*1000
        for key in tfile.GetListOfKeys():
            if key.GetName()[0] != 'F': continue
            if vl.varList[self.var]['var_response'] not in key.GetName(): continue
            if key.GetName() == 'F' + vl.varList[self.var]['var_response']:
                self.bkgs[''] = copy.deepcopy(key.ReadObj())
                self.bkgs[''].Scale(scalevalone)
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                self.bkgs[sysName] = copy.deepcopy(key.ReadObj())
                self.bkgs[sysName].Scale(scalevalone)
                
        tfile.Close()

        if '' not in self.responseMatrices:
            raise RuntimeError("The nominal response matrix is not present in the provided rootfile.")
        
        if verbose: print "Printy thingy. listOfSysts:\n", self.listOfSysts, "\nsystListResp:\n", self.systListResp
        return
    

    def getInputs(self,nuis):
        # ttbar-related systs. are the only that don't have a varied response matrix
        if verbose: print '> Getting inputs for nuisance', nuis, vl.varList['Names']['ttbarSysts'] + vl.varList['Names']['colorSysts']
        if nuis in vl.varList['Names']['ttbarSysts'] + vl.varList['Names']['colorSysts'] + vl.varList['Names']['NormSysts'] + ['LumiUp','LumiDown', 'asimov']:
            return self.unfoldingInputs[nuis], self.responseMatrices[''], self.bkgs['']
        elif nuis == 'JERDown':
            return self.unfoldingInputs['JERUp'], self.responseMatrices['JERUp'], self.bkgs['JERUp']
        elif nuis == 'DSDown':
            return self.unfoldingInputs['DSUp'], self.responseMatrices['DSUp'], self.bkgs['DSUp']
        else:
            if nuis not in self.responseMatrices:
                raise RuntimeError("%s is not in the list of varied response matrices nor in the blacklist of systs. w/o associated response matrices."%nuis)
            return self.unfoldingInputs[nuis], self.responseMatrices[nuis], self.bkgs[nuis]

    def getResponse(self, nuis):
        if nuis not in self.systListResp:
            RuntimeError("%s is not in the list of varied response matrices nor in the blacklist of systs. w/o associated response matrices."%nuis)
        return self.responseMatrices[nuis]



class UnfolderHelper:
    ''' Performs unfolding for one specific nuisance'''
    def __init__(self, var, nuis):
        self.var        = var
        self.nuis       = nuis
        self.plotspath  = ''
        self.doArea     = False
        self.Init       = False
        self.tau        = 0
        self.wearedoingasimov = False
    
    
    def makeUnfolderCore(self, unfInputNresponse):
        self.unfInput, self.response, self.bkg = unfInputNresponse
        self.tunfolder = r.TUnfoldDensity(self.response, r.TUnfold.kHistMapOutputHoriz,
                                             r.TUnfold.kRegModeCurvature, r.TUnfold.kEConstraintArea if self.doArea else r.TUnfold.kEConstraintNone,
                                             r.TUnfoldDensity.kDensityModeNone)

        #for bin in range(1, self.unfInput.GetNbinsX() + 1):
            #self.unfInput.SetBinContent(bin, self.unfInput.GetBinContent(bin) - self.bkg.GetBinContent(bin))
        self.tunfolder.SetInput(self.unfInput)
        self.tunfolder.SubtractBackground(self.bkg, 'Non fiducial events')
    
    
    def doLCurveScan(self):
        if self.nuis != '':
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances.")

        self.logTauX    = r.TSpline3()
        self.logTauY    = r.TSpline3()
        self.logTauCurv = r.TSpline3()
        self.lCurve     = r.TGraph(0)

        #self.themax = self.tunfolder.ScanLcurve(10000, 1e-10, 1e-4, self.lCurve, self.logTauX, self.logTauY, self.logTauCurv)
        self.themax = self.tunfolder.ScanLcurve(5000, 1e-30, 1e-4, self.lCurve, self.logTauX,  self.logTauY, self.logTauCurv)
        
        self.tau = self.tunfolder.GetTau()
        return
    
    
    def doTauScan(self):
        if self.nuis != '':
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances.")

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
        self.themax = r.myScanner(self.tunfolder, 1000, 1e-9, 1e-3, self.scanRes, r.TUnfoldDensity.kEScanTauRhoAvg,
                                  self.logTauX, self.logTauY)
        
        self.tau = self.tunfolder.GetTau()
        return
    
    
    def doScanPlots(self):
        # Plot the scan variable and the maximum
        t=r.Double(0)
        x=r.Double(0)
        y=r.Double(0)
        if not hasattr(self, 'logTauX'):
            self.doLCurveScan()
        
        # First: L-curve plot
        plot = bp.beautifulUnfoldingPlots('{var}_asimov_LCurve'.format(var = self.var) if (self.wearedoingasimov) else '{var}_LCurve'.format(var = self.var))
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
        
        plot.addTLatex(0.75, 0.9, "#tau = {taupar}".format(taupar = round(self.tau, 15)))
        #plot.addTLatex(0.75, 0.9, "#tau = {taupar}".format(taupar = self.tau))
        plot.saveCanvas('TR', leg = False)
        del plot
        
        # Second: L-curve curvature plot
        plot = bp.beautifulUnfoldingPlots('{var}_asimov_LogTauCurv'.format(var = self.var) if (self.wearedoingasimov) else '{var}_LogTauCurv'.format(var = self.var))
        plot.plotspath  = self.plotspath
        plot.initCanvasAndAll()
        #plot.addHisto(self.lCurve, 'AL','',0)
        plot.canvas.cd()
        self.logTauCurv.Draw("AL")
        plot.saveCanvas('TR')
        del plot
    
    
    def getConditionNumber(self, nuis = ''):
        prob = copy.deepcopy( self.response.Clone(self.response.GetName() + '_' + nuis + '_probMatrix'))
        self.tunfolder.GetProbabilityMatrix(self.response.GetName() + '_' + nuis + '_probMatrix', '', r.TUnfold.kHistMapOutputHoriz)
        matrx = r.TMatrixD( prob.GetYaxis().GetNbins(), prob.GetXaxis().GetNbins()) # rows are y, x are columns
        for i in range(prob.GetXaxis().GetNbins()):
            for j in range(prob.GetYaxis().GetNbins()):
                matrx[j][i] = prob.GetBinContent( prob.GetBin(i+1,j+1) )
        
        if verbose: matrx.Print()
        decomp = r.TDecompSVD(matrx)
        condn  = decomp.Condition()
        del matrx, decomp, prob
        if verbose: print '> Matrix condition is', condn
        return condn



class Unfolder():
    def __init__(self, var, fileName, fileNameReponse):
        self.var              = var
        self.doSanityCheck    = True
        self.Data             = DataContainer(var, fileName, fileNameReponse)
        self.systListResp     = self.Data.systListResp
        self.sysList          = self.Data.listOfSysts
        self.helpers          = { nuis : UnfolderHelper(self.var, nuis) for nuis in self.sysList }
        self.helpers['']      = UnfolderHelper(self.var, '')
        self.wearedoingasimov = (not vl.asimov and not 'asimov' in self.sysList)
        self.helpers[''].wearedoingasimov = self.wearedoingasimov    # ONLY IMPLEMENTED FOR NOMINAL ONES!!!!!
        self.plotspath        = ""
        self.doColorEnvelope  = False
        self.doRegularisation = False
        self.taulist          = { nuis : 0 for nuis in self.sysList } # Different taus for all the response matrices not implemented.
        self.taulist['']      = 0
        self.doAreaConstraint = False


    def prepareAllHelpers(self, force = False):
        # maybe protect these boys so they dont get initialized several times
        self.prepareNominalHelper(force)

        for nuis in self.sysList:
            if nuis == '' or (not force and self.helpers[nuis].Init): continue
            self.helpers[nuis].doArea = self.doAreaConstraint
            self.helpers[nuis].makeUnfolderCore(self.Data.getInputs(nuis))
            self.helpers[nuis].Init   = True
        return


    def prepareNominalHelper(self, force = False):
        # maybe protect these boys so they dont get initialized several times
        if not force and self.helpers[''].Init: return
        self.helpers[''].doArea = self.doAreaConstraint
        self.helpers[''].makeUnfolderCore(self.Data.getInputs(''))
        self.helpers[''].Init   = True
        return


    def doLCurveScan(self):      # Different taus for all the response matrices not implemented.
        self.helpers[''].doLCurveScan()
        self.taulist[''] = self.helpers[''].tau
        return


    def doTauScan(self):
        self.helpers[''].doTauScan()
        self.taulist[''] = self.helpers[''].tau


    def doScanPlots(self):
        self.helpers[''].plotspath = self.plotspath
        self.helpers[''].doScanPlots()
        self.taulist[''] = self.helpers[''].tau


    def doClosure(self, tau = None):
        if tau:
            self.helpers[''].DoUnfold(tau)


    def doNominalPlot(self, tau = None):
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
        if self.taulist[''] == 0: self.doLCurveScan()             # Different taus for all the response matrices not implemented.
        self.helpers[''].tunfolder.DoUnfold(self.taulist[''])
        regularized = self.helpers[''].tunfolder.GetOutput('forPlot_regu')
        self.helpers[''].tunfolder.DoUnfold(0)
        unregularized = self.helpers[''].tunfolder.GetOutput('forPlot_nuregu')
        
        regularized  .SetLineWidth(2)
        unregularized.SetLineWidth(2)
        regularized  .SetLineColor( bp.colorMap[0])
        unregularized.SetLineColor( bp.colorMap[1])
        
        for bin in range(1, regularized.GetNbinsX()+1):
            regularized.SetBinContent(bin, regularized.GetBinContent(bin)/unregularized.GetBinContent(bin))
        
        plot = bp.beautifulUnfoldingPlots(self.var + '_asimov_regcomp' if (self.wearedoingasimov) else self.var + '_regcomp')
#        regularized.Draw()
        regularized.GetXaxis().SetTitle(vl.varList[self.var]['xaxis'])
        regularized.GetYaxis().SetTitle('Reg./Unreg.')
        #regularized.GetYaxis().SetRangeUser(0, 1.1*regularized.GetMaximum())
        plot.addHisto(regularized  ,'hist'     ,'regucomp'  ,'L')
#        plot.addHisto(unregularized,'hist,same','UnRegularized','L')
        plot.plotspath = 'results/'
        plot.saveCanvas('BR', '', False)


    def doAreaConstraintComparison(self):
        if self.doRegularisation and self.taulist[''] == 0: self.doLCurveScan()             # Different taus for all the response matrices not implemented.
        tauval = 0
        if self.doRegularisation: tauval = self.taulist['']             # Different taus for all the response matrices not implemented.
        self.prepareNominalHelper()
        self.helpers[''].tunfolder.SetConstraint(r.TUnfold.kEConstraintArea)
        self.helpers[''].tunfolder.DoUnfold(tauval)
        withareaconst    = self.helpers[''].tunfolder.GetOutput('forPlot_area')
        self.helpers[''].tunfolder.SetConstraint(r.TUnfold.kEConstraintNone)
        self.helpers[''].tunfolder.DoUnfold(tauval)
        withoutareaconst = self.helpers[''].tunfolder.GetOutput('forPlot_noarea')
        self.helpers[''].tunfolder.SetConstraint(r.TUnfold.kEConstraintArea)
        
        withareaconst   .SetLineWidth(2)
        withoutareaconst.SetLineWidth(2)
        withareaconst   .SetLineColor(bp.colorMap[0])
        withoutareaconst.SetLineColor(bp.colorMap[1])
        
        for bin in range(1, withareaconst.GetNbinsX()+1):
            if withoutareaconst.GetBinContent(bin) != 0: withareaconst.SetBinContent(bin, withareaconst.GetBinContent(bin)/withoutareaconst.GetBinContent(bin))
            else:                                        withoutareaconst.SetBinContent(bin, 0)
        
        plot = bp.beautifulUnfoldingPlots(self.var + '_asimov_areacomp' if (self.wearedoingasimov) else self.var + '_areacomp')
        withareaconst.GetXaxis().SetTitle(vl.varList[self.var]['xaxis'])
        withareaconst.GetYaxis().SetTitle('Area/No area')
        plot.addHisto(withareaconst  ,'hist'     ,'areacomp'  ,'L')
        plot.plotspath = 'results/'
        plot.saveCanvas('BR', '', False)


    def doUnfoldingForAllNuis(self):
        allHistos   = {}
        self.prepareAllHelpers()
        if self.doRegularisation and self.tau == 0:
            print '> Performing regularisation...'
            self.doLCurveScan()
        tauval = 0
        if self.doRegularisation: tauval = self.taulist['']

        if verbose: print '> Unfolding nominal distribution'
        self.helpers[''].tunfolder.DoUnfold(tauval)
        nominal     = copy.deepcopy(self.helpers[''].tunfolder.GetOutput(self.var))
        covnom = copy.deepcopy(self.helpers[''].tunfolder.GetEmatrixTotal("CovMat"))
        for bin in range(1, nominal.GetNbinsX() + 1):
            nominal.SetBinError(bin, math.sqrt(covnom.GetBinContent(bin, bin)))
        
        for nuis in self.sysList:
            if verbose: print '> Unfolding distribution of {sys} systematic'.format(sys = nuis)
            self.helpers[nuis].tunfolder.DoUnfold(tauval)
            allHistos[nuis] = self.helpers[nuis].tunfolder.GetOutput(self.var + '_' + nuis)
        
        scaleval = 1/vl.Lumi/1000 if vl.doxsec else 1
        nominal.Scale(scaleval)
        for key in allHistos:
            allHistos[key].Scale(scaleval)
        
        if not self.wearedoingasimov:
            if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist')
            tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var))
            #tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW'))
            #tru2 = copy.deepcopy(tmptfile2.Get('tW'))
            #tru2.Scale(vl.Lumi*1000)
            for bin in range(1, allHistos['asimov'].GetNbinsX() + 1):
                #print "\nasimov:", allHistos['asimov'].GetBinContent(bin)
                #print "verdad:", tru.GetBinContent(bin)
                allHistos['asimov'].SetBinError(bin,   abs(allHistos['asimov'].GetBinContent(bin) - tru.GetBinContent(bin)))
                #allHistos['asimov'].SetBinContent(bin, nominal.GetBinContent(bin))
                #print "errorin:", allHistos['asimov'].GetBinError(bin)
                #print "nominal:", nominal.GetBinContent(bin)
                #print "relativo", round(allHistos['asimov'].GetBinError(bin)/allHistos['asimov'].GetBinContent(bin)*100, 1)
                #print "ratio:", allHistos['asimov'].GetBinContent(bin)/tru.GetBinContent(bin)
            #for bin in range(1, tru2.GetNbinsX() + 1):
                #print "verdad2:", tru2.GetBinContent(bin)
            #tmptfile2.Close()
            tmptfile.Close()
            del tru
        
        if not self.wearedoingasimov:
            savetfile = r.TFile("temp/{var}_/unfOutput_{var}.root".format(var = self.var), "recreate")
            nominal.Write()
            for key in allHistos: allHistos[key].Write()
            covnom.Scale(scaleval**2)
            covnom.Write()
            #covitwo = copy.deepcopy(self.helpers[''].tunfolder.GetEmatrixInput("CovMatInput"))
            #covitwo.Scale(scaleval**2)
            #covitwo.Write()
            #covithree = copy.deepcopy(self.helpers[''].tunfolder.GetEmatrixSysUncorr("CovMatResponse"))
            #covithree.Scale(scaleval**2)
            #covithree.Write()
            savetfile.Close()
        
        if not self.wearedoingasimov: nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, self.doColorEnvelope, True)
        else:                         nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, self.doColorEnvelope)
        plot               = bp.beautifulUnfoldingPlots(self.var + "_asimov"  if self.wearedoingasimov else self.var)
        plot.doRatio       = True
        plot.doFit         = False
        plot.plotspath     = self.plotspath
        
        nominal.SetMarkerStyle(r.kFullCircle)
        nominal.SetLineColor(r.kBlack)
        nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
        nominal_withErrors[0].SetLineColor(r.kBlue)
        nominal_withErrors[0].SetFillStyle(1001)
        
        if not self.wearedoingasimov:
            savetfile2 = r.TFile("temp/{var}_/unfOutput_{var}.root".format(var = self.var), "update")
            nom0 = copy.deepcopy(nominal_withErrors[0].Clone("nom0"))
            nom1 = copy.deepcopy(nominal_withErrors[1].Clone("nom1"))
            nom0.Write()
            nom1.Write()
            savetfile2.Close()
            del nom0,nom1
        
        #############################
        print "\nLOS RESULTAOS - {uno} - {dos}".format(uno = "ASIMOV" if self.wearedoingasimov else "DATOS", dos = self.var)
        for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
            print "Bin", bin, "(abs.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin), 4), "-", round(nominal_withErrors[1].GetBinError(bin), 4), ") pb"
            print "Bin", bin, "(rel.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), "-", round(nominal_withErrors[1].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), ") pb\n"
        print "\n"
        #############################
        
        if   "legpos_unf"   in vl.varList[self.var] and not self.wearedoingasimov: legloc = vl.varList[self.var]["legpos_unf"]
        elif "legpos_unfas" in vl.varList[self.var] and     self.wearedoingasimov: legloc = vl.varList[self.var]["legpos_unfas"]
        else:                                                                      legloc = "TR"
        
        if self.doSanityCheck:
            if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information does not exist.')
            tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var))
            tru = copy.deepcopy(tmptfile.Get('tW').Clone('tru'))
            tru.SetLineWidth(2)
            tru.SetLineColor(bp.colorMap[0])
            
            if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist.')
            tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = self.var))
            aMCatNLO = copy.deepcopy(tmptfile2.Get('tW').Clone('aMCatNLO'))
            aMCatNLO.SetLineWidth(2)
            aMCatNLO.SetLineColor(r.kAzure)
            
            if not os.path.isfile('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = self.var)):
                raise RuntimeError('The rootfile with the generated information with the DS variation does not exist.')
            tmptfile3 = r.TFile.Open('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = self.var))
            hDS = copy.deepcopy(tmptfile3.Get('tW').Clone('hDS'))
            hDS.SetLineWidth(2)
            hDS.SetLineColor(r.kGreen)
            
            plot.addHisto(nominal_withErrors, 'hist',   'Total unc.',   'F', 'unc')
            plot.addHisto(tru,                'L,same', 'tW Powheg',    'L', 'mc')
            plot.addHisto(aMCatNLO,           'L,same', 'tW aMCatNLO',  'L', 'mc')
            plot.addHisto(hDS,                'L,same', 'tW DS',        'L', 'mc')
            
            if self.wearedoingasimov: plot.addHisto(nominal, 'P,E,same',"Pseudodata",'P', 'data')
            else:                     plot.addHisto(nominal, 'P,E,same',vl.labellegend,'P', 'data')
            plot.saveCanvas(legloc)
            tmptfile.Close()
            tmptfile2.Close()
            tmptfile3.Close()
        else:
            plot.addHisto(nominal_withErrors, 'E2',     'Total unc.',   'F')
            if self.wearedoingasimov: plot.addHisto(nominal, 'P,same',"Pseudodata",'P', 'data')
            else:                     plot.addHisto(nominal, 'P,same',vl.labellegend,'P', 'data')
            plot.saveCanvas(legloc)

        del plot
        plot2       = bp.beautifulUnfoldingPlots(self.var + 'uncertainties_asimov' if self.wearedoingasimov else self.var + 'uncertainties')
        plot2.doFit = False
        if not self.wearedoingasimov: uncList = ep.getUncList(nominal, allHistos, self.doColorEnvelope, False, True)
        else:                         uncList = ep.getUncList(nominal, allHistos, self.doColorEnvelope, False)
        print 'Full uncertainties list (ordered by impact):', uncList
        uncList     = uncList[:vl.nuncs]
        
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

        #if (maxinctot >= 0.9):
            #if maxinctot >= 5:
                #uncList[0][1].GetYaxis().SetRangeUser(0, 5)
            #else:
                #uncList[0][1].GetYaxis().SetRangeUser(0, maxinctot + 0.1)
            
        #else:
            #uncList[0][1].GetYaxis().SetRangeUser(0, 0.9)
        
        uncList[0][1].GetYaxis().SetRangeUser(0, 2)
        
        for i in range(vl.nuncs):
            uncList[i][1].SetLineColor( vl.NewColorMap[uncList[i][0]] )
            uncList[i][1].SetLineWidth( 2 )
            if "Stat" in uncList[i][0]:
                uncList[i][1].SetLineColor(r.kBlack)
                uncList[i][1].SetLineStyle( 2 )
            elif "Lumi" in uncList[i][0]:
                uncList[i][1].SetLineColor(r.kBlack)
                uncList[i][1].SetLineStyle( 4 )
            
            plot2.addHisto(uncList[i][1], 'H,same' if i else 'H',uncList[i][0],'L')
        
        plot2.addHisto(hincmax, 'H,same', 'Total', 'L')
        plot2.plotspath = self.plotspath
        
        if   "uncleg_unf"   in vl.varList[self.var] and not self.wearedoingasimov: unclegpos = vl.varList[self.var]["uncleg_unf"]
        elif "uncleg_unfas" in vl.varList[self.var]:                               unclegpos = vl.varList[self.var]["uncleg_unfas"]
        else:                                                                      unclegpos = "TR"
        
        plot2.saveCanvas(unclegpos)
        del plot2
        return


    def getConditionNumber(self, nuis):
        return self.helpers[nuis].getConditionNumber()



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
            print "> Special path to things chosen:", pathtothings, "\n"
        else:
            pathtothings    = 'temp/{var}_/'.format(var = varName)
        
    else:
        print "> Default variable and path chosen\n"
        varName       = 'LeadingLepEta'
        pathtothings  = 'temp/{var}_/'.format(var = varName)

    print "\n> Beginning unfolding...\n"
    a = Unfolder(varName, pathtothings + 'cutOutput_' + varName + '.root', 'temp/UnfoldingInfo.root')
    
    a.plotspath       = "results/"
    a.doSanityCheck   = True
    a.doColorEnvelope = True
    a.doRegularisation= vl.doReg
    a.doAreaConstraint= vl.doArea
    
    a.doUnfoldingForAllNuis()       # Unfolding
    a.doScanPlots()                 # L-Curve and curvature plots
    a.doRegularizationComparison()  # Comparison plot between regularisation and not
    a.doAreaConstraintComparison()  # Comparison plot between area constraint and not
    
                                    # Condition numbers text file
    if not os.path.isdir('results/CondN'):
        os.system('mkdir -p results/CondN')
    fcn = open('results/CondN/{var}'.format(var = varName) + '.txt', 'w')
    out = 'Condition number   Systematic     \n'
    for syst in a.sysList + ['']:
        if syst in vl.varList['Names']['ttbarSysts'] + vl.varList['Names']['colorSysts'] + vl.varList['Names']['specialSysts'] + vl.varList['Names']['NormSysts'] + ['LumiUp', 'LumiDown']: continue
        out += str(round(a.getConditionNumber(syst), 4)) +  '          ' + syst + '\n'
    fcn.write(out)
    fcn.close
    del a
    
    if not vl.asimov:
        b = Unfolder(varName, pathtothings + 'cutOutput_' + varName + '_goodasimov.root', 'temp/UnfoldingInfo.root')
        b.plotspath       = "results/"
        b.doSanityCheck   = True
        b.doColorEnvelope = True
        b.doRegularisation= vl.doReg
        b.doAreaConstraint= vl.doArea
        
        b.doUnfoldingForAllNuis()       # Unfolding
        b.doScanPlots()                 # L-Curve and curvature plots
        b.doRegularizationComparison()  # Comparison plot between regularisation and not
        b.doAreaConstraintComparison()  # Comparison plot between area constraint and not
    print "> Unfolding done!"
    
