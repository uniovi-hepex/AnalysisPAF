#ifndef PDFunc_h 
#define PDFunc_h 1

#include "Histo.h"
#include "Looper.h"
#include <iostream>
#include <fstream>

const Int_t totalNweights = 254;
const TString hSumGenWeightsName = "SumGenWeights";

//#### Extra functions
Int_t GetLHEid(Int_t i);
Int_t GetLHEidNom(Int_t i);
//_________________________________________________________

class PDFunc{
  /*
 * Esta clase, a partir de un mini tree, lee toda la información necesaria
 * para calcular incertidumbres de PDF y ME scale
 *
 * Se necesita:
 *   Un histograma con la suma de pesos generados
 *   Un histograma con la suma de pesos a nivel fiducial (si se quiere calcular para sucesos fiduciales)
 *   Una branch con los pesos para cada suceso
 *   Un histograma con el Count
 *
 * Se puede elegir enetre varios metodos para PDF y ME scale (work in progress)
 * Si se quiere hace un sistematico de shape, se necesita el input de los bines y la variable
 *
 * 
 *  How to:
 *  
 *  // Inclusivo:
 *  PDFunc *pdf = new PDFunc(path, sample, "tree", "TNJets >= 2 && TNBtags >= 1", "ElMu");
 *  pdf->PrintLHE();
 *  Float_t pdf_syst   = pdf->NNPDFsyst();
 *  Float_t scale_syst = pdf->MEsyst();
 * 
 *  // Shape:
 *  PDFunc *pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree,  cut, chan, var, nbins, bin0, binN);
 *  Histo* hUp =   pdf->GetSystHisto("up",   "pdf");
 *  Histo* hDown = pdf->GetSystHisto("down", "pdf");
 *
 *  // Print everything:
 *  for(Int_t bin = 0; bin < pdf->GetNbins(); bin++) PrintLHE(bin);
 *  // If you want to print it into a txt file, add an argument "outputText.txt"
 *  // at the end of the constructor.
 *
 */

public:
  PDFunc(TString path, TString sample, TString tn = "tree", TString _cut = "1", TString _chan = "ElMu", TString filename = ""){
    pathToTree = path;
    sampleName = sample;
    treeName = tn; 
    cut = _cut; chan = _chan;
    nBins = 1; var = "";
    saveoutput = filename;
    Init();
    FillHistoWeights();
  }
  PDFunc(TString path, TString sample, TString tn,  TString _cut, TString _chan, TString _var, Int_t nb, Float_t b0 = 0, Float_t bN = 300, TString filename = ""){
    pathToTree = path;
    sampleName = sample;
    treeName = tn;
    var = _var; cut = _cut; chan = _chan;
    nBins = nb;  bin0 = b0; binN = bN;
    saveoutput = filename;
    Init();
    FillHistoWeightsAllBins();
  }
  PDFunc(TString path, TString sample, TString tn, TString _cut, TString _chan, TString _var, Int_t nb, Float_t *b0 = 0, TString filename = ""){
    pathToTree = path;
    sampleName = sample;
    treeName = tn;
    var = _var; cut = _cut; chan = _chan;
    nBins = nb;  bin0 = 0; binN = 0; bins = b0; 
    saveoutput = filename;
    Init();
    FillHistoWeightsAllBins();
  }
  ~PDFunc(){
    if(weights) delete weights;
    if(hSumGenWeights) delete hSumGenWeights;
    if(hSumFiduWeights) delete hSumFiduWeights;
    if(FVar) delete FVar; 
    if(FCut) delete FCut; 
    if(FLHEweight) delete FLHEweight;
    wBins.clear();
  };
  void Init(){
    Lumi = 35850;
    hSumGenWeights = nullptr;
    hSumFiduWeights = nullptr;
    weights = nullptr;
    FVar = nullptr; FCut = nullptr; FLHEweight = nullptr; FCutBin = nullptr;
    bincut = "1";
    weight = "TWeight";
    LHEweighName = "TLHEWeight";
    wBins.clear();
    verbose = true;

    tree = (TTree*) loadTree(pathToTree, sampleName, treeName)->Clone("mytree");
    nEntries = tree->GetEntries();
    hSumGenWeights = loadHistogram1D(pathToTree, sampleName, "CountLHE");
    TH1D* hCount = loadHistogram1D(pathToTree, sampleName, "Count");
    count = hCount->GetBinContent(1);
    SetFormulas();
    if(saveoutput != "") os = ofstream(saveoutput);
  }

  Bool_t verbose;

  void SetPathToTree(TString t){ pathToTree = t;}
  void SetTreeName(TString t){ treeName = t;}
  void SetCut(TString t){ cut = t;}
  void SetVar(TString t){ var = t;}
  void SetChan(TString t){ chan = t;}
  void SetWeight(TString t){ weight = t;}

  void SetLHEwNameInTree(TString t){ LHEweighName = t;}
  void SetLHEsumFiduInTree(TString t){ SumFiduWeightsName = t;}

  Int_t GetNbins(){ return nBins;}

  void CreateHisto();
  TString GetBinCut(Int_t bin);

  void SetHistoWeights(Int_t i = 0);
  void SetFormulas();
  Histo* GetSystHisto(TString dir = "up", TString systname = "pdf");
  void SetBinCutFormula(Int_t bin = 1);
  Float_t GetNormWeight(Int_t i = 1);
  void FillHistoWeights(Int_t bin = 0);
  void FillHistoWeightsAllBins();
  void SetBin(Int_t bin);

  Float_t GetWyield(Int_t i);
  Float_t GetNomYield(Int_t i);

  Float_t NNPDFsyst();
  Float_t MEsyst();

  void line();
  void line2();
  void printi(Int_t i);
  void printv(Int_t i);
  void PrintInfo(Int_t bin = 0);
  void PrintWeightHisto(Int_t bin = 0);
  void PrintSumOfWeightsHisto(){ hSumGenWeights->Draw();}
  void PrintMuRMuFvariations();
  void PrintNNPDFvariations();
  void PrintNNPDFsyst();
  void PrintLHE(Int_t bin = 0);

protected:
  // Main tree and histograms to open
  TH1D* hSumGenWeights; TH1D* hSumFiduWeights; TTree* tree; Int_t nEntries;
  TString SumFiduWeightsName; Float_t nomyield;
  TString LHEweighName;
  TString saveoutput;
  ofstream os;
  
  // Here we store all the LHE-weighted events
  TH1F* weights; vector<TH1F*> wBins; Double_t count;

  // Info about the histogram
  Int_t nBins; Float_t bin0; Float_t binN; Float_t *bins; Float_t Lumi;

  // Info about paths and file names
  TString pathToTree; TString sampleName; TString treeName;

  // Info about variable, cut, channel...
  TString var; TString cut; TString chan; TString bincut; TString weight;
  
  // The varied histogram --> Must be returned if asked for a shape variation
  //Histo* SystHisto; 

  // Formulas to use in the loop
  TTreeFormula* FVar; TTreeFormula* FCut; TTreeFormula* FLHEweight; TTreeFormula* FCutBin;
};

void PDFunc::SetHistoWeights(Int_t bin){
  if(verbose) cout << "[PDFunc::SetHistoWeights] Setting histo weights for bin " << bin << endl;
  //if(weights) delete weights;
  weights = new TH1F(Form("weights_%i",bin), Form("weights_%i",bin), totalNweights, 0, totalNweights);
}

void PDFunc::SetFormulas(){
//  if(FVar)        delete FVar;
//  if(FCut)        delete FCut;
//  if(FLHEweight)  delete FLHEweight;
 
  TString strcut = CraftFormula(cut, chan, "", weight, tree);
  if(verbose) cout << "[PDFunc::SetFormulas] Cut: " << strcut << endl;
  FCut       = new TTreeFormula("Form_cut", strcut, tree);//GetFormula("Form_cut", strcut, tree);
  FLHEweight = new TTreeFormula("Form_LHEweight", LHEweighName, tree);//GetFormula("Form_cut", strcut, tree);
}

void PDFunc::SetBinCutFormula(Int_t bin){
  if(FCutBin) delete FCutBin;
  Float_t downbin; Float_t upbin;
  if(bin0 == binN){ // Bins are in array!!
    downbin = bins[bin-1];
    upbin   = bins[bin];
  }
  else{
    downbin = bin0 + (binN - bin0)/(nBins)*(bin-1); 
    upbin   = bin0 + (binN - bin0)/(nBins)*(bin); 
  }
  bincut = Form("((%s >= %f) && (%s < %f))", var.Data(), downbin, var.Data(), upbin);
  if(verbose) cout << "[PDFunc::SetBinCutFormula] Creating formula with bincut: " << bincut << endl;
  FCutBin = new TTreeFormula(Form("Form_bincut_%i", bin), bincut, tree);
}

void PDFunc::FillHistoWeights(Int_t bin){
  SetHistoWeights(bin);
  Float_t weight=0; Float_t normWeight; Float_t LHEweight;
  Float_t EventWeight;  Float_t bincutWeight;

  if(bin != 0) SetBinCutFormula(bin);
  if(verbose) PrintInfo(bin); // Print all info

  // Loop over all entries
  for(Long64_t i = 0; i < nEntries; i++){
    tree->GetEntry(i);
    weight = FCut->EvalInstance();
    if(bin != 0){
      bincutWeight = FCutBin->EvalInstance();
      weight *= bincutWeight;
    }
    FLHEweight->GetNdata();

    // Fill histo for each weight
    for(Int_t k = 0; k < totalNweights; k++){
      LHEweight = FLHEweight->EvalInstance(k); 
      normWeight = GetNormWeight(k);
      EventWeight = weight*LHEweight*normWeight;
      weights->Fill(k, EventWeight);
    }  // Close loop over LHE weights
  } // Close loop over events
  wBins.push_back(weights);
}

void PDFunc::FillHistoWeightsAllBins(){
  for(Int_t i = 1; i <= nBins; i++) FillHistoWeights(i);
}

void PDFunc::PrintWeightHisto(Int_t bin){
  TH1F* hist;
  if(bin == 0 || var == ""){
    hist = weights;
  }
  else{
    hist = wBins.at(bin); 
  }
  hist->Draw();
}

Histo* PDFunc::GetSystHisto(TString dir, TString systname){
  Histo* SystHisto;
  if(bin0 != binN) SystHisto = new Histo(TH1F("SystHisto_" + systname + "_" + dir, var, nBins, bin0, binN));
  else             SystHisto = new Histo(TH1F("SystHisto_" + systname + "_" + dir, var, nBins, bins));
  SystHisto->Init();
  Float_t variation;
  if((Int_t) wBins.size() < nBins) cout << "[PDFunc::GetSystHisto] WARNING: histograms unfilled!" << endl;
  else{
    for(Int_t i = 0; i < nBins; i++){
      SetBin(i);
      nomyield = GetNomYield(1);
      if(systname == "pdf" || systname == "PDF")
        variation = (dir == "Up" || dir == "up")? nomyield + NNPDFsyst() : nomyield - NNPDFsyst();
      else
        variation = (dir == "Up" || dir == "up")? nomyield + MEsyst() : nomyield - MEsyst();
      if(dir == "0" || dir == "nom" || dir == "nominal") variation = nomyield;
      SystHisto->SetBinContent(i+1, variation);
    }
  }
  return SystHisto; 
}

void PDFunc::SetBin(Int_t bin){
  weights = wBins.at(bin);
}

Float_t PDFunc::GetNormWeight(Int_t i){
  // i starts in 0 !!
// You should multiply by this!
  Float_t nom = 0; Float_t weight = 0;
  Int_t bin    = GetLHEid(i);
  //Int_t binNom = GetLHEidNom(i);

  //nom    = hSumGenWeights->GetBinContent(hSumGenWeights->FindBin(binNom));
  weight = hSumGenWeights->GetBinContent(hSumGenWeights->FindBin(bin)); 
  return count/weight;
}

Float_t PDFunc::GetWyield(Int_t i){
  // From histo with weighted yields
  // Starts with 0!! so 0 is first bin, fuctions would check bin 0+1
  Float_t w =  weights->GetBinContent(i+1);
  return w*Lumi;
}

Float_t PDFunc::GetNomYield(Int_t i){
  Int_t bin = 0;
  if      (i<112) bin = 1;
  else if (i<166) bin = 112;
  else if (i<222) bin = 166;
  else if (i<249) bin = 222;
  Float_t w = weights->GetBinContent(bin);
  return w*Lumi;
}


//#############################################################################################
//
// Methods...
Float_t PDFunc::NNPDFsyst(){
  Float_t e = 0; float n = GetWyield(0);
  float y = 0;
  for(int i = 10; i<110; i++){
    y = GetWyield(i);
    e += (y-n)*(y-n);
  }
  float v110 = TMath::Abs(GetWyield(110)-n);
  float v111 = TMath::Abs(GetWyield(111)-n);
  float rms = TMath::Sqrt(e/100);
  return TMath::Sqrt(rms*rms + ((v110-v111)*0.75/2)*((v110-v111)*0.75/2));
}

Float_t PDFunc::MEsyst(){
  float nom = 0; float y = 0; float vmax = 0;
  nom = GetWyield(0); vmax = nom;
  for(int k = 0; k<9; k++) {
    if (TMath::Abs(GetWyield(k)-nom) > TMath::Abs(vmax-nom)) vmax = GetWyield(k);
  }
  return  TMath::Abs(vmax-nom);
}

//#############################################################################################
//
// Printing...
void PDFunc::PrintInfo(Int_t bin){
  if(bin == 0){
    cout << "nEntries:     "    << nEntries << endl;
    cout << "Selection:    "    << cut   << endl;
    cout << "Channel:      "    << chan  << endl;
    cout << "Event weight: "    << weight << endl;
    if(saveoutput != ""){
      os << "nEntries:     "    << nEntries << endl;
      os << "Selection:    "    << cut   << endl;
      os << "Channel:      "    << chan  << endl;
      os << "Event weight: "    << weight << endl;
    }
    if(var == "") cout << " Getting weights inclusively for the selection above. " << endl; 
  }
  else if(bin == 1){
    if(bin0 == binN){
        cout << "Getting weights for different bins... " << endl;
        cout << "nBins    = " << nBins << endl;
        cout << "Variable = " << var << endl;
        cout << Form("\n=========== BIN %i ===========\n", bin);
        cout << "Bincut   = " << bincut << endl;
      if(saveoutput != ""){
        os << "Getting weights for different bins... " << endl;
        os << "nBins    = " << nBins << endl;
        os << "Variable = " << var << endl;
        os << Form("\n=========== BIN %i ===========\n", bin);
        os << "Bincut   = " << bincut << endl;
      }
    }
    else{
        cout << "Getting weights for different bins... " << endl;
        cout << "nBins = " << nBins << ", bin0     = " << bin0 << ", binN = " << binN << endl;
        cout << "Variable = " << var << endl;
        cout << Form("\n=========== BIN %i ===========\n", bin);
        cout << "Bincut   = " << bincut << endl;
      if(saveoutput != ""){
        os << "Getting weights for different bins... " << endl;
        os << "nBins = " << nBins << ", bin0     = " << bin0 << ", binN = " << binN << endl;
        os << "Variable = " << var << endl;
        os << Form("\n=========== BIN %i ===========\n", bin);
        os << "Bincut   = " << bincut << endl;
      }
    }
  }
  else{
      cout << Form("\n=========== BIN %i ===========\n", bin);
      cout << "Bincut   = " << bincut << endl;
    if(saveoutput != ""){
      os << Form("\n=========== BIN %i ===========\n", bin);
      os << "Bincut   = " << bincut << endl;
    }
  }
}

void PDFunc::PrintMuRMuFvariations(){
  TString muRmuFtext[9] ={
    " muR = 1  , muF = 1    (nominal)    || ", 
    " muR = 1  , muF = 2                 || ", 
    " muR = 1  , muF = 0.5               || ", 
    " muR = 2  , muF = 1                 || ", 
    " muR = 2  , muF = 2                 || ", 
    " muR = 2  , muF = 0.5  (unphysical) || ", 
    " muR = 0.5, muF = 1                 || ", 
    " muR = 0.5, muF = 2    (unphysical) || ", 
    " muR = 0.5, muF = 0.5               || " 
  };
  line2();
  cout << " ### muR/muF ME variations (hdamp = mtop), nominal + 8" << endl;
  if(saveoutput != "") os << " ### muR/muF ME variations (hdamp = mtop), nominal + 8" << endl;
  line();
  float nom = 0; float y = 0; float vmax = 0;
  TString o = " Maximum variation                                 ";
  nom = GetWyield(0); vmax = nom;
  for(int k = 0; k<9; k++) {
    printi(k);  cout << muRmuFtext[k];  if(saveoutput != "") os << muRmuFtext[k]; printv(k);
    if (TMath::Abs(GetWyield(k)-nom) > TMath::Abs(vmax-nom)) vmax = GetWyield(k);
  }
  line();
    o += Form("||  %6.2f (%2.2f %c)  ", TMath::Abs(vmax-nom), TMath::Abs(vmax-nom)/nom*100, '%');
  cout << o << endl;
  if(saveoutput != "") os << o << endl;
  line2();
}

void PDFunc::PrintNNPDFvariations(){
  line2();
  cout << " ### NNPDF variations: 100 + 2 (alpha_s) " << endl;
  if(saveoutput != "") os << " ### NNPDF variations: 100 + 2 (alpha_s) " << endl;
  line();
  for(int i = 9; i<109; i++){
    printi(i); printv(i);
  }
  line();
  cout << " - NNPDF alpha_s variations " << endl;
  if(saveoutput != "") os << " - NNPDF alpha_s variations " << endl;
  line();
  printi(109); printv(109);
  printi(110); printv(110);
  line2();
}

void PDFunc::PrintNNPDFsyst(){
  float nom = GetWyield(1); float val = NNPDFsyst();
  line2();
  cout << " >>>> NNPDF systematic uncertainty" << endl;
  cout << " Evaluated by taking the RMS under the 100 weights" << endl;
  cout << " Alpha_s variations are added in quadrature after rescaling by 0.75" << endl;
  cout << " The formula is: sqrt(RMS^2 + ((alphas var 1 - alphas var 2)*0.75/2)^2 )" << endl;
  if(saveoutput != ""){
    os << " >>>> NNPDF systematic uncertainty" << endl;
    os << " Evaluated by taking the RMS under the 100 weights" << endl;
    os << " Alpha_s variations are added in quadrature after rescaling by 0.75" << endl;
    os << " The formula is: sqrt(RMS^2 + ((alphas var 1 - alphas var 2)*0.75/2)^2 )" << endl;

  }
  line(); 
  cout << Form("  ====>   %4.2f (%2.3f %c)  ", val, val/nom*100, '%') << endl;
  if(saveoutput != "") os << Form("  ====>   %4.2f (%2.3f %c)  ", val, val/nom*100, '%') << endl;
  line2();
}

void PDFunc::PrintLHE(Int_t bin){
  SetBin(bin);
  cout << Form("\n========== bin %i ==========\n", bin);
  if(saveoutput != "") os << Form("\n========== bin %i ==========\n", bin);
  PrintMuRMuFvariations();
  PrintNNPDFvariations();
  PrintNNPDFsyst();
}

//#############################################################################################
//
// Extra finctions

void PDFunc::line() { 
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  if(saveoutput != "") os << "--------------------------------------------------------------------------------------------------------" << endl;
}
void PDFunc::line2(){ 
  cout << "========================================================================================================" << endl;
  if(saveoutput != "") os << "========================================================================================================" << endl;
}
void PDFunc::printi(Int_t i){ 
  cout << Form("i = %i, LHE id = %5i    |  ", i, GetLHEid(i));
  if(saveoutput != "") os << Form("i = %i, LHE id = %5i    |  ", i, GetLHEid(i));
}
void PDFunc::printv(Int_t i){ 
  cout << Form(" %6.2f (%2.2f %c) ", GetWyield(i), (GetWyield(i)-GetNomYield(i))/GetNomYield(i)*100, '%');
  if(saveoutput != "") os << Form(" %6.2f (%2.2f %c) ", GetWyield(i), (GetWyield(i)-GetNomYield(i))/GetNomYield(i)*100, '%');
  if(GetLHEid(i) == GetLHEidNom(i)){
    cout << " <--- Nominal" << endl;
    if(saveoutput != "") os << " <--- Nominal" << endl;
  }
  else{
    cout << endl;
    if(saveoutput != "") os << endl;
  }
}

Int_t GetLHEid(Int_t i){
  // i starts in 0 !!
  Int_t bin = 0;
  if      (i<9 )  bin = i +  1001;   // 1002-1010: muRmuF
  else if (i<111) bin = i +  1992;   // 2002-2103: NNPDF
  else if (i<166) bin = i +  2890;   // 3002-3056: CT10
  else if (i<222) bin = i +  3835;   // 4000-4057: MMHT2014
  else if (i<249) bin = i +  4779;   // 5002-5028: muRmuF, hdamp 
  return bin;
}

Int_t GetLHEidNom(Int_t i){
  // i starts in 0 !!
  Int_t bin = 0;
  if      (i<111) bin = 0 + 1001;   // 1002-1010: muRmuF
  else if (i<166) bin = 165 + 2890;   // 3002-3056: CT10
  else if (i<222) bin = 221 + 3835;   // 4000-4057: MMHT2014
  else if (i<249) bin = 248 + 4779;   // 5002-5028: muRmuF, hdamp 
  return bin;
}


#endif
