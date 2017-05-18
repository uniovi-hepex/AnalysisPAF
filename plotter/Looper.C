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
    var = var.ReplaceAll("TBDT", "TBDTJESUp");
  }
  else if(sys == "JESDown"){
    var = var.ReplaceAll("TNJets", "TNJetsJESDown");
    var = var.ReplaceAll("TMET", "TMETJESDown");
    var = var.ReplaceAll("TMT2ll", "TMT2llJESDown");
    var = var.ReplaceAll("THT", "THTJESDown");
    var = var.ReplaceAll("TJet_Pt", "TJetJESDown_Pt");
    var = var.ReplaceAll("TNBtags", "TNBtagsJESDown");
    var = var.ReplaceAll("TBDT", "TBDTJESDown");
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

TString CraftFormula(TString cuts, TString chan, TString sys, TString options){
  TString schan = ("1");
  if     (chan == "ElMu")  schan = (Form("(TChannel == %i)", iElMu));
  else if(chan == "Muon")  schan = (Form("(TChannel == %i)", iMuon));
  else if(chan == "Elec")  schan = (Form("(TChannel == %i)", iElec));
  else if(chan == "2lss")  schan = (Form("(TChannel == %i)", i2lss));
  else if(chan == "3l")    schan = (Form("(TChannel == %i)", iTriLep));
  else if(chan == "4l")    schan = (Form("(TChannel == %i)", iFourLep));
  else if(chan == "SF" || chan == "sameF") schan = (Form("(TChannel != %i)", iElMu));
  else schan = ("1");

  TString weight = TString("TWeight");
  if     (sys == "LepEffUp"  ) weight += "_LepEffUp";
  else if(sys == "LepEffDown") weight += "_LepEffDown";
  else if(sys == "ElecEffUp"  ) weight += "_ElecEffUp";
  else if(sys == "ElecEffDown") weight += "_ElecEffDown";
  else if(sys == "MuonEffUp"  ) weight += "_MuonEffUp";
  else if(sys == "MuonEffDown") weight += "_MuonEffDown";
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
  else if(sys == "JER" || sys == "JERUp" || sys == "JERDown"){
    cuts = ( ((TString) cuts).ReplaceAll("TNJets", "TNJetsJER"));   
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
  if(options.Contains("isr") || options.Contains("ISR")) formula = "TISRweight*(" + formula + ")";
  return formula;
}

void Looper::SetFormulas(TString systematic){
  if(FormulasCuts) delete FormulasCuts;
  if(FormulasVars) delete FormulasVars;
  if(FormulasLHE)  delete FormulasLHE;
  stringcut = ""; stringvar = "";
  TString cu = ""; TString ch = ""; TString v = ""; 
  stringcut = CraftFormula(cut, chan, systematic, options);
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
  if(doSysScale || doSysPDF){
    FormulasLHE  = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_LHE", "TLHEWeight", tree);
    FormulasLHE->GetNdata();
  }
}

void Looper::CreateHisto(TString sys){
  if(Hist) delete Hist;
  //Hist = new Histo(TH1F(sampleName+"_"+sys+"_"+var,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  TString name = sampleName;
  if(sys != "0") name += "_" + sys;
  Hist = new Histo(TH1F(name,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  if(doSysPDF || doSysScale){
    for(Int_t i = 0; i < nLHEweights; i++){
      hLHE[i] = new TH1F(name+"_"+Form("%i", i),sampleName+"_"+sys+"_"+var+"_"+Form("%i",i), nbins, bin0, binN);
    }
  }
}

void Looper::Loop(TString sys){
  //SetFormulas(sys); // Change systematic
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t weight = 0;
  Int_t nInstances = FormulasVars->GetNdata();
  Bool_t doAllInstances(false);
  if(options.Contains("AllInstances")){
    doAllInstances = true;
    cout << "Looping over all values of the variable!! Number of instances: " << nInstances << endl;
  }

  // Options for systematics
  if(doSysPDF || doSysScale) hLHEweights = loadSumOfLHEweights(pathToHeppyTrees, sampleName);

  // For fake or flips from data
/*  Bool_t histo2D(false);
  TTreeFormula *LepPt;
  TTreeFormula *LepEta;
  if(options.Contains("UseHisto2D")){
    histo2D = true; 

    TString InfoHisto = TString(options.Data());
    InfoHisto.ReplaceAll(" ", "");
    InfoHisto.ReplaceAll("UseHisto2D:", "");
    // Careful if there's more than one option! it should be after the UseHisto!
    if(InfoHisto.Contains(",")) InfoHisto = InfoHisto(0, InfoHisto.First(','));
    TString Path_to_histo = InfoHisto(0, InfoHisto.First(':')); InfoHisto.ReplaceAll(Path_to_histo + ":", "");
    TString histo_name    = InfoHisto(0, InfoHisto.First(':')); InfoHisto.ReplaceAll(histo_name + ":", "");
    TString varname1      = InfoHisto(0, InfoHisto.First(':')); InfoHisto.ReplaceAll(varname1 + ":", "");
    TString varname2      = InfoHisto(0, InfoHisto.First(':')); InfoHisto.ReplaceAll(varname2 + ":", "");
    loadHisto2D(Path_to_histo, histo_name);
    LepPt    = GetFormula("LepPt",    "TFLep_Pt");
    LepEta   = GetFormula("LepEta",   "TFLep_Eta");
  }*/
  Float_t f;
  if(options.Contains("Fake") || options.Contains("fake")){
    TString WorkingDir = gSystem->WorkingDirectory();

/*
    LepPt    = GetFormula("LepPt",    "TFLep_Pt");
    LepEta   = GetFormula("LepEta",   "TFLep_Eta");
    LepPdgId = GetFormula("LepPdgId", "TFLep_pdgId");
    LepChar  = GetFormula("LepChar",  "TFLep_Charge");
    nSelLep  = GetFormula("nSelLep",  "TNSelLeps");
    nFakeLep = GetFormula("nFakeLep", "TNFakeableLeps");
*/
    tree->SetBranchAddress("TFLep_Pt", FLepPt);
    tree->SetBranchAddress("TFLep_Eta", FLepEta);
    tree->SetBranchAddress("TFLep_pdgId", FLepPdgId);
    tree->SetBranchAddress("TFLep_Charge", FLepCharge);
    tree->SetBranchAddress("TLep_Charge", LepCharge);
    tree->SetBranchAddress("TNSelLeps", &nLeps);
    tree->SetBranchAddress("TNFakeableLeps", &nFakeLeps);
  }

  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    if(numberInstance != 0) FormulasVars->GetNdata();
    weight  = FormulasCuts->EvalInstance();
    val     = FormulasVars->EvalInstance(numberInstance);

    // For ScaleME and PDF systematics
    if(doSysScale || doSysPDF){
      Float_t LHEweight = 1;
      for(Int_t i = 0; i < nLHEweights; i++){
        LHEweight = weight*getLHEweight(i);
        hLHE[i]->Fill(val, LHEweight);
      }
    }

    // For fakes or flips
    /*if(histo2D){
      // One weight for lepton
        Float_t hWeight = 1;
          Float_t v1 = 0; Float_t v2 = 0; Float_t vIso = 0;
          for(Int_t = 0; i < nLeptons; i++){
          v1 = var1->EvalInstance(i);
          v2 = var2->EvalInstance(i);
          } 
     }     */

    if(options.Contains("Fake") || options.Contains("fake")){
      f = 1;
      if(nFakeLeps <= 0) continue;
      if(nLeps >= 3)     continue;
      if(nLeps == 2){ // If is SS, is not fake
        if(LepCharge[0] == LepCharge[1]) continue;
      }
      for(Int_t i = 0; i < nFakeLeps; i++){
        //if(FLepPdgId[i] == 11) f *= electronFakeRate(FLepPt[i], FLepEta[i]);
        //if(FLepPdgId[i] == 13) f *=     muonFakeRate(FLepPt[i], FLepEta[i]);
      }
      weight *= f/(1-f);
      if(options.Contains("Sub") || options.Contains("sub")) weight *= -1;
    }

    if(doAllInstances){
      nInstances = FormulasVars->GetNdata();
      for(Int_t g = 0; g < nInstances; g++){
        val = FormulasVars->EvalInstance(g);
        Hist->Fill(val, weight);
      }
    }
    else    Hist->Fill(val, weight);
  }


  Float_t nom = 0; Float_t var = 0; Float_t ext = 0; Float_t env = 0;
  if(doSysScale){ // Get envelope!!
    //cout << " Scale matrix element weights: \n";
    for(Int_t bin = 1; bin <= nbins; bin++){
      ext = 0; env = 0;
      nom = hLHE[0]->GetBinContent(bin);
      for(Int_t w = 1; w < 9; w++){
        var = hLHE[w]->GetBinContent(bin);  
        if(sys.Contains("Up") || sys.Contains("up")){
          if(nom-var > ext){ ext = nom-var; env = var;}
        }
        else{
          if(var-nom > ext){ ext = var-nom; env = var;}
        }
        //cout << "   nom = " << nom << ", var = " << var << endl;
      }
      Hist->SetBinContent(bin, env);
    }
  }
  else if(doSysPDF){
    Float_t rms = 0; Float_t alpha_up = 0; Float_t alpha_dw = 0;
    //cout << " PDF weights: \n";
    for(Int_t bin = 1; bin <= nbins; bin++){
      nom = hLHE[0]->GetBinContent(bin);
      for(Int_t w = 9; w < 109; w++){
        var = hLHE[w]->GetBinContent(bin);  
        ext += (nom-var)*(nom-var);
        //cout << "   nom = " << nom << ", var = " << var << endl;
      }
      rms = TMath::Sqrt(ext/100);
      alpha_up = TMath::Abs(hLHE[109]->GetBinContent(bin) - nom);
      alpha_dw = TMath::Abs(hLHE[110]->GetBinContent(bin) - nom);
      env = TMath::Sqrt(rms*rms + ((alpha_up-alpha_dw)*0.75/2)*((alpha_up-alpha_dw)*0.75/2));
      if(sys.Contains("Up") || sys.Contains("up"))  Hist->SetBinContent(bin, nom + env);
      else                                          Hist->SetBinContent(bin, nom - env);
    }
  }
}

Float_t Looper::getLHEweight(Int_t i){
  Float_t weight = 0; Float_t norm = 0; 
  Int_t bin = 0; 
  if      (i<9 )  bin = i + 1002;   // 1002-1010: muRmuF
  else if (i<111) bin = i + 1993;   // 2002-2103: NNPDF
  else if (i<166) bin = i + 2891;   // 3002-3056: CT10
  else if (i<222) bin = i + 3836;   // 4000-4057: MMHT2014
  else if (i<249) bin = i + 4780;   // 5002-5028: muRmuF, hdamp 
  norm = hLHEweights->GetBinContent(1002);
  weight = FormulasLHE->EvalInstance(i)/hLHEweights->GetBinContent(bin)*norm; 
  return weight;
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



TTreeFormula* Looper::GetFormula(TString name, TString var){
  TTreeFormula *f = new TTreeFormula(name, var, tree);
  f->GetNdata();
  return f;
}

void Looper::loadHisto2D(TString Path_to_histo, TString histo_name){

}
