//////////////////////////////////////////////////////////////////////////
//
//  Jet Selector
//  Produce vectors with: genJets, selJets, vetoJets...
//  also some variables like number of b-jets
//
//  All SFs and variables are within the Jet definition
//
//  ToDo: add JER syst (and JER jet variations to selJets)...
//
/////////////////////////////////////////////////////////////////////////


#include "JetSelector.h"
#include <string>

ClassImp(JetSelector);
JetSelector::JetSelector() : PAFChainItemSelector() {
  fBTagSFnom = 0;
  fBTagSFbUp = 0;
  fBTagSFbDo = 0;
  fBTagSFlUp = 0;
  fBTagSFlDo = 0;
  MeasType = "comb";
  minDR = 0;
  jet_MaxEta = 0;
  jet_MinPt = 0;
  vetoJet_minPt = 0;
}
void JetSelector::Summary(){}

void JetSelector::Initialise(){
  gIsData    = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Int_t>("iSelection");

  //---- Select your wp for b-tagging and pt, eta for the jets
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  if      (gSelection == iStopSelec || gSelection == iTopSelec || gSelection == ittDMSelec || gSelection == iTWSelec || gSelection == ittHSelec || gSelection == i4tSelec)  stringWP = "Medium";
  else if (gSelection == iWWSelec)    stringWP = "Loose";
  else if (gSelection == iWZSelec)    stringWP = "Loose";
  else                                stringWP = "Medium";
  if     (gSelection == iStopSelec || gSelection == iTopSelec || gSelection == ittDMSelec){
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == i4tSelec){
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 25;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == iTWSelec){
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 4.7;
    minDR = 0.4;
  }
  else if(gSelection == iWWSelec){
    jet_MaxEta = 4.7;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if(gSelection == iWZSelec){
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == ittHSelec) {
		jet_MaxEta 			= 2.4;
    jet_MinPt  			= 25;
    vetoJet_maxEta 	= 2.4;
    vetoJet_minPt 	= 25;
    minDR 					= 0.4;
	}
  else{
    jet_MaxEta = 2.4;
    vetoJet_maxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    minDR = 0.4;
  }
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (gSelection == iTopSelec) MeasType = "mujets";
  fBTagSFnom = new BTagSFUtil(MeasType, "CSVv2", stringWP,  0);
  fBTagSFbUp = new BTagSFUtil(MeasType, "CSVv2", stringWP,  1);
  fBTagSFbDo = new BTagSFUtil(MeasType, "CSVv2", stringWP, -1);
  fBTagSFlUp = new BTagSFUtil(MeasType, "CSVv2", stringWP,  3);
  fBTagSFlDo = new BTagSFUtil(MeasType, "CSVv2", stringWP, -3);

  Leptons  = std::vector<Lepton>();
  selJets  = std::vector<Jet>();
  mcJets   = std::vector<Jet>();
  genJets  = std::vector<Jet>();
  vetoJets = std::vector<Jet>();
}

void JetSelector::GetJetVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet_px",i), Get<Float_t>("Jet_py",i), Get<Float_t>("Jet_pz", i), Get<Float_t>("Jet_energy",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = Get<Float_t>("Jet_rawPt",i);
  pt_corrUp   = Get<Float_t>("Jet_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("Jet_corr_JECDown",i);
  jetId       = Get<Int_t>("Jet_id",i);
  csv         = Get<Float_t>("Jet_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("Jet_hadronFlavour", i);
    tmcJ.SetPxPyPzE(Get<Float_t>("Jet_mcPx",i), Get<Float_t>("Jet_mcPy",i), Get<Float_t>("Jet_mcPz",i), Get<Float_t>("Jet_mcEnergy",i));
  }
}

void JetSelector::GetJetFwdVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("JetFwd_px",i), Get<Float_t>("JetFwd_py",i), Get<Float_t>("JetFwd_pz", i), Get<Float_t>("JetFwd_energy",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = Get<Float_t>("JetFwd_rawPt",i);
  pt_corrUp   = Get<Float_t>("JetFwd_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("JetFwd_corr_JECDown",i);
  jetId       = Get<Int_t>("JetFwd_id",i);
  csv         = Get<Float_t>("JetFwd_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("JetFwd_hadronFlavour", i);
    tmcJ.SetPxPyPzE(Get<Float_t>("JetFwd_mcPx",i), Get<Float_t>("JetFwd_mcPy",i), Get<Float_t>("JetFwd_mcPz",i), Get<Float_t>("JetFwd_mcEnergy",i));
  }
}

void JetSelector::GetGenJetVariables(Int_t i){
  tpJ.SetPtEtaPhiM(Get<Float_t>("genJet_pt",i), Get<Float_t>("genJet_eta",i), Get<Float_t>("genJet_phi", i), Get<Float_t>("genJet_mass",i));
  eta = Get<Float_t>("genJet_eta",i);
  pt =  Get<Float_t>("genJet_pt",i);
}

void JetSelector::GetmcJetVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet_mcPx",i), Get<Float_t>("Jet_mcPy",i), Get<Float_t>("Jet_mcPz", i), Get<Float_t>("Jet_mcEnergy",i));
  eta = tpJ.Eta();
  pt =  Get<Float_t>("Jet_mcPt",i);
  flavmc = TMath::Abs(Get<Int_t>("Jet_mcFlavour",i));
  csv         = Get<Float_t>("Jet_btagCSV", i);
}

void JetSelector::InsideLoop(){
  // Clear vectors...
  selJets.clear();
  mcJets.clear();
  genJets.clear();
  vetoJets.clear();
  Jets15.clear();
  nBtagJets = 0;
  Leptons.clear();
  if (gSelection == ittHSelec) Leptons = GetParam<vector<Lepton>>("vetoLeptons");
  else                         Leptons = GetParam<vector<Lepton>>("selLeptons"); 

  evt = (UInt_t)Get<ULong64_t>("evt");

  // Loop over the jets
  nJet = Get<Int_t>("nJet");
  for(Int_t i = 0; i < nJet; i++){
    GetJetVariables(i);
    tJ = Jet(tpJ, csv, jetId, flavmc);
    tJ.isBtag = IsBtag(tJ);
    // Fill the vectors
    if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
      SetSystematics(&tJ);
      tJ.isBtag = IsBtag(tJ);
      if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
	if(tJ.p.Pt() > 15 || tJ.pTJESUp > 15 || tJ.pTJESDown > 15 ) Jets15.push_back(tJ);
	if(tJ.p.Pt() > jet_MinPt){
	  selJets.push_back(tJ);
	  if(tJ.isBtag) nBtagJets++; 
    if(!gIsData){
      GetmcJetVariables(i);
      tJ.SetMCjet(tpJ);
      GetJetVariables(i);
    }
	} 
      }
      if (tJ.p.Pt() > vetoJet_minPt && TMath::Abs(tJ.p.Eta()) < vetoJet_maxEta){
	if      (gSelection == iWWSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
	else if (gSelection == iWZSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
	else if (gSelection == i4tSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
	else if (gSelection == iTWSelec)                vetoJets.push_back(tJ);
	else                                            vetoJets.push_back(tJ);
      }
    }
  }
  if(jet_MaxEta > 2.4 || vetoJet_maxEta > 2.4){ // Add jets from JetFwd collection
    nJet = Get<Int_t>("nJetFwd");
    for(Int_t i = 0; i < nJet; i++){
      GetJetFwdVariables(i);
      tJ = Jet(tpJ, csv, jetId, flavmc);
      tJ.isBtag = false; // Fwd jets are not b-tagged
      if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
        SetSystematics(&tJ);
        tJ.isBtag  = false; // Fwd jets are not b-tagged
        if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
          if(tJ.p.Pt() > 15 || tJ.pTJESUp > 15 || tJ.pTJESDown > 15) Jets15.push_back(tJ);
          if(tJ.p.Pt() > jet_MinPt) selJets.push_back(tJ);
        }
	 if (tJ.p.Pt() > vetoJet_minPt && TMath::Abs(tJ.p.Eta()) < vetoJet_maxEta) vetoJets.push_back(tJ);
      }
    }
  }
  if(!gIsData){  // Add gen and mc jets...
    ngenJet = Get<Int_t>("ngenJet");
    for(Int_t i = 0; i < ngenJet; i++){
      GetGenJetVariables(i);
      tJ = Jet(tpJ, 0, 1, 0);
      genJets.push_back(tJ);    
    }
    nJet = Get<Int_t>("nJet");
    for(Int_t i = 0; i < nJet; i++){
      GetmcJetVariables(i);
      tJ = Jet(tpJ, csv, 1, flavmc);
      tJ.isBtag = (flavmc == 5);
      mcJets.push_back(tJ);
    }

  }

  nSelJets  = selJets.size();
  nJets15   = Jets15.size();
  nVetoJets = vetoJets.size();
  nGenJets  = genJets.size();

  // Set params...
  SetParam("selJets",  selJets);
  SetParam("Jets15",   Jets15);
  SetParam("vetoJets", vetoJets);
  SetParam("genJets",  genJets);
  SetParam("mcJets",  mcJets);
  SetParam("nSelJets",  nSelJets);
  SetParam("nJets15",  nJets15);
  SetParam("nVetoJets",  nVetoJets);
  SetParam("nGenJets",  nGenJets);
  SetParam("nSelBJets",  nBtagJets);

  // Propagate JES to MET
  if(nSelJets > 0){
    Float_t met_pt  = Get<Float_t>("met_pt");
    Float_t met_phi = Get<Float_t>("met_phi");
    MET_JESUp   = JEStoMET(selJets, met_pt, met_phi,  1);
    MET_JESDown = JEStoMET(selJets, met_pt, met_phi, -1);
    SetParam("MET_JESUp",   MET_JESUp);
    SetParam("MET_JESDown", MET_JESDown);
  }
}

Bool_t JetSelector::IsBtag(Jet j){
  Bool_t isbtag;
  if(gIsData) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  else if(stringWP == "Loose") isbtag = Get<Float_t>("Jet_csv");//fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta());
  else        isbtag = fBTagSFnom->IsTagged(j.csv,j.flavmc, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  return isbtag;
}

void JetSelector::SetSystematics(Jet *j){
  Float_t _csv = j->csv; Int_t _flavmc = j->flavmc; Float_t _pt = j->p.Pt(); Float_t _eta = j->p.Eta();
  if(gIsData) return;
  j->pTJESUp     = rawPt*pt_corrUp;
  j->pTJESDown   = rawPt*pt_corrDown;
  j->pTJERUp     = getJetJERpt(*j);
  j->pTJERDown   = getJetJERpt(*j);
  j->isBtag_BtagUp      = fBTagSFbUp->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt+1);
  j->isBtag_BtagDown    = fBTagSFbDo->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt-1);
  j->isBtag_MisTagUp    = fBTagSFlUp->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt+3);
  j->isBtag_MisTagDown  = fBTagSFlDo->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt-3);
}

Bool_t JetSelector::Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR){
  Int_t nLeps = vLep.size();
  for(Int_t i = 0; i < nLeps; i++){
    if(j.p.DeltaR(vLep[i].p) < minDR) return false;
  }
  return true;
}
