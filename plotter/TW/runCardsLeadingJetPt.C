#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(Datacard.C+)
R__LOAD_LIBRARY(CrossSection.C+)
R__LOAD_LIBRARY(./TW/Differential/temp/LeadingJetPt.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "Datacard.h"
#include "./TW/Differential/temp/LeadingJetPt.C"

TString pathToTree = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/19_04_2018/";
TString NameOfTree = "Mini1j1t";


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString chlabel);

void runCardsLeadingJetPt() {
  
  DrawPlot("theBDt_bin1( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt < 50 )"                       , "ElMu", 10, 0.5, 10.5, "LeadingJetPt_1","");
  DrawPlot("theBDt_bin2( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >= 50  && TLeadingJetPt < 70 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_2","");
  DrawPlot("theBDt_bin3( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >= 70  && TLeadingJetPt < 90 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_3","");
  DrawPlot("theBDt_bin4( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >= 90  && TLeadingJetPt <110 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_4","");
  DrawPlot("theBDt_bin5( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >=110  && TLeadingJetPt <130 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_5","");
  DrawPlot("theBDt_bin6( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >=130  && TLeadingJetPt <150 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_6","");
  DrawPlot("theBDt_bin7( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >=150  && TLeadingJetPt <170 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_7","");
  DrawPlot("theBDt_bin8( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TLeadingJetPt >=170  && TLeadingJetPt <500 )", "ElMu", 10, 0.5, 10.5, "LeadingJetPt_8","");
  return;
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString chlabel) {
  cout << "Xtitle is " << Xtitle << endl;

  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);

  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetLimitFolder("./TW/Differential/temp/");
  p->SetPathSignal(pathToTree);
  // p->verbose = false;
  p->verbose = true;
  // p->chlabel = chlabel;
  // p->SetVarName("forFit");

  // Adding general-purpose samples --- Beware of the ORDER of the samples!!!!!!
  p->AddSample("WZ",                          "VVttV", itBkg, 390); // 0.50
  p->AddSample("WW",                          "VVttV", itBkg);
  p->AddSample("ZZ",                          "VVttV", itBkg);
  p->AddSample("TTWToLNu",                    "VVttV", itBkg);//, 0.5);
  p->AddSample("TTWToQQ" ,                    "VVttV", itBkg);
  p->AddSample("TTZToQQ" ,                    "VVttV", itBkg);
  p->AddSample("TTZToLLNuNu",                 "VVttV", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY",    itBkg, 852);//, 0.50);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",     "DY",    itBkg);
  p->AddSample("TTbar_Powheg",                "ttbar", itBkg, 633);//, -99);
  p->AddSample("TW",                          "tW",    itBkg, TColor::GetColor("#ffcc33"));// -99);
  p->AddSample("TbarW",                       "tW",    itBkg);
  p->AddSample("TTbar_PowhegSemi",            "Fakes", itBkg, 413);//, 0.5);
  p->AddSample("WJetsToLNu_MLM",              "Fakes", itBkg);//, 0.5);

  // Adding samples for systematic uncertanties
  p->AddSample("TTbar_Powheg",                "ttbar", itSys, 1, "JERUp"); 
  p->AddSample("TW"   ,                       "tW",    itSys, 1, "JERUp");
  p->AddSample("TbarW",                       "tW",    itSys, 1, "JERUp");

  // p->AddSample("MuonEG",                      "Data",  itData);
  // p->AddSample("SingleMuon",                  "Data",  itData);
  // p->AddSample("SingleElec",                  "Data",  itData);

  p->AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  
  p->AddSymmetricHisto("ttbar", "JERUp");
  p->AddSymmetricHisto("tW",    "JERUp");

  p->doSignal=false;

  // Other settingsX fan
  p->SetLegendPosition(20, 20, 20, 20);
  p->doSetLogy = false;
  
  // p->SetOutputName("forCards_" + Xtitle);
  p->NoShowVarName = true;
  // p->SaveHistograms();
  p->DrawStack(Xtitle);
  delete p;
}


void runLooper(){
  Looper* p = new Looper(pathToTree, NameOfTree, "TMET", "TMET > 0", "ElMu", 30, 0, 300);
  Histo* h = p->GetHisto("TW");
  h->SetLineColor(kRed);
  cout << "nEntries = " << h->GetEntries() << endl;
  cout << "Yield = " << h->Integral()*35900 << endl;
  h->Draw();
}
