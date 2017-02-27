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
	haveStyle = true;
	SetStats(0);
	StackOverflow();
	yield = Integral();
	max = GetMaximum();
	if(type != itData){ // Add MC stats to errors!
		Int_t nbins = GetNbinsX();
		if(haveStyle) return; // Only once...
		vsysu = new Float_t[nbins];
		vsysd = new Float_t[nbins];
		Float_t err2 = 1;
		for(int i = 0; i < nbins; i++){ 
			err2 = GetBinError(i); err2 = err2*err2;
			vsysu[i] = err2; vsysd[i] = err2;
		}
	}
	GetXaxis()->SetTitle(xlabel);
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

void Histo::SetSyst(Float_t s){
  syst = s;
}

void Histo::AddToLegend(TLegend* leg, Bool_t doyi){
  TH1F* h = (TH1F*) Clone();
  TString op = "f";
  if      (type == itSignal) op = "l";
  else if (type == itData)   op = "pe";
  else if (type == itCompare)op = "pe";
  if(doyi) leg->AddEntry(h, Form(process + ": %1.0f", yield), op);
  else leg->AddEntry(h, process, op);
}

TH1F* Histo::GetVarHistoStatBin(Int_t bin, TString dir){
  Float_t var = GetBinContent(bin);
  Float_t stat = GetBinError(bin);
  TH1F* h = (TH1F*) Clone();
  if      (dir == "up" || dir == "Up" || dir == "UP")  h->SetBinContent(bin, var + stat);
  else if (dir == "down" || dir == "Down" || dir == "DOWN")  h->SetBinContent(bin, var - stat);
  else    cout << " ---> ERROR!!!! No valid direction: " << dir << endl;
  return h;
}

void Histo::AddToSystematics(Histo* hsys, TString dir){
  Int_t nbins = hsys->GetNbinsX();
  Float_t diff = 0;
  if(GetNbinsX() != nbins)  std::cout << " [Histo] WARNING: cannot add to systematics" << std::endl; 
  if(dir == "Down" || dir == "down"){ // It's a "down" systematic
  //if(yield > hsys->yield){ // It's a "down" systematic
  //cout << " ---> [Histo] Adding to systematics DOWN: " << endl;
    for(Int_t k = 0; k < nbins; k++){
      diff = TMath::Abs(GetBinContent(k+1) - hsys->GetBinContent(k+1));
      vsysd[k] += diff*diff;
    }
  }
  else{ // It's an "up" systematic
  //cout << " ---> [Histo] Adding to systematics UP: " << endl;
    for(Int_t k = 0; k < nbins; k++){
      diff = TMath::Abs(GetBinContent(k+1) - hsys->GetBinContent(k+1));
      vsysu[k] += diff*diff;
    }
  }
}

AnalHisto::AnalHisto(TString sample, TCut ct, TString channel, TString p, TString tN, Int_t isyst){
  sampleName = sample; cut = ct; chan = channel;
  syst = isyst;
  path = p;
  treeName = tN;
  loadTree();  
}

void AnalHisto::SetHisto(TString name, Int_t nb, Float_t xi, Float_t xe){
  if(name != "") histoname = name;
  if(nb != 0)    nbins = nb;
  if(xi != 0)    x0 = xi;
  if(xe != 0)    xf = xe;
  h = new Histo(histoname, histoname, nb, x0, xf);
}

void AnalHisto::SetHisto(TString name, Int_t nb, Float_t thebins[100]){ // To be completed... use the prevous definition
  return;
}

void AnalHisto::Fill(TString variable, Int_t isyst){
  if(variable != "") var = variable;
  TCut schan = TCut("1");
  if     (chan == "Elec")  schan = TCut("TChannel == 3");
  else if(chan == "Muon")  schan = TCut("TChannel == 2");
  else if(chan == "ElMu")  schan = TCut("TChannel == 1");
  else if(chan == "SF" || chan == "sameF") schan = TCut("TChannel != 1");
  else schan = TCut("1");

  weight = TString("TWeight");
       if(isyst == iLepEffUp  ) weight += "_LepEffUp";
  else if(isyst == iLepEffDown) weight += "_LepEffDown";
  else if(isyst == iTrigUp    ) weight += "_TrigUp";
  else if(isyst == iTrigDown  ) weight += "_TrigDown";
  else if(isyst == iPUUp      ) weight += "_PUUp";
  else if(isyst == iPUDown    ) weight += "_PUDown";
  else if(isyst == iFSUp      ) weight += "_FSUp";
  else if(isyst == iFSDown    ) weight += "_FSDown";

  else if(isyst == iJESUp){
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
  else if(isyst == iJESDown){
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
  else if(isyst == iJER){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsJER"));   
    var = var.ReplaceAll("TNJets", "TNJetsJER");
    cut = TCut( ((TString) cut).ReplaceAll("TJet_Pt", "TJetJER_Pt"));   
    var = var.ReplaceAll("TJet_Pt", "TJetJER_Pt");
  }
  else if(isyst == iBtagUp){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsBtagUp"));   
    var = var.ReplaceAll("TNJets", "TNJetsBtagUp");
  }
  else if(isyst == iBtagDown){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsBtagDown"));   
    var = var.ReplaceAll("TNJets", "TNJetsBtagDown");
  }
  else if(isyst == iMisTagUp){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsMisTagUp"));   
    var = var.ReplaceAll("TNJets", "TNJetsMisTagUp");
  }
  else if(isyst == iMisTagDown){
    cut = TCut( ((TString) cut).ReplaceAll("TNJets", "TNJetsMisTagDown"));   
    var = var.ReplaceAll("TNJets", "TNJetsMisTagDown");
  }

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
