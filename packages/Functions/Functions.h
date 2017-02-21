#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1

#include "Lepton.h"
#include "Jet.h"
#include "TTree.h"
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

enum sel{iStopSelec, iOldStopSelec, iTopSelec, iWWSelec, ittDMSelec, ittHSelec};

enum sys{iNom, 
	iJesUp, iJesDown, iJERUp, iJERDown, 
	iBtagUp, iBtagDown, iBtagMisUp, iBtagMisDown, 
	iLepEffUp, iLepEffDown, 
	iTrigUp, iTrigDown, 
	iPUUp, iPUDown, 
	iFSUp, iFSDown,
	nSys
};

enum eChannel{iElMu, iElec, iMuon, nChannels};

bool LepMomentumComparator(Lepton i, Lepton j);
bool JetMomentumComparator(Jet i   , Jet    j);

vector<Lepton> SortLeptonsByPt(vector<Lepton>& Leptons);
vector<Jet>       SortJetsByPt(vector<Jet>   & Jets);

Float_t JEStoMET(vector<Jet> vjets, Float_t met, Float_t met_phi, Int_t dir = 0);
Float_t getJetJERpt(Jet jet);
Float_t p2(Float_t x);

Float_t getDeltaPhillJet(Lepton lep1, Lepton lep2, Jet jet);
Float_t getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep);
Float_t getHT(vector<Jet> jet);
Float_t getMT(TLorentzVector v1, TLorentzVector v2);
Float_t getMinDPhiMetJets(vector<Jet> vjets, Float_t met, Float_t met_phi);
Float_t getDelPhi(TLorentzVector v1, TLorentzVector v2);
Float_t getDelEta(TLorentzVector v1, TLorentzVector v2);
Float_t getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, Float_t mass);
Float_t getMT2ll(Lepton l1, Lepton l2, Float_t met, Float_t met_phi);
Float_t getMeff(Lepton l1, Lepton l2, vector<Jet> vjets, Float_t met);
TLorentzVector getPtllb(Lepton l1, Lepton l2, Float_t met, Float_t met_phi);


/*
//#########################################################################
// Mini Tree ..............................................................

// Tree variables ..................................................
Int_t TChannel = 0;
Float_t TWeight;   // Total nominal weight
Float_t TMll;      // Invariant mass
Float_t TMET;      // MET
Float_t TMET_Phi;  // MET phi
Float_t THT;       // HT
Int_t   TNVetoLeps;
Int_t   TNSelLeps;
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

// For systematics...
Int_t   TNISRJets;
Int_t   TNJetsJESUp;
Int_t   TNJetsJESDown;
Int_t   TNJetsJER;
Int_t   TNBtagsUp;
Int_t   TNBtagsDown;
Int_t   TNBtagsMisTagUp;
Int_t   TNBtagsMisTagDown;
Float_t TJetJESUp_Pt[20];
Float_t TJetJESDown_Pt[20];
Float_t TJetJER_Pt[20];

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
*/

// For SUSY
/*Float_t TGenMET;
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
	*/

void SetLeptonVariables(TTree* fTree);
void SetJetVariables(TTree* fTree);
void SetEventVariables(TTree* fTree);

void GetLeptonVariables(std::vector<Lepton> selLeptons, Int_t nVetoLeptons = 0);
void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
#endif
