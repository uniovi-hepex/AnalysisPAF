#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(Datacard.C+)
R__LOAD_LIBRARY(CrossSection.C+)
// R__LOAD_LIBRARY(Datacard.C)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "Datacard.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);

TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/jul25/";

TString NameOfTree = "MiniTree";


void runCards(){
  // BEGIN COSAS VIEJAS
  // ACORDARSE DE PONER TOOOOODAS LAS VARIABLES SEPARADAS
  // DrawPlot("ShapVarWithNJets( TNJets , TNBtags , nLooseCentral )", "(TIsSS == 0)", "ElMu", 5, -0.5,4.5, "FitWithNJets");

  //  DrawPlot("ShapVarWithLoosePt( TNJets , TNBtags , TJetLoosept )", "(TIsSS == 0)", "ElMu", 10, -0.5,9.5, "FitWithPt");
  // DrawPlot("ShapVar( TNJets , TNBtags )", "(TIsSS == 0)", "ElMu", 3, -0.5,2.5, "SimpleFit");
  // DrawPlot("1","(TIsSS == 0) && (TNJets == 1 && TNBtags ==1)","ElMu",1,0.5,1.5,"Nothing");
  // DrawPlot("ShapVarWithBDt( TNJets , TNBtags , TBDT )", "(TIsSS == 0)", "ElMu", 12, -0.5,11.5, "BDT");
  // DrawPlot("ShapVarWithBDTada( TNJets , TNBtags , TBDTada )"  , "(TIsSS == 0)", "ElMu", 10, -0.5,9.5, "BDTada");
  // END COSAS VIEJAS



   // DrawPlot("ShapVar( TNJets , TNBtags )", "(TIsSS == 0)", "ElMu", 3, -0.5, 2.5, "YieldsOnly");
   // DrawPlot("ShapVarWith1j1tBDt( TNJets , TNBtags , TBDT )", "(TIsSS == 0)", "ElMu", 12, -0.5, 11.5, "With1j1t");
   // DrawPlot("ShapVarWithBothBDts( TNJets , TNBtags , TBDT , TBDT2j1t )", "(TIsSS == 0)", "ElMu", 17, -0.5, 16.5, "withBothBDTs");
   DrawPlot("ShapVarWith1j1tBDtAndJetPt( TNJets , TNBtags , TBDT , TBDT2j1t , TJet2_Pt )", "(TIsSS == 0)", "ElMu", 24, -0.5, 23.5, "WithEverything");



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
  p->AddSample("TW", "tW", itBkg, kMagenta);// -99);
  p->AddSample("TbarW", "tW", itBkg);
  p->AddSample("TTbar_PowhegSemi","NonWZ", itBkg, kGray);//, 0.5);
  p->AddSample("WJetsToLNu_MLM","NonWZ", itBkg, kGray);//, 0.5);

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
  p->AddSample("TW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp","tW", itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp","tW", itSys, 1,  "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");


  p->AddSample("MuonEG", "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);

  p->AddSample("TW_DS","tW", itSys, 1,  "DiagramSubtractionUp");
  p->AddSample("TbarW_DS","tW", itSys, 1,  "DiagramSubtractionUp");
  p->AddSymmetricHisto("tW",  "DiagramSubtractionUp");

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfDown"); 

  // p->AddSample("DoubleEG", "Data", itData);
  // p->AddSample("DoubleMuon", "Data", itData);
  
  p->AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  
  p->AddSymmetricHisto("ttbar",  "JERUp");
  p->AddSymmetricHisto("tW",  "JERUp");


  // p->AddSystematic("stat"); //,LepEff
  // p->doSetLogy = false;
  // p->DrawStack("ElMu_"+Xtitle,1);
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
