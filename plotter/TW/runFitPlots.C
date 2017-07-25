#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varname, bool setLegendLeft=false );
TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/jul25/";
TString NameOfTree = "MiniTree";


void runFitPlots(){

  DrawPlot("ShapVarWith1j1tBDtAndJetPt( TNJets , TNBtags , TBDT , TBDT2j1t , TJet2_Pt )", "(TIsSS == 0)", "ElMu", 24, -0.5, 23.5, "Bin","FullFit");
  DrawPlot("TBDT ", "(TIsSS == 0 && TNJets == 1 && TNBtags == 1)", "ElMu", 20, -1., 1., "BDT","TBDT1j1t",true);
  DrawPlot("TBDT2j1t ", "(TIsSS == 0 && TNJets == 2 && TNBtags == 1)", "ElMu", 20, -1., 1., "BDT","TBDT2j1t",true);
  return; 

  
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varname, bool setLegendLeft = false){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose = true;
  p->SetVarName(varname);

  p->AddSample("TTWToLNu","Other", itBkg, kOrange-3);
  p->AddSample("TTWToQQ", "Other", itBkg);
  p->AddSample("TTZToQQ", "Other", itBkg);
  p->AddSample("TTZToLLNuNu", "Other", itBkg);
  p->AddSample("WZ", "Other", itBkg);
  p->AddSample("WW", "Other", itBkg);
  p->AddSample("ZZ", "Other", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", " Z/#gamma* #rightarrow e^{#pm}#mu^{#mp}", itBkg, TColor::GetColor("#3b0160")); // kRed);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",     " Z/#gamma* #rightarrow e^{#pm}#mu^{#mp}", itBkg);
  p->AddSample("TTbar_Powheg", "t#bar{t}", itBkg, TColor::GetColor("#669966")); // TColor::GetColor("#8ADCFF"));
  p->AddSample("TW", "tW", itBkg, TColor::GetColor("#ff4800")); // TColor::GetColor("#ffc878"));
  p->AddSample("TbarW", "tW", itBkg);
  p->AddSample("TTbar_PowhegSemi","Non W/Z", itBkg, kGray);
  p->AddSample("WJetsToLNu_MLM","Non W/Z", itBkg, kGray);


  p->AddSample("MuonEG", "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);
  
  cout << "hdamp" << endl;
  p->AddSample("TTbar_Powheg_hdampUp", "t#bar{t}", itSys, 1,  "hdampUp"); 
  p->AddSample("TTbar_Powheg_hdampDown", "t#bar{t}", itSys, 1,  "hdampDown"); 

  cout << "ue" << endl;


  // SERGIO: nuevo


  p->AddSample("TTbar_Powheg_ueUp", "t#bar{t}", itSys, 1,  "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown", "t#bar{t}", itSys, 1,  "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp", "t#bar{t}", itSys, 1,  "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "t#bar{t}", itSys, 1,  "isrDown"); 
  // p->AddSample("TTbar_Powheg_fsrUp", "t#bar{t}", itSys, 1,  "fsrUp"); 
  // p->AddSample("TTbar_Powheg_fsrDown", "t#bar{t}", itSys, 1,  "fsrDown"); 

  p->AddSample("TW_noFullyHadr_isrUp","tW", itSys, 1,  "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  // p->AddSample("TW_noFullyHadr_fsrUp","tW", itSys, 1,  "fsrUp");
  // p->AddSample("TW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");

  p->AddSample("TW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp","tW", itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  // p->AddSample("TbarW_noFullyHadr_fsrUp","tW", itSys, 1,  "fsrUp");
  // p->AddSample("TbarW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp","tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");
  // // SERGIO: end nuevo

//SERGIO: volver a poer  p->AddSample("TW_DS","tW", itSys, 1,  "DiagramSubtractionUp");
//SERGIO: volver a poer  p->AddSample("TbarW_DS","tW", itSys, 1,  "DiagramSubtractionUp");

  cout << "powheg" << endl;

  // SERGIO: to addd ME, pdf
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfDown"); 


  
  // SERGIO: to add JER
  // p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1,  "JERUp"); 
  // p->AddSample("TW"   , "tW", itSys, 1, "JERUp");
  // p->AddSample("TbarW", "tW", itSys, 1, "JERUp");

  
  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  // p->AddSystematic("stat"); //,LepEff
  
  // p->AddSymmetricHisto("ttbar",  "JERUp");
  // p->AddSymmetricHisto("tW",  "JERUp");


  p->doYieldsInLeg=false;


  // p->AddSystematic("JES,Btag,MisTag,PU,ElecEff,MuonEff"); //,LepEff
  // p->doStackSignal = true;
  p->doSetLogy = false;
  p->doData = true;
  if (setLegendLeft) p->SetLegendPosition("UL");
  p->DrawStack("ElMu_0_log", 1);
  //cout << "done stack" << endl;
  // p->PrintSystematics();
  //p->PrintSystYields();

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
