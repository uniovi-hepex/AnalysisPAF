#include "EventDump.h"

std::vector<TString> GetAllVars(TString varstring){
  std::vector<TString> g;
  TString var; Int_t i;
  TString chain = varstring;
  while(chain.Contains("T")){
    i = 0;
    chain = chain(chain.First('T'), chain.Sizeof());
    while(TString(chain[i]).IsAlnum() || TString(chain[i]) == "_") i++;
    var = chain(0,i);
    g.push_back(var);
    chain = chain(i, chain.Sizeof());
  }
  return g;
}

std::vector<TString> GetAllFormats(TString varstring){
  std::vector<TString> g;
  TString var; Int_t i;
  TString chain = varstring;
  while(chain.Contains("%")){
    i = 1;
    chain = chain(chain.First('%'), chain.Sizeof());
    while(TString(chain[i]) != "," && i < chain.Sizeof()-1) i++;
    var = chain(0,i);
    g.push_back(var);
    chain = chain(i, chain.Sizeof());
  }
  return g;
}

std::vector<Int_t> GetAllIndex(TString varstring){
  std::vector<Int_t> g;
  Int_t index = 0; Int_t i;
  TString chain = varstring;
  while(chain.Contains("T")){
    i = 0;
    chain = chain(chain.First('T'), chain.Sizeof());
    while(TString(chain[i]).IsAlnum()) i++;
    if(chain[i] == '['){ 
      if(TString(chain[i+1]).IsDec()) index = TString(chain[i+1]).Atoi();
      else index = 0;
    }
    g.push_back(index);
    chain = chain(i, chain.Sizeof());
  }
  return g; 
}

TString CraftFormula(TString cuts, TString chan){
  TString schan = ("1");
  if     (chan == "ElMu")  schan = (Form("(TChannel == %i)", iElMu));
  else if(chan == "Muon")  schan = (Form("(TChannel == %i)", iMuon));
  else if(chan == "Elec")  schan = (Form("(TChannel == %i)", iElec));
  else if(chan == "2lss")  schan = (Form("(TChannel == %i)", i2lss));
  else if(chan == "3l")    schan = (Form("(TChannel == %i)", iTriLep));
  else if(chan == "4l")    schan = (Form("(TChannel == %i)", iFourLep));
  else if(chan == "SF" || chan == "sameF") schan = (Form("(TChannel != %i)", iElMu));
  else schan = chan;
  TString weight = TString("1");
  TString  formula = TString("(") + cuts + TString(")*(") + schan + TString(")*") + weight;
  return formula;
}

void EventDump::loadTree(){
  TString prefix = "Tree_"; TString sufix = ".root";
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(path + prefix + sampleName + sufix);
  if (!f || !f->IsOpen()) {
    f = new TFile(path + prefix + sampleName + sufix);
  }
  f->GetObject(treeName,tree);
}

TTreeFormula* EventDump::GetFormula(TString var){
  TTreeFormula *f = new TTreeFormula(var + "_name", var, tree);
  f->GetNdata();
  return f;
}

EventDump::EventDump(TString vars, TString formato, TString c, TString ch){
  cut = c;
  chan = ch;
  stringcut = CraftFormula(c, ch);
  stringvars = vars;
  format = formato;
  vv = GetAllVars(stringvars); 
  ii = GetAllIndex(stringvars);
  ff = GetAllFormats(formato); 
  nVars = vv.size();
}

void EventDump::Print(TString file){
  loadTree();
  for(Int_t i = 0; i < nVars; i++){
    FormVars[i] = GetFormula(vv.at(i));
  }
  FormCut = GetFormula(stringcut);
  Int_t nEntries = tree->GetEntries();

  std::ofstream outputfile;
  Bool_t wf = false;
  if(file != ""){
    if(!file.EndsWith(".txt")) file += ".txt";
    outputfile.open(file);
    wf = true;
  }
/*
  cout << "=========================================================" << endl;
  cout << "Scanning file " << file << endl;
  cout << "nEntries = " << nEntries << endl;
  cout << "Cut = " << stringcut << endl;
  cout << "nVars = " << nVars << endl;
  for(Int_t k = 0; k < nVars; k++){
    cout << k << ": " << ff.at(k) << ", " << vv.at(k) << endl;
  } 
  cout << "=========================================================" << endl;
*/
  FormCut->GetNdata();

  Int_t iEntry = 0; Int_t iVar = 0;
  for(iEntry = 0; iEntry < nEntries; iEntry++){
    tree->GetEntry(iEntry);
    for(Int_t k = 0; k < nVars; k++) FormVars[k]->GetNdata();
    if(! (FormCut->EvalInstance())){
      continue;
    }
    for(iVar = 0; iVar < nVars; iVar++){
      if(wf) outputfile << Form(ff.at(iVar), FormVars[iVar]->EvalInstance(ii.at(iVar)));
      else   cout       << Form(ff.at(iVar), FormVars[iVar]->EvalInstance(ii.at(iVar)));
    }
    if(wf) outputfile << endl;
    else   cout       << endl;
  }
}
