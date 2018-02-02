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
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString Xtitle, TString name = "0");

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString var = "TgenMT2";
TString sample = "TTbar_Powheg_WithTausInfo";
TString sample2 = "T2tt_175_1_FS";
TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;

//=== The main function
void genMT2taus(){
    //DrawPlot(var, dilepgen, chan, 20, 0, 250, "gen MET [GeV]",    "genMETttbar");
    DrawPlot(var, dilepgen, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "genMT2");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString Xtitle, TString name){
  Plot* p;
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins, "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp/");
  p->verbose = true;
  p->doYieldsInLeg = false;
  p->SetVarName(name);

  p->SetWeight("IsEmuFromWW(TgenLep1_Mid, TgenLep0_Mid)");
  p->AddSample(sample, "TTbar[WW --> #font[12]{ll}]",             itSignal, 1,        "0");
  p->AddSample(sample2, "T2tt[WW --> #font[12]{ll}]",             itSignal, 1,        "0");
  p->SetWeight("IsEmuFromWTau(TgenLep1_Mid, TgenLep0_Mid)");
  p->AddSample(sample, "TTbar[WW --> #font[12]{l}#tau --> #font[12]{ll}]",    itSignal, kBlue+1,  "0");
  p->AddSample(sample2, "T2tt[WW --> #font[12]{l}#tau --> #font[12]{ll}]",    itSignal, kBlue+1,  "0");
  p->SetWeight("IsEmuFromTauTau(TgenLep1_Mid, TgenLep0_Mid)");
  p->AddSample(sample, "TTbar[WW --> #tau#tau --> #font[12]{ll}]", itSignal, kRed+1,   "0");
  p->AddSample(sample2, "T2tt[WW --> #tau#tau --> #font[12]{ll}]", itSignal, kRed+1,   "0");
  p->GetHisto("TTbar[WW --> #font[12]{ll}]")->SetMarkerStyle(0);
  p->GetHisto("TTbar[WW --> #font[12]{ll}]")->SetDrawStyle("hist");
 
  p->SetLineStyle("T2tt[WW --> #font[12]{ll}]", 2);
  p->SetLineStyle("T2tt[WW --> #font[12]{l}#tau --> #font[12]{ll}]", 2);
  p->SetLineStyle("T2tt[WW --> #tau#tau --> #font[12]{ll}]", 2);

  TString options = "hist, ratiocolors";
  p->SetLegendPosition(0.6, 0.72, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(0.2); p->SetRatioMax(1.8);
  p->doSetLogy = false;
  p->DrawComp(name, 1, options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 0, options);

  delete p;
  return;
}

