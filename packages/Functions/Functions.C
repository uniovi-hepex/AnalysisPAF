#include "Functions.h"
#include "mt2.h"

// ==================================================================
// ==================== Kinematic variables ========================
// ==================================================================
Float_t getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep){
  Float_t minDphi = 9999.;
  for (UInt_t i=0; i<vJet.size(); i++) {
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
  for (UInt_t i=0; i < jet.size(); i++) ht += jet.at(i).p.Pt();
  return ht;
}

Float_t getMHT(vector<Lepton> lepton, vector<Jet> jet) { // (~MET but with selected objects)
  Float_t mht = 0;
  TLorentzVector vectemp(0.,0.,0.,0.);
  for (UInt_t i = 0; i < jet.size(); i++)    vectemp += jet.at(i).p;
  for (UInt_t i = 0; i < lepton.size(); i++) vectemp += lepton.at(i).p;
  mht = vectemp.Pt();
  
  return mht;
}

Float_t getMETLD(Float_t MET, Float_t MHT) {
  Float_t metld;
  metld = MET * 0.00397 + MHT * 0.00265;
  
  return metld;
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

Float_t getISRJetsWeight(Int_t nISRJet){
  // https://indico.cern.ch/event/592621/contributions/2398559/attachments/1383909/2105089/16-12-05_ana_manuelf_isr.pdf --> Slide 8
  Float_t SF = 0;
  if     (nISRJet == 0) SF = 1;
  else if(nISRJet == 1) SF = 0.920;
  else if(nISRJet == 2) SF = 0.821;
  else if(nISRJet == 3) SF = 0.715;
  else if(nISRJet == 4) SF = 0.652;
  else if(nISRJet == 5) SF = 0.561;
  else                  SF = 0.511;
  return SF;
}

Jet GetMatchedJet(Jet origJet, std::vector<Jet> jetCollection, Float_t etaRange, Float_t ptRange){
  Float_t mieta = origJet.p.Eta();
  Float_t mipt  = origJet.p.Pt();
  Int_t nJet = jetCollection.size();
  Float_t eta = 0; Float_t pt = 0;
  for(Int_t i = 0; i < nJet; i++){
    eta = jetCollection.at(i).Eta();
    pt  = jetCollection.at(i).Pt();
    if(TMath::Abs(eta - mieta) > etaRange && TMath::Abs(pt - mipt) > ptRange) return jetCollection.at(i);
  }
  return Jet(TLorentzVector(0,0,0,0), 0, 0, 0);
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

// Playing with lepton masses...

Float_t ClosestMlltoZ(vector<Lepton> leptons){
  Int_t nLeps = leptons.size(); Float_t mll = 0; Float_t best_mll = 0;
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          mll = (leptons.at(i).p + leptons.at(j).p).M(); if(best_mll = 0) best_mll = mll; 
          if( TMath::Abs(mll - Zm) < TMath::Abs(best_mll - Zm) ) best_mll = mll;
        }
      }
    }
  } 
  return best_mll;
}

Bool_t hasOSSF(vector<Lepton> leptons){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          return true;
        }
      }
    }
  } 
  return false;
}

Bool_t has2OSSFwMlmm(vector<Lepton> lepton, Float_t mm) {
  if (lepton.size() < 4) return false;
  vector<TLorentzVector> OSSFpair;
  vector<UInt_t> OSSFpair1;
  vector<UInt_t> OSSFpair2;
  OSSFpair	  = vector<TLorentzVector>();
  OSSFpair1	  = vector<UInt_t>();
  OSSFpair2	  = vector<UInt_t>();
  
  for (UInt_t i = 0; i < lepton.size(); i++) {
    for (UInt_t j = i+1; j < lepton.size(); j++) {
      if (lepton.at(i).type != lepton.at(j).type)      continue;
      if (lepton.at(i).charge*lepton.at(j).charge > 0) continue;
      OSSFpair.push_back(lepton.at(i).p+lepton.at(j).p);
      OSSFpair1.push_back(i);
      OSSFpair2.push_back(j);
    }
  }
  
  if (OSSFpair.size() < 2) return false;
  for (UInt_t i = 0; i < OSSFpair.size(); i++) {
    for (UInt_t j = i+1; j < OSSFpair.size(); j++) {
      if ((OSSFpair1.at(i) == OSSFpair1.at(j)) || 
          (OSSFpair1.at(i) == OSSFpair2.at(j)) || 
          (OSSFpair2.at(i) == OSSFpair2.at(j)))         continue;
      if ((OSSFpair.at(i) + OSSFpair.at(j)).M() < mm)   return true;
    }
  }
  
  return false;
}


Bool_t IsOnZ(vector<Lepton> leptons){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          if( TMath::Abs( (leptons.at(i).p + leptons.at(j).p).M() - Zm) < 15){ // on Z
            return true;
          }
        }
      }
    }
  } 
  return false;
}

Bool_t PassLowInvMass(vector<Lepton> leptons, Float_t Mll_max){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 0){ // opposite sign
          if( (leptons.at(i).p + leptons.at(j).p).M()  < Mll_max) return false;
        }
      }
    }
  } 
  return true;
}

Bool_t PassesLowMassLimit(vector<Lepton> lepton, Float_t mm_max) {
  if(lepton.size() < 2) return false;
  for(UInt_t i = 0; i < lepton.size(); i++) {
    for(UInt_t j = i+1; j < lepton.size(); j++) {
      if((lepton.at(i).p + lepton.at(j).p).M() < mm_max) return false;
    }
  }
  return true;
}

Bool_t IsThereSSpair(vector<Lepton> leptons){
//  return true;
  Int_t nLeps = leptons.size();
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
        if(leptons.at(i).charge*leptons.at(j).charge > 0) return true;
    }
  }
  return false;
}

Int_t getCS(vector<Lepton> lepton) { // Get the sum of charges of a vector of Lepton objects.
  Int_t cs = 0;
  for (UInt_t i = 0; i < lepton.size(); i++) cs += lepton.at(i).charge;
  
  return cs;
}

void co(TString out, TString co = "1;30"){
  std::cout << "\033[" << co << "m" << out << "\033[0m" << endl;
}
