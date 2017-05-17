R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, Int_t i = 0);
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/Baseline/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
TString NameOfTree = "tree";

void comp(){
 // for(Int_t i = 0; i < 9; i++){
//  DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", i);
  //DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", 3);
  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "M_{T2} [GeV]", 4);
//  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, Int_t pl){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;
  p->doYieldsInLeg = false;

  p->AddSample("TTbar_PowhegLHE", "Nominal", itSignal, 1, 0.05);
  p->AddSample("TTbar_PowhegLHE", "Scale Up", itSignal, kRed+1, 0.05, "ScaleUp");
  p->AddSample("TTbar_PowhegLHE", "Scale Down", itSignal, kBlue+1, 0.05, "ScaleDown");
  p->DrawComp("Scale", 1);

  //p->PrintYields();
  //p->PrintSystYields();
  delete p;

}

