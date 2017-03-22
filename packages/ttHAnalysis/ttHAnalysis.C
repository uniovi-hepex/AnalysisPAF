/*==============================================================================

							ttHAnalyzer selector

==============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//		Preprocessor directives
////////////////////////////////////////////////////////////////////////////////
// C++ usual debugging procedures
#ifdef DEBUGC
	#undef DEBUGC
#endif
//#define DEBUGC								// Uncomment for C++ debugging

//	Package inclusion
#include "ttHAnalyzer.h"
#include <iostream>
#include <math.h>

ClassImp(ttHAnalyzer); // PAF definition as class

////////////////////////////////////////////////////////////////////////////////
//		Initial definitions
////////////////////////////////////////////////////////////////////////////////
ttHAnalyzer::ttHAnalyzer() : PAFChainItemSelector() {
	fHDummy = 0;

	// Defining to zero the aim of the histogram pointers
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			fHEvents    		[icat][ichan]	=	0; // Events
			fHTightLep			[icat][ichan]	=	0; // Yields
			fHFakeLep			[icat][ichan]	=	0;
			fHLooseLep			[icat][ichan]	=	0;
			fHTau				[icat][ichan]	=	0;
			fHJet				[icat][ichan]	=	0;
			fHMedBTagJet		[icat][ichan]	=	0;
			fHLosBTagJet		[icat][ichan]	=	0;
			fHPtLeading			[icat][ichan]	=	0; // Kinematic
			fHPtSubLeading		[icat][ichan]	=	0;
			fHPtSubSubLeading	[icat][ichan]	=	0;
			fHMET				[icat][ichan]	=	0; // MET
			fHMHT				[icat][ichan]	=	0;
			fHMETLD				[icat][ichan]	=	0;
			fHChargeSum			[icat][ichan]	=	0; // Misc
			fHMass				[icat][ichan]	=	0;
		}
	}
}

//	Core PAF-analysis methods
//------------------------------------------------------------------------------
void ttHAnalyzer::Initialise() {
	PAF_INFO("ttHAnalyzer", "+ Preprocess DONE");
	cout << endl;
	PAF_INFO("ttHAnalyzer", "======================================== Initialization");

	PAF_INFO("ttHAnalyzer", "+ Initializing parameters");
	GetParameters();

	PAF_INFO("ttHAnalyzer", "+ Initializing histograms");
	TH1::SetDefaultSumw2();
	fHDummy 		= CreateH1F("fHDummy","",1,0,1);
	InitialiseEventHistos();
	InitialiseYieldHistos();
	InitialiseKinematicHistos();
	InitialiseMETHistos();
	InitialiseMiscHistos();

	//	PU Reweight
	PAF_INFO("ttHAnalyzer", "+ Initializing pile-up reweighting tool and b-tag scale factors");
	medfBTagSFnom 	= new BTagSFUtil("mujets", "CSVv2", "Medium",  0);
	losfBTagSFnom 	= new BTagSFUtil("mujets", "CSVv2", "Loose",  0);

	cout << endl;

	PAF_INFO("ttHAnalyzer", "+ Initialization DONE");
	cout << endl;
	PAF_INFO("ttHAnalyzer", "======================================== Analysis");
}

void ttHAnalyzer::InsideLoop() {
	#ifdef DEBUGC
		cout << "DEBUGC - Beginning of InsideLoop" << endl;
	#endif
	// SPECIAL ----- for running with 2015 data (03-03-16 draft note)
	//run	= Get<Int_t>("run");
	//if ((gIsData) && ((run < 254227) || (run > 254914 && run < 256630) || (run > 260627))) return; 	// Comment this for running with ALL the data
	fHDummy->Fill(0.5);

	// Get and set data members
	GetTreeVariables();
	SetOriginalObjects();
	SetEventObjects();

    if (!PassesPreCuts()) return;

	// Fill histograms
	FillEventHistos();
	FillYieldHistos();
	FillKinematicHistos();
	FillMETHistos();
	FillMiscHistos();

	#ifdef DEBUGC
		cout << "DEBUGC - End of InsideLoop" << endl;
	#endif
}

void ttHAnalyzer::Summary() {
	PAF_INFO("ttHAnalyzer", "+ Analysis DONE");
	cout << endl;
}

////////////////////////////////////////////////////////////////////////////////
//		Tree-related methods
////////////////////////////////////////////////////////////////////////////////
void ttHAnalyzer::GetTreeVariables() {
	#ifdef DEBUGC
		cout << "DEBUGC - Beginning of GetTreeVariables"<< endl;
	#endif
    nLepGood             = Get<Int_t>("nLepGood");
	nTauGood             = Get<Int_t>("nTauGood");
	nJet                 = Get<Int_t>("nJet");
    evt                  = Get<Long_t>("evt");

    if (!gIsData){
		genWeight            = Get<Float_t>("genWeight");
	}

	for (Int_t k = 0; k < nLepGood; k++) {
	    LepGood_px[k]      	= Get<Float_t>("LepGood_px", k);
	    LepGood_py[k]       = Get<Float_t>("LepGood_py", k);
	    LepGood_pz[k]      	= Get<Float_t>("LepGood_pz", k);
	    LepGood_energy[k]  	= Get<Float_t>("LepGood_energy", k);
	    LepGood_pt[k]      	= Get<Float_t>("LepGood_pt", k);
	    LepGood_eta[k]     	= Get<Float_t>("LepGood_eta", k);
	    LepGood_dxy[k]     	= Get<Float_t>("LepGood_dxy", k);
	    LepGood_dz[k]      	= Get<Float_t>("LepGood_dz", k);
	    LepGood_pdgId[k]   	= Get<Int_t>("LepGood_pdgId", k);
	    LepGood_charge[k]  	= Get<Int_t>("LepGood_charge", k);
		LepGood_sip3d[k]	= Get<Float_t>("LepGood_sip3d", k);
		LepGood_miniRelIso[k]       = Get<Float_t>("LepGood_miniRelIso", k);
		LepGood_jetBTagCSV[k]		= Get<Float_t>("LepGood_jetBTagCSV", k);
		LepGood_mediumMuonId[k]		= Get<Int_t>("LepGood_mediumMuonId", k);
		LepGood_mvaTTH[k]			= Get<Float_t>("LepGood_mvaTTH", k);
		LepGood_jetPtRatiov2[k]		= Get<Float_t>("LepGood_jetPtRatiov2", k);
		LepGood_mvaIdSpring16GP[k]	= Get<Float_t>("LepGood_mvaIdSpring16GP", k);
		LepGood_sigmaIEtaIEta[k]	= Get<Float_t>("LepGood_sigmaIEtaIEta", k);
		LepGood_hadronicOverEm[k]	= Get<Float_t>("LepGood_hadronicOverEm", k);
		LepGood_dEtaScTrkIn[k]		= Get<Float_t>("LepGood_dEtaScTrkIn", k);
		LepGood_dPhiScTrkIn[k]		= Get<Float_t>("LepGood_dPhiScTrkIn", k);
		LepGood_eInvMinusPInv[k]	= Get<Float_t>("LepGood_eInvMinusPInv", k);
		LepGood_convVeto[k]			= Get<Float_t>("LepGood_convVeto", k);
		LepGood_lostHits[k]			= Get<Int_t>("LepGood_lostHits", k);
		LepGood_tightCharge[k]		= Get<Int_t>("LepGood_tightCharge", k);
		LepGood_jetDR[k]			= Get<Float_t>("LepGood_jetDR", k);
	}
	for (Int_t k = 0; k < nJet; k++) {
		Jet_px[k]          = Get<Float_t>("Jet_px", k);
		Jet_py[k]          = Get<Float_t>("Jet_py", k);
		Jet_pz[k]          = Get<Float_t>("Jet_pz", k);
		Jet_energy[k]      = Get<Float_t>("Jet_energy", k);
		Jet_eta[k]         = Get<Float_t>("Jet_eta", k);
		Jet_btagCSV[k]     = Get<Float_t>("Jet_btagCSV", k);
	}
	for (Int_t k = 0; k < nTauGood; k++) {
		TauGood_idDecayModeNewDMs[k]	=	Get<Int_t>("TauGood_idDecayModeNewDMs", k);
		TauGood_pt[k]					=	Get<Float_t>("TauGood_pt", k);
		TauGood_eta[k]					=	Get<Float_t>("TauGood_eta", k);
		TauGood_phi[k]					=	Get<Float_t>("TauGood_phi", k);
		TauGood_mass[k]					=	Get<Float_t>("TauGood_mass", k);
		TauGood_idCI3hit[k]				=	Get<Int_t>("TauGood_idCI3hit", k);
	}
}

void ttHAnalyzer::GetParameters() {
    gSampleName		=	GetParam<TString>("sampleName");
    gIsData        	= 	GetParam<Bool_t>("IsData");
    gWeight        	= 	GetParam<Float_t>("weight"); // cross section / events in the sample
	gIsMCatNLO     	= 	GetParam<Bool_t>("IsMCatNLO");
}

////////////////////////////////////////////////////////////////////////////////
//		Histogram-related methods
////////////////////////////////////////////////////////////////////////////////
// Initialising
//------------------------------------------------------------------------------
void ttHAnalyzer::InitialiseEventHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHEvents[icat][ichan] = CreateH1F("H_Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""						, 1, 0, 1);
		}
	}
}

void ttHAnalyzer::InitialiseYieldHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHTightLep	[icat][ichan] = CreateH1F("H_TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 6, 0, 6);
			fHFakeLep	[icat][ichan] = CreateH1F("H_FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""					, 5, 0, 5);
			fHLooseLep	[icat][ichan] = CreateH1F("H_LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 5, 0, 5);
			fHTau		[icat][ichan] = CreateH1F("H_Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""						, 3, 0, 3);
			fHJet		[icat][ichan] = CreateH1F("H_Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""						, 10, 0, 10);
			fHMedBTagJet[icat][ichan] = CreateH1F("H_MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 6, 0, 6);
			fHLosBTagJet[icat][ichan] = CreateH1F("H_LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 6, 0, 6);
		}
	}
}

void ttHAnalyzer::InitialiseKinematicHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHPtLeading			[icat][ichan] = CreateH1F("H_PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""		, 10, 0, 200);
			fHPtSubLeading		[icat][ichan] = CreateH1F("H_PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""	, 10, 0, 200);
			fHPtSubSubLeading	[icat][ichan] = CreateH1F("H_PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""	, 10, 0, 200);
		}
	}
}

void ttHAnalyzer::InitialiseMETHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHMET				[icat][ichan] = CreateH1F("H_MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 10, 0, 400);
			fHMHT				[icat][ichan] = CreateH1F("H_MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""				, 10, 0, 1000);
			fHMETLD				[icat][ichan] = CreateH1F("H_METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""			, 10, 0, 2);
		}
	}
}

void ttHAnalyzer::InitialiseMiscHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == twolSS 		&& ichan != All) 	continue;
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHChargeSum			[icat][ichan] = CreateH1F("H_ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""		, 7, -3.5, 3.5);
			fHMass				[icat][ichan] = CreateH1F("H_Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan],""			, 10, 0, 400);
		}
	}
}

// Filling methods
//------------------------------------------------------------------------------
void ttHAnalyzer::FillEventHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat 	== twolSS 		&& !Is2lSSEvent()) 											continue;
			if (icat 	== twolSS 		&& (!triggermumuSS() && !triggereeSS() && !triggeremuSS()))	continue;
			if (ichan 	== MuMu 		&& (!IsMuMuEvent() 	|| !triggermumuSS())) 					continue;
			if (ichan 	== ElEl 		&& (!IsElElEvent() 	|| !triggereeSS())) 					continue;
			if (ichan 	== ElMu 		&& (!IsElMuEvent() 	|| !triggeremuSS())) 					continue;
			if (icat 	== threel 		&& (!Is3lEvent() 	|| !trigger3l4l() 	|| ichan != All)) 	continue;
			if (icat 	== Total 		&& ichan != All ) 	continue;
			if (icat 	== Total 		&& (!Is2lSSEvent() || !IsMuMuEvent() || !triggermumuSS()) && (!Is2lSSEvent() || !IsElElEvent() || !triggereeSS()) && (!Is2lSSEvent() || !IsElMuEvent() || !triggeremuSS()) && (!Is3lEvent() ||  !trigger3l4l())) 	continue;

			fHEvents[icat][ichan]->Fill(0.5,EventWeight);
		}
	}
}

void ttHAnalyzer::FillYieldHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat 	== twolSS 		&& !Is2lSSEvent()) 											continue;
			if (icat 	== twolSS 		&& (!triggermumuSS() && !triggereeSS() && !triggeremuSS()))	continue;
			if (ichan 	== MuMu 		&& (!IsMuMuEvent() 	|| !triggermumuSS())) 					continue;
			if (ichan 	== ElEl 		&& (!IsElElEvent() 	|| !triggereeSS())) 					continue;
			if (ichan 	== ElMu 		&& (!IsElMuEvent() 	|| !triggeremuSS())) 					continue;
			if (icat 	== threel 		&& (!Is3lEvent() 	|| !trigger3l4l() 	|| ichan != All)) 	continue;
			if (icat 	== Total 		&& ichan != All ) 	continue;
			if (icat 	== Total 		&& (!Is2lSSEvent() || !IsMuMuEvent() || !triggermumuSS()) && (!Is2lSSEvent() || !IsElElEvent() || !triggereeSS()) && (!Is2lSSEvent() || !IsElMuEvent() || !triggeremuSS()) && (!Is3lEvent() ||  !trigger3l4l())) 	continue;
			fHTightLep	[icat][ichan]->Fill(nTightElec+nTightMuon,EventWeight);
			fHFakeLep	[icat][ichan]->Fill(nFakeableElec+nFakeableMuon,EventWeight);
			fHLooseLep	[icat][ichan]->Fill(nLooseElec+nLooseMuon,EventWeight);
			fHTau		[icat][ichan]->Fill(nTaus,EventWeight);
			fHJet		[icat][ichan]->Fill(nJets,EventWeight);
			fHMedBTagJet[icat][ichan]->Fill(nMediumBTags,EventWeight);
			fHLosBTagJet[icat][ichan]->Fill(nLooseBTags,EventWeight);
		}
	}
}

void ttHAnalyzer::FillKinematicHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat 	== twolSS 		&& !Is2lSSEvent()) 											continue;
			if (icat 	== twolSS 		&& (!triggermumuSS() && !triggereeSS() && !triggeremuSS()))	continue;
			if (ichan 	== MuMu 		&& (!IsMuMuEvent() 	|| !triggermumuSS())) 					continue;
			if (ichan 	== ElEl 		&& (!IsElElEvent() 	|| !triggereeSS())) 					continue;
			if (ichan 	== ElMu 		&& (!IsElMuEvent() 	|| !triggeremuSS())) 					continue;
			if (icat 	== threel 		&& (!Is3lEvent() 	|| !trigger3l4l() 	|| ichan != All)) 	continue;
			if (icat 	== Total 		&& ichan != All ) 	continue;
			if (icat 	== Total 		&& (!Is2lSSEvent() || !IsMuMuEvent() || !triggermumuSS()) && (!Is2lSSEvent() || !IsElElEvent() || !triggereeSS()) && (!Is2lSSEvent() || !IsElMuEvent() || !triggeremuSS()) && (!Is3lEvent() ||  !trigger3l4l())) 	continue;
			fHPtLeading			[icat][ichan]->Fill(TightLepton[0].p.Pt(),EventWeight);
			fHPtSubLeading		[icat][ichan]->Fill(TightLepton[1].p.Pt(),EventWeight);
			fHPtSubSubLeading	[icat][ichan]->Fill(TightLepton[2].p.Pt(),EventWeight);
		}
	}
}

void ttHAnalyzer::FillMETHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat 	== twolSS 		&& !Is2lSSEvent()) 											continue;
			if (icat 	== twolSS 		&& (!triggermumuSS() && !triggereeSS() && !triggeremuSS()))	continue;
			if (ichan 	== MuMu 		&& (!IsMuMuEvent() 	|| !triggermumuSS())) 					continue;
			if (ichan 	== ElEl 		&& (!IsElElEvent() 	|| !triggereeSS())) 					continue;
			if (ichan 	== ElMu 		&& (!IsElMuEvent() 	|| !triggeremuSS())) 					continue;
			if (icat 	== threel 		&& (!Is3lEvent() 	|| !trigger3l4l() 	|| ichan != All)) 	continue;
			if (icat 	== Total 		&& ichan != All ) 	continue;
			if (icat 	== Total 		&& (!Is2lSSEvent() || !IsMuMuEvent() || !triggermumuSS()) && (!Is2lSSEvent() || !IsElElEvent() || !triggereeSS()) && (!Is2lSSEvent() || !IsElMuEvent() || !triggeremuSS()) && (!Is3lEvent() ||  !trigger3l4l())) 	continue;
			fHMET	[icat][ichan]->Fill(MET,EventWeight);
			fHMHT	[icat][ichan]->Fill(MHT,EventWeight);
			fHMETLD	[icat][ichan]->Fill(getMETLD(),EventWeight);
		}
	}
}

void ttHAnalyzer::FillMiscHistos() {
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat 	== twolSS 		&& (!Is2lSSEvent()	|| ichan != All))						continue;
			if (icat 	== twolSS 		&& (!triggermumuSS() && !triggereeSS() && !triggeremuSS()))	continue;
			if (ichan 	== MuMu 		&& (!IsMuMuEvent() 	|| !triggermumuSS())) 					continue;
			if (ichan 	== ElEl 		&& (!IsElElEvent() 	|| !triggereeSS())) 					continue;
			if (ichan 	== ElMu 		&& (!IsElMuEvent() 	|| !triggeremuSS())) 					continue;
			if (icat 	== threel 		&& (!Is3lEvent() 	|| !trigger3l4l() 	|| ichan != All)) 	continue;
			if (icat 	== Total 		&& ichan != All ) 	continue;
			if (icat 	== Total 		&& (!Is2lSSEvent() || !IsMuMuEvent() || !triggermumuSS()) && (!Is2lSSEvent() || !IsElElEvent() || !triggereeSS()) && (!Is2lSSEvent() || !IsElMuEvent() || !triggeremuSS()) && (!Is3lEvent() ||  !trigger3l4l())) 	continue;
			fHChargeSum	[icat][ichan]->Fill(getCS(),EventWeight);
			if (icat == twolSS || icat == threel || icat == Total) fHMass	[icat][ichan]->Fill((TightLepton[0].p+TightLepton[1].p).M(),EventWeight);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//	   Leptons, jets and MET selection
////////////////////////////////////////////////////////////////////////////////
Int_t ttHAnalyzer::getSelectedLeptons(){
    // Loops over the total number of muons and electrons and returns the number of leptons.
    if (Lepton.size() > 0) {
        cout << "[WARNING]: you have called this function previously... RESETTING..."<<endl;
        Lepton.clear();
        LooseLepton.clear();
		FakeableLepton.clear();
		TightLepton.clear();
    }

    vector<lepton> tmp_lepton;
    vector<lepton> tmp_looselepton;
    vector<lepton> tmp_fakeablelepton;
    vector<lepton> tmp_tightlepton;
    nTightMuon 		= 0;
    nFakeableMuon 	= 0;
    nLooseMuon 		= 0;
    nTightElec 		= 0;
    nFakeableMuon 	= 0;
    nLooseMuon 		= 0;
	nTaus			= 0;
    TLorentzVector lep;
    Int_t thetype 	= 0;

	for (Int_t i = 0; i < nLepGood; i++) {
		if(IsLooseMuon(i)){
			thetype = 4;
			nLooseMuon++;
			if(IsFakeableMuon(i)){
				thetype = 2;
				nFakeableMuon++;
				if(IsTightMuon(i)){
					thetype = 0;
					nTightMuon++;
		        }
	        }
        }
        if(IsLooseElectron(i)){
            thetype = 5;
            nLooseElec++;
	        if(IsFakeableElectron(i)){
	            thetype = 3;
	            nFakeableElec++;
		        if(IsTightElectron(i)){
		            thetype = 1;
		            nTightElec++;
		        }
	        }
        }
		if (!IsTightMuon(i) && !IsFakeableMuon(i) && !IsLooseMuon(i) && !IsTightElectron(i) && !IsFakeableElectron(i) && !IsLooseElectron(i)) continue;

        lep.SetPxPyPzE(LepGood_px[i], LepGood_py[i], LepGood_pz[i], LepGood_energy[i]);
        lepton tmpLepton(lep, LepGood_charge[i], thetype, i);
		if (thetype == 4 || thetype == 5 || thetype == 2 || thetype == 3 || thetype == 0 || thetype == 1) {
		    tmp_looselepton.push_back(tmpLepton);
		}
		if (thetype == 2 || thetype == 3 || thetype == 0 || thetype == 1) {
		    tmp_fakeablelepton.push_back(tmpLepton);
		}
		if (thetype == 0 || thetype == 1) {
		    tmp_tightlepton.push_back(tmpLepton);
		}
        tmp_lepton.push_back(tmpLepton);
    }

	TightLepton 	= SortLeptonsByPt(tmp_tightlepton);
    FakeableLepton 	= SortLeptonsByPt(tmp_fakeablelepton);
    LooseLepton 	= SortLeptonsByPt(tmp_looselepton);
    Lepton 			= SortLeptonsByPt(tmp_lepton);

	for (Int_t i = 0; i < nTauGood; i++) {
		if (IsGoodTau(i)) nTaus++;
	}

    return Lepton.size();
}

Bool_t momentumComparator(lepton i, lepton j){ return (i.p.Pt()>j.p.Pt()); }

vector<lepton> ttHAnalyzer::SortLeptonsByPt(vector<lepton>& leptons){
    vector<lepton> theLep = leptons;
    sort (theLep.begin(), theLep.end(), momentumComparator);
    return theLep;
}

//	Muon selectors
//------------------------------------------------------------------------------
Bool_t ttHAnalyzer::IsTightMuon(UInt_t iMuon,Float_t ptcut){
	if ((TMath::Abs(LepGood_pdgId[iMuon])) != 13) return false;
	if (TMath::Abs(LepGood_eta[iMuon]) > 2.4) return false;
	if (LepGood_pt[iMuon] < 10) return false;
	if (TMath::Abs(LepGood_dxy[iMuon]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iMuon]) > 0.1) return false;
	if (LepGood_sip3d[iMuon] > 8) return false;
	if (LepGood_miniRelIso[iMuon] > 0.4) return false;
	// The condition of "loose muon" is not demanded as the trees which we are
	// doing the analysis already are filtered by it.
	if (LepGood_jetBTagCSV[iMuon] > 0.89) return false;
	if (LepGood_mediumMuonId[iMuon] != 1) return false;
	if (LepGood_tightCharge[iMuon] == 0) return false;
	if (LepGood_mvaTTH[iMuon] < 0.75) return false;
    return true;
}

Bool_t ttHAnalyzer::IsFakeableMuon(UInt_t iMuon,Float_t ptcut){
	if ((TMath::Abs(LepGood_pdgId[iMuon])) != 13) return false;
	if (TMath::Abs(LepGood_eta[iMuon]) > 2.4) return false;
	if (LepGood_pt[iMuon] < 10) return false;
	if (TMath::Abs(LepGood_dxy[iMuon]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iMuon]) > 0.1) return false;
	if (LepGood_sip3d[iMuon] > 8) return false;
	if (LepGood_miniRelIso[iMuon] > 0.4) return false;
	// The condition of "loose muon" is not demanded as the trees which we are
	// doing the analysis already are filtered by it.
	if (LepGood_mvaTTH[iMuon] < 0.75) {
		if (LepGood_jetPtRatiov2[iMuon] < 0.3) return false;
		if (LepGood_jetBTagCSV[iMuon] > 0.605) return false;
	}
	else {
		if (LepGood_jetBTagCSV[iMuon] > 0.89) return false;
	}
    return true;
}

Bool_t ttHAnalyzer::IsLooseMuon(UInt_t iMuon,Float_t ptcut){
	if ((TMath::Abs(LepGood_pdgId[iMuon])) != 13) return false;
	if (TMath::Abs(LepGood_eta[iMuon]) > 2.4) return false;
	if (LepGood_pt[iMuon] < 5) return false;
	if (TMath::Abs(LepGood_dxy[iMuon]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iMuon]) > 0.1) return false;
	if (LepGood_sip3d[iMuon] > 8) return false;
	if (LepGood_miniRelIso[iMuon] > 0.4) return false;
	// The condition of "loose muon" is not demanded as the trees which we are
	// doing the analysis already are filtered by it.
    return true;
}


//	Electron selectors
//------------------------------------------------------------------------------
Bool_t ttHAnalyzer::IsTightElectron(UInt_t iElec, Float_t ptcut, Int_t an){
	if ((TMath::Abs(LepGood_pdgId[iElec])) != 11) return false;
	if (TMath::Abs(LepGood_eta[iElec]) > 2.5) return false;
	if (an == 2) {
		if (LepGood_pt[iElec] < 15) return false;
	}
	else {
		if (LepGood_pt[iElec] < 10) return false;
	}
	if (TMath::Abs(LepGood_dxy[iElec]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iElec]) > 0.1) return false;
	if (LepGood_sip3d[iElec] > 8) return false;
	if (LepGood_miniRelIso[iElec] > 0.4) return false;
	if (TMath::Abs(LepGood_eta[iElec]) < 0.8) {
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.011) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.10) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.01) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.04) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.0010) return false;
		}
	}
	else if ((TMath::Abs(LepGood_eta[iElec]) < 1.479) && (TMath::Abs(LepGood_eta[iElec]) >= 0.8)){
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.011) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.10) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.01) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.04) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.0010) return false;
		}
	}
	else if (TMath::Abs(LepGood_eta[iElec]) >= 1.479) {
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.030) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.07) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.008) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.07) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.005) return false;
		}
    }
    Float_t	 A = -0.86+(-0.85+0.86)*(abs(LepGood_eta[iElec])>0.8)+(-0.81+0.86)*(abs(LepGood_eta[iElec])>1.479);
	Float_t  B = -0.96+(-0.96+0.96)*(abs(LepGood_eta[iElec])>0.8)+(-0.95+0.96)*(abs(LepGood_eta[iElec])>1.479);
    if (LepGood_pt[iElec] > 10) {
	    if (!(LepGood_mvaIdSpring16GP[iElec] > min( A , max( B , A+(B-A)/10*(LepGood_pt[iElec]-15))) )) return false;
	}
	if (LepGood_jetBTagCSV[iElec] > 0.89) return false;
	if (LepGood_tightCharge[iElec] == 0) return false;
    if (LepGood_convVeto[iElec] > 0.89) return false;
	if (LepGood_lostHits[iElec] != 0) return false;
	if (LepGood_mvaTTH[iElec] < 0.75) return false;
	return true;
}

Bool_t ttHAnalyzer::IsFakeableElectron(UInt_t iElec, Float_t ptcut){
	if ((TMath::Abs(LepGood_pdgId[iElec])) != 11) return false;
	if (TMath::Abs(LepGood_eta[iElec]) > 2.5) return false;
	if (LepGood_pt[iElec] < 10) return false;
	if (TMath::Abs(LepGood_dxy[iElec]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iElec]) > 0.1) return false;
	if (LepGood_sip3d[iElec] > 8) return false;
	if (LepGood_miniRelIso[iElec] > 0.4) return false;
	if (TMath::Abs(LepGood_eta[iElec]) < 0.8) {
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.011) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.10) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.01) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.04) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.0010) return false;
		}
	}
	else if ((TMath::Abs(LepGood_eta[iElec]) < 1.479) && (TMath::Abs(LepGood_eta[iElec]) >= 0.8)){
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.011) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.10) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.01) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.04) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.0010) return false;
		}
	}
	else if (TMath::Abs(LepGood_eta[iElec]) >= 1.479) {
		if (LepGood_pt[iElec] > 30) {
			if (LepGood_sigmaIEtaIEta[iElec] > 0.030) return false;
            if (LepGood_hadronicOverEm[iElec] > 0.07) return false;
            if (LepGood_dEtaScTrkIn[iElec] > 0.008) return false;
            if (LepGood_dPhiScTrkIn[iElec] > 0.07) return false;
            if (LepGood_eInvMinusPInv[iElec] < -0.05 || LepGood_eInvMinusPInv[iElec] > 0.005) return false;
		}
    }
    Float_t	 A = -0.86+(-0.85+0.86)*(abs(LepGood_eta[iElec])>0.8)+(-0.81+0.86)*(abs(LepGood_eta[iElec])>1.479);
	Float_t  B = -0.96+(-0.96+0.96)*(abs(LepGood_eta[iElec])>0.8)+(-0.95+0.96)*(abs(LepGood_eta[iElec])>1.479);
    if (LepGood_pt[iElec] > 10) {
	    if (!(LepGood_mvaIdSpring16GP[iElec] > min( A , max( B , A+(B-A)/10*(LepGood_pt[iElec]-15))) )) return false;
	}
    if (LepGood_mvaTTH[iElec] < 0.75){
        if (LepGood_jetBTagCSV[iElec] > 0.605) return false;
		if (LepGood_jetPtRatiov2[iElec] < 0.3) return false;
    }
    else {
        if (LepGood_jetBTagCSV[iElec] > 0.89) return false;
    }

	if (LepGood_lostHits[iElec] != 0) return false;
	return true;
}

Bool_t ttHAnalyzer::IsLooseElectron(UInt_t iElec, Float_t ptcut){
	if ((TMath::Abs(LepGood_pdgId[iElec])) != 11) return false;
	if (TMath::Abs(LepGood_eta[iElec]) > 2.5) return false;
	if (LepGood_pt[iElec] < 7) return false;
	if (TMath::Abs(LepGood_dxy[iElec]) > 0.05) return false;
	if (TMath::Abs(LepGood_dz[iElec]) > 0.1) return false;
	if (LepGood_sip3d[iElec] > 8) return false;
	if (LepGood_miniRelIso[iElec] > 0.4) return false;
    Float_t	 A = -0.86+(-0.85+0.86)*(abs(LepGood_eta[iElec])>0.8)+(-0.81+0.86)*(abs(LepGood_eta[iElec])>1.479);
	Float_t  B = -0.96+(-0.96+0.96)*(abs(LepGood_eta[iElec])>0.8)+(-0.95+0.96)*(abs(LepGood_eta[iElec])>1.479);
    if (LepGood_pt[iElec] > 10) {
	    if (!(LepGood_mvaIdSpring16GP[iElec] > min( A , max( B , A+(B-A)/10*(LepGood_pt[iElec]-15))) )) return false;
	}
	if (LepGood_lostHits[iElec] > 2) return false;
	return true;
}

//	Tau selectors
//------------------------------------------------------------------------------

Bool_t ttHAnalyzer::IsGoodTau(UInt_t iTau) {
	if (TauGood_pt[iTau] < 20) return false;
	if (TMath::Abs(TauGood_eta[iTau]) > 2.3) return false;
	if (TauGood_idDecayModeNewDMs[iTau] == 0) return false;
    TLorentzVector tau;
    tau.SetPtEtaPhiE(TauGood_pt[iTau], TauGood_eta[iTau], TauGood_phi[iTau], sqrt(TauGood_mass[iTau]*TauGood_mass[iTau]+TauGood_pt[iTau]*TauGood_pt[iTau]));
    for (UInt_t i = 0; i < LooseLepton.size(); i++) {
        if(tau.DeltaR(LooseLepton[i].p) < 0.5) return false;
    }
    if (TauGood_idCI3hit[iTau] < 1) return false;

    //if (TauGood_idAntiE[iTau] < 1) return false;
    //if (TauGood_idAntiMu[iTau] < 1) return false;

	return true;
}

//	Jets methods
//------------------------------------------------------------------------------
Int_t ttHAnalyzer::getSelectedJets(){
	Int_t nj		= 0;
	njpt 			= 0;
	nBTags			= 0;
	nMediumBTags	= 0;
	nLooseBTags		= 0;
	if (Jet.size() > 0) {
		cout << "[WARNING]: you have called this function previously, RESETTING..."<<endl;
		Jet.clear();
	}
    TLorentzVector jt;
    for (Int_t i = 0; i < nJet; i++) {
        if(!IsGoodJet(i,gJetEtCut)) continue;

        Float_t jetbtagi      = Jet_btagCSV[i];
        Float_t jetetai       = Jet_eta[i];
        Float_t jetenergyi    = Jet_energy[i];

        jt.SetPtEtaPhiE(JetPt.at(i), jetetai, JetPhi.at(i), jetenergyi);
        Bool_t ismediumbtag = false;
        Bool_t isloosebtag 	= false;
        if (1) {																										// WOLOLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
            ismediumbtag 	= medfBTagSFnom->IsTagged(Jet_btagCSV[i], -999999, JetPt.at(i), jetetai);
			isloosebtag 	= losfBTagSFnom->IsTagged(Jet_btagCSV[i], -999999, JetPt.at(i), jetetai);
        }
        else {
			Int_t jetmcflavouri = Get<Int_t>("Jet_mcFlavour", i);
            ismediumbtag 	= medfBTagSFnom->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
			isloosebtag 	= losfBTagSFnom->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
        }
		Int_t isbtag = 0;
		if (isloosebtag) isbtag = -1;
		if (ismediumbtag) isbtag = 1;
        jet tmpjet(jt, isbtag, i);
        Jet.push_back(tmpjet);
        nj++;
        if (IsGoodJetforprecuts(i,gJetEtCut)) 	njpt++;
		if (ismediumbtag || isloosebtag) nBTags++;
		if (ismediumbtag) nMediumBTags++;
		if (isloosebtag) nLooseBTags++;
    }
    return nj;
}

Bool_t ttHAnalyzer::IsGoodJet(UInt_t ijet, Float_t ptcut){
    Float_t minDR = 0.4;
    TLorentzVector jet;
    jet.SetPtEtaPhiE(JetPt.at(ijet), Jet_eta[ijet], JetPhi.at(ijet), Jet_energy[ijet]);
    if (jet.Pt() < ptcut)     return false;
    if (abs(jet.Eta()) > 2.4) return false;
    if (Get<Int_t>("Jet_id",ijet) <= 0)    return false;
    // Remove jets close to all selected leptons...
    for(UInt_t i = 0; i < FakeableLepton.size(); i++){
        if(jet.DeltaR(FakeableLepton[i].p) < minDR) return false;
    }
    return true;
}

Bool_t ttHAnalyzer::IsGoodJetforprecuts(UInt_t ijet, Float_t ptcut){
    Float_t minDR = 0.4;
    TLorentzVector jet;
    jet.SetPtEtaPhiE(JetPt.at(ijet), Jet_eta[ijet], JetPhi.at(ijet), Jet_energy[ijet]);
    if (jet.Pt() < ptcut)     return false;
    if (abs(jet.Eta()) > 2.4) return false;
    if (Get<Int_t>("Jet_id",ijet) <= 0)    return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//	   Events selection
////////////////////////////////////////////////////////////////////////////////
Bool_t ttHAnalyzer::IsElMuEvent(){
	return (IsDileptonEvent() == -3 || IsDileptonEvent() == 3);
}

Bool_t ttHAnalyzer::IsMuMuEvent(){
	return (IsDileptonEvent() == -1 || IsDileptonEvent() == 1);
}

Bool_t ttHAnalyzer::IsElElEvent(){
    return (IsDileptonEvent() == -2 || IsDileptonEvent() == 2);
}

Bool_t ttHAnalyzer::IsSSEvent() {
	if (IsDileptonEvent() <= 0) return false;
	return true;
}

Int_t ttHAnalyzer::IsDileptonEvent(){
#ifdef DEBUGC
	cout << "IsDileptonEvent(): nTightLeptons =" << TightLepton.size()<< endl;
#endif
	if (TightLepton.size() != 2) return 0;
	Int_t select = TightLepton[0].charge*TightLepton[1].charge;
	Int_t result = 0;
	if      (TightLepton[0].type == 0 && TightLepton[1].type == 0) result = 1; 	// mu/mu
	else if (TightLepton[0].type == 1 && TightLepton[1].type == 1) result = 2; 	// el/el
    else result = 3; 															// mu/el

	fHypLepton1 = lepton(TightLepton[0]);
	fHypLepton2 = lepton(TightLepton[1]);

	if(TightLepton[0].type == 1 && TightLepton[1].type == 0){
		fHypLepton1 = lepton(TightLepton[1]);
		fHypLepton2 = lepton(TightLepton[0]);
	} //fHypLepton1 will ALWAYS be the muon in mu/el events

	result *= select; // Add charge to result
#ifdef DEBUGC
    cout << result;
	cout << " DONE!" << endl;
#endif
	return result;
}

Bool_t ttHAnalyzer::Is2lSSEvent() {
	if (nTightElec + nTightMuon != 2) 	return false;
	if (!IsSSEvent()) 					return false;
	if (TightLepton[0].p.Pt() < 20) 	return false;
	if (TightLepton[1].type == 1){
		if (TightLepton[1].p.Pt() < 15) return false;
	}

	// UInt_t nicejets = 0;
    // for (Int_t i = 0; i < nJet; i++) {
    //     if(IsGoodJetforprecuts(i,gJetEtCut)) nicejets++;
	// }
	// if (nicejets < 4) 					return false;

	if (nJets < 4) 					return false;

	if (IsElElEvent()){
		if (abs((fHypLepton1.p + fHypLepton2.p).M() - Zm) < 10) return false;
		if (getMETLD() < 0.2) return false;
	}
	return true;
}

Bool_t ttHAnalyzer::Is3lEvent() {
	if (nTightElec + nTightMuon < 3) return false;
	if (TightLepton[0].p.Pt() < 20) return false;
	if (TightLepton[1].p.Pt() < 10 || TightLepton[2].p.Pt() < 10) return false;
	if ((nLooseMuon + nLooseElec) > 1) {
		for (UInt_t i = 0; i < nLooseMuon + nLooseElec; i++) {
			for (UInt_t j = i; j < nLooseMuon + nLooseElec; j++) {
				if (LooseLepton[i].type != LooseLepton[j].type) continue;
				if (LooseLepton[i].charge*LooseLepton[j].charge > 0) continue;
				if (abs((LooseLepton[i].p+LooseLepton[j].p).M() - Zm) < 10) return false;
			}
		}
	}
	UInt_t twolds = 0;
	for (UInt_t i = 0; i < nTightMuon + nTightElec; i++) {
		for (UInt_t j = i; j < nTightMuon + nTightElec; j++) {
			if (TightLepton[i].type != TightLepton[j].type) continue;
			if (TightLepton[i].charge*TightLepton[j].charge < 0) twolds = 1;
		}
	}
	if (twolds != 1 && nJets < 4) {
		if (getMETLD() < 0.2) return false;
	} else if (nJets < 4) {
		if (getMETLD() < 0.3) return false;
	}
	if (abs(TightLepton[0].charge + TightLepton[1].charge + TightLepton[2].charge) != 1) return false;
	return true;
}

Bool_t ttHAnalyzer::PassesPreCuts(){
	// This function requires n[]Elec and n[]Muon to be initialized and
	// declared, where [] = { Tight, Fakeable, Loose }.
	if ((nTightElec + nTightMuon) < 2) return false;
	if (LooseLepton.size() > 1) {
		lepton tmp_looselep0;
		lepton tmp_looselep1;
		tmp_looselep0 = LooseLepton[0];
		for (UInt_t i = 0; i < LooseLepton.size(); i++) {
			tmp_looselep1 = LooseLepton[i+1];
			if ((tmp_looselep0.p + tmp_looselep1.p).M() < 12) return false;
		}
	}

	// =========== AQUÍ

	if (nJets < 2) return false;
	if (nLooseBTags < 2) {
		if (nMediumBTags < 1) return false;
	}

	// if (njpt < 2) return false;
	//
	// TLorentzVector jt;
	// UInt_t njloose;
	// UInt_t njmedium;
	//
    // for (Int_t i = 0; i < nJet; i++) {
    //     if(!IsGoodJetforprecuts(i,gJetEtCut)) continue;
	//
    //     Float_t jetbtagi      = Jet_btagCSV[i];
    //     Float_t jetetai       = Jet_eta[i];
    //     Float_t jetenergyi    = Jet_energy[i];
	//
    //     jt.SetPtEtaPhiE(JetPt.at(i), jetetai, JetPhi.at(i), jetenergyi);
    //     Bool_t isbtagm = false;
    //     Bool_t isbtagl = false;
	//
    //     if (1) {																						// WOLOLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
    //         isbtagm = medfBTagSFnom->IsTagged(Jet_btagCSV[i], -999999, JetPt.at(i), jetetai);
    //         isbtagl = losfBTagSFnom->IsTagged(Jet_btagCSV[i], -999999, JetPt.at(i), jetetai);
    //     }
    //     else {
	// 		Int_t jetmcflavouri = Get<Int_t>("Jet_mcFlavour", i);
    //         isbtagm = medfBTagSFnom->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    //         isbtagl = losfBTagSFnom->IsTagged(jetbtagi, jetmcflavouri, JetPt.at(i), jetetai);
    //     }
	// 	if (isbtagm) njmedium++;
	// 	if (isbtagl) njloose++;
    // }
	//
    // if (njloose < 2) {
    // 	if (njmedium < 1 ) return false;
    // }
	return true;
}


////////////////////////////////////////////////////////////////////////////////
//	   Trigger methods
////////////////////////////////////////////////////////////////////////////////
/*	The triggers for each region are defined exactly the same as in the CMS Draft Note of
03-03-2016 ("Search for ttH in multilepton final states at 13 TeV").
*/
Bool_t ttHAnalyzer::triggermumuSS() {
    Bool_t pass = false;
    //cout<<"COMIENZOMUMUSS"<<endl;
    pass =  (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoMu20_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoTkMu20_v"));
    //cout<<"FINMUMUSS"<<endl;
    return pass;
}

Bool_t ttHAnalyzer::triggereeSS(){
    Bool_t pass = false;
    //cout<<"COMIENZOEESS"<<endl;
    pass =  (Get<Int_t>("HLT_BIT_HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
           Get<Int_t>("HLT_BIT_HLT_Ele25_WPTight_Gsf_v") ||
           Get<Int_t>("HLT_BIT_HLT_Ele45_WPLoose_Gsf_v"));
    //cout<<"FINEESS"<<endl;
    return pass;
}

Bool_t ttHAnalyzer::triggeremuSS(){
    Bool_t pass = false;
    //cout<<"COMIENZOEMUSS"<<endl;
    pass =  (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoMu20_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoTkMu20_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele25_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele45_WPLoose_Gsf_v"));
    //cout<<"FINEMUSS"<<endl;
    return pass;
}

Bool_t ttHAnalyzer::trigger3l4l(){
    Bool_t pass = false;
    //cout<<"COMIENZO3L4L"<<endl;
    pass =  (Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoMu20_v") ||
            Get<Int_t>("HLT_BIT_HLT_IsoTkMu20_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele25_WPTight_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele45_WPLoose_Gsf_v") ||
            Get<Int_t>("HLT_BIT_HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v") ||
            Get<Int_t>("HLT_BIT_HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v") ||
            Get<Int_t>("HLT_BIT_HLT_TripleMu_12_10_5_v") ||
            Get<Int_t>("HLT_BIT_HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v"));
    //cout<<"FIN3L4LSS"<<endl;
    return pass;
}

////////////////////////////////////////////////////////////////////////////////
//	   Set/reset methods
////////////////////////////////////////////////////////////////////////////////
void ttHAnalyzer::SetOriginalObjects(){
	#ifdef DEBUGC
		cout << "DEBUGC - Beginning of SetOriginalObjects"<< endl;
	#endif
	ResetHypLeptons();

	// SAVING ORIGINAL VALUES FOR MET, JET, LEPTONS for SYST
	JetEt.clear();
	JetPt.clear();
	JetPhi.clear();
	MuPx.clear();
	MuPy.clear();
	MuPz.clear();
	MuEnergy.clear();
	ElPx.clear();
	ElPy.clear();
	ElPz.clear();
	ElEnergy.clear();

	// Save original values for MET, Jets and Leptons
	TLorentzVector j;
	for (Int_t i=0; i < nJet; i++){
		j.SetPxPyPzE(Jet_px[i], Jet_py[i], Jet_pz[i], Jet_energy[i]);
		JetEt.push_back(j.Et());
		JetPt.push_back(j.Pt());
		JetPhi.push_back(j.Phi());
	}
	for (Int_t i=0; i < nLepGood; i++){
		if(TMath::Abs(LepGood_pdgId[i]) == 11){
			ElPx.push_back(LepGood_px[i]);
			ElPy.push_back(LepGood_py[i]);
			ElPz.push_back(LepGood_pz[i]);
			ElEnergy.push_back(LepGood_energy[i]);
		}
	}
	for (Int_t i=0; i < nLepGood; i++){
		if(TMath::Abs(LepGood_pdgId[i]) == 13){
			MuPx.push_back(LepGood_px[i]);
			MuPy.push_back(LepGood_py[i]);
			MuPz.push_back(LepGood_pz[i]);
			MuEnergy.push_back(LepGood_energy[i]);
		}
	}
}

void ttHAnalyzer::SetEventObjects(){
	#ifdef DEBUGC
		cout << "DEBUGC - SetEventObjects"<< endl;
	#endif

	EventWeight 	= 1.;
	if (!gIsData) {
		EventWeight = gWeight;
		if (gIsMCatNLO) EventWeight *= genWeight;
	}

	// Counters initialization
	nJets       	= 0;
	nLeptons    	= 0;
    nTightMuon 		= 0;
    nFakeableMuon 	= 0;
    nLooseMuon 		= 0;
    nTightElec 		= 0;
    nFakeableElec 	= 0;
    nLooseElec 		= 0;
	nBTags			= 0;
	nMediumBTags	= 0;
	nLooseBTags		= 0;
	nTaus			= 0;
	njpt			= 0;

	MET  = 0.;
	MHT  = 0.;


	// Read and save objects
	Jet.clear();
	Lepton.clear();
	LooseLepton.clear();
	FakeableLepton.clear();
	TightLepton.clear();


	MET     =	Get<Float_t>("met_pt"); //met
	MHT		=	Get<Float_t>("mhtJet25");

	nLeptons 	= getSelectedLeptons(); // Also gets n[]Muon/Elec
	nJets    	= getSelectedJets(); // Gets the total number of jets.

	//MHT 	= getMHT(); //mht
}
void ttHAnalyzer::ResetHypLeptons(){
  TLorentzVector vec(0., 0., 0., 0.);
  fHypLepton1 = lepton(vec, 0, -1, -1);
  fHypLepton2 = lepton(vec, 0, -1, -1);
}

void ttHAnalyzer::setMET(Float_t newmet) {MET = newmet;}

////////////////////////////////////////////////////////////////////////////////
//	   Get methods
////////////////////////////////////////////////////////////////////////////////
Float_t ttHAnalyzer::getHT(){
	Float_t ht(0);
	for (UInt_t i=0; i<Jet.size(); i++) ht+=Jet[i].p.Pt();
	return ht;
}

Float_t ttHAnalyzer::getMHT() {
	Float_t mht;
	mht = 0;
	for (UInt_t i = 0; i < Jet.size(); i++) mht += Jet[i].p.Pt();
	for (UInt_t i = 0; i < Lepton.size(); i++) mht += Lepton[i].p.Pt();
	return mht;
}

Float_t ttHAnalyzer::getMETLD() {
	Float_t metld;
	metld = MET * 0.00397 + MHT * 0.00265;
	return metld;
}

Float_t ttHAnalyzer::getMET(){ return MET; }

Int_t ttHAnalyzer::getCS(){
	Int_t cs = 0;
	for (UInt_t i = 0; i < TightLepton.size(); i++) {
		cs += TightLepton[i].charge;
	}
	return cs;
}
