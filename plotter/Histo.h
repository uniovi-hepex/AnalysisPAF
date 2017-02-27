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

enum types{itBkg, itSignal, itData, itCompare, nTypes};
enum isysts{iJESUp, iJESDown, iJER, iBtagUp, iBtagDown, iMisTagUp, iMisTagDown, iLepEffUp, iLepEffDown, iTrigUp, iTrigDown, iPUUp, iPUDown, iFSUp, iFSDown, iGenMETUp, iGenMETDown, iISRJetsUp, iISRJetsDown, iQ2Up, iQ2Down, iPDFUp, iPDFDown, iHadUp, iHadDown, iNLOUp, iNLODown, nSysts};

TString SystName[nSysts] = {"JESUp", "JESDown", "JER", "BtagUp", "BtagDown", "MisTagUp", "MisTagDown", "LepEffUp", "LepEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "FSUp", "FSDown", "GenMETUp", "GenMETDown", "ISRJetsUp", "ISRJetsDown", "Q2Up", "Q2Down", "PDFUp", "PDFDown", "HadUp", "HadDown", "NLOUp", "NLODown"};

const TString DefaultPath = "../temp/";
const TString prefix = "Tree_"; const TString sufix = ".root";
const TString DefaultTreeName = "MiniTree";


class Histo : public TH1F{
	public:
		Histo(const char *name, const char *title, Int_t nbins, Double_t xlow, Double_t xup);
		Histo(const char *name, const char *title, Int_t nbins, const Float_t* xbins);
		Histo(const TH1F &h, Int_t tipo = 0, Int_t c = 1){
			((Histo&)h).Copy(*this);
			cuts = "cut"; xlabel = "[GeV]"; syst = 0.25;
			SetType(tipo);
			SetColor(c);
			SetStyle();
		}
		~Histo(){};

		void SetType(Int_t tipo = 0);
		void SetColor(Int_t c);
		void SetStyle();

    TString GetTag(){return tag;}
    Int_t GetType(){return type;}
    Float_t GetYield(){return yield;}
    Float_t GetMax(){return max;}
    TString GetProcess(){return process;}
    Float_t GetSyst(){return syst;}
		void StackOverflow(Bool_t doStackOverflow = 1);
		void SetTag(TString p, TString t="", TString x = "", TString c = "");
		void SetProcess(TString p);
		void SetTitles(TString x, TString c = "");
		void SetSyst(Float_t s);
		void SetParams(Int_t tipo = 0, Int_t color = 1, Float_t s = 0.25, TString t = "", TString x = "", TString c = "");
    void SetSystematic(Float_t s){ syst = s;}

		void AddToLegend(TLegend* leg, Bool_t doyi = 1);
		TH1F* GetVarHistoStatBin(Int_t bin = 0, TString dir = "Up");
		void AddToSystematics(Histo* hsys, TString dir = "Down");

		Float_t *vsysu; 
		Float_t *vsysd;

	protected:
		Int_t type; 
		Int_t color;
		TString tag = "0"; TString process="0"; TString cuts="0"; TString xlabel="0";
		Float_t syst;
		Double_t yield;
		Double_t max;
		Float_t haveStyle = 0;
		Bool_t IsStackOverflow = true;

};

Histo::Histo(const char *name, const char *title, Int_t nbins, Double_t xlow, Double_t xup)	: TH1F(name, title, nbins, xlow, xup){
	SetType(0);
	SetStyle();
	SetColor(1);
	tag = TString(name);
	process = TString(title);
}

Histo::Histo(const char *name, const char *title, Int_t nbins, const Float_t* xbins): TH1F(name,title,nbins,xbins){
	SetType(0);
	SetStyle();
	SetColor(1);
	tag = TString(name);
	process = TString(title);
}

class AnalHisto{
  public:
   //AnalHisto(){};
   //AnalHisto(TString sample = "TTbar_Powheg", TString ct="", TString channel = "All", TString path = "", TString treeName = "", Int_t isyst = -1);
   AnalHisto(TString sample = "TTbar_Powheg", TCut    ct="", TString channel = "All", TString path = "", TString treeName = "", Int_t isyst = -1);
   ~AnalHisto(){
		 delete tree->GetCurrentFile();
   };

   void SetCut(TCut t){cut = TCut(t);}
   void SetChannel(TString t){chan = t;}
   void SetChannel(Int_t i){
     if(i == 0 || i == 1) chan = "ElMu";
     else if(i == 2) chan = "Muon";
     else if(i == 3) chan = "Elec";
     else if(i == 4) chan = "SF";
     else chan = "All";
   }
   void SetSystematic(Int_t i){syst = i;}
   Histo* GetHisto(){return h;}
   void Fill(TString variable = "", Int_t isyst = -1);   
   void SetHisto(TString name, Int_t nb, Float_t xi, Float_t xe);
   void SetHisto(TString name, Int_t nb, Float_t thebins[100]);

   Float_t *xbins;
   TTree* tree;
	 void SetTreeName(TString t);
	 void SetPath(TString t);

  protected:
   TString path;
   TString treeName;
   Histo *h;
   TString sampleName;
   TCut cut;
   TString chan;
   TString weight;
   Int_t syst;
   TString var;
   Int_t nbins; Float_t x0; Float_t xf;
   TString histoname;

   void loadTree();

};

#endif
