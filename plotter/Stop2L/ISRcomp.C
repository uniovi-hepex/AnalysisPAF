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

void ISRcomp(){
 // for(Int_t i = 0; i < 9; i++){
//  DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", i);
  //DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", 3);
  DrawPlot("TMT2", "!TIsSS && TNVetoLeps < 3 && TMT2 > 0", "ElMu", 10, 0, 140, "M_{T2} [GeV]", 4);
//  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, Int_t pl){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt_noSkim/");
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;
  p->doYieldsInLeg = false;


  Plot* pisr = new Plot(var, "TISRweight*(" + cut + ")", chan, nbins, bin0, binN, "Title2", Xtitle);
  pisr->SetPath(pathToTree); pisr->SetTreeName(NameOfTree);
  pisr->SetPathSignal(pathToTree + "T2tt_noSkim/");
  pisr->AddSample("T2tt_225_50_FS_summer", "ISR_Reweighting", itSignal, kRed, 0.05);
  Histo *isrvar = pisr->GetHisto("ISR_Reweighting");
  p->AddToHistos(isrvar);
  p->AddSample("T2tt_225_50_FS_summer", "No_ISR_Reweighting", itSignal, 1, 0.05);

  p->SetRatioMin(0.95); p->SetRatioMax(1.05);
  p->DrawComp("ISRweights", 1);

  //p->PrintYields();
  //p->PrintSystYields();
  delete p;

}

