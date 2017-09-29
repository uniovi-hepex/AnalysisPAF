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

void AddFRhisto(TString filename, TString histoName, TString name = "");
void AddFRhisto(TH2F* h, TString name = "");
TH2F* GetFRhisto(TString name);

//=== Load a fr map
void AddFRhisto(TString filename, TString histoName, TString name){
  TH2F* h; 
  TString thefile = "./FRmaps/" + filename;
  if(!filename.EndsWith(".root")) thefile = thefile + ".root";
  TFile* inputfile = TFile::Open(thefile);
  h = (TH2F*) inputfile->Get(histoName);
  if(name != "") h->SetName(name);
  h->SetDirectory(0);
  delete inputfile;
  AddFRhisto(h);
}

//=== Just store a fr map
void AddFRhisto(TH2F* h, TString name){
  //TH2D* histo = (TH2D*) h->Clone();
  //histo->SetDirectory(0);
  if(name != "") h->SetName(name);
  FRhistos.push_back(h); 
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

Float_t GetFRweightMVAM(Float_t pt, Float_t eta, Int_t isTight, Int_t pdgId){
  TH2F* hEl = GetFRhisto("mvaMFRhistoEl");
  TH2F* hMu = GetFRhisto("mvaMFRhistoMu");
  Float_t f = 0;
  if (pdgId == 11 && !isTight){//Electrons 
    f = hEl->GetBinContent(hEl->FindBin(pt,eta));
  }
  else if (pdgId == 13 && !isTight){
    f = hMu->GetBinContent(hMu->FindBin(pt,eta));
  }  
  if (f!= 0 || f == 0){ 
    std::cout << f << std::endl;
    std::cout << pt << "," << eta << "," << isTight << "," << pdgId;  
  }

  //delete hEl;
  //delete hMu; =>We don't want crashes, don't we?
  return f;
}

Float_t GetFRweightlepMVAM3lep(Float_t f1, Float_t f2, Float_t f3, Int_t doSub){
  Float_t f[3] = {f1,f2,f3};
  Float_t fWeight = 1.;
  for (int j = 0; j < 3; j++){
    if  (f[j] > 0.){
      fWeight *= (1 - f[j]/(1.-f[j]));
    }
  }

  fWeight = 1-fWeight;
  fWeight *= doSub;
  //std::cout << fWeight;
  return fWeight;
}


Float_t dumpFR(Float_t a){
  return 1;
  } 
#endif
