#include "JetSelector.h"

ClassImp(JetSelector);
JetSelector::JetSelector() : PAFChainItemSelector() {}
void JetSelector::Summary(){}

void JetSelector::Initialise(){
  gIsData    = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Bool_t>("iSelection");
  //Leptons    = GetParam<vector<Lepton>>("selLeptons"); 

	TString stringWP;
	TString BtagVar;

  if(gSelection == iStopSelec){
    stringWP = "Medium";
    BtagVar  = "CSVv2";
  }

	fBTagSFnom = new BTagSFUtil("mujets", BtagVar, stringWP,  0);
	fBTagSFbUp = new BTagSFUtil("mujets", BtagVar, stringWP,  1);
	fBTagSFbDo = new BTagSFUtil("mujets", BtagVar, stringWP, -1);
	fBTagSFlUp = new BTagSFUtil("mujets", BtagVar, stringWP,  3);
	fBTagSFlDo = new BTagSFUtil("mujets", BtagVar, stringWP, -3);
}

void JetSelector::GetJetVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet_px",i), Get<Float_t>("Jet_py",i), Get<Float_t>("Jet_pz", i), Get<Float_t>("Jet_energy",i));
  eta = tJ.Eta();
  pt = tJ.Pt();
  rawPt = Get<Float_t>("Jet_rawPt",i);
  pt_corrUp   = Get<Float_t>("Jet_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("Jet_corr_JECDown",i);
  jetId = Get<Int_t>("Jet_id",i);;
  csv = Get<Float_t>("Jet_btagCSV", i);
	if(!gIsData){
		mcfalv = Get<Float_t>("Jet_mcFlavour", i);
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
    GetHetVariables(i);
    tJ = Jet(tpJ, csv, jetId, mcflav);
    tJ.isBtag = IsBtag(tJ);
    SetSystematics(tJ);
    // Fill the vectors
    //if(tJ.id > 0 && tJ.p.Pt() > 15 && tJ.p.Eta() < 2.4){
    //}
  }

}

Bool_t JetSelector::IsBtag(Jet j){
  Bool_t isbtag;
  if(gIsData) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta());
  else        isbtag = fBTagSFnom->IsTagged(j.csv,j.mcflav, j.p.Pt(), j.p.Eta());
  return isbtag;
}

void Jet::SetSystematics(Jet j){
  Float_t _csv = j.csv; Int_t _mvflav = j.mcflav; Float_t _pt = j.p.Pt(); Float_t _eta = j.p.Eta();
	if(gIsData) return;
	j.pTJESUp     = _pt;
	j.pTJESDown   = _pt;
	j.pTJERUp     = _pt;
	j.pTJERDown   = _pt;
	j.isBtag_BtagUp      = fBTagSFbUp->IsTagged(_csv, _mcflav, _pt, _eta);
	j.isBtag_BtagDown    = fBTagSFbDo->IsTagged(_csv, _mcflav, _pt, _eta);
	j.isBtag_MisTagUp    = fBTagSFlUp->IsTagged(_csv, _mcflav, _pt, _eta);
	j.isBtag_MisTagDown  = fBTagSFlDo->IsTagged(_csv, _mcflav, _pt, _eta);
}
