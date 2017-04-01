R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

const TString Signalmc[1]      = {"TTHNonbb"};                   // ttH
const TString TTWmc[2] 	       = {"TTWToLNu1", "TTWToQQ"};			 // TTW
const TString TTZmc[2] 	       = {"TTZToLLNuNu", "TTZToQQ"};	   // TTZ
const TString TTbarmc[2] 	     = {"TTGJets", "TTJets_aMCatNLO"}; // TTbar
const TString WJetsmc[1]       = {"WJetsToLNu_aMCatNLO"};        // WJets
const TString STmc[6]    	     = {"TW", "TbarW", "T_tch", "Tbar_tch", "TToLeptons_sch_amcatnlo","TGJets"}; // ST
const TString DYmc[2]          = {"DYJetsToLL_M50_aMCatNLO", "DYJetsToLL_M10to50_aMCatNLO"};               // DY
const TString DiTriCuatrimc[13]= {"WGToLNuG", "ZGTo2LG", "WpWpJJ", "WWW", "WWZ", "WZZ", "ZZZ", "WW", "tZq_ll", "TTTT", "WZTo3LNu_amcatnlo", "WWTo2L2Nu", "ZZ"}; // Di&Tri&Cuatriboson
const TString Data[5]          = {"MuonEG", "SingleMuon", "SingleElec", "DoubleEG", "DoubleMuon"}; // Data samples

const TString path             = "/nfs/fanae/user/vrbouza/Documents/TFG/AnalysisPAF/";
const TString outputpath       = "/nfs/fanae/user/vrbouza/www/Results/";


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "", TString tag = "0");

void DrawPlots(TString chan = "ElMu", TString tag = "0"){
  TString cut = "(TCat == 2 || TCat == 3 || TCat == 4)";
  if (chan == "2lSS" || chan == "Elec" || chan == "Muon" || chan == "ElMu") {
    cut   = "(TCat == 2)";
  }
  else if (chan == "3l") {
    cut   = "(TCat == 3)";
  }
  else if (chan == "4l") {
    cut   = "(TCat == 4)";
  }

  DrawPlot("TnTightLepton",    cut, chan, 6, 0, 6,     "nTightLep (#)", "nTightLepton", tag);
  DrawPlot("TnFakeableLepton", cut, chan, 5, 0, 5,     "nFakeLep (#)", "nFakeLepton", tag);
  DrawPlot("TnLooseLepton",    cut, chan, 5, 0, 5,     "nLooseLep (#)", "nLooseLepton", tag);
  DrawPlot("TnTaus",           cut, chan, 3, 0, 3,     "nTaus (#)", "nTaus", tag);
  DrawPlot("TnJets",           cut, chan, 10, 0, 10,   "nJets (#)", "nJets", tag);
  DrawPlot("TnMediumBTags",    cut, chan, 6, 0, 6,     "nMediumBTags (#)", "nMediumBTags", tag);
  DrawPlot("TnLooseBTags",     cut, chan, 6, 0, 6,     "nLooseBTags (#)", "nLooseBTags", tag);
  DrawPlot("TPtLeading",       cut, chan, 10, 0, 200,  "Pt (GeV)", "PtLeading", tag);
  DrawPlot("TPtSubLeading",    cut, chan, 10, 0, 200,  "Pt (GeV)", "PtSubLeading", tag);
  DrawPlot("TPtSubSubLeading", cut, chan, 10, 0, 200,  "Pt (GeV)", "PtSubSubLeading", tag);
  DrawPlot("TMET",             cut, chan, 10, 0, 400,  "MET (GeV)", "MET", tag);
  DrawPlot("TMHT",             cut, chan, 10, 0, 1000, "MHT (GeV)", "MHT", tag);
  DrawPlot("THT",              cut, chan, 10, 0, 1000, "HT (GeV)", "HT", tag);
  DrawPlot("TMETLD",           cut, chan, 10, 0, 2,    "METLD (GeV)", "METLD", tag);
  DrawPlot("TCS",              cut, chan, 7, -3.5, 3.5,"Sum of charges", "CS", tag);
  DrawPlot("TMass",            cut, chan, 10, 0, 400,  "Invariant mass (GeV)", "Mass", tag);
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString tag = "0"){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(path+"ttH_temp/"); p->SetTreeName("MiniTree");
  p->SetPathSignal(path+"ttH_temp/");
  p->verbose = true;
  p->SetVarName(name);
  p->SetPlotFolder(outputpath);
  p->doStackSignal   = true;

  /*for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
	  p->AddSample(Signalmc[isample], "ttH", itSignal, kRed);
  }*/
  
  for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
    p->AddSample(TTWmc[isample], "TTW", itBkg, kGreen-5);
  }
  for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
	  p->AddSample(TTZmc[isample], "TTZ", itBkg, kSpring+2);
  }
  for (UInt_t isample = 0; isample < sizeof(TTbarmc)/sizeof(*TTbarmc); isample++) {
	  p->AddSample(TTbarmc[isample], "TTbar", itBkg, kSpring+10);
  }
  for (UInt_t isample = 0; isample < sizeof(WJetsmc)/sizeof(*WJetsmc); isample++) {
	  p->AddSample(WJetsmc[isample], "WJets", itBkg, kViolet+10);
  }
  for (UInt_t isample = 0; isample < sizeof(STmc)/sizeof(*STmc); isample++) {
	  p->AddSample(STmc[isample], "ST", itBkg, kYellow);
  }
  for (UInt_t isample = 0; isample < sizeof(DYmc)/sizeof(*DYmc); isample++) {
	  p->AddSample(DYmc[isample], "DY", itBkg, kOrange);
  }
  for (UInt_t isample = 0; isample < sizeof(DiTriCuatrimc)/sizeof(*DiTriCuatrimc); isample++) {
	  p->AddSample(DiTriCuatrimc[isample], "Di&Tri&Cuatriboson", itBkg, kAzure-9);
  }
  for (UInt_t isample = 0; isample < sizeof(Data)/sizeof(*Data); isample++) {
	  p->AddSample(Data[isample], "Data", itData);
  }

  p->AddSample(Signalmc[0], "ttH", itBkg, kRed);

  p->doSetLogy = false;
  p->DrawStack(tag, 1);
  //p->doSetLogy = true;
  //p->DrawStack("0_log", 1);
  p->PrintYields();
  delete p;
}
