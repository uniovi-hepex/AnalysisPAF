//###################################################################################################
//
// Draw multple plots at gen level
// Uses minitrees produced only with gen info, no cuts on reco variables, no skimmed n-tupples
// Select your options below:
//    genModelingT2tt
//    recoModelingT2tt
//    topModelingT2tt
//    genxqcutSimple
//    recoxqcut
//    topxqcut
//    recoFastFull
//    genStopMass
//    recoStopMass
//  Ussage:
//    root -l -b -q 'GenLevelPlots.C("")'
//
//###################################################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

//=== Functions
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "0", TString lab = "");

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
//TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/sep22/forComp/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString dilepgen = "TNgenLeps >= 2 && fabs(TgenLep0_Eta) < 2.4 && fabs(TgenLep1_Eta) < 2.4";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";

//=== The main function
void GenLevelPlots(TString lab = ""){

  //>>> gen comparisons
  if(lab.Contains("gen")){ 
//    DrawPlot("fabs(TgenLep0_Eta) : fabs(TgenLep1_Eta)",      dilepgen, chan, 20, 0, 2.4, "gen lep #eta",             "genEta", lab);
//    DrawPlot("TgenLep0_Pt : TgenLep1_Pt",      dilepgen, "TChannel >= 0", 20, 0, 200, "gen lep p_{T} [GeV]",   "genPt", lab);
    DrawPlot("TgenMT2",      dilepgen, chan, 20, 0, 120, "gen M_{T2} [GeV]",             "genMT2", lab);
/*    DrawPlot("TgenMT2",      dilepgen, chan, 20, 0, 120, "gen M_{T2} [GeV]",             "genMT2", lab);
    //DrawPlot("TgenMT2",      dilepgen + " && TgenMT2 > 0", "ElMu", 20, 0, 120, "gen M_{T2} [GeV]", "genMT2_no0", lab);
    DrawPlot("TgenLep0_Pt",  dilepgen, chan, 15, 0, 200, "gen Leading lep p_{T} [GeV]",    "genLep0Pt", lab);
//    DrawPlot("TgenLep1_Pt",  dilepgen, chan, 15, 0, 200, "gen Subleading lep p_{T} [GeV]", "genLep1Pt", lab);
    //DrawPlot("TgenLep0_Pt",  dilepgen + " && TgenLep0_Id == 11", chan, 15, 0, 200, "gen Elec p_{T} [GeV]", "genElec0Pt", lab);
    //DrawPlot("TgenLep0_Pt",  dilepgen + " && TgenLep0_Id == 13", chan, 15, 0, 200, "gen Muon p_{T} [GeV]", "genMuonPt", lab);
    DrawPlot("TgenMET",      dilepgen, chan, 20, 0, 250, "gen MET [GeV]",                "genMET", lab);
    DrawPlot("GetDilepPtDPhi(TgenLep0_Pt, TgenLep1_Pt, TgenDeltaPhi)",      dilepgen, chan, 20, 0, 250, "gen p_{T}^{#font[12]{ll}} [GeV]",                "genDielpPt", lab);
    DrawPlot("TgenDeltaPhi", dilepgen, chan, 15, 0, 3.2, "gen #Delta#phi",                "gDeltaPhi", lab);
    DrawPlot("TgenDeltaEta", dilepgen, chan, 15, 0, 3.2, "gen #Delta#eta",                "gDeltaEta", lab);
 */   //DrawPlot("TMath::Abs(TgenMETPhi)",   dilepgen, chan, 15, 0, 3.2, "gen MET Phi",                  "genMETPhi", lab);
  }

  //>>> top related variables
  else if(lab.Contains("top")){ 
    DrawPlot("TgenTop1M",       dilepgen, chan, 40, 170, 180, "gen Top Mass", "Top1Mass", lab);
    DrawPlot("TgenTop2M",       dilepgen, chan, 40, 170, 180, "gen Top Mass", "Top2Mass", lab);
    DrawPlot("TgentopDeltaPhi", dilepgen, chan, 15, 0, 3.2,   "gen Top DeltaPhi", "TopDeltaPhi", lab);
    DrawPlot("TgentopDeltaR",   dilepgen, chan, 15, 0, 3.2,   "gen Top DeltaR",   "TopDeltaR", lab);
  }

  //>>> Reco vs gen
  else if(lab.Contains("reco")){ 
    DrawPlot("TLep0_Pt",  BaselineCut + " && TLep0_Id == 11", "ElMu", 15, 0, 200, "Elec p_T [GeV]", "ElecPt", lab);
    DrawPlot("TLep0_Pt",  BaselineCut + " && TLep0_Id == 13", "ElMu", 15, 0, 200, "Muon p_T [GeV]", "MuonPt", lab);
    DrawPlot("TLep0_Pt",  BaselineCut , "ElMu", 15, 0, 200, "Leading lep p_T [GeV]",    "Lep0Pt", lab);
    DrawPlot("TLep1_Pt",  BaselineCut , "ElMu", 15, 0, 200, "Subleading lep p_T [GeV]", "Lep1Pt", lab);
    DrawPlot("TDeltaPhi", BaselineCut , "ElMu", 15, 0, 3.2, "Delta Phi", "DeltaPhi", lab);
    DrawPlot("TDeltaEta", BaselineCut , "ElMu", 15, 0, 3.2, "Delta Eta", "DeltaEta", lab);
    DrawPlot("DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",    
        BaselineCut , "ElMu", 30, 0, 300, "p_{T}^{#font[12]{ll}} [GeV]",  "DilepPt", lab);
    DrawPlot("TMll",      BaselineCut , "ElMu", 20, 0, 200, "Mll [GeV]", "InvMass", lab);
    DrawPlot("TMET",      BaselineCut , "ElMu", 20, 0, 250, "MET [GeV]", "MET", lab);
    DrawPlot("TMath::Abs(TMET_Phi)",  BaselineCut , "ElMu", 15, 0, 3.2, "MET Phi", "METPhi", lab);
    DrawPlot("TMT2",      BaselineCut , "ElMu", 20, 0, 120, "M_{T2} [GeV]", "MT2", lab);
    DrawPlot("TMT2",      BaselineCut + " && TMT2 > 0", "ElMu", 20, 0, 120, "M_{T2} [GeV]", "MT2_no0", lab);
  }
  else{
    //GenLevelPlots("genModelingT2tt");
    GenLevelPlots("genTopPtReweight");
    //GenLevelPlots("recoModelingT2tt");
    //GenLevelPlots("topModelingT2tt");
    //GenLevelPlots("genxqcutSimple");
    //GenLevelPlots("recoxqcut");
    //GenLevelPlots("topxqcut");
    //GenLevelPlots("recoFastFull");
    //GenLevelPlots("genStopMass");
    //GenLevelPlots("recoStopMass");
    //GenLevelPlots("topStopMass");
  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString lab){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp_topPtWeight/" + lab + "/");
  p->verbose = true;
  p->doYieldsInLeg = false;
  p->SetVarName(name);

  //>>> This sample is always on the comparison
  //if(!lab.Contains("FastFull") && !lab.Contains("ModelingT2tt")) 
  //p->AddSample("TTbar_Powheg", "tt Powheg Gen", itSignal, 1,       "0", "noWeight");

  //>>> Modeling of T2tt processes
  if(lab.Contains("ModelingT2tt")){
    p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
    p->AddSample("TTbar_Powheg", "tt Powheg Gen", itSignal, 1,       "0");
   // p->AddSample("TTJets_aMCatNLO",       "tt aMCatNLO",   itSignal, kViolet-1,  "0", "");
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1,    "0");
    p->SetWeight("1");
    p->AddSample("T2tt_175_1_FS",         "T2tt [175,1]",  itSignal, kRed+1,     "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kBlue+1,    "0", "noWeight");
  }
  else if(lab.Contains("TopPtReweight")){
    p->AddSample("TTbar_Powheg", "tt nominal", itSignal, 1,       "0", "noWeight");
    p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
    p->AddSample("TTbar_Powheg", "tt top-pT weighted", itSignal, kRed+1,       "0");
  }

  //>>> Comparison of xqcut values
  else if(lab.Contains("xqcut")){
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1,  "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",          "xqcut30 [175,1]",  itSignal, kBlue+1, "0", "noWeight");
    //p->AddSample("T2tt_200_50_FS_summer",  "xqcut30 [200,50]", itSignal, 1,   "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer",  "xqcut30 [225,50]", itSignal, 1,   "0", "noWeight");
    //p->AddSample("T2tt_250_50_FS_summer",  "xqcut30 [250,50]", itSignal, 1,   "0", "noWeight");
    p->AddSample("T2tt_175_1_FS_xqcut20",  "xqcut20 [175,1]",  itSignal, kBlue+1, "0", "noWeight");
    //p->AddSample("T2tt_200_50_FS_xqcut20", "xqcut20 [200,50]", itSignal, kBlue+1,   "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_xqcut20", "xqcut20 [225,50]", itSignal, kGreen+1,   "0", "noWeight");
    //p->AddSample("T2tt_250_50_FS_xqcut20", "xqcut20 [250,50]", itSignal, kViolet-1,   "0", "noWeight");
    p->SetLineStyle("xqcut20 [175,1]", 2);
    //p->SetLineStyle("xqcut20 [200,50]", 2);
    //p->SetLineStyle("xqcut20 [225,50]", 2);
    //p->SetLineStyle("xqcut20 [250,50]", 2);
  } 

  //>>> Gen vs Reco comparisons
  else if(lab.Contains("GenReco")){
    p->AddSample("T2tt_175_1_FS",         "T2tt [175,1] Gen",  itSignal, kRed+1, "0", "noWeight");
    Plot* r = p->NewPlot(var.ReplaceAll("gen", ""), dilep);
    r->AddSample("TTbar_Powheg", "tt Powheg Reco", itSignal, 1,    "0", "noWeight");
    p->AddToHistos(r->GetHisto("tt Powheg Reco"));
    r->AddSample("T2tt_175_1_FS", "T2tt [175,1] Reco", itSignal, kRed+1,   "0", "noWeight");
    p->AddToHistos(r->GetHisto("T2tt [175,1] Reco"));
    p->SetLineStyle("tt Powheg Reco", 2);
    p->SetLineStyle("T2tt [175,1] Reco", 2);
    p->SetRatioMax(2);
    p->SetRatioMin(0);
  }

  //>>> Fast vs Full simpulations
  else if(lab.Contains("FastFull")){
    //p->AddSample("TTJets_MLM",             "tt MLM",           itSignal, kGray+1,    "0");
    //p->AddSample("T2tt_175_1_FS",          "FullSim [175,1]",  itSignal, kBlue+1,    "0");
    //p->AddSample("T2tt_FastSim_175_1",    "FastSim [175,1]",  itSignal, kBlue+1,   "0");
    p->AddSample("T2tt_200_50_FS_summer",  "FullSim [200,50]", itSignal, kBlue+1,     "0");
    p->SetWeight("TWeight*TBtagSFFS");
    p->AddSample("T2tt_mStop200_mLsp50",   "FastSim [200,50]", itSignal, kBlue+1, "0");
    p->SetWeight("TWeight");
    p->AddSample("T2tt_225_50_FS_summer",  "FullSim [225,50]", itSignal, kRed+1,     "0");
    p->SetWeight("TWeight*TBtagSFFS");
    p->AddSample("T2tt_mStop225_mLsp50",   "FastSim [225,50]", itSignal, kRed+1, "0");
    p->SetWeight("TWeight");
    p->AddSample("T2tt_250_50_FS_summer",  "FullSim [250,50]", itSignal, kGreen+1,     "0");
    p->SetWeight("TWeight*TBtagSFFS");
    p->AddSample("T2tt_mStop250_mLsp50",   "FastSim [250,50]", itSignal, kGreen+1, "0");
    p->SetWeight("TWeight");
    p->SetLineStyle("FastSim [200,50]", 2);
    p->SetLineStyle("FastSim [225,50]", 2);
    p->SetLineStyle("FastSim [250,50]", 2);
  }

  //>>> Different masses of stop and LSP particles
  else if(lab.Contains("StopMass")){
    p->AddSample("TTJets_MLM",             "tt MLM",            itSignal, kGray+1,    "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",         "T2tt [175,1]" , itSignal, kViolet+2,   "0", "noWeight");
    p->AddSample("T2tt_200_50_FS_summer", "T2tt [200,50]", itSignal, kGreen+2,   "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kRed+1,   "0", "noWeight");
    p->AddSample("T2tt_250_50_FS_summer", "T2tt [250,50]", itSignal, kBlue+2,   "0", "noWeight");
//    p->AddSample("T2tt_175_1_FS",          "FullSim [175,1]",   itSignal, kBlue+1,    "0");
//    p->AddSample("T2tt_mStop175_mLsp10",   "FastSim [175,10]",  itSignal, kGreen+1,   "0");
//    p->AddSample("T2tt_mStop175_mLsp20",   "FastSim [175,20]",  itSignal, kMagenta+1,   "0");
//    p->AddSample("T2tt_mStop190_mLsp1",    "FastSim [190,1]",   itSignal, kGreen+2,   "0");
//    p->AddSample("T2tt_mStop190_mLsp10",   "FastSim [190,10]",  itSignal, kBlue+1,   "0");
//    p->AddSample("T2tt_mStop190_mLsp20",   "FastSim [190,20]",  itSignal, kRed+1,   "0");
//    p->AddSample("T2tt_mStop192_mLsp25",   "FastSim [192,25]",  itSignal, kViolet-1,   "0");
  }

  //p->AddSample("DT2tt_mStop175_mLsp1", "stop [175,1]", itSignal, kBlue+1, "0", "noWeight");
 // p->AddSample("T2tt_175_1_FS", "FullSim [175,1]", itSignal, kGreen+1, "0", "noWeight");
  //p->AddSample("T2tt_175_1_FS_noFSSF", "FullSim noSF [175,1]", itSignal, kGreen+1);
 // p->AddSample("T2tt_250_50_FS_xqcut20", "FullSim [250,50] xqcut20", itSignal, kBlue+1, "0", "noWeight");
 // p->AddSample("T2tt_250_50_FS_summer", " FullSim [250,50] xqcut30", itSignal, kRed+1,  "0", "noWeight");

 //>>> Options and drawing...
  TString options;
  if(lab.Contains("FastFull"))      options = "hist,ratiocolors";
  else if(lab.Contains("StopMass")){
    options = "hist";
    p->GetHisto("tt Powheg Gen")->SetDrawStyle("pe");
  }
  options = "hist";

  p->SetLegendPosition(0.6, 0.72, 0.92, 0.92);
  if(var == "TgenDeltaPhi")  p->SetLegendPosition(0.2, 0.72, 0.62, 0.92);
  p->SetLegendTextSize(0.04);
  Float_t rMax = 1.2; Float_t rMin = 0.8;/*
  if     (var == "TgenLep0_Pt" || var == "TgenLep1_Pt") rMax = 1.2;
  else if(var == "TgenDeltaPhi") rMax = 1.2; 
  else if(var == "TgenDeltaEta") rMax = 1.4; 
  else if(var == "TgenMET"     ) rMax = 2.0;
  else if(var == "TgenMT2"     ) rMax = 2.0;
  else if(var.BeginsWith("GetDilepPtDPhi")) rMax = 1.3;
  else if(var == "TgenMET"     ) rMax = 2.0;*/

  rMin = 2 - rMax;
  p->SetRatioMin(rMin); p->SetRatioMax(rMax);

  p->doSetLogy = false;
  p->DrawComp(name, 1,options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 1,options);

  delete p;
}

