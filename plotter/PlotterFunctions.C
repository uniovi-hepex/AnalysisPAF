///////////////////////////////////////////////////////////////////////
//
// Functions to use with DrawStack or DrawComp
// Include here all functions that can be used by anyone
// Please, include a brief description of the function
//
///////////////////////////////////////////////////////////////////////

//#################### Dilepton Variables
// All these functions have as arguments: 
//   Using arrays ---> (*pt, *eta, *phi, *m)
//   Or           ---> (pt1, eta1, phi1, m1, pt2, eta2, phi2, m2)

#include "TLorentzVector.h"
#include "t4/t4Functions.C"
#include "stop/StopFunctions.C"

#ifndef PlotterFunctions_C
#define PlotterFunctions_C 1

Float_t GetEntry(Float_t *pt, Int_t pos);

Float_t InvMass(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DeltaEta(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DeltaPhi(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DilepPt(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DeltaEta(Float_t e1, Float_t e2);

Float_t InvMass(Float_t pt1, Float_t eta1, Float_t phi1, Float_t e1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t e2);
Float_t DeltaEta(Float_t pt1, Float_t eta1, Float_t phi1, Float_t e1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t e2);
Float_t DeltaPhi(Float_t pt1, Float_t eta1, Float_t phi1, Float_t e1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t e2);
Float_t DilepPt(Float_t pt1, Float_t eta1, Float_t phi1, Float_t e1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t e2);
Float_t GetDilepPtDPhi(Float_t pt1, Float_t pt2, Float_t dphi);


//#################### Jet variables

Float_t HT(Float_t *jet_pt, Int_t nJets);
Int_t NBtagNJets(Int_t nJets, Int_t nBtags);


//#################### Angular distributions
// Between leptons, jets and MET...


//#################### EXTRA
Float_t GetPt(Float_t lep0pt, Float_t lep1pt, Int_t lep0id, Int_t lep1id, Int_t id);
Bool_t PassEtaCut(Float_t lep0eta, Float_t lep1eta, Int_t lep0id, Int_t lep1id, Float_t etamin, Float_t etamax, Int_t id);

////////////////////////////////////////////////////////////
// Funtion definitions below here
////////////////////////////////////////////////////////////
Float_t DilepPt(Float_t pt1, Float_t eta1, Float_t phi1, Float_t e1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t e2){
  TLorentzVector l1; l1.SetPtEtaPhiM(pt1, eta1, phi1, e1);
  TLorentzVector l2; l2.SetPtEtaPhiM(pt2, eta2, phi2, e2);
  return (l1+l2).Pt();
}

Float_t GetDilepPtDPhi(Float_t pt1, Float_t pt2, Float_t dphi){
  dphi = TMath::Abs(dphi);
  Float_t p2 = pt1*pt1 + pt2*pt2 + 2*TMath::Cos(dphi)*pt1*pt2;
  return TMath::Sqrt(p2);
}

Float_t GetEntry(Float_t *pt, Int_t pos){
  return pt[pos];
}

Int_t NBtagNJets(Int_t nJets, Int_t nBtags){
  if(     nJets == 0) return 0;
  else if(nJets == 1) return 1+nBtags;
  else if(nJets == 2) return 3+nBtags;
  else if(nJets == 3) return 6+nBtags;
  else if(nJets == 4) return 10+nBtags;
  else return 15;
}

Float_t GetPt(Float_t lep0pt, Float_t lep1pt, Int_t lep0id, Int_t lep1id, Int_t id){
  id = abs(id);
  if     (id == 11){
    if     (abs(lep0id) == 11) return lep0pt;
    else if(abs(lep1id) == 11) return lep1pt;
  }
  else if(id == 13){
    if     (abs(lep0id) == 13) return lep0pt;
    else if(abs(lep1id) == 13) return lep1pt;
  }
  return 0;
}

Bool_t PassEtaCut(Float_t lep0eta, Float_t lep1eta, Int_t lep0id, Int_t lep1id, Float_t etamin, Float_t etamax, Int_t id){
  id = abs(id); Float_t eta = 0;
  if     (id == 11){
    if     (abs(lep0id) == 11) eta = lep0eta;
    else if(abs(lep1id) == 11) eta = lep1eta;
  }
  else if(id == 13){
    if     (abs(lep0id) == 13) eta = lep0eta;
    else if(abs(lep1id) == 13) eta = lep1eta;
  }
  else{
    return false;
  }
  eta = fabs(eta); etamax = fabs(etamax); etamin = fabs(etamin);
  return (eta < etamax && eta > etamin);
}

Float_t InvMass(Float_t pt1, Float_t eta1, Float_t phi1, Float_t m1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t m2){
  TLorentzVector l1; l1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
  TLorentzVector l2; l2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
  return (l1 + l2).M();
}

Float_t DeltaEta(Float_t pt1, Float_t eta1, Float_t phi1, Float_t m1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t m2){
  TLorentzVector l1; l1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
  TLorentzVector l2; l2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
  return TMath::Abs(l1.Eta() - l2.Eta());
}

Float_t DeltaEta(Float_t e1, Float_t e2){ return TMath::Abs(e1 - e2);}

Float_t DeltaPhi(Float_t pt1, Float_t eta1, Float_t phi1, Float_t m1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t m2){
  TLorentzVector l1; l1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
  TLorentzVector l2; l2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
  return l1.DeltaPhi(l2);
}

Float_t DeltaR(Float_t pt1, Float_t eta1, Float_t phi1, Float_t m1, Float_t pt2, Float_t eta2, Float_t phi2, Float_t m2){
  TLorentzVector l1; l1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
  TLorentzVector l2; l2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
  return l1.DeltaR(l2);
}

// TopPtReweighting
Float_t NormTopPtSF = 1.00298; //  7.72293e+07/7.69995e+07
Float_t GetTopPtWeight(Float_t Pt1, Float_t Pt2){
  Float_t a = 0.0615; Float_t b = 0.0005;
  Float_t SF1; Float_t SF2;
  SF1 = TMath::Exp(a - Pt1*b);
  SF2 = TMath::Exp(a - Pt2*b);
  return TMath::Sqrt(SF1*SF2)*1.00298;
}

Float_t GetFSRsf(Float_t n, Float_t v){
  Float_t d = TMath::Abs(n-v); 
  Float_t x = 1/TMath::Sqrt(2);
  if(n > v) return (n+d*x)/v;
  else      return (n-d*x)/v;
}

Float_t GetModMET(Float_t met, Float_t met_phi, Float_t lepPt, Float_t lepEta, Float_t lepPhi, Float_t lepM){
  TLorentzVector pMET; TLorentzVector pLep; TLorentzVector p;
  pMET.SetPtEtaPhiM(met, 0, met_phi, 0);
  pLep.SetPtEtaPhiM(lepPt, lepEta, lepPhi, lepM);
  p = pMET + pLep;
  return p.Pt();
}

Float_t GetMetResolutionSF(Float_t met){
  Float_t fNorm = 0.92631;
  if(met < 0.000000) met = 3.750000;
  if(met > 150.000000) met = 149;
  if     (met < 7.500000) return 0.940594;
  else if(met < 15.000000) return 0.957707;
  else if(met < 22.500000) return 0.971954;
  else if(met < 30.000000) return 0.989865;
  else if(met < 37.500000) return 0.997489;
  else if(met < 45.000000) return 0.993762;
  else if(met < 52.500000) return 0.984065;
  else if(met < 60.000000) return 0.963710;
  else if(met < 67.500000) return 0.946336;
  else if(met < 75.000000) return 0.919873;
  else if(met < 82.500000) return 0.893570;
  else if(met < 90.000000) return 0.873634;
  else if(met < 97.500000) return 0.860273;
  else if(met < 105.000000) return 0.858863;
  else if(met < 112.500000) return 0.846743;
  else if(met < 120.000000) return 0.849624;
  else if(met < 127.500000) return 0.857365;
  else if(met < 135.000000) return 0.880115;
  else if(met < 142.500000) return 0.887628;
  else if(met < 150.000000) return 0.901220;
  else return 0.;
}

Float_t GetMetResNorm(Float_t met){
   Float_t fNorm = 0.92631;
   return GetMetResolutionSF(met)/fNorm;
}

Float_t GetFactorFSR(Float_t nom, Float_t var){
  Float_t f = TMath::Sqrt(2);
  Float_t x = (nom - f*(nom-var))/var;
  return 1/x;
}

Float_t GetMetUnclWeightUp(Float_t met){
  if(met < 20.000000) return 0.987000;
  else if(met < 25.000000) return 0.986885;
  else if(met < 30.000000) return 0.986851;
  else if(met < 35.000000) return 0.986817;
  else if(met < 40.000000) return 0.987062;
  else if(met < 45.000000) return 0.987308;
  else if(met < 50.000000) return 0.991306;
  else if(met < 55.000000) return 0.995303;
  else if(met < 60.000000) return 0.994664;
  else if(met < 65.000000) return 0.994025;
  else if(met < 70.000000) return 0.995359;
  else if(met < 75.000000) return 0.996694;
  else if(met < 80.000000) return 0.999391;
  else if(met < 85.000000) return 1.002088;
  else if(met < 90.000000) return 1.003458;
  else if(met < 95.000000) return 1.004829;
  else if(met < 100.000000) return 1.012460;
  else if(met < 200.000000) return 1.020092;
  else if(met > 200) return 1.020092;
  return 0.;
}

Float_t GetMetUnclWeightDown(Float_t met){
  if(met < 20.000000) return 1.0067;
  else if(met < 25.000000) return 1.006760;
  else if(met < 30.000000) return 1.006936;
  else if(met < 35.000000) return 1.007112;
  else if(met < 40.000000) return 1.005701;
  else if(met < 45.000000) return 1.004291;
  else if(met < 50.000000) return 1.003551;
  else if(met < 55.000000) return 1.002811;
  else if(met < 60.000000) return 1.002362;
  else if(met < 65.000000) return 1.001913;
  else if(met < 70.000000) return 1.002917;
  else if(met < 75.000000) return 1.003921;
  else if(met < 80.000000) return 0.999800;
  else if(met < 85.000000) return 0.995679;
  else if(met < 90.000000) return 0.996548;
  else if(met < 95.000000) return 0.997417;
  else if(met < 100.000000) return 0.994609;
  else if(met < 200.000000) return 0.991801;
  else if(met > 200) return 0.9918;
  return 0.;
}

float GetMT2orDeltaEta(float mt2, float deta){
  if(mt2 == 0) return 200+deta;
  else if(mt2 >= 200) return 199;
  return mt2;
}

float GetMT2orMETorDeltaEta(float mt2, float met, float deta){
  if      (met >  200) return 208;
  else if (met >  150) return 204;
  else if (mt2 ==   0) return 200+deta;
  else if (mt2 >= 200) return 199;
  return mt2;
}

int GetMT2orDeltaEtaBins(float mt2, float met, float deta){
  deta = fabs(deta);
  if     (met >= 150 && met < 200) return 1;
  else if(met >= 200             ) return 2;
  else{
    if(mt2 == 0){
      if     (deta <= 0.2) return 3;
      else if(deta <= 0.4) return 4;
      else if(deta <= 0.6) return 5;
      else if(deta <= 0.8) return 6;
      else if(deta <= 1.2) return 7;
      else if(deta <= 1.6) return 8; 
      else if(deta >  1.6) return 9;
    }
    else{
      if(mt2 <  20) return 10;
      if(mt2 <  40) return 11;
      if(mt2 <  60) return 12;
      if(mt2 <  80) return 13;
      if(mt2 <  90) return 14;
      if(mt2 < 100) return 15;
      if(mt2 > 100) return 16;
    }
  }
  return 0;
}

int GetMT2orDiPtorDeltaEtaBins(float mt2, float lep0pt, float lep1pt, float dphi, float deta){
  deta = fabs(deta);
  float dileppt = GetDilepPtDPhi(lep0pt, lep1pt, dphi);
  if(mt2 == 0){
    if(dileppt < 30) return 1;
    else{
      if     (deta <= 0.2) return 2;
      else if(deta <= 0.4) return 3;
      else if(deta <= 0.6) return 4;
      else if(deta <= 0.8) return 5;
      else if(deta <= 1.2) return 6;
      else if(deta <= 1.6) return 7; 
      else if(deta >= 1.6) return 8;
    }
  }
  else{
    if(mt2 <  20) return  9;
    if(mt2 <  40) return 10;
    if(mt2 <  60) return 11;
    if(mt2 <  80) return 12;
    if(mt2 < 100) return 13;
    if(mt2 > 100) return 14;
  }
  return 0;
}

#endif
