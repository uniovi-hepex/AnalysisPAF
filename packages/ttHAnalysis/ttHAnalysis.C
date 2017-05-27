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
  
  // Check trigger and precuts
  if (!PassesPreCuts()) return;
  if (!passTrigger)     return;
  
  SetMiniTreeVariables();
  
  Reset3l4lLeptonSF();
  CalculateWeight(); 
  
  // Set branches and fill minitree
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
  MET   = 0;
  Tevt  = 0;
  Trun  = 0;
  
  if (!gIsData){
    genWeight   = Get<Float_t>("genWeight");
  }

  MET         = Get<Float_t>("met_pt");
  Tevt        = Get<Long_t>("evt");
  Trun        = Get<Long_t>("run");
}

// Minitree methods
//------------------------------------------------------------------------------
void ttHAnalysis::SetLeptonBranches() {
  fTree->Branch("TnTightLepton",    &nTightLepton,      "nTightLepton/I");
  fTree->Branch("TnFakeableLepton", &nFakeableLepton,   "nFakeableLepton/I");
  fTree->Branch("TnLooseLepton",    &nLooseLepton,      "nLooseLepton/I");
  fTree->Branch("TnTaus",           &nTaus,             "nTaus/I");
  fTree->Branch("TPtLeading",       &TPtLeading,        "TPtLeading/F");
  fTree->Branch("TPtSubLeading",    &TPtSubLeading,     "TPtSubLeading/F");
  fTree->Branch("TPtSubSubLeading", &TPtSubSubLeading,  "TPtSubSubLeading/F");
  fTree->Branch("TPtVector", 				&TPtVector);
}

void ttHAnalysis::SetJetBranches() {
  fTree->Branch("TnMediumBTags" ,   &nMediumBTags,      "nMediumBTags/I");
  fTree->Branch("TnLooseBTags",     &nLooseBTags,       "nLooseBTags/I");
  fTree->Branch("TnJets",           &nJets,             "nJets/I");
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
  fTree->Branch("TWeight_PUUp",     &EventWeight_PUUp,  "EventWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",   &EventWeight_PUDown,"EventWeight_PUDown/F");
  fTree->Branch("Tevt",             &Tevt,              "Tevt/L");
  fTree->Branch("Trun",             &Trun,              "Trun/L");
}

void ttHAnalysis::SetMiniTreeVariables() {
  if      (Is2lSSEvent())       TCat  = 2;
  else if (Is3lEvent())         TCat  = 3;
  else if (Is4lEvent())         TCat  = 4;
  
  if (nTightLepton >= 1) TPtLeading       = TightLepton.at(0).Pt();
  if (nTightLepton >= 2) TPtSubLeading    = TightLepton.at(1).Pt();
  if (nTightLepton > 2)  TPtSubSubLeading = TightLepton.at(2).Pt();
  
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
Bool_t ttHAnalysis::PassesPreCuts() {
  if (nTightLepton < 2)                     return false;
  if (nTaus != 0)                           return false;
  if (!PassesLowMassLimit(LooseLepton,12))  return false;
  if (nJets < 2)                            return false;
  
  if (nLooseBTags < 2) {
    if (nMediumBTags < 1)                   return false;
  }
  
  return true;
}

Bool_t ttHAnalysis::Is2lSSEvent() {
  if (nTightLepton != 2)            return false;
  if (!isSS) 					              return false;
  if (TightLepton.at(0).Pt() < 25)  return false;
  if (!PassesTightChargeCuts())     return false;

  if (nJets < 4)                    return false;

  if (gChannel == iElec) {
    if (abs((TightLepton.at(0).p + TightLepton.at(1).p).M() - Zm) < 10) return false;
    if (METLD < 0.2)                return false;
  }
  return true;
}

Bool_t ttHAnalysis::Is3lEvent() {
  if (nTightLepton != 3)            return false;
  if (TightLepton.at(0).Pt() < 25)  return false;
  if (abs(ClosestMlltoZ(LooseLepton) - Zm) < 10) return false;
  
  if (nJets < 4) {
    if (!hasOSSF(TightLepton)) {
      if (METLD < 0.2) return false;
    } 
    else {
      if (METLD < 0.3) return false;
    }
  }
  
  if (abs(getCS(TightLepton)) != 1) return false;
  if (has2OSSFwMlmm(LooseLepton, 140)) return false;
  
  return true;
}

Bool_t ttHAnalysis::Is4lEvent() {
  if (nTightLepton <= 3)           return false;
  if (TightLepton.at(0).Pt() < 25) return false;
  if (abs(ClosestMlltoZ(LooseLepton) - Zm) < 10) return false;  
  
  if (nJets < 4) {
    if (!hasOSSF(TightLepton)) {
      if (METLD < 0.2) return false;
    } 
    else {
      if (METLD < 0.3) return false;
    }
  }

  if (abs(TightLepton.at(0).charge + TightLepton.at(1).charge + TightLepton.at(2).charge) != 1) return false;
  if (has2OSSFwMlmm(LooseLepton, 140)) return false;
  
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//	   Other methods (get, set, misc...)
////////////////////////////////////////////////////////////////////////////////
void ttHAnalysis::InitialiseVariables() {
  TightLepton.clear();
  FakeableLepton.clear();
  LooseLepton.clear();
  Tau.clear();
  Jets.clear();
  TPtVector.clear();
  
  nTightLepton    = 0;
  nFakeableLepton = 0;
  nLooseLepton    = 0;
  nTaus           = 0;
  nJets           = 0;
  nMediumBTags    = 0;
  nLooseBTags     = 0;
  gChannel        = 0;
  PUSF           = 0;
  passTrigger     = 0;
  isSS            = 0;
  MET             = 0;
  METLD           = 0;
  MHT             = 0;
  HT              = 0;

  TCat            = 0;
  TPtLeading      = 0;
  TPtSubLeading   = 0;
  TPtSubSubLeading= 0;
  TCS             = 0;
  TMass           = 0;
  Tevt            = 0;
  Trun            = 0;
  
  gSampleName	    =	"0";
  gIsData         =	0;
  gWeight         =	0;
  gIsMCatNLO      =	0;
  
  EventWeight         = 0;
  EventWeight_PUUp    = 0;
  EventWeight_PUDown  = 0;
  
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
  if (nTightLepton < 3) return;
  for (UInt_t i = 0; i < nTightLepton; i++) {
    TightLepton.at(i).SetSF(   Lep3l4lSF->GetLeptonSF(     TightLepton.at(i).Pt(), TightLepton.at(i).Eta(), TightLepton.at(i).type) );
    TightLepton.at(i).SetSFerr(Lep3l4lSF->GetLeptonSFerror(TightLepton.at(i).Pt(), TightLepton.at(i).Eta(), TightLepton.at(i).type) );
  }
}


void ttHAnalysis::GetParameters() {
  // Import essential and global variables of the execution
  gSampleName	=	GetParam<TString>("sampleName");
  gIsData     =	GetParam<Bool_t>("IsData");
  gWeight     =	GetParam<Float_t>("weight"); // cross section / events in the sample
  gIsMCatNLO  =	GetParam<Bool_t>("IsMCatNLO");
  gLocalPath  = GetParam<TString>("WorkingDir");
}


void ttHAnalysis::GetEventVariables() {
  // Clean all event variables (also the minitree ones)
  TightLepton.clear();
  FakeableLepton.clear();
  LooseLepton.clear();
  Tau.clear();
  Jets.clear();
  TPtVector.clear();
  
  nTightLepton    = 0;
  nFakeableLepton = 0;
  nLooseLepton    = 0;
  nTaus           = 0;
  nJets           = 0;
  nMediumBTags    = 0;
  nLooseBTags     = 0;
  gChannel        = 0;
  PUSF            = 0;
  passTrigger     = 0;
  isSS            = 0;
  METLD           = 0;
  MHT             = 0;
  HT              = 0;

  TCat            = 0;
  TPtLeading      = 0;
  TPtSubLeading   = 0;
  TPtSubSubLeading= 0;
  TCS             = 0;
  TMass           = 0;

  // Import event-dependent variables
  TightLepton     = GetParam<vector<Lepton>>("selLeptons");
  FakeableLepton  = GetParam<vector<Lepton>>("vetoLeptons");
  LooseLepton     = GetParam<vector<Lepton>>("looseLeptons");
  Tau             = GetParam<vector<Lepton>>("selTaus");
  Jets            = GetParam<vector<Jet>>("selJets");

  nTightLepton    = GetParam<UInt_t>("nSelLeptons");
  nFakeableLepton = GetParam<UInt_t>("nVetoLeptons");
  nLooseLepton    = GetParam<UInt_t>("nLooseLeptons");
  nTaus           = GetParam<UInt_t>("nSelTaus");
  nJets           = GetParam<UInt_t>("nSelJets");

  gChannel        = GetParam<Int_t>("gChannel");
  PUSF         	  =	GetParam<Float_t>("PUSF");
  PUSF_Up      	  =	GetParam<Float_t>("PUSF_Up");
  PUSF_Down    	  =	GetParam<Float_t>("PUSF_Down");
  passTrigger     = GetParam<Bool_t>("passTrigger");
  isSS            = GetParam<Bool_t>("isSS");
  
  nMediumBTags    = GetnMediumBTags();
  nLooseBTags     = GetnLooseBTags();
  MHT             = getMHT(FakeableLepton,Jets);
  HT              = getHT(Jets);
  METLD           = getMETLD(MET,MHT);

}


void ttHAnalysis::CalculateWeight() {
  EventWeight 	      = 1.;
  EventWeight_PUUp 	  = 1.;
  EventWeight_PUDown 	= 1.;
  
  if (!gIsData) {
    // First: normalization and PU weight.
    EventWeight         = gWeight*PUSF;
    EventWeight_PUUp    = gWeight*PUSF_Up;
    EventWeight_PUDown  = gWeight*PUSF_Down;
    
    // Second: leptons weight.
    if (TCat >= 2) {
      Float_t ElecSF = 1; Float_t MuonSF = 1;
      Float_t ElecSF_Up = 1; Float_t ElecSF_Down = 1;
			Float_t MuonSF_Up = 1; Float_t MuonSF_Down = 1;
      
      if (TightLepton.at(0).isMuon) {
        MuonSF  *= TightLepton.at(0).GetSF( 0);
      }
      else {
        ElecSF  *= TightLepton.at(0).GetSF( 0);
      }
      if (TightLepton.at(1).isMuon) {
        MuonSF  *= TightLepton.at(1).GetSF( 0);
      }
      else {
        ElecSF  *= TightLepton.at(1).GetSF( 0);
      }
      if (TCat >= 3) {
        if (TightLepton.at(2).isMuon) {
          MuonSF  *= TightLepton.at(2).GetSF( 0);
        }
        else {
          ElecSF  *= TightLepton.at(2).GetSF( 0);
        }
        if (TCat == 4) {
          for (UInt_t i = 3; i < nTightLepton; i++) {
            if (TightLepton.at(i).isMuon) {
              MuonSF  *= TightLepton.at(i).GetSF( 0);
            }
            else {
              ElecSF  *= TightLepton.at(i).GetSF( 0);
            }
          }
        }
      }
      EventWeight         *= ElecSF*MuonSF;
      EventWeight_PUUp    *= ElecSF*MuonSF;
      EventWeight_PUDown  *= ElecSF*MuonSF;
    }
    
    if (gIsMCatNLO) {
      EventWeight         *= genWeight;
      EventWeight_PUUp    *= genWeight;
      EventWeight_PUDown  *= genWeight;
    }
  }
}


UInt_t ttHAnalysis::GetnMediumBTags() {
  UInt_t nmediumbtag = 0;
  for(UInt_t i = 0; i < nJets; i++) {
    if (Jets.at(i).csv > 0.8484) nmediumbtag++;
  }
  return nmediumbtag;
}


UInt_t ttHAnalysis::GetnLooseBTags() {
  UInt_t nloosebtag = 0;
  for(UInt_t i = 0; i < nJets; i++) {
    if (Jets.at(i).csv > 0.5426) nloosebtag++;
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
