#include "WWAnalysis.h"

ClassImp(WWAnalysis);
WWAnalysis::WWAnalysis() : PAFChainItemSelector() {
	//inicializar las variables definidas en el WWanalysis.h
	fTree = 0;

	TrigSF = 0; TrigSFerr = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0;
	gChannel = 0; passMETfilters = 0; passTrigger = 0; TIsSS = 0;  NormWeight = 0; TWeight = 0;
	TMll = 0;  TPtdil = 0; TMET = 0; TMET_Phi = 0; TNJets = 0; TNBtags = 0; THT = 0; 
	TNVetoLeps = 0; TNSelLeps = 0; TChannel = 0;
	TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJER = 0; TNVetoJetsJESUp = 0; TNVetoJetsJESDown = 0; TgenMET = 0;
	TNVetoJetsBtagUp = 0; TNVetoJetsBtagDown = 0; TNVetoJetsMisTagUp = 0; TNVetoJetsMisTagDown = 0;
	THTJESUp = 0; THTJESDown = 0; TNISRJets = 0;
	TMETJESUp = 0; TMETJESDown = 0; TMT2llJESUp = 0; TMT2llJESDown = 0; TMT2 = 0; TMT = 0; TMTprime = 0;
	TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
	TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;  TNVetoJets = 0;
	TDeltaPhi = 0; TDeltaEta = 0;
	
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


void WWAnalysis::Summary(){}

void WWAnalysis::Initialise(){
	gIsData      = GetParam<Bool_t>("IsData");
	gSelection   = GetParam<Int_t>("iSelection");
	gSampleName  = GetParam<TString>("sampleName");
	gDoSyst      = GetParam<Bool_t>("doSyst");
	
	fTree = CreateTree("tree","Created with PAF");
	
	gIsLHE = false;
  	if(gSampleName.Contains("WW")) gIsLHE = true;

	SetLeptonVariables();
	SetJetVariables();
	SetEventVariables();

	selLeptons  = std::vector<Lepton>();
	vetoLeptons = std::vector<Lepton>();
  	genLeptons = std::vector<Lepton>();
	selJets = std::vector<Jet>();
	Jets15  = std::vector<Jet>();
	vetoJets = std::vector<Jet>();
	nVetoJets = 0;
}



void WWAnalysis::InsideLoop(){
  selLeptons.clear(); vetoLeptons.clear(); genLeptons.clear(); selJets.clear(); Jets15.clear();
  // Vectors with the objects
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  genLeptons =  GetParam<vector<Lepton>>("genLeptons");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  vetoJets    = GetParam<vector<Jet>>("vetoJets");
  nVetoJets   = GetParam<Int_t>("nVetoJets");
  // Weights and SFs
  NormWeight   = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSFerr    = GetParam<Float_t>("TriggerSFerr");
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
    			Float_t ElecSF = 1; Float_t MuonSF = 1;
    			Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
    			if(TChannel == iElec){
      				ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
     				ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      				ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      				MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
    			}
    			else if(TChannel == iMuon){
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
        				ElecSFDo *= selLeptons.at(0).GetSF(-1);
      				}
   	    		}
		
    			TWeight             = NormWeight*ElecSF*MuonSF*TrigSF*PUSF;
    			TWeight_ElecEffUp   = NormWeight*ElecSFUp*MuonSF*TrigSF*PUSF;
    			TWeight_ElecEffDown = NormWeight*ElecSFDo*MuonSF*TrigSF*PUSF;
    			TWeight_MuonEffUp   = NormWeight*ElecSF*MuonSFUp*TrigSF*PUSF;
    			TWeight_MuonEffDown = NormWeight*ElecSF*MuonSFDo*TrigSF*PUSF;
    			TWeight_TrigUp      = NormWeight*lepSF*(TrigSF+TrigSFerr)*PUSF;
    			TWeight_TrigDown    = NormWeight*lepSF*(TrigSF-TrigSFerr)*PUSF;
    			TWeight_PUDown      = NormWeight*lepSF*TrigSF*PUSF_Up;
    			TWeight_PUUp        = NormWeight*lepSF*TrigSF*PUSF_Down;
			if(gIsData) TWeight = 1;

			// Event Selection
			// ===================================================================================================================
			// utilizando minitrees aquí pongo el nivel más básico de seleccion de eventos. Será en el plotter donde escogeremos más a fondo.
			
//==============================// analisis de cosas básicas de Top:
			//if( ((selLeptons.at(0).p +selLeptons.at(1).p).M() > 20) // mll > 20 GeV
				//&& (gChannel == 1 || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  )//  Z Veto in ee, µµ
				//&& TNJets > 1      
				//&& TNBtags > 0 ){      
				//fTree->Fill();
			//};
//=============================// analisis de PRUEBA
			//if((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20 && selLeptons.at(0).p.Pt() > 25){ // mll > 20 GeV
        			//if(gChannel == iElMu && TNJets == 0){
              					//fTree->Fill();
				//}
			//}          
//=============================// analisis de cosas básicas de WW
			if( ((selLeptons.at(0).p +selLeptons.at(1).p).M() > 12) && gChannel == iElMu)// mll > 12 GeV
				//&& (gChannel == iElMu || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15))){ //Channel e mu y con Z Veto in ee, µµ
				fTree->Fill();
			//};
//============================// analisis de cosas básicas de DY inclusivo (con pico Z para quitarlo con el plot)			
		    //if( ((selLeptons.at(0).p + selLeptons.at(1).p).M() > 0) && (!TIsSS)){ // mll > 12 GeV y signo opuesto
		      //fTree->Fill();
		    //};
		    
		};
	};
};


//#####################################################################
// Functions
//------------------------------------------------------------------

void WWAnalysis::SetLeptonVariables(){
	fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
  	fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  	fTree->Branch("TLep_Pt",     &TLep_Pt,     "TLep_Pt/F");
  	fTree->Branch("TLep_Eta",     &TLep_Eta,     "TLep_Eta/F");
  	fTree->Branch("TLep_Phi",     &TLep_Phi,     "TLep_Phi/F");
  	fTree->Branch("TLep_E" ,     &TLep_E ,     "TLep_E/F");
  	fTree->Branch("TLep_Charge", &TLep_Charge, "TLep_Charge/F");
  	fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
	fTree->Branch("TPtdil",         &TPtdil ,       "TPtdil/F");
	fTree->Branch("TMT",       &TMT,        "TMT/F");
  	fTree->Branch("TMll",      &TMll,      "TMll/F");
  	fTree->Branch("TMT2",      &TMT2,      "TMT2/F");
  	fTree->Branch("TDeltaPhi",      &TDeltaPhi,      "TDeltaPhi/F");
  	fTree->Branch("TDeltaEta",      &TDeltaEta,      "TDeltaPhi/F");
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
  	fTree->Branch("TNJetsJESUp",      &TNJetsJESUp,       "TNJetsJESUp/I");
  	fTree->Branch("TNJetsJESDown",    &TNJetsJESDown,     "TNJetsJESDown/I");
  	fTree->Branch("TNJetsJER",        &TNJetsJER,         "TNJetsJER/I");
  	fTree->Branch("TNVetoJetsJESUp",      &TNVetoJetsJESUp,         "TNVetoJetsJESUp/I");
  	fTree->Branch("TNVetoJetsJESDown",    &TNVetoJetsJESDown,         "TNVetoJetsJESDown/I");

  	fTree->Branch("TNVetoJetsBtagUp",       &TNVetoJetsBtagUp,     "TNVetoJetsBtagUp/I");
  	fTree->Branch("TNVetoJetsBtagDown",     &TNVetoJetsBtagDown,   "TNVetoJetsBtagDown/I");
  	fTree->Branch("TNVetoJetsMisTagUp",     &TNVetoJetsMisTagUp,   "TNVetoJetsMisTagUp/I");
  	fTree->Branch("TNVetoJetsMisTagDown",   &TNVetoJetsMisTagDown, "TNVetoJetsMisTagDown/I");

  	fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  	fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  	fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");
};


void WWAnalysis::SetEventVariables(){
	fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
	fTree->Branch("TMET",         &TMET,         "TMET/F");
	fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
	fTree->Branch("TIsSS",        &TIsSS,        "TIsSS/B");
	
	if(gIsData) return;
	fTree->Branch("TgenMET",         &TgenMET,         "TgenMET/F");
	fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
	fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
	fTree->Branch("TWeight_ElecEffUp",      &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  	fTree->Branch("TWeight_ElecEffDown",    &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  	fTree->Branch("TWeight_MuonEffUp",      &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  	fTree->Branch("TWeight_MuonEffDown",    &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  	fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  	fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
	fTree->Branch("TWeight_PUUp",          &TWeight_PUUp,          "TWeight_PUUp/F");
	fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");
  	fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
	fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
	fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
  	fTree->Branch("TMT2llJESUp",    &TMT2llJESUp,    "TMT2llJESUp/F");
  	fTree->Branch("TMT2llJESDown",  &TMT2llJESDown,  "TMT2llJESDown/F");
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
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) TChannel = iElMu;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) TChannel = iElMu;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) TChannel = iMuon;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) TChannel = iElec;
  TChannel = gChannel;
  if(TNSelLeps > 1){
	TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();    
	TPtdil = (selLeptons.at(0).p + selLeptons.at(1).p).Pt(); 
	TMT2 = getMT2ll(selLeptons.at(0), selLeptons.at(1), Get<Float_t>("met_pt"), Get<Float_t>("met_phi") ); 
	TDeltaPhi = (selLeptons.at(0).p.DeltaPhi(selLeptons.at(1).p));
	TDeltaEta = TMath::Abs(TMath::Abs(selLeptons.at(0).p.Eta()) - TMath::Abs(selLeptons.at(1).p.Eta()));
	TLorentzVector met;
	met.SetPtEtaPhiM(Get<Float_t>("met_pt"), 0, Get<Float_t>("met_phi"), 0);
	TMT = getMT(selLeptons.at(0).p + selLeptons.at(1).p, met);
	//defincion directa con el TLorentzVector de TMT (predefinida)
	TMTprime = (selLeptons.at(0).p + selLeptons.at(1).p).Mt();
  }
}

void WWAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNVetoJets = nVetoJets;
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0; TNVetoJetsBtagUp = 0; TNVetoJetsBtagDown = 0;
  TNVetoJetsMisTagUp = 0;  TNVetoJetsMisTagDown = 0;
  for(Int_t i = 0; i < 20; i++){
    if(i < TNJets){
      TJet_Pt[i]     = selJets.at(i).Pt();
      TJet_Eta[i]    = selJets.at(i).Eta();
      TJet_Phi[i]    = selJets.at(i).Phi();
      TJet_E[i]      = selJets.at(i).E();
      TJet_isBJet[i] = selJets.at(i).isBtag;
      if(selJets.at(i).isBtag) TNBtags++;
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

  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNVetoJetsJESUp    = 0;
  TNVetoJetsJESDown  = 0;
  TNJetsJER      = 0;  
  THTJESUp = 0; THTJESDown = 0;


  for(Int_t i = 0; i < (Int_t) cleanedJets15.size(); i++){
    	if(cleanedJets15.at(i).pTJESUp > 30){
      		TNJetsJESUp++;
      		TJetJESUp_Pt[i] = cleanedJets15.at(i).pTJESUp;
		THTJESUp += TJetJESUp_Pt[i];
	}
	if(cleanedJets15.at(i).pTJESDown > 30){
		TNJetsJESDown++;
		TJetJESDown_Pt[i] = cleanedJets15.at(i).pTJESDown;
      		THTJESDown += TJetJESUp_Pt[i];
	}
	if(cleanedJets15.at(i).pTJERUp > 30){
	      	TNJetsJER++;
      		TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
	}
	if((cleanedJets15.at(i).pTJESUp > 20) && (cleanedJets15.at(i).isBtag)) TNVetoJetsJESUp++;
	if((cleanedJets15.at(i).pTJESDown > 20) && (cleanedJets15.at(i).isBtag)) TNVetoJetsJESDown++;

	if(cleanedJets15.at(i).p.Pt() > 20){
		if(cleanedJets15.at(i).isBtag_BtagUp) TNVetoJetsBtagUp++;
		if(cleanedJets15.at(i).isBtag_BtagDown  ) TNVetoJetsBtagDown++;
    		if(cleanedJets15.at(i).isBtag_MisTagUp  ) TNVetoJetsMisTagUp++;
    		if(cleanedJets15.at(i).isBtag_MisTagDown) TNVetoJetsMisTagDown++;
	}
  }
}


void WWAnalysis::GetMET(){
  TMET        = Get<Float_t>("met_pt");
  TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
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

