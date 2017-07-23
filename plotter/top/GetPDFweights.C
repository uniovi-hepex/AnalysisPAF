#include "SetPlotter.C"

using namespace std;

const TString pathsum = "/pool/ciencias/heppyTreesDR76X/5TeV/v3/";
const TString samplename = "TTbar_Powheg";
const int nWeights = 170;

TH1F* hsum;
TFile* inputfile0 = TFile::Open(pathsum + "Tree_" + samplename + ".root");
float fac = 0;

float GetWeight(Int_t index);
double GetWeightSum(Int_t i);
float MEvariations(bool docout = false);
float NNPDFsyst(bool docout = false);
void printNNPDFvariations();
void printNNPDFsyst();
void printw(int i, int inom);



void InitSum(){
	if(fac != 0) return;
	inputfile0->GetObject("hsumW", hsum);
  if(IsFidu) fac = 1;
	else       fac = hsum->GetBinContent(0);
}

double GetWeightSum(Int_t i){ 
	if(IsFidu){
		TString inputfilename = path + "Tree_" + samplename + "Fidu" + ".root";
		TFile* inputfile = TFile::Open(inputfilename);
		TH1F* hsumFidu; inputfile->GetObject("hPDFweightsFidu", hsumFidu); 
		return hsumFidu->GetBinContent(i);
	}
	else return hsum->GetBinContent(i-1);
}

float GetWeight(Int_t index){
  TH1F* hist;
  TString inputfilename = path + "Tree_" + samplename + ".root";
  if(IsFidu) inputfilename = path + "Tree_" + samplename + "Fidu" + ".root";
  TFile* inputfile = TFile::Open(inputfilename);
  inputfile->GetObject("hPDFweights_"+ Chan + "_" + Level,hist);
  float weight = hist->GetBinContent(index)*Lumi;
  inputfile -> Close();
  delete inputfile;
  return weight/GetWeightSum(index)*fac;
}

void line() { cout << "-----------------------------------------------------------------------------------------" << endl;}
void line2(){ cout << "=========================================================================================" << endl;}
void printi(int i){ cout << Form(" i = %3i || ", i);}
void printw(int i, int inom){
  TString lev = Level;
  float v3 = GetWeight(i);
  float e3 = TMath::Abs(GetWeight(inom)-v3)/GetWeight(inom)*100;
  cout << Form("  %6.2f (%2.2f ", v3, e3) << "%)  ";
  if(i == inom) cout << " <---- NOMINAL ";
  cout << endl;
}

float MEvariations(bool docout){
  TString lev = Level;
	InitSum();
	if(docout){
		line2();
		cout << " ### muR/muF ME variations (hdamp = mtop), nominal + 8" << endl;
		line();
		printi(1);  cout << " muR = 1  , muF = 1    || " ; printw( 1 ,1);
		printi(2);  cout << " muR = 1  , muF = 2    || " ; printw( 2 ,1);
		printi(3);  cout << " muR = 1  , muF = 0.5  || " ; printw( 3 ,1);
		printi(4);  cout << " muR = 2  , muF = 1    || " ; printw( 4 ,1);
		printi(5);  cout << " muR = 2  , muF = 2    || " ; printw( 5 ,1);
		printi(6);  cout << " muR = 2  , muF = 0.5  || " ; printw( 6 ,1);
		printi(7);  cout << " muR = 0.5, muF = 1    || " ; printw( 7 ,1);
		printi(8);  cout << " muR = 0.5, muF = 2    || " ; printw( 8 ,1);
		printi(9);  cout << " muR = 0.5, muF = 0.5  || " ; printw( 9 ,1);
		line();
	}
	float nom = 0; float y = 0; float vmax = 0;
	TString o = " Maximum variation                 ";
  for(int i = 0; i < nChannels; i++){
    nom = GetWeight(1); vmax = nom;
    for(int k = 0; k<9; k++) {
       if (TMath::Abs(GetWeight(k+1)-nom) > TMath::Abs(vmax-nom)) vmax = GetWeight(k+1);
    }
	}
	if(docout){  
		o += Form("||  %6.2f (%2.2f ", TMath::Abs(vmax-nom), TMath::Abs(vmax-nom)/nom*100);
		o +=  "%)";
		cout << o << endl;
		line2();
	}
  return vmax-nom;
}

void printNNPDFvariations(){
	InitSum();
  line2();
  cout << " ### NNPDF variations: 100 + 2 (alpha_s) " << endl;
  line();
  for(int i = 10; i<110; i++){
    printi(i); printw(i, 1);
  }
  line();
  cout << " - NNPDF alpha_s variations " << endl;
  line();
  printi(110); printw(110, 1);
  printi(111); printw(111, 1);
  line2();
}

float NNPDFsyst(bool docout){
	InitSum();
  float e = 0; float n = GetWeight(1);
  float y = 0;
  for(int i = 10; i<110; i++){
    y = GetWeight(i);
    e += (y-n)*(y-n);
  }
  float v110 = TMath::Abs(GetWeight(110)-n);
  float v111 = TMath::Abs(GetWeight(111)-n);
  float rms = TMath::Sqrt(e/100);
  if(docout){
    printNNPDFvariations();
    printNNPDFsyst();
  }
  return TMath::Sqrt(rms*rms + ((v110-v111)*0.75/2)*((v110-v111)*0.75/2));
}

void printNNPDFsyst(){
  float nem = GetWeight(1); float eem = NNPDFsyst();
  line2();
  cout << " >>>> NNPDF systematic uncertainty" << endl;
  cout << " Evaluated by taking the RMS under the 100 weights" << endl;
  cout << " Alpha_s variations are added in quadrature after rescaling by 0.75" << endl;
  cout << " The formula is: sqrt(RMS^2 + ((alphas var 1 - alphas var 2)*0.75/2)^2 )" << endl;
  line(); 
  cout << Form("      %4.2f (%2.3f ", eem, eem/nem*100) << "%)  " << endl;
  line2();
}

void GetPDFweights(){
 MEvariations(1); 
 NNPDFsyst(1);
}
