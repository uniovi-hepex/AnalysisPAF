#ifndef PLOTTOPY_H
#define PLOTTOPY_H 1

#include"Plot.h"


Plot* PlotToPy(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Double_t bin0 = 0, Double_t binN = 0, TString tit = "title", TString xtit = "VAR"){
  return new Plot(variable, cuts, channel, nbins, bin0, binN, tit, xtit);
}
#endif
