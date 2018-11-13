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
  BtagSFFS = 1;
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
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  gSampleName  = GetParam<TString>("sampleName");
  gIs2017eccop = false;
  gOptions     = GetParam<TString>("_options");
  if(gOptions.Contains("2017eccop")) gIs2017eccop = true;

  gIsFSRUp = false; gIsFSRDown = false;
  if     (gSampleName.Contains("fsrUp"))   gIsFSRUp = true;
  else if(gSampleName.Contains("fsrDown")) gIsFSRDown = true;

  //---- Select your wp for b-tagging and pt, eta for the jets
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iStopTopSelec || gSelection == ittDMSelec){
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 30;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
    if(gIs2017eccop){
      taggerName=="CSVv2_94XSF_V1_B_F"; // DeepCSV_94XSF_V1_B_F
    }
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
  else if (gSelection == iWZSelec){
    taggerName="CSVv2"; // Soon to be: taggerName="DeepCSV" 
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 40;
    vetoJet_minPt = 25;
    vetoJet_maxEta = 2.4;
    minDR = 0.4;
  }
  else if (gSelection == iTWSelec || gSelection == iTWTTbarSelec){
    taggerName="CSVv2";
    stringWP = "Medium";
    jet_MaxEta = 2.4;
    jet_MinPt  = 30;
    vetoJet_minPt = 15.;
    vetoJet_maxEta = 4.7;
    minDR = 0.4;
  }
  else if(gSelection == iWWSelec || gSelection == iHWWSelec){
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
    stringWP = "Medium";
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

  if (gSelection == iTopSelec || gSelection == iStopTopSelec || gSelection == iTWSelec || gSelection == iTWTTbarSelec) MeasType = "mujets";
  TString pwd  = GetParam<TString>("WorkingDir");
  TString BTagSFPath = Form("%s/packages/BTagSFUtil", pwd.Data());
  
  TString FastSimDataset = "";
  if(gIsFastSim) FastSimDataset = "1";
  fBTagSFnom = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  0, FastSimDataset);
  fBTagSFbUp = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  1, FastSimDataset);
  fBTagSFbDo = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP, -1, FastSimDataset);
  fBTagSFlUp = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP,  3, FastSimDataset);
  fBTagSFlDo = new BTagSFUtil(MeasType, BTagSFPath, taggerName.Data(), stringWP, -3, FastSimDataset);

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
  //tpJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_px",i), Get<Float_t>("Jet"+jec+"_py",i), Get<Float_t>("Jet"+jec+"_pz", i), Get<Float_t>("Jet"+jec+"_energy",i));
  Float_t FSRSF = 1;
  if(gIsFSRUp)   FSRSF = GetFSR_JECSF_Up(  Get<Float_t>("Jet"+jec+"_pt",i));
  if(gIsFSRDown) FSRSF = GetFSR_JECSF_Down(Get<Float_t>("Jet"+jec+"_pt",i));
  tpJ.SetPtEtaPhiM(1/FSRSF*Get<Float_t>("Jet"+jec+"_pt",i), Get<Float_t>("Jet"+jec+"_eta",i), Get<Float_t>("Jet"+jec+"_phi", i), Get<Float_t>("Jet"+jec+"_mass",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = Get<Float_t>("Jet"+jec+"_rawPt",i)/FSRSF;
  pt_corrUp   = Get<Float_t>("Jet"+jec+"_corr_JECUp",i); 
  pt_corrDown = Get<Float_t>("Jet"+jec+"_corr_JECDown",i);
  jetId       = Get<Int_t>("Jet"+jec+"_id",i);
  csv         = Get<Float_t>("Jet"+jec+"_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("Jet"+jec+"_hadronFlavour", i);
    if(gIs2017eccop) tmcJ.SetPtEtaPhiM(Get<Float_t>("Jet_mcPt", i), Get<Float_t>("Jet"+jec+"_eta",i), Get<Float_t>("Jet"+jec+"_phi", i), Get<Float_t>("Jet"+jec+"_mass",i));
    else tmcJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_mcPx",i), Get<Float_t>("Jet"+jec+"_mcPy",i), Get<Float_t>("Jet"+jec+"_mcPz",i), Get<Float_t>("Jet"+jec+"_mcEnergy",i));
  }
}

void JetSelector::GetDiscJetVariables(Int_t i){
  tpJ.SetPtEtaPhiM(Get<Float_t>("DiscJet_pt",i), Get<Float_t>("DiscJet_eta",i), Get<Float_t>("DiscJet_phi", i), Get<Float_t>("DiscJet_mass",i));
  eta = tpJ.Eta();;
  pt = tpJ.Pt();
  rawPt       = gIs2017eccop? Get<Float_t>("DiscJet_pt",i) : Get<Float_t>("DiscJet_rawPt",i);
  pt_corrUp   = gIs2017eccop? Get<Float_t>("DiscJet_pt",i) : Get<Float_t>("DiscJet_corr_JECUp",i); 
  pt_corrDown = gIs2017eccop? Get<Float_t>("DiscJet_pt",i) : Get<Float_t>("DiscJet_corr_JECDown",i);
  jetId       = gIs2017eccop? 1 : Get<Int_t>("DiscJet_id",i);
  csv         = Get<Float_t>("DiscJet_btagCSV", i);
  flavmc = -999999;
  if(!gIsData){
    flavmc = Get<Int_t>("DiscJet_mcFlavour", i);  // Get<Int_t>("DiscJet_hadronFlavour", i);
    if(!gIs2017eccop) tmcJ.SetPxPyPzE(Get<Float_t>("DiscJet_mcPx",i), Get<Float_t>("DiscJet_mcPy",i), Get<Float_t>("DiscJet_mcPz",i), Get<Float_t>("DiscJet_mcEnergy",i));
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
  if(gIs2017eccop) tpJ.SetPtEtaPhiM(Get<Float_t>("Jet_mcPt", i), Get<Float_t>("Jet"+jec+"_eta",i), Get<Float_t>("Jet"+jec+"_phi", i), Get<Float_t>("Jet"+jec+"_mass",i));
  else tpJ.SetPxPyPzE(Get<Float_t>("Jet"+jec+"_mcPx",i), Get<Float_t>("Jet"+jec+"_mcPy",i), Get<Float_t>("Jet"+jec+"_mcPz", i), Get<Float_t>("Jet"+jec+"_mcEnergy",i));
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

  BtagSFFS         = 1.;
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

  Float_t mcTag     = 1.;
  Float_t dataTag   = 1.;
  Float_t mcNoTag   = 1.;
  Float_t dataNoTag = 1.;
  Float_t errHup  = 0.;
  Float_t errHdn  = 0.;
  Float_t errLup  = 0.;
  Float_t errLdn  = 0.;

  
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
        if      (gSelection == iWWSelec || gSelection == iHWWSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iWZSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == i4tSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iWZSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iTopSelec){if (tJ.isBtag) vetoJets.push_back(tJ);}
        else if (gSelection == iTWSelec || gSelection == iTWTTbarSelec){
          vetoJets.push_back(tJ);
          if (!gIsData){
            if ( tJ.p.Pt() < 20.) continue;
            Float_t eff   = fBTagSFnom->JetTagEfficiency( tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
            Float_t sf    = fBTagSFnom->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
            Float_t sfHUp = fBTagSFbUp->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());	    
            Float_t sfHDn = fBTagSFbDo->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
            Float_t sfLUp = fBTagSFlUp->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());
            Float_t sfLDn = fBTagSFlDo->GetJetSF(tJ.csv, tJ.flavmc, tJ.p.Pt(), tJ.p.Eta());

            if (tJ.isBtag){

              mcTag   *= eff;
              dataTag *= eff * sf;

              if (tJ.flavmc == 5 || tJ.flavmc == 4){
                errHup += (sfHUp - sf ) / sf;
                errHdn += (sf - sfHDn ) / sf;
              }
              else{
                errLup += (sfLUp - sf ) / sf;
                errLdn += (sf - sfLDn ) / sf;
              }
            }
            else{
              mcNoTag   *= ( 1 - eff    );
              dataNoTag *= ( 1 - eff*sf );
              if (tJ.flavmc == 5 || tJ.flavmc == 4){
                errHup -= eff*(sfHUp - sf ) / (1 - eff*sf);
                errHdn -= eff*(sf - sfHDn ) / (1 - eff*sf);
              }
              else{
                errLup -= eff*(sfLUp - sf ) / (1 - eff*sf);
                errLdn -= eff*(sf - sfLDn ) / (1 - eff*sf);
              }

            }
          }
        }
        else  vetoJets.push_back(tJ);
      }
    }
  }

  BtagSF           *= (dataNoTag * dataTag) / (mcNoTag * mcTag);
  BtagSFBtagUp     *= BtagSF * ( 1 + errHup );
  BtagSFBtagDown   *= BtagSF * ( 1 - errHdn );
  BtagSFMistagUp   *= BtagSF * ( 1 + errLup );
  BtagSFMistagDown *= BtagSF * ( 1 - errLdn );
  

  // If stop, disc
  if(gSelection == iStopSelec || gSelection == iStopTopSelec || gSelection == iTopSelec){
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
              TLorentzVector DisctpJ;
              DisctpJ.SetPxPyPzE(Get<Float_t>("DiscJet_mcPx",i), Get<Float_t>("DiscJet_mcPy",i), Get<Float_t>("DiscJet_mcPz",i), Get<Float_t>("DiscJet_mcEnergy",i));
              tJ.SetMCjet(DisctpJ);
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
  if (!gIsData && (gSelection == iTWSelec || gSelection == iTWTTbarSelec)){
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
  if (!gIsData  && (gSelection == iTWSelec || gSelection == iTWTTbarSelec)){
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

  if (!gIsData  && (gSelection == iTWSelec || gSelection == iTWTTbarSelec)){
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
    if(!gIs2017eccop) ngenJet = Get<Int_t>("ngenJet");
    else ngenJet = Get<Int_t>("nJet");
    for(Int_t i = 0; i < ngenJet; i++){
      if(!gIs2017eccop) GetGenJetVariables(i);
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
  SetParam("BtagSFFS"        , BtagSFFS        );


  // Propagate JES to MET
  Float_t met_pt  = Get<Float_t>("met_pt");
  Float_t met_phi = Get<Float_t>("met_phi");
  MET_JERUp   = met_pt;
  MET_JESUp   = met_pt;
  MET_JESDown = met_pt;
  if(nSelJets > 0){
    MET_JERUp   = JERtoMET(selJets, met_pt, met_phi);
    MET_JESUp   = JEStoMET(selJets, met_pt, met_phi,  1);
    MET_JESDown = JEStoMET(selJets, met_pt, met_phi, -1);
  }
  SetParam("MET_JERUp",   MET_JERUp);
  SetParam("MET_JESUp",   MET_JESUp);
  SetParam("MET_JESDown", MET_JESDown);


}

Bool_t JetSelector::IsBtag(Jet j){
  if(j.Pt() < 20) return false;
  Bool_t isbtag;
  if(gIsData || gSelection == i4tSelec || gSelection == iWZSelec) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  // using "weights" as scale factors in the tW analysis :)
  else if(gSelection == iTWSelec || gSelection == iTWTTbarSelec) isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  //else if(stringWP == "Loose") isbtag = fBTagSFnom->IsTagged(j.csv, -999999, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  else                         isbtag = fBTagSFnom->IsTagged(j.csv,j.flavmc, j.p.Pt(), j.p.Eta(), evt+(UInt_t)j.p.Pt());
  if(gIsFastSim && BtagSFFS == 1. && isbtag){  
    BtagSFFS = fBTagSFnom->GetFastSimBtagSF(j.flavmc, j.p.Eta(), j.p.Pt(), j.csv);
    //cout << "BtagSFFS = " << BtagSFFS << endl;
  }
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

