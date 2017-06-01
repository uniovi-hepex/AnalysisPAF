#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

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
    std::vector<Jet> Jets15  ;
    std::vector<Jet> vetoJets;

    TTree* fTW1j1b;
    void SetTWVariables(); 


    void GetVarsFromTop();
    void CalculateTWVariables();
    void SetHistos();
    void GetGenStuff();

  protected:
    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsTW;
    TMVA::Reader* BDT;


    Bool_t  TIsOSDilep;
    Int_t   TNJets;
    Int_t   TNBtags;
    void    GetMET();
    Double_t getDilepMETJetPt();
    Double_t getDilepJetPt();
    Double_t getLep1METJetPt();
    Double_t getPtSys(TLorentzVector*, int);
    Double_t getDilepMETJet1Pz();
    Double_t getPzSys(TLorentzVector*, int);
    Double_t getDPtDilep_JetMET();
    Double_t getDPtDilep_MET();
    Double_t getDPtLep1_MET();
    Double_t getDeltaPt(vector<TLorentzVector> col1, vector<TLorentzVector> col2);
    void     get20Jets();
    Double_t getSysM();
    Double_t getM(vector<TLorentzVector>);
    void     setBDT();

    Float_t TWeight        ;
    Float_t DilepMETJetPt  ;
    Float_t Lep1METJetPt   ;
    Float_t DPtDilep_JetMET;
    Float_t DPtDilep_MET   ;
    Float_t DPtLep1_MET    ;
    Float_t DilepMETJet1Pz ; 
    Float_t TMET           ;
    Float_t TMET_Phi       ;
    Float_t TJet2csv       ;
    Float_t TJet2pt        ;
    Float_t MSys           ; 
    Float_t TBDT           ;
    Float_t THT            ; 
    Float_t DilepJetPt     ;

    Int_t nLooseCentral  ;
    Int_t nLooseFwd      ;
    Int_t nBLooseCentral ;
    Int_t nBLooseFwd     ;
    
    Float_t   TJet1_pt   ;
    Float_t   TJet1_Dphi ;
    Float_t   TJet1_pz   ;
    Float_t   TJet1_E    ;
    Float_t   TLep1_pt   ;
    Float_t   TLep1_Dphi ;
    Float_t   TLep1_pz   ;
    Float_t   TLep1_E    ;
    Float_t   TLep2_pt   ;
    Float_t   TLep2_Dphi ;
    Float_t   TLep2_pz   ;
    Float_t   TLep2_E    ;
    Float_t   C_jll      ;
    
    TH1F*   fHGenB_eta;
    TH1F*   fHGenB_pt;
    TH1F*   fHGenB_RecoJetDR;
    TH1F*   fHNGenB_unmatched;
    TH2F*   fHGenB_eta_pt;
    TH1F*   fHNCSV;
    ClassDef(TWAnalysis, 0);
};
