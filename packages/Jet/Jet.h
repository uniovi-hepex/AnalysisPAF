#ifndef JET 
#define JET 1

#include <iostream>
#include "TLorentzVector.h"

class Jet{

  public:
    Jet(){};
    Jet(TLorentzVector vec, Float_t btag_csv, Int_t Id = -1, Int_t mcFlavour = 0){
      p = vec;
      csv = btag_csv;
      id = Id;
      flavmc = mcFlavour;
      //InitSyst();
    }
    ~Jet(){};


    Bool_t isBtag;
    TLorentzVector p;
    TLorentzVector mcp;
    Bool_t id;
    Int_t flavmc;
    Float_t csv; 

    // For systematics
    Float_t pTJESUp;
    Float_t pTJESDown;
    Float_t pTJERUp;
    Float_t pTJERDown;
    Bool_t  isBtag_BtagUp;
    Bool_t  isBtag_BtagDown;
    Bool_t  isBtag_MisTagUp;
    Bool_t  isBtag_MisTagDown;

    void InitSyst();
    Float_t Pt(){ return p.Pt();}
    Float_t Eta(){ return p.Eta();}
    Float_t Phi(){ return p.Phi();}
    Float_t E(){ return p.E();}
    void SetMCjet(TLorentzVector p){ mcp = p;}

};

#endif
