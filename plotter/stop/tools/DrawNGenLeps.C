#include "FunctionsPAF.C"

void DrawNGenLeps(TString path, TString name){
  TChain* t = GetTChain(path, name);

  TTreeReader reader(t);
  TTreeReaderValue<int> bngenLep(       reader, "ngenLep");
  TTreeReaderValue<int> bngenLepFromTau(reader, "ngenLepFromTau");
  TTreeReaderValue<int> bGenLepMID(reader, "genLep_motherId");
  TTreeReaderValue<int> bGenLepGMID(reader, "genLep_grandmotherId");
  TTreeReaderValue<int> bGenLepFromTauGMotherId(reader, "genLepFromTau_grandmotherId");

  TH1F* h = new TH1F("ngenleps", ";n Gen Leptons;Events", 4, 0, 3); 
  int ngenLep; int nleps;
  int ngenLepFromTau; int genLepMID; int genLepFromTauGMID; int genLepGMID;

  while(reader.Next()) {
    ngenLep        = *bngenLep.Get();
    ngenLepFromTau = *bngenLepFromTau.Get();
    nleps = 0;
    for(int i = 0; i < ngenLep; i ++){
      genLepMID         = abs(bGenLepMID.Get()[i]);
      genLepGMID        = abs(bGenLepGMID.Get()[i]);
      if( (genLepMID == 23 || genLepMID == 24) && (genLepGMID == 6)) nleps++;
    }
    for(int i = 0; i < ngenLepFromTau; i++){
      genLepFromTauGMID = abs(bGenLepFromTauGMotherId.Get()[i]);
      if(genLepFromTauGMID == 6) nleps++;
    }
    h->Fill(nleps);
  }
  h->Draw();

}


