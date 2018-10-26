#include "Histo.h"
#include "Looper.h"
#include "TString.h"
#include "TTree.h"
#include <iostream>

void SkimMiniTrees(TString path, TString files, TString outDir, TString cut, TString treeName = "tree");
void SkimMiniTree(TString path, TString sampleName, TString outDir, TString cut, TString treeName);

void SkimMiniTrees(TString path, TString files, TString outDir, TString cut, TString treeName){
	gSystem->mkdir(outDir, kTRUE);
  TString sampleName;
  TString in  = path + "/Tree_" + sampleName + ".root";
  TString out = outDir + "/Tree_" + sampleName + ".root";
  vector<TString> vfile = TStringToVector(files);
  Int_t nFiles = vfile.size(); Int_t i = 0;

  cout << "\n ##### SKIM MINI TREES " << endl;
  cout << " #####   TREES IN: " << path << endl;
  cout << " #####   OUTPUT  : " << outDir << endl;
  cout << " #####   CUT     : " << cut  << endl;
  for(i = 0; i < nFiles; i++){
    sampleName = vfile.at(i);
    in = path + "/Tree_" + sampleName + ".root";
    SkimMiniTree(path, sampleName, outDir, cut, treeName);
  }
  cout << " ##### DONE!!!! \n " << endl;
  
}

void SkimMiniTree(TString path, TString sampleName, TString outDir, TString cut, TString treeName){
  TString in  = path + "/Tree_" + sampleName + ".root";
  TString out = outDir + "/Tree_" + sampleName + ".root";
  TFile* f; TTree* t; TFile* fout; 
  TTree* SkimmedTree;  Int_t inEntries = 0; Int_t outEntries = 0;

  cout << "\033[1;37m >> Skimming sample \033[1;31m " << sampleName;
  // Open file and get the tree
  f = TFile::Open(in);
  t = (TTree*) f->Get(treeName);
  inEntries = t->GetEntries();
  cout << "\033[1;37m ---> In: \033[1;33m" << inEntries;

  // Open output rootfile
  fout = new TFile(out, "RECREATE"); 
    
  // Skim and clone
  SkimmedTree = t->CopyTree(cut);
  outEntries = SkimmedTree->GetEntries();

  cout << "\033[1;37m, out: \033[1;32m" << outEntries << "\033[0m" << endl;

  // Write the new tree
  SkimmedTree->SetDirectory(0);
  fout->WriteTObject(SkimmedTree, "tree");
  //SkimmedTree->AutoSave();
  delete fout;
}
