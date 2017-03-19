#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>


class TWAnalysis : public PAFChainItemSelector{
  public:
    TWAnalysis();
    virtual ~TWAnalysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> genLeptons  ;
    std::vector<Lepton> selLeptons  ;
    std::vector<Jet> selJets ;

    TTree* fTW1j1b;
    void SetTWVariables(); 


    void GetVarsFromTop();
    void CalculateTWVariables();

  protected:
    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsTW;

    Bool_t  TIsOSDilep;
    Int_t   TNJets;
    Int_t   TNBtags;
    void    GetMET();
    Double_t getDilepMETJetPt();
    Double_t getLep1METJetPt();
    Double_t getPtSys(TLorentzVector*, int);
    Double_t getDPtDilep_JetMET();
    Double_t getDPtDilep_MET();
    Double_t getDPtLep1_MET();
    Double_t getDeltaPt(vector<TLorentzVector> col1, vector<TLorentzVector> col2);
    
    Float_t TWeight        ;
    Float_t DilepMETJetPt  ;
    Float_t Lep1METJetPt   ;
    Float_t DPtDilep_JetMET;
    Float_t DPtDilep_MET   ;
    Float_t DPtLep1_MET    ;
    Float_t TMET;
    Float_t TMET_Phi;

    ClassDef(TWAnalysis, 0);
};
