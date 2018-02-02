R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
#include "Histo.h"
#include "Looper.h"

TString path = "/pool/ciencias/HeppyTreesFall17/";
TString samples = "MuonEG_Run2017B_12Sep2017_v1, MuonEG_Run2017C_PromptReco_v1, MuonEG_Run2017C_PromptReco_v2, MuonEG_Run2017C_PromptReco_v3, MuonEG_Run2017D_PromptReco_v1, MuonEG_Run2017E_PromptReco_v1";
TString var = "run";
TString cut = "";
TString hName = "Run number";
Int_t nbins = 200;
Float_t bin0 = 296000;
Float_t binN = 303434;

void Run2017Distribution(){
  Histo* h = GetHistoFromHeppyTrees(path, samples, var, cut, hName, nbins, bin0, binN);
  TCanvas* c= new TCanvas("c","c",10,10,800,600);
  //h->SetLineColor(0); h->SetFillColor(kBlack);
  h->Draw();
  c->Print("RunNumberDist.png", "png");
  c->Print("RunNumberDist.pdf", "pdf");
}
