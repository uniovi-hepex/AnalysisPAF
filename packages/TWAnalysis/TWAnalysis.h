#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

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

    TTree* fTree;
    TTree* fTWTree;
    Float_t TLHEWeight[254];
    void SetLeptonVariables();
    void SetJetVariables();
    void SetEventVariables();

    Bool_t makeHistos = false;
    Bool_t makeTree   = false;

    void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets);
    void GetMET();
    Int_t nFiduJets; Int_t nFidubJets; 

    Float_t TrigSF;
    Float_t TrigSFerr;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t   gChannel;
    Bool_t  passMETfilters;
    Bool_t  passTrigger;
    Bool_t  isSS;
    Float_t NormWeight;
    Float_t  BtagSF          ;    
    Float_t  BtagSFBtagUp    ;
    Float_t  BtagSFBtagDown  ;
    Float_t  BtagSFMistagUp  ;
    Float_t  BtagSFMistagDown;

    Float_t TDilepPt;

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
    Double_t getDPtL1_L2(const TString& sys = "Norm");
    Double_t getDeltaPt(vector<TLorentzVector>, vector<TLorentzVector>);
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


    //Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TMET;      // MET
    Float_t TGenMET;     
    Float_t TMET_Phi;  // MET phi
    Float_t TMET_PhiJESUp;  // MET phi
    Float_t TMET_PhiJESDown;  // MET phi
    Float_t TMET_PhiJER;  // MET phi

    Int_t   TNVetoLeps;
    Int_t   TNSelLeps;
    Int_t   TChannel;
    Bool_t   TIsSS;
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
    Float_t THTtotJER;
    Float_t THTtot2j;
    Float_t THTtot2jJESUp;
    Float_t THTtot2jJESDown;
    Float_t THTtot2jJER;
    Float_t THtRejJ2;

    Float_t TDilepMETPt     ;
    Float_t TETSys          ;
    Float_t TET_LLJetMET    ;
    Float_t TDPtL1_L2       ;
    Float_t TDPtJ2_L2       ;
    Float_t TDR_L1_J1       ;
    Float_t TDR_L1L2_J1J2   ;
    Float_t TDR_L1L2_J1J2MET;


    // For systematics...
    Int_t   TNJetsJESUp;
    Int_t   TNJetsJESDown;
    Int_t   TNJetsJER;
    Int_t   TNBtagsJESUp;
    Int_t   TNBtagsJESDown;
    Int_t   TNBtagsJER;
    Float_t TJetJESUp_Pt[20];
    Float_t TJetJESDown_Pt[20];
    Float_t TJetJER_Pt[20];
    Float_t THTJESUp;
    Float_t THTJESDown;
    Float_t THTJER;

    Int_t   TNISRJets;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t TMETJER;
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
    
    Float_t  TWeight_MistagUp  ;
    Float_t  TWeight_MistagDown;
    Float_t  TWeight_BtagUp    ;
    Float_t  TWeight_BtagDown  ;
    
    Float_t   LeadingLeptPt_   ;
    Float_t   LeadingLeptEta_  ;    
    Float_t   jetPtSubLeading_ ;
    Float_t   jetEtaSubLeading_;


    Float_t  DilepMETJetPt  , DilepMETJetPtJESUp  , DilepMETJetPtJESDown  , DilepMETJetPtJER  ;
    Float_t  Lep1METJetPt   , Lep1METJetPtJESUp   , Lep1METJetPtJESDown   , Lep1METJetPtJER   ;
    Float_t  DPtDilep_JetMET, DPtDilep_JetMETJESUp, DPtDilep_JetMETJESDown, DPtDilep_JetMETJER;
    Float_t  DPtDilep_MET   , DPtDilep_METJESUp   , DPtDilep_METJESDown   , DPtDilep_METJER   ;
    Float_t  DPtLep1_MET    , DPtLep1_METJESUp    , DPtLep1_METJESDown    , DPtLep1_METJER    ;
    Float_t  DilepMETJet1Pz , DilepMETJet1PzJESUp , DilepMETJet1PzJESDown , DilepMETJet1PzJER ;
    Float_t  nLooseCentral  , nLooseCentralJESUp  , nLooseCentralJESDown  , nLooseCentralJER  ;
    Float_t  nLooseFwd      , nLooseFwdJESUp      , nLooseFwdJESDown      , nLooseFwdJER      ;
    Float_t  nBLooseCentral , nBLooseCentralJESUp , nBLooseCentralJESDown , nBLooseCentralJER ;
    Float_t  nBLooseFwd     , nBLooseFwdJESUp     , nBLooseFwdJESDown     , nBLooseFwdJER     ;
    Float_t  TJet2csv       , TJet2csvJESUp       , TJet2csvJESDown       , TJet2csvJER       ;
    Float_t  MSys           , MSysJESUp           , MSysJESDown           , MSysJER           ;
    Float_t  TJetLoosept    , TJetLooseptJESUp    , TJetLooseptJESDown    , TJetLooseptJER    ;
    Float_t  TJetLooseCentralpt, TJetLooseCentralptJESUp    , TJetLooseCentralptJESDown    , TJetLooseCentralptJER    ;
    Float_t  C_jll          , C_jllJESUp          , C_jllJESDown          , C_jllJER          ;
    Float_t  DilepJetPt     , DilepJetPtJESUp     , DilepJetPtJESDown     , DilepJetPtJER     ;
    Float_t  TBDTada        , TBDTadaJESUp        , TBDTadaJESDown        , TBDTadaJER        ;
    Float_t  TBDTgrad       , TBDTgradJESUp       , TBDTgradJESDown       , TBDTgradJER       ;
    Float_t  TBDT       , TBDTJESUp       , TBDTJESDown       , TBDTJER       ;
    Float_t  TBDT2j1t       , TBDT2j1tJESUp       , TBDT2j1tJESDown       , TBDT2j1tJER       ;
    /* Float_t  TBDTBTagUp     , TBDTBTagDown; */
    /* Float_t  TBDTMistagUp   , TBDTBMistagDown; */

    Float_t  nBTotal          , nBTotalJESUp          , nBTotalJESDown          , nBTotalJER          ; 
    Float_t  DilepmetjetOverHT, DilepmetjetOverHTJESUp, DilepmetjetOverHTJESDown, DilepmetjetOverHTJER; 
    Float_t  HTLepOverHT      , HTLepOverHTJESUp      , HTLepOverHTJESDown      , HTLepOverHTJER      ; 
    Float_t  TJet1_pt         , TJet1_ptJESUp         , TJet1_ptJESDown         , TJet1_ptJER         ;

    Float_t  TDilepPtJESUp          , TDilepPtJESDown         , TDilepPtJER         ; 
    Float_t  TDilepMETPtJESUp       , TDilepMETPtJESDown      , TDilepMETPtJER      ;
    Float_t  TETSysJESUp            , TETSysJESDown           , TETSysJER           ;
    Float_t  TET_LLJetMETJESUp      , TET_LLJetMETJESDown     , TET_LLJetMETJER     ;
    Float_t  THtRejJ2JESUp          , THtRejJ2JESDown         , THtRejJ2JER         ;
    Float_t  TDPtL1_L2JESUp         , TDPtL1_L2JESDown        , TDPtL1_L2JER        ;
    Float_t  TDPtJ2_L2JESUp         , TDPtJ2_L2JESDown        , TDPtJ2_L2JER        ;
    Float_t  TDR_L1_J1JESUp         , TDR_L1_J1JESDown        , TDR_L1_J1JER        ;
    Float_t  TDR_L1L2_J1J2JESUp     , TDR_L1L2_J1J2JESDown    , TDR_L1L2_J1J2JER    ;
    Float_t  TDR_L1L2_J1J2METJESUp  , TDR_L1L2_J1J2METJESDown , TDR_L1L2_J1J2METJER ;
    Float_t  LeadingLeptPt_JESUp    , LeadingLeptPt_JESDown   , LeadingLeptPt_JER   ;
    Float_t  LeadingLeptEta_JESUp   , LeadingLeptEta_JESDown  , LeadingLeptEta_JER  ;
    Float_t  jetPtSubLeading_JESUp  , jetPtSubLeading_JESDown , jetPtSubLeading_JER ;
    Float_t  jetEtaSubLeading_JESUp , jetEtaSubLeading_JESDown, jetEtaSubLeading_JER;




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
    void    setTWBDT();
    TMVA::Reader* BDTada;
    TMVA::Reader* BDTada_JESUp;
    TMVA::Reader* BDTada_JESDown;
    TMVA::Reader* BDTada_JER;
    TMVA::Reader* BDTgrad;
    TMVA::Reader* BDTgrad_JESUp;
    TMVA::Reader* BDTgrad_JESDown;
    TMVA::Reader* BDTgrad_JER;
    TMVA::Reader* BDT;
    TMVA::Reader* BDT_JESUp;
    TMVA::Reader* BDT_JESDown;
    TMVA::Reader* BDT_JER;
    TMVA::Reader* BDT2j1t;
    TMVA::Reader* BDT2j1tJESUp;
    TMVA::Reader* BDT2j1tJESDown;
    TMVA::Reader* BDT2j1tJER;


    ClassDef(TWAnalysis, 0);
};
