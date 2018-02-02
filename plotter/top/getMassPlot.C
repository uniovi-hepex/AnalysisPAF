#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include <unistd.h>
#include <getopt.h>

using namespace std;

namespace{
  //string filename = "/nfs/fanae/user/juanr/TOP13TeV_13TeV/TOP13TeV/TopCode/tools/xsec_mass.txt"; 
  string filename = "./xsec_mass.txt"; 
}
bool Contains(const string &str, const string &pat){
  return str.find(pat) != string::npos;
}

float xsec_central = 792.2;
float xsec_unc_central = TMath::Sqrt(8.3*8.3 + 36.6*36.6+21.4*21.4);

Double_t func(Double_t *x, Double_t *par);
void getMassPlot(TString function = "");
void DrawTLatex(Double_t x,Double_t y, Double_t tsize, Short_t align, TString text);

vector<double> vtopmass, vxsec, vlumi, vsys, vstat, verr, vmerr;
double topmass, xsec, xlumi, sys, stat, err; 
TMultiGraph* mg = new TMultiGraph();
TGraph* g; TGraphErrors* ge;
float x0 = 172.5; float xs = 0;

Double_t func(Double_t *x, Double_t *par){
	float x0 = 172.5; 
	Double_t value = xs*(1+par[0]*(x[0]-x0));
	return value;
}

void getMassPlot(TString function){
  if(filename == "") throw runtime_error("No input file provided");
	ifstream infile("/nfs/fanae/user/juanr/TOP13TeV_13TeV/TOP13TeV/TopCode/tools/xsec_mass_mar1.txt");
	string line;
	bool start_read = false;
	while(getline(infile, line)){
		if(Contains(line, "---")){
			start_read = true;
		}else if(start_read){
			istringstream iss(line);
			topmass = 0; xsec = 0; xlumi = 0; sys = 0; stat = 0; 
			iss >> topmass >> xsec >> stat >> sys >> xlumi; 
			//err = TMath::Sqrt(stat*stat + sys*sys + xlumi*xlumi);
			err = stat;
			vtopmass.push_back(topmass);
			vxsec.push_back(xsec);
			vlumi.push_back(xlumi);
			vsys.push_back(sys);
			vstat.push_back(stat);
			verr.push_back(err);
			vmerr.push_back(0.);
		}
	}
	infile.close();

  TCanvas* c = new TCanvas("c", "Graph", 200, 10, 700, 500);

	g  = new TGraph(TVectorD(vtopmass.size(),&vtopmass[0]), TVectorD(vxsec.size(), &vxsec[0]) ); 
  ge = new TGraphErrors( TVectorD(vtopmass.size(),&vtopmass[0]), TVectorD(vxsec.size(), &vxsec[0]), TVectorD(vmerr.size(), &vmerr[0]), TVectorD(verr.size(), &verr[0]));
	xs = vxsec[0];
  g->SetMarkerStyle(20);
  ge->SetFillStyle(3002); ge->SetFillColor(4);
  mg->SetTitle("13 TeV, e#mu + #geq 2 jets #geq 1-btag, #int L dt = 35.9 fb^{-1}; top-quark mass (GeV); ttbar cross-section (pb)");
  mg->Add(ge);
  mg->Add(g);
  mg->Draw("a1P");

	float e0 = 0; float p0 = 0; float p1 = 0; float p2 = 0;
  TFitResultPtr f;
  if(function == "lin"){
    TF1* fuf = new TF1("flin",func,165,180,1);
    f = g->Fit("flin", "s", "", 165, 180);
    p0 = f->Parameter(0); e0 = f->ParError(0);// "xs*(1+k(x-x0))" // xs --> sigma_tt(172.5), x0 = 172.5 GeV, x = m_top
    DrawTLatex(0.85, 0.88, 0.035, 33, TString("Fit: #sigma_{t#bar{t}}(13TeV,m_{top}) = #sigma_{t#bar{t}}(13TeV,172.5 GeV)*(1+k(m_{top}-172.5 GeV))"));
    DrawTLatex(0.65, 0.80, 0.035, 33, TString(Form("k = %2.5f #pm %1.5f (%1.2f", p0, e0, TMath::Abs(e0/p0*100))) + "%)" );
    DrawTLatex(0.75, 0.72, 0.035, 33, TString(Form("#Delta#sigma_{t#bar{t}}(0.5 GeV) = %2.5f #pm %1.5f ", xsec_central*p0/2, e0/2*xsec_central)));
  }
  else if(function == "pol"){
    f = g->Fit("pol2", "s", "", 165, 180);
    p0 = f->Parameter(0); p1 = f->Parameter(1); p2 = f->Parameter(2);
    DrawTLatex(0.55, 0.88, 0.035, 33, Form("Fit: #sigma_{t#bar{t}}(13TeV,m_{top}) = a m_{top}^{2} + b m_{top} + c"));
    DrawTLatex(0.75, 0.83, 0.035, 33, Form("a=%4.3f (pb/GeV)^{2}\n,  b=%4.3f (pb/GeV)\n,  c=%4.3f pb", p2,p1,p0));
  }
  else if(function == "exp"){
    f = g->Fit("expo", "s", "", 165, 180);
    p0 = f->Parameter(0); p1 = f->Parameter(1);
    DrawTLatex(0.65, 0.88, 0.035, 33, Form("Fit: exp(a+bm_{top})      a=%4.3f     b=%4.3f GeV^{-1}", p0, p1));
  }
  else{
    cout << "Choose function!!!: 'lin', 'pol', 'exp'.\n";
    return;
  }

  mg->GetYaxis()->SetLimits(740,860); mg->SetMinimum(740); mg->SetMaximum(860);
  mg->GetXaxis()->SetLimits(165,180);

  c->Update();
  TString outputdir = "./XsecWithMass/";
  gSystem->mkdir(outputdir, kTRUE); 
  c->Print(outputdir+"top_mass_"+function+".pdf","pdf");
  c->Print(outputdir+"top_mass_"+function+".png","png");
  delete c;
}


void DrawTLatex(Double_t x,Double_t y, Double_t tsize, Short_t align, TString  text){
  TLatex* tl = new TLatex(x, y, text);
  tl->SetNDC      (true);
  tl->SetTextAlign( align);
  tl->SetTextFont (    42);
  tl->SetTextSize ( tsize);
  tl->Draw("same");
}

/*const TString path = "/nfs/fanae/user/palencia/testHeppy/TOP/TopTrees/feb24/Tree_13TeV_EA_";
const int nSamples = 7;
const TString masses[nSamples] = {"1665", "1695", "1715", "1725", "1735", "1755", "1785"};
const float Lumi = 2223.;

float yield(TString mass){
  TString samplename = "TTbar_Powheg";
  if (mass == "1725") samplename += ".root";
  else samplename += "_mtop" + mass + ".root"; 
  TFile *f = TFile::Open(path + samplename);
  TH1F* h;
  f->GetObject("H_Yields_ElMu", h);
  float y = h->GetBinContent(5); 
  delete f;
  return y*Lumi;
}*/
/*
float stat(TString mass){
  TString samplename = "TTbar_Powheg";
  if (mass == "1725") samplename += ".root";
  else samplename += "_mtop" + mass + ".root"; 
  TFile *f = TFile::Open(path + samplename);
  TH1F* h;
  f->GetObject("H_Yields_ElMu", h);
  float y = h->GetBinError(5); 
  delete f;
  return y*Lumi;
}

void GetStatTable(){
  cout << " Lumi = 2223 fb-1, level = 1btag, channel = ElMu" << endl;
  cout << "=======================================================" << endl;
  cout << " Mass   Yield         Stat" << endl; 
  for(int i = 0; i < nSamples; i++){
    cout << " " << masses[i] << Form(",  %4.1f  +/- %2.3f (%2.3f %)", yield(masses[i]), stat(masses[i]), stat(masses[i])/yield(masses[i])*100) << endl;
  }
  cout << "=======================================================" << endl;
}*/
