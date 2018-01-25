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
  iMuonReco, iMuonId, iMuonIdSUSY, iMuonIsoTightId, iMuonIsoMediumId, iMuonIsoSUSY, iMuonIP2D, iMuonSIP3D,
  iMuonFastSim, iMuonIdFastSimStop, iMuonIsoFastSimStop,
  iElecReco, iElecId, iElecIdSUSY, iElecIso, iElecIsoSUSY, iElecIP2D, iElecSIP3D, 
  iElecFastSim, iElecIdFastSimStop, iElecIsoFastSimStop,
  iTrigDoubleMuon, iTrigDoubleElec, iTrigElMu,
  iMuonlepMVA2lSSttH, iMuonlepMVA3l4lttH, 
  iMuonLooseTracksttH, iMuonLooseMiniIsottH, iMuonTightIP2DttH,
  iEleclepMVA2lSSttH, iEleclepMVA3l4lttH, 
  iElecTightIP2DM17ttH, iElecMini4M17ttH, iElecConvVetoM17ttH,
  nfiles
};

const TString SFTString[nfiles] = {
  "iMuonReco", "iMuonId", "iMuonIdSUSY", "iMuonIsoTightId", "iMuonIsoMediumId", "iMuonIsoSUSY", "iMuonIP2D", "iMuonSIP3D", 
  "iMuonFastSimStop", "iMuonIdFastSimStop", "iMuonIsoFastSimStop",
  "iElecReco", "iElecId", "iElecIdSUSY", "iElecIso", "iElecIsoSUSY", "iElecIP2D", "iElecSIP3D", 
  "iElecFastSim", "iElecIdFastSimStop", "iElecIsoFastSimStop",
  "iTrigDoubleMuon", "iTrigDoubleElec", "iTrigElMu",
  "iMuonlepMVA2lSSttH", "iMuonlepMVA3l4lttH", 
  "iMuonLooseTracksttH", "iMuonLooseMiniIsottH", "iMuonTightIP2DttH",
  "iEleclepMVA2lSSttH", "iEleclepMVA3l4lttH", 
  "iElecTightIP2DM17ttH", "iElecMini4M17ttH", "iElecConvVetoM17ttH"
};


enum wps{iVeto, iVeryLoose, iLoose, iMedium, iTight, iVeryTight, iExtreamlyTight, iWPforStop, iLooseWPforStop};

enum sel{iStopSelec, iTopSelec, iTWSelec, iWWSelec, iHWWSelec, ittDMSelec, ittHSelec, iWZSelec, i4tSelec, iStopTopSelec, iNSelecs};
const TString LabSelection[] = {"stop", "top", "tW", "WW", "tt+DM", "ttH", "WZ", "tttt"};

enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, i2LOS, TotalDefinedChannels};

enum sys{iNom, 
  iJesUp, iJesDown, iJERUp, iJERDown, 
  iBtagUp, iBtagDown, iBtagMisUp, iBtagMisDown, 
  iLepEffUp, iLepEffDown, 
  iTrigUp, iTrigDown, 
  iPUUp, iPUDown, 
  iFSUp, iFSDown,
  nSys
};

const Int_t   nLHEWeight  = 254;
const Float_t Zm          = 91.1876;
//enum eChannel{iElMu, iElec, iMuon, nChannels};


bool LepMomentumComparator(Lepton i, Lepton j);
bool JetMomentumComparator(Jet i   , Jet    j);

vector<Lepton> SortLeptonsByPt(vector<Lepton>& Leptons);
vector<Jet>       SortJetsByPt(vector<Jet>   & Jets);

Bool_t Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR);
Bool_t Cleaning(TLorentzVector p, vector<TLorentzVector> particles, Float_t minDR);
Float_t JEStoMET(vector<Jet> vjets, Float_t met, Float_t met_phi, Int_t dir = 0);
Float_t JERtoMET(vector<Jet> vjets, Float_t met, Float_t met_phi);
Float_t LepScaleToMET(vector<Lepton> vlep, Float_t met, Float_t met_phi, Int_t id, Int_t dir);
Float_t getJetJERpt(Jet jet, Float_t rho);
Float_t p2(Float_t x);
Jet GetMatchedJet(Jet origJet, std::vector<Jet> jetCollection, Float_t etaRange = 0.3, Float_t ptRange = 10000);

Float_t getDeltaPhillJet(Lepton lep1, Lepton lep2, Jet jet);
Float_t getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep);
Float_t getHT(vector<Jet> jet);
Float_t getMHT(vector<Lepton> lepton, vector<Jet> jet);
Float_t getMETLD(Float_t MET, Float_t MHT);
Float_t getMT(TLorentzVector v1, TLorentzVector v2);
Float_t getMinDPhiMetJets(vector<Jet> vjets, Float_t met, Float_t met_phi);
Float_t getDelPhi(TLorentzVector v1, TLorentzVector v2);
Float_t getDelEta(TLorentzVector v1, TLorentzVector v2);
Float_t getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, Float_t mass);
Float_t getMT2ll(Lepton l1, Lepton l2, Float_t met, Float_t met_phi);
Float_t getMT2llLepScale(Lepton l1, Lepton l2, Float_t met, Float_t met_phi, int id, int dir);
Float_t getMeff(Lepton l1, Lepton l2, vector<Jet> vjets, Float_t met);
TLorentzVector getPtllb(Lepton l1, Lepton l2, Float_t met, Float_t met_phi);
Float_t getISRJetsWeight(Int_t nISRJet);
Bool_t IsOnZ(vector<Lepton> leptons);
Bool_t PassLowInvMass(vector<Lepton> leptons, Float_t Mll_max = 12);
Bool_t  PassesLowMassLimit(vector<Lepton> lepton, Float_t mm_max);
Bool_t hasOSSF(vector<Lepton> leptons);
Bool_t  has2OSSFwMlmm(vector<Lepton> lepton, Float_t mm);
Float_t ClosestMlltoZ(vector<Lepton> leptons);
Bool_t IsThereSSpair(vector<Lepton> leptons);
Bool_t IsThere3SS(vector<Lepton> lepton);
Bool_t ByPt(Jet, Jet);
Int_t GetDileptonicChannel(vector<Lepton> leptons);
vector<Lepton> AssignWZLeptons(vector<Lepton> leptonList);

Float_t GetFSR_JECSF_Up(Float_t pt);
Float_t GetFSR_JECSF_Down(Float_t pt);
Float_t GetTopPtWeight(Float_t Pt1, Float_t Pt2);
Float_t GetMetResolutionSF(Float_t met);
Float_t GetMetUnclWeightDown(Float_t met);
Float_t GetMetUnclWeightUp(Float_t met);
Float_t GetMuonEnergyScale();
Float_t GetMuonEnergySigma(Float_t eta);

Int_t   getCS(vector<Lepton> lepton);
void co(TString out, TString co);
void DumpVar(Int_t evt, TString varname, Float_t val, Bool_t pass = false, Bool_t verbose = false);
void DumpEvent(Int_t evt, TString s, Bool_t verbose = false);

//const Int_t nExploredEvents = 140;
//const Int_t ExploredEvents[nExploredEvents] = {1004830, 1027781, 1082281, 1122356, 1126764, 1129038, 1165159, 1278903, 1287498, 1302344, 1311746, 1342800, 13792, 1410767, 1420060, 1481974, 1530609, 1567745, 1569491, 157688, 1585171, 1602182, 1625235, 1637719, 1661741, 1691691, 1695279, 1706881, 1715192, 1722790, 1766127, 1816696, 1825084, 186035, 1883562, 1901547, 191569, 1919539, 1923141, 1933527, 1936852, 1943927, 1970319, 2063155, 2143242, 2143251, 2153584, 2160602, 2220866, 22382, 2262834, 2304890, 2315491, 231845, 2320568, 2368132, 2402573, 248883, 250897, 2529060, 2611239, 26202, 2650742, 2690212, 2696839, 2740198, 2808071, 2812534, 2868132, 2878073, 2975910, 298133, 2987804, 2993531, 3003409, 303164, 3080220, 3089130, 3114212, 3141384, 3142171, 317267, 3238101, 3249807, 3276724, 3293589, 3326851, 33515, 3380057, 3397894, 3410012, 3421593, 3422422, 342739, 3437639, 3459373, 3460867, 3488198, 352767, 3528344, 3545622, 3551792, 3561157, 3620059, 367642, 368995, 402510, 404254, 411625, 421731, 42924, 432544, 440083, 469319, 499765, 502665, 510694, 516036, 51857, 534972, 551479, 555177, 606403, 612619, 635318, 639001, 662479, 688695, 76172, 769964, 77804, 809962, 841257, 849025, 880010, 93217, 943912, 952900, 968189, 969379};
const Int_t nExploredEvents = 0;
const Int_t ExploredEvents[nExploredEvents] = {};

float GetMuonPtScale();
float GetMuonPtRes(float eta);
float GetElecPtScaleRes();

#endif
