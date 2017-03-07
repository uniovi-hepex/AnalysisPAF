#include "Plot.h"
#include "TSystem.h"
#include <fstream> 

Histo Plot::GetH(TString sample, TString sys, Int_t type){ 
  // Returns a Histo for sample and systematic variation 
  TString pathToMiniTree = path;
  if(type == itSignal || sample.BeginsWith("T2tt")) pathToMiniTree = pathSignal;
  AnalHisto* ah = new AnalHisto(sample, cut, chan, pathToMiniTree, treeName, sys);
	ah->SetHisto(sample, nb, x0, xN);
	ah->Fill(var, sys);
	Histo g = *(ah->GetHisto());
	delete ah;
	return g;
}

void Plot::AddSample(TString p, TString pr, Int_t type, Int_t color, Float_t S, TString sys){
  // Add a sample to your plot...
  //  -> p  is the name of the sample
  //  -> pr is the name of the process that includes the sample
  //  -> type: itBkg    for a background sample
  //           itSignal for a signal sample
  //           itData   for a data sample
  //           -1       for a systematic variation
  //  -> S is the normalization uncertainty of the sample (for datacards)
  //  -> iSyst is the index of the systematic variaton
  Histo* g = new Histo(GetH(p, sys, type));
  g->SetDirectory(0);
  Float_t val; Float_t err;
  Histo* h = new Histo(p, p, nb, x0, xN);   // If not... problem when mergin histograms
  for(Int_t i = 0; i <= xN; i++){
   val = g->GetBinContent(i); err = g->GetBinError(i);
   h->SetBinContent(i, val); h->SetBinError(i,err); 
  }
  h->SetDirectory(0);
  h->SetProcess(pr);
  h->SetName(p);
  TString t = (sys == "0")? pr : pr + "_" + sys;
  h->SetTag(t); // Tag = process + sys
  h->SetSysTag(sys);
  h->SetStatUnc();
  Int_t n; Int_t i = 0;

  if(type != itData)  h->Scale(Lumi*1000);
  if(type == itBkg){ // Backgrounds
    n = VBkgs.size();
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
		for(i = 0; i < n; i++){
			if(t == VBkgs.at(i)->GetTag()){ // Group backgrounds
				VBkgs.at(i)->Add((TH1F*) h); 
				VBkgs.at(i)->SetStyle(); VBkgs.at(i)->SetStatUnc();
				return;
			}
		}
    h->SetName(t);
  }
	else if(type == itSys){ // Systematic!!
		n = VSyst.size();
		for(i = 0; i < n; i++){ // Group systematics
			if(t == VSyst.at(i)->GetTag()){
				VSyst.at(i)->Add(h); VSyst.at(i)->SetStyle();
				return;
			}
		}
    h->SetName(t);
  }
  else if(type == itData){
    h->SetLineColor(kBlack); 
    h->SetMarkerStyle(20); 
    h->SetMarkerSize(1.1); 
  }

  h->SetType(type); h->SetColor(color); h->SetStyle(); 
  h->SetSysNorm(S);

  AddToHistos(h);
}

THStack* Plot::GetStack(){ // Returns the stack with all Bkg
	THStack* hStack0 = new THStack(varname, "");
  Int_t nBkgs = VBkgs.size();
  if(nBkgs < 1) return hStack0;
	for(Int_t i = 0; i < nBkgs; i++){
		hStack0->Add((TH1F*) VBkgs.at(i));
	}
  return hStack0;
}

Histo* Plot::GetAllBkg(){ // Return histogram with the sum of all bkg
  THStack* hStack0 = GetStack();
	Histo* AllBkg0 = new Histo(*(TH1F*) hStack0->GetStack()->Last(), 3);
	AllBkg0->SetStyle();
	AllBkg0->SetTag("TotalBkg");
  return AllBkg0;
}

Histo* Plot::SetData(){  // Returns histogram for Data
  Histo* hData0 = new Histo(TH1F());
	if(VData.size() < 1){
		doData = false;
		return hData0;
	}
  Bool_t HaveDataForChannel = false;
  TString p = "";
	for(Int_t i = 0; i < (Int_t) VData.size(); i++){
    p = VData.at(i)->GetName();
    if     (chan == "ElMu" && (p == "DoubleMuon" || p == "DoubleEG"))   continue;
    else if(chan == "Muon" && (p != "DoubleMuon" && p!= "SingleMuon"))  continue;
    else if(chan == "Elec" && (p != "DoubleEG" && p!= "SingleElec"))    continue;
    else if( (chan == "SF" || chan == "sameF") && (p == "MuonEG"))      continue;
    if(HaveDataForChannel) hData0->Add(VData.at(i));
    else{
      hData0 = new Histo(* (TH1F*) VData.at(i)->Clone());
      HaveDataForChannel = true;
    }
	} 
  if(!HaveDataForChannel){
    cout << "[Plot] Data is missing!!" << endl;
    doData = false;
    return hData0;
  }
	hData0->SetLineColor(kBlack); 
	hData0->SetMarkerStyle(20); 
	hData0->SetMarkerSize(1.1); 
	hData0->SetStyle(); hData0->SetType(itData); 
  return hData0;
}

void Plot::AddToHistos(Histo* p){ // Add the histogram to the right vector
  if     (p->GetType() == itBkg   ) VBkgs.push_back(p);
  else if(p->GetType() == itSignal) VSignals.push_back(p);
  else if(p->GetType() == itData  ) VData.push_back(p);
  else if(p->GetType() == 4 || p->GetType() == -1) VSyst.push_back(p);
  else return;
  if(verbose) cout << "[Plot::AddToHistos] Added histogram " << p->GetName() << " to " << p->GetTag() << " group (" << p->GetType() << ")" << endl;
}

//================================================================================
// Systematics
//================================================================================

void Plot::AddSystematic(TString var){
	for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++){
		AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, 0, var+"Up");
		AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, 0, var+"Down");
	}
  for(Int_t i = 0; i < (Int_t) VSignals.size(); i++){
    AddSample(VSignals.at(i)->GetName(), VSignals.at(i)->GetTag(), itSys, 1, 0, var+"Up");
    AddSample(VSignals.at(i)->GetName(), VSignals.at(i)->GetTag(), itSys, 1, 0, var+"Down");
  }
  if(verbose) cout << "[Plot::AddSystematic] Systematic histograms added to the list for variation: " << var << endl;
  AddSystematicLabel(var);
}

void Plot::IncludeBkgSystematics(){
	if(ShowSystematics) return;
	TString pr; TString ps; TString dir;
	Int_t nBkgs = VBkgs.size(); Int_t nSig = VSignals.size();
	Int_t nSyst = VSyst.size();
	for(int i = 0; i < nBkgs; i++){  // Backgrounds in i
		pr = VBkgs.at(i)->GetProcess() ;
		for(int k = 0; k < nSyst; k++){ // Systematics in k
			ps = VSyst.at(k)->GetTag();
			dir = (ps.Contains("Down") || ps.Contains("down"))? "Down" : "Up";
			if(ps.BeginsWith(pr))	VBkgs.at(i)->AddToSystematics(VSyst.at(k), dir);
		}
	}
	for(int i = 0; i < nSig; i++){ // Signals in i
		pr = VSignals.at(i)->GetProcess();
		for(int k = 0; k < nSyst; k++){ // Backgrounds in k
			ps = VSyst.at(k)->GetProcess();
			dir = (ps.Contains("Down") || ps.Contains("down"))? "Down" : "Up";
			if(ps.BeginsWith(pr))   VSignals.at(i)->AddToSystematics(VSyst.at(k), dir);
		}
	}
	ShowSystematics = true;
}

Histo* Plot::AllBkgSyst(){
  Histo* AllBkg = GetAllBkg(); AllBkg->SetStyle();
  Int_t nbins = AllBkg->GetNbinsX();
  Int_t nBkgs = VBkgs.size();
  Float_t valu = 0; Float_t vald = 0; Float_t maxsyst = 0;
  TotalSysUp   = new Float_t[nbins];
	TotalSysDown = new Float_t[nbins];

  AllBkg->SetStatUnc();

	for(int i = 0; i < nbins; i++){ // Loop all the bins
		valu = 0; vald = 0;
		for(int k = 0; k < nBkgs; k++){ // Loop all the samples
			valu += VBkgs.at(k)->vsysu[i];
			vald += VBkgs.at(k)->vsysd[i];
		}
		TotalSysUp[i]   = TMath::Sqrt(valu);
		TotalSysDown[i] = TMath::Sqrt(vald);
		maxsyst = TotalSysUp[i] > TotalSysDown[i]? TotalSysUp[i] : TotalSysDown[i];
		AllBkg->SetBinError(i+1, maxsyst);
  if(verbose) cout << "Bin " << i+1 << ", val = " << AllBkg->GetBinContent(i+1) << ", syst up = " << TotalSysUp[i] << ", syst down = " << TotalSysDown[i] << endl;
  }
  return AllBkg;
}

//================================================================================
// Drawing
//================================================================================
// SetLegend()
// TCanvas* SetCanvas()
// void DrawStack(TString tag = "0", bool sav = 0)
void Plot::PrintYields(){
  Int_t nBkgs = VBkgs.size();
  Int_t nSig  = VSignals.size();
                                     cout << " ························ Yields ························ " << endl;
  if(nBkgs > 0){
                                     cout << "----------------------------------------------------------" << endl;
	  for(Int_t i = 0; i < nBkgs; i++) cout << " " << VBkgs.at(i)->GetProcess() << ":      " << VBkgs.at(i)->GetYield() << endl;
    Histo* AllBkg = GetAllBkg();
                                     cout << " " << "Total Bkg: " << AllBkg->GetYield() << endl;
	}
  if(nSig>0)                         cout << "----------------------------------------------------------" << endl;
  for(Int_t i = 0; i < nSig;  i++)   cout << " " << VSignals.at(i)->GetProcess() << ":      " << VSignals.at(i)->GetYield() << endl;
  if(doData){
    Histo* hData = SetData();
	  	                               cout << "----------------------------------------------------------" << endl;
                                 		 cout << " " << "Data:      " << hData->GetYield() << endl;
	}
	                                   cout << "==========================================================" << endl;
}

void Plot::PrintSystYields(){
	Int_t nBkgs = VBkgs.size();
	Int_t nSig  = VSignals.size();
	Int_t nSys  = VSystLabel.size();
	TString pr; TString sys;
  cout << "\033[1;31m      ";
	for(Int_t gs = 0; gs < nSys; gs++) cout << "    " << VSystLabel.at(gs);
	cout << "\033[0m\n";
	for(Int_t i = 0; i < nBkgs; i++){
		pr = VBkgs.at(i)->GetProcess();
		cout << Form("\033[1;33m  %s \033[0m ", VBkgs.at(i)->GetProcess().Data());
    cout << "\033[1;34m";
		for(Int_t j = 0; j < nSys; j++){
			sys = VSystLabel.at(j);
			cout << Form("   %1.2f ", TMath::Abs(GetYield(pr)-GetYield(pr,sys))/GetYield(pr)*100);
		}
		cout << "\033[0m\n";
  }
}

Float_t Plot::GetYield(TString pr, TString systag){
	Float_t val = 0; Float_t sys = 0; TString ps; 
	Int_t nSyst = VSyst.size();
	Int_t nBkg = VBkgs.size();
	Int_t nSig = VSignals.size();
	if(systag == "0"){
		for(Int_t i = 0; i < nBkg; i++){
			if(pr == VBkgs.at(i)->GetProcess()) return  VBkgs.at(i)->GetYield();
		}
		for(Int_t i = 0; i < nSig; i++){
			if(pr == VSignals.at(i)->GetProcess()) return  VSignals.at(i)->GetYield();
		}
		//cout << " >>> Process not found!!!: " << pr << endl;
		return 0;
	}
	else{
		for(int k = 0; k < nSyst; k++){ // Systematics in k
			ps = VSyst.at(k)->GetTag();
			if(!ps.BeginsWith(pr))   continue;
			if(!ps.Contains(systag)) continue;
			return VSyst.at(k)->GetYield();
		}
	}
	//cout << "No systematic " << systag << " for process " << pr << endl;
	return GetYield(pr);
}


TLegend* Plot::SetLegend(){ // To be executed before using the legend
  TLegend* leg = new TLegend(0.70,0.65,0.93,0.93);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  Float_t MinYield = 0; 
  int nVBkgs = VBkgs.size();
	if(nVBkgs > 0){
		Histo* AllBkg = GetAllBkg();
		MinYield = AllBkg->GetYield()/5000;
	}
  for(int i = nVBkgs-1; i >= 0; i--){
    if(VBkgs.at(i)->GetYield() < MinYield) continue;
    else VBkgs.at(i)->AddToLegend(leg,doYieldsInLeg);
  }
  for(int i = VSignals.size()-1; i >= 0; i--) VSignals[i]->AddToLegend(leg, doYieldsInLeg);
  if(doData){
		Histo* hData = SetData(); hData->SetProcess("Data");
		hData->AddToLegend(leg,doYieldsInLeg);
	}
  return leg;
}

TCanvas* Plot::SetCanvas(){ // Returns the canvas
    TCanvas* c= new TCanvas("c","c",10,10,800,600);
  c->Divide(1,2);

  plot = (TPad*)c->GetPad(1);
  plot->SetPad(0.0, 0.23, 1.0, 1.0);
  plot->SetTopMargin(0.06); plot->SetRightMargin(0.02);

  pratio = (TPad*)c->GetPad(2);
  pratio->SetPad(0.0, 0.0, 1.0, 0.29);
  pratio->SetGridy();// pratio->SetGridx();
  pratio->SetTopMargin(0.03); pratio->SetBottomMargin(0.4); pratio->SetRightMargin(0.02);

  texlumi = new TLatex(-20.,50., Form("%2.1f fb^{-1}, #sqrt{s} = 13 TeV", Lumi));
  texlumi->SetNDC();
  texlumi->SetTextAlign(12);
  texlumi->SetX(0.72);
  texlumi->SetY(0.97);
  texlumi->SetTextFont(42);
  texlumi->SetTextSize(0.045);
  texlumi->SetTextSizePixels(22);
  texcms = new TLatex(0.,0., "CMS Preliminary");
  texcms->SetNDC();
  texcms->SetTextAlign(12);
  texcms->SetX(0.15);
  texcms->SetY(0.9);
  texcms->SetTextSize(0.052);
  texcms->SetTextSizePixels(23);
  return c;
}

void Plot::DrawComp(TString tag = "0", bool sav = 0){
  TCanvas* c = SetCanvas();
  int nsamples = VSignals.size();
	float themax = 0;
  Histo* signal;
  if(nsamples == 0) return;
  else signal = VSignals.at(0);
	float max = VSignals.at(0)->GetMaximum();
  signal->Draw("pe");
	for(Int_t  i = 1; i < nsamples; i++){
		max = VSignals.at(i)->GetMaximum(); 
		if(max > themax) themax = max;
		VSignals.at(i)->Draw("pesame");
	}
	TLegend* leg = SetLegend();
	leg->Draw("same");

/*	pratio->cd();
  hratio = (TH1F*)hData->Clone();
  hratio->Divide(AllBkg);
  SetHRatio();
  hratio->Draw("same");
  hratioerr->Draw("same,e2");
  hratio->Draw("same");
*/
  if(sav){ // Save the histograms
    TString dir = plotFolder + "StopPlots/";
    TString plotname = varname + "_" + chan + "_" + tag + "_compare";
    gSystem->mkdir(dir, kTRUE);
    c->Print( dir + plotname + ".png", "png");
    delete c;
  }
}

void Plot::DrawStack(TString tag = "0", bool sav = 0){
	Histo* AllBkg;
	THStack* hStack = GetStack();
	if(doSys){
		IncludeBkgSystematics();
		AllBkg =	AllBkgSyst();
	}
  else AllBkg = GetAllBkg();

  TCanvas* c = SetCanvas(); plot->cd(); 

  Histo* hData = SetData();
  if(!doData) hData = AllBkg;

  float maxData = hData->GetMax();
  float maxMC = AllBkg->GetMax();
  float Max = maxMC > maxData? maxMC : maxData;
  if(doSetLogy){
    hStack->SetMaximum(Max*300);
    hStack->SetMinimum(0.1);
    plot->SetLogy();
  }
  else{
    hStack->SetMaximum(Max*1.15);
    hStack->SetMinimum(0);
  }

  hStack->Draw("hist");
  hStack->GetYaxis()->SetTitle("Number of Events");
  hStack->GetYaxis()->SetTitleSize(0.06);
  hStack->GetYaxis()->SetTitleOffset(0.5);
  hStack->GetYaxis()->SetNdivisions(505);
  hStack->GetXaxis()->SetLabelSize(0.0);

  // Draw systematics histo
  AllBkg->SetFillStyle(3145);
  AllBkg->SetFillColor(kGray+2);
  AllBkg->SetLineColor(kGray+2);
  AllBkg->SetLineWidth(0);
  AllBkg->SetMarkerSize(0);
  if(doSys) AllBkg->Draw("same,e2");

  hData->Draw("pesame");
  if(doSignal){
		// Draw systematics signal
		Histo* hSignalerr; 
    Int_t nSignals = VSignals.size();
		if(verbose) cout << "Drawing " << nSignals << " signals..." << endl;
    for(Int_t  i = 0; i < nSignals; i++){
      if(VSignals.at(i)->GetType() != itSignal) continue; // Consistency
			hSignalerr = (Histo*) VSignals.at(i)->Clone(); // Add systematics to signal
		  hSignalerr->SetFillColor(17); 
		  hSignalerr->SetMarkerStyle(0);
		  hSignalerr->SetFillStyle(3013);
      VSignalsErr.push_back(hSignalerr);
    }
    for(Int_t  i = 0; i < nSignals; i++){
      if(doSys) VSignalsErr.at(i)->Draw("same,e2");
      VSignals.at(i)->Draw("lsame");
    }
  }

  // Draw systematics ratio
  AllBkg->SetFillStyle(3145);
  TH1F* hratioerr =  (TH1F*) AllBkg->Clone();
  Int_t nbins = AllBkg->GetNbinsX(); Float_t binval = 0; Float_t errbin = 0; Float_t totalerror = 0;
  for(int bin = 1; bin <= nbins; bin++){  // Set bin error
    totalerror = AllBkg->GetBinError(bin); 
    binval = AllBkg->GetBinContent(bin);
    errbin = binval > 0 ? totalerror/binval : 0.0;
    hratioerr->SetBinContent(bin, 1);
    hratioerr->SetBinError(bin, errbin);
  }
  hratioerr->SetFillColor(kTeal-7);
  hratioerr->SetFillStyle(3144);
  hratioerr->SetMarkerSize(0);

  TLegend* leg = SetLegend();
  leg->Draw("same");      
	texcms->Draw("same");   // CMS Preliminary
	texlumi->Draw("same");  // The luminosity

  // Set ratio
	pratio->cd();
	hratio = (TH1F*)hData->Clone();
	hratio->Divide(AllBkg);
	SetHRatio();
	hratio->Draw("same");
	hratioerr->Draw("same,e2");
	hratio->Draw("same");

	if(sav){ // Save the histograms
		TString dir = plotFolder + "StopPlots/";
    TString plotname = (outputName == "")? varname + "_" + chan + "_" + tag : outputName;
	  	
		gSystem->mkdir(dir, kTRUE);
		c->Print( dir + plotname + ".pdf", "pdf");
		c->Print( dir + plotname + ".png", "png");
		delete c;
	}
	//delete AllBkg; delete hData; delete leg; delete hratioerr; delete hStack; 
}


//================================================================================
// Datacards
//================================================================================
// SaveHistograms(TString tag = "0")
// TString GetStatUncDatacard()
// MakeDatacardBin(Int_t bin, TString tag = "b")
// MakeDatacardAllBins(TString tag = "b")
// MakeDatacard(TString tag = "0")
// GetShapeUncLines()


void Plot::SaveHistograms(){
  // Save all histograms: all bkg processes, signals, data, systematics, statistics
	if(!doSignal){ std::cout << "No datacards without signal!" << std::endl; return;}
  SavedHistos = true;
	TFile *f;

  TString filename =  varname + "_" + chan;
  if(outputName != "") filename = outputName;
  gSystem->mkdir(limitFolder, kTRUE);
	f = new TFile(limitFolder + filename + ".root", "recreate");

	TH1F* statup; TH1F* statdown; Histo* nom;
	int nVBkgs = (int) VBkgs.size(); 
	int nSig   = (int) VSignals.size();
	int nbins;
	for(int i = 0; i < nVBkgs; i++){
		nom = VBkgs.at(i);
		nbins = nom->GetNbinsX();
		nom->Write();
		if(nom->GetType() == itSys) continue; // no stat for systematics
		for(int j = 1; j <= nbins; j++){
			statup   = nom->GetVarHistoStatBin(j, "up");
			statdown = nom->GetVarHistoStatBin(j, "down");
			statup  ->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Up");
			statdown->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Down");
			statup  ->Write(); statdown->Write();
		}
	}
	for(int i = 0; i < nSig; i++){
		nom = VSignals.at(i);
		nom->SetName(nom->GetProcess());
		nbins = nom->GetNbinsX();
		nom->Write();
		for(int j = 1; j <= nbins; j++){
			statup   = nom->GetVarHistoStatBin(j, "up");
			statdown = nom->GetVarHistoStatBin(j, "down");
			statup  ->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Up");
			statdown->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Down");
			statup  ->Write(); statdown->Write();
		}
	}
  for(int i = 0; i < (Int_t) VSyst.size(); i++){
    nom = VSyst.at(i);
		nom->Write();
  }
  Histo* hData = SetData(); THStack* hStack = GetStack();
  hData->SetName("data_obs"); hData->SetTag("data_obs");
	hData->Write();
	hStack->Write();  
	cout << "-------> Root file created: " << limitFolder + filename + ".root" << endl;
	f->Close(); delete f;
}

TString Plot::GetStatUncDatacard(Int_t iSignal){
  // Returns the lines for statistic uncertainties for
  // signal in position iSignal 
	Histo* nom;
  Histo* hSignal = VSignals.at(iSignal);
	int nbins = 0;
  Int_t nBkgs = VBkgs.size();
	TString lin = TString("");
	for(int i = 0; i < nBkgs+1; i++){
		if(i<nBkgs) nom = VBkgs.at(i);
		else        nom = hSignal;
		nbins = nom->GetNbinsX();
		for(int j = 1; j <= nbins; j++){
			lin += nom->GetProcess() + "_" + chan + Form("_statbin%i shape ", j);
			for(int k = 0; k < nBkgs+1; k++){
				if(i==k) lin += TString(" 1 ");
				else lin += TString(" - ");
			}
			lin += TString("\n");
		}
	}
  return lin;
}

void Plot::MakeDatacard(TString tag, Int_t iSignal){
	if(!doSignal){ std::cout << "No datacards without signal!" << std::endl; return;}
  Histo* hData = SetData(); 
  THStack* hStack = GetStack();
  Histo* hSignal = VSignals.at(iSignal);
	hData->SetTag("data_obs");
	// if(!doData){ hData = hAllBkg;}
	if(!SavedHistos) SaveHistograms(); 
	ofstream outputfile;
	TString filename = TString("datacard_") + varname + TString("_") + chan + "_" + tag + TString(".txt");
  gSystem->mkdir(limitFolder, kTRUE);
	outputfile.open(limitFolder + filename);

  Int_t nBkgs = VBkgs.size();
	Int_t nChan = 1;
	Int_t nSyst = nBkgs + 1;

  outputfile << Form("imax %i\n", nChan);
	outputfile << "jmax *\n";
	//outputfile << Form("kmax %i\n", nSyst);
	outputfile << "kmax *\n";
	outputfile << "##-----------\n";
  outputfile << TString("shapes * ") + chan + " " + varname + "_" + chan + TString(".root") + TString(" $PROCESS $PROCESS_$SYSTEMATIC \n");
	outputfile << "##-----------\n";
  outputfile << TString("bin ") + chan + "\n";
  outputfile << Form("observation %1.0f \n",hData->GetYield());
	outputfile << "##-----------\n";
  TString bin      = TString("bin ");
  TString process1 = TString("process ");
  TString process2 = TString("process ");
  TString rate     = TString("rate    ");
	for(int i = 0; i < nBkgs; i++){ 
		bin += chan + TString(" ");
		process1 += VBkgs.at(i)->GetProcess() + TString(" ");
		process2 += Form(" %i ", i+1);
		rate += Form(" %1.2f ", VBkgs.at(i)->GetYield());
	}
	// Add signal
	bin += chan + TString(" ");
	process1 += hSignal->GetProcess() + TString(" ");
	process2 += Form(" %i ", -1);
	rate += Form(" %1.2f ", hSignal->GetYield());

	outputfile << bin      + TString("\n");
	outputfile << process1 + TString("\n");
	outputfile << process2 + TString("\n");
	outputfile << rate     + TString("\n");
		outputfile << "##-----------\n";
	TString out = TString("Lumi  lnN  ");
	for(int i = 0; i < nBkgs+1; i++){ out += Form(" %1.2f ", 1+sys_lumi);}

	for(int i = 0; i < nBkgs+1; i++){
		if(i<nBkgs){
			out = VBkgs[i]->GetProcess() + " lnN ";
    }
		else	out = hSignal ->GetProcess() + " lnN ";
		for(int j = 0; j < nBkgs+1; j++){
			if(j != i) out += TString(" - ");
			else{
				if(i<nBkgs) out += Form(" %1.2f ", 1+VBkgs[i]->GetSysNorm());
				else        out += Form(" %1.2f ", 1+hSignal->GetSysNorm()); 
			}
		}
		out += TString("\n");
		outputfile << out;
	}
  TString stat = GetStatUncDatacard(iSignal);
  outputfile << stat;
  TString systshapes = GetShapeUncLines();
  outputfile << systshapes;
	outputfile.close();
  cout << "-------> Datacard created: " << limitFolder + filename << endl;
}

TString Plot::GetShapeUncLines(){
	Histo* nom;
  TString sys;
  Int_t nBkgs = VBkgs.size();
	TString lin = TString(""); Int_t nsyst = VSystLabel.size();
	for(Int_t gs = 0; gs < nsyst; gs++){
    sys = VSystLabel.at(gs);
	  lin += sys + " shape ";
    if(sys == "FS" || sys == "genMET"){ // only for FastSim 
			for(int k = 0; k < nBkgs+1; k++){
				if(k < nBkgs) lin += TString(" - ");
				else          lin += TString(" 1 ");
			}
			lin += TString("\n");
		}
    else if(sys == "nlo" || sys == "fsr" || sys == "isr" || sys == "ue"){
			for(int k = 0; k < nBkgs+1; k++){
        if     (k >= nBkgs)                       lin += TString(" - ");
				else if(VBkgs.at(k)->GetTag() == "ttbar") lin += TString(" 1 ");
				else                                      lin += TString(" - ");
			}
			lin += TString("\n");
    }
		else{
			for(int k = 0; k < nBkgs+1; k++) lin += TString(" 1 ");
			lin += TString("\n");
		}
	}
  return lin;
}

//================================================================================
// Extra functions for datacards
//================================================================================
void Plot::MakeDatacardBin(Int_t bin, TString tag, Int_t iSignal){
  if((Int_t) VSignals.size() < 1){ std::cout << "No datacards without signal!" << std::endl; return;}
  Histo* hData = SetData(); THStack* hStack = GetStack();
  Histo* hSignal = VSignals.at(iSignal);
	hData->SetTag("data_obs");

	ofstream outputfile;
	TString filename = TString("datacard_") + varname + TString("_") + chan + "_" + tag + Form("%i",bin) + TString(".txt");
	outputfile.open(limitFolder + filename);

	Int_t nChan = 1;
	Int_t nSyst = nBkgs + 1;

  outputfile << Form("imax %i\n", nChan);
	outputfile << "jmax *\n";
	outputfile << "kmax *\n";
  //outputfile << Form("jmax *", nBkgs);
	//outputfile << Form("kmax %i\n", nSyst);
	outputfile << "##-----------\n";
  outputfile << TString("bin ") + chan + "\n";
  outputfile << Form("observation %1.0f \n",hData->GetBinContent(bin));
	outputfile << "##-----------\n";
  TString bint     = TString("bin ");
  TString process1 = TString("process ");
	TString process2 = TString("process ");
	TString rate     = TString("rate    ");
	for(int i = 0; i < nBkgs; i++){ 
		bint += chan + TString(" ");
		process1 += VBkgs[i]->GetProcess() + TString(" ");
		process2 += Form(" %i ", i+1);
		rate += Form(" %1.2f ", VBkgs[i]->GetBinContent(bin));
	}
	bint += chan + TString(" ");
	process1 += hSignal->GetProcess() + TString(" ");
	process2 += Form(" %i ", -1);
	rate += Form(" %1.2f ", hSignal->GetBinContent(bin));

	outputfile << bint      + TString("\n");
	outputfile << process1 + TString("\n");
	outputfile << process2 + TString("\n");
	outputfile << rate     + TString("\n");
		outputfile << "##-----------\n";
	TString out = TString("Lumi  lnN  ");
	for(int i = 0; i < nBkgs+1; i++){ out += Form(" %1.2f ", 1+sys_lumi);}

	for(int i = 0; i < nBkgs+1; i++){
		if(i<nBkgs) 	out = VBkgs[i]->GetProcess() + " lnN ";
		else	out = hSignal ->GetProcess() + " lnN ";
		for(int j = 0; j < nBkgs+1; j++){
			if(j != i) out += TString(" - ");
			else{
				if(i<nBkgs) out += Form(" %1.2f ", 1+VBkgs[i]->GetSysNorm());
				else        out += Form(" %1.2f ", 1+hSignal->GetSysNorm()); 
			}
		}
		out += TString("\n");
		outputfile << out;
	}
	outputfile.close();
  cout << "-------> Datacard created: " << limitFolder + filename << endl;
}

void Plot::MakeDatacardAllBins(TString tag = "b", Int_t iSignal){
  Histo* hSignal = VSignals.at(iSignal);
  Int_t nbins = hSignal->GetNbinsX();
  for(int i = 1; i <= nbins; i++){
    MakeDatacardBin(i, tag);
  } 
}


//================================================================================
// Other estetics and style
//================================================================================

void Plot::SetTexChan(TString cuts){
  TString t = "";
  if (chan == "ElMu") t += "e^{#pm}#mu^{#mp}";
  else if (chan == "Elec") t += "e^{+}e^{-}";
  else if (chan == "Muon") t += "#mu^{+}#mu^{-}";
  else if (chan == "All") t += "#mu^{+}#mu^{-} + e^{+}e^{-} + e^{#pm}#mu^{#mp}";
  else if (chan == "sameF") t += "#mu^{+}#mu^{-} + e^{+}e^{-}";
  t += cuts;
  texchan = new TLatex(-20, 50, t);
  texchan->SetNDC();
  texchan->SetTextAlign(12);
  texchan->SetX(0.15);
  texchan->SetY(0.81);
  texchan->SetTextFont(42);
  texchan->SetTextSize(0.05);
  texchan->SetTextSizePixels(22);
}

void Plot::SetHRatio(){
  hratio->SetTitle("");
  hratio->GetYaxis()->SetTitle("Data/MC");
  hratio->GetXaxis()->SetTitleSize(0.05);
  hratio->GetYaxis()->CenterTitle();
  hratio->GetYaxis()->SetTitleOffset(0.25);
  hratio->GetYaxis()->SetTitleSize(0.1);
  hratio->GetYaxis()->SetLabelSize(0.1);
  hratio->GetYaxis()->SetNdivisions(402);
  hratio->GetXaxis()->SetTitleOffset(0.9);
  hratio->GetXaxis()->SetLabelSize(0.13);
  hratio->GetXaxis()->SetTitleSize(0.16);
  if (varname == "NBtagsNJets") {  //change bin labels
    hratio->GetXaxis()->SetBinLabel( 1, "(0, 0)");
    hratio->GetXaxis()->SetBinLabel( 2, "(1, 0)");
    hratio->GetXaxis()->SetBinLabel( 3, "(1, 1)");
    hratio->GetXaxis()->SetBinLabel( 4, "(2, 0)");
    hratio->GetXaxis()->SetBinLabel( 5, "(2, 1)");
    hratio->GetXaxis()->SetBinLabel( 6, "(2, 2)");
    hratio->GetXaxis()->SetBinLabel( 7, "(3, 0)");
    hratio->GetXaxis()->SetBinLabel( 8, "(3, 1)");
    hratio->GetXaxis()->SetBinLabel( 9, "(3, 2)");
    hratio->GetXaxis()->SetBinLabel(10, "(3, 3)");
    hratio->GetXaxis()->SetBinLabel(11, "(4, 0)");
    hratio->GetXaxis()->SetBinLabel(12, "(4, 1)");
    hratio->GetXaxis()->SetBinLabel(13, "(4, 2)");
    hratio->GetXaxis()->SetBinLabel(14, "(4, 3)");
    hratio->GetXaxis()->SetBinLabel(15, "(4, 4)");
    hratio->GetXaxis()->SetLabelSize(0.14);
    hratio->GetXaxis()->SetLabelOffset(0.02);
  }
  hratio->GetXaxis()->SetTitle(xtitle);
  hratio->SetMinimum(0.5);
  hratio->SetMaximum(1.5);
}

void Plot::PrintSamples(){
  std::cout << "All the samples included in the plot: " << std::endl;
	for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++)
		cout << " >>> Sample " << VTagSamples.at(i) << " in process " << VTagProcesses.at(i) << std::endl;
}

void Plot::PrintSystematics(){
	std::cout << "Systematics included: \n";
	for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++)
		std::cout << " " << VSystLabel.at(i) << " ";
	std::cout << std::endl;
}
