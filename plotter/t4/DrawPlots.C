R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName="", TString cutName="", TString outFolder="");
TString NameOfTree = "tree";
TString pathToTree = "/pool/ciencias/userstorage/juanr/tttt/oct20_prueba/";
TString CutNames[] = {"CRZ", "CRW", "SR1",  "SR2", "SR3", "SR4", "SR5", "SR6", "SR7", "SR8", "SR9", "SR10", "CRT"}; Int_t nCutNames = 10;
TString Processes[] = {"ttW", "ttZ", "ttH", "ttVV", "Conv", "Rare SM", "Flips", "Fakes", "AllBkg", "Data", "tttt"}; Int_t nProcesses = 11;
TString sys = "JER, LepEff, Btag, Mistag, PU, Trig";

bool doSync = false;
// Baseline
//TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && !TIsOnZ";
//TString CRZ      = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && TIsOnZ";
TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && !TIsOnZ && TPassTrigger && TPassMETFilters";
TString tauVeto  = doSync ? " && 1 " : " && TNTaus==0 ";
TString CRT      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 0 && TNJets >1";
TString CRZ      = tauVeto + "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TIsOnZ  && TPassTrigger && TPassMETFilters";
TString CRW      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets <= 5";
TString SR1      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 6";
TString SR2      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 7";
TString SR3      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets >= 8";
TString SR4      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && (TNJets == 5 || TNJets == 6)";
TString SR5      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >= 7";
TString SR6      = baseline + tauVeto + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >= 5";
TString SR7      = baseline + tauVeto + "&& TNSelLeps == 3 && TNBtags == 2 && TNJets >= 5";
TString SR8      = baseline + tauVeto + "&& TNSelLeps == 3 && TNBtags >= 3 && TNJets >= 4";
TString SR9      = baseline + "&& TNSelLeps >= 2 && TNTaus>=1 && TNBtags == 2";
TString SR10     = baseline + "&& TNSelLeps >= 2 && TNBtags >= 3 &&  TNTaus>=1";


TString NoFake   = Form("TChannel == %i || TChannel == %i || TChannel == %i", i2lss, iTriLep, iSS1tau);

void DrawPlots(TString cutName = "BaselineCut"){
  
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="vischia") pathToTree ="/nfs/fanae/user/juanr/AnalysisPAF/tttt_temp/";

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
  //else {cout << "Wrong name!!" << endl; return;}
  else cut = baseline;
  TString chan = NoFake;

  // Signal and control regios
  //DrawPlot("SR(TNSelLeps, TNJets, TNBtags)",         baseline, NoFake, 10, 0, 10, "Region", "SRsync");
  //DrawPlot("SR(TNSelLeps, TNJets, TNBtags, TNTaus)", baseline, NoFake, 10, 0, 10, "Region", "SR");

  // Channel
  //DrawPlot("GetChannel(TChannel)",  baseline, NoFake, 3, 1, 4, cutName, "Categories");

  // Kinematic plots
  DrawPlot("TNJets",     cut, NoFake     , 7, 2, 9, "Jet Multiplicity", "nJets");
  DrawPlot("TNBtags",    cut, NoFake     , 4, 2, 6, "Btag Multiplicity", "nBtags");
  DrawPlot("TMET",       cut, NoFake     , 15, 0, 500, "MET [GeV]", "MET");
  DrawPlot("THT",       cut, NoFake     , 20, 300, 800, "HT [GeV]", "HT");
  //InvMass
  //min m_T
  //etc...

 gApplication->Terminate();
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName, TString cutName, TString outFolder){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPlotFolder("./t4/Plots/");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = false;
  p->doData = true;
  if(varName != "") p->SetVarName(varName);
  if(cutName != "") p->SetOutputName(cutName);
  TString cutForFakes = cut;
  cutForFakes.ReplaceAll("TNSelLeps", "(TNSelLeps + TNFakeableLeps)");
  p->SetWeight("TWeight");

  if(var.Contains("GetChannel")) p->SetBinLabels(t4ChannelLabels);
  if(varName == "nJets") p->SetBinLabels("2, 3, 4, 5, 6, 7, 8");
  if(varName == "nBtags") p->SetBinLabels("2, 3, 4, 5");
  if(var.Contains("GetChannel")) p->SetBinLabels(t4ChannelLabels);

  p->AddSample("TTWToLNu",                                         "ttW",   itBkg,    kGreen+4, sys); // ttW
  p->AddSample("TTZToLLNuNu, TTZToLL_M1to10",                      "ttZ",   itBkg,    kBlue+2,  sys);  // ttZ
  p->AddSample("TTHNonbb",                                         "ttH",   itBkg,    kOrange+2,  sys);  // ttH
  p->AddSample("TTZH, TTHH, TTZZ, TTWZ, TTTW, TTTJ, TTWH, TTWW",         "ttVV",  itBkg,    kTeal+2, sys);  // ttVV
  p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG",                "X+#gamma", itBkg, kViolet+2, sys);  // X+gamma //"WZG_amcatnlo" "WWG_amcatnlo"
  p->AddSample("WZTo3LNu, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat, WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo, tZq_ll",    "Rare SM",  itBkg, kMagenta-7); // RareSM

  // Nonprompt from MC
  //p->AddSample("WJetsToLNu_MLM, DYJetsToLL_M5to50_MLM, DYJetsToLL_M50_MLM",                                 "Nonprompt", itBkg, kGray);
  //p->AddSample("TTbar_Powheg, TW_noFullyHadr, TbarW_noFullyHadr, T_tch, Tbar_tch, TToLeptons_sch_amcatnlo", "Nonprompt", itBkg);

  // Nonprompt from data
  p->SetCut(cutForFakes);
  p->SetChan("IsFake(TChannel)");
  p->SetWeight("FRweight(TNFakeableLeps, TFLep0Pt, TFLep1Pt, TFLep0Eta, TFLep1Eta, TFLep0Id, TFLep1Id)");
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",     "Nonprompt", itBkg, kGray, "0", "noScaleLumi");
  
  p->SetWeight("-1*TWeight*FRweight(TNFakeableLeps, TFLep0Pt, TFLep1Pt, TFLep0Eta, TFLep1Eta, TFLep0Id, TFLep1Id)");
  p->AddSample("WZTo3LNu, WpWpJJ,WWTo2L2Nu_DoubleScat, WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo,TTZToLLNuNu, TTZToLL_M1to10, TTHNonbb, tZq_ll,TTWToLNu", "Nonprompt", itBkg, kGray, "0");

  // Charge misID
  p->SetCut(cut);
  p->SetChan(Form("TChannel == %i",i2LOS));
  p->SetWeight("FlipWeight(TLep0Pt, TLep0Eta, TLep0Id, TLep1Pt, TLep1Eta, TLep1Id)");
  p->AddSample("MuonEG, DoubleEG, DoubleMuon",     "Flips", itBkg, kBlack, "0", "noScaleLumi");

  p->SetWeight("TWeight"); p->SetChan(chan);
  p->AddSample("TTTT",                                             "tttt",  itSignal, kRed+1, sys);   // tttt signal
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",                     "Data",  itData);             // Data

  p->SetSignalProcess("tttt");
  p->SetSignalStyle("SM");
  p->SetRatioMin(0.0); p->SetRatioMax(2.0);

  p->AddSystematic("stat");
  p->doSetLogy = false;
  p->DrawStack("0", 1);
  p->doSetLogy = true;
  p->DrawStack("log", 1);
  delete p;
}

