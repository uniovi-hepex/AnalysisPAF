R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

const TString Signalmc[]      = {"TTHNonbb"};                   // ttH
const TString TTWmc[] 	      = {"TTWToLNu", "TTWToQQ"};			  // TTW
const TString TTZmc[] 	      = {"TTZToLLNuNu", "TTZToQQ"};	    // TTZ
const TString TTbarmc[] 	    = {"TTGJets", "TTJets_aMCatNLO"}; // TTbar
const TString WJetsmc[]       = {"WJetsToLNu_MLM"};             // WJets
const TString STmc[]    	    = {"TW", "TbarW", "T_tch", "Tbar_tch", "TToLeptons_sch_amcatnlo","TGJets"}; // ST
const TString DYmc[]          = {"DYJetsToLL_M50_aMCatNLO", "DYJetsToLL_M10to50_aMCatNLO"};               // DY
const TString DiTriCuatrimc[] = {"WGToLNuG", "ZGTo2LG", "WpWpJJ", "WWW", "WWZ", "WZZ", "ZZZ", "WW", "tZq_ll", "TTTT", "WZTo3LNu", "WWTo2L2Nu", "ZZ"}; // Di&Tri&Cuatriboson
const TString Data[]          = {"MuonEG", "SingleMuon", "SingleElec", "DoubleEG", "DoubleMuon"}; // Data samples

const TString path            = "/nfs/fanae/user/vrbouza/Documents/TFG/AnalysisPAF/";
const TString outputpath      = "/nfs/fanae/user/vrbouza/www/Results/";


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");

void DrawPlots(TString chan = "ElMu"){
  TString cut = "";
  if (chan == "2lSS") {
    chan  = "";
    cut   = "((TChannel == 1) || (TChannel == 2) || (TChannel == 3))";
  }

  DrawPlot("TEvent",          cut, chan, 1, 1, 2,     " ", "Events");
  DrawPlot("nTightLepton",    cut, chan, 6, 0, 6,     "nTightLep (#)", "nTightLepton");
  DrawPlot("nFakeableLepton", cut, chan, 5, 0, 5,     "nFakeLep (#)", "nFakeLepton");
  DrawPlot("nLooseLepton",    cut, chan, 5, 0, 5,     "nLooseLep (#)", "nLooseLepton");
  DrawPlot("nTaus",           cut, chan, 3, 0, 3,     "nTaus (#)", "nTaus");
  DrawPlot("nJets",           cut, chan, 10, 0, 10,   "nJets (#)", "nJets");
  DrawPlot("nMediumBTags",    cut, chan, 6, 0, 6,     "nMediumBTags (#)", "nMediumBTags");
  DrawPlot("nLooseBTags",     cut, chan, 6, 0, 6,     "nLooseBTags (#)", "nLooseBTags");
  DrawPlot("TPtLeading",      cut, chan, 10, 0, 200,  "Pt (GeV)", "PtLeading");
  DrawPlot("TPtSubLeading",   cut, chan, 10, 0, 200,  "Pt (GeV)", "PtSubLeading");
  DrawPlot("TPtSubSubLeading",cut, chan, 10, 0, 200,  "Pt (GeV)", "PtSubSubLeading");
  DrawPlot("MET",             cut, chan, 10, 0, 400,  "MET (GeV)", "MET");
  DrawPlot("MHT",             cut, chan, 10, 0, 1000, "MHT (GeV)", "MHT");
  DrawPlot("TMETLD",          cut, chan, 10, 0, 2,    "METLD (GeV)", "METLD");
  DrawPlot("TCS",             cut, chan, 7, -3.5, 3.5,"Sum of charges", "CS");
  DrawPlot("TMass",           cut, chan, 10, 0, 400,  "Invariant mass (GeV)", "Mass");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(path+"ttH_temp/"); p->SetTreeName("MiniTree");
  p->SetPathSignal(path+"ttH_temp/");
  p->verbose = false;
  p->SetVarName(name);
  p->SetPlotFolder(outputpath);

  for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
	  p->AddSample(Signalmc[isample], "ttH", itSignal, kRed);
  }
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

  cout<<"wololo"<<endl;
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  //p->doSetLogy = true;
  //p->DrawStack("0_log", 1);
  p->PrintYields();
  delete p;
}
