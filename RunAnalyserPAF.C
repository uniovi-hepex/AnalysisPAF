////////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION:
//    Main macro.
//
//  USAGE:
//    RunAnalyserPAF("sampleName", "TheAnalysis", nSlots);
//    RunAnalyserPAF("sampleName", "TheAnalysis", nSlots, nEvents = 0);
//
////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Includes
// + Load DatasetManager needed to find out information about datasets
R__LOAD_LIBRARY(DatasetManager/DatasetManager.C+)
#include "DatasetManager/DatasetManager.h"
#include <iostream>
#include <fstream>
//
//=============================================================================


//=============================================================================
// Declaration of auxiliary functions
//
void RunAnalyserPAF(TString sampleName  = "TTbar_Powheg", TString Selection = "StopDilep", 
		    Int_t nSlots = 1, Long64_t nEvents = 0, Long64_t FirstEvent = 0, 
		    Float_t uxsec = 1.0, Int_t stopMass = 0, Int_t lspMass  = 0, 
		    TString options = "");

void            GetCount(vector<TString> Files, Bool_t IsData = false);
Float_t         GetSMSnorm(Int_t mStop, Int_t mLsp);
Float_t         GetISRweight(Int_t mStop, Int_t mLsp);
Double_t        GetStopXSec(Int_t StopMass);
vector<TString> GetAllFiles(TString path, TString  filename = "");
TH1D* GetHistoFromFiles(vector<TString> Files, TString histoName);
void SaveCountHistos(vector<TString> Files, TString filename); 
void SaveCountHistos(TString path, TString  inputfile, TString filename); 

void CheckTreesInDir(TString path, TString treeName = "tree", Int_t verbose = 0);
void CheckTree(TString filename, TString treeName = "tree", Int_t verbose = 0);

//Float_t* GetCountLHE(std::vector<TString> Files, Float_t a[]);
//
//=============================================================================

//=============================================================================
// Global variables
//
vector<TString> Files;
Double_t SumOfWeights;
Int_t Count;
Int_t nTrueEntries;
Float_t xsec;
Float_t NormISRweights;
Bool_t verbose = true;
Bool_t nukeIt = true;
Bool_t G_IsFastSim = false;
//
//=============================================================================


//=============================================================================
// Global Enums
enum  ESelector               {iStopSelec, iTopSelec, iTWSelec, iWWSelec, 
			 iHWWSelec,  ittDMSelec, ittHSelec, iWZSelec, i4tSelec, iStopTopSelec, nSel};
const TString kTagSel[nSel] = {"Stop",     "Top",     "TW",     "WW", "HWW",    
"ttDM", "ttH", "WZ", "tttt", "StopTop" };
//
//=============================================================================



//=============================================================================
// Main function
void RunAnalyserPAF(TString sampleName, TString Selection, Int_t nSlots, 
		    Long64_t nEvents, Long64_t FirstEvent,
		    Float_t uxsec, Int_t stopMass, Int_t lspMass, 
		    TString options) {

  // By adding this line we get all the helper functions in PAF (PAF_INFO...)
  PAFProject* myProject = 0;

  if(sampleName.BeginsWith("Check:")){
    verbose = false;
    sampleName.ReplaceAll("Check:", "");
    CheckTreesInDir(sampleName, Selection, nSlots);
    return;
  }
  
  if(options.Contains("noForcedHadd")) nukeIt=false;

  Int_t iChunk = Int_t(uxsec);
  if(FirstEvent != 0) verbose = false;
  TString orig_sampleName = sampleName;

  vector<TString> tempfiles;
  Files.clear();
	SumOfWeights = 0;
	Count = 0;
	nTrueEntries = 0;
	xsec = 0;
  NormISRweights = 0;


  TString WorkingDir = gSystem->WorkingDirectory();

  // VARIABLES TO BE USED AS PARAMETERS...
  Float_t G_Event_Weight  = 1.0;         
  Bool_t  G_IsData        = false;       
  Bool_t  G_IsMCatNLO     = false;
  Bool_t  G_DoSystematics = false;
  G_IsFastSim     = false;

  if(options.Contains("FastSim")) G_IsFastSim = true;

  // Selection
  ESelector sel = iStopSelec;
  if     (Selection == "StopDilep" || Selection == "stop"    ) sel = iStopSelec;
  else if(Selection == "Top"       || Selection == "TOP"     ) sel = iTopSelec;
  else if(Selection == "TW"        || Selection == "tW"      ) sel = iTWSelec;
  else if(Selection == "ttDM"      || Selection == "ttMET"   ) sel = ittDMSelec;
  else if(Selection == "ttH"       || Selection == "TTH"     ) sel = ittHSelec;
  else if(Selection == "tttt"      || Selection == "4t"      ) sel = i4tSelec;
  else if(Selection == "StopTop"   || Selection == "topSUSY" ) sel = iStopTopSelec;
  else if(Selection == "WW"                                  ) sel = iWWSelec;
  else if(Selection == "HWW"                                 ) sel = iHWWSelec;
  else if(Selection == "WZ"                                  ) sel = iWZSelec;
  else { 
    PAF_ERROR("RunAnalyserPAF", Form("Wrong selection \"%s\".",
				     Selection.Data()));
    TString suppsel = kTagSel[0];
    for (unsigned int i = 1; i < nSel; i++) {
      suppsel += ", ";
      suppsel += kTagSel[i];
    }
    PAF_ERROR("RunAnalyserPAF", Form("Supported selections: %s", suppsel.Data()));
    PAF_FATAL("RunAnalyserPAF", "Cannot continue. Exiting!");
  }

  cout << "\n" << endl;
  if(verbose) cout << Form("\033[1;35m >>> Analysis: %s \033[0m\n", kTagSel[sel].Data());

  // INPUT DATA SAMPLE
  //----------------------------------------------------------------------------
  TString dataPath = "/pool/ciencias/"; // Base path to input files
  TString userhome = "/mnt_pool/fanae105/user/$USER/";
  DatasetManager* dm = DatasetManager::GetInstance();

  // Tab in the spreadsheet https://docs.google.com/spreadsheets/d/1b4qnWfZrimEGYc1z4dHl21-A9qyJgpqNUbhOlvCzjbE
  dm->SetTab("DR80XSummer16asymptoticMiniAODv2_2");
  //dm->SetTab("DR80XSummer16asymptoticMiniAODv2_2_noSkim");
  
  TString pathToFiles = dataPath + dm->FindLocalFolder();
  // Deal with data samples
  if(sampleName == "DoubleEG" || sampleName == "DoubleMuon" || sampleName == "MuonEG" || sampleName.BeginsWith("Single")){
    G_Event_Weight = 1.;
    G_IsData = true;
    TString datasuffix[] = { 
      "16B_03Feb2017",
      "16C_03Feb2017",
      "16D_03Feb2017",
      "16E_03Feb2017",
      "16F_03Feb2017",
      "16G_03Feb2017",
      "16H_03Feb2017_v2",
      "16H_03Feb2017_v3"
    };
    const unsigned int nDataSamples = 8;
    for(unsigned int i = 0; i < nDataSamples; i++) {
      TString asample = Form("Tree_%s_%s",sampleName.Data(), datasuffix[i].Data());
      //myProject->AddDataFiles(dm->GetRealDataFiles(asample));
      vector<TString> tempFiles = dm->GetRealDataFiles(asample);
			Files.insert(Files.end(), (tempFiles).begin(), (tempFiles).end());
    }
    GetCount(Files, G_IsData);
    //cout << "Total events in the dataset: " << Count << endl;
    //cout << "Will loop on total number of entries: " << nTrueEntries << endl;  
  }
  else{ // Deal with MC samples           Double_t sumnormFromFiles = GetCount(path, dm->GetRealDataFiles(asample));
    
    G_IsData = false; 
    if(options.Contains("IsData") || options.Contains("isData")) G_IsData = true;
    TString theSample = "";
    if(sampleName.BeginsWith("LocalFile:")){ // LocalFile
      theSample = sampleName.ReplaceAll("LocalFile:", ""); 
      if(verbose) cout << " >>> Analysing a local sample: " << theSample << endl;
      sampleName = TString( theSample(theSample.Last('/')+1, theSample.Sizeof())).ReplaceAll(".root", "").ReplaceAll("Tree_", "").ReplaceAll("_*", "").ReplaceAll("*", "");
      TString searchsample = TString( theSample(theSample.Last('/') + 1, theSample.Sizeof()));
      theSample = theSample(0, theSample.Last('/'));
      Files = GetAllFiles(theSample, searchsample);
      GetCount(Files, G_IsData);
      xsec = uxsec;
      G_Event_Weight = xsec/Count;
    }
    else if(sampleName.BeginsWith("Scan:")){ // T2tt sample
      theSample = sampleName.ReplaceAll("Scan:", "");
      if(verbose) cout << " >>> Analysing a scan... : " << theSample << endl;
      std::vector<TString> tempFiles = dm->GetRealDataFiles(theSample);
      Files.insert(Files.end(), (tempFiles).begin(), (tempFiles).end());
      GetCount(Files);
      sampleName = Form("T2tt_mStop%i_mLsp%i",stopMass, lspMass);
      G_IsFastSim = true;
      xsec = GetStopXSec(stopMass);
      if(theSample == "T2tt_mStop160to210mLSP1to20")  xsec *= (3*0.108)*(3*0.108); // Dileptonic sample
      Count = GetSMSnorm(stopMass, lspMass);
      NormISRweights = GetISRweight(stopMass, lspMass);

      G_Event_Weight = xsec/Count;
    } 
    else{ // Use dataset manager
      Float_t sumNorm = 1; long double totalXSec = 0; long double totalNorm = 0;
      TString sampleChain = TString(sampleName);  
      if(sampleName.Contains("&")) sampleName = TString(sampleName(0, sampleName.First('&'))); // For output file
      sampleName.ReplaceAll(" ", "");
      Int_t nFiles = sampleChain.CountChar('&') + 1;
      for(Int_t k = 0; k < nFiles; k++){
        if(sampleChain.Contains("&")){
          theSample  = TString(sampleChain(0,sampleChain.First('&')));
          sampleChain= TString(sampleChain(sampleChain.First('&')+1, sampleChain.Sizeof()));
        }
        else theSample  = sampleChain;
        theSample.ReplaceAll(" ", "");
        dm->LoadDataset(theSample);
        //myProject->AddDataFiles(dm->GetFiles()); 
        Files.insert(Files.end(), (dm->GetFiles()).begin(), (dm->GetFiles()).end());
        xsec    = dm->GetCrossSection();
        // if(uxsec != 1) xsec    = uxsec;
      }
      GetCount(Files);
      if(options.Contains("ISR") || options.Contains("isr")) NormISRweights = GetISRweight(stopMass, lspMass);
      if(SumOfWeights != Count){ // is aMCatNLO
        G_IsMCatNLO = true;
        if(verbose) cout << " >>> This is an aMCatNLO sample!!" << endl;
        G_Event_Weight = xsec/SumOfWeights;
      }
      else{  G_Event_Weight = xsec/Count;}
    }
    if(sampleName.Contains("FastSim")) G_IsFastSim = true;
  }
  if(verbose){
    //cout << "\033[1;30m=================================================\033[0m\n";
    //for(Int_t i = 0; i < (Int_t) Files.size(); i++) cout << Form("\033[1;32m >>> Including file: %s \033[0m\n", Files.at(i).Data());
    cout << "\033[1;30m-------------------------------------------------\033[0m\n";
    if(!G_IsData)   cout << Form("\033[1;34m #### XSec             = %g \033[0m\n", xsec);
    cout << Form("\033[1;34m #### Total Entries    = %d \033[0m\n", nTrueEntries);
    cout << Form("\033[1;34m #### Total gen events = %d \033[0m\n", Count);
    cout << Form("\033[1;34m #### Weight for norm  = %g \033[0m\n", G_Event_Weight);
    if(G_IsMCatNLO) cout << Form("\033[1;34m #### Sum of weights   = %g \033[0m\n", SumOfWeights);
    cout << "\033[1;30m=================================================\033[0m\n";
  }
  
  // ------->>>>> Termporary solution:
  //if(sampleName.Contains("PowhegLHE")) CountLHE = GetCountLHE(Files, arr);

  // Output dir and tree name
  //----------------------------------------------------------------------------
	
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  TString outPrefix("./");
  if(username=="vischia")
    {
      if(Selection=="tttt" || Selection=="4t")
        outPrefix="/pool/cienciasrw/userstorage/pietro/tttt/2l_skim_wmt2/";
      else if(Selection=="WZ")
        outPrefix="/pool/cienciasrw/userstorage/pietro/wz/";
    }
  // Insert here your conditional. Si no, por defecto es ./
  TString outputDir = outPrefix + kTagSel[sel] + "_temp";
  if(sampleName.BeginsWith("T2tt")) outputDir += "/T2tt/";
  gSystem->mkdir(outputDir, kTRUE);
  if(sampleName.Contains("_ext2")) sampleName.ReplaceAll("_ext2",""); 
  if(sampleName.Contains("_ext1")) sampleName.ReplaceAll("_ext1",""); 
  if(sampleName.Contains("_ext"))  sampleName.ReplaceAll("_ext",""); 
  
  //if     (nEvents > 0 && FirstEvent == 0) myProject->SetNEvents(nEvents);
  if(nEvents < 0 && FirstEvent <= 0){ // Divide the sample
    Int_t nChunks = TMath::Abs(nEvents);
    Int_t firstEvent = 0;
    cout << endl;
    if(FirstEvent < 0){
      cout << "Generating commands..." << endl;
      ofstream os("commands_" + sampleName + ".sh");
      os << "# Dividing sample " << sampleName << " in " << nChunks << " chunks...\n";
      TString command;
      for(Int_t i = 0; i < nChunks; i++){
        firstEvent = (nTrueEntries/nChunks)*i+1;
        nEvents = nTrueEntries/nChunks;
        if(i == nChunks - 1) nEvents = nTrueEntries-firstEvent;
        command = Form("root -l -b -q \'RunAnalyserPAF.C(\"%s\", \"%s\", %i, %lli, %i, %i, %i, %i)\'", orig_sampleName.Data(), Selection.Data(), nSlots, nEvents, firstEvent, i, stopMass, lspMass);
        cout << command << endl;
        os << command << endl;
      }
      return;
    }
    cout << Form("\033[0;97m >>> The sample is going to be divided in %i chunks!! \033[0m\n\n", nChunks);
    for(Int_t i = 0; i < nChunks; i++){
      firstEvent = (nTrueEntries/nChunks)*i+1;
      nEvents = nTrueEntries/nChunks;
      if(i == nChunks - 1) nEvents = nTrueEntries-firstEvent;
      RunAnalyserPAF(orig_sampleName, Selection, nSlots, nEvents, firstEvent, i, stopMass , lspMass);
      //gSystem->Exec("resetpaf -a");
      //gSystem->Exec("resetpaf -a");
    }
    cout << "\033[1;31m >>> Merging trees... \n\033[0m";
    TString haddCommand = "hadd " + (nukeIt ? TString("-f ") : TString("") ) + outputDir + "/Tree_" + sampleName + ".root " + outputDir + "/Tree_" + sampleName + "_*.root";
    //TString haddCommand = "hadd " + outputDir + "/Tree_" + sampleName + ".root " + outputDir + "/Tree_" + sampleName + "_*.root";
    gSystem->Exec(haddCommand);
    cout << "\033[1;37m================================================\n\033[0m";
    cout << "\033[1;37m >>>>> >>>> >>> >> > Finito! < << <<< <<<< <<<<<\n\033[0m";
    cout << "\033[1;37m================================================\n\033[0m";
    return;
  }
  else if(FirstEvent != 0){
    if(FirstEvent == 1) FirstEvent = 0;
    cout << Form("\033[1;36m >>> Running chunk number %i, starting in event %lli... will loop over %lli events (last event = %lli)\n\n\033[0m", iChunk, FirstEvent, nEvents, FirstEvent + nEvents);
    sampleName += Form("_%i", iChunk);
  }
 
  if(options.Contains("pretend")) return; 
  
  // PAF mode selection (based on number of slots)
  //----------------------------------------------------------------------------
  PAFIExecutionEnvironment* pafmode = 0;
  if      (nSlots <=1 ) {
    PAF_INFO("RunAnalyserPAF", "Sequential mode selected");
    pafmode = new PAFSequentialEnvironment();
  }
  else if (nSlots <=64 ) {
    PAF_INFO("RunAnalyserPAF", "PROOF Lite mode selected");
    pafmode = new PAFPROOFLiteEnvironment(nSlots);
  }
  else {
    PAF_INFO("RunAnalyserPAF", "PoD mode selected");
    pafmode = new PAFPoDEnvironment(nSlots);
  }

  // Create PAF Project whith that environment
  myProject = new PAFProject(pafmode); 
  
  // Add TMVA library for TMVA Analysis
  if(sel == ittDMSelec || sel == iTWSelec){
    TString tmvalibpath = gSystem->Getenv("ROOTSYS");
    tmvalibpath += "/lib/libTMVA.so";
    myProject->AddLibrary(tmvalibpath);
    cout << tmvalibpath << endl;
  }
  
  // Add the input data files
  myProject->AddDataFiles(Files); 

  // Deal with first and last event
  if     (nEvents > 0 && FirstEvent == 0) myProject->SetNEvents(nEvents);
  else if(FirstEvent != 0){
    myProject->SetNEvents(nEvents);
    myProject->SetFirstEvent(FirstEvent);
  }

  // Set output file
  TString outputFile = outputDir + "/Tree_" + sampleName + ".root";
  PAF_INFO("RunAnalyserPAF", Form("Output file is \"%s\"\n\n",outputFile.Data()));
  myProject->SetOutputFile(outputFile);


  // Parameters for the analysis
  //----------------------------------------------------------------------------
  // COMMON PARAMETERS
  myProject->SetInputParam("sampleName",        sampleName       );
  myProject->SetInputParam("IsData",            G_IsData         );
  myProject->SetInputParam("weight",            G_Event_Weight   );
  myProject->SetInputParam("IsMCatNLO",         G_IsMCatNLO      );  
  myProject->SetInputParam("iSelection",        (int) sel        );
  myProject->SetInputParam("WorkingDir",        WorkingDir       );
  myProject->SetInputParam("pathToHeppyTrees",  pathToFiles);
  myProject->SetInputParam("SumOfWeights",  SumOfWeights);
  myProject->SetInputParam("nEntries",  nTrueEntries);
  myProject->SetInputParam("Count",  Count);
  myProject->SetInputParam("xsec",  xsec);
  myProject->SetInputParam("_options",  options);
  
  // EXTRA PARAMETERS
  myProject->SetInputParam("IsFastSim"      , G_IsFastSim);
  myProject->SetInputParam("stopMass"       , stopMass         );
  myProject->SetInputParam("lspMass"        , lspMass          );
  myProject->SetInputParam("NormISRweights" , NormISRweights   );
  myProject->SetInputParam("doSyst"         , G_DoSystematics  ); 
  
  
  // Name of analysis class
  //----------------------------------------------------------------------------
  myProject->AddSelectorPackage("LeptonSelector");
  if(sel == ittHSelec || sel == i4tSelec) myProject->AddSelectorPackage("TauSelector");
  myProject->AddSelectorPackage("JetSelector");
  myProject->AddSelectorPackage("EventBuilder");
  if      (sel == iStopSelec)    myProject->AddSelectorPackage("StopAnalysis");
  else if (sel == ittDMSelec)    myProject->AddSelectorPackage("ttDM");
  else if (sel == iTopSelec )    myProject->AddSelectorPackage("TopAnalysis");
  else if (sel == ittHSelec )    myProject->AddSelectorPackage("ttHAnalysis");
  else if (sel == i4tSelec)      myProject->AddSelectorPackage("t4Analysis");
  else if (sel == iStopTopSelec) myProject->AddSelectorPackage("StopTopAnalysis");
  else if (sel == iTWSelec  ){
    //myProject->AddSelectorPackage("TopAnalysis");
    myProject->AddSelectorPackage("TWAnalysis");
  }
  else if (sel == iWWSelec  )  myProject->AddSelectorPackage("WWAnalysis");
  else if (sel == iHWWSelec )  myProject->AddSelectorPackage("HWWAnalysis");
  else if (sel == iWZSelec  )  myProject->AddSelectorPackage("WZAnalysis");
  else                         PAF_FATAL("RunAnalyserPAF", "No selector defined for this analysis!!!!");
  
  // Additional packages
  //----------------------------------------------------------------------------
  myProject->AddPackage("Lepton");
  myProject->AddPackage("Jet");
  if(sel == i4tSelec) myProject->AddPackage("SFfor4top");
  myProject->AddPackage("mt2");
  myProject->AddPackage("Functions");
  myProject->AddPackage("LeptonSF");
  myProject->AddPackage("BTagSFUtil");
  myProject->AddPackage("PUWeight");

  // Let's rock!
  //----------------------------------------------------------------------------
  myProject->Run();
  delete myProject;

  //=============================================================================
  //== Save info in files
  //=============================================================================
  SaveCountHistos(Files, outputFile);
}





//=============================================================================
//=== Auxiliary functions
//=============================================================================



void GetCount(std::vector<TString> Files, Bool_t IsData){
	Int_t nFiles = Files.size(); TFile *f;
	TH1D *hcount; TH1D *hsum; TTree* tree;
	if(verbose) cout << "\033[1;30m=================================================\033[0m\n";
	for(Int_t i = 0; i < nFiles; i++){
		if(verbose) cout << Form("\033[1;32m >>> Including file: %s \033[0m\n", Files.at(i).Data());
		//cout << "Including file: " << Files.at(i) << endl;
		f = TFile::Open(Files.at(i));
    f->GetObject("tree", tree);
		f->GetObject("Count", hcount);
		Count        += hcount-> GetEntries();
    nTrueEntries += tree  -> GetEntries();
		if(!IsData){ 
			f->GetObject("SumGenWeights", hsum);
			SumOfWeights += hsum  -> GetBinContent(1);
		}
		f->Close();    
	}
}

Float_t GetSMSnorm(Int_t mStop, Int_t mLsp){
  cout << Form("\033[1;36m >>> Searching for normalization factor for stop point with masses [%i, %i]... ", mStop, mLsp);
  Int_t nFiles = Files.size(); TFile *f;
  TH3D *hcount; Float_t val = 0; Float_t ms = 0; Float_t mn = 0;
  Float_t count = 0;
  for(Int_t k = 0; k < nFiles; k++){
    f = TFile::Open(Files.at(k));
    f -> GetObject("CountSMS", hcount);
    Int_t nx = hcount->GetNbinsX();
    Int_t ny = hcount->GetNbinsY();
    for(Int_t i = 0; i < nx; i++){
      for(Int_t j = 0; j < ny; j++){
        val = hcount->GetBinContent(i,j,1);
        if(val != 0){
          ms = hcount->GetXaxis()->GetBinCenter(i);
          mn = hcount->GetYaxis()->GetBinCenter(j);
          if(ms == mStop && mLsp == mn) count += hcount->GetBinContent(hcount->FindBin(ms, mn, 0));
        } 
      }
    }
  }
  return count;
}

Float_t GetISRweight(Int_t mStop, Int_t mLsp){
  cout << Form("\033[1;36m >>> Searching for normalization factor for ISR Jets reweighting for stop point with masses [%i, %i]... ", mStop, mLsp);
	Int_t nFiles = Files.size(); TFile *f;
  Float_t weight = 0; Float_t nEntries = 0; Float_t nWeightedEntries = 0; Float_t TotalWeightedEntries = 0; Float_t TotalEntries;
  TTree* t;
	TH1F *hcount;
	for(Int_t k = 0; k < nFiles; k++){
    f = TFile::Open(Files.at(k));
    f -> GetObject("tree", t);
    TString strweight = "((nISRJet30==0) + (nISRJet30==1)*0.920 + (nISRJet30==2)*0.821 + (nISRJet30==3)*0.715 + (nISRJet30==4)*0.652 + (nISRJet30==5)*0.561 + (nISRJet30>5)*0.511)";
    hcount = new TH1F("hcount", "hcount", 1, 0, 2); 
    if(G_IsFastSim){ 
      TString strpoint  = Form("(GenSusyMStop == %i && GenSusyMNeutralino == %i)", Int_t(mStop), Int_t(mLsp));
      t->Project("hcount", "1", strweight + "*" + strpoint);
    }
    else {
      t->Project("hcount", "1", strweight);
    }


    nEntries = hcount->GetEntries();
    nWeightedEntries = hcount->GetBinContent(1);
    TotalEntries += nEntries;
    TotalWeightedEntries += nWeightedEntries;
    //cout << Form("nFile = %i, nEntries = %f, nWent = %f",k, nEntries, nWeightedEntries) << endl;
	}
  weight = TotalWeightedEntries/TotalEntries; 
  cout << Form("NormWeight = %1.4f\033[0m\n", weight);
  return weight;
}

Double_t GetStopXSec(Int_t StopMass){
  if      (StopMass == 125) return 574.981;
  else if (StopMass == 150) return 249.409;
  else if (StopMass == 175) return 121.416;
  else if (StopMass == 200) return 64.5085;
  else if (StopMass == 225) return 36.3818;
  else if (StopMass == 250) return 21.5949;
  else if (StopMass == 275) return 13.3231;
  else if (StopMass == 300) return 8.51615;
  else if (StopMass == 325) return 5.60471;
  else if (StopMass == 350) return 3.78661;
  else if (StopMass == 375) return 2.61162;
  else if (StopMass == 400) return 1.83537;
  else if (StopMass == 425) return 1.31169;
  else if (StopMass == 450) return 0.948333;
  else if (StopMass == 475) return 0.697075;
  else if (StopMass == 500) return 0.51848;
  else if (StopMass == 525) return 0.390303;
  else if (StopMass == 550) return 0.296128;
  else if (StopMass == 575) return 0.226118;
  else if (StopMass == 600) return 0.174599;
  else if (StopMass == 625) return 0.136372;
  else if (StopMass == 650) return 0.107045;
  else if (StopMass == 675) return 0.0844877;
  else if (StopMass == 700) return 0.0670476;
  else if (StopMass == 725) return 0.0536438;
  else if (StopMass == 750) return 0.0431418;
  else if (StopMass == 775) return 0.0348796;
  else if (StopMass == 800) return 0.0283338;
  else if (StopMass == 825) return 0.0241099;
  else if (StopMass == 850) return 0.0189612;
  else if (StopMass == 875) return 0.015625;
  else if (StopMass == 900) return 0.0128895;
  else if (StopMass == 925) return 0.0106631;
  else if (StopMass == 950) return 0.00883465;
  else if (StopMass == 975) return 0.00735655;
  else{ 
    PAF_WARN("GetStopXSec", 
	     Form("No Cross Section for that mass!! (mStop = %i) Extrapolating...", StopMass));
    Float_t v0 = GetStopXSec(StopMass - StopMass%25);
    Float_t vf = GetStopXSec(StopMass - StopMass%25 + 25);
    Float_t x  = float(StopMass%25)/25;
    return v0 + (vf-v0)*x;
  }
}

vector<TString> GetAllFiles(TString path, TString  filename) {
  if(verbose) 
    PAF_DEBUG("GetAllFiles", Form("Obtaining files of form %s in folder %s", 
				  filename.Data(), path.Data()));

  vector<TString> theFiles;

  TString command("ls ");
  if(filename != "")
    command += 
      path + "/" + filename + " " +
      path + "/" + filename + ".root " +
      path + "/" + filename + "_[0-9].root " +
      path + "/" + filename + "_[0-9][0-9].root " +
      path + "/Tree_" + filename + ".root " +
      path + "/Tree_" + filename + "_[0-9].root " +
      path + "/Tree_" + filename + "_[0-9][0-9].root";
  else command += path;

  command += " 2> /dev/null";
  if(verbose) PAF_DEBUG("GetAllFiles", Form("Executing command: %s", command.Data()));

  //We cannot use GetFromPipe because it is too verbose, so we implement
  //the full code
  //    TString result=gSystem->GetFromPipe(command);
  TString result;
  FILE *pipe = gSystem->OpenPipe(command, "r");
  if (!pipe) PAF_ERROR("GetAllFiles", Form("Cannot run command \"%s\"", command.Data()));
  else {
    TString line;
    while (line.Gets(pipe)) {
      if (result != "")	result += "\n";
      result += line;
    }
    gSystem->ClosePipe(pipe);
  }
  
  if (result != "" ) {
    TObjArray* filesfound = result.Tokenize(TString('\n'));
    if (!filesfound) 
      PAF_ERROR("GetAllFiles","Could not parse output while finding files");
    else {
      for (int i = 0; i < filesfound->GetEntries(); i++) theFiles.push_back(filesfound->At(i)->GetName());
      filesfound->Clear();
      delete filesfound;
    }
  }

  if (theFiles.size() == 0) 
    PAF_ERROR("GetAllFiles", "Could not find data!");
  return theFiles;
}

void CheckTree(TString filename, TString treeName, Int_t verbose){
  if(verbose) cout << "Checking rootfile " << filename << "... "; 
  TFile* f = TFile::Open(filename);
  if(verbose) cout << "Looking for TTree " << treeName << "... ";
  TTree* t; f->GetObject(treeName, t);
  if(t){
    if(verbose) cout << "Found! ";  
    Int_t nEntries = t->GetEntries();
    if(verbose) cout << "nEntries = " << nEntries << endl;
  }
  else{
    if(verbose) cout << "TTree not found in " << filename << endl;
  }
}

void CheckTreesInDir(TString path, TString treeName, Int_t verbose){
  if(path.EndsWith(".root")){
    CheckTree(path, treeName);
    return;
  }
  vector<TString> AllFiles = GetAllFiles(path, "Tree_*.root");
  for(Int_t i = 0; i < (Int_t) AllFiles.size(); i++){
    CheckTree(AllFiles.at(i), treeName, verbose);
  }
}

TH1D* GetHistoFromFiles(vector<TString> Files, TString histoName){
  Int_t nFiles = Files.size(); TFile *f;
  TH1D *h; TH1D *htemp;
  for(Int_t i = 0; i < nFiles; i++){
    //cout << "Opening: " << Files.at(i) << endl;
    f = TFile::Open(Files.at(i));
    if(!f->FindKey(histoName)){
      //cout << "WARNING: not found histogram \"" << histoName << "\"" << endl;
      return h;
    }
    if(i == 0) {
      f->GetObject(histoName, h); 
    }
    else{
      f->GetObject(histoName, htemp); 
      h->Add(htemp);
    }
  }
  return h;
}

void SaveCountHistos(vector<TString> Files, TString filename){ 
  TH1D* _SumOfWeights;
  TH1D* _SumOfLHEweights;
  TH1D* _Count;

  _SumOfWeights    = GetHistoFromFiles(Files, "SumGenWeights");
  _SumOfLHEweights = GetHistoFromFiles(Files, "CountLHE");
  _Count           = GetHistoFromFiles(Files, "Count");
  
  TFile* _file = TFile::Open(filename, "UPDATE");
  if(_SumOfWeights) _SumOfWeights->Write();
  if(_SumOfLHEweights) _SumOfLHEweights->Write();
  if(_Count) _Count->Write();
}

void SaveCountHistos(TString path, TString  inputfile, TString filename){ 
  vector<TString> Files = GetAllFiles(path, inputfile);
  SaveCountHistos(Files, filename);
}


