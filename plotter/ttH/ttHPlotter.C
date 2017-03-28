#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TMath.h"
#include "TMatrix.h"

using namespace std;

//------------------------------------------------------------------------------
void ttHPlotter() {
	const Float_t lumi			  =	35870;
	const UInt_t nmcSamples 	= 29;
	const UInt_t ndataSamples = 5;
	enum gCategories {
	    categories_begin,
	    twolSS = categories_begin,
	    threel,
	    fourl,
	    Total,
	    gNCATEGORIES,
	};
	enum gChannel {
	    channels_begin,
	    MuMu = channels_begin,
	    ElEl,
	    ElMu,
	    All,
	    gNCHANNELS,
	};
	const TString gCatLabel			[gNCATEGORIES] 	= {"2lSS","3l","4l","Total"};
	const TString gChanLabel		[gNCHANNELS] 	= {"MuMu","ElEl","ElMu","All"};
	const TString mcsample			[nmcSamples] 	= {
		"TTWToLNu", "TTWToQQ",												// TTW
		"TTZToLLNuNu", "TTZToQQ",											// TTZ
		"TTGJets", "TTJets_aMCatNLO",										// TT+
		"TW", "TbarW", "T_tch", "Tbar_tch", "TToLeptons_sch_amcatnlo",	// T+
	  "WGToLNuG", "ZGTo2LG", "TGJets", "ZZZ", "WWZ", "WZZ", "WWW", "tZq_ll", "TTTT", "WZTo3LNu",// Rares
		"WW", "WpWpJJ", "WWTo2L2Nu",										// WW/WW+
		"WJetsToLNu_MLM",														// W+
		"ZZ",																// ZZ
		"DYJetsToLL_M50_aMCatNLO", "DYJetsToLL_M10to50_aMCatNLO", 			// DY
	  "TTHNonbb"																// Signal
	};
	Int_t mcsampleColors	[nmcSamples] 	= {
		kGreen-5, kGreen-5,
		kSpring+2, kSpring+2,
		kSpring+10, kSpring+10,
		kViolet-6, kViolet-6, kViolet-6, kViolet-6, kViolet-6,
	  kAzure-9, kAzure-9, kAzure-9,  kAzure-9, kAzure-9, kAzure-9, kAzure-9, kAzure-9, kAzure-9, kAzure-9,
		kBlue, kBlue, kBlue,
	  kCyan+1,
		kYellow,
		kOrange, kOrange,
		kRed
	};
	const TString datasample		[ndataSamples] 	= {
		"MuonEG", "DoubleMuon", "DoubleEG", "SingleMuon", "SingleElec"			// Data samples
	};
	TString codepath 	  = 	"/nfs/fanae/user/vrbouza/Documents/TFG/AnalysisPAF";
	TString outputpath 	= 	"/nfs/fanae/user/vrbouza/www/Results";
	//TString filename	=	"Results.pdf";

	// Initializing THStacks of all the histograms.
	//------------------------------------------------------------------------------
	THStack*   	fHSEvents    		    [gNCATEGORIES][gNCHANNELS]; // Events
	THStack*   	fHSTightLep			    [gNCATEGORIES][gNCHANNELS]; // Yields
	THStack*   	fHSFakeLep			    [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSLooseLep		    	[gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSTau				      [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSJet				      [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSMedBTagJet		    [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSLosBTagJet		    [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSPtLeading		    [gNCATEGORIES][gNCHANNELS]; // Kinematic
	THStack*   	fHSPtSubLeading		  [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSPtSubSubLeading	[gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSMET				      [gNCATEGORIES][gNCHANNELS]; // MET
	THStack*   	fHSMHT				      [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSMETLD			      [gNCATEGORIES][gNCHANNELS];
	THStack*   	fHSChargeSum		    [gNCATEGORIES][gNCHANNELS]; // Misc
	THStack*   	fHSMass				      [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDEvents    		      [gNCATEGORIES][gNCHANNELS]; // Events
	TH1F*   	fHDTightLep			      [gNCATEGORIES][gNCHANNELS]; // Yields
	TH1F*   	fHDFakeLep			      [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDLooseLep			      [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDTau				        [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDJet				        [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDMedBTagJet		      [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDLosBTagJet		      [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDPtLeading	        [gNCATEGORIES][gNCHANNELS]; // Kinematic
	TH1F*   	fHDPtSubLeading		    [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDPtSubSubLeading	  [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDMET				        [gNCATEGORIES][gNCHANNELS]; // MET
	TH1F*   	fHDMHT				        [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDMETLD			        [gNCATEGORIES][gNCHANNELS];
	TH1F*   	fHDChargeSum		      [gNCATEGORIES][gNCHANNELS]; // Misc
	TH1F*   	fHDMass				        [gNCATEGORIES][gNCHANNELS];

	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			fHSEvents    		[icat][ichan]	=	new THStack("HS_Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NEvents_"+gCatLabel[icat]+"_"+gChanLabel[ichan]); // Events
			fHSTightLep			[icat][ichan]	=	new THStack("HS_TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NTightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]); // Yields
			fHSFakeLep			[icat][ichan]	=	new THStack("HS_FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NFakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSLooseLep			[icat][ichan]	=	new THStack("HS_LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NLooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSTau				[icat][ichan]	=	new THStack("HS_Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NTau_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSJet				[icat][ichan]	=	new THStack("HS_Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSMedBTagJet		[icat][ichan]	=	new THStack("HS_MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NMedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSLosBTagJet		[icat][ichan]	=	new THStack("HS_LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NLosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSPtLeading		[icat][ichan]	=	new THStack("HS_PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]); // Kinematic
			fHSPtSubLeading		[icat][ichan]	=	new THStack("HS_PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSPtSubSubLeading	[icat][ichan]	=	new THStack("HS_PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSMET				[icat][ichan]	=	new THStack("HS_MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan]); // MET
			fHSMHT				[icat][ichan]	=	new THStack("HS_MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHSMETLD			[icat][ichan]	=	new THStack("HS_METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHDEvents    		[icat][ichan]	=	new TH1F("HS_Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NEvents_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 1, 0, 1); // Events
			fHDTightLep			[icat][ichan]	=	new TH1F("HS_TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NTightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 6, 0, 6); // Yields
			fHDFakeLep			[icat][ichan]	=	new TH1F("HS_FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NFakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 5, 0, 5);
			fHDLooseLep			[icat][ichan]	=	new TH1F("HS_LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NLooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 5, 0, 5);
			fHDTau				[icat][ichan]	=	new TH1F("HS_Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NTau_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 3, 0, 3);
			fHDJet				[icat][ichan]	=	new TH1F("HS_Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 10);
			fHDMedBTagJet		[icat][ichan]	=	new TH1F("HS_MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NMedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 6, 0, 6);
			fHDLosBTagJet		[icat][ichan]	=	new TH1F("HS_LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "NLosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 6, 0, 6);
			fHDPtLeading		[icat][ichan]	=	new TH1F("HS_PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 200); // Kinematic
			fHDPtSubLeading		[icat][ichan]	=	new TH1F("HS_PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 200);
			fHDPtSubSubLeading	[icat][ichan]	=	new TH1F("HS_PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 200);
			fHDMET				[icat][ichan]	=	new TH1F("HS_MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 400); // MET
			fHDMHT				[icat][ichan]	=	new TH1F("HS_MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 1000);
			fHDMETLD			[icat][ichan]	=	new TH1F("HS_METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 2);
			if (icat == twolSS 		&& ichan != All) 	continue;
			fHSChargeSum		[icat][ichan]	=	new THStack("HS_ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan]); // Misc
			fHSMass				[icat][ichan]	=	new THStack("HS_Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan]);
			fHDChargeSum		[icat][ichan]	=	new TH1F("HS_ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 7, -3.5, 3.5); // Misc
			fHDMass				[icat][ichan]	=	new TH1F("HS_Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan], "Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan], 10, 0, 400);
		}
	}

	TH1F* histEvents;
	TH1F* histTightLep;
	TH1F* histFakeLep;
	TH1F* histLooseLep;
	TH1F* histTau;
	TH1F* histJet;
	TH1F* histMedBTagJet;
	TH1F* histLosBTagJet;
	TH1F* histPtLeading;
	TH1F* histPtSubLeading;
	TH1F* histPtSubSubLeading;
	TH1F* histMET;
	TH1F* histMHT;
	TH1F* histMETLD;
	TH1F* histChargeSum;
	TH1F* histMass;
	TH1F* histDEvents;
	TH1F* histDTightLep;
	TH1F* histDFakeLep;
	TH1F* histDLooseLep;
	TH1F* histDTau;
	TH1F* histDJet;
	TH1F* histDMedBTagJet;
	TH1F* histDLosBTagJet;
	TH1F* histDPtLeading;
	TH1F* histDPtSubLeading;
	TH1F* histDPtSubSubLeading;
	TH1F* histDMET;
	TH1F* histDMHT;
	TH1F* histDMETLD;
	TH1F* histDChargeSum;
	TH1F* histDMass;

	//cout << "WOLOLOOOOO" << endl; // HASTA AQUÍ BIEN

	for (UInt_t isample = 0; isample < nmcSamples; isample++) {
		TFile* f = TFile::Open(codepath + "/ttH_temp/" + "Tree_" + mcsample[isample] + ".root");
		for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
			for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
				if (icat == threel 	&& ichan != All) 	continue;
			  if (icat == fourl 	&& ichan != All) 	continue;
				if (icat == Total 	&& ichan != All) 	continue;
				f	->	GetObject("H_Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histEvents); // Events
				f	->	GetObject("H_TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histTightLep); // Yields
				f	->	GetObject("H_FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histFakeLep);
				f	->	GetObject("H_LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histLooseLep);
				f	->	GetObject("H_Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histTau);
				f	->	GetObject("H_Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histJet);
				f	->	GetObject("H_MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histMedBTagJet);
				f	->	GetObject("H_LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histLosBTagJet);
				f	->	GetObject("H_PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histPtLeading); // Kinematic
				f	->	GetObject("H_PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histPtSubLeading);
				f	->	GetObject("H_PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histPtSubSubLeading);
				f	->	GetObject("H_MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histMET); // MET
				f	->	GetObject("H_MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histMHT);
				f	->	GetObject("H_METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histMETLD);

				if (!(icat == twolSS 		&& ichan != All)) {
					f	->	GetObject("H_ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histChargeSum); // Misc
					f	->	GetObject("H_Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histMass);
				}

              	histEvents			->	SetFillColor(mcsampleColors[isample]);
              	histTightLep		->	SetFillColor(mcsampleColors[isample]);
              	histFakeLep			->	SetFillColor(mcsampleColors[isample]);
              	histLooseLep		->	SetFillColor(mcsampleColors[isample]);
              	histTau				->	SetFillColor(mcsampleColors[isample]);
              	histJet				->	SetFillColor(mcsampleColors[isample]);
              	histMedBTagJet		->	SetFillColor(mcsampleColors[isample]);
              	histLosBTagJet		->	SetFillColor(mcsampleColors[isample]);
              	histPtLeading		->	SetFillColor(mcsampleColors[isample]);
              	histPtSubLeading	->	SetFillColor(mcsampleColors[isample]);
              	histPtSubSubLeading	->	SetFillColor(mcsampleColors[isample]);
              	histMET				->	SetFillColor(mcsampleColors[isample]);
              	histMHT				->	SetFillColor(mcsampleColors[isample]);
              	histMETLD			->	SetFillColor(mcsampleColors[isample]);
				if (!(icat == twolSS 		&& ichan != All)) {
		          	histChargeSum		->	SetFillColor(mcsampleColors[isample]);
		          	histMass			->	SetFillColor(mcsampleColors[isample]);
				}

              	histEvents			->	SetTitle(mcsample[isample]);
              	histTightLep		->	SetTitle(mcsample[isample]);
              	histFakeLep			->	SetTitle(mcsample[isample]);
              	histLooseLep		->	SetTitle(mcsample[isample]);
              	histTau				->	SetTitle(mcsample[isample]);
              	histJet				->	SetTitle(mcsample[isample]);
              	histMedBTagJet		->	SetTitle(mcsample[isample]);
              	histLosBTagJet		->	SetTitle(mcsample[isample]);
              	histPtLeading		->	SetTitle(mcsample[isample]);
              	histPtSubLeading	->	SetTitle(mcsample[isample]);
              	histPtSubSubLeading	->	SetTitle(mcsample[isample]);
              	histMET				->	SetTitle(mcsample[isample]);
              	histMHT				->	SetTitle(mcsample[isample]);
              	histMETLD			->	SetTitle(mcsample[isample]);
				if (!(icat == twolSS 		&& ichan != All)) {
		          	histChargeSum		->	SetTitle(mcsample[isample]);
		          	histMass			->	SetTitle(mcsample[isample]);
				}

				histEvents			->Scale(lumi);
				histTightLep		->Scale(lumi);
				histFakeLep			->Scale(lumi);
				histLooseLep		->Scale(lumi);
				histTau				->Scale(lumi);
				histJet				->Scale(lumi);
				histMedBTagJet		->Scale(lumi);
				histLosBTagJet		->Scale(lumi);
				histPtLeading		->Scale(lumi);
				histPtSubLeading	->Scale(lumi);
				histPtSubSubLeading	->Scale(lumi);
				histMET				->Scale(lumi);
				histMHT				->Scale(lumi);
				histMETLD			->Scale(lumi);
				if (!(icat == twolSS 		&& ichan != All)) {
					histChargeSum		->Scale(lumi);
				 	histMass			->Scale(lumi);
				}

				fHSEvents    		[icat][ichan]	-> Add(histEvents); // Events
				fHSTightLep			[icat][ichan]	-> Add(histTightLep); // Yields
				fHSFakeLep			[icat][ichan]	-> Add(histFakeLep);
				fHSLooseLep			[icat][ichan]	-> Add(histLooseLep);
				fHSTau				[icat][ichan]	-> Add(histTau);
				fHSJet				[icat][ichan]	-> Add(histJet);
				fHSMedBTagJet		[icat][ichan]	-> Add(histMedBTagJet);
				fHSLosBTagJet		[icat][ichan]	-> Add(histLosBTagJet);
				fHSPtLeading		[icat][ichan]	-> Add(histPtLeading); // Kinematic
				fHSPtSubLeading		[icat][ichan]	-> Add(histPtSubLeading);
				fHSPtSubSubLeading	[icat][ichan]	-> Add(histPtSubSubLeading);
				fHSMET				[icat][ichan]	-> Add(histMET); // MET
				fHSMHT				[icat][ichan]	-> Add(histMHT);
				fHSMETLD			[icat][ichan]	-> Add(histMETLD);
				if (!(icat == twolSS 		&& ichan != All)) {
					fHSChargeSum		[icat][ichan]	-> Add(histChargeSum); // Misc
					fHSMass				[icat][ichan]	-> Add(histMass);
				}
			}
		}
	}

	//cout<< "jojojojojojo" << endl;

	for (UInt_t isample = 0; isample < ndataSamples; isample++) {
		TFile* f = TFile::Open(codepath + "/ttH_temp/" + "Tree_" + datasample[isample] + ".root");
		for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
			for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
				if (icat == threel 		&& ichan != All) 	continue;
			  if (icat == fourl 		&& ichan != All) 	continue;
				if (icat == Total 		&& ichan != All) 	continue;
				f	->	GetObject("H_Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDEvents); // Events
				f	->	GetObject("H_TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDTightLep); // Yields
				f	->	GetObject("H_FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDFakeLep);
				f	->	GetObject("H_LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDLooseLep);
				f	->	GetObject("H_Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDTau);
				f	->	GetObject("H_Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDJet);
				f	->	GetObject("H_MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDMedBTagJet);
				f	->	GetObject("H_LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDLosBTagJet);
				f	->	GetObject("H_PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDPtLeading); // Kinematic
				f	->	GetObject("H_PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDPtSubLeading);
				f	->	GetObject("H_PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDPtSubSubLeading);
				f	->	GetObject("H_MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDMET); // MET
				f	->	GetObject("H_MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDMHT);
				f	->	GetObject("H_METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDMETLD);

				if (!(icat == twolSS 		&& ichan != All)) {
					f	->	GetObject("H_ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDChargeSum); // Misc
					f	->	GetObject("H_Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan],histDMass);
				}

				//cout<< histDEvents->Integral()<<endl;

				fHDEvents    		[icat][ichan]	-> Add(histDEvents); // Events
				fHDTightLep			[icat][ichan]	-> Add(histDTightLep); // Yields
				fHDFakeLep			[icat][ichan]	-> Add(histDFakeLep);
				fHDLooseLep			[icat][ichan]	-> Add(histDLooseLep);
				fHDTau				[icat][ichan]	-> Add(histDTau);
				fHDJet				[icat][ichan]	-> Add(histDJet);
				fHDMedBTagJet		[icat][ichan]	-> Add(histDMedBTagJet);
				fHDLosBTagJet		[icat][ichan]	-> Add(histDLosBTagJet);
				fHDPtLeading		[icat][ichan]	-> Add(histDPtLeading); // Kinematic
				fHDPtSubLeading		[icat][ichan]	-> Add(histDPtSubLeading);
				fHDPtSubSubLeading	[icat][ichan]	-> Add(histDPtSubSubLeading);
				fHDMET				[icat][ichan]	-> Add(histDMET); // MET
				fHDMHT				[icat][ichan]	-> Add(histDMHT);
				fHDMETLD			[icat][ichan]	-> Add(histDMETLD);
				if (!(icat == twolSS 		&& ichan != All)) {
					fHDChargeSum		[icat][ichan]	-> Add(histDChargeSum); // Misc
					fHDMass				[icat][ichan]	-> Add(histDMass);
				}
			}
		}
	}

	gROOT->Reset();
	//gROOT->SetStyle("Plain");

	TStyle *MyStyle = new TStyle("MyStyle","My Root Styles");
	MyStyle->SetStatColor(0);
	MyStyle->SetCanvasColor(0);
	MyStyle->SetPadColor(0);
	MyStyle->SetPadBorderMode(0);
	MyStyle->SetCanvasBorderMode(0);
	MyStyle->SetFrameBorderMode(0);
	MyStyle->SetOptStat(0);
	MyStyle->SetStatBorderSize(2);
	MyStyle->SetOptTitle(1);
	MyStyle->SetPadTickX(1);
	MyStyle->SetPadTickY(1);
	MyStyle->SetPadBorderSize(2);
	MyStyle->SetPalette(51, 0);
	MyStyle->SetPadBottomMargin(0.15);
	MyStyle->SetPadTopMargin(0.11);
	MyStyle->SetPadLeftMargin(0.15);
	MyStyle->SetPadRightMargin(0.25);
	MyStyle->SetTitleColor(1);
	MyStyle->SetTitleFillColor(0);
	MyStyle->SetTitleFontSize(0.05);
	MyStyle->SetTitleBorderSize(0);
	MyStyle->SetLineWidth(1);
	MyStyle->SetHistLineWidth(1);
	MyStyle->SetLegendBorderSize(0);
	MyStyle->SetNdivisions(502, "x");
	MyStyle->SetMarkerSize(0.3);
	MyStyle->SetTickLength(0.03);
	MyStyle->SetTitleOffset(1.5, "x");
	MyStyle->SetTitleOffset(1.5, "y");
	MyStyle->SetTitleOffset(1.0, "z");
	MyStyle->SetLabelSize(0.05, "x");
	MyStyle->SetLabelSize(0.05, "y");
	MyStyle->SetLabelSize(0.05, "z");
	MyStyle->SetLabelOffset(0.03, "x");
	MyStyle->SetLabelOffset(0.03, "y");
	MyStyle->SetLabelOffset(0.03, "z");
	MyStyle->SetTitleSize(0.05, "x");
	MyStyle->SetTitleSize(0.05, "y");
	MyStyle->SetTitleSize(0.05, "z");
	MyStyle->SetHistLineColor(kBlack);
	gROOT->SetStyle("MyStyle");

	// Drawing
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDEvents    		[icat][ichan]	-> 	SetMinimum(0);
			fHDEvents    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDEvents    		[icat][ichan]	-> 	Draw("pe");
			fHSEvents    		[icat][ichan]	-> 	Draw("histsame"); // Events
			fHDEvents    		[icat][ichan]	-> 	Draw("pesame");

			TList* EventsList;
			EventsList = fHSEvents    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = EventsList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDEvents[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"Events_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDTightLep    		[icat][ichan]	-> SetMinimum(0);
			fHDTightLep    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDTightLep    		[icat][ichan]	-> Draw("pe");
			fHSTightLep			[icat][ichan]	-> Draw("histsame");
			fHDTightLep    		[icat][ichan]	-> Draw("pesame");

			TList* TightLepList;
			TightLepList = fHSTightLep    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = TightLepList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDTightLep[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"TightLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDFakeLep    		[icat][ichan]	-> SetMinimum(0);
			fHDFakeLep    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDFakeLep    		[icat][ichan]	-> 	Draw("pe");
			fHSFakeLep			[icat][ichan]	-> Draw("histsame");
			fHDFakeLep    		[icat][ichan]	-> 	Draw("pesame");

			TList* FakeLepList;
			FakeLepList = fHSFakeLep    		[icat][ichan]	-> 	GetHists(); // Events


			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = FakeLepList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDFakeLep[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"FakeLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDLooseLep    		[icat][ichan]	-> SetMinimum(0);
			fHDLooseLep    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDLooseLep    		[icat][ichan]	-> 	Draw("pe");
			fHSLooseLep			[icat][ichan]	-> Draw("histsame");
			fHDLooseLep    		[icat][ichan]	-> 	Draw("pesame");

			TList* LooseLepList;
			LooseLepList = fHSLooseLep    		[icat][ichan]	-> 	GetHists(); // Events


			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = LooseLepList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDLooseLep[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"LooseLep_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDTau    		[icat][ichan]	-> SetMinimum(0);
			fHDTau    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDTau    		[icat][ichan]	-> 	Draw("pe");
			fHSTau			[icat][ichan]	-> Draw("histsame");
			fHDTau    		[icat][ichan]	-> 	Draw("pesame");

			TList* TauList;
			TauList = fHSTau    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = TauList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDTau[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"Tau_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDJet    		[icat][ichan]	-> SetMinimum(0);
			fHDJet    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDJet    		[icat][ichan]	-> 	Draw("pe");
			fHSJet				[icat][ichan]	-> Draw("histsame");
			fHDJet    		[icat][ichan]	-> 	Draw("pesame");

			TList* JetList;
			JetList = fHSJet    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = JetList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDJet[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"Jet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDMedBTagJet    		[icat][ichan]	-> SetMinimum(0);
			fHDMedBTagJet    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDMedBTagJet    	[icat][ichan]	-> 	Draw("pe");
			fHSMedBTagJet		[icat][ichan]	-> Draw("histsame");
			fHDMedBTagJet    	[icat][ichan]	-> 	Draw("pesame");

			TList* MedBTagJetList;
			MedBTagJetList = fHSMedBTagJet    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = MedBTagJetList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDMedBTagJet[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"MedBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDLosBTagJet    	[icat][ichan]	-> SetMinimum(0);
			fHDLosBTagJet    	[icat][ichan]	->	SetMarkerStyle(20);
			fHDLosBTagJet    	[icat][ichan]	-> 	Draw("pe");
			fHSLosBTagJet		[icat][ichan]	-> Draw("histsame");
			fHDLosBTagJet    	[icat][ichan]	-> 	Draw("pesame");

			TList* LosBTagJetList;
			LosBTagJetList = fHSLosBTagJet    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = LosBTagJetList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDLosBTagJet[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"LosBTagJet_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDPtLeading    		[icat][ichan]	-> SetMinimum(0);
			fHDPtLeading    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDPtLeading    	[icat][ichan]	-> 	Draw("pe");
			fHSPtLeading		[icat][ichan]	-> Draw("histsame");
			fHDPtLeading    	[icat][ichan]	-> 	Draw("pesame");

			TList* PtLeadingList;
			PtLeadingList = fHSPtLeading    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = PtLeadingList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDPtLeading[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"PtLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDPtSubLeading    		[icat][ichan]	-> SetMinimum(0);
			fHDPtSubLeading    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDPtSubLeading    	[icat][ichan]	-> 	Draw("pe");
			fHSPtSubLeading		[icat][ichan]	-> Draw("histsame");
			fHDPtSubLeading    	[icat][ichan]	-> 	Draw("pesame");

			TList* PtSubLeadingList;
			PtSubLeadingList = fHSPtSubLeading    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = PtSubLeadingList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDPtSubLeading[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"PtSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDPtSubSubLeading    		[icat][ichan]	-> SetMinimum(0);
			fHDPtSubSubLeading    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDPtSubSubLeading    	[icat][ichan]	-> 	Draw("pe");
			fHSPtSubSubLeading	[icat][ichan]	-> Draw("histsame");
			fHDPtSubSubLeading    	[icat][ichan]	-> 	Draw("pesame");

			TList* PtSubSubLeadingList;
			PtSubSubLeadingList = fHSPtSubSubLeading    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = PtSubSubLeadingList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDPtSubSubLeading[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"PtSubSubLeading_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDMET    		[icat][ichan]	-> SetMinimum(0);
			fHDMET    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDMET    			[icat][ichan]	-> 	Draw("pe");
			fHSMET				[icat][ichan]	-> Draw("histsame");
			fHDMET    			[icat][ichan]	-> 	Draw("pesame");

			TList* METList;
			METList = fHSMET    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = METList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDMET[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"MET_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDMHT    			[icat][ichan]	-> 	SetMinimum(0);
			fHDMHT    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDMHT		    	[icat][ichan]	-> 	Draw("pe");
			fHSMHT				[icat][ichan]	-> 	Draw("histsame");
			fHDMHT		    	[icat][ichan]	-> 	Draw("pesame");

			TList* MHTList;
			MHTList = fHSMHT    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = MHTList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDMHT[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"MHT_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDMETLD    		[icat][ichan]	-> SetMinimum(0);
			fHDMETLD    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDMETLD	    	[icat][ichan]	-> 	Draw("pe");
			fHSMETLD			[icat][ichan]	-> Draw("histsame");
			fHDMETLD	    	[icat][ichan]	-> 	Draw("pesame");

			TList* METLDList;
			METLDList = fHSMETLD    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = METLDList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDMETLD[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"METLD_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			if (icat == twolSS 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDChargeSum    		[icat][ichan]	-> SetMinimum(0);
			fHDChargeSum    		[icat][ichan]	->	SetMarkerStyle(20);
			fHDChargeSum    	[icat][ichan]	-> 	Draw("pe");
			fHSChargeSum		[icat][ichan]	-> Draw("histsame");
			fHDChargeSum    	[icat][ichan]	-> 	Draw("pesame");

			TList* ChargeSumList ;
			ChargeSumList  = fHSChargeSum     		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = ChargeSumList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDChargeSum[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"ChargeSum_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
	for (UInt_t icat = 0; icat < gNCATEGORIES; icat++) {
		for (UInt_t ichan = 0; ichan < gNCHANNELS; ichan++) {
			if (icat == threel 		&& ichan != All) 	continue;
			if (icat == fourl 		&& ichan != All) 	continue;
			if (icat == Total 		&& ichan != All) 	continue;
			if (icat == twolSS 		&& ichan != All) 	continue;
			TCanvas *c = new TCanvas("c", "c", 800, 600);
			//if (icat != threel) c -> SetLogy();
			fHDMass    		[icat][ichan]	-> SetMinimum(0);
			fHDMass    			[icat][ichan]	->	SetMarkerStyle(20);
			fHDMass		    	[icat][ichan]	-> 	Draw("pe");
			fHSMass				[icat][ichan]	-> Draw("histsame");
			fHDMass		    	[icat][ichan]	-> 	Draw("pesame");

			TList* MassList;
			MassList = fHSMass    		[icat][ichan]	-> 	GetHists(); // Events

			TLegend *l = new TLegend(0.76, 0.4, 1.0, 0.8);
			TObjLink *lnk = MassList->FirstLink();
			while (lnk) {
				l->AddEntry(lnk->GetObject());
				lnk = lnk->Next();
			}
			l->AddEntry(fHDMass[icat][ichan]);
			l->Draw("same");

			c->Print(outputpath+"/"+"Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".pdf");
			c->Print(outputpath+"/"+"Mass_"+gCatLabel[icat]+"_"+gChanLabel[ichan]+".png");
			delete c;
			delete l;
		}
	}
}
