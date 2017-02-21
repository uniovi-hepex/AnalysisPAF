#include "MiniTreeProducer.h"

ClassImp(MiniTreeProducer);
MiniTreeProducer::MiniTreeProducer() : PAFChainItemSelector() {}
void MiniTreeProducer::Summary(){}

void MiniTreeProducer::Initialise(){
	gIsData = GetParam<Bool_t>("IsData");
	gSelection = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");

  // Initialise all the tree variable to -1...
	TChannel = -1; TWeight = -1;   
	TMll = -1; TMET = -1; TMET_Phi = -1;
	THT = -1;     
	TLep1_Pt = -1; TLep1_Eta = -1; TLep1_Phi = -1; TLep1_E = -1; TLep1_Charge = -1;
	TLep2_Pt = -1; TLep2_Eta = -1; TLep2_Phi = -1; TLep2_E = -1; TLep2_Charge = -1;
	TNJets = -1; TNJetsBtag = -1;

	TNISRJets = -1; TNJetsJESUp = -1; TNJetsJESDown = -1; TNJetsJER = -1;
	TNJetsBtagUp = -1; TNJetsBtagDown = -1; TNJetsBtagMisTagUp = -1; TNJetsBtagMisTagDown = -1;

	TGenMET = -1;
	TMETJESUp = -1; TMETJESDown = -1;
	THTJESUp = -1; THTJESDown = -1;
	TMT2llJESUp = -1; TMT2llJESDown = -1;

	TWeight_LepEffUp = -1; TWeight_LepEffDown = -1;
	TWeight_TrigUp = -1; TWeight_TrigDown = -1;
	TWeight_FSUp = -1; TWeight_PUDown = -1;
	TWeight_PUUp = -1; TWeight_FSDown = -1;

	TMT2ll = -1; TMT2jj = -1;	TMT2lblb = -1; TPtllb = -1; TMeff = -1;
	TdPhiPtllbMET = -1; TdPhiJetMet = -1; TdPhiLepMet = -1; TdPhiLepJet = -1;
	TdPhill = -1; TdEtall = -1;
	TMinDPhiMetJets = -1;

  //fTree = new TTree("mitree", "This is a Tree");
	fTree = CreateTree("MiniTree","Created with PAF");
	fTree->Branch("TWeight",      &TWeight,      "TWeight/F");

	fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
	fTree->Branch("TMET",         &TMET,         "TMET/F");
	fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
	fTree->Branch("THT",          &THT,          "THT/F");

	fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
	fTree->Branch("TLep1_Pt",     &TLep1_Pt,     "TLep1_Pt/F");
	fTree->Branch("TLep1_Eta",     &TLep1_Eta,     "TLep1_Eta/F");
	fTree->Branch("TLep1_Phi",     &TLep1_Phi,     "TLep1_Phi/F");
	fTree->Branch("TLep1_E" ,     &TLep1_E ,     "TLep1_E/F");
	fTree->Branch("TLep1_Charge",  &TLep1_Charge, "TLep1_Charge/F");
	fTree->Branch("TLep2_Pt",     &TLep2_Pt,     "TLep2_Pt/F");
	fTree->Branch("TLep2_Eta",     &TLep2_Eta,     "TLep2_Eta/F");
	fTree->Branch("TLep2_Phi",     &TLep2_Phi,     "TLep2_Phi/F");
	fTree->Branch("TLep2_E" ,     &TLep2_E ,     "TLep2_E/F");
	fTree->Branch("TLep2_Charge",  &TLep2_Charge, "TLep2_Charge/F");

	fTree->Branch("TNJets",           &TNJets,         "TNJets/I");
	fTree->Branch("TNJetsBtag",       &TNJetsBtag,     "TNJetsBtag/I");
	fTree->Branch("TJet_isBJet",       TJet_isBJet,       "TJet_isBJet[TNJets]/I");
	fTree->Branch("TJet_Pt",           TJet_Pt,           "TJet_Pt[TNJets]/F");
	fTree->Branch("TJet_Eta",           TJet_Eta,           "TJet_Eta[TNJets]/F");
	fTree->Branch("TJet_Phi",           TJet_Phi,           "TJet_Phi[TNJets]/F");
	fTree->Branch("TJet_E",            TJet_E,            "TJet_E[TNJets]/F");

		if(gDoSyst){
			fTree->Branch("TNJetsJESUp",           &TNJetsJESUp,         "TNJetsJESUp/I");
			fTree->Branch("TNJetsJESDown",           &TNJetsJESDown,         "TNJetsJESDown/I");
			fTree->Branch("TNJetsJER",           &TNJetsJER,         "TNJetsJER/I");

			fTree->Branch("TNJetsBtag",       &TNJetsBtag,     "TNJetsBtag/I");
			fTree->Branch("TNJetsBtagUp",     &TNJetsBtagUp,   "TNJetsBtagUp/I");
			fTree->Branch("TNJetsBtagDown",   &TNJetsBtagDown, "TNJetsBtagDown/I");
			fTree->Branch("TNJetsBtagMisTagUp",     &TNJetsBtagMisTagUp,   "TNJetsBtagMisTagUp/I");
			fTree->Branch("TNJetsBtagMisTagDown",   &TNJetsBtagMisTagDown, "TNJetsBtagMisTagDown/I");

			fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
			fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
			fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");

			fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
			fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
			fTree->Branch("THTJESUp",     &THTJESUp,     "THTJESUp/F");
			fTree->Branch("THTJESDown",   &THTJESDown,   "THTJESDown/F");

			fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
			fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
			fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
			fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
			fTree->Branch("TWeight_FSUp",          &TWeight_FSUp,          "TWeight_FSUp/F");
			fTree->Branch("TWeight_FSDown",        &TWeight_FSDown,        "TWeight_FSDown/F");
			fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
			fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");
		}

		if(gSelection == iStopSelec){
			fTree->Branch("TMT2ll",    &TMT2ll,   "TMT2ll/F");
			fTree->Branch("TMT2jj",    &TMT2jj,   "TMT2jj/F");
			fTree->Branch("TMT2lblb",    &TMT2lblb,   "TMT2lblb/F");
			fTree->Branch("TPtllb",    &TPtllb,   "TPtllb/F");
			fTree->Branch("TMeff",    &TMeff,   "TMeff/F");
			fTree->Branch("TdPhiPtllbMET",    &TdPhiPtllbMET,   "TdPhiPtllbMET/F");
			fTree->Branch("TdPhiJetMet",    &TdPhiJetMet,   "TdPhiJetMet/F");
			fTree->Branch("TdPhiLepMet",    &TdPhiLepMet,   "TdPhiLepMet/F");
			fTree->Branch("TdPhiLepJet",    &TdPhiLepJet,   "TdPhiLepJet/F");
			fTree->Branch("TdPhill",    &TdPhill,   "TdPhill/F");
			fTree->Branch("TdEtall",    &TdEtall,   "TdEtall/F");
			fTree->Branch("TMinDPhiMetJets",    &TMinDPhiMetJets,   "TMinDPhiMetJets/F");
			fTree->Branch("TdPhill",    &TdPhill,   "TdPhill/F");
		}
}

void MiniTreeProducer::InsideLoop(){




  GetDefaultVariables();
  //if(gDoSyst) GetSystematicVariables();
  fTree->Fill();
}

void MiniTreeProducer::GetDefaultVariables(){
	//TMll = (selLeptons[0].p+selLeptons[1].p).M();
	TMET     = Get<Float_t>("met_pt");
	TMET_Phi = Get<Float_t>("met_phi");
	//Int_t gChan = GetParam<Int_t>("gChan");
	TChannel = GetParam<Int_t>("gChannel");

	TWeight = GetParam<Float_t>("EventWeight");
	TNJets      = GetParam<Int_t>("nSelJets");            // Jets...
	TNJetsBtag  = GetParam<Int_t>("nSelBJets");


	TMll = getMll(selLeptons[0], selLeptons[1]);
	THT  = getHT(selJets);

	TLep1_Pt      = selLeptons[0].p.Pt();
	TLep1_Eta      = selLeptons[0].p.Eta();
	TLep1_Phi      = selLeptons[0].p.Phi();
	TLep1_E       = selLeptons[0].p.E();
	TLep1_Charge  = selLeptons[0].charge;
	TLep2_Pt      = selLeptons[1].p.Pt();
	TLep2_Eta      = selLeptons[1].p.Eta();
	TLep2_Phi      = selLeptons[1].p.Phi();
	TLep2_E       = selLeptons[1].p.E();
	TLep2_Charge  = selLeptons[1].charge;

	for(int k = 0; k<40; k++){
		if(k<TNJets){
			TJet_Pt[k]           = selJets[k].p.Pt();
			TJet_Eta[k]           = selJets[k].p.Eta();
			TJet_Phi[k]           = selJets[k].p.Phi();
			TJet_E[k]            = selJets[k].p.E();
			TJet_isBJet[k]       = selJets[k].isbtag;
		}
		else{
			TJet_Pt[k]           = 0;
			TJet_Eta[k]           = 0;
			TJet_Phi[k]           = 0;
			TJet_E[k]            = 0;
			TJet_isBJet[k]       = 0;
		}
	}



}

void MiniTreeProducer::GetSystematicVariables(){
	GetParam<Float_t>("TriggerSF");
	GetParam<Float_t>("TriggerSF_Up");
	GetParam<Float_t>("TriggerSF_Down");
	GetParam<Float_t>("PUSF");
	GetParam<Float_t>("PUSF_Up");
	GetParam<Float_t>("PUSF_Down");
}
