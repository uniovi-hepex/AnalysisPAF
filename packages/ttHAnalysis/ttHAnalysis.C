/*==============================================================================

              ttHAnalysis selector

==============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//		Preprocessor directives
////////////////////////////////////////////////////////////////////////////////
//	Package inclusion
#include "ttHAnalysis.h"

ClassImp(ttHAnalysis); // PAF definition as class

////////////////////////////////////////////////////////////////////////////////
//		Initial definitions
////////////////////////////////////////////////////////////////////////////////
ttHAnalysis::ttHAnalysis() : PAFChainItemSelector() {
  // Defining to zero the aim of the minitree.
  fTree       = 0;
}


//	Core PAF-analysis methods
//------------------------------------------------------------------------------
void ttHAnalysis::Initialise() {
  PAF_INFO("ttHAnalysis", "+ Preprocess DONE");
  cout << endl;
  PAF_INFO("ttHAnalysis", "======================================== Initialization");
  PAF_INFO("ttHAnalysis", "+ Initializing analysis parameters");
  GetParameters();

  PAF_INFO("ttHAnalysis", "+ Initializing minitree");
  TH1::SetDefaultSumw2();
  fTree = CreateTree("MiniTree","Created with PAF");
  SetLeptonBranches();
  SetJetBranches();
  SetEventBranches();
  SetSystBranches();

  PAF_INFO("ttHAnalysis", "+ Initializing variables");
  InitialiseVariables();
  
  PAF_INFO("ttHAnalysis", "+ Initializing tool for resetting SF for 3l and 4l categories");
  Initialise3l4lLeptonSF();
  
  cout << endl;
  PAF_INFO("ttHAnalysis", "+ Initialization DONE");
  cout << endl;
  PAF_INFO("ttHAnalysis", "======================================== Analysis");
}


void ttHAnalysis::InsideLoop() {
  // Get and set data members
  GetTreeVariables();
  GetEventVariables();
  
  // Check trigger
  if (!passTrigger)                                                             return;
  
  // Filter events that never pass the precuts
  if (!PassesPreCuts(nJets, nLooseBTags, nMediumBTags) && 
      !PassesPreCuts(TnJetsJESUp, TnLooseBTagsJESUp, TnMediumBTagsJESUp) && 
      !PassesPreCuts(TnJetsJESDown, TnLooseBTagsJESDown, TnMediumBTagsJESDown)) return;
  
  // Get minitree variables, specifically the event selection
  SetMiniTreeVariables();
  
  // Filter events that are never selected.
  if (TCat == 0 && TCatJESUp == 0 && TCatJESDown == 0)                          return;
  
  // Set SF for 3l and 4l categories, and get the correct weight
  Reset3l4lLeptonSF();
  CalculateWeight(); 
  
  // Fill minitree
  fTree->Fill();  
}


void ttHAnalysis::Summary() {
  PAF_INFO("ttHAnalysis", "+ Analysis DONE");
  cout << endl;
}


////////////////////////////////////////////////////////////////////////////////
//		Tree-related methods
////////////////////////////////////////////////////////////////////////////////
void ttHAnalysis::GetTreeVariables() {
  MET       = 0;
  Tevt      = 0;
  Trun      = 0;
  genWeight = 1;
  for (UInt_t i = 0; i < 254; i++) TLHEWeight[i] = 0;  
  
  if (!gIsData){
    genWeight   = Get<Float_t>("genWeight");
  }

  MET       = Get<Float_t>("met_pt");
  Tevt      = Get<ULong64_t>("evt");
  Trun      = Get<UInt_t>("run");
  if (gIsLHE) {
    for (UInt_t i = 0; i < Get<UInt_t>("nLHEweight"); i++) TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
  }
}


// Minitree methods
//------------------------------------------------------------------------------
void ttHAnalysis::SetLeptonBranches() {
  fTree->Branch("TnTightLepton",        &nTightLepton,        "nTightLepton/i");
  fTree->Branch("TnFakeableLepton",     &nFakeableLepton,     "nFakeableLepton/i");
  fTree->Branch("TnLooseLepton",        &nLooseLepton,        "nLooseLepton/i");
  fTree->Branch("TnTaus",               &nTaus,               "nTaus/i");
  fTree->Branch("TPtLeading",           &TPtLeading,          "TPtLeading/F");
  fTree->Branch("TPtSubLeading",        &TPtSubLeading,       "TPtSubLeading/F");
  fTree->Branch("TPtSubSubLeading",     &TPtSubSubLeading,    "TPtSubSubLeading/F");
  fTree->Branch("TPtSubSubSubLeading",  &TPtSubSubSubLeading, "TPtSubSubSubLeading/F");
  fTree->Branch("TPtVector",            &TPtVector);
}


void ttHAnalysis::SetJetBranches() {
  fTree->Branch("TnMediumBTags" ,   &nMediumBTags,      "nMediumBTags/i");
  fTree->Branch("TnLooseBTags",     &nLooseBTags,       "nLooseBTags/i");
  fTree->Branch("TnJets",           &nJets,             "nJets/i");
}


void ttHAnalysis::SetEventBranches() {
  fTree->Branch("TChannel",         &gChannel,          "gChannel/I");
  fTree->Branch("TCat",             &TCat,              "TCat/I");
  fTree->Branch("TpassTrigger",     &passTrigger,       "passTrigger/B");
  fTree->Branch("TisSS",            &isSS,              "isSS/B");
  fTree->Branch("TMET",             &MET,               "MET/F");
  fTree->Branch("TMHT",             &MHT,               "MHT/F");
  fTree->Branch("THT",              &HT,                "HT/F");
  fTree->Branch("TMETLD",           &METLD,             "METLD/F");
  fTree->Branch("TCS",              &TCS,               "TCS/I");
  fTree->Branch("TMass",            &TMass,             "TMass/F");
  fTree->Branch("TWeight",          &EventWeight,       "EventWeight/F");
  fTree->Branch("Tevt",             &Tevt,              "Tevt/l");
  fTree->Branch("Trun",             &Trun,              "Trun/i");
}


void ttHAnalysis::SetSystBranches() {
  fTree->Branch("TWeight_PUUp",         &EventWeight_PUUp,    "EventWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",       &EventWeight_PUDown,  "EventWeight_PUDown/F");
  fTree->Branch("TWeight_TrigUp",       &EventWeight_TrigUp,  "EventWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",     &EventWeight_TrigDown,"EventWeight_TrigDown/F");
  fTree->Branch("TWeight_ElecEffUp",    &EventWeight_ElecUp,  "EventWeight_ElecUp/F");
  fTree->Branch("TWeight_ElecEffDown",  &EventWeight_ElecDown,"EventWeight_ElecDown/F");
  fTree->Branch("TWeight_MuonEffUp",    &EventWeight_MuonUp,  "EventWeight_MuonUp/F");
  fTree->Branch("TWeight_MuonEffDown",  &EventWeight_MuonDown,"EventWeight_MuonDown/F");
  fTree->Branch("THTJESUp",             &THTJESUp,            "THTJESUp/F");
  fTree->Branch("THTJESDown",           &THTJESDown,          "THTJESDown/F");
  fTree->Branch("TMHTJESUp",            &TMHTJESUp,           "TMHTJESUp/F");
  fTree->Branch("TMHTJESDown",          &TMHTJESDown,         "TMHTJESDown/F");
  fTree->Branch("TMETJESUp",            &TMETJESUp,           "TMETJESUp/F");
  fTree->Branch("TMETJESDown",          &TMETJESDown,         "TMETJESDown/F");
  fTree->Branch("TMETLDJESUp",          &TMETLDJESUp,         "TMETLDJESUp/F");
  fTree->Branch("TMETLDJESDown",        &TMETLDJESDown,       "TMETLDJESDown/F");
  fTree->Branch("TnJetsJESUp",          &TnJetsJESUp,         "TnJetsJESUp/F");
  fTree->Branch("TnJetsJESDown",        &TnJetsJESDown,       "TnJetsJESDown/F");
  fTree->Branch("TnLooseBTagsJESUp",    &TnLooseBTagsJESUp,   "TnLooseBTagsJESUp/F");
  fTree->Branch("TnLooseBTagsJESDown",  &TnLooseBTagsJESDown, "TnLooseBTagsJESDown/F");
  fTree->Branch("TnMediumBTagsJESUp",   &TnMediumBTagsJESUp,  "TnMediumBTagsJESUp/F");
  fTree->Branch("TnMediumBTagsJESDown", &TnMediumBTagsJESDown,"TnMediumBTagsJESDown/F");
  fTree->Branch("TCatJESUp",            &TCatJESUp,           "TCatJESUp/I");
  fTree->Branch("TCatJESDown",          &TCatJESDown,         "TCatJESDown/I");
  fTree->Branch("TLHEWeight",           TLHEWeight,           "TLHEWeight[254]/F");
}


void ttHAnalysis::SetMiniTreeVariables() {
  if (PassesPreCuts(TnJetsJESUp, TnLooseBTagsJESUp, TnMediumBTagsJESUp)) {
    if      (Is2lSSEvent(TnJetsJESUp, TMETLDJESUp))     TCatJESUp   = 2;
    else if (Is3lEvent  (TnJetsJESUp, TMETLDJESUp))     TCatJESUp   = 3;
    else if (Is4lEvent())                               TCatJESUp   = 4;
  }
  
  if (PassesPreCuts(TnJetsJESDown, TnLooseBTagsJESDown, TnMediumBTagsJESDown)) {
    if      (Is2lSSEvent(TnJetsJESDown, TMETLDJESDown)) TCatJESDown = 2;
    else if (Is3lEvent  (TnJetsJESDown, TMETLDJESDown)) TCatJESDown = 3;
    else if (Is4lEvent())                               TCatJESDown = 4;
  }
  
  if (PassesPreCuts(nJets, nLooseBTags, nMediumBTags)) {
    if      (Is2lSSEvent(nJets, METLD))                 TCat        = 2;
    else if (Is3lEvent  (nJets, METLD))                 TCat        = 3;
    else if (Is4lEvent())                               TCat        = 4;
  }
  
  if (nTightLepton >= 1) TPtLeading           = TightLepton.at(0).Pt();
  if (nTightLepton >= 2) TPtSubLeading        = TightLepton.at(1).Pt();
  if (nTightLepton >= 3) TPtSubSubLeading     = TightLepton.at(2).Pt();
  if (nTightLepton >= 4) TPtSubSubSubLeading  = TightLepton.at(3).Pt();
  
  TCS             = getCS(TightLepton);
  TMass           = (TightLepton.at(0).p+TightLepton.at(1).p).M();
  if (nTightLepton >= 2) {
    TPtVector.push_back(TightLepton.at(0).Pt());
    TPtVector.push_back(TightLepton.at(1).Pt());
  }
  if (nTightLepton >= 3) {
    TPtVector.push_back(TightLepton.at(2).Pt());
  }
  if (nTightLepton >= 4) {
    TPtVector.push_back(TightLepton.at(3).Pt());
  }
}


////////////////////////////////////////////////////////////////////////////////
//	   Events selection
////////////////////////////////////////////////////////////////////////////////
Bool_t ttHAnalysis::PassesPreCuts(UInt_t njets, UInt_t nloosebtag, UInt_t nmediumbtag) {
  if (nTightLepton < 2)                           return false;
  if (nTaus != 0)                                 return false;
  if (!PassesLowMassLimit(LooseLepton,12))        return false;
  if (njets < 2)                                  return false;
  if ((nloosebtag < 2) && (nmediumbtag < 1))      return false;
  
  return true;
}


Bool_t ttHAnalysis::Is2lSSEvent(UInt_t njets, Float_t metld) {
  if (nTightLepton != 2)                          return false;
  if (!isSS) 					                            return false;
  if (TightLepton.at(0).Pt() < 25)                return false;
  if (TightLepton.at(1).Pt() < 15)                return false;
  if (!PassesTightChargeCuts())                   return false;
  if (njets < 4)                                  return false;

  if (gChannel == iElec) {
    if (abs(ClosestMlltoZ(TightLepton) - Zm) < 10)return false;
    if (metld < 0.2)                              return false;
  }
  
  return true;
}


Bool_t ttHAnalysis::Is3lEvent(UInt_t njets, Float_t metld) {
  if (nTightLepton != 3)                          return false;
  if (TightLepton.at(0).Pt() < 25)                return false;
  if (TightLepton.at(1).Pt() < 15)                return false;
  if (TightLepton.at(2).Pt() < 15)                return false;
  if (abs(ClosestMlltoZ(LooseLepton) - Zm) < 10)  return false;
  if (abs(getCS(TightLepton)) != 1)               return false;
  if (has2OSSFwMlmm(LooseLepton, 140))            return false;
  
  if (njets < 4) {
    if (!hasOSSF(LooseLepton)) {
      if (metld < 0.2)                            return false;
    } 
    else {
      if (metld < 0.3)                            return false;
    }
  }
  
  return true;
}


Bool_t ttHAnalysis::Is4lEvent() {
  if (nTightLepton <= 3)                          return false;
  if (TightLepton.at(0).Pt() < 25)                return false;
  if (TightLepton.at(1).Pt() < 15)                return false;
  if (TightLepton.at(2).Pt() < 15)                return false;
  if (abs(ClosestMlltoZ(LooseLepton) - Zm) < 10)  return false;  
  if (has2OSSFwMlmm(LooseLepton, 140))            return false;
  
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//	   Other methods (get, set, misc...)
////////////////////////////////////////////////////////////////////////////////
void ttHAnalysis::InitialiseVariables() {
  ResetVariables();
  
  Tevt                  = 0;
  Trun                  = 0;
  MET                   = 0;
  for (UInt_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
  
  
  EventWeight           = 0;
  EventWeight_PUUp      = 0;
  EventWeight_PUDown    = 0;
  EventWeight_TrigUp    = 0;
  EventWeight_TrigDown  = 0;
  EventWeight_ElecUp    = 0;
  EventWeight_ElecDown  = 0;
  EventWeight_MuonUp    = 0;
  EventWeight_MuonDown  = 0;
  genWeight             = 0;
}


void ttHAnalysis::ResetVariables() {
  TightLepton.clear();
  FakeableLepton.clear();
  LooseLepton.clear();
  Tau.clear();
  Jets.clear();
  Jets15.clear();
  TPtVector.clear();
  
  nTightLepton        = 0;
  nFakeableLepton     = 0;
  nLooseLepton        = 0;
  nTaus               = 0;
  nJets               = 0;
  nJets15             = 0;
  nMediumBTags        = 0;
  nLooseBTags         = 0;
  gChannel            = 0;
  PUSF                = 0;
  passTrigger         = 0;
  isSS                = 0;
  METLD               = 0;
  MHT                 = 0;
  HT                  = 0;

  TCat                = 0;
  TPtLeading          = 0;
  TPtSubLeading       = 0;
  TPtSubSubLeading    = 0;
  TPtSubSubSubLeading = 0;
  TCS                 = 0;
  TMass               = 0;
  
  THTJESUp            = 0;
  THTJESDown          = 0;
  TMHTJESUp           = 0;
  TMHTJESDown         = 0;
  TMETJESUp           = 0;
  TMETJESDown         = 0;
  TMETLDJESUp         = 0;
  TMETLDJESDown       = 0;
  TnJetsJESUp         = 0;
  TnJetsJESDown       = 0;
  TnLooseBTagsJESUp   = 0;
  TnLooseBTagsJESDown = 0;
  TnMediumBTagsJESUp  = 0;
  TnMediumBTagsJESDown= 0;
  TCatJESUp           = 0;
  TCatJESDown         = 0;
}


void ttHAnalysis::Initialise3l4lLeptonSF() {
  Lep3l4lSF = new LeptonSF(gLocalPath + "/InputFiles/");
  Lep3l4lSF->loadHisto(iMuonlepMVA3l4lttH);
  Lep3l4lSF->loadHisto(iEleclepMVA3l4lttH);
  Lep3l4lSF->loadHisto(iMuonReco);
  Lep3l4lSF->loadHisto(iMuonLooseTracksttH);
  Lep3l4lSF->loadHisto(iMuonLooseMiniIsottH);
  Lep3l4lSF->loadHisto(iMuonTightIP2DttH);
  Lep3l4lSF->loadHisto(iElecTightIP2DM17ttH);
  Lep3l4lSF->loadHisto(iElecMini4M17ttH);
  Lep3l4lSF->loadHisto(iElecConvVetoM17ttH);
}


void ttHAnalysis::Reset3l4lLeptonSF() {
  if (nTightLepton >= 3) {
    for (UInt_t i = 0; i < nTightLepton; i++) {
      TightLepton.at(i).SetSF(   Lep3l4lSF->GetLeptonSF(     TightLepton.at(i).Pt(), TightLepton.at(i).Eta(), TightLepton.at(i).type));
      TightLepton.at(i).SetSFerr(Lep3l4lSF->GetLeptonSFerror(TightLepton.at(i).Pt(), TightLepton.at(i).Eta(), TightLepton.at(i).type));
    }
  }
}


void ttHAnalysis::GetParameters() {
  // Import essential and global variables of the execution
  gIsData     =	GetParam<Bool_t>("IsData");
  gSampleName =	GetParam<TString>("sampleName");
  gWeight     =	GetParam<Float_t>("weight"); // cross section / events in the sample
  gIsMCatNLO  =	GetParam<Bool_t>("IsMCatNLO");
  gLocalPath  = GetParam<TString>("WorkingDir");
  
  gIsLHE       = false;
  if (gSampleName.Contains("TTWToLNu")    || gSampleName.Contains("TTWToQQ") ||
      gSampleName.Contains("TTZToLLNuNu") || gSampleName.Contains("TTZToQQ") ||
      gSampleName.Contains("TTHNonbb")) gIsLHE = true;
  if (gIsLHE) cout << "+ The sample is tagged for pdf and scale systs" << endl;
}


void ttHAnalysis::GetEventVariables() {
  // Clean all event variables (also the minitree ones)
  ResetVariables();

  // Import event-dependent variables
  TightLepton     = GetParam<vector<Lepton>>("selLeptons");
  FakeableLepton  = GetParam<vector<Lepton>>("vetoLeptons");
  LooseLepton     = GetParam<vector<Lepton>>("looseLeptons");
  Tau             = GetParam<vector<Lepton>>("selTaus");
  Jets            = GetParam<vector<Jet>>("selJets");
  Jets15          = GetParam<vector<Jet>>("Jets15");

  nTightLepton    = GetParam<UInt_t>("nSelLeptons");
  nFakeableLepton = GetParam<UInt_t>("nVetoLeptons");
  nLooseLepton    = GetParam<UInt_t>("nLooseLeptons");
  nTaus           = GetParam<UInt_t>("nSelTaus");
  nJets           = GetParam<UInt_t>("nSelJets");
  nJets15         = GetParam<UInt_t>("nJets15");

  gChannel        = GetParam<Int_t>("gChannel");
  PUSF         	  =	GetParam<Float_t>("PUSF");
  PUSF_Up      	  =	GetParam<Float_t>("PUSF_Up");
  PUSF_Down    	  =	GetParam<Float_t>("PUSF_Down");
  passTrigger     = GetParam<Bool_t>("passTrigger");
  isSS            = GetParam<Bool_t>("isSS");
  
  TMETJESUp       = GetParam<Float_t>("MET_JESUp");
  TMETJESDown     = GetParam<Float_t>("MET_JESDown");
  
  // Obtain the rest of the needed variables
  nMediumBTags    = GetnMediumBTags(Jets);
  nLooseBTags     = GetnLooseBTags(Jets);
  MHT             = getMHT(FakeableLepton,Jets);
  HT              = getHT(Jets);
  METLD           = getMETLD(MET,MHT);
  
  GetJESJetVariables();
}


void ttHAnalysis::CalculateWeight() {
  EventWeight           = 1;
  EventWeight_PUUp      = 1;
  EventWeight_PUDown    = 1;
  EventWeight_TrigUp    = 1;
  EventWeight_TrigDown  = 1;
  EventWeight_ElecUp    = 1;
  EventWeight_ElecDown  = 1;
  EventWeight_MuonUp    = 1;
  EventWeight_MuonDown  = 1;
  
  if (!gIsData) {
    Float_t ElecSF = 1; Float_t MuonSF = 1;
    Float_t ElecSF_Up = 1; Float_t ElecSF_Down = 1;
    Float_t MuonSF_Up = 1; Float_t MuonSF_Down = 1;
    if (nTightLepton != 0) {
      for (UInt_t i = 0; i < nTightLepton; i++) {
        if (TightLepton.at(i).isMuon) {
          MuonSF      *= TightLepton.at(i).GetSF( 0);
          MuonSF_Up   *= TightLepton.at(i).GetSF(+1);
          MuonSF_Down *= TightLepton.at(i).GetSF(-1);
        }
        else {
          ElecSF      *= TightLepton.at(i).GetSF( 0);
          ElecSF_Up   *= TightLepton.at(i).GetSF(+1);
          ElecSF_Down *= TightLepton.at(i).GetSF(-1);
        }
      }
    }
    
    Float_t Trig = 1; Float_t Trig_Up = 1; Float_t Trig_Down = 1;
    if (nTightLepton <= 2) {
      if      (gChannel == iElec) {
        Trig      = 1.01;
        Trig_Up   = 1.03;
        Trig_Down = 0.99;
      }
      else if (gChannel == iMuon) {
        Trig      = 1.00;
        Trig_Up   = 1.01;
        Trig_Down = 0.99;
      }
      else if (gChannel == iElMu) {
        Trig      = 1.01;
        Trig_Up   = 1.02;
        Trig_Down = 1.00;
      }
    }
    else {
      Trig      = 1.00;
      Trig_Up   = 1.03;
      Trig_Down = 0.97;      
    }
    
    EventWeight           *= gWeight*Trig*PUSF*ElecSF*MuonSF;
    EventWeight_PUUp      *= gWeight*Trig*PUSF_Up*ElecSF*MuonSF;
    EventWeight_PUDown    *= gWeight*Trig*PUSF_Down*ElecSF*MuonSF;
    EventWeight_TrigUp    *= gWeight*Trig_Up*PUSF*ElecSF*MuonSF;
    EventWeight_TrigDown  *= gWeight*Trig_Down*PUSF*ElecSF*MuonSF;
    EventWeight_ElecUp    *= gWeight*Trig*PUSF*ElecSF_Up*MuonSF;
    EventWeight_ElecDown  *= gWeight*Trig*PUSF*ElecSF_Down*MuonSF;
    EventWeight_MuonUp    *= gWeight*Trig*PUSF*ElecSF*MuonSF_Up;
    EventWeight_MuonDown  *= gWeight*Trig*PUSF*ElecSF*MuonSF_Down;
    
    if (gIsMCatNLO) {
      EventWeight           *= genWeight;
      EventWeight_PUUp      *= genWeight;
      EventWeight_PUDown    *= genWeight;
      EventWeight_TrigUp    *= genWeight;
      EventWeight_TrigDown  *= genWeight;
      EventWeight_ElecUp    *= genWeight;
      EventWeight_ElecDown  *= genWeight;
      EventWeight_MuonUp    *= genWeight;
      EventWeight_MuonDown  *= genWeight;
    }
  }
}


UInt_t ttHAnalysis::GetnMediumBTags(vector<Jet> jets) {
  UInt_t nmediumbtag = 0;
  for(UInt_t i = 0; i < jets.size(); i++) {
    if (jets.at(i).csv > 0.8484) nmediumbtag++;
  }
  return nmediumbtag;
}


UInt_t ttHAnalysis::GetnLooseBTags(vector<Jet> jets) {
  UInt_t nloosebtag = 0;
  for(UInt_t i = 0; i < jets.size(); i++) {
    if (jets.at(i).csv > 0.5426) nloosebtag++;
  }
  return nloosebtag;
}


Bool_t ttHAnalysis::PassesTightChargeCuts() {
  for (UInt_t i = 0; i < 2; i++) {
    if (TightLepton.at(i).isMuon) {
      if (Get<Int_t>("LepGood_tightCharge",i) < 1) return false;
    }
    else {
      if (Get<Int_t>("LepGood_tightCharge",i) < 2) return false;      
    }
  }
  return true;
}


void ttHAnalysis::GetJESJetVariables() {
  std::vector<Jet> tmp_JetsJESUp;
  std::vector<Jet> tmp_JetsJESDown;
  tmp_JetsJESUp.clear();
  tmp_JetsJESDown.clear();
  
  for(UInt_t i = 0; i < nJets15; i++) {
    if (Jets15.at(i).pTJESUp > 25) {
      tmp_JetsJESUp.push_back(Jets15.at(i));
      tmp_JetsJESUp.back().p.SetPtEtaPhiE(tmp_JetsJESUp.back().pTJESUp,tmp_JetsJESUp.back().p.Eta(),tmp_JetsJESUp.back().Phi(),tmp_JetsJESUp.back().E());
    }
    if (Jets15.at(i).pTJESDown > 25) {
      tmp_JetsJESDown.push_back(Jets15.at(i));
      tmp_JetsJESDown.back().p.SetPtEtaPhiE(tmp_JetsJESDown.back().pTJESDown,tmp_JetsJESDown.back().p.Eta(),tmp_JetsJESDown.back().Phi(),tmp_JetsJESDown.back().E());
    }
  }
  
  TnJetsJESUp           = tmp_JetsJESUp.size();
  TnJetsJESDown         = tmp_JetsJESDown.size();
  TnLooseBTagsJESUp     = GetnLooseBTags(tmp_JetsJESUp);
  TnLooseBTagsJESDown   = GetnLooseBTags(tmp_JetsJESDown);
  TnMediumBTagsJESUp    = GetnMediumBTags(tmp_JetsJESUp);
  TnMediumBTagsJESDown  = GetnMediumBTags(tmp_JetsJESDown);
  TMHTJESUp             = getMHT(FakeableLepton,tmp_JetsJESUp);
  TMHTJESDown           = getMHT(FakeableLepton,tmp_JetsJESDown);
  TMETLDJESUp           = getMETLD(TMETJESUp,TMHTJESUp);
  TMETLDJESDown         = getMETLD(TMETJESDown,TMHTJESDown);
  THTJESUp              = getHT(tmp_JetsJESUp);
  THTJESDown            = getHT(tmp_JetsJESDown);
}
