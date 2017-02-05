#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "Jet.h"
#include "BTagSFUtil.h"
#include "Functions.h"
#include <iostream>
#include <vector>

enum wps{iVeto, iLoose, iMedium, iTight, iVeryTight};
enum sel{iStopSelec, iOldStopSelec, iTopSelec, iWWSelec, ittDMSelec};
enum vdir{inom, iup, idown}

class LeptonSelector : public PAFChainItemSelector{

	public:

		JetSelector();
		virtual ~JetSelector() {}
		virtual void InsideLoop();
		void Initialise();
		void Summary();

    std::vector<Lepton> Leptons;
		std::vector<Jet> selJets;
		std::vector<Jet> Jets15;
		std::vector<Jet> genJets;
		std::vector<Jet> mcJets;
		std::vector<Jet> vetoJets;

	protected:

		Bool_t gIsData;
		Bool_t gSelection;

		// Jets
		Int_t nJet;
		Jet tJ;
		TLorentzVector tpJ; 
		TLorentzVector tmcJ; 
		Int_t jetId;
    Int_t mcflav;
		Int_t csv;
		// genJet
    Int_t ngenJet;
		TLorentzVector gtP;
		Int_t isgenBtag;

		void GetJetVariables(Int_t i);
		Bool_t IsBtag(Jet j); 
    void SetSystematics(Jet j);
    Bool_t Cleaning(Jet j, vector<Lepton> vLep);

  ClassDef(JetSelector, 0);
};

