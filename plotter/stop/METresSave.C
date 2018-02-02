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

//=== Define paths and tree name
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/nov20/";
TString NameOfTree = "tree";
TString outputFolder = "./output/MET_Resolution/2jets/"; //_topPtWeight

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";
TString DYcut       = "!TIsSS && TPassTrigger && TPassMETfilters && TMath::Abs(TMll - 91) < 15 && TNJets >= 2";
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
TString channels = "SF, Elec, Muon";

// #########################################################################
// ## Functions
void SavePlot(TString name);
void SetMultiPlot();

// Global
MultiPlot* mp = new MultiPlot();
TString DistName = "InvMass, MET, METmod";
TString DYsamples = "DYJetsToLL_M50_aMCatNLOSum_0, DYJetsToLL_M50_aMCatNLOSum_1, DYJetsToLL_M50_aMCatNLOSum_2, DYJetsToLL_M50_aMCatNLOSum_3, DYJetsToLL_M50_aMCatNLOSum_4, DYJetsToLL_M50_aMCatNLOSum_5"; 

void METresSave(){
  SetMultiPlot();
  TString chan = "";
  vector<TString> vchan = TStringToVector(channels);
  chan = "Muon";
//  for(Int_t i = 0; i < (int) vchan.size(); i++){
 //  chan = vchan.at(i);
   SavePlot("InvMass_" + chan);
 //  SavePlot("MET_"     + chan);
 //  SavePlot("METmod_"  + chan);
 // }
  return;
}

void SetMultiPlot(){
  TString cut = BaselineCut;
  TString weight = "TWeight";
  TString chan = "";
  TString fun =  "GetModMET(TMET, TMET_Phi, TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E)";

  mp->SetPath(pathToTree);
  mp->SetTreeName(NameOfTree);
  mp->SetPathData(pathToTree);
  mp->SetPathSignal(pathToTree + "T2tt/");
  mp->SetPlotFolder(outputFolder);

  cut = DYcut;
chan = "Elec";
    mp->AddDistribution("InvMass_" + chan, "TMll", cut, chan, 40, 76, 106);
    mp->AddDistribution("MET_"     + chan, "TMET", cut, chan, 40, 0, 200);
    mp->AddDistribution("METmod_"  + chan,  fun,   cut, chan, 40, 0, 300);
chan = "Muon";
    mp->AddDistribution("InvMass_" + chan, "TMll", cut, chan, 40, 76, 106);
    mp->AddDistribution("MET_"     + chan, "TMET", cut, chan, 40, 0, 200);
    mp->AddDistribution("METmod_"  + chan,  fun,   cut, chan, 40, 0, 300);
chan = Form("(TChannel == %i || TChannel == %i)", iMuon, iElec);
    mp->AddDistribution("InvMass_SF", "TMll", cut, chan, 40, 76, 106);
    mp->AddDistribution("MET_SF"   , "TMET", cut, chan, 40, 0, 200);
    mp->AddDistribution("METmod_SF",  fun,   cut, chan, 40, 0, 300);

  TString samples = DYsamples + ", DYJetsToLL_M10to50_aMCatNLO";
  samples += ", WZ, WW, ZZ";
  samples += ", TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu";
  samples += ", TW, TbarW";
  samples += ", WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG";
  samples += ", TTbar_Powheg";

	mp->AddSample(samples, "Bkg",        itBkg, kAzure-8,   systematics, weight);
	mp->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  // Plotting options  
  mp->SetOutputName("S");
  mp->SetLegendTextSize(0.042);
  mp->SetLegendPosition(0.60, 0.60, 0.93, 0.93);

  mp->SetRatioErrorColor(kTeal-2);
  mp->SetRatioErrorStyle(3244);
  mp->SetStackErrorStyle(3244);
  mp->doYieldsInLeg = false; mp->doSignal = false;
  mp->SetRatioMin(0.8); mp->SetRatioMax(1.2);
}



  // InvaMass....................................................
void SavePlot(TString name){
  mp->SetPlot(name);
  mp->SaveHistograms();
}

