#ifndef Looper_h 
#define Looper_h 1

#include <TROOT.h>
#include <TChain.h>
#include <THStack.h>
#include <TH1F.h>
#include <TFile.h>
#include <TMath.h>
#include <TLegend.h>
#include <iostream>
#include "TCut.h"
#include "TTreeFormula.h"
#include "Histo.h"


TString CraftFormula(TString var, TString cut, TString chan, TString sys);
TString CraftVar(TString varstring, TString sys);

class Looper{
  public:
    Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t b0 = 0, Float_t bN = 300){
   Hist = NULL; 
   FormulasCuts = NULL;
   FormulasVars = NULL;
   tree = NULL;
   path = pathToTree;
   treeName = NameOfTree;
   //loadTree();
   nbins = nb;
   bin0 = b0;
   binN = bN;
   var = _var;
   cut = _cut;
   chan = _chan;
  }  

   ~Looper(){
		 delete tree->GetCurrentFile();
     if(FormulasCuts) delete FormulasCuts;
     if(FormulasVars) delete FormulasVars;
     //if(Hist) delete Hist;
   };

   void SetCut(    TString t){cut  = t;}
   void SetVar(    TString t){var  = t;}
   void SetChannel(TString t){chan = t;}
   void SetChannel(Int_t i){
     if(i == 0 || i == 1) chan = "ElMu";
     else if(i == 2) chan = "Muon";
     else if(i == 3) chan = "Elec";
     else if(i == 4) chan = "SF";
     else chan = "All";
   }
   void SetSampleName(TString t){sampleName   = t;}
	 void SetTreeName(  TString t){treeName     = t;}
	 void SetPath(      TString t){path         = t;}

   Histo* GetHisto(TString sampleName, TString sys = "0");

	 void SetFormulas(TString sys = "0"); 
	 void CreateHisto(TString sys = "0"); 
   void Loop();

 // protected:
   Histo* Hist;
   TTreeFormula* FormulasCuts;
   TTreeFormula* FormulasVars;
   Int_t   nbins;
   Float_t bin0;
   Float_t binN;
   TString hname;
   TString xtit;
   TString stringcut; TString stringvar;

   TString sys; // This has another dim!!!! 

   Bool_t HistosCreated;

   TString path;
   TString treeName;
   TString cut; TString chan; TString var;

   TString sampleName;

   void loadTree();
   TTree* tree;

};

#endif
