import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
from   multiprocessing import Pool
import sys, os, copy, math, shutil
#===================================

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    print "> Chosen variable:", varName, "\n"
    if (len(sys.argv) > 2):
        nCores      = int(sys.argv[2])
        print ('> Parallelization will be done with ' + str(nCores) + ' cores')
    else:
        print '> Sequential execution mode chosen'
        nCores      = 1
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'
    nCores      = 1


vl.SetUpWarnings()
r.gROOT.SetBatch(True)
yieldsoutputpath = "results/Yields"
if not os.path.isdir(yieldsoutputpath):
    os.system('mkdir -p ' + yieldsoutputpath)


#class TableOfResults:
    #def __init__(self, name):
        #self.name              = name
        #self.content           = ""
    
    #def AddLine(self, newline):
    
    #def AddCol(self, newcol):


def printTheDamnResults(tsk):
    var, ty = tsk
    if ty == 'unfolded': tfile = r.TFile("temp/{var}_/unfOutput_{var}.root".format(var = varName), "read")
    else:                tfile = r.TFile("temp/{var}_/cutOutput_{var}.root".format(var = varName), "read")
    
    if ty == "folded": nominal = copy.deepcopy(tfile.Get("data_").Clone("nominal"))
    else:              nominal = copy.deepcopy(tfile.Get(var).Clone("nominal"))
    
    varHist  = {}
    for key in tfile.GetListOfKeys():
        if key.GetName().replace(var+"_", "") == "data_" or (ty == "folded" and "asimov" in key.GetName()) or key.GetName().replace("data_", "").replace(var+"_", "") in vl.varList['Names']['colorSysts']: continue # Colour envelope is applied in any case
        else: varHist[key.GetName().replace("data_", "").replace(var + "_", "")] = copy.deepcopy(key.ReadObj())
    colup = copy.deepcopy(nominal.Clone("ColorRUp"))
    coldn = copy.deepcopy(nominal.Clone("ColorRDown"))
    
    for bin in range(1, nominal.GetNbinsX() + 1): # Colour envelope is applied in any case
        tmpuncUp   = nominal.GetBinContent(bin)
        tmpuncDown = nominal.GetBinContent(bin)
        tmpunc     = 0.
        for key in tfile.GetListOfKeys():
            if key.GetName().replace("data_", "").replace(var+"_", "") in vl.varList['Names']['colorSysts']:
                tmpunc  = key.ReadObj().GetBinContent(bin)
                if tmpunc > tmpuncUp:
                    tmpuncUp    = tmpunc
                elif tmpunc < tmpuncDown:
                    tmpuncDown  = tmpunc
        colup.SetBinContent(bin, tmpuncUp)
        coldn.SetBinContent(bin, tmpuncDown)
    
    # Begin writing
    firstline = "        |"
    for i in range(1, nominal.GetNbinsX()):
        firstline += "        | Bin {bn}"
    firstline += "\n"
    
    output = open(yieldsoutputpath + "/{vr}_results.txt".format(vr = var, t = ty), 'w')
    output.write(out)
    output.close()
    
    return


if nCores == 1:
    if varName != "All":
        printTheDamnResults(varName, 'folded')
        printTheDamnResults(varName, 'unfolded')
    else:
        for var in vl.varList['Names']['Variables']:
            printTheDamnResults(var, 'folded')
            printTheDamnResults(var, 'unfolded')
else:
    tasks = []
    if varName == "All":
        for var in vl.varList['Names']['Variables']:
            for el in ['folded', 'unfolded']:
                tasks.append( (var, el) )
    else:
        tasks.append((varName, 'folded'))
        tasks.append((varName, 'unfolded'))
    pool = Pool(nCores)
    pool.map(printTheDamnResults, tasks)
    pool.close()
    pool.join()
    del pool
