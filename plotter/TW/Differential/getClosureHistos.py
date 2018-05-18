import ROOT as r
import varList
import sys, os
from array import array
print "===== Plots for closure test comparisons procedure\n"

storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
pathToTree    = ""

def GetLastFolder(stpth):
    savefolders   = next(os.walk(stpth))[1]
    saveyears     = map(int, [i[6:]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[6:]) == max(saveyears)]
    savemonths    = map(int, [i[3:5] for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[3:5]) == max(savemonths)]
    savedays      = map(int, [i[:2]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[:2]) == max(savedays)]
    return (stpth + savefolders[0] + "/")


if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    print "> Chosen variable:", varName, "\n"
    if (len(sys.argv) > 2):
      pathToTree    = storagepath + sys.argv[2] + "/"
    else:
      pathToTree    = GetLastFolder(storagepath)
    print "> Minitrees will be read from:", pathToTree, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingJetPt'
    minipath    = "../../../TW_temp/"

NameOfTree = "Mini1j1t";

r.gROOT.SetBatch(True)
r.gROOT.LoadMacro('../../Histo.C+')
r.gROOT.LoadMacro('../../Looper.C+')
r.gROOT.LoadMacro('../../Plot.C+')
r.gROOT.LoadMacro('../../PlotToPyC.C+')


binning = array('f', varList.varList[varName]['genbinning'])
print "> Beginning to produce the histogram", "\n"

p = r.PlotToPyC(r.TString('TGen' + varName), r.TString('1/TWeight*TWeight_normal*(Tpassgen == 1)'), r.TString('All'), len(binning)-1, binning, r.TString('TGen' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder("temp/");
p.SetPathSignal(pathToTree);
p.verbose = True

p.AddSample("UNF_TW",    "tW", r.itBkg, 2)
p.AddSample("UNF_TbarW", "tW", r.itBkg);

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_" + varName);
p.SaveHistograms();
del p

print "> Done!", "\n"
