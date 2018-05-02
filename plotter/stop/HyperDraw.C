R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"
#include "PlotterFunctions.C"

//=== Define paths and tree name
TString path = "/pool/ciencias/userstorage/juanr/stop/jan5/";
TString NameOfTree = "tree";
//TString outputFolder = "./output/StopPlotsData/";
TString outputFolder = "/nfs/fanae/user/juanr/www/stop/mar2018/";

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
const Int_t ngbins = 21; const Int_t ngpbins = 16;
Float_t gbins[ngbins+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200};
Float_t gpbins[ngpbins+1] = {0, 20, 30, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 200};

const Int_t nDeltaEtaBins = 8; const Int_t nMT2BinsS3 = 16;
Float_t gDeltaEtaBins[nDeltaEtaBins+1] = {0, 0.1, 0.2, 0.4, 0.6, 0.8, 1.2, 1.6, 2.4};
Float_t gMT2BinsS3[nMT2BinsS3+1]    = {0, 20, 30, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 200};

const Int_t nmetbins = 2;
Float_t metbins[nmetbins+1] = {150, 200, 400};

const Int_t ndetabins = 8;
Float_t detabins[ndetabins+1] = {0, 0.1, 0.2, 0.4, 0.6, 0.8, 1.2, 1.6, 2.4};

//=== Systematics
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,unclMET";

void SavePlot(TString name, TString xtitle);
void SaveSignalPlot(TString name, TString xtitle = "", float ratiomax = 0.1);
void SetSystematics();
void SetMultiPlot();

MultiPlot* mp = new MultiPlot();

TString DistName = "Lep0Pt_dilep,               MET_dilep, MT2_dilep,  NBtagsNJets, NJets,  InvMass,              Lep0Pt,                     Lep1Pt, Lep0Eta, Lep1Eta, NBtags, DilepPt, DeltaPhi, DeltaEta, MT2, MET, Count";
TString TitlesX  = "Leading lepton p_{T} (GeV), MET (GeV), M_{T2} GeV,            , #NJets, M_{e#mu} (GeV),       Leading lepton p_{T} (GeV), Subleading lepton p_{T} (GeV), Leading lepton #eta, Subleading lepton #eta, #b tags, p_{t}^{e#mu}, #Delta #varphi_{e#mu}, #Delta#eta_{e#mu}, M_{T2} (GeV), MET (GeV), Counts";

void HyperDraw(){
  SetMultiPlot();
  TString dName = "NBtagsNJets, NJets,  InvMass,        Lep0Pt,                     Lep1Pt,                        Lep0Eta,             Lep1Eta, NBtags, DilepPt, DeltaPhi, DeltaEta, MT2, MET, Count";
  TString titX  = "           , #NJets, M_{e#mu} (GeV), Leading lepton p_{T} (GeV), Subleading lepton p_{T} (GeV), Leading lepton #eta, Subleading lepton #eta, #b tags, p_{t}^{e#mu}, #Delta #varphi_{e#mu}, #Delta#eta_{e#mu}, M_{T2} (GeV), MET (GeV), Counts";
  vector<TString> vDistName = TStringToVector(dName);
  vector<TString> vxtitle   = TStringToVector(titX);
  Int_t nDistNames = vDistName.size();
  TString var; TString xtit;
  for(Int_t i = 0; i < nDistNames; i++){
    var = vDistName.at(i); xtit = vxtitle.at(i);
    //SavePlot(var+"_ElMu", xtit);
  }
  SavePlot("MT2_21", "M_{T2} (GeV)");
  SavePlot("MT2_0", "M_{T2} (GeV)");
  cout << "Saving signal plot..." << endl;
  //SavePlot("MET", "MET [GeV]");
  //SaveSignalPlot("DeltaEta_scenario3", "#Delta#eta(e#mu)");
  //SaveSignalPlot("MET_scenario3", "MET (GeV)");
  //SaveSignalPlot("MT2_21", "M_{T2} (GeV)");
/*  SavePlot("MT2_21", "M_{T2} (GeV)");
  SavePlot("MET_2", "MET");
  SavePlot("MT2_16", "M_{T2} (GeV)");
  SavePlot("DeltaEta_8", "#Delta#eta");
  SavePlot("MT2_16_METb150", "M_{T2} (GeV)");
  SavePlot("DeltaEta_8_METb150", "#Delta#eta");

  mp->SetPlotFolder(outputFolder+"Yields/");
  mp->SetYieldsTableName("StopYields");
  mp->PrintYields("", "", "", "tex, txt, html");
  mp->SetYieldsTableName("StopYields");
  mp->PrintBinsYields("tex, txt, html");
  
*/}

void SetMultiPlot(){
  mp->SetPath(path);
  mp->SetTreeName(NameOfTree);
  mp->SetPathData(  path + "/Full2016Dataset/");
  mp->SetPathSignal(path + "T2tt/");
  mp->SetPlotFolder(outputFolder);

  TString chan = "ElMu";
  TString weight = "TWeight";
  TString cut = BaselineCut; 
  mp->AddDistribution("MT2_21",              "TMT2",      cut+ " && TMT2 > 0", chan, 21,    0, 105);
  mp->AddDistribution("MT2_0",              "TMT2",      cut+ " && TMT2 == 0", chan, 1, -0.5, 0.5);
  //mp->AddDistribution("MT2_scenario3"     ,    "TMT2",           cut + " && TMET < 150 && TMT2 > 0",  chan, nMT2BinsS3,    0, 0, gMT2BinsS3);
 // mp->AddDistribution("DeltaEta_scenario3",    "TDeltaEta",      cut + " && TMET < 150 && TMT2 == 0", chan, nDeltaEtaBins, 0, 0, gDeltaEtaBins);
//   mp->AddDistribution("MET_scenario3"     ,    "TMET",           cut + " && TMET >= 150",             chan, 2,    150, 250);
//  mp->AddDistribution("MT2_21",              "TMT2",      cut + " && TMT2 > 0 && TMET < 150", chan, ngpbins,    0, 0, gpbins);
/*
  mp->AddDistribution("MT2_" + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);
  mp->AddDistribution("MT2_21",              "TMT2",      cut, chan, npgbins,    0, 0, gpbins);
  mp->AddDistribution("MET_2",               "TMET",      cut, chan, nmetbins,  0, 0, metbins);

  mp->AddDistribution("DeltaEta_8",          "TDeltaEta", cut + " && TMT2 == 0", chan, ndetabins, 0, 0, detabins);
  mp->AddDistribution("MT2_16",              "TMT2",      cut + " && TMT2 > 0", chan, ngpbins,    0, 0, gpbins);

  mp->AddDistribution("DeltaEta_8_METb150",  "TDeltaEta", cut + " && TMT2 == 0 && TMET < 150", chan, ndetabins, 0, 0, detabins);
  mp->AddDistribution("MT2_16_METb150",      "TMT2",      cut + " && TMT2 > 0 && TMET < 150", chan, ngpbins,    0, 0, gpbins);
*/
/*  
  cut = Dilepton; chan = "ElMu";
  mp->AddDistribution("Lep0Pt_dilep_" + chan,  "TLep0_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("MET_dilep_" + chan, "TMET", cut, chan, 40, 0, 400, 0);
  mp->AddDistribution("MT2_dilep_" + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);
  mp->AddDistribution("NBtagsNJets_" + chan,   "NBtagNJets(TNJets,TNBtags)",   cut, chan,  16, 0, 5);
  mp->AddDistribution("NJets_" + chan,   "TNJets",   cut, chan,  9, 0, 8);

  cut = Dilepton; chan = "All";
  mp->AddDistribution("Lep0Pt_dilep_" + chan,  "TLep0_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("MET_dilep_" + chan, "TMET", cut, chan, 40, 0, 400, 0);
  mp->AddDistribution("MT2_dilep_" + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);
  mp->AddDistribution("NBtagsNJets_" + chan,   "NBtagNJets(TNJets,TNBtags)",   cut, chan,  16, 0, 5);
  mp->AddDistribution("NJets_" + chan,   "TNJets",   cut, chan,  9, 0, 8);
*/
/*  cut = Dilepton; chan = "ElMu";
  mp->AddDistribution("NBtagsNJets_" + chan,   "NBtagNJets(TNJets,TNBtags)",   cut, chan,  16, 0, 5);
  mp->AddDistribution("NJets_" + chan,   "TNJets",   cut, chan,  9, 0, 8);

  cut = BaselineCut; chan = "ElMu";
  mp->AddDistribution("InvMass_" + chan, "TMll",     cut, chan, 30, 0, 600);
  mp->AddDistribution("Lep0Pt_" + chan,  "TLep0_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("Lep1Pt_" + chan,  "TLep1_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("Lep0Eta_" + chan, "TLep0_Eta", cut, chan, 20, -2.4, 2.4);
  mp->AddDistribution("Lep1Eta_" + chan, "TLep1_Eta", cut, chan, 20, -2.4, 2.4);
  mp->AddDistribution("NBtags_" + chan,   "TNBtags",   cut, chan,  5, 0, 4);
  mp->AddDistribution("DilepPt_" + chan,   "DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",   cut, chan,  30, 0, 300);
  mp->AddDistribution("DeltaPhi_" + chan, "TDeltaPhi",     cut, chan, 30, 0, 3.15);
  mp->AddDistribution("DeltaEta_" + chan, "TDeltaEta",     cut, chan, 30, 0, 2.4);
  mp->AddDistribution("MET_" + chan, "TMET", cut, chan, 40, 0, 400, 0);*/
//  mp->AddDistribution("Count_" + chan, "TChannel", cut, chan, 1, 0, 10);
/*
  cut = BaselineCut; chan = "All";
  mp->AddDistribution("InvMass_" + chan, "TMll",     cut, chan, 40, 0, 600);
  mp->AddDistribution("Lep0Pt_" + chan,  "TLep0_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("Lep1Pt_" + chan,  "TLep1_Pt", cut, chan, 25, 0, 250);
  mp->AddDistribution("Lep0Eta_" + chan, "TLep0_Eta", cut, chan, 20, -2.4, 2.4);
  mp->AddDistribution("Lep1Eta_" + chan, "TLep1_Eta", cut, chan, 20, -2.4, 2.4);
  mp->AddDistribution("NBtags_" + chan,   "TNBtags",   cut, chan,  5, 0, 4);
  mp->AddDistribution("DilepPt_" + chan,   "DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",   cut, chan,  30, 0, 300);
  mp->AddDistribution("DeltaPhi_" + chan, "TDeltaPhi",     cut, chan, 30, 0, 3.15);
  mp->AddDistribution("DeltaEta_" + chan, "TDeltaEta",     cut, chan, 30, 0, 2.4);
  mp->AddDistribution("MET_" + chan, "TMET", cut, chan, 40, 0, 400, 0);
  mp->AddDistribution("MT2_" + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);
  mp->AddDistribution("Count_" + chan, "TChannel", cut, chan, 1, 0, 10);
*/
  mp->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics,weight);
	mp->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics,weight);
	mp->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   systematics,weight);
	mp->AddSample("TW, TbarW",                                            "tW",        itBkg, kViolet-2,   systematics,weight);
	mp->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics,weight);
	mp->AddSample("TTbar_Powheg",                                         "t#bar{t}",     itBkg, kRed+1,     systematics,weight);
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData, weight);

  mp->AddSample("T2tt_mStop227_mLsp52", "T2tt[227.5,52.5]", itSignal, kAzure+8, systematics,weight); 

  // t#bar{t} modeling
  mp->AddSample("TTbar_Powheg_ueUp", "t#bar{t}", itSys, 1, "ueUp",weight);
  mp->AddSample("TTbar_Powheg_ueDown", "t#bar{t}", itSys, 1, "ueDown",weight);
  mp->AddSample("TTbar_Powheg_isrUp", "t#bar{t}", itSys, 1, "isrUp",weight);
  mp->AddSample("TTbar_Powheg_isrDown", "t#bar{t}", itSys, 1, "isrDown",weight);
  mp->AddSample("TTbar_Powheg_hdampUp", "t#bar{t}", itSys, 1, "hdampUp",weight);
  mp->AddSample("TTbar_Powheg_hdampDown", "t#bar{t}", itSys, 1, "hdampDown",weight);

  // FSR
  TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
  mp->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrUp",     "t#bar{t}", itSys, 1, "fsrUp","TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  mp->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrDown",   "t#bar{t}", itSys, 1, "fsrDown","TWeight*GetFSR_BtagSF_Down(TBtagPt)");

  //>>> Color Reconnection
  mp->AddSample("TTbar_Powheg_erdON", "t#bar{t}", itSys, 1, "Powheg_erdON",weight);
  mp->AddSample("TTbar_QCDbasedCRTune_erdON", "t#bar{t}", itSys, 1, "QCDbasedCRTune_erdON",weight);
  mp->AddSample("TTbar_GluonMoveCRTune", "t#bar{t}", itSys, 1, "GluonMoveCRTune",weight);
  mp->AddSample("TTbar_GluonMoveCRTune_erdON", "t#bar{t}", itSys, 1, "GluonMoveCRTune_erdON",weight);

  // Top mass
  mp->AddSample("TTbar_Powheg_mtop1715", "t#bar{t}", itSys, 1, "topMassDown",weight);
  mp->AddSample("TTbar_Powheg_mtop1735", "t#bar{t}", itSys, 1, "topMassUp",weight);
  
  // Top pT
  //p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  //p->AddSample("TTbar_Powheg","t#bar{t}", itSys, 1, "TopPtUp","GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  //p->SetWeight("TWeight");

  // Plotting options  
  cout << "Adjusting options...\n";
  mp->SetOutputName("H_");
  mp->SetLegendTextSize(0.042);
  mp->SetLegendPosition(0.60, 0.60, 0.93, 0.93);

  mp->SetRatioErrorColor(kTeal-2);
  mp->SetRatioErrorStyle(3244);
  mp->SetStackErrorStyle(3244);
  mp->doYieldsInLeg = false; mp->doSignal = false;
  mp->SetRatioMin(0.8); mp->SetRatioMax(1.2);
  cout << "Done SetMultiplot!\n";
}


void SetSystematics(){
  // Norm systematics
  mp->AddSystematic("stat"); 
  mp->AddNormUnc("t#bar{t}", 0.05);
  mp->AddNormUnc("VV", 0.30);
  mp->AddNormUnc("ttV", 0.30);
  mp->AddNormUnc("DY", 0.15);
  mp->AddNormUnc("tW", 0.30);
  mp->AddNormUnc("Nonprompt", 0.50);

  mp->UseEnvelope("t#bar{t}", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

  // Scale fakes
  Float_t fakeSS = 1.5;
  mp->ScaleProcess("Nonprompt", fakeSS);

  // Group minor backgrounds
  mp->GroupProcesses("VV", "Others");
  mp->GroupProcesses("ttV", "Others");
  mp->GroupProcesses("DY", "Others");
  mp->GroupProcesses("Nonprompt", "Others");
  mp->SetLineColor("Others", kSpring-2);

  // Scale FSR
  Float_t n  = mp->GetYield("t#bar{t}"); 
  Float_t vu = mp->GetYield("t#bar{t}", "fsrUp");
  Float_t vd = mp->GetYield("t#bar{t}", "fsrDown");
  mp->ScaleSys("t#bar{t}", "fsrUp",   GetFactorFSR(n, vu));
  mp->ScaleSys("t#bar{t}", "fsrDown", GetFactorFSR(n, vd));
}


  // InvaMass....................................................
void SavePlot(TString name, TString xtitle){
  float ratioscale = 1.3;
  mp->SetPlot(name, xtitle, "Events");
  SetSystematics();
  mp->doSignal = false;
  if(name.Contains("NBtagsNJets")) mp->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  else if(name.Contains("eta") || name.Contains("Eta")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.70, 0.70, 0.93, 0.93);
  }
  else if(name.Contains("DeltaPhi")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.5, 0.70, 0.7, 0.93);
  }
  else if(name.Contains("MET")) ratioscale = 1.4;
  else if(name.Contains("MT2")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.75, 0.75, 0.9, 0.9);
    //mp->SetNColumns(2);
    mp->SetPlotMinimum(50);
    mp->SetPlotMaximum(1.5e5);
    ratioscale = 1.2;
   // mp->doSignal = true;
   // mp->SetSignalStyle("H");
  }
  //if(name.Contains("DeltaEta")) ratioscale = 1.2;

  mp->SetRatioMin(2-ratioscale); mp->SetRatioMax(ratioscale);
  mp->SetOutputName(name);
  mp->doSetLogy = false;
  mp->DrawStack();
  mp->doSetLogy = true;
  mp->SetOutputName(name+ "_log");
  mp->DrawStack();
}

void SaveSignalPlot(TString name, TString xtitle, float ratiomax){
  cout << "Setting plot..." << endl;
  mp->SetPlot(name, xtitle, "Events");
  SetSystematics();
  mp->doSignal= true; mp->doData = false; 
  mp->doUncInLegend = false;

  cout << "Setting plot options..." << endl;
  mp->SetLegendPosition(0.78, 0.75, 0.9, 0.9);
  mp->SetPlotMinimum(50);
  mp->SetPlotMaximum(1.5e5);
//  mp->SetRatioMin(0.0); mp->SetRatioMax(ratiomax);
  mp->SetOutputName("Signal_"+name);
  mp->SetSignalProcess("T2tt[227.5,52.5]");
  mp->SetSignalStyle("SM");
  mp->SetRatioYtitle("S/B");

  //mp->SetBinLabels("MET #in [150,200] GeV | MET > 200 GeV", '|');
  mp->doSetLogy = false;
  mp->SetScaleMax(1.25);
  cout << "Drawing plot..." << endl;
  mp->DrawStack();
  mp->doSetLogy = true;
  mp->SetScaleMax(1000);
  mp->SetOutputName("Signal_"+name + "_log");
  mp->DrawStack();
}

