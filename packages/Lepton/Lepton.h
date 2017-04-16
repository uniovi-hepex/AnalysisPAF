#ifndef LEPTON_H
#define LEPTON_H 1

#include "TLorentzVector.h"
#include "TString.h"

class Lepton : public TObject{
  public:
    Lepton(){
      p = TLorentzVector(0,0,0,0);
      charge = 0;
      type = -1;
      isElec = 0; isMuon = 0; isTau = 0;
    };
    //Lepton(const Lepton &l): p(l.p), charge(l.charge), type(l.type), index(l.index){ };
    Lepton(TLorentzVector vec, Int_t ch = 0, Int_t ty = -1){
      p = vec;
      charge = ch;
      type = ty;

      if(ty == 0){ isMuon = 1; isElec = 0; isTau = 0;}
      if(ty == 1){ isMuon = 0; isElec = 1; isTau = 0;}
      if(ty == 2){ isMuon = 0; isElec = 0; isTau = 1;}
    }
    ~Lepton(){};
    TLorentzVector p;
    Int_t charge;
    Int_t type;
    Bool_t isElec;
    Bool_t isMuon;
    Bool_t isTau;
    Float_t GetSF(Int_t sys = 0);
    void SetSF(Float_t val);
    void SetSFerr(Float_t val);
    void SetIso(Float_t val){ Iso = val;}
    Float_t GetIso(){ return Iso;} 

    Float_t Pt(){return p.Pt();}
    Float_t Eta(){return p.Eta();}
    Float_t Phi(){return p.Phi();}
    Float_t E(){return p.E();}

  protected:
    Float_t SF;
    Float_t SFerr;
    Float_t Iso;

    //ClassDef(Lepton, 0);
};

#endif
