R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

TString treename = "tree";
TString path = "/pool/ciencias/userstorage/juanr/stop/genLevel/";
TString outpath = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/stop/TopMass/";

TString dilepgen = "TNgenLeps >= 2";
TString dilep    = "TNSelLeps >= 2"; 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters";

Float_t ttbar_xsec_top_mass(Float_t m);
void DrawTex(TString t, Float_t x, Float_t y, Float_t size = 0.03);
void DrawComp();
void Graph();
void PrintXsec();

void DrawComp(){
  MultiPlot *mp = new MultiPlot();

  mp->SetPath(path);
  mp->SetTreeName(treename);
  mp->SetPlotFolder(outpath);
  mp->doYieldsInLeg = false;

  TString chan = "1";
  TString weight = "1";

  mp->AddDistribution("MT2", "TgenMT2", dilepgen, chan, 20, 0, 120);
  mp->AddDistribution("MET", "TgenMET", dilepgen, chan, 20, 0, 250);
  mp->AddDistribution("DeltaPhi", "TgenDeltaPhi", dilepgen, chan, 15, 0, 3.2);
  mp->AddDistribution("DeltaEta", "TgenDeltaEta", dilepgen, chan, 15, 0, 3.2);

  mp->AddSample("TTbar_Powheg",           "t#bar{t}, m_{t} = 172.5 GeV/c^{2}",  itSignal, 1,         "", weight);
  mp->AddSample("TTbar_Powheg_mtop1665",  "t#bar{t}, m_{t} = 165.5 GeV/c^{2}",  itSignal, kViolet-1, "", weight);
  mp->AddSample("TTbar_Powheg_mtop1695",  "t#bar{t}, m_{t} = 169.5 GeV/c^{2}",  itSignal, kBlue+1,   "", weight);
  mp->AddSample("TTbar_Powheg_mtop1715",  "t#bar{t}, m_{t} = 171.5 GeV/c^{2}",  itSignal, kGreen+2,  "", weight);
  mp->AddSample("TTbar_Powheg_mtop1735",  "t#bar{t}, m_{t} = 173.5 GeV/c^{2}",  itSignal, kOrange,   "", weight);
  mp->AddSample("TTbar_Powheg_mtop1755",  "t#bar{t}, m_{t} = 175.5 GeV/c^{2}",  itSignal, kOrange+2, "", weight);
  mp->AddSample("TTbar_Powheg_mtop1785",  "t#bar{t}, m_{t} = 178.5 GeV/c^{2}",  itSignal, kRed+1,    "", weight);

  mp->SetLegendTextSize(0.04);
  mp->SetLegendPosition(0.50, 0.60, 0.93, 0.93);

  mp->SetPlot("MT2", "M_{T2} [GeV]", "", "", 1.2); 
  mp->doSetLogy = true;
  mp->DrawComp("MT2", 1, "phist");

  mp->doSetLogy = false;
  mp->SetPlot("MET", "MET [GeV]", "", "", 1.2); 
  mp->DrawComp("MET", 1, "hist");

  mp->SetPlot("DeltaPhi", "\\Delta\\Phi(\\ell\\ell)", "", "", 1.2); 
  mp->SetLegendPosition(0.2, 0.70, 0.5, 0.9);
  mp->DrawComp("DeltaPhi", 1);

  mp->SetPlot("DeltaEta", "#Delta#Eta", "", "", 1.2); 
  mp->SetLegendPosition(0.60, 0.60, 0.90, 0.90);
  mp->DrawComp("DeltaEta", 1);
}

void Graph(){
  const Int_t n = 7;
  Float_t m[n] = {165.5, 169.5, 171.5, 172.5, 173.5, 175.5, 179.5};
  Float_t xsec[n];
  for(Int_t i = 0; i < n; i ++) xsec[i] = ttbar_xsec_top_mass(m[i]);
  TGraph *t = new TGraph(n, m, xsec);
  Float_t a1 = -0.745047; Float_t a2 = 0.127417;

  TString sref   = "#sigma_{t#bar{t}}^{ref}";
  TString pdftex = Form("#splitline{NNPDF2.3 NNLO (5f FFN)}{m_{t}^{ref} = 172.5 GeV, %s = 831.76 pb}", sref.Data());
  TString frac = "#left( #frac{m_{t} - m_{t}^{ref}}{m_{t}^{ref}} #right)";
  TString f4   = "#left( #frac{m_{t}^{ref}}{m_{t}} #right)^{4}";
  TString formula = Form("#sigma_{t#bar{t}}(m_{t}) = %s %s  #left[1 + a_{1} %s + a_{2} %s^{2} #right]", sref.Data(), f4.Data(), frac.Data(), frac.Data());
  TString coef   = Form("#splitline{a_{1} = %1.6f}{a_{2} = %1.6f}", a1, a2);
  


  t->SetTitle("");
  t->GetXaxis()->SetTitle("m_{t} [GeV/c^{2}]");
  t->GetYaxis()->SetTitle("#sigma_{t#bar{t}}(m_{t}) [pb]");

  t->SetLineWidth(2); t->SetMarkerStyle(20); t->SetMarkerSize(1.12);
  t->SetLineColor(kRed+1); t->SetMarkerColor(kBlack);

  TCanvas* c= new TCanvas("c","c",10,10,800,600);

  t->Draw();
  // Print info about PDFs...
  //tex->Draw("same");
  DrawTex(pdftex, 0.45, 0.82, 0.03);
  DrawTex(formula, 0.34, 0.71, 0.025);
  DrawTex(coef, 0.7, 0.59, 0.028);

  c->Print(outpath + "TopMassXsec.png", "png");
  c->Print(outpath + "TopMassXsec.pdf", "pdf");
}


void TopMassVar(){
  PrintXsec();
  //Graph();
  //DrawComp();
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

void DrawTex(TString t, Float_t x, Float_t y, Float_t size){
  TLatex* tex = new TLatex(0.,0., t);
  tex->SetNDC();
  tex->SetTextAlign(12);
  tex->SetX(x);
  tex->SetY(y);
  tex->SetTextSize(size);
  tex->SetTextSizePixels(22);

  tex->Draw("same");

}

void PrintXsec(){
  const Int_t n = 7;
  Float_t m[n] = {165.5, 169.5, 171.5, 172.5, 173.5, 175.5, 179.5};
  Float_t xsec[n];
  for(Int_t i = 0; i < n; i ++){ 
xsec[i] = ttbar_xsec_top_mass(m[i]);
  
  cout << Form("Mass = %3.1f, xsec = %f",m[i],xsec[i]) << endl;
}
}
