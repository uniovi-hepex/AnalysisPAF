//////////////////////////////////////////////////////////////////////////
//
//  Jet Selector
//  Produce vectors with: genJets, selJets, vetoJets...
//  also some variables like number of b-jets
//
//  All SFs and variables are within the Jet definition
//
/////////////////////////////////////////////////////////////////////////


#include "JetSelector.h"
#include <string>

ClassImp(JetSelector);
JetSelector::JetSelector() : PAFChainItemSelector() {}
void JetSelector::Summary(){}

void JetSelector::Initialise(){
  gIsData    = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Bool_t>("iSelection");
  //Leptons    = GetParam<vector<Lepton>>("selLeptons"); 

	TString stringWP;

  if(gSelection == iStopSelec)   stringWP = "Medium";
  else stringWP = "Medium";

	fBTagSFnom = new BTagSFUtil("mujets", "CSVv2", stringWP,  0);
	fBTagSFbUp = new BTagSFUtil("mujets", "CSVv2", stringWP,  1);
	fBTagSFbDo = new BTagSFUtil("mujets", "CSVv2", stringWP, -1);
	fBTagSFlUp = new BTagSFUtil("mujets", "CSVv2", stringWP,  3);
	fBTagSFlDo = new BTagSFUtil("mujets", "CSVv2", stringWP, -3);
}

void JetSelector::GetJetVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet_px",i), Get<Float_t>("Jet_py",i), Get<Float_t>("Jet_pz", i), Get<Float_t>("Jet_energy",i));
  eta = tJ.p.Eta();
  pt = tJ.p.Pt();
  rawPt       = Get<Float_t>("Jet_rawPt",i);
  pt_corrUp   = Get<Float_t>("Jet_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("Jet_corr_JECDown",i);
  jetId       = Get<Int_t>("Jet_id",i);
  csv         = Get<Float_t>("Jet_btagCSV", i);
	if(!gIsData){
		flavmc = Get<Float_t>("Jet_mcFlavour", i);
		tmcJ.SetPxPyPzE(Get<Float_t>("Jet_mcPx",i), Get<Float_t>("Jet_mcPy",i), Get<Float_t>("Jet_mcPz",i), Get<Float_t>("Jet_mcEnergy",i));
	}
}

void JetSelector::InsideLoop(){
  // Clear vectors...
  selJets.clear();
  mcJets.clear();
  genJets.clear();
  vetoJets.clear();
  Jets15.clear();

  // Loop over the jets
  nJet = Get<Int_t>("nJet");
  for(Int_t i = 0; i < nJet; i++){
    GetJetVariables(i);
    tJ = Jet(tpJ, csv, jetId, flavmc);
		tJ.isBtag = IsBtag(tJ);
	SetSystematics(tJ);
	// Fill the vectors
		if(tJ.id > 0 && Cleaning(tJ, Leptons) && TMath::Abs(tJ.p.Eta()) < 2.4){
			SetSystematics(tJ);
			tJ.isBtag = IsBtag(tJ);
			if(tJ.p.Pt() > 15 || tJ.pTJESUp > 15 || tJ.pTJESDown > 15) Jets15.push_back(tJ);
			if(tJ.p.Pt() > 30 || tJ.pTJESUp > 30 || tJ.pTJESDown > 30) selJets.push_back(tJ);
			if(tJ.isBtag && tJ.p.Pt() > 20) vetoJets.push_back(tJ);
		}
	}
  // Add gen jets...
}

Bool_t JetSelector::IsBtag(Jet j){
	Bool_t isbtag;
  if(gIsData) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta());
  else        isbtag = fBTagSFnom->IsTagged(j.csv,j.flavmc, j.p.Pt(), j.p.Eta());
  return isbtag;
}

void JetSelector::SetSystematics(Jet j){
  Float_t _csv = j.csv; Int_t _flavmc = j.flavmc; Float_t _pt = j.p.Pt(); Float_t _eta = j.p.Eta();
	if(gIsData) return;
	j.pTJESUp     = rawPt*pt_corrUp;
	j.pTJESDown   = rawPt*pt_corrDown;
	j.pTJERUp     = _pt;
	j.pTJERDown   = _pt;
	j.isBtag_BtagUp      = fBTagSFbUp->IsTagged(_csv, _flavmc, _pt, _eta);
	j.isBtag_BtagDown    = fBTagSFbDo->IsTagged(_csv, _flavmc, _pt, _eta);
	j.isBtag_MisTagUp    = fBTagSFlUp->IsTagged(_csv, _flavmc, _pt, _eta);
	j.isBtag_MisTagDown  = fBTagSFlDo->IsTagged(_csv, _flavmc, _pt, _eta);
}

Bool_t JetSelector::Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR){
  Int_t nLeps = vLep.size();
  for(Int_t i = 0; i < nLeps; i++){
    if(j.p.DeltaR(vLep[i].p) < minDR) return false;
  }
  return true;
}
