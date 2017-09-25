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

enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, TotalDefinedChannels};
const Int_t nLHEweights = 112;
std::vector<TString> TStringToVector(TString t, char separator = ',');
void PrintVector(std::vector<TString> v);
void PrintVector(std::vector<Float_t> v);

Histo* GetHisto(TString path, TString samplename, TString treeName = "tree", TString var = "", TString cut = "", TString chan = "TChannel > 0", TString weight = "TWeight", TString sys = "", Int_t nbins = 1, Float_t bin0 = 0, Float_t binN = 1, Float_t *bins = nullptr, TString options = "");
std::vector<TString> GetAllVars(TString varstring, Bool_t verbose = false); 
TString CraftFormula(TString cut, TString chan, TString sys, TString weight, TTree* tree, TString options = "");
TString CraftVar(TString varstring, TString sys, TTree* tree);
TTreeFormula *GetFormula(TString name, TString var, TTree* tree);
TH1D* loadSumOfLHEweights(TString pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/", TString sampleName = "TTbar_PowhegLHE");
TTree* loadTree(TString path, TString sampleName, TString treeName);
TH1D* loadHistogram1D(TString path, TString sampleName, TString histoName);
TH1F* loadHistogram1F(TString path, TString sampleName, TString histoName);
TH1F* hLHE[nLHEweights];

class Looper{
  public:
    Looper(){};

    Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t b0 = 0, Float_t bN = 300){
   verbose = false;
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

   weight = "TWeight";

   pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/";
  }  
    Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t* bins = 0){
   verbose = false;
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

   weight = "TWeight";

   pathToHeppyTrees = "/pool/ciencias/HeppyTreesSummer16/v2/";
  } 

   ~Looper(){
		 delete tree->GetCurrentFile();
     if(FormulasCuts) delete FormulasCuts;
     if(FormulasVars) delete FormulasVars;
     if(FormulasLHE)  delete FormulasLHE;
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
   Bool_t doSysPDF = false;
   Bool_t doSysScale = false;
   Bool_t doISRweight = false;
   Bool_t verbose = false;
   Int_t numberInstance;
   void SetSampleName(TString t){sampleName   = t;}
   void SetHeppySampleName(TString t){HeppySampleName   = t;}
	 void SetTreeName(  TString t){treeName     = t;}
	 void SetPath(      TString t){path         = t;}
   void SetWeight(    TString t){weight       = t;}

   void loadHisto2D(TString Path_to_histo, TString histo_name);

   Float_t GetPDFweight(TString sys = "PDF");
   Float_t GetScaleWeight(TString sys = "Scale");
   Histo* GetHisto(TString sampleName, TString sys = "0");

	 void SetFormulas(TString sys = "0"); 
	 void CreateHisto(TString sys = "0"); 
   void Loop(TString sys = "");
   Float_t getLHEweight(Int_t i);
   void SetOptions(TString p){options = p;}

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
   TString HeppySampleName;
   TString weight;

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

//################################################################
//
// UNIVERSAL FUNCTIONS READY TO USE !!!!!!!!!!!!!!!!!!!
//

// GET ALL FILES IN PATH
vector<TString> GetAllFiles(TString path, TString  filename) {
  vector<TString> theFiles;
  if(filename.Contains("&")){
    filename.ReplaceAll(" ", "");
    TString partial;
    vector<TString> a;
    while(filename.Contains("&")){
      a.clear();
      partial =  filename(0, filename.First("&"));
      a = GetAllFiles(path, partial);
      theFiles.insert(theFiles.end(), a.begin(), a.end());
      filename = filename(filename.First("&")+1, filename.Sizeof());
    } 
    a = GetAllFiles(path, filename);
    theFiles.insert(theFiles.end(), a.begin(), a.end());
    return theFiles;
  }

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


std::vector<TString> TStringToVector(TString t, char separator){
  std::vector<TString> v;
  t.ReplaceAll(" ", "");
  Int_t n = t.CountChar(separator);
  TString element;
  for(Int_t i = 0; i < n; i++){
    element = t(0, t.First(separator));
    t = t(t.First(separator)+1, t.Sizeof());
    v.push_back(element);
  }
  v.push_back(t);
  return v;
}

void PrintVector(std::vector<TString> v){
  Int_t dim = v.size();
  cout << "[size = " << dim << "]: (";
  for(Int_t i = 0; i < dim-1; i++) cout << v.at(i) << ", ";
  cout << v.at(dim-1) << ")" << endl;
}

void PrintVector(std::vector<Float_t> v){
  Int_t dim = v.size();
  cout << "[size = " << dim << "]: (";
  for(Int_t i = 0; i < dim-1; i++) cout << v.at(i) << ", ";
  cout << v.at(dim-1) << ")" << endl;
}

void PrintArray(Float_t *a, Int_t dim){
  TString out;
  out = Form("[size = %i] [", dim);
  for(Int_t i = 0; i < dim-1; i++) out += Form("%g, ",a[i]); 
  out += Form("%g]\n", a[dim-1]);
  cout << out;
}


Bool_t IsWord(TString s, Int_t pos = 0, TString word = ""){
  if(word == "") word = s;
  if(s.Index(word, word.Sizeof()-1, pos, TString::kExact) < 0){
    cout << "[IsWord] WARNING: no word \"" << word << "\" in string \"" << s << "\"" << endl;
    return false;
  }
  TString LimitWord = ",. ?:)]([{}@\"'&|<>=!";
  Int_t nstop = LimitWord.Sizeof()-1;
  Bool_t isword = true;

  // Check containt
  for(Int_t i = 0; i < nstop; i++){
    if(word.Contains(LimitWord[i])){
      isword = false;
      return isword; 
    }
  }

  // Check the begining
  if(pos != 0){
    isword = false;
    char c = s[pos-1];
    for(Int_t i = 0; i < nstop; i++){
      if(c == LimitWord[i]){
        isword = true;
        break;  
      }
    }
  } 
  if(!isword) return false;

  // Check the end 
  if(s.Sizeof() == pos+word.Sizeof()) return true;
  else{
    isword = false;
    char d = s[pos+word.Sizeof()-1];
    for(Int_t i = 0; i < nstop; i++){
      if(d == LimitWord[i]){
        isword = true;
        break;  
      }
    }
  }
  return isword;
}

TString ReplaceWords(TString orig, TString search, TString replace){
  if(!IsWord(replace)){
    cout << "[ReplaceWords] Warning!! \""  << replace << "\" is not a word!!" << endl;
    return orig;
  }
  if(!IsWord(search)){
    cout << "[ReplaceWords] Warning!! \""  << search << "\" is not a word!!" << endl;
    return orig;
  }
  if(search == replace) return orig;

  Int_t wordsize = search.Sizeof()-1;
  Int_t replsize = replace.Sizeof()-1;
  Int_t pos = 0; Int_t found = 0; Int_t nreplaces = 0;


  while(orig.Index(search, wordsize, pos, TString::kExact) >= 0){
    
    found = orig.Index(search, wordsize, pos, TString::kExact);
    pos = found;
    //cout << "orig = " << orig << endl;
    //cout << "pos = " << pos << endl;
    //cout << "search = " << search << endl;
    if(IsWord(orig, pos, search)){ // Replace and restart the loop
      orig.Replace(pos, wordsize, replace);
      pos += replsize;
      continue;
    }
    else{ // Go on!
      pos += wordsize;
    } 
  }
  return orig;
}

std::vector<TString> GetAllVars(TString varstring, Bool_t verbose){ 
  std::vector<TString> g;
  TString var; Int_t i;
  TString chain = varstring;
  //cout << "[GetAllVars] varstring = " << varstring << endl;
  while(chain.Contains("T")){
    i = 0;
    var = chain(chain.First('T'), chain.Sizeof());
    while(TString(var[i]).IsAlnum() || var[i] == '_') i++;
    //while( var[i] != ' ') i++;
    var = var(0,i);
    //cout << "Found var; " << var << endl;
    //cout << "Replacing word \"" << var << "\" in chain \"" << chain << "\" " << endl;
    if(!IsWord(chain, chain.First('T'), var)) break;
    g.push_back(var);
    //chain.ReplaceAll(var, "");
    chain = ReplaceWords(chain, var, "");
  }
  if(verbose) {
    cout << "[GetAllVars] List of variables: ";
    for(Int_t i = 0; i < (Int_t) g.size(); i++) cout << g.at(i) << ", ";
    cout << endl;
  }
  return g;
}

TString CraftVar(TString varstring, TString sys, TTree *tree){
  TString var = varstring;
  std::vector<TString> AllVars = GetAllVars(var);
  Int_t nvars = AllVars.size();
  //if(verbose) cout << "[Looper::CraftVar] Systematic = " << sys << "... Looping over " << nvars << " variables..." << endl;
  for(Int_t i = 0; i < nvars; i++) 
    if(tree->GetBranchStatus(AllVars.at(i) + sys)){
      var = ReplaceWords(var, AllVars.at(i), AllVars.at(i)+sys);
    }
  return var;
}

TString CraftFormula(TString cuts, TString chan, TString sys, TString weight, TTree* tree, TString options){
  TString schan = ("1");
  if     (chan == "ElMu")  schan = (Form("(TChannel == %i)", iElMu));
  else if(chan == "Muon")  schan = (Form("(TChannel == %i)", iMuon));
  else if(chan == "Elec")  schan = (Form("(TChannel == %i)", iElec));
  else if(chan == "2lss")  schan = (Form("(TChannel == %i)", i2lss));
  else if(chan == "3l")    schan = (Form("(TChannel == %i)", iTriLep));
  else if(chan == "4l")    schan = (Form("(TChannel == %i)", iFourLep));
  else if(chan == "SF" || chan == "sameF") schan = (Form("(TChannel != %i)", iElMu));
  else if(chan == "PromptLep") schan = Form("(TChannel == %i || TChannel == %i)", iTriLep, i2lss);
  else if(chan == "PromptTau") schan = Form("(TChannel == %i || TChannel == %i)", iSS1tau, iOS1tau);
  else if(chan == "SSTau") schan = Form("(TChannel == %i)", iSS1tau);
  else if(chan == "OSTau") schan = Form("(TChannel == %i)", iOS1tau);
  else if(chan == "All")   schan = ("1");
  else schan = chan;

  //TString weight = TString("TWeight");
  if(tree->GetBranchStatus(weight + "_" + sys)){
    weight += "_" + sys; 
  }

  std::vector<TString> AllVars = GetAllVars((TString) cuts);
  Int_t nvars = AllVars.size();
  for(Int_t i = 0; i < nvars; i++) 
    if(tree->GetBranchStatus(AllVars.at(i) + sys)){
      cuts = ReplaceWords((TString) cuts, AllVars.at(i), AllVars.at(i)+sys);
    }
  TString  formula = TString("(") + cuts + TString(")*(") + schan + TString(")*") + weight;
/*  if((options.Contains("Fake") || options.Contains("fake"))){
    if(chan.Contains("Lep")) schan = Form("(TChannel == %i || TChannel == %i)", i2lss_fake, iTriLep_fake);
    //if(chan.Contains("Tau")) schan = Form("(TChannel == %i)", i1Tau_emufake);
    if(options.Contains("sub") || options.Contains("Sub"))  formula = TString("(") + cuts + TString(")*(") + schan + TString(")*") + weight;
    else formula = TString("(") + cuts + TString(")*(") + schan + TString(")");
  }*/
  if(options.Contains("isr") || options.Contains("ISR"))   formula = "TISRweight*(" + formula + ")";
  if(options.Contains("noWeight"))                         formula = TString("(") + cuts + TString(")*(") + schan + TString(")"); 
  return formula;
}

TTreeFormula* GetFormula(TString name, TString var, TTree* tree){
  TTreeFormula *f = new TTreeFormula(name, var, tree);
  f->GetNdata();
  return f;
} 

TTree* loadTree(TString path, TString sampleName, TString treeName){
  TTree* t;
  TString prefix = "Tree_"; TString sufix = ".root";
  TString totalname = path + prefix + sampleName + sufix;
  cout << "[loadTree]: Getting tree \"" << treeName << "\" in file " << totalname << endl;
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(totalname);
	if (!f || !f->IsOpen()) {
		f = new TFile(totalname);
	}
	f->GetObject(treeName,t);
  t->SetDirectory(0);
  //delete f;
  return t;
}

TH1D* loadHistogram1D(TString path, TString sampleName, TString histoName){
  TH1D* h;
  if(!path.EndsWith("/")) path += "/";
  sampleName.ReplaceAll(".root", "");
  if(sampleName.BeginsWith("Tree_")) sampleName = sampleName(5, sampleName.Sizeof());
  TString thefile = path + "Tree_" + sampleName + ".root";
  cout << "[loadHistogram1D]: Getting histogram \"" << histoName << "\" in file " << thefile << endl;
  TFile* inputfile = TFile::Open(thefile);
  inputfile->GetObject(histoName, h);
  h->SetDirectory(0);
  delete inputfile;
  return h;
}

TH1F* loadHistogram1F(TString path, TString sampleName, TString histoName){
  if(!path.EndsWith("/")) path += "/";
  sampleName.ReplaceAll(".root", "");
  if(sampleName.BeginsWith("Tree_")) sampleName = sampleName(5, sampleName.Sizeof());
  TString thefile = path + "Tree_" + sampleName + ".root";
  TH1F* h;
  TFile* inputfile = TFile::Open(thefile);
  inputfile->GetObject(histoName, h);
  h->SetDirectory(0);
  delete inputfile;
  return h;
}









#endif
