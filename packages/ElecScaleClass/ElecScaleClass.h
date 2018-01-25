#ifndef ELECSCALE_h 
#define ELECSCALE_h 1

#include "TString.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

std::vector<TString> TStringToVector(const TString &t, char separator = ',');

struct ElecScaleVal{
  unsigned runMin;
  unsigned runMax;
  unsigned category;
  float    uncertainty;
};

ElecScaleVal GetElecScaleVal(unsigned category, unsigned runMin, unsigned runMax, float unc = 0);

class ElecScaleClass{
  public:
  ElecScaleClass(TString _filename){
    SetFileName(_filename, false);
    loadCategories();
  }

  void SetFileName(TString f, bool loadCat = true){
    verbose = false;
    filename = f; 
    if(loadCat) loadCategories();
  }

  static unsigned GetCategory(float eta, float pt, float R9);
  static unsigned GetCategory(const unsigned &etaCat, unsigned &ptCat, const unsigned &R9Cat);
  static unsigned GetEtaCat(float eta);
  static unsigned GetPtCat( float pt, float eta, float R9);
  static unsigned GetPtCat( float pt, unsigned etaCat, unsigned R9Cat);
  static unsigned GetR9Cat( float R9);

  static unsigned GetCategoryFromLine(const TString &line);
  static unsigned GetRunMinFromLine(  const TString &line);
  static unsigned GetRunMaxFromLine(  const TString &line);
  static float    GetUncFromLine(     const TString &line);

  const ElecScaleVal &FindValues(unsigned cat, unsigned run = 1); 
  float GetUnc(unsigned cat, unsigned run = 1);
  float GetUnc(float pt, float eta, float R9, unsigned run = 1);
  
  private:
  vector<ElecScaleVal> ElecCorrCategories;
  void loadCategories();
  void AddCategory(TString line);
  TString filename;
  bool verbose;
};

#endif
