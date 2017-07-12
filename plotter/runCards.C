#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
// R__LOAD_LIBRARY(Datacard.C)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);

TString pathToTree = "/pool/ciencias/userstorage/sscruz/TW/jun11/";

TString NameOfTree = "MiniTree";


void runCards(){
  // ACORDARSE DE PONER TOOOOODAS LAS VARIABLES SEPARADAS
  // DrawPlot("ShapVarWithNJets( TNJets , TNBtags , nLooseCentral )", "(TIsSS == 0)", "ElMu", 5, -0.5,4.5, "FitWithNJets");

  //  DrawPlot("ShapVarWithLoosePt( TNJets , TNBtags , TJetLoosept )", "(TIsSS == 0)", "ElMu", 10, -0.5,9.5, "FitWithPt");
  // DrawPlot("ShapVar( TNJets , TNBtags )", "(TIsSS == 0)", "ElMu", 10, -0.5,2.5, "SimpleFit");
  
  DrawPlot("ShapVarWithBDtgrad( TNJets , TNBtags , TBDTgrad )", "(TIsSS == 0)", "ElMu", 12, -0.5,11.5, "BDTgrad");
  // DrawPlot("ShapVarWithBDTada( TNJets , TNBtags , TBDTada )"  , "(TIsSS == 0)", "ElMu", 10, -0.5,9.5, "BDTada");
  
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);

  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose = false;
  // p->SetVarName("forFit");

  p->AddSample("WZ", "VV", itBkg, kYellow-10); // 0.50
  p->AddSample("WW", "VV", itBkg);
  p->AddSample("ZZ", "VV", itBkg);
  p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3);//, 0.5);
  p->AddSample("TTWToQQ", "ttV", itBkg);
  p->AddSample("TTZToQQ", "ttV", itBkg);
  p->AddSample("TTZToLLNuNu", "ttV", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);//, 0.50);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",     "DY", itBkg);
  p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1);//, -99);
  // p->AddSample("TW", "tW", itSignal, kMagenta);// -99);
  // p->AddSample("TbarW", "tW", itSignal);
  p->AddSample("TW", "tW", itBkg, kMagenta);// -99);
  p->AddSample("TbarW", "tW", itBkg);
  p->AddSample("TTbar_PowhegSemi","NonWZ", itBkg, kGray);//, 0.5);
  p->AddSample("WJetsToLNu_MLM","NonWZ", itBkg, kGray);//, 0.5);
  // p->AddSample("MuonEG", "Data", itData);
  // p->AddSample("SingleMuon", "Data", itData);
  // p->AddSample("SingleElec", "Data", itData);
  // p->AddSample("DoubleEG", "Data", itData);
  // p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1,  "JERUp"); 
  p->AddSample("TW"   , "tW", itSys, 1, "JERUp");
  p->AddSample("TbarW", "tW", itSys, 1, "JERUp");
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1,  "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1,  "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1,  "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1,  "isrDown"); 
  p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1,  "fsrUp"); 
  p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1,  "fsrDown"); 

  p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1,  "hdampUp"); 
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1,  "hdampDown"); 



  p->AddSample("TW_noFullyHadr_isrUp","tW", itSys, 1,  "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp","tW", itSys, 1,  "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");
  p->AddSample("TW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMescaleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMescaleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp","tW", itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp","tW", itSys, 1,  "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMescaleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMescaleDown");


  p->AddSample("MuonEG", "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);

  p->AddSample("TW_DS","tW", itSys, 1,  "DiagramSubtractionUp");
  p->AddSample("TbarW_DS","tW", itSys, 1,  "DiagramSubtractionUp");
  p->AddSymmetricHisto("tW",  "DiagramSubtractionUp");
  // p->AddSample("DoubleEG", "Data", itData);
  // p->AddSample("DoubleMuon", "Data", itData);

  p->AddSystematic("JES,Btag,MisTag,PU,ElecEff,MuonEff"); //,LepEff
  
  p->AddSymmetricHisto("ttbar",  "JERUp");
  p->AddSymmetricHisto("tW",  "JERUp");
  

  // p->doSetLogy = false;
  // p->DrawStack("ElMu",1);
  p->SetOutputName("forCards_" + Xtitle);
  p->SaveHistograms();
  // p->PrintSystYields();
  delete p;
  // Datacard *d = new Datacard("tW", "ttV, VV, DY, tW, ttbar,NonWZ", "ue, isr, hdamp, Scale, pdf, JES, Btag, MisTag, LepEff, PU", "ElMu"); 
  // d->SetRootFileName("forCards.root");  

}


void runLooper(){
  Looper* p = new Looper(pathToTree, NameOfTree, "TMET", "TMET > 0", "ElMu", 30, 0, 300);
  Histo* h = p->GetHisto("TW");
  h->SetLineColor(kRed);
  cout << "nEntries = " << h->GetEntries() << endl;
  cout << "Yield = " << h->Integral()*35900 << endl;
  h->Draw();
}
