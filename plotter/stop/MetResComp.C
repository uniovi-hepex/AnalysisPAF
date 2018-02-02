//#########################################################################################
//
//#########################################################################################

R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"
#include "PlotterFunctions.C"

//=== Functions 
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins = 0, TString Xtitle = "", TString name = "");
void PrintNonWZestimate(TString chan = "ElMu", TString cut = "");
void PrintYieldTalbe(TString chan = "ElMu", TString cut = "");
void DrawSignalPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name);
void DrawComp(TString var = "");

//=== Define paths and tree name
TString pathToTree    = "/pool/ciencias/userstorage/juanr/stop/sep22/";
TString pathToTreeMET = "/pool/ciencias/userstorage/juanr/stop/nov20/";
TString NameOfTree = "tree";
TString outputFolder = "./output/MET_Resolution/"; //_topPtWeight

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";
TString DYcut       = "!TIsSS && TPassTrigger && TPassMETfilters && TMath::Abs(TMll - 91) < 15";
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
TString channels = "Muon, Elec, ElMu";

// #########################################################################
// ## Functions
void SavePlot(TString name, TString titlex, Float_t ratioscale = 1.4);
void SetSystematics();
void SetMultiPlot();

// Global
MultiPlot* mp = new MultiPlot();
TString DistName = "InvMass, MET, METmod, MT2";
TString DYsamples = "DYJetsToLL_M50_aMCatNLOSum_0, DYJetsToLL_M50_aMCatNLOSum_1, DYJetsToLL_M50_aMCatNLOSum_2, DYJetsToLL_M50_aMCatNLOSum_3, DYJetsToLL_M50_aMCatNLOSum_4, DYJetsToLL_M50_aMCatNLOSum_5"; 

void MetResComp(){
  //DrawComp();
  TString chan = "";
  vector<TString> vchan = TStringToVector(channels);
  SetMultiPlot();
  for(Int_t i = 0; i < (Int_t) vchan.size(); i++){
    chan = vchan.at(i);
    SavePlot("InvMass_"+chan, "M_{\\ell\\ell} [GeV]");
    SavePlot("MET_Corr_"+chan, "MET [GeV]");
    SavePlot("METmod_Corr_"+chan, "M_{T2} [GeV]");
  }
  return;
}

void SetMultiPlot(){
  TString cut = BaselineCut;
  TString weight = "TWeight";
  TString chan = "";
  TString fun =  "GetModMET(TMET, TMET_Phi, TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E)";

  mp->SetPath(pathToTreeMET);
  mp->SetPathData(pathToTreeMET);
  mp->SetTreeName(NameOfTree);
  mp->SetPlotFolder(outputFolder);

  cut = DYcut;

  vector<TString> vchan = TStringToVector(channels);
  for(Int_t i = 0; i < (Int_t) vchan.size(); i++){
    chan = vchan.at(i);
    mp->AddDistribution("InvMass_" + chan, "TMll", cut, chan, 40, 76, 106);
    mp->AddDistribution("MET_Corr_"     + chan, "TMET", cut, chan, 40, 0, 200);
    mp->AddDistribution("METmod_Corr_"  + chan,  fun,   cut, chan, 40, 0, 300);
    //mp->AddDistribution("MET_Corr"     + chan, "TMET*GetMetResNorm(TMET)", cut, chan, 40, 0, 200);
    //mp->AddDistribution("METmod_Corr"  + chan,  fun+"*GetMetResNorm(TMET)",   cut, chan, 40, 0, 300);
    //mp->AddDistribution("MT2_"     + chan, "TMT2", cut, chan, ngbins, 0, 0, gbins);
  }

  weight = "TWeight*GetMetResNorm(TMET)";
  mp->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics, weight);
	mp->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics, weight);
	mp->AddSample(DYsamples + ", DYJetsToLL_M10to50_aMCatNLO",            "DY",        itBkg, kAzure-8,   systematics, weight);
	mp->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   systematics, weight);
	mp->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics, weight);
	mp->AddSample("TTbar_Powheg",                                         "ttbar",     itBkg, kRed+1,     systematics + ", JER", weight);
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData, 1, "", "1");

  // Plotting options  
  mp->SetOutputName("H_");
  mp->SetLegendTextSize(0.042);
  mp->SetLegendPosition(0.60, 0.60, 0.93, 0.93);

  mp->SetRatioErrorColor(kTeal-2);
  mp->SetRatioErrorStyle(3244);
  mp->SetStackErrorStyle(3244);
  mp->doYieldsInLeg = false; mp->doSignal = false;
  mp->SetRatioMin(0.8); mp->SetRatioMax(1.2);
}


void SetSystematics(){
  // Norm systematics
  mp->AddSystematic("stat"); 
  mp->AddNormUnc("ttbar", 0.05);
  mp->AddNormUnc("VV", 0.30);
  mp->AddNormUnc("ttV", 0.30);
  mp->AddNormUnc("DY", 0.20);
  mp->AddNormUnc("tW", 0.30);
  mp->AddNormUnc("Nonprompt", 0.50);

  // Scale fakes
  Float_t fakeSS = 1.5;
  mp->ScaleProcess("Nonprompt", fakeSS);
}


  // InvaMass....................................................
void SavePlot(TString name, TString titlex, Float_t ratioscale){
  mp->SetPlot(name);
  mp->SetTitleX(titlex);
  SetSystematics();
  // mp->SetLegendTextSize(0.035);
  // mp->SetLegendPosition(0.70, 0.70, 0.93, 0.93);

  mp->SetRatioMin(2-ratioscale); mp->SetRatioMax(ratioscale);
  mp->SetOutputName(name);
  mp->doSetLogy = false;
  mp->DrawStack();
  mp->doSetLogy = true;
  mp->SetOutputName(name+ "_log");
  mp->DrawStack();
  mp->PrintBinsYields();
}











void DrawComp(TString var){
  if(var == ""){
    DrawComp("TMT2");
    DrawComp("TMET");
    return;
  }
  TString cut = BaselineCut;
  TString weight = "TWeight";
  TString chan = "ElMu";
  TString fun =  "GetMetResNorm(TMET)";
  Int_t nbins = 40; Float_t bin0 = 0; Float_t binN = 400;
  Float_t *bins = 0;
  TString Xtitle = "MET [GeV]";

  if(var == "TMT2"){
    nbins = ngbins;
    bin0 = 1; binN = 1;
    bins = gbins;
    Xtitle = "M_{T2} GeV";
  }

  Plot* p;
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);

  p->SetPath(pathToTree);
  p->SetTreeName(NameOfTree);
  p->SetPlotFolder(outputFolder);
  p->SetLegendTextSize(0.045); p->SetLegendPosition(0.50, 0.70, 0.93, 0.93);
  p->doYieldsInLeg = false;

  p->AddSample("TTbar_Powheg", "t#bar{t} nominal", itSignal, 1, "");
  p->SetWeight("TWeight*" + fun);
  p->AddSample("TTbar_Powheg", "t#bar{t} MET weighted", itSignal, kRed+1, "MET corrected");

  p->doSetLogy = false;
  p->DrawComp("CompMETres");
  p->doSetLogy = true;
  p->DrawComp("CompMETres_log");
}
