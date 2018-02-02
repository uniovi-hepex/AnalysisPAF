//###################################################################################################
//
// Draw genMET for ttbar at gen level
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
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "0");

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
//TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/forComp/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString var = "TgenMET";
TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";

//=== The main function
void genMETttbar(){
    DrawPlot(var, dilepgen, chan, 20, 0, 250, "gen MET [GeV]",    "genMETttbar");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp/");
  p->verbose = true;
  p->doYieldsInLeg = false;
  p->SetVarName(name);

  //>>> This sample is always on the comparison
  p->AddSample("TTbar_Powheg", "tt Powheg",                   itSignal, 1,       "0", "noWeight");
  //p->AddSample("TTbar_Powheg", "tt Powheg",                   itSignal, 1,       "0", "noWeight");
  p->AddSample("TTJets_MLM",            "tt MLM",             itSignal, kBlue+1,  "0", "noWeight");
  p->SetWeight("TgenWeight");
  p->AddSample("TTJets_aMCatNLO",       "tt aMCatNLO",        itSignal, kRed+1,  "0");

  TString options = "pe";
  p->SetLegendPosition(0.6, 0.72, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(0.9); p->SetRatioMax(1.1);
  p->doSetLogy = false;
  p->DrawComp(name, 1, options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 0, options);

  delete p;
}

