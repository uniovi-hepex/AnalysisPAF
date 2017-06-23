#include "t4Analysis.h"

ClassImp(t4Analysis);
t4Analysis::t4Analysis() : PAFChainItemSelector() {
  fTree = 0;

  TrigSF = 0; TrigSF_Up = 0; TrigSF_Down = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0;
  gChannel = 0; passMETfilters = 0; passTrigger = 0; isSS = 0;  NormWeight = 0; TWeight = 0; TIsOnZ = 0;
  TIsSS = false; TMZ = 0; TM3l = 0; TMll = 0;  TMET = 0; TMET_Phi = 0; TMT2 = 0; TNTaus = 0; TNJets = 0; TNBtags = 0; THT = 0; 
  TNFakeableLeps = 0; TNSelLeps = 0; TChannel = 0; TPassTrigger = 0; TPassMETFilters = 0; TPassLowInvMass = 0;
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

    TLep_Pt       [i] = 0;
    TLep_Eta      [i] = 0;
    TLep_Phi      [i] = 0;
    TLep_E        [i] = 0;
    TLep_Charge   [i] = 0;
    TLep_pdgId    [i] = 0;
  }
  for(Int_t i = 0; i < 5; i++){
    TFLep_Pt       [i] = 0;
    TFLep_Eta      [i] = 0;
    TFLep_Phi      [i] = 0;
    TFLep_E        [i] = 0;
    TFLep_Charge   [i] = 0;
    TFLep_pdgId    [i] = 0;
  }
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
}

void t4Analysis::Summary(){}

void t4Analysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  fTree = CreateTree("tree","Created with PAF");

  gIsLHE = false;
  if(gSampleName.Contains("LHE")) gIsLHE = true;

  SetTauVariables();
  SetLeptonVariables();
  SetJetVariables();
  SetEventVariables();

  selTaus      = std::vector<Lepton>();
  selLeptons   = std::vector<Lepton>();
  zLeptons     = std::vector<Lepton>();
  xLeptons     = std::vector<Lepton>();
  vetoLeptons  = std::vector<Lepton>();
  looseLeptons = std::vector<Lepton>();
  selJets      = std::vector<Jet>();
  selBTags     = std::vector<Jet>();
  Jets15       = std::vector<Jet>();
}


void t4Analysis::InsideLoop(){
  // Vectors with the objects
  selTaus.clear();
  selLeptons.clear(); vetoLeptons.clear(); looseLeptons.clear(); zLeptons.clear(); xLeptons.clear();
  selJets.clear(); Jets15.clear(); selBTags.clear();

  selTaus      = GetParam<vector<Lepton> >("selTaus"     );
  selLeptons   = GetParam<vector<Lepton> >("selLeptons"  );
  vetoLeptons  = GetParam<vector<Lepton> >("vetoLeptons" );
  looseLeptons = GetParam<vector<Lepton> >("looseLeptons");
  selJets      = GetParam<vector<Jet>    >("selJets"     );
  Jets15       = GetParam<vector<Jet>    >("Jets15"      );


  for(Int_t i = 0; i < (Int_t)   selLeptons.size(); i++) zLeptons.push_back(  selLeptons.at(i));
  for(Int_t i = 0; i < (Int_t) looseLeptons.size(); i++) zLeptons.push_back(looseLeptons.at(i));
  for(Int_t i = 0; i < (Int_t)   selLeptons.size(); i++) xLeptons.push_back(  selLeptons.at(i));
  for(Int_t i = 0; i < (Int_t)  vetoLeptons.size(); i++) xLeptons.push_back( vetoLeptons.at(i));

  // Weights and SFs
  NormWeight   = GetParam<Float_t>("NormWeight");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");

  // Event variables
  gChannel        = GetParam<Int_t>("gChannel");
  passMETfilters = GetParam<Bool_t>("METfilters");
  passTrigger    = GetParam<Bool_t>("passTrigger");
  isSS           = GetParam<Bool_t>("isSS");
  
  // Leptons and Jets
  GetTauVariables(selTaus);
  GetLeptonVariables(selLeptons);
  GetFakeableLeptonVariables(vetoLeptons);
  GetJetVariables(selJets, Jets15);
  GetMET();
  
  // Getting lepton SFs...
  Int_t pdgId  = 0; Int_t pdgId2 = 0;
  for(Int_t i = 0; i < (Int_t) selLeptons.size(); i++){
    pdgId = selLeptons.at(i).isElec? 11 : 13;
    selLeptons.at(i).SetSF(   leptonScaleFactor(    pdgId, selLeptons.at(i).p.Pt(), selLeptons.at(i).p.Eta(), THT));
    selLeptons.at(i).SetSFerr(leptonScaleFactor_err(pdgId, selLeptons.at(i).p.Pt(), selLeptons.at(i).p.Eta(), THT));
  } 

  // Getting trigger SF...
  if((Int_t) selLeptons.size() >= 2){
    pdgId  = selLeptons.at(0).isElec? 11 : 13;
    pdgId2 = selLeptons.at(1).isElec? 11 : 13;
    TrigSF       = TotalTriggerSF(pdgId, selLeptons.at(0).p.Pt(), selLeptons.at(0).p.Eta(), pdgId2, selLeptons.at(1).p.Pt(), selLeptons.at(1).p.Eta(), THT);
    TrigSF_Up    = TrigSF;
    TrigSF_Down  = TrigSF;
  }
  else TrigSF = 1;

  bool  tauFakesSelection(false);
  Int_t nReqLeps (tauFakesSelection ? 1        :    2);
  bool  passJetReq(tauFakesSelection ? TNJets>0 : true);

  TChannel = -1;
  if(TNTaus == 0){
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
    if( (TNSelLeps == 1 && TNFakeableLeps >= 1) || (TNSelLeps == 0 && TNFakeableLeps >= 2) ) TChannel = i1Tau_emufake;
  }
  if(TChannel == i2lss_fake || TChannel == iTriLep_fake || TChannel == i1Tau_emufake) if(!PassLowInvMass(xLeptons, 12)) TChannel = -1;

  //if( (TNSelLeps > nReqLeps || TNFakeableLeps > nReqLeps) && passJetReq && passTrigger && passMETfilters){
  //if(TChannel > 0 && passTrigger && passMETfilters){ // It's in some valid category and passes triggers and MET
  if(TChannel > 0){ 
    // Deal with weights:
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

    // Event Selection
    // ===================================================================================================================
    //if(PassLowInvMass(xLeptons, 12)){ // mll > 12 GeV
      //if(TNJets >= 2 || TNJetsJESUp >= 2 || TNJetsJESDown >= 2 || TNJetsJER >= 2){ //At least 2 jets
      if(xLeptons.at(0).p.Pt() > 25)
      fTree->Fill();
      //}
    //}
  }
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
  fTree->Branch("TLep_Pt"         ,TLep_Pt,         "TLep_Pt[TNSelLeps]/F");
  fTree->Branch("TLep_Eta"        ,TLep_Eta,        "TLep_Eta[TNSelLeps]/F");
  fTree->Branch("TLep_Phi"        ,TLep_Phi,        "TLep_Phi[TNSelLeps]/F");
  fTree->Branch("TLep_E"          ,TLep_E ,         "TLep_E[TNSelLeps]/F");
  fTree->Branch("TLep_Charge"     ,TLep_Charge,     "TLep_Charge[TNSelLeps]/I");
  fTree->Branch("TLep_pdgId"      ,TLep_pdgId,      "TLep_pdgId[TNSelLeps]/I");
  fTree->Branch("TFLep_pdgId",     TFLep_pdgId,     "TFLep_pdgId[TNFakeableLeps]/I");
  fTree->Branch("TFLep_Pt"        ,TFLep_Pt,        "TFLep_Pt[TNFakeableLeps]/F");
  fTree->Branch("TFLep_Eta"       ,TFLep_Eta,       "TFLep_Eta[TNFakeableLeps]/F");
  fTree->Branch("TFLep_Phi"       ,TFLep_Phi,       "TFLep_Phi[TNFakeableLeps]/F");
  fTree->Branch("TFLep_E"         ,TFLep_E ,        "TFLep_E[TNFakeableLeps]/F");
  fTree->Branch("TFLep_Charge"    ,TFLep_Charge,    "TFLep_Charge[TNFakeableLeps]/F");
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
  for(Int_t i = 0; i < 10; i++){
    if(i < TNSelLeps){
      TLep_Pt[i]     = selLeptons.at(i).Pt();
      TLep_Eta[i]    = selLeptons.at(i).Eta();
      TLep_Phi[i]    = selLeptons.at(i).Phi();
      TLep_E[i]      = selLeptons.at(i).E();
      TLep_pdgId[i] = selLeptons.at(i).charge;
    }
    else{
      TLep_Pt[i]     = 0;
      TLep_Eta[i]    = 0;
      TLep_Phi[i]    = 0;
      TLep_E[i]      = 0;
      TLep_Charge[i] = 0;
      TLep_pdgId[i] = 0;
    }
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
  for(Int_t i = 0; i < 5; i++){
    if(i < TNFakeableLeps ){
      TFLep_Pt[i]     = vetoLeptons.at(i).Pt();
      TFLep_Eta[i]    = vetoLeptons.at(i).Eta();
      TFLep_Phi[i]    = vetoLeptons.at(i).Phi();
      TFLep_E[i]      = vetoLeptons.at(i).E();
      TFLep_Charge[i] = vetoLeptons.at(i).charge;
      TFLep_pdgId[i]  = vetoLeptons.at(i).isElec? 11 : 13;
    }
    else{
      TFLep_Pt[i]     = 0;
      TFLep_Eta[i]    = 0;
      TFLep_Phi[i]    = 0;
      TFLep_E[i]      = 0;
      TFLep_Charge[i] = 0;
      TFLep_pdgId[i]  = 0;
    }
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
  //TNBtags = GetParam<Int_t>("nVetoJets");
  if(gIsData) return;  // For systematics...
  for(Int_t i = 0; i < ntotalJets; i++){
    if(cleanedJets15.at(i).p.Pt() > 20){
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
  TPassMETFilters = passMETfilters;
  if(gIsData) return;
  TMETJESUp   = GetParam<Float_t>("MET_JESUp");
  TMETJESDown = GetParam<Float_t>("MET_JESDown");
  if(gIsLHE) for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

