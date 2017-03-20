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

void runComp(){
 // for(Int_t i = 0; i < 9; i++){
//  DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", i);
  //DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "MET [GeV]", 3);
  DrawPlot("TMT2", "TMT2 > 0 && TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 120, "M_{T2} [GeV]", 4);
//  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, Int_t pl){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;
  p->doYieldsInLeg = false;

  p->AddSample("TTbar_PowhegLHE", "Nominal", itSignal, 1, 0.05);
  if(pl == 0){
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kGreen+1, 0.05, "JESUp");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kRed+1, 0.05, "JESDown");
    p->DrawComp("JES", 1);
  }
  if(pl == 1){
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kGreen+1, 0.05, "BtagUp");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kRed+1, 0.05, "BtagDown");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kBlue+1, 0.05, "MisTagUp");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kPink, 0.05, "MisTagDown");
    p->DrawComp("Btag", 1);
  }
  if(pl == 2){
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kGreen+1, 0.05, "LepEffUp");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kRed+1, 0.05, "LepEffDown");
    p->DrawComp("LepEff", 1);
  }
  if(pl == 3){
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kGreen+1, 0.05, "PUUp");
    p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, kRed+1, 0.05, "PUDown");
    p->DrawComp("PU", 1);
  }
  if(pl == 4){
    p->AddSample("TTbar_Powheg_ueUp",   "UEUp", itSignal, kGreen+1, 0.05);
    p->AddSample("TTbar_Powheg_ueDown", "UEDown", itSignal, kRed+1, 0.05);
    p->DrawComp("UE", 1);
  }
  if(pl == 5){
    p->AddSample("TTbar_Powheg_isrUp", "ISRUp", itSignal, kGreen+1, 0.05);
    p->AddSample("TTbar_Powheg_isrDown", "ISRDown", itSignal, kRed+1, 0.05);
    p->DrawComp("ISR", 1);
  }
  if(pl == 6){
    p->AddSample("TTbar_Powheg_fsrUp", "FSRUp", itSignal, kGreen+1, 0.05);
    p->AddSample("TTbar_Powheg_fsrDown", "FSRDown", itSignal, kRed+1, 0.05);
    p->DrawComp("FSR", 1);
  }
  if(pl == 7){
    p->AddSample("TTbar_Powheg_Herwig", "powheg+herwig", itSignal, kRed+1, 0.05);
    p->DrawComp("HAD", 1);
  }
  if(pl == 8){
    p->AddSample("TTJets_aMCatNLO", "aMCatNLO", itSignal, kRed+1, 0.05);
    p->DrawComp("NLO", 1);
  }

  //p->PrintYields();
  //p->PrintSystYields();
  delete p;

}

