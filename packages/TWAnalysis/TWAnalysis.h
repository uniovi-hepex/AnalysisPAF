#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TEfficiency.h"
#include "TFile.h"

//enum eChannels{iUnkChan, iElMu, iMuon, iElec, nChannels};
const Int_t nChannels = 3;
enum eLevels  {idilepton, iZVeto, iMETcut, i2jets, i1btag, i1j1b, i2j1b, i2j2b, nLevels};
enum eSysts   {inorm, nSysts};
const int nWeights = 248;
const TString gChanLabel[nChannels] = {"ElMu", "Muon","Elec"};
const TString sCut[nLevels] = {"dilepton", "ZVeto", "MET", "2jets", "1btag", "1j1b", "2j1b","2j2b"};
const TString gSys[nSysts] = {"0"};


class TWAnalysis : public PAFChainItemSelector{
  public:
    TWAnalysis();
    virtual ~TWAnalysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> genLeptons  ;
    std::vector<Lepton> selLeptons  ;
    std::vector<Lepton> vetoLeptons ;
    std::vector<Jet> selJets ;
    std::vector<Jet> selJetsJecUp   ;
    std::vector<Jet> selJetsJecDown ;
    std::vector<Jet> selJetsJER     ;

    std::vector<Jet> Jets15  ;
    std::vector<Jet> genJets  ;
    std::vector<Jet> mcJets  ;
    std::vector<Jet> vetoJets;

    std::vector<Jet> SergioJets;
    std::vector<Jet> SergioLooseCentralJets;
    std::vector<Jet> SergioLooseFwdJets;
    std::vector<Lepton> SergioLeps;
    
    TTree* fTree;
    TTree* fMini;
    TTree* fMini1j1t;
    TTree* fMini2j1t;

    bool hasTW;
    Float_t TLHEWeight[254];
    void SetLeptonVariables();
    void SetJetVariables();
    void SetEventVariables();

    Bool_t makeHistos = false;
    Bool_t makeTree   = false;

    void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets);
    void GetGenLepVariables();
    float getTopPtRW();
    void GetMET();
    void GetGenMET();
    Int_t nFiduJets; Int_t nFidubJets; 

    Float_t TrigSF;
    Float_t TrigSFerr;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t   gChannel;
    TString gPar;
    Bool_t  passMETfilters;
    Bool_t  passTrigger;
    Bool_t  isSS;
    Float_t NormWeight;
    Float_t  BtagSF          ;
    Float_t  BtagSFBtagUp    ;
    Float_t  BtagSFBtagDown  ;
    Float_t  BtagSFMistagUp  ;
    Float_t  BtagSFMistagDown;


    Float_t TVetoJet1_Pt;
    Float_t TVetoJet1_Eta;
    Float_t TVetoJet2_Pt;
    Float_t TVetoJet2_Eta;
    Float_t TVetoJet3_Pt;
    Float_t TVetoJet3_Eta;



    void InitHistos();
    void FillDYHistos(Int_t ch);
    void FillHistos(Int_t ch, Int_t cut);
  
    void CalculateTWVariables();
    void get20Jets();
    void ReSetTWVariables();
    void SetTWVariables();
    Double_t getDilepMETJetPt(int sys = 0);
    Double_t getDilepPt();
    Double_t getDilepJetPt(const TString& sys = "Norm");
    Double_t getLep1METJetPt(const TString& sys = "Norm");
    Double_t getPtSys(TLorentzVector*, int);
    Double_t getDilepMETJet1Pz(const TString& sys = "Norm");
    Double_t getPzSys(TLorentzVector*, int);
    Double_t getDPtDilep_JetMET(const TString& sys = "Norm");
    Double_t getDPtDilep_MET(const TString& sys = "Norm");
    Double_t getDPtLep1_MET(const TString& sys = "Norm");
    Double_t getDPhiLep1_Lep2();
    Double_t getDPhiLep1_Jet1(const TString& sys = "Norm");
    Double_t getDPhiLep2_Jet1(const TString& sys = "Norm");
    Double_t getDPtL1_L2(const TString& sys = "Norm");
    Double_t getDeltaPt(vector<TLorentzVector>, vector<TLorentzVector>);
    Double_t getDeltaPhi(vector<TLorentzVector>, vector<TLorentzVector>);
    Double_t getSysM(const TString& sys = "Norm");
    Double_t getM(vector<TLorentzVector>);
    Double_t getHTtot(const TString& sys = "Norm");
    Double_t getHTtot2j(const TString& sys = "Norm");

    Double_t getDeltaRDilep_METJets12(int sys = 0);
    Double_t getDeltaRDilep_Jets12(int sys = 0);
    Double_t getDeltaRLep1_Jet1(int sys = 0);

    Double_t getDeltaR(vector<TLorentzVector>, vector<TLorentzVector>);

    Double_t getETSys(int sys = 0);
    Double_t getET_LLJetMET(int sys = 0);

    Double_t getDilepMETPt(int sys= 0);

    Float_t getWeightFromHist(Float_t pt, Float_t eta, TEfficiency* hist);
    Float_t getWeightFromTrueHist(Float_t pt, Float_t eta, TH2F* hist);

    //Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TMllJESUp;   // Invariant mass
    Float_t TMllJESDown; // Invariant mass
    Float_t TMllJERUp;   // Invariant mass
    Float_t TMET;      // MET
    Float_t TGenMET;
    Float_t TMET_Phi;  // MET phi
    Float_t TMET_PhiJESUp;  // MET phi
    Float_t TMET_PhiJESDown;  // MET phi
    Float_t TMET_PhiJERUp;  // MET phi
    Float_t TLeadingJetPt    ;
    Float_t TLeadingJetEta   ;
    Float_t TLeadingJetCSV   ;
    Float_t TSubLeadingJetPt ;
    Float_t TSubLeadingJetEta;
    Float_t TSubLeadingJetCSV;

    Int_t   TNVetoLeps;
    Int_t   TNSelLeps;
    Int_t   TChannel;
    Bool_t   TIsSS;
    Bool_t   TIsFid;
    Float_t TLep_Pt[10];    
    Float_t TLep_Eta[10];
    Float_t TLep_Phi[10];
    Float_t TLep_E[10];
    Float_t TLep_Charge[10];

    Int_t TNJets;            // Jets...
    Int_t TNVetoJets;            // Jets...
    Int_t TNBtags;
    Float_t TJet_Pt[20];
    Float_t TJet_Eta[20];
    Float_t TJet_Phi[20];
    Float_t TJet_E[20];
    Int_t TJet_isBJet[20];
    Float_t THT;       // HT
    Float_t THTtot;       
    Float_t THTtotJESUp;
    Float_t THTtotJESDown;
    Float_t THTtotJERUp;
    Float_t THTtot2j;
    Float_t THTtot2jJESUp;
    Float_t THTtot2jJESDown;
    Float_t THTtot2jJERUp;
    Float_t THtRejJ2;

    Float_t TDilepMETPt     ;
    Float_t TETSys          ;
    Float_t TET_LLJetMET    ;
    Float_t TDPtL1_L2       ;
    Float_t TDPtJ2_L2       ;
    Float_t TDR_L1_J1       ;
    Float_t TDR_L1L2_J1J2   ;
    Float_t TDR_L1L2_J1J2MET;

    // ADDED:
    
    Float_t TM_LeadingB       ;
    Float_t TM_SubLeadingB    ;
    Float_t TE_LLB            ;
    Float_t TMT_LLMETB        ;
    Float_t TM_LLB            ;
    Float_t TLeadingJetE       ;
    Float_t TLeadingJetPhi     ;
    Float_t TLeadingLepPhi     ;
    Float_t TLeadingLepE       ;
    Float_t TSubLeadingLepE    ;
    Float_t TSubLeadingLepPhi  ;
    Float_t TLLMETBEta      ;
    Float_t TDPhiLL      ;
    Float_t TDPhiLeadJet ;
    Float_t TDPhiSubLeadJet ;
    
    Float_t TM_LeadingBJESUp       ;
    Float_t TM_SubLeadingBJESUp    ;
    Float_t TE_LLBJESUp            ;
    Float_t TMT_LLMETBJESUp        ;
    Float_t TM_LLBJESUp            ;
    Float_t TLeadingJetPtJESUp    ;
    Float_t TLeadingJetEJESUp       ;
    Float_t TLeadingJetPhiJESUp     ;
    Float_t TLeadingJetEtaJESUp;
    Float_t TLeadingLepPtJESUp;
    Float_t TLeadingLepEJESUp;
    Float_t TLeadingLepPhiJESUp;
    Float_t TLeadingLepEtaJESUp;
    Float_t TSubLeadingLepPtJESUp;
    Float_t TSubLeadingLepEJESUp;
    Float_t TSubLeadingLepPhiJESUp;
    Float_t TSubLeadingLepEtaJESUp;
    Float_t TLLMETBEtaJESUp      ;
    Float_t TDPhiLLJESUp      ;
    Float_t TDPhiLeadJetJESUp ;
    Float_t TDPhiSubLeadJetJESUp ;
    
    Float_t TM_LeadingBJESDown       ;
    Float_t TM_SubLeadingBJESDown    ;
    Float_t TE_LLBJESDown            ;
    Float_t TMT_LLMETBJESDown        ;
    Float_t TM_LLBJESDown            ;
    Float_t TLeadingJetPtJESDown    ;
    Float_t TLeadingJetEJESDown       ;
    Float_t TLeadingJetPhiJESDown     ;
    Float_t TLeadingJetEtaJESDown   ;
    Float_t TLeadingLepPtJESDown;
    Float_t TLeadingLepEJESDown;
    Float_t TLeadingLepPhiJESDown;
    Float_t TLeadingLepEtaJESDown;
    Float_t TSubLeadingLepPtJESDown;
    Float_t TSubLeadingLepEJESDown;
    Float_t TSubLeadingLepPhiJESDown;
    Float_t TSubLeadingLepEtaJESDown;
    Float_t TLLMETBEtaJESDown      ;
    Float_t TDPhiLLJESDown      ;
    Float_t TDPhiLeadJetJESDown ;
    Float_t TDPhiSubLeadJetJESDown ;
    
    Float_t TM_LeadingBJERUp       ;
    Float_t TM_SubLeadingBJERUp    ;
    Float_t TE_LLBJERUp            ;
    Float_t TMT_LLMETBJERUp        ;
    Float_t TM_LLBJERUp            ;
    Float_t TLeadingJetPtJERUp    ;
    Float_t TLeadingJetEJERUp       ;
    Float_t TLeadingJetPhiJERUp     ;
    Float_t TLeadingJetEtaJERUp   ;
    Float_t TLeadingLepPtJERUp;
    Float_t TLeadingLepEJERUp;
    Float_t TLeadingLepPhiJERUp;
    Float_t TLeadingLepEtaJERUp;
    Float_t TSubLeadingLepPtJERUp;
    Float_t TSubLeadingLepEJERUp;
    Float_t TSubLeadingLepPhiJERUp;
    Float_t TSubLeadingLepEtaJERUp;
    Float_t TLLMETBEtaJERUp      ;
    Float_t TDPhiLLJERUp      ;
    Float_t TDPhiLeadJetJERUp ;
    Float_t TDPhiSubLeadJetJERUp ;
    
    Float_t TGenLeadingJetPt   ;
    Float_t TGenLeadingJetE    ;
    Float_t TGenLeadingJetPhi  ;
    Float_t TGenLeadingJetEta  ;
    Float_t TGenLeadingLooseFwdJetPt   ;
    Float_t TGenLeadingLooseFwdJetE    ;
    Float_t TGenLeadingLooseFwdJetPhi  ;
    Float_t TGenLeadingLooseFwdJetEta  ;
    Float_t TGenSubLeadingLooseCentralJetPt   ;
    Float_t TGenSubLeadingLooseCentralJetE    ;
    Float_t TGenSubLeadingLooseCentralJetPhi  ;
    Float_t TGenSubLeadingLooseCentralJetEta  ;
    Float_t TGenLeadingLepPt   ;
    Float_t TGenLeadingLepE    ;
    Float_t TGenLeadingLepPhi  ;
    Float_t TGenLeadingLepEta  ;
    Float_t TGenSubLeadingLepPt ;
    Float_t TGenSubLeadingLepE  ;
    Float_t TGenSubLeadingLepPhi;
    Float_t TGenSubLeadingLepEta;
    Float_t TGenM_LeadingB    ;
    Float_t TGenM_SubLeadingB ;
    Float_t TGenE_LLB         ;
    Float_t TGenMT_LLMETB     ;
    Float_t TGenM_LLB         ;
    Float_t TGenDilepPt        ;
    Float_t TGenDilepJetPt     ;
    Float_t TGenDilepMETJetPt  ;
    Float_t TGenHTtot          ;
    Float_t TGenMET_Phi        ;
    Float_t TGenDilepMETJet1Pz ;
    Float_t TGenLLMETBEta      ;
    Float_t TGenMSys           ;
    Float_t TGenMll           ;
    Float_t TGenDPhiLL      ;
    Float_t TGenDPhiLeadJet ;
    Float_t TGenDPhiSubLeadJet ;
    Int_t GenChannel           ;
    Bool_t TGenIsSS;
    Bool_t Tpassgen            ;
    Bool_t Tpassreco           ;
    Bool_t TpassrecoJESUp      ;
    Bool_t TpassrecoJESDown    ;
    Bool_t TpassrecoJERUp      ;
    Float_t TWeight_normal     ;
    Int_t nSergioJets;
    Int_t nSergiobJets;
    Int_t nSergioLooseCentralJets;
    Int_t nSergioLooseCentralbJets;
    Int_t nSergioLooseFwdJets;
    Int_t nSergioLeps;
    Int_t nSergioGenJets;
    Int_t nSergioGenLeps;
    Int_t nSergioGenMET;
    Jet tJ;
    TLorentzVector tpJ;
    Lepton tL;
    TLorentzVector tpL;
    TLorentzVector SergioMET;
    TLorentzVector tMET;
    
    Float_t prefWeight;
    Float_t prefWeight1;
    Float_t prefWeight2;
    Float_t prefWeight3;
    
    TH2F* hPrefWeight;
    TEfficiency* hPrefWeight1;
    TEfficiency* hPrefWeight2;
    TEfficiency* hPrefWeight3;

    // For systematics...
    Int_t   TNJetsJESUp;
    Int_t   TNJetsJESDown;
    Int_t   TNJetsJERUp;
    Int_t   TNBtagsJESUp;
    Int_t   TNBtagsJESDown;
    Int_t   TNBtagsJERUp;
    Float_t TJetJESUp_Pt[20];
    Float_t TJetJESDown_Pt[20];
    Float_t TJetJER_Pt[20];
    Float_t THTJESUp;
    Float_t THTJESDown;
    Float_t THTJERUp;

    Int_t   TNISRJets;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t TMETJERUp;
    Float_t TMT2llJESUp;
    Float_t TMT2llJESDown;

    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_ElecEffUp;
    Float_t  TWeight_ElecEffDown;
    Float_t  TWeight_MuonEffUp;
    Float_t  TWeight_MuonEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSDown;

    Float_t   TWeight_upFrag       ;    
    Float_t   TWeight_downFrag     ;	
    Float_t   TWeight_PetersonFrag ;
    Float_t   TWeight_semilepbrUp  ;	
    Float_t   TWeight_semilepbrDown;

    
    Float_t  TWeight_MistagUp  ;
    Float_t  TWeight_MistagDown;
    Float_t  TWeight_BtagUp    ;
    Float_t  TWeight_BtagDown  ;
    Float_t  TWeight_TopPt  ;
    Float_t  TWeight_TopPtDown;
    
    Float_t   LeadingLeptPt_   ;
    Float_t   LeadingLeptEta_  ;    
    Float_t   jetPtSubLeading_ ;
    Float_t   jetEtaSubLeading_;

    Float_t TLeadingLepPt    ;
    Float_t TLeadingLepEta   ;
    Float_t TDilepPt         ;
    Float_t TSubLeadingLepPt ;
    Float_t TSubLeadingLepEta;



    Float_t  DilepMETJetPt  , DilepMETJetPtJESUp  , DilepMETJetPtJESDown  , DilepMETJetPtJERUp  ;
    Float_t  Lep1METJetPt   , Lep1METJetPtJESUp   , Lep1METJetPtJESDown   , Lep1METJetPtJERUp   ;
    Float_t  DPtDilep_JetMET, DPtDilep_JetMETJESUp, DPtDilep_JetMETJESDown, DPtDilep_JetMETJERUp;
    Float_t  DPtDilep_MET   , DPtDilep_METJESUp   , DPtDilep_METJESDown   , DPtDilep_METJERUp   ;
    Float_t  DPtLep1_MET    , DPtLep1_METJESUp    , DPtLep1_METJESDown    , DPtLep1_METJERUp    ;
    Float_t  DilepMETJet1Pz , DilepMETJet1PzJESUp , DilepMETJet1PzJESDown , DilepMETJet1PzJERUp ;
    Int_t    nLooseCentral  , nLooseCentralJESUp  , nLooseCentralJESDown  , nLooseCentralJERUp  ;
    Int_t    nLooseFwd      , nLooseFwdJESUp      , nLooseFwdJESDown      , nLooseFwdJERUp      ;
    Int_t    nBLooseCentral , nBLooseCentralJESUp , nBLooseCentralJESDown , nBLooseCentralJERUp ;
    Int_t    nBLooseFwd     , nBLooseFwdJESUp     , nBLooseFwdJESDown     , nBLooseFwdJERUp     ;
    Float_t  TJet2csv       , TJet2csvJESUp       , TJet2csvJESDown       , TJet2csvJERUp       ;
    Float_t  MSys           , MSysJESUp           , MSysJESDown           , MSysJERUp           ;
    Float_t  TJetLoosept    , TJetLooseptJESUp    , TJetLooseptJESDown    , TJetLooseptJERUp    ;
    Float_t  TJetLooseCentralpt, TJetLooseCentralptJESUp    , TJetLooseCentralptJESDown    , TJetLooseCentralptJERUp    ;
    Float_t  TJetLooseFwdpt, TJetLooseFwdptJESUp    , TJetLooseFwdptJESDown    , TJetLooseFwdptJERUp    ;
    Float_t  C_jll          , C_jllJESUp          , C_jllJESDown          , C_jllJERUp          ;
    Float_t  DilepJetPt     , DilepJetPtJESUp     , DilepJetPtJESDown     , DilepJetPtJERUp     ;
    /* Float_t  TBDTada        , TBDTadaJESUp        , TBDTadaJESDown        , TBDTadaJERUp        ; */
    /* Float_t  TBDTgrad       , TBDTgradJESUp       , TBDTgradJESDown       , TBDTgradJERUp       ; */
    Float_t  TBDT       , TBDTJESUp       , TBDTJESDown       , TBDTJERUp       ;
    Float_t  TBDT2j1t       , TBDT2j1tJESUp       , TBDT2j1tJESDown       , TBDT2j1tJERUp       ;
    Float_t TBDT2j1t_DR, TBDT2j1t_ot;
    /* Float_t  TBDT2j1tv1, TBDT2j1tv2, TBDT2j1tv3; */
    /* Float_t  TBDTBTagUp     , TBDTBTagDown; */
    /* Float_t  TBDTMistagUp   , TBDTBMistagDown; */

    Int_t    nBTotal          , nBTotalJESUp          , nBTotalJESDown          , nBTotalJERUp          ; 
    Float_t  DilepmetjetOverHT, DilepmetjetOverHTJESUp, DilepmetjetOverHTJESDown, DilepmetjetOverHTJERUp; 
    Float_t  HTLepOverHT      , HTLepOverHTJESUp      , HTLepOverHTJESDown      , HTLepOverHTJERUp      ; 
    Float_t  TJet1_pt         , TJet1_ptJESUp         , TJet1_ptJESDown         , TJet1_ptJERUp         ;

    Float_t  TDilepPtJESUp          , TDilepPtJESDown         , TDilepPtJERUp         ; 
    Float_t  TDilepMETPtJESUp       , TDilepMETPtJESDown      , TDilepMETPtJERUp      ;
    Float_t  TETSysJESUp            , TETSysJESDown           , TETSysJERUp           ;
    Float_t  TET_LLJetMETJESUp      , TET_LLJetMETJESDown     , TET_LLJetMETJERUp     ;
    Float_t  THtRejJ2JESUp          , THtRejJ2JESDown         , THtRejJ2JERUp         ;
    Float_t  TDPtL1_L2JESUp         , TDPtL1_L2JESDown        , TDPtL1_L2JERUp        ;
    Float_t  TDPtJ2_L2JESUp         , TDPtJ2_L2JESDown        , TDPtJ2_L2JERUp        ;
    Float_t  TDR_L1_J1JESUp         , TDR_L1_J1JESDown        , TDR_L1_J1JERUp        ;
    Float_t  TDR_L1L2_J1J2JESUp     , TDR_L1L2_J1J2JESDown    , TDR_L1L2_J1J2JERUp    ;
    Float_t  TDR_L1L2_J1J2METJESUp  , TDR_L1L2_J1J2METJESDown , TDR_L1L2_J1J2METJERUp ;
    Float_t  LeadingLeptPt_JESUp    , LeadingLeptPt_JESDown   , LeadingLeptPt_JERUp   ;
    Float_t  LeadingLeptEta_JESUp   , LeadingLeptEta_JESDown  , LeadingLeptEta_JERUp  ;
    Float_t  jetPtSubLeading_JESUp  , jetPtSubLeading_JESDown , jetPtSubLeading_JERUp ;
    Float_t  jetEtaSubLeading_JESUp , jetEtaSubLeading_JESDown, jetEtaSubLeading_JERUp;
    Float_t  TJet2_Pt, TJet2_PtJESUp,TJet2_PtJESDown, TJet2_PtJERUp;



// Histograms
//=====================================================0
  TH1F* fHLHEweights[nChannels][nLevels][nSysts];
  TH1F* fHMET[nChannels][nLevels][nSysts];
  TH1F* fHLep0Eta[nChannels][nLevels][nSysts];
  TH1F* fHLep1Eta[nChannels][nLevels][nSysts];
  TH1F* fHDelLepPhi[nChannels][nLevels][nSysts];
  TH1F* fHHT[nChannels][nLevels][nSysts];
  TH1F* fHJet0Eta[nChannels][nLevels][nSysts];
  TH1F* fHJet1Eta[nChannels][nLevels][nSysts];

  TH1F* fHDiLepPt[nChannels][nLevels][nSysts];
  TH1F* fHLep0Pt[nChannels][nLevels][nSysts];
  TH1F* fHLep1Pt[nChannels][nLevels][nSysts];
  TH1F* fHLep0Iso[nChannels][nLevels][nSysts];
  TH1F* fHLep1Iso[nChannels][nLevels][nSysts];
  TH1F* fHJet0Pt[nChannels][nLevels][nSysts];
  TH1F* fHJet1Pt[nChannels][nLevels][nSysts];
  TH1F* fHNJets[nChannels][nLevels][nSysts];
  TH1F* fHNBtagJets[nChannels][nLevels][nSysts];

  TH1F* fHDYInvMass[nChannels][nLevels][nSysts];
  TH1F* fHInvMass[nChannels][nLevels][nSysts];
  TH1F* fHInvMass2[nChannels][nLevels][nSysts];
  TH1F* fHNBtagsNJets[nChannels][nLevels][nSysts];
  TH1F* fHJetCSV[nChannels][nLevels][nSysts];
  TH1F* fHJet0CSV[nChannels][nLevels][nSysts];
  TH1F* fHJet1CSV[nChannels][nLevels][nSysts];
  TH1F* fHvertices[nChannels][nLevels][nSysts]; 

  TH1F* fhDummy;
  TH1F* fHWeightsFidu;
  TH1F* fHWeightsNotFidu;
  TH1F*  fHyields[nChannels][nSysts];
  TH1F*  fHFiduYields[nChannels][nSysts];
  TH1F*  fHSSyields[nChannels][nSysts];

  protected:
  
    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsTW;
    Bool_t  gIsLHE;
    Bool_t  gIsFSRUp;
    Bool_t  gIsFSRDown;

    
    void    setTWBDT();
    /* TMVA::Reader* BDTada; */
    /* TMVA::Reader* BDTada_JESUp; */
    /* TMVA::Reader* BDTada_JESDown; */
    /* TMVA::Reader* BDTada_JER; */
    /* TMVA::Reader* BDTgrad; */
    /* TMVA::Reader* BDTgrad_JESUp; */
    /* TMVA::Reader* BDTgrad_JESDown; */
    /* TMVA::Reader* BDTgrad_JER; */
    TMVA::Reader* BDT;
    TMVA::Reader* BDT_JESUp;
    TMVA::Reader* BDT_JESDown;
    TMVA::Reader* BDT_JER;
    TMVA::Reader* BDT2j1t;
    TMVA::Reader* BDT2j1tJESUp;
    TMVA::Reader* BDT2j1tJESDown;
    TMVA::Reader* BDT2j1tJER;

    TMVA::Reader* BDT2j1t_DR;
    TMVA::Reader* BDT2j1t_ot;

    /* TMVA::Reader* BDT2j1tv1; */
    /* TMVA::Reader* BDT2j1tv2; */
    /* TMVA::Reader* BDT2j1tv3; */

    ClassDef(TWAnalysis, 0);
};
