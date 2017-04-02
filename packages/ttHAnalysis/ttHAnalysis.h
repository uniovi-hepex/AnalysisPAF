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

// Other variable type definitions and other constants
enum gCategories {
    categories_begin,
    twolSS = categories_begin,
    threel,
    fourl,
    Total,
    gNCATEGORIES,
};

const TString gCatLabel	[gNCATEGORIES] 	= {"2lSS","3l","4l","Total"};
const TString gChanLabel[4] 	  = {"MuMu","ElEl","ElMu","All"};

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
		//		Histogram-related methods declarations
		////////////////////////////////////////////////////////////////////////
    // Initialising
		//----------------------------------------------------------------------
  	virtual void InitialiseEventHistos();
  	virtual void InitialiseYieldHistos();
  	virtual void InitialiseKinematicHistos();
  	virtual void InitialiseMETHistos();
  	virtual void InitialiseMiscHistos();

    //	Filling methods
		//----------------------------------------------------------------------
		void 	  FillEventHistos();
		void 	  FillYieldHistos();
		void 	  FillKinematicHistos();
		void 	  FillMETHistos();
		void 	  FillMiscHistos();

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
    
		Float_t GetMETLD();
    Float_t GetHT();
    Float_t GetMHT();
		Int_t 	GetCS();
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
		Float_t genWeight;

		//	Histograms
		//----------------------------------------------------------------------
		TH1F*   fHEvents    		  [gNCATEGORIES][4]; // Events
		TH1F*   fHTightLep			  [gNCATEGORIES][4]; // Yields
		TH1F*   fHFakeLep			    [gNCATEGORIES][4];
		TH1F*   fHLooseLep			  [gNCATEGORIES][4];
		TH1F*   fHTau				      [gNCATEGORIES][4];
		TH1F*   fHJet				      [gNCATEGORIES][4];
		TH1F*   fHMedBTagJet		  [gNCATEGORIES][4];
		TH1F*   fHLosBTagJet		  [gNCATEGORIES][4];
		TH1F*   fHPtLeading			  [gNCATEGORIES][4]; // Kinematic
		TH1F*   fHPtSubLeading		[gNCATEGORIES][4];
		TH1F*   fHPtSubSubLeading	[gNCATEGORIES][4];
		TH1F*   fHMET				      [gNCATEGORIES][4]; // MET
		TH1F*   fHMHT				      [gNCATEGORIES][4];
		TH1F*   fHMETLD				    [gNCATEGORIES][4];
		TH1F*   fHChargeSum			  [gNCATEGORIES][4]; // Misc
		TH1F*   fHMass				    [gNCATEGORIES][4];

    //	Minitree variables needed
		//----------------------------------------------------------------------
    Int_t   TCat;
    Float_t TPtLeading;
    Float_t TPtSubLeading;
    Float_t TPtSubSubLeading;
    Int_t   TCS;
    Float_t TMass;

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

		ClassDef(ttHAnalysis,0);  // ROOT definition as class
};
