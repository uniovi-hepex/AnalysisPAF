#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "SFfor4top.h"


class t4Analysis : public PAFChainItemSelector{
  public:
    TTree* fTree;

    Int_t TNLHEWeight;
    Float_t TLHEWeight[254];
 
    t4Analysis();
    virtual ~t4Analysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    void ResetParameters();
    std::vector<Lepton> selTaus     ;
    std::vector<Lepton> selLeptons  ;
    std::vector<Lepton> vetoLeptons ;
    std::vector<Lepton> looseLeptons;
    std::vector<Lepton> zLeptons;
    std::vector<Lepton> xLeptons;
    std::vector<Jet>    selJets ;
    std::vector<Jet>    selBTags;
    std::vector<Jet>    Jets15  ;

    void SetTauVariables();
    void SetLeptonVariables();
    void SetJetVariables();
    void SetEventVariables();

    void GetTauVariables(std::vector<Lepton> selTaus);
    void GetLeptonVariables(std::vector<Lepton> selLeptons);
    void GetFakeableLeptonVariables(std::vector<Lepton> vetoLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetMET();
    Int_t Get4tChannel(vector<Lepton> selLeptons, vector<Lepton> xLeptons);

    Float_t TrigSF;
    Float_t TrigSF_Up;
    Float_t TrigSF_Down;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t   gChannel;
    Bool_t  passMETfilters;
    Bool_t  passTrigger;
    Bool_t  passTrigger2;
    Bool_t  isSS;
    Float_t NormWeight;
    Bool_t  gIsFastSim = false;
  
    // Tree Variables

    UInt_t TRun;
    UInt_t TLumi;
    UInt_t TEvent;
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TM3l;
    bool    TIsSS;
    Float_t TMZ;      
    Float_t TMET;      // MET
    Float_t TMET_Phi;  // MET phi
    Float_t TMT2;
    Int_t   TIsOnZ;

    Int_t   TNFakeableLeps;
    Int_t   TNSelLeps;
    Int_t   TChannel;
    Int_t   TPassTrigger;
    Int_t   TPassTrigger2;
    Int_t   TPassMETFilters;
    Int_t   TPassLowInvMass;
    Float_t TLep0Pt;
    Float_t TLep0Eta;
    Float_t TLep0Phi;
    Float_t TLep0E;
    Int_t TLep0Id;
    Float_t TLep1Pt;
    Float_t TLep1Eta;
    Float_t TLep1Phi;
    Float_t TLep1E;
    Int_t TLep1Id;
    Float_t TLep2Pt;
    Float_t TLep2Eta;
    Float_t TLep2Phi;
    Float_t TLep2E;
    Int_t TLep2Id;
    Float_t TLep3Pt;
    Float_t TLep3Eta;
    Float_t TLep3Phi;
    Float_t TLep3E;
    Int_t TLep3Id;
    Float_t TLep4Pt;
    Float_t TLep4Eta;
    Float_t TLep4Phi;
    Float_t TLep4E;
    Int_t TLep4Id;
    Float_t TFLep0Pt;
    Float_t TFLep0Eta;
    Float_t TFLep0Phi;
    Float_t TFLep0E;
    Int_t TFLep0Id;
    Float_t TFLep1Pt;
    Float_t TFLep1Eta;
    Float_t TFLep1Phi;
    Float_t TFLep1E;
    Int_t TFLep1Id;
    /*   Float_t TLep_Pt[10];
         Float_t TLep_Eta[10];
         Float_t TLep_Phi[10];
         Float_t TLep_E[10];
         Int_t   TLep_Charge[10];
         Int_t   TLep_pdgId[10];
         Float_t TFLep_Pt[5];
         Float_t TFLep_Eta[5];
         Float_t TFLep_Phi[5];
         Float_t TFLep_E[5];
         Int_t   TFLep_Charge[5];
         Int_t TFLep_pdgId[10];
         */
    Int_t TNTaus;
    Float_t TTau_Pt         [10];
    Float_t TTau_Eta        [10];
    Float_t TTau_Charge     [10];
    Float_t TTau_DecayMode  [10];
    Float_t TTau_IdDecayMode[10];   
    Float_t TTau_IdMVA      [10];   
    Float_t TTau_IdAntiE    [10];   
    Float_t TTau_IdAntiMu   [10];   

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

    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSDown;

  protected:
    Bool_t  gIsData;
    Bool_t  gIsLHE;
    Bool_t  gDoSyst;
    TString gSampleName;

    ClassDef(t4Analysis, 0);
};
