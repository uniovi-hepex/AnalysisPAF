R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Plot.h"

//TString path = "/nfs/fanae/user/juanr/stop/TOP13TeV/temp/"; 
TString path = "~/AnalyserPAF/temp/"; 
void Draw(TString var = "TMET", TString cuts = "TNJets > 1 && TNBtags > 0", TString chan = "ElMu", Int_t nbins = 30, Int_t x0 = 0, Int_t xN = 300, TString xlabel = "GeV", TString tag = "0");

void MakePlots_Stop2L(){
  Draw("TMET", "TNJets > 1 && TNBtags > 0", "ElMu", 40, 0, 400, "MET [GeV]", "0");
}

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag){
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
	//p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kGreen-3, 0.50);
	p->SetPath(path);
	p->AddSample("WW", "VV", itBkg, kYellow-10, 0.50);
	p->AddSample("WZ", "VV", itBkg, kYellow-10, 0.50);
	p->AddSample("ZZ", "VV", itBkg, kYellow-10, 0.50);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	p->AddSample("DYJetsToLL_M5to50_MLM", "DY", itBkg, kAzure-8, 0.50);
	p->AddSample("DYJetsToLL_M50_MLM",     "DY", itBkg);
	p->AddSample("TW", "TW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "TW", itBkg);
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
//	p->AddSample("DoubleEG", "Data", itData);
//	p->AddSample("DoubleMuon", "Data", itData);

  //for(int i = 0; i < (int) p->VBkgs.size(); i++){
  //  p->VBkgs.at(i)->Draw("same");
  //}
  p->doSetLogy = false;
  p->DrawStack(tag, 1);
  p->doSetLogy = true;
  p->DrawStack(tag + "_log", 1);
  delete p;
}
