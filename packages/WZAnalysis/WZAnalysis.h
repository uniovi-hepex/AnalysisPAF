#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include "LeptonSF.h"
#include <iostream>
#include <vector>

enum eChannels{iElElEl, iElElMu, iElMuMu,iMuMuMu};
const Int_t nChannels = 4;
enum eLevels  {itrilepton, ionZ, imet, i0btag, im3l, nLevels};
enum eSysts   {inorm, nSysts};
enum eWPoints  {nolepMVA, veryLoose, loose, medium, tight, veryTight, extraTight, top};

//const int nWPoints = 8;
const int nWPoints = 4;
const int nWeights = 248;
const TString gChanLabel[nChannels] = {"ElElEl", "ElElMu","ElMuMu","MuMuMu"};
const TString sCut[nLevels] = {"trilepton","onZ","met","0btag","m3l"};
//const TString sWPoints[nWPoints] = {"nolepMVA","veryLoose", "loose", "medium", "tight", "veryTight", "extraTight", "top"};
const TString sWPoints[nWPoints]  = {"nolepMVA", "medium", "veryTight", "top"};
const Int_t   WPointVal[nWPoints] = {0, 3, 5, 7}; //Just don't want to tinker with maps
const TString gSys[nSysts] = {"0"};


class WZAnalysis : public PAFChainItemSelector{
  public:
    WZAnalysis();
    virtual ~WZAnalysis(){delete leptonSFtop; delete leptonSFEWKM; delete leptonSFEWKVT;}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> genLeptons  ;
    std::vector<Lepton> genParticles;
    std::vector<Lepton> selLeptons  ; // Main container
    std::vector<Lepton> foLeptons   ; // Main container
    std::vector<Lepton> looseLeptons;


    std::vector<Lepton> tightLeptons;
    std::vector<Lepton> fakeableLeptons;


    std::vector<Jet> selJets ;
    std::vector<Jet> selJetsJecUp   ;
    std::vector<Jet> selJetsJecDown ;
    Lepton lepZ1;
    Lepton lepZ2;
    Lepton lepW;
    Float_t nomZmass = 91.1876;


    std::vector<Jet> Jets15  ;
    std::vector<Jet> genJets  ;
    std::vector<Jet> mcJets  ;
    std::vector<Jet> vetoJets;

    LeptonSF * leptonSFtop;
    LeptonSF * leptonSFEWKM;
    LeptonSF * leptonSFEWKVT;
    TTree* fTree[nWPoints] = {0};
    Float_t TLHEWeight[254];
    void SetLeptonVariables(TTree* iniTree);
    void SetJetVariables(TTree* iniTree);
    void SetEventVariables(TTree* iniTree);

    Bool_t makeHistos = false;
    Bool_t makeTree   = false;

    void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> foLeptons, std::vector<Lepton> looseLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets);
    void GetLeptonsByWP(Int_t wPValue); 
    void GetMET();
    Bool_t passesMCTruth(std::vector<Lepton> sLep, Int_t addConvs, Int_t requiredLeps);
    
    Int_t nFiduJets; Int_t nFidubJets; 

    Float_t TrigSF;
    Float_t TrigSFerr;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t   gChannel;
    Bool_t  passMETfilters;
    Bool_t  passTrigger;
    Float_t NormWeight;

    TString localPath;
    void InitHistos();
    void FillDYHistos(Int_t ch);
    void FillHistos(Int_t ch, Int_t cut);

    void get20Jets();
    Double_t getDilepMETJetPt(const TString& sys = "Norm");
    Double_t getDilepJetPt(const TString& sys = "Norm");
    Double_t getLep1METJetPt(const TString& sys = "Norm");
    Double_t getPtSys(TLorentzVector*, int);
    Double_t getDilepMETJet1Pz(const TString& sys = "Norm");
    Double_t getPzSys(TLorentzVector*, int);
    Double_t getDPtDilep_JetMET(const TString& sys = "Norm");
    Double_t getDPtDilep_MET(const TString& sys = "Norm");
    Double_t getDPtLep1_MET(const TString& sys = "Norm");
    Double_t getDeltaPt(vector<TLorentzVector>, vector<TLorentzVector>);
    Double_t getSysM(const TString& sys = "Norm");
    Double_t getM(vector<TLorentzVector>);


    void makeLeptonCollections();

    //Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass of OSSF (best Z mass)
    Float_t TMZ1W;
    Float_t TMZ2W;
    Float_t TMtW;       // M_T of the W boson
    Float_t TMtWZ;       // M_T of the WZ system
    Float_t TM3l;      // Invariant mass of the three leptons
    Float_t TMinMll;   // Invariant mass of any pair
    Int_t   TNOSSF;      // Number of OSSF pairs
    Float_t TMET;      // Reco MET
    Float_t TGenMET;   // Gent MET
    Float_t TMET_Phi;  // MET phi

    ULong64_t  TEvtNum;

    // Event classification
    Bool_t  TIsSR;
    Bool_t  TIsCRTT;
    Bool_t  TIsCRDY;
    Bool_t  TIsNewCRTT;
    Bool_t  TIsNewCRDY;


    Int_t   TNFOLeps;
    Int_t   TNTightLeps;
    Int_t   TChannel;

    // Lepton Things
    Float_t TLep_PtZ1;    
    Float_t TLep_PtZ2;
    Float_t TLep_PtW;  

    Float_t TLep_EtaZ1;    
    Float_t TLep_EtaZ2;
    Float_t TLep_EtaW;  

    Float_t TLep_PhiZ1;    
    Float_t TLep_PhiZ2;
    Float_t TLep_PhiW;  

    Float_t TLep_EZ1;    
    Float_t TLep_EZ2;
    Float_t TLep_EW;  

    Float_t TLep_ChargeZ1;    
    Float_t TLep_ChargeZ2;
    Float_t TLep_ChargeW;  

    Float_t TLep_IsTightZ1;    
    Float_t TLep_IsTightZ2;
    Float_t TLep_IsTightW;  

    Float_t TLep_IsPromptZ1;    
    Float_t TLep_IsPromptZ2;
    Float_t TLep_IsPromptW;  
  
    Float_t TLep_pdgIdZ1;    
    Float_t TLep_pdgIdZ2;
    Float_t TLep_pdgIdW;  
  
    Int_t  TLep_isConvVeto[10];
    Int_t   TConvNumber;
    Int_t   TFakeNumber;

    // Jet Things
    Int_t TNJets;            
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
    Int_t   TNJetsJERUp;
    Int_t   TNBtagsBtagUp;
    Int_t   TNBtagsBtagDown;
    Int_t   TNBtagsMisTagUp;
    Int_t   TNBtagsMisTagDown;
    Int_t   TNBtagsJESUp;
    Int_t   TNBtagsJESDown;
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


    Float_t  DilepMETJetPt  , DilepMETJetPtJESUp  , DilepMETJetPtJESDown  ;
    Float_t  Lep1METJetPt   , Lep1METJetPtJESUp   , Lep1METJetPtJESDown   ;
    Float_t  DPtDilep_JetMET, DPtDilep_JetMETJESUp, DPtDilep_JetMETJESDown;
    Float_t  DPtDilep_MET   , DPtDilep_METJESUp   , DPtDilep_METJESDown   ;
    Float_t  DPtLep1_MET    , DPtLep1_METJESUp    , DPtLep1_METJESDown    ;
    Float_t  DilepMETJet1Pz , DilepMETJet1PzJESUp , DilepMETJet1PzJESDown ;
    Float_t  nLooseCentral  , nLooseCentralJESUp  , nLooseCentralJESDown  ;
    Float_t  nLooseFwd      , nLooseFwdJESUp      , nLooseFwdJESDown      ;
    Float_t  nBLooseCentral , nBLooseCentralJESUp , nBLooseCentralJESDown ;
    Float_t  nBLooseFwd     , nBLooseFwdJESUp     , nBLooseFwdJESDown     ;
    Float_t  TJet2csv       , TJet2csvJESUp       , TJet2csvJESDown       ;
    Float_t  MSys           , MSysJESUp           , MSysJESDown           ;
    Float_t  TJetLoosept    , TJetLooseptJESUp    , TJetLooseptJESDown    ;
    Float_t  C_jll          , C_jllJESUp          , C_jllJESDown          ;
    Float_t  DilepJetPt     , DilepJetPtJESUp     , DilepJetPtJESDown     ;
    Float_t  TBDT           , TBDTJESUp           , TBDTJESDown           ;


    Float_t  nBTotal          , nBTotalJESUp          , nBTotalJESDown          ; 
    Float_t  DilepmetjetOverHT, DilepmetjetOverHTJESUp, DilepmetjetOverHTJESDown; 
    Float_t  HTLepOverHT      , HTLepOverHTJESUp      , HTLepOverHTJESDown      ; 
    Float_t  TJet1_pt         , TJet1_ptJESUp         , TJet1_ptJESDown         ;







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

  protected:

    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsLHE;

    ClassDef(WZAnalysis, 0);
};
