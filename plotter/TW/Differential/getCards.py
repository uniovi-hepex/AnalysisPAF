import ROOT 
import varList
import sys, os
print "===== BDT's histograms procedure"

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
    minipath    = '/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/19_04_2018/'

NameOfTree = "Mini1j1t";

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.LoadMacro('../../Histo.C+')
ROOT.gROOT.LoadMacro('../../Looper.C+')
ROOT.gROOT.LoadMacro('../../Plot_sub.C+')
ROOT.gROOT.ProcessLine('.L temp/' + varName + '.C+')
print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"

#print ROOT.theBDt_bin1

indx = 0
binning = varList.varList[varName]['recobinning']
print "> Beginning to produce histograms", "\n"

for binDn,binUp in zip(binning, binning[1:]):
    indx = indx+1
    p = ROOT.Plot_sub(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), 10, ROOT.Double(0.5), ROOT.Double(10.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("../../TW/Differential/temp/");
    p.SetPathSignal(pathToTree);
    p.verbose = True

    p.AddSample("WZ",                          "VVttV", ROOT.itBkg, 390);
    p.AddSample("WW",                          "VVttV", ROOT.itBkg);
    p.AddSample("ZZ",                          "VVttV", ROOT.itBkg);
    p.AddSample("TTWToLNu",                    "VVttV", ROOT.itBkg);
    p.AddSample("TTWToQQ" ,                    "VVttV", ROOT.itBkg);
    p.AddSample("TTZToQQ" ,                    "VVttV", ROOT.itBkg);
    p.AddSample("TTZToLLNuNu",                 "VVttV", ROOT.itBkg);

    p.AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY",    ROOT.itBkg, 852)
    p.AddSample("DYJetsToLL_M50_aMCatNLO",     "DY",    ROOT.itBkg);
    p.AddSample("TTbar_Powheg",                "ttbar", ROOT.itBkg, 633)
    p.AddSample("TW",                          "tW",    ROOT.itBkg, 2)
    p.AddSample("TbarW",                       "tW",    ROOT.itBkg);
    p.AddSample("TTbar_PowhegSemi",            "Fakes", ROOT.itBkg, 413)
    p.AddSample("WJetsToLNu_MLM",              "Fakes", ROOT.itBkg)
    
    p.AddSample("TTbar_Powheg",                "ttbar", ROOT.itSys, 1, "JERUp"); 
    p.AddSample("TW"   ,                       "tW",    ROOT.itSys, 1, "JERUp");
    p.AddSample("TbarW",                       "tW",    ROOT.itSys, 1, "JERUp");

    p.AddSample("MuonEG",                      "Data",  ROOT.itData);
    p.AddSample("SingleMuon",                  "Data",  ROOT.itData);
    p.AddSample("SingleElec",                  "Data",  ROOT.itData);

    p.AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig")
  
    p.AddSymmetricHisto("ttbar", "JERUp");
    p.AddSymmetricHisto("tW",    "JERUp");

    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.SaveHistograms();
    del p
    p = None

print "> Done!", "\n"
sys.exit()
