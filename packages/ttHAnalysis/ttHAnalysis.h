/*==============================================================================

                            ttHAnalysis selector

==============================================================================*/
//------------------------------------------------------------------------------
//		Preprocessor directives
//------------------------------------------------------------------------------
#pragma once

// PAF inclusion
#include "PAFChainItemSelector.h"

// Analysis packages inclusion
#include "Functions.h"
#include "LeptonSF.h"

// C++ packages inclusion
#include <vector>
#include <iostream>
#include <math.h>


class ttHAnalysis : public PAFChainItemSelector {
  public:
    ////////////////////////////////////////////////////////////////////////
    //		Initial declarations
    ////////////////////////////////////////////////////////////////////////
    // Constructor and destructor
    ttHAnalysis();
    virtual ~ttHAnalysis() {}

    // Core PAF-analysis methods
    virtual void Initialise();
    virtual void InsideLoop();
    virtual void Summary();

    ////////////////////////////////////////////////////////////////////////
    //		Trees-related declarations
    ////////////////////////////////////////////////////////////////////////
    //	Methods
    //----------------------------------------------------------------------
    void 	  GetTreeVariables();
    void    SetLeptonBranches();
    void    SetJetBranches();
    void    SetEventBranches();
    void    SetSystBranches();
    
    void    SetMiniTreeVariables();

    // Minitree things
    //----------------------------------------------------------------------
    TTree* fTree;

    ////////////////////////////////////////////////////////////////////////
    //	   Events selection
    ////////////////////////////////////////////////////////////////////////
    Bool_t 	PassesPreCuts(UInt_t njets, UInt_t nloosebtag, UInt_t nmediumbtag);
    
    Bool_t 	Is2lSSEvent(UInt_t njets, Float_t metld);
    Bool_t 	Is3lEvent(UInt_t njets, Float_t metld);
    Bool_t 	Is4lEvent();

    ////////////////////////////////////////////////////////////////////////
    //	   Other methods (get, set, misc...)
    ////////////////////////////////////////////////////////////////////////
    void 	  GetParameters();
    void    GetEventVariables();
    void    GetJESJetVariables();
    void    InitialiseVariables();
    void    ResetVariables();    
    void    Initialise3l4lLeptonSF();
    void    Reset3l4lLeptonSF();
    void    CalculateWeight();
    
    UInt_t  GetnMediumBTags(vector<Jet> jets);
    UInt_t  GetnLooseBTags(vector<Jet> jets);
    
    Bool_t 	PassesTightChargeCuts();

  protected:
    ////////////////////////////////////////////////////////////////////////
    //		Data members
    ////////////////////////////////////////////////////////////////////////
    //	Analysis parameters
    //----------------------------------------------------------------------
    TString gSampleName;
    TString gLocalPath;
    Bool_t  gIsData;
    Bool_t  gIsLHE;
    Bool_t  gIsMCatNLO;
    Float_t gWeight;
    Float_t gPUSF;
    
    
    //  SF tool for resetting 3l and 4l leptons SF.
    //----------------------------------------------------------------------    
    LeptonSF *Lep3l4lSF;
    
    //	Weight of the event
    //----------------------------------------------------------------------
    Float_t EventWeight;
		Float_t EventWeight_PUUp;
		Float_t EventWeight_PUDown;
		Float_t EventWeight_TrigUp;
		Float_t EventWeight_TrigDown;
		Float_t EventWeight_ElecUp;
		Float_t EventWeight_ElecDown;
		Float_t EventWeight_MuonUp;
		Float_t EventWeight_MuonDown;
    Float_t genWeight;

    //	Minitree variables needed
    //----------------------------------------------------------------------
    Int_t     TCat;
    Int_t     TCatJESUp;
    Int_t     TCatJESDown;
    Float_t   TPtLeading;
    Float_t   TPtSubLeading;
    Float_t   TPtSubSubLeading;
    Float_t   TPtSubSubSubLeading;
    Int_t     TCS;
    Float_t   TMass;
    ULong64_t Tevt;
    UInt_t    Trun;
    std::vector<Float_t>  TPtVector;
    
    Float_t   THTJESUp;
    Float_t   THTJESDown;
    Float_t   TMHTJESUp;
    Float_t   TMHTJESDown;
    Float_t   TMETJESUp;
    Float_t   TMETJESDown;
    Float_t   TMETLDJESUp;
    Float_t   TMETLDJESDown;
    Float_t   TnJetsJESUp;
    Float_t   TnJetsJESDown;
    Float_t   TnLooseBTagsJESUp;
    Float_t   TnLooseBTagsJESDown;
    Float_t   TnMediumBTagsJESUp;
    Float_t   TnMediumBTagsJESDown;

    Float_t   TLHEWeight[254];

    Bool_t THLT_DiMu9_Ele9_CaloIdL_TrackIdL_v                   ;
    Bool_t THLT_Mu8_DiEle12_CaloIdL_TrackIdL_v                  ;
    Bool_t THLT_TripleMu_12_10_5_v                              ;
    Bool_t THLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v             ;
    Bool_t THLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v    ;
    Bool_t THLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v ;
    Bool_t THLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v    ;
    Bool_t THLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v ;
    Bool_t THLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v         ;
    Bool_t THLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v               ;
    Bool_t THLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v             ;
    Bool_t THLT_IsoMu22_v                                       ;
    Bool_t THLT_IsoTkMu22_v                                     ;
    Bool_t THLT_IsoMu24_v                                       ;
    Bool_t THLT_IsoTkMu24_v                                     ;
    Bool_t THLT_Ele27_WPTight_Gsf_v                             ;
    Bool_t THLT_Ele25_eta2p1_WPTight_Gsf_v                      ;
    Bool_t THLT_Ele27_eta2p1_WPLoose_Gsf_v                      ;

    //	Input variables and vectors
    //----------------------------------------------------------------------
    UInt_t 	nTightLepton;
    UInt_t 	nFakeableLepton;
    UInt_t 	nLooseLepton;
    UInt_t 	nTaus;
    UInt_t 	nJets;
    UInt_t 	nJets15;
    UInt_t 	nMediumBTags;
    UInt_t 	nLooseBTags;

    std::vector<Lepton> LooseLepton;
    std::vector<Lepton> FakeableLepton;
    std::vector<Lepton> TightLepton;
    std::vector<Lepton> Tau;
    std::vector<Jet>    Jets;
    std::vector<Jet>    Jets15;
    
    Float_t MET;
    Float_t METLD;
    Float_t MHT;
    Float_t HT;
    
    Int_t   gChannel;
    Bool_t  passTrigger;
    Bool_t  isSS;
    
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    
    ClassDef(ttHAnalysis,0);  // ROOT definition as class
};
