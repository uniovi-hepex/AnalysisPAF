/*==============================================================================

                            ttHAnalyzer selector

==============================================================================*/
//------------------------------------------------------------------------------
//		Preprocessor directives
//------------------------------------------------------------------------------

#pragma once

// PAF inclusion
#include "PAFChainItemSelector.h"

// Analysis packages inclusion
//#include "GlobalVariables.h"
#include "BTagSFUtil.h"

// ROOT packages inclusion
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"
#include "TString.h"

// C++ packages inclusion
#include <vector>


//------------------------------------------------------------------------------
//		Enumerations, constants and other variable type declarations
//------------------------------------------------------------------------------
// Physical parameters and mathematical constants
const Double_t pi = 3.1415926535897932384;
const Float_t Zm = 91.1876;

// Cuts
const Float_t gJetEtCut = 25.;
const Float_t gTightMuonPtCut = 10.;
const Float_t gFakeableMuonPtCut = 10.;
const Float_t gLooseMuonPtCut = 5.;
const Float_t gTightElectronPtCut = 10.;
const Float_t gFakeableElectronPtCut = 10.;
const Float_t gLooseElectronPtCut = 7.;

// Other variable type definitions and other constants
enum gCategories {
    categories_begin,
    twolSS = categories_begin,
    threel,
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

const TString gCatLabel	[gNCATEGORIES] 	= {"2lSS","3l","Total"};
const TString gChanLabel[gNCHANNELS] 	= {"MuMu","ElEl","ElMu","All"};

//------------------------------------------------------------------------------
//		Classes declarations
//------------------------------------------------------------------------------
// Particles class
class lepton {
    public:
        //lepton(){}
        //lepton(const lepton &l): p(l.p), charge(l.charge), type(l.type), index(l.index){ };
        lepton(TLorentzVector vec = TLorentzVector(0,0,0,0), Int_t ch = 0, Int_t ty = -1, Int_t ind = -1){
            p = vec;
            charge = ch;
            type = ty;
            index = ind;
        }
    	TLorentzVector p;
    	Int_t charge;
    	Int_t type; // -1(unknown), 0(tight muon), 1(tight electron); 2(fakeable muon), 3(fakeable electron); 4(loose muon), 5(loose electron)
    	Int_t index;
};

class jet {
	public:
    	jet(){};
    	jet(TLorentzVector vec, Int_t btag, Int_t ind){
    		p = vec;
    		isbtag = btag;
    		index = ind;
    	};
    	TLorentzVector p;
    	Int_t isbtag; // =1 for MEDIUM (~tight) b-tagged jets, =-1 for loose b-tagged jets and 0 for no b-tagged jets.
    	Int_t index;
};

// Analysis class
class ttHAnalyzer : public PAFChainItemSelector {
	public:
		////////////////////////////////////////////////////////////////////////
		//		Initial declarations
		////////////////////////////////////////////////////////////////////////
		// Constructor and destructor
		ttHAnalyzer();
		virtual ~ttHAnalyzer() {}

		// Core PAF-analysis methods
		virtual void Initialise();
		virtual void InsideLoop();
		virtual void Summary();

        // For printing

		////////////////////////////////////////////////////////////////////////
		//		Trees-related declarations
		////////////////////////////////////////////////////////////////////////
		//	Methods
		//----------------------------------------------------------------------
		void 	GetTreeVariables();
		void 	GetParameters();

        //	Tree variables
		//----------------------------------------------------------------------
		Int_t   nLepGood;
        Int_t   nTauGood;
		Int_t   nJet;
        Long_t  evt;
		Int_t   run;
		Float_t LepGood_px[30];
		Float_t LepGood_py[30];
		Float_t LepGood_pz[30];
		Float_t LepGood_energy[30];
		Float_t LepGood_pt[30];
		Float_t LepGood_eta[30];
		Float_t LepGood_dxy[30];
		Float_t LepGood_dz[30];
		Int_t   LepGood_charge[30];
		Int_t   LepGood_pdgId[30];
		Float_t LepGood_sip3d[30];
		Float_t LepGood_miniRelIso[30];
		Float_t LepGood_jetBTagCSV[30];
		Int_t 	LepGood_mediumMuonId[30];
		Float_t LepGood_mvaTTH[30];
		Float_t LepGood_jetPtRatiov2[30];
		Float_t	LepGood_mvaIdSpring16GP[30];
		Float_t LepGood_sigmaIEtaIEta[30];
		Float_t LepGood_hadronicOverEm[30];
		Float_t LepGood_dEtaScTrkIn[30];
		Float_t LepGood_dPhiScTrkIn[30];
		Float_t LepGood_eInvMinusPInv[30];
		Float_t LepGood_convVeto[30];
		Int_t 	LepGood_lostHits[30];
		Int_t 	LepGood_tightCharge[30];
		Float_t LepGood_jetDR[30];
		Float_t Jet_px[50];
		Float_t Jet_py[50];
		Float_t Jet_pz[50];
		Float_t Jet_energy[50];
		Float_t Jet_eta[50];
		Float_t Jet_btagCSV[50];
		Int_t	TauGood_idDecayModeNewDMs[30];
		Float_t	TauGood_pt[30];
		Float_t	TauGood_eta[30];
		Float_t	TauGood_phi[30];
		Float_t	TauGood_mass[30];
		Int_t	TauGood_idCI3hit[30];
		//Int_t TauGood_idAntiMu[30];
		//Int_t TauGood_idAntiE[30];

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
		//	   Leptons and jets selection
		////////////////////////////////////////////////////////////////////////
		Int_t  	getSelectedLeptons();
		std::vector<lepton> SortLeptonsByPt(std::vector<lepton>&);

        //  Muons
        //----------------------------------------------------------------------
		Bool_t	IsTightMuon(UInt_t, Float_t ptcut = gTightMuonPtCut);
		Bool_t	IsFakeableMuon(UInt_t, Float_t ptcut = gFakeableMuonPtCut);
		Bool_t	IsLooseMuon(UInt_t, Float_t ptcut = gLooseMuonPtCut);

        //  Electrons
        //----------------------------------------------------------------------
		Bool_t	IsTightElectron(UInt_t,Float_t ptcut = gTightElectronPtCut, Int_t an=2);
		Bool_t	IsFakeableElectron(UInt_t,Float_t ptcut = gFakeableElectronPtCut);
		Bool_t	IsLooseElectron(UInt_t,Float_t ptcut = gLooseElectronPtCut);

        //  Taus
        //----------------------------------------------------------------------
		Bool_t 	IsGoodTau(UInt_t iTau);

        //  Jets
        //----------------------------------------------------------------------
		Int_t 	getSelectedJets();
		Bool_t	IsGoodJet(UInt_t, Float_t ptcut=25.);
		Bool_t 	IsGoodJetforprecuts(UInt_t, Float_t ptcut=25.);

		////////////////////////////////////////////////////////////////////////
		//	   Events selection
		////////////////////////////////////////////////////////////////////////
		Int_t  	IsDileptonEvent();
		Bool_t	IsMuMuEvent();
		Bool_t	IsElMuEvent();
		Bool_t	IsElElEvent();
	    Bool_t 	IsSSEvent();
	    Bool_t 	Is2lSSEvent();
	    Bool_t 	Is3lEvent();

		Bool_t 	PassesPreCuts();

        ////////////////////////////////////////////////////////////////////////
		//	   Trigger methods
		////////////////////////////////////////////////////////////////////////
		Bool_t	triggermumuSS();
		Bool_t	triggereeSS();
		Bool_t	triggeremuSS();
		Bool_t	trigger3l4l();

        ////////////////////////////////////////////////////////////////////////
		//	   Set/reset methods
		////////////////////////////////////////////////////////////////////////
		void 	SetOriginalObjects();
		void 	SetEventObjects();
		void 	ResetHypLeptons();
        void    setMET(Float_t);

        ////////////////////////////////////////////////////////////////////////
		//	   Get methods
		////////////////////////////////////////////////////////////////////////
		Float_t getMET();
		Float_t getHT();
		Float_t getMHT();
		Float_t getMETLD();
		Int_t 	getCS();

	protected:
		////////////////////////////////////////////////////////////////////////
		//		Data members
		////////////////////////////////////////////////////////////////////////
		//	Input parameters
		//----------------------------------------------------------------------
	    TString gSampleName;
		Bool_t  gIsData;
		Bool_t  gIsMCatNLO;
		Float_t gWeight;
		Float_t gTotalLumi;

		//	PU and SF
		//----------------------------------------------------------------------
		BTagSFUtil    *medfBTagSFnom;
		BTagSFUtil    *losfBTagSFnom;

		//	EventWeight
		//----------------------------------------------------------------------
		Float_t EventWeight;
		Float_t genWeight;

		//	Histograms and trees
		//----------------------------------------------------------------------
		TH1F*   fHDummy;
		TH1F*   fHEvents    		[gNCATEGORIES][gNCHANNELS]; // Events
		TH1F*   fHTightLep			[gNCATEGORIES][gNCHANNELS]; // Yields
		TH1F*   fHFakeLep			[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHLooseLep			[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHTau				[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHJet				[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMedBTagJet		[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHLosBTagJet		[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHPtLeading			[gNCATEGORIES][gNCHANNELS]; // Kinematic
		TH1F*   fHPtSubLeading		[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHPtSubSubLeading	[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMET				[gNCATEGORIES][gNCHANNELS]; // MET
		TH1F*   fHMHT				[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHMETLD				[gNCATEGORIES][gNCHANNELS];
		TH1F*   fHChargeSum			[gNCATEGORIES][gNCHANNELS]; // Misc
		TH1F*   fHMass				[gNCATEGORIES][gNCHANNELS];

		//	General
		//----------------------------------------------------------------------
		UInt_t 	nJets;	// nJets = nBtags + n(NOTBTags)
		UInt_t 	nBTags; // nBTags = nMediumBTags + nLooseBTags
		UInt_t 	nMediumBTags;
		UInt_t 	nLooseBTags;
		UInt_t 	nLeptons; // nLeptons does NOT include taus
		UInt_t 	nTightMuon;
		UInt_t 	nFakeableMuon;
		UInt_t 	nLooseMuon;
		UInt_t 	nTightElec;
		UInt_t 	nFakeableElec;
		UInt_t 	nLooseElec;
		UInt_t 	nTaus;
		UInt_t 	njpt;

		Float_t MET;
		Float_t MHT;

		lepton  fHypLepton1;
		lepton  fHypLepton2;
		std::vector<lepton> Lepton;
		std::vector<lepton> LooseLepton;
		std::vector<lepton> FakeableLepton;
		std::vector<lepton> TightLepton;
		std::vector<jet>    Jet;

		std::vector<Float_t> JetEt;
		std::vector<Float_t> JetPt;
		std::vector<Float_t> JetPhi;
		std::vector<Float_t> MuPx;
		std::vector<Float_t> MuPy;
		std::vector<Float_t> MuPz;
		std::vector<Float_t> MuEnergy;
		std::vector<Float_t> ElPx;
		std::vector<Float_t> ElPy;
		std::vector<Float_t> ElPz;
		std::vector<Float_t> ElEnergy;

		ClassDef(ttHAnalyzer,0);  // ROOT definition as class
};
