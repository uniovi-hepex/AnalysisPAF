TString path = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/";
TString sample = "TTbar_Powheg";
TString treeName = "tree";
TString outName = "TriggerEffStop";

TFile* f; TTree* t; Int_t nEntries;

TH2F* H_All;
TH2F* H_Pass;
TH2F* H_Trig;

Int_t nbins = 7;
Float_t binsPt1[8]   = {25, 30, 40, 60, 80, 100, 150, 200};
Float_t binsPt2[8]   = {20, 30, 40, 60, 80, 100, 150, 200};
Float_t binsEta1[8]  = {0, 0.5,  1,1.2,1.5,1.8, 2.0,  2.4};

Float_t Lep0Pt;  Float_t Lep1Pt;
Float_t Lep0Eta; Float_t Lep1Eta;
Int_t   PassTrigger;

void GetTriggerEfficiencies(){

  cout << " >> Opening sample " << sample << endl;
  cout << "    in dir " << path << endl;
  f = TFile::Open(path + "/Tree_" + sample + ".root");
  t = (TTree*) f->Get(treeName);
  nEntries = t->GetEntries();
  Int_t nPass = 0;
  cout << " >> Number of entries = " << nEntries << endl;

  t->SetBranchAddress("TLep0_Pt", &Lep0Pt);
  t->SetBranchAddress("TLep1_Pt", &Lep1Pt);
  t->SetBranchAddress("TLep0_Eta", &Lep0Eta);
  t->SetBranchAddress("TLep1_Eta", &Lep1Eta);
  t->SetBranchAddress("TPassTrigger", &PassTrigger);

  H_All  = new TH2F("All",  "All",  nbins, binsPt1, nbins, binsPt2);
  H_Pass = new TH2F("Pass", "Pass", nbins, binsPt1, nbins, binsPt2);
  //H_Trig = new TH2F("Trig", "Trig", nbins, binsPt1, nbins, binsPt2);

  cout << " >> Starting loop..." << endl;
  Int_t divisor = Int_t(Float_t(nEntries)/10);
  for(Int_t i = 0; i < nEntries; i++){
    t->GetEntry(i);
    if(i%divisor == 0) cout << " >> Progress... " << i/divisor*10 << " %" << endl;

    H_All ->Fill(Lep0Pt, Lep1Pt, 1);
    if(PassTrigger){
      H_Pass->Fill(Lep0Pt, Lep1Pt, 1);
      nPass++;
    }
  }
  cout << " >> Done! Integrated efficiency = " << Float_t(nPass)/nEntries << endl;
  cout << " >> Saving histograms in output file: " << outName + ".root" << endl;

  H_Trig = (TH2F*) H_Pass->Clone("trig");
  H_Trig->Divide(H_All);
  
  TFile* out = new TFile(outName + ".root","recreate"); 
  H_All->Write();
  H_Pass->Write();
  H_Trig->Write();
 
}
