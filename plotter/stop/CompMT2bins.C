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

//=== Functions
void DrawPlot(Int_t nbins, Float_t* bins);

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";
const Int_t ngbins = 26;
Float_t gbins[ngbins+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200};
const Int_t ngbins21 = 21;
Float_t gbins21[ngbins21+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200};
const TString ttbar = "TTbar_Powheg";

//=== Main function
void CompMT2bins(){
  DrawPlot(ngbins, gbins);
  DrawPlot(ngbins21, gbins21);
}

//=== Plotting function
void DrawPlot(Int_t nbins, Float_t* bins){
  TString var = "TMT2"; TString cut = BaselineCut; TString chan = "ElMu";
  TString Xtitle = "M_{T2}";
  Plot* p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetOutputName("");
  p->SetPlotFolder("output/MT2bins/");
  p->verbose = false;  p->doYieldsInLeg = false;
  p->SetLegendTextSize(0.045); p->SetLegendPosition(0.60, 0.70, 0.93, 0.93);
  p->SetVarName(Form("%i", nbins));

  //>>> Ratio range for each syst variation
  Float_t RatioMax = 0.3; Float_t RatioMin = 0;
  p->SetRatioMin(RatioMin); p->SetRatioMax(RatioMax);

  //>>> Add nominal samples
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSignal, 1);
  p->SetPathSignal(pathToTree + "/T2tt/");
  p->AddSample("T2tt_200_50_FS_summer", "T2tt[200,50]", itSignal, kBlue+1); 
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50]", itSignal, kGreen+1); 
  p->AddSample("T2tt_250_50_FS_summer", "T2tt[250,50]", itSignal, kRed+1); 

  p->DrawComp("MT2");
  if(p) delete p;
  return;
}



