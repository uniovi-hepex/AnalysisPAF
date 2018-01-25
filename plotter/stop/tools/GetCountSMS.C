#include "FunctionsPAF.C"

float mChiMin  = 0;    float mChiMax  = 82.5;
float mStopMin = 175 - 7.5;  float mStopMax = 250+7.5;

void GetCountSMS(TString path, TString name){
  TChain* t = GetTChain(path, name);

  TTreeReader reader(t);
  TTreeReaderValue<int> bMStop(reader, "GenSusyMStop");
  TTreeReaderValue<int> bMChi(reader, "GenSusyMNeutralino");

  int nx = (mStopMax - mStopMin)/7.5;
  int ny = (mChiMax  - mChiMin)/7.5;
  TH2F *h = new TH2F("CountSMS", "CountSMS", nx, mStopMin, mStopMax, ny, mChiMin, mChiMax);

  int mStop; int mChi;
  while(reader.Next()) {
    mStop = *bMStop.Get();
    mChi  = *bMChi.Get();
    h->Fill(mStop, mChi);
  }
  
  TFile* out = new TFile("CountSMS.root", "recreate");
  h->Write();

  h->GetYaxis()->SetNdivisions(ny, 0);
  h->GetXaxis()->SetNdivisions(nx, 0);
  h->GetXaxis()->SetLabelSize(0.03);
  h->GetXaxis()->CenterLabels();
  h->GetYaxis()->CenterLabels();
  
  Print2D(h, "StopMass.png", "Stop mass [GeV]", "Neutralino mass [GeV]");
}
 
void DrawNGenLeps(){
  TChain* t = GetTChain(path, name);

  TTreeReader reader(t);
  TTreeReaderValue<int> bngenLep(       reader, "ngenLep");
  TTreeReaderValue<int> bngenLepFromTau(reader, "ngenLepFromTau");
  TTreeReaderValue<int*> bGenLepModethID(reader, "genLep_motherId");
  TTreeReaderValue<int*> bGenLepGModethID(reader, "genLep_grandmotherId");
  TTreeReaderValue<int*> bGenLepFromTauGMotherId(reader, "genLepFromTau_grandmotherId");
 
 TH1F* h = new TH1F("ngenleps", ",n Gen Leptons", "Events", 4, 0, 3); 
 int ngenLep; int nleps;
 int ngenLepFromTau; int genLepMID; int genLepFromTauGMID; int genLepGMID;

 while(reader.Next()) {
   ngenLep        = *bngenLep.Get();
   ngenLepFromTau = *bngenLepFromTau.Get();
   for(int i = 0; i < ngenLep; i ++){
     genLepMID = fabs(*bGenLepModethID.Get()[i]);
   }
   //h->Fill(nleps);
 }
}

  
