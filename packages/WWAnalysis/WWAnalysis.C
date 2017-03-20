#include "WWAnalysis.h"

ClassImp(WWAnalysis);
WWAnalysis::WWAnalysis() : PAFChainItemSelector() {
	//inicializar las variables definidas en el WWanalysis.h
	fTree = 0;

	TrigSF = 0; TrigSF_Up = 0; TrigSF_Down = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0;
	gChannel = 0; passMETfilters = 0; passTrigger = 0; TIsSS = 0;  NormWeight = 0; TWeight = 0;
	TMll = 0;  TPtdil = 0; TMET = 0; TMET_Phi = 0; TNJets = 0; TNBtags = 0; THT = 0; 
	TNVetoLeps = 0; TNSelLeps = 0; TChannel = 0;
	TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJER = 0;
	TNBtagsUp = 0; TNBtagsDown = 0; TNBtagsMisTagUp = 0; TNBtagsMisTagDown = 0;
	THTJESUp = 0; THTJESDown = 0; TNISRJets = 0;
	TMETJESUp = 0; TMETJESDown = 0; TMT2llJESUp = 0; TMT2llJESDown = 0; TMT2 = 0; TMT = 0; TMTprime = 0;
	TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
	TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;  TNVetoJets = 0;
	TDeltaPhi = 0;
	
	for(Int_t i = 0; i < 10; i++){
		TLep_Pt[i] = 0;  
		TLep_Eta[i] = 0;
		TLep_Phi[i] = 0;
		TLep_E[i] = 0;
		TLep_Charge[i] = 0;
	}
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
}
void WWAnalysis::Summary(){}

void WWAnalysis::Initialise(){
	gIsData      = GetParam<Bool_t>("IsData");
	gSelection   = GetParam<Int_t>("iSelection");
	gSampleName  = GetParam<TString>("sampleName");
	gDoSyst      = GetParam<Bool_t>("doSyst");

	fTree = CreateTree("tree","Created with PAF");

	SetLeptonVariables();
	SetJetVariables();
	SetEventVariables();

	selLeptons  = std::vector<Lepton>();
	vetoLeptons = std::vector<Lepton>();
	selJets = std::vector<Jet>();
	Jets15  = std::vector<Jet>();
	vetoJets = std::vector<Jet>();
	nVetoJets = 0;
}

void WWAnalysis::InsideLoop(){
  // Vectors with the objects
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  vetoJets    = GetParam<vector<Jet>>("vetoJets");
  nVetoJets   = GetParam<Int_t>("nVetoJets");

  // Weights and SFs
  NormWeight   = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSF_Up    = GetParam<Float_t>("TriggerSF_Up");
  TrigSF_Down  = GetParam<Float_t>("TriggerSF_Down");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");

  // Event variables
	gChannel        = GetParam<Int_t>("gChannel");
	passMETfilters  = GetParam<Bool_t>("METfilters");
	passTrigger     = GetParam<Bool_t>("passTrigger");
	TIsSS           = GetParam<Bool_t>("isSS");
	
  // Leptons and Jets
  GetLeptonVariables(selLeptons, vetoLeptons);
  GetJetVariables(selJets, Jets15);
	GetMET();


// ===================================================================================================================
	// nivel más básico: dileptónico veto tercer lepton, Mll > 12, dos leptones, passes Trigger y MET y no cumplen mismo signo (SameSing)
	
	if(TNSelLeps == 2 && passTrigger && passMETfilters){ // 2 leptons, OS
		if(TNVetoLeps < 3){  // Veto to 3rd lepton

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
			// utilizando minitrees aquí pongo el nivel más básico de seleccion de eventos. Será en el plotter donde escogeremos más a fondo.
			
						
						
						// analisis de cosas básicas de Top:
			//if( ((selLeptons.at(0).p +selLeptons.at(1).p).M() > 20) // mll > 20 GeV
				//&& (gChannel == 1 || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  )//  Z Veto in ee, µµ
				//&& TNJets > 1      
				//&& TNBtags > 0 ){      
				//fTree->Fill();
			//};
						// analisis de cosas básicas de WW:
			if( ((selLeptons.at(0).p +selLeptons.at(1).p).M() > 12) // mll > 12 GeV
				&& (gChannel == 1 || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15))){ //Channel e mu y con Z Veto in ee, µµ
				fTree->Fill();
			};
		};
	};
};


//#####################################################################
// Functions
//------------------------------------------------------------------

void WWAnalysis::SetLeptonVariables(){
	fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
	fTree->Branch("TNSelLeps",     &TNSelLeps,     	  "TNSelLeps/I");
	fTree->Branch("TLep_Pt",     TLep_Pt,    "TLep_Pt[TNSelLeps]/F");
	fTree->Branch("TLep_Eta",     TLep_Eta, "TLep_Eta[TNSelLeps]/F");
	fTree->Branch("TLep_Phi",     TLep_Phi, "TLep_Phi[TNSelLeps]/F");
	fTree->Branch("TLep_E" ,     TLep_E ,     "TLep_E[TNSelLeps]/F");
	fTree->Branch("TLep_Charge",  TLep_Charge, "TLep_Charge[TNSelLeps]/F");
	fTree->Branch("TMll",         &TMll ,       "TMll/F");
	fTree->Branch("TPtdil",         &TPtdil ,       "TPtdil/F");
	fTree->Branch("TChannel",      &TChannel,          "TChannel/I");
	fTree->Branch("TMT2",      &TMT2,      "TMT2/F");
	fTree->Branch("TMT",       &TMT,        "TMT/F");
	fTree->Branch("TMTprime",    &TMTprime,    "TMTprime/F");
};

void WWAnalysis::SetJetVariables(){
  fTree->Branch("TNJets",        &TNJets,         "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
  fTree->Branch("TJet_isBJet",   TJet_isBJet,       "TJet_isBJet[TNJets]/I");
  fTree->Branch("TJet_Pt",       TJet_Pt,           "TJet_Pt[TNJets]/F");
  fTree->Branch("TJet_Eta",      TJet_Eta,           "TJet_Eta[TNJets]/F");
  fTree->Branch("TJet_Phi",      TJet_Phi,           "TJet_Phi[TNJets]/F");
  fTree->Branch("TJet_E",        TJet_E,            "TJet_E[TNJets]/F");
  fTree->Branch("THT",          &THT,          "THT/F");
  fTree->Branch("TNVetoJets",     &TNVetoJets,     "TNVetoJets/I");
  
  if(gIsData) return;
  fTree->Branch("TNJetsJESUp",      &TNJetsJESUp,         "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",    &TNJetsJESDown,         "TNJetsJESDown/I");
  fTree->Branch("TNJetsJER",        &TNJetsJER,         "TNJetsJER/I");

  fTree->Branch("TNBtagsUp",           &TNBtagsUp,   "TNBtagsUp/I");
  fTree->Branch("TNBtagsDown",         &TNBtagsDown, "TNBtagsDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");

 
}

void WWAnalysis::SetEventVariables(){
	fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
	fTree->Branch("TMET",         &TMET,         "TMET/F");
	fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
	fTree->Branch("TIsSS",        &TIsSS,        "TIsSS/B");
	
	if(gIsData) return;
	fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
	fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
	fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
	fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
	fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
	fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");

	fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
	fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
}

void WWAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
  TNSelLeps = selLeptons.size();
  Int_t nVetoLeptons = VetoLeptons.size();
  TNVetoLeps = (nVetoLeptons == 0) ? TNSelLeps : nVetoLeptons;
  for(Int_t i = 0; i < TNSelLeps; i++){
    TLep_Pt[i]     = selLeptons.at(i).Pt();    
    TLep_Eta[i]    = selLeptons.at(i).Eta();    
    TLep_Phi[i]    = selLeptons.at(i).Phi();    
    TLep_E[i]      = selLeptons.at(i).E();    
    TLep_Charge[i] = selLeptons.at(i).charge;    
  }
  if(TNSelLeps < 2) TChannel = -1;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) TChannel = 1;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) TChannel = 1;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) TChannel = 2;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) TChannel = 3;
  
  if(TNSelLeps > 1){
	TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();    
	TPtdil = (selLeptons.at(0).p + selLeptons.at(1).p).Pt(); 
	TMT2 = getMT2ll(selLeptons.at(0), selLeptons.at(1), Get<Float_t>("met_pt"), Get<Float_t>("met_phi") ); 
	
	
	

	TLorentzVector met;
	met.SetPtEtaPhiM(Get<Float_t>("met_pt"), 0, Get<Float_t>("met_phi"), 0);
	TMT = getMT(selLeptons.at(0).p + selLeptons.at(1).p, met);
	
	//defincion directa con el TLorentzVector:
	TMTprime = (selLeptons.at(0).p + selLeptons.at(1).p).Mt();
  }
  
}

void WWAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNJets = selJets.size(); THT = 0;
  TNVetoJets = nVetoJets;
  TNBtags = 0; TNBtagsUp = 0; TNBtagsDown = 0;
  TNBtagsMisTagUp = 0;  TNBtagsMisTagDown = 0;
  for(Int_t i = 0; i < TNJets; i++){
    TJet_Pt[i]     = selJets.at(i).Pt();
    TJet_Eta[i]    = selJets.at(i).Eta();
    TJet_Phi[i]    = selJets.at(i).Phi();
    TJet_E[i]      = selJets.at(i).E();
    TJet_isBJet[i] = selJets.at(i).isBtag;
    THT += TJet_Pt[i];
    if(selJets.at(i).isBtag)            TNBtags++;
    if(selJets.at(i).isBtag_BtagUp    ) TNBtagsUp++;
    if(selJets.at(i).isBtag_BtagDown  ) TNBtagsDown++;
    if(selJets.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
    if(selJets.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
  }
  // For systematics...
  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNJetsJER      = 0;  
  THTJESUp = 0; THTJESDown = 0;
  for(Int_t i = 0; i < (Int_t) cleanedJets15.size(); i++){
    if(cleanedJets15.at(i).pTJESUp > ptCut){
      TNJetsJESUp++;
      TJetJESUp_Pt[i] = cleanedJets15.at(i).pTJESUp;
      THTJESUp += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJESDown > ptCut){
      TNJetsJESDown++;
      TJetJESDown_Pt[i] = cleanedJets15.at(i).pTJESDown;
      THTJESDown += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJERUp > ptCut){
      TNJetsJER++;
      TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
    }
  }
}

void WWAnalysis::GetMET(){
		TMET        = Get<Float_t>("met_pt");
		TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
		TMETJESUp   = Get<Float_t>("met_jecUp_pt"  );
		TMETJESDown = Get<Float_t>("met_jecDown_pt");
}

