#include "mt2.C"
//gSystem->loadMacro("mt2.C");

//TString path = "/pool/ciencias/HeppyTreesDR80X/v2/";
TString path = "/pool/ciencias/HeppyTreesSummer16/v2/jet25/";
TString outputpath = "./";
TString namettbar = "TTJets_MLM_0";
TString namettbar2 = "TTbar_Powheg_ext_0";
TString namestop  = "T2tt_150to250_0";
TString prefix = "Tree_";
TString sufix = ".root";

// Funciones
float getDPhibMet();
TLorentzVector getPtllb();
float getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, float mass);
float getMT2lb();
float getMT2b();
float getMT2ll();

TTree* fChain;
TFile* file;
TFile* outputfile;
//const TString process[nprocess]     = {"ttbar_Powheg", "ttbar_MLM", "ttbar_MLM_fastSim", "stop_175_1", "stop_250_75"};
//const TString processfile[nprocess] = {"TTbar_Powheg_ext_0", "TTJets_MLM_0", "TTJets_MLM_FastSim_0", "T2tt_150to250_0", "T2tt_150to250_0"};

const int nprocess = 15;
const TString process[nprocess]     = {"ttbar_Powheg", "S_217_50", "S_233_50", "S_275_50", "S_200_50","S_225_50", "S_250_50", "SFS_200_50","SFS_225_50", "SFS_250_50", "SFStt_200_50","SFStt_225_50", "SFStt_250_50", "ttbar_MLM", "ttbar_MLM_FastSim"};
const TString processfile[nprocess] = {"TTbar_Powheg_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_250to350_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_200_50_FS_summer_0", "T2tt_225_50_FS_summer_0", "T2tt_250_50_FS_summer_0", "T2tt_200_50_FS_xqcut20_0", "T2tt_225_50_FS_xqcut20_0", "T2tt_250_50_FS_xqcut20_0", "TTJets_MLM_0", "TTJets_MLM_FastSim_0"};

//const TString process[nprocess]     = {"ttbar_Powheg", "S_217_50", "S_233_50", "S_275_50", "S_200_50","S_225_50", "S_250_50", "SFS_200_50","SFS_225_50", "SFS_250_50", "SFStt_200_50","SFStt_225_50", "SFStt_250_50", "ttbar_MLM"};
//const TString processfile[nprocess] = {"TTbar_Powheg_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_250to350_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_150to250_0", "T2tt_200_50_FS_summer_0", "T2tt_225_50_FS_summer_0", "T2tt_250_50_FS_summer_0", "T2tt_200_50_FS_xqcut20_0", "T2tt_225_50_FS_xqcut20_0", "T2tt_250_50_FS_xqcut20_0", "TTJets_MLM_0"};

//
// S_mass_mass     --> Scan FastSim
// SFS_mass_mass   --> Full Sim con FastSim-like configuration
// SFStt_mass_mass --> Full Sim con ttbar-like configuration
//
//

TH1F* njets[nprocess];
TH1F* nbjets[nprocess];
TH1F* lep0pt[nprocess];
TH1F* lep1pt[nprocess];
TH1F* jet0pt[nprocess];
TH1F* jet1pt[nprocess];
TH1F* ht[nprocess];
TH1F* met[nprocess];
TH1F* meff[nprocess];

TH1F* dileppt[nprocess];
TH1F* mll[nprocess];
TH1F* mt2ll[nprocess];
TH1F* mt2ll_no0[nprocess];
TH1F* mt2lb[nprocess];
TH1F* mt2bb[nprocess];
TH1F* dphill[nprocess];
TH1F* dphijj[nprocess];
TH1F* dphilepjet[nprocess];
TH1F* dphilepmet[nprocess];
TH1F* dphijetmet[nprocess];

Int_t           nTrueGenLep;
// ---------------------------------------------------------------------------------------------
Int_t           nLepGood;
Int_t           ngenLep;
Int_t           genLep_motherId[10];   //[ngenLep]
Int_t           genLep_grandmotherId[10];   //[ngenLep]
Int_t           genLep_sourceId[10];   //[ngenLep]
Float_t         genLep_charge[10];   //[ngenLep]
Int_t           genLep_status[10];   //[ngenLep]
Int_t           genLep_isPromptHard[10];   //[ngenLep]
Int_t           genLep_pdgId[10];   //[ngenLep]
Float_t         genLep_pt[10];   //[ngenLep]
Float_t         genLep_eta[10];   //[ngenLep]
Float_t         genLep_phi[10];   //[ngenLep]
Float_t         genLep_mass[10];   //[ngenLep]
Int_t           genLep_motherIndex[10];   //[ngenLep]
Int_t           ngenLepFromTau;
Int_t           genLepFromTau_motherId[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_grandmotherId[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_sourceId[4];   //[ngenLepFromTau]
Float_t         genLepFromTau_charge[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_status[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_isPromptHard[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_pdgId[4];   //[ngenLepFromTau]
Float_t         genLepFromTau_pt[4];   //[ngenLepFromTau]
Float_t         genLepFromTau_eta[4];   //[ngenLepFromTau]
Float_t         genLepFromTau_phi[4];   //[ngenLepFromTau]
Float_t         genLepFromTau_mass[4];   //[ngenLepFromTau]
Int_t           genLepFromTau_motherIndex[4];   //[ngenLepFromTau]
Int_t           nJet;
Int_t           Jet_id[15];   //[nJet]
Float_t         Jet_pt[15];   //[nJet]
Float_t         Jet_eta[15];   //[nJet]
Float_t         Jet_phi[15];   //[nJet]
Float_t         Jet_mass[15];   //[nJet]
Int_t           Jet_mcFlavour[15];   //[nJet]
Float_t         met_genPt;
Float_t         met_genPhi;
Int_t           GenSusyMNeutralino;
Int_t           GenSusyMStop;


TBranch        *b_ngenLepFromTau;   //!
TBranch        *b_genLepFromTau_motherId;   //!
TBranch        *b_genLepFromTau_grandmotherId;   //!
TBranch        *b_genLepFromTau_sourceId;   //!
TBranch        *b_genLepFromTau_charge;   //!
TBranch        *b_genLepFromTau_status;   //!
TBranch        *b_genLepFromTau_isPromptHard;   //!
TBranch        *b_genLepFromTau_pdgId;   //!
TBranch        *b_genLepFromTau_pt;   //!
TBranch        *b_genLepFromTau_eta;   //!
TBranch        *b_genLepFromTau_phi;   //!
TBranch        *b_genLepFromTau_mass;   //!
TBranch        *b_genLepFromTau_motherIndex;   //!
TBranch        *b_nJet;   //!
TBranch        *b_Jet_id;   //!
TBranch        *b_Jet_pt;   //!
TBranch        *b_Jet_eta;   //!
TBranch        *b_Jet_phi;   //!
TBranch        *b_Jet_mass;   //!
TBranch        *b_Jet_mcFlavour;   //!
TBranch        *b_ngenLep;   //!
TBranch        *b_nLepGood;   //!
TBranch        *b_genLep_motherId;   //!
TBranch        *b_genLep_grandmotherId;   //!
TBranch        *b_genLep_sourceId;   //!
TBranch        *b_genLep_charge;   //!
TBranch        *b_genLep_status;   //!
TBranch        *b_genLep_isPromptHard;   //!
TBranch        *b_genLep_pdgId;   //!
TBranch        *b_genLep_pt;   //!
TBranch        *b_genLep_eta;   //!
TBranch        *b_genLep_phi;   //!
TBranch        *b_genLep_mass;   //!
TBranch        *b_genLep_motherIndex;   //!
TBranch        *b_met_genPt;   //!
TBranch        *b_met_genPhi;   //!
TBranch        *b_GenSusyMNeutralino;   //!
TBranch        *b_GenSusyMStop;   //!



//----------------------------------------------------------------------------------------------





void Init(){
	for(int i = 0; i < nprocess; i++){
		njets[i]  = new TH1F("NJets_"  + process[i], "NJets_"   + process[i], 9, -0.5, 8.5);
		nbjets[i] = new TH1F("NBJets_" + process[i], "NJets_"   + process[i], 9, -0.5, 8.5);
		lep0pt[i] = new TH1F("Lep0Pt_" + process[i], "Lep0Pt_"  + process[i], 30, 0, 600);
		lep1pt[i] = new TH1F("Lep1Pt_" + process[i], "Lep1Pt_"  + process[i], 30, 0, 600);
		jet0pt[i] = new TH1F("Jet0Pt_" + process[i], "Jet0Pt_"  + process[i], 40, 0, 800);
		jet1pt[i] = new TH1F("Jet1Pt_" + process[i], "Jet1Pt_"  + process[i], 40, 0, 800);
		ht[i]     = new TH1F("HT_"     + process[i], "HT_"      + process[i], 40, 0, 1600);
		met[i]    = new TH1F("MET_"    + process[i], "MET_"     + process[i], 30, 0, 600);
		meff[i]   = new TH1F("Meff_"   + process[i], "Meff_"    + process[i], 40, 0, 1600);

		mll[i]    = new TH1F("Mll_"   + process[i], "Mll_"    + process[i], 20, 0, 400);
		dileppt[i] = new TH1F("DilepPt_" + process[i], "DilepPt_" + process[i], 15, 0, 300);
		mt2ll[i]   = new TH1F("MT2_" + process[i], "MT2_" + process[i], 20, 0, 140);
		mt2ll_no0[i]   = new TH1F("MT2no0_" + process[i], "MT2no0_" + process[i], 20, 0, 120);
		mt2lb[i] = new TH1F("MT2lblb_" + process[i], "MT2lblb_" + process[i], 30, 0, 600);
		mt2bb[i] = new TH1F("MT2bb_" + process[i], "MT2bb_" + process[i], 30, 0, 600);
		dphill[i] = new TH1F("DeltaPhill_" + process[i], "DeltaPhill_" + process[i], 20, -3.2, 3.2);
		dphijj[i] = new TH1F("DeltaPhijj_" + process[i], "DeltaPhijj_" + process[i], 20, -3.2, 3.2);
		dphilepjet[i] = new TH1F("DeltaPhilj_" + process[i], "DeltaPhilj_" + process[i], 20, -3.2, 3.2);
		dphilepmet[i] = new TH1F("DeltaPhilm_" + process[i], "DeltaPhilm_" + process[i], 20, -3.2, 3.2);
		dphijetmet[i] = new TH1F("DeltaPhijm_" + process[i], "DeltaPhijm_" + process[i], 20, -3.2, 3.2);
	}
}

void SBA(){
	fChain->SetBranchAddress("met_genPt", &met_genPt, &b_met_genPt);
	fChain->SetBranchAddress("met_genPhi", &met_genPhi, &b_met_genPhi);
	fChain->SetBranchAddress("ngenLepFromTau", &ngenLepFromTau, &b_ngenLepFromTau);
	fChain->SetBranchAddress("genLepFromTau_motherId", genLepFromTau_motherId, &b_genLepFromTau_motherId);
	fChain->SetBranchAddress("genLepFromTau_grandmotherId", genLepFromTau_grandmotherId, &b_genLepFromTau_grandmotherId);
	fChain->SetBranchAddress("genLepFromTau_sourceId", genLepFromTau_sourceId, &b_genLepFromTau_sourceId);
	fChain->SetBranchAddress("genLepFromTau_charge", genLepFromTau_charge, &b_genLepFromTau_charge);
	fChain->SetBranchAddress("genLepFromTau_status", genLepFromTau_status, &b_genLepFromTau_status);
	fChain->SetBranchAddress("genLepFromTau_isPromptHard", genLepFromTau_isPromptHard, &b_genLepFromTau_isPromptHard);
	fChain->SetBranchAddress("genLepFromTau_pdgId", genLepFromTau_pdgId, &b_genLepFromTau_pdgId);
	fChain->SetBranchAddress("genLepFromTau_pt", genLepFromTau_pt, &b_genLepFromTau_pt);
	fChain->SetBranchAddress("genLepFromTau_eta", genLepFromTau_eta, &b_genLepFromTau_eta);
	fChain->SetBranchAddress("genLepFromTau_phi", genLepFromTau_phi, &b_genLepFromTau_phi);
	fChain->SetBranchAddress("genLepFromTau_mass", genLepFromTau_mass, &b_genLepFromTau_mass);
	fChain->SetBranchAddress("genLepFromTau_motherIndex", genLepFromTau_motherIndex, &b_genLepFromTau_motherIndex);
	fChain->SetBranchAddress("nJet", &nJet, &b_nJet);
	fChain->SetBranchAddress("Jet_id", Jet_id, &b_Jet_id);
	fChain->SetBranchAddress("Jet_pt", Jet_pt, &b_Jet_pt);
	fChain->SetBranchAddress("Jet_eta", Jet_eta, &b_Jet_eta);
	fChain->SetBranchAddress("Jet_phi", Jet_phi, &b_Jet_phi);
	fChain->SetBranchAddress("Jet_mass", Jet_mass, &b_Jet_mass);
	fChain->SetBranchAddress("Jet_mcFlavour", Jet_mcFlavour, &b_Jet_mcFlavour);
	fChain->SetBranchAddress("nLepGood", &nLepGood, &b_nLepGood);
	fChain->SetBranchAddress("ngenLep", &ngenLep, &b_ngenLep);
	fChain->SetBranchAddress("genLep_motherId", genLep_motherId, &b_genLep_motherId);
	fChain->SetBranchAddress("genLep_grandmotherId", genLep_grandmotherId, &b_genLep_grandmotherId);
	fChain->SetBranchAddress("genLep_sourceId", genLep_sourceId, &b_genLep_sourceId);
	fChain->SetBranchAddress("genLep_charge", genLep_charge, &b_genLep_charge);
	fChain->SetBranchAddress("genLep_status", genLep_status, &b_genLep_status);
	fChain->SetBranchAddress("genLep_isPromptHard", genLep_isPromptHard, &b_genLep_isPromptHard);
	fChain->SetBranchAddress("genLep_pdgId", genLep_pdgId, &b_genLep_pdgId);
	fChain->SetBranchAddress("genLep_pt", genLep_pt, &b_genLep_pt);
	fChain->SetBranchAddress("genLep_eta", genLep_eta, &b_genLep_eta);
	fChain->SetBranchAddress("genLep_phi", genLep_phi, &b_genLep_phi);
	fChain->SetBranchAddress("genLep_mass", genLep_mass, &b_genLep_mass);
	fChain->SetBranchAddress("genLep_motherIndex", genLep_motherIndex, &b_genLep_motherIndex);
	fChain->SetBranchAddress("GenSusyMNeutralino", &GenSusyMNeutralino, &b_GenSusyMNeutralino);
	fChain->SetBranchAddress("GenSusyMStop", &GenSusyMStop, &b_GenSusyMStop);

}

void loop(Int_t iprocess = 0){
	file = TFile::Open(path + prefix + processfile[iprocess] + sufix);  
	file->GetObject("tree", fChain);
	SBA();
	int nEntries = fChain->GetEntries();
	TLorentzVector lep0 = TLorentzVector(0,0,0,0);
	TLorentzVector lep1 = TLorentzVector(0,0,0,0);
	TLorentzVector jet0 = TLorentzVector(0,0,0,0);
	TLorentzVector jet1 = TLorentzVector(0,0,0,0);
  TLorentzVector mett = TLorentzVector(0,0,0,0);
  int nJets = 0; int nBJets = 0;
  float ht_ = 0; float meff_ = 0;

  float dphill_; float dphilepjet_; float dileppt_; float mll_;
  float dphilepmet_; float dphijetmet_; float dphijj_;
  float mt2ll_; float mt2lb_; float mt2bb_;

  int p5 = int(float(nEntries)/20);
	// LOOP
	for(Int_t k = 0; k<nEntries; k++){
	//for(Int_t k = 0; k<10000; k++){
    if(k%p5 == 0) cout << " --> " << Form("%1.1f", float(k*100)/nEntries) << " %" << endl;
		fChain->GetEntry(k);

   // if(process[iprocess] == "S_217_50" && (GenSusyMStop != 217 || GenSusyMNeutralino !=  50)) continue;
   // if(process[iprocess] == "S_233_50" && (GenSusyMStop != 233 || GenSusyMNeutralino !=  50)) continue;
   // if(process[iprocess] == "S_275_50" && (GenSusyMStop != 275 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 1 && (GenSusyMStop != 217 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 2 && (GenSusyMStop != 233 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 3 && (GenSusyMStop != 275 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 4 && (GenSusyMStop != 200 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 5 && (GenSusyMStop != 225 || GenSusyMNeutralino !=  50)) continue;
    if(iprocess == 6 && (GenSusyMStop != 250 || GenSusyMNeutralino !=  50)) continue;

		if(nLepGood < 2) continue;
		if(ngenLep < 2) continue;
		if(nJet < 2)    continue;
		lep0 = TLorentzVector(0,0,0,0);  lep1 = TLorentzVector(0,0,0,0);
		jet0 = TLorentzVector(0,0,0,0);  jet1 = TLorentzVector(0,0,0,0);
    mett = TLorentzVector(0,0,0,0);
    nJets = 0; nBJets = 0; ht_ = 0; meff_ = 0;
    nTrueGenLep = 0;

		for(int i = 0; i < ngenLep; i++){
			if(abs(genLep_motherId[i]) != 24 || abs(genLep_grandmotherId[i]) != 6) continue; 
			else{
        nTrueGenLep++;
				if(lep0.Pt() == 0) lep0.SetPtEtaPhiM(genLep_pt[i], genLep_eta[i], genLep_phi[i], genLep_mass[i]);
				else if(lep1.Pt() == 0) lep1.SetPtEtaPhiM(genLep_pt[i], genLep_eta[i], genLep_phi[i], genLep_mass[i]);
				else continue;
			}
		}
    if(nTrueGenLep < 2) continue;
    if(lep0.Pt() == 0 || lep1.Pt() == 0) continue;

    for(int i = 0; i < nJet; i++){
      if(Jet_pt[i] > 30 && abs(Jet_eta[i]) < 2.4){
        nJets++;
        if( abs(Jet_mcFlavour[i]) == 5) nBJets++;
        if     (jet0.Pt() == 0) jet0.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
        else if(jet1.Pt() == 0) jet1.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
        ht_ += Jet_pt[i];
      }
    }
    mett.SetPtEtaPhiM(met_genPt, 0, met_genPhi, 0);

    meff_ = jet0.Pt() + jet1.Pt() + lep0.Pt() + lep1.Pt() + met_genPt;
    mll_ = (lep0+lep1).M();
    dileppt_ = (lep0+lep1).Pt();
    dphill_ = lep0.DeltaPhi(lep1);
    dphijj_ = jet0.DeltaPhi(jet1);
    dphilepjet_ = lep0.DeltaPhi(jet0);
    dphilepmet_ = lep0.DeltaPhi(mett);
    dphijetmet_ = jet0.DeltaPhi(mett);
		mt2ll_ = getMT2ll(); mt2bb_ = getMT2b(); mt2lb_ = getMT2lb();

			njets[iprocess]  ->Fill(nJets);
			nbjets[iprocess] ->Fill(nBJets);
			lep0pt[iprocess] ->Fill(lep0.Pt());
			lep1pt[iprocess] ->Fill(lep1.Pt());
			jet0pt[iprocess] ->Fill(jet0.Pt());
			jet1pt[iprocess] ->Fill(jet1.Pt());
			ht[iprocess]     ->Fill(ht_);
			met[iprocess]    ->Fill(met_genPt);
			meff[iprocess]   ->Fill(meff_);
			mll[iprocess]      ->Fill(mll_); 
			dileppt[iprocess] ->Fill(dileppt_); 
			mt2ll[iprocess]   ->Fill(mt2ll_); 
      if(mt2ll_ > 0.) mt2ll_no0[iprocess]   ->Fill(mt2ll_); 
			mt2lb[iprocess] ->Fill(mt2lb_); 
			mt2bb[iprocess] ->Fill(mt2bb_); 
			dphill[iprocess] ->Fill(dphill_); 
			dphijj[iprocess] ->Fill(dphijj_); 
			dphilepjet[iprocess] ->Fill(dphilepjet_); 
			dphilepmet[iprocess] ->Fill(dphilepmet_); 
			dphijetmet[iprocess] ->Fill(dphijetmet_); 

	} //End LOOP --> 
	cout << " --> End!!" << endl;
	file->Close();
}


void genAnal(){
	//outputfile = new TFile(outputpath + process[iprocess] + ".root", "recreate");
	Init();
	for(int iprocess = 0; iprocess < nprocess; iprocess++){
		loop(iprocess);
	}
	outputfile = new TFile(outputpath + "genVars.root", "recreate");
	for(int iprocess = 0; iprocess < nprocess; iprocess++){
		njets[iprocess]  ->Write();
		nbjets[iprocess] ->Write();
		lep0pt[iprocess] ->Write();
		lep1pt[iprocess] ->Write();
		jet0pt[iprocess] ->Write();
		jet1pt[iprocess] ->Write();
		ht[iprocess]     ->Write();
		met[iprocess]    ->Write();
		meff[iprocess]   ->Write();
		mll[iprocess]      ->Write();
		dileppt[iprocess] ->Write();
		mt2ll[iprocess]   ->Write();
		mt2ll_no0[iprocess]   ->Write();
		mt2lb[iprocess] ->Write();
		mt2bb[iprocess] ->Write();
		dphill[iprocess] ->Write();
		dphijj[iprocess] ->Write();
		dphilepjet[iprocess]->Write();
		dphilepmet[iprocess]->Write();
		dphijetmet[iprocess]->Write();
	}
	outputfile->Close();
}


float getMT2ll(){
	TLorentzVector plep1, plep2;
	plep1.SetPtEtaPhiM(genLep_pt[0], genLep_eta[0], genLep_phi[0], genLep_mass[0]);
	plep2.SetPtEtaPhiM(genLep_pt[1], genLep_eta[1], genLep_phi[1], genLep_mass[1]);
	TLorentzVector pmet;
	pmet.SetPtEtaPhiM(met_genPt, 0., met_genPhi, 0.);
	return getMT2(plep1, plep2, pmet, 0.);
}

float getMT2b(){
	TLorentzVector plep1, plep2;
	plep1.SetPtEtaPhiM(genLep_pt[0], genLep_eta[0], genLep_phi[0], genLep_mass[0]);
	plep2.SetPtEtaPhiM(genLep_pt[1], genLep_eta[1], genLep_phi[1], genLep_mass[1]);
	TLorentzVector pmet;
	pmet.SetPtEtaPhiM(met_genPt, 0., met_genPhi, 0.);
	TLorentzVector pjet0; TLorentzVector pjet1;
	pjet0.SetPtEtaPhiM(Jet_pt[0], Jet_eta[0], Jet_phi[0], Jet_mass[0]);
	pjet1.SetPtEtaPhiM(Jet_pt[1], Jet_eta[1], Jet_phi[1], Jet_mass[1]);
	TLorentzVector lv;
	lv.SetPtEtaPhiM(TMath::Sqrt( pow(pmet.Px()+(plep1+plep2).Px(), 2) +  pow(pmet.Py()+(plep1+plep2).Py(), 2)), 0., TMath::ATan2(pmet.Py()+(plep1+plep2).Py(),pmet.Px()+(plep1+plep2).Px()), 0.);
	return getMT2(pjet0, pjet1, lv, 80.398);
}

float getMT2lb(){
	TLorentzVector plep1, plep2;
	plep1.SetPtEtaPhiM(genLep_pt[0], genLep_eta[0], genLep_phi[0], genLep_mass[0]);
	plep2.SetPtEtaPhiM(genLep_pt[1], genLep_eta[1], genLep_phi[1], genLep_mass[1]);
	TLorentzVector pmet;
	pmet.SetPtEtaPhiM(met_genPt, 0., met_genPhi, 0.);
	TLorentzVector pjet0; TLorentzVector pjet1;
	pjet0.SetPtEtaPhiM(Jet_pt[0], Jet_eta[0], Jet_phi[0], Jet_mass[0]);
	pjet1.SetPtEtaPhiM(Jet_pt[1], Jet_eta[1], Jet_phi[1], Jet_mass[1]);
	float MT2llbb00, MT2llbb01, MT2llbb;
	float METx = pmet.Px(); float METy = pmet.Py();
	float MET = pmet.Pt(); float MET_phi = pmet.Phi();
	TLorentzVector LepPlusBtagJet00 = plep1 + pjet0;
	TLorentzVector LepPlusBtagJet10 = plep2 + pjet0;
	TLorentzVector LepPlusBtagJet11 = plep2 + pjet1;
	TLorentzVector LepPlusBtagJet01 = plep1 + pjet1;
	if (LepPlusBtagJet11.M()<173 && LepPlusBtagJet00.M()<173 && (LepPlusBtagJet10.M()>173 || LepPlusBtagJet01.M()>173))
		MT2llbb=getMT2(LepPlusBtagJet00, LepPlusBtagJet11, pmet, 0.);
	else if ((LepPlusBtagJet11.M()>173 || LepPlusBtagJet00.M()>173) && LepPlusBtagJet10.M()<173 && LepPlusBtagJet01.M()<173)
		MT2llbb=getMT2(LepPlusBtagJet01, LepPlusBtagJet10, pmet, 0.);
	else if (LepPlusBtagJet11.M()<173 && LepPlusBtagJet00.M()<173 && LepPlusBtagJet10.M()<173 && LepPlusBtagJet01.M()<173) {
		if ( fabs(LepPlusBtagJet11.M()-LepPlusBtagJet00.M()) < fabs(LepPlusBtagJet10.M()-LepPlusBtagJet01.M()) )
			MT2llbb=getMT2(LepPlusBtagJet00, LepPlusBtagJet11, pmet, 0.);
		else
			MT2llbb=getMT2(LepPlusBtagJet01, LepPlusBtagJet10, pmet, 0.);
	}
	else
		MT2llbb=0;
	return MT2llbb;
}

float getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, float mass){
	double pa[3]; double pb[3]; double pmiss[3];
	pmiss[0] = 0.; // irrelevant
	pmiss[1] = pmet.Px(); pmiss[2] = pmet.Py();
	pa[0] = 0.; pa[1] = plep1.Px(); pa[2] = plep1.Py();
	pb[0] = 0.; pb[1] = plep2.Px(); pb[2] = plep2.Py();
	mt2::mt2 MT2bisect;
  MT2bisect.set_momenta(pa, pb, pmiss);
	MT2bisect.set_mn(mass); // testmass
	float MT2 = MT2bisect.get_mt2();
	return MT2;
  return 1;
}

TLorentzVector getPtllb(){
	TLorentzVector pmet; pmet.SetPtEtaPhiM(met_genPt, 0., met_genPhi, 0.);
	TLorentzVector plep1, plep2;
	plep1.SetPtEtaPhiM(genLep_pt[0], genLep_eta[0], genLep_phi[0], genLep_mass[0]);
	plep2.SetPtEtaPhiM(genLep_pt[1], genLep_eta[1], genLep_phi[1], genLep_mass[1]);
	return plep1 + plep2 + pmet;
}

float getDPhibMet(){
	TLorentzVector pmet; pmet.SetPtEtaPhiM(met_genPt, 0., met_genPhi, 0.);
	TLorentzVector Ptllb = getPtllb();
	return pmet.DeltaPhi(Ptllb);
}


