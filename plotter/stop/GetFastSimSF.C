//###############################################################################
//
// Produce FullSim/FastSim SF
// Usage:
//   root -l -b- 1 'GetFastSimSF("Elec")' 
//
//###############################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

//=== Constants
const TString path = "/nfs/fanae/user/juanr/AnalysisPAF/StopTop_temp/";
const TString treeName = "tree";
const TString FullSimSampleName = "TTJets_MLM";
const TString FastSimSampleName = "TTJets_MLM_FastSim";
const TString outputPath = "output/";
const TString outputName = "FullSimFastSim";

const TString cut  = "TPassTrigger";
const TString chan = "ElMu";
Histo* hFullSimPt[netabins];
Histo* hFastSimPt[netabins];
TH2D*  SF;

//=== Binning
const Int_t netabins = 3;
const Int_t nptbins  = 4;
const Float_t etabinsElec[netabins+1] = {0, 1.442, 1.556, 2.4};
const Float_t etabinsMuon[netabins+1] = {0, 1.2,   2.1,   2.4};
const Float_t ptbins[nptbins+1]   = {20, 30, 40, 50, 200};

//=== Options
Bool_t doErrors = true;

//=== Functions
Histo* GetPtHisto(TString samplename, Float_t etamin, Float_t etamax, TString flav);
void SetSFmap(TString flav);

//=== Main function
void GetFastSimSF(TString obj){ 
  if(obj != "Elec" && obj != "Muon"){
    cout << "WARNING: Invalid object name!" << endl;
    return;
  }
  Float_t *etabins = (Float_t*) &etabinsMuon[0]; if(obj == "Elec") etabins = (Float_t*) &etabinsElec[0];

  //>>> Verbose
  cout << "=========== Printing FullSim/FastSim SF for " << obj << endl;
  cout << ">> Path = " << path << endl;
  cout << ">> FullSim histo name = " << FullSimSampleName << endl;
  cout << ">> FastSim histo name = " << FastSimSampleName << endl;
  cout << ">> x bins (pt)  = "; PrintArray((Float_t*) ptbins, nptbins+1);
  cout << ">> y bins (eta) = "; PrintArray((Float_t*) etabins, netabins+1);


  //>>> Getting all the pt histos...
  cout << "   --> Computing pT histograms for FullSim and FastSim samples..." << endl;
  for(Int_t i = 0; i < netabins; i++){
    hFullSimPt[i] = GetPtHisto(FullSimSampleName, etabins[i], etabins[i+1], obj);
    hFastSimPt[i] = GetPtHisto(FastSimSampleName, etabins[i], etabins[i+1], obj);
  }

  //>>> Create and set the 2D map
  cout << "   --> Creating 2D map..." << endl;
  SetSFmap(obj); 
  SF->GetXaxis()->SetTitle(obj + "p_{T}");
  SF->GetYaxis()->SetTitle(obj + "|#eta|");

  //>>> Create output names, rootfile, canvas
  TString plotname = outputName + "_" + obj; 
  TFile* f = new TFile(outputPath + plotname +".root", "RECREATE");
  TCanvas *c = new TCanvas("c","c",10,10,800,600);

  //>>> Style and drawing options
  SF->SetStats(0);
  SF->SetMinimum(0.85); SF->SetMaximum(1.00);
  if(doErrors) SF->Draw("colz, text, errors");
  else         SF->Draw("colz, text");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("1.2f");
  
  //>>> Saving...
  gSystem->mkdir(outputPath, kTRUE);
  cout << "   --> Saving as: " << plotname << ".png and .pdf... Maps saved in a .root" << endl;
  SF->Write(); c->Write();
  c->Print( outputPath + plotname + ".png", "png");
  c->Print( outputPath + plotname + ".pdf", "pdf");
  delete c; 
}

Histo* GetPtHisto(TString samplename, Float_t etamin, Float_t etamax, TString flav){
  Int_t id = 11; if(flav == "Muon") id = 13;
  TString tcut = "(" + cut + Form(")*PassEtaCut(TLep0_Eta, TLep1_Eta, TLep0_Id, TLep1_Id, %f, %f, %i)", etamin, etamax,id); 
  TString var = Form("GetPt(TLep0_Pt, TLep1_Pt, TLep0_Id, TLep1_Id, %i)", id);
  Plot* p = new Plot(var, tcut, chan, (Int_t) nptbins, (Float_t*) ptbins, flav, flav + " p_{T}");
  p->SetPath(path); p->SetTreeName(treeName); p->verbose = false;
  p->AddSample(samplename);
  Histo* h = p->GetHisto(samplename)->CloneHisto(Form("samplename_%1.1f_%1.1f",etamin, etamax));
  h->SetDirectory(0);
  delete p;
  return h;
}

void SetSFmap(TString flav){
  Float_t *etabins = (Float_t*) &etabinsMuon[0]; if(flav == "Elec") etabins = (Float_t*) &etabinsElec[0];
  SF = new TH2D("SF", "SF", nptbins, ptbins, netabins, etabins);
  Int_t bin;
  Float_t num; Float_t den; Float_t binerror;
  Float_t numerr; Float_t denerr; 
  for(Int_t x = 1; x <= nptbins; x++){
    for(Int_t y = 1; y <= netabins; y++){
      bin    = SF->GetBin(x, y, 1);
      num    = hFullSimPt[y-1]->GetBinContent(x);
      den    = hFastSimPt[y-1]->GetBinContent(x);
      numerr = hFullSimPt[y-1]->GetBinError(x);
      denerr = hFastSimPt[y-1]->GetBinError(x);
      binerror = num/den*TMath::Sqrt((denerr/den)*(denerr/den) + (numerr/num)*(numerr/num));
      //cout << "[num, dem] -- [" << x << ", " << y << "] -- [" << num << ", " << den << "]" << endl;
      SF->SetBinContent(bin, num/den);
      SF->SetBinError(bin, binerror);
    }
  }
}
