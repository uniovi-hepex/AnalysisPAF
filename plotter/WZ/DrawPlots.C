R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName="", TString cutName="", TString outFolder="");
TString NameOfTree = "top"; //"nolepMVA", "medium"
TString pathToTree = "";

bool doSync = false;
// Baseline
//TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && !TIsOnZ";
//TString CRZ      = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && TIsOnZ";
TString SR        = "TIsSR == 1 && TNTightLeps == 3";
TString CRDY      = "TIsCRDY == 1 && TNTightLeps == 3";
TString CRTT      = "TIsCRTT == 1 && TNTightLeps == 3";


void DrawPlots(TString cutName){
  
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="carlosec") pathToTree ="/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp";

  TString cut;
  if     (cutName == "SR" ) cut = SR ;
  else if(cutName == "CRDY" ) cut = CRDY ;
  else if(cutName == "CRTT" ) cut = CRTT ;
  else {cout << "Wrong name!!" << endl; return;}

  DrawPlot("TMET",  cut, "ElElEl", 10, 0, 300, "E_{T}^{miss}", cutName);
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
  


	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",                     "Data",  itData);             // Data
  p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ


  p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kBlue+10);  // WW
  p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, ZZG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
  p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo",    "VVV/VH",  itBkg, kGreen-9); // RareSM
  p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
  p->AddSample("tZq_ll",    "tZq",  itBkg, kGreen); // RareSM
  p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM

  if (NameOfTree == "top"){//Load MC FAKES
  	p->AddSample("TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo" ,     "Single t", itBkg, kGray);
    p->AddSample("TTbar_Powheg", "tt", itBkg, kRed);
    p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kOrange-3);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "DY", itBkg, kOrange);
  }

  if (NameOfTree != "top"){//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    //PROMPT SUB
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kBlue+10);  // WW
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, ZZG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo",    "VVV/VH",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM

  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",                     "DataFakes",  itBkg);             // Data
  }

  // Charge misID
  

  p->SetSignalProcess("WZ");
  //p->ScaleSignal(10);
  p->SetSignalStyle("SM");

  p->SetRatioMin(0.2);
  p->SetRatioMax(1.8);

//  p->AddSystematic("stat");
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

