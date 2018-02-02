TString path = "/mnt_pool/ciencias_users/user/juanr/AnalysisPAF/plotter/output/Datacards_jan5/";
//TString path = "/mnt_pool/ciencias_users/user/juanr/CMSSW_8_1_0/src/StopDatacards/jan5/";
TH1F* GetHisto(TString var = "", TString bkg = "ttbar", TString filename = "MT2", bool norm = true);
void DrawSystematic(TString var, TString bkg = "ttbar", TString filename = "MT2", TString xtit = "M_{T2} (GeV)", TString varname = "Unclustered MET", float lsize = 0.045, float lx0 = 0.65, float ly0 = 0.7, float lx1 = 0.9, float ly1 = 0.9);

void DrawSyst(){
  DrawSystematic("unclMET", "ttbar", "MT2_21");
  //DrawSystematic("uncl", "ttbar", "MET");
}

void DrawSystematic(TString var, TString bkg, TString filename, TString xtit, TString varname, float lsize, float lx0, float ly0, float lx1, float ly1){
  TH1F* hnom; TH1F* hup; TH1F* hdo;

  hnom = GetHisto("", bkg, filename);
  hnom->SetLineColor(1);

  hnom->GetXaxis()->SetTitle("");
  hnom->GetYaxis()->SetTitle("Events");
  hnom->GetYaxis()->SetTitleOffset(1.2);
  hnom->GetXaxis()->SetLabelSize(0);
  hnom->GetYaxis()->SetTitleSize(0.16);
  hnom->GetYaxis()->SetLabelSize(0.05);

  hup = GetHisto(var + "Up",   bkg, filename);
  hdo = GetHisto(var + "Down", bkg, filename);
  hup->SetLineColor(kGreen);
  hdo->SetLineColor(kRed+1);

  TCanvas* c= new TCanvas("c","c",10,10,800,600);
  c->Divide(1,2);

  TPad* plot = (TPad*)c->GetPad(1);
  plot->SetPad(0.0, 0.23, 1.0, 1.0);
  plot->SetTopMargin(0.06); plot->SetRightMargin(0.02);

  TPad* pratio = (TPad*)c->GetPad(2);
  pratio->SetPad(0.0, 0.0, 1.0, 0.29);
  pratio->SetGridy();// pratio->SetGridx();
  pratio->SetTopMargin(0.03); pratio->SetBottomMargin(0.4); pratio->SetRightMargin(0.02);

  plot->cd();
  hnom->Draw("pl"); hup->Draw("same"); hdo->Draw("same");

  TH1F* hraup = (TH1F*) hup->Clone("hraup"); hraup->Divide(hnom);
  TH1F* hrado = (TH1F*) hdo->Clone("hrado"); hrado->Divide(hnom);
  hraup->SetMinimum(0.8); hraup->SetMaximum(1.2);
  hraup->GetXaxis()->SetTitleOffset(1.2);
  hraup->GetXaxis()->SetTitleSize(0.16);
  hraup->GetXaxis()->SetLabelOffset(0.02);
  hraup->GetXaxis()->SetLabelSize(0.16);
  hraup->GetYaxis()->SetTitleOffset(0.27);
  hraup->GetYaxis()->CenterTitle();
  hraup->GetYaxis()->SetTitleSize(0.16);
  hraup->GetYaxis()->SetLabelSize(0.13);
  hraup->GetYaxis()->SetNdivisions(404);
  hraup->GetXaxis()->SetTitle(xtit);
  hraup->GetYaxis()->SetTitle("Var/Nom");
  
  float x0, x1, y0, y1;

  TLegend* l = new TLegend(lx0, ly0, lx1, ly1);
  l->SetTextSize(lsize);
  l->AddEntry(hnom, "Nominal", "l");
  l->AddEntry(hup , varname + " Up",   "l");
  l->AddEntry(hdo , varname + " Down", "l");
  l->SetBorderSize(0);
  l->SetFillColor(10);
  l->Draw("same");

  pratio->cd();
  pratio->SetGrid(1,1);
  hraup->Draw(); hrado->Draw("same");

  c->Print(bkg + "_" + var + ".png", "png");
  c->Print(bkg + "_" + var + ".pdf", "pdf");
  plot->SetLogy();
  c->Print(bkg + "_" + var + "_log.png", "png");
  c->Print(bkg + "_" + var + "_log.pdf", "pdf");
}

TH1F* GetHisto(TString var, TString bkg, TString file, bool norm){
  if(file.EndsWith(".root")) file = file(0, file.Sizeof()-6);
  TFile* f = TFile::Open(path + file + ".root");
  TString name = bkg;
  if(var != "") name += "_" + var;
  TH1F* h; f->GetObject(name, h);
  h->SetDirectory(0);
  delete f;
  h->SetStats(0); h->SetFillColor(0);
  h->SetLineWidth(2); h->SetTitle("");
  if(norm) h->Scale(1/h->Integral());
  return h;
}
