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

  cout << endl;
	PAF_INFO("ttHAnalysis", "+ Initialization DONE");
	cout << endl;
	PAF_INFO("ttHAnalysis", "======================================== Analysis");
}

void ttHAnalysis::InsideLoop() {
	// Get and set data members
	GetTreeVariables();
  GetEventVariables();
  
  SetMiniTreeVariables();
  
  CalculateWeight();
  
  // Check trigger and precuts
  if (!PassesPreCuts()) return;
  if (!passTrigger)     return;
  
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
  
  if (!gIsData){
		genWeight   = Get<Float_t>("genWeight");
	}

  MET   = Get<Float_t>("met_pt");
  Tevt  = Get<Long_t>("evt");
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
  fTree->Branch("Tevt",             &Tevt,              "Tevt/L");
}

void ttHAnalysis::SetMiniTreeVariables() {
  if      (Is2lSSEvent())       TCat  = 2;
  else if (Is3lEvent())         TCat  = 3;
  else if (Is4lEvent())         TCat  = 4;
  
  TPtLeading      = TightLepton.at(0).Pt();
  TPtSubLeading   = TightLepton.at(1).Pt();
  if (TightLepton.size() > 2) TPtSubSubLeading = TightLepton.at(2).Pt();
  
  TCS             = GetCS();
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
	if (nTightLepton < 2)          return false;
  if (nTaus != 0)								 return false;
  
	for (Int_t i = 0; i < nLooseLepton; i++) {
		for (Int_t j = i+1; j < nLooseLepton; j++) {
		  if ((LooseLepton.at(i).p + LooseLepton.at(j).p).M() < 12) return false;
	  }
  }

	if (nJets < 2)                 return false;
	if (nLooseBTags < 2) {
		if (nMediumBTags < 1)          return false;
	}
	return true;
}

Bool_t ttHAnalysis::Is2lSSEvent() {
	if (!isSS) 					              return false;
	if (TightLepton.at(0).Pt() < 25)  return false;
	if (TightLepton.at(1).Pt() < 15)  return false;
	if (nTightLepton > 2) {
    if (TightLepton.at(2).Pt() > 10)  return false;
  }

	if (nJets < 4) 					          return false;

	if (gChannel == iElec) {
		if (abs((TightLepton.at(0).p + TightLepton.at(1).p).M() - Zm) < 10) return false;
		if (METLD < 0.2)                return false;
	}
	return true;
}

Bool_t ttHAnalysis::Is3lEvent() {
	if (nTightLepton != 3)         return false;
	if (TightLepton.at(0).Pt() < 25)  return false;
	if (TightLepton.at(1).Pt() < 15)  return false;
	if (TightLepton.at(2).Pt() < 15)  return false;

	for (Int_t i = 0; i < nLooseLepton; i++) {
		for (Int_t j = i+1; j < nLooseLepton; j++) {
			if (LooseLepton.at(i).type != LooseLepton.at(j).type) continue;
			if (LooseLepton.at(i).charge*LooseLepton.at(j).charge > 0) continue;
			if (abs((LooseLepton.at(i).p+LooseLepton.at(j).p).M() - Zm) < 10) return false;
		}
	}

	Int_t OSSF = 0;
	for (Int_t i = 0; i < nTightLepton; i++) {
		for (Int_t j = i+1; j < nTightLepton; j++) {
			if (TightLepton.at(i).type != TightLepton.at(j).type) continue;
			if (TightLepton.at(i).charge*TightLepton.at(j).charge < 0) OSSF = 1;
		}
	}

	if (OSSF != 1 && nJets < 4) {
		if (METLD < 0.2) return false;
	} else if (nJets < 4) {
		if (METLD < 0.3) return false;
	}

	if (abs(TightLepton.at(0).charge + TightLepton.at(1).charge + TightLepton.at(2).charge) != 1) return false;
  
	std::vector<TLorentzVector> OSSFpair;
  OSSFpair	  = std::vector<TLorentzVector>();
  for (Int_t i = 0; i < nLooseLepton; i++) {
		for (Int_t j = i+1; j < nLooseLepton; j++) {
			if (LooseLepton.at(i).type != LooseLepton.at(j).type)      continue;
			if (LooseLepton.at(i).charge*LooseLepton.at(j).charge > 0) continue;
      OSSFpair.push_back(LooseLepton.at(i).p+LooseLepton.at(j).p);
		}
	}
	
	for (UInt_t i = 0; i < OSSFpair.size(); i++) {
		for (UInt_t j = i+1; j < OSSFpair.size(); j++) {
			if ((OSSFpair.at(i)+OSSFpair.at(j)).M() < 140) return false;
		}
	}
	
	return true;
}

Bool_t ttHAnalysis::Is4lEvent() {
  if (nTightLepton <= 3)        return false;
	if (TightLepton.at(0).Pt() < 25) return false;
	if (TightLepton.at(1).Pt() < 15) return false;
	if (TightLepton.at(2).Pt() < 15) return false;
	if (TightLepton.at(3).Pt() < 10) return false;

	for (Int_t i = 0; i < nLooseLepton; i++) {
		for (Int_t j = i+1; j < nLooseLepton; j++) {
			if (LooseLepton.at(i).type != LooseLepton.at(j).type) continue;
			if (LooseLepton.at(i).charge*LooseLepton.at(j).charge > 0) continue;
			if (abs((LooseLepton.at(i).p+LooseLepton.at(j).p).M() - Zm) < 10) return false;
		}
	}

	Int_t OSSF = 0;
	for (Int_t i = 0; i < nTightLepton; i++) {
		for (Int_t j = i+1; j < nTightLepton; j++) {
			if (TightLepton.at(i).type != TightLepton.at(j).type) continue;
			if (TightLepton.at(i).charge*TightLepton.at(j).charge < 0) OSSF = 1;
		}
	}

	if (OSSF != 1 && nJets < 4) {
		if (METLD < 0.2) return false;
	} else if (nJets < 4) {
		if (METLD < 0.3) return false;
	}

	if (abs(TightLepton.at(0).charge + TightLepton.at(1).charge + TightLepton.at(2).charge) != 1) return false;

	std::vector<TLorentzVector> OSSFpair;
	OSSFpair	  = std::vector<TLorentzVector>();
  for (Int_t i = 0; i < nLooseLepton; i++) {
		for (Int_t j = i+1; j < nLooseLepton; j++) {
			if (LooseLepton.at(i).type != LooseLepton.at(j).type)      continue;
			if (LooseLepton.at(i).charge*LooseLepton.at(j).charge > 0) continue;
      OSSFpair.push_back(LooseLepton.at(i).p+LooseLepton.at(j).p);
		}
	}
	for (UInt_t i = 0; i < OSSFpair.size(); i++) {
		for (UInt_t j = i+1; j < OSSFpair.size(); j++) {
			if ((OSSFpair.at(i)+OSSFpair.at(j)).M() < 140) return false;
		}
	}
		
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
  gPUSF           = 0;
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
  
  gSampleName	    =	"0";
  gIsData         =	0;
  gWeight         =	0;
	gIsMCatNLO      =	0;
  
  EventWeight     = 0;
  
}


void ttHAnalysis::GetParameters() {
  // Import essential and global variables of the execution
  gSampleName	    =	GetParam<TString>("sampleName");
  gIsData         =	GetParam<Bool_t>("IsData");
  gWeight         =	GetParam<Float_t>("weight"); // cross section / events in the sample
	gIsMCatNLO      =	GetParam<Bool_t>("IsMCatNLO");
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
  gPUSF           = 0;
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

  nTightLepton    = GetParam<Int_t>("nSelLeptons");
  nFakeableLepton = GetParam<Int_t>("nVetoLeptons");
  nLooseLepton    = GetParam<Int_t>("nLooseLeptons");
  nTaus           = GetParam<Int_t>("nSelTaus");
  nJets           = GetParam<Int_t>("nSelJets");

  gChannel        = GetParam<Int_t>("gChannel");
  gPUSF         	=	GetParam<Float_t>("PUSF");
  passTrigger     = GetParam<Bool_t>("passTrigger");
  isSS            = GetParam<Bool_t>("isSS");
  
  nMediumBTags    = GetnMediumBTags();
  nLooseBTags     = GetnLooseBTags();
  MHT             = GetMHT();
  HT              = GetHT();
  METLD           = GetMETLD();

}


void ttHAnalysis::CalculateWeight() {
  EventWeight 	= 1.;
  
  if (!gIsData) {
    // First: normalization and PU weight.
    EventWeight = gWeight*gPUSF;
    
    // Second: leptons weight.
    if (TCat >= 2) {
      Float_t ElecSF = 1; Float_t MuonSF = 1;
      
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
      	  for (UInt_t i = 3; i < TightLepton.size(); i++) {
            if (TightLepton.at(i).isMuon) {
              MuonSF  *= TightLepton.at(i).GetSF( 0);
            }
            else {
              ElecSF  *= TightLepton.at(i).GetSF( 0);
            }
          }
        }
      }
      EventWeight *= ElecSF*MuonSF;
    }
    
    if (gIsMCatNLO) EventWeight *= genWeight;
  }
}


Float_t ttHAnalysis::GetMETLD() {
	Float_t metld;
	metld = MET * 0.00397 + MHT * 0.00265;
	return metld;
}


Float_t ttHAnalysis::GetHT() {
	Float_t ht = 0;
  for (UInt_t i = 0; i < Jets.size(); i++){
    ht += Jets.at(i).Pt();
  } 
	return ht;
}


Float_t ttHAnalysis::GetMHT() {
	Float_t mht = 0;
  TLorentzVector vectemp(0.,0.,0.,0.);
  for (UInt_t i = 0; i < Jets.size(); i++){
    vectemp += Jets.at(i).p;
  }
  for (UInt_t i = 0; i < FakeableLepton.size(); i++){
    vectemp += FakeableLepton.at(i).p;
  }
  vectemp = -vectemp;
  mht = vectemp.Pt();
	return mht;
}


Int_t ttHAnalysis::GetCS() {
	Int_t cs = 0;
	for (UInt_t i = 0; i < TightLepton.size(); i++) {
		cs += TightLepton.at(i).charge;
	}
	return cs;
}


Int_t ttHAnalysis::GetnMediumBTags(){
  Int_t nmediumbtag = 0;
  for(UInt_t i = 0; i < Jets.size(); i++) {
    if (Jets.at(i).csv > 0.8484) nmediumbtag++;
  }
  return nmediumbtag;
}


Int_t ttHAnalysis::GetnLooseBTags(){
  Int_t nloosebtag = 0;
  for(UInt_t i = 0; i < Jets.size(); i++) {
    if (Jets.at(i).csv > 0.5426) nloosebtag++;
  }
  return nloosebtag;
}
