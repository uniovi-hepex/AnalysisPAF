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

void Histo::StackOverflow(Bool_t doStackOverflow){
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

void Histo::SetStatUnc(){
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
	SetMarkerColor(c); SetLineColor(c); SetFillColor(c);
}

void Histo::AddToLegend(TLegend* leg, Bool_t doyi){
  TH1F* h2 = (TH1F*) Clone("toLeg");
  TString op = "f";
  if      (type == itSignal) op = "l";
  else if (type == itData)   op = "pe";
  else if (type == itCompare)op = "pe";
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
  Float_t diff = 0;
  if(GetNbinsX() != nbins)  std::cout << " [Histo] WARNING: cannot add to systematics" << std::endl; 
  for(Int_t k = 0; k < nbins; k++){
    diff = GetBinContent(k+1) - hsys->GetBinContent(k+1);
    if(diff >  0) vsysd[k] += diff*diff;
    else          vsysu[k] += diff*diff;
  }

}

void Histo::SetBinsErrorFromSyst(){
  Int_t nbins = GetNbinsX();
  Float_t max = 0;
  for(Int_t k = 1; k < nbins+1; k++){
    //max = vsysd[k] > vsysu[k] ? vsysd[k] : vsysu[k];
    SetBinError(k, TMath::Sqrt((vsysu[k]+vsysd[k])/2));
  }
}

AnalHisto::AnalHisto(TString sample, TCut ct, TString channel, TString p, TString tN, TString sys){
  sampleName = sample; cut = ct; chan = channel;
  SysTag = sys;
  path = p;
  treeName = tN;
  loadTree();  
  h = new Histo(TH1F());
}

void AnalHisto::SetHisto(TString name, Int_t nb, Double_t xi, Double_t xe){
  if(name != "") histoname = name;
  if(nb != 0.)    AnalHistoBins = nb;
  if(xi != 0.)    AnalHistoX0 = xi;
  if(xe != 0.)    AnalHistoXf = xe;
  *h = Histo(TH1F(histoname, histoname, AnalHistoBins, AnalHistoX0, AnalHistoXf));
  for(Int_t i = 0; i <= nb; i++)   h->SetBinContent(i, 0);
}

//void AnalHisto::SetHisto(TString name, Int_t nb, Float_t thebins[100]){ // To be completed... use the prevous definition
//  return;
//}

Double_t AnalHisto::GetYield(TString sys){
  SetHisto("Yield", 1, 0, 2);
  Fill("", sys);
  return h->GetBinContent(1); 
}

void AnalHisto::Fill(TString variable, TString sys, Bool_t isFastSim){
  if(variable != "") var = variable;
  TCut schan = TCut("1");
  if     (chan == "Elec")  schan = TCut("TChannel == 3");
  else if(chan == "Muon")  schan = TCut("TChannel == 2");
  else if(chan == "ElMu")  schan = TCut("TChannel == 1");
  else if(chan == "3l")  schan = TCut("TChannel == 4");
  else if(chan == "4l")  schan = TCut("TChannel == 5");
  else if(chan == "SF" || chan == "sameF") schan = TCut("TChannel != 1");
  else schan = TCut("1");

  weight = TString("TWeight");
       if(sys == "LepEffUp"  ) weight += "_LepEffUp";
  else if(sys == "LepEffDown") weight += "_LepEffDown";
  else if(sys == "TrigUp"    ) weight += "_TrigUp";
  else if(sys == "TrigDown"  ) weight += "_TrigDown";
  else if(sys == "PUUp"      ) weight += "_PUUp";
  else if(sys == "PUDown"    ) weight += "_PUDown";
  else if(sys == "FSUp"      ) weight += "_FSUp";
  else if(sys == "FSDown"    ) weight += "_FSDown";

  else if(sys == "JESUp"){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsJESUp"));   
    var = var.ReplaceAll("TNJets", "TNJetsJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("TMET", "TMETJESUp"));   
    var = var.ReplaceAll("TMET", "TMETJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("TMT2ll", "TMT2llJESUp"));   
    var = var.ReplaceAll("TMT2ll", "TMT2llJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("THT", "THTJESUp"));   
    var = var.ReplaceAll("THT", "THTJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("TJet_Pt", "TJetJESUp_Pt"));   
    var = var.ReplaceAll("TJet_Pt", "TJetJESUp_Pt");
  }
  else if(sys == "JESDown"){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsJESDown"));   
    var = var.ReplaceAll("TNJets", "TNJetsJESDown");
    cut = TCut( ((TString) cut).ReplaceAll("TMET", "TMETJESDown"));   
    var = var.ReplaceAll("TMET", "TMETJESDown");
    cut = TCut( ((TString) cut).ReplaceAll("TMT2ll", "TMT2llJESDown"));   
    var = var.ReplaceAll("TMT2ll", "TMT2llJESDown");
    cut = TCut( ((TString) cut).ReplaceAll("THT", "THTJESDown"));   
    var = var.ReplaceAll("THT", "THTJESDown");
    cut = TCut( ((TString) cut).ReplaceAll("TJet_Pt", "TJetJESDown_Pt"));   
    var = var.ReplaceAll("TJet_Pt", "TJetJESDown_Pt");
  }
  else if(sys == "JER"){
    cut = TCut( ((TString) cut).ReplaceAll("TNBtags", "TNJetsJER"));   
    var = var.ReplaceAll("TNBtags", "TNJetsJER");
    cut = TCut( ((TString) cut).ReplaceAll("TJet_Pt", "TJetJER_Pt"));   
    var = var.ReplaceAll("TJet_Pt", "TJetJER_Pt");
  }
  else if(sys == "BtagUp"){
    cut = TCut( ((TString) cut).ReplaceAll("TNBtags", "TNBtagsUp"));   
    var = var.ReplaceAll("TNBtags", "TNBtagsUp");
  }
  else if(sys == "BtagDown"){
    cut = TCut( ((TString) cut).ReplaceAll("TNBtags", "TNBtagsDown"));   
    var = var.ReplaceAll("TNBtags", "TNBtagsDown");
  }
  else if(sys == "MisTagUp"){
    cut = TCut( ((TString) cut).ReplaceAll("TNBtags", "TNBtagsMisTagUp"));   
    var = var.ReplaceAll("TNBtags", "TNBtagsMisTagUp");
  }
  else if(sys == "MisTagDown"){
    cut = TCut( ((TString) cut).ReplaceAll("TNBtags", "TNBtagsMisTagDown"));   
    var = var.ReplaceAll("TNBtags", "TNBtagsMisTagDown");
  }
  else if(sys == "genMETUp"){
    cut = TCut( ((TString) cut).ReplaceAll("TMET", "TMETGenUp"));
    var = var.ReplaceAll("TMET", "TMETJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("TMT2ll", "TMT2llJESUp"));
    var = var.ReplaceAll("TMT2ll", "TMT2llJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("THT", "THTJESUp"));
    var = var.ReplaceAll("THT", "THTJESUp");
    cut = TCut( ((TString) cut).ReplaceAll("TJet_Pt", "TJetJESUp_Pt"));
    var = var.ReplaceAll("TJet_Pt", "TJetJESUp_Pt");
  }
  else if(sys == "genMETDown"){

  }

  for(Int_t i = 0; i < AnalHistoBins; i++) h->SetBinContent(1,0.0); // To magically solve some problems
  tree->Project(histoname, var, (cut && schan)*weight); 

}

void AnalHisto::loadTree(){
  if(path == ""){
    path = DefaultPath;
		if(sampleName.BeginsWith("T2tt")) path = DefaultPath + "Susy/";
  }
  if(treeName == ""){
   treeName = DefaultTreeName; 
  }
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(path + prefix + sampleName + sufix);
	if (!f || !f->IsOpen()) {
		f = new TFile(path + prefix + sampleName + sufix);
	}
	f->GetObject(treeName,tree);
}

void AnalHisto::SetTreeName(TString t){ treeName = t;}

void AnalHisto::SetPath(TString t){ path = t;}
