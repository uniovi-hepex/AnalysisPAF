#include "Functions.h"

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

/*
float getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep){
	float minDphi = 9999.;
	for (unsigned int i=0; i<vJet.size(); i++) {
		if (minDphi > TMath::Abs(lep.DeltaPhi(Jet[i].p))) minDphi = TMath::Abs(lep.DeltaPhi(Jet[i].p));
	}
	return minDphi;
}

float getDeltaPhillJet(){
	if (fHypLepton1.index == -1) return -999.;
	if (fHypLepton2.index == -1) return -999.;
	Int_t ij = getLeadingJetbTag();
	if (ij < 0) return -999.; 
	TLorentzVector dilep = fHypLepton1.p+fHypLepton2.p;
	TLorentzVector jet = Jet[ij].p; 
	return TMath::Abs(dilep.DeltaPhi(jet));
}


vector<Jet> getSelectedJets(Float_t ptcut, Float_t etacut, TString idname, TString btagwp, Int_t nJet, Int_t *id, Float_t *Jet_px, Float_t *Jet_px, Float_t *Jet_px, Float_t *Jet_E, Float_t *Jet_CSV, Int_t *Jet_mcFlavour, Int_t sys = 0){
  vector<Jet> vJets;
  Jet j();
  TLorentzVector t = TLorentzVector(0,0,0,0);
	for(int i = 0; i < nJet; i++){
    t = TLorentzVector(Jet_px[i], Jet_py[i], Jet_pz[i], Jet_E[i]);
    j = Jet(t, Jet_CSV[i], i, id[i], Jet_mcFlavour[i], ptcut, etacut);
  }
  TLorentzVector jt;
  for (Int_t i=0; i<nJet; i++) {
    if(!IsGoodJet(i,gJetEtCut)) continue;

    Float_t jetbtagi      = Jet_btagCSV[i];
    Float_t jetetai       = Jet_eta[i];
    Float_t jetenergyi    = Jet_energy[i];
    
    jt.SetPtEtaPhiE(JetPt.at(i), jetetai, JetPhi.at(i), jetenergyi);
    bool isbtag = false;
    if (gIsData) {
      isbtag = fBTagSFnom->IsTagged(Jet_btagCSV[i], -999999, JetPt.at(i), jetetai);
    }
    else {
			Int_t   jetmcflavouri = Get<Int_t>  ("Jet_mcFlavour", i);
			// official b-tag recommendation: use JetHadronFlavour instead of JetPartonFlavor
    if      (gSysSource == BtagUp)      isbtag = fBTagSFbUp->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai); 
    else if (gSysSource == BtagDown)    isbtag = fBTagSFbDo->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    else if (gSysSource == MisTagUp)    isbtag = fBTagSFlUp->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    else if (gSysSource == MisTagDown)  isbtag = fBTagSFlDo->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    else                                isbtag = fBTagSFnom->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    }
    jet tmpjet(jt, isbtag, i);
    Jet.push_back(tmpjet);
    nj++;
  }
  return nj;
}*/

//vector<Lepton> getSelectedLeptons(Float_t ptcut, Float_t etacut, Float_t *px, Float_t *py, Float_t *pz, Float_t *E, Int_t *ch, Int_t *pdgIf, Float_t dxy, Float_t dz, Float_t SIP, Float_t etaSC, Int_t , Float_t dx, Float_t dx, Float_t dx, 

//void SelectedGenLepton() {

//void propagateMET(TLorentzVector nVec, TLorentzVector oVec){

// SmearJetPts(int flag){
/*	if(gIsData)   return; 
	if(flag == 0) return; 
	if (!gIsData) {
		std::vector<int> cleanJets = CleanedJetIndices(15.);
		TLorentzVector ojets, jets, tmp, genJet;         // 4-vec of old jets, newjets and a tmp-vector

		std::vector<int>::const_iterator it = cleanJets.begin();

		for( it = cleanJets.begin(); it != cleanJets.end(); ++it) {
			tmp.SetPtEtaPhiE(JetPt.at(*it), Jet_eta[*it], JetPhi.at(*it), Jet_energy[*it]);         // set temp to the jet
			// Gen info for jets... 
			genJet.SetPxPyPzE(Get<Float_t>("Jet_mcPx",*it), 
					Get<Float_t>("Jet_mcPy",*it), 
					Get<Float_t>("Jet_mcPz",*it),  
					Get<Float_t>("Jet_mcEnergy",*it));        
			ojets += tmp; 
			if(flag == 1) JetPt.at(*it) = Get<Float_t>("Jet_rawPt", *it)*Get<Float_t>("Jet_corr_JECUp",*it);   // vary up   for flag 1 
			if(flag == 2) JetPt.at(*it) = Get<Float_t>("Jet_rawPt", *it)*Get<Float_t>("Jet_corr_JECDown",*it); // vary down for flag 2;
			if(flag == 3){
				float jerScaleUp   = getJERScaleUp(*it);	  
				float jerScale     = getJERScale(*it);	  
				float factor1 = 0.;
				if (genJet.DeltaR(tmp) < 0.5) factor1 = max(0.0, genJet.Pt() + jerScale*(tmp.Pt() - genJet.Pt()) );
				else                          factor1 = tmp.Pt();
				float sigmaMC  = getErrPt(JetPt.at(*it), Jet_eta[*it]) / JetPt.at(*it);
				float factor   = fRand3->Gaus(1., sqrt(jerScale*jerScale -1.) * sigmaMC );
				JetPt.at(*it) = JetPt.at(*it) * factor;		// smear for flag 3
			}
			// set tmp to the scaled/smeared jet
			tmp.SetPtEtaPhiE(JetPt.at(*it), Jet_eta[*it], JetPhi.at(*it), Jet_energy[*it]);
			jets += tmp;  // add scaled/smeared jet to the new jets
		}
		propagateMET(jets, ojets);  // propagate this change to the MET
	}
}*/

// ScaleLeptons(int flag){
// ScaleMET(int flag){ }

