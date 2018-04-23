R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Plot.h"

TString path = "/nfs/fanae/user/juanr/CMSSW_8_1_0/src/StopDatacards/apr2018/";
TString filename = "MT2_ElMu.root";

const TString Sys = "ue, isr, hdamp, ME, PDF, JES, Btag, MisTag, MuonEff, ElecEff, PU, JER, Trig, CR, fsr, topMass, MuScale, ElScale, TopPt, SignalISR, SignalScale";
bool doPrintAll = true;

float GetNormUnc(float n, TString process);

void getUncertaintiesFromDatacard(TString process = "ttbar", TString syst = Sys){
  if(process.Contains(',')){
    vector<TString> vPr = TStringToVector(process);
    for(int i = 0; i < (int) vPr.size(); i++) getUncertaintiesFromDatacard(vPr.at(i), syst);
    return;
  }
  if(!ExistsHistoInFile(path + filename, process)) return;
  vector<TString> vSyst; 
  if(syst.Contains(",")) vSyst = TStringToVector(syst); else vSyst.push_back(syst); Int_t nSyst = vSyst.size();
  float u; float d; float y; float sum = 0; float tot; float dif = 0;

  Plot* p = new Plot("TMT2", "", "ElMu", 21, 0, 105, "Title", "M_{T2} (GeV)");
  p->AddHistoFromFile(path + filename, process, itSignal, 1);
  n = p->GetHisto(process)->GetYield();
  for(int i = 0; i < nSyst; i++){
    syst = vSyst.at(i);
    if(!ExistsHistoInFile(path + filename, process+"_"+syst+"Up") && !ExistsHistoInFile(path + filename, process+"_"+syst+"Down")) continue;
    n = p->GetHisto(process)->GetYield(); 
    p->AddHistoFromFile(path + filename, process+"_"+syst+"Down", itSignal, kRed+1);
    p->AddHistoFromFile(path + filename, process+"_"+syst+"Up", itSignal, kAzure+1);
    u = p->GetHisto(process+"_"+syst+"Up")->GetYield();
    d = p->GetHisto(process+"_"+syst+"Down")->GetYield();
    dif = TMath::Abs(n-d); if(TMath::Abs(n-u) > dif) dif = TMath::Abs(n-u);
    sum += dif*dif;
    if(doPrintAll) cout << Form(" >> [Syst: %s]: [n = %1.0f] [%1.0f, %1.0f] %1.0f  (%1.2f %s)\n", syst.Data(),n, u, d, dif, dif/n*100,"%");
  }
  sum += GetNormUnc(n,process)*GetNormUnc(n,process);
  tot = TMath::Sqrt(sum);
  cout << Form("%s: %1.0f +/- %1.0f (%1.2f %s)\n", process.Data(), n, tot, tot/n*100,"%");
}

float GetNormUnc(float n, TString process){
  float d = 0.6;
  if(process == "ttbar" || process == "t#bar{t}") d = 0.06;
  else if(process == "Nonprompt") d = 0.6;
  else if(process == "DY") d = 0.15;
  else if(process == "ttV" || process == "VV" || process == "tW") d = 0.3;
  else d = 0.15;
  return n*d;
}
