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

enum gChannel {
    channels_begin,
    MuMu = channels_begin,
    ElEl,
    ElMu,
    All,
    gNCHANNELS,
};

const TString gCatLabel	[gNCATEGORIES] 	= {"2lSS","3l","4l","Total"};
const TString gChanLabel[gNCHANNELS] 	  = {"MuMu","ElEl","ElMu","All"};

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
		void 	GetTreeVariables();
    void  SetLeptonBranches();
    void  SetJetBranches();
    void  SetEventBranches();
    void  SetMiniTreeVariables();

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
		void 	FillEventHistos();
		void 	FillYieldHistos();
		void 	FillKinematicHistos();
		void 	FillMETHistos();
		void 	FillMiscHistos();

		////////////////////////////////////////////////////////////////////////
		//	   Events selection
		////////////////////////////////////////////////////////////////////////
    Bool_t 	Is2lSSEvent();
    Bool_t 	Is3lEvent();
    Bool_t 	Is4lEvent();

		Bool_t 	PassesPreCuts();

    ////////////////////////////////////////////////////////////////////////
		//	   Get/set/other methods
		////////////////////////////////////////////////////////////////////////
		Float_t getMET();
		Float_t getMETLD();
		Int_t 	getCS();
		void 	  GetParameters();
    Int_t   GetnLooseBTags();
    void    GetEventVariables();
    void    InitialiseVariables();

	protected:
		////////////////////////////////////////////////////////////////////////
		//		Data members
		////////////////////////////////////////////////////////////////////////
		//	Essential and global parameters
		//----------------------------------------------------------------------
    TString gSampleName;
		Bool_t  gIsData;
		Bool_t  gIsMCatNLO;
		Float_t gWeight;

		//	EventWeight
		//----------------------------------------------------------------------
		Float_t EventWeight;
		Float_t genWeight;

		//	Histograms and trees
		//----------------------------------------------------------------------
		TH1F*   fHEvents    		  [gNCATEGORIES][gNCHANNELS]; // Events
		TH1F*   fHTightLep			  [gNCATEGORIES][gNCHANNELS]; // Yields
		TH1F*   fHFakeLep			    [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHLooseLep			  [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHTau				      [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHJet				      [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMedBTagJet		  [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHLosBTagJet		  [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHPtLeading			  [gNCATEGORIES][gNCHANNELS]; // Kinematic
		TH1F*   fHPtSubLeading		[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHPtSubSubLeading	[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMET				      [gNCATEGORIES][gNCHANNELS]; // MET
		TH1F*   fHMHT				      [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMETLD				    [gNCATEGORIES][gNCHANNELS];
		TH1F*   fHChargeSum			  [gNCATEGORIES][gNCHANNELS]; // Misc
		TH1F*   fHMass				    [gNCATEGORIES][gNCHANNELS];

    Int_t   TCat;
    Int_t   TisSS;
    Int_t   TIsEvent;
    Float_t TPtLeading;
    Float_t TPtSubLeading;
    Float_t TPtSubSubLeading;
    Int_t   TCS;
    Float_t TMass;
    Float_t TMETLD;
    Float_t TWeight;
    Int_t   TpassTrigger;
    Int_t   TChannel;

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
		Float_t MHT;

    Int_t   gChannel;
    Bool_t  passTrigger;
    Bool_t  isSS;

		ClassDef(ttHAnalysis,0);  // ROOT definition as class
};
