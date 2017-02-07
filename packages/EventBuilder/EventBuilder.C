#include "EventBuilder.h"

ClassImp(EventBuilder);
EventBuilder::EventBuilder() : PAFChainItemSelector() {}
void EventBuilder::Summary(){}

void EventBuilder::Initialise(){
	gIsData = GetParam<Bool_t>("IsData");
	gIsSelection = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gIsMCatNLO   = GetParam<Bool_t>("IsMCatNLO");
  IsFastSim =  gSampleName.BeginsWith("T2tt")? true : false; 
  
  gIsDoubleElec = false; gIsDoubleMuon = false; gIsSingleElec = false;
  gIsSingleMuon = false; gIsMuonEG = false;
       if(gSampleName.Contains("SingleMuon")) gIsDoubleElec = true;
  else if(gSampleName.Contains("DoubleMuon")) gIsDoubleMuon = true;
  else if(gSampleName.Contains("SingleElec")) gIsSingleElec = true;
  else if(gSampleName.Contains("SingleMuon")) gIsSingleMuon = true;
  else if(gSampleName.Contains("MuonEG")) gIsMuonEG     = true;
  
	TriggSF       = new LeptonSF();
	fPUWeight     = new PUWeight(19664.225, Spring2016_25ns_poisson_OOTPU, "2016_ichep");
	fPUWeightUp   = new PUWeight(18494.9,  Spring2016_25ns_poisson_OOTPU, "2016_ichep"); //  18494.9 
	fPUWeightDown = new PUWeight(20441.7,  Spring2016_25ns_poisson_OOTPU, "2016_ichep"); //  20441.7 

  NormWeight = GetParam<float>("weight");

  // Get selected leptons:
  // SelLeptons = GetParam<vector<Lepton>>("selLeptons");

	// if(gSelection == iStopSelec){
  // Load histograms for trigger SFs here!!!! 
	// }
	
	nTrueInt = 0;

	TrigSFElec = 0;
	TrigSFMuon = 0;
	TrigSFElMu = 0;
	TrigSFElec_Up = 0;
	TrigSFElMu_Up = 0;
	TrigSFMuon_Up = 0;
	TrigSFElec_Down = 0;
	TrigSFMuon_Down = 0;
	TrigSFElMu_Down = 0;
	PUSF = 0;
	PUSF_Up = 0;
	PUSF_Down = 0;
}

void EventBuilder::InsideLoop(){
  // Calculate norm weight
  if(gIsMCatNLO) genWeight = Get<Float_t>("genWeight");
  else           genWeight = 1;
  EventWeight = NormWeight*genWeight;

  // Calculate PU weight and variations  
  nTrueInt = Get<Float_t>("nTrueInt");
  PUSF      = fPUWeight    ->GetWeight(nTrueInt);
  PUSF_Up   = fPUWeightUp  ->GetWeight(nTrueInt);
  PUSF_Down = fPUWeightDown->GetWeight(nTrueInt);

  // Calculate Trigger SF and variations
/*  if(SelLeptons.size() < 2) continue;

  SF_Elec = TrigSF_Elec->GetLeptonSF(SelLeptons[0].p.Pt(),SelLeptons[0].p.Eta(), SelLeptons[1].p.Pt(), SelLeptons[1].p.Eta());  
  SF_Muon = TrigSF_Muon->GetLeptonSF(SelLeptons[0].p.Pt(),SelLeptons[0].p.Eta(), SelLeptons[1].p.Pt(), SelLeptons[1].p.Eta());  
  SF_ElMu = TrigSF_ElMu->GetLeptonSF(SelLeptons[0].p.Pt(),SelLeptons[0].p.Eta(), SelLeptons[1].p.Pt(), SelLeptons[1].p.Eta());  
*/

  // Pass trigger ee/eµ/µµ
	PassHLT_Elec = PassesDoubleElecTrigger();
	PassHLT_Muon = PassesDoubleMuonTrigger();
	PassHLT_ElMu = PassesElMuTrigger();

  // Set Params to pass all the info...
  SetParam("PassDoubleElecTrig", PassHLT_Elec);
  SetParam("PassDoubleMuonTrig", PassHLT_Muon);
  SetParam("PassDoubleElMuTrig", PassHLT_ElMu);

  SetParam("TrigSFElec",      TrigSFElec);
  SetParam("TrigSFMuon",      TrigSFMuon);
  SetParam("TrigSFElMu",      TrigSFElMu);
  SetParam("TrigSFElec_Up",   TrigSFElec_Up);
  SetParam("TrigSFMuon_Up",   TrigSFMuon_Up);
  SetParam("TrigSFElMu_Up",   TrigSFElMu_Up);
  SetParam("TrigSFElec_Down", TrigSFElec_Down);
  SetParam("TrigSFMuon_Down", TrigSFMuon_Down);
  SetParam("TrigSFElMu_Down", TrigSFElMu_Down);

  SetParam("PUSF",      PUSF);
  SetParam("PUSF_Up",   PUSF_Up);
  SetParam("PUSF_Down", PUSF_Down);

  SetParam("EventWeight", EventWeight);
}

Bool_t EventBuilder::PassesDoubleElecTrigger(){
  Bool_t pass = false;
  Bool_t passEE = Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");
  if(!gIsData){ // for MC: DOUBLE || SINGLE
    pass = passEE || PassesSingleElecTrigger();
  }
  else{ // for Data, avoid double counting
    if     (gIsDoubleElec) pass =  passEE;
    else if(gIsSingleElec) pass = !passEE && PassesSingleElecTrigger();
  }
  return pass;
}

Bool_t EventBuilder::PassesDoubleMuonTrigger(){
  Bool_t pass = false;
	Bool_t passMM = (
			Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"  ) ||
			Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") );
  if(!gIsData) pass = passMM || PassesSingleMuonTrigger();
  else{
    if     (gIsDoubleMuon) pass =  passMM;
    else if(gIsSingleMuon) pass = !passMM && PassesSingleMuonTrigger();
  }
  return pass;
}

Bool_t EventBuilder::PassesElMuTrigger(){
  Bool_t pass = false;
  Bool_t passEM = (
			Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
			Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") );
  if(!gIsData) pass = passEM || PassesSingleMuonTrigger() || PassesSingleElecTrigger();
  else{
    if      (gIsMuonEG)     pass = passEM;
    else if (gIsSingleMuon) pass = !passEM &&  PassesSingleMuonTrigger();
    else if (gIsSingleElec) pass = !passEM && !PassesSingleMuonTrigger() && PassesSingleElecTrigger();
  }
  return pass;
}

Bool_t EventBuilder::PassesSingleElecTrigger(){
  Bool_t passSingleElec = Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v");
  return passSingleElec;
}

Bool_t EventBuilder::PassesSingleMuonTrigger(){
	Bool_t passSingleMuon = (
			Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v") ||
			Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") );
  return passSingleMuon;
}

