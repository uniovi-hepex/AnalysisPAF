//##################################################################################################################
//
// Crate a .root files with all needed histograms to calculate limits
//
//    root -l -b -q 'SaveModTTbar.C("")'  
//
//##################################################################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"

//============================================================== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
TString FastSimSignals = "S_175_1, S_183_1, S_192_25, S_200_25, S_208_25, S_217_50, S_225_50, S_242_75, S_250_75, SD_160_1, SD_160_10, SD_160_20, SD_170_1, SD_170_10, SD_170_20, SD_175_1, SD_175_10, SD_175_20, SD_180_1, SD_180_10, SD_180_20, SD_190_1, SD_190_10, SD_190_20, SD_200_1, SD_200_10, SD_200_20, SD_210_1, SD_210_10, SD_210_20";
TString FullSimSignals = "SFS_175_1, SFS_200_50, SFS_225_50, SFS_250_50";
Plot   *p = nullptr;
PDFunc *pdf = nullptr;

//============================================================== Selection
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters"; // && TNVetoLeps < 3 

//============================================================== Functions
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name);
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name);
void SetPaths();
void AddSamples();
void ScaleFakes();
void AddScaledTTbar(TString syst = "");


//============================================================== SaveModTTbar (Main function)
void SaveModTTbar(){
    CreatePlot("TChannel", BaselineCut, "ElMu", 1, 0, 10,      "Yields",       "CutAndCount");
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins, gbins, "M_{T2} [GeV]", "MT2");
}



//=============================================================== Other options
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->verbose = true;
  pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN);
  SetPaths();
  AddSamples();
  ScaleFakes(); 
  if(name != "") p->SetVarName(name);
  p->doData = true;
  p->PrintSystYields();
  p->SaveHistograms();
  delete p;
}

void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bin0, TString Xtitle, TString name){
  p = new Plot(var, cut, chan, nbins, bin0, "Title", Xtitle);
  p->verbose = true;
  pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0);
  SetPaths();
  AddSamples();
  ScaleFakes(); 
  if(name != "") p->SetVarName(name);
  p->doData = true;
  p->PrintSystYields();
  p->SaveHistograms();
  delete p;
}

void AddSamples(){
  //>>> Bkgs and data
  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
  p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
  p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG", "Nonprompt", itBkg, kGray);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
  p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
  p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1);
  p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);

  //>>> ttbar systematics
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
  p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");
  //----------> Missing FSR unc.
  //p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, "fsrUp");
  //p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, "fsrDown");

  //----------> Missing unc on top pt????, b-fragmentation

  //>>> Color Reconnection
  p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
  p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
  p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
  p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
  p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

  //>>> PDF and scale using LHE weights...
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
  
  //>>> Set systematics
  TString syst = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU";
  p->AddSystematic(syst);

  //>>> Scaled ttbar as signal
  AddScaledTTbar("0");
  AddScaledTTbar(syst);

}

void SetPaths(){
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = true;
  p->doData = false;
  p->SetLimitFolder("output/Datacards/ttbarSignal/");
}

void ScaleFakes(){
  SSCR *ss = new SSCR();
  Float_t fakeSF = ss->GetFakeSF();
  p->ScaleProcess("Nonprompt", fakeSF);
}


void AddScaledTTbar(TString syst){
  if(syst.Contains(",")){
    vector<TString> vs = TStringToVector(syst);
    Int_t ns = vs.size();
    for(Int_t i = 0; i < ns; i++){
      AddScaledTTbar(vs.at(i));
    }
    return;
  }
  Histo* stt;
  if(syst == "" || syst == "0"){
    stt = p->GetHisto("ttbar")->CloneHisto("ttbar_scaled"); 
    stt->SetTag("ttbar_scaled"); stt->SetName("ttbar_scaled");
  }
  else{
    stt = p->GetHisto("ttbar", syst)->CloneHisto("ttbar_scaled_" + syst); 
    stt->SetTag("ttbar scale_" + syst); stt->SetName("ttbar_scaled_" + syst);
  }
  stt->SetProcess("ttbar_scaled"); 
  stt->Scale(36.3818/831.8); 
  stt->SetDirectory(0);
  stt->SetType(itSignal); stt->SetStyle();
  p->AddToHistos(stt);
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
