#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>


class StopTopAnalysis : public PAFChainItemSelector{
  public:
    TTree* fTree;

    TH1F* fSumISRJets;  
    TH1F* fTopPtSumOfWeights;
    TH1F* fMetResSumOfWeights;
    TH1F* fISRJets;  
    TH2F* fTopPt12;    
    TH1F* fDeltaPhi;
    TH1F* fDeltaEta;
    TH2F* fDeltaPhiEta;
    Float_t fNormMetRes;

    Int_t TNLHEWeight;
    Float_t TLHEWeight[254];
    Float_t genWeight;
 
    StopTopAnalysis();
    virtual ~StopTopAnalysis(){}
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
    void SetMetNoNeutralinos();
    void SetPolWeights();

    void GetLeptonVariables(std::vector<Lepton> selLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetMET();
    void GetMHT();
    void GetGenInfo();

    Float_t TrigSF;
    Float_t TrigSFerr;
    Float_t FSSF; 
    Float_t FSSFerr; 
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Float_t BtagSFFS;
    Int_t gChannel;
    Bool_t passMETfilters;
    Bool_t passTrigger;
    Bool_t isSS;
    Bool_t gIsMCatNLO;
    Float_t NormWeight;
    Bool_t IsT2ttScan = false;
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
    Float_t TMHT;      // MHT
    Float_t TPfMET;      // MET
    Float_t TMET_Phi;  // MET phi
    Int_t   TNVert;
    Bool_t  TIsSS;
    Float_t TBtagPt;;
    Float_t TBtagSFFS;
    Float_t TMT2_METres;      // Invariant mass
    Float_t TMET_METres;      // MET
    Float_t TMET_noSUSY;
    Float_t TMET_Phi_noSUSY;
    Float_t TMT2_noSUSY;
    Float_t TgenMET_noSUSY;
    Float_t TgenMT2_noSUSY;

    Int_t   TNLooseLeps;
    Int_t   TNSelLeps;
    Int_t   TNSelLepsMuScaleUp;
    Int_t   TNSelLepsMuScaleDown;
    Int_t   TNSelLepsElScaleUp;
    Int_t   TNSelLepsElScaleDown;
    Int_t TChannel;
    Int_t TPassTrigger;
    Int_t TPassMETfilters;
    Float_t TLep0_Pt;    
    Float_t TLep0_PtMuScaleUp;    
    Float_t TLep0_PtMuScaleDown;    
    Float_t TLep0_PtElScaleUp;    
    Float_t TLep0_PtElScaleDown;    
    Float_t TLep0_Eta;
    Float_t TLep0_Phi;
    Float_t TLep0_E;
    Int_t   TLep0_Id;
    Float_t TLep0_Charge;
    Int_t   TLep0_GenMatch;    
    Float_t TLep1_Pt;    
    Float_t TLep1_PtMuScaleUp;    
    Float_t TLep1_PtMuScaleDown;    
    Float_t TLep1_PtElScaleUp;    
    Float_t TLep1_PtElScaleDown;    
    Float_t TLep1_Eta;
    Float_t TLep1_Phi;
    Float_t TLep1_E;
    Int_t   TLep1_Id;
    Int_t   TLep1_GenMatch;    
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
    Int_t   TNJetsJERUp;
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
    Float_t TMETJERUp;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t TMT2JERUp;
    Float_t TMT2JESUp;
    Float_t TMT2JESDown;
    Float_t TMETMETUp;
    Float_t TMETMETDown;
    Float_t TMT2METUp;
    Float_t TMT2METDown;
    Float_t TMETunclUp;
    Float_t TMETunclDown;
    Float_t TMT2unclUp;
    Float_t TMT2unclDown;
  
    Float_t TMETMuScaleUp;
    Float_t TMETMuScaleDown;
    Float_t TMETElScaleUp;
    Float_t TMETElScaleDown;
    Float_t TMT2MuScaleUp;
    Float_t TMT2MuScaleDown;
    Float_t TMT2ElScaleUp;
    Float_t TMT2ElScaleDown;

    Float_t  TISRweight;
    Float_t  TISRweightUp;
    Float_t  TISRweightDown;

    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_ElecEffUp;
    Float_t  TWeight_ElecEffDown;
    Float_t  TWeight_MuonEffUp;
    Float_t  TWeight_MuonEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_FSDown;
    Float_t  TWeight_ISRUp;
    Float_t  TWeight_ISRDown;
    Float_t  TWeight_UnclMETUp;
    Float_t  TWeight_UnclMETDown;
    Float_t  TWeight_PolR;
    Float_t  TWeight_PolL;

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
    Int_t TgenTop1ID; Int_t TgenTop2ID;

    Int_t TnGenW = 0; Int_t TnGenNeutrinos = 0; 
    Int_t TnGenStop = 0; Int_t TnGenLsp = 0;
    Float_t TgenNeu1Pt ;
    Float_t TgenNeu1Eta;
    Float_t TgenNeu1Phi;
    Float_t TgenNeu1M  ;
    Float_t TgenNeu2Pt ;
    Float_t TgenNeu2Eta;
    Float_t TgenNeu2Phi;
    Float_t TgenNeu2M  ;
    Float_t TgenW1Pt ;
    Float_t TgenW1Eta;
    Float_t TgenW1Phi;
    Float_t TgenW1M  ;
    Float_t TgenW2Pt ;
    Float_t TgenW2Eta;
    Float_t TgenW2Phi;
    Float_t TgenW2M  ;
    Float_t TgenStop1Pt ;
    Float_t TgenStop1Eta;
    Float_t TgenStop1Phi;
    Float_t TgenStop1M  ;
    Float_t TgenStop2Pt ;
    Float_t TgenStop2Eta;
    Float_t TgenStop2Phi;
    Float_t TgenStop2M  ;
    Float_t TgenLsp1Pt ;
    Float_t TgenLsp1Eta;
    Float_t TgenLsp1Phi;
    Float_t TgenLsp1M  ;
    Float_t TgenLsp2Pt ;
    Float_t TgenLsp2Eta;
    Float_t TgenLsp2Phi;
    Float_t TgenLsp2M  ;
    Int_t TgenStop1ID; Int_t TgenStop2ID;

    Float_t TgenWeight;
    Float_t TgenMET;   // MET_genPt
    Float_t TgenMETPhi;
    Int_t   TEventTruth;
    Int_t   TNgenLeps;
    Int_t   TgenLep0_Id;
    Int_t   TgenLep1_Id;
    Float_t TgenLep0_Pt;
    Float_t TgenLep0_Eta;
    Float_t TgenLep1_Pt;
    Float_t TgenLep1_Eta;
    Float_t TgenLep0_Phi; Float_t TgenLep1_Phi; Float_t TgenLep1_M; Float_t TgenLep0_M;
    Int_t   TgenLep0_Mid;
    Int_t   TgenLep1_Mid;
    Int_t TgenLep0_Ch; Int_t TgenLep1_Ch;
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
    Bool_t  gIsTTbar;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gDoISR;

    ClassDef(StopTopAnalysis, 0);
};
