''' 
Piece of art for doing the unfolding analysis - with a bit of tW flavor.

''' 

import copy
import ROOT as r



class DataContainer:
    ''' Class to store all the needed inputs: response matrices 
    and varied input distributions'''

    def __init__(self, fileName, fileNameReponse):
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
            if key.GetName() == 'tW':
                self.responseMatrices[''] = copy.deepcopy(key.ReadObj())
            else:
                sysName = '_'.join(key.GetName().split('_')[1:])
                systListResp.append(systListResp)
                self.responseMatrices[sysName] = copy.deepcopy(key.ReadObj())
        if '' not in self.responseMatrices:
            raise RuntimeError("Nominal response matrix not there")

        if (systListResp != listOfSysts):
            raise RuntimeError("Different nuisances in response and inputs")

    def getMatrixNInput(nuis):
        if nuis not in listOfSysts:
            raise RuntimeError("%s is not in the list of response"%nuis)
        return self.unfoldingInputs[nuis], self.responseMatrices[nuis]



class UnfolderHelper:
    ''' Performs unfolding for one specific nuisance'''
    def __init__(self, var, nuis):
        self.var  = var
        self.nuis = nuis

    def makeUnfolderCore(unfInput, response):
        self.unfInput = unfInput
        self.response = response
        self.tunfolder = ROOT.TUnfoldDensity(self.response, ROOT.TUnfold.kHistMapOutputHoriz,
                                             ROOT.TUnfold.kRegModeCurvature, ROOT.TUnfold.kEConstraintNone,
                                             ROOT.TUnfoldDensity.kDensityModeeNone)
        self.tunfolder.SetInput( self.unfInput )

    def doLCurveScan(self):
        if nuis != '': 
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX=ROOT.TSpline3() 
        self.logTauY=ROOT.TSpline3() 
        self.logTauCurv= ROOT.TSpline3() 
        self.lCurve=ROOT.TGraph(0) 

        self.themax = self.tunfolder.ScanLcurve(1000, 1e-8, 1e-2, self.lCurve, self.logTauX,
                                                self.logTauY,self.logTauCurv)

        
    def doTauScan(self):
        if nuis != '': 
            raise RuntimeError("Attempted to do naughty things like doing the scan with varied nuisances")

        self.logTauX=ROOT.TSpline3() 
        self.logTauY=ROOT.TSpline3() 
        self.scanRes=ROOT.TGraph(0) 

        
        # kEScanTauRhoAvg          average global correlation coefficient (from TUnfold::GetRhoI())
        # kEScanTauRhoMax          maximum global correlation coefficient (from TUnfold::GetRhoI())
        # kEScanTauRhoAvgSys       average global correlation coefficient (from TUnfoldSys::GetRhoItotal())
        # kEScanTauRhoMaxSys       maximum global correlation coefficient (from TUnfoldSys::GetRhoItotal())
        # kEScanTauRhoSquareAvg    average global correlation coefficient squared (from TUnfold::GetRhoI())
        # kEScanTauRhoSquareAvgSys average global correlation coefficient squared (from TUnfoldSys::GetRhoItotal())
        ROOT.TInterpreter.LoadMacro('myScanner.C+')
        self.themax = ROOT.myScanner(tunfolder, 1000, 1e-8, 1e-2, self.scanRes, ROOT.TUnfoldDensity.kEScanTauRhoAvg,
                                     logTauXPlot=self.logTauX, logTauYPlot=self.logTauY)


    def doScanPlots(self):

        # Plot the scan variable and the maximum
        t=ROOT.Double(0)
        x=ROOT.Double(0)
        y=ROOT.Double(0)
        
        c = TCanvas() 

        self.logTauX.GetKnot( themax, t, x)
        self.logTauY.GetKnot( themax, t, y)
        
        self.logTauX.Draw()


        
    
class Unfolder():
    def __init__(self, var, fileName, fileNameReponse):
        self.var  = var
        self.Data = DataContainer(fileName, fileNameReponse)
        self.sysList = self.Data.listOfSysts
        self.helpers = { nuis : UnfolderHelper(self.var, nuis) for nuis in sysList }

    def prepareAllHelpers(self):
        for nuis in self.sysList:
            self.helpers[nuis].makeUnfolderCore(self.Data.getMatrixNInput(nuis))

    def prepareNominalHelper(self):
        self.helpers[''].makeUnfolderCore(self.Data.getMatrixNInput(''))

    def doLCurveScan(self):
        self.helpers[''].doLCurveScan()

    def doScanTau(self):
        self.helpers[''].doScanTau()
    
if __name__=="__main__":
    DataContainer
