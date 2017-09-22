R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"

// TODO:
// --> Btag SF on weight (TWeight*TBTagSF) for Fast sim signals

//============================================================== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep21/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";

TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TNVetoLeps < 3 && TPassTrigger && TPassMETfilters";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
TString FastSimSignals = "S_175_1, S_183_1, S_192_25, S_200_25, S_208_25, S_217_50, S_225_50, S_242_75, S_250_75, SD_160_1, SD_160_20, SD_170_1, SD_170_10, SD_170_20, SD_175_1, SD_175_10, SD_175_20, SD_180_1, SD_180_10, SD_180_20, SD_190_1, SD_190_10, SD_190_20, SD_200_1, SD_200_10, SD_200_20, SD_210_1, SD_210_10, SD_210_20";
TString FullSimSignals = "SFS_175_1, SFS_200_50, SFS_225_50, SFS_250_50";

Plot   *p = nullptr;
PDFunc *pdf = nullptr;


//============================================================== Functions
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString options = "");
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name, TString options = "");
void AddSamples(TString options = "");
void SaveFile();
void SetPaths();
void ScaleFakes();


//============================================================== SaveFiles
void prueba(TString options = ""){
    CreatePlot("TChannel", BaselineCut, "ElMu", 1, 0, 10,      "Yields",       "CutAndCount", options);
    //CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins, gbins, "M_{T2} [GeV]", "MT2"        , options);
}



//============================================================================================
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString options){
  cout << "Creating plot.." << endl;
  p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  cout << "Setting verbose to true.. " << endl;
  p->verbose = true;
  cout << "Creating PDFunc object..." << endl;
  pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN);
  cout << "Setting paths... " << endl;
  SetPaths();
  AddSamples(options);
  ScaleFakes(); 
  if(name != "") p->SetVarName(name);
  p->doData = false;
  p->PrintSystYields();
  p->SaveHistograms();
  delete p;
}

void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name, TString options){
  p = new Plot(var, cut, chan, nbins, bin0, "Title", Xtitle);
  p->verbose = true;
  pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0);
  SetPaths();
  AddSamples(options);
  ScaleFakes(); 
  if(name != "") p->SetVarName(name);
  p->doData = false;
  p->PrintSystYields();
  p->SaveHistograms();
  delete p;
}

void AddSamples(TString options){
  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
  // Bkgs and data
  p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
  p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi", "Nonprompt", itBkg, kGray);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
  p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
  p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1);
  //p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);
/*  p->doSys = false;
  p->GetStack();
  Histo* hall = (Histo*) (p->hAllBkg->Clone());
  hall->SetProcess("Data"); hall->SetTag("Data");
  hall->SetType(itData); hall->SetColor(1); hall->SetStyle();
  p->AddToHistos(hall);
*/

  // ttbar systematics
  if(options.Contains("ttbar")){
    p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
    p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
    p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
    p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");

    // Color Reconnection... FSR?
    p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
    p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

    // PDF and scale using LHE weights...
    Histo* hPDFUp   =  pdf->GetSystHisto("up",   "pdf")->CloneHisto();
    Histo* hPDFDown =  pdf->GetSystHisto("down", "pdf")->CloneHisto();
    Histo* hMEUp    =  pdf->GetSystHisto("up",   "ME")->CloneHisto();
    Histo* hMEDown  =  pdf->GetSystHisto("down", "ME")->CloneHisto();
    hPDFUp->SetProcess("ttbar"); hPDFDown->SetProcess("ttbar"); hMEUp->SetProcess("ttbar"); hMEDown->SetProcess("ttbar");
    hPDFUp->SetSysTag("PDFUp"); hPDFDown->SetSysTag("PDFDown"); hMEUp->SetSysTag("MEUp");  hMEDown->SetSysTag("MEDown");
    hPDFUp->SetTag("ttbar_PDFUp"); hPDFDown->SetTag("ttbar_PDFDown"); hMEUp->SetTag("ttbar_MEUp"); hMEDown->SetTag("ttbar_MEDown");
    hPDFUp->SetName("ttbar_PDFUp"); hPDFDown->SetName("ttbar_PDFDown"); hMEUp->SetName("ttbar_MEUp"); hMEDown->SetName("ttbar_MEDown");
    hPDFUp->SetType(itSys); hPDFDown->SetType(itSys); hMEUp->SetType(itSys); hMEDown->SetType(itSys);
    hPDFUp->SetStyle(); hPDFDown->SetStyle(); hMEUp->SetStyle(); hMEDown->SetStyle();
    p->AddToHistos(hPDFUp); p->AddToHistos(hPDFDown); p->AddToHistos(hMEUp); p->AddToHistos(hMEDown);
  }
  // FulSim samples
  if(options.Contains("FullSim")){
    p->AddSample("T2tt_175_1_FS",         "SFS_175_1",  itSignal, kCyan);
    p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan);
    p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink);
    p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1);
  }

  // Full Sim xqcut = 20
  if(options.Contains("xqcut")){
    //p->AddSample("T2tt_175_1_FS_xqcut20",  "SFS_175_1_xqcut20",  itSignal, kCyan); 
    p->AddSample("T2tt_200_50_FS_xqcut20", "SFS_200_50_xqcut20", itSignal, kCyan); 
    p->AddSample("T2tt_225_50_FS_xqcut20", "SFS_225_50_xqcut20", itSignal, kPink); 
    p->AddSample("T2tt_250_50_FS_xqcut20", "SFS_250_50_xqcut20", itSignal, 1); 
  }

  // FastSim Inclusive grid
  if(options.Contains("incl")){
    p->SetWeight("TWeight*TBtagSFFS");
    //p->AddSample("T2tt_mStop175_mLsp1", "S_175_1", itSignal, kCyan); // In the dileptonic samples
    p->AddSample("T2tt_mStop183_mLsp1", "S_183_1", itSignal, kCyan); 
    p->AddSample("T2tt_mStop192_mLsp25", "S_192_25", itSignal, kCyan); 
    p->AddSample("T2tt_mStop200_mLsp25", "S_200_25", itSignal, kCyan); 
    p->AddSample("T2tt_mStop208_mLsp25", "S_208_25", itSignal, kCyan); 
    p->AddSample("T2tt_mStop200_mLsp50", "S_200_50", itSignal, kCyan); 
    p->AddSample("T2tt_mStop217_mLsp50", "S_217_50", itSignal, kCyan); 
    p->AddSample("T2tt_mStop225_mLsp50", "S_225_50", itSignal, kCyan); 
    p->AddSample("T2tt_mStop233_mLsp50", "S_233_50", itSignal, kCyan); 
    p->AddSample("T2tt_mStop250_mLsp50", "S_250_50", itSignal, kCyan); 
    p->AddSample("T2tt_mStop242_mLsp75", "S_242_75", itSignal, kCyan); 
    p->AddSample("T2tt_mStop250_mLsp75", "S_250_75", itSignal, kCyan); 
    p->SetWeight("TWeight");
  }

  // FastSim Dileptonic grid
  if(options.Contains("dilep")){
    p->SetWeight("TWeight*TBtagSFFS");
    p->AddSample("T2tt_mStop160_mLsp1" , "SD_160_1",  itSignal, 1); 
    //p->AddSample("T2tt_mStop160_mLsp10", "SD_160_10", itSignal, 1); 
    p->AddSample("T2tt_mStop160_mLsp20", "SD_160_20", itSignal, 1); 
    p->AddSample("T2tt_mStop170_mLsp1" , "SD_170_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop170_mLsp10", "SD_170_10", itSignal, 1); 
    p->AddSample("T2tt_mStop170_mLsp20", "SD_170_20", itSignal, 1); 
    p->AddSample("T2tt_mStop175_mLsp1" , "SD_175_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop175_mLsp10", "SD_175_10", itSignal, 1); 
    p->AddSample("T2tt_mStop175_mLsp20", "SD_175_20", itSignal, 1); 
    p->AddSample("T2tt_mStop180_mLsp1" , "SD_180_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop180_mLsp10", "SD_180_10", itSignal, 1); 
    p->AddSample("T2tt_mStop180_mLsp20", "SD_180_20", itSignal, 1); 
    p->AddSample("T2tt_mStop190_mLsp1" , "SD_190_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop190_mLsp10", "SD_190_10", itSignal, 1); 
    p->AddSample("T2tt_mStop190_mLsp20", "SD_190_20", itSignal, 1); 
    p->AddSample("T2tt_mStop200_mLsp1" , "SD_200_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop200_mLsp10", "SD_200_10", itSignal, 1); 
    p->AddSample("T2tt_mStop200_mLsp20", "SD_200_20", itSignal, 1); 
    p->AddSample("T2tt_mStop210_mLsp1" , "SD_210_1",  itSignal, 1); 
    p->AddSample("T2tt_mStop210_mLsp20" ,"SD_210_20", itSignal, 1); 
    //p->AddSample("T2tt_mStop210_mLsp10", "SD_210_10", itSignal, 1); 
    p->AddSample("T2tt_mStop210_mLsp20", "SD_210_20", itSignal, 1); 
    p->SetWeight("TWeight");
  }

  if(options.Contains("AllSyst")){
    p->AddSystematic("Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU");
    p->AddSystematic("JER", "ttbar,"+FastSimSignals+","+FullSimSignals);
    p->AddSystematic("MET", FastSimSignals);
  }
    p->AddSystematic("ElecEff,MuonEff");

}

void SaveFile(){
}

void SetPaths(){
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = true;
  p->doData = false;
  p->SetLimitFolder("prueba/");
}


void ScaleFakes(){
  SSCR *ss = new SSCR();
  Float_t fakeSF = ss->GetFakeSF();
  p->ScaleProcess("Nonprompt", fakeSF);
}













/*
  Float_t b[] = {0, 100, 200}; Int_t nb = 2;
  Float_t b1[] = {0,50,100,200}; Int_t nb1 = 3;
  Float_t b2[] = {0,20,40,60,80,100,200}; Int_t nb2 = 6;
  Float_t b3[] = {0,20,40,60,80,100,150,200}; Int_t nb3 = 7;
  Float_t b4[] = {0,20,40,60,80,100,120,140,160,200}; Int_t nb4 = 9;
  Float_t b5[] = {0,20,40,60,80,100,120,140,160,180,200}; Int_t nb5 = 10;
  Float_t b6[] = {0,20,40,60,80,100,110,120,130,140,160,180,200}; Int_t nb6 = 12;
  Float_t mbins[] = {0,20,40,60,80,100,105,110,115,120,140,200}; Int_t nmbins = 11;
*/
/*  DrawPlot("TChannel", BaselineCut + " && TMT2 > 70", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g70");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 80", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g80");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 90", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g90");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 100", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g100");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 110", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g110");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 120", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g120");
  DrawPlot("TChannel", BaselineCut + " && TMT2 > 130", "ElMu", 1, 0, 4, "Channel", "CutAndCount_MT2g130");*/
//  DrawPlot("TChannel", BaselineCut, "Muon", 1, 0, 4, "Channel", "CutAndCount");
////  DrawPlot("TChannel", BaselineCut, "Elec", 1, 0, 4, "Channel", "CutAndCount");
 // DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2");
  //DrawPlot("TMT2", BaselineCut + " && TMT2 < 120", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2_leq120");
  //DrawPlot("TMT2", BaselineCut + " && TMT2 < 140", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2_leq140");
//  DrawPlot("TMT2", BaselineCut, "ElMu", nmbins, mbins, "M_{T2} [GeV]", "MT2_11Bins");
//  DrawPlot("TMT2", BaselineCut, "ElMu", ngbins, gbins, "M_{T2} [GeV]", "MT2_26Bins");
  //DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", 2, b, "M_{T2} [GeV]", "MT2_no0");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb1, b1, "M_{T2} [GeV]", "MT2_no0_1");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb2, b2, "M_{T2} [GeV]", "MT2_no0_2");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb3, b3, "M_{T2} [GeV]", "MT2_no0_3");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb4, b4, "M_{T2} [GeV]", "MT2_no0_4");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb5, b5, "M_{T2} [GeV]", "MT2_no0_5");
//  DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", "ElMu", nb6, b6, "M_{T2} [GeV]", "MT2_no0_6");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TMT2", BaselineCut, "Muon", 40, 0, 200, "M_{T2} [GeV]", "MT2");
//  DrawPlot("TMT2", BaselineCut, "Elec", 40, 0, 200, "M_{T2} [GeV]", "MT2");
  //DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TDeltaPhi", BaselineCut, "Muon", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TDeltaPhi", BaselineCut, "Elec", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi");
//  DrawPlot("TMT2", "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 < 140", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "TMT2low"); //  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
 // DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]");
//  DrawPlot("TDeltaEta", BaselineCut, "ElMu", 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]");
  //DrawPlot("TChannel", BaselineCut, "All", 3, 1, 4, "Channel", "Comb3chan");
  //DrawPlot("TNJets", BaselineCut, "ElMu", 1, 0, 10, "CutAndCount","CutAndCount");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
  //DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
