#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TString.h"
#include "TTreeFormula.h"
#include <TFile.h>
#include <TROOT.h>

#ifndef EVENTDUMP_h
#define EVENTTDUMP_h 1

enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufake, nTotalDefinedChannels};

TString CraftFormula(TString cuts, TString chan);
std::vector<Int_t> GetAllIndex(TString varstring);

class EventDump{

  public:
   EventDump(TString vars, TString formato = "%2.2f", TString cut = "", TString ch = "");
   ~EventDump(){};

   void Loop();
   void loadTree();
   void SetFormat(TString f){ format = f;}
   void SetStringVars(TString v){stringvars = v;}
   void Print(TString file = "");
   void SetSampleName(TString t){sampleName   = t;}
   void SetTreeName(  TString t){treeName     = t;}
   void SetPath(      TString t){path         = t;}
   TTreeFormula* GetFormula(TString var);

  protected:
   TString sampleName;
   TString path;
   TString treeName;
   TString format;
   TString stringvars;
   TString stringcut;
   TString cut;
   TString chan;
   Int_t nVars;
   TTree* tree;
   std::vector<TString> vv;
   std::vector<TString> ff;
   std::vector<Int_t> ii;
   TTreeFormula *FormVars[30]; 
   TTreeFormula *FormCut;
};

#endif
