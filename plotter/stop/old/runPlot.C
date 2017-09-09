R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/";
TString NameOfTree = "tree";

void runPlot(){
  DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
  DrawPlot("TMT2", "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 30, 0, 300, "M_{T2} [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;

//p->GetHistoNormLHE("TTbar_PowhegLHE");

  p->AddSample("WZ", "VV", itBkg, kYellow-10, 0.50);
  p->AddSample("WW", "VV", itBkg);
  p->AddSample("ZZ", "VV", itBkg);
  //p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray, 0.5);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	//p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kAzure-8, 0.50);
	//p->AddSample("DYJetsToLL_M5to50_MLM",     "DY", itBkg);
	p->AddSample("DYJetsToLL_M50_aMCatNLO", "DY", itBkg, kAzure-8, 0.50);
	p->AddSample("DYJetsToLL_M10to50_aMCatNLO",     "DY", itBkg);
	p->AddSample("TW", "tW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "tW", itBkg);
	p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("T2tt_mStop175_mLsp1" , "T2tt-175-1" , itSignal, kGreen+0, 0.10);
  p->AddSample("T2tt_mStop250_mLsp75", "T2tt-250-75", itSignal, kGreen+4, 0.10);
 // p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp"); 
 // p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown"); 
 // p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp"); 
 // p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown"); 
 // p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp"); 
 // p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown"); 
  //p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nlo"); 

  p->AddSystematic("JES,Btag,MisTag");
  p->PrintYields();
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  p->doSetLogy = true;
  p->DrawStack("0_log", 1);
  p->MakeAllDatacards();
  p->PrintSystematics();
  p->PrintSystYields();
  delete p;
}


void runLooper(){
  Looper* p = new Looper(pathToTree, NameOfTree, "TMET", "TMET > 0", "ElMu", 30, 0, 300);
  Histo* h = p->GetHisto("TW");
  h->SetLineColor(kRed);
  cout << "nEntries = " << h->GetEntries() << endl;
  cout << "Yield = " << h->Integral()*35900 << endl;
  h->Draw();
}
