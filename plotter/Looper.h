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
#include "TH2F.h"
#include "TSystem.h"
#include "../InputFiles/for4t/fake_rates.h"

enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufake, nTotalDefinedChannels};
const Int_t nLHEweights = 112;

std::vector<TString> GetAllVars(TString varstring); 
TH1D* loadSumOfLHEweights(TString pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/", TString sampleName = "TTbar_PowhegLHE");
TH1F* hLHE[nLHEweights];

class Looper{
  public:
    Looper(){};
    Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t b0 = 0, Float_t bN = 300){
   Hist = NULL; 
   FormulasCuts = NULL;
   FormulasVars = NULL;
   FormulasLHE  = NULL;
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

   pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/";
  }  
    Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t* bins = 0){
   Hist = NULL; 
   FormulasCuts = NULL;
   FormulasVars = NULL;
   FormulasLHE  = NULL;
   tree = NULL;
   path = pathToTree;
   treeName = NameOfTree;
   //loadTree();
   nbins = nb;
   bin0 = 0;
   binN = 0;
   vbins = bins;
   var = _var;
   cut = _cut;
   chan = _chan;

   pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/";
  } 

   ~Looper(){
		 delete tree->GetCurrentFile();
     if(FormulasCuts) delete FormulasCuts;
     if(FormulasVars) delete FormulasVars;
     if(FormulasLHE)  delete FormulasLHE;
     //if(Hist) delete Hist;
   };

   TString CraftFormula(TString cut, TString chan, TString sys, TString options = "");
   TString CraftVar(TString varstring, TString sys);

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
   Bool_t doSysPDF = false;
   Bool_t doSysScale = false;
   Bool_t doISRweight = false;
   Int_t numberInstance;
   void SetSampleName(TString t){sampleName   = t;}
	 void SetTreeName(  TString t){treeName     = t;}
	 void SetPath(      TString t){path         = t;}

   void loadHisto2D(TString Path_to_histo, TString histo_name);

   Float_t GetPDFweight(TString sys = "PDF");
   Float_t GetScaleWeight(TString sys = "Scale");
   Histo* GetHisto(TString sampleName, TString sys = "0");

	 void SetFormulas(TString sys = "0"); 
	 void CreateHisto(TString sys = "0"); 
   void Loop(TString sys = "");
   Float_t getLHEweight(Int_t i);
   void SetOptions(TString p){options = p;}
   TTreeFormula *GetFormula(TString name, TString var);

 // protected:
   Histo* Hist;
   TTreeFormula* FormulasCuts;
   TTreeFormula* FormulasVars;
   TTreeFormula* FormulasLHE;
   Int_t   nbins;
   Float_t bin0;
   Float_t binN;
   Float_t *vbins;
   TString hname;
   TString xtit;
   TString stringcut; TString stringvar;

   TString sys; // This has another dim!!!! 

   Bool_t HistosCreated;

   TH1D* hLHEweights;
   TString pathToHeppyTrees;
   TString path;
   TString treeName;
   TString cut; TString chan; TString var;
   TString options = "";
   TString sampleName;

   TH2F* hWeights;

   void loadTree();
   TTree* tree;

   Int_t nLeps; 
   Int_t nTaus; 
   Int_t nFakeLeps; 
   Float_t FLepPt; 
   Float_t FLepEta; 
   Float_t FLepCharge; 
   Float_t LepCharge; 
   Int_t   FLepPdgId; 

   TTreeFormula *ForFLepPt;
   TTreeFormula *ForFLepEta; 
   TTreeFormula *ForFLepPdgId;
   TTreeFormula *ForLepChar;
   TTreeFormula *FornSelLep;
   TTreeFormula *FornSelTau;
   TTreeFormula *FornFakeLep;

};


// GET ALL FILES IN PATH
vector<TString> GetAllFiles(TString path, TString  filename) {
  vector<TString> theFiles;

  TString command("ls ");
  if(filename != "")
    command += 
      path + "/" + filename + " " +
      path + "/" + filename + ".root " +
      path + "/" + filename + "_[0-9].root " +
      path + "/" + filename + "_[0-9][0-9].root " +
      path + "/Tree_" + filename + ".root " +
      path + "/Tree_" + filename + "_[0-9].root " +
      path + "/Tree_" + filename + "_[0-9][0-9].root" +
      path + "/" + filename + "_ext[0-9].root " +
      path + "/" + filename + "_ext[0-9]_[0-9].root " +
      path + "/" + filename + "_ext[0-9]_[0-9][0-9].root " +
      path + "/Tree_" + filename + "_ext[0-9].root " +
      path + "/Tree_" + filename + "_ext[0-9]_[0-9].root " +
      path + "/Tree_" + filename + "_ext[0-9]_[0-9][0-9].root";
  else command += path;

  command += " 2> /dev/null";

  //We cannot use GetFromPipe because it is too verbose, so we implement
  //the full code
  //    TString result=gSystem->GetFromPipe(command);
  TString result;
  FILE *pipe = gSystem->OpenPipe(command, "r");
  if (!pipe) cerr << "ERROR: in GetAllFiles. Cannot run command \"" << command << "\"" << endl;
  else {
    TString line;
    while (line.Gets(pipe)) {
      if (result != "")	result += "\n";
      result += line;
    }
    gSystem->ClosePipe(pipe);
  }
  
  if (result != "" ) {
    TObjArray* filesfound = result.Tokenize(TString('\n'));
    if (!filesfound) cerr << "ERROR: in GetAllFiles. Could not parse output while finding files" << endl;
    else {
      for (int i = 0; i < filesfound->GetEntries(); i++) theFiles.push_back(filesfound->At(i)->GetName());
      filesfound->Clear();
      delete filesfound;
    }
  }
  if (theFiles.size() == 0) cerr << "ERROR: in GetAllFiles. Could not find data!" << endl;
  return theFiles;
}






#endif
