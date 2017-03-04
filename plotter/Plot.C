#include "Plot.h"
#include "TSystem.h"
#include <fstream> 

Histo* Plot::GetH(TString sample, TString sys){ 
  // Returns a Histo* for sample and systematic variation 
  AnalHisto* ah = new AnalHisto(sample, cut, chan, path, treeName, sys);
	ah->SetHisto(sample, nb, x0, xN);
	ah->Fill(var, sys);
	Histo* g = ah->GetHisto();
	g->SetDirectory(0);
  g->SetName(sample);
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
  Histo* h = GetH(p, sys);
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
      //  cout << "[" << t << "]Adding histogram with lowx = " << h->GetBinLowEdge(1) << " with father histogram with lowx = " << VBkgs.at(i)->GetBinLowEdge(1) << endl;
				VBkgs.at(i)->Add((TH1F*) h); 
				VBkgs.at(i)->SetStyle(); VBkgs.at(i)->SetStatUnc();
				return;
			}
		}
  }
	else if(type < 0){ // Systematic!!
		n = VSyst.size();
		for(i = 0; i < n; i++){ // Group systematics
			if(t == VSyst.at(i)->GetTag()){
     //   cout << "[" << t << "]Adding sys histogram with lowx = " << h->GetBinLowEdge(1) << " with father histogram with lowx = " << VSyst.at(i)->GetBinLowEdge(1) << endl;
        //cout << "Adding histogram with nbins = " << h->GetNbinsX() << " with father histogram with nbins = " << VSyst.at(i)->GetNbinsX() << endl;
				VSyst.at(i)->Add(h); VSyst.at(i)->SetStyle();
				return;
			}
		}
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
	THStack* hStack0 = new THStack(var, "");
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
  if(verbose) cout << "[Plot::AddToHistos] Added histogram " << p->GetName() << " to " << p->GetTag() << " group" << endl;
}

//================================================================================
// Systematics
//================================================================================

void Plot::AddSystematic(TString var){
	for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++){
		AddSample(VTagSamples.at(i), VTagProcesses.at(i), -1, 0, 0, var+"Up");
		AddSample(VTagSamples.at(i), VTagProcesses.at(i), -1, 0, 0, var+"Down");
	}
  if(verbose) cout << "[Plot::AddSystematic] Systematic histograms added to the list for variation: " << var << endl;
	VSystLabel.push_back(var);
}

void Plot::IncludeBkgSystematics(){
  if(ShowSystematics) return;
  //cout << "Including syst" << endl;
	TString pr; TString ps; TString dir;
  Int_t nBkgs = VBkgs.size(); Int_t nSig = VSignals.size();
  Int_t nSyst = VSyst.size();
	for(int i = 0; i < nBkgs; i++){  // Backgrounds in i
		pr = VBkgs.at(i)->GetProcess() ;
		for(int k = 0; k < nSyst; k++){ // Systematics in k
			ps = VSyst.at(k)->GetTag();
			dir = (ps.Contains("Down") || ps.Contains("down"))? "Down" : "Up";
			if(ps.BeginsWith(pr)){
        //cout << " process = " << pr << ", var = " << ps << endl;
        //cout << " Bin 1, bkg y var: " << VBkgs.at(i)->GetBinContent(10) << ", " << VSyst.at(k)->GetBinContent(10) << endl;
        //cout << " Bin 1, sys up y down, antes: " << VBkgs.at(i)->vsysu[9] << ", " << VBkgs.at(i)->vsysd[9] << endl;
    		VBkgs.at(i)->AddToSystematics(VSyst.at(k), dir);
        //cout << " Bin 1, sys up y down, luego: " << VBkgs.at(i)->vsysu[9] << ", " << VBkgs.at(i)->vsysd[9] << endl;
      }
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

 // cout << "nbins = " << nbins << endl;
  AllBkg->SetStatUnc();
  //for(int k = 0; k < nBkgs; k++) VBkgs.at(k)->SetStatUnc();

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
  for(int i = VSignals.size()-1; i >= 0; i--){
      if(VSignals.at(i)->GetType() < 0) continue;
			VSignals[i]->AddToLegend(leg, doYieldsInLeg);
		}
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
    TString plotname = var + "_" + chan + "_" + tag + "_compare";
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
		TH1F* hSignalerr; 
    for(Int_t  i = 0; i < (Int_t) VSignals.size(); i++){
      if(VSignals.at(i)->GetType() < 0) continue; // 
	//		hSignalerr = (Histo*) VSignals.at(i)->Clone(); // Add systematics to signal
	//	  hSignalerr->SetFillColor(kGreen-10); 
	//	  hSignalerr->SetMarkerStyle(0);
	//	  hSignalerr->SetFillStyle(3145);
  //    hSignalerr->SetType(-10); // Error bands for signals
  //    VSignals.push_back(hSignalerr);
      VSignals.at(i)->Draw("lsame");
    }
    for(Int_t  i = 0; i < (Int_t) VSignals.size(); i++){
      if(VSignals.at(i)->GetType() != -10) continue; 
  //    VSignals.at(i)->Draw("same,e2");
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
		TString plotname = var + "_" + chan + "_" + tag;
		gSystem->mkdir(dir, kTRUE);
		c->Print( dir + plotname + ".pdf", "pdf");
		c->Print( dir + plotname + ".png", "png");
		delete c;
	}
}


//================================================================================
// Datacards
//================================================================================
// SaveHistograms(TString tag = "0")
// TString GetStatUncDatacard()
// MakeDatacardBin(Int_t bin, TString tag = "b")
// MakeDatacardAllBins(TString tag = "b")
// MakeDatacard(TString tag = "0")
// GetShapeUncDatacard()

/*
void Plot::SaveHistograms(TString tag = "0"){
	if(!doSignal){ std::cout << "No datacards without signal!" << std::endl; return;}
	TFile *f;
  TString filename =  var + "_" + chan + "_" + tag + ".root";
	f = new TFile(limitFolder + filename, "recreate");

	TH1F* statup; TH1F* statdown; Histo* nom;
	int nVBkgs = (int) VBkgs.size(); 
	int nSig   = (int) VSignals.size();
	int nbins;
	for(int i = 0; i < nVBkgs; i++){
		nom = VBkgs.at(i);
		nbins = nom->GetNbinsX();
		nom->Write();
		if(nom->GetType() < 0) continue; // no stat for systematics
		for(int j = 1; j <= nbins; j++){
			statup   = nom->GetVarHistoStatBin(j, "up");
			statdown = nom->GetVarHistoStatBin(j, "down");
			statup  ->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Up");
			statdown->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Down");
			statup  ->Write(); statdown->Write();
			//delete statup; delete statdown;
		}
		//delete nom;
	}
	for(int i = 0; i < nSig; i++){
		nom = VSignals.at(i);
		nbins = nom->GetNbinsX();
		nom->Write();
    //cout << "Saving " << nom->tag << "..." << endl;
		if(nom->GetType() < 0) continue; // no stat for systematics
		for(int j = 1; j <= nbins; j++){
			statup   = nom->GetVarHistoStatBin(j, "up");
			statdown = nom->GetVarHistoStatBin(j, "down");
			statup  ->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Up");
			statdown->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Down");
			statup  ->Write(); statdown->Write();
			//delete statup; delete statdown;
		}
		//delete nom;
	}
  for(int i = 0; i < (Int_t) VSyst.size(); i++){
    nom = VSyst.at(i);
		nom->Write();
  }
  Histo* hData = SetData(); THStack* hStack = GetStack();
	hData->Write();
	hStack->Write();  
	cout << "-------> Root file created: " << limitFolder + filename << endl;
	f->Close(); delete f;
}

TString Plot::GetStatUncDatacard(){
	Histo* nom;
	int nbins = 0;
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

void Plot::MakeDatacardBin(Int_t bin, TString tag = "b"){
  if(!doSignal){ std::cout << "No datacards without signal!" << std::endl; return;}
  Histo* hData = SetData(); THStack hStack = GetStack();
	hData->SetTag("data_obs");

	ofstream outputfile;
	TString filename = TString("datacard_") + var + TString("_") + chan + "_" + tag + Form("%i",bin) + TString(".txt");
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

void Plot::MakeDatacardAllBins(TString tag = "b"){
  Int_t nbins = hSignal->GetNbinsX();
  for(int i = 1; i <= nbins; i++){
    MakeDatacardBin(i, tag);
  } 
}

void Plot::MakeDatacard(TString tag = "0"){
	if(!doSignal){ std::cout << "No datacards without signal!" << std::endl; return;}
  Histo* hData = SetData(); 
  THStack* hStack = GetStack();
	hData->SetTag("data_obs");
	// if(!doData){ hData = hAllBkg;}
	SaveHistograms(tag); 
	ofstream outputfile;
	TString filename = TString("datacard_") + var + TString("_") + chan + "_" + tag + TString(".txt");
	outputfile.open(limitFolder + filename);

	Int_t nChan = 1;
	Int_t nSyst = nBkgs + 1;

  outputfile << Form("imax %i\n", nChan);
	outputfile << "jmax *\n";
	//outputfile << Form("kmax %i\n", nSyst);
	outputfile << "kmax *\n";
	outputfile << "##-----------\n";
  outputfile << TString("shapes * ") + chan + " " + limitFolder + var + "_" + chan + "_" + tag + TString(".root") + TString(" $PROCESS $PROCESS_$SYSTEMATIC \n");
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
		process1 += VBkgs[i]->GetProcess() + TString(" ");
		process2 += Form(" %i ", i+1);
		rate += Form(" %1.2f ", VBkgs[i]->GetYield());
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
  TString stat = GetStatUncDatacard();
  outputfile << stat;
  TString systshapes = GetShapeUncDatacard();
  outputfile << systshapes;
	outputfile.close();
  cout << "-------> Datacard created: " << limitFolder + filename << endl;
}

TString Plot::GetShapeUncDatacard(){
	Histo* nom;
  TString sys;
	TString lin = TString(""); Int_t nsyst = VSystLabel.size();
	for(Int_t gs = 0; gs < nsyst; gs++){
    sys = VSystLabel.at(gs);
	  lin += sys + " shape ";
    if(sys == "FS"){ // FullSim/FastSim -> only signal
			for(int k = 0; k < nBkgs+1; k++){
				if(k < nBkgs) lin += TString(" - ");
				else          lin += TString(" 1 ");
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
*/

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
  if (var == "NBtagsNJets") {  //change bin labels
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
