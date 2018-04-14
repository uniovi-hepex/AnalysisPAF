''' 
Piece of art for doing the unfolding analysis - with a bit of tW flavor.

''' 

import copy
import ROOT as r
import beautifulUnfoldingPlots


class DataContainer:
    ''' Class to store all the needed inputs: response matrices 
    and varied input distributions'''

    def __init__(self, var, fileName, fileNameReponse):
        self.var             = var
        self.fileName        = fileName
        self.fileNameReponse = fileNameReponse
        self.listOfSysts = [] 
        self.responseMatrices = {}
        self.unfoldingInputs  = {}
        self.readAndStore()

    def readAndStore(self):
        # Getting uncertainties in the input
        tfile = r.TFile.Open(self.fileName)
        for key in tfile.GetListOfKeys():
            print key
            if key.GetName() == 'tW':
                self.unfoldingInputs[''] = copy.deepcopy(key.ReadObj())
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
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
            if self.var not in key.GetName(): continue
            if key.GetName() == 'R'+self.var:
                self.responseMatrices[''] = copy.deepcopy(key.ReadObj())
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                systListResp.append(systListResp)
                self.responseMatrices[sysName] = copy.deepcopy(key.ReadObj())

        if '' not in self.responseMatrices:
            raise RuntimeError("Nominal response matrix not there")

        if (systListResp != self.listOfSysts):
            print RuntimeWarning("Different nuisances in response and inputs")
            #raise RuntimeWarning("Different nuisances in response and inputs")

    def getMatrixNInput(self,nuis):
        if nuis not in self.listOfSysts + ['']:
            raise RuntimeError("%s is not in the list of response"%nuis)
        return self.unfoldingInputs[nuis], self.responseMatrices[nuis]



class UnfolderHelper:
    ''' Performs unfolding for one specific nuisance'''
    def __init__(self, var, nuis):
        self.var  = var
        self.nuis = nuis

    def makeUnfolderCore(self,unfInputNresponse):
        self.unfInput , self.response = unfInputNresponse
        self.tunfolder = r.TUnfoldDensity(self.response, r.TUnfold.kHistMapOutputHoriz,
                                             r.TUnfold.kRegModeCurvature, r.TUnfold.kEConstraintNone,
                                             r.TUnfoldDensity.kDensityModeeNone)
        self.tunfolder.SetInput( self.unfInput )

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

        
    
class Unfolder():
    def __init__(self, var, fileName, fileNameReponse):
        self.var  = var
        self.Data = DataContainer(var,fileName, fileNameReponse)
        self.sysList = self.Data.listOfSysts
        self.helpers = { nuis : UnfolderHelper(self.var, nuis) for nuis in self.sysList }
        self.helpers[''] = UnfolderHelper(self.var, '')
        
        print self.sysList

    def prepareAllHelpers(self):
        for nuis in self.sysList:
            self.helpers[nuis].makeUnfolderCore(self.Data.getMatrixNInput(nuis))

    def prepareNominalHelper(self):
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
        
    def doNominalPlot(self):
        data = self.helpers[''].tunfolder.GetOutput('forPlot')
        plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots(self.var)
        data.SetMarkerStyle(r.kFullCircle)
        data.GetXaxis().SetNdivisions(505,True)
        plot.addHisto(data,'P,E','Data','P')
        plot.saveCanvas('TR')
        
        
    
if __name__=="__main__":
    a = Unfolder('LeadingJetPt','cards/cardFile_Jet1_pt.root','~vrbouza/www/TFM/Unfolding/UnfoldingInfo.root')
    a.prepareNominalHelper()
    a.doLCurveScan()
    #a.doTauScan()
    a.doScanPlots()
    a.doNominalPlot()

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
