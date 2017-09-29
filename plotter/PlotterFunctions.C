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

#include "t4/t3Functions.C"

Float_t GetEntry(Float_t *pt, Int_t pos);

Float_t InvMass(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DeltaEta(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DeltaPhi(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);
Float_t DilepPt(Float_t *pt, Float_t *eta, Float_t *phi, Float_t *e);

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
  TLorentzVector l1; l1.SetPtEtaPhiE(pt1, eta1, phi1, e1);
  TLorentzVector l2; l2.SetPtEtaPhiE(pt2, eta2, phi2, e2);
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


