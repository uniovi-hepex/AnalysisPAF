#include "Looper.h"

void Looper::SetFormulas(TString systematic){
  if(FormulasCuts) delete FormulasCuts;
  //if(FormulasVars) delete FormulasVars;
  vvars.clear();
  vinst.clear();
  vector<TString> vvariables = TStringToVector(var, ':');
  nVars = vvariables.size();
  
  //>>> Cut
  stringcut = ""; stringvar = "";
  TString cu = ""; TString ch = ""; 
  stringcut = CraftFormula(cut, chan, systematic, weight, tree, options);
  if(verbose) cout << "[Looper::SetFormulas] Formula: " << stringcut << endl;
  FormulasCuts = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_cut", stringcut, tree);

  //>>> var
  TString v = "";
  for(Int_t k = 0; k < nVars; k++){
    FormulasVars = NULL;
    v = vvariables.at(k);
    stringvar = CraftVar(v, systematic, tree);
    if(verbose) cout << "[Looper::SetFormulas] Variable: " << stringvar << endl;

    if(stringvar.Contains("[") && stringvar.Contains("]") && !stringvar.Contains("(")){
      cout << "Getting instance from:  " << stringvar << endl;
      TString number = TString(stringvar(stringvar.First("["), stringvar.First("]")) );
      stringvar.ReplaceAll(number, "");
      numberInstance = TString(number.ReplaceAll("[", "").ReplaceAll("]","")).Atoi(); 
      vinst.push_back(numberInstance);
    }
    else vinst.push_back(0);
    FormulasVars = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_var", stringvar, tree);
    vvars.push_back(FormulasVars);
  }
}

void Looper::CreateHisto(TString sys){
  if(Hist) delete Hist;
  TString name = sampleName;
  if(sys != "0") name += "_" + sys;
  if(bin0 != binN) Hist = new Histo(TH1F(name,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  else             Hist = new Histo(TH1F(name,sampleName+"_"+sys+"_"+var, nbins, vbins));
}

void Looper::Loop(TString sys){
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t w = 0;
  Bool_t doAllInstances(false);
  if(options.Contains("AllInstances")){
    doAllInstances = true;
  }

  //>>> Starting the loop
  Int_t counter = 0;
  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    counter ++;

    //>>> Getting values for weight and variable
    w = FormulasCuts->EvalInstance();

    //>>> Fill the histogram with all the entries in an array
    if(nVars == 1 && doAllInstances){
      FormulasVars = vvars.at(0);
      Int_t nInstances = FormulasVars->GetNdata();
      for(Int_t g = 0; g < nInstances; g++){
        val = FormulasVars->EvalInstance(g);
        Hist->Fill(val, w);
      }
    }
    //>>> Fill the histogram
    else{
      for(Int_t k = 0; k < nVars; k++){
        vvars.at(k)->GetNdata();
        val     = vvars.at(k)->EvalInstance(vinst.at(k));
        Hist->Fill(val, w);
      }
    }
  }
}

//=== Load the tree
void Looper::loadTree(){
  TString prefix = "Tree_"; TString sufix = ".root";
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(path + prefix + sampleName + sufix);
	if (!f || !f->IsOpen()) {
		f = new TFile(path + prefix + sampleName + sufix);
	}
	f->GetObject(treeName,tree);
}

//=== Workflow fot creating and filling the histogram...
Histo* Looper::GetHisto(TString sample, TString sys){
  SetSampleName(sample); 
  loadTree();
  if(verbose) cout << "### Creating Histo..." << endl;
  CreateHisto(sys);
  if(verbose) cout << "### Setting formulas..." << endl;
  SetFormulas(sys);
  if(verbose) cout << "### Looping..." << endl;
  Loop(sys);
  return Hist;
}


//=== Magical function that obtains a simple plot from a minitree using Looper
Histo* GetHisto(TString path, TString samplename, TString treeName, TString var, TString cut, TString chan, TString weight, TString sys, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString options){
  Looper* ah;
  if(bin0 != binN) ah = new Looper(path, treeName, var, cut, chan, nbins, bin0, binN);
  else             ah = new Looper(path, treeName, var, cut, chan, nbins, bins);

  ah->SetWeight(weight);
  ah->SetOptions(options);
  Histo* h = ah->GetHisto(samplename, sys);
  h->SetDirectory(0);
  h->SetStyle(); 
  delete ah;
  return h;
}

//Histo* GetHistoFromHeppy(TString path, TString samplename, TString var, TString cut, TString nbins, Float_t bin0, Float_t binN, Float_t *bins){
//  Histo *h;
//  if(bin0 != binN) h = new Histo("histo", "title", nbins, bins);
//  else             h = new Histo("histo", "title", nbins, bin0, binN); 
//  
//  TChain* t = new TChain("tree", "tree");
//  std::vector<TString> files = 
//}

//
//
// MULTILOOP!!
//
void Multilooper::CreateHisto(TString sys){
  Histo* h;
  TString name = sampleName;
  if(bin0 != binN) h = new Histo(TH1F(sampleName+"_"+sys+"_"+var,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  else             h = new Histo(TH1F(sampleName+"_"+sys+"_"+var,sampleName+"_"+sys+"_"+var, nbins, vbins));
  if(sys != "0" && sys != ""){
    name += "_" + sys;
    h->SetType(itSys);
    h->SetSysTag(sys);
  }
  else{
    h->SetType(itBkg);
    h->SetSysTag("");
  }
  h->SetProcess(sampleName);
  h->SetTag(name);
  vHistos.push_back(h);
}

void Multilooper::SetFormulas(TString systematic){
  TTreeFormula* FormulasCuts;
  
  //>>> Cut
  stringcut = ""; stringvar = "";
  stringcut = CraftFormula(cut, chan, systematic, weight, tree, options);
  FormulasCuts = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_cut", stringcut, tree);

  //>>> var
  TString v = "";
  FormulasVars = NULL;
  stringvar = CraftVar(var, systematic, tree);

  if(stringvar.Contains("[") && stringvar.Contains("]") && !stringvar.Contains("(")){
    TString number = TString(stringvar(stringvar.First("["), stringvar.First("]")) );
    stringvar.ReplaceAll(number, "");
    numberInstance = TString(number.ReplaceAll("[", "").ReplaceAll("]","")).Atoi(); 
  }
  else numberInstance = 0;
  FormulasVars = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_var", stringvar, tree);
  vFormulas.push_back(FormulasCuts);
  vFormVars.push_back(FormulasVars);
}

void Multilooper::CreateHistosAndFormulas(){
  TString sys; Int_t nSyst = systLabels.size();
  CreateHisto(""); SetFormulas("");
  for(Int_t i = 0; i < nSyst; i++){
    sys = systLabels.at(i);
    if(!sys.EndsWith("Up") && !sys.EndsWith("Down")){
      CreateHisto(sys + "Up");   SetFormulas(sys + "Up");
      CreateHisto(sys + "Down"); SetFormulas(sys + "Down");
    }
    else{
      CreateHisto(sys);
      SetFormulas(sys);
    }
  } 
}

void Multilooper::Loop(){
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t w = 0;
  Int_t nHistos = 0;
  Bool_t doAllInstances(false);
  if(options.Contains("AllInstances")){
    doAllInstances = true;
  }

  //>>> Starting the loop
  Int_t counter = 0;
  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    counter ++;

    nHistos = vHistos.size();
    for(Int_t iH = 0; iH < nHistos; iH++){
      //>>> Getting values for weight and variable
      w = vFormulas.at(iH)->EvalInstance();

      //>>> Fill the histogram with all the entries in an array
      if(doAllInstances){
        Int_t nInstances = vFormVars.at(iH)->GetNdata();
        for(Int_t g = 0; g < nInstances; g++){
          val = vFormVars.at(iH)->EvalInstance(g);
          vHistos.at(iH)->Fill(val, w);
        }
      }
      //>>> Fill the histogram
      else{
        vFormVars.at(iH)->GetNdata();
        val = vFormVars.at(iH)->EvalInstance(numberInstance);
        vHistos.at(iH)->Fill(val, w);
      }
    }
  }
}

Histo* Multilooper::GetHisto(TString syst){
  Int_t nHistos = vHistos.size();
  for(Int_t iH = 0; iH < nHistos; iH++){
    Histo* h = vHistos.at(iH);
    if(h->GetSysTag() == syst) return h;
  }
  cout << "WARNING [Multilooper::GetHisto] Not found histo with syst tag: " << syst << endl;
  return vHistos.at(0);
}

void Multilooper::Fill(){
  if((Int_t) systLabels.size() == 0) cout << "WARNING [Multilooper::Fill] No systematics!! Looping for nominal..." << endl;
  if(sampleName == ""){
    cout << "ERROR [Multilooper::Fill] Please, give a valid sample name!!!" << endl;
    return;
  }
  SetSampleName(sampleName); 
  loadTree();
  CreateHistosAndFormulas();
  Loop();
}



Int_t Hyperlooper::GetPos(TString Name){
  Int_t nDist = VDist.size();
  for(Int_t i = 0; i < nDist; i++){
    if(VDist.at(i).name == Name) return i;
  }
  cout << "WARNING: Not found distribution \"" << Name << "\"..." << endl;
  return 0;
}

void Hyperlooper::AddDistribution(TString name, TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString op){
  if(op == "") op = options;
  distribution d;
  d.name = name;
  d.nbins = nbins;
  d.bin0 = bin0;
  d.binN = binN;
  d.bins = bins;
  d.var = var;
  d.chan = chan;
  d.cut  = cut;
  d.options = op;
  //d.weight = weight;
  //d.sys = sys;
  d.vf.clear();
  d.vv.clear();
  d.vh.clear();
  VDist.push_back(d); 
}

void Hyperlooper::SetFormulas(Int_t pos, TString systematic){
  distribution d = VDist.at(pos); 
  TTreeFormula* FormulasCuts;
  TTreeFormula* FormulasVars;
  
  //>>> Cut
  stringcut = ""; stringvar = "";
  stringcut = CraftFormula(d.cut, d.chan, systematic, weight, tree, d.options);
  FormulasCuts = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_cut", stringcut, tree);

  //>>> var
  FormulasVars = NULL;
  stringvar = CraftVar(d.var, systematic, tree);

  FormulasVars = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_var", stringvar, tree);
  VDist.at(pos).vf.push_back(FormulasCuts);
  VDist.at(pos).vv.push_back(FormulasVars);
} 

void Hyperlooper::CreateHisto(Int_t pos, TString sys){
  distribution d = VDist.at(pos); 
  Histo* h;
  TString name = sampleName;
  if(d.bin0 != d.binN) h = new Histo(TH1F(name+"_"+sys+"_"+d.name+"_"+process,name+"_"+sys+"_"+d.name, d.nbins, d.bin0, d.binN));
  else                 h = new Histo(TH1F(name+"_"+sys+"_"+d.name+"_"+process,name+"_"+sys+"_"+d.name, d.nbins, d.bins));
  if(sys != "0" && sys != ""){
    name += "_" + sys;
    h->SetType(itSys);
    h->SetSysTag(sys);
  }
  else{
    h->SetType(itBkg);
    h->SetSysTag("");
  }
  h->SetProcess(process);
  h->SetTag(name);
  VDist.at(pos).vh.push_back(h);
}

void Hyperlooper::CreateHistosAndFormulas(Int_t  pos){
  vector<TString> VSyst = TStringToVector(syst);
  Int_t nSyst = VSyst.size();
  TString sys;
  if(type != itSys) CreateHisto(pos, ""); SetFormulas(pos, "");// Nominal
  for(Int_t i = 0; i < nSyst; i++){ // All variations
    sys = VSyst.at(i);
    if(sys == "" || sys == "0") continue;
    if(!sys.EndsWith("Up") && !sys.EndsWith("Down")){
      CreateHisto(pos, sys + "Up");   SetFormulas(pos, sys + "Up");
      CreateHisto(pos, sys + "Down"); SetFormulas(pos, sys + "Down");
    }
    else{
      CreateHisto(pos, sys);
      SetFormulas(pos, sys);
    }
  }
}

void Hyperlooper::CreateDistributions(){
  Int_t nD = VDist.size();
  for(Int_t i = 0; i < nD; i++) CreateHistosAndFormulas(i);
  return;
}

void Hyperlooper::HyperLoop(){
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t w = 0;
  Int_t nHistos = 0; Int_t nDist = 0;
  Bool_t doAllInstances(false);
  if(options.Contains("AllInstances")){
    doAllInstances = true;
  }

  //>>> Starting the loop
  Int_t counter = 0; Int_t nInstances = 0;
  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    counter++;

    distribution d;
    nDist   = VDist.size();

    for(Int_t iD = 0; iD < nDist; iD++){
      d = VDist.at(iD);
      nHistos = d.vh.size();

      for(Int_t iH = 0; iH < nHistos; iH++){
        //>>> Getting values for weight and variable
        w = d.vf.at(iH)->EvalInstance();
        nInstances = d.vv.at(iH)->GetNdata();
        val = d.vv.at(iH)->EvalInstance();

        //>>> Fill the histogram with all the entries in an array
        if(doAllInstances){ 
          for(Int_t g = 0; g < nInstances; g++){
            val = d.vv.at(iH)->EvalInstance(g);
            d.vh.at(iH)->Fill(val, w);
          }
        }

        else{
          //>>> Fill the histogram
          d.vh.at(iH)->Fill(val, w);
        }
      }
    }
  }
}

void Hyperlooper::Fill(){
  if(sampleName == ""){
    cout << "ERROR [Hyperlooper::Fill] Please, give a valid sample name!!!" << endl;
    return;
  }
  loadTree();
  CreateDistributions();
  HyperLoop();
}


Histo* Hyperlooper::GetHisto(TString name, TString sys){
  Histo* h = nullptr;
  Int_t nDist = VDist.size(); 
  Int_t iDist = -1;
  for(Int_t i = 0; i < nDist; i++){
    if(VDist.at(i).name == name) iDist = i;
  }
  if(iDist < 0){
    cout << "[Hyperlooper::GetHisto] ERROR: not found distribution with name \"" << name << "\"..." << endl;
    return h;
  }
  distribution d = VDist.at(iDist); 
  Int_t nHistos = d.vh.size();
  for(Int_t iH = 0; iH < nHistos; iH++){
    h = d.vh.at(iH);
    if(h->GetSysTag() == sys) return h;
  }
  cout << "WARNING [Hyperlooper::GetHisto] Error: not found histo with sys tag: " << sys << endl;
  return h;
}
