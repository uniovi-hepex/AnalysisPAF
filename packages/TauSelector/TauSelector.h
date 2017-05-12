#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "LeptonSF.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "LinkDef.h"

class TauSelector : public PAFChainItemSelector{
  
 public:
  
  TauSelector();
  virtual ~TauSelector() {}
  virtual void InsideLoop();
  virtual void Initialise();
  virtual void Summary();
  
  std::vector<Lepton> selTaus;
  std::vector<Lepton> looseTaus;
  std::vector<Lepton> vetoTaus;
  
  std::vector<Lepton> looseLeptons;
  
 protected:
  
  LeptonSF* TauSF;
  Bool_t    gIsData;
  Int_t     gSelection;
  TString   localPath;
  
  // TauGood
  Lepton tL;
  Int_t   nLep;
  TLorentzVector tP;
  Float_t pt;
  Float_t eta;
  Int_t type;
  Int_t charge;
  Int_t decayMode;
  Int_t idDecayMode;
  Int_t idMVA;
  Int_t idAntiE;
  Int_t idAntiMu;
  
  Int_t nTau;
  
  Int_t nSelTaus;
  Int_t nVetoTaus;
  Int_t nLooseTaus;
  
  void GetTauVariables(Int_t i);
  
  Bool_t isGoodTau(Lepton tau);
  Bool_t isLooseTau(Lepton tau);
  Bool_t isVetoTau(Lepton tau);
  
  ClassDef(TauSelector, 0);
};
