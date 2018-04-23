R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Plot.h"

TString path = "/nfs/fanae/user/juanr/CMSSW_8_1_0/src/StopDatacards/preapproval/";
TString outpath = "/nfs/fanae/user/juanr/CMSSW_8_1_0/src/StopDatacards/preapproval/";
TString filename = "fitDiagnostics.root";
TString signals = "182p5_7p5, 205_30, 242p5_67p5";

void plotCombine(TString name = "", Bool_t setlogy = true, TString signal = ""){
  if(name == ""){
    vector<TString> vsignals;
    if(signal == "")  vsignals = TStringToVector(signals);
    //else vsignals.push_back("");
    TString sig = ""; int nsingals = vsignals.size();
    for(int i = 0; i < nsingals; i++){ 

      sig = vsignals.at(i);
      plotCombine("prefit",1,sig);
      plotCombine("fit_b",1,sig);
      plotCombine("fit_s",1,sig);
      plotCombine("prefit", 0,sig);
      plotCombine("fit_b", 0,sig);
      plotCombine("fit_s", 0,sig);
    }
      return;
  }
  TString pathInRootfile = "shapes_" + name + "/ElMu/";
  TString outname = name;
  if(signal != ""){
    filename = "fitDiagnostics_" + signal + ".root";
    //filename = "fitDiagnostics_rMin0_" + signal + ".root";
    outname = name + "_" + signal;
  }

  Plot* p = new Plot("TMT2", "", "ElMu", 21, 0, 105, "Title", "M_{T2} (GeV)");

  p->SetLegendTextSize(0.050);
  p->SetLegendPosition(0.75, 0.60, 0.93, 0.90);

  p->SetRatioErrorColor(kTeal-2);
  p->SetRatioErrorStyle(3244);
  p->SetStackErrorStyle(3244);
  p->doYieldsInLeg = false;
  p->doLegend = true;
  p->doSignal = true;
  if(name == "fit_b" || name == "fit_s") p->doSignal = false;
  p->SetSignalStyle("H");
  p->SetRatioMin(0.8); p->SetRatioMax(1.2);

  p->AddHistoFromFile(path + filename, "VV", itBkg, kSpring-2, "", pathInRootfile);
  p->AddHistoFromFile(path + filename, "ttV",itBkg, kSpring-2, "", pathInRootfile);
  p->AddHistoFromFile(path + filename, "DY", itBkg, kSpring-2, "", pathInRootfile);
  p->AddHistoFromFile(path + filename, "Nonprompt", itBkg, kSpring-2, "", pathInRootfile);
  p->AddHistoFromFile(path + filename, "tW", itBkg, kViolet-5, "", pathInRootfile);
  p->AddHistoFromFile(path + filename, "ttbar", itBkg, kRed+1, "", pathInRootfile);

  int s = itSignal;
  if(name == "fit_s") s = itBkg; 
  p->AddHistoFromFile(path + filename, "total_signal", s, kAzure+8, "", pathInRootfile);
  p->RenameProcess("total_signal", "Signal+Bkg"); 
  //if(name == "fit_s") p->GroupProcesses("Signal+Bkg", "ttbar");
  //TString SignalText = "M_{#tilde{t}_{1}} = 227.5 GeV, M_{#tilde{#Chi}_{1}^{0}} = 52.5 GeV";

  p->GroupProcesses("VV, ttV, DY, Nonprompt", "Other");
  p->AddHistoFromFileTGraph(path + filename, "data", itData, kRed+1, "", pathInRootfile);

  p->AddHistoFromFile(path+filename, "total_background", itOther, 0, "", pathInRootfile);
  p->SetHistoTotalSyst(p->GetHisto("total_background"));

  p->SetPlotFolder(outpath);
  //  p->AddHistoFromFile(path + filename, "DY", 0, kAzure+1, "", pathInRootfile);
  if(setlogy){
    p->SetOutputName(outname+"_log");
    p->SetPlotMinimum(10);
    p->doSetLogy = true;
    p->DrawStack();
  }
  else{
    p->SetOutputName(outname);
    p->SetPlotMinimum(0);
    //p->SetPlotMaximum(1e5);
    p->doSetLogy = false;
    p->DrawStack();
  }
  delete p;
}


