//=============================================================================================
//
//  Load your FR maps and use your own fr functions definded here!
//  Remember!!! Depending on the number of fake leptons:
//    0 fakes --> weight = 0
//    1 fake  --> weight = f/(1-f) 
//    2 fakes --> weight = -f1*f2/(1-f1)(1-f2)
//    3 fakes --> weight = +f1*f2*f3/((1-f1)(1-f2)(1-f3))
//                     
//=============================================================================================

#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include <iostream>

#ifndef FRFunctions_C
#define FRFunctions_C 1

std::vector<TH2F*> FRhistos;
std::vector<TH1F*> FRhistos1D;

void AddFRhisto(TString filename, TString histoName, TString name = "");
void AddFRhisto(TH2F* h, TString name = "");
TH2F* GetFRhisto(TString name);

void AddFRhisto1D(TString filename, TString histoName, TString name = "");
void AddFRhisto1D(TH1F* h, TString name = "");
TH1F* GetFRhisto1D(TString name);

//=== Load a fr map
void AddFRhisto(TString filename, TString histoName, TString name){
  TH2F* h; 
  TString thefile;
  if(filename.Contains("/")) thefile = filename;
  else                       thefile = "./FRmaps/" + filename;
  
  if(!filename.EndsWith(".root")) thefile = thefile + ".root";
  TFile* inputfile = TFile::Open(thefile);
  h = (TH2F*) inputfile->Get(histoName);
  if(name != "") h->SetName(name);
  h->SetDirectory(0);
  delete inputfile;
  AddFRhisto(h);
}

void AddFRhisto1D(TString filename, TString histoName, TString name){
  TH1F* h; 
  TString thefile;
  if(filename.Contains("/")) thefile = filename;
  else                       thefile = "./FRmaps/" + filename;
  
  if(!filename.EndsWith(".root")) thefile = thefile + ".root";
  TFile* inputfile = TFile::Open(thefile);
  h = (TH1F*) inputfile->Get(histoName);
  if(name != "") h->SetName(name);
  h->SetDirectory(0);
  delete inputfile;
  AddFRhisto1D(h);
}


//=== Just store a fr map
void AddFRhisto(TH2F* h, TString name){
  //TH2D* histo = (TH2D*) h->Clone();
  //histo->SetDirectory(0);
  if(name != "") h->SetName(name);
  FRhistos.push_back(h); 
}

void AddFRhisto1D(TH1F* h, TString name){
  //TH2D* histo = (TH2D*) h->Clone();
  //histo->SetDirectory(0);
  if(name != "") h->SetName(name);
  FRhistos1D.push_back(h); 
}

//=== Search for a stored fr map
TH2F* GetFRhisto(TString name){
  TH2F* h = nullptr;
  TString n;
  for(Int_t i = 0; i < (Int_t) FRhistos.size(); i++){
    h = FRhistos.at(i);
    n = h->GetName();
    if(name == n) return h;
  }
  std::cout << "[Looper::GetFRhisto] ERROR: not found histogram \"" << name << "\" on FR histos..." << std::endl;
  return h;
}

TH1F* GetFRhisto1D(TString name){
  TH1F* h = nullptr;
  TString n;
  for(Int_t i = 0; i < (Int_t) FRhistos1D.size(); i++){
    h = FRhistos1D.at(i);
    n = h->GetName();
    if(name == n) return h;
  }
  std::cout << "[Looper::GetFRhisto] ERROR: not found histogram \"" << name << "\" on FR histos..." << std::endl;
  return h;
}


//=== This is only an example in case you have only one fakeable lepton!!!!
Float_t GetFRweight(Float_t pt, Float_t eta, Float_t mvaVal){
  Float_t f = 1; Int_t bin = 0; 

  //>>> Define the iso cut for your good leptons
  Float_t mvaValCut = 0.86;

  //>>> Check if it's a fake
  if(mvaVal > mvaValCut) return 0;

  //>>> Get the histo (you must have loaded it before)
  TH2F* h = GetFRhisto("miFRhisto");

  //>>> Assure pt and eta on range
  pt  = abs(pt); if(pt > 200) pt = 199; 
  eta = abs(eta);

  //>>> Read the SF value from map
  bin = h->FindBin(pt, eta, 1); 
  f   = h->GetBinContent(bin);

  //>>> This function is for a single lepton...
  return f/(1-f);
}

Float_t GetStopTopPt(Float_t pt1, Float_t pt2){
  if(pt1 > 400) pt1 = 399;
  if(pt2 > 400) pt2 = 399;

  TH2F* h = GetFRhisto("TopPtSF");
  Int_t bin = h->FindBin(pt1, pt2);
  return 1/h->GetBinContent(bin);
}

Float_t GetDeltaPhiEtaWeight(Float_t dphi, Float_t deta){
  if(dphi >= 3.2) dphi = 3.1;
  if(deta >= 3.2) deta = 3.1;

  TH2F* h = GetFRhisto("DeltaPhiEta");
  Int_t bin = h->FindBin(dphi, deta);
  return 1/h->GetBinContent(bin);
}

Float_t GetFSR_BtagSF_Up(Float_t pt){
  if(pt < 30) pt = 30.1; if(pt > 600) pt = 599;
  TH1F* h = GetFRhisto1D("FSR_btagSF_Up");
  Int_t bin = h->FindBin(pt);
  return 1/h->GetBinContent(bin);
}

Float_t GetFSR_BtagSF_Down(Float_t pt){
  if(pt < 30) pt = 30.1; if(pt > 600) pt = 599;
  TH1F* h = GetFRhisto1D("FSR_btagSF_Down");
  Int_t bin = h->FindBin(pt);
  return h->GetBinContent(bin);
}

#endif
