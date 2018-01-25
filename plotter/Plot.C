#include "Plot.h"
#include "TSystem.h"
#include "TLine.h"
#include "TStyle.h"
#include "TLatex.h"
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

  ah->SetWeight(weight);
  ah->SetOptions(LoopOptions);
  Histo* h = ah->GetHisto(sample, sys);
  h->SetDirectory(0);
  h->doStackOverflow = doStackOverflow;
  h->SetStyle(); 
  h->Sumw2();
  delete ah;
  return h;
}

void Plot::AddSample(TString p, TString pr, Int_t type, Int_t color, TString sys, TString options){
  //>>> Multiprocess...
  p.ReplaceAll(" ", "");
  if(pr == "") pr = p;
  if(p.Contains(",")){
    TString First_p = p(0, p.First(','));
    TString theRest = p(p.First(',')+1, p.Sizeof());
    AddSample(First_p, pr, type, color, sys, options);
    AddSample(theRest, pr, type, color, sys, options);
    return;
  }
  
  //>>> Propagating options to looper
  SetLoopOptions(options);
  VTagOptions.push_back(options);
  if(sys != "0" && sys != "") AddToSystematicLabels(sys);
  if(type == itBkg || type == itSignal){
    VTagSamples.push_back(p);    // 
    VTagProcesses.push_back(pr);
  } 

  //>>> Using MultiLooper??
  if( (type == itBkg || type == itSignal) && (sys != "" && sys != "0")){
    VTagSamples.push_back(p);  
    VTagProcesses.push_back(pr);
    if(type == itSignal){
      nSignalSamples++;
      SetSignalProcess(pr);
    }
    Multiloop(p, pr, type, color, sys);
    return;
  }

  //>>> Getting histo from looper
  Histo* h = GetH(p, sys, type);
  if(type != itData && !LoopOptions.Contains("noScaleLumi"))  h->Scale(Lumi*1000);
  PrepareHisto(h, p, pr, type, color, sys);
} 

void Plot::GetStack(){ // Sets the histogram hStack
  if(hStack) delete hStack;
  hStack = new THStack(varname, "");
  Int_t nBkgs = VBkgs.size();
  for(Int_t i = 0; i < nBkgs; i++){
    hStack->Add((TH1F*) VBkgs.at(i));
  }
  if(hAllBkg) delete hAllBkg;
  hAllBkg = (new Histo(*(TH1F*) hStack->GetStack()->Last(), 3))->CloneHisto("AllBkg");
  hAllBkg->doStackOverflow = doStackOverflow;
  hAllBkg->SetStyle();
  hAllBkg->SetTag("Uncertainty");
  if(doSys && ((Int_t) VSystLabel.size() > 0)) GroupSystematics();
  if(verbose) cout << Form(" Adding %i systematic to sum of bkg...\n", (Int_t) VSumHistoSystUp.size());
  
  for(Int_t i = 0; i < (Int_t) VSumHistoSystUp.size(); i++){
    hAllBkg->AddToSystematics(VSumHistoSystUp.at(i));
    hAllBkg->AddToSystematics(VSumHistoSystDown.at(i));
  }
  if(doSys && ((Int_t) VSystLabel.size() > 0)) hAllBkg->SetBinsErrorFromSyst();
}

Histo* Plot::GetAllBkgClone(TString newname){
  if(!hStack) GetStack();
  Histo *h = hAllBkg->CloneHisto(newname);
  h->SetDirectory(0);
  return h;
}

void Plot::SetData(){  // Returns histogram for Data
  if(!doData){
    GetStack();
    if(hData) delete hData;
    if(gROOT->FindObject("HistoData")) delete gROOT->FindObject("HistoData");
    hData = hAllBkg;
  }
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
  hData->doStackOverflow = doStackOverflow;
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

void Plot::AddSystematic(TString var, TString pr){
  if(var == "stat"){
    AddToSystematicLabels("stat");
    AddStatError();
    if(verbose) cout << "[Plot::AddSystematic] Added statistical uncertainties! " << endl;
    return;
  }
  var.ReplaceAll(" ", "");
  if(var.Contains(",")){
    TString OneSyst;
    TString TheRest;
    OneSyst = TString(var(0, var.First(",")));
    TheRest = TString(var(var.First(",")+1, var.Sizeof()));
    AddSystematic(OneSyst, pr);
    AddSystematic(TheRest, pr);
    return;
  }
  if(pr == ""){
    for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++){
      if(VTagOptions.at(i).Contains("Fake") || VTagOptions.at(i).Contains("fake")) continue;
      AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Up", VTagOptions.at(i));
      AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Down", VTagOptions.at(i));
    }
  }
  else{
    std::vector<TString> vpr = TStringToVector(pr);
    TString p;
    for(Int_t k = 0; k < (Int_t) vpr.size(); k++){
      p = vpr.at(k);
      for(Int_t i = 0; i < (Int_t) VTagSamples.size(); i++){
        if(p != VTagProcesses.at(i)) continue;
        AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Up", VTagOptions.at(i));
        AddSample(VTagSamples.at(i), VTagProcesses.at(i), itSys, 1, var+"Down", VTagOptions.at(i));
      }
    }
  }
  if(verbose) cout << "[Plot::AddSystematic] Systematic histograms added to the list for variation: " << var << endl;
}

void Plot::AddStatError(TString process){
  TString pr;
  if(process == ""){
    for(Int_t i = 0; i < (Int_t) VTagProcesses.size(); i++){
      pr = VTagProcesses.at(i);
      if(i != 0) if(pr == VTagProcesses.at(i-1)) continue;
      AddStatError(pr);
    }
    return;
  }
  //cout << " --> process = " << process << endl;
  Float_t nom; Float_t stat;
  TString nameUp = process + "_statUp"; TString nameDown = process + "_statDown";
  if(gROOT->FindObject(nameUp)) delete gROOT->FindObject(nameUp);
  if(gROOT->FindObject(nameDown)) delete gROOT->FindObject(nameDown);
  Histo *hUp   = (Histo*)GetHisto(process)->CloneHisto(nameUp);
  Histo* hDown = (Histo*)GetHisto(process)->CloneHisto(nameDown);
  hUp->SetDirectory(0); hDown->SetDirectory(0);
  for(Int_t iBin = 0; iBin <= hUp->GetNbinsX(); iBin++){
    nom = hUp->GetBinContent(iBin); stat = hUp->GetBinError(iBin);
    hUp  ->SetBinContent(iBin, nom+stat);
    hDown->SetBinContent(iBin, nom-stat);
  }
  hUp->SetProcess(process); hDown->SetProcess(process);
  hUp->SetSysTag("statUp"); hDown->SetSysTag("statDown");
  hUp->SetTag(process + "_statUp"); hDown->SetTag(process + "_statDown");
  hUp->SetType(itSys); hDown->SetType(itSys);
  hUp->doStackOverflow = doStackOverflow;
  hDown->doStackOverflow = doStackOverflow;
  hUp->SetStyle(); hDown->SetStyle();
  AddToHistos(hUp); AddToHistos(hDown);
}

void Plot::GroupSystematics(){
  TString var = ""; TString pr = "";
  //>>> Here we store the histos
  VSumHistoSystUp.clear(); VSumHistoSystDown.clear();
  Histo* hsumSysUp = NULL; Histo* hsumSysDown = NULL; 

  Int_t nBkgs = VBkgs.size();

  //>>> Loop over each systematic
  for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++){
    var = VSystLabel.at(i);
    hsumSysUp = NULL; hsumSysDown = NULL; 
    pr = VBkgs.at(0)->GetProcess();
    hsumSysUp   = GetHisto(pr, var+"Up"  )->CloneHisto(pr + "_" + var + "Up");
    hsumSysDown = GetHisto(pr, var+"Down")->CloneHisto(pr + "_" + var + "Down");
    for(Int_t i = 1; i < nBkgs; i++){
      pr = VBkgs.at(i)->GetProcess();
      hsumSysUp  ->Add(GetHisto(pr, var+"Up"  ));
      hsumSysDown->Add(GetHisto(pr, var+"Down"));
    }
    hsumSysUp  ->SetName(var  + "_SystSumUp"   );
    hsumSysDown->SetName(var + "_SystSumDownn");
    AddSumHistoSystematicUp(hsumSysUp);
    AddSumHistoSystematicDown(hsumSysDown);
  }
}


void Plot::Multiloop(TString p, TString pr, Int_t type, Int_t color, TString sys){
  TString pathToMiniTree = path;
  TString sample = p;
  if     (type == itSignal || sample.Contains("T2tt")) pathToMiniTree = pathSignal;
  else if(type == itData  ) pathToMiniTree = pathData;
  else if(sample.Contains("/")){
    pathToMiniTree = sample(0, sample.Last('/')+1);
    sample = sample(sample.Last('/')+1, sample.Sizeof());
  }
  Multilooper* ah;
  if(xN != x0) ah = new Multilooper(pathToMiniTree, treeName, var, cut, chan, nb, x0, xN);
  else         ah = new Multilooper(pathToMiniTree, treeName, var, cut, chan, nb, vbins);

  ah->SetWeight(weight);
  ah->SetOptions(LoopOptions);
  ah->SetSampleName(sample);
  ah->SetSystematics(sys);
  ah->Fill();
 
  vector<Histo*> vH = ah->GetAllHistos();
  if(type != itData && !LoopOptions.Contains("noScaleLumi"))
  for(Int_t i = 0; i < (Int_t) vH.size(); i++) vH.at(i)->Scale(Lumi*1000);
  PrepareHisto(vH, sample, pr, type, color);
}


void Plot::PrepareHisto(vector<Histo*> vH, TString sampleName, TString pr, Int_t type, Int_t color, TString sys){
  Int_t n = vH.size(); Int_t i = 0;
  PrepareHisto(vH.at(0), sampleName, pr, type, color, sys);
  for(i = 1; i < n; i++) PrepareHisto(vH.at(i), sampleName, pr, itSys, color, sys);
}

void Plot::PrepareHisto(Histo* h, TString sampleName, TString pr, Int_t type, Int_t color, TString sys){
  TString s = (sys == "" || sys == "0") ? h->GetSysTag() : sys;
  TString p = pr;
  TString name = sampleName;
  if(s != "0" && s != "")  p    = pr   + "_" + s;
  if(s != "0" && s != "")  name = name + "_" + s;
  h->SetSysTag(s);
  h->SetProcess(pr); h->SetTag(p); h->SetName(name);
  h->SetType(type); h->SetColor(color);
  h->doStackOverflow = doStackOverflow;
  h->SetStyle();
  h->SetDirectory(0); 
  Group(h);
}

void Plot::Group(Histo* h){
  TString t = h->GetTag();
  TString tag; Int_t type = h->GetType();
  Int_t n; Int_t i;
  if(type == itBkg){
    n = VBkgs.size(); 
    for(i = 0; i < n; i++){
      tag = VBkgs.at(i)->GetTag();
      if(t == tag){
        VBkgs.at(i)->Add((TH1F*) h);
        VBkgs.at(i)->ReCalcValues();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
        return;
      }
    }
  }
  else if(type == itSignal){
    n = VSignals.size();
     for(i = 0; i < n; i++){
      tag = VSignals.at(i)->GetTag();
      if(t == tag){
        VSignals.at(i)->Add((TH1F*) h);
        VSignals.at(i)->ReCalcValues();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
        return;
      }
    }
  }
  else if(type == itSys){
    n = VSyst.size();
    for(i = 0; i < n; i++){
      tag = VSyst.at(i)->GetTag();
      if(t == tag){
        VSyst.at(i)->Add((TH1F*) h);
        VSyst.at(i)->ReCalcValues();
        if(verbose) cout << "[Plot::AddToHistos] Added histogram " << h->GetName() << " to " << h->GetTag() << " group (" << type << ")" << endl;
        return;
      }
    }
    h->SetName(t);
  }
  else if(type == itData){
    VTagDataSamples.push_back(h->GetName());
    h->SetLineColor(kBlack); 
    h->SetMarkerStyle(20); 
    h->SetMarkerSize(1.1); 
  }
  AddToHistos(h);
}


//================================================================================
// Drawing
//================================================================================
// SetLegend()
// TCanvas* SetCanvas()
// void DrawStack(TString tag = "0")
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
      if(!ps.EndsWith(systag)) continue;
      return VSyst.at(k);
    }
    for(Int_t k = 0; k < nSig; k++){ 
      ps = VSignals.at(k)->GetTag();
      if(!ps.BeginsWith(pr))   continue;
      if(!ps.EndsWith(systag)) continue;
      return VSignals.at(k);
    }
  }
  //cout << "[Plot::GetHisto] WARNING: No systematic " << systag << " for process " << pr << "........... Returning nominal histo..." << endl;
  return GetHisto(pr, "0");
}

Int_t Plot::GetProcessType(TString pr, TString systag){
  if(systag != "0" && systag != "") return itSys;
  Int_t nBkg = VBkgs.size();
  Int_t nSig = VSignals.size();
  for(Int_t i = 0; i < nBkg; i++) if(pr == VBkgs.at(i)   ->GetProcess()) return  itBkg;
  for(Int_t i = 0; i < nSig; i++) if(pr == VSignals.at(i)->GetProcess()) return  itSignal;
  if(verbose) cout << "[Plot::GetProcessType]: Not found process " << pr << endl;
  return -1;
}

Int_t Plot::GetProcessIndex(TString pr, TString systag){
  Int_t type = GetProcessType(pr, systag);
  Int_t nBkg = VBkgs.size();
  Int_t nSig = VSignals.size();
  Int_t nSyst = VSyst.size();
  if     (type == itBkg){    for(Int_t i = 0; i < nBkg; i++) if(pr == VBkgs.at(i)   ->GetProcess()) return  i;}
  else if(type == itSignal){ for(Int_t i = 0; i < nSig; i++) if(pr == VSignals.at(i)->GetProcess()) return i;}
  else if(type == itBkg){
    TString ps = "";
    for(int k = 0; k < nSyst; k++){
      ps = VSyst.at(k)->GetTag();
      if(ps.BeginsWith(pr) && ps.EndsWith(systag)) return k;
    }
  }
  return -1;
}

void Plot::RemoveProcess(TString pr, TString systag){
  Int_t type = GetProcessType(pr, systag);
  Int_t pos  = GetProcessIndex(pr, systag);
  if     (type == itBkg)    VBkgs.erase(   VBkgs.begin() + itBkg   ); 
  else if(type == itSignal) VSignals.erase(VBkgs.begin() + itSignal); 
  else if(type == itSys)    VSyst.erase(   VBkgs.begin() + itSys   ); 
  else{
    if(verbose) cout << "[Plot::RemoveProcess]: Not found process " << pr << ", with syst " << systag << endl;
    return;
  }
  if(systag == "" || systag == "0"){
    Int_t nS = VSyst.size(); Int_t iS;
    TString ps;
    for(iS = 0; iS < nS; iS++){
      ps = VSyst.at(iS)->GetProcess();
      if(pr == ps){
        VSyst.erase(VSyst.begin() + iS);
        iS--;
      }
    }
  }
}

Bool_t Plot::Exists(TString pr, TString systag){
  if(GetProcessIndex(pr, systag) == -1) return false;
  else return true;
}

Bool_t Plot::RenameProcess(TString pr, TString newname, TString systag){
  if(Exists(newname)) return false;
  TString t = pr;
  Histo *h = GetHisto(pr, systag);
  h->SetProcess(newname);
  if(systag != "" && systag != "0") t += "_" + systag; 
  h->SetTag(t);

  // Propagate to all systematics
  Int_t nSyst = VSystLabel.size();
  TString s;
  if(systag == "" || systag == "0"){
    for(Int_t iS = 0; iS < nSyst; iS++){
      s = VSystLabel.at(iS);
      if(Exists(pr, s))          RenameProcess(pr, newname, s       );
      if(Exists(pr, s + "Up"  )) RenameProcess(pr, newname, s+"Up"  );
      if(Exists(pr, s + "Down")) RenameProcess(pr, newname, s+"Down");
    }
    return true;
  }
  return true;
}

void Plot::GroupProcesses(TString pr, TString newProcess){
  if(pr.Contains(",")){
    vector<TString> vpr = TStringToVector(pr);
    for(Int_t i = 0; i < (Int_t) vpr.size(); i++) GroupProcesses(vpr.at(i), newProcess);
    return;
  }
  if(!Exists(pr)){
    cout << "[Plot::GroupProcesses] ERROR: process \"" << pr << "\" does not exist!" << endl;
    return;
  }
  if(!Exists(newProcess)){
    cout << "No existe el proceso nuevo: " << newProcess << endl;
    RenameProcess(pr, newProcess); 
    return;
  }
  cout << "Existe el proceso nuevo " << newProcess << endl;
  GetHisto(newProcess)->Add(GetHisto(pr));
  Int_t nSyst = VSyst.size();
  TString ipr; TString it; Int_t index;
  for(Int_t iS = 0; iS < nSyst; iS++){
    ipr = VSyst.at(iS)->GetProcess(); it = VSyst.at(iS)->GetSysTag();
    index = GetProcessIndex(ipr, it);
    if(index != -1) VSyst.at(iS)->Add(GetHisto(ipr, it));
  }
  // TODO: Add properly norm uncertainties!!
  RemoveProcess(pr);
}



Histo* Plot::GetSymmetricHisto(TString pr, TString systag){
  Histo* nom = GetHisto(pr, "0");
  Histo* var = GetHisto(pr, systag);
  Histo* sym = (Histo*) var->CloneHisto("newHisto");
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
  sym->SetTag(pr + "_" + newtag); sym->SetSysTag(newtag);
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
  if(systag == "0" || systag == ""){
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
  //cout << "[Plot::GetYield] WARNING: No systematic " << systag << " for process " << pr << "!!! ...... Returning nominal value... " << endl;
  return GetYield(pr);
}

TLegend* Plot::SetLegend(){ // To be executed before using the legend
  TLegend* leg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  leg->SetTextSize(LegendTextSize);
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

  if(doData && hData && VData.size() > 0){
    hData->SetTag("Data");
    hData->SetProcess("Data");
    hData->AddToLegend(leg,doYieldsInLeg);
  }

  for(int i = nVBkgs-1; i >= 0; i--){
    if(VBkgs.at(i)->GetYield() < MinYield) continue;
    else VBkgs.at(i)->AddToLegend(leg,doYieldsInLeg);
  }

  if(doSys && doUncInLegend && ((Int_t) VSystLabel.size() > 0)) hAllBkg->AddToLegend(leg,doYieldsInLeg); // add legend for uncertainty

  if(doSignal && (SignalStyle == "scan" || SignalStyle == "BSM" || SignalStyle == "") )
    for(int i = VSignals.size()-1; i >= 0; i--) VSignals[i]->AddToLegend(leg, doYieldsInLeg);
  
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
  texlumi->SetX(0.73);
  texlumi->SetY(0.97);
  texlumi->SetTextFont(42);
  texlumi->SetTextSize(0.05);
  texlumi->SetTextSizePixels(22);
  
  texcms = new TLatex(0.,0., CMSlabel);
  texcms->SetNDC();
  texcms->SetTextAlign(12);
  texcms->SetX(0.15);
  texcms->SetY(0.89);
  //texcms->SetTextFont(42);
  texcms->SetTextSize(0.06);
  texcms->SetTextSizePixels(22);

  texPrelim  = new TLatex(0.,0., CMSmodeLabel); 
  texPrelim->SetNDC();
  texPrelim->SetTextAlign(12);
  texPrelim->SetX(0.15);
  texPrelim->SetY(0.83);
  texPrelim->SetTextFont(52);
  texPrelim->SetTextSize(0.052);
  texPrelim->SetTextSizePixels(22);
 

  return c;
}

void Plot::DrawComp(TString tag, bool doNorm, TString options){
  doUncInLegend = false;
  TString drawstyle = "pe";
  if(options.Contains("hist")) drawstyle = "hist";
  if(options.Contains("style=l")) drawstyle = "l";
  TCanvas* c = SetCanvas();  plot->cd();
  int nsamples = VSignals.size();
  float themax = 0;
  float yield = 0;


  //>>> Set Signal
  Histo* signal;
  if(nsamples == 0) return;
  else signal = VSignals.at(0);
  yield = VSignals.at(0)->Integral();
  if(doNorm) signal->Scale(1/yield);
  TString sD = signal->GetDrawStyle();
  if(sD == "") sD = drawstyle;
  signal->SetDrawStyle(sD);
  signal->SetTitle("");
  signal->SetLineWidth(3);
  signal->SetMarkerStyle(24); signal->SetMarkerSize(1.8);
  //SetYaxis(signal->GetYaxis());
  signal->GetXaxis()->SetLabelSize(0.0);
  signal->Draw(signal->GetDrawStyle());

  //>>> Set maximum and drawing all samples
  Float_t max = VSignals.at(0)->GetMaximum();
  for(Int_t  i = 1; i < nsamples; i++){
    yield = VSignals.at(i)->Integral();
    if(doNorm) VSignals.at(i)->Scale(1/yield);
    max = VSignals.at(i)->GetMaximum(); 
    if(max > themax) themax = max;
    VSignals.at(i)->Draw(drawstyle + "same");
  }
  signal->SetMaximum(themax*ScaleMax);
  if(!doSetLogy) PlotMinimum = PlotMinimum == -999? 0 : PlotMinimum;
  signal->SetMinimum(PlotMinimum);
  if(doSetLogy){
    if(PlotMinimum == 0 || PlotMinimum == -999)  PlotMinimum = 1e-2;
    PlotMaximum = PlotMaximum == -999? themax*50 : PlotMaximum;
    signal->SetMaximum(PlotMaximum);
    signal->SetMinimum(PlotMinimum);
    plot->SetLogy();
  }

  //>>> Setting legend and labels...
  TLegend* leg = SetLegend();
  if(doLegend) leg->Draw("same");
//  if (chlabel != ""){
//    SetTexChan();
//    texchan->Draw("same");
//  }
  
  //>>> Chi square
  if(gof!=""){
      cout << "WARNING: at the moment, only the GoF between the first and the second plot is supported. If you plot more than two plots, the remaining ones will be ignored. Further functionality will be added later." << endl;
      double pvalue(-999.);
      TString theComp("");
      if(nsamples>1){
          if(gof=="chi2"){
              pvalue=VSignals.at(0)->Chi2Test(VSignals.at(1), "WW CHI2/NDF");
              theComp="#frac{#chi^2}{NDOF}";
              cout << "WARNING: this is good for comparisons between Weighted-Weighted histograms, i.e. for comparisons between MonteCarlos. Todo: add switch for comparing data w/ MC or data w/ data." << endl;
            }
          else if(gof=="ks"){
              pvalue=VSignals.at(0)->KolmogorovTest(VSignals.at(1), "X");
              theComp="p-value (KS)";
              cout << "WARNING: this does not include comparison of normalization. Todo: add switch for that. Also, this runs pseudoexperiments, and will fail in case of negative bin contents. In case of negative weights, please rebin them to elimitate any negative bin content." << endl;
            }
          else if(gof=="ad"){
              pvalue=VSignals.at(0)->AndersonDarlingTest(VSignals.at(1));
              theComp="p-value (AD)";
              cout << "WARNING: the Anderson Darling test does not work for bins with negative content, at least in the ROOT implementation" << endl;
            }
          else  cout << "ERROR: this GoF test does not exist or is not currently implemented. What a cruel world." << endl;
        }
      else cout << "ERROR: only one sample is selected. How can I compare it with another one?" << endl;
     
      TText *t = new TText(.7,.7,Form("%s: %f", theComp.Data(), pvalue));
      t->SetTextAlign(22);
      t->SetTextColor(kRed+2);
      t->SetTextFont(43);
      t->SetTextSize(40);
      t->SetTextAngle(45);
      t->Draw("same");
  }

  //>>> Setting ratio histogram
  pratio->cd();
  vector<TH1F*> ratios;
  TH1F* htemp = NULL;
  hratio = (TH1F*)VSignals.at(0)->Clone("hratio_sig");
  SetHRatio();
  if(options.Contains("ratiocolor")){
    for(Int_t  i = 1; i < nsamples; i++){
      if(VSignals.at(i)->GetColor() == VSignals.at(i-1)->GetColor()){
        hratio = (TH1F*)VSignals.at(i-1)->Clone("hratio"); 
        htemp  = (TH1F*)VSignals.at(i  )->Clone("htemp");
        htemp->Divide(hratio);
        SetHRatio(htemp);
        htemp->SetLineColor(VSignals.at(i)->GetColor());
        htemp->SetLineStyle(VSignals.at(i)->GetLineStyle());
        htemp->SetMarkerColor(VSignals.at(i)->GetColor());
        htemp->SetMarkerStyle(VSignals.at(i)->GetMarkerStyle());
        //htemp->SetDrawStyle(VSignals.at(i)->GetDrawStyle());
        ratios.push_back(htemp);
        i++;
      }        
    }
    for(Int_t k = 0; k < (Int_t) ratios.size(); k++){
      //sD = ratios.at(k)->GetDrawStyle();
      sD = drawstyle;
      ratios.at(k)->Draw(sD + "same");
    }
  }
  else{
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
      //sD = ratios.at(i-1)->GetDrawStyle();
      sD = drawstyle;
      ratios.at(i-1)->Draw(sD + "same");
    }
  }

  //>>> Saving the plot...
  TString dir = plotFolder;
  TString plotname = (outputName == "")? varname : outputName;
  if(outputName != "" && varname != "")  plotname += "_" + varname;
  if(plotname != "" && tag != "")      plotname += "_" + tag;
  if(plotname == "" && tag != "")      plotname = tag;

  gSystem->mkdir(dir, kTRUE);
  plotname.ReplaceAll(" ","");
  plotname.ReplaceAll("/","_");
  c->Print( dir + plotname + ".png", "png");
  c->Print( dir + plotname + ".pdf", "pdf");
  c->Print( dir + plotname + ".eps", "eps");
  delete c;
  if(htemp) delete htemp;
  ratios.clear();
}


void Plot::DrawStack(TString tag){
  std::vector<Histo*> VStackedSignals;
  if(verbose) cout << "[Plot::DrawStack] Setting Canvas..." << endl;
  TCanvas* c = SetCanvas(); plot->cd(); 
  GetStack();
  SetData();

  //--------- Plotting options for the signal
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
/*  TH1F* hSigDraw;
  if(doSignal){
    hSigDraw = (TH1F*) hSignal->Clone("hSigDraw");
    hSigDraw->Add((TH1F*) hAllBkg->Clone("hAllBkg_clone"));
    hSigDraw->Draw("hist"); 
  }*/
  hStack->Draw("hist");

  //--------- Adjust max and min for the plot
  float maxData = doData? hData->GetMax() : hAllBkg->GetMax();
  float maxMC = hAllBkg->GetMax();
  float Max = maxMC > maxData? maxMC : maxData;
  if(doSetLogy){
    if(verbose) cout << "[Plot::DrawStack] Setting log scale..." << endl;
    if(PlotMinimum == 0 || PlotMinimum == -999)  PlotMinimum = 0.1;
    PlotMaximum = PlotMaximum == -999? Max*ScaleLog : PlotMaximum;
    hStack->SetMaximum(PlotMaximum);
    hStack->SetMinimum(PlotMinimum);
    plot->SetLogy();
  }
  else{
    PlotMinimum = PlotMinimum == -999? 0 : PlotMinimum;
    PlotMaximum = PlotMaximum == -999? Max*ScaleMax : PlotMaximum;
    
    hStack->SetMaximum(PlotMaximum);
    hStack->SetMinimum(PlotMinimum);
  }  

  SetYaxis(hStack->GetYaxis());
  hStack->GetXaxis()->SetLabelSize(0.0);

  //--------- Draw signal
  if(doSignal && (SignalStyle == "scan" || SignalStyle == "BSM" || SignalStyle == "") )
    for(Int_t  i = 0; i < nSignals; i++) VSignals.at(i)->Draw(SignalDrawStyle + "same");

  //---------  Draw systematic errors 
  //if(doSignal && (SignalStyle == "scan" || SignalStyle == "BSM" || SignalStyle == "") )
  hAllBkg->SetFillStyle(3444); // 3444 o 3004 (3145 default here)
  hAllBkg->SetFillColor(StackErrorColor); // kGray+2 as default
  hAllBkg->SetLineColor(StackErrorColor);
  hAllBkg->SetLineWidth(0);
  hAllBkg->SetMarkerSize(0);
  if(doSys && ((Int_t) VSystLabel.size() > 0))  hAllBkg->Draw("same,e2");

  //--------- Draw Data
  if(doData && RatioYtitle != "S/B") hData->Draw("psameE1X0");

  //--------- Draw systematics ratio
  hAllBkg->SetFillStyle(StackErrorStyle);
  TH1F* hratioerr =  (TH1F*) hAllBkg->Clone("hratioerr");
  Int_t nbins = hAllBkg->GetNbinsX(); Float_t binval = 0; Float_t errbin = 0; Float_t totalerror = 0;
  for(int bin = 1; bin <= nbins; bin++){  // Set bin error
    totalerror = hAllBkg->GetBinError(bin); 
    binval = hAllBkg->GetBinContent(bin);
    errbin = binval > 0 ? totalerror/binval : 0.0;
    hratioerr->SetBinContent(bin, 1);
    hratioerr->SetBinError(bin, errbin);
  }
  hratioerr->SetFillColor(RatioErrorColor); 
  hratioerr->SetFillStyle(RatioErrorStyle); 
  hratioerr->SetMarkerSize(0);

  //--------- Set legend and other texts
  TLegend* leg = SetLegend();
  if(doLegend) leg->Draw("same");      
  texcms->Draw("same");     // CMS 
  texlumi->Draw("same");    // The luminosity
  texPrelim->Draw("same");  // Preliminary
  if (chlabel != ""){
    SetTexChan();
    texchan->Draw("same");
  }

  //---------- Set ratio... with Data, S/B, etc
  pratio->cd();
  TLine *hline = nullptr;
  if(RatioYtitle == "S/B"){
    if(!doSignal) cout << "[Plot::DrawStack] WARNING: cannot print ratio Signal/Background without signal!!" << endl;
    else{
      Float_t StoBmean = hSignal->GetYield()/hAllBkg->GetYield();
      hline = new TLine(0, StoBmean, 200, StoBmean); hline->SetLineColor(kOrange-2);
      hratio = (TH1F*)hSignal->Clone("hratio");
      hratio->Divide(hAllBkg);
      Float_t rmax = hratio->GetMaximum()*1.15;
      Float_t rmin = hratio->GetMinimum()*0.85;
      SetRatioMin(rmin);
      SetRatioMax(rmax);
      for(int bin = 1; bin <= nbins; bin++){  // Set bin error
        totalerror = hAllBkg->GetBinError(bin); 
        binval = hAllBkg->GetBinContent(bin);
        errbin = binval > 0 ? totalerror/binval : 0.0;
        float signalVal = hSignal->GetBinContent(bin);
        errbin = errbin*(signalVal/binval);
        hratioerr->SetBinContent(bin, StoBmean);
        hratioerr->SetBinError(bin, errbin);
      }
    }
  }
  else if(RatioYtitle == "S/sqrtB")   {cout << "Option not implemented yet!!!! Sorry!!!! [DO IT YOURSELF!]\n";}
  else if(RatioYtitle == "S/sqrtSpB") {cout << "Option not implemented yet!!!! Sorry!!!! [DO IT YOURSELF!]\n";}
  else{ // ratio Data/MC
    //if(!doData) cout << "[Plot::DrawStack] WARNING: cannot print ratio Data/MC without data!!" << endl;
    //else{
      if(doData) hratio = (TH1F*)hData->Clone("hratio");
      else       hratio = (TH1F*)hAllBkg->Clone("hratio");
      // ratio by hand so systematic (background) errors don't get summed up to statistical ones (data)
      for (int bin = 0; bin < hratio->GetNbinsX(); ++bin){
        if (hratio->GetBinContent(bin+1) > 0){
          hratio->SetBinContent( bin+1, hratio->GetBinContent(bin+1) / hAllBkg->GetBinContent(bin+1));
          hratio->SetBinError  ( bin+1, hratio->GetBinError  (bin+1) / hAllBkg->GetBinContent(bin+1));
        }
        else{ hratio->SetBinError  ( bin+1, 0.); }
      }
    //}
  }
  SetHRatio();
  //if(!RatioYtitle.Contains("S")) hratio->SetLineWidth(0); 
  hratio->Draw("same");

  if(RatioYtitle == "S/B") hline->Draw();
  hratioerr->Draw("same,e2"); //hratio->Draw("same");

  //-------- Saving options
    TString dir = plotFolder;
    TString plotname = (outputName == "")? varname : outputName;
    if(outputName != "" && varname != "")  plotname += "_" + varname;
    if(outputName != "" && tag != "")      plotname += "_" + tag;
    else outputName = tag;
    
    gSystem->mkdir(dir, kTRUE);
    plotname.ReplaceAll(" ","");
    plotname.ReplaceAll("/","_");
    c->Print( dir + plotname + ".pdf", "pdf");
    c->Print( dir + plotname + ".png", "png");
    c->Print( dir + plotname + ".eps", "eps");
    c->SaveAs( dir + plotname + ".root");
    delete c;
  if(leg) delete leg; if(hratioerr) delete hratioerr; if(hline) delete hline;
  VStackedSignals.clear();
}

void Plot::ScaleProcess(TString pr, Float_t SF){
  for(Int_t i = 0; i < (Int_t) VBkgs.size(); i++) if(VBkgs.at(i)->GetProcess() == (pr)){
    VBkgs.at(i)->Scale(SF);
    VBkgs.at(i)->ReCalcValues();
  }
  for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) if(VSignals.at(i)->GetProcess() == (pr)){
    VSignals.at(i)->Scale(SF);
    VSignals.at(i)->ReCalcValues();
  }
  for(Int_t i = 0; i < (Int_t) VSyst.size(); i++) if(VSyst.at(i)->GetTag().BeginsWith(pr+"_")){
    VSyst.at(i)->Scale(SF);
    VSyst.at(i)->ReCalcValues();
  }
}

void Plot::ScaleSignal(Float_t SF){
  ScaleProcess(SignalProcess, SF);
}

void Plot::ScaleSys(TString pr, TString sys, Float_t SF){
  GetHisto(pr, sys)->Scale(SF);
  GetHisto(pr,sys)->ReCalcValues();
}

//=======================================================================================
// Save all the histograms into a root file (also bin-to-bin statistical uncertainties) 
//=======================================================================================
void Plot::SaveHistograms(){

  TString filename =  varname;
  if(outputName != "") {
    if(varname != "") filename = outputName + "_" + varname;
    else filename = outputName;
  }
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

void Plot::SetTexChan(){
  texchan = new TLatex(-20, 50, chlabel);
  texchan->SetNDC();
  texchan->SetTextAlign(12);
  texchan->SetX(chX); 
  texchan->SetY(chY);
  texchan->SetTextFont(42);
  texchan->SetTextSize(0.05);
  texchan->SetTextSizePixels(chSize);
}

void Plot::SetHRatio(TH1F* h){
  if(h == nullptr) h = hratio;
  h->SetTitle("");
  if     (RatioYtitle == "S/B"    )   h->GetYaxis()->SetTitle("S/B");
  else if(RatioYtitle == "S/sqrtB")   h->GetYaxis()->SetTitle("S/#sqrt{B}");
  else if(RatioYtitle == "S/sqrtSpB") h->GetYaxis()->SetTitle("S/#sqrt{S+B}");
  else                                 h->GetYaxis()->SetTitle(RatioYtitle);

  //h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleSize(0.16);
  h->GetXaxis()->SetLabelOffset(0.02);
  h->GetXaxis()->SetLabelSize(0.16);

  h->GetYaxis()->SetTitleOffset(0.27);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleSize(0.13);
  h->GetYaxis()->SetLabelSize(0.13);
  h->GetYaxis()->SetNdivisions(402);

  int iBin = 1;
  for (auto& label : VBinLabels){
    h->GetXaxis()->SetBinLabel( iBin, label );
    iBin++;
  }

  h->GetXaxis()->SetTitle(xtitle);
  h->SetMinimum(RatioMin);
  h->SetMaximum(RatioMax);
}

void Plot::SetYaxis(TAxis *a){
  a->SetTitle(ytitle);
  a->SetTitleSize(ytitleSize);
  a->SetTitleOffset(ytitleOffset);
  a->SetNdivisions(ytitleDivisions);
  a->SetLabelSize(ytitleLabelSize);
  a->SetNoExponent(kFALSE);
}

Int_t Plot::GetColorOfProcess(TString pr){
  for(Int_t i = 0; i < (Int_t) VBkgs.size(); i++) if(VBkgs.at(i)->GetProcess() == (pr)) return VBkgs.at(i)->GetFillColor();
  for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) if(VSignals.at(i)->GetProcess() == (pr)) return VSignals.at(i)->GetLineColor();
  return 0;
}

void Plot::RemoveSystematic(TString sys){
  if(sys.Contains(",")){
    std::vector<TString> v = TStringToVector(sys);
    Int_t n = v.size();
    for(Int_t i = 0; i < n; i++) RemoveSystematic(v.at(i));
    return;
  }
  TString tag = "";
  for(Int_t k = 0; k < (Int_t) VSyst.size(); k++){ 
    if(VSyst.at(k)->GetTag() == sys || VSyst.at(k)->GetTag() == sys+"Up" || VSyst.at(k)->GetTag() == sys+"Down") VSyst.erase(VSyst.begin()+k); 
  }
  for(Int_t j = 0; j < (Int_t) VSystLabel.size(); j++){
   tag = VSystLabel.at(j); 
    if (sys.EndsWith("Up"))   sys = sys(0, sys.Sizeof()-3);
    if (sys.EndsWith("Down")) sys = sys(0, sys.Sizeof()-5);
    if(tag == sys || tag == sys + "Up" || tag == sys + "Down"){
      VSystLabel.erase(VSystLabel.begin()+j);
      if(verbose) cout << "Removing systematic " << tag << endl;
    }
  }
}

void Plot::UseEnvelope(TString pr, TString tags, TString newname){
  Histo* envelopeUp =  GetHisto(pr)->CloneHisto("envelopeUp"); Histo* envelopeDown = GetHisto(pr)->CloneHisto("envelopeDown"); 
  vector<Histo*> vhistos =  vector<Histo*>();
  vector<TString> v = TStringToVector(tags);
  if(verbose){
    for(Int_t i = 0; i < (Int_t) v.size(); i++) cout << v.at(i) << ", "; cout << endl;
  }
  // Naming the new systematic
  if(newname == "") newname = v.at(0);
  if(newname.EndsWith("Up")) newname = newname(0, newname.Sizeof()-3);
  else if(newname.EndsWith("Down")) newname = newname(0, newname.Sizeof()-5);
  if(verbose) cout << "New name of the systematic: " << newname << endl;
  for(Int_t k = 0; k < (Int_t) v.size(); k++) vhistos.push_back((Histo*) GetHisto(pr, v.at(k))->Clone(pr+"_"+v.at(k)+"cp"));

  envelopeUp   ->GetEnvelope(vhistos,  1);
  envelopeDown ->GetEnvelope(vhistos, -1);


  PrepareHisto(envelopeUp,   pr + "_Up", pr, itSys, 0, newname + "Up");
  PrepareHisto(envelopeDown, pr + "_Up", pr, itSys, 0, newname + "Down");
/*
  envelopeUp  ->SetProcess(pr); envelopeUp  ->SetTag(pr+"_"+newname+"Up");   envelopeUp  ->SetName(pr+"_"+newname+"Up");   
  envelopeUp  ->SetType(itSys); envelopeUp->doStackOverflow = doStackOverflow; envelopeUp  ->SetStyle();
  envelopeDown->SetProcess(pr); envelopeDown->SetTag(pr+"_"+newname+"Down"); envelopeDown->SetName(pr+"_"+newname+"Down"); 
  envelopeDown->SetType(itSys); envelopeDown->doStackOverflow = doStackOverflow; envelopeDown->SetStyle();
  AddToHistos(envelopeUp);
  AddToHistos(envelopeDown);
*/
  AddToSystematicLabels(newname);

  RemoveSystematic(tags); 
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
  for(Int_t i = 0; i < nBkgs+nSig; i++){
    if(i < nBkgs) pr = VBkgs.at(i)         ->GetProcess();
    else          pr = VSignals.at(i-nBkgs)->GetProcess();
    cout << Form("\033[1;33m  %s \033[0m ", pr.Data());
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


void Plot::PrintBinsYields(TString options){
  Int_t nBkgs = VBkgs.size();
  Int_t nSignals = VSignals.size();
  Int_t nrows = nBkgs + nSignals; 
  if(nBkgs > 0) nrows++;
  if(doData) nrows++;
  Int_t ncolumns = nb; 

  TResultsTable t(nrows, ncolumns, 1); //cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetRowTitleHeader("Process");
  t.SetFormatNum(tableFormats);

  // Set row titles
  for(Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i, VBkgs.at(i)->GetProcess());
  t.SetRowTitle(nBkgs, "Total bkg");
  for(Int_t i = nBkgs+1; i < nSignals+nBkgs+1; i++) t.SetRowTitle(i, VSignals.at(i-(nBkgs+1))->GetProcess());
  if(doData) t.SetRowTitle(nBkgs+nSignals+1, "Data");

  // Set column titles
  Float_t l0; Float_t l1;
  for(Int_t k = 0; k < ncolumns; k++){
    if(x0 == xN){
      l0 = vbins[k]; 
      l1 = vbins[k+1];
    }
    else{
      l0 = x0 + k*(xN - x0)/nb;
      l1 = x0 + (k+1)*(xN - x0)/nb;
    }
    t.SetColumnTitle(k, Form("[%2.0f, %2.0f]", l0, l1));
  }

  // Fill the table with the yields
  TString pr; Float_t nom; Float_t var;
  Float_t bTotBkgSyst= 0; Float_t bTotBkg    = 0;
  for(Int_t k = 0; k < ncolumns; k++){
    //>>> Yields for bkg
    bTotBkgSyst= 0; bTotBkg = 0;
    for(Int_t i = 0; i < nBkgs; i++){
      pr = VBkgs.at(i)->GetProcess();
      nom = GetBinYield(pr, k+1); var = GetBinYield(pr, k+1, "Up");
      t[i][k] = nom;
      t[i][k].SetError( TMath::Abs(nom-var));
      bTotBkg     += nom;
      bTotBkgSyst += (nom-var)*(nom-var);
    }

    //>>> Total bkg
    bTotBkgSyst= TMath::Sqrt(bTotBkgSyst);
    t[nBkgs][k] = bTotBkg;
    t[nBkgs][k].SetError(bTotBkgSyst); 

    //>>> Yields for signal
    for(Int_t i = nBkgs+1; i < nSignals+nBkgs+1; i++){
      pr = VSignals.at(i-(nBkgs+1))->GetProcess();
      nom = GetBinYield(pr, k+1); var = GetBinYield(pr, k+1, "Up");
      t[i][k] = nom;
      t[i][k].SetError( TMath::Abs(nom-var));
    }
    if(doData){
      pr = "Data";
      nom = GetBinYield(pr, k+1); var = GetBinYield(pr, k+1, "Up");
      t[nBkgs+nSignals+1][k] = nom;
      t[nBkgs+nSignals+1][k].SetError( TMath::Abs(nom-var));
    }
  }
  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  if(options.Contains("tex"))  t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".tex"));
  if(options.Contains("html")) t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".html"));
  if(options.Contains("txt"))  t.SaveAs(gSystem->ExpandPathName(plotFolder + "/" + YieldsTableName + ".txt"));
}


void Plot::AddNormUnc(TString pr, Float_t systUp, Float_t systDown){
  if(systDown == -99) systDown = systUp;
  Histo* hUp   = GetHisto(pr)->CloneHisto(pr + "_" + pr + "Up");
  Histo* hDown = GetHisto(pr)->CloneHisto(pr + "_" + pr + "Down");
  hUp->SetType(itSys); hDown->SetType(itSys);
  hUp->SetTag(pr + "_" + pr + "Up"); hDown->SetTag(pr + "_" + pr + "Down");
  hUp->SetSysTag(pr+"Up"); hDown->SetSysTag(pr+"Down");
  hUp->Scale(1+systUp); hDown->Scale(1-systDown);
  hUp->SetDirectory(0); hDown->SetDirectory(0);
  AddToSystematicLabels(pr);
  AddToHistos(hUp); AddToHistos(hDown);
  return;
}

Float_t Plot::GetBinYield(TString pr, Int_t bin, TString systag){
  if     (systag == "Up"   || systag == "UP"   || systag == "up"  ) systag = "Up"; 
  else if(systag == "Down" || systag == "DOWN" || systag == "down") systag = "Down"; 
  if(pr == "Data"){
    if(!hData) SetData();
    if(systag == "" || systag == "0") return hData->GetBinContent(bin);
    else{
      if(systag.Contains("Down") || systag.Contains("down")) return hData->GetBinContent(bin) - hData->GetBinError(bin);
      else return hData->GetBinContent(bin) + hData->GetBinError(bin);
    }
  }
  Float_t val = 0; Float_t sys = 0; TString ps; 
  Int_t nSyst = VSyst.size();
  Int_t nBkg = VBkgs.size();
  Int_t nSig = VSignals.size();
  Float_t var = 0;
  if(systag == "Up" || systag == "Down"){
    Float_t nom = GetBinYield(pr, bin, "0");
    var = 0; Float_t diff = 0; Float_t tempvar = 0;
    for(int k = 0; k < nSyst; k++){ // Systematics in k
      ps = VSyst.at(k)->GetTag();
      if(!ps.BeginsWith(pr+"_"))   continue;
      if(!ps.Contains(systag)) continue;
      tempvar = VSyst.at(k)->GetBinContent(bin);
      //cout << ">>> pr = " << ps << endl;
      //cout << ">>> nom = " << nom << ", var = " << tempvar << endl;
      diff += (tempvar-nom)*(tempvar-nom);
    }
    var = TMath::Sqrt(diff);
    if     (var != 0 && systag == "Up")   return nom + var;
    else if(var != 0 && systag == "Down") return nom - var;
  }
  else if(systag == "" || systag == "0"){
    for(Int_t i = 0; i < nBkg; i++){
      if(pr == VBkgs.at(i)->GetProcess()) return  VBkgs.at(i)->GetBinContent(bin);
    }
    for(Int_t i = 0; i < nSig; i++){
      if(pr == VSignals.at(i)->GetProcess()) return  VSignals.at(i)->GetBinContent(bin);
    }
    return 0;
  }
  else{ 
    var = 0; 
    for(int k = 0; k < nSyst; k++){ // Systematics in k
      ps = VSyst.at(k)->GetTag();
      if(!ps.BeginsWith(pr))   continue;
      if(!ps.Contains(systag)) continue;
      var = VSyst.at(k)->GetBinContent(bin);
      if(var != 0) return var;
    }
  }
  //cout << "[Plot::GetYield] WARNING: No systematic " << systag << " for process " << pr << "!!! ...... Returning nominal value... " << endl;
  return GetBinYield(pr, bin);
}

void Plot::Reset(){
  VData.clear();
  VBkgs.clear();
  VSignals.clear();
  VSignalsErr.clear();
  VSyst.clear();
  VSumHistoSystUp.clear();
  VSumHistoSystDown.clear();
  VSystLabel.clear();
  VTagDataSamples.clear();
  if(hratio) delete hratio;
  if(TotalSysUp) delete TotalSysUp;
  if(TotalSysDown) delete TotalSysDown;
//  if(hData && doData) delete hData;
//  if(hStack) delete hStack;
//  if(hAllBkg) delete hAllBkg;
  VBinLabels.clear();
  RatioMin = 0.8;
  RatioMax = 1.2;
  ScaleMax = 1.2;
  ScaleLog = 500;
  PlotMinimum = -999;
  PlotMaximum = -999;
}


void Plot::AddPlotFromHyperlooper(Hyperlooper *HyperLoop, TString plotname){
  distribution d = HyperLoop->GetDistribution(plotname);
  var  = d.var; cut  = d.cut; chan = d.chan; 
  nb   = d.nbins; x0   = d.bin0;  xN   = d.binN; vbins = d.bins; 
  LoopOptions = d.options; 
  weight = HyperLoop->GetWeight();
  systematics = HyperLoop->GetSyst();
  if(systematics != "0" && systematics != "")
    AddToSystematicLabels(systematics);

  Int_t type = HyperLoop->GetType(); Int_t color = HyperLoop->GetColor();
  TString process = HyperLoop->GetProcess();
  Int_t nHistos = d.vh.size();
  TString sampleName = HyperLoop->GetSampleName();
  TString systag;

  for(Int_t i = 0; i < nHistos; i++){
    systag = d.vh.at(i)->GetSysTag();
    if(systag != "0" && systag != "") type = itSys;
    if(type != itData && !LoopOptions.Contains("noScaleLumi")) d.vh.at(i)->Scale(Lumi*1000);
    PrepareHisto(d.vh.at(i), sampleName, process, type, color);
  }
}


//=========================================== MULTIPLOT

void MultiPlot::AddHyperlooper(TString sample, TString process, Int_t type, Int_t color, TString syst, TString weight){
  nDist = Dname.size(); 
  TString pathToMiniTree; 
  pathToMiniTree = path;
  if     (type == itSignal || sample.Contains("T2tt")) pathToMiniTree = pathSignal;
  else if(type == itData  ) pathToMiniTree = pathData;
  else if(sample.Contains("/")){
    pathToMiniTree = sample(0, sample.Last('/')+1);
    sample = sample(sample.Last('/')+1, sample.Sizeof());
  }
  Hyperlooper* h = new Hyperlooper(pathToMiniTree, treeName, sample, process, type, color, weight, syst); 
  for(Int_t i = 0; i < nDist; i++)
    h->AddDistribution(Dname.at(i), Dvar.at(i), Dcut.at(i), Dchan.at(i), Dnbins.at(i), Dbin0.at(i), DbinN.at(i), Dbins.at(i));
  h->Fill();
  HL.push_back(h); 
}

void MultiPlot::Loop(){
  if((Int_t) HL.size() > 0) return;
  nSamples = VTagSamples.size();
  Int_t type; Int_t color;
  TString stype = "backgroud";

  for(Int_t i = 0; i < nSamples; i++){
    if     (VTypes.at(i) == itSignal) stype = "signal";
    else if(VTypes.at(i) == itData)   stype = "data";
    else if(VTypes.at(i) == itSys)    stype = "systematic";
    cout << Form("\033[1;39mAnalyzing sample: \033[1;31m%s \033[1;39m(\033[1;35m%s\033[1;39m) as \033[1;34m%s\033[0m\n", VTagSamples.at(i).Data(), VTagProcesses.at(i).Data(), stype.Data());
    AddHyperlooper(VTagSamples.at(i), VTagProcesses.at(i), VTypes.at(i), VColors.at(i), VSystL.at(i), VWeight.at(i));
  }
}



void MultiPlot::AddDistribution(TString name, TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins){
  Dname.push_back(name);
  Dvar.push_back(var);
  Dcut.push_back(cut);
  Dchan.push_back(chan);
  //Dweight.push_back(weight);
  //DSyst.push_back(sys);
  Dnbins.push_back(nbins);
  Dbin0.push_back(bin0);
  DbinN.push_back(binN);
  Dbins.push_back(bins);
}

void MultiPlot::AddSample(TString p, TString pr, Int_t type, Int_t color, TString syst, TString weight){
  p.ReplaceAll(" ", "");
  if(pr == "") pr = p;
  if(p.Contains(",")){
    TString First_p = p(0, p.First(','));
    TString theRest = p(p.First(',')+1, p.Sizeof());
    AddSample(First_p, pr, type, color, syst, weight);
    AddSample(theRest, pr, type, color, syst, weight);
    return;
  }
  VTagSamples.push_back(p);
  VTagProcesses.push_back(pr);
  VTypes.push_back(type);
  VColors.push_back(color);
  VSystL.push_back(syst);
  VWeight.push_back(weight);
}


void MultiPlot::SetPlot(TString name, TString xtitle, TString ytitle, TString seltext, Float_t ratioMax, Float_t ratioMin){
  Reset();
  Loop();
  nSamples = VTagSamples.size();
  for(Int_t i = 0; i < nSamples; i++) 
    AddPlotFromHyperlooper(HL.at(i), name);
  SetTitleX(xtitle);
  SetTitleY(ytitle);
  SetChLabel(seltext);
  SetRatioMax(ratioMax);
  if(ratioMin == -999) ratioMin = 2-ratioMax;
  SetRatioMin(ratioMin);
} 




    // Draw systematics signal
    /* 
 
  Histo* hSignalerr = NULL; 
  VSignalsErr.clear();
  if(verbose) cout << "Drawing " << nSignals << " signals..." << endl;
       for(Int_t  i = 0; i < nSignals; i++){
       if(VSignals.at(i)->GetType() != itSignal) continue; // Consistency
       hSignalerr = (Histo*) VSignals.at(i)->Clone("hSignalErr"); // Add systematics to signal
       hSignalerr->SetFillColor(17); hSignalerr->SetMarkerStyle(0);
       hSignalerr->SetFillStyle(3013); VSignalsErr.push_back(hSignalerr);
       }
      //if(doSys) VSignalsErr.at(i)Stat->Draw("same,e2");*/
