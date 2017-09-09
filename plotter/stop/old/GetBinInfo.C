R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0,  Float_t binN, TString Xtitle, TString name = "");
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t *bin0, TString Xtitle, TString name = "");
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/TopLikeObjects/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/ForAN_may31/";
TString NameOfTree = "tree";
TString BaselineCut = "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";

void GetBinInfo(){
  Float_t b[] = {0, 100, 200}; Int_t nb = 2;
Float_t b1[] = {0,50,100,200}; Int_t nb1 = 3;
Float_t b2[] = {0,20,40,60,80,100,200}; Int_t nb2 = 6;
Float_t b3[] = {0,20,40,60,80,100,150,200}; Int_t nb3 = 7;
Float_t b4[] = {0,20,40,60,80,100,120,140,160,200}; Int_t nb4 = 9;
Float_t b5[] = {0,20,40,60,80,100,120,140,160,180,200}; Int_t nb5 = 10;
Float_t b6[] = {0,20,40,60,80,100,110,120,130,140,160,180,200}; Int_t nb6 = 12;
//  DrawPlot("TChannel", BaselineCut, "ElMu", 1, 0, 4, "Channel", "CutAndCount");
//  DrawPlot("TChannel", BaselineCut, "Muon", 1, 0, 4, "Channel", "CutAndCount");
////  DrawPlot("TChannel", BaselineCut, "Elec", 1, 0, 4, "Channel", "CutAndCount");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2_no0");
  //DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", 2, b, "M_{T2} [GeV]", "MT2_no0");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb1, b1, "M_{T2} [GeV]", "MT2_no0_1");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb2, b2, "M_{T2} [GeV]", "MT2_no0_2");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb3, b3, "M_{T2} [GeV]", "MT2_no0_3");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb4, b4, "M_{T2} [GeV]", "MT2_no0_4");
  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb5, b5, "M_{T2} [GeV]", "MT2_no0_5");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb6, b6, "M_{T2} [GeV]", "MT2_no0_6");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TMT2", BaselineCut, "Muon", 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TMT2", BaselineCut, "Elec", 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TDeltaPhi", BaselineCut, "Muon", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TDeltaPhi", BaselineCut, "Elec", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TMT2", "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 < 140", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "TMT2low"); //  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
 // DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]");
//  DrawPlot("TDeltaEta", BaselineCut, "ElMu", 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]");
  //DrawPlot("TChannel", BaselineCut, "All", 3, 1, 4, "Channel", "Comb3chan");
  //DrawPlot("TNJets", BaselineCut, "ElMu", 1, 0, 10, "CutAndCount","CutAndCount");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
  //DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/FullDataset/");
  p->verbose = true;
  //p->doData = false;
  if(name != "") p->SetVarName(name);

  // Main samples
  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
  p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
	p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1);
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);
/*
  // ttbar systematics
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown"); 
  p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp"); 
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfDown"); 

  // FastSim inclusive grid
  p->AddSample("T2tt_mStop175_mLsp1", "S_175_1", itSignal, kCyan); 
  p->AddSample("T2tt_mStop183_mLsp1", "S_183_1", itSignal, kCyan); 
  p->AddSample("T2tt_mStop192_mLsp25", "S_192_25", itSignal, kCyan); 
  p->AddSample("T2tt_mStop200_mLsp25", "S_200_25", itSignal, kCyan); 
  p->AddSample("T2tt_mStop208_mLsp25", "S_208_25", itSignal, kCyan); 
  p->AddSample("T2tt_mStop217_mLsp50", "S_217_50", itSignal, kCyan); 
  p->AddSample("T2tt_mStop225_mLsp50", "S_225_50", itSignal, kCyan); 
  p->AddSample("T2tt_mStop242_mLsp75", "S_242_75", itSignal, kCyan); 
  p->AddSample("T2tt_mStop250_mLsp75", "S_250_75", itSignal, kCyan); 

  // FullSim samples
  p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan); 
*/  p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink); 
  p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1); 
/*
  // FastSim Dileptonic grid
  p->AddSample("T2ttD_mStop160_mLsp1" , "SD_160_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop160_mLsp10", "SD_160_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop160_mLsp20", "SD_160_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop170_mLsp1" , "SD_170_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop170_mLsp10", "SD_170_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop170_mLsp20", "SD_170_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop175_mLsp1" , "SD_175_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop175_mLsp10", "SD_175_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop175_mLsp20", "SD_175_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop180_mLsp1" , "SD_180_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop180_mLsp10", "SD_180_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop180_mLsp20", "SD_180_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop190_mLsp1" , "SD_190_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop190_mLsp10", "SD_190_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop190_mLsp20", "SD_190_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop200_mLsp1" , "SD_200_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop200_mLsp10", "SD_200_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop200_mLsp20", "SD_200_20", itSignal, 1); 

  p->AddSample("T2ttD_mStop210_mLsp1" , "SD_210_1",  itSignal, 1); 
  p->AddSample("T2ttD_mStop210_mLsp10", "SD_210_10", itSignal, 1); 
  p->AddSample("T2ttD_mStop210_mLsp20", "SD_210_20", itSignal, 1); 

  p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
*/ // p->AddSystematic("Btag,MisTag,PU");
  Histo* h = p->GetHisto("SFS_250_50");
  Float_t nEntries = 0;
  Float_t relError = 0;
  Float_t Integral = h->Integral();
  Int_t nTotalEntries = h->GetEntries();
  for(Int_t i = 0; i <= (Int_t) h->GetNbinsX(); i++){
    relError = h->GetBinError(i)/h->GetBinContent(i);
    nEntries = h->GetBinContent(i)/Integral*nTotalEntries;
    cout << "bin = " << i << ", CentralValue = " << h->GetBinCenter(i) << ", nEntries = " << nEntries << ", relError = " << relError << endl;
  }


  //p->PrintSystYields();
  //p->PrintYields("","","","tex,html,txt");
  //p->MakeAllDatacards();
  delete p;
}

