#include "Histo.h"


void Histo::SetType(Int_t tipo){
	type = tipo;
	if(type < 0 || type > nTypes) type = nTypes;
	if(type == itBkg){         // Background: Fill
		SetLineStyle(0);
		SetFillStyle(1001);
	}
	else if(type == itSignal){    // Signal: lines
		SetLineStyle(1);
    SetLineWidth(2);
    SetFillColor(0);
	}
	else if(type == itData){    // Data: points
		SetLineStyle(0);
    SetFillStyle(0);
    SetColor(kBlack);
    SetMarkerStyle(20);
    SetMarkerSize(1.1);
	}
  else if(type == itCompare){ // points and lines
    SetFillStyle(0);
    SetLineStyle(1);
    SetLineWidth(2);
    SetMarkerStyle(24);
    SetMarkerSize(1.3);
  }
  else{   // Other
    SetLineStyle(1);
    SetLineWidth(2);
  }
}

void Histo::StackOverflow(){
	if(!doStackOverflow) return;
	int lastBin = GetNbinsX();
	float lastBinContent = GetBinContent(lastBin);
	float overflow = GetBinContent(lastBin+1);
	SetBinContent(lastBin, lastBinContent + overflow);
	SetBinContent(lastBin+1, 0);
}

void Histo::SetStyle(){
	SetStats(0);
	StackOverflow();
	yield = Integral();
	max = GetMaximum();
	GetXaxis()->SetTitle(xlabel);
}

void Histo::ReCalcValues(){
	yield = Integral();
	max = GetMaximum();
}

void Histo::SetStatUnc(){
  return; // Does not work that way anymore...
	if(type == itData) return; // Add MC stats to errors!
  if(vsysd) return;
	Int_t nbins = GetNbinsX();
	vsysu = new Float_t[nbins+1];
	vsysd = new Float_t[nbins+1];
	Float_t err2 = 1;
	for(int i = 0; i < nbins+1; i++){
		err2 = GetBinError(i); err2 = err2*err2;
		vsysu[i] = err2; vsysd[i] = err2;
	}
}

void Histo::SetTag(TString t, TString p, TString x, TString c){
  if(t != "") tag = t;
  if(p != "") process = p; 
  if(x != "") xlabel = x;
  if(c != "") cuts = c;
  return;
}

void Histo::SetProcess(TString p){
  process = p;
}

void Histo::SetTitles(TString x, TString c){
	xlabel = x; 
	if(cuts != "") cuts = c;
}

void Histo::SetColor(Int_t c){
	color = c;
	SetMarkerColor(c); SetLineColor(c); if(type != itSignal) SetFillColor(c);
}

void Histo::AddToLegend(TLegend* leg, Bool_t doyi){
  TH1F* h2 = (TH1F*) Clone("toLeg");
  TString op = "f";
  if(DrawStyle != "") op = DrawStyle;
  if(DrawStyle == "hist") op = "l";
  else{
    if      (type == itSignal){
      if(GetFillColor() == 0) op = "l";
      else op = "f";
    }
    else if (type == itData)   op = "pe";
    else if (type == itCompare)op = "pe";
  }
  
  if(doyi) leg->AddEntry(h2, Form(process + ": %1.0f", yield), op);
  else leg->AddEntry(h2, tag, op);
}

TH1F* Histo::GetVarHistoStatBin(Int_t bin, TString dir){
  Float_t var = GetBinContent(bin);
  Float_t stat = GetBinError(bin);
  TH1F* h2 = (TH1F*) Clone();
  if      (dir == "up" || dir == "Up" || dir == "UP")  h2->SetBinContent(bin, var + stat);
  else if (dir == "down" || dir == "Down" || dir == "DOWN")  h2->SetBinContent(bin, var - stat);
  else    cout << " ---> ERROR!!!! No valid direction: " << dir << endl;
  return h2;
}

void Histo::AddToSystematics(Histo* hsys, TString dir){
  Int_t nbins = hsys->GetNbinsX();
  Int_t ourbins = GetNbinsX();
  if(!vsysd){
    vsysu = new Float_t[ourbins+1];
    vsysd = new Float_t[ourbins+1];
    for(Int_t k = 0; k <= ourbins; k++){
      vsysu[k] = 0;
      vsysd[k] = 0;
    } 
  }

  Float_t diff = 0;
  if(ourbins != nbins)  std::cout << " [Histo] WARNING: cannot add to systematics" << std::endl; 
  for(Int_t k = 0; k < nbins; k++){
    diff = GetBinContent(k+1) - hsys->GetBinContent(k+1);
    // if (k == 2) cout << hsys->GetName() << " " << diff/GetBinContent(k+1) << endl;
    if(diff >  0) vsysd[k] += diff*diff;
    else          vsysu[k] += diff*diff;
  }
}

void Histo::SetBinsErrorFromSyst(){
  Int_t nbins = GetNbinsX();
  Float_t max = 0;
  for(Int_t k = 0; k < nbins; k++){
    //max = vsysd[k] > vsysu[k] ? vsysd[k] : vsysu[k];
    SetBinError(k+1, (TMath::Sqrt(vsysu[k]) + TMath::Sqrt(vsysd[k])) /2);
  }
}



Histo* Histo::CloneHisto(const char* newname) const{
  TH1F* h = (TH1F*) Clone(newname);
  Histo* g = new Histo(*h);
  g->SetTag(tag, process, xlabel, cuts);
  g->SetType(type); g->SetColor(color);
  g->SetStyle();
  return g;
}


void Histo::GetEnvelope(vector<Histo*> v, Int_t dir) {
  Int_t nHistos = v.size();
  Int_t nbins = GetNbinsX();
  Float_t extremeval; Float_t val = 0;

  for(Int_t iB = 0; iB <= nbins; iB++){
    extremeval = GetBinContent(iB);
    for(Int_t iH = 0; iH < nHistos; iH++){
      val = v.at(iH)->GetBinContent(iB);
      if(dir >= 0) if(val > extremeval) extremeval = val;
      if(dir <  0) if(val < extremeval) extremeval = val;
    }
    SetBinContent(iB, extremeval);
  }
}
