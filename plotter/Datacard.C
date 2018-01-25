#include "Datacard.h"
#include <fstream>

// =======================================================================
// For Datacard class
// =======================================================================

// Constructor from TString
Datacard::Datacard(TString sigName, TString bkgNames, TString nuiNames, TString bin){
  processes = TStringToVector(bkgNames);
  processes.push_back(sigName);
  nuisName = TStringToVector(nuiNames);
  signalName = sigName;
  binName = bin;
  InitialiseParams();      
} 

// Constructor from TString vectors
Datacard::Datacard(TString sigName, vector<TString> bkgNames, vector<TString> nuiNames, TString bin){
  for(Int_t i = 0; i < (Int_t) bkgNames.size(); i++) processes.push_back(bkgNames.at(i));
  processes.push_back(signalName);
  for(Int_t i = 0; i < (Int_t) nuiNames.size();  i++) nuisName.push_back(nuisName.at(i));
  signalName = sigName;
  binName = bin;
  InitialiseParams();      
}

void Datacard::SetParams(){
  if(rate) delete rate;
  if(normUnc) delete normUnc;
  if(doLumi) delete doLumi;
  if(doNuis) delete doNuis;
  if(isShape) delete isShape;
  InitialiseParams();
}

// Initialise all params from a datacard
void Datacard::InitialiseParams(){
  nProcesses = processes.size();
  nNuisances = nuisName.size();
  rate    = new Float_t[nProcesses];
  normUnc = new Float_t[nProcesses];
  doLumi  = new Bool_t[nProcesses];
  doStat  = new Bool_t[nProcesses];
  doNuis  = new Float_t*[nNuisances];
  isShape = new Bool_t[nNuisances];
  for(Int_t i = 0; i < nProcesses; i++){
    rate[i] = 0;
    normUnc[i] = 1;
    doLumi[i] = 1;
    doStat[i] = 1;
  }
  for(Int_t i = 0; i < nNuisances; i++)  {
    doNuis[i] = new Float_t[nProcesses];
    isShape[i] = true;
    for(Int_t j = 0; j < nProcesses; j++) doNuis[i][j] = 1;
  }
  dataRate = 0;
  lumiUnc = 0;
  nHistoBins = 1;
  pathToFile = ""; rootFileName = ""; cardName = "temp_datacard.txt";
  IsShapeDatacard = true;
  doBeestonBarlow = true;
}

void Datacard::GetParamsFormFile(TString options){
  if(rootFileName == ""){
    cout << "[Datacard] Error: cannot open root file!!\n";
    return;
  }
  TH1F* h; TFile* f;
  TString filename = (pathToFile != "") ? pathToFile + rootFileName : rootFileName;
  f = TFile::Open(filename);
  if(!f) cout << "[Datacard::GetParamsFormFile] ERROR: cannot find file " << filename << endl;

  // Getting rates
  for(Int_t i = 0; i < nProcesses; i++){
   if(!f->GetListOfKeys()->Contains(processes.at(i)) ) continue;
    f->GetObject(processes.at(i), h); 
    rate[i] = h->Integral();
    //h->SetDirectory();
  }   
  h = (TH1F*) f->Get("data_obs"); 
  SetRateData(h->Integral());
  nHistoBins = h->GetNbinsX();

  // Check if syst variation exists
  if(!IsShapeDatacard) return;
  TString hname;
  for(Int_t i = 0; i < nProcesses; i++){
    for(Int_t j = 0; j < nNuisances; j++){
      hname = processes.at(i) + "_" + nuisName.at(j) + "Up";
      if(!f->GetListOfKeys()->Contains(hname)) doNuis[j][i] = 0;
    }
  }
}

void Datacard::SetStatDatacard(Bool_t activate){
  // Activate all stat unc for all bkgs
  for(Int_t i = 0; i < nProcesses; i++) doStat[i] = activate;
}

void Datacard::SetStatProcess(TString p, Float_t activate){
  // Activate the stat unc for a single process
  TString process = "";
  for(Int_t i = 0; i < nProcesses; i++){
    process = processes.at(i);
    if(p==process){
      doStat[i] = activate;
      return;
    }
  }
  cout << "[Datacard::SetStatDatacard] Warning: process " << p << " not found\n";
}

void Datacard::SetNuisanceProcess(TString p, TString nuisname, Float_t activate){
  // Activate a nuisance for a process
  TString process; TString sys;
  for(Int_t i = 0; i < nProcesses; i++){
    process = processes.at(i);
    if(p != process) continue;
    for(Int_t j = 0; j < nNuisances; j++){
      sys = nuisName.at(j);
      if(sys == nuisname){
        doNuis[j][i] = activate;
        return;
      }
    }
  }
  cout << "[Datacard::SetNuisanceProcess] Warning: process " << p << "or nuisance name " << nuisname << " not found\n";
}

void Datacard::SetNuisancesToProcess(TString p, TString nuisance, Float_t val){
  // Activate a nuisace ONLY for ONE process, deativate for other processes
  if(nuisance.Contains(",")){
    nuisance.ReplaceAll(" ", "");
    TString first = nuisance(0, nuisance.First(","));
    TString rest  = nuisance(nuisance.First(",") + 1, nuisance.Sizeof());
    SetNuisancesToProcess(p, first, val);
    SetNuisancesToProcess(p, rest,  val);
    return;
  }

  TString process; TString sys;
  for(Int_t j = 0; j < nNuisances; j++){
    sys = nuisName.at(j);
    if(sys != nuisance) continue;
    for(Int_t i = 0; i < nProcesses; i++){
      process = processes.at(i);
      if(p == process) doNuis[j][i] = val;
      else doNuis[j][i] = 0;     
    }
    return;
  }
  cout << "[Datacard::SetNuisancesToProcess] Warning: process " << p << "or nuisance name " << nuisance << " not found\n";
}

void Datacard::SetNuisancesToAllProcesses(TString nuisance, Float_t val){
  TString sys;
  for(Int_t j = 0; j < nNuisances; j++){
    sys = nuisName.at(j);
    if(sys == nuisance){
      for(Int_t i = 0; i < nProcesses; i++) doNuis[j][i] = val;
      return;
    }
  }
}

void Datacard::SetRateProcess(TString p, Float_t v){
  // Change the rate for a given process
  if(p.CountChar(',')+1 == nProcesses){
    TString num;
    for(Int_t i = 0; i < nProcesses-1; i++){
      num = p(0, p.First(","));
      rate[i] = (Float_t) num.Atoll();
      p = p(p.First(",")+1, p.Sizeof());
    }
    rate[nProcesses-1] = (Float_t) p.Atoll();
    return;
  }
  TString process; TString sys;
  for(Int_t i = 0; i < nProcesses; i++){
    process = processes.at(i);
    if(p != process) continue;
    else{
      rate[i] = v;    
      return;
    }
  }
  cout << "[Datacard::SetRateProcess] Warning: process " << p << " not found\n";
}

void Datacard::SetNormUnc(TString p, Float_t v){
  // Set the norm uncertainty for a given process
  if(p.CountChar(',')+1 == nProcesses){
    TString num;
    p.ReplaceAll(" ", "");
    for(Int_t i = 0; i < nProcesses-1; i++){
      num = p(0, p.First(","));
      normUnc[i] = (Float_t) num.Atof();
      p = p(p.First(",")+1, p.Sizeof());
    }
    normUnc[nProcesses-1] = (Float_t) p.Atof();
    return;
  }
  TString process; TString sys;
  for(Int_t i = 0; i < nProcesses; i++){
    process = processes.at(i);
    if(p != process) continue;
    else{
      normUnc[i] = v;    
      return;
    }
  }
  cout << "[Datacard::SetNormUnc] Warning: process " << p << " not found\n";
}

TString Datacard::GetFileString(){
  if(!IsShapeDatacard) return "## It's not a shape datacard";
  TString shapes("shapes * ");
  shapes += binName + " ";
  shapes += rootFileName;
  shapes += " $PROCESS $PROCESS_$SYSTEMATIC\n";
  return shapes;
}

void Datacard::PrintDatacard(TString f){
  TextDatacard text = TextDatacard(this); 
  TString t = text.GetText();
  if(f == "") cout << t;
  else{
    if(!f.EndsWith(".txt")) f += ".txt";
    std::ofstream outputfile;
    outputfile.open(f);
    outputfile << t;
    outputfile.close();
  }
}


// =======================================================================
//  DataLine
// =======================================================================
// Constructor... you can directly set the title and you MUST set the dimension
DataLine::DataLine(Int_t d, TString tit){
  dim = d;
  title = tit;
  pos = new TString[dim];
  for(Int_t i = 0; i < dim; i++) pos[i] = "-";
}

// Just turn the DataLine into a String
TString DataLine::GetLine(){
  TString t = title + " ";
  for(Int_t i = 0; i < dim; i++) t += pos[i] + " ";
  t += "\n";
  return t;
}

void DataLine::SetElementsFromArray(Float_t *f){
  TString c = "-";
  for(Int_t i = 0; i < dim; i++){
    if     (f[i] == 0) c = "-";
    else if(f[i] == 1) c = "1";
    else               c = TString(Form("%1.2f",f[i]));
    pos[i] = c;
  }
}

void DataLine::SetElementsFromBool(Bool_t *f){
  TString c = "-";
  for(Int_t i = 0; i < dim; i++){
    c = f[i] ? "1" : "-";
    pos[i] = c;
  }
}

void DataLine::SetElementsFromVectorTString(std::vector<TString> t){
  if((Int_t) t.size() != dim){
    cout << "Datacard::SetElementsFromVectorTString] Error: dim for t (" << t.size() << ")does not correspond with dim of line(" << dim << ")\n";
    return;
  }
  TString c = "-";
  for(Int_t i = 0; i < dim; i++){
    pos[i] = t.at(i);
  }
}

DataLine DataLine::operator+(DataLine dl){
  if(title != dl.title) cout << "[DataLine] WARNING: summing lines with different titles... \n";
  DataLine d = DataLine(dim+dl.dim, title);
  for(Int_t i = 0; i < dim; i++) d.pos[i] = pos[i];
  for(Int_t i = dim; i < dim+dl.dim; i++) d.pos[i] = dl.pos[i-dim];
  return d;
}


// =======================================================================
//  TextDatacard
// =======================================================================
TextDatacard::TextDatacard(Int_t nPro, Int_t nNui, Int_t nHistoB, Int_t nChan){
  nProcesses  = nPro;
  nNuisances  = nNui;
  nHistoBins  = nHistoB;
  nChannels   = nChan;  
  Init();
}

TextDatacard::TextDatacard(Datacard *datacard){
  nProcesses  = datacard->GetNProcesses();
  nNuisances  = datacard->GetNNuisances();
  nHistoBins  = datacard->GetNHistoBins();
  nChannels   = 1;
  Init();

  doBeestonBarlow = datacard->doBeestonBarlow;
  shapes = datacard->GetFileString();
  obs_bin.SetElement(    0, datacard->GetBinName());
  observation.SetElement(0, TString(Form("%i",datacard->GetDataRate())));
  for(Int_t i = 0; i < nProcesses; i++) pro_bin.SetElement(i, datacard->GetBinName());
  pro_name.SetElementsFromVectorTString(datacard->GetVectorProcesses());
  for(Int_t i = 0; i < nProcesses-1; i++) pro_num.SetElement(i, TString(Form("%i",i+1)));
  pro_num.SetElement(nProcesses-1, "-1");
  rates.SetElementsFromArray(datacard->GetArrayRates());
  Lumi.SetElementsFromBool(datacard->GetDoLumi());
  for(Int_t i = 0; i < nProcesses; i++) if(Lumi.GetElement(i).Atoi()) Lumi.SetElement(i, TString(Form("%1.3f", datacard->GetLumiUnc())));

  for(Int_t i = 0; i < nProcesses; i++){
    processNorm[i].SetTitle(datacard->GetProcessName(i) + " lnN");
    for(Int_t j = 0; j < nProcesses; j++){
      if(i == j) processNorm[i].SetElement(j, TString(Form("%1.3f",datacard->GetNormUnc(i))));
      else       processNorm[i].SetElement(j,"-");
    }
  }
  if(doBeestonBarlow){
    for(Int_t i = 0; i < nProcesses; i++){
      for(Int_t j = 0; j < nHistoBins; j++){
        statUnc[i*nHistoBins +j].SetTitle(datacard->GetProcessName(i) + "_" + datacard->GetBinName() + Form("_statbin%i shape", j+1));
        statUnc[i*nHistoBins +j].SetElement(i%nProcesses, "1");
      }
    }
    if(nChannels == 1){
      for(Int_t i = 0; i < nProcesses*nHistoBins; i++) totalStatDatacard += statUnc[i].GetLine();
    }
  }

  for(Int_t i = 0; i < nNuisances; i++){
    if  (datacard->IsShape(i)) systUnc[i].SetTitle(datacard->GetNuisName(i) + " shape");
    else                       systUnc[i].SetTitle(datacard->GetNuisName(i) + " lnN");
    systUnc[i].SetElementsFromArray(datacard->GetDoNuis(i));
  }
}

TString TextDatacard::GetStatUncData(Int_t nExtraPos){
  TString t = "";
  DataLine extraPos;
  if(nExtraPos == 0)  for(Int_t i = 0; i < nProcesses*nHistoBins; i++) t += (statUnc[i]).GetLine();
  else if(nExtraPos < 0){
    for(Int_t i = 0; i < nProcesses*nHistoBins; i++){
      extraPos = DataLine(TMath::Abs(nExtraPos), statUnc[i].GetTitle());
      statUnc[i] = extraPos + statUnc[i];
      t += (statUnc[i]).GetLine();
    }
  }
  else{
    for(Int_t i = 0; i < nProcesses*nHistoBins; i++){
      extraPos = DataLine(nExtraPos, statUnc[i].GetTitle());
      statUnc[i] = statUnc[i] + extraPos;
      t += (statUnc[i]).GetLine();
    }
  }
  return t;
}

TString TextDatacard::GetText(){
  Int_t i = 0;
  TString out = Form("imax %i\n", nChannels);
  out += "jmax *\nkmax *\n" + _separation;
  out += shapes + _separation; 
  out += obs_bin.GetLine() + observation.GetLine() + _separation;
  out += pro_bin.GetLine() + pro_name.GetLine() + pro_num.GetLine() + rates.GetLine() + _separation;
  out += Lumi.GetLine();
  for(i = 0; i < nProcesses; i++)            out += processNorm[i].GetLine();
  if(!doBeestonBarlow) out += totalStatDatacard;
  //for(Int_t i = 0; i < nProcesses*nHistoBins*nChannels; i++) out += statUnc[i].GetLine();
  for(i = 0; i < nNuisances; i++)            out += systUnc[i].GetLine();
  if(doBeestonBarlow) out += "* autoMCStats 0";
  return out;
}

void TextDatacard::PrintDatacard(TString f){
  TString t = GetText();
  if(f == "") cout << t;
  else{
    if(!f.EndsWith(".txt")) f += ".txt";
    std::ofstream outputfile;
    outputfile.open(f);
    outputfile << t;
    outputfile.close();
  }
}


TextDatacard TextDatacard::operator+(TextDatacard d){
  if(nProcesses != d.nProcesses) cout << "Different number of processes... Imposible to merge!\n";
  if(nNuisances != d.nNuisances) cout << "Different number of nuisances... Imposible to merge!\n";

  TextDatacard t =  *this;
  t.nChannels  += d.nChannels;
 // t.nProcesses += d.nProcesses;
  t.shapes += d.shapes;
  t.obs_bin     = t.obs_bin     + d.obs_bin;
  t.observation = t.observation + d.observation;
  t.pro_bin     = t.pro_bin     + d.pro_bin;
  t.pro_name    = t.pro_name    + d.pro_name;
  t.pro_num     = t.pro_num     + d.pro_num;
  t.rates       = t.rates       + d.rates;
  t.Lumi        = t.Lumi        + d.Lumi;
  for(Int_t i = 0; i < nProcesses; i++) t.processNorm[i] = t.processNorm[i] + d.processNorm[i];
  for(Int_t i = 0; i < nNuisances; i++) t.systUnc[i]     = t.systUnc[i]     + d.systUnc[i];
  t.totalStatDatacard = t.GetStatUncData(nProcesses*d.nChannels) + d.GetStatUncData(-nProcesses*(t.nChannels-d.nChannels));
  
  return t;
}


// =======================================================================
// Other functions
// =======================================================================
std::vector<TString> Datacard::TStringToVector(TString t, char separator){
  std::vector<TString> v;
  t.ReplaceAll(" ", "");
  Int_t n = t.CountChar(separator);
  TString element;
  for(Int_t i = 0; i < n; i++){
    element = t(0, t.First(separator));
    t = t(t.First(separator)+1, t.Sizeof());
    v.push_back(element);
  }
  v.push_back(t);
  return v;
}


