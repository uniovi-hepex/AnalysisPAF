#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1

#include "Lepton.h"
#include "Jet.h"
#include <iostream>
#include <vector>

using namespace std;

// enum 
enum iSFs{
	iMuonReco, iMuonId, iMuonIso, iMuonIP2D, iMuonSIP3D,
	iElecReco, iElecId, iElecIso, iElecIP2D, iElecSIP3D,
	iTrigDoubleMuon, iTrigDoubleElec, iTrigElMu,
	nfiles
};

enum wps{iVeto, iLoose, iMedium, iTight, iVeryTight};

enum sel{iStopSelec, iOldStopSelec, iTopSelec, iWWSelec, ittDMSelec};

enum sys{iNom, 
	iJesUp, iJesDown, iJERUp, iJERDown, 
	iBtagUp, iBtagDown, iBtagMisUp, iBtagMisDown, 
	iLepEffUp, iLepEffDown, 
	iTrigUp, iTrigDown, 
	iPUUp, iPUDown, 
	iFSUp, iFSDown,
	nSys
};

bool LepMomentumComparator(Lepton i, Lepton j);
bool JetMomentumComparator(Jet i   , Jet    j);

vector<Lepton> SortLeptonsByPt(vector<Lepton>& Leptons);
vector<Jet>       SortJetsByPt(vector<Jet>   & Jets);

Float_t JEStoMET(vector<Jet> vjets, Float_t met, Float_t met_phi, Int_t dir = 0);
Float_t getJetJERpt(Jet jet);




#endif
