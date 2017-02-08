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
  Float_t ht = getHT(vjets);
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
  
  for(Int_t i = 0; i < vjets.size(); i++){
    nomJets += vjets[i].p;
         if(dir < 0) temp.SetPtEtaPhiM(vjets[i].pTJESDown, vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    else if(dir > 0) temp.SetPtEtaPhiM(vjets[i].pTJESUp,   vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    varJets += temp;
  }
  return (vmet+nomJets-varJets).Pt();
}

/*Float_t getJetJERpt(Jet jet){

			if(flag == 3){
				Float_t jerScaleUp   = getJERScaleUp(*it);	  
				Float_t jerScale     = getJERScale(*it);	  
				Float_t factor1 = 0.;
				if (genJet.DeltaR(tmp) < 0.5) factor1 = max(0.0, genJet.Pt() + jerScale*(tmp.Pt() - genJet.Pt()) );
				else                          factor1 = tmp.Pt();
				Float_t sigmaMC  = getErrPt(JetPt.at(*it), Jet_eta[*it]) / JetPt.at(*it);
				Float_t factor   = fRand3->Gaus(1., sqrt(jerScale*jerScale -1.) * sigmaMC );
				JetPt.at(*it) = JetPt.at(*it) * factor;		// smear for flag 3
			}
}*/


// ==================================================================
// ========================== Other ================================
// ==================================================================
Float_t p2(Float_t x){
  return TMath::Sqrt(x*x);
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
