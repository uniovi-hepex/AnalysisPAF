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
    
    void    SetMiniTreeVariables();

    // Minitree things
    //----------------------------------------------------------------------
    TTree* fTree;

    ////////////////////////////////////////////////////////////////////////
    //	   Events selection
    ////////////////////////////////////////////////////////////////////////
    Bool_t 	PassesPreCuts();
    
    Bool_t 	Is2lSSEvent();
    Bool_t 	Is3lEvent();
    Bool_t 	Is4lEvent();

    ////////////////////////////////////////////////////////////////////////
    //	   Other methods (get, set, misc...)
    ////////////////////////////////////////////////////////////////////////
    void 	  GetParameters();
    void    GetEventVariables();
    void    InitialiseVariables();
    void    Initialise3l4lLeptonSF();
    void    Reset3l4lLeptonSF();
    void    CalculateWeight();
    
    UInt_t  GetnMediumBTags();
    UInt_t  GetnLooseBTags();
    
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
    Bool_t  gIsMCatNLO;
    Float_t gWeight;
    Float_t gPUSF;
    
    //	Weight of the event
    //----------------------------------------------------------------------
    Float_t EventWeight;
    Float_t genWeight;

    //	Minitree variables needed
    //----------------------------------------------------------------------
    Int_t     TCat;
    Float_t   TPtLeading;
    Float_t   TPtSubLeading;
    Float_t   TPtSubSubLeading;
    Int_t     TCS;
    Float_t   TMass;
    Long64_t  Tevt;
    Long64_t  Trun;
    std::vector<Float_t>  TPtVector;

    //	Input variables and vectors
    //----------------------------------------------------------------------
    UInt_t 	nTightLepton;
    UInt_t 	nFakeableLepton;
    UInt_t 	nLooseLepton;
    UInt_t 	nTaus;
    UInt_t 	nJets;
    UInt_t 	nMediumBTags;
    UInt_t 	nLooseBTags;

    std::vector<Lepton> LooseLepton;
    std::vector<Lepton> FakeableLepton;
    std::vector<Lepton> TightLepton;
    std::vector<Lepton> Tau;
    std::vector<Jet>    Jets;

    Float_t MET;
    Float_t METLD;
    Float_t MHT;
    Float_t HT;
    
    Int_t   gChannel;
    Bool_t  passTrigger;
    Bool_t  isSS;

    ClassDef(ttHAnalysis,0);  // ROOT definition as class
};
