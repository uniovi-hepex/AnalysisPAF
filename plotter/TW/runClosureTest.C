#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(Datacard.C+)
R__LOAD_LIBRARY(CrossSection.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "Datacard.h"

TString pathToTree = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/16_04_2018/";
TString NameOfTree = "Mini1j1t";


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bins, TString Xtitle, TString chlabel);

void runClosureTest() {
  Float_t binsJetPt[] = {0,50,100,150,500};
  Float_t binsJetEta[] = {-2.4,-1.2,0,1.2,2.4};
  DrawPlot("TGenLeadingJetPt", "1/TWeight*TWeight_normal*(Tpassgen == 1)", "All", 4, binsJetPt, "TGenLeadingJet","");
  DrawPlot("TGenLeadingJetEta", "1/TWeight*TWeight_normal*(Tpassgen == 1)", "All", 4, binsJetEta , "TGenLeadingJet","");
    DrawPlot("TGenLeadingLepEta", "1/TWeight*TWeight_normal*(Tpassgen == 1)", "All", 4, binsJetEta, "TGenLeadingJet","");

  return;
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bins, TString Xtitle, TString chlabel) {
  cout << "Xtitle is " << Xtitle << endl;

  Plot* p = new Plot(var, cut, chan, nbins, bins, "Title", Xtitle);

  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  // p->verbose = false;
  p->verbose = true;
  // p->chlabel = chlabel;
  // p->SetVarName("forFit");

  p->AddSample("UNF_TW", "tW", itBkg, TColor::GetColor("#ffcc33"));// -99);
  p->AddSample("UNF_TbarW", "tW", itBkg);


  // Other settingsX fan
  p->SetLegendPosition(20,20,20,20);
  p->doSetLogy = false;
  
  p->SetOutputName("closuretest_" + Xtitle);
  p->SaveHistograms();
  
  // p->DrawStack(Xtitle, 1);

  delete p;
}
