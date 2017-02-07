#include "Lepton.h"

//ClassImp(Lepton);

Float_t Lepton::GetSF(Int_t sys){
  if      (sys ==  0) return SF;
  else if (sys == -1) return SF-SFerr;
  else if (sys ==  1) return SF+SFerr;
  else return SF;
}

void Lepton::SetSF(Float_t val){
  SF = val;
}

void Lepton::SetSFerr(Float_t val){
  SFerr = val;
}

