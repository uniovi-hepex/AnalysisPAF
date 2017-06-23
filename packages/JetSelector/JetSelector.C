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

JetSelector::~JetSelector() {
  delete fBTagSFnom; 
  delete fBTagSFbUp; 
  delete fBTagSFbDo;
  delete fBTagSFlUp; 
  delete fBTagSFlDo;
}

void JetSelector::Summary(){}

void JetSelector::Initialise(){
  gIsData    = GetParam<Bool_t>("IsData");
  gSelection = GetParam<Int_t>("iSelection");

  //---- Select your wp for b-tagging and pt, eta for the jets
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if     (gSelection == iStopSelec || gSelection == iTopSelec || gSelection == ittDMSelec){
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == i4tSelec){
    taggerName="CSVv2"; // Soon to be: taggerName="DeepCSV" 
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 40;
    vetoJet_minPt = 25;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == iTWSelec){
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 15.;
    vetoJet_maxEta = 4.7;
    minDR = 0.4;
  }
  else if(gSelection == iWWSelec){
    taggerName="CSVv2";
    stringWP = "Loose";
    jet_MaxEta = 4.7;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if(gSelection == iWZSelec){
    taggerName="CSVv2";
    stringWP = "Loose";
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == ittHSelec) {
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta 	   = 2.4;
    jet_MinPt  	   = 25;
    vetoJet_maxEta = 2.4;
    vetoJet_minPt  = 25;
    minDR          = 0.4;
  }
  else{
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    vetoJet_maxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 20;
    minDR = 0.4;
  }
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (gSelection == iTopSelec) MeasType = "mujets";
  TString pwd  = GetParam<TString>("WorkingDir");
  TString BTagSFPath = Form("%s/packages/BTagSFUtil", pwd.Data());
  

  fBTagSFnom = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  0);
  fBTagSFbUp = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  1);
  fBTagSFbDo = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP, -1);
  fBTagSFlUp = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  3);
  fBTagSFlDo = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP, -3);

  Leptons  = std::vector<Lepton>();
  selJets  = std::vector<Jet>();
  mcJets   = std::vector<Jet>();
  genJets  = std::vector<Jet>();
  vetoJets = std::vector<Jet>();
  selJetsJecUp = std::vector<Jet>();
  selJetsJER   = std::vector<Jet>();
  selJetsJecDown = std::vector<Jet>();
  

}

void JetSelector::GetJetVariables(Int_t i, const TString& jec){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_px",i), Get<Float_t>("Jet"+jec+"_py",i), Get<Float_t>("Jet"+jec+"_pz", i), Get<Float_t>("Jet"+jec+"_energy",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = Get<Float_t>("Jet"+jec+"_rawPt",i);
  pt_corrUp   = Get<Float_t>("Jet"+jec+"_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("Jet"+jec+"_corr_JECDown",i);
  jetId       = Get<Int_t>("Jet"+jec+"_id",i);
  csv         = Get<Float_t>("Jet"+jec+"_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("Jet"+jec+"_hadronFlavour", i);
    tmcJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_mcPx",i), Get<Float_t>("Jet"+jec+"_mcPy",i), Get<Float_t>("Jet"+jec+"_mcPz",i), Get<Float_t>("Jet"+jec+"_mcEnergy",i));
  }
}

void JetSelector::GetDiscJetVariables(Int_t i){
  tpJ.SetPxPyPzE(Get<Float_t>("DiscJet_px",i), Get<Float_t>("DiscJet_py",i), Get<Float_t>("DiscJet_pz", i), Get<Float_t>("DiscJet_energy",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = Get<Float_t>("DiscJet_rawPt",i);
  pt_corrUp   = Get<Float_t>("DiscJet_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("DiscJet_corr_JECDown",i);
  jetId       = Get<Int_t>("DiscJet_id",i);
  csv         = Get<Float_t>("DiscJet_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("DiscJet_hadronFlavour", i);
    tmcJ.SetPxPyPzE(Get<Float_t>("DiscJet_mcPx",i), Get<Float_t>("DiscJet_mcPy",i), Get<Float_t>("DiscJet_mcPz",i), Get<Float_t>("DiscJet_mcEnergy",i));
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

void JetSelector::GetmcJetVariables(Int_t i, const TString& jec){
  tpJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_mcPx",i), Get<Float_t>("Jet"+jec+"_mcPy",i), Get<Float_t>("Jet"+jec+"_mcPz", i), Get<Float_t>("Jet"+jec+"_mcEnergy",i));
  eta = tpJ.Eta();
  pt =  Get<Float_t>("Jet"+jec+"_mcPt",i);
  flavmc = TMath::Abs(Get<Int_t>("Jet"+jec+"_mcFlavour",i));
  csv         = Get<Float_t>("Jet"+jec+"_btagCSV", i);
}

void JetSelector::InsideLoop(){
  // Clear vectors...
  selJets.clear();
  selJetsJecUp.clear();
  selJetsJecDown.clear();
  selJetsJER.clear();
  mcJets.clear();
  genJets.clear();
  vetoJets.clear();
  Jets15.clear();
  nBtagJets = 0;
  nBtagJetsJECUp = 0;
  nBtagJetsJECDown = 0;
  Leptons.clear();
  VetoLeptons.clear();

  BtagSF           = 1.;
  BtagSFBtagUp     = 1.;
  BtagSFBtagDown   = 1.;
  BtagSFMistagUp   = 1.;
  BtagSFMistagDown = 1.;

  if (gSelection == ittHSelec) Leptons = GetParam<vector<Lepton>>("vetoLeptons");
  else                         Leptons = GetParam<vector<Lepton>>("selLeptons"); 
  VetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  if (gSelection == i4tSelec){
    for(Int_t i = 0; i < (Int_t) VetoLeptons.size(); i++) Leptons.push_back(VetoLeptons.at(i));
  }

  evt = (UInt_t)Get<ULong64_t>("evt");
  rho = Get<Float_t>("rho");

  // Loop over the jets
  nJet = Get<Int_t>("nJet");
  for(Int_t i = 0; i < nJet; i++){
    GetJetVariables(i);
    tJ = Jet(tpJ, csv, jetId, flavmc);
    tJ.isBtag = IsBtag(tJ);
    if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
      if(!gIsData){
	GetmcJetVariables(i);
	tJ.SetMCjet(tpJ);
	GetJetVariables(i);
      }
      SetSystematics(&tJ);

      // Fill the vectors

      tJ.isBtag = IsBtag(tJ);
      if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
        if(tJ.p.Pt() > 15 || tJ.pTJESUp > 15 || tJ.pTJESDown > 15 || tJ.pTJERUp > 15 ) Jets15.push_back(tJ);
        if(tJ.p.Pt() > jet_MinPt){
          selJets.push_back(tJ);
          if(tJ.isBtag) nBtagJets++; 
        } 
      }
      if (tJ.p.Pt() > vetoJet_minPt && TMath::Abs(tJ.p.Eta()) < vetoJet_maxEta){
        if      (gSelection == iWWSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iWZSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == i4tSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iTWSelec){
	  vetoJets.push_back(tJ);
	  if (!gIsData){
	    if ( tJ.p.Pt() < 20.) continue;
	    BtagSF           *= fBTagSFnom->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
	    BtagSFBtagUp     *= fBTagSFbUp->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
	    BtagSFBtagDown   *= fBTagSFbDo->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
	    BtagSFMistagUp   *= fBTagSFlUp->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
	    BtagSFMistagDown *= fBTagSFlDo->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
	  }
	}
        else                                            vetoJets.push_back(tJ);
      }
    }
  }
  // If stop, disc
  if(gSelection == iStopSelec){
    nJet = Get<Int_t>("nDiscJet");
    for(Int_t i = 0; i < nJet; i++){
      GetDiscJetVariables(i);
      tJ = Jet(tpJ, csv, jetId, flavmc);
      tJ.isBtag = IsBtag(tJ);
      
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
          vetoJets.push_back(tJ);
        }
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

  // Loop over the jets
  if (!gIsData && gSelection == iTWSelec){
    nJet = Get<Int_t>("nJet_jecUp");
    for(Int_t i = 0; i < nJet; i++){
      GetJetVariables(i,"_jecUp");
      tJ = Jet(tpJ, csv, jetId, flavmc);
      tJ.isBtag = IsBtag(tJ);
      // Fill the vectors
      if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
	SetSystematics(&tJ);
	tJ.isBtag = IsBtag(tJ);
	if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
	  if(tJ.p.Pt() > jet_MinPt){
	  selJetsJecUp.push_back(tJ);
	  if(tJ.isBtag) nBtagJetsJECUp++; 
	  }
	}
      }
    }
    std::sort( selJetsJecUp.begin(), selJetsJecUp.end(), ByPt);
  }
  if (!gIsData  && gSelection == iTWSelec){
    nJet = Get<Int_t>("nJet_jecDown");
    for(Int_t i = 0; i < nJet; i++){
      GetJetVariables(i,"_jecDown");
      tJ = Jet(tpJ, csv, jetId, flavmc);
      tJ.isBtag = IsBtag(tJ);
      // Fill the vectors
      if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
	SetSystematics(&tJ);
	tJ.isBtag = IsBtag(tJ);
	if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
	  if(tJ.p.Pt() > jet_MinPt){
	  selJetsJecDown.push_back(tJ);
	  if(tJ.isBtag) nBtagJetsJECDown++; 
	  }
	}
      }
    }
    std::sort( selJetsJecDown.begin(), selJetsJecDown.end(), ByPt);
  }

  if (!gIsData  && gSelection == iTWSelec){
    nJet = Get<Int_t>("nJet");
    TLorentzVector diffMET(0,0,0,0);
    for(Int_t i = 0; i < nJet; i++){
      GetJetVariables(i);
      tJ = Jet(tpJ, csv, jetId, flavmc);
      GetmcJetVariables(i);
      tJ.SetMCjet(tpJ);
      tJ.isBtag = IsBtag(tJ);
      // Fill the vectors
      if(tJ.id > 0 && Cleaning(tJ, Leptons, minDR)){
	SetSystematics(&tJ);
	TLorentzVector tmp = tJ.p;
	tJ.p.SetPtEtaPhiE( getJetJERpt(tJ,rho), tJ.p.Eta(), tJ.p.Phi(), tJ.p.E() * getJetJERpt(tJ,rho) / tJ.p.Pt());
	diffMET += (-tmp + tJ.p);
	tJ.isBtag = IsBtag(tJ);
	if (TMath::Abs(tJ.p.Eta()) < jet_MaxEta){
	  if(tJ.p.Pt() > jet_MinPt){
	  selJetsJER.push_back(tJ);
	  if(tJ.isBtag) nBtagJetsJER++; 
	  }
	}
      }
    }
    
    SetParam("diff_MET_JER_pt" , Float_t(diffMET.Pt()) );
    SetParam("diff_MET_JER_phi" , Float_t(diffMET.Phi()) );
    std::sort( selJetsJER.begin(), selJetsJER.end(), ByPt);
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
  SetParam("selJetsJecUp", selJetsJecUp);
  SetParam("selJetsJecDown", selJetsJecDown);
  SetParam("selJetsJER" , selJetsJER);
  SetParam("Jets15",   Jets15);
  SetParam("vetoJets", vetoJets);
  SetParam("genJets",  genJets);
  SetParam("mcJets",  mcJets);
  SetParam("nSelJets",  nSelJets);
  SetParam("nJets15",  nJets15);
  SetParam("nVetoJets",  nVetoJets);
  SetParam("nGenJets",  nGenJets);
  SetParam("nSelBJets",  nBtagJets);
  SetParam("nSelBJetsJECUp",  nBtagJetsJECUp);
  SetParam("nSelBJetsJECDown",  nBtagJetsJECDown);
  SetParam("BtagSF"          , BtagSF          );
  SetParam("BtagSFBtagUp"    , BtagSFBtagUp    );
  SetParam("BtagSFBtagDown"  , BtagSFBtagDown  );
  SetParam("BtagSFMistagUp"  , BtagSFMistagUp  );
  SetParam("BtagSFMistagDown", BtagSFMistagDown);


  // Propagate JES to MET
  Float_t met_pt  = Get<Float_t>("met_pt");
  Float_t met_phi = Get<Float_t>("met_phi");
  MET_JESUp   = met_pt;
  MET_JESDown = met_pt;
  if(nSelJets > 0){
    MET_JESUp   = JEStoMET(selJets, met_pt, met_phi,  1);
    MET_JESDown = JEStoMET(selJets, met_pt, met_phi, -1);
  }
  SetParam("MET_JESUp",   MET_JESUp);
  SetParam("MET_JESDown", MET_JESDown);


}

Bool_t JetSelector::IsBtag(Jet j){
  if(j.Pt() < 20) return false;
  Bool_t isbtag;
  if(gIsData || gSelection == i4tSelec) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  // using "weights" as scale factors in the tW analysis :)
  else if(gSelection == iTWSelec) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  else if(stringWP == "Loose") isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  else                         isbtag = fBTagSFnom->IsTagged(j.csv,j.flavmc, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  return isbtag;
}

void JetSelector::SetSystematics(Jet *j){
  Float_t _csv = j->csv; Int_t _flavmc = j->flavmc; Float_t _pt = j->p.Pt(); Float_t _eta = j->p.Eta();
  if(gIsData) return;
  j->pTJESUp     = rawPt*pt_corrUp;
  j->pTJESDown   = rawPt*pt_corrDown;
  j->pTJERUp     = getJetJERpt(*j,rho);
  j->pTJERDown   = getJetJERpt(*j,rho);
  j->isBtag_BtagUp      = fBTagSFbUp->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt+1);
  j->isBtag_BtagDown    = fBTagSFbDo->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt-1);
  j->isBtag_MisTagUp    = fBTagSFlUp->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt+3);
  j->isBtag_MisTagDown  = fBTagSFlDo->IsTagged(_csv, _flavmc, _pt, _eta, evt+(UInt_t)_pt-3);
}

