TString outputDir = "./Figures/";

void PrintHisto2F(TString fileName, TString hName,TString outname = "histo2D", Bool_t doErrors = true){
  cout << "Opening " << fileName << ", taking histogram " << hName << " and saving it as " << outname << endl;
  TFile* f = TFile::Open(fileName + ".root");
  TH2F* h; f->GetObject(hName, h);
  h->SetStats(0);
  h->SetMinimum(0.8); h->SetMaximum(1.2);

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
  if(doErrors) h->Draw("colz, text, errors");
  else         h->Draw("colz, text");
  gSystem->mkdir(outputDir, kTRUE);
  c->Print( outputDir + outname + ".pdf", "pdf");
  c->Print( outputDir + outname + ".png", "png");
  delete c;
}

void PrintHisto2D(TString fileName, TString hName,TString outname = "histo2D", Bool_t doErrors = true){
  cout << "Opening " << fileName << ", taking histogram " << hName << " and saving it as " << outname << endl;
  TFile* f = TFile::Open(fileName + ".root");
  TH2D* h; f->GetObject(hName, h);
  h->SetStats(0);
  h->SetMinimum(0.92); h->SetMaximum(1.001);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("4.3f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
  if(doErrors) h->Draw("colz, text, errors");
  else         h->Draw("colz, text");
  gSystem->mkdir(outputDir, kTRUE);
  c->Print( outputDir + outname + ".pdf", "pdf");
  c->Print( outputDir + outname + ".png", "png");
  delete c;
}


void PrintGraphAsymmErrors(TString fileName, TString hName,TString outname = "histo2D"){
  TFile* f = TFile::Open(fileName + ".root");
  TGraphAsymmErrors* h; f->GetObject(hName, h);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("4.3f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
    h->Draw("");
    gSystem->mkdir(outputDir, kTRUE);
    c->Print( outputDir + outname + ".pdf", "pdf");
    c->Print( outputDir + outname + ".png", "png");
    delete c;
}


void PrintHistosForTop(){
  const Int_t nHistos = 9;
  TString files[nHistos]  = {"TriggerSF_mumu2016_pt", "TriggerSF_ee2016_pt", "TriggerSF_emu2016_pt", "ElecRecoM17", "MuonSFId_BCDEF", "MuonSFId_GH", "MuonSFIso_BCDEF", "MuonSFIso_GH", "ElecTightCBidM17"};
  TString histos[nHistos] = {"lepton_pt_2D_sf", "lepton_pt_2D_sf", "lepton_pt_2D_sf", "EGamma_SF2D", "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio", "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio", "TightISO_TightID_pt_eta/pt_abseta_ratio", "TightISO_TightID_pt_eta/pt_abseta_ratio", "EGamma_SF2D"};
  TString names[nHistos] = {"TrigMuMu", "TrigEE", "TrigEM", "ElecTracker", "MuonID_BCDEF", "MuonID_GH", "MuonIso_BCDEF", "MuonIso_GH", "ElecIDIso"};

 for(Int_t i = 0; i <       3; i++) PrintHisto2D(files[i], histos[i], names[i], 1);
 for(Int_t i = 3; i < nHistos; i++) PrintHisto2F(files[i], histos[i], names[i], 1);
 PrintGraphAsymmErrors("Tracking_EfficienciesAndSF_BCDEFGH", "ratio_eff_eta3_dr030e030_corr", "MuonTracker");
}

void PrintHistos(){
  PrintHistosForTop();
}
