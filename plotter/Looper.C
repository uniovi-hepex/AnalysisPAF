#include "Looper.h"

TString CraftVar(TString varstring, TString sys){
  TString var = varstring;
	if(sys == "JESUp"){
		var = var.ReplaceAll("TNJets", "TNJetsJESUp");
    var = var.ReplaceAll("TMET", "TMETJESUp");
    var = var.ReplaceAll("TMT2ll", "TMT2llJESUp");
    var = var.ReplaceAll("THT", "THTJESUp");
    var = var.ReplaceAll("TJet_Pt", "TJetJESUp_Pt");
    var = var.ReplaceAll("TNBtags", "TNBtagsJESUp");
  }
  else if(sys == "JESDown"){
    var = var.ReplaceAll("TNJets", "TNJetsJESDown");
    var = var.ReplaceAll("TMET", "TMETJESDown");
    var = var.ReplaceAll("TMT2ll", "TMT2llJESDown");
    var = var.ReplaceAll("THT", "THTJESDown");
    var = var.ReplaceAll("TJet_Pt", "TJetJESDown_Pt");
    var = var.ReplaceAll("TNBtags", "TNBtagsJESDown");
  }
  else if(sys == "JER"){
    var = var.ReplaceAll("TNBtags", "TNJetsJER");
    var = var.ReplaceAll("TJet_Pt", "TJetJER_Pt");
  }
  else if(sys == "BtagUp"){
    var = var.ReplaceAll("TNBtags", "TNBtagsUp");
  }
  else if(sys == "BtagDown"){
    var = var.ReplaceAll("TNBtags", "TNBtagsDown");
  }
  else if(sys == "MisTagUp"){
    var = var.ReplaceAll("TNBtags", "TNBtagsMisTagUp");
  }
  else if(sys == "MisTagDown"){
    var = var.ReplaceAll("TNBtags", "TNBtagsMisTagDown");
  }
  else if(sys == "genMETUp"){
    var = var.ReplaceAll("TMET", "TMETJESUp");
    var = var.ReplaceAll("TMT2ll", "TMT2llJESUp");
    var = var.ReplaceAll("THT", "THTJESUp");
    var = var.ReplaceAll("TJet_Pt", "TJetJESUp_Pt");
  }
  else if(sys == "genMETDown"){

  }
  return var;
}

TString CraftFormula(TString cuts, TString chan, TString sys){
  TString schan = ("1");
  if     (chan == "Elec")  schan = (Form("(TChannel == %i)", iElec));
  else if(chan == "Muon")  schan = (Form("(TChannel == %i)", iMuon));
  else if(chan == "ElMu")  schan = (Form("(TChannel == %i)", iElMu));
  else if(chan == "SF" || chan == "sameF") schan = (Form("(TChannel != %i)", iElMu));
  else if(chan == "3l") schan = (Form("(TChannel == %i)", iTriLep));
  else if(chan == "4l") schan = (Form("(TChannel == %i)", iFourLep));
  else schan = ("1");

  TString weight = TString("TWeight");
  if(sys == "LepEffUp"  ) weight += "_LepEffUp";
  else if(sys == "LepEffDown") weight += "_LepEffDown";
  else if(sys == "TrigUp"    ) weight += "_TrigUp";
  else if(sys == "TrigDown"  ) weight += "_TrigDown";
  else if(sys == "PUUp"      ) weight += "_PUUp";
  else if(sys == "PUDown"    ) weight += "_PUDown";
  else if(sys == "FSUp"      ) weight += "_FSUp";
  else if(sys == "FSDown"    ) weight += "_FSDown";

  else if(sys == "JESUp"){
    cuts = ( ((TString) cuts).ReplaceAll("TNJets", "TNJetsJESUp"));   
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsJESUp"));   
    cuts = ( ((TString) cuts).ReplaceAll("TMET", "TMETJESUp"));   
    cuts = ( ((TString) cuts).ReplaceAll("TMT2ll", "TMT2llJESUp"));   
    cuts = ( ((TString) cuts).ReplaceAll("THT", "THTJESUp"));   
    cuts = ( ((TString) cuts).ReplaceAll("TJet_Pt", "TJetJESUp_Pt"));   
  }
  else if(sys == "JESDown"){
    cuts = ( ((TString) cuts).ReplaceAll("TNJets", "TNJetsJESDown"));   
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsJESDown"));   
    cuts = ( ((TString) cuts).ReplaceAll("TMET", "TMETJESDown"));   
    cuts = ( ((TString) cuts).ReplaceAll("TMT2ll", "TMT2llJESDown"));   
    cuts = ( ((TString) cuts).ReplaceAll("THT", "THTJESDown"));   
    cuts = ( ((TString) cuts).ReplaceAll("TJet_Pt", "TJetJESDown_Pt"));   
  }
  else if(sys == "JER"){
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNJetsJER"));   
    cuts = ( ((TString) cuts).ReplaceAll("TJet_Pt", "TJetJER_Pt"));   
  }
  else if(sys == "BtagUp"){
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsUp"));   
  }
  else if(sys == "BtagDown"){
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsDown"));   
  }
  else if(sys == "MisTagUp"){
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsMisTagUp"));   
  }
  else if(sys == "MisTagDown"){
    cuts = ( ((TString) cuts).ReplaceAll("TNBtags", "TNBtagsMisTagDown"));   
  }
  else if(sys == "genMETUp"){
    cuts = ( ((TString) cuts).ReplaceAll("TMET", "TMETGenUp"));
    cuts = ( ((TString) cuts).ReplaceAll("TMT2ll", "TMT2llJESUp"));
    cuts = ( ((TString) cuts).ReplaceAll("THT", "THTJESUp"));
    cuts = ( ((TString) cuts).ReplaceAll("TJet_Pt", "TJetJESUp_Pt"));
  }
  else if(sys == "genMETDown"){ // Extra MET systematic for FS

  }
  TString formula = TString("(") + cuts + TString(")*") + schan + TString("*") + weight;
  return formula;
}

void Looper::SetFormulas(TString systematic){
  if(FormulasCuts) delete FormulasCuts;
  if(FormulasVars) delete FormulasVars;
  if(FormulasLHE)  delete FormulasLHE;
  stringcut = ""; stringvar = "";
  TString cu = ""; TString ch = ""; TString v = ""; 
  stringcut = CraftFormula(cut, chan, systematic);
  stringvar = CraftVar(var, systematic);

  if(stringvar.Contains("[") && stringvar.Contains("]")){
    TString number = TString(stringvar(stringvar.First("["), stringvar.First("]")) );
    stringvar.ReplaceAll(number, "");
    numberInstance = TString(number.ReplaceAll("[", "").ReplaceAll("]","")).Atoi(); 
  }
  //cout << "stringcuts = " << stringcut << endl;
  //cout << "stringvars = " << stringvar << endl;
  //cout << "numberInstance = " << numberInstance << endl;

  FormulasCuts = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_cut", stringcut, tree);
  FormulasVars = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_var", stringvar, tree);
  if(doSysScale || doSysPDF) 
    FormulasLHE  = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_LHE", "TLHEWeight", tree);
}

void Looper::CreateHisto(TString sys){
  if(Hist) delete Hist;
  //Hist = new Histo(TH1F(sampleName+"_"+sys+"_"+var,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  TString name = sampleName;
  if(sys != "0") name += "_" + sys;
  Hist = new Histo(TH1F(name,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
}

void Looper::Loop(TString sys){
  //SetFormulas(sys); // Change systematic
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t weight = 0;
  if(doSysPDF || doSysScale) hLHEweights = loadSumOfLHEweights(pathToHeppyTrees, sampleName);
  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    if(numberInstance != 0) FormulasVars->GetNdata();
    weight  = FormulasCuts->EvalInstance();
    val     = FormulasVars->EvalInstance(numberInstance);
    if(doSysPDF)   weight *= GetPDFweight(sys);
    if(doSysScale) weight *= GetScaleWeight(sys);
    Hist->Fill(val, weight);
  }
}

Float_t Looper::GetScaleWeight(TString sys){
  FormulasLHE->GetNdata();
  Float_t weight = 0; Float_t nom = 0; 
  Float_t norm = 0; 
  Int_t bin = 1002; Int_t index = 0;
  Float_t max_diff = 0;
  Float_t max_weight = 0;

  norm = hLHEweights->GetBinContent(bin);
  nom = FormulasLHE->EvalInstance(0); 

  for(Int_t index = 1; index < 9; index++){
    weight = fabs(FormulasLHE->EvalInstance(index)/hLHEweights->GetBinContent(bin+index)*norm); 
    if( fabs(weight - nom) > max_diff ) max_weight = weight;
  }
  return max_weight; //1 + fabs(max_weight - 1);
  if(sys.Contains("Up")) return max_weight;
  else                   return (1 + (1 - max_weight));
  //else                   return 1 - fabs(max_weight - 1);
}

Float_t Looper::GetPDFweight(TString sys){
  FormulasLHE->GetNdata();
  Float_t weight = 0; Float_t nom = 0; 
  Float_t norm = 0; 
  Int_t bin = 1993; Int_t index = 0;
  Float_t e = 0; Float_t v109 = 0; Float_t v110 = 0;
  Float_t rms = 0; Float_t pdfWeight = 0;

  norm = hLHEweights->GetBinContent(1002); //nominal
  nom = FormulasLHE->EvalInstance(0); 

  for(Int_t index = 9; index < 109; index++){
    weight = FormulasLHE->EvalInstance(index)/hLHEweights->GetBinContent(bin+index)*norm; 
    //cout << "weight = " << weight << endl;
    if(index < 109) e += fabs(weight - nom)*fabs(weight - nom);
    else if(index == 109) v109 = fabs(weight - nom); // Alpha s var
    else if(index == 110) v110 = fabs(weight - nom); // Alpha s var
  }
  rms = TMath::Sqrt(e/100);
  pdfWeight = TMath::Sqrt(rms*rms + ((v109-v110)*0.75/2)*((v109-v110)*0.75/2));
  //cout << "pdf weight = " << pdfWeight << endl;
  
  if   (sys.Contains("Up")) return nom + pdfWeight;
  else                      return nom - pdfWeight;
}

void Looper::loadTree(){
  TString prefix = "Tree_"; TString sufix = ".root";
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(path + prefix + sampleName + sufix);
	if (!f || !f->IsOpen()) {
		f = new TFile(path + prefix + sampleName + sufix);
	}
	f->GetObject(treeName,tree);
}

Histo* Looper::GetHisto(TString sample, TString sys){
  SetSampleName(sample); 
  loadTree();

  // For scale and PDF uncertainties
  // ----------------------------------------------------------------
  if(sys.Contains("ME") || sys.Contains("Scale") || sys.Contains("scale") || sys.Contains("Q2") || sys.Contains("PDF") || sys.Contains("pdf")){ // Using LHE weights
    if(tree->GetBranchStatus("TLHEWeight")){
      if(sys.Contains("PDF") || sys.Contains("pdf")) doSysPDF = true;
      else doSysScale = true;
    }
  }

  CreateHisto(sys);
  SetFormulas(sys);
  Loop(sys);
 /* 
  cout << " >>> Sample = " << sampleName << endl;
  cout << " >>> Cut  = " << stringcut << endl;
  cout << " >>> Var  = " << stringvar << endl;
  cout << " >>> Chan = " << chan << endl;
  cout << " >>> Systematic = " << sys << endl;
  */
  return Hist;
}

//TH1F* loadSumOfLHEweights(TString pathToHeppy, TString sampleName){
TH1D* loadSumOfLHEweights(TString pathToHeppyTrees, TString sampleName){
  Int_t nFiles = (gSystem->GetFromPipe("ls " + pathToHeppyTrees + "/Tree_" + sampleName +"_[0-9].root | wc -l")).Atoi(); 
  //cout << "Number of files = " << nFiles << endl;
  TFile* f; TH1D* hSumOfLHEweights; TH1D* htemp;

  f = TFile::Open(pathToHeppyTrees + "Tree_" + sampleName + "_0.root");
  f->GetObject("CountLHE", hSumOfLHEweights);

  for(Int_t k = 1; k < nFiles; k++){
    f = TFile::Open(pathToHeppyTrees + "Tree_" + sampleName + Form("_%i", k) + TString(".root"));
    f->GetObject("CountLHE", htemp);
    hSumOfLHEweights->Add(htemp);
  }
  hSumOfLHEweights->SetDirectory(0);
  return hSumOfLHEweights;
}
