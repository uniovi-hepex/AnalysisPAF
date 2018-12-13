import ROOT as r
import varList as vl
import sys, os
from array import array
print "===== Plots for closure test comparisons procedure\n"
vl.SetUpWarnings()

pathToTree  = ""
recocut     = "Tpassreco == 1"
gencut      = '1/TWeight*TWeight_normal*(Tpassgen == 1)'

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    print "> Chosen variable:", varName, "\n"
    if (len(sys.argv) > 2):
        if sys.argv[2] == 'last':
            pathToTree    = vl.GetLastFolder(vl.storagepath)
        else:
            pathToTree    = vl.storagepath + sys.argv[2] + "/"
    else:
        pathToTree  = vl.minipath
    print "> Minitrees will be read from:", pathToTree, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'
    pathToTree  = vl.minipath


NameOfTree = "Mini1j1t";

r.gROOT.SetBatch(True)
r.gROOT.LoadMacro('../../Histo.C+')
r.gROOT.LoadMacro('../../Looper.C+')
r.gROOT.LoadMacro('../../Plot.C+')
r.gROOT.LoadMacro('../../PlotToPyC.C+')


if vl.doxsec: opts = 'noScaleLumi'
else:         opts = ''
binning     = array('f', vl.varList[varName]['genbinning'])
binningreco = array('f', vl.varList[varName]['recobinning'])
if not os.path.isdir('temp/{var}_'.format(var = varName)):
    os.system('mkdir -p temp/{var}_'.format(var = varName))
print "> Beginning to produce the histograms", "\n"

p = r.PlotToPyC(r.TString(vl.varList[varName]['var_gen']), r.TString(gencut), r.TString('All'), len(binning)-1, binning, r.TString('TGen' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

#p.AddSample("UNF_TW",    "tW", r.itBkg, 2, '', opts)
#p.AddSample("UNF_TbarW", "tW", r.itBkg, 2, '', opts)

specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('UNF_TW',                     'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('UNF_TW_noFullyHadr',         'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('UNF_TbarW',                  'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('UNF_TbarW_noFullyHadr',      'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
p.SetWeight('TWeight')

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_" + varName);
p.SaveHistograms();
del p

p = r.PlotToPyC(r.TString(vl.varList[varName]['var_gen']), r.TString(gencut), r.TString('All'), len(binning)-1, binning, r.TString('TGen' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

p.AddSample("UNF_TW_aMCatNLO",    "tW", r.itBkg, 2, '', opts)

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_aMCatNLO_" + varName);
p.SaveHistograms();
del p

p = r.PlotToPyC(r.TString(vl.varList[varName]['var_gen']), r.TString(gencut), r.TString('All'), len(binning)-1, binning, r.TString('TGen' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

p.AddSample("UNF_TW_noFullyHadr_DS",    "tW", r.itBkg, 2, '', opts)
p.AddSample("UNF_TbarW_noFullyHadr_DS", "tW", r.itBkg, 2, '', opts)

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_DS_" + varName);
p.SaveHistograms();
del p

p = r.PlotToPyC(r.TString(vl.varList[varName]['var']), r.TString(recocut), r.TString('All'), len(binningreco)-1, binningreco, r.TString('T' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

#p.AddSample("TW",    "tW", r.itBkg, 2, '', opts)
#p.AddSample("TbarW", "tW", r.itBkg, 2, '', opts)

specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('TW',                     'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('TW_noFullyHadr',         'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('TbarW',                  'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
p.SetWeight('TWeight*' + str(specialweight))
p.AddSample('TbarW_noFullyHadr',      'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
p.SetWeight('TWeight')

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_recobinning_" + varName);
p.SaveHistograms();
del p

p = r.PlotToPyC(r.TString(vl.varList[varName]['var']), r.TString(recocut), r.TString('All'), len(binningreco)-1, binningreco, r.TString('T' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

p.AddSample("TW_aMCatNLO",    "tW", r.itBkg, 2, '', opts)

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_aMCatNLO_recobinning_" + varName);
p.SaveHistograms();
del p

p = r.PlotToPyC(r.TString(vl.varList[varName]['var']), r.TString(recocut), r.TString('All'), len(binningreco)-1, binningreco, r.TString('T' + varName), r.TString(''))
p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
p.SetLimitFolder('temp/{var}_/'.format(var = varName));
p.SetPathSignal(pathToTree);
p.verbose = True
p.verbose = False
p.SetLumi(vl.Lumi)

p.AddSample("TW_noFullyHadr_DS",    "tW", r.itBkg, 2, '', opts)
p.AddSample("TbarW_noFullyHadr_DS", "tW", r.itBkg, 2, '', opts)

p.NoShowVarName = True;
p.SetOutputName("ClosureTest_DS_recobinning_" + varName);
p.SaveHistograms();
del p

print "> Done!", "\n"
