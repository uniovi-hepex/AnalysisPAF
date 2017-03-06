R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Plot.h"

TString NBtagsNJets = "TNBtags + (TNJets == 1)*1 + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";
//TString path = "/nfs/fanae/user/juanr/stop/TOP13TeV/temp/"; 
//TString path = "/mnt_pool/fanae105/user/juanr/stop/TOP13TeV/temp/";
TString path = "/nfs/fanae/user/juanr/AnalyserPAF/StopTrees/";
void Draw(TString var = "TMET", TString cuts = "TNJets > 1 && TNBtags > 0", TString chan = "ElMu", Int_t nbins = 30, Double_t x0 = 0., Double_t xN = 300., TString xlabel = "GeV", TString tag = "0");

void MakePlots_Stop2L(){
  
  Draw("TMET", "TMET > 50 && TNJets > 1 && TNBtags > 0", "ElMu", 40, 50, 450, "MET [GeV]", "0");
  //Draw("TMT2ll", "TMET > 50 && TNJets > 1 && TNBtags > 0", "ElMu", 30,  0, 300, "MT2 [GeV]", "0");
  //Draw(NBtagsNJets, "1", "ElMu", 15 , -0.5, 14.5, "(NJets, NBtags)", "0");
  //Draw("TNBtags + TNJets", "1", "ElMu", 15 , -0.5, 14.5, "(NJets, NBtags)", "0");
}

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Double_t x0, Double_t xN, TString xlabel, TString tag){
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
	//p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kGreen-3, 0.50);
	p->SetPath(path);
  //p->SetOutputName("NBtagNJets");
	p->SetPathSignal(path + "Susy/");
  //p->verbose = true;
	p->SetTreeName("tree");
	//p->SetTreeName("sTopTree");
	p->AddSample("WW", "VV", itBkg, kYellow-10, 0.50);
	p->AddSample("WZ", "VV", itBkg);
	p->AddSample("ZZ", "VV", itBkg);
  p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray, 0.5);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kAzure-8, 0.50);
	p->AddSample("DYJetsToLL_M5to50_MLM",     "DY", itBkg);
	p->AddSample("TW", "TW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "TW", itBkg);
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
//	p->AddSample("DoubleEG", "Data", itData);
//	p->AddSample("DoubleMuon", "Data", itData);
  //p->AddSample("T2tt_mStop175_mLsp1" , "T2tt-175-1" , itSignal, kGreen+0, 0.10);
  //p->AddSample("T2tt_mStop250_mLsp75", "T2tt-250-75", itSignal, kGreen+4, 0.10);

 //p->AddSystematic("JES");
 //p->AddSystematic("LepEff");
 //p->AddSystematic("Btag");
 //p->AddSystematic("MisTag");
 p->PrintYields();
  //for(int i = 0; i < (int) p->VBkgs.size(); i++){
  //  p->VBkgs.at(i)->Draw("same");
  //}
  p->doSetLogy = false;
  p->DrawStack(tag, 1);
  p->doSetLogy = true;
  p->DrawStack(tag + "_log", 1);
  p->PrintSystematics();
  delete p;
}
