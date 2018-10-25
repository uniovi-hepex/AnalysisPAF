#ifndef PLOTTOPYC_H
#define PLOTTOPYC_H 1

#include"Plot.h"
#include"PDFunc.C"


Plot* PlotToPyC(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t* bins = 0, TString tit = "title", TString xtit = "VAR"){
  return new Plot(variable, cuts, channel, nbins, bins, tit, xtit);
}


PDFunc* PDFToPyC(TString path, TString sample, TString tn, TString _cut, TString _chan, TString _var, Int_t nb, Float_t *b0 = 0, TString filename = ""){
  return new PDFunc(path, sample, tn,  _cut,  _chan,  _var,  nb,  b0,  filename);
}
#endif
