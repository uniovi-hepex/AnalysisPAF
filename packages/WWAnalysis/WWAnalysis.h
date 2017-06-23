#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>

class WWAnalysis : public PAFChainItemSelector{
	public:
    TTree* fTree;

		WWAnalysis();
		virtual ~WWAnalysis(){}
		virtual void InsideLoop();
		virtual void Initialise();
		virtual void Summary();
		std::vector<Lepton> selLeptons  ;
		std::vector<Lepton> genLeptons  ;
		std::vector<Lepton> vetoLeptons ;
		std::vector<Jet> selJets ;
		std::vector<Jet> Jets15  ;
		std::vector<Jet> vetoJets ;
		Int_t nVetoJets;
		
		Float_t TLHEWeight[254];
		void SetLeptonVariables();
		void SetJetVariables();
		void SetEventVariables();

		void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons);
		void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 20);
		void GetMET();

		Float_t TrigSF;
		Float_t TrigSF_Up;
		Float_t TrigSF_Down;
    		Float_t TrigSFerr;
		Float_t PUSF;
		Float_t PUSF_Up;
		Float_t PUSF_Down;
		Int_t gChannel;
		Bool_t passMETfilters;
		Bool_t passTrigger;
		Bool_t TIsSS;
		Float_t NormWeight;
  
    // Tree Variables
		Float_t TWeight;   // Total nominal weight
		Float_t TMll;      // Invariant mass
		Float_t TPtdil;      // Dilepton pT
		Float_t TMET;      // MET
		Float_t TMET_Phi;  // MET phi
		Float_t TgenMET;
		Float_t TMT2;     // Invariant mass transverse 2
		Float_t TMT;
		Float_t TMTprime;
		Float_t TDeltaPhi;
		Float_t TDeltaEta;

		Int_t   TNVetoLeps;
		Int_t   TNSelLeps;
		Int_t   TNVetoJets;
		Int_t TChannel;
		Float_t TLep_Pt[10];    
		Float_t TLep_Eta[10];
		Float_t TLep_Phi[10];
		Float_t TLep_E[10];
		Float_t TLep_Charge[10];

		Int_t TNJets;            // Jets...
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
		Int_t   TNJetsJER;
    		Int_t   TNVetoJetsJESUp;
    		Int_t   TNVetoJetsJESDown;
		Int_t   TNVetoJetsBtagUp;
		Int_t   TNVetoJetsBtagDown;
		Int_t   TNVetoJetsMisTagUp;
		Int_t   TNVetoJetsMisTagDown;
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

  
	protected:
		Bool_t  gIsData;
		Bool_t  gDoSyst;
		Int_t   gSelection;
		TString gSampleName;
 		Bool_t  gIsLHE;

		ClassDef(WWAnalysis, 0);
};
