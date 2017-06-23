//////////////////////////////////////////////////////////////////////////////////////
//
//  EventBuilder: compute all important variables related with the event
//  Triggers, PU reweighting, global SFs and systematics...
//
//  To do: Add trigger SFs when become available
//         Add LHE weights when become available
//         Update PU weights
//
//////////////////////////////////////////////////////////////////////////////////////


#include "EventBuilder.h"

ClassImp(EventBuilder);
EventBuilder::EventBuilder() : PAFChainItemSelector() {}
void EventBuilder::Summary(){}


// Select your triggers
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool_t EventBuilder::PassesDoubleElecTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;
  gIsData = GetParam<Bool_t>("IsData");
  if (gIsData)
    run     = Get<Int_t>("run");
  if(gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec || gSelection == iWZSelec){ //|| gSelection == i4tSelec
    // Run B-G, same as H
    pass = (Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"));
    return pass;
  }
  else if (gSelection == iStopSelec){
    pass = (Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") || // Este sobra?
	     	Get<Int_t>("HLT_BIT_HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v")); 
	     	//Get<Int_t>("HLT_BIT_HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v") ||
	     	//Get<Int_t>("HLT_BIT_HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v") 
    return pass;
  }
  else if (gSelection == ittHSelec) {
    pass = (Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele25_eta2p1_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_eta2p1_WPLoose_Gsf_v"));
    return pass;
  }
  else{
    cout << "[EventBuilder] Wrong selection for checking trigger requirements!!" << endl;
    return false;
  }
}

Bool_t EventBuilder::PassesDoubleMuonTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;
  if (gIsData)
    run     = Get<Int_t>("run");
  if(gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec || gSelection == iWZSelec){// || gSelection == i4tSelec){
    // Run B-G or MC
    if ( (gIsData && run <= 280385) || (!gIsData)){
      pass = (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v")  ||
	      Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v"));
      return pass;
    }
    else{
      pass = ( Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
	       Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"));
      return pass;
    }
  }
  else if (gSelection == iStopSelec){
    pass = (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
        Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v") ||
        Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v") ||
        Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ); // Ifca: HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*
    return pass;
  }
  else if (gSelection == ittHSelec) {
    pass =  (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
        Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ||
        Get<Int_t>("HLT_BIT_HLT_IsoMu22_v")   ||
        Get<Int_t>("HLT_BIT_HLT_IsoTkMu22_v") ||
        Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") ||
        Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v"));
    return pass;
  }
  else{
    cout << "[EventBuilder] Wrong selection for checking trigger requirements!!" << endl;
    return pass;
  }
}

Bool_t EventBuilder::PassesElMuTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;
  if (gIsData)
    run     = Get<Int_t>("run");
  if( gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec || gSelection == iWZSelec){// || gSelection == i4tSelec){
    // Run B-G or MC
    if ( (gIsData && run <= 280385) || (!gIsData)){
      pass = ( Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v")  ||
	       Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") );
      return pass;
    }
    else{
      pass = ( Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v")||
	       Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") );
      return pass;
    }
  }
  else if (gSelection == iStopSelec){
    pass = (
	    //Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v") ||
	    //Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
	    Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
	    Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
	    //Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
	    //Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") ||
	    Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
	    Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v")   //IFCA: Mu12
	    //Get<Int_t>("HLT_BIT_HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v") 
	    );
    return pass;
  }
  else if (gSelection == ittHSelec) {
    pass =  (Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v") ||
	    	    Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
			      Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
			      Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
         	  Get<Int_t>("HLT_BIT_HLT_IsoMu22_v")   ||
         	  Get<Int_t>("HLT_BIT_HLT_IsoTkMu22_v") ||
         	  Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") ||
         	  Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele25_eta2p1_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_eta2p1_WPLoose_Gsf_v"));
	return pass;
  }
  else{
    cout << "[EventBuilder] Wrong selection for checking trigger requirements!!" << endl;
    return false;
  }
}

Bool_t EventBuilder::PassesSingleElecTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;
  if(gSelection == iStopSelec)
    pass =  Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v") ||
      Get<Int_t>("HLT_BIT_HLT_Ele25_eta2p1_WPTight_Gsf_v");
  else if( gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec || gSelection == iWZSelec) 
    pass =  Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v");
  return pass;
}

Bool_t EventBuilder::PassesSingleMuonTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;
  if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec || gSelection == iWZSelec)
    pass = (
	    Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v") ||
	    Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") );
  return pass;
}

Bool_t EventBuilder::PassesDoubleMuonHTTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  return Get<Int_t>("HLT_BIT_HLT_DoubleMu8_Mass8_PFHT300_v");
}

Bool_t EventBuilder::PassesDoubleElecHTTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  return Get<Int_t>("HLT_BIT_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v");
}

Bool_t EventBuilder::PassesElMuHTTrigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  return Get<Int_t>("HLT_BIT_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v");
}

Bool_t EventBuilder::PassesPFJet450Trigger(){
  if(gIsFastSim) return true; // no triger in FastSim samples
  return Get<Int_t>("HLT_BIT_HLT_PFHT400_v"); // Jet450
}

Bool_t EventBuilder::PassesThreelFourlTrigger() {
  if(gIsFastSim) return true; // no triger in FastSim samples
  Bool_t pass = false;

  if (gSelection == ittHSelec) {
    pass =  (Get<Int_t>("HLT_BIT_HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v") ||
    		    Get<Int_t>("HLT_BIT_HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v") ||
            Get<Int_t>("HLT_BIT_HLT_TripleMu_12_10_5_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoMu22_v")   ||
            Get<Int_t>("HLT_BIT_HLT_IsoTkMu22_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele25_eta2p1_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele27_eta2p1_WPLoose_Gsf_v"));
  }
  return pass;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void EventBuilder::Initialise(){
  gIsData = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gIsMCatNLO   = GetParam<Bool_t>("IsMCatNLO");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  gChannel = -1;

  selLeptons = std::vector<Lepton>();
  vetoLeptons = std::vector<Lepton>();

  gIsDoubleElec = false; gIsDoubleMuon = false; gIsSingleElec = false;
  gIsSingleMuon = false; gIsMuonEG = false;
  if(gSampleName.Contains("DoubleEG")) gIsDoubleElec = true;
  else if(gSampleName.Contains("DoubleMuon")) gIsDoubleMuon = true;
  else if(gSampleName.Contains("SingleElec")) gIsSingleElec = true;
  else if(gSampleName.Contains("SingleMuon")) gIsSingleMuon = true;
  else if(gSampleName.Contains("MuonEG"))     gIsMuonEG     = true;

  fPUWeight     = new PUWeight(19468.3, Moriond17MC_PoissonOOTPU, "2016_Moriond17");
  if (!gIsData) {
    fPUWeightUp   = new PUWeight(18494.9,  Moriond17MC_PoissonOOTPU, "2016_Moriond17"); //  18494.9
    fPUWeightDown = new PUWeight(20441.7,  Moriond17MC_PoissonOOTPU, "2016_Moriond17"); //  20441.7
  }

  Weight = GetParam<Float_t>("weight");

  TriggSF       = new LeptonSF();
  // >>>>>>>>>>>>>>> Load histograms for trigger used in analysis  <<<<<<<<<<<<<<<<<<<<<<< Needs to be updated!!!
  // if(gSelection == iStopSelec){
  // Load histograms for trigger SFs here!!!!
  // }
  passTrigger = 1;
  isSS = 0;
  nTrueInt = 0;

  PUSF = 1;
  PUSF_Up = 1;
  PUSF_Down = 1;
}

void EventBuilder::InsideLoop(){
  // >>>>>>>>>>>>>> Get selected leptons:
  selLeptons = GetParam<std::vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<std::vector<Lepton>>("vetoLeptons");
  // Set channel
  if(selLeptons.size() >= 2){ // Dilepton Channels
    if     (selLeptons.at(0).isElec && selLeptons.at(1).isMuon) gChannel = iElMu;
    else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) gChannel = iElMu;
    else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) gChannel = iMuon;
    else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) gChannel = iElec;
    isSS = (selLeptons[0].charge*selLeptons[1].charge) > 0;
  }
  else{
    isSS = false;
    gChannel = -1;
  }
  
  if(gSelection == ittHSelec && selLeptons.size() > 2){
    if      (selLeptons.size() == 3) gChannel = iTriLep;
    else if (selLeptons.size() >= 4) gChannel = iFourLep;
  }

  passTrigger = false;

  if(gSelection == i4tSelec){
    Int_t nLepForCharge = 0;
    Int_t pdgIdsum = 0;
    for(Int_t i = 0; i < (Int_t) selLeptons.size(); i++){
      if(nLepForCharge >= 2) continue;
      pdgIdsum = selLeptons.at(i).isElec? pdgIdsum + 11 : pdgIdsum + 13;
      nLepForCharge++;
    }
    for(Int_t i = 0; i < (Int_t) vetoLeptons.size(); i++){
      if(nLepForCharge >= 2) continue;
      pdgIdsum = vetoLeptons.at(i).isElec? pdgIdsum + 11 : pdgIdsum + 13;
      nLepForCharge++;
    }
    if(nLepForCharge < 2) pdgIdsum = -1;
    if(gIsData){
      if(gIsMuonEG){
        if     (pdgIdsum == 24) passTrigger = PassesElMuHTTrigger()       || PassesPFJet450Trigger();
        else if(pdgIdsum == 26) passTrigger = !PassesPFJet450Trigger() && !PassesDoubleMuonHTTrigger() && (PassesElMuHTTrigger() || PassesDoubleElecHTTrigger());
        else if(pdgIdsum == 22) passTrigger = !PassesPFJet450Trigger() && !PassesDoubleElecHTTrigger() && (PassesElMuHTTrigger() || PassesDoubleMuonHTTrigger());
        else                    passTrigger = false;
      }
      if(gIsDoubleMuon){
        if     (pdgIdsum == 26) passTrigger = PassesDoubleMuonHTTrigger() || PassesPFJet450Trigger();
        else                    passTrigger = false;
      }
      if(gIsDoubleElec){
        if     (pdgIdsum == 22) passTrigger = PassesDoubleElecHTTrigger() || PassesPFJet450Trigger();
        else                    passTrigger = false;
      }
    }
    else                    passTrigger = PassesElMuHTTrigger() || PassesDoubleMuonHTTrigger() || PassesDoubleElecHTTrigger() || PassesPFJet450Trigger();
  }

  else{
    if      (gChannel == iElMu && TrigElMu()) passTrigger = true;
    else if (gChannel == iMuon && TrigMuMu()) passTrigger = true;
    else if (gChannel == iElec && TrigElEl()) passTrigger = true;
    else if ((gChannel == iTriLep || gChannel == iFourLep) && Trig3l4l()) passTrigger = true;
  }

  METfilters = PassesMETfilters();

  // >>>>>>>>> Calculate norm weight
  if(gIsMCatNLO) genWeight = Get<Float_t>("genWeight");
  else           genWeight = 1;
  NormWeight = Weight*genWeight;

  // >>>>>>>>> Calculate PU weight and variations
  if(!gIsData){
    nTrueInt = Get<Float_t>("nTrueInt");
    PUSF      = fPUWeight    ->GetWeight(nTrueInt);
    PUSF_Up   = fPUWeightUp  ->GetWeight(nTrueInt);
    PUSF_Down = fPUWeightDown->GetWeight(nTrueInt);
  } else{
    PUSF      = 1;
    PUSF_Up   = 1;
    PUSF_Down = 1;
  }

  // Set Params to pass all the info...
  SetParam("PUSF",      PUSF);
  SetParam("PUSF_Up",   PUSF_Up);
  SetParam("PUSF_Down", PUSF_Down);

  SetParam("gChannel",        gChannel);
  SetParam("NormWeight",      NormWeight);
  SetParam("passTrigger",     passTrigger);
  SetParam("isSS",            isSS);
  SetParam("METfilters",      METfilters);
}


Bool_t EventBuilder::TrigElEl(){
  Bool_t pass = false;
  if(gIsData){
    if     (gIsDoubleElec) pass =  PassesDoubleElecTrigger();
    else if(gIsSingleElec) pass = !PassesDoubleElecTrigger() && PassesSingleElecTrigger();
  }
  else pass = PassesDoubleElecTrigger() || PassesSingleElecTrigger();
  return pass;
}

Bool_t EventBuilder::TrigMuMu(){
  Bool_t pass = false;
  if(gIsData){
    if     (gIsDoubleMuon) pass =  PassesDoubleMuonTrigger();
    else if(gIsSingleMuon) pass = !PassesDoubleMuonTrigger() && PassesSingleMuonTrigger();
  }
  else pass = PassesDoubleMuonTrigger() || PassesSingleMuonTrigger();
  return pass;
}

Bool_t EventBuilder::TrigElMu(){
  Bool_t pass = false;
  if(gIsData){
    if(gIsMuonEG    ) pass =  PassesElMuTrigger();
    else if(gIsSingleMuon) pass = !PassesElMuTrigger() &&  PassesSingleMuonTrigger();
    else if(gIsSingleElec) pass = !PassesElMuTrigger() && !PassesSingleMuonTrigger() && PassesSingleElecTrigger();
  }
  else pass = PassesElMuTrigger() || PassesSingleMuonTrigger() || PassesSingleElecTrigger();
  return pass;
}

Bool_t EventBuilder::Trig3l4l() {
  Bool_t pass = false;
  Int_t ne    = 0;
  Int_t nm    = 0;
  
  for (UInt_t i = 0; i < selLeptons.size(); i++) {
    if (selLeptons.at(i).isMuon)  nm++;
    else                          ne++;
  }
  
  pass = PassesThreelFourlTrigger();
  
  if(gIsData) {
    if      (gIsSingleMuon && ((nm+ne != nm) || (nm + ne != ne))) pass = false;
    else if (gIsSingleElec && ((nm+ne != nm) || (nm + ne != ne))) pass = false;
    else if (gIsDoubleMuon) pass = false;
    else if (gIsDoubleElec) pass = false;
  }
  return pass;
}


Bool_t EventBuilder::PassesMETfilters(){
  // https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#Moriond_2017
  if(gIsFastSim) return true;
  if( (Get<Int_t>("Flag_HBHENoiseFilter") &&        // MET filters for data and MC
        Get<Int_t>("Flag_HBHENoiseIsoFilter") &&
        Get<Int_t>("Flag_EcalDeadCellTriggerPrimitiveFilter") &&
        Get<Int_t>("Flag_goodVertices") &&
        Get<Int_t>("Flag_badMuonFilter") &&
        Get<Int_t>("Flag_badChargedHadronFilter"))
      && (
        gIsFastSim || // no more MET filters for Fast Sim
        (!gIsData && Get<Int_t>("Flag_globalTightHalo2016Filter")) || // for MC
        ( gIsData && Get<Int_t>("Flag_globalTightHalo2016Filter") && Get<Int_t>("Flag_eeBadScFilter")) ) // for Data
        //( gIsData && Get<Int_t>("Flag_globalTightHalo2016Filter")) ) // for Data
    ) return true;
  else return false;
}
