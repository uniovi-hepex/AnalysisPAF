R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

//=== Constants
TString path = "/nfs/fanae/user/juanr/AnalysisPAF/StopTop_temp/T2tt/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TNVetoLeps < 3 && TPassTrigger && TPassMETfilters";

//=== The main function
void METcomp(){
  TString var = "TMET";
  TString cut = BaselineCut;
  TString chan = "ElMu";
  Float_t nbins = 20;
  Float_t bin0 = 0;
  Float_t binN = 400;
  TString weight = "TWeight*TBtagSFFS";

  TString sampleName = "T2tt_mStop175_mLsp1";

  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", "[GeV]");
  p->SetPath(path); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genMET/");
  p->verbose = false;
  p->doYieldsInLeg = false;
  p->SetVarName("METcomp");
  p->SetWeight(weight);
  

  //>>> This sample is always on the comparison
  p->AddSample(sampleName, "FastSim MET", itSignal, 1,  "0", "noWeight");

  Histo* hPf  = GetHisto(path, sampleName, NameOfTree, "TPfMET", cut, chan, weight, "", nbins, bin0, binN);
  Histo* hGen = GetHisto(path, sampleName, NameOfTree, "TgenMET", cut, chan, weight, "", nbins, bin0, binN);

  hPf ->SetType(itSignal); hPf ->SetProcess("pfMET");  hPf ->SetTag("pfMET");  hPf ->SetColor(kBlue+1);
  hGen->SetType(itSignal); hGen->SetProcess("genMET"); hGen->SetTag("genMET"); hGen->SetColor(kRed);

  p->AddToHistos(hPf); p->AddToHistos(hGen);

  p->SetLegendPosition(0.6, 0.75, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(0.6); p->SetRatioMax(1.4);
  p->doSetLogy = false;
  p->DrawComp("", 1);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 1);

  delete p;
}

