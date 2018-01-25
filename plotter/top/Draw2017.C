R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

TString pathToTree  = "/pool/ciencias/userstorage/juanr/Data2017/dec5/";
TString outFolder   = "/nfs/fanae/user/juanr/www/Data2017/dec5/";
TString NameOfTree  = "MiniTree";

TString dilepton = "!TIsSS";
TString jets     = "!TIsSS && TNJets >= 2";
TString btag     = "!TIsSS && TNJets >= 2 && TNBtags >= 1";

TString data2016   = "MuonEG_Run2016GH";
TString ReReco2017 = "MuonEG_RunBC_12Sep2017";
TString RunDE2017  = "MuonEG_RunD, MuonEG_RunE";
TString RunF2017   = "MuonEG_RunF";

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name);

void Draw2017(TString cut = ""){
  if(cut == ""){
    Draw2017(dilepton);
    //Draw2017(jets);
    //Draw2017(btag);
    return;
  }
  // for(Int_t i = 0; i < 9; i++){
  DrawPlot("TNBtags", cut + " && TNVert <= 20", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_b20");
  DrawPlot("TNBtags", cut + " && TNVert <= 25 && TNVert > 20", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_g20b25");
  DrawPlot("TNBtags", cut + " && TNVert <= 30 && TNVert > 25", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_g25b30");
  DrawPlot("TNBtags", cut + " && TNVert <= 35 && TNVert > 30", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_g30b35");
  DrawPlot("TNBtags", cut + " && TNVert <= 40 && TNVert > 35", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_g35b40");
  DrawPlot("TNBtags", cut + " && TNVert >  40", "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags_g40");
/*  DrawPlot("TNVert", cut, "ElMu", 60, 0, 60, "# PV", "npv");
  DrawPlot("TRun", cut, "ElMu", 50, 303435, 306434, "# Run", "run");
  DrawPlot("TNJets", cut, "ElMu", 8, 0-0.5, 8-0.5, "# Jets", "nJet");
  DrawPlot("TNBtags", cut, "ElMu", 5, 0-0.5, 5-0.5, "# b-tags", "nbtags");
  DrawPlot("NBtagNJets(TNJets,TNBtags)", cut, "ElMu", 16, 0, 15, "[#jets|#btags]", "NBtagsNJets");
  DrawPlot("THT", cut + " && THT > 0", "ElMu", 30, 0, 500, "H_{T} [GeV]", "HT");
  DrawPlot("TMET", cut, "ElMu", 30, 0, 300, "MET [GeV]", "MET");
  DrawPlot("TMET_Phi", cut, "ElMu",  20, -3.2, 3.2, "MET #varphi", "METphi");

  DrawPlot("TLep0Pt", cut + "*(abs(TLep0Id) == 13)", "ElMu", 15, 20, 200, "Leading Muon p_{T}", "MuonPt");
  DrawPlot("TLep0Pt", cut + "*(abs(TLep0Id) == 11)", "ElMu", 15, 20, 200, "Leading Elec p_{T}", "ElecPt");
  DrawPlot("TLep0Phi", cut + "*(abs(TLep0Id) == 13)", "ElMu",  20, -3.2, 3.2, "Leading Muon #varphi", "MuonPhi");
  DrawPlot("TLep0Phi", cut + "*(abs(TLep0Id) == 11)", "ElMu",  20, -3.2, 3.2, "Leading Elec #varphi", "ElecPhi");
  DrawPlot("TLep0Eta", cut + "*(abs(TLep0Id) == 13)", "ElMu", 15, -2.4, 2.4, "Leading Muon #eta", "MuonEta");
  DrawPlot("TLep0Eta", cut + "*(abs(TLep0Id) == 11)", "ElMu", 15, -2.4, 2.4, "Leading Elec #eta", "ElecEta");
  DrawPlot("TLep1Pt", cut + "*(abs(TLep1Id) == 13)", "ElMu", 15, 20, 200, "Muon p_{T}", "MuonPt");
  DrawPlot("TLep1Pt", cut + "*(abs(TLep1Id) == 11)", "ElMu", 15, 20, 200, "Elec p_{T}", "ElecPt");
  DrawPlot("TLep1Phi", cut + "*(abs(TLep1Id) == 13)", "ElMu",  20, -3.2, 3.2, "Muon #varphi", "MuonPhi");
  DrawPlot("TLep1Phi", cut + "*(abs(TLep1Id) == 11)", "ElMu",  20, -3.2, 3.2, "Elec #varphi", "ElecPhi");
  DrawPlot("TLep1Eta", cut + "*(abs(TLep1Id) == 13)", "ElMu", 15, -2.4, 2.4, "Muon #eta", "MuonEta");
  DrawPlot("TLep1Eta", cut + "*(abs(TLep1Id) == 11)", "ElMu", 15, -2.4, 2.4, "Elec #eta", "ElecEta");
  DrawPlot("TDilep_Pt", cut, "ElMu", 30, 0, 300, "p_{T}^{e#mu} [GeV]", "DilepPt");
  DrawPlot("TMll", cut, "ElMu", 30, 0, 300, "M_{e#mu} [GeV]", "InvMass");

  DrawPlot("TJet_Pt", cut + " && TNJets > 0", "ElMu", 15, 30, 300, "Leading jet p_{T}", "JetPt");
  DrawPlot("TJet_Eta", cut + " && TNJets > 0", "ElMu", 15, -2.4, 2.4, "Jet #eta", "JetEta");
  DrawPlot("TJet_Phi", cut + " && TNJets > 0", "ElMu", 15, -2.4, 2.4, "Jet #varphi", "JetPhi");
  DrawPlot("TJet_Csv", cut + " && TNJets > 0", "ElMu", 15, 0, 1, "Jet csv", "JetCsv");
  DrawPlot("TJet0Pt", cut + " && TNJets > 0", "ElMu", 15, 30, 300, "Leading jet p_{T}", "Jet0Pt");
  DrawPlot("TJet0Eta", cut + " && TNJets > 0", "ElMu", 15, -2.4, 2.4, "Leading Jet #eta", "Jet0Eta");
  DrawPlot("TJet0Csv", cut + " && TNJets > 0", "ElMu", 15, 0, 1, "Leading Jet csv", "Jet0csv");
  DrawPlot("TJet0Phi", cut + " && TNJets > 0", "ElMu",  20, -3.2, 3.2, "Leading Jet #varphi", "Jet0Phi");
  DrawPlot("TJet0Pt", cut + " && TNJets > 0" + " && TJet0IsBTag", "ElMu", 15, 30, 300, "Btag p_{T}", "BtagPt");
  DrawPlot("TJet0Eta", cut + " && TNJets > 0" + " && TJet0IsBTag", "ElMu", 15, -2.4, 2.4, "Btag #eta", "BtagEta");
  DrawPlot("TJet0Phi", cut + " && TNJets > 0" + " && TJet0IsBTag", "ElMu", 20, -3.2, 3.2, "Btag #varphi", "BtagPhi");
*/
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->doYieldsInLeg = false;
  p->doStackOverflow = false;
  Float_t RatioRange = 0.4;
  if(name == "NBtagsNJets") p->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  if(name == "npv") RatioRange = 1;

  TString options = ""; 
  if(name.BeginsWith("TJet_")) options = "AllInstances";

  // Scenario 1: 2016 Data G + H          | data2016
  // Scenario 2: 2017 B + C (Rereco)      | ReReco2017
  // Scenario 3: 2017 D + E (PromptReco)  | RunDE2017
  // Scenario 4: 2017 Run F               | RunF2017

  //p->AddSample(data2016,   "2016 GH",            itSignal, 1);
  //p->AddSample(ReReco2017, "2017 BC (ReReco)",   itSignal, kGreen+1);   
  p->AddSample(RunDE2017,  "2017 DE",            itSignal, kBlue+1);
  p->AddSample(RunF2017,   "2017 F",             itSignal, kRed+1);


  p->SetLegendPosition(0.62, 0.72, 0.9, 0.9);
  p->SetLegendTextSize(0.06);
  p->SetRatioYtitle("F/DE");

  if(name.Contains("Phi") || name.Contains("phi"))  p->SetLegendPosition(0.62, 0.42, 0.9, 0.6);
  if(name.Contains("npv")) p->SetLegendPosition(0.12, 0.72, 0.3, 0.9);
  //p->doLegend = false;

  p->SetPlotFolder(outFolder + "/Data2017_RunF_vs_RunDE/");
  p->doSetLogy = false;
  p->SetRatioMin(1-RatioRange);
  p->SetRatioMax(1+RatioRange);
  p->SetVarName(name);
  p->SetScaleMax(1.3);

  TString level = "dilepton";
  if(cut.BeginsWith(jets)) level = "2jets";
  if(cut.BeginsWith(btag)) level = "1btag";
  
  p->DrawComp(level, 1, 1);

  delete p;
}

