#ifndef Histo_h
#define Histo_h

#include <TROOT.h>
#include <TChain.h>
#include <THStack.h>
#include <TH1F.h>
#include <TFile.h>
#include <TMath.h>
#include <TLegend.h>
#include <iostream>
#include "TCut.h"

enum types{itBkg, itSignal, itData, itCompare, itSys, itOther, nTypes};
enum isysts{iJESUp, iJESDown, iJER, iBtagUp, iBtagDown, iMisTagUp, iMisTagDown, iLepEffUp, iLepEffDown, iTrigUp, iTrigDown, iPUUp, iPUDown, iFSUp, iFSDown, iGenMETUp, iGenMETDown, iISRJetsUp, iISRJetsDown, iQ2Up, iQ2Down, iPDFUp, iPDFDown, iHadUp, iHadDown, iNLOUp, iNLODown, nSysts};
TString SystName[nSysts] = {"JESUp", "JESDown", "JER", "BtagUp", "BtagDown", "MisTagUp", "MisTagDown", "LepEffUp", "LepEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "FSUp", "FSDown", "GenMETUp", "GenMETDown", "ISRJetsUp", "ISRJetsDown", "Q2Up", "Q2Down", "PDFUp", "PDFDown", "HadUp", "HadDown", "NLOUp", "NLODown"};

const TString DefaultPath = "../temp/";
const TString prefix = "Tree_"; const TString sufix = ".root";
const TString DefaultTreeName = "MiniTree";


class Histo : public TH1F{
 public:
  Bool_t doStackOverflow = true;

  Histo(const char *name, const char *title, Int_t nbins, Double_t xlow, Double_t xup);
  Histo(const char *name, const char *title, Int_t nbins, Float_t* bins);
  //Histo(const char *name, const char *title, Int_t nbins, const Float_t* xbins);
 Histo(const TH1F &h, Int_t tipo = 0, Int_t c = 1): tag(""), process(""){
    ((Histo&)h).Copy(*this);
    cuts = "cut"; xlabel = "[GeV]"; sysNorm = 0.25;
    SysTag = "";
    SetType(tipo);
    SetColor(c);
    SetStyle();
  }
  ~Histo(){
    //if(vsysd) delete vsysd;
    //if(vsysu) delete vsysu;
  };
  void Init(){ 
    type = 0; color = 0; SysTag = "0";
    tag = "0"; process="0"; cuts="0"; xlabel="0";
    DrawStyle = "";
  }
  
  void SetType(Int_t tipo = 0);
  void SetColor(Int_t c);
  void SetStyle();
  void ReCalcValues();
  void SetStatUnc();
  Histo* CloneHisto(const char* newname=0) const;


  TString GetTag(){return tag;}
  Int_t GetType(){return type;}
  Float_t GetYield(){return yield;}
  Float_t GetMax(){return max;}
  TString GetProcess(){return process;}
  Float_t GetSysNorm(){return sysNorm;}
  Int_t GetColor(){ return color;}
  void StackOverflow();
  void SetTag(TString p, TString t="", TString x = "", TString c = "");
  void SetProcess(TString p);
  void SetTitles(TString x, TString c = "");
  void SetParams(Int_t tipo = 0, Int_t color = 1, Float_t s = 0.25, TString t = "", TString x = "", TString c = "");
  void SetSysNorm(Float_t s){ sysNorm = s;}
  void SetSysTag(TString s){ SysTag = s;}
  TString GetSysTag(){return SysTag;}
  
  void AddToLegend(TLegend* leg, Bool_t doyi = 1);
  TH1F* GetVarHistoStatBin(Int_t bin = 0, TString dir = "Up");
  void AddToSystematics(Histo* hsys, TString dir = "Down");
  void SetBinsErrorFromSyst();
  void SetDrawStyle(TString g){ DrawStyle = g;}
  TString GetDrawStyle(){ return DrawStyle;}
  void GetEnvelope(vector<Histo*> v, Int_t dir = 1) ;
  
  Float_t *vsysu = NULL; 
  Float_t *vsysd = NULL;


 protected:
  Int_t type; 
  TString DrawStyle;
  Int_t color;
  TString tag = "0"; TString process="0"; TString cuts="0"; TString xlabel="0";
  Float_t sysNorm;
  Double_t yield;
  Double_t max;
  Float_t haveStyle = 0;
  Bool_t IsStackOverflow = true;
  TString SysTag = "0";

};

Histo::Histo(const char *name, const char *title, Int_t nbins, Double_t xlow, Double_t xup)	: TH1F(name, title, nbins, xlow, xup){
	SetType(0);
	SetStyle();
	SetColor(1);
	tag = TString(name);
	process = TString(title);
}

#endif
