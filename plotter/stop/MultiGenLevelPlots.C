//###################################################################################################
//
// Draw multple plots at gen level
// Uses minitrees produced only with gen info, no cuts on reco variables, no skimmed n-tupples
// Select your options below:
//    genModelingT2tt
//    recoModelingT2tt
//    topModelingT2tt
//    genxqcutSimple
//    recoxqcut
//    topxqcut
//    recoFastFull
//    genStopMass
//    recoStopMass
//  Ussage:
//    root -l -b -q 'GenLevelPlots.C("")'
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

//=== Functions
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "0", TString lab = "");

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
//TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/forComp/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";

void DrawPlot(TString name, TString xtitle = "");

MultiPlot* mp;

//=== The main function
void MultiGenLevelPlots(TString lab = ""){
  mp = new MultiPlot();
  mp->SetPath(pathToTree); mp->SetTreeName(NameOfTree);
  mp->SetPlotFolder("output/genMultiComp/");
  mp->doYieldsInLeg = false;

  mp->AddDistribution("MT2",      "TgenMT2",     dilepgen, chan, 20, 0, 120);
  mp->AddDistribution("MET",      "TgenMET",     dilepgen, chan, 20, 0, 250);
  mp->AddDistribution("LepPt",    "TgenLep0_Pt", dilepgen, chan, 20, 0, 200);
  mp->AddDistribution("DilepPt",  "GetDilepPtDPhi(TgenLep0_Pt, TgenLep1_Pt, TgenDeltaPhi)", dilepgen, chan, 20, 0, 250);
  mp->AddDistribution("DeltaPhi", "TDeltaPhi", dilepgen, chan, 15, 0, 3.2);
  mp->AddDistribution("DeltaEta", "TDeltaEta", dilepgen, chan, 15, 0, 3.2);

  TString weight_TopPt = "GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)";
  TString weight_aMCatNLO = "TgenWeight";
  TString weight = "1";

  mp->AddSample("TTbar_Powheg",          "tt Powheg Gen", itSignal, 1,          "0", weight);
  //mp->AddSample("TTJets_aMCatNLO",       "tt aMCatNLO",   itSignal, kViolet-1,  "0", weight_aMCatNLO);
  mp->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1,    "0", weight);
  mp->AddSample("T2tt_175_1_FS",         "T2tt [175,1]",  itSignal, kRed+1,     "0", weight);
  mp->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1,    "0", weight);
  //mp->AddSample("T2tt_200_50_FS_summer", "T2tt [200,50]", itSignal, kGreen+2,   "0", "weight");
  //mp->AddSample("T2tt_250_50_FS_summer", "T2tt [250,50]", itSignal, kBlue+2,   "0",  "weight");
  
  Int_t   nplots = 6;
  TString plotname[] = {"MT2",        "MET",     "LepPt",            "DilepPt",                         "DeltaPhi",       "DeltaEta"};
  TString plotxtit[] = {"gen M_{T2}", "gen MET", "gen Lepton p_{T}", "gen p_{T}^{\\ell\\ell} [GeV]", "gen #Delta#phi", "gen #Delta#eta"};

  for(Int_t i = 0; i < nplots; i++) DrawPlot(plotname[i], plotxtit[i]);
}

void DrawPlot(TString name, TString xtitle){
  mp->SetVarName(name);
  mp->SetPlot(name, xtitle);

  mp->doSetLogy = false;
  mp->DrawComp("", 1);
  mp->doSetLogy = true;
  mp->SetPlotMinimum(5e-4);
  mp->SetPlotMaximum(0.5);
  mp->DrawComp("log",1);
}

