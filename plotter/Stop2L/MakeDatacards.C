R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/Baseline/";
TString NameOfTree = "tree";
TString BaselineCut = "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";

void MakeDatacards(){
  DrawPlot("TChannel", BaselineCut, "ElMu", 1, 0, 2, "Channel", "CutAndCount");
//  DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
//  DrawPlot("TMT2", "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3 && TMT2 < 140", "ElMu", 40, 0, 200, "M_{T2} [GeV]", "TMT2low"); //  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
 // DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]");
//  DrawPlot("TDeltaEta", BaselineCut, "ElMu", 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]");
  //DrawPlot("TChannel", BaselineCut, "All", 3, 1, 4, "Channel", "Comb3chan");
  //DrawPlot("TNJets", BaselineCut, "ElMu", 1, 0, 10, "CutAndCount","CutAndCount");
  //DrawPlot("TMT2", BaselineCut, "ElMu", 40, 0, 200, "M_{T2} [GeV]");
  //DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", "ElMu", 40, 0, 400, "MET [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathSignal(pathToTree + "T2tt_noSkim/");
  p->verbose = true;
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
	p->AddSample("TW", "tW", itBkg, kMagenta, 0.3);
	p->AddSample("TbarW", "tW", itBkg);
	p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
//	p->AddSample("DoubleEG", "Data", itData);
//	p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown"); 
//  p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp"); 
//  p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown"); 
//  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp"); 
//  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "nloDown"); 
//  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp"); 
//  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "hadDown"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "ScaleUp"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "ScaleDown"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "pdfUp"); 
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "pdfDown"); 
  p->AddSample("T2tt_200_50_FS_summer", "S_200_50", itSignal, kPink-1,  0.1, "0", "isrweight"); 
//  p->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kPink-1,  0.1, "0", "isrweight"); 
//  p->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kGreen+2, 0.1, "0", "isrweight"); 
/*  p->AddSample("T2tt_mStop175_mLsp1", "S_175_1", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop183_mLsp1", "S_183_1", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop192_mLsp25", "S_192_25", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop200_mLsp25", "S_200_25", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop208_mLsp25", "S_208_25", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop217_mLsp50", "S_217_50", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop225_mLsp50", "S_225_50", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop242_mLsp75", "S_242_75", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_mStop250_mLsp75", "S_250_75", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan, 0.1); 
  p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink, 0.1); 
  p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1, 0.1); 
*/
  p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
 // p->AddSystematic("Btag,MisTag,PU");

  //p->PrintSystYields();
  p->PrintYields("","","","tex,html,txt");
  p->MakeAllDatacards();
  delete p;
}

