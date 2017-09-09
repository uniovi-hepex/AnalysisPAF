R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t *bin0, TString Xtitle, TString name = "");
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Dilepton/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/Baseline/";
TString NameOfTree = "tree";
TString BaselineCut = "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";
TString SScut = "TMT2 > 0 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps < 3";
TString ThirdLep = "TMT2 > 0 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps > 2";
TString Dilepton    = "!TIsSS && TNVetoLeps < 3";
TString VarNBtagsNJets = "TNBtags + (TNJets == 1) + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";

// Baseline

Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/ForAN_may31/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/jun26/";
void DrawPlots(TString chan = "ElMu"){
//  DrawPlot("TMT2", BaselineCut, "ElMu", ngbins, gbins, "M_{T2} [GeV]", "MT2");
  //DrawPlot("TChannel", BaselineCut + " && TMT2 > 100", chan, 1, 0, 10, "Events", "Events_MT2g100");
  DrawPlot("TChannel", BaselineCut, chan, 1, 0, 10, "Events", "Events");
//  DrawPlot("TMT2", BaselineCut, chan, 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, 0, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi_bl");
//  DrawPlot("TDeltaEta", BaselineCut, chan, 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]", "DeltaEta_bl");
//  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", chan, 40, 0, 400, "MET [GeV]", "MET_bl");
//  DrawPlot("fabs(TLep0_Eta - TLep1_Eta)", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi_bl");
//  DrawPlot("TLep0_Eta", BaselineCut, chan, 20, -2.4, 2.4, "Leading lepton #eta [rad]", "Lep0Eta_bl");
//  DrawPlot("TLep1_Eta", BaselineCut, chan, 20, -2.4, 2.4, "Subleading lepton #eta [rad]", "Lep1Eta_bl");
//  DrawPlot("TLep0_Pt", BaselineCut, chan, 25, 25, 250, "Leading lepton p_{T} [GeV]", "Lep0Pt_bl");
//  DrawPlot("TLep1_Pt", BaselineCut, chan, 30, 0, 200, "Subleading lepton p_T [GeV]", "Lep1Pt_bl");
//  DrawPlot("TJet_Eta[0]", BaselineCut, chan, 20, -2.4, 2.4, "Leading jet #eta [rad]", "Jet0Eta_bl");
//  DrawPlot("TJet_Eta[1]", BaselineCut, chan, 20, -2.4, 2.4, "Subleading jet #eta [rad]", "Jet1Eta_bl");
//  DrawPlot("TJet_Pt[0]", BaselineCut, chan, 27, 30, 300, "Leading jet p_T [GeV]", "Jet0Pt_bl");
//  DrawPlot("TJet_Pt[1]", BaselineCut, chan, 30, 0, 300, "Subleading jet p_T [GeV]", "Jet1Pt_bl");
//  DrawPlot("TMll", BaselineCut, chan, 40, 0, 600, "M_{e#mu} [GeV]", "InvMass_bl");
}


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/FullDataset/");
  p->verbose = false;
  if(name != "") p->SetVarName(name);

  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
	p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
	p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi", "Nonprompt", itBkg, kGray+1, "0");
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, "0");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);
  p->AddSample("T2tt_175_1_FS", "SFS_175_1", itSignal, kGreen+1);
  p->AddSample("T2tt_mStop175_mLsp1", "T2tt_175_1", itSignal, kGreen+1);
  p->AddSample("T2tt_mStop225_mLsp50", "T2tt_225_50", itSignal, kAzure-2);
  p->AddSample("T2tt_200_50_FS_summer", "S_200_50", itSignal, kBlack, "0"); 
  p->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kBlack, "0"); 
  p->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kBlack, "0"); 

  /*Plot* isr = new Plot(var, "TISRweight*(" + cut + ")", chan, nbins, bin0, binN, "Title", Xtitle);
  isr->SetPath(pathToTree); isr->SetTreeName(NameOfTree); isr->SetPathSignal(pathToTree + "T2tt_noSkim/");
  isr->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kPink-1, 0.1); 
  isr->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kGreen+2, 0.1); 
  Histo* S_225_50 = isr->GetHisto("S_225_50");
  Histo* S_250_50 = isr->GetHisto("S_250_50");
  p->AddToHistos(S_225_50);
  p->AddToHistos(S_250_50);
*/
 
  //p->SetSignalProcess("T2tt_175_1");
 
  SSCR* a = new SSCR();
  Float_t fakeSS = a->GetFakeSF();
  p->ScaleProcess("Nonprompt", fakeSS);

  p->SetSignalStyle("SM");
  p->doData = true;


  p->SetRatioMin(0.0);
  p->SetRatioMax(0.1);
  p->SetRatioOptions("S/B");
  p->doData = false;
  p->SetPlotFolder("~/www/stopJun2017/Results17jul/");


  //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
  p->AddSystematic("stat");
  //p->AddSystematic("JER", "ttbar");
  //p->AddSystematic("MET", "T2tt_175_1, T2tt_250_75");

  p->SetYieldsTableName("StopYields"); p->PrintYields("", "", "", "tex, txt, html");
  //p->PrintYields();
	//p->PrintSystYields();

//  p->doSetLogy = false;
//  p->DrawStack("SignalRegion", 1);
//  p->doSetLogy = true;
//  p->DrawStack("SignalRegion_log", 1);

  delete p;
}


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  //p->SetPathData(  pathToTree + "/DataBD_10invfb/"); p->SetLumi(10.0);
  p->SetPathData(  pathToTree + "/FullDataset/");
  p->verbose = true;
  if(name != "") p->SetVarName(name);

  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
  //p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kGray, 0.5);
  //p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray, 0.5);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
	p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
	p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi", "Fakes", itBkg, kGray+1, "0");
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, "0");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);
  //p->AddSample("T2tt_mStop175_mLsp1", "T2tt_175_1", itSignal, kGreen+1, 0.1, "0", "isrweight"); 
 // p->AddSample("T2tt_mStop250_mLsp75", "T2tt_250_75", itSignal, kAzure-2, 0.1, "0", "isrweight"); 
//  p->AddSample("T2tt_200_50_FS_summer", "S_200_50", itSignal, kPink-1,  0., "0"); 
  p->AddSample("T2tt_200_50_FS_summer", "FullSim 200_50", itSignal);
  p->AddSample("T2tt_mStop200_mLsp50",  "FastSim 200_50", itSignal);
  p->AddSample("T2tt_mStop200_mLsp50_nopu",  "FastSim 200_50 noPU", itSignal);
  p->AddSample("T2tt_225_50_FS_summer", "FullSim 225_50", itSignal);
  p->AddSample("T2tt_mStop225_mLsp50",  "FastSim 225_50", itSignal);
  p->AddSample("T2tt_mStop225_mLsp50_nopu",  "FastSim 225_50 noPU", itSignal);
  p->AddSample("T2tt_250_50_FS_summer", "FullSim 250_50", itSignal);
  p->AddSample("T2tt_mStop250_mLsp50",  "FastSim 250_50", itSignal);
  p->AddSample("T2tt_mStop250_mLsp50_nopu",  "FastSim 250_50 noPU", itSignal);
//  p->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kPink-1,  0.1, "0"); 
//  p->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kGreen+2, 0.1, "0"); 

  /*Plot* isr = new Plot(var, "TISRweight*(" + cut + ")", chan, nbins, bin0, binN, "Title", Xtitle);
  isr->SetPath(pathToTree); isr->SetTreeName(NameOfTree); isr->SetPathSignal(pathToTree + "T2tt_noSkim/");
  isr->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kPink-1, 0.1); 
  isr->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kGreen+2, 0.1); 
  Histo* S_225_50 = isr->GetHisto("S_225_50");
  Histo* S_250_50 = isr->GetHisto("S_250_50");
  p->AddToHistos(S_225_50);
  p->AddToHistos(S_250_50);
*/
 
  //p->SetSignalProcess("T2tt_175_1");
  p->SetSignalStyle("SM");

  p->doData = true;
//  p->SetRatioMin(0.0);
//  p->SetRatioMax(0.1);
//  p->SetRatioOptions("S/B");
  p->SetPlotFolder("~/www/stopJun2017/ControlPlots/");


  //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
  p->AddSystematic("stat");

  p->SetYieldsTableName("StopYields"); p->PrintYields("", "", "", "tex, txt, html");
  //p->PrintYields();
	//p->PrintSystYields();

//  p->doSetLogy = false;
//  p->DrawStack("ControlPlot", 1);
//  p->doSetLogy = true;
//  p->DrawStack("ControlPlot_log", 1);

  delete p;
}

