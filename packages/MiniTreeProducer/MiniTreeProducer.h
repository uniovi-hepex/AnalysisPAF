#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include "Lepton.h"
#include "Jet.h"

class MiniTreeProducer : public PAFChainItemSelector{
	public:
    TTree* fTree;

		MiniTreeProducer();
		virtual ~MiniTreeProducer(){}
		virtual void InsideLoop();
		virtual void Initialise();
		virtual void Summary();

		void GetDefaultVariables();
		void GetSystematicVariables();

		// Tree variables ..................................................
		Int_t TChannel;
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TMET;      // MET
    Float_t TMET_Phi;  // MET phi
    Float_t THT;       // HT
    Float_t TLep1_Px;    // Leptons...
    Float_t TLep1_Py;
    Float_t TLep1_Pz;
    Float_t TLep1_E;
    Float_t TLep1_Charge;
    Float_t TLep2_Px;
    Float_t TLep2_Py;
    Float_t TLep2_Pz;
    Float_t TLep2_E;
    Float_t TLep2_Charge;

    Int_t TNJets;            // Jets...
    Int_t TNJetsBtag;
    Float_t TJet_Px[40];
    Float_t TJet_Py[40];
    Float_t TJet_Pz[40];
    Float_t TJet_E[40];
    Int_t TJet_isBJet[40];

    // For systematics...
    Int_t   TNISRJets;
    Int_t   TNJetsJESUp;
    Int_t   TNJetsJESDown;
    Int_t   TNJetsJER;
    Int_t   TNJetsBtagUp;
    Int_t   TNJetsBtagDown;
    Int_t   TNJetsBtagMisTagUp;
    Int_t   TNJetsBtagMisTagDown;
    Float_t TJetJESUp_Pt[40];
    Float_t TJetJESDown_Pt[40];
    Float_t TJetJER_Pt[40];

    Float_t TGenMET;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t THTJESUp;
    Float_t THTJESDown;
    Float_t TMT2llJESUp;
    Float_t TMT2llJESDown;

    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSDown;

    Float_t TMT2ll;
    Float_t TMT2jj;
    Float_t TMT2lblb;
    Float_t TPtllb;
    Float_t TMeff;
    Float_t TdPhiPtllbMET;
    Float_t TdPhiJetMet;
    Float_t TdPhiLepMet;
    Float_t TdPhiLepJet;
    Float_t TdPhill;
    Float_t TdEtall;
    Float_t TMinDPhiMetJets;

	protected:

    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;

		ClassDef(MiniTreeProducer, 0);
};
