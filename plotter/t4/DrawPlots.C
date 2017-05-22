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
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Trees4t/may11_RA7objects/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Trees4t/may20_withSFs/";

// Baseline
TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2";
//enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, i2l1tau, i2l2taus, i2lss_fake, iTriLep_fake, nTotalDefinedChannels};

void DrawPlots(TString chan = "ElMu"){
/*  DrawPlot("TNJets", "TNSelLeps == 2  && "  + baseline, "SS",       6, 2, 8, "Jet Multiplicity", "nJets");
  DrawPlot("TNJets", "TNSelLeps >  2  && "  + baseline, "MultiLep", 6, 2, 8, "Jet Multiplicity", "nJets");
  DrawPlot("TNJets",                          baseline, "All"     , 6, 2, 8, "Jet Multiplicity", "nJets");
  DrawPlot("TNBtags", "TNSelLeps == 2  && " + baseline, "SS",       7, 0, 7, "b-jet Multiplicity", "nBJets");
  DrawPlot("TNBtags", "TNSelLeps >  2  && " + baseline, "MultiLep", 7, 0, 7, "b-jet Multiplicity", "nBJets");
  DrawPlot("TNBtags",                         baseline, "All"     , 7, 0, 7, "b-jet Multiplicity", "nBJets");
  DrawPlot("THT", "TNSelLeps == 2  && "     + baseline, "SS",        16, 0, 1600, "HT [GeV]", "HT");
  DrawPlot("THT", "TNSelLeps >  2  && "     + baseline, "MultiLep",  16, 0, 1600, "HT [GeV]", "HT");
  DrawPlot("THT",                             baseline, "All"     ,  16, 0, 1600, "HT [GeV]", "HT");
  DrawPlot("TMET", "TNSelLeps == 2  && "    + baseline, "SS",       15, 0, 600, "MET [GeV]", "MET");
  DrawPlot("TMET", "TNSelLeps >  2  && "    + baseline, "MultiLep", 15, 0, 600, "MET [GeV]", "MET");
*/  DrawPlot("TMET", "THT > 300 && TMET > 50",   Form("TChannel == %i || TChannel == %i", i2lss, iTriLep)     , 15, 0, 600, "MET [GeV]", "MET");
 // DrawPlot("TMET", "TNJets >= 2",  "TChannel == 3"  , 15, 0, 600, "MET [GeV]", "MET");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = false;
//  p->doData = false;
  if(name != "") p->SetVarName(name);

  // WZ
  p->AddSample("WZTo3LNu", "WZ", itBkg, kOrange, 0.50);

  // WW
  p->AddSample("WWTo2L2Nu",            "WW", itBkg, kOrange-3, 0.50);
  p->AddSample("WpWpJJ",               "WW", itBkg);
  p->AddSample("WWTo2L2Nu_DoubleScat", "WW", itBkg);

  // X+gamma
  p->AddSample("TGJets",   "X+#gamma", itBkg, kViolet+2, 0.50);
  p->AddSample("TTGJets",  "X+#gamma", itBkg);
  p->AddSample("WGToLNuG", "X+#gamma", itBkg);
  p->AddSample("ZGTo2LG",  "X+#gamma", itBkg);
  //"WZG_amcatnlo" "WWG_amcatnlo"
  
   // RareSM
  p->AddSample("WWW",                "Rare SM", itBkg, kMagenta-7, 0.50);
  p->AddSample("WWZ",                "Rare SM", itBkg);
  p->AddSample("WZZ",                "Rare SM", itBkg);
  p->AddSample("ZZZ",                "Rare SM", itBkg);
  p->AddSample("ZZTo4L",             "Rare SM", itBkg);
  p->AddSample("VHToNonbb_amcatnlo", "Rare SM", itBkg);

  // Charge misID

  // Nonprompt
  p->AddSample("WJetsToLNu_MLM", "Nonprompt", itBkg, kGray, 0.50);
  p->AddSample("DYJetsToLL_M5to50_MLM", "Nonprompt", itBkg);
  p->AddSample("DYJetsToLL_M50_MLM", "Nonprompt", itBkg);
  p->AddSample("TTbar_Powheg", "Nonprompt", itBkg);
  p->AddSample("TbarW_noFullyHadr, TW_noFullyHadr", "Nonprompt", itBkg);
  //p->AddSample("TbarW, TW" , "Nonprompt", itBkg);
  p->AddSample("T_tch", "Nonprompt", itBkg);
  p->AddSample("Tbar_tch", "Nonprompt", itBkg);
  p->AddSample("TToLeptons_sch_amcatnlo", "Nonprompt", itBkg);

	//p->AddSample("MuonEG",     "Nonprompt", itBkg, kGray, 0.50, "0", "Fake");
 	//p->AddSample("DoubleEG",   "Nonprompt", itBkg, kGray, 0.50, "0", "Fake");
	//p->AddSample("DoubleMuon", "Nonprompt", itBkg, kGray, 0.50, "0", "Fake");

 
  // ttZ/H
  p->AddSample("TTZToLLNuNu",    "ttZ/H", itBkg, kTeal+2);
  p->AddSample("TTZToLL_M1to10", "ttZ/H", itBkg);
  p->AddSample("TTHNonbb",       "ttZ/H", itBkg);
  p->AddSample("tZq_ll",         "ttZ/H", itBkg);

  // ttW
  p->AddSample("TTWToLNu",       "ttW", itBkg, kGreen+4, 0.50);

  p->AddSample("TTTT", "tttt", itSignal, kRed+1, 0.50);

	p->AddSample("MuonEG",     "Data", itData);
 	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);

  //p->SetSignalProcess("tttt");
  //p->ScaleSignal(10);
  p->SetSignalStyle("SM");

  p->SetRatioMin(0.2);
  p->SetRatioMax(1.8);

  //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
  //p->AddSystematic("stat");
  //p->PrintYields();
  //p->PrintSamples();
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  p->doSetLogy = true;
  p->DrawStack("log", 1);
  //p->PrintSystYields();
  delete p;
}

