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
TString path = "/pool/ciencias/userstorage/juanr/stop/sep22/";
TString NameOfTree = "tree";
TString outputFolder = "./output/StopPlotsData/";

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins = 21;

//=== Systematics
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU";

void SavePlot(TString name);
void SetSystematics();
void SetMultiPlot();

MultiPlot* mp = new MultiPlot();

void SaveBinByBin(){
  SetMultiPlot();
  TString bintag;
  for(Int_t ibin = 0; ibin < ngbins; ibin++){
    bintag = Form("MT2g%1.0fb%1.0f", gbins[ibin], gbins[ibin+1]);
    SavePlot("MT2_" + bintag);
  }
}

void SetMultiPlot(){
  mp->SetPath(path);
  mp->SetTreeName(NameOfTree);
  mp->SetPathData(  path + "/Full2016Dataset/");
  mp->SetPathSignal(path + "T2tt/");
  mp->SetPlotFolder("./test/");
  mp->SetLimitFolder(outputFolder);

  TString chan = "ElMu";
  TString weight = "TWeight";
  TString cut = BaselineCut; 
  TString var = "TMT2";
  TString bintag;
  
  TString MT2cut = "";
  for(Int_t ibin = 0; ibin < ngbins; ibin++){
    MT2cut = Form(" && (TMT2 >= %f && TMT2 < %f)", gbins[ibin], gbins[ibin+1]);
    bintag = Form("MT2g%1.0fb%1.0f", gbins[ibin], gbins[ibin+1]);
    mp->AddDistribution("MT2_" + bintag, "TMT2", cut + MT2cut, chan, 1, 0, 200);
  }

  mp->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics, weight);
	mp->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics, weight);
	mp->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   systematics, weight);
	mp->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   systematics, weight);
	mp->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics, weight);
	mp->AddSample("TTbar_Powheg",                                         "ttbar",  itBkg, kRed+1,     systematics + ", JER", weight);
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  mp->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan, systematics + ", JER", weight);
  mp->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink, systematics + ", JER", weight);
  mp->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1,     systematics + ", JER", weight);
    
  // TopPt
  mp->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  mp->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtUp");
  mp->SetWeight("TWeight");
  mp->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtDown");

  // ttbar modeling
  mp->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
  mp->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
  mp->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
  mp->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
  mp->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
  mp->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

  //>>> Color Reconnection
  mp->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
  mp->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
  mp->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
  mp->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");

  // Top mass
  mp->AddSample("TTbar_Powheg_mtop1715", "ttbar", itSys, 1, "topMassDown");
  mp->AddSample("TTbar_Powheg_mtop1735", "ttbar", itSys, 1, "topMassUp");

  // FSR
  TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
  mp->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
  mp->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
  mp->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );
  mp->SetWeight("TWeight");
  
  // Plotting options  
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
void SavePlot(TString name){
  mp->SetPlot(name);
  SetSystematics();

  mp->SetOutputName(name);
  mp->SaveHistograms();
}

