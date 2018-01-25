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
TString outputFolder = "/nfs/fanae/user/juanr/www/stop/jan2018/SignalPlots/";

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins = 21;

//=== Systematics
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale";

void SavePlot(TString name, TString xtitle);
void SaveSignalPlot();
void SetSystematics();
void SetMultiPlot();

MultiPlot* mp = new MultiPlot();

TString DistName = "Lep0Pt_dilep,               MET_dilep, MT2_dilep,  NBtagsNJets, NJets,  InvMass,              Lep0Pt,                     Lep1Pt, Lep0Eta, Lep1Eta, NBtags, DilepPt, DeltaPhi, DeltaEta, MT2, MET, Count";
TString TitlesX  = "Leading lepton p_{T} (GeV), MET (GeV), M_{T2} GeV,            , #NJets, M_{e#mu} (GeV),       Leading lepton p_{T} (GeV), Subleading lepton p_{T} (GeV), Leading lepton #eta, Subleading lepton #eta, #b tags, p_{t}^{e#mu}, #Delta #varphi_{e#mu}, #Delta#eta_{e#mu}, M_{T2} (GeV), MET (GeV), Counts";

void HyperDraw(){
  SetMultiPlot();
  vector<TString> vDistName = TStringToVector(DistName);
  vector<TString> vxtitle   = TStringToVector(TitlesX);
  Int_t nDistNames = vDistName.size();
//  for(Int_t i = 0; i < nDistNames; i++){
//    SavePlot(vDistName.at(i)+"_ElMu", vxtitle.at(i));
//    SavePlot(vDistName.at(i)+"_All",  vxtitle.at(i));
//  }
  SaveSignalPlot();
}

void SetMultiPlot(){
  mp->SetPath(path);
  mp->SetTreeName(NameOfTree);
  mp->SetPathData(  path + "/Full2016Dataset/");
  mp->SetPathSignal(path + "T2tt/");
  mp->SetPlotFolder(outputFolder);

  TString chan = "ElMu";
  TString weight = "TWeight";
  TString cut = BaselineCut; 
  mp->AddDistribution("MT2_" + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);

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

  cut = BaselineCut; chan = "ElMu";
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
  mp->AddDistribution("Count_" + chan, "TChannel", cut, chan, 1, 0, 10);

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
  mp->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics);
	mp->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics);
	mp->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   systematics);
	mp->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   systematics);
	mp->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics);
	mp->AddSample("TTbar_Powheg",                                         "ttbar",  itBkg, kRed+1,     systematics + ", JER, uncl");
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  mp->AddSample("T2tt_225_50_FS_summer", "Stop[225,50]", itSignal, kCyan, systematics + ",JER, uncl"); 
/*
  // ttbar modeling
  mp->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
  mp->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
  mp->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
  mp->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
  mp->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
  mp->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

  // FSR
  TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
  mp->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
  mp->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );


  //>>> Color Reconnection
  mp->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
  mp->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
  mp->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
  mp->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");

  // Top mass
  mp->AddSample("TTbar_Powheg_mtop1715", "ttbar", itSys, 1, "topMassDown");
  mp->AddSample("TTbar_Powheg_mtop1735", "ttbar", itSys, 1, "topMassUp");
 */ 
  // Top pT
  //p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  //p->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtUp");
  //p->SetWeight("TWeight");

  // Plotting options  
  mp->SetOutputName("H_");
  mp->SetLegendTextSize(0.042);
  mp->SetLegendPosition(0.60, 0.60, 0.93, 0.93);

  mp->SetRatioErrorColor(kTeal-2);
  mp->SetRatioErrorStyle(3244);
  mp->SetStackErrorStyle(3244);
  mp->doYieldsInLeg = false; mp->doSignal = false;
  mp->SetRatioMin(0.8); mp->SetRatioMax(1.2);
}


void SetSystematics(){
  // Norm systematics
  mp->AddSystematic("stat"); 
  mp->AddNormUnc("ttbar", 0.05);
  mp->AddNormUnc("VV", 0.30);
  mp->AddNormUnc("ttV", 0.30);
  mp->AddNormUnc("DY", 0.15);
  mp->AddNormUnc("tW", 0.30);
  mp->AddNormUnc("Nonprompt", 0.50);

  mp->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

  // Scale fakes
  Float_t fakeSS = 1.5;
  mp->ScaleProcess("Nonprompt", fakeSS);

  // Scale FSR
  Float_t n  = mp->GetYield("ttbar"); 
  Float_t vu = mp->GetYield("ttbar", "fsrUp");
  Float_t vd = mp->GetYield("ttbar", "fsrDown");
  mp->ScaleSys("ttbar", "fsrUp",   GetFactorFSR(n, vu));
  mp->ScaleSys("ttbar", "fsrDown", GetFactorFSR(n, vd));
}


  // InvaMass....................................................
void SavePlot(TString name, TString xtitle){
  float ratioscale = 1.3;
  mp->SetPlot(name, xtitle, "Events");
  SetSystematics();
  if(name.Contains("NBtagsNJets")) mp->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  else if(name.Contains("eta") || name.Contains("Eta")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.70, 0.70, 0.93, 0.93);
  }
  else if(name.Contains("DeltaPhi")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.5, 0.70, 0.7, 0.93);
  }
  if(name.Contains("DeltaEta")) ratioscale = 1.2;

  mp->doSignal = false;
  mp->SetRatioMin(2-ratioscale); mp->SetRatioMax(ratioscale);
  mp->SetOutputName(name);
  mp->doSetLogy = false;
  mp->DrawStack();
  mp->doSetLogy = true;
  mp->SetOutputName(name+ "_log");
  mp->DrawStack();
}

void SaveSignalPlot(){
  TString name = "MT2_ElMu";
  mp->SetPlot(name);
  mp->SetLegendPosition(0.7, 0.65, 0.9, 0.9);
  SetSystematics();
  mp->SetRatioMin(0.0); mp->SetRatioMax(0.1);
  mp->SetOutputName("Signal");
  mp->SetSignalProcess("Stop[225,50]");
  mp->SetSignalStyle("SM");
  mp->SetRatioYtitle("S/B");
  mp->doData = false;
  mp->doSignal= true;
  mp->doSys = true;

  mp->doSetLogy = false;
  mp->SetScaleMax(1.25);
  mp->DrawStack();
  mp->doSetLogy = true;
  mp->SetScaleMax(1000);
  mp->SetOutputName("Signal_log");
  mp->DrawStack();
}

