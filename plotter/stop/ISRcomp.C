//##################################################################################################################    
// 
// Print plots with nominal and systematic variations for ttbar sample
//    root -l -b -1 'SystematicPlots("ElMu")'
//
//##################################################################################################################    
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PDFunc.C"
#include "PlotterFunctions.C"

//=== Functions
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name);
void DrawComp(TString systName, Bool_t VariedSample = false);

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/T2tt/ISR/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";
const Int_t ngbins = 26;
Float_t gbins[ngbins+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200};
Plot* p = nullptr;
PDFunc *pdf = nullptr;

//=== Main function
void ISRcomp(TString chan = "ElMu"){
  DrawPlot("TNJets", Dilepton, chan, 8, 0, 7, 0, "[#Jets|#BJets]", "NJets"); 
  DrawPlot("NBtagNJets(TNJets,TNBtags)", Dilepton, chan, 16, 0, 15, 0, "[#Jets|#BJets]", "NBtagsNJets"); 
  DrawPlot("TMT2", BaselineCut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2_dilep");
}

//=== Plotting function
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name){
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetOutputName("");
  p->SetPlotFolder("output/ISR/");
  p->verbose = false;  p->doYieldsInLeg = false;
  p->SetLegendTextSize(0.050); p->SetLegendPosition(0.50, 0.70, 0.93, 0.93);
  p->SetVarName(name);
  //>>> Ratio range for each syst variation
  Float_t RatioMax = 1.1; 

  if(var.Contains("NBtagNJets")){
    p->doSetLogy = false;
    p->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  }
  else if(name == "NJets"){
    RatioMax = 1.2;
    p->doSetLogy = false;
  }
  Float_t RatioMin = 2 - RatioMax;
  p->SetRatioMin(RatioMin); p->SetRatioMax(RatioMax);

  //>>> Add nominal samples
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50] no ISR correction", itSignal, 1);
  p->GetHisto("T2tt[225,50] no ISR correction")->SetDrawStyle("p");
  p->SetWeight("TWeight*TISRweight");
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50] ISR weighting", itSignal, kRed+1);

  p->DrawComp("ISR", 0, "hist");
  if(p) delete p;
  return;
}


