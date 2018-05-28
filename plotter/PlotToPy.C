#ifndef PLOTTOPY_H
#define PLOTTOPY_H 1

#include"Plot.h"
#include"PDFunc.C"


Plot* PlotToPy(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t bin0 = 0, Float_t binN = 0, TString tit = "title", TString xtit = "VAR"){
  return new Plot(variable, cuts, channel, nbins, bin0, binN, tit, xtit);
}


PDFunc* PDFToPy(TString path, TString sample, TString tn,  TString _cut, TString _chan, TString _var, Int_t nb, Float_t b0 = 0, Float_t bN = 300){
  return new PDFunc(path, sample, tn,  _cut,  _chan,  _var,  nb,  b0,  bN, "");
}

#endif
