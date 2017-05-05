#include "StopAnalysis.h"

ClassImp(StopAnalysis);
StopAnalysis::StopAnalysis() : PAFChainItemSelector() {
  fTree = 0;
  fSumISRJets = 0;  
  fISRJets = 0;  

  TrigSF = 0; TrigSF_Up = 0; TrigSF_Down = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0;
  gChannel = 0; passMETfilters = 0; passTrigger = 0; isSS = 0;  NormWeight = 0; TWeight = 0;
  TMT2 = 0; TMll = 0;  TMET = 0; TMET_Phi = 0; TgenMET = 0; TNJets = 0; TNBtags = 0; THT = 0; 
  TNVetoLeps = 0; TNSelLeps = 0; TChannel = 0; TDeltaPhi = 0; TDeltaEta = 0;
  TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJER = 0; TNBtagsJESUp = 0; TNBtagsJESDown = 0;
  TNBtagsUp = 0; TNBtagsDown = 0; TNBtagsMisTagUp = 0; TNBtagsMisTagDown = 0;
  THTJESUp = 0; THTJESDown = 0; TNISRJets = 0;
  TMETJESUp = 0; TMETJESDown = 0; TMT2llJESUp = 0; TMT2llJESDown = 0;
  TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
  TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;
  TIsSS = false; TNLHEWeight = 0; TISRweight = 0; 
  TLep0_Pt = 0; TLep0_Eta = 0; TLep0_Phi = 0; TLep0_E = 0; TLep0_Charge = 0;
  TLep1_Pt = 0; TLep1_Eta = 0; TLep1_Phi = 0; TLep1_E = 0; TLep1_Charge = 0;
  for(Int_t i = 0; i < 20; i++){
    TJet_Pt[i] = 0;
    TJet_Eta[i] = 0;
    TJet_Phi[i] = 0;
    TJet_E[i] = 0;
    TJet_isBJet[i] = 0;
    TJetJESUp_Pt[i] = 0;
    TJetJESDown_Pt[i] = 0;
    TJetJER_Pt[i] = 0;
  }
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;

}
void StopAnalysis::Summary(){}

void StopAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  if(gIsFastSim){
    gStopMass = GetParam<Int_t>("stopMass");
    gLspMass  = GetParam<Int_t>("lspMass");
  }
  fTree = CreateTree("tree","Created with PAF");
  fSumISRJets = CreateH1F("SumISRweights", "SumISRweights", 1 , 0, 2); 
  fISRJets =    CreateH1F(   "ISRweights",    "ISRweights", 6 , 0, 6); 

  normISRweight = 1;
  if(gSampleName.Contains("250_50_FS_summer")) normISRweight = 0.928859;
  if(gSampleName.Contains("225_50_FS_summer")) normISRweight = 0.926817;
  if(gSampleName.Contains("200_50_FS_summer")) normISRweight = 0.923371;

  gIsLHE = false;
  if(gSampleName.Contains("LHE")) gIsLHE = true;

  TNLHEWeight = 254; 
  SetLeptonVariables();
  SetJetVariables();
  SetEventVariables();

  selLeptons  = std::vector<Lepton>();
  vetoLeptons = std::vector<Lepton>();
  genLeptons = std::vector<Lepton>();
  selJets = std::vector<Jet>();
  Jets15  = std::vector<Jet>();
}


void StopAnalysis::InsideLoop(){
  if(gIsFastSim) if(fabs(fabs((Get<Int_t>("GenSusyMStop"))- gStopMass)) > 1 || fabs((fabs(Get<Int_t>("GenSusyMNeutralino")) - gLspMass)) > 1) return;
  // Vectors with the objects
  selLeptons.clear(); vetoLeptons.clear();
  genLeptons.clear();
  selJets.clear(); Jets15.clear();

  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  genLeptons = GetParam<vector<Lepton>>("genLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  Jets15      = GetParam<vector<Jet>>("Jets15");

  // Weights and SFs
  NormWeight = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSF_Up    = GetParam<Float_t>("TriggerSF_Up");
  TrigSF_Down  = GetParam<Float_t>("TriggerSF_Down");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");

  // Event variables
  gChannel        = GetParam<Int_t>("gChannel");
  passMETfilters = GetParam<Bool_t>("METfilters");
  passTrigger    = GetParam<Bool_t>("passTrigger");
  isSS           = GetParam<Bool_t>("isSS");

  // Leptons and Jets
  GetLeptonVariables(selLeptons, vetoLeptons);
  GetJetVariables(selJets, Jets15);
  GetMET();
  TISRweight = getISRJetsWeight(TNISRJets)/normISRweight;

  fSumISRJets->Fill(1, getISRJetsWeight(TNISRJets)); 

  GetGenInfo();

  if(TNSelLeps == 2 && passTrigger && passMETfilters){ // 2 leptons, OS
//  if(TNSelLeps == 2){ // 2 leptons, OS
//  if(TNSelLeps == 2 && passTrigger && passMETfilters && !isSS){ // 2 leptons, OS
    //if(TNVetoLeps < 3){  // Veto to 3rd lepton
    if(1){  // Veto to 3rd lepton

      // Deal with weights:
      Float_t lepSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
      Float_t lepSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      Float_t lepSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      TWeight            = NormWeight*lepSF*TrigSF*PUSF;
      TWeight_LepEffUp   = NormWeight*lepSFUp*TrigSF*PUSF;
      TWeight_LepEffDown = NormWeight*lepSFDo*TrigSF*PUSF;
      TWeight_TrigUp     = NormWeight*lepSF*TrigSF_Up*PUSF;
      TWeight_TrigDown   = NormWeight*lepSF*TrigSF_Down*PUSF;
      TWeight_PUDown     = NormWeight*lepSF*TrigSF*PUSF_Up;
      TWeight_PUUp       = NormWeight*lepSF*TrigSF*PUSF_Down;
      if(gIsData) TWeight = 1;

      // Event Selection
      // ===================================================================================================================
      if((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20 && selLeptons.at(0).p.Pt() > 25){ // mll > 20 GeV
        if(gChannel == iElMu || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  ){ //  Z Veto in ee, µµ
          if(TNJets > 1 || TNJetsJESUp > 1 || TNJetsJESDown > 1 || TNJetsJER > 1){ //At least 2 jets
            if(TNBtags > 0 || TNBtagsUp > 0 || TNBtagsDown > 0 || TNBtagsMisTagUp > 0 || TNBtagsMisTagDown > 0 || TNBtagsJESUp > 0 || TNBtagsJESDown > 0){ // At least 1 b-tag
              fTree->Fill();
              if(TNBtags > 0 && TNJets > 1) fISRJets->Fill(TNISRJets, TWeight);
            }
          }
        }
      }
    }
  }
}


//#####################################################################
// Functions
//------------------------------------------------------------------

void StopAnalysis::SetLeptonVariables(){
  fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
  fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  fTree->Branch("TLep0_Pt",     &TLep0_Pt,     "TLep0_Pt/F");
  fTree->Branch("TLep0_Eta",     &TLep0_Eta,     "TLep0_Eta/F");
  fTree->Branch("TLep0_Phi",     &TLep0_Phi,     "TLep0_Phi/F");
  fTree->Branch("TLep0_E" ,     &TLep0_E ,     "TLep0_E/F");
  fTree->Branch("TLep0_Charge", &TLep0_Charge, "TLep0_Charge/F");
  fTree->Branch("TLep1_Pt",     &TLep1_Pt,     "TLep1_Pt/F");
  fTree->Branch("TLep1_Eta",     &TLep1_Eta,     "TLep1_Eta/F");
  fTree->Branch("TLep1_Phi",     &TLep1_Phi,     "TLep1_Phi/F");
  fTree->Branch("TLep1_E" ,     &TLep1_E ,     "TLep1_E/F");
  fTree->Branch("TLep1_Charge",  &TLep1_Charge, "TLep1_Charge/F");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  fTree->Branch("TMll",      &TMll,      "TMll/F");
  fTree->Branch("TMT2",      &TMT2,      "TMT2/F");
  fTree->Branch("TDeltaPhi",      &TDeltaPhi,      "TDeltaPhi/F");
  fTree->Branch("TDeltaEta",      &TDeltaEta,      "TDeltaPhi/F");
}

void StopAnalysis::SetJetVariables(){
  fTree->Branch("TNJets",        &TNJets,         "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
  fTree->Branch("TJet_isBJet",   TJet_isBJet,       "TJet_isBJet[TNJets]/I");
  fTree->Branch("TJet_Pt",       TJet_Pt,           "TJet_Pt[TNJets]/F");
  fTree->Branch("TJet_Eta",      TJet_Eta,           "TJet_Eta[TNJets]/F");
  fTree->Branch("TJet_Phi",      TJet_Phi,           "TJet_Phi[TNJets]/F");
  fTree->Branch("TJet_E",        TJet_E,            "TJet_E[TNJets]/F");
  fTree->Branch("THT",          &THT,          "THT/F");

  if(gIsData) return;
  fTree->Branch("TNJetsJESUp",      &TNJetsJESUp,         "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",    &TNJetsJESDown,         "TNJetsJESDown/I");
  fTree->Branch("TNBtagsJESUp",      &TNBtagsJESUp,         "TNBtagsJESUp/I");
  fTree->Branch("TNBtagsJESDown",    &TNBtagsJESDown,         "TNBtagsJESDown/I");
  fTree->Branch("TNJetsJER",        &TNJetsJER,         "TNJetsJER/I");

  fTree->Branch("TNBtagsUp",           &TNBtagsUp,   "TNBtagsUp/I");
  fTree->Branch("TNBtagsDown",         &TNBtagsDown, "TNBtagsDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");
}

void StopAnalysis::SetEventVariables(){
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  fTree->Branch("TMET",         &TMET,         "TMET/F");
  fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  fTree->Branch("TIsSS",        &TIsSS,        "TIsSS/B");  

  if(gIsData) return;
  fTree->Branch("TgenMET",         &TgenMET,         "TgenMET/F");
  fTree->Branch("TISRweight",      &TISRweight,      "TISRweight/F");
  fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");

  //fTree->Branch("TNLHEWeight",        &TNLHEWeight,         "TNLHEWeight/I");
  fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
  fTree->Branch("TMT2llJESUp",    &TMT2llJESUp,    "TMT2llJESUp/F");
  fTree->Branch("TMT2llJESDown",  &TMT2llJESDown,  "TMT2llJESDown/F");

  fTree->Branch("TgenMETPhi"  , &TgenMETPhi , "TgenMETPhi/F" ); 
  fTree->Branch("TgenTop1Pt"  , &TgenTop1Pt , "TgenTop1Pt/F" );
  fTree->Branch("TgenTop1Eta" , &TgenTop1Eta, "TgenTop1Eta/F"); 
  fTree->Branch("TgenTop1Phi" , &TgenTop1Phi, "TgenTop1Phi/F"); 
  fTree->Branch("TgenTop1M"   , &TgenTop1M  , "TgenTop1M/F"  ); 
  fTree->Branch("TgenTop2Pt"  , &TgenTop2Pt , "TgenTop2Pt/F" ); 
  fTree->Branch("TgenTop2Eta" , &TgenTop2Eta, "TgenTop2Eta/F"); 
  fTree->Branch("TgenTop2Phi" , &TgenTop2Phi, "TgenTop2Phi/F"); 
  fTree->Branch("TgenTop2M"   , &TgenTop2M  , "TgenTop2M/F"  ); 

}

void StopAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
  TNSelLeps = selLeptons.size();
  Int_t nVetoLeptons = VetoLeptons.size();
  TNVetoLeps = (nVetoLeptons == 0) ? TNSelLeps : nVetoLeptons;
  if(TNSelLeps < 2) TChannel = -1;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) TChannel = iElMu;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) TChannel = iElMu;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) TChannel = iMuon;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) TChannel = iElec;
  Int_t Index0 = 0; Int_t Index1 = 1;
  if(TChannel == iElMu && selLeptons.at(0).isElec){ Index1 = 0; Index0 = 1;};

  if(TNSelLeps > 0){
    TLep0_Pt     = selLeptons.at(0).Pt();    
    TLep0_Eta    = selLeptons.at(0).Eta();    
    TLep0_Phi    = selLeptons.at(0).Phi();    
    TLep0_E      = selLeptons.at(0).E();    
    TLep0_Charge = selLeptons.at(0).charge;    
  }
  if(TNSelLeps > 1){
    TLep1_Pt     = selLeptons.at(1).Pt();    
    TLep1_Eta    = selLeptons.at(1).Eta();    
    TLep1_Phi    = selLeptons.at(1).Phi();    
    TLep1_E      = selLeptons.at(1).E();    
    TLep1_Charge = selLeptons.at(1).charge;    
    TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();      
    TDeltaPhi = (selLeptons.at(0).p.DeltaPhi(selLeptons.at(1).p));
    TDeltaEta = TMath::Abs(TMath::Abs(selLeptons.at(0).p.Eta()) - TMath::Abs(selLeptons.at(1).p.Eta()));
  }
}

void StopAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNISRJets = Int_t(Get<Float_t>("nISRJet30"));
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0; TNBtagsUp = 0; TNBtagsDown = 0;
  TNBtagsMisTagUp = 0;  TNBtagsMisTagDown = 0;
  for(Int_t i = 0; i < 20; i++){
    if(i < TNJets){
      TJet_Pt[i]     = selJets.at(i).Pt();
      TJet_Eta[i]    = selJets.at(i).Eta();
      TJet_Phi[i]    = selJets.at(i).Phi();
      TJet_E[i]      = selJets.at(i).E();
      TJet_isBJet[i] = selJets.at(i).isBtag;
      if(selJets.at(i).isBtag)            TNBtags++;
    }
    else{
      TJet_Pt[i]     = 0;
      TJet_Eta[i]    = 0;
      TJet_Phi[i]    = 0;
      TJet_E[i]      = 0;
      TJet_isBJet[i] = 0;
    }
    THT += TJet_Pt[i];
  }
  if(gIsData) return;  // For systematics...
  for(Int_t i = 0; i < TNJets; i++){
    if(selJets.at(i).isBtag_BtagUp    ) TNBtagsUp++;
    if(selJets.at(i).isBtag_BtagDown  ) TNBtagsDown++;
    if(selJets.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
    if(selJets.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
  }
  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNBtagsJESUp    = 0;
  TNBtagsJESDown  = 0;
  TNJetsJER      = 0;  
  THTJESUp = 0; THTJESDown = 0;
  for(Int_t i = 0; i < (Int_t) cleanedJets15.size(); i++){
    if(cleanedJets15.at(i).pTJESUp > ptCut){
      TNJetsJESUp++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESUp++;
      TJetJESUp_Pt[i] = cleanedJets15.at(i).pTJESUp;
      THTJESUp += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJESDown > ptCut){
      TNJetsJESDown++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESDown++;
      TJetJESDown_Pt[i] = cleanedJets15.at(i).pTJESDown;
      THTJESDown += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJERUp > ptCut){
      TNJetsJER++;
      TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
    }
  }
}

void StopAnalysis::GetMET(){
  TMET        = Get<Float_t>("met_pt");
  TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
  TIsSS       = isSS;
  if(TNSelLeps>1)  TMT2 = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET,        TMET_Phi);
  if(gIsData) return;
  TgenMET     = Get<Float_t>("met_genPt");
  TMETJESUp   = GetParam<Float_t>("MET_JESUp");
  TMETJESDown = GetParam<Float_t>("MET_JESDown");
  if(TNSelLeps > 1){
    TMT2llJESUp   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESUp,   TMET_Phi);
    TMT2llJESDown = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESDown, TMET_Phi);
  }
  if(gIsLHE) for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}


void StopAnalysis::GetGenInfo(){

  TgenMETPhi = Get<Float_t>("met_genPhi");

  TgenTop1Pt  = -1;   TgenTop2Pt  = -1;
  TgenTop1Eta = -1;   TgenTop2Eta = -1;
  TgenTop1Phi = -1;   TgenTop2Phi = -1;
  TgenTop1M   = -1;   TgenTop2M   = -1;
  
  if (Get<Int_t>("nGenTop") > 1){
    TgenTop1Pt  = Get<Float_t>("GenTop_pt"  , 0);   TgenTop2Pt  = Get<Float_t>("GenTop_pt"  , 1);
    TgenTop1Eta = Get<Float_t>("GenTop_eta" , 0);   TgenTop2Eta = Get<Float_t>("GenTop_eta" , 1);
    TgenTop1Phi = Get<Float_t>("GenTop_phi" , 0);   TgenTop2Phi = Get<Float_t>("GenTop_phi" , 1);
    TgenTop1M   = Get<Float_t>("GenTop_mass", 0);   TgenTop2M   = Get<Float_t>("GenTop_mass", 1);
  }

  return; 
  

}
