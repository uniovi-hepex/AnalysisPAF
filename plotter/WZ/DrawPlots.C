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


void DrawPlots(TString cutName, TString treeName = ""){
  
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="carlosec") pathToTree ="/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/";
//  if(username=="carlosec") pathToTree ="/pool/ciencias/userstorage/carlosec/WZTrees_20_09_2017/"; //Testing the plotter
  if(treeName != "") NameOfTree = treeName;

  TString cut;
  if     (cutName == "SR" ) cut = SR ;
  else if(cutName == "CRDY" ) cut = CRDY ;
  else if(cutName == "CRTT" ) cut = CRTT ;
  else {cout << "Wrong name!!" << endl; return;}

  DrawPlot("TMET",  cut, "All", 10, 0, 300, "E_{T}^{miss}", cutName);
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
  

  if (NameOfTree == "top"){//Load MC FAKES
    p->SetCut(cut + " && TConvNumber > 0 && TFakeNumber == 0");
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
  
    p->SetCut(cut + " && TConvNumber == 0 && TFakeNumber == 0");
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",                     "Data",  itData);             // Data
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ

//  p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kBlue+10);  // WW
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
/*    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
  	p->AddSample("TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo" ,     "Single t", itBkg, kGray);
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "tt + Single t", itBkg, kRed);
    p->AddSample("WJetsToLNu_MLM", "WJets", itBkg, kOrange-3);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "DY", itBkg, kOrange);*/
  }
    AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_el_data_comb", "mvaMFRhistoEl");
    AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_mu_data_comb", "mvaMFRhistoMu");
  if (NameOfTree != "top"){//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    p->SetCut(cut + " && TConvNumber > 0 && TFakeNumber == 0");
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut + " && TConvNumber == 0 && TFakeNumber == 0");
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",                     "Data",  itData);             // Data
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ  
    p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kBlue+10);  // WW
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll",    "tZq",  itBkg, kGreen); // RareSM
    //p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
      //p->SetCut("TIsSR == 1 && TNFOLeps == 3");
      //p->SetWeight("GetFRweightlepMVAM3lep(GetFRweightMVAM(TLep_Pt[0], TLep_Eta[0], TIsTight[0], TLep_pdgId[0]), GetFRweightMVAM(TLep_Pt[1], TLep_Eta[1], TIsTight[1], TLep_pdgId[1]), GetFRweightMVAM(TLep_Pt[2], TLep_Eta[2], TIsTight[2], TLep_pdgId[2]), 1)");
      //p->SetWeight("GetFRweightlepMVAM3lep(GetFRweightMVAM(TLep_Pt[0], TLep_Eta[0], TIsTight[0], TLep_pdgId[0]), GetFRweightMVAM(TLep_Pt[1], TLep_Eta[1], TIsTight[1], TLep_pdgId[1]), GetFRweightMVAM(TLep_Pt[2], TLep_Eta[2], TIsTight[2], TLep_pdgId[2]), 1)");
      p->SetCut("TIsSR == 1 && TNFOLeps == 3");      
    //p->SetWeight("GetFRweightlepMVAM3lep(GetFRweightMVAM(TLep_Pt[0], TLep_Eta[0], TIsTight[0], TLep_pdgId[0]), 0.01, 0.01, 1)");
    p->SetWeight("GetFRweightMVAM(TLep_Pt+1, TLep_Eta+1, TIsTight+1, TLep_pdgId+1)");

    //p->SetWeight("dumpFR(TIsTight[1])");
    //DATA FAKES

  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",                     "DataFakes",  itBkg, kBlue,"0", "fake");             // Data
    //PROMPT SUB
    //p->SetWeight("GetFRweightlepMVAM(TLep_Pt, TLep_Eta, TIsTight, TLep_pdgId,TNTightLeps, true)");
    //p->SetCut(cut + " && TConvNumber > 0 && TFakeNumber == 0");
//    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2, "0");  // X+gamma 
    //p->SetCut(cut + " && TConvNumber == 0 && TFakeNumber == 0");
    //p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow, "0");    // WZ
    //p->AddSample("WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",         "WW",       itBkg, kBlue+10, "0");  // WW
    //p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo",    "VVV/VH",  itBkg, kGreen-9, "0"); // RareSM
    //p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5, "0"); // RareSM
    //p->AddSample("tZq_ll",    "tZq",  itBkg, kGreen, "0"); // RareSM
    //  p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10,"0"); // RareSM
  }



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
  //p->doSetLogy = true;
  p->DrawStack("0", 1);
  //p->doSetLogy = true;
  p->DrawStack("log", 1);
  //p->PrintSystYields();
  delete p;
}

