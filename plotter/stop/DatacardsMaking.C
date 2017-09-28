//#########################################################################################
// 
// Make datacards for stop
// You need an input rootfile with all the histograms
// Select Syst, bkg, norm uncertainties, signal names and variables...
//
// HowTo:
// root -l -b -1 DatacardMaking.C
//
//#########################################################################################
R__LOAD_LIBRARY(Datacard.C)

//=== Select systematics to have into acount
const TString Sys = "ue, isr, hdamp, ME, PDF, JES, Btag, MisTag, MuonEff, ElecEff, PU, JER, MET, Trig, CR";

//=== Select backgrounds (processes...)
const TString Bkg =      "ttV,  VV,   DY,   tW,   Nonprompt, ttbar";

//=== Normalization uncertainties for the backgrounds and the signal
const TString NormSyst = "1.30, 1.30, 1.15, 1.30, 1.50,      1.053, 1.10";

//=== Luminosity uncertainty (relative)
const Float_t LumiSys = 2.5/100;

//=== Select channel
const Int_t nChannels = 1;
const TString chan[nChannels]   = {"ElMu"};//{"Elec", "ElMu", "Muon"};

//=== Select variables
const Int_t nVars     = 2;
const TString vars[nVars]       = {"MT2", "CutAndCount"};

//=== Select signals...
//const Int_t nSignals  = 3; //3, 11, 20
//const TString signals[nSignals] = {"SFS_200_50", "SFS_225_50", "SFS_250_50"};
//const TString signals[nSignals] = {"S_183_1", "S_192_25", "S_200_25", "S_208_25", "S_200_50", "S_217_50", "S_225_50", "S_233_50", "S_250_50","S_242_75", "S_250_75"};  
//const TString signals[nSignals] = {"SD_160_1", "SD_160_20","SD_170_1","SD_170_10","SD_170_20","SD_175_1","SD_175_10","SD_175_20","SD_180_1","SD_180_10","SD_180_20","SD_190_1","SD_190_10","SD_190_20","SD_200_1","SD_200_10","SD_200_20","SD_210_1","SD_210_10","SD_210_20"}; 

const Int_t nSignals  = 3 + 11 + 20;
const TString signals[nSignals] = {"SFS_200_50", "SFS_225_50", "SFS_250_50", "S_183_1", "S_192_25", "S_200_25", "S_208_25", "S_200_50", "S_217_50", "S_225_50", "S_233_50", "S_250_50","S_242_75", "S_250_75", "SD_160_1", "SD_160_20","SD_170_1","SD_170_10","SD_170_20","SD_175_1","SD_175_10","SD_175_20","SD_180_1","SD_180_10","SD_180_20","SD_190_1","SD_190_10","SD_190_20","SD_200_1","SD_200_10","SD_200_20","SD_210_1","SD_210_10","SD_210_20"};
const TString path = "output/Datacards/";

Datacard *CreateDatacard(TString signal, TString var, TString chan = "ElMu"){
  // Create a Datacard with the inputs above
  Datacard *d = new Datacard(signal, Bkg, Sys, chan);
  //d->AddNuis("TriggerEff"); d->SetShape("TriggerEff", false); 
  d->SetPathToFile(path);
  d->SetRootFileName(var);
  d->GetParamsFormFile();
  //d->SetNuisancesToAllProcesses("TriggerEff", 1.02);
  d->SetNormUnc(NormSyst);
  d->SetLumiUnc(1 + LumiSys);
  d->PrintDatacard(path + "/datacard_" + var + "_" + signal + "_" + chan + ".txt");
  return d;
}

void DatacardsMaking(){
  // Make datacards for every signal and variable in the loop
  for(Int_t i = 0; i < nSignals; i++){ 
    for(Int_t j = 0; j < nVars; j++){ 
      cout << " Signal = " << signals[i] << endl;
      Datacard *b = CreateDatacard(signals[i], vars[j], "ElMu");

     /* Datacard *c = CreateDatacard(signals[i], vars[j], "Elec");
      * Datacard *d = CreateDatacard(signals[i], vars[j], "Muon");
      * TextDatacard a = TextDatacard(b); a = a + c + d;
      * a.PrintDatacard(path + "/datacard_" + vars[j] + "_" + signals[i] + "_comb.txt");
      */
    }
  }
}
