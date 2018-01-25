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
TString dilepgen = "TNgenLeps >= 2";

//=== The main function
void genTTbarMasses(){
    //DrawPlot(var, dilepgen, chan, 20, 0, 250, "gen MET [GeV]",    "genMETttbar");
    DrawPlot("TgenLep0_Pt",  dilepgen, chan, 15, 0, 200, "gen Leading lep p_T [GeV]",    "genLep0Pt");
    DrawPlot("TgenLep1_Pt",  dilepgen, chan, 15, 0, 200, "gen Subleading lep p_T [GeV]", "genLep1Pt");
    DrawPlot("TgenTop1M",       dilepgen, chan, 40, 170, 180, "gen Top Mass", "Top1Mass");
    DrawPlot("TgenTop2M",       dilepgen, chan, 40, 170, 180, "gen Top Mass", "Top2Mass");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp/");
  p->verbose = true;
  p->doYieldsInLeg = false;
  p->SetVarName(name);

  //>>> This sample is always on the comparison
  p->AddSample("TTbar_Powheg", "mtop = 172.5",                itSignal, 1,       "0", "noWeight");
  p->AddSample("TTbar_Powheg_mtop1755", "mtop = 175.5",       itSignal, kRed+1,  "0", "noWeight");
  p->AddSample("TTbar_Powheg_mtop1695", "mtop = 169.5",       itSignal, kBlue+1,  "0", "noWeight");

  TString options = "pe";
  p->SetLegendPosition(0.6, 0.72, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(0.8); p->SetRatioMax(1.2);
  p->doSetLogy = false;
  p->DrawComp(name, 1, options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 0, options);

  delete p;
}

