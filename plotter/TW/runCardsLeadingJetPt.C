#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(Datacard.C+)
R__LOAD_LIBRARY(CrossSection.C+)
// R__LOAD_LIBRARY(Datacard.C)
R__LOAD_LIBRARY(TTJet1_pt.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "Datacard.h"
#include "TTJet1_pt.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString chlabel);
// pas
// TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/aug21/";
TString pathToTree = "/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/TW_temp/";

TString NameOfTree = "Mini1j1t";


void runCardsLeadingJetPt(){


 //  DrawPlot("TDilepMETJetPt "	   , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 3,  0.,   120.,  "p^{boost}",false);

 // return; 
 //  DrawPlot("theBDt_bin1( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TDilepMETJetPt < 40  )", "ElMu", 10, 0.5, 10.5, "Test_1","");
 //  DrawPlot("theBDt_bin2( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TDilepMETJetPt < 80 && TDilepMETJetPt > 40  )", "ElMu", 10, 0.5, 10.5, "Test_2","");
 //  DrawPlot("theBDt_bin3( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TDilepMETJetPt < 120 && TDilepMETJetPt > 80  )", "ElMu", 10, 0.5, 10.5, "Test_3","");

  // DrawPlot("TTJet1_pt ", "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 3,  0.,   150.,  "p_{T}^{lep}",false);

  DrawPlot("theBDt_bin1( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt < 50 )"                   , "ElMu", 10, 0.5, 10.5, "Jet1_pt_1","");
  DrawPlot("theBDt_bin2( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt > 50  && TTJet1_pt < 70 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_2","");
  DrawPlot("theBDt_bin3( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt > 70  && TTJet1_pt < 90 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_3","");
  DrawPlot("theBDt_bin4( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt > 90  && TTJet1_pt <110 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_4","");
  DrawPlot("theBDt_bin5( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt >110  && TTJet1_pt <130 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_5","");
  DrawPlot("theBDt_bin6( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt >130  && TTJet1_pt <150 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_6","");
  DrawPlot("theBDt_bin7( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt >150  && TTJet1_pt <170 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_7","");
  DrawPlot("theBDt_bin8( TBDT )", "(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && TTJet1_pt >170  && TTJet1_pt <500 )", "ElMu", 10, 0.5, 10.5, "Jet1_pt_8","");
  return;


}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString chlabel){
  cout << "Xtitle is " << Xtitle << endl;

  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);

  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose = false;
  // p->chlabel = chlabel;
  // p->SetVarName("forFit");

  p->AddSample("WZ", "VVttV", itBkg, 390); // 0.50
  p->AddSample("WW", "VVttV", itBkg);
  p->AddSample("ZZ", "VVttV", itBkg);
  p->AddSample("TTWToLNu", "VVttV", itBkg);//, 0.5);
  p->AddSample("TTWToQQ" , "VVttV", itBkg);
  p->AddSample("TTZToQQ" , "VVttV", itBkg);
  p->AddSample("TTZToLLNuNu", "VVttV", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, 852);//, 0.50);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",     "DY", itBkg);
  p->AddSample("TTbar_Powheg", "ttbar", itBkg, 633);//, -99);
  p->AddSample("TW", "tW", itBkg, TColor::GetColor("#ffcc33"));// -99);
  p->AddSample("TbarW", "tW", itBkg);
  p->AddSample("TTbar_PowhegSemi","Fakes", itBkg, 413);//, 0.5);
  p->AddSample("WJetsToLNu_MLM","Fakes", itBkg);//, 0.5);

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1,  "JERUp"); 
  p->AddSample("TW"   , "tW", itSys, 1, "JERUp");
  p->AddSample("TbarW", "tW", itSys, 1, "JERUp");



  p->AddSample("MuonEG", "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);

  p->AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  
  p->AddSymmetricHisto("ttbar",  "JERUp");
  p->AddSymmetricHisto("tW",  "JERUp");

  p->SetLegendPosition(20,20,20,20);


  p->doSetLogy = false;
  
  // esto es lo gueno
  p->SetOutputName("forCards_" + Xtitle);
  p->SaveHistograms();
  
  // p->DrawStack(Xtitle, 1);

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
