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
TString path = "/pool/ciencias/userstorage/juanr/stop/apr2018/ElMu/";
TString NameOfTree = "tree";
//TString outputFolder = "./output/StopPlotsData/";
TString outputFolder = "/nfs/fanae/user/juanr/www/stop/apr2018/plots/";
Bool_t doFakesFromData = false;
Bool_t doAllPlots = false;
Bool_t doTTbarSyst = false;

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
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
TString systematics = "Trig,JES,JER,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale";

void SavePlot();
void SaveSignalPlot();
void SetSystematics();
void SetMultiPlot();

MultiPlot* mp = new MultiPlot();
TString name; TString cut; TString weight; TString chan; TString var;

TString DistName = "Lep0Pt_dilep,               MET_dilep, MT2_dilep,  NBtagsNJets, NJets,  InvMass,              Lep0Pt,                     Lep1Pt, Lep0Eta, Lep1Eta, NBtags, DilepPt, DeltaPhi, DeltaEta, MT2, MET, Count";
TString TitlesX  = "Leading lepton p_{T} (GeV), MET (GeV), M_{T2} GeV,            , #NJets, M_{e#mu} (GeV),       Leading lepton p_{T} (GeV), Subleading lepton p_{T} (GeV), Leading lepton #eta, Subleading lepton #eta, #b tags, p_{t}^{e#mu}, #Delta #varphi_{e#mu}, #Delta#eta_{e#mu}, M_{T2} (GeV), MET (GeV), Counts";

void AnalResults(){
  // Run over all samples!
  SetMultiPlot();

  // Draw all data plots
  //mp->doSignal = true; mp->SetSignalStyle("SM");
  mp->SetPlotFolder(outputFolder);
  name = "MT2"; mp->SetPlot(name, "M_{T2} (GeV)", "Events / 5 GeV"); SetSystematics(); SavePlot();
  mp->SetYieldsTableName("StopYields_group");
  mp->PrintYields("", "", "", "tex, txt, html");
  mp->SetYieldsTableName("StopPerBin_group");
  mp->PrintBinsYields("tex, txt, html");

  if(doAllPlots){
  name = "Lep0Pt_dilep"; mp->SetPlot(name, "Leading lepton p_{T} (GeV)", "Events"); SetSystematics(); SavePlot();
  name = "Lep0Eta_dilep"; mp->SetPlot(name, "Leading lepton #eta", "Events"); SetSystematics(); SavePlot();
  name = "Lep1Pt_dilep"; mp->SetPlot(name, "Subleading lepton p_{T} (GeV)", "Events"); SetSystematics(); SavePlot();
  name = "Lep1Eta_dilep"; mp->SetPlot(name, "Subleading lepton #eta", "Events"); SetSystematics(); SavePlot();
  name = "Elec0Pt_dilep"; mp->SetPlot(name, "Leading electron p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Muon0Pt_dilep"; mp->SetPlot(name, "Leading muon p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "InvMass_dilep"; mp->SetPlot(name, "M_{e#mu} (GeV)", "Events"); SetSystematics(); SavePlot();
  name = "MET_dilep"; mp->SetPlot(name, "p_{T}^{miss} (GeV)", "Events"); SetSystematics(); SavePlot();
  name = "MT2_dilep"; mp->SetPlot(name, "M_{T2} (GeV)", "Events"); SetSystematics(); SavePlot();
  name = "NBtagsNJets"; mp->SetPlot(name, "[#Jets|#Btags]", "Events"); SetSystematics(); SavePlot();
  name = "NJets"; mp->SetPlot(name, "Jet multiplicity", "Events"); SetSystematics(); SavePlot();

  name = "NBtags"; mp->SetPlot(name, "b tag multiplicity", "Events"); SetSystematics(); SavePlot();
  name = "MT2_21"; mp->SetPlot(name, "M_{T2} (GeV)", "Events / 5 GeV"); SetSystematics(); SavePlot();
  name = "InvMass"; mp->SetPlot(name, "M_{e#mu} (GeV)", "Events / 20 GeV"); SetSystematics(); SavePlot();
  name = "DilepPt"; mp->SetPlot(name, "p_{T}^{e#mu} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "DeltaPhi"; mp->SetPlot(name, "#Delta#phi(e,#mu) (rad/#pi)", "Events / 0.05"); SetSystematics(); SavePlot();
  name = "DeltaEta"; mp->SetPlot(name, "#Delta#eta(e,#mu)", "Events"); SetSystematics(); SavePlot();
  name = "Lep0Pt"; mp->SetPlot(name, "Leading lepton p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Lep0Eta"; mp->SetPlot(name, "Leading lepton #eta", "Events / 0.1"); SetSystematics(); SavePlot();
  name = "Lep1Pt"; mp->SetPlot(name, "Subleading lepton p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Elec0Pt"; mp->SetPlot(name, "Leading electron p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Muon0Pt"; mp->SetPlot(name, "Leading muon p_{T} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Lep1Eta"; mp->SetPlot(name, "Subleading lepton #eta", "Events / 0.1"); SetSystematics(); SavePlot();
  name = "MET"; mp->SetPlot(name, "p_{T}^{miss} (GeV)", "Events / 10 GeV"); SetSystematics(); SavePlot();
  name = "Jet0Pt"; mp->SetPlot(name, "Leading jet  p_{T} (GeV)", "Events / 20 GeV"); SetSystematics(); SavePlot();
  name = "Jet0Eta"; mp->SetPlot(name, "Leading jet #eta", "Events / 0.1"); SetSystematics(); SavePlot();
  }

}

void SetMultiPlot(){
  mp->SetPath(path);
  mp->SetTreeName(NameOfTree);
  mp->SetPathData(  path + "/Full2016Dataset/");
  mp->SetPathSignal(path + "T2tt/");
  mp->SetPlotFolder(outputFolder);

  weight = "TWeight";
  cut = BaselineCut; chan = "ElMu";
  mp->AddDistribution("MT2",  "TMT2",  cut+ " && TMT2 > 0", chan, 21,    0, 105);

  if(doAllPlots){
  cut = Dilepton; chan = "ElMu";
  mp->AddDistribution("Lep0Pt_dilep",  "TLep0_Pt", cut, chan, 20, 0, 200);
  mp->AddDistribution("Lep1Pt_dilep",  "TLep0_Pt", cut, chan, 15, 0, 150);
  mp->AddDistribution("Lep0Eta_dilep", "TLep0_Eta", cut, chan, 24, -2.4, 2.4);
  mp->AddDistribution("Lep1Eta_dilep", "TLep1_Eta", cut, chan, 24, -2.4, 2.4);
  mp->AddDistribution("Elec0Pt_dilep",  "TLep0_Pt", cut + " && TLep0_Id == 11", chan, 20, 0, 200);
  mp->AddDistribution("Muon0Pt_dilep",  "TLep0_Pt", cut + " && TLep0_Id == 13", chan, 20, 0, 200);
  mp->AddDistribution("InvMass_dilep", "TMll",     cut, chan, 20, 0, 400);
  mp->AddDistribution("MET_dilep", "TMET", cut, chan, 25, 0, 250, 0);
  mp->AddDistribution("MT2_dilep", "TMT2", cut + " && TMT2 > 0", chan, 21, 0, 105);

  mp->AddDistribution("NBtagsNJets",   "NBtagNJets(TNJets,TNBtags)",   cut, chan,  16, 0, 15);
  mp->AddDistribution("NJets",   "TNJets",   cut, chan,  9, 0, 9);

  cut = BaselineCut; chan = "ElMu";
  mp->AddDistribution("MT2_21",  "TMT2",  cut, chan, 21,    0, 105);
  mp->AddDistribution("InvMass", "TMll",     cut, chan, 20, 0, 400);
  mp->AddDistribution("Lep0Pt",  "TLep0_Pt", cut, chan, 20, 0, 200);
  mp->AddDistribution("Lep1Pt",  "TLep1_Pt", cut, chan, 15, 0, 150);
  mp->AddDistribution("Lep0Eta", "TLep0_Eta", cut, chan, 24, -2.4, 2.4);
  mp->AddDistribution("Lep1Eta", "TLep1_Eta", cut, chan, 24, -2.4, 2.4);
  mp->AddDistribution("Elec0Pt",  "TLep0_Pt", cut + " && TLep0_Id == 11", chan, 20, 0, 200);
  mp->AddDistribution("Muon0Pt",  "TLep0_Pt", cut + " && TLep0_Id == 13", chan, 20, 0, 200);
  mp->AddDistribution("NBtags",   "TNBtags",   cut, chan,  5, 0, 4);
  mp->AddDistribution("DilepPt",   "DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",   cut, chan,  25, 0, 250);
  mp->AddDistribution("DeltaPhi", "TDeltaPhi/3.141592",     cut, chan, 20, 0, 1);
  mp->AddDistribution("DeltaEta", "TDeltaEta",     cut, chan, 24, 0, 2.4);
  mp->AddDistribution("MET", "TMET", cut, chan, 25, 0, 250, 0);
  mp->AddDistribution("Count", "TChannel", cut, chan, 1, 0, 10);
  mp->AddDistribution("Jet0Pt",  "TJet_Pt", cut, chan, 15, 0, 300);
  mp->AddDistribution("Jet0Eta", "TJet_Eta", cut, chan, 24, -2.4, 2.4);
  }

  weight = "TWeight";
  if(doFakesFromData) weight = "TWeight*(TEventTruth != 2 && !TIsSS)";
  mp->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics, weight);
	mp->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics, weight);
	mp->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   systematics, weight);
	mp->AddSample("TW, TbarW",                                            "tW",        itBkg, kViolet-5,   systematics, weight); // TColor::GetColor("#ffcc33")
	mp->AddSample("TTbar_Powheg",                                         "t#bar{t}",     itBkg, kRed+1,     systematics, weight);

  if(doFakesFromData) weight = "TWeight*(!TIsSS)";
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData, 1, "", weight);

  if(doFakesFromData){

    mp->SetPath(path + "/SkimSS/Full2016Dataset/"); 
    weight = "TWeight*(TIsSS)";
    mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Nonprompt", itBkg, 1, systematics, weight, "noScaleLumi,noSys"); // Data SS

    mp->SetPath(path + "/SkimSS/");
    weight = "(-1*TWeight)*(TEventTruth != 2)";  // Prompt or flips

    mp->AddSample("TTbar_Powheg",  "Nonprompt",    itBkg, 1, systematics, weight);
    mp->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Nonprompt",  itBkg, 1, systematics, weight);
    mp->AddSample("TW, TbarW, WZ, WW, ZZ, TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",  "Nonprompt", itBkg, 1, systematics, weight);

  }
  else{
    mp->SetPath(path);
    weight = "TWeight*(TIsSS)";
    mp->AddSample("TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics);
  }
  
  mp->SetPath(path);
  weight = "TWeight*(!TIsSS)";
  //mp->AddSample("T2tt_mStop205_mLsp30", "Signal(205,30)", itSignal, kAzure+8, systematics);
  mp->AddSample("T2tt_mStop175_mLsp0", "Signal(175,1)", itSignal, kAzure+8, systematics);

  if(doTTbarSyst){
  mp->SetPath(path);
  weight = "TWeight*(!TIsSS && TEventTruth != 2)";
  // t#bar{t} modeling
  mp->AddSample("TTbar_Powheg_ueUp", "t#bar{t}", itSys, 1, "ueUp");
  mp->AddSample("TTbar_Powheg_ueDown", "t#bar{t}", itSys, 1, "ueDown");
  mp->AddSample("TTbar_Powheg_isrUp", "t#bar{t}", itSys, 1, "isrUp");
  mp->AddSample("TTbar_Powheg_isrDown", "t#bar{t}", itSys, 1, "isrDown");
  mp->AddSample("TTbar_Powheg_hdampUp", "t#bar{t}", itSys, 1, "hdampUp");
  mp->AddSample("TTbar_Powheg_hdampDown", "t#bar{t}", itSys, 1, "hdampDown");

  // FSR
  TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
  mp->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrUp",     "t#bar{t}", itSys, 1, "fsrUp"    );
  mp->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrDown",   "t#bar{t}", itSys, 1, "fsrDown"  );

  //>>> Color Reconnection
  mp->AddSample("TTbar_Powheg_erdON", "t#bar{t}", itSys, 1, "Powheg_erdON");
  mp->AddSample("TTbar_QCDbasedCRTune_erdON", "t#bar{t}", itSys, 1, "QCDbasedCRTune_erdON");
  mp->AddSample("TTbar_GluonMoveCRTune", "t#bar{t}", itSys, 1, "GluonMoveCRTune");
  mp->AddSample("TTbar_GluonMoveCRTune_erdON", "t#bar{t}", itSys, 1, "GluonMoveCRTune_erdON");

  // Top mass
  mp->AddSample("TTbar_Powheg_mtop1715", "t#bar{t}", itSys, 1, "topMassDown");
  mp->AddSample("TTbar_Powheg_mtop1735", "t#bar{t}", itSys, 1, "topMassUp");
  
  // Top pT
  //mp->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  //mp->AddSample("TTbar_Powheg","t#bar{t}", itSys, 1, "TopPtUp");
  //mp->SetWeight("TWeight");
  }

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
  mp->AddNormUnc("t#bar{t}", 0.06);
  mp->AddNormUnc("VV", 0.30);
  mp->AddNormUnc("ttV", 0.30);
  mp->AddNormUnc("DY", 0.15);
  mp->AddNormUnc("tW", 0.30);

  if(doTTbarSyst) mp->UseEnvelope("t#bar{t}", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

  // Fakes
  if(doFakesFromData){
/*
    mp->AddNormUnc("PromptSub_ttbar", 0.06);
    mp->AddNormUnc("PromptSub_DY", 0.15);
    mp->AddNormUnc("PromptSub_Other", 0.30);
    mp->GroupProcesses("PromptSub_ttbar,PromptSub_DY,PromptSub_Other", "Nonprompt");
*/
    mp->ScaleProcess("Nonprompt", transferFactor);
//    mp->ScaleProcess("PromptSub_ttbar", transferFactor);
//    mp->ScaleProcess("PromptSub_DY", transferFactor);
//    mp->ScaleProcess("PromptSub_Other", transferFactor);
  }
  else{
    Float_t fakeSS = 1.5;
    mp->ScaleProcess("Nonprompt", fakeSS);
    mp->AddNormUnc("Nonprompt", 0.50);
  }
  
  // Scale FSR
  if(doTTbarSyst){
  Float_t n  = mp->GetYield("t#bar{t}"); 
  Float_t vu = mp->GetYield("t#bar{t}", "fsrUp");
  Float_t vd = mp->GetYield("t#bar{t}", "fsrDown");
  mp->ScaleSys("t#bar{t}", "fsrUp",   GetFactorFSR(n, vu));
  mp->ScaleSys("t#bar{t}", "fsrDown", GetFactorFSR(n, vd));
  }

  // Group processes
  mp->GroupProcesses("VV", "Others");
  mp->GroupProcesses("ttV", "Others");
  mp->GroupProcesses("DY", "Others");
  mp->GroupProcesses("Nonprompt", "Others");
  mp->SetLineColor("Others", kSpring-2);
}


// InvaMass....................................................
void SavePlot(){
  float ratioscale = 1.4;
  mp->SetCMSmodeLabel("");
  mp->doSignal = true;
  if(name.Contains("NBtagsNJets")) mp->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  else if(name.Contains("eta") || name.Contains("Eta")){
    mp->SetLegendTextSize(0.035);
    mp->SetLegendPosition(0.77, 0.70, 0.90, 0.90);
  }
  else if(name.Contains("DeltaPhi")){
    mp->SetLegendTextSize(0.045);
    mp->SetLegendPosition(0.3, 0.65, 0.55, 0.93);
  }
  else if(name.Contains("MET") || name.Contains("Lep0Pt") || name.Contains("Lep1Pt")){
    mp->SetLegendTextSize(0.065);
    mp->SetLegendPosition(0.65, 0.40, 0.9, 0.90);
  }
  else if(name.Contains("MT2")){
    mp->SetLegendTextSize(0.055);
    mp->SetLegendPosition(0.7, 0.65, 0.9, 0.9);
    //mp->SetNColumns(2);
   // mp->SetPlotMinimum(50);
   // mp->SetPlotMaximum(1.5e5);
    ratioscale = 1.4;
    mp->doSignal = true;
    mp->SetSignalStyle("H");
  }
  //if(name.Contains("DeltaEta")) ratioscale = 1.2;

  mp->SetRatioMin(2-ratioscale); mp->SetRatioMax(ratioscale);
  mp->SetOutputName(name);
  mp->doSetLogy = false;
  mp->DrawStack();
//  mp->doSetLogy = true;
//  mp->SetOutputName(name+ "_log");
//  mp->DrawStack();
}

void SaveSignalPlot(){
  cout << "Setting plot..." << endl;
  mp->doSignal= true; mp->doData = true; 
  mp->doUncInLegend = false;
  mp->SetCMSmodeLabel("");

  cout << "Setting plot options..." << endl;
  mp->SetLegendPosition(0.78, 0.75, 0.9, 0.9);
  mp->SetPlotMinimum(50);
  mp->SetPlotMaximum(1.5e5);
//  mp->SetRatioMin(0.0); mp->SetRatioMax(ratiomax);
  mp->SetOutputName("Signal_"+name);
  mp->SetSignalProcess("Signal(205,30)");
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

