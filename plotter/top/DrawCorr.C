const TString path     = "/pool/ciencias/userstorage/juanr/top/oct10/forFSRJECCorr/";
const TString pathBtag = "/pool/ciencias/userstorage/juanr/top/oct10/forFSRbtagCorr/";
const TString outpath = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/";

const Int_t nPtBins = 14;
const Float_t ptBins[nPtBins+1] = {30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 200, 300, 400, 600};
const TString hnam = "H_JetGenRecoPtRatio_";
const TString nomSampleName = "TTbar_Powheg";

Float_t GetRecoGenRatio(TString sample, Int_t bin){
  //>>> Returns mean for Reco/Gen ratio in a pt bin for a given sample
  TFile* f;
  TH1F* h;
  TString histoname = hnam + Form("%i", bin);
  f = TFile::Open(path + "Tree_" + sample + ".root");
  h = (TH1F*) f->Get(histoname);
  Float_t val;
  val = h->GetMean();

  TFitResultPtr res =  h->Fit("gaus", "s");
  val = res->Parameter(1);
  delete f;
  
  return val;
}

void PrintJetPtRatio(){
  //>>> Prints all the gaussian distributions for Reco/Gen pT ration
  //    in all the pt bins, for the three samples
  TFile *f; TH1F* h[nPtBins];
  TH1F* hU[nPtBins]; TH1F* hD[nPtBins];
  TString histoname; TString sample;
 
  sample = nomSampleName;
  f = TFile::Open(path + "Tree_" + sample + ".root");
  for(Int_t bin = 0; bin < nPtBins; bin++){
    histoname = hnam + Form("%i", bin);
    h[bin] = (TH1F*) f->Get(histoname);
    h[bin]->SetDirectory(0); h[bin]->Scale(1/h[bin]->Integral());
    h[bin]->SetLineWidth(2); h[bin]->SetLineColor(1); h[bin]->SetMarkerStyle(24); h[bin]->SetMarkerColor(1);
  }

  sample = "TTbar_Powheg_fsrUp";
  f = TFile::Open(path + "Tree_" + sample + ".root");
  for(Int_t bin = 0; bin < nPtBins; bin++){
    histoname = hnam + Form("%i", bin);
    hU[bin] = (TH1F*) f->Get(histoname);
    hU[bin]->SetDirectory(0); hU[bin]->Scale(1/hU[bin]->Integral());
    hU[bin]->SetLineWidth(2); hU[bin]->SetLineColor(kBlue); hU[bin]->SetMarkerStyle(25); hU[bin]->SetMarkerColor(kRed);
  }
 
  sample = "TTbar_Powheg_fsrDown";
  f = TFile::Open(path + "Tree_" + sample + ".root");
  for(Int_t bin = 0; bin < nPtBins; bin++){
    histoname = hnam + Form("%i", bin);
    hD[bin] = (TH1F*) f->Get(histoname);
    hD[bin]->SetDirectory(0); hD[bin]->Scale(1/hD[bin]->Integral());
    hD[bin]->SetLineWidth(2); hD[bin]->SetLineColor(kRed); hD[bin]->SetMarkerStyle(26); hD[bin]->SetMarkerColor(kBlue);
  }

  Float_t nm(0), um(0), dm(0);
  
    gStyle->SetTitleY(1.02);
  TCanvas* c= new TCanvas("c","c",10,10,1000,600);
  c->Divide(5,3);
  for(Int_t bin = 0; bin < nPtBins; bin++){
   ((TPad*) c->GetPad(bin+1))->cd();
    nm = h[bin]->GetMean(); um = hU[bin]->GetMean(); dm = hD[bin]->GetMean(); 
    h[bin]->SetTitle(Form("#splitline{JetPt = [%1.0f, %1.0f] GeV}{ MeanVal[Down,Nom,Up] = [%1.2f, %1.2f, %1.2f]}", ptBins[bin], ptBins[bin+1], dm, nm, um));
    h[bin]->SetStats(0);
    h[bin]->Draw("pc");
    hU[bin]->Draw("pc,same");
    hD[bin]->Draw("pc,same");
  }
  TLegend *l = new TLegend(0.1, 0.1, 0.9, 0.9);
  l->AddEntry(h[0],     "Nominal",   "pl");
  l->AddEntry(hU[0],   "FSR Up",    "pl");
  l->AddEntry(hD[0],   "FSR Down",    "pl");
  l->SetTextSize(0.08);
  ((TPad*) c->GetPad(nPtBins+1))->cd();
  l->Draw();
  c->Print(outpath + "JetPtRatio_"+sample+".png", "png");  
  c->Print(outpath + "JetPtRatio_"+sample+".pdf", "pdf");  
  delete c;
}

TGraph* GetGraph(TString sample){
  //>>> Returns a TGraph with mean values of Reco/Gen pT values for each pT bin
  Double_t x[nPtBins]; Double_t y[nPtBins];
  for(Int_t i = 0; i < nPtBins; i++){
    x[i] = (ptBins[i] + ptBins[i+1])/2;
    y[i] = GetRecoGenRatio(sample, i);
  }
  TGraph* t = new TGraph(nPtBins, x, y);
  t->SetLineWidth(2);
  t->SetMarkerStyle(0);
  return t;
}

void DrawGraph(){
  //>>> Draws the TGraph above for each sample
  TGraph* nom     = GetGraph(nomSampleName);
  TGraph* fsrUp   = GetGraph("TTbar_Powheg_fsrUp");
  TGraph* fsrDown = GetGraph("TTbar_Powheg_fsrDown");

  TCanvas* c= new TCanvas("c","c",10,10,800,600);
  nom->SetLineColor(kBlack); fsrUp->SetLineColor(kRed); fsrDown->SetLineColor(kBlue);
  //nom->SetMinimum(0.63); nom->SetMaximum(0.77);

  nom->SetTitle("");
  nom->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
  nom->GetYaxis()->SetTitle("Mean Reco/Gen p_{T}");

  nom->Draw();

  nom    ->Draw("c");
  fsrUp  ->Draw("c, same");
  fsrDown->Draw("c, same");
  
  TLegend *l = new TLegend(0.70, 0.2, 0.9, 0.5);
  l->AddEntry(nom,     "Nominal",   "l");
  l->AddEntry(fsrUp,   "FSR Up",    "l");
  l->AddEntry(fsrDown, "FSR Down",  "l");

  l->Draw();
  c->Print(outpath + "ptCorr.png", "png");  
  c->Print(outpath + "ptCorr.pdf", "pdf");  
  delete c;
}

void SaveJECSF(){
  //>>> Save the JEC corr SF in a rootfile and draw them
  Float_t ynom; Float_t yup; Float_t ydo;
  TH1F* SFUp   = new TH1F("FSR_JECSF_Up",   "FSR_JECSF_Up",   nPtBins, ptBins); 
  TH1F* SFDown = new TH1F("FSR_JECSF_Down", "FSR_JECSF_Down", nPtBins, ptBins); 
  for(Int_t bin = 0; bin < nPtBins; bin++){
    ynom = GetRecoGenRatio(nomSampleName, bin);
    yup  = GetRecoGenRatio("TTbar_Powheg_fsrUp", bin);
    ydo  = GetRecoGenRatio("TTbar_Powheg_fsrDown", bin);

    SFUp  ->SetBinContent(bin+1, yup/ynom);
    SFDown->SetBinContent(bin+1, ydo/ynom);
  }
  TFile* f = new TFile(outpath + "FSR_JECSF.root", "recreate");
  TCanvas* c= new TCanvas("c","c",10,10,800,600);

  SFUp->SetDirectory(0);
  SFDown->SetDirectory(0);
  SFUp  ->SetLineWidth(2); SFUp  ->SetLineColor(kRed);
  SFDown->SetLineWidth(2); SFDown->SetLineColor(kBlue);

  SFUp->SetTitle(""); 
  SFUp->SetStats(0); 
  SFUp->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
  SFUp->GetYaxis()->SetTitle("Scale factor");
  SFUp->GetYaxis()->SetTitleOffset(1.05);

  SFUp->Write();
  SFDown->Write();

  SFUp->SetMinimum(0.98); SFUp->SetMaximum(1.02);
  SFUp  ->Draw("hist");
  SFDown->Draw("same,hist");
  
  TLegend *l = new TLegend(0.2, 0.8, 0.8, 0.9);
  l->AddEntry(SFUp,   "SF for FSR Up",    "l");
  l->AddEntry(SFDown, "SF for FSR Down",  "l");
  l->SetNColumns(2);

  l->Draw();
  c->Write();
  c->Print(outpath + "JEC_FSR_SF.png", "png");  
  c->Print(outpath + "JEC_FSR_SF.pdf", "pdf");  
  delete c;
}


TH1F* GetBtagEffPlot(TString sample){
  TFile* f;
  TH1F* fnum; TH1F* fden;
  f = TFile::Open(pathBtag + "Tree_" + sample + ".root");
  fnum = (TH1F*) f->Get("H_JetPtRecoB");
  fden = (TH1F*) f->Get("H_JetPtGenB");
  TH1F* heff = (TH1F*) fnum->Clone("Eff_" + sample);
  heff->Divide(fden);
  heff->SetLineWidth(2);
  heff->SetDirectory(0);
  delete f;
  heff->SetStats(0);
  return heff;
}

void DrawBtagEff(){
  TH1F* nom     = GetBtagEffPlot(nomSampleName);
  TH1F* fsrUp   = GetBtagEffPlot("TTbar_Powheg_fsrUp");
  TH1F* fsrDown = GetBtagEffPlot("TTbar_Powheg_fsrDown");

  TFile* f = new TFile(outpath + "FSR_BtagSF.root", "recreate");
  TCanvas* c= new TCanvas("c","c",10,10,800,600);
  nom->SetLineColor(kBlack); fsrUp->SetLineColor(kRed); fsrDown->SetLineColor(kBlue);
  nom->SetMinimum(0.63); nom->SetMaximum(0.77);
  nom->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
  nom->GetYaxis()->SetTitle("b-tag efficiency");

  TH1F* SFUp   = (TH1F*) fsrUp->Clone("FSR_btagSF_Up");
  TH1F* SFDown = (TH1F*) fsrUp->Clone("FSR_btagSF_Down");
  SFUp->Divide(nom); SFDown->Divide(nom);

  nom->Write(); fsrUp->Write(); fsrDown->Write();
  SFUp->Write();
  SFDown->Write();

  nom    ->Draw("c");
  fsrUp  ->Draw("c, same");
  fsrDown->Draw("c, same");
  
  TLegend *l = new TLegend(0.70, 0.7, 0.9, 0.9);
  l->AddEntry(nom,     "Nominal",   "l");
  l->AddEntry(fsrUp,   "FSR Up",    "l");
  l->AddEntry(fsrDown, "FSR Down",  "l");

  l->Draw();
  c->Write();
  c->Print(outpath + "btagCorr.png", "png");  
  c->Print(outpath + "btagCorr.pdf", "pdf");  
  delete c;
}

void DrawCorr(){
  DrawBtagEff();
  PrintJetPtRatio();
  DrawGraph();
  SaveJECSF();
}
