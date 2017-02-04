#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "LeptonSF.h"
#include <iostream>
#include <vector>

:
class LeptonSelector : public PAFChainItemSelector{

	public:

		LeptonSelector() : PAFChainItemSelector() {}
		LeptonSelector(); 
		virtual ~LeptonSelector() {}
		virtual void InsideLoop();
		virtual void Initialise();
		virtual void Summary();

		std::vector<Lepton> selLeptons;
		std::vector<Lepton> genLeptons;
		std::vector<Lepton> vetoLeptons;




	protected:

		LeptonSF *fLeptonSF;
		Bool_t lIsData;
		Bool_t gSelection;

		// LepGood
		Lepton tL;
		Int_t nLep;
		TLorentzVector tP; 
    Float_t pt;
    Float_t eta;
		Int_t charge; 
		Int_t type;
		Int_t tightVar;
		Int_t mediumMuonId;
		Float_t etaSC;
		Float_t RelIso03;
		Float_t RelIso04;
		Float_t ptRel;
		Float_t ptRatio;
		Float_t miniIso;
		Float_t dxy;
		Float_t dz; 
		Float_t sip;
    Float_t SF;
		// genLeptons
		std::vector<Lepton> gtL;
    Int_t ngenLep;
		Int_t ngenLepFromTau;
		TLorentzVector gtP;
		Int_t gpdgId;

		void GetLeptonVariables(Int_t i);
		Bool_t getSIPcut(Float_t cut); 
		Bool_t getGoodVertex(Int_t wp); 
		Bool_t getRelIso03POG(Int_t wp); 
		Bool_t getRelIso04POG(Int_t wp);
		Bool_t getMuonId(Int_t wp);
		Bool_t getElecCutBasedId(Int_t wp); 
		Bool_t getMultiIso(Int_t wp);
		Bool_t isGoodLepton(Lepton lep);
		Bool_t isVetoLepton(Lepton lep);

    // SFs...
		Float_t GetMuonIsoSF(Int_t wp, Int_t sys);
		Float_t GetMuonIdSF(Int_t wp, Int_t sys);
		Float_t GetMuonIP2DSF(Int_t wp, Int_t sys);
		Float_t GetMuonSIP3DSF(Int_t wp, Int_t sys);
		Float_t GetMuonTrackerSF(Int_t sys);

		Float_t GetElectonIdIsoSF(Int_t wp, Int_t sys);
		Float_t GetElecIP2DSF(Int_t wp, Int_t sys);
		Float_t GetElecSIP3DSF(Int_t wp, Int_t sys);
		Float_t GetElecTrackerSF(Int_t sys);

  ClassDef(LeptonSelector, 0);
};

