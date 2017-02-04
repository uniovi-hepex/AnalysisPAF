#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "Jet.h"
#include "BTagSFUtil.h"
#include <iostream>
#include <vector>

enum wps{iVeto, iLoose, iMedium, iTight, iVeryTight};
enum sel{iStopSelec, iOldStopSelec, iTopSelec, iWWSelec, ittDMSelec};
enum vdir{inom, iup, idown}

class LeptonSelector : public PAFChainItemSelector{

	public:

		JetSelector() : PAFChainItemSelector() {}
		virtual ~JetSelector() {}
		virtual void InsideLoop();
		void Initialise();
		void Summary();
		void GetParameters();

		ClassDef(JetSelector, 0);

		std::vector<Jet> selJets;
		std::vector<Jet> genJets;
		std::vector<Jet> vetoJets;

	protected:

		Bool_t lIsData;
		Bool_t lSelection;

		// Jets
		Int_t nJet;
		Jet tJ;
		TLorentzVector tJ; 
		Int_t idVar;
		Int_t mediumMuonId;
    Int_t mcflav;
		Int_t isBtag;
		// genJet
    Int_t ngenJet;
		TLorentzVector gtP;
		Int_t isgenBtag;

		void GetJetVariables(Int_t i);
		Bool_t getBtag(); 
		Bool_t getJetId(Int_t wp); 
		Bool_t isGoodJet(Jet lep);
		Bool_t isVetoJet(Jet lep);

  ClassDef(JetSelector, 1);
};

