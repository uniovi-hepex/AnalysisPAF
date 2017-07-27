R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString pl = "", TString nplot = "");
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/ElMu/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/may10/";
TString NameOfTree = "MiniTree";
TString bjet1 = "TNBtags > 0 && !TIsSS";
TString dilep = "!TIsSS";
TString VarNBtagsNJets = "TNBtags + (TNJets == 1) + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";

//TString nplot = "FSR";

void PlotSystematics(TString nplot = ""){
 // for(Int_t i = 0; i < 9; i++){
//  DrawPlot("TNJets", dilep, "ElMu", 9, 0, 9, "Jet Multiplicity", "NJet", nplot);
//  DrawPlot(VarNBtagsNJets, "!TIsSS", "ElMu", 15, 0, 15, "(#Jet, #Btag)", "NBtagsNJets", nplot);
  DrawPlot("TMET", "!TIsSS && TNJets >= 2 && TNBtags >= 1", "ElMu", 40, 0, 400, "MET [GeV]", "MET", nplot);
//  DrawPlot("TNBtags", "!TIsSS", "ElMu", 5, 0, 5, "B-tag multiplicity", "Btags", nplot);
//  DrawPlot("TNBtags", "!TIsSS && TNJets >= 2", "ElMu", 5, 0, 5, "B-tag multiplicity", "Btags_2jets", nplot);
//  }
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString pl, TString nplot){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = false;
  p->doYieldsInLeg = false;
  p->SetRatioMax(1.03);
  p->SetRatioMin(0.97);
  if(pl != "")  p->SetVarName(pl);

  p->SetPlotFolder("Syst/" + nplot + "/");

  p->AddSample("TTbar_Powheg", "Nominal", itSignal, 1, 0.05);

  if(nplot == "FSR"){
    p->AddSample("TTbar_Powheg_fsrUp", nplot + " Up", itSignal, kRed+1, 0.05 );
    p->AddSample("TTbar_Powheg_fsrDown", nplot + " Down", itSignal, kBlue+1, 0.05);
  }
  if(nplot == "ISR"){
    p->AddSample("TTbar_Powheg_isrUp", nplot + " Up", itSignal, kRed+1, 0.05 );
    p->AddSample("TTbar_Powheg_isrDown", nplot + " Down", itSignal, kBlue+1, 0.05);
  }
  if(nplot == "ScaleME"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "ScaleUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "ScaleDown");
  }
  if(nplot == "Btag"){
    p->AddSample("TTbar_Powheg", "Btag Up", itSignal, kBlue+1, 0.05, "BtagUp");
    p->AddSample("TTbar_Powheg", "Btag Down", itSignal, kGreen+1, 0.05, "BtagDown");
    p->AddSample("TTbar_Powheg", "Mistag Up", itSignal, kViolet+1, 0.05, "MisTagUp");
    p->AddSample("TTbar_Powheg", "Mistag Down", itSignal, kRed+1, 0.05, "MisTagDown");
  }
  if(nplot == "hdamp"){
    p->AddSample("TTbar_Powheg_hdampDown", nplot + " Up", itSignal, kRed+1, 0.05, "hdampUp");
    p->AddSample("TTbar_Powheg_hdampUp",   nplot + " Down", itSignal, kBlue+1, 0.05, "hdampDown");
  }
  if(nplot == "Elec"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "ElecEffUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "ElecEffDown");
  }
  if(nplot == "Muon"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "MuonEffUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "MuonEffDown");
  }
  if(nplot == "Trig"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "TrigUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "TrigDown");
  }
  if(nplot == "JES"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "JESUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "JESDown");
  }
  if(nplot == "PU"){
    p->AddSample("TTbar_Powheg", nplot + " Up", itSignal, kRed+1, 0.05, "PUUp");
    p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "PUDown");
  }
  if(nplot == "PDF"){
    p->AddSample("TTbar_Powheg", nplot + " Up",   itSignal, kRed+1,  0.05, "PDFUp");
    //p->AddSample("TTbar_Powheg", nplot + " Down", itSignal, kBlue+1, 0.05, "Down");
  }
  if(nplot == "NLO"){
    p->AddSample("TTJets_aMCatNLO", "aMCatNLO",   itSignal, kRed+1,  0.05, "");
  }
  if(nplot == "Had"){
    p->AddSample("TTbar_Powheg_Herwig", "TTbar_Powheg_Herwig",   itSignal, kRed+1,  0.05, "");
  }


  p->DrawComp(nplot,           1, 0);
  p->DrawComp(nplot + "_norm", 1, 1);

  //p->PrintYields();
  //p->PrintSystYields();
  delete p;
}

