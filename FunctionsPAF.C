//########################################################################################################
//
//  Several functions used in PAF
//
//########################################################################################################
#include "TMath.h"
#include "TFile.h"
#include "TLatex.h"
#include "TString.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TChain.h"
#include <iostream>
#include <fstream>

#ifndef FuntionsPAF_C
#define FuntionsPAF_C 1

//----------------- Working with files and Heppy trees
//--------------------------------------------------------------------

//=== Get all files in a path with name "Tree_" + filename + "_[number].root"
vector<TString> GetAllFiles(TString path, TString  filename = "", Bool_t verbose = 0);

//=== Get an histogram or values from plenty of Heppy Trees
TH1D* GetHistoFromFiles(vector<TString> Files, TString histoName);
Float_t GetSMSnorm(std::vector<TString> Files, Float_t mStop, Float_t mLsp);
Float_t GetSMSnorm(TString Files, Float_t mStop, Float_t mLsp);

//=== Saving histos from HeppyTrees in MiniTrees
void SaveCountHistos(vector<TString> Files, TString filename); 
void SaveCountHistos(TString path, TString  inputfile, TString filename); 

//=== Checking trees...
void CheckTreesInDir(TString path, TString treeName = "tree", Int_t verbose = 0);
void CheckTree(TString filename, TString treeName = "tree", Int_t verbose = 0);

//=== Utils
TString GetFunctionFromTH1F(TH1* h, TString funcname = "GetSF", TString varname = "x");
TString GetFunctionFromTH1F(TString path, TString hname, TString funcname = "GetSF", TString varname = "x");

// ttbar cross section at 13 TeV for a given top mass:
Float_t ttbar_xsec_top_mass(Float_t m);

//----------------- For Stop
//--------------------------------------------------------------------
//=== Stop cross section
Double_t GetStopXSec(Int_t StopMass);
//=== Get ISR weight for a mStop, mLsp in a scan file
Float_t  GetISRweight(std::vector<TString> Files, Float_t mStop, Float_t mLsp, Bool_t IsT2ttScan = false);
//=== Get stop and LSP masses from a string "T2tt:[225,25]"
Float_t GetStopMass(TString options);
Float_t GetLspMass(TString options);




//########################################################################################################
//########################################################################################################


//=== Get norm for [mStop, mLsp] in a T2tt scan
Float_t GetSMSnorm(std::vector<TString> Files, Float_t mStop, Float_t mLsp){
  cout << Form("\033[1;36m >>> Searching for normalization factor for stop point with masses [%1.1f, %1.1f]... ", mStop, mLsp);
  Int_t nFiles = Files.size(); TFile *f;
  TH3D *hcount; Float_t val = 0; Float_t ms = 0; Float_t mn = 0;
  Float_t count = 0;
  for(Int_t k = 0; k < nFiles; k++){
    f = TFile::Open(Files.at(k));
    f -> GetObject("CountSMS", hcount);
    Int_t nx = hcount->GetNbinsX();
    Int_t ny = hcount->GetNbinsY();
    for(Int_t i = 0; i < nx; i++){
      for(Int_t j = 0; j < ny; j++){
        val = hcount->GetBinContent(i,j,1);
        if(val != 0){
          ms = hcount->GetXaxis()->GetBinCenter(i);
          mn = hcount->GetYaxis()->GetBinCenter(j);
          if(ms == mStop && mLsp == mn) count += hcount->GetBinContent(hcount->FindBin(ms, mn, 0));
        } 
      }
    }
  }
  return count;
}

Float_t GetSMSnorm(TString pathToCountFile, Float_t mStop, Float_t mLsp){
  int mx; int my;
  TFile* f = TFile::Open(pathToCountFile);
  TH2D*  h = (TH2D*) f->Get("CountSMS");
  return h->GetBinContent(h->FindBin(mStop, mLsp));
}


//=== Get sum of weights for ISR reweighting for [mStop, mLsp] in a T2tt scan
Float_t GetISRweight(std::vector<TString> Files, Float_t mStop, Float_t mLsp, Bool_t IsT2ttScan){
  cout << Form("\033[1;36m >>> [GetISRweight] Searching for normalization factor for ISR Jets reweighting for stop point with masses [%1.1f, %1.1f]... ", mStop, mLsp);
	Int_t nFiles = Files.size(); TFile *f;
  Float_t weight = 0; Float_t nEntries = 0; Float_t nWeightedEntries = 0; Float_t TotalWeightedEntries = 0; Float_t TotalEntries = 0;
  TTree* t;
	TH1F *hcount;
	for(Int_t k = 0; k < nFiles; k++){
    f = TFile::Open(Files.at(k));
    f -> GetObject("tree", t);
    TString strweight = "((nISRJet30==0) + (nISRJet30==1)*0.920 + (nISRJet30==2)*0.821 + (nISRJet30==3)*0.715 + (nISRJet30==4)*0.652 + (nISRJet30==5)*0.561 + (nISRJet30>5)*0.511)";
    hcount = new TH1F("hcount", "hcount", 1, 0, 2); 
    if(IsT2ttScan){ 
      TString strpoint  = Form("(GenSusyMStop == %i && GenSusyMNeutralino == %i)", Int_t(mStop), Int_t(mLsp));
      t->Project("hcount", "1", strweight + "*" + strpoint);
    }
    else {
      t->Project("hcount", "1", strweight);
    }


    nEntries = hcount->GetEntries();
    nWeightedEntries = hcount->GetBinContent(1);
    TotalEntries += nEntries;
    TotalWeightedEntries += nWeightedEntries;
    cout << Form(" [GetISRweight] nFile = %i, nEntries = %f, nWent = %f",k, nEntries, nWeightedEntries) << endl;
	}
  weight = TotalWeightedEntries/TotalEntries; 
  cout << Form("NormWeight = %1.4f\033[0m\n", weight);
  return weight;
}

//=== Cross section for stop masses...
Double_t GetStopXSec(Int_t StopMass){

  if      (StopMass == 100) return 1521.11;
  else if (StopMass == 105) return 1233.18;
  else if (StopMass == 110) return 1013.76;
  else if (StopMass == 115) return 832.656;
  else if (StopMass == 120) return 689.799;
  else if (StopMass == 125) return 574.981;
  else if (StopMass == 130) return 481.397;
  else if (StopMass == 135) return 405.159;
  else if (StopMass == 140) return 342.865;
  else if (StopMass == 145) return 291.752;
  else if (StopMass == 150) return 249.409;
  else if (StopMass == 155) return 214.221;
  else if (StopMass == 160) return 184.623;
  else if (StopMass == 165) return 159.614;
  else if (StopMass == 170) return 139.252;
  else if (StopMass == 175) return 121.416;
  else if (StopMass == 180) return 106.194;
  else if (StopMass == 185) return 93.3347;
  else if (StopMass == 190) return 82.2541;
  else if (StopMass == 195) return 72.7397;
  else if (StopMass == 200) return 64.5085;
  else if (StopMass == 205) return 57.2279;
  else if (StopMass == 210) return 50.9226;
  else if (StopMass == 215) return 45.3761;
  else if (StopMass == 220) return 40.5941;
  else if (StopMass == 225) return 36.3818;
  else if (StopMass == 230) return 32.6679;
  else if (StopMass == 235) return 29.3155;
  else if (StopMass == 240) return 26.4761;
  else if (StopMass == 245) return 23.8853;
  else if (StopMass == 250) return 21.5949;
  else if (StopMass == 255) return 19.5614;
  else if (StopMass == 260) return 17.6836;
  else if (StopMass == 265) return 16.112;
  else if (StopMass == 270) return 14.6459;
  else if (StopMass == 275) return 13.3231;
  else if (StopMass == 280) return 12.1575;
  else if (StopMass == 285) return 11.0925;
  else if (StopMass == 290) return 10.1363;
  else if (StopMass == 295) return 9.29002;
  else if (StopMass == 300) return 8.51615;
  else if (StopMass == 305) return 7.81428;
  else if (StopMass == 310) return 7.17876;
  else if (StopMass == 315) return 6.60266;
  else if (StopMass == 320) return 6.08444;
  else if (StopMass == 325) return 5.60471;
  else if (StopMass == 330) return 5.17188;
  else if (StopMass == 335) return 4.77871;
  else if (StopMass == 340) return 4.41629;
  else if (StopMass == 345) return 4.08881;
  else if (StopMass == 350) return 3.78661;
  else if (StopMass == 355) return 3.50911;
  else if (StopMass == 360) return 3.25619;
  else if (StopMass == 365) return 3.02472;
  else if (StopMass == 370) return 2.8077; 
  else if (StopMass == 375) return 2.61162;
  else if (StopMass == 380) return 2.43031;
  else if (StopMass == 385) return 2.26365;
  else if (StopMass == 390) return 2.10786;
  else if (StopMass == 395) return 1.9665;
  else if (StopMass == 400) return 1.83537;
  else if (StopMass == 425) return 1.31169;
  else if (StopMass == 450) return 0.948333;
  else if (StopMass == 475) return 0.697075;
  else if (StopMass == 500) return 0.51848;
  else if (StopMass == 525) return 0.390303;
  else if (StopMass == 550) return 0.296128;
  else if (StopMass == 575) return 0.226118;
  else if (StopMass == 600) return 0.174599;
  else if (StopMass == 625) return 0.136372;
  else if (StopMass == 650) return 0.107045;
  else if (StopMass == 675) return 0.0844877;
  else if (StopMass == 700) return 0.0670476;
  else if (StopMass == 725) return 0.0536438;
  else if (StopMass == 750) return 0.0431418;
  else if (StopMass == 775) return 0.0348796;
  else if (StopMass == 800) return 0.0283338;
  else if (StopMass == 825) return 0.0241099;
  else if (StopMass == 850) return 0.0189612;
  else if (StopMass == 875) return 0.015625;
  else if (StopMass == 900) return 0.0128895;
  else if (StopMass == 925) return 0.0106631;
  else if (StopMass == 950) return 0.00883465;
  else if (StopMass == 975) return 0.00735655;
  else{ 
    cout << Form("Warning [GetStopXSec]: No Cross Section for that mass!! (mStop = %i) Extrapolating...\n", StopMass);
    Float_t v0; Float_t vf; Int_t pass;
    Float_t pmass; Float_t nmass;
    pass = 5; if(StopMass > 400) pass = 25;

    pmass = StopMass - StopMass%pass;
    nmass = StopMass - StopMass%pass + pass;

    v0 = GetStopXSec(pmass);
    vf = GetStopXSec(nmass);

    Float_t x  = float(StopMass%pass)/pass;
    Float_t newXsec = v0 + (vf-v0)*x;

    cout << Form("xsec(%g) = %g; xsec(%g) = %g --> xsec(%g) = %g", pmass, v0, nmass, vf, (Float_t) StopMass, newXsec);

    return newXsec;
  }
}

Float_t ttbar_xsec_top_mass(Float_t m){
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO#Top_quark_pair_cross_sections_at
  // Using coefficients for 13 TeV, NNPDF2.3 NNLO (5f FFN)
  Float_t mref = 172.5;
  Float_t sref = 831.76;
  Float_t a1 = -0.745047; Float_t a2 = 0.127417;
  Float_t p = (m-mref)/mref;
  Float_t s = sref * mref*mref*mref*mref/(m*m*m*m) * (1 + a1*p + a2*p*p);
  return s;
}

vector<TString> GetAllFiles(TString path, TString  filename, Bool_t verbose) {
  if(verbose) cout << Form("[GetAllFiles]: Obtaining files of form %s in folder %s\n", filename.Data(), path.Data());
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
      path + "/Tree_" + filename + "_[0-9][0-9].root";
  else command += path;

  command += " 2> /dev/null";
  if(verbose) cout << "[GetAllFiles] " << Form("Executing command: %s\n", command.Data());

  //We cannot use GetFromPipe because it is too verbose, so we implement
  //the full code
  //    TString result=gSystem->GetFromPipe(command);
  TString result;
  FILE *pipe = gSystem->OpenPipe(command, "r");
  if (!pipe) cout << Form("ERROR [GetAllFiles] Cannot run command \"%s\"\n", command.Data());
  else {
    TString line;
    while (line.Gets(pipe)) {
      if(line.Contains("13")) continue;
      if (result != "")	result += "\n";
      result += line;
    }
    gSystem->ClosePipe(pipe);
  }
  
  if (result != "" ) {
    TObjArray* filesfound = result.Tokenize(TString('\n'));
    if (!filesfound) cout << ("ERROR [GetAllFiles]: Could not parse output while finding files\n");
    else {
      for (int i = 0; i < filesfound->GetEntries(); i++) theFiles.push_back(filesfound->At(i)->GetName());
      filesfound->Clear();
      delete filesfound;
    }
  }

  if (theFiles.size() == 0) cout << ("ERROR [GetAllFiles]: Could not find data!\n");
  return theFiles;
}

//=== Check that the trees can be read
void CheckTree(TString filename, TString treeName, Int_t verbose){
  if(verbose) cout << "Checking rootfile " << filename << "... "; 
  TFile* f = TFile::Open(filename);
  if(verbose) cout << "Looking for TTree " << treeName << "... ";
  TTree* t; f->GetObject(treeName, t);
  if(t){
    if(verbose) cout << "Found! ";  
    Int_t nEntries = t->GetEntries();
    if(verbose) cout << "nEntries = " << nEntries << endl;
  }
  else{
    if(verbose) cout << "TTree not found in " << filename << endl;
  }
}

//=== Check that the trees can be read
void CheckTreesInDir(TString path, TString treeName, Int_t verbose){
  if(path.EndsWith(".root")){
    CheckTree(path, treeName);
    return;
  }
  vector<TString> AllFiles = GetAllFiles(path, "Tree_*.root");
  for(Int_t i = 0; i < (Int_t) AllFiles.size(); i++){
    CheckTree(AllFiles.at(i), treeName, verbose);
  }
}

//=== Get a histogram from a list of files
TH1D* GetHistoFromFiles(vector<TString> Files, TString histoName){
  Int_t nFiles = Files.size(); TFile *f;
  TH1D *h = nullptr; TH1D *htemp = nullptr;
  for(Int_t i = 0; i < nFiles; i++){
    f = TFile::Open(Files.at(i));
    if(!f->FindKey(histoName)){
      cout << "WARNING: not found histogram \"" << histoName << "\"" << endl;
      return h;
    }
    if(i == 0) {
      f->GetObject(histoName, h); 
    }
    else{
      f->GetObject(histoName, htemp); 
      h->Add(htemp);
    }
  }
  return h;
}

//=== Saving histos from a file to another
void SaveCountHistos(vector<TString> Files, TString filename){ 
  TH1D* _SumOfWeights;
  TH1D* _SumOfLHEweights;
  TH1D* _Count;

  _SumOfWeights    = GetHistoFromFiles(Files, "SumGenWeights");
  _SumOfLHEweights = GetHistoFromFiles(Files, "CountLHE");
  _Count           = GetHistoFromFiles(Files, "Count");
  
  TFile* _file = TFile::Open(filename, "UPDATE");
  if(_SumOfWeights) _SumOfWeights->Write();
  if(_SumOfLHEweights) _SumOfLHEweights->Write();
  if(_Count) _Count->Write();
}

//=== Saving histos from a file to another
void SaveCountHistos(TString path, TString  inputfile, TString filename){ 
  vector<TString> Files = GetAllFiles(path, inputfile);
  SaveCountHistos(Files, filename);
}

//=== Get stop mass from string
Float_t GetStopMass(TString options){
  Float_t mStop = 1;
  options.ReplaceAll(" ", "");
  if(options.Contains("T2tt:")){
    Int_t l = options.First(']');
    Int_t f = options.Index("T2tt:[") + 6;
    TString n = options(f, l-f);
    mStop = ((TString) n(0, n.First(','))).Atof();
  }
  return mStop;
}

//=== Get LSP mass from string
Float_t GetLspMass(TString options){
  Float_t mLsp = 1;
  options.ReplaceAll(" ", "");
  if(options.Contains("T2tt:")){
    options = options(options.Index("T2tt:["), options.Sizeof());
    Int_t l = options.First(']');
    Int_t f = options.Index("T2tt:[") + 6;
    TString n = options(f, l-f);
    mLsp = ((TString) n(n.First(',')+1, n.Sizeof())).Atof();
  }
  return mLsp;
}


TString GetFunctionFromTH1F(TString path, TString hname, TString funcname, TString varname){
  TFile* f = TFile::Open(path);
  TH1* h = (TH1*) f->Get(hname);
  h->SetDirectory(0);
  return GetFunctionFromTH1F(h, funcname, varname);
}

TString GetFunctionFromTH1F(TH1* h, TString funcname, TString varname){
  Int_t nbins = h->GetNbinsX();
  Float_t minval = h->GetBinLowEdge(1);
  Float_t secondval = h->GetBinLowEdge(2);
  Float_t maxval = h->GetBinLowEdge(nbins+1);
  Float_t penultval = h->GetBinLowEdge(nbins);
  Float_t val = h->GetBinContent(1);
  Float_t binval = 0;

  TString out = "Float_t " + funcname + "(Float_t " + varname + "){\n";
  out += Form("  if(%s < %f) %s = %f;\n", varname.Data(), minval, varname.Data(), minval+(minval-secondval)/2);
  out += Form("  if(%s > %f) %s = %f;\n", varname.Data(), maxval, varname.Data(), maxval-(maxval-penultval)/2);
  out += Form("  if     (%s < %f) return %f;\n", varname.Data(), secondval, val);
  for(Int_t bin = 2; bin <= nbins; bin++){
    binval = h->GetBinLowEdge(bin+1);
    val = h->GetBinContent(bin);
    out += Form("  else if(%s < %f) return %f;\n", varname.Data(), binval, val);
  }
  out += "  else return 0.;\n}\n";
  cout << out;
  return out;
}

void Print2D(TH2* h, TString name, TString XTitle = "", TString YTitle = ""){
  TCanvas* c = new TCanvas("c","c",10,10,800,600);
  c->SetRightMargin(0.15);
  c->SetGrid(1, 1);
  
  gStyle->SetPalette(1);
  h->SetStats(0);
  h->Draw("colz");
  h->SetTitle("");
  h->GetXaxis()->SetTitle(XTitle);
  h->GetYaxis()->SetTitle(YTitle);
  h->GetYaxis()->SetTitleOffset(1.25);
  
  if(     name.EndsWith(".png")) c->Print(name, "png");
  else if(name.EndsWith(".pdf")) c->Print(name, "pdf");
  else                           c->Print(name+".png", "png");
}


TChain *GetTChain(TString path, TString name, TString treeName = "tree"){
  TChain* t = new TChain(treeName, treeName);
  vector<TString> files = GetAllFiles(path, name);
  for(int i = 0; i < (int) files.size(); i++) t->Add(files.at(i));
  return t;
}



#endif
