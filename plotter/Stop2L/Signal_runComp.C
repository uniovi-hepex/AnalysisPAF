R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/Baseline/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar13/Baseline/";
TString NameOfTree = "tree";

void Signal_runComp(){
  //DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]");
  //DrawPlot("TMT2", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 > 0 ", "ElMu", 28, 0, 140, "M_{T2} [GeV]");
  //DrawPlot("TJet_Pt[0]", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 > 0 ", "ElMu", 27, 30, 300, "Leading Jet p_{T} [GeV]");
  DrawPlot("TMT2", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 10, 0, 120, "M_{T2} [GeV]");
//  DrawPlot("TMT2", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 > 0 ", "ElMu", 10, 0, 120, "M_{T2} [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetVarName("FSFS_MT2");
  p->verbose = false;

  //p->AddSample("T2tt_200_50_FS_summer", "S_200_50", itSignal, kGreen+2, 0.05);
  p->AddSample("TTbar_PowhegLHE", "Powheg", itSignal, 1, 0.05);
  p->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kBlue, 0.05);
  p->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kRed, 0.05);

//  p->AddSample("T2tt_225_50_FS_summer", "FullSim", itSignal, 1, 0.05);
//  p->AddSample("T2tt_mStop225_mLsp50", "FastSim", itSignal, kRed, 0.05);

//  p->AddSample("TTbar_PowhegLHE", "Powheg", itSignal, 1, 0.05);
//  p->AddSample("TTJets_MLM", "MLM-FullSim", itSignal, kBlue, 0.05);
//  p->AddSample("TTJets_MLM_FastSim", "MLM-FastSim", itSignal, kRed, 0.05);

  //p->PrintYields();
  //p->DrawComp("0", 1, 0);
  p->DrawComp("norm", 1, 1);
  p->doSetLogy = true;
  p->DrawComp("norm_log", 1, 1);
  //p->PrintSystYields();
  delete p;

}

