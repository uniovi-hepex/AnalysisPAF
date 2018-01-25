#include "t4Analysis.h"

ClassImp(t4Analysis);
t4Analysis::t4Analysis() : PAFChainItemSelector() {
  //>>> Initialize everything
  fTree = 0;
  TrigSF = 0; TrigSF_Up = 0; TrigSF_Down = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0;
  gChannel = 0; passMETfilters = 0; passTrigger = 0; passTrigger2 = 0; isSS = 0;  NormWeight = 0; 
  TWeight = 0; TIsOnZ = 0; TIsSS = false; TMZ = 0; TM3l = 0; TMll = 0;  TMET = 0; TMET_Phi = 0; TMT2 = 0; 
  TNTaus = 0; TNJets = 0; TNBtags = 0; THT = 0; 
  TNFakeableLeps = 0; TNSelLeps = 0; TChannel = 0; TPassTrigger = 0; TPassTrigger2 = 0; TPassMETFilters = 0; TPassLowInvMass = 0;
  TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJER = 0; TNBtagsJESUp = 0; TNBtagsJESDown = 0;
  TNBtagsBtagUp = 0; TNBtagsBtagDown = 0; TNBtagsMisTagUp = 0; TNBtagsMisTagDown = 0;
  THTJESUp = 0; THTJESDown = 0; TNISRJets = 0; TMETJESUp = 0; TMETJESDown = 0;
  TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
  TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;
  TRun = 0; TLumi = 0; TEvent = 0;
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
  for(Int_t i = 0; i < 10; i++){
    TTau_Pt         [i] = 0;
    TTau_Eta        [i] = 0;
    TTau_Charge     [i] = 0;
    TTau_DecayMode  [i] = 0;
    TTau_IdDecayMode[i] = 0;   
    TTau_IdMVA      [i] = 0;   
    TTau_IdAntiE    [i] = 0;   
    TTau_IdAntiMu   [i] = 0;   
  }
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
}

void t4Analysis::Summary(){}

//=== Initialise function... executed once per sample
void t4Analysis::Initialise(){
  //>>> Sample-dependent parameters
  gIsData      = GetParam<Bool_t>("IsData");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  fTree = CreateTree("tree","Created with PAF");

  //>>> LHE weights, for PDF and ME uncertainties
  gIsLHE = false;
  if(gSampleName.Contains("LHE")) gIsLHE = true;

  //>>> Set variables in tree
  SetTauVariables();
  SetLeptonVariables();
  SetJetVariables();
  SetEventVariables();

  //>>> Initialize vectors
  selTaus      = std::vector<Lepton>();
  selLeptons   = std::vector<Lepton>();
  zLeptons     = std::vector<Lepton>(); // Containing also loose leptons, that can come from a Z
  xLeptons     = std::vector<Lepton>(); // Containing also fakeable leptons
  vetoLeptons  = std::vector<Lepton>();
  looseLeptons = std::vector<Lepton>();
  selJets      = std::vector<Jet>();
  selBTags     = std::vector<Jet>();
  Jets15       = std::vector<Jet>();
}

// Reset parameters... to be executed one per event!
void t4Analysis::ResetParameters(){
  //>>> Clear every vector
  selTaus.clear();
  selLeptons.clear(); vetoLeptons.clear(); looseLeptons.clear(); zLeptons.clear(); xLeptons.clear();
  selJets.clear(); Jets15.clear(); selBTags.clear();

  //>>> Get vectors from previous selectors
  selTaus      = GetParam<vector<Lepton> >("selTaus"     );
  selLeptons   = GetParam<vector<Lepton> >("selLeptons"  );
  vetoLeptons  = GetParam<vector<Lepton> >("vetoLeptons" );
  looseLeptons = GetParam<vector<Lepton> >("looseLeptons");
  selJets      = GetParam<vector<Jet>    >("selJets"     );
  Jets15       = GetParam<vector<Jet>    >("Jets15"      );

  //>>> Fill extra vectors with leptons
  for(Int_t i = 0; i < (Int_t)   selLeptons.size(); i++) zLeptons.push_back(  selLeptons.at(i));
  for(Int_t i = 0; i < (Int_t) looseLeptons.size(); i++) zLeptons.push_back(looseLeptons.at(i));
  for(Int_t i = 0; i < (Int_t)   selLeptons.size(); i++) xLeptons.push_back(  selLeptons.at(i));
  for(Int_t i = 0; i < (Int_t)  vetoLeptons.size(); i++) xLeptons.push_back( vetoLeptons.at(i));

  //>>> Get weights and SFs from previous selectors
  NormWeight   = GetParam<Float_t>("NormWeight");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");

  //>>> Get event variables from previous selectors
  gChannel        = GetParam<Int_t>("gChannel");
  passMETfilters = GetParam<Bool_t>("METfilters");
  passTrigger    = GetParam<Bool_t>("passTrigger");
  passTrigger2   = GetParam<Bool_t>("passTrigger2");
  isSS           = GetParam<Bool_t>("isSS");
  
  //>>> Get leptons and jets
  GetTauVariables(selTaus);
  GetLeptonVariables(selLeptons);
  GetFakeableLeptonVariables(vetoLeptons);
  GetJetVariables(selJets, Jets15);
  GetMET();
}

//=== Main looping function
void t4Analysis::InsideLoop(){
  ResetParameters();

  //>>> Getting lepton SFs... using specific functions
  Int_t pdgId  = 0; Int_t pdgId2 = 0;
  for(Int_t i = 0; i < (Int_t) selLeptons.size(); i++){
    pdgId = selLeptons.at(i).isElec? 11 : 13;
    selLeptons.at(i).SetSF(   leptonScaleFactor(    pdgId, selLeptons.at(i).p.Pt(), selLeptons.at(i).p.Eta(), THT));
    selLeptons.at(i).SetSFerr(leptonScaleFactor_err(pdgId, selLeptons.at(i).p.Pt(), selLeptons.at(i).p.Eta(), THT));
  } 

  //>>> Getting trigger SF... using specific funtions
  if((Int_t) selLeptons.size() >= 2){
    pdgId  = selLeptons.at(0).isElec? 11 : 13;
    pdgId2 = selLeptons.at(1).isElec? 11 : 13;
    TrigSF       = TotalTriggerSF(pdgId, selLeptons.at(0).p.Pt(), selLeptons.at(0).p.Eta(), pdgId2, selLeptons.at(1).p.Pt(), selLeptons.at(1).p.Eta(), THT);
    TrigSF_Up    = TrigSF;
    TrigSF_Down  = TrigSF;
  }
  else TrigSF = 1;

  //>>> For tau fake estimate
  bool  tauFakesSelection(false);
  Int_t nReqLeps (tauFakesSelection ? 1        :    2);
  bool  passJetReq(tauFakesSelection ? TNJets>0 : true); 
 
  TChannel = Get4tChannel(selLeptons, xLeptons);
  Bool_t isFake = (TChannel == i2lss_fake || TChannel == iTriLep_fake || TChannel == i1Tau_emufakeSS);

  if(TChannel <= 0) return;
  if(!gIsData && TChannel == i2LOS) return;

  //>>> We select the events entering on one of the previous defined categories 
  //>>> Cuts for prompt lepton evets
  if(!isFake){
    if(IsThere3SS(selLeptons))          return; 
    if(!PassLowInvMass(selLeptons, 12)) return;
    if(selLeptons.at(0).p.Pt() < 25)    return;
  }
  //>>> Cuts for events with fake leptons
  else{
    if(IsThere3SS(xLeptons))          return; 
    if(!PassLowInvMass(xLeptons, 12)) return;
    if(xLeptons.at(0).p.Pt() < 25)    return;
  }

  //>>> Deal with weights:
  Float_t lepSF = 1; Float_t lepSFUp = 1; Float_t lepSFDo = 0;
  if((Int_t) selLeptons.size() >= 2){
    lepSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
    lepSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
    lepSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
  }
  TWeight            = NormWeight*lepSF*TrigSF*PUSF;
  TWeight_LepEffUp   = NormWeight*lepSFUp*TrigSF*PUSF;
  TWeight_LepEffDown = NormWeight*lepSFDo*TrigSF*PUSF;
  TWeight_TrigUp     = NormWeight*lepSF*TrigSF_Up*PUSF;
  TWeight_TrigDown   = NormWeight*lepSF*TrigSF_Down*PUSF;
  TWeight_PUDown     = NormWeight*lepSF*TrigSF*PUSF_Up;
  TWeight_PUUp       = NormWeight*lepSF*TrigSF*PUSF_Down;
  if(gIsData) TWeight = 1;

  // if(TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TPassTrigger && TPassMETFilters)
  fTree->Fill();
}

//#####################################################################
// Functions
//------------------------------------------------------------------

void t4Analysis::SetTauVariables(){
  fTree->Branch("TNTaus"          , &TNTaus         , "TNTaus/I"                   );
  fTree->Branch("TTau_Pt"         , TTau_Pt         , "TTau_Pt[TNTaus]/F"          );
  fTree->Branch("TTau_Eta"        , TTau_Eta        , "TTau_Eta[TNTaus]/F"         );
  fTree->Branch("TTau_Charge"     , TTau_Charge     , "TTau_Charge[TNTaus]/F"      );
  fTree->Branch("TTau_DecayMode"  , TTau_DecayMode  , "TTau_DecayMode[TNTaus]/F"   );
  fTree->Branch("TTau_IdDecayMode", TTau_IdDecayMode, "TTau_IdDecayMode[TNTaus]/F" );
  fTree->Branch("TTau_IdMVA"      , TTau_IdMVA      , "TTau_IdMVA[TNTaus]/F"       );
  fTree->Branch("TTau_IdAntiE"    , TTau_IdAntiE    , "TTau_IdAntiE[TNTaus]/F"     );
  fTree->Branch("TTau_IdAntiMu"   , TTau_IdAntiMu   , "TTau_IdAntiMu[TNTaus]/F"    );
}

void t4Analysis::SetLeptonVariables(){
  fTree->Branch("TNFakeableLeps"  ,&TNFakeableLeps, "TNFakeableLeps/I");
  fTree->Branch("TNSelLeps"       ,&TNSelLeps,      "TNSelLeps/I");
  //fTree->Branch("TLep_Pt"         ,TLep_Pt,         "TLep_Pt[TNSelLeps]/F");
  //fTree->Branch("TLep_Eta"        ,TLep_Eta,        "TLep_Eta[TNSelLeps]/F");
  //fTree->Branch("TLep_Phi"        ,TLep_Phi,        "TLep_Phi[TNSelLeps]/F");
  //fTree->Branch("TLep_E"          ,TLep_E ,         "TLep_E[TNSelLeps]/F");
  //fTree->Branch("TLep_pdgId"      ,TLep_pdgId,      "TLep_pdgId[TNSelLeps]/I");
  //fTree->Branch("TFLep_pdgId",     TFLep_pdgId,     "TFLep_pdgId[TNFakeableLeps]/I");
  //fTree->Branch("TFLep_Pt"        ,TFLep_Pt,        "TFLep_Pt[TNFakeableLeps]/F");
  //fTree->Branch("TFLep_Eta"       ,TFLep_Eta,       "TFLep_Eta[TNFakeableLeps]/F");
  //fTree->Branch("TFLep_Phi"       ,TFLep_Phi,       "TFLep_Phi[TNFakeableLeps]/F");
  //fTree->Branch("TFLep_E"         ,TFLep_E ,        "TFLep_E[TNFakeableLeps]/F");
  
  fTree->Branch("TLep0Pt",         &TLep0Pt,        "TLep0Pt/F");
  fTree->Branch("TLep0Eta",        &TLep0Eta,       "TLep0Eta/F");
  fTree->Branch("TLep0Phi",        &TLep0Phi,       "TLep0Phi/F");
  fTree->Branch("TLep0E",          &TLep0E,         "TLep0E/F");
  fTree->Branch("TLep0Id",         &TLep0Id,        "TLep0Id/I");
  fTree->Branch("TLep1Pt",         &TLep1Pt,        "TLep1Pt/F");
  fTree->Branch("TLep1Eta",        &TLep1Eta,       "TLep1Eta/F");
  fTree->Branch("TLep1Phi",        &TLep1Phi,       "TLep1Phi/F");
  fTree->Branch("TLep1E",          &TLep1E,         "TLep1E/F");
  fTree->Branch("TLep1Id",         &TLep1Id,        "TLep1Id/I");
  fTree->Branch("TLep2Pt",         &TLep2Pt,        "TLep2Pt/F");
  fTree->Branch("TLep2Eta",        &TLep2Eta,       "TLep2Eta/F");
  fTree->Branch("TLep2Phi",        &TLep2Phi,       "TLep2Phi/F");
  fTree->Branch("TLep2E",          &TLep2E,         "TLep2E/F");
  fTree->Branch("TLep2Id",         &TLep2Id,        "TLep2Id/I");
  fTree->Branch("TLep3Pt",         &TLep3Pt,        "TLep3Pt/F");
  fTree->Branch("TLep3Eta",        &TLep3Eta,       "TLep3Eta/F");
  fTree->Branch("TLep3Phi",        &TLep3Phi,       "TLep3Phi/F");
  fTree->Branch("TLep3E",          &TLep3E,         "TLep3E/F");
  fTree->Branch("TLep3Id",         &TLep3Id,        "TLep3Id/I");
  fTree->Branch("TLep4Pt",         &TLep4Pt,        "TLep4Pt/F");
  fTree->Branch("TLep4Eta",        &TLep4Eta,       "TLep4Eta/F");
  fTree->Branch("TLep4Phi",        &TLep4Phi,       "TLep4Phi/F");
  fTree->Branch("TLep4E",          &TLep4E,         "TLep4E/F");
  fTree->Branch("TLep4Id",         &TLep4Id,        "TLep4Id/I");
  fTree->Branch("TFLep0Pt",         &TFLep0Pt,        "TFLep0Pt/F");
  fTree->Branch("TFLep0Eta",        &TFLep0Eta,       "TFLep0Eta/F");
  fTree->Branch("TFLep0Phi",        &TFLep0Phi,       "TFLep0Phi/F");
  fTree->Branch("TFLep0E",          &TFLep0E,         "TFLep0E/F");
  fTree->Branch("TFLep0Id",         &TFLep0Id,        "TFLep0Id/I");
  fTree->Branch("TFLep1Pt",         &TFLep1Pt,        "TFLep1Pt/F");
  fTree->Branch("TFLep1Eta",        &TFLep1Eta,       "TFLep1Eta/F");
  fTree->Branch("TFLep1Phi",        &TFLep1Phi,       "TFLep1Phi/F");
  fTree->Branch("TFLep1E",          &TFLep1E,         "TFLep1E/F");
  fTree->Branch("TFLep1Id",         &TFLep1Id,        "TFLep1Id/I");

  fTree->Branch("TChannel"        ,&TChannel,       "TChannel/I");
  fTree->Branch("TMll"            ,&TMll,           "TMll/F");
  fTree->Branch("TM3l"            ,&TM3l,           "TM3l/F");
  fTree->Branch("TMZ"             ,&TMZ,            "TMZ/F");
  fTree->Branch("TIsSS"           ,&TIsSS,          "TIsSS/B");
}

void t4Analysis::SetJetVariables(){
  fTree->Branch("TNJets",        &TNJets,       "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,      "TNBtags/I");
  fTree->Branch("TJet_isBJet",   TJet_isBJet,   "TJet_isBJet[TNJets]/I");
  fTree->Branch("TJet_Pt",       TJet_Pt,       "TJet_Pt[TNJets]/F");
  fTree->Branch("TJet_Eta",      TJet_Eta,      "TJet_Eta[TNJets]/F");
  fTree->Branch("TJet_Phi",      TJet_Phi,      "TJet_Phi[TNJets]/F");
  fTree->Branch("TJet_E",        TJet_E,        "TJet_E[TNJets]/F");
  fTree->Branch("THT",           &THT,          "THT/F");

  if(gIsData) return;
  fTree->Branch("TNJetsJESUp",      &TNJetsJESUp,       "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",    &TNJetsJESDown,     "TNJetsJESDown/I");
  fTree->Branch("TNBtagsJESUp",     &TNBtagsJESUp,      "TNBtagsJESUp/I");
  fTree->Branch("TNBtagsJESDown",   &TNBtagsJESDown,    "TNBtagsJESDown/I");
  fTree->Branch("TNJetsJER",        &TNJetsJER,         "TNJetsJER/I");

  fTree->Branch("TNBtagsBtagUp",       &TNBtagsBtagUp,     "TNBtagsBtagUp/I");
  fTree->Branch("TNBtagsBtagDown",     &TNBtagsBtagDown,   "TNBtagsBtagDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");
}

void t4Analysis::SetEventVariables(){
  fTree->Branch("TWeight",         &TWeight,         "TWeight/F");
  fTree->Branch("TMET",            &TMET,            "TMET/F");
  fTree->Branch("TMET_Phi",        &TMET_Phi,        "TMET_Phi/F");
  fTree->Branch("TMT2",            &TMT2,            "TMT2/F");
  fTree->Branch("TIsOnZ",          &TIsOnZ,          "TIsOnZ/I");
  fTree->Branch("TPassTrigger",    &TPassTrigger,    "TPassTrigger/I");
  fTree->Branch("TPassTrigger2",    &TPassTrigger2,    "TPassTrigger2/I");
  fTree->Branch("TPassMETFilters", &TPassMETFilters, "TPassMETFilters/I");
  fTree->Branch("TPassLowInvMass", &TPassLowInvMass, "TPassLowInvMass/I");

  fTree->Branch("TRun",        &TRun,      "TRun/I");
  fTree->Branch("TLumi",       &TLumi,     "TLumi/I");
  fTree->Branch("TEvent",      &TEvent,    "TEvent/I");

  if(gIsData) return;
  fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");

  fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
}

void t4Analysis::GetTauVariables(std::vector<Lepton> selTaus){
  TNTaus = selTaus.size();
  for(Int_t i=0; i<10; ++i){
    if(i<TNTaus){
      TTau_Pt         [i] = selTaus.at(i).Pt();
      TTau_Eta        [i] = selTaus.at(i).Eta();
      TTau_Charge     [i] = selTaus.at(i).charge;
      TTau_DecayMode  [i] = selTaus.at(i).decayMode;  
      TTau_IdDecayMode[i] = selTaus.at(i).idDecayMode;
      TTau_IdMVA      [i] = selTaus.at(i).idMVA;      
      TTau_IdAntiE    [i] = selTaus.at(i).idAntiE;    
      TTau_IdAntiMu   [i] = selTaus.at(i).idAntiMu;   
    }
    else{
      TTau_Pt         [i] = 0;
      TTau_Eta        [i] = 0;
      TTau_Charge     [i] = 0;
      TTau_DecayMode  [i] = 0;
      TTau_IdDecayMode[i] = 0;
      TTau_IdMVA      [i] = 0;
      TTau_IdAntiE    [i] = 0;
      TTau_IdAntiMu   [i] = 0;
    }
  }
}

void t4Analysis::GetLeptonVariables(std::vector<Lepton> selLeptons){
  TNSelLeps = selLeptons.size();
  TLep0Pt = 0; TLep0Eta = 0; TLep0Phi = 0; TLep0E = 0; TLep0Id = 0;
  TLep1Pt = 0; TLep1Eta = 0; TLep1Phi = 0; TLep1E = 0; TLep1Id = 0;
  TLep2Pt = 0; TLep2Eta = 0; TLep2Phi = 0; TLep2E = 0; TLep2Id = 0;
  TLep3Pt = 0; TLep3Eta = 0; TLep3Phi = 0; TLep3E = 0; TLep3Id = 0;
  if(TNSelLeps >= 1){
    TLep0Pt = selLeptons.at(0).Pt(); TLep0Eta = selLeptons.at(0).Eta(); TLep0Phi = selLeptons.at(0).Phi(); TLep0E = selLeptons.at(0).E(); TLep0Id = selLeptons.at(0).isMuon? 13 : 11;
  }
  if(TNSelLeps >= 2){
    TLep1Pt = selLeptons.at(1).Pt(); TLep1Eta = selLeptons.at(1).Eta(); TLep1Phi = selLeptons.at(1).Phi(); TLep1E = selLeptons.at(1).E(); TLep1Id = selLeptons.at(1).isMuon? 13 : 11;
  }
  if(TNSelLeps >= 3){
    TLep2Pt = selLeptons.at(2).Pt(); TLep2Eta = selLeptons.at(2).Eta(); TLep2Phi = selLeptons.at(2).Phi(); TLep2E = selLeptons.at(2).E(); TLep2Id = selLeptons.at(2).isMuon? 13 : 11;
  }
  if(TNSelLeps >= 4){
    TLep3Pt = selLeptons.at(3).Pt(); TLep3Eta = selLeptons.at(3).Eta(); TLep3Phi = selLeptons.at(3).Phi(); TLep3E = selLeptons.at(3).E(); TLep3Id = selLeptons.at(3).isMuon? 13 : 11;
  }
  TMll = TNSelLeps < 2 ? 0 : (selLeptons.at(0).p + selLeptons.at(1).p).M();      
  TM3l = TNSelLeps < 3 ? 0 : (selLeptons.at(0).p + selLeptons.at(1).p + selLeptons.at(2).p).M();      
  TMZ    = ClosestMlltoZ(zLeptons);
  TIsOnZ = IsOnZ(zLeptons);
  TPassLowInvMass = PassLowInvMass(selLeptons, 12);
  TRun   = Get<UInt_t>("run");
  TLumi  = Get<UInt_t>("lumi");
  TEvent = Get<ULong64_t>("evt");
}

void t4Analysis::GetFakeableLeptonVariables(std::vector<Lepton> vetoLeptons){
  TNFakeableLeps = vetoLeptons.size();
  TFLep0Pt = 0; TFLep0Eta = 0; TFLep0Phi = 0; TFLep0E = 0; TFLep0Id = 0;
  TFLep1Pt = 0; TFLep1Eta = 0; TFLep1Phi = 0; TFLep1E = 0; TFLep1Id = 0;
  if(TNFakeableLeps >= 1){
    TFLep0Pt = vetoLeptons.at(0).Pt(); TFLep0Eta = vetoLeptons.at(0).Eta(); TFLep0Phi = vetoLeptons.at(0).Phi(); TFLep0E = vetoLeptons.at(0).E(); TFLep0Id = vetoLeptons.at(0).isMuon? 13 : 11;
  }
  if(TNFakeableLeps >= 2){
    TFLep1Pt = vetoLeptons.at(1).Pt(); TFLep1Eta = vetoLeptons.at(1).Eta(); TFLep1Phi = vetoLeptons.at(1).Phi(); TFLep1E = vetoLeptons.at(1).E(); TFLep1Id = vetoLeptons.at(1).isMuon? 13 : 11;
  }
}

void t4Analysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0; TNBtagsBtagUp = 0; TNBtagsBtagDown = 0;
  TNBtagsMisTagUp = 0;  TNBtagsMisTagDown = 0;
  Int_t ntotalJets = cleanedJets15.size();
  for(Int_t i = 0; i < 20; i++){
    if(i < TNJets){
      TJet_Pt[i]     = selJets.at(i).Pt();
      TJet_Eta[i]    = selJets.at(i).Eta();
      TJet_Phi[i]    = selJets.at(i).Phi();
      TJet_E[i]      = selJets.at(i).E();
      TJet_isBJet[i] = selJets.at(i).isBtag;
      //if(selJets.at(i).isBtag)            TNBtags++;
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
  for(Int_t i = 0; i < ntotalJets; i++) 
    if(cleanedJets15.at(i).p.Pt() > 25 && cleanedJets15.at(i).isBtag) selBTags.push_back(cleanedJets15.at(i)); 
  TNBtags = selBTags.size();
  if(gIsData) return;  // For systematics...
  for(Int_t i = 0; i < ntotalJets; i++){
    if(cleanedJets15.at(i).p.Pt() > 25){
      if(cleanedJets15.at(i).isBtag_BtagUp    ) TNBtagsBtagUp++;
      if(cleanedJets15.at(i).isBtag_BtagDown  ) TNBtagsBtagDown++;
      if(cleanedJets15.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
      if(cleanedJets15.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
    }
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

void t4Analysis::GetMET(){
  TMET        = Get<Float_t>("met_pt");
  TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
  TIsSS       = isSS;
  if(TNSelLeps>1)  TMT2 = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET,        TMET_Phi);
  TPassTrigger = passTrigger;
  TPassTrigger2 = passTrigger2;
  TPassMETFilters = passMETfilters;
  if(gIsData) return;
  TMETJESUp   = GetParam<Float_t>("MET_JESUp");
  TMETJESDown = GetParam<Float_t>("MET_JESDown");
  if(gIsLHE) for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

Int_t t4Analysis::Get4tChannel(vector<Lepton> selLeptons, vector<Lepton> xLeptons){
  //>>> Select the channel... i2lss, iTrilep, iSS1tau, iOS1tau, i2lss_fake, iTrilep_fake, i1Tau_emufakeSS, i1Tau_emufakeOS
  TNSelLeps = selLeptons.size(); TNFakeableLeps = xLeptons.size() - TNSelLeps;
  
  //>>> 2LOS
  if(TNSelLeps == 2 && TNFakeableLeps == 0 && !IsThereSSpair(selLeptons)) return i2LOS;

  if(TNTaus == 0){
    // --> 0 Lep
    if(TNSelLeps == 0){
      if(TNFakeableLeps < 2) return -1;
      else if(TNFakeableLeps == 2){
        if(IsThereSSpair(xLeptons)) return i2lss_fake;
      }
      else return -1;
    }
    // ---> 1 Lep
    else if(TNSelLeps == 1){
      if(TNFakeableLeps < 1) return -1;
      else if(TNFakeableLeps == 1){
        if(IsThereSSpair(xLeptons)) return i2lss_fake;
      }
      else if(TNFakeableLeps == 2){
        if(PassLowInvMass(xLeptons, 12)) return iTriLep_fake;
      }
      else return -1;
    }
    // ---> 2 Lep
    else if(TNSelLeps == 2){
      if(IsThereSSpair(selLeptons)) return i2lss;
      else if(TNFakeableLeps >= 1){
        return iTriLep_fake;
      }
    }
    // ---> 3 Lep
    else if(TNSelLeps >= 3) return iTriLep;
  }

  else if(TNTaus >= 1){
    if(TNSelLeps >= 2){
      if(IsThereSSpair(selLeptons)) return iSS1tau;
      else                          return iOS1tau;
    }
    if( (TNSelLeps == 1 && TNFakeableLeps >= 1) || (TNSelLeps == 0 && TNFakeableLeps >= 2) ){
      if  (IsThereSSpair(xLeptons)) return i1Tau_emufakeSS; 
      else                          return i1Tau_emufakeOS;
    }
  }

  return -1;


/*

    if     ( (TNSelLeps == 0 && TNFakeableLeps >= 2) || (TNSelLeps == 1 && TNFakeableLeps == 1)){ // Fakes for 2lss
      if(IsThereSSpair(xLeptons) && PassLowInvMass(xLeptons, 12))   TChannel = i2lss_fake; 
    }
    else if(TNSelLeps == 1 && TNFakeableLeps >= 2){ // Fakes for 2lss
      if(       IsThere3SS(   xLeptons)) TChannel = -1;
      else if  (IsThereSSpair(xLeptons)) TChannel = i2lss_fake; 
      else                               TChannel = iTriLep_fake; // Fakes for 3L
    }
    else if(TNSelLeps == 2 && TNFakeableLeps == 0){ // 2lss
      if(IsThereSSpair(selLeptons)) TChannel = i2lss;
      else                          TChannel = -1;
    }
    else if(TNSelLeps == 2 && TNFakeableLeps >= 1){ // Fakes for 3L
      if(IsThereSSpair(selLeptons)) TChannel = i2lss;
      else{  
        if(IsThere3SS(xLeptons)) TChannel = -1;
        else                     TChannel = iTriLep_fake;
      }  
    } 
    else if(TNSelLeps >= 3){
      TChannel = iTriLep;
      if(     IsThere3SS(   selLeptons)) TChannel = -1;
    }
  }
  else if(TNTaus >= 1){
    if(TNSelLeps >= 2){
      if(IsThereSSpair(selLeptons)) TChannel = iSS1tau;
      else                          TChannel = iOS1tau;
    }
    if( (TNSelLeps == 1 && TNFakeableLeps >= 1) || (TNSelLeps == 0 && TNFakeableLeps >= 2) ){
      if  (IsThereSSpair(xLeptons)) TChannel = i1Tau_emufakeSS; 
      else  TChannel = i1Tau_emufakeOS;
    }
  }
  if(TChannel == i2lss_fake || TChannel == iTriLep_fake || TChannel == i1Tau_emufakeOS || TChannel == i1Tau_emufakeSS) if(!PassLowInvMass(xLeptons, 12)) TChannel = -1;

*/
}

