#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1

#include "Lepton.h"
#include "Jet.h"
#include "TTree.h"
#include "TRandom3.h"
#include <iostream>
#include <vector>

using namespace std;

// enum 
enum iSFs{
  iMuonReco, iMuonId, iMuonIdSUSY, iMuonIsoTightId, iMuonIsoMediumId, iMuonIsoSUSY, iMuonIP2D, iMuonSIP3D, iMuonIdFastSim, iMuonIsoFastSim,
  iElecReco, iElecId, iElecIdSUSY, iElecIso, iElecIsoSUSY, iElecIP2D, iElecSIP3D, iElecFastSim,
  iTrigDoubleMuon, iTrigDoubleElec, iTrigElMu,
  nfiles
};

enum wps{iVeto, iVeryLoose, iLoose, iMedium, iTight, iVeryTight, iExtreamlyTight, iWPforStop, iLooseWPforStop};

enum sel{iStopSelec, iTopSelec, iTWSelec, iWWSelec, ittDMSelec, ittHSelec, iWZSelec, i4tSelec};

enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, nTotalDefinedChannels};

enum sys{iNom, 
  iJesUp, iJesDown, iJERUp, iJERDown, 
  iBtagUp, iBtagDown, iBtagMisUp, iBtagMisDown, 
  iLepEffUp, iLepEffDown, 
  iTrigUp, iTrigDown, 
  iPUUp, iPUDown, 
  iFSUp, iFSDown,
  nSys
};

const Int_t nLHEWeight = 254;
//enum eChannel{iElMu, iElec, iMuon, nChannels};


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
Float_t getISRJetsWeight(Int_t nISRJet);
Bool_t IsOnZ(vector<Lepton> leptons);
Bool_t PassLowInvMass(vector<Lepton> leptons, Float_t Mll_max = 12);
Bool_t hasOSSF(vector<Lepton> leptons);
Float_t ClosestMlltoZ(vector<Lepton> leptons);

#endif
