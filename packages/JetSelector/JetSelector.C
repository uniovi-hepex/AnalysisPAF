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
JetSelector::JetSelector() : PAFChainItemSelector() {}
void JetSelector::Summary(){}

void JetSelector::Initialise(){
  gIsData    = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Int_t>("iSelection");

  //---- Select your wp for b-tagging and pt, eta fot the jets
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  if      (gSelection == iStopSelec || iTopSelec)  stringWP = "Medium";
  else if (gSelection == iWWSelec)    stringWP = "Loose";
  else                                stringWP = "Medium";
  if     (gSelection == iStopSelec || iTopSelec){
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    minDR = 0.4;
  }
  else if(gSelection == iWWSelec){
    jet_MaxEta = 4.7;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    minDR = 0.4;
  }
  else{
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    minDR = 0.4;
  }
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  fBTagSFnom = new BTagSFUtil("mujets", "CSVv2", stringWP,  0);
  fBTagSFbUp = new BTagSFUtil("mujets", "CSVv2", stringWP,  1);
  fBTagSFbDo = new BTagSFUtil("mujets", "CSVv2", stringWP, -1);
  fBTagSFlUp = new BTagSFUtil("mujets", "CSVv2", stringWP,  3);
  fBTagSFlDo = new BTagSFUtil("mujets", "CSVv2", stringWP, -3);

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

void JetSelector::GetGenJetVariables(Int_t i){
  tpJ.SetPtEtaPhiM(Get<Float_t>("genJet_pt",i), Get<Float_t>("genJet_eta",i), Get<Float_t>("genJet_phi", i), Get<Float_t>("genJet_mass",i));
  eta = Get<Float_t>("genJet_eta",i);
  pt =  Get<Float_t>("genJet_pt",i);
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
  Leptons    = GetParam<vector<Lepton>>("selLeptons"); 

  // Loop over the jets
  nJet = Get<Int_t>("nJet");
  for(Int_t i = 0; i < nJet; i++){
    GetJetVariables(i);
    tJ = Jet(tpJ, csv, jetId, flavmc);
    tJ.isBtag = IsBtag(tJ);
    // Fill the vectors
    if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR) && TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
      SetSystematics(&tJ);
      tJ.isBtag = IsBtag(tJ);
      if(tJ.p.Pt() > 15 || tJ.pTJESUp > 15 || tJ.pTJESDown > 15) Jets15.push_back(tJ);
      if(tJ.isBtag && tJ.p.Pt() > vetoJet_minPt) vetoJets.push_back(tJ);
      if(tJ.p.Pt() > jet_MinPt){
        selJets.push_back(tJ);
        if(tJ.isBtag) nBtagJets++; 
      } 
    }
  }
  if(!gIsData){  // Add gen jets...
    ngenJet = Get<Int_t>("ngenJet");
    for(Int_t i = 0; i < ngenJet; i++){
      GetGenJetVariables(i);
      tJ = Jet(tpJ, 0, 0, 0);
      genJets.push_back(tJ);    
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
  SetParam("nSelJets",  nSelJets);
  SetParam("nJets15",  nJets15);
  SetParam("nVetoJets",  nVetoJets);
  SetParam("nGenJets",  nGenJets);
  SetParam("nSelBJets",  nBtagJets);
}

Bool_t JetSelector::IsBtag(Jet j){
  Bool_t isbtag;
  if(gIsData) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta());
  else if(stringWP == "Loose") isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta());
  else        isbtag = fBTagSFnom->IsTagged(j.csv,j.flavmc, j.p.Pt(), j.p.Eta());
  return isbtag;
}

void JetSelector::SetSystematics(Jet *j){
  Float_t _csv = j->csv; Int_t _flavmc = j->flavmc; Float_t _pt = j->p.Pt(); Float_t _eta = j->p.Eta();
  if(gIsData) return;
  j->pTJESUp     = rawPt*pt_corrUp;
  j->pTJESDown   = rawPt*pt_corrDown;
  j->pTJERUp     = _pt;
  j->pTJERDown   = _pt;
  j->isBtag_BtagUp      = fBTagSFbUp->IsTagged(_csv, _flavmc, _pt, _eta);
  j->isBtag_BtagDown    = fBTagSFbDo->IsTagged(_csv, _flavmc, _pt, _eta);
  j->isBtag_MisTagUp    = fBTagSFlUp->IsTagged(_csv, _flavmc, _pt, _eta);
  j->isBtag_MisTagDown  = fBTagSFlDo->IsTagged(_csv, _flavmc, _pt, _eta);
}

Bool_t JetSelector::Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR){
  Int_t nLeps = vLep.size();
  for(Int_t i = 0; i < nLeps; i++){
    if(j.p.DeltaR(vLep[i].p) < minDR) return false;
  }
  return true;
}
