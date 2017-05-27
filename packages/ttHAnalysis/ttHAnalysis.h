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

// C++ packages inclusion
#include <vector>
#include <iostream>
#include <math.h>


//------------------------------------------------------------------------------
//		Enumerations, constants and other variable type declarations
//------------------------------------------------------------------------------
// Physical parameters and mathematical constants
const Double_t pi = 3.1415926535897932384;
const Float_t Zm = 91.1876;


//------------------------------------------------------------------------------
//		Classes declarations
//------------------------------------------------------------------------------
// Analysis class
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
    void    CalculateWeight();
    
		Float_t GetMETLD();
    Float_t GetHT();
    Float_t GetMHT();
		Int_t 	GetCS();
    Int_t   GetnMediumBTags();
    Int_t   GetnLooseBTags();


	protected:
		////////////////////////////////////////////////////////////////////////
		//		Data members
		////////////////////////////////////////////////////////////////////////
		//	Analysis parameters
		//----------------------------------------------------------------------
    TString gSampleName;
		Bool_t  gIsData;
		Bool_t  gIsMCatNLO;
		Float_t gWeight;
    
		//	Weight of the event
		//----------------------------------------------------------------------
		Float_t EventWeight;
		Float_t EventWeight_PUUp;
		Float_t EventWeight_PUDown;
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
		std::vector<Float_t>  TPtVector;

		//	Input variables and vectors
		//----------------------------------------------------------------------
		Int_t 	nTightLepton;
		Int_t 	nFakeableLepton;
		Int_t 	nLooseLepton;
		Int_t 	nTaus;
		Int_t 	nJets;
		Int_t 	nMediumBTags;
		Int_t 	nLooseBTags;

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
    
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    
		ClassDef(ttHAnalysis,0);  // ROOT definition as class
};
