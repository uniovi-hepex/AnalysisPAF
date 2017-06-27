#include "Plot.h"
#include "TSystem.h"
#include "TLine.h"
#include <fstream> 

Histo* Plot::GetH(TString sample, TString sys, Int_t type){ 
  // Returns a Histo for sample and systematic variation 
  TString pathToMiniTree = path;
  if     (type == itSignal || sample.Contains("T2tt")) pathToMiniTree = pathSignal;
  else if(type == itData  ) pathToMiniTree = pathData;
  else if(sample.Contains("/")){
    pathToMiniTree = sample(0, sample.Last('/')+1);
    sample = sample(sample.Last('/')+1, sample.Sizeof());
  }
  Looper* ah;
  if(xN != x0) ah = new Looper(pathToMiniTree, treeName, var, cut, chan, nb, x0, xN);
  else         ah = new Looper(pathToMiniTree, treeName, var, cut, chan, nb, vbins);

  ah->SetOptions(LoopOptions);
  Histo* h = ah->GetHisto(sample, sys);
  h->SetDirectory(0);
  h->SetStyle(); 
  if(sys.Contains("stat")){
    if(sys.Contains("Up")) for(Int_t i = 0; i <= nb; i++) h->SetBinContent(i, h->GetBinContent(i) + h->GetBinError(i));
    else                   for(Int_t i = 0; i <= nb; i++) h->SetBinContent(i, h->GetBinContent(i) - h->GetBinError(i));
  }
  delete ah;
  return h;
}

void Plot::AddSample(TString p, TString pr, Int_t type, Int_t color, TString sys, TString options){
  p.ReplaceAll(" ", "");
  if(p.Contains(",")){
    TString First_p = p(0, p.First(','));
    TString theRest = p(p.First(',')+1, p.Sizeof());
    AddSample(First_p, pr, type, color, sys, options);
    AddSample(theRest, pr, type, color, sys, options);
    return;
  }

  SetLoopOptions(options);
  VTagOptions.push_back(options);

  Histo* h = GetH(p, sys, type);
  h->SetProcess(pr);
  TString t = (sys == "0")? pr : pr + "_" + sys;
  h->SetTag(t); // Tag = process + sys
  h->SetSysTag(sys);
  Int_t n; Int_t i = 0;
  if(sys != "0") AddToSystematicLabels(sys);

  Bool_t isFakeFromData = (options.Contains("fake") || options.Contains("Fake")) && ! options.Contains("sub");
  if(type != itData && !isFakeFromData)  h->Scale(Lumi*1000);
  if(p == "TTJets_aMCatNLO") h->Scale((1.08*0.9)*(1.08*0.9));
  if(type == itBkg){ // Backgrounds
    n = VBkgs.size();
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
    for(i = 0; i < n; i++){
      if(t == VBkgs.at(i)->GetTag()){ // Group backgrounds
        VBkgs.at(i)->Add((TH1F*) h); 
        VBkgs.at(i)->SetStyle(); 
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
        return;
      }
    }
  }
  else if(type == itSys){ // Systematic!!
    n = VSyst.size();
    for(i = 0; i < n; i++){ // Group systematics
      if(t == VSyst.at(i)->GetTag()){
        VSyst.at(i)->Add(h); VSyst.at(i)->SetStyle();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
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
    SetSignalProcess(pr);
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
    nSignalSamples++;
    n = VSignals.size();
    for(i = 0; i < n; i++){
      if(t == VSignals.at(i)->GetTag()){ // Group signals 
        VSignals.at(i)->Add((TH1F*) h); 
        VSignals.at(i)->SetStyle(); 
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
        return;
      }
    }
  }

  h->SetType(type); h->SetColor(color); h->SetStyle(); h->SetSysNorm(1);
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
  
  if(x0 != xN) hData = new Histo(TH1F("HistoData", "Data", nb, x0, xN));
  else         hData = new Histo(TH1F("HistoData", "Data", nb, vbins));
  if(VData.size() < 1) doData = false;
  TString p = "";

  for(Int_t i = 0; i < (Int_t) VData.size(); i++){
    p = VData.at(i)->GetName();
    if     (chan == "ElMu" && (p == "DoubleMuon" || p == "DoubleEG"))   continue;
    else if((chan == "MuMu" || chan == "Muon") && (p != "DoubleMuon" && p!= "SingleMuon"))  continue;
    else if((chan == "ElEl" || chan == "Elec") && (p != "DoubleEG" && p!= "SingleElec"))    continue;
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
    if(VTagOptions.at(i).Contains("Fake") || VTagOptions.at(i).Contains("fake")) continue;
    AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Up", VTagOptions.at(i));
    AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Down", VTagOptions.at(i));
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
    if(x0 != xN){
      hsumSysUp   = new Histo(TH1F(tag + "_SystSumUp",  tag + "_SystSumUp" ,   nb, x0, xN));
      hsumSysDown = new Histo(TH1F(tag + "_SystSumDown",tag + "_SystSumDown" , nb, x0, xN));
    }
    else{
      hsumSysUp   = new Histo(TH1F(tag + "_SystSumUp",  tag + "_SystSumUp" ,   nb, vbins));
      hsumSysDown = new Histo(TH1F(tag + "_SystSumDown",tag + "_SystSumDown" , nb, vbins));
    }
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
Float_t Plot::GetData(){
  if(!hData) SetData();
  return hData->GetYield();
}

Histo* Plot::GetHData(){
  if(!hData) SetData();
  return hData;
}

Histo* Plot::GetHisto(TString pr, TString systag){
  if(pr == "Data") return GetHData();
  Float_t val = 0; Float_t sys = 0; TString ps; 
  Int_t nSyst = VSyst.size();
  Int_t nBkg = VBkgs.size();
  Int_t nSig = VSignals.size();
  if(systag == "0" || systag == ""){
    for(Int_t i = 0; i < nBkg; i++) if(pr == VBkgs.at(i)   ->GetProcess()) return  VBkgs.at(i);
    for(Int_t i = 0; i < nSig; i++) if(pr == VSignals.at(i)->GetProcess()) return  VSignals.at(i);
    return 0;
  }
  else{  
    for(int k = 0; k < nSyst; k++){ 
      ps = VSyst.at(k)->GetTag();
      if(!ps.BeginsWith(pr))   continue;
      if(!ps.Contains(systag)) continue;
      return VSyst.at(k);
    }
    for(Int_t k = 0; k < nSig; k++){ 
      ps = VSignals.at(k)->GetTag();
      if(!ps.BeginsWith(pr))   continue;
      if(!ps.Contains(systag)) continue;
      return VSignals.at(k);
    }
  }
  cout << "[Plot::GetHisto] WARNING: No systematic " << systag << " for process " << pr << "........... Returning nominal histo..." << endl;
  return GetHisto(pr, "0");
}

Histo* Plot::GetSymmetricHisto(TString pr, TString systag){
  Histo* nom = GetHisto(pr, "0");
  Histo* var = GetHisto(pr, systag);
  Histo* sym = (Histo*) var->CloneHisto();
  Int_t nbins = nom->GetNbinsX();
  Float_t bindiff = 0; Float_t cont;
  for(Int_t i = 0; i <= nbins+1; i++){
    cont    = nom->GetBinContent(i); 
    bindiff = var->GetBinContent(i)-nom->GetBinContent(i);
    sym->SetBinContent(i, cont - bindiff);
  } 
  sym->SetDirectory(0);
  sym->SetType(var->GetType());
  sym->SetColor(var->GetColor());
  TString newtag = systag;
  if(systag.Contains("Up")) newtag.ReplaceAll("Up", "Down");
  else newtag.ReplaceAll("Down", "Up");
  sym->SetTag(pr + "_" + newtag);
  sym->SetProcess(pr);
  sym->SetName(pr + "_" + newtag);
  return sym;
}

void Plot::AddSymmetricHisto(TString pr, TString systag){
  Histo* h = GetSymmetricHisto(pr, systag);
  AddToHistos(h);
}

Float_t Plot::GetYield(TString pr, TString systag){
  if(pr == "Data") return GetData();
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
    Float_t nom = GetYield(pr, "0");
    Float_t var = 0; Float_t diff = 0; Float_t tempvar = 0;
    for(int k = 0; k < nSyst; k++){ // Systematics in k
      ps = VSyst.at(k)->GetTag();
      if(!ps.BeginsWith(pr))   continue;
      if(!ps.Contains(systag)) continue;
      tempvar = VSyst.at(k)->GetYield();
      if( TMath::Abs(tempvar - nom) > diff){
        var = tempvar;
        diff = TMath::Abs(tempvar - nom);
      }
    }
    if(var != 0) return var;
  }
  cout << "[Plot::GetYield] WARNING: No systematic " << systag << " for process " << pr << "!!! ...... Returning nominal value... " << endl;
  return GetYield(pr);
}

TLegend* Plot::SetLegend(){ // To be executed before using the legend
  TLegend* leg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  Float_t MinYield = 0; 
  int nVBkgs = VBkgs.size();
  if(nVBkgs > 0 && hAllBkg) MinYield = hAllBkg->GetYield()/5000;

  if(doSignal){
    Int_t nSignals = VSignals.size();
    if(SignalStyle == "CrossSection" || SignalStyle == "xsec" || SignalStyle == "Bkg" || SignalStyle == "Fill"){
      for(Int_t i = 0; i < nSignals; i++){
        if(VSignals.at(i)->GetProcess() == SignalProcess){
          VSignals.at(i)->SetLineWidth(2);
          VSignals.at(i)->SetFillColor(VSignals.at(i)->GetColor());
          VSignals.at(i)->AddToLegend(leg, doYieldsInLeg);  
        }
      }
    }
    else if((SignalStyle == "SM" || SignalStyle == "H")){ 
      for(Int_t i = 0; i < nSignals; i++){
        if(VSignals.at(i)->GetProcess() == SignalProcess){
          VSignals.at(i)->SetLineWidth(2);
          VSignals.at(i)->SetFillColor(0);
          VSignals.at(i)->AddToLegend(leg, doYieldsInLeg);  
        }
      }
    }
  }
  for(int i = nVBkgs-1; i >= 0; i--){
    if(VBkgs.at(i)->GetYield() < MinYield) continue;
    else VBkgs.at(i)->AddToLegend(leg,doYieldsInLeg);
  }
  if(doSignal && (SignalStyle == "scan" || SignalStyle == "BSM" || SignalStyle == "") )
    for(int i = VSignals.size()-1; i >= 0; i--) VSignals[i]->AddToLegend(leg, doYieldsInLeg);

  if(doData && hData && VData.size() > 0){
    hData->SetTag("Data");
    hData->SetProcess("Data");
    hData->AddToLegend(leg,doYieldsInLeg);
  }
  return leg;
}

void Plot::SetLegendPosition(TString pos)
{
  if (pos=="UR")
    SetLegendPosition(0.70, 0.65, 0.93, 0.93);
  else if (pos == "UL")
    SetLegendPosition(0.15, 0.65, 0.38, 0.85);
  else if (pos == "DL")
    SetLegendPosition(0.15, 0.25, 0.38, 0.45);
  else{
    cout << "[Plot::SetLegendPosition] Sorry, position " << pos 
	 << " is not yet implemented. Fucking do it yourself" <<endl;
  }
  
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
  texcms = new TLatex(0.,0., CMSlabel);
  texcms->SetNDC();
  texcms->SetTextAlign(12);
  texcms->SetX(0.15);
  texcms->SetY(0.9);
  texcms->SetTextSize(0.052);
  texcms->SetTextSizePixels(23);
  return c;
}

void Plot::DrawComp(TString tag, bool sav, bool doNorm, TString lineStyle){
  TCanvas* c = SetCanvas();  plot->cd();
  int nsamples = VSignals.size();
  float themax = 0;
  float yield = 0;
  Histo* signal;
  if(nsamples == 0) return;
  else signal = VSignals.at(0);
  yield = VSignals.at(0)->Integral();
  if(doNorm) signal->Scale(1/yield);
  signal->SetDrawStyle("pe");
  signal->SetTitle("");
  signal->SetLineWidth(3);
  signal->SetMarkerStyle(24); signal->SetMarkerSize(1.8);
  signal->GetXaxis()->SetLabelSize(0.0);
  signal->Draw(signal->GetDrawStyle());
  float max = VSignals.at(0)->GetMaximum();

  for(Int_t  i = 1; i < nsamples; i++){
    yield = VSignals.at(i)->Integral();
    if(doNorm) VSignals.at(i)->Scale(1/yield);
    max = VSignals.at(i)->GetMaximum(); 
    if(max > themax) themax = max;
    VSignals.at(i)->Draw(lineStyle + ",same");
  }
  VSignals.at(0)->SetMaximum(themax*ScaleMax);
  if(!doSetLogy) PlotMinimum = PlotMinimum == -999? 0 : PlotMinimum;
  VSignals.at(0)->SetMinimum(PlotMinimum);
  if(doSetLogy){
    PlotMinimum = PlotMinimum == -999? 1e-2 : PlotMinimum;
    PlotMaximum = PlotMaximum == -999? themax*50 : PlotMaximum;
    signal->SetMaximum(PlotMaximum);
    signal->SetMinimum(PlotMinimum);
    plot->SetLogy();
  }
  TLegend* leg = SetLegend();
  leg->SetTextSize(0.041);
  leg->Draw("same");
  
  if(gof!="")
    {

      cout << "WARNING: at the moment, only the GoF between the first and the second plot is supported. If you plot more than two plots, the remaining ones will be ignored. Further functionality will be added later." << endl;
      
      double pvalue(-999.);
      TString theComp("");
      if(nsamples>1)
        {
          if(gof=="chi2")
            {
              pvalue=VSignals.at(0)->Chi2Test(VSignals.at(1), "WW CHI2/NDF");
              theComp="#frac{#chi^2}{NDOF}";
              cout << "WARNING: this is good for comparisons between Weighted-Weighted histograms, i.e. for comparisons between MonteCarlos. Todo: add switch for comparing data w/ MC or data w/ data." << endl;
            }
          else if(gof=="ks")
            {
              pvalue=VSignals.at(0)->KolmogorovTest(VSignals.at(1), "X");
              theComp="p-value (KS)";
              cout << "WARNING: this does not include comparison of normalization. Todo: add switch for that. Also, this runs pseudoexperiments, and will fail in case of negative bin contents. In case of negative weights, please rebin them to elimitate any negative bin content." << endl;
            }
          else if(gof=="ad")
            {
              pvalue=VSignals.at(0)->AndersonDarlingTest(VSignals.at(1));
              theComp="p-value (AD)";
              cout << "WARNING: the Anderson Darling test does not work for bins with negative content, at least in the ROOT implementation" << endl;
            }
          else
            {
              cout << "ERROR: this GoF test does not exist or is not currently implemented. What a cruel world." << endl;
            }
        }
      else
        cout << "ERROR: only one sample is selected. How can I compare it with another one?" << endl;
     
      TText *t = new TText(.7,.7,Form("%s: %f", theComp.Data(), pvalue));
      t->SetTextAlign(22);
      t->SetTextColor(kRed+2);
      t->SetTextFont(43);
      t->SetTextSize(40);
      t->SetTextAngle(45);
      t->Draw("same");
    }
  
  pratio->cd();
  vector<TH1F*> ratios;
  TH1F* htemp = NULL;
  hratio = (TH1F*)VSignals.at(0)->Clone("hratio_sig");
  SetHRatio();
  for(Int_t  i = 1; i < nsamples; i++){
    //htemp = (TH1F*)hratio->Clone("htemp");
    //htemp->Divide(VSignals.at(i)); 
    htemp = (TH1F*)VSignals.at(i)->Clone("htemp");
    htemp->Divide(hratio);
    SetHRatio(htemp);
    htemp->SetLineColor(VSignals.at(i)->GetColor());
    htemp->SetLineStyle(VSignals.at(i)->GetLineStyle());
    htemp->SetMarkerColor(VSignals.at(i)->GetColor());
    htemp->SetMarkerStyle(VSignals.at(i)->GetMarkerStyle());
    ratios.push_back(htemp);
    ratios.at(i-1)->Draw("pe,same");
  }
  if(sav){ // Save the histograms
    TString dir = plotFolder;
    TString plotname = (outputName == "")? varname + "_" + tag : outputName + "_" + varname;
    gSystem->mkdir(dir, kTRUE);
    c->Print( dir + plotname + ".png", "png");
    c->Print( dir + plotname + ".pdf", "pdf");
    delete c;
  }
  if(htemp) delete htemp;
  ratios.clear();
}

void Plot::DrawStack(TString tag = "0", bool sav = 0){
  std::vector<Histo*> VStackedSignals;

  if(verbose) cout << "[Plot::DrawStack] Setting Canvas..." << endl;
  TCanvas* c = SetCanvas(); plot->cd(); 
  GetStack();
  SetData();
  //if(!doData) hData = hAllBkg;

  Int_t nSignals = 0;
  Histo* hSignal = nullptr;
  if(doSignal){
    nSignals = VSignals.size();
    if(verbose) cout << "[Plot::DrawStack] Drawing " << nSignals << " signals..." << endl;
    for(Int_t i = 0; i < nSignals; i++) if(VSignals.at(i)->GetProcess() == SignalProcess) hSignal = VSignals.at(i);
    if(verbose) cout << " Signal process: " << SignalProcess << endl;
    if((SignalStyle == "SM" || SignalStyle == "H")){ // Only supports one signal
      hSignal->SetFillColor(0);
      hSignal->SetLineColor(hSignal->GetColor());
      hSignal->SetLineWidth(2);
      hStack->Add(hSignal);
    }
    else if(SignalStyle == "CrossSection" || SignalStyle == "xsec" || SignalStyle == "Bkg" || SignalStyle == "Fill"){
      hSignal->SetLineWidth(2);
      hSignal->SetLineColor(hSignal->GetColor());
      hSignal->SetFillColor(hSignal->GetColor());
      hStack->Add(hSignal);
    }   
  }

  float maxData = doData? hData->GetMax() : hAllBkg->GetMax();
  float maxMC = hAllBkg->GetMax();
  float Max = maxMC > maxData? maxMC : maxData;
  if(doSetLogy){
    if(verbose) cout << "[Plot::DrawStack] Setting log scale..." << endl;
    PlotMinimum = PlotMinimum == -999? 0.1*(1) : PlotMinimum;
    PlotMaximum = PlotMaximum == -999? Max*ScaleLog : PlotMaximum;
    hStack->SetMaximum(PlotMaximum);
    hStack->SetMinimum(PlotMinimum);
    plot->SetLogy();
  }
  else{
    PlotMinimum = PlotMinimum == -999? 0 : PlotMinimum;
    PlotMaximum = PlotMaximum == -999? Max*ScaleMax : PlotMaximum;
    hStack->SetMaximum(Max*ScaleMax);
      hStack->SetMinimum(PlotMinimum);
  }

  hStack->Draw("hist");
  hStack->GetYaxis()->SetTitle("Number of Events");
  hStack->GetYaxis()->SetTitleSize(0.06);
  hStack->GetYaxis()->SetTitleOffset(0.8);
  hStack->GetYaxis()->SetNdivisions(505);
  hStack->GetXaxis()->SetLabelSize(0.0);

  if(doSignal && (SignalStyle == "scan" || SignalStyle == "BSM" || SignalStyle == "") )
    for(Int_t  i = 0; i < nSignals; i++) VSignals.at(i)->Draw(SignalDrawStyle + "same");

  // Draw systematics histo
  hAllBkg->SetFillStyle(3145);
  hAllBkg->SetFillColor(kGray+2);
  hAllBkg->SetLineColor(kGray+2);
  hAllBkg->SetLineWidth(0);
  hAllBkg->SetMarkerSize(0);
  if(doSys)  hAllBkg->Draw("same,e2");
  if(doData) hData->Draw("pesame");
  Histo* hSignalerr = NULL; 
  VSignalsErr.clear();


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
  TLine *hline = nullptr;
  if(RatioOptions == "S/B"){
    if(!doSignal) cout << "[Plot::DrawStack] WARNING: cannot print ratio Signal/Background without signal!!" << endl;
    else{
      Float_t StoBmean = hSignal->GetYield()/hAllBkg->GetYield();
      hline = new TLine(0, StoBmean, 200, StoBmean); hline->SetLineColor(kOrange-2);
      cout << "StoBmean = " << StoBmean << endl;
      hratio = (TH1F*)hSignal->Clone("hratio");
      hratio->Divide(hAllBkg);
      Float_t rmax = hratio->GetMaximum()*1.15;
      Float_t rmin = hratio->GetMinimum()*0.85;
      SetRatioMin(rmin);
      SetRatioMax(rmax);
    }
  }
  else if(RatioOptions == "S/sqrtB")   {cout << "Option not implemented yet!!!! Sorry!!!!\n";}
  else if(RatioOptions == "S/sqrtSpB") {cout << "Option not implemented yet!!!! Sorry!!!!\n";}
  else{
    if(!doData) cout << "[Plot::DrawStack] WARNING: cannot print ratio Data/MC without data!!" << endl;
    else{
      hratio = (TH1F*)hData->Clone("hratio");
      hratio->Divide(hAllBkg);
    }
  }
  SetHRatio();
  hratio->Draw("same");

  if(RatioOptions == "S/B"){
    hline->Draw();
  }
  else{
    hratioerr->Draw("same,e2");
    hratio->Draw("same");
  }
  if(sav){ // Save the histograms
    TString dir = plotFolder;

    TString plotname = (outputName == "")? varname + "_" + tag : outputName + "_" + varname;
    
    gSystem->mkdir(dir, kTRUE);
    c->Print( dir + plotname + ".pdf", "pdf");
    c->Print( dir + plotname + ".png", "png");
    c->SaveAs( dir + plotname + ".root");
    delete c;
  }
  if(leg) delete leg; //if(hData) delete hData;
  //if(hAllBkg) delete hAllBkg; if(hStack) delete hStack; 
  if(hratioerr) delete hratioerr; if(hSignalerr) delete hSignalerr; if(hline) delete hline;
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
  for(Int_t i = 0; i < (Int_t) VSyst.size(); i++) if(VSyst.at(i)->GetTag().BeginsWith(pr+"_")){
    VSyst.at(i)->Scale(SF);
    VSyst.at(i)->SetStyle();
  }
}

void Plot::ScaleSignal(Float_t SF){
  ScaleProcess(SignalProcess, SF);
}

void Plot::ScaleSys(TString pr, Float_t SF){
  for(Int_t i = 0; i < (Int_t) VSyst.size(); i++) if(VSyst.at(i)->GetTag().BeginsWith(pr)){
    VSyst.at(i)->Scale(SF);
    VSyst.at(i)->SetStyle();
  }
}

//=======================================================================================
// Save all the histograms into a root file (also bin-to-bin statistical uncertainties) 
//=======================================================================================
void Plot::SaveHistograms(){
  TFile *f;

  TString filename =  varname;
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
    nom->Write();
  }
  SetData(); GetStack();
  hData->SetName("data_obs"); 
  hData->SetTag("data_obs");
  hData->Write();
  hStack->Write();  
  cout << "All histograms saved in " << limitFolder + filename + ".root\n";
}

//================================================================================
// Other estetics and style
//================================================================================

void Plot::SetGoF(TString thegof)
{
  if(thegof=="" || thegof=="chi2" || thegof=="ks" || thegof=="ad")
    gof = thegof;
  else
    {
      cout << "Warning: unknown goodness of fit test. Defaulting to chi2" << endl;
      gof="chi2";
    }
}

void Plot::SetTexChan(TString cuts){
  TString t = "";
  if (chan == "ElMu") t += "e^{#pm}#mu^{#mp}";
  else if (chan == "ElEl" || chan == "Elec") t += "e^{+}e^{-}";
  else if (chan == "MuMu" || chan == "Muon") t += "#mu^{+}#mu^{-}";
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

void Plot::SetHRatio(TH1F* h){
  if(h == nullptr) h = hratio;
  h->SetTitle("");
  if     (RatioOptions == "S/B"    )   h->GetYaxis()->SetTitle("S/B");
  else if(RatioOptions == "S/sqrtB")   h->GetYaxis()->SetTitle("S/#sqrt{B}");
  else if(RatioOptions == "S/sqrtSpB") h->GetYaxis()->SetTitle("S/#sqrt{S+B}");
  else                                 h->GetYaxis()->SetTitle("Data/MC");
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleOffset(0.25);
  h->GetYaxis()->SetTitleSize(0.1);
  h->GetYaxis()->SetLabelSize(0.1);
  h->GetYaxis()->SetNdivisions(402);
  h->GetXaxis()->SetTitleOffset(0.9);
  h->GetXaxis()->SetLabelSize(0.13);
  h->GetXaxis()->SetTitleSize(0.16);
  if (varname == "NBtagsNJets") {  //change bin labels
    h->GetXaxis()->SetBinLabel( 1, "(0, 0)");
    h->GetXaxis()->SetBinLabel( 2, "(1, 0)");
    h->GetXaxis()->SetBinLabel( 3, "(1, 1)");
    h->GetXaxis()->SetBinLabel( 4, "(2, 0)");
    h->GetXaxis()->SetBinLabel( 5, "(2, 1)");
    h->GetXaxis()->SetBinLabel( 6, "(2, 2)");
    h->GetXaxis()->SetBinLabel( 7, "(3, 0)");
    h->GetXaxis()->SetBinLabel( 8, "(3, 1)");
    h->GetXaxis()->SetBinLabel( 9, "(3, 2)");
    h->GetXaxis()->SetBinLabel(10, "(3, 3)");
    h->GetXaxis()->SetBinLabel(11, "(4, 0)");
    h->GetXaxis()->SetBinLabel(12, "(4, 1)");
    h->GetXaxis()->SetBinLabel(13, "(4, 2)");
    h->GetXaxis()->SetBinLabel(14, "(4, 3)");
    h->GetXaxis()->SetBinLabel(15, "(4, 4)");
    h->GetXaxis()->SetLabelSize(0.14);
    h->GetXaxis()->SetLabelOffset(0.02);
  }
  h->GetXaxis()->SetTitle(xtitle);
  h->SetMinimum(RatioMin);
  h->SetMaximum(RatioMax);
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
  p->doStatUncInDatacard = doStatUncInDatacard;

  p->SetSignalStyle(SignalStyle);
  p->SetSignalProcess(SignalProcess);

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
  for(Int_t j = 0; j < nSys; j++){
    sys2 += fabs((GetYield(pr,VSystLabel.at(j))-nom) * (GetYield(pr,VSystLabel.at(j))-nom));
  }
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

  TResultsTable t(nrows, ncolumns, 1); //cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetRowTitleHeader("Process");
  t.SetFormatNum(tableFormats);
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
  if(doData) t.SetRowTitle(nBkgs+1+nSignals, "Data");

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
//      t[i][k].SetStatError(np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()));
      TotalBkgSystematic += (np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()))*(np->GetTotalSystematic(np->VBkgs.at(i)->GetProcess()));
    }
    TotalBkgSystematic = TMath::Sqrt(TotalBkgSystematic);
    np->GetStack();
    t[nBkgs][k] = np->hAllBkg->GetYield();
    t[nBkgs][k].SetError(TotalBkgSystematic); 
//    t[nBkgs][k].SetStatError(TotalBkgSystematic); 
    for(Int_t i = nBkgs+1; i < nSignals+nBkgs+1; i++){
      t[i][k] = np->VSignals.at(i-(nBkgs+1))->GetYield();
      t[i][k].SetError(np->GetTotalSystematic(np->VSignals.at(i-(nBkgs+1))->GetProcess()));
//      t[i][k].SetStatError(np->GetTotalSystematic(np->VSignals.at(i-(nBkgs+1))->GetProcess()));
    }
    if(doData){
      np->SetData();
      t[nBkgs+1+nSignals][k] = np->hData->GetYield();
      t[nBkgs+1+nSignals][k].SetError(TMath::Sqrt(np->hData->GetYield()));
//      t[nBkgs+1+nSignals][k].SetStatError(TMath::Sqrt(np->hData->GetYield()));
    }
  }
  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  if(options.Contains("tex"))  t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".tex"));
  if(options.Contains("html")) t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".html"));
  if(options.Contains("txt"))  t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".txt"));
}



    // Draw systematics signal
    /* if(verbose) cout << "Drawing " << nSignals << " signals..." << endl;
       for(Int_t  i = 0; i < nSignals; i++){
       if(VSignals.at(i)->GetType() != itSignal) continue; // Consistency
       hSignalerr = (Histo*) VSignals.at(i)->Clone("hSignalErr"); // Add systematics to signal
       hSignalerr->SetFillColor(17); hSignalerr->SetMarkerStyle(0);
       hSignalerr->SetFillStyle(3013); VSignalsErr.push_back(hSignalerr);
       }
      //if(doSys) VSignalsErr.at(i)Stat->Draw("same,e2");*/
