///////////////////////////////////////////////////////////////////////////////////////////
//
//  Lepton Selector: create vectors with selected leptons, veto (or fakeable) leptons...
//  
//  All SFs and variables are within the Lepton definition
//
//  To do: compute gen leptons (include taus) and counters
//         definition of veto leptons
//
///////////////////////////////////////////////////////////////////////////////////////////


#include "LeptonSelector.h"

ClassImp(LeptonSelector);
LeptonSelector::LeptonSelector() : PAFChainItemSelector() {}
void LeptonSelector::Summary(){}

void LeptonSelector::Initialise(){
  // Initialise LeptonSelector
  gIsData        = GetParam<Bool_t>("IsData");
  gSelection     = GetParam<Int_t>("iSelection");
  LepSF = new LeptonSF();

	if(gSelection == iStopSelec || gSelection == iStopPOG || gSelection == iTopSelec){
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonId,   iMedium);
    LepSF->loadHisto(iMuonIP2D, iTight);
    LepSF->loadHisto(iElecId,   iTight);
  }
  else std::cout << ">>>>>>>>>>>> WRONG SELECTION!!!!!!!!" << std::endl;
  selLeptons = std::vector<Lepton>();
}


void LeptonSelector::InsideLoop(){
	// Clear vectors...
	selLeptons.clear();
	looseLeptons.clear();
	genLeptons.clear();
	vetoLeptons.clear(); 
  nLeptonsFromTau = 0;
	// Loop over the leptons and select
	nLep     = Get<Int_t>("nLepGood"); 
	for(Int_t i = 0; i < nLep; i++){
		GetLeptonVariables(i);
		tL = Lepton(tP, charge, type);
		if(isGoodLepton(tL)){
			tL.SetSF(   LepSF->GetLeptonSF(     pt, eta) ); // Set SF and error
			tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta) );
			selLeptons.push_back(tL);
		}
		if(isVetoLepton(tL)){
			tL.SetSF(1); tL.SetSFerr(1); // (no SF for Veto leptons...)
			vetoLeptons.push_back(tL);
		}
    if(isLooseLepton(tL)){
      tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      looseLeptons.push_back(tL); 
    }
	}

	// Loop over the gen leptons and get gen info...
	if(!gIsData){  
		ngenLep         = Get<Int_t>("ngenLep");
		ngenLepFromTau  = Get<Int_t>("ngenLepFromTau");
		for(Int_t i = 0; i < ngenLep; i++){
			GetGenLeptonVariables(i);
			if(gpdgMId == 24 || gpdgMId == 25 || gpdgMId == 25){
        tL = Lepton(tP, charge, type);
        genLeptons.push_back(tL);
			} 
 		}
		for(Int_t i = 0; i < ngenLepFromTau; i++){
			GetGenLepFromTauVariables(i);
			if(gpdgMId == 24 || gpdgMId == 25 || gpdgMId == 25){
				tL = Lepton(tP, charge, type);
        nLeptonsFromTau++;
				genLeptons.push_back(tL);
			} 
		} 
	}
	nSelLeptons = selLeptons.size();
	nVetoLeptons = vetoLeptons.size();;
  nGenLeptons  = genLeptons.size();
  
  // Set params for the next selectors
  SetParam("selLeptons",  selLeptons );
  SetParam("vetoLeptons", vetoLeptons);
  SetParam("looseLeptons", looseLeptons);
  SetParam("genLeptons",  genLeptons );
  SetParam("nLeptonsFromTau", nLeptonsFromTau);
  SetParam("nGenLeptons", nGenLeptons);
  SetParam("nSelLeptons", nSelLeptons);
  SetParam("nVetoLeptons", nVetoLeptons);
}

//################################################################
//## Get important variables
//################################################################
void LeptonSelector::GetLeptonVariables(Int_t i){ // Once per muon, get all the info
	tP.SetPxPyPzE(Get<Float_t>("LepGood_px", i), Get<Float_t>("LepGood_py", i), Get<Float_t>("LepGood_pz", i), Get<Float_t>("LepGood_energy", i));
  pt = tP.Pt();
  eta = tP.Eta();
	charge = Get<Int_t>("LepGood_charge", i); 
	type = TMath::Abs(Get<Int_t>("LepGood_pdgId",i)) == 11 ? 1 : 0;
	tightVar = Get<Int_t>("LepGood_tightId", i); 
	mediumMuonId = Get<Int_t>("LepGood_mediumMuonId",i); 
	etaSC = Get<Float_t>("LepGood_etaSc",i);
	RelIso03 = Get<Float_t>("LepGood_relIso03",i);
	RelIso04 = Get<Float_t>("LepGood_relIso04",i);
	ptRel = Get<Float_t>("LepGood_jetPtRelv2",i);
	ptRatio = Get<Float_t>("LepGood_jetPtRatiov2",i);
	miniIso =Get<Float_t>("LepGood_miniRelIso",i);
  dxy = Get<Float_t>("LepGood_dxy", i);
  dz  = Get<Float_t>("LepGood_dz", i);
  sip = Get<Float_t>("LepGood_sip3d",i);
	SF = 1;
}

void LeptonSelector::GetGenLeptonVariables(Int_t i){
	tP.SetPtEtaPhiM(Get<Float_t>("genLep_pt", i), Get<Float_t>("genLep_eta", i), Get<Float_t>("genLep_phi", i), Get<Float_t>("genLep_mass", i));
	charge = Get<Int_t>("genLep_charge", i);
	gpdgMId = TMath::Abs(Get<Int_t>("genLep_motherId", i));
	type = TMath::Abs(Get<Int_t>("genLep_pdgId",i)) == 11 ? 1 : 0;
}

void LeptonSelector::GetGenLepFromTauVariables(Int_t i){
	tP.SetPtEtaPhiM(Get<Float_t>("genLepFromTau_pt", i), Get<Float_t>("genLepFromTau_eta", i), Get<Float_t>("genLepFromTau_phi", i), Get<Float_t>("genLepFromTau_mass", i));
	charge = Get<Int_t>("genLepFromTau_charge", i);
	gpdgMId = TMath::Abs(Get<Int_t>("genLepFromTau_grandmotherId", i));
	type = TMath::Abs(Get<Int_t>("genLepFromTau_pdgId",i)) == 11 ? 1 : 0;
}

//################################################################
//## Definition of wps...
//################################################################
Bool_t LeptonSelector::getSIPcut(Float_t cut){ 
  if(sip > cut) return false;
  return true; 
}

Bool_t LeptonSelector::getGoodVertex(Int_t wp){ 
  if(wp == iMedium && (dxy > 0.2  || dz > 0.5)) return false;
  if(wp == iTight  && (dxy > 0.05 || dz > 0.1)) return false;
  return true;
}

Bool_t LeptonSelector::getRelIso03POG(Int_t wp){ 
	if(type == 1){ // electrons 
		if(etaSC <= 1.479){
			if(wp == iVeto   && RelIso03 > 0.1750) return false; 
			if(wp == iLoose  && RelIso03 > 0.0994) return false; 
			if(wp == iMedium && RelIso03 > 0.0695) return false; 
			if(wp == iTight  && RelIso03 > 0.0588) return false; 
		}
		else if(etaSC > 1.479){
			if(wp == iVeto   && RelIso03 > 0.1590) return false; 
			if(wp == iLoose  && RelIso03 > 0.1070) return false; 
			if(wp == iMedium && RelIso03 > 0.0821) return false; 
			if(wp == iTight  && RelIso03 > 0.0571) return false; 
		}
	}
	else{ // muons
		if(wp == iLoose  && RelIso03 > 0.10) return false; 
		if(wp == iTight  && RelIso03 > 0.05) return false; 
	}
	return true;
}

Bool_t LeptonSelector::getRelIso04POG(Int_t wp){ // wps for muons  
	if(type == 1) return false; // electrons
	if(wp == iLoose  && RelIso04 > 0.25) return false; 
	if(wp == iTight  && RelIso04 > 0.15) return false; 
	return true;
}

Bool_t LeptonSelector::getMuonId(Int_t wp){ 
	if(wp == iTight   && !tightVar)     return false;
	if(wp == iMedium  && !mediumMuonId) return false;
	return true;
}

Bool_t LeptonSelector::getElecCutBasedId(Int_t wp){ 
	if(wp == iTight   && tightVar < 3)     return false;
	if(wp == iMedium  && tightVar < 2)     return false;
	if(wp == iLoose   && tightVar < 1)     return false;
	if(wp == iVeto    && tightVar < 0)     return false;
	return true;
}

Bool_t LeptonSelector::getMultiIso(Int_t wp){ 
	if(wp == iVeryTight && !(miniIso < 0.09 && (ptRatio > 0.84 || ptRel > 7.2)) )     return false;
	if(wp == iTight   )     return false;
	if(wp == iMedium  )     return false;
	if(wp == iLoose   )     return false;
	if(wp == iVeto    )     return false;
	return true;
}


//################################################################
//## Lepton definitions for each analysis
//################################################################
//
// Use the functions above to define your objects
//
//=============================================================== SELECTED LEPTONS
Bool_t LeptonSelector::isGoodLepton(Lepton lep){ 
	Bool_t passId; Bool_t passIso;
	if(gSelection == iStopPOG){
		// Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4 
		// Medium Muon ID, RelIso POG, tightIP2D, SIP3D > 4  
		if(lep.isMuon){
			passId  = getMuonId(iMedium);
			passIso = getRelIso04POG(iTight);
		}
		if(lep.isElec){
			passId = getElecCutBasedId(iTight);
			passIso = getRelIso03POG(iTight);
		}
		if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
		if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
		else return false;
	}
	if(gSelection == iStopSelec){
		// Tight cut-based electrons, pT > 20, |eta| < 2.4, MultiIsoVT, tightIP2D, SIP3D > 4 
		// Medium Muon ID, MultiIsoVT, tightIP2D, SIP3D > 4
		if(lep.isMuon){
			passId  = getMuonId(iMedium);
			passIso = getMultiIso(iVeryTight);
		}
		if(lep.isElec){
			passId = getElecCutBasedId(iTight);
			passIso = getMultiIso(iVeryTight);
		}
		if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
		if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
		else return false;
	}
	else if(gSelection == iTopSelec){
		// Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4 
		// Tight Muon ID, RelIso POG, tightIP2D, SIP3D > 4 
		if(lep.isMuon){
			passId  = getMuonId(iTight);
			passIso = getRelIso04POG(iTight);
		}
		if(lep.isElec){
			passId = getElecCutBasedId(iTight);
			passIso = getRelIso03POG(iTight);
		}
		if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
		if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
		else return false;
	}
	else if(gSelection == iWWSelec){
		if(lep.isMuon){
			passId  = getMuonId(iTight);
			passIso = getRelIso04POG(iTight);
		}
		if(lep.isElec){
			passId = getElecCutBasedId(iTight);
			passIso = getRelIso03POG(iTight);
		}
		if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
		if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
		else return false;
		return false;
	}
}

//============================================== VETO LEPTONS
Bool_t LeptonSelector::isVetoLepton(Lepton lep){ 
	Bool_t passId;
	if(gSelection == iStopSelec){
		return true;
	}
	else if(gSelection == iTopSelec){
		return true;
	}
	else if(gSelection == iWWSelec){
		return true;
	}
	else if(gSelection == ittHSelec){ //FO
		return true;
	}
	return false;
}

//============================================== Loose leptons (or other)
Bool_t LeptonSelector::isLooseLepton(Lepton lep){
	if(gSelection == ittHSelec){
		return true;
	}
}
