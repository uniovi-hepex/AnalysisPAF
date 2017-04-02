#include "Plot.h"
#include "TSystem.h"
#include <fstream> 

Histo* Plot::GetH(TString sample, TString sys, Int_t type){ 
  // Returns a Histo for sample and systematic variation 
  TString pathToMiniTree = path;
  if     (type == itSignal) pathToMiniTree = pathSignal;
  else if(type == itData  ) pathToMiniTree = pathData;
  else if(sample.Contains("/")){
    pathToMiniTree = sample(0, sample.Last('/')+1);
    sample = sample(sample.Last('/')+1, sample.Sizeof());
  }
  Looper* ah = new Looper(pathToMiniTree, treeName, var, cut, chan, nb, x0, xN);
  Histo* h = ah->GetHisto(sample, sys);
  h->SetDirectory(0);
  delete ah;
  return h;
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
  Histo* h = GetH(p, sys, type);
  h->SetProcess(pr);
  TString t = (sys == "0")? pr : pr + "_" + sys;
  h->SetTag(t); // Tag = process + sys
  h->SetSysTag(sys);
  h->SetStatUnc();
  Int_t n; Int_t i = 0;
  if(sys != "0") AddToSystematicLabels(sys);

  if(type != itData)  h->Scale(Lumi*1000);
  if(p == "TTJets_aMCatNLO") h->Scale((1.08*0.9)*(1.08*0.9));
  if(type == itBkg){ // Backgrounds
    n = VBkgs.size();
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
    for(i = 0; i < n; i++){
      if(t == VBkgs.at(i)->GetTag()){ // Group backgrounds
        VBkgs.at(i)->Add((TH1F*) h); 
        VBkgs.at(i)->SetStyle(); VBkgs.at(i)->SetStatUnc();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << p << " to " << pr << " group (" << type << ")" << endl;
        return;
      }
    }
  }
  else if(type == itSys){ // Systematic!!
    n = VSyst.size();
    for(i = 0; i < n; i++){ // Group systematics
      if(t == VSyst.at(i)->GetTag()){
        VSyst.at(i)->Add(h); VSyst.at(i)->SetStyle();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << p << " to " << pr << " group (" << type << ")" << endl;
        return;
      }
    }
    h->SetName(t);
  }
  else if(type == itData){
    VTagDataSamples.push_back(p);
    h->SetLineColor(kBlack); 
    h->SetMarkerStyle(20); 
    h->SetMarkerSize(1.1); 
  }
  else if(type == itSignal){
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
    nSignalSamples++;
    n = VSignals.size();
    for(i = 0; i < n; i++){
      if(t == VSignals.at(i)->GetTag()){ // Group signals 
        VSignals.at(i)->Add((TH1F*) h); 
        VSignals.at(i)->SetStyle(); VSignals.at(i)->SetStatUnc();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << p << " to " << pr << " group (" << type << ")" << endl;
        return;
      }
    }

  }
  h->SetType(type); h->SetColor(color); 

  h->SetStyle(); 
  h->SetSysNorm(S);

  AddToHistos(h);
} 

void Plot::GetStack(){ // Sets the histogram hStack
  if(hStack) delete hStack;
  hStack = new THStack(varname, "");
  Int_t nBkgs = VBkgs.size();
  for(Int_t i = 0; i < nBkgs; i++){
    hStack->Add((TH1F*) VBkgs.at(i));
  }
  if(hAllBkg) delete hAllBkg;
  hAllBkg = new Histo(*(TH1F*) hStack->GetStack()->Last(), 3);
  hAllBkg->SetStyle();
  hAllBkg->SetTag("TotalBkg");
  hAllBkg->SetStatUnc();
  if(verbose) cout << Form(" Adding %i systematic to sum of bkg...\n", (Int_t) VSumHistoSystUp.size());
  if(doSys) GroupSystematics();
  
  for(Int_t i = 0; i < (Int_t) VSumHistoSystUp.size(); i++){
    hAllBkg->AddToSystematics(VSumHistoSystUp.at(i));
    hAllBkg->AddToSystematics(VSumHistoSystDown.at(i));
  }
  hAllBkg->SetBinsErrorFromSyst();
}

void Plot::SetData(){  // Returns histogram for Data
  if(hData) delete hData;
  if(gROOT->FindObject("HistoData")) delete gROOT->FindObject("HistoData");
  
  hData = new Histo(TH1F("HistoData", "Data", nb, x0, xN));
  if(VData.size() < 1) doData = false;
  TString p = "";
  for(Int_t i = 0; i < (Int_t) VData.size(); i++){
    p = VData.at(i)->GetName();
    if     (chan == "ElMu" && (p == "DoubleMuon" || p == "DoubleEG"))   continue;
    else if(chan == "Muon" && (p != "DoubleMuon" && p!= "SingleMuon"))  continue;
    else if(chan == "Elec" && (p != "DoubleEG" && p!= "SingleElec"))    continue;
    else if( (chan == "SF" || chan == "sameF") && (p == "MuonEG"))      continue;
    hData->Add(VData.at(i));
  }
  hData->SetLineColor(kBlack); 
  hData->SetMarkerStyle(20); 
  hData->SetMarkerSize(1.1); 
  hData->SetStyle(); hData->SetType(itData); 
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
  var.ReplaceAll(" ", "");
  if(var.Contains(",")){
    TString OneSyst;
    TString TheRest;
    OneSyst = TString(var(0, var.First(",")));
    TheRest = TString(var(var.First(",")+1, var.Sizeof()));
    AddSystematic(OneSyst);
    AddSystematic(TheRest);
    return;
  }
  for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++){
    AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, 0, var+"Up");
    AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, 0, var+"Down");
  }
  if(verbose) cout << "[Plot::AddSystematic] Systematic histograms added to the list for variation: " << var << endl;
}

void Plot::GroupSystematics(){
  VSumHistoSystUp.clear(); VSumHistoSystDown.clear();
  TString var = "";
  for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++){
    var = VSystLabel.at(i);
    TString tag = var; TString name = "";
    Histo* hsumSysUp = NULL; Histo* hsumSysDown = NULL; 
    if(gROOT->FindObject(tag + "_SystSumUp"))   delete gROOT->FindObject(tag + "_SystSumUp");
    if(gROOT->FindObject(tag + "_SystSumDown")) delete gROOT->FindObject(tag + "_SystSumDown");
    hsumSysUp   = new Histo(TH1F(tag + "_SystSumUp",  tag + "_SystSumUp" ,   nb, x0, xN));
    hsumSysDown = new Histo(TH1F(tag + "_SystSumDown",tag + "_SystSumDown" , nb, x0, xN));
    for(Int_t i = 0; i < (Int_t) VSyst.size(); i++){
      tag =  VSyst.at(i)->GetTag();
      name = VSyst.at(i)->GetName();
      if(name.Contains("T2tt") || name.Contains("FastSim") || name.Contains("FullSim") ) continue;
      if(name.BeginsWith("S_")) continue;
      if(tag.Contains(var+"Up") )   hsumSysUp  ->Add( VSyst.at(i));
      if(tag.Contains(var+"Down")) hsumSysDown->Add( VSyst.at(i));
    }
    AddSumHistoSystematicUp(hsumSysUp);
    AddSumHistoSystematicDown(hsumSysDown);
  }
}

//================================================================================
// Drawing
//================================================================================
// SetLegend()
// TCanvas* SetCanvas()
// void DrawStack(TString tag = "0", bool sav = 0)
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
  if(nVBkgs > 0 && hAllBkg) MinYield = hAllBkg->GetYield()/5000;
  for(int i = nVBkgs-1; i >= 0; i--){
    if(VBkgs.at(i)->GetYield() < MinYield) continue;
    else VBkgs.at(i)->AddToLegend(leg,doYieldsInLeg);
  }
  for(int i = VSignals.size()-1; i >= 0; i--) VSignals[i]->AddToLegend(leg, doYieldsInLeg);
  if(doData && hData && VData.size() > 0){
    hData->SetTag("Data");
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

void Plot::DrawComp(TString tag, bool sav, bool doNorm){
  TCanvas* c = SetCanvas();  plot->cd();
  int nsamples = VSignals.size();
  float themax = 0;
  float yield = 0;
  Histo* signal;
  if(nsamples == 0) return;
  else signal = VSignals.at(0);
  yield = VSignals.at(0)->Integral();
  if(doNorm) signal->Scale(1/yield);
  signal->SetTitle("");
  signal->SetLineWidth(2);
  signal->GetXaxis()->SetLabelSize(0.0);
  signal->Draw("pe");
  float max = VSignals.at(0)->GetMaximum();

  for(Int_t  i = 1; i < nsamples; i++){
    yield = VSignals.at(i)->Integral();
    if(doNorm) VSignals.at(i)->Scale(1/yield);
    max = VSignals.at(i)->GetMaximum(); 
    if(max > themax) themax = max;
    VSignals.at(i)->Draw("pesame");
  }
  VSignals.at(0)->SetMaximum(themax*ScaleMax);
  VSignals.at(0)->SetMinimum(PlotMinimum);
  TLegend* leg = SetLegend();
  leg->SetTextSize(0.041);
  leg->Draw("same");

  pratio->cd();
  vector<TH1F*> ratios;
  TH1F* htemp = NULL;
  hratio = (TH1F*)VSignals.at(0)->Clone("hratio_sig");
  SetHRatio();
  hratio->SetMaximum(RatioMax);
  hratio->SetMinimum(RatioMin);
  for(Int_t  i = 1; i < nsamples; i++){
    htemp = (TH1F*)hratio->Clone("htemp");
    htemp->Divide(VSignals.at(i)); 
    htemp->SetLineColor(VSignals.at(i)->GetColor());
    ratios.push_back(htemp);
    ratios.at(i-1)->Draw("same");
  }

  if(sav){ // Save the histograms
    TString dir = plotFolder;
    TString plotname = varname + "_" + chan + "_" + tag + "_compare";
    gSystem->mkdir(dir, kTRUE);
    c->Print( dir + plotname + ".png", "png");
    delete c;
  }
  if(htemp) delete htemp;
  ratios.clear();
}

void Plot::DrawStack(TString tag = "0", bool sav = 0){
  //if(doSys) IncludeBkgSystematics();
  std::vector<Histo*> VStackedSignals;

  TCanvas* c = SetCanvas(); plot->cd(); 
  GetStack();
  SetData();
  if(!doData) hData = hAllBkg;

  float maxData = hData->GetMax();
  float maxMC = hAllBkg->GetMax();
  float Max = maxMC > maxData? maxMC : maxData;
  if(doSetLogy){
    hStack->SetMaximum(Max*ScaleLog);
    hStack->SetMinimum(PlotMinimum + 0.1*(PlotMinimum + 1));
    plot->SetLogy();
  }
  else{
    hStack->SetMaximum(Max*ScaleMax);
    hStack->SetMinimum(PlotMinimum);
  }
  hStack->Draw("hist");
  hStack->GetYaxis()->SetTitle("Number of Events");
  hStack->GetYaxis()->SetTitleSize(0.06);
  hStack->GetYaxis()->SetTitleOffset(0.5);
  hStack->GetYaxis()->SetNdivisions(505);
  hStack->GetXaxis()->SetLabelSize(0.0);

  // Draw systematics histo
  hAllBkg->SetFillStyle(3145);
  hAllBkg->SetFillColor(kGray+2);
  hAllBkg->SetLineColor(kGray+2);
  hAllBkg->SetLineWidth(0);
  hAllBkg->SetMarkerSize(0);
  if(doSys) hAllBkg->Draw("same,e2");
  hData->Draw("pesame");
  Histo* hSignalerr = NULL; 
  VSignalsErr.clear();
  if(doSignal){
    // Draw systematics signal
    Int_t nSignals = VSignals.size();
    /* if(verbose) cout << "Drawing " << nSignals << " signals..." << endl;
       for(Int_t  i = 0; i < nSignals; i++){
       if(VSignals.at(i)->GetType() != itSignal) continue; // Consistency
       hSignalerr = (Histo*) VSignals.at(i)->Clone("hSignalErr"); // Add systematics to signal
       hSignalerr->SetFillColor(17); 
       hSignalerr->SetMarkerStyle(0);
       hSignalerr->SetFillStyle(3013);
       VSignalsErr.push_back(hSignalerr);
       }*/
    Histo* htemp;
    for(Int_t  i = 0; i < nSignals; i++){
      //if(doSys) VSignalsErr.at(i)->Draw("same,e2");
      if(doStackSignal){
	htemp = VSignals.at(i);
	htemp->Add(hAllBkg);
	VStackedSignals.push_back(htemp);
	VStackedSignals.at(i)->Draw("lsame");
      }
      else VSignals.at(i)->Draw("lsame");
    }
  }

  // Draw systematics ratio
  hAllBkg->SetFillStyle(3145);
  TH1F* hratioerr =  (TH1F*) hAllBkg->Clone("hratioerr");
  Int_t nbins = hAllBkg->GetNbinsX(); Float_t binval = 0; Float_t errbin = 0; Float_t totalerror = 0;
  for(int bin = 1; bin <= nbins; bin++){  // Set bin error
    totalerror = hAllBkg->GetBinError(bin); 
    binval = hAllBkg->GetBinContent(bin);
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
  hratio = (TH1F*)hData->Clone("hratio");
  hratio->Divide(hAllBkg);
  SetHRatio();
  hratio->Draw("same");
  hratioerr->Draw("same,e2");
  hratio->Draw("same");

  if(sav){ // Save the histograms
    TString dir = plotFolder;
    TString plotname = (outputName == "")? varname + "_" + chan + "_" + tag : outputName;
	  	
    gSystem->mkdir(dir, kTRUE);
    c->Print( dir + plotname + ".pdf", "pdf");
    c->Print( dir + plotname + ".png", "png");
    delete c;
  }
  if(leg) delete leg; //if(hData) delete hData;
  //if(hAllBkg) delete hAllBkg; if(hStack) delete hStack; 
  if(hratioerr) delete hratioerr; if(hSignalerr) delete hSignalerr;
  VStackedSignals.clear();
}

void Plot::ScaleProcess(TString pr, Float_t SF){
  for(Int_t i = 0; i < (Int_t) VBkgs.size(); i++) if(VBkgs.at(i)->GetProcess() == (pr)){
    VBkgs.at(i)->Scale(SF);
    VBkgs.at(i)->SetStyle();
  }
  for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) if(VSignals.at(i)->GetProcess() == (pr)){
    VSignals.at(i)->Scale(SF);
    VSignals.at(i)->SetStyle();
  }
  for(Int_t i = 0; i < (Int_t) VSyst.size(); i++) if(VSyst.at(i)->GetProcess().BeginsWith(pr+"_")){
    VSyst.at(i)->Scale(SF);
    VSyst.at(i)->SetStyle();
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
    nom->SetName(nom->GetProcess());
    nbins = nom->GetNbinsX();
    nom->Write();
    if(doStatUncInDatacard){
      if(nom->GetType() == itSys) continue; // no stat for systematics
      for(int j = 1; j <= nbins; j++){
        statup   = nom->GetVarHistoStatBin(j, "up");
        statdown = nom->GetVarHistoStatBin(j, "down");
        statup  ->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Up");
        statdown->SetName(nom->GetProcess() + "_" + nom->GetProcess() + "_" + chan + Form("_statbin%i", j) + "Down");
        statup  ->Write(); statdown->Write();
      }
    }
  }
  TString namesignal;
  for(int i = 0; i < nSig; i++){
    nom = VSignals.at(i);
    cout << "nominal integral for " << nom->GetProcess() << " is " <<nom->Integral() << endl;
    namesignal = nom->GetProcess();
    nom->SetName(namesignal);
    nbins = nom->GetNbinsX();
    nom->Write();
    if(doStatUncInDatacard){
      for(int j = 1; j <= nbins; j++){
        statup   = nom->GetVarHistoStatBin(j, "up");
        statdown = nom->GetVarHistoStatBin(j, "down");
        statup  ->SetName(namesignal + "_" + namesignal + "_" + chan + Form("_statbin%i", j) + "Up");
        statdown->SetName(namesignal + "_" + namesignal + "_" + chan + Form("_statbin%i", j) + "Down");
        statup  ->Write(); statdown->Write();
      }
    }
  }
  for(int i = 0; i < (Int_t) VSyst.size(); i++){
    nom = VSyst.at(i);
    cout << "Saving unc variation " << nom->GetName() << " " << nom->Integral() << endl;
    nom->Write();
  }
  SetData(); GetStack();
  hData->SetName("data_obs"); 
  hData->SetTag("data_obs");
  hData->Write();
  hStack->Write();  
  cout << "-------> Root file created: " << limitFolder + filename + ".root" << endl;
  //	f->Close(); 
  //	delete f;
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
    else        nom = VSignals.at(iSignal);
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
  SetData(); 
  Histo* hSignal = VSignals.at(iSignal);
  cout << "signal in makedatacard" << hSignal->Integral() << endl;
  hSignal->SetStyle();
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
  for(int i = 0; i < nBkgs+1; i++){ out += Form(" %1.3f ", 1+sys_lumi);}
  out += TString("\n");
  outputfile << out;
 
  for(int i = 0; i < nBkgs+1; i++){
    if(i<nBkgs)  out = VBkgs[i]->GetProcess() + " lnN ";

    else{
      out = hSignal ->GetProcess() + " lnN ";
      cout << "jeje " <<  hSignal->GetProcess() << " " << hSignal->GetSysNorm() << endl;
    }

    for(int j = 0; j < nBkgs+1; j++){
      if(j != i) out += TString(" - ");
      else{
	if(i<nBkgs) out += Form(" %1.2f ", 1+VBkgs[i]->GetSysNorm());
	else        out += Form(" %1.2f ", 1+hSignal->GetSysNorm()); 
      }
    }
    if( (i<nBkgs && 	VBkgs[i]->GetSysNorm() < 0) || (i == nBkgs && hSignal ->GetSysNorm() < 0))    continue;

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
    else if(sys == "nlo" || sys == "fsr" || sys == "isr" || sys == "ue" || sys == "had"){
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
  SetData(); GetStack();
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
  for(int i = 0; i < nBkgs+1; i++){ out += Form(" %1.3f ", 1+sys_lumi);}
  out += TString("\n");
  outputfile << out;

  for(int i = 0; i < nBkgs+1; i++){
    if(i<nBkgs)  out = VBkgs[i]->GetProcess() + " lnN ";
    else  out = hSignal ->GetProcess() + " lnN ";
    for(int j = 0; j < nBkgs+1; j++){
      if(j != i) out += TString(" - ");
      else{
	if(i<nBkgs) out += Form(" %1.2f ", 1+VBkgs[i]->GetSysNorm());
	else        out += Form(" %1.2f ", 1+hSignal->GetSysNorm()); 
      }
    }

    if( (i<nBkgs && 	VBkgs[i]->GetSysNorm() < 0) || (i == nBkgs && hSignal ->GetSysNorm() < 0))  continue;
	  
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
  hratio->SetMinimum(RatioMin);
  hratio->SetMaximum(RatioMax);
}

Int_t Plot::GetColorOfProcess(TString pr){
  for(Int_t i = 0; i < (Int_t) VBkgs.size(); i++) if(VBkgs.at(i)->GetProcess() == (pr)) return VBkgs.at(i)->GetFillColor();
  for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) if(VSignals.at(i)->GetProcess() == (pr)) return VSignals.at(i)->GetLineColor();
  return 0;
}

//================================================================================
// Printing tables, yields, systematics...
//================================================================================

Plot* Plot::NewPlot(TString newVar, TString newCut, TString newChan, Int_t newnbins, Float_t newbin0, Float_t newbinN, TString newtitle, TString newXtitle){
  if(newVar  == "") newVar  = var;
  if(newCut  == "") newCut  = cut;
  if(newChan == "") newChan = chan;
  if(newnbins == -1  ) newnbins = nb;
  if(newbin0  == -999) newbin0  = x0;
  if(newbinN  == -999) newbinN  = xN;
  if(newtitle == "" ) newtitle = title;
  if(newXtitle == "" ) newXtitle = xtitle;

  Plot* p = new Plot(newVar, newCut, newChan, newnbins, newbin0, newbinN, newtitle, newXtitle);
  p->SetPath(path); p->SetTreeName(treeName);
  p->SetOutputName(outputName);
  p->SetPathSignal(pathSignal);

  p->verbose         = false;        
  p->doSys           = doSys;          
  p->doData          = doData;         
  p->doYieldsInLeg   = doYieldsInLeg;  
  p->doSingleLep     = doSingleLep;    
  p->doStackOverflow = doStackOverflow;
  p->doSignal        = doSignal;       
  p->doSetLogy       = doSetLogy;      
  p->doStackSignal   = doStackSignal;  
  p->doStatUncInDatacard = doStatUncInDatacard;

  Int_t nMCSamples   = VTagSamples.size();
  Int_t nDataSamples = VTagDataSamples.size();
  Int_t type;

  for(Int_t i = 0; i < nMCSamples; i++){
    type = i < (nMCSamples-nSignalSamples) ? itBkg : itSignal;
    p->AddSample(VTagSamples.at(i), VTagProcesses.at(i), type, GetColorOfProcess(VTagProcesses.at(i)));
  }
  for(Int_t i = 0; i < nDataSamples; i++)
    p->AddSample(VTagDataSamples.at(i), "Data", itData, 1);

  return p;
}

void Plot::PrintSamples(){
  std::cout << "All the samples included in the plot: " << std::endl;
  for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++)
    cout << " >>> Sample " << VTagSamples.at(i) << " in process " << VTagProcesses.at(i) << std::endl;
  for(Int_t i = 0; i < (Int_t) VTagDataSamples.size(); i++)
    cout << " >>> Data sample " << VTagDataSamples.at(i) << std::endl;    
}

void Plot::PrintSystematics(){
  std::cout << "Systematics included: \n";
  for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++)
    std::cout << " " << VSystLabel.at(i) << " ";
  std::cout << std::endl;
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

Float_t Plot::GetTotalSystematic(TString pr){
  Float_t sys2 = 0;
  Int_t nSys  = VSystLabel.size();
  Float_t nom = GetYield(pr);
  for(Int_t j = 0; j < nSys; j++) sys2 += fabs((GetYield(pr,VSystLabel.at(j))-nom) * (GetYield(pr,VSystLabel.at(j))-nom));
  return TMath::Sqrt(sys2);
}

void Plot::PrintYields(TString cuts, TString labels, TString channels, TString options){
  if(cuts == "") cuts = cut;
  //cuts.ReplaceAll(" ", ""); 
  std::vector<TString> VCuts = std::vector<TString>();
  std::vector<TString> VLCut = std::vector<TString>();
  std::vector<TString> VChan = std::vector<TString>();
  TString thisCut = ""; TString thisLabel = ""; TString thisChan;
  Int_t nBkgs = VBkgs.size();
  Int_t nSignals = VSignals.size();
  Int_t nrows = nBkgs + nSignals; 
  if(nBkgs > 0) nrows++; if(doData) nrows++;
  Int_t ncolumns = 1 + cuts.CountChar(','); 
  channels.ReplaceAll(" ", "");
  Bool_t doChannels = false; if(cuts.CountChar(',') == channels.CountChar(',')) doChannels = true;
  TResultsTable t(nrows, ncolumns, true); //cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetRowTitleHeader("Process");
  Plot* np = NULL;
  for(Int_t k = 0; k < ncolumns; k++){
    if(cuts.Contains(",")){
      thisCut = cuts(0, cuts.First(','));
      thisLabel = labels(0, labels.First(','));
      thisChan = channels(0, channels.First(','));
      VCuts.push_back(thisCut);
      VLCut.push_back(thisLabel);
      if(doChannels) VChan.push_back(thisChan);
      else VChan.push_back(chan);
      cuts = cuts(cuts.First(',')+1, cuts.Sizeof());
      labels = labels(labels.First(',')+1, labels.Sizeof());
      channels = channels(channels.First(',')+1, channels.Sizeof());
    }
    else{
      VCuts.push_back(cuts);
      VLCut.push_back(labels);
      if(doChannels) VChan.push_back(channels);
      else VChan.push_back(chan);
    }
  }

  // Set row titles
  for(Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i, VBkgs.at(i)->GetProcess());
  t.SetRowTitle(nBkgs, "Total Background");
  for(Int_t i = nBkgs+1; i < nSignals+nBkgs+1; i++) t.SetRowTitle(i, VSignals.at(i-(nBkgs+1))->GetProcess());
  t.SetRowTitle(nBkgs+1+nSignals, "Data");

  // Set column titles
  for(Int_t k = 0; k < ncolumns; k++) t.SetColumnTitle(k, VLCut.at(k));

  // Fill the table with the yields
  for(Int_t k = 0; k < ncolumns; k++){
    if(VCuts.at(k) == cut) np = this;
    else{
      np = NewPlot(var, VCuts.at(k), VChan.at(k));
      for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++) np->AddSystematic(VSystLabel.at(i));
    }

    Float_t TotalBkgSystematic = 0;
    for(Int_t i = 0; i < nBkgs; i++){
      t[i][k] = np->VBkgs.at(i)->GetYield();
      t[i][k].SetError(np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()));
      TotalBkgSystematic += (np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()))*(np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()));
    }
    TotalBkgSystematic = TMath::Sqrt(TotalBkgSystematic);
    np->GetStack();
    t[nBkgs][k] = np->hAllBkg->GetYield();
    t[nBkgs][k].SetError(TotalBkgSystematic); 
    for(Int_t i = nBkgs+1; i < nSignals+nBkgs+1; i++){
      t[i][k] = np->VSignals.at(i-(nBkgs+1))->GetYield();
      t[i][k].SetError(np->GetTotalSystematic(np->VSignals.at(i-(nBkgs+1))->GetProcess()));
    }
    if(doData){
      np->SetData();
      t[nBkgs+1+nSignals][k] = np->hData->GetYield();
      t[nBkgs+1+nSignals][k].SetError(TMath::Sqrt(np->hData->GetYield()));
    }
  }
  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  if(options.Contains("tex"))  t.SaveAs(plotFolder + "/" + YieldsTableName + ".tex");
  if(options.Contains("html")) t.SaveAs(plotFolder + "/" + YieldsTableName + ".html");
  if(options.Contains("txt"))  t.SaveAs(plotFolder + "/" + YieldsTableName + ".txt");
}
