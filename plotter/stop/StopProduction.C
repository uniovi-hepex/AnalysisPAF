//###################################################################################################
//
// Draw multple plots at gen level
// Uses minitrees produced only with gen info, no cuts on reco variables, no skimmed n-tupples
//  Ussage:
//    root -l -b -q 'StopProduction.C("")'
//
//###################################################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"
#include "FRFunctions.C"

//=== Functions
void DrawPlot(TString var, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, Float_t rmin, Float_t rmax,  Int_t scenario);

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/StopProduction/";
TString NameOfTree = "tree";
TString chan = "1"; //"ElMu"
TString dilepgen = "TNgenLeps >= 2 && fabs(TgenLep0_Eta) < 2.4 && fabs(TgenLep1_Eta) < 2.4";
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";

//=== The main function
void StopProduction(Int_t scenario = 0){

  //>>> Scenario 1: "TTbar_Powheg" "TTJets_MLM" "T2tt_175_1"  "T2tt_225_50" 
  //>>> Scenario 2: "TTbar_Powheg" "T2tt_175_1"  "T2tt_200_50" "T2tt_225_50"  "T2tt_250_50"
  if(scenario == 1 || scenario == 2 || scenario == 4 || scenario == 5){
    // Top

    DrawPlot("TgenTop1Pt : TgenTop2Pt",   50, 0, 400, "gen p_{T}^{top} [GeV]", "TopPt",   0.5, 1.5, scenario);
    DrawPlot("TgenDeltaPhi", 15, 0, 3.2, "gen #Delta#phi",                "gDeltaPhi", 0.8, 1.2, scenario);
    DrawPlot("TgenDeltaEta", 15, 0, 3.2, "gen #Delta#eta",                "gDeltaEta", 0.6, 1.4, scenario);
    DrawPlot("TgenMET",       20, 0, 250, "gen MET [GeV]",                "genMET",0, 2, scenario);
    DrawPlot("TgenLep0_Pt",   15, 0, 200, "gen Leading lep p_{T} [GeV]",    "genLep0Pt", 0.8, 1.2, scenario);
    DrawPlot("TgenMT2",       20, 0, 120, "gen M_{T2} [GeV]",             "genMT2", 0, 2, scenario);
    DrawPlot("TgenTop1Pt : TgenTop2Pt",   50, 0, 400, "gen p_{T}^{top} [GeV]", "TopPt",   0.5, 1.5, scenario);
   // DrawPlot("TgenTop1M  : TgenTop2M",    40, 165, 185, "gen M_{top} [GeV]", "TopMass", 0.5, 1.5, scenario);
 /*   DrawPlot("TgenTop1Pt : TgenTop2Pt",   50, 0, 400, "gen p_{T}^{top} [GeV]", "TopPt",   0.5, 1.5, scenario);
    DrawPlot("DeltaPhi(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             30, 0, 3.2,   "gen #Delta#Phi(t,#bar{t})", "TopDeltaPhi", 0.4, 1.6, scenario);
    DrawPlot("DeltaR(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             30, 0, 6,   "gen #DeltaR(t,#bar{t})",   "TopDeltaR",   0.4, 1.6, scenario);
    DrawPlot("InvMass(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             40, 300, 1000,   "gen M_{t#bar{t}}",   "Mttbar",   0.4, 1.6, scenario);

    // W
    DrawPlot("TgenW1M  : TgenW2M",    40, 76, 120, "gen M_{W} [GeV]", "WMass", 0.5, 1.5, scenario);
    DrawPlot("TgenW1Pt : TgenW2Pt",   40, 0, 300, "gen p_{T}^{W} [GeV]", "WPt",   0.8, 1.2, scenario);
    DrawPlot("DeltaPhi(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
              30, 0, 3.2,   "gen #Delta#Phi(W^{+},W^{-})", "WDeltaPhi", 0.8, 1.2, scenario);
    DrawPlot("DeltaR(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
              30, 0, 6,   "gen #DeltaR(W^{+},W^{-})",   "WDeltaR",   0.8, 1.2, scenario);
    DrawPlot("InvMass(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
             40, 140, 600,   "gen M_{WW}",   "MWW",   0.4, 1.6, scenario);

    // Neutralino
    DrawPlot("DilepPt(TgenNeu1Pt, TgenNeu1Eta, TgenNeu1Phi, TgenNeu1M, TgenNeu2Pt, TgenNeu2Eta, TgenNeu2Phi, TgenNeu2M)",    
    DrawPlot("DeltaPhi(TgenNeu1Pt, TgenNeu1Eta, TgenNeu1Phi, TgenNeu1M, TgenNeu2Pt, TgenNeu2Eta, TgenNeu2Phi, TgenNeu2M)",   
              30, 0, 3.2,   "gen #Delta#Phi(#nu,#nu)", "NeuDeltaPhi", 0.8, 1.2, scenario);
    DrawPlot("DeltaEta(TgenNeu1Eta,TgenNeu2Eta)",    30, 0, 7, "gen #Delta#eta(#nu#nu)", "NeuDeltaEta", 0.4, 1.6, scenario);
*/

   // Other:
   // Delta phi (dilep, MET)
   

  }

  //>>> "T2tt_175_1"  "T2tt_200_50" "T2tt_225_50"  "T2tt_250_50"
  else if(scenario == 3){
    // Neutralino
    //DrawPlot("DilepPt()",    30, 0, 300, "gen P_{T}^{#chi#chi} [GeV]", "DiNeutrinoPt", 0.8, 1.2, scenario);
    
    //DrawPlot("TgenNeu1Pt : TgenNeu2Pt",  30, 0, 300, "gen P_{T}^{#nu#nu} [GeV]", "DiNeutrinoPt", 0.8, 1.2, scenario);

    // Neutrino vs neutralino
    // - di-neutrino vs di neutralino pt
    // - DPhi(di-neutrino, di-neutralino)
    // - pt(neutrinosss) : pt(neutralinossss)

    // Stop
 
  }

  else if(scenario == 7){
    DrawPlot("StopTopPt",      50, 0,   400, "gen p_{T} [GeV]",            "StopTopPt",   0.0, 2.0, scenario); 
    DrawPlot("StopTopDiPt",    50, 0,   400, "gen p_{T}^{t#bar{t}} [GeV]", "StopTopDiPt",   0.0, 2.0, scenario); 
    DrawPlot("StopTopInvMass", 50, 330,1000, "gen M_{t#bar{t}} [GeV]",     "StopTopInvMass",   0.0, 2.0, scenario); 
  }

  else if(scenario == 11 || scenario == 13){
/*    DrawPlot("TgenTop1M  : TgenTop2M",    40, 165, 185, "gen M_{top} [GeV]", "TopMass", 0.5, 1.5, scenario);
    DrawPlot("DeltaPhi(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             30, 0, 3.2,   "gen #Delta#Phi(t,#bar{t})", "TopDeltaPhi", 0.4, 1.6, scenario);
    DrawPlot("DeltaR(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             30, 0, 6,   "gen #DeltaR(t,#bar{t})",   "TopDeltaR",   0.4, 1.6, scenario);
    DrawPlot("InvMass(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)",   
             40, 300, 1000,   "gen M_{t#bar{t}}",   "Mttbar",   0.4, 1.6, scenario);
    DrawPlot("TgenW1M  : TgenW2M",    40, 76, 120, "gen M_{W} [GeV]", "WMass", 0.5, 1.5, scenario);
    DrawPlot("TgenW1Pt : TgenW2Pt",   40, 0, 300, "gen p_{T}^{W} [GeV]", "WPt",   0.8, 1.2, scenario);
    DrawPlot("DeltaPhi(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
              30, 0, 3.2,   "gen #Delta#Phi(W^{+},W^{-})", "WDeltaPhi", 0.8, 1.2, scenario);
    DrawPlot("DeltaR(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
              30, 0, 6,   "gen #DeltaR(W^{+},W^{-})",   "WDeltaR",   0.8, 1.2, scenario);
    DrawPlot("InvMass(TgenW1Pt, TgenW1Eta, TgenW1Phi, TgenW1M, TgenW2Pt, TgenW2Eta, TgenW2Phi, TgenW2M)",   
             40, 140, 600,   "gen M_{WW}",   "MWW",   0.4, 1.6, scenario);

    DrawPlot("TgenDeltaPhi", 15, 0, 3.2, "gen #Delta#phi",                "gDeltaPhi", 0.8, 1.2, scenario);
    DrawPlot("TgenDeltaEta", 15, 0, 3.2, "gen #Delta#eta",                "gDeltaEta", 0.6, 1.4, scenario);
    DrawPlot("TgenMET",       20, 0, 250, "gen MET [GeV]",                "genMET",0, 2, scenario);
    DrawPlot("TgenLep0_Pt",   15, 0, 200, "gen Leading lep p_{T} [GeV]",    "genLep0Pt", 0.8, 1.2, scenario);
    DrawPlot("TgenMT2",       20, 0, 120, "gen M_{T2} [GeV]",             "genMT2", 0, 2, scenario);
    DrawPlot("TgenTop1Pt : TgenTop2Pt",   50, 0, 400, "gen p_{T}^{top} [GeV]", "TopPt",   0.5, 1.5, scenario);
*/
//    DrawPlot("fabs(TgenLep0_Eta) : fabs(TgenLep1_Eta)",   20, 0, 2.4, "gen lep #eta",    "genLepEta", 0.8, 1.2, scenario);
    DrawPlot("TgenLep0_Pt : TgenLep1_Pt",   20, 0, 200, "gen lep p_{T} [GeV]",    "genLepPt", 0.8, 1.2, scenario);
    
  }
  else if(scenario == 12){
    DrawPlot("MET",       20, 0, 250, "gen MET [GeV]",                "genMET",0, 2, scenario);
    DrawPlot("MT2",       20, 0, 120, "gen M_{T2} [GeV]",             "genMT2",0, 2, scenario);
  }
  else if(scenario == 99){
   DrawPlot("TgenTop1Pt : TgenTop2Pt",   60, 0, 600, "gen p_{T}^{top} [GeV]", "TopPt",   0.8, 1.2, scenario);
   DrawPlot("TgenTop1Pt : TgenTop2Pt",   60, 0, 600, "gen p_{T}^{top} [GeV]", "TopStopPt",   0.8, 1.2, scenario);
  }

  else{
    StopProduction(7);
  }

}

void DrawPlot(TString var, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, Float_t rmin, Float_t rmax, Int_t scenario){
  Plot* p = new Plot(var, dilepgen, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder(TString("output/StopProd/") + Form("scenario%i/", scenario));
  p->verbose = false;
  p->doYieldsInLeg = false;
  p->SetVarName(name);


  //>>> 
  if(scenario == 1){
    p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",         "T2tt [175, 1]", itSignal, kRed+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1, "0", "noWeight");
  }

  //>>> 
  else if(scenario == 2){
    p->AddSample("TTbar_Powheg",           "tt Powheg",     itSignal, 1,         "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",          "T2tt [175, 1]", itSignal, kViolet+1, "0", "noWeight");
    p->AddSample("T2tt_200_50_FS_summer",  "T2tt [200,50]", itSignal, kGreen+2,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer",  "T2tt [225,50]", itSignal, kBlue+1,   "0", "noWeight");
    p->AddSample("T2tt_250_50_FS_summer",  "T2tt [250,50]", itSignal, kRed,      "0", "noWeight");
  } 

  //>>> 
  else if(scenario == 3){
    p->AddSample("T2tt_175_1",             "T2tt [175, 1]", itSignal, kViolet+1, "0", "noWeight");
    p->AddSample("T2tt_200_50_FS_summer",  "T2tt [200,50]", itSignal, kGreen+2,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer",  "T2tt [225,50]", itSignal, kBlue+1,   "0", "noWeight");
    p->AddSample("T2tt_250_50_FS_summer",  "T2tt [250,50]", itSignal, kRed,      "0", "noWeight");
  }

  else if(scenario == 4){
    p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
    p->AddSample("T2tt_175_1_FS_xqcut20", "T2tt [175, 1]", itSignal, kRed+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_xqcut20", "T2tt [225,50]", itSignal, kBlue+1, "0", "noWeight");
  }

  //>>> 
  else if(scenario == 5){
    p->AddSample("TTbar_Powheg",           "tt Powheg",     itSignal, 1,         "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",          "T2tt [175, 1]", itSignal, kViolet+1, "0", "noWeight");
    p->AddSample("T2tt_200_50_FS_xqcut20",  "T2tt [200,50]", itSignal, kGreen+2,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_xqcut20",  "T2tt [225,50]", itSignal, kBlue+1,   "0", "noWeight");
    p->AddSample("T2tt_250_50_FS_xqcut20",  "T2tt [250,50]", itSignal, kRed,      "0", "noWeight");
  } 

  //>>> 
  else if(scenario == 6){
    p->AddSample("T2tt_175_1",             "T2tt [175, 1]", itSignal, kViolet+1, "0", "noWeight");
    p->AddSample("T2tt_200_50_FS_xqcut20",  "T2tt [200,50]", itSignal, kGreen+2,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_xqcut20",  "T2tt [225,50]", itSignal, kBlue+1,   "0", "noWeight");
    p->AddSample("T2tt_250_50_FS_xqcut20",  "T2tt [250,50]", itSignal, kRed,      "0", "noWeight");
  }

  else if(scenario == 7){
    TString varTop = ""; TString varStop = "";
    if     (var == "StopTopPt"){
      varTop  = "TgenTop1Pt  : TgenTop2Pt";
      varStop = "TgenStop1Pt : TgenStop2Pt";
    }
    else if(var == "StopTopDiPt"){
      varTop  = "DilepPt(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)";
      varStop = "DilepPt(TgenStop1Pt, TgenStop1Eta, TgenStop1Phi, TgenStop1M, TgenStop2Pt, TgenStop2Eta, TgenStop2Phi, TgenStop2M)";
    }
    else if(var == "StopTopInvMass"){
      varTop  = "InvMass(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M, TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M)";
      varStop = "InvMass(TgenStop1Pt, TgenStop1Eta, TgenStop1Phi, TgenStop1M, TgenStop2Pt, TgenStop2Eta, TgenStop2Phi, TgenTop2M)";
    }
    TString stoplabel = "#tilde{t}#tilde{t}";
    TString  toplabel = "t#bar{t}";
    if(var == "StopTopPt"){
      stoplabel = "#tilde{t}";
      toplabel = "t";
    }
    p->SetVar(varTop);
    p->AddSample("TTbar_Powheg",          "tt Powheg, " + toplabel,     itSignal, 1,       "0", "noWeight");
    p->AddSample("TTJets_MLM",            "tt MLM, " + toplabel,        itSignal, kGray+1, "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",         "T2tt [175, 1], " + toplabel, itSignal, kRed+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50], " + toplabel, itSignal, kBlue+1, "0", "noWeight");
    p->SetVar(varStop);
    p->AddSample("T2tt_175_1_FS",         "T2tt [175, 1], " + stoplabel, itSignal, kRed+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50], " + stoplabel, itSignal, kBlue+1, "0", "noWeight");
    p->SetLineStyle("T2tt [175, 1], " + stoplabel, 2);
    p->SetLineStyle("T2tt [225,50], " + stoplabel, 2);
  }

  else if(scenario == 11){
    TString pathToSF   = "./stop/";
    TString fileSFname = "TopPtSF";
    TString hname      = "ratio";
    AddFRhisto(pathToSF + fileSFname, hname, "TopPtSF");

    p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
    p->SetWeight("GetStopTopPt(TgenTop1Pt,TgenTop2Pt)");
    p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]", itSignal, kRed+1,  "0");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1, "0");

  }
  else if(scenario == 13){
    TString pathToSF = "./stop/";
    TString fileSFname = "DeltaPhiEta";
    TString hname = "ratio";
    AddFRhisto(pathToSF + fileSFname, hname, "DeltaPhiEta");

    p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
    p->SetWeight("GetDeltaPhiEtaWeight(TgenDeltaPhi, TgenDeltaEta)");
    p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]", itSignal, kRed+1,  "0");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1, "0");
  }

  else if(scenario == 12){
    if(var == "MET"){
      p->SetVar("TgenMET");
      p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
      p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
      p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]", itSignal, kGreen+2,  "0", "noWeight");
      p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kViolet+1, "0", "noWeight");
      p->SetVar("TgenMET_noSUSY"); 
      p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]**", itSignal, kRed+1,  "0", "noWeight");
      p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]**", itSignal, kBlue+1, "0", "noWeight");
    }
    if(var == "MT2"){
      p->SetVar("TgenMT2");
      p->AddSample("TTbar_Powheg",          "tt Powheg",     itSignal, 1,       "0", "noWeight");
      p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1, "0", "noWeight");
      p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]", itSignal, kGreen+2,  "0", "noWeight");
      p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kViolet+1, "0", "noWeight");
      p->SetVar("TgenMT2_noSUSY"); 
      p->AddSample("T2tt_175_1_FS", "T2tt [175, 1]**", itSignal, kRed+1,  "0", "noWeight");
      p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]**", itSignal, kBlue+1, "0", "noWeight");
    }
/*    p->SetVar("DilepPt(TgenNeu1Pt, TgenNeu1Eta, TgenNeu1Phi, TgenNeu1M, TgenNeu2Pt, TgenNeu2Eta, TgenNeu2Phi, TgenNeu2M)");
    p->AddSample("T2tt_175_1_FS", "T2tt [175, 1] DiNuPt", itSignal, kViolet+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50] DiNuPt", itSignal, kOrange+1, "0", "noWeight");
    p->SetVar("DilepPt(TgenLsp1Pt, TgenLsp1Eta, TgenLsp1Phi, TgenLsp1M, TgenLsp2Pt, TgenLsp2Eta, TgenLsp2Phi, TgenLsp2M)");
    p->AddSample("T2tt_175_1_FS", "T2tt [175, 1] DiLspPt", itSignal, kCyan+1,  "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50] DiLspPt", itSignal, kGreen+6, "0", "noWeight");
*/  }

  else if(scenario == 99){
    if(name == "TopStopPt"){
      p->AddSample("TTbar_Powheg",          "t#bar{t}",     itSignal, 1,       "0", "noWeight");
      p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
      p->AddSample("TTbar_Powheg",          "t#bar{t}, top p_{T} reweighted",     itSignal, kGreen+1,       "0");
      p->AddSample("T2tt_175_1_FS",         "T2tt [175, 1]", itSignal, kRed+1,  "0", "noWeight");
      p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1, "0", "noWeight");
    }
    else{
      p->AddSample("TTbar_Powheg",          "t#bar{t}",     itSignal, 1,       "0", "noWeight");
      p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
      p->AddSample("TTbar_Powheg",          "t#bar{t}, top p_{T} reweighted",     itSignal, kGreen+1,       "0");
    }
  }

 //>>> Options and drawing...
  TString options = "hist";
  if(scenario < 3)  p->GetHisto("tt Powheg")->SetDrawStyle("pe");

  p->SetLegendPosition(0.6, 0.72, 0.92, 0.92);
  if(var.Contains("DeltaPhi"))  p->SetLegendPosition(0.2, 0.72, 0.62, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(2 - rmax); p->SetRatioMax(rmax);

  options = "hist";
  p->doSetLogy = false;
  p->DrawComp(name, 1,options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 1,options);

  delete p;
}

