R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/Baseline/";
TString NameOfTree = "tree";
TString BaselineCut = "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";

void MakePlots_Stop2L(){
  DrawPlot("TNJets", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 1, 0, 20, "MET [GeV]");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
  //DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]");
  //DrawPlot("TDeltaEta", BaselineCut, "ElMu", 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]");
  //DrawPlot("TNJets", BaselineCut, "ElMu", 1, 0, 10, "CutAndCount","CutAndCount");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
  //DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;
  //p->doData = false;
  if(name != "") p->SetVarName(name);

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
	p->AddSample("TW", "tW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "tW", itBkg);
	p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown"); 
//  p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp"); 
//  p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown"); 
  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "nloDown"); 
  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "hadDown"); 
//  p->AddSample("T2tt_mStop175_mLsp1", "S-175-1", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_200_50_FS_summer", "S-200-50", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_225_50_FS_summer", "S-225-50", itSignal, kPink, 0.1); 
  p->AddSample("T2tt_250_50_FS_summer", "S-250-50", itSignal, 1, 0.1); 

  p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
//  p->AddSystematic("Btag,MisTag,PU");

  p->PrintYields();
  p->PrintSystYields();
  p->MakeAllDatacards();
  delete p;

}

