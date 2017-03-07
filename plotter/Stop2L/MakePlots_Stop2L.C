R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Plot.h"

TString NBtagsNJets = "TNBtags + (TNJets == 1)*1 + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";
// Predefined cut
TString SRcut       = "TMET > 50 && TNJets > 1 && TNBtags > 0 && TNVetoLeps == 2 && !TIsSS";
TString SScut       = "TMET > 50 && TNJets > 1 && TNBtags > 0 && TNVetoLeps == 2 && TIsSS";
TString ExtraLep    = "TMET > 50 && TNJets > 1 && TNBtags > 0 && TNVetoLeps > 2";
//TString path = "/nfs/fanae/user/juanr/stop/TOP13TeV/temp/"; 
//TString path = "/mnt_pool/fanae105/user/juanr/stop/TOP13TeV/temp/";
TString path = "/nfs/fanae/user/juanr/AnalyserPAF/StopTrees/Baseline/";
void Draw(TString var = "TMET", TString cuts = "TNJets > 1 && TNBtags > 0", TString chan = "ElMu", Int_t nbins = 30, Double_t x0 = 0., Double_t xN = 300., TString xlabel = "GeV", TString tag = "0");

void MakePlots_Stop2L(){
  Draw("TMT2", SRcut, "ElMu", 30, 0, 300, "MET [GeV]", "MT2");
  //Draw("TMT2", "TMET > 50 && TNJets > 1 && TNBtags > 0 && TNVetoLeps == 2 && !TIsSS", "ElMu", 30,  0, 300, "MT2 [GeV]", "0");
  //Draw(NBtagsNJets, "1", "ElMu", 15 , -0.5, 14.5, "(NJets, NBtags)", "0");
  //Draw("TMll", "1", "Muon", 80 , 91-20, 91+20, "M_{#ell#ell} [GeV]", "0");
  //Draw("TNBtags + TNJets", "1", "ElMu", 15 , -0.5, 14.5, "(NJets, NBtags)", "0");
}

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Double_t x0, Double_t xN, TString xlabel, TString name){
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
	//p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kGreen-3, 0.50);
	p->SetPath(path);
  
  if      (var  == NBtagsNJets) p->SetVarName("NBtagsNJets");
  else if (var  == "1")         p->SetVarName("CutAndCount");
  else if (name == "0")         p->SetVarName("var");
  else                          p->SetVarName(name);



  //p->SetOutputName("NBtagNJets");
	p->SetPathSignal(path + "T2tt/");
  p->verbose = false;
	p->SetTreeName("tree");
	//p->SetTreeName("sTopTree");
	p->AddSample("WZ", "VV", itBkg, kYellow-10, 0.50);
	p->AddSample("WW", "VV", itBkg);
	p->AddSample("ZZ", "VV", itBkg);
//  p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray, 0.5);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	//p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kAzure-8, 0.50);
	//p->AddSample("DYJetsToLL_M5to50_MLM",     "DY", itBkg);
	p->AddSample("DYJetsToLL_M50_aMCatNLO", "DY", itBkg, kAzure-8, 0.50);
	p->AddSample("DYJetsToLL_M10to50_aMCatNLO",     "DY", itBkg);
	p->AddSample("TW", "TW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "TW", itBkg);
	p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("T2tt_mStop175_mLsp1" , "T2tt-175-1" , itSignal, kGreen+0, 0.10);
  p->AddSample("T2tt_mStop250_mLsp75", "T2tt-250-75", itSignal, kGreen+4, 0.10);
  //p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp"); 
  //p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown"); 
  //p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp"); 
  //p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown"); 
  //p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp"); 
  //p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown"); 
  //p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nlo"); 

  p->AddSystematicLabel("ue");
  p->AddSystematicLabel("fsr");
  p->AddSystematicLabel("isr");
  p->AddSystematicLabel("nlo");
  p->AddSystematic("JES");
  p->AddSystematic("LepEff");
  p->AddSystematic("Btag");
  p->AddSystematic("MisTag");
  
  p->PrintYields();
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  p->doSetLogy = true;
  p->DrawStack("0_log", 1);
  //p->PrintSystYields();
  p->MakeAllDatacards();
  delete p;
}
