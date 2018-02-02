R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(CrossSection.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString NameOfTree = "MiniTree";
TString SScut = "TMT2 > 0 && TMET > 50 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps < 3";
TString VarNBtagsNJets = "TNBtags + (TNJets == 1) + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";

//TString dilepton = "TLep_Charge[0]*TLep_Charge[1] < 0";
//TString jets2    = "TLep_Charge[0]*TLep_Charge[1] < 0 && TNJets >= 2";
//TString btag1    = "TLep_Charge[0]*TLep_Charge[1] < 0 && TNJets >= 2 && TNBtags > 0";
TString dilepton = "!TIsSS";
TString ZVeto    = "!TIsSS && fabs(TMll - 91) > 15";
TString met      = "!TIsSS && fabs(TMll - 91) > 15 && TMET > 40";
TString jets2    = "!TIsSS && fabs(TMll - 91) > 15 && TMET > 40 && TNJets >= 2";
TString btag1    = "!TIsSS && fabs(TMll - 91) > 15 && TMET > 40 && TNJets >= 2 && TNBtags > 0";
//TString dilepton = "!TIsSS";
//TString jets2    = "!TIsSS && TNJets >= 2";
//TString btag1    = "!TIsSS && TNJets >= 2 && TNBtags > 0";

// Baseline

TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/Muon/";
void xsec(){
  Plot* p = new Plot("TChannel", dilepton, "Muon", 1, 0, 10, "Channel", "xsec");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
  //p->doData = false;
  //p->doStackSignal = true;

  p->AddSample("WZ", "VV", itBkg, kYellow-10, 0.50);
  p->AddSample("WW", "VV", itBkg);
  p->AddSample("ZZ", "VV", itBkg);
  p->AddSample("TTbar_Powheg_Semilep", "NonWZ", itBkg, kGreen+1, 0.5);
  p->AddSample("WJetsToLNu_aMCatNLO", "NonWZ", itBkg);
	p->AddSample("TTWToLNu", "ttV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTWToQQ", "ttV", itBkg);
	p->AddSample("TTZToQQ", "ttV", itBkg);
	p->AddSample("TTZToLLNuNu", "ttV", itBkg);
	p->AddSample("TW", "tW", itBkg, kMagenta, 0.3);
	p->AddSample("TbarW", "tW", itBkg);
  p->AddSample("DYJetsToLL_M50_aMCatNLO", "DY", itBkg, kAzure-8, 0.50);
  p->AddSample("DYJetsToLL_M10to50_aMCatNLO",     "DY", itBkg);
  p->AddSample("TTbar_PowhegLHE", "ttbar", itBkg, kRed+1, 0.00);
  p->AddSample("MuonEG", "Data", itData);
  //p->AddSample("DoubleEG", "Data", itData);
  //p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);
/*
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp");
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown");
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp");
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown");
  p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp");
  p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown");
  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp");
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "nloDown");
  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp");
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "hadDown");
*/


 // p->AddSystematic("JES");
  p->PrintYields("", "", "", "html");
  //p->PrintYields(dilepton + ", " + ZVeto + ", " + met + ", " + jets2 + ", " + btag1, "Dilepton, ZVeto, met, 2jets, 1btag", "Muon, Muon, Muon, Muon, Muon", "html");

  CrossSection *x = new CrossSection(p, "ttbar");
  x->SetTheoXsec(831.8);
  x->SetChannelTag("ElMu");
  x->SetLevelTag(">2jets");
  x->PrintSystematicTable();
  x->PrintCrossSection();
  //x->SetBR();


  //p->ScaleProcess("VV", 2.);
  //p->PrintYields();

  //p->PrintSamples();
  //p->PrintSystYields();
  //p->PrintYields("TMET > 50, TMET > 50, TMET > 50", "ElMu, Elec, Muon", "ElMu, Elec, Muon");
  //p->DrawStack("prueba", 1);
 
  //Plot* k = p->NewPlot("TMET", "TMET > 200");
  //k->PrintYields();
  //k->PrintSamples();
  //k->PrintSystYields();

  //delete p;
}

