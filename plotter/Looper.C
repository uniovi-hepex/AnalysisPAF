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
  if     (chan == "Elec")  schan = ("(TChannel == 3)");
  else if(chan == "Muon")  schan = ("(TChannel == 2)");
  else if(chan == "ElMu")  schan = ("(TChannel == 1)");
  else if(chan == "SF" || chan == "sameF") schan = ("(TChannel != 1)");
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
  else if(sys == "genMETDown"){

  }
  TString formula = TString("(") + cuts + TString(")*") + schan + TString("*") + weight;
  return formula;
}

void Looper::SetFormulas(TString systematic){
  if(FormulasCuts) delete FormulasCuts;
  if(FormulasVars) delete FormulasVars;
  stringcut = ""; stringvar = "";
  TString cu = ""; TString ch = ""; TString v = ""; 
  stringcut = CraftFormula(cut, chan, systematic);
  stringvar = CraftVar(var, systematic);
  FormulasCuts = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_cut", stringcut, tree);
  FormulasVars = new TTreeFormula("Form_" + sampleName + "_" + systematic + "_var", stringvar, tree);
}

void Looper::CreateHisto(TString sys){
  if(Hist) delete Hist;
  //Hist = new Histo(TH1F(sampleName+"_"+sys+"_"+var,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
  TString name = sampleName;
  if(sys != "0") name += "_" + sys;
  Hist = new Histo(TH1F(name,sampleName+"_"+sys+"_"+var, nbins, bin0, binN));
}

void Looper::Loop(){
  //SetFormulas(sys); // Change systematic
  Int_t nEntries = tree->GetEntries();
  Float_t val = 0; Float_t weight = 0;
  for (Long64_t jentry=0; jentry<nEntries; jentry++) {
    tree->GetEntry(jentry);
    weight  = FormulasCuts->EvalInstance();
    val     = FormulasVars->EvalInstance();
    Hist->Fill(val, weight);
  }
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
  CreateHisto(sys);
  SetFormulas(sys);
  Loop();
 /* 
  cout << " >>> Sample = " << sampleName << endl;
  cout << " >>> Cut  = " << stringcut << endl;
  cout << " >>> Var  = " << stringvar << endl;
  cout << " >>> Chan = " << chan << endl;
  cout << " >>> Systematic = " << sys << endl;
  */
  return Hist;
}
