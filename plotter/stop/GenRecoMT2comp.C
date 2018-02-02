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
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;

//=== The main function
void GenRecoMT2comp(){
  TString var = "TgenMT2"; TString cut = dilepgen; chan = "All"; Int_t nbins = ngbins; Float_t *bins = gbins;

  Plot* p = new Plot(var, cut, chan, nbins, bins, "Title", "M_{T2} [GeV]");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp/");
  p->verbose = false;
  p->doYieldsInLeg = false;
  p->SetVarName("MT2_GenRecoComp");
  p->SetWeight("TWeight");

  p->AddSample("TTbar_Powheg", "Gen  ttbar", itSignal, 1,    "0", "noWeight");
  p->SetCut("TNSelLeps >= 2"); p->SetVar("TMT2");
  p->AddSample("TTbar_Powheg", "Reco ttbar", itSignal, 1,    "0", "noWeight");

  p->SetCut("TNgenLeps >= 2"); p->SetVar("TgenMT2");
  p->AddSample("T2tt_200_50_FS_summer", "Gen  T2tt[200,50]", itSignal, kGreen+1,    "0", "noWeight");
  p->SetCut("TNSelLeps >= 2"); p->SetVar("TMT2");
  p->AddSample("T2tt_200_50_FS_summer", "Reco T2tt[200,50]", itSignal, kGreen+1,    "0", "noWeight");

  p->SetCut("TNgenLeps >= 2"); p->SetVar("TgenMT2");
  p->AddSample("T2tt_225_50_FS_summer", "Gen  T2tt[225,50]", itSignal, kBlue+1,    "0", "noWeight");
  p->SetCut("TNSelLeps >= 2"); p->SetVar("TMT2");
  p->AddSample("T2tt_225_50_FS_summer", "Reco T2tt[225,50]", itSignal, kBlue+1,    "0", "noWeight");

  p->SetCut("TNgenLeps >= 2"); p->SetVar("TgenMT2");
  p->AddSample("T2tt_250_50_FS_summer", "Gen  T2tt[250,50]", itSignal, kRed+1,    "0", "noWeight");
  p->SetCut("TNSelLeps >= 2"); p->SetVar("TMT2");
  p->AddSample("T2tt_250_50_FS_summer", "Reco T2tt[250,50]", itSignal, kRed+1,    "0", "noWeight");

  p->SetLineStyle("Reco ttbar", 2);
  p->SetLineStyle("Reco T2tt[200,50]", 2);
  p->SetLineStyle("Reco T2tt[225,50]", 2);
  p->SetLineStyle("Reco T2tt[250,50]", 2);

  TString options = "hist,ratiocolors";

  p->SetLegendPosition(0.65, 0.62, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  Float_t rMax = 2.5; Float_t rMin = 0.5;

  //rMin = 2 - rMax;
  p->SetRatioMin(rMin); p->SetRatioMax(rMax);

  p->doSetLogy = false;
  p->DrawComp("", 1,options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 1,options);

  delete p;
}

