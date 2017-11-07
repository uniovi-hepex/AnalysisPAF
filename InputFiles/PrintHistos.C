
void PrintHistos(TString fileName, TString hName,TString outname = "histo2D", Bool_t doErrors = true){
  TString outputDir = "";
  if(outname.Contains("/")) outputDir = outname(0, outname.Last('/'));
  cout << "Opening " << fileName << ", taking histogram " << hName << " and saving it as " << outname << endl;
  TFile* f = TFile::Open(fileName + ".root");
  TH2F* h; f->GetObject(hName, h);
  h->SetStats(0);
  h->SetMinimum(0.); //h->SetMaximum();

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("1.0f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
  if(doErrors) h->Draw("colz, text, errors");
  else         h->Draw("colz, text");
  if(outputDir != "") gSystem->mkdir(outputDir, kTRUE);
  c->Print( outname + ".pdf", "pdf");
  c->Print( outname + ".png", "png");
  delete c;
}

void PrintHisto2D(TString fileName, TString hName,TString outname = "histo2D", Bool_t doErrors = true){
  TString outputDir = "";
  if(outname.Contains("/")) outputDir = outname(0, outname.Last('/'));
  cout << "Opening " << fileName << ", taking histogram " << hName << " and saving it as " << outname << endl;
  TFile* f = TFile::Open(fileName + ".root");
  TH2D* h; f->GetObject(hName, h);
  h->SetStats(0);
  h->SetMinimum(0.); h->SetMaximum(2);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("4.0f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
  if(doErrors) h->Draw("colz, text, errors");
  else         h->Draw("colz, text");
  if(outputDir != "") gSystem->mkdir(outputDir, kTRUE);
  c->Print( outname + ".pdf", "pdf");
  c->Print( outname + ".png", "png");
  delete c;
}


void PrintGraphAsymmErrors(TString fileName, TString hName,TString outname = "histo2D"){
  TString outputDir = "";
  if(outname.Contains("/")) outputDir = outname(0, outname.Last('/'));
  TFile* f = TFile::Open(fileName + ".root");
  TGraphAsymmErrors* h; f->GetObject(hName, h);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("4.3f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
    h->Draw("");
    gSystem->mkdir(outputDir, kTRUE);
    c->Print( outname + ".pdf", "pdf");
    c->Print( outname + ".png", "png");
    delete c;
}

TH2F* GetSFfromTH2F(TString path, TString file1, TString file2, TString hname1, TString hname2 = "", TString outname = "SF", Bool_t doErrors = false){
  TString outputDir = "";
  if(outname.Contains("/")) outputDir = outname(0, outname.Last('/'));
  if(hname2 == "") hname2 = hname1;
  if(file1.EndsWith(".root")) file1.ReplaceAll(".root", "");
  if(file2.EndsWith(".root")) file2.ReplaceAll(".root", "");
  cout << "Opening " << path + file1 + ".root, taking histogram " << hname1 << "..." << endl; 
  cout << "Opening " << path + file2 + ".root, taking histogram " << hname2 << "..." << endl; 
  TFile* f1 = TFile::Open(path + file1 + ".root");
  TH2F*  h1; f1->GetObject(hname1, h1);
  TFile* f2 = TFile::Open(path + file2 + ".root");
  TH2F*  h2; f2->GetObject(hname2, h2);
  h1->SetStats(0); h1->SetMinimum(0.); //h->SetMaximum();
  h1->Scale(1/h1->Integral());  h2->Scale(1/h2->Integral());

  TH2F* hratio = (TH2F*) h1->Clone("ratio");
  hratio->Divide(h2);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("1.2f");

  TCanvas* c= new TCanvas("c","c",10,10,800*1.5,600*1.5);
  if(doErrors) hratio->Draw("colz, text, errors");
  else         hratio->Draw("colz, text");
  if(outputDir != "") gSystem->mkdir(outputDir, kTRUE);
  c->Print( outname + ".pdf", "pdf");
  c->Print( outname + ".png", "png");
  delete c;
  return hratio;
}

