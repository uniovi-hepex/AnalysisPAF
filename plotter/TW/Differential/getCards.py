import ROOT
import varList
import sys, os
print "===== BDT's histograms procedure\n"

storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
pathToTree    = ""

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    print "> Chosen variable:", varName, "\n"
    if (len(sys.argv) > 2):
      pathToTree    = storagepath + sys.argv[2] + "/"
    else:
      pathToTree    = varList.GetLastFolder(storagepath)
    print "> Minitrees will be read from:", pathToTree, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingJetPt'
    minipath    = "../../../TW_temp/"

NameOfTree = "Mini1j1t";

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.LoadMacro('../../Histo.C+')
ROOT.gROOT.LoadMacro('../../Looper.C+')
ROOT.gROOT.LoadMacro('../../PlotToPy.C+')
ROOT.gROOT.ProcessLine('.L temp/' + varName + '.C+')
print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"


indx = 0
binning = varList.varList[varName]['recobinning']
print "> Beginning to produce histograms", "\n"

for binDn,binUp in zip(binning, binning[1:]):
    indx = indx+1
    p = ROOT.PlotToPy(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), varList.nBinsInBDT, ROOT.Double(0.5), ROOT.Double(varList.nBinsInBDT+0.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/");
    p.SetPathSignal(pathToTree);
    p.SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
    p.verbose = True
    
    p.AddSample("TW",                           "tW",    ROOT.itBkg, ROOT.TColor.GetColor("#ffcc33"))
    p.AddSample("TbarW",                        "tW",    ROOT.itBkg);
    
    p.AddSample("TTbar_PowhegSemi",             "Fakes", ROOT.itBkg, 413)
    p.AddSample("WJetsToLNu_MLM",               "Fakes", ROOT.itBkg)
    
    p.AddSample("WZ",                           "VVttV", ROOT.itBkg, 390);
    p.AddSample("WW",                           "VVttV", ROOT.itBkg);
    p.AddSample("ZZ",                           "VVttV", ROOT.itBkg);
    p.AddSample("TTWToLNu",                     "VVttV", ROOT.itBkg);
    p.AddSample("TTWToQQ" ,                     "VVttV", ROOT.itBkg);
    p.AddSample("TTZToQQ" ,                     "VVttV", ROOT.itBkg);
    p.AddSample("TTZToLLNuNu",                  "VVttV", ROOT.itBkg);

    p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY",    ROOT.itBkg, 852)
    p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY",    ROOT.itBkg);
    
    p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itBkg, 633)
    
    p.AddSample("MuonEG",                       "Data",  ROOT.itData);
    p.AddSample("SingleMuon",                   "Data",  ROOT.itData);
    p.AddSample("SingleElec",                   "Data",  ROOT.itData);
    
    p.AddSample("TW",                           "tW",    ROOT.itSys, 1, "JERUp");
    p.AddSample("TW_noFullyHadr_isrUp",         "tW",    ROOT.itSys, 1, "isrUp");
    p.AddSample("TW_noFullyHadr_isrDown",       "tW",    ROOT.itSys, 1, "isrDown");
    p.AddSample("TW_noFullyHadr_fsrUp",         "tW",    ROOT.itSys, 1, "fsrUp");
    p.AddSample("TW_noFullyHadr_fsrDown",       "tW",    ROOT.itSys, 1, "fsrDown");
    p.AddSample("TW_noFullyHadr_MEscaleUp",     "tW",    ROOT.itSys, 1, "tWMEUp");
    p.AddSample("TW_noFullyHadr_MEscaleDown",   "tW",    ROOT.itSys, 1, "tWMEDown");
    p.AddSample("TW_noFullyHadr_PSscaleUp",     "tW",    ROOT.itSys, 1, "tWPSUp");
    p.AddSample("TW_noFullyHadr_PSscaleDown",   "tW",    ROOT.itSys, 1, "tWPSDown");
    
    p.AddSample("TbarW",                        "tW",    ROOT.itSys, 1, "JERUp");
    p.AddSample("TbarW_noFullyHadr_isrUp",      "tW",    ROOT.itSys, 1, "isrUp");
    p.AddSample("TbarW_noFullyHadr_isrDown",    "tW",    ROOT.itSys, 1, "isrDown");
    p.AddSample("TbarW_noFullyHadr_fsrUp",      "tW",    ROOT.itSys, 1, "fsrUp");
    p.AddSample("TbarW_noFullyHadr_fsrDown",    "tW",    ROOT.itSys, 1, "fsrDown");
    p.AddSample("TbarW_noFullyHadr_MEscaleUp",  "tW",    ROOT.itSys, 1, "tWMEUp");
    p.AddSample("TbarW_noFullyHadr_MEscaleDown","tW",    ROOT.itSys, 1, "tWMEDown");
    p.AddSample("TbarW_noFullyHadr_PSscaleUp"  ,"tW",    ROOT.itSys, 1, "tWPSUp");
    p.AddSample("TbarW_noFullyHadr_PSscaleDown","tW",    ROOT.itSys, 1, "tWPSDown");
    
    p.AddSample("TW_noFullyHadr_DS",            "tW",    ROOT.itSys, 1, "DSUp");
    p.AddSample("TbarW_noFullyHadr_DS",         "tW",    ROOT.itSys, 1, "DSUp");
    p.AddSymmetricHisto("tW",  "DSUp");
    p.AddSymmetricHisto("tW",  "JERUp");
    
    p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itSys, 1, "JERUp");
    #p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itSys, 1, "ScaleUp");
    #p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itSys, 1, "ScaleDown");
    #p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itSys, 1, "pdfUp");
    #p.AddSample("TTbar_Powheg",                 "ttbar", ROOT.itSys, 1, "pdfDown");
    p.AddSample("TTbar_Powheg_hdampUp",         "ttbar", ROOT.itSys, 1, "hdampUp");
    p.AddSample("TTbar_Powheg_hdampDown",       "ttbar", ROOT.itSys, 1, "hdampDown");
    p.AddSample("TTbar_Powheg_ueUp",            "ttbar", ROOT.itSys, 1, "UEUp");
    p.AddSample("TTbar_Powheg_ueDown",          "ttbar", ROOT.itSys, 1, "UEDown");
    p.AddSample("TTbar_Powheg_isrUp",           "ttbar", ROOT.itSys, 1, "isrUp");
    p.AddSample("TTbar_Powheg_isrDown",         "ttbar", ROOT.itSys, 1, "isrDown");
    p.AddSample("TTbar_Powheg_fsrUp",           "ttbar", ROOT.itSys, 1, "fsrUp");
    p.AddSample("TTbar_Powheg_fsrDown",         "ttbar", ROOT.itSys, 1, "fsrDown");
    p.AddSample("TTbar_GluonMoveCRTune",        "ttbar", ROOT.itSys, 1, "GluonMoveCRTune");
    p.AddSample("TTbar_GluonMoveCRTune_erdON",  "ttbar", ROOT.itSys, 1, "GluonMoveCRTuneerdON");
    p.AddSample("TTbar_Powheg_erdON",           "ttbar", ROOT.itSys, 1, "PowhegerdON");
    p.AddSample("TTbar_QCDbasedCRTune_erdON",   "ttbar", ROOT.itSys, 1, "QCDbasedCRTuneerdON");
    #p.UseEnvelope("ttbar", "GluonMoveCRTune,GluonMoveCRTuneerdON,PowhegerdON,QCDbasedCRTuneerdON", "ColorReconnection");
    p.AddSymmetricHisto("ttbar",  "JERUp");
    
    # NOTE: statistic uncertanties are plugged inside the fitter under "fit" unc.
    #       category. This uncertanties are provided by combine in specific bins.
    #       The rest of the uncertanties must be plugged here (or through the 
    #       "AddSample" format).
    p.AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig")
    
    p.doUncInLegend  = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );

    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg  = False;
    p.doSetLogy      = False;
    #p.doData         = False;
    p.doSignal       = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.DrawStack();
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.SaveHistograms();
    del p

print "> Done!", "\n"
