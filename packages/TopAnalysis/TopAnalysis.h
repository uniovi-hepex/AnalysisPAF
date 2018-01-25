#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include "BTagSFUtil.h"
#include <iostream>
#include <vector>

//enum eChannels{iUnkChan, iElMu, iMuon, iElec, nChannels};
const Int_t nChannels = 3;
enum eLevels  {idilepton, iZVeto, iMETcut, i2jets, i1btag, nLevels};
enum eSysts   {inorm, nSysts};
const int nWeights = 248;
const TString gChanLabel[nChannels] = {"ElMu", "Muon","Elec"};
const TString sCut[nLevels] = {"dilepton", "ZVeto", "MET", "2jets", "1btag"};
const TString gSys[nSysts] = {"0"};
const Int_t nPtBins = 14;
const Float_t ptBins[nPtBins+1] = {30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 200, 300, 400, 600};


class TopAnalysis : public PAFChainItemSelector{
  public:
    TopAnalysis();
    virtual ~TopAnalysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> genLeptons  ;
    std::vector<Lepton> selLeptons  ;
    std::vector<Lepton> vetoLeptons ;
    std::vector<Jet> selJets ;
    std::vector<Jet> selJetsJecUp   ;
    std::vector<Jet> selJetsJecDown ;

    std::vector<Jet> Jets15  ;
    std::vector<Jet> genJets  ;
    std::vector<Jet> mcJets  ;
    std::vector<Jet> vetoJets;

    BTagSFUtil *fBTagSFLoose;

    TTree* fTree;
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
    Bool_t  TPassMETFilters;
    Bool_t  TPassTrigger;
    Bool_t  isSS;
    Float_t NormWeight;

    void InitHistos();
    void FillDYHistos(Int_t ch);
    void FillHistos(Int_t ch, Int_t cut);
    void FillCorrHistos();
  
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

    //Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TDilep_Pt;
    Float_t TMuon_Pt;
    Float_t TElec_Pt;
    Float_t TMuon_Eta;
    Float_t TElec_Eta;
    Int_t   TRun;
    Int_t   TNVert;
    Float_t TMET;      // MET
    Float_t TGenMET;     
    Float_t TMET_Phi;  // MET phi
    Float_t TgenTop1Pt = 0;
    Float_t TgenTop2Pt = 0;

    Int_t   TNVetoLeps;
    Int_t   TNSelLeps;
    Int_t   TChannel;
    Bool_t   TIsSS;
    Float_t TLep0Pt;    
    Float_t TLep0Eta;
    Float_t TLep0Phi;
    Float_t TLep0M;
    Int_t   TLep0Id;
    Float_t TLep1Pt;    
    Float_t TLep1Eta;
    Float_t TLep1Phi;
    Float_t TLep1M;
    Int_t   TLep1Id;

    Int_t TNJets;            // Jets...
    Int_t TNFwdJets; 
    Int_t TNBtags;
    Float_t TJet_Pt[20];
    Float_t TJet_Eta[20];
    Float_t TJet_Phi[20];
    Float_t TJet_M[20];
    Float_t TJet_Csv[20];
    Int_t TNBtagsLoose;
    Float_t TJet0Pt;
    Float_t TJet0Eta;
    Float_t TJet0Csv;

    Float_t TJet0Phi; 
    Float_t TJet0M; 
    Int_t TJet0IsBTag;
    Float_t  TJet1Pt; 
    Float_t TJet1Eta; 
    Float_t TJet1Phi; 
    Float_t TJet1M; 
    Float_t TJet1Csv; 
    Float_t TJet1IsBTag;

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
    Float_t TBtagPt;

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
    /* Float_t  TBDTBTagUp     , TBDTBTagDown; */
    /* Float_t  TBDTMistagUp   , TBDTBMistagDown; */

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
  TH1F* fHDYInvMassSF[nChannels][nLevels][nSysts];
  TH1F* fHInvMass[nChannels][nLevels][nSysts];
  TH1F* fHInvMass2[nChannels][nLevels][nSysts];
  TH1F* fHNBtagsNJets[nChannels][nLevels][nSysts];
  TH1F* fHJetCSV[nChannels][nLevels][nSysts];
  TH1F* fHJet0CSV[nChannels][nLevels][nSysts];
  TH1F* fHJet1CSV[nChannels][nLevels][nSysts];
  TH1F* fHvertices[nChannels][nLevels][nSysts]; 

  TH1F* fhDummy;
  TH1F*  fHWeightsFidu;
  TH1F*  fHyields[nChannels][nSysts];
  TH1F*  fHFiduYields[nChannels][nSysts];
  TH1F*  fHSSyields[nChannels][nSysts];

  TH1F* hJetPtReco;
  TH1F* hJetPtGen;
  TH1F* hJetPtRecoB;
  TH1F* hJetPtGenB;
  TH1F* hJetGenRecoPtRatio[nPtBins];
  TH1F* hJetGenRecoPtRatio2[nPtBins];

  protected:

    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsLHE;

    ClassDef(TopAnalysis, 0);
};
