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

#endif
