R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName="", TString cutName="", TString outFolder="");
TString NameOfTree = "tree";
TString pathToTree = "";

bool doSync = false;
// Baseline
//TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && !TIsOnZ";
//TString CRZ      = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && TIsOnZ";
TString baseline = Form("TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 3 && !TIsOnZ && TPassTrigger %s", doSync? "" : "&& TPassMETFilters ") ;
TString tauVeto  = doSync ? " && 1 " : " && TNTaus==0 ";
TString CRZ      = tauVeto + "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TIsOnZ";
TString CRW      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets <= 5";
TString CRT      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 0 && TNJets >1";
TString SR1      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 6";
TString SR2      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 7";
TString SR3      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets >= 8";
TString SR4      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && (TNJets == 5 || TNJets == 6)";
TString SR5      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >= 7";
TString SR6      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >= 5";
TString SR7      = baseline + tauVeto + "&& TNSelLeps == 3 && TNBtags == 2 && TNJets >= 5";
TString SR8      = baseline + tauVeto + "&& TNSelLeps == 3 && TNBtags >= 3 && TNJets >= 4";
TString SR9      = baseline + "&& TNSelLeps == 2 && TNTaus==1 && TChannel == iSS1tau && TNBtags >= 3";
TString SR10     = baseline + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >=5 && TNTaus==1";


TString NoFake   = Form("TChannel == %i || TChannel == %i || TChannel == %i || TChannel == %i", i2lss, iTriLep, iSS1tau, iOS1tau);

void DrawPlots(TString cutName){
  
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="vischia") pathToTree ="/nfs/fanae/user/juanr/AnalysisPAF/tttt_temp/";
  else pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Trees4t/jun15/";

  TString cut;
  if     (cutName == "CRW" ) cut = CRW ;
  else if(cutName == "CRZ" ) cut = CRZ ;
  else if(cutName == "CRT" ) cut = CRT ;
  else if(cutName == "SR1" ) cut = SR1 ;
  else if(cutName == "SR2" ) cut = SR2 ;
  else if(cutName == "SR3" ) cut = SR3 ;
  else if(cutName == "SR4" ) cut = SR4 ;
  else if(cutName == "SR5" ) cut = SR5 ;
  else if(cutName == "SR6" ) cut = SR6 ;
  else if(cutName == "SR7" ) cut = SR7 ;
  else if(cutName == "SR8" ) cut = SR8 ;
  else if(cutName == "SR9" ) cut = SR9 ;
  else if(cutName == "SR10") cut = SR10;
  else {cout << "Wrong name!!" << endl; return;}

  NoFake = (cutName=="SR9" || cutName=="SR10" || cutName=="CRT") ? "1" : NoFake; 

  cut=baseline;
  DrawPlot("TChannel",  cut, "SSTau", 1, 0, 15, "Count", cutName);
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
*/  
  // DrawPlot("TMET", "THT > 300 && TMET > 50 && TNTaus == 0",   Form("TChannel == %i || TChannel == %i", i2lss, iTriLep)     , 15, 0, 600, "MET [GeV]", "MET");
  //DrawPlot("TMll", "THT > 300 && TMET > 50 && TNTaus == 0 && TNBtags >= 2 && TNJets >= 2",   Form("TChannel == %i || TChannel == %i", i2lss, iTriLep)     , 15, 0, 300, "InvMass [GeV]", "InvMass");

 // DrawPlot("TMET", "TNJets >= 2",  "TChannel == 3"  , 15, 0, 600, "MET [GeV]", "MET");

 gApplication->Terminate();
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName, TString cutName, TString outFolder){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  if(outFolder!="") p->SetPlotFolder(outFolder);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
//  p->doData = false;
  if(varName != "") p->SetVarName(varName);
  if(cutName != "") p->SetOutputName(cutName);

  if(cutName.Contains("SR")) p->doData=false;
  
  p->AddSample("WZTo3LNu",                                        "WZ",       itBkg, kOrange);    // WZ
  p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kOrange-3);  // WW
  p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG",                "X+#gamma", itBkg, kViolet+2);  // X+gamma //"WZG_amcatnlo" "WWG_amcatnlo"
  p->AddSample("WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo, tZq_ll",    "Rare SM",  itBkg, kMagenta-7); // RareSM
  p->AddSample("WZTo3LNu, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat", "Rare SM",  itBkg, kMagenta-7); // RareSM

  // Nonprompt from MC
  //p->AddSample("WJetsToLNu_MLM, DYJetsToLL_M5to50_MLM, DYJetsToLL_M50_MLM",                                 "Nonprompt", itBkg, kGray);
  //p->AddSample("TTbar_Powheg, TW_noFullyHadr, TbarW_noFullyHadr, T_tch, Tbar_tch, TToLeptons_sch_amcatnlo", "Nonprompt", itBkg);

  // Nonprompt from data
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",     "Nonprompt", itBkg, kGray, "0", "FakeLep");
  p->AddSample("WZTo3LNu, WWTo2L2Nu,WpWpJJ,WWTo2L2Nu_DoubleScat, WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo,TTZToLLNuNu, TTZToLL_M1to10, TTHNonbb, tZq_ll,TTWToLNu", "Nonprompt", itBkg, kGray, "0", "FakesubsLep");

  // Charge misID
  
  p->AddSample("TTHNonbb",                                         "ttH",   itBkg,    kTeal+2);  // ttH
  p->AddSample("TTZToLLNuNu, TTZToLL_M1to10",                      "ttZ",   itBkg,    kTeal+2);  // ttZ
  p->AddSample("TTWToLNu",                                         "ttW",   itBkg,    kGreen+4); // ttW
  p->AddSample("TTTT",                                             "tttt",  itSignal, kRed+1);   // tttt signal
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",                     "Data",  itData);             // Data

  p->SetSignalProcess("tttt");
  p->ScaleSignal(10);
  p->SetSignalStyle("SM");

  p->SetRatioMin(0.2);
  p->SetRatioMax(1.8);

  //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");
  p->AddSystematic("stat");
  cout << "Selection = " << varName << endl;
  cout << "Corresponding to cut: " << cut << endl;
  p->PrintYields();
  //p->PrintSamples();
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  //p->doSetLogy = true;
  //p->DrawStack("log", 1);
  //p->PrintSystYields();
  delete p;
}

