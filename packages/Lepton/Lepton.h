#ifndef LEPTON_H
#define LEPTON_H 1

#include "TLorentzVector.h"
#include "TString.h"
#include "ElecScaleClass.h"

enum genLepMatch{kLGMtoGenLep, kLGMgood, kLGMfake, kLGMflip, kLGMconv, kLGMtoGenB, kLGMother};

class Lepton : public TObject{
  public:
    Lepton(){
      p = TLorentzVector(0,0,0,0);
      charge = 0;
      type = -1;
      isElec = 0; isMuon = 0; isTau = 0;
      Mid = 0;
    };
    //Lepton(const Lepton &l): p(l.p), charge(l.charge), type(l.type), index(l.index){ };
 Lepton(TLorentzVector vec, 
        Int_t ch = 0,
        Int_t ty = -1,
        Int_t tdecayMode   = -1,
        Int_t tidDecayMode = -1, 
        Int_t tidMVA       = -1, 
        Int_t tidAntiE     = -1, 
        Int_t tidAntiMu    = -1 ):
      charge(ch),
      type(ty),
      decayMode   (tdecayMode  ),
      idDecayMode (tidDecayMode), 
      idMVA       (tidMVA      ), 
      idAntiE     (tidAntiE    ), 
      idAntiMu    (tidAntiMu   ) 
      {
        p = vec;
        
        
        if(ty == 0){ isMuon = 1; isElec = 0; isTau = 0;}
        if(ty == 1){ isMuon = 0; isElec = 1; isTau = 0;}
      if(ty == 2){ isMuon = 0; isElec = 0; isTau = 1;}
    }
    ~Lepton(){};
    TLorentzVector p;
    Int_t charge;
    Int_t decayMode;
    Int_t idDecayMode;
    Int_t idMVA;
    Int_t idAntiE;
    Int_t idAntiMu;
    Int_t type;
    Int_t Mid; // mother Id, for gen leptons
    Bool_t isElec;
    Bool_t isMuon;
    Bool_t isTau;
    Float_t GetSF(Int_t sys = 0);
    void SetSF(Float_t val);
    void SetSFerr(Float_t val);
    void SetIso(Float_t val){ Iso = val;}
    void SetR9(Float_t val){ R9 = val;}
    void SetEnergyUnc(Float_t val){ EnergyUnc = val;}
    void SetGenMatch(Int_t m){ genMatch = m;}

    Float_t Pt(){return p.Pt();}
    Float_t Eta(){return p.Eta();}
    Float_t Phi(){return p.Phi();}
    Float_t E(){return p.E();}
    Float_t GetIso(){ return Iso;} 
    Float_t GetR9(){return R9;}
    Float_t GetEnergyUnc(){return EnergyUnc;}
    Int_t   GetGenMatch(){return genMatch;}
    

  protected:
    Float_t SF;
    Float_t SFerr;
    Float_t Iso;
    Float_t R9;
    Float_t EnergyUnc;
    Int_t   genMatch;

    //ClassDef(Lepton, 0);
};

#endif
