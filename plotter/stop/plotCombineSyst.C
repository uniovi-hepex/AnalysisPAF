R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Plot.h"

TString path = "/nfs/fanae/user/juanr/CMSSW_8_1_0/src/StopDatacards/apr2018/";
TString filename = "MT2_ElMu.root";

TString Sys = "ue, isr, hdamp, ME, PDF, JES, Btag, MisTag, MuonEff, ElecEff, PU, JER, Trig, CR, fsr, topMass, MuScale, ElScale, TopPt, SignalISR, UnclMET, SignalISR,SignalScale";

void DrawSystCom(TString syst, TString process = "ttbar"){
  if(!ExistsHistoInFile(path + filename, process)) return;
  if(!ExistsHistoInFile(path + filename, process+"_"+syst+"Up") && !ExistsHistoInFile(path + filename, process+"_"+syst+"Down")) return;
  Plot* p = new Plot("TMT2", "", "ElMu", 21, 0, 105, "Title", "M_{T2} (GeV)");
  p->SetLegendTextSize(0.050);
  p->SetLegendPosition(0.75, 0.60, 0.93, 0.90);

  p->doYieldsInLeg = false;
  p->doLegend = true;
  p->SetRatioMin(0.97); p->SetRatioMax(1.03);

  p->AddHistoFromFile(path + filename, process, itSignal, 1);
  p->AddHistoFromFile(path + filename, process+"_"+syst+"Down", itSignal, kRed+1);
  p->AddHistoFromFile(path + filename, process+"_"+syst+"Up", itSignal, kAzure+1);
  p->SetPadPlotLimits( "0.0, 0.56, 1.0, 1.0");
  p->SetPadRatioLimits("0.0, 0.0,  1.0, 0.59");
  //p->SetPadPlotMargins( t);
  p->SetPadRatioMargins("0.03, 0.2, 0.02, 0.1");
  p->SetYaxisOffset(0.67, 0.06);
  p->SetXaxisLabelSize(0.07);
  p->SetXaxisTitleSize(0.08);
  p->SetPlotMinimum(0);
  p->SetTitleY("Var/Nom", 0.07);
  p->SetYaxisDivisions(510);
  cout << Form("[Nom: %1.0f] [Up: %1.0f] [Down: %1.0f] \n", p->GetHisto(process)->GetYield(), p->GetHisto(process+"_"+syst+"Up")->GetYield(), p->GetHisto(process+"_"+syst+"Down")->GetYield());

  p->SetPlotFolder("/nfs/fanae/user/juanr/www/stop/apr2018/Systematics/" + process + "/");
  p->SetVarName("");
  p->SetOutputName(process+"_"+syst);
  p->doSetLogy = false;
  p->DrawComp("",0, "hist");
  delete p;
}

void plotCombineSyst(TString process = "ttbar", TString syst = ""){
  vector<TString> v = TStringToVector(Sys);
  if(syst != ""){
    DrawSystCom(syst, process);
    return;
  }
  for(int i = 0; i < (int) v.size(); i++){
    syst = v.at(i);
    DrawSystCom(v.at(i), process);
  }
}
