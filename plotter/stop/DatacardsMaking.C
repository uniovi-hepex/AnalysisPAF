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
const TString Sys = "ue, isr, hdamp, ME, PDF, JES, Btag, MisTag, MuonEff, ElecEff, PU, JER, MET, Trig, CR, fsr, topMass, MuScale, ElScale, unclMET";

//=== Select backgrounds (processes...)
const TString Bkg =      "ttV,  VV,   DY,   tW,   Nonprompt, ttbar";

//=== Normalization uncertainties for the backgrounds and the signal
float normxsec = TMath::Sqrt(2.8*2.8 + 5.3*5.3);
const TString NormSyst = Form("1.30, 1.30, 1.15, 1.30, 1.50,      %1.3f, 1.15", 1 + normxsec/100);

//=== Luminosity uncertainty (relative)
const Float_t LumiSys = 2.5/100;

//=== Select channel
const Int_t nChannels = 1;
const TString chan[nChannels]   = {"ElMu"};//{"Elec", "ElMu", "Muon"};

//=== Select variables
const Int_t nVars     = 1;
const TString vars[nVars]       = {"MT2_MET_DeltaEta3_ElMu"}; // "MT2_21", "DeltaEta", "DeltaPhi", "MET"};//, "CutAndCount"};
//const Int_t nVars     = 11;
//const TString vars[nVars]       = {"CutAndCount", "MT2_26", "MT2_25", "MT2_20", "MT2_15", "MT2_9", "MT2_5", "MT2_3", "MT2_252", "MT2_4", "MT2_32"};
//const Int_t nVars     = 4;
//const TString vars[nVars]       = {"MT2_Elec", "MT2_Muon", "MT2_ElMu", "Channel"};
//const Int_t nVars = 21;
/*const TString vars[nVars] = {
"MT2_MT2g0b5", 
"MT2_MT2g5b10", 
"MT2_MT2g10b15", 
"MT2_MT2g15b20", 
"MT2_MT2g20b25", 
"MT2_MT2g25b30", 
"MT2_MT2g30b35", 
"MT2_MT2g35b40", 
"MT2_MT2g40b45", 
"MT2_MT2g45b50", 
"MT2_MT2g50b55", 
"MT2_MT2g55b60", 
"MT2_MT2g60b65", 
"MT2_MT2g65b70", 
"MT2_MT2g70b75", 
"MT2_MT2g75b80", 
"MT2_MT2g80b85", 
"MT2_MT2g85b90", 
"MT2_MT2g90b95", 
"MT2_MT2g95b100", 
"MT2_MT2g100b200"};
*/

//=== Select signals...
//const Int_t nSignals  = 3; //3, 11, 20
//const TString signals[nSignals] = {"SFS_200_50", "SFS_225_50", "SFS_250_50"};
//const TString signals[nSignals] = {"S_183_1", "S_192_25", "S_200_25", "S_208_25", "S_200_50", "S_217_50", "S_225_50", "S_233_50", "S_250_50","S_242_75", "S_250_75"};  
//const TString signals[nSignals] = {"SD_160_1", "SD_160_20","SD_170_1","SD_170_10","SD_170_20","SD_175_1","SD_175_10","SD_175_20","SD_180_1","SD_180_10","SD_180_20","SD_190_1","SD_190_10","SD_190_20","SD_200_1","SD_200_10","SD_200_20","SD_210_1","SD_210_10","SD_210_20"}; 

//const Int_t nSignals  = 3 + 11 + 20 + 3;
//const TString signals[nSignals] = {"SFS_200_50", "SFS_225_50", "SFS_250_50", "S_183_1", "S_192_25", "S_200_25", "S_208_25", "S_200_50", "S_217_50", "S_225_50", "S_233_50", "S_250_50","S_242_75", "S_250_75", "SD_160_1", "SD_160_20","SD_170_1","SD_170_10","SD_170_20","SD_175_1","SD_175_10","SD_175_20","SD_180_1","SD_180_10","SD_180_20","SD_190_1","SD_190_10","SD_190_20","SD_200_1","SD_200_10","SD_200_20","SD_210_1","SD_210_10","SD_210_20", "SFS_200_50_xqcut20", "SFS_225_50_xqcut20", "SFS_250_50_xqcut20"};
const Int_t nSignals  = 30 + 6;
//const TString signals[nSignals] = {"SFS_200_50", "SFS_225_50", "SFS_250_50"};
const TString signals[nSignals] = {"T2tt_167p5_1p0", "T2tt_175p0_1p0", "T2tt_175p0_7p5", "T2tt_182p5_1p0", "T2tt_182p5_7p5", "T2tt_182p5_15p0", "T2tt_190p0_7p5", "T2tt_190p0_15p0", "T2tt_190p0_22p5", "T2tt_197p5_15p0", "T2tt_197p5_22p5", "T2tt_197p5_30p0", "T2tt_205p0_22p5", "T2tt_205p0_30p0", "T2tt_205p0_37p5", "T2tt_212p5_30p0", "T2tt_212p5_37p5", "T2tt_212p5_45p0", "T2tt_220p0_37p5", "T2tt_220p0_45p0", "T2tt_220p0_52p5", "T2tt_227p5_45p0", "T2tt_227p5_52p5", "T2tt_227p5_60p0", "T2tt_235p0_52p5", "T2tt_235p0_60p0", "T2tt_235p0_67p5", "T2tt_242p5_60p0", "T2tt_242p5_67p5", "T2tt_242p5_75p0","SFS_200_50_xqcut20", "SFS_225_50_xqcut20", "SFS_250_50_xqcut20", "SFS_200_50", "SFS_225_50", "SFS_250_50"};

//const Int_t nSignals  = 3;
//const TString signals[nSignals] = {"T2tt_227p5_52p5_mod", "T2tt_227p5_52p5_mod2", "T2tt_227p5_52p5_mod3"};//, "T2tt_227p5_52p5_mod06", "T2tt_227p5_52p5_mod16", "T2tt_227p5_52p5_mod26", "T2tt_227p5_52p5_mod36", "T2tt_227p5_52p5_mod46", "T2tt_227p5_52p5_mod56"};

//const Int_t nSignals  = 1;
//const TString signals[nSignals] = {"T2tt_227p5_52p5"};

//const TString path = "output/Datacards/";
//const TString path = "output/Datacards/";
const TString path = "./output/Datacards_jan5/";

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
  d->doBeestonBarlow = true;
  d->PrintDatacard(path + "/datacard_" + var + "_" + signal + "_" + chan + ".txt");
  return d;
}

void DatacardsMaking(){
  // Make datacards for every signal and variable in the loop
  TString chan = "ElMu";
  for(Int_t j = 0; j < nVars; j++){ 
    for(Int_t i = 0; i < nSignals; i++){ 
      cout << " Signal = " << signals[i] << endl;
      Datacard *b = CreateDatacard(signals[i], vars[j], chan);

     /* Datacard *c = CreateDatacard(signals[i], vars[j], "Elec");
      * Datacard *d = CreateDatacard(signals[i], vars[j], "Muon");
      * TextDatacard a = TextDatacard(b); a = a + c + d;
      * a.PrintDatacard(path + "/datacard_" + vars[j] + "_" + signals[i] + "_comb.txt");
      */
    }
  }
}
