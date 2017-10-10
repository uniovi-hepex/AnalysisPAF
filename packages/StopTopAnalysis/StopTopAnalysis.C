#include "StopTopAnalysis.h"

ClassImp(StopTopAnalysis);
StopTopAnalysis::StopTopAnalysis() : PAFChainItemSelector() {
  fTree = 0;
  fSumISRJets = 0;  
  fISRJets = 0;  
  ResetVariables();
  IsT2ttScan = false;
}

void StopTopAnalysis::Summary(){}

void StopTopAnalysis::ResetVariables(){
  TrigSF = 0; TrigSFerr = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0; genWeight = 0;
  gChannel = 0; passMETfilters = 0; passTrigger = 0; isSS = 0;  NormWeight = 0; TWeight = 0;
  TMT2 = 0; TMll = 0;  TPfMET = 0; TMET = 0; TMET_Phi = 0; TgenMET = 0; TNJets = 0; TNBtags = 0; THT = 0; 
  TNVetoLeps = 0; TNSelLeps = 0; TChannel = 0; TDeltaPhi = 0; TDeltaEta = 0; TBtagSFFS = 0;
  TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJERUp = 0; TNBtagsJESUp = 0; TNBtagsJESDown = 0;
  TNBtagsBtagUp = 0; TNBtagsBtagDown = 0; TNBtagsMisTagUp = 0; TNBtagsMisTagDown = 0;
  THTJESUp = 0; THTJESDown = 0; TNISRJets = 0; TNVert = 0; FSSF = 0; FSSFerr = 0;
  TMETJERUp = 0; TMETJESUp = 0; TMETJESDown = 0; TMT2JERUp = 0; TMT2JESUp = 0; TMT2JESDown = 0;
  TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
  TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;
  TIsSS = false; TNLHEWeight = 0; TISRweight = 0; TISRweightUp = 0; TISRweightDown = 0; TWeight_ISRUp = 0; TWeight_ISRDown = 0;
  TLep0_Pt = 0; TLep0_Eta = 0; TLep0_Phi = 0; TLep0_E = 0; TLep0_Charge = 0; TLep0_Id = 0; TgenLep0_Mid = 0; 
  TLep1_Pt = 0; TLep1_Eta = 0; TLep1_Phi = 0; TLep1_E = 0; TLep1_Charge = 0; TLep1_Id = 0; TgenLep1_Mid = 0;
  TgenMETPhi = 0; TNgenLeps = 0;
  TgenLep0_Pt = 0; TgenLep0_Eta = 0; TgenLep1_Pt = 0; TgenLep1_Eta = 0; TgenLep0_Id = 0; TgenLep1_Id = 0;
  TgenDeltaPhi = 0; TgenDeltaEta = 0; TgenMT2 = 0; TNgenJets = 0;  TgenHT = 0;
  TgentopDeltaPhi = 0; TgentopDeltaR = 0;
  TMETMETDown = 0; TMETMETUp = 0; TMT2METUp = 0; TMT2METDown = 0;
  TPassTrigger = 0; TPassMETfilters = 0;
  for(Int_t i = 0; i < 20; i++){
    TJet_Pt[i] = 0;
    TJet_Eta[i] = 0;
    TJet_Phi[i] = 0;
    TJet_E[i] = 0;
    TJet_isBJet[i] = 0;
    TJetJESUp_Pt[i] = 0;
    TJetJESDown_Pt[i] = 0;
    TJetJER_Pt[i] = 0;
    TgenJet_Pt[i] = 0;
    TgenJet_Eta[i] = 0;
  }
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
  TgenTop1Pt  = 0;   TgenTop2Pt  = 0;
  TgenTop1Eta = 0;   TgenTop2Eta = 0;
  TgenTop1Phi = 0;   TgenTop2Phi = 0;
  TgenTop1M   = 0;   TgenTop2M   = 0;
  TgenNeu1Pt  = 0; TgenNeu2Pt  = 0;
  TgenNeu1Eta = 0; TgenNeu2Eta = 0;
  TgenNeu1Phi = 0; TgenNeu2Phi = 0;
  TgenNeu1M   = 0; TgenNeu2M   = 0;
  TgenW1Pt  = 0; TgenW2Pt  = 0;
  TgenW1Eta = 0; TgenW2Eta = 0;
  TgenW1Phi = 0; TgenW2Phi = 0;
  TgenW1M   = 0; TgenW2M   = 0;
  TgenStop1Pt  = 0; TgenStop2Pt  = 0;
  TgenStop1Eta = 0; TgenStop2Eta = 0;
  TgenStop1Phi = 0; TgenStop2Phi = 0;
  TgenStop1M   = 0; TgenStop2M   = 0;
  TgenLsp1Pt  = 0; TgenLsp2Pt  = 0;
  TgenLsp1Eta = 0; TgenLsp2Eta = 0;
  TgenLsp1Phi = 0; TgenLsp2Phi = 0;
  TgenLsp1M   = 0; TgenLsp2M   = 0;

  selLeptons.clear(); vetoLeptons.clear();
  genLeptons.clear();
  selJets.clear(); Jets15.clear();
  genJets.clear();
}

void StopTopAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  if(gIsFastSim){
    gStopMass = GetParam<Int_t>("stopMass");
    gLspMass  = GetParam<Int_t>("lspMass");
    NormISRweights = GetParam<Float_t>("NormISRweights");
    if(gSampleName.Contains("T2tt")) IsT2ttScan = true;
  }
  else if(gSampleName.BeginsWith("T2tt")) NormISRweights = GetParam<Float_t>("NormISRweights");
  fTree = CreateTree("tree","Created with PAF");
  fSumISRJets = CreateH1F("SumISRweights", "SumISRweights", 1 , 0, 2); 
  fISRJets =    CreateH1F(   "ISRweights",    "ISRweights", 6 , 0, 6); 

  gIsLHE = false; gIsTTbar = false;
  if(gSampleName == "TTbar_Powheg") gIsLHE = true;
  if(gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  
  gDoISR = false;
  if(gSampleName.Contains("T2tt")) gDoISR = true;

  TNLHEWeight = 254; 
  SetLeptonVariables();
  SetJetVariables();
  SetEventVariables();

  selLeptons  = std::vector<Lepton>();
  vetoLeptons = std::vector<Lepton>();
  genLeptons = std::vector<Lepton>();
  selJets = std::vector<Jet>();
  Jets15  = std::vector<Jet>();
  genJets = std::vector<Jet>();
}


void StopTopAnalysis::InsideLoop(){
  if(IsT2ttScan) if(fabs(fabs((Get<Int_t>("GenSusyMStop"))- gStopMass)) > 1 || fabs((fabs(Get<Int_t>("GenSusyMNeutralino")) - gLspMass)) > 1) return;

  ResetVariables();

  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  genJets     = GetParam<vector<Jet>>("genJets");
  if(gIsTTbar){
    //if(genLeptons.size() < 2) return;
  }

  // Weights and SFs
  NormWeight   = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSFerr    = GetParam<Float_t>("TriggerSFerr");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");
  BtagSFFS     = GetParam<Float_t>("BtagSFFS");
  FSSF         = GetParam<Float_t>("FSSF");
  FSSFerr      = GetParam<Float_t>("FSSFerr");
  gIsMCatNLO   = GetParam<Bool_t>("IsMCatNLO");

  if(gIsMCatNLO) genWeight = Get<Float_t>("genWeight");
  else           genWeight = 1;

  // Event variables
  gChannel        = GetParam<Int_t>("gChannel");
  passMETfilters  = GetParam<Bool_t>("METfilters");
  passTrigger     = GetParam<Bool_t>("passTrigger");
  isSS            = GetParam<Bool_t>("isSS");

  // Leptons and Jets
  GetLeptonVariables(selLeptons, vetoLeptons);
  GetJetVariables(selJets, Jets15);
  // -----------> hdamp casca después de esto!
  GetMET();
  // -----------> hdamp casca antes de esto!
  GetGenInfo();

  fSumISRJets->Fill(1, getISRJetsWeight(TNISRJets)); 

  //if(TNSelLeps >= 1){
  //  TChannel = iElec;
  //  if(selLeptons.at(0).isMuon) TChannel = iMuon;
  //  fTree->Fill();
  //  return;
  //}
  //else return;

  // =================== Tree at gen level here! ==================
  if(TNgenLeps >= 2){
    TChannel = GetDileptonicChannel(genLeptons);
    fTree->Fill();
  }
  return;

  // =================== Tree at reco level here! ==================
  if(TNSelLeps == 2){ // 2 leptons, OS
    // TPassTrigger && TPassMETfilters && !TIsSS
     
    Float_t ElecSF = 1; Float_t MuonSF = 1;
    Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
    if(TNSelLeps >= 2){
      //lepSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
      //lepSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      //lepSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      if(TChannel == iElec){
        ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
        ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
        ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
      }
      else if(TChannel == iMuon){
        MuonSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
        MuonSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
        MuonSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
        ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
      }
      else{
        if(selLeptons.at(0).isMuon){
          MuonSF   *= selLeptons.at(0).GetSF( 0);
          MuonSFUp *= selLeptons.at(0).GetSF( 1);
          MuonSFDo *= selLeptons.at(0).GetSF(-1);
        }
        else{
          ElecSF   *= selLeptons.at(0).GetSF( 0);
          ElecSFUp *= selLeptons.at(0).GetSF( 1);
          ElecSFDo *= selLeptons.at(0).GetSF(-1);
        }
        if(selLeptons.at(1).isMuon){
          MuonSF   *= selLeptons.at(1).GetSF( 0);
          MuonSFUp *= selLeptons.at(1).GetSF( 1);
          MuonSFDo *= selLeptons.at(1).GetSF(-1);
        }
        else{
          ElecSF   *= selLeptons.at(1).GetSF( 0);
          ElecSFUp *= selLeptons.at(1).GetSF( 1);
          ElecSFDo *= selLeptons.at(1).GetSF(-1);
        }
      }
      if(gIsFastSim){
        //TrigSF *= 0.98; // trigg efficiency for FastSim
        PUSF = 1;
        PUSF_Up  = 1;
        PUSF_Down = 1;
      }
      else FSSF = 1;
      Float_t MuonSF_diffUp   = MuonSFUp - MuonSF;
      Float_t MuonSF_diffDown = MuonSF - MuonSFDo;
      MuonSFUp = MuonSF + TMath::Sqrt(0.005*0.005+0.005*0.005 + 0.01*0.01 + MuonSF_diffUp*MuonSF_diffUp);
      MuonSFDo = MuonSF + TMath::Sqrt(0.005*0.005+0.005*0.005 + 0.01*0.01 + MuonSF_diffDown*MuonSF_diffDown);
      TWeight             = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*FSSF;
      TWeight_ElecEffUp   = NormWeight*ElecSFUp*MuonSF*TrigSF*PUSF*FSSF;
      TWeight_ElecEffDown = NormWeight*ElecSFDo*MuonSF*TrigSF*PUSF*FSSF;
      TWeight_MuonEffUp   = NormWeight*ElecSF*MuonSFUp*TrigSF*PUSF*FSSF;
      TWeight_MuonEffDown = NormWeight*ElecSF*MuonSFDo*TrigSF*PUSF*FSSF;
      TWeight_TrigUp     = NormWeight*ElecSF*MuonSF*(TrigSF+TrigSFerr)*PUSF;
      TWeight_TrigDown   = NormWeight*ElecSF*MuonSF*(TrigSF-TrigSFerr)*PUSF;
      TWeight_FSUp     = NormWeight*ElecSF*MuonSF*TrigSF*(FSSF+FSSFerr)*PUSF*FSSF;
      TWeight_FSDown   = NormWeight*ElecSF*MuonSF*TrigSF*(FSSF-FSSFerr)*PUSF*FSSF;
      TWeight_PUDown     = NormWeight*ElecSF*MuonSF*TrigSF*PUSF_Up*FSSF;
      TWeight_PUUp       = NormWeight*ElecSF*MuonSF*TrigSF*PUSF_Down*FSSF;
      TWeight_ISRUp      = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*TISRweightUp*FSSF;
      TWeight_ISRDown    = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*TISRweightDown*FSSF;
      if(gIsData) TWeight = 1;

      // Event Selection
      // ===================================================================================================================
      if((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20 && selLeptons.at(0).p.Pt() > 25){ // mll > 20 GeV
        if(gChannel == iElMu || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  ){ //  Z Veto in ee, µµ
          //if(TNJets > 1 || TNJetsJESUp > 1 || TNJetsJESDown > 1 || TNJetsJERUp > 1){ //At least 2 jets
          // if(TNBtags > 0 || TNBtagsBtagUp > 0 || TNBtagsBtagDown > 0 || TNBtagsMisTagUp > 0 || TNBtagsMisTagDown > 0 || TNBtagsJESUp > 0 || TNBtagsJESDown > 0){ // At least 1 b-tag
          fTree->Fill();
          // }
          // }
        }
      }
    }
  }
  //if(TNgenLeps >= 2) fTree->Fill();
}


//#####################################################################
// Functions
//------------------------------------------------------------------

void StopTopAnalysis::SetLeptonVariables(){
  fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
  fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  fTree->Branch("TLep0_Pt",     &TLep0_Pt,     "TLep0_Pt/F");
  fTree->Branch("TLep0_Eta",     &TLep0_Eta,     "TLep0_Eta/F");
  fTree->Branch("TLep0_Phi",     &TLep0_Phi,     "TLep0_Phi/F");
  fTree->Branch("TLep0_E" ,     &TLep0_E ,     "TLep0_E/F");
  fTree->Branch("TLep0_Id" ,     &TLep0_Id ,     "TLep0_Id/I");
  fTree->Branch("TLep0_Charge", &TLep0_Charge, "TLep0_Charge/F");
  fTree->Branch("TLep1_Pt",     &TLep1_Pt,     "TLep1_Pt/F");
  fTree->Branch("TLep1_Eta",     &TLep1_Eta,     "TLep1_Eta/F");
  fTree->Branch("TLep1_Phi",     &TLep1_Phi,     "TLep1_Phi/F");
  fTree->Branch("TLep1_E" ,     &TLep1_E ,     "TLep1_E/F");
  fTree->Branch("TLep1_Charge",  &TLep1_Charge, "TLep1_Charge/F");
  fTree->Branch("TLep1_Id" ,     &TLep1_Id ,     "TLep1_Id/I");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  fTree->Branch("TPassTrigger",  &TPassTrigger,  "TPassTrigger/I");
  fTree->Branch("TPassMETfilters",  &TPassMETfilters,  "TPassMETfilters/I");
  fTree->Branch("TMll",      &TMll,      "TMll/F");
  fTree->Branch("TMT2",      &TMT2,      "TMT2/F");
  fTree->Branch("TDeltaPhi",      &TDeltaPhi,      "TDeltaPhi/F");
  fTree->Branch("TDeltaEta",      &TDeltaEta,      "TDeltaPhi/F");

  if(gIsData) return;
  fTree->Branch("TNgenLeps",     &TNgenLeps,     "TNgenLeps/I");
  fTree->Branch("TgenLep0_Pt",     &TgenLep0_Pt,     "TgenLep0_Pt/F");
  fTree->Branch("TgenLep0_Eta",     &TgenLep0_Eta,     "TgenLep0_Eta/F");
  fTree->Branch("TgenLep0_Id",     &TgenLep0_Id,     "TgenLep0_Id/I");
  fTree->Branch("TgenLep1_Id",     &TgenLep1_Id,     "TgenLep1_Id/I");
  fTree->Branch("TgenLep1_Pt",     &TgenLep1_Pt,     "TgenLep1_Pt/F");
  fTree->Branch("TgenLep1_Eta",     &TgenLep1_Eta,     "TgenLep1_Eta/F");
  fTree->Branch("TgenLep0_Mid" ,    &TgenLep0_Mid ,    "TgenLep0_Mid/I");
  fTree->Branch("TgenLep1_Mid" ,     &TgenLep1_Mid ,     "TgenLep1_Mid/I");
  fTree->Branch("TgenDeltaPhi",      &TgenDeltaPhi,      "TgenDeltaPhi/F");
  fTree->Branch("TgenDeltaEta",      &TgenDeltaEta,      "TgenDeltaPhi/F");
  fTree->Branch("TgenMT2",      &TgenMT2,      "TgenMT2/F");
}

void StopTopAnalysis::SetJetVariables(){
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
  fTree->Branch("TNJetsJERUp",        &TNJetsJERUp,         "TNJetsJERUp/I");

  fTree->Branch("TNBtagsBtagUp",           &TNBtagsBtagUp,   "TNBtagsBtagUp/I");
  fTree->Branch("TNBtagsBtagDown",         &TNBtagsBtagDown, "TNBtagsBtagDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJERUp]/F");

  fTree->Branch("TNgenJets",        &TNgenJets,       "TNgenJets/I");
  fTree->Branch("TNgenBtags",       &TNgenBtags,      "TNgenBtags/I");
  fTree->Branch("TgenJet_Pt",       TgenJet_Pt,       "TgenJet_Pt[TNJets]/F");
  fTree->Branch("TgenJet_Eta",      TgenJet_Eta,      "TgenJet_Eta[TNJets]/F");
  fTree->Branch("TgenHT",           &TgenHT,          "TgenHT/F");
}

void StopTopAnalysis::SetEventVariables(){
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  fTree->Branch("TBtagSFFS",    &TBtagSFFS,    "TBtagSFFS/F");
  fTree->Branch("TMET",         &TMET,         "TMET/F");
  fTree->Branch("TPfMET",         &TPfMET,         "TPfMET/F");
  fTree->Branch("TNVert",         &TNVert,         "TNVert/I");
  fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  fTree->Branch("TIsSS",        &TIsSS,        "TIsSS/B");  

  if(gIsData) return;
  fTree->Branch("TgenMET",         &TgenMET,         "TgenMET/F");
  fTree->Branch("TISRweight",      &TISRweight,      "TISRweight/F");
  fTree->Branch("TWeight_MuonEffUp",      &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fTree->Branch("TWeight_MuonEffDown",    &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fTree->Branch("TWeight_ElecEffUp",      &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fTree->Branch("TWeight_ElecEffDown",    &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");
  fTree->Branch("TWeight_FSUp",        &TWeight_FSUp,        "TWeight_FSUp/F");
  fTree->Branch("TWeight_FSDown",        &TWeight_FSDown,        "TWeight_FSDown/F");

  //fTree->Branch("TNLHEWeight",        &TNLHEWeight,         "TNLHEWeight/I");
  fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fTree->Branch("TMETJERUp",    &TMETJERUp,    "TMETJERUp/F");
  fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
  fTree->Branch("TMT2JERUp",    &TMT2JERUp,    "TMT2JERUp/F");
  fTree->Branch("TMT2JESUp",    &TMT2JESUp,    "TMT2JESUp/F");
  fTree->Branch("TMT2JESDown",  &TMT2JESDown,  "TMT2JESDown/F");
  fTree->Branch("TMETMETUp",    &TMETMETUp,    "TMETMETUp/F");
  fTree->Branch("TMETMETDown",  &TMETMETDown,  "TMETMETDown/F");
  fTree->Branch("TMT2METUp",    &TMT2METUp,    "TMT2METUp/F");
  fTree->Branch("TMT2METDown",  &TMT2METDown,  "TMT2METDown/F");

  fTree->Branch("TgenWeight"  , &TgenWeight,  "TgenWeight/F" ); 
  fTree->Branch("TgenMETPhi"  , &TgenMETPhi , "TgenMETPhi/F" ); 
  fTree->Branch("TgenTop1Pt"  , &TgenTop1Pt , "TgenTop1Pt/F" );
  fTree->Branch("TgenTop1Eta" , &TgenTop1Eta, "TgenTop1Eta/F"); 
  fTree->Branch("TgenTop1Phi" , &TgenTop1Phi, "TgenTop1Phi/F"); 
  fTree->Branch("TgenTop1M"   , &TgenTop1M  , "TgenTop1M/F"  ); 
  fTree->Branch("TgenTop2Pt"  , &TgenTop2Pt , "TgenTop2Pt/F" ); 
  fTree->Branch("TgenTop2Eta" , &TgenTop2Eta, "TgenTop2Eta/F"); 
  fTree->Branch("TgenTop2Phi" , &TgenTop2Phi, "TgenTop2Phi/F"); 
  fTree->Branch("TgenTop2M"   , &TgenTop2M  , "TgenTop2M/F"  ); 

  fTree->Branch("TnGenW",         &TnGenW,         "TnGenW/I");
  fTree->Branch("TnGenStop",         &TnGenStop,         "TnGenStop/I");
  fTree->Branch("TnGenLsp",         &TnGenLsp,         "TnGenLsp/I");
  fTree->Branch("TnGenNeutrinos",         &TnGenNeutrinos,         "TnGenNeutrinos/I");
  fTree->Branch("TgenNeu1Pt"  , &TgenNeu1Pt , "TgenNeu1Pt/F" );
  fTree->Branch("TgenNeu1Eta" , &TgenNeu1Eta, "TgenNeu1Eta/F"); 
  fTree->Branch("TgenNeu1Phi" , &TgenNeu1Phi, "TgenNeu1Phi/F"); 
  fTree->Branch("TgenNeu1M"   , &TgenNeu1M  , "TgenNeu1M/F"  ); 
  fTree->Branch("TgenNeu2Pt"  , &TgenNeu2Pt , "TgenNeu2Pt/F" ); 
  fTree->Branch("TgenNeu2Eta" , &TgenNeu2Eta, "TgenNeu2Eta/F"); 
  fTree->Branch("TgenNeu2Phi" , &TgenNeu2Phi, "TgenNeu2Phi/F"); 
  fTree->Branch("TgenNeu2M"   , &TgenNeu2M  , "TgenNeu2M/F"  ); 
  fTree->Branch("TgenW1Pt"  , &TgenW1Pt , "TgenW1Pt/F" );
  fTree->Branch("TgenW1Eta" , &TgenW1Eta, "TgenW1Eta/F"); 
  fTree->Branch("TgenW1Phi" , &TgenW1Phi, "TgenW1Phi/F"); 
  fTree->Branch("TgenW1M"   , &TgenW1M  , "TgenW1M/F"  ); 
  fTree->Branch("TgenW2Pt"  , &TgenW2Pt , "TgenW2Pt/F" ); 
  fTree->Branch("TgenW2Eta" , &TgenW2Eta, "TgenW2Eta/F"); 
  fTree->Branch("TgenW2Phi" , &TgenW2Phi, "TgenW2Phi/F"); 
  fTree->Branch("TgenW2M"   , &TgenW2M  , "TgenW2M/F"  ); 
  fTree->Branch("TgenStop1Pt"  , &TgenStop1Pt , "TgenStop1Pt/F" );
  fTree->Branch("TgenStop1Eta" , &TgenStop1Eta, "TgenStop1Eta/F"); 
  fTree->Branch("TgenStop1Phi" , &TgenStop1Phi, "TgenStop1Phi/F"); 
  fTree->Branch("TgenStop1M"   , &TgenStop1M  , "TgenStop1M/F"  ); 
  fTree->Branch("TgenStop2Pt"  , &TgenStop2Pt , "TgenStop2Pt/F" ); 
  fTree->Branch("TgenStop2Eta" , &TgenStop2Eta, "TgenStop2Eta/F"); 
  fTree->Branch("TgenStop2Phi" , &TgenStop2Phi, "TgenStop2Phi/F"); 
  fTree->Branch("TgenStop2M"   , &TgenStop2M  , "TgenStop2M/F"  ); 
  fTree->Branch("TgenLsp1Pt"  , &TgenLsp1Pt , "TgenLsp1Pt/F" );
  fTree->Branch("TgenLsp1Eta" , &TgenLsp1Eta, "TgenLsp1Eta/F"); 
  fTree->Branch("TgenLsp1Phi" , &TgenLsp1Phi, "TgenLsp1Phi/F"); 
  fTree->Branch("TgenLsp1M"   , &TgenLsp1M  , "TgenLsp1M/F"  ); 
  fTree->Branch("TgenLsp2Pt"  , &TgenLsp2Pt , "TgenLsp2Pt/F" ); 
  fTree->Branch("TgenLsp2Eta" , &TgenLsp2Eta, "TgenLsp2Eta/F"); 
  fTree->Branch("TgenLsp2Phi" , &TgenLsp2Phi, "TgenLsp2Phi/F"); 
  fTree->Branch("TgenLsp2M"   , &TgenLsp2M  , "TgenLsp2M/F"  ); 
}

void StopTopAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
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
    TLep0_Id     = selLeptons.at(0).isElec ? 11 : 13;
  }
  if(TNSelLeps > 1){
    TLep1_Pt     = selLeptons.at(1).Pt();    
    TLep1_Eta    = selLeptons.at(1).Eta();    
    TLep1_Phi    = selLeptons.at(1).Phi();    
    TLep1_E      = selLeptons.at(1).E();    
    TLep1_Charge = selLeptons.at(1).charge;    
    TLep1_Id     = selLeptons.at(1).isElec ? 11 : 13;
    TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();      
    TDeltaPhi = (selLeptons.at(0).p.DeltaPhi(selLeptons.at(1).p));
    TDeltaEta = TMath::Abs(TMath::Abs(selLeptons.at(0).p.Eta()) - TMath::Abs(selLeptons.at(1).p.Eta()));
  }
  TPassTrigger = passTrigger;
  TPassMETfilters = passMETfilters;
  if(gIsData) return;  
}

void StopTopAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TBtagSFFS   = BtagSFFS;
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0; TNBtagsBtagUp = 0; TNBtagsBtagDown = 0;
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
  TNISRJets = Int_t(Get<Float_t>("nISRJet30"));
  for(Int_t i = 0; i < TNJets; i++){
    if(selJets.at(i).isBtag_BtagUp    ) TNBtagsBtagUp++;
    if(selJets.at(i).isBtag_BtagDown  ) TNBtagsBtagDown++;
    if(selJets.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
    if(selJets.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
  }
  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNBtagsJESUp    = 0;
  TNBtagsJESDown  = 0;
  TNJetsJERUp      = 0;  
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
      TNJetsJERUp++;
      TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
    }
  }
}

void StopTopAnalysis::GetMET(){
  TMET        = Get<Float_t>("met_pt");
  TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
  TNVert      = Get<Int_t>("nVert");
  TIsSS       = isSS;
  TPfMET = TMET;
  if(TNSelLeps>1)  TMT2 = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET,        TMET_Phi);
  if(gIsData) return;
  TgenMET     = Get<Float_t>("met_genPt");
  TMETMETUp = TMET; TMETMETDown = TMET;
  TMT2METDown = TMT2; TMT2METUp = TMT2;
  TMETJERUp   = GetParam<Float_t>("MET_JERUp");
  TMETJESUp   = GetParam<Float_t>("MET_JESUp");
  TMETJESDown = GetParam<Float_t>("MET_JESDown");
  if(gIsFastSim){
    TMET = (TMET + TgenMET)/2;
    Float_t difMET = TMath::Abs(TMET-TgenMET)/2;
    TMETMETUp = TMET + difMET;
    TMETMETDown = TMET - difMET;
    TMETJERUp   = JERtoMET(selJets, TMET, TMET_Phi);
    TMETJESUp   = JEStoMET(selJets, TMET, TMET_Phi,  1);
    TMETJESDown = JEStoMET(selJets, TMET, TMET_Phi, -1);
  }
  if(TNSelLeps > 1){
    TMT2JERUp   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJERUp,   TMET_Phi);
    TMT2JESUp   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESUp,   TMET_Phi);
    TMT2JESDown = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESDown, TMET_Phi);
    if(gIsFastSim){
      TMT2METUp     = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETMETUp,   TMET_Phi);
      TMT2METDown   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETMETUp,   TMET_Phi);
    }
  }
  TISRweight     = 1;
  if(gDoISR) TISRweight = getISRJetsWeight(TNISRJets)/NormISRweights;
  Float_t diff   = TMath::Abs(TISRweight - 1)/2;
  TISRweightUp   = TISRweight + diff;
  TISRweightDown = TISRweight - diff;
  if(gIsLHE) for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
  
}

void StopTopAnalysis::GetGenInfo(){
  if(gIsData) return; 
  TgenMETPhi = Get<Float_t>("met_genPhi");
  TgenWeight = genWeight;

  TLorentzVector top1; 
  TLorentzVector top2; 
  if (Get<Int_t>("nGenTop") > 1){
    TgenTop1Pt  = Get<Float_t>("GenTop_pt"  , 0);   TgenTop2Pt  = Get<Float_t>("GenTop_pt"  , 1);
    TgenTop1Eta = Get<Float_t>("GenTop_eta" , 0);   TgenTop2Eta = Get<Float_t>("GenTop_eta" , 1);
    TgenTop1Phi = Get<Float_t>("GenTop_phi" , 0);   TgenTop2Phi = Get<Float_t>("GenTop_phi" , 1);
    TgenTop1M   = Get<Float_t>("GenTop_mass", 0);   TgenTop2M   = Get<Float_t>("GenTop_mass", 1);
  }
  top1.SetPtEtaPhiM(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M);
  top2.SetPtEtaPhiM(TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M);
  TgentopDeltaPhi = top1.DeltaPhi(top2);
  TgentopDeltaR   = top1.DeltaR(top2);

  TgenMET     = Get<Float_t>("met_genPt");
  TNgenLeps = genLeptons.size();
  if(TNgenLeps >= 2){
    TgenLep0_Pt     = genLeptons.at(0).Pt();    
    TgenLep0_Eta    = genLeptons.at(0).Eta();    
    TgenLep0_Id     = genLeptons.at(0).isMuon? 13 : 11;
    TgenLep0_Mid    = genLeptons.at(0).Mid;
    TgenLep1_Pt     = genLeptons.at(1).Pt();    
    TgenLep1_Eta    = genLeptons.at(1).Eta();    
    TgenLep1_Id     = genLeptons.at(1).isMuon? 13 : 11;
    TgenLep1_Mid    = genLeptons.at(1).Mid;
    TgenDeltaPhi = (genLeptons.at(0).p.DeltaPhi(genLeptons.at(1).p));
    TgenDeltaEta = TMath::Abs(TMath::Abs(genLeptons.at(0).p.Eta()) - TMath::Abs(genLeptons.at(1).p.Eta()));
    TgenMT2 = getMT2ll(genLeptons.at(0), genLeptons.at(1), TgenMET,     Get<Float_t>("met_genPhi"));
  }
  TNgenJets = genJets.size();
  TNgenBtags= 2; // Has to be modified
  for(Int_t i = 0; i < 20; i++){
    if(i < TNgenJets){
      TgenJet_Pt[i]     = genJets.at(i).Pt();
      TgenJet_Eta[i]    = genJets.at(i).Eta();
    }
    else{
      TgenJet_Pt[i]     = 0;
      TgenJet_Eta[i]    = 0;
    }
    TgenHT += TgenJet_Pt[i];
  }

  TnGenW = 0; TnGenStop = 0; TnGenLsp = 0; TnGenNeutrinos = 0;

  Int_t nGenPart = Get<Int_t>("nGenPart");
  Int_t id = 0; Int_t mid = 0;
  for(Int_t i = 0; i < nGenPart; i++){
    id = TMath::Abs(Get<Int_t>("GenPart_pdgId",i));
    //>>> This is a neutrino
    if(id == 12 || id == 14 || id == 16){
      mid = TMath::Abs(Get<Int_t>("GenPart_motherId",i));
      if(mid == 23 || mid == 24){ // From W/Z
        TnGenNeutrinos++;
        if(TnGenNeutrinos == 1){
          TgenNeu1Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenNeu1Eta = Get<Float_t>("GenPart_eta" , i);
          TgenNeu1Phi = Get<Float_t>("GenPart_phi" , i);
          TgenNeu1M   = Get<Float_t>("GenPart_mass", i);
        }
        else if(TnGenNeutrinos >=2){
          TgenNeu2Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenNeu2Eta = Get<Float_t>("GenPart_eta" , i);
          TgenNeu2Phi = Get<Float_t>("GenPart_phi" , i);
          TgenNeu2M   = Get<Float_t>("GenPart_mass", i);
        }
      }
    }
 
    //>>> This is a W
    if(id == 24){
      mid = TMath::Abs(Get<Int_t>("GenPart_motherId",i));
      if(mid == 6 || mid == 1000006){ // From top/stop
        TnGenW++;
        if(TnGenW == 1){
          TgenW1Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenW1Eta = Get<Float_t>("GenPart_eta" , i);
          TgenW1Phi = Get<Float_t>("GenPart_phi" , i);
          TgenW1M   = Get<Float_t>("GenPart_mass", i);
        }
        else if(TnGenW >=2){
          TgenW2Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenW2Eta = Get<Float_t>("GenPart_eta" , i);
          TgenW2Phi = Get<Float_t>("GenPart_phi" , i);
          TgenW2M   = Get<Float_t>("GenPart_mass", i);
        }
      }
    }
    //>>> This is a stop 
    else if(id == 1000006){ 
      TnGenStop++;
      if(TnGenStop == 1){
        TgenStop1Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenStop1Eta = Get<Float_t>("GenPart_eta" , i);
        TgenStop1Phi = Get<Float_t>("GenPart_phi" , i);
        TgenStop1M   = Get<Float_t>("GenPart_mass", i);
      }
      else if(TnGenStop >=2){
        TgenStop2Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenStop2Eta = Get<Float_t>("GenPart_eta" , i);
        TgenStop2Phi = Get<Float_t>("GenPart_phi" , i);
        TgenStop2M   = Get<Float_t>("GenPart_mass", i);
      }
    }
    //>>> This is a neutralino
    else if(id == 1000022){
      TnGenLsp++;
      if(TnGenLsp == 1){
        TgenLsp1Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenLsp1Eta = Get<Float_t>("GenPart_eta" , i);
        TgenLsp1Phi = Get<Float_t>("GenPart_phi" , i);
        TgenLsp1M   = Get<Float_t>("GenPart_mass", i);
      }
      else if(TnGenLsp >=2){
        TgenLsp2Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenLsp2Eta = Get<Float_t>("GenPart_eta" , i);
        TgenLsp2Phi = Get<Float_t>("GenPart_phi" , i);
        TgenLsp2M   = Get<Float_t>("GenPart_mass", i);
      }
    }
  }
  return; 
}
