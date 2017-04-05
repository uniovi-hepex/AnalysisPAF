R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Dilepton/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/Baseline/";
TString NameOfTree = "tree";
TString BaselineCut = "TMET > 50 && TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";
TString SScut = "TMT2 > 0 && TMET > 50 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps < 3";
TString ThirdLep = "TMT2 > 0 && TMET > 50 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps > 2";
TString Dilepton    = "!TIsSS && TNVetoLeps < 3";
TString VarNBtagsNJets = "TNBtags + (TNJets == 1) + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";

// Baseline

TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
void DrawPlots(TString chan = "ElMu"){
  DrawPlot("TMT2", SScut,    chan, 40, 0, 200, "M_{T2} [GeV]", "MT2_SS");
//  DrawPlot("TMT2", ThirdLep, chan, 20, 0, 120, "M_{T2} [GeV]", "MT2_3Lep");
//  DrawPlot("TMT2", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", chan, 40, 0, 200, "M_{T2} [GeV]", "MT2_bl");
//  DrawPlot("TDeltaPhi", BaselineCut, "ElMu", 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi_bl");
//  DrawPlot("TDeltaEta", BaselineCut, chan, 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]", "DeltaEta_bl");
//  DrawPlot("TMET", "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3", chan, 40, 0, 400, "MET [GeV]", "MET_bl");
//  DrawPlot("TLep_Eta[0]", BaselineCut, chan, 20, -2.4, 2.4, "Leading lepton #eta [rad]", "Lep0Eta_bl");
//  DrawPlot("TLep_Eta[1]", BaselineCut, chan, 20, -2.4, 2.4, "Subleading lepton #eta [rad]", "Lep1Eta_bl");
//  DrawPlot("TLep_Pt", BaselineCut, chan, 25, 25, 250, "Leading lepton p_T [GeV]", "Lep0Pt_bl");
 //DrawPlot("TLep_Pt[1]", BaselineCut, chan, 30, 0, 200, "Subleading lepton p_T [GeV]", "Lep1Pt_bl");
//  DrawPlot("TJet_Eta[0]", BaselineCut, chan, 20, -2.4, 2.4, "Leading jet #eta [rad]", "Jet0Eta_bl");
 // DrawPlot("TJet_Eta[1]", BaselineCut, chan, 20, -2.4, 2.4, "Subleading jet #eta [rad]", "Jet1Eta_bl");
//  DrawPlot("TJet_Pt[0]", BaselineCut, chan, 27, 30, 300, "Leading jet p_T [GeV]", "Jet0Pt_bl");
 // DrawPlot("TJet_Pt[1]", BaselineCut, chan, 30, 0, 300, "Subleading jet p_T [GeV]", "Jet1Pt_bl");
 // DrawPlot("TMll", BaselineCut, chan, 40, 0, 600, "M_{e#mu} [GeV]", "InvMass_bl");
}

//Dilepton

//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Dilepton/";
//void DrawPlots(TString chan = "ElMu"){
  //DrawPlot("TMET","!TIsSS", chan, 20, 0, 200, "MET [GeV]", "TMET_AllDilep");
  //DrawPlot("TNJets","!TIsSS", chan, 9, -0.5, 8.5, "Jet Multiplicity", "NJet_Dilep");
  //DrawPlot("TMET","!TIsSS && TNVetoLeps < 3 && (TChannel == 1 || (TMll<76 || TMll > 106))", chan, 20, 0, 200, "MET [GeV]", "TMET200");
  //DrawPlot("TMET","!TIsSS && TNVetoLeps < 3 && (TChannel == 1 || (TMll<76 || TMll > 106))", chan, 40, 0, 400, "MET [GeV]", "TMET400");
  //DrawPlot("TMET","!TIsSS && TNVetoLeps < 3 && (TChannel == 1 || (TMll<76 || TMll > 106))", chan, 80, 0, 800, "MET [GeV]", "TMET800");
//  DrawPlot("TLep_Pt[0]", Dilepton, chan, 25, 0, 250, "Leading lepton p_T [GeV]", "Lep0Pt_dil");
//}
/*  DrawPlot("TMT2", Dilepton, chan, 40, 0, 200, "M_{T2} [GeV]", "MT2_dil");
  DrawPlot("TMET",Dilepton, chan, 40, 0, 800, "MET [GeV]", "TMET_dil");
  DrawPlot("TDeltaPhi", Dilepton, chan, 30, -3.15, 3.15, "#Delta#varphi_{e#mu} [rad]", "DeltaPhi_dil");
  DrawPlot("TDeltaEta", Dilepton, chan, 30, 0, 2.4, "#Delta#eta_{e#mu} [rad]", "DeltaEta_dil");
  DrawPlot("TMET",Dilepton, chan, 40, 0, 400, "MET [GeV]", "TMET_dil");
  DrawPlot("TLep_Eta[0]", Dilepton, chan, 20, -2.4, 2.4, "Leading lepton #eta [rad]", "Lep0Eta_dil");
  DrawPlot("TLep_Eta[1]", Dilepton, chan, 20, -2.4, 2.4, "Subleading lepton #eta [rad]", "Lep1Eta_dil");
  DrawPlot("TLep_Pt[0]", Dilepton, chan, 30, 0, 200, "Leading lepton p_T [GeV]", "Lep0Pt_dil");
  DrawPlot("TLep_Pt[1]", Dilepton, chan, 30, 0, 200, "Subleading lepton p_T [GeV]", "Lep0Pt_dil");
  DrawPlot("TMll", Dilepton, chan, 40, 0, 600, "M_{e#mu} [GeV]", "InvMass_dil");
  DrawPlot(VarNBtagsNJets, Dilepton, chan, 15, -0.5, 14.5, "(NJets, NBtags)", "NBtagsNJets");
}*/

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = true;
  p->SetLumi(10.0);
  //p->doData = false;
  //p->doStackSignal = true;
  if(name != "") p->SetVarName(name);
  //p->doData = false;

  p->AddSample("WZ", "VV", itBkg, kYellow-10, 0.50);
  p->AddSample("WW", "VV", itBkg);
  p->AddSample("ZZ", "VV", itBkg);
  //p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kGray, 0.5);
  //p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kGray, 0.5);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	p->AddSample("DYJetsToLL_M50_aMCatNLO", "DY", itBkg, kAzure-8, 0.50);
	p->AddSample("DYJetsToLL_M10to50_aMCatNLO",     "DY", itBkg);
	p->AddSample("TW", "tW", itBkg, kMagenta, 0.2);
	p->AddSample("TbarW", "tW", itBkg);
	p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.05);
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
//	p->AddSample("DoubleEG", "Data", itData);
//	p->AddSample("DoubleMuon", "Data", itData);
 // p->AddSample("T2tt_mStop175_mLsp1", "T2tt_175_1", itSignal, kGreen+1, 0.1); 
 // p->AddSample("T2tt_mStop250_mLsp75", "T2tt_250_75", itSignal, kAzure-2, 0.1); 
  p->AddSample("T2tt_200_50_FS_summer", "S_200_50", itSignal, kBlue+2, 0.1); 
  p->AddSample("T2tt_225_50_FS_summer", "S_225_50", itSignal, kPink-1, 0.1); 
  p->AddSample("T2tt_250_50_FS_summer", "S_250_50", itSignal, kGreen+2, 0.1); 

  //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
  p->AddSystematic("JES,Btag");
  p->PrintYields();
  p->PrintSamples();
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  p->doSetLogy = true;
  p->DrawStack("0_log", 1);
  p->PrintSystYields();
  delete p;
}

