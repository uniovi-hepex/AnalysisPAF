#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);
TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/jul25/";
TString NameOfTree = "Mini2j1t";


void run2j1bBDTPlots(){

  //\item \pt of Sub-leading jet
  DrawPlot("TjetPtSubLeading_ " ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 30., 200., "Sub-leading jet p_{T} [GeV]");

  //\item $\delta R (L1, J1)$: separation between leading lepton and leading jet in $\phi -\eta$~space.
  DrawPlot("TDR_L1_J1 "         ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 50, 0., 5., "#Delta R(#ell_{1}, j_{1})");

  //\item $\delta R (L1L2, J1J2)$: separation between leptons and jets in $\phi -\eta$~space.
  DrawPlot("TDR_L1L2_J1J2 "     ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 50, 0., 5., "#Delta R(e^{#pm}#mu^{#mp}, j_{1} j_{2})");

  //\item $\delta R (L1L2, J1J2E_T^{sys})$: separation between leptons and jets,met in $\phi -\eta$~space.
  DrawPlot("TDR_L1L2_J1J2MET "  ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 50, 0., 5., "#Delta R(e^{#pm}#mu^{#mp}, j_{1} j_{2} MET)");
  

  // DrawPlot("the2j1tBDt( TBDT2j1t )" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6, 0.5,6.5, "BDT bin");
  // DrawPlot("TBDT2j1t " ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 50, -1.,1., "BDT score");
  // DrawPlot("TBDT2j1t_DR " ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 50, -1.,1., "BDT score");


  // DrawPlot( "TBDT2j1tv1" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,-1., 1., "BDT bin");
  // DrawPlot( "TBDT2j1tv2" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,-1., 1., "BDT bin");
  // DrawPlot( "TBDT2j1tv3" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,-1., 1., "BDT bin");


  // return; 
  // DrawPlot("the2j1tBDt( TBDT2j1t )" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,0.5,6.5, "BDT bin");
  
  // return;
  //\item Leading lepton $p_T$~(\pt $>$ 25 GeV)
  // DrawPlot("TLep_Pt[0] " ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 30., 300., "p_{T}^{l1} [GeV]");
      
  // \item Leading lepton $|\eta|$~( $|\eta| <$ 2.4)
  // DrawPlot("TLep_Eta[0]" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, -2.4, 2.4, "#eta_{l1}");
  


  
  // //\item $|\eta|$ of Sub-leading jet
  // DrawPlot("TJet_Eta[1]","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, -2.4, 2.4, "#eta^{j2}");

  //\item Vector sum of \pt of two leptons
  // DrawPlot("TDilepPt ","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 0., 200., "p_{T}^{ll}  [GeV]");

  //\item vector sum of \pt of two leptons and \met
  // DrawPlot("TDilepMETPt ","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 0., 400., "p_{T}^{llMET}  [GeV]");

  // //\item $E_T^{sys}$ Scalar sum of transverse energy of all selected objects: leptons, jets and \met.
  // DrawPlot("TETSys","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 100., 1000., "#sum E_{T}  [GeV]");

  // //\item Scalar sum of transverse energy of all selected objects, excluding sub-leading jet.
  // DrawPlot("TET_LLJetMET","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 100., 1000., "#sum E_{T} - E_{T}^{j2}  [GeV]");

  // //\item $H_T$: Scalar sum of \pt of all selected objects leptons, jets, and \met.
  // DrawPlot("THTtot2j","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 100., 1000., "H_{T}  [GeV]");

  // // \item Scalar sum of \pt of all selected objects, excluding sub-leading jet.
  // DrawPlot("THtRejJ2","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 100., 1000., "H_{T} - p_{T}^{j2} [GeV]");
  

  //\item $\pt (L1, L2)$: \pt difference between two leptons.
  // DrawPlot("TDPtL1_L2 ","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, 0., 200., "p_{T}^{l1} - p_{T}^{l2}  [GeV]");

  //\item $\pt (L2, J2)$: \pt difference between sub-leading lepton and sub-leading jet.
  // DrawPlot("TDPtJ2_L2 ","(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 40, -150., 150., "p_{T}^{l2} - p_{T}^{j2} [GeV]");
  

  // DrawPlot("the2j1tBDt( TBDT2j1t )" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,0.5,6.5, "BDT bin");
  
// DrawPlot("TBDT2j1t" ,"(TNJets == 2) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 6,-1.,1., "BDT bin");

}
  

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose = false;
  // p->SetVarName("forFit");

  TGaxis::SetMaxDigits(3);
  p->chlabel="e^{#pm}#mu^{#mp} + 2j1b";

  p->AddSample("WZ"         , "VV+t#bar{t}V", itBkg, kOrange-3);
  p->AddSample("WW"         , "VV+t#bar{t}V", itBkg);
  p->AddSample("ZZ"         , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTWToLNu"   , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTWToQQ"    , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToQQ"    , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToLLNuNu", "VV+t#bar{t}V", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", "Z/#gamma* #rightarrow e^{#pm}#mu^{#mp}", itBkg, TColor::GetColor("#3b0160")); // kRed);
  p->AddSample("DYJetsToLL_M50_aMCatNLO"    , "Z/#gamma* #rightarrow e^{#pm}#mu^{#mp}", itBkg);
  p->AddSample("TTbar_Powheg"               , "t#bar{t}", itBkg, TColor::GetColor("#669966")); // TColor::GetColor("#8ADCFF"));
  p->AddSample("TTbar_PowhegSemi"           , "Non W/Z" , itBkg, kGray);
  p->AddSample("WJetsToLNu_MLM"             , "Non W/Z" , itBkg, kGray);
  p->AddSample("TW"                         , "tW"      , itBkg, TColor::GetColor("#ff4800")); // TColor::GetColor("#ffc878"));
  p->AddSample("TbarW"                      , "tW"      , itBkg);


  p->AddSample("MuonEG"    , "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);

  p->AddSample("TTbar_Powheg_hdampUp" , "t#bar{t}", itSys, 1,  "hdampUp"); 
  p->AddSample("TTbar_Powheg_hdampDown", "t#bar{t}", itSys, 1,  "hdampDown"); 

  p->AddSample("TTbar_Powheg_ueUp"   , "t#bar{t}", itSys, 1,  "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown" , "t#bar{t}", itSys, 1,  "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp"  , "t#bar{t}", itSys, 1,  "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "t#bar{t}", itSys, 1,  "isrDown"); 
  p->AddSample("TTbar_Powheg_fsrUp"  , "t#bar{t}", itSys, 1,  "fsrUp"); 
  p->AddSample("TTbar_Powheg_fsrDown", "t#bar{t}", itSys, 1,  "fsrDown"); 

  p->AddSample("TW_noFullyHadr_isrUp"  ,"tW", itSys, 1,  "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"  ,"tW", itSys, 1,  "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");

  p->AddSample("TW_noFullyHadr_MEscaleUp"  ,"tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp"      ,"tW", itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    ,"tW", itSys, 1,  "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      ,"tW", itSys, 1,  "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    ,"tW", itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  ,"tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfDown"); 

  p->SetRatioMin( 0.6 );
  p->SetRatioMax( 1.4 );

  p->SetLegendPosition(0.7, 0.45, 0.93, 0.92);

  p->SetPlotFolder("2j1t/");
  
  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  //p->AddSystematic("stat"); //,LepEff

  p->doYieldsInLeg=false;

  // p->doStackSignal = true;
  p->doSetLogy = false;
  p->doData = true;
  // p->SetLegendPosition("DL");
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
