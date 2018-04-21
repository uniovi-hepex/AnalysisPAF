import ROOT 
import varList
import sys

pathToTree = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/19_04_2018/";
NameOfTree = "Mini1j1t";

varName = sys.argv[1]

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.LoadMacro('../../Histo.C+')
ROOT.gROOT.LoadMacro('../../Looper.C+')
ROOT.gROOT.LoadMacro('../../Plot_sub.C+')
ROOT.gROOT.ProcessLine('.L temp/'+varName+'.C+')
print 'Loaded temp/'+varName+'.C'

print ROOT.theBDt_bin1

indx = 0

binning = varList.varList[varName]['recobinning']

for binDn,binUp in zip(binning, binning[1:]):
    indx = indx+1
    p = ROOT.Plot_sub(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >= %4.2f  && TLeadingJetPt < %4.2f )'%(binDn, binUp)), ROOT.TString('ElMu'), 10, ROOT.Double(0.5), ROOT.Double(10.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("../../TW/Differential/temp/");
    p.SetPathSignal(pathToTree);

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
    p.SaveHistograms();
    del p
                  
