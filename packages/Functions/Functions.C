#include "Functions.h"
#include "mt2.h"

// ==================================================================
// ==================== Kinematic variables ========================
// ==================================================================
Float_t getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep){
  Float_t minDphi = 9999.;
  for (unsigned int i=0; i<vJet.size(); i++) {
    if (minDphi > TMath::Abs(lep.DeltaPhi(vJet[i].p))) minDphi = TMath::Abs(lep.DeltaPhi(vJet[i].p));
  }
  return minDphi;
}

Float_t getDeltaPhillJet(Lepton lep1, Lepton lep2, Jet jet){
  TLorentzVector dilep = lep1.p+lep2.p;
  return TMath::Abs(dilep.DeltaPhi(jet.p));
}

Float_t getHT(vector<Jet> jet){
  Float_t ht(0);
  for (unsigned int i=0; i<jet.size(); i++) ht+=jet[i].p.Pt();
  return ht;
}

Float_t getMT(TLorentzVector v1, TLorentzVector v2){
  Float_t pt1 = v1.Pt();
  Float_t pt2 = v2.Pt();
  Float_t dphi = v1.DeltaPhi(v2);
  return TMath::Sqrt(2*pt1*pt2*(1-TMath::Cos(dphi)));
}

Float_t getMinDPhiMetJets(vector<Jet> vjets, Float_t met, Float_t met_phi){
  Float_t deltaphi = 999;
  Float_t minDphi  = 999;
  TLorentzVector pmet;
  pmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  for(Int_t i = 0; i < vjets.size(); i++){
    deltaphi = pmet.DeltaPhi(vjets[i].p);
    if(minDphi > deltaphi) minDphi = deltaphi;
  }
  return minDphi;
}

Float_t getDelPhi(TLorentzVector v1, TLorentzVector v2){ return v1.DeltaPhi(v2);}
Float_t getDelEta(TLorentzVector v1, TLorentzVector v2){ return TMath::Abs(v1.Eta() - v2.Eta());}

Float_t getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, Float_t mass){
  double pa[3]; double pb[3]; double pmiss[3];
  pmiss[0] = 0.; // irrelevant
  pmiss[1] = pmet.Px(); pmiss[2] = pmet.Py();
  pa[0] = 0.; pa[1] = plep1.Px(); pa[2] = plep1.Py();
  pb[0] = 0.; pb[1] = plep2.Px(); pb[2] = plep2.Py();
  mt2 MT2bisect;
  MT2bisect.set_momenta(pa, pb, pmiss);
  MT2bisect.set_mn(mass); // testmass
  Float_t MT2 = MT2bisect.get_mt2();
  return MT2;
}

Float_t getMT2ll(Lepton l1, Lepton l2, Float_t met, Float_t met_phi){
  TLorentzVector pmet;
  pmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  Float_t MT2 = getMT2(l1.p, l2.p, pmet, 0);
  return MT2;
}

Float_t getMeff(Lepton l1, Lepton l2, vector<Jet> vjets, Float_t met){
  return l1.p.Pt() + l2.p.Pt() + met;
} 

TLorentzVector getPtllb(Lepton l1, Lepton l2, Float_t met, Float_t met_phi){
  TLorentzVector pmet; pmet.SetPtEtaPhiM(met, 0., met_phi, 0.);
  TLorentzVector pl1 = l1.p; TLorentzVector pl2 = l2.p;
  return pl1 + pl2 + pmet;
}


// ==================================================================
// ========================== Systematics ===========================
// ==================================================================
Float_t JEStoMET(vector<Jet> vjets, Float_t met, Float_t met_phi, Int_t dir){
  if(dir == 0){
    std::cout << "[ERROR] Wrong value in JEStoMET" << std::endl;
    return 0;
  }
  TLorentzVector vmet    = TLorentzVector();
  vmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  TLorentzVector nomJets = TLorentzVector(0,0,0,0);
  TLorentzVector varJets = TLorentzVector(0,0,0,0);
  TLorentzVector temp = TLorentzVector(0,0,0,0);

  for(Int_t i = 0; i < (Int_t) vjets.size(); i++){
    nomJets += vjets[i].p;
    if(dir < 0) temp.SetPtEtaPhiM(vjets[i].pTJESDown, vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    else if(dir > 0) temp.SetPtEtaPhiM(vjets[i].pTJESUp,   vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    varJets += temp;
  }
  return (vmet+nomJets-varJets).Pt();
}

Float_t getJERscale(Float_t jet_eta, Int_t dir = 0){
  // SFs for 80DX https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
  Float_t eta = TMath::Abs(jet_eta);
  Float_t SF = 1; Float_t err = 0;
  if     (eta < 0.5){SF = 1.109; err = 0.008; } 
  else if(eta < 0.8){SF = 1.138; err = 0.013; }
  else if(eta < 1.1){SF = 1.114; err = 0.013; }
  else if(eta < 1.3){SF = 1.123; err = 0.024; }
  else if(eta < 1.7){SF = 1.084; err = 0.011; }
  else if(eta < 1.9){SF = 1.082; err = 0.035; }
  else if(eta < 2.1){SF = 1.140; err = 0.047; }
  else if(eta < 2.3){SF = 1.067; err = 0.053; }
  else if(eta < 2.5){SF = 1.177; err = 0.041; }
  else if(eta < 2.8){SF = 1.364; err = 0.039; }
  else if(eta < 3.0){SF = 1.857; err = 0.071; }
  else if(eta < 3.2){SF = 1.328; err = 0.022; }
  else              {SF = 1.160; err = 0.029;  }
  if     (dir == +1) return SF + err;
  else if(dir == -1) return SF - err;
  return SF;
}
Float_t getJERscale(Jet jet, Int_t dir = 0){ return getJERscale(jet.p.Eta(), dir);}

Float_t getJetPtErr(Jet jet){
  Float_t pt = jet.p.Pt(); Float_t eta = TMath::Abs(jet.p.Eta());
  Float_t InvPerr2 = 1;
  Float_t N(0.), S(0.), C(0.), m(0.);
  if     ( eta < 0.5) { N = 3.96859; S = 0.18348; C = 0.; m = 0.62627;  }
  else if( eta < 1. ) { N = 3.55226; S = 0.24026; C = 0.; m = 0.52571;  } 
  else if( eta < 1.5) { N = 4.54826; S = 0.22652; C = 0.; m = 0.58963;  } 
  else if( eta < 2. ) { N = 4.62622; S = 0.23664; C = 0.; m = 0.48738;  } 
  else if( eta < 2.5) { N = 2.53324; S = 0.34306; C = 0.; m = 0.28662;  } 
  else if( eta < 3. ) { N = 3.33814; S = 0.73360; C = 0.; m = 0.08264;  } 
  else if( eta < 5. ) { N = 2.95397; S = 0.11619; C = 0.; m = 0.96086;  }
  // this is the absolute resolution (squared), not sigma(pt)/pt
  // so have to multiply by pt^2, thats why m+1 instead of m-1
  InvPerr2 =  (N*TMath::Abs(N)) + (S*S)*pow(pt, m+1) + (C*C)*pt*pt;
  return sqrt(InvPerr2);
}

Float_t getJetJERpt(Jet jet){
  // Hybrid method https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
  Float_t jerScale(1.), genpt(1.), pt(1.), factor1(0.), factor2(0.), sigmaMC(1.), rdm(1.);
  jerScale     = getJERscale(jet, 0);    
  genpt = jet.mcp.Pt(); pt = jet.p.Pt();

  // Using JER SF 
  factor1  = TMath::Max(Float_t(0.0), genpt + jerScale*(pt - genpt))/pt;
  if(jet.mcp.DeltaR(jet.p) > 0.2) factor1 = -1; // Check matching

  // smearing...
  TRandom3 *fRand3 = new TRandom3(50);
  sigmaMC  = getJetPtErr(jet)/pt;
  rdm  = fRand3->Gaus(0., sigmaMC ); //smear
  //rdm  = fRand3->Gaus(1., TMath::Sqrt(jerScale*jerScale -1.) * sigmaMC ); //smear
  factor2 = 1 + rdm*TMath::Sqrt(TMath::Max(Float_t(0.0), jerScale*jerScale - 1));
  delete fRand3;

  if(factor1 == -1) return pt;//*factor2;
  return  pt*factor1;   
}


// ==================================================================
// ========================== Other ================================
// ==================================================================
Float_t p2(Float_t x){
  return (x*x);
}

bool LepMomentumComparator(Lepton i, Lepton j){ 
  return (i.p.Pt()>j.p.Pt()); 
}

bool JetMomentumComparator(Jet i   , Jet    j){
  return (i.p.Pt()>j.p.Pt()); 
}

vector<Lepton> SortLeptonsByPt(vector<Lepton>& Leptons){
  vector<Lepton> theLep = Leptons;
  sort (theLep.begin(), theLep.end(), LepMomentumComparator);
  return theLep;
}

vector<Jet> SortJetsByPt(vector<Jet>& Jets){
  vector<Jet> theJet = Jets;
  sort (theJet.begin(), theJet.end(), JetMomentumComparator);
  return theJet;
}
