R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

const TString Signalmc[1]       = {"TTHNonbb"};                   // ttH
const TString TTWmc[2] 	        = {"TTWToLNu", "TTWToQQ"};			  // TTW
const TString TTZmc[2] 	        = {"TTZToLLNuNu", "TTZToQQ"};	    // TTZ
const TString TTbarmc[2] 	      = {"TTGJets", "TTJets_aMCatNLO"}; // TTbar
const TString WJetsmc[1]        = {"WJetsToLNu_MLM"};             // WJets
const TString STmc[6]    	      = {"TW", "TbarW", "T_tch", "Tbar_tch", "TToLeptons_sch_amcatnlo","TGJets"}; // ST
const TString DYmc[1]           = {"DYJetsToLL_M50_aMCatNLO", "DYJetsToLL_M10to50_aMCatNLO"};               // DY
const TString DiTriCuatrimc[12] = {"WGToLNuG", "ZGTo2LG", "WpWpJJ", "ZZZ", "WWZ", "WZZ", "WW", "tZq_ll", "TTTT", "WZTo3LNu", "WWTo2L2Nu", "ZZ"}; // Di&Tri&Cuatriboson
const TString Data[5]           = {"MuonEG", "SingleMuon", "SingleElec", "DoubleEG", "DoubleMuon"}; // Data samples

const TString path              = "../../ttH_temp";
const TString outputpath        = "/nfs/fanae/user/vrbouza/www/Results";

void DrawPlots(TString chan = "ElMu"){
  for (UInt_t icutcat = 0; icutcat < nCat; i++) {
    DrawPlot("nTightLepton",, chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
    DrawPlot("nTightLepton","", chan, 6, 0, 6, "nTightLep (#)", "nTightLepton");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(path); p->SetTreeName("minitree");
  p->SetPathSignal(path);
  p->verbose = false;
  p->SetVarName(name);

  for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); i++) {
	  p->AddSample(Signalmc[isample], "ttH", itSignal, kRed);
  }
  for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); i++) {
	  p->AddSample(TTWmc[isample], "TTW", itBkg, kGreen-5);
  }
  for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); i++) {
	  p->AddSample(TTZmc[isample], "TTZ", itBkg, kSpring+2);
  }
  for (UInt_t isample = 0; isample < sizeof(TTbarmc)/sizeof(*TTbarmc); i++) {
	  p->AddSample(TTbarmc[isample], "TTbar", itBkg, kSpring+10);
  }
  for (UInt_t isample = 0; isample < sizeof(WJetsmc)/sizeof(*WJetsmc); i++) {
	  p->AddSample(WJetsmc[isample], "WJets", itBkg, kViolet+10);
  }
  for (UInt_t isample = 0; isample < sizeof(STmc)/sizeof(*STmc); i++) {
	  p->AddSample(STmc[isample], "ST", itBkg, kYellow);
  }
  for (UInt_t isample = 0; isample < sizeof(DYmc)/sizeof(*DYmc); i++) {
	  p->AddSample(DYmc[isample], "DY", itBkg, kOrange);
  }
  for (UInt_t isample = 0; isample < sizeof(DiTriCuatrimc)/sizeof(*DiTriCuatrimc); i++) {
	  p->AddSample(DiTriCuatrimc[isample], "Di&Tri&Cuatriboson", itBkg, kAzure-9);
  }
  for (UInt_t isample = 0; isample < sizeof(Data)/sizeof(*Data); i++) {
	  p->AddSample(Data[isample], "Data", itData);
  }


  p->doSetLogy = false;
  p->DrawStack("0", 1);
  //p->doSetLogy = true;
  //p->DrawStack("0_log", 1);
  p->PrintYields();
  delete p;
}
