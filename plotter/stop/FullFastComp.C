//###################################################################################################
//
// Draw FullSim/FastSim comparisons and yields
//
//###################################################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"



//=== Functions
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "0", TString lab = "");

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
TString NameOfTree = "tree";
TString chan = "ElMu";
TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 

//=== The main function
void GenLevelPlots(TString lab = ""){

  //>>> gen comparisons
  if(lab.Contains("gen")){ 
    DrawPlot("TgenLep0_Pt",  dilepgen, chan, 15, 0, 200, "gen Leading lep p_T [GeV]",    "genLep0Pt", lab);
    DrawPlot("TgenLep1_Pt",  dilepgen, chan, 15, 0, 200, "gen Subleading lep p_T [GeV]", "genLep1Pt", lab);
    DrawPlot("TgenLep0_Pt",  dilepgen + " && TgenLep0_Id == 11", chan, 15, 0, 200, "gen Elec p_T [GeV]", "genElec0Pt", lab);
    DrawPlot("TgenLep0_Pt",  dilepgen + " && TgenLep0_Id == 13", chan, 15, 0, 200, "gen Muon p_T [GeV]", "genMuonPt", lab);
    DrawPlot("TgenMET",      dilepgen, chan, 20, 0, 250, "gen MET [GeV]",                "genMET", lab);
    DrawPlot("GetDilepPtDPhi(TgenLep0_Pt, TgenLep1_Pt, TgenDeltaPhi)",      dilepgen, chan, 30, 0, 300, "gen p_{T}^{#font[12]{ll}} [GeV]",                "genDielpPt", lab);
    DrawPlot("TgenDeltaPhi", dilepgen, chan, 15, 0, 3.2, "gen Delta Phi",                "gDeltaPhi", lab);
    DrawPlot("TgenDeltaEta", dilepgen, chan, 15, 0, 3.2, "gen Delta Eta",                "gDeltaEta", lab);
    DrawPlot("TMath::Abs(TgenMETPhi)",   dilepgen, chan, 15, 0, 3.2, "gen MET Phi",                  "genMETPhi", lab);
    DrawPlot("TgenMT2",      dilepgen, chan, 20, 0, 120, "gen M_{T2} [GeV]",             "genMT2", lab);
    DrawPlot("TgenMT2",      dilepgen + " && TgenMT2 > 0", "ElMu", 20, 0, 120, "gen M_{T2} [GeV]", "genMT2_no0", lab);
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
    DrawPlot("TLep0_Pt",  dilep + " && TLep0_Id == 11", "ElMu", 15, 0, 200, "Elec p_T [GeV]", "ElecPt", lab);
    DrawPlot("TLep0_Pt",  dilep + " && TLep0_Id == 13", "ElMu", 15, 0, 200, "Muon p_T [GeV]", "MuonPt", lab);
    DrawPlot("TLep0_Pt",  dilep, "ElMu", 15, 0, 200, "Leading lep p_T [GeV]",    "Lep0Pt", lab);
    DrawPlot("TLep1_Pt",  dilep, "ElMu", 15, 0, 200, "Subleading lep p_T [GeV]", "Lep1Pt", lab);
    DrawPlot("TDeltaPhi", dilep, "ElMu", 15, 0, 3.2, "Delta Phi", "DeltaPhi", lab);
    DrawPlot("TDeltaEta", dilep, "ElMu", 15, 0, 3.2, "Delta Eta", "DeltaEta", lab);
    DrawPlot("DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",    
                          dilep, "ElMu", 30, 0, 300, "p_{T}^{#font[12]{ll}} [GeV]",  "DilepPt", lab);
    DrawPlot("TMll",      dilep, "ElMu", 20, 0, 200, "Mll [GeV]", "InvMass", lab);
    DrawPlot("TMET",      dilep, "ElMu", 20, 0, 250, "MET [GeV]", "MET", lab);
    DrawPlot("TMath::Abs(TMET_Phi)",  dilep, "ElMu", 15, 0, 3.2, "MET Phi", "METPhi", lab);
    DrawPlot("TMT2",      dilep, "ElMu", 20, 0, 120, "M_{T2} [GeV]", "MT2", lab);
    DrawPlot("TMT2",      dilep + " && TMT2 > 0", "ElMu", 20, 0, 120, "M_{T2} [GeV]", "MT2_no0", lab);
  }
  else{
    GenLevelPlots("genModelingT2tt");
    GenLevelPlots("recoModelingT2tt");
    GenLevelPlots("topModelingT2tt");
    GenLevelPlots("genxqcutSimple");
    GenLevelPlots("recoxqcut");
    GenLevelPlots("topxqcut");
    GenLevelPlots("recoFastFull");
    GenLevelPlots("genStopMass");
    GenLevelPlots("recoStopMass");
    GenLevelPlots("topStopMass");
  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString lab){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPlotFolder("output/genComp/" + lab + "/");
  p->verbose = false;
  p->doYieldsInLeg = false;
  p->SetVarName(name);

  //>>> This sample is always on the comparison
  p->AddSample("TTbar_Powheg", "tt Powheg Gen", itSignal, 1,       "0", "noWeight");

  //>>> Modeling of T2tt processes
  if(lab.Contains("ModelingT2tt")){
    p->AddSample("TTJets_MLM",            "tt MLM",        itSignal, kGray+1,  "0", "noWeight");
    p->AddSample("T2tt_175_1_FS",         "T2tt [175,1]",  itSignal, kGreen+1, "0", "noWeight");
    p->AddSample("T2tt_225_50_FS_summer", "T2tt [225,50]", itSignal, kRed+1,   "0", "noWeight");
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
    p->AddSample("T2tt_FastSim_200_50",   "FastSim [200,50]", itSignal, kBlue+1, "0");
    p->AddSample("T2tt_225_50_FS_summer",  "FullSim [225,50]", itSignal, kRed+1,     "0");
    p->AddSample("T2tt_FastSim_225_50",   "FastSim [225,50]", itSignal, kRed+1, "0");
    p->AddSample("T2tt_250_50_FS_summer",  "FullSim [250,50]", itSignal, kGreen+1,     "0");
    p->AddSample("T2tt_FastSim_250_50",   "FastSim [250,50]", itSignal, kGreen+1, "0");
    p->SetLineStyle("FastSim [200,50]", 2);
    p->SetLineStyle("FastSim [225,50]", 2);
    p->SetLineStyle("FastSim [250,50]", 2);
  }

  //>>> Different masses of stop and LSP particles
  else if(lab.Contains("StopMass")){
    p->AddSample("TTJets_MLM",             "tt MLM",            itSignal, kGray+1,    "0");
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
  if(lab.Contains("FastFull")) options = "hist,ratiocolors";
  p->SetLegendPosition(0.6, 0.75, 0.92, 0.92);
  p->SetLegendTextSize(0.04);
  p->SetRatioMin(0.6); p->SetRatioMax(1.4);
  p->doSetLogy = false;
  p->DrawComp(name, 1,options);
  p->doSetLogy = true;
  p->SetPlotMinimum(5e-4);
  p->SetPlotMaximum(0.5);
  p->DrawComp("log", 1,options);

  delete p;
}

