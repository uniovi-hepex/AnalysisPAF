#ifndef PLOTTOPYC_H
#define PLOTTOPYC_H 1

#include"Plot.h"


Plot* PlotToPyC(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t* bins = 0, TString tit = "title", TString xtit = "VAR"){
  return new Plot(variable, cuts, channel, nbins, bins, tit, xtit);
}
#endif
