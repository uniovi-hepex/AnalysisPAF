#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>


class StopAnalysis : public PAFChainItemSelector{
  public:
    TTree* fTree;

    TH1F* fSumISRJets;  
    TH1F* fISRJets;  
    Int_t TNLHEWeight;
    Float_t TLHEWeight[254];
 
    StopAnalysis();
    virtual ~StopAnalysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> selLeptons  ;
    std::vector<Lepton> vetoLeptons ;
    std::vector<Lepton> genLeptons ;
    std::vector<Jet> selJets ;
    std::vector<Jet> Jets15  ;
    std::vector<Jet> genJets  ;

    void ResetVariables();
    void SetLeptonVariables();
    void SetJetVariables();
    void SetEventVariables();

    void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetMET();
    void GetGenInfo();

    Float_t TrigSF;
    Float_t TrigSFerr;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t gChannel;
    Bool_t passMETfilters;
    Bool_t passTrigger;
    Bool_t isSS;
    Float_t NormWeight;
    Bool_t gIsFastSim = false;
    Int_t gStopMass;
    Int_t gLspMass;
    Float_t NormISRweights;

    // Tree Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TDeltaPhi; 
    Float_t TDeltaEta;
    Float_t TMT2;      // Invariant mass
    Float_t TMET;      // MET
    Float_t TMET_Phi;  // MET phi
    Bool_t  TIsSS;

    Int_t   TNVetoLeps;
    Int_t   TNSelLeps;
    Int_t TChannel;
    Float_t TLep0_Pt;    
    Float_t TLep0_Eta;
    Float_t TLep0_Phi;
    Float_t TLep0_E;
    Int_t   TLep0_Id;
    Float_t TLep0_Charge;
    Float_t TLep1_Pt;    
    Float_t TLep1_Eta;
    Float_t TLep1_Phi;
    Float_t TLep1_E;
    Int_t   TLep1_Id;
    Float_t TLep1_Charge;

    Int_t TNJets;            // Jets...
    Int_t TNBtags;
    Float_t TJet_Pt[20];
    Float_t TJet_Eta[20];
    Float_t TJet_Phi[20];
    Float_t TJet_E[20];
    Int_t TJet_isBJet[20];
    Float_t THT;       // HT

    // For systematics...
    Int_t   TNJetsJESUp;
    Int_t   TNJetsJESDown;
    Int_t   TNBtagsJESUp;
    Int_t   TNBtagsJESDown;
    Int_t   TNJetsJER;
    Int_t   TNBtagsBtagUp;
    Int_t   TNBtagsBtagDown;
    Int_t   TNBtagsMisTagUp;
    Int_t   TNBtagsMisTagDown;
    Float_t TJetJESUp_Pt[20];
    Float_t TJetJESDown_Pt[20];
    Float_t TJetJER_Pt[20];
    Float_t THTJESUp;
    Float_t THTJESDown;

    Int_t   TNISRJets;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t TMT2llJESUp;
    Float_t TMT2llJESDown;

    Float_t  TISRweight;
    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSDown;

    Float_t TgenTop1Pt ;
    Float_t TgenTop1Eta;
    Float_t TgenTop1Phi;
    Float_t TgenTop1M  ;
    Float_t TgenTop2Pt ;
    Float_t TgenTop2Eta;
    Float_t TgenTop2Phi;
    Float_t TgenTop2M  ;
    Float_t TgentopDeltaPhi; 
    Float_t TgentopDeltaR;

    Float_t TgenMET;   // MET_genPt
    Float_t TgenMETPhi;
    Int_t   TNgenLeps;
    Int_t   TgenLep0_Id;
    Int_t   TgenLep1_Id;
    Float_t TgenLep0_Pt;
    Float_t TgenLep0_Eta;
    Float_t TgenLep1_Pt;
    Float_t TgenLep1_Eta;
    Float_t TgenDeltaPhi;
    Float_t TgenDeltaEta;
    Float_t TgenMT2;
    Int_t   TNgenJets;
    Int_t   TNgenBtags; 
    Float_t TgenJet_Pt[20];
    Float_t TgenJet_Eta[20];
    Float_t TgenHT;

  protected:
    Bool_t  gIsData;
    Bool_t  gIsLHE;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;

    ClassDef(StopAnalysis, 0);
};
