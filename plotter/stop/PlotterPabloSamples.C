R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Plot.h"
TString pathTrees = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
TString NameOfTree = "tree";
TString dilepgen    = "TNgenLeps == 2 && fabs(TgenLep0_Eta) <= 2.4 && fabs(TgenLep1_Eta) <= 2.4";// && TgenMT2 > 0";

TString outpath = "./temp/";
TString path = "/pool/ciencias/userstorage/juanr/stop/pablo/";
TString basecut = "ptlep1 > 20 && ptlep2 > 20 && fabs(etalep1) <= 2.4 && fabs(etalep2) <= 2.4";// && mt2 > 0";

Plot* p = new Plot("TgenLep0_Pt", dilepgen, "1", 20, 0, 200, "Title", "Lep0 p_{T} (GeV)");
//Plot* p = new Plot("TgenMT2", dilepgen, "1", 21, 0, 105, "Title", "M_{T2} (GeV)");

Histo* GetHistoFromPabloTree(TString file, TString var = "mt2", TString cut = "ptlep1 > 20 && ptlep2 > 20 && fabs(etalep1) <= 2.4 && fabs(etalep2) <= 2.4 && mt2 > 0", int nbins = 21, float bin0 = 0, float binN = 105);

Histo* GetHistoFromPabloTree(TString file, TString var, TString cut, int nbins, float bin0, float binN){
  if(!file.EndsWith(".root")) file+=".root";
  TFile* f = TFile::Open(path + file);
  TString name = file(0,file.First(".root"));

  TTree* tree;
  tree = (TTree*) f->Get("events");
  Histo* h = new Histo(name, name, nbins, bin0, binN);
  tree->Draw(var + ">>" + name, cut); 

  //Histo* h = new Histo(*hf);
  h->SetProcess(name);
  h->SetDirectory(0);

  // Add overflow, etc
  h->SetStyle();
  
  // Norm
  h->Scale(1/h->Integral());

  delete f;
  return h;
}

void PlotterPabloSamples(){

  // Add MADGRAPH sample
  p->SetPath(pathTrees);
  p->SetTreeName(NameOfTree);
  p->SetWeight("1");
  //p->AddSample("TTJets_MLM", "t#bar{t} Madgraph", itSignal, 1, "noWeight");
  p->AddSample("T2tt_225_50_FS_summer", "Signal(225,50) xqcut20_3J", itSignal, 1, "noWeight");

//  p->PrepareHisto(GetHistoFromPabloTree("ttbar172p5"), "ttbar172p5", "t#bar{t} m_{t} = 172.5", itSignal, kGreen-2);
//  p->PrepareHisto(GetHistoFromPabloTree("ttbar171p5"), "ttbar171p5", "t#bar{t} m_{t} = 171.5", itSignal, kAzure+5);
//  p->PrepareHisto(GetHistoFromPabloTree("ttbar173p5"), "ttbar173p5", "t#bar{t} m_{t} = 173.5", itSignal, kRed+1);
  p->PrepareHisto(GetHistoFromPabloTree("point225_50_172p5", "ptlep1", basecut, 20, 0, 200), "point225_50_172p5", "Signal(225,50) toy", itSignal, kRed+1);

  //############################## Cosmetics
  //###########################################
  // Legend
  p->doYieldsInLeg = false; p->doLegend = true;
  p->SetLegendTextSize(0.050);
  p->SetLegendPosition(0.60, 0.75, 0.93, 0.90);
  p->SetCMSmodeLabel("");

  // Ratio plot
  p->SetRatioErrorColor(kTeal-2); p->SetRatioErrorStyle(3244); p->SetStackErrorStyle(3244);
  p->SetRatioMin(0.8); p->SetRatioMax(1.2);

  //###########################################
  // Save
  p->doSetLogy = false;
  p->DrawComp("", 1);
  
}
