////////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION:
//    Main macro.
//
//  USAGE:
//    root -l -b -q 'RunAnalyserPAF("Check:tttt_temp", "tree")'
//    RunAnalyserPAF("sampleName", "TheAnalysis", nSlots);
//    RunAnalyserPAF("sampleName", "TheAnalysis", nSlots, nEvents = 0);
//
////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Includes
// + Load DatasetManager needed to find out information about datasets
R__LOAD_LIBRARY(DatasetManager/DatasetManager.C+)
R__LOAD_LIBRARY(FunctionsPAF.C+)
#include "DatasetManager/DatasetManager.h"
#include <iostream>
#include <fstream>

//=============================================================================
void RunAnalyserPAF(TString sampleName  = "TTbar_Powheg", TString Selection = "StopDilep", 
		    Int_t nSlots = 1, Long64_t nEvents = 0, Long64_t FirstEvent = 0, 
		    Float_t uxsec = 1.0, TString options = "");

//=============================================================================
void GetCount(vector<TString> Files, Bool_t IsData = false);

// Global variables
//
vector<TString> Files = vector<TString>();
Double_t        SumOfWeights;
Int_t           Count;
Int_t           nTrueEntries;
Float_t         xsec;
Float_t         NormISRweights;
Bool_t          verbose = true;
Bool_t          nukeIt = true;
Bool_t          G_IsFastSim = false;
Float_t stopMass; Float_t lspMass;


//=============================================================================
// Global Enums
enum  ESelector {iStopSelec, iTopSelec, iTWSelec, iWWSelec, iHWWSelec,
                 ittDMSelec, ittHSelec, iWZSelec, i4tSelec, iStopTopSelec,
                 iTWTTbarSelec, nSel};
const TString kTagSel[nSel] = {"Stop", "Top", "TW", "WW", "HWW", "ttDM", "ttH",
                               "WZ", "tttt", "StopTop", "TWTTbar"};

//=============================================================================
// Datasets:
    //>>> 2016 datasets
    TString data2016[] = {
    "16B_03Feb2017", "16C_03Feb2017", "16D_03Feb2017", "16E_03Feb2017",
    "16F_03Feb2017", 
    "16G_03Feb2017", "16H_03Feb2017_v2", "16H_03Feb2017_v3"};
    const unsigned int nData2016 = 8;

    //>>> 2017 datasets
    TString data2017[] = { 
    //"Run2017B_12Sep2017_v1",  "Run2017C_12Sep2017_v1"};
    //"Run2017D_PromptReco_v1", 
    //"Run2017E_PromptReco_v1"
    "Run2017F_PromptReco_v1"
    };

    const unsigned int nData2017 = 1;

    TString *SelectedDataset   = data2016;
    unsigned int SelectedNdata = nData2016;

//=============================================================================
// Tabs
// Tab in the spreadsheet https://docs.google.com/spreadsheets/d/1b4qnWfZrimEGYc1z4dHl21-A9qyJgpqNUbhOlvCzjbE
const TString tab2016       = "DR80XSummer16asymptoticMiniAODv2_2";
const TString tab2016noSkim = "DR80XSummer16asymptoticMiniAODv2_2_noSkim"; 
const TString tab2017       = "2017data";
const TString tab2017v2     = "2017data_v2";

TString SelectedTab = tab2016;


//=============================================================================
// Main function
void RunAnalyserPAF(TString sampleName, TString Selection, Int_t nSlots, 
		    Long64_t nEvents, Long64_t FirstEvent,
		    Float_t uxsec, TString options) {

  // By adding this line we get all the helper functions in PAF (PAF_INFO...)
  PAFProject* myProject = 0;

  if(sampleName.BeginsWith("Check:")){
    verbose = false;
    sampleName.ReplaceAll("Check:", "");
    CheckTreesInDir(sampleName, Selection, nSlots);
    return;
  }
  
  sampleName.ReplaceAll("&&", "&");

  if(options.Contains("noForcedHadd")) nukeIt=false;

  Int_t iChunk = Int_t(uxsec);
  if(FirstEvent != 0) verbose = true;
  TString orig_sampleName = sampleName;

  if(options.Contains("xsec:")){
    pos = options.Index("xsec:") + 5;
    TString xx = options(pos, options.Sizeof());
    xx.ReplaceAll(" ", "");
    pos = xx.Contains(",") ? xx.First(",") : xx.Sizeof();
    xx = xx(0, pos);
    uxsec = xx.Atof();
  }

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

  //============================================================================
  
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
  else if(Selection == "TWTTbar"   || Selection == "tWttbar" ) sel = iTWTTbarSelec;
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

  TString tab = SelectedTab;
  if(verbose) cout << Form("\033[1;36m >>> Setting tab: %s \033[0m\n", tab.Data());
  dm->SetTab(tab.Data());

  TString pathToFiles = dataPath + dm->FindLocalFolder();
  //Temporal fix
  //TString pathToFiles = "/pool/ciencias/HeppyTreesSummer16/v2/";
  //Deal with data samples
  if(sampleName == "DoubleEG" || sampleName == "DoubleMuon" || sampleName == "MuonEG" || sampleName.BeginsWith("Single")){
    if(verbose) cout << ("\033[1;39m >>> DATA SAMPLES \033[0m\n");
    G_Event_Weight = 1.; G_IsData = true;

    TString *datasuffix             = SelectedDataset;
    const unsigned int nDataSamples = SelectedNdata;
    for(unsigned int i = 0; i < nDataSamples; i++) {
      TString asample = Form("Tree_%s_%s",sampleName.Data(), datasuffix[i].Data());
      //myProject->AddDataFiles(dm->GetRealDataFiles(asample));
      vector<TString> tempFiles = dm->GetRealDataFiles(asample);
			Files.insert(Files.end(), (tempFiles).begin(), (tempFiles).end());
      if(verbose) cout << Form("\033[1;39m >>> Searching for: \033[1;34m %s \033[1;39m ... \033[0m\n", asample.Data());
    }
    GetCount(Files, G_IsData);
  }
  else{ // Deal with MC samples 
    G_IsData = false;
    if(options.Contains("Data") || options.Contains("data")) G_IsData = true;
    TString theSample = "";
    if(sampleName.BeginsWith("LocalFile:")|| sampleName.BeginsWith("/")){ // LocalFile
      theSample = sampleName.ReplaceAll("LocalFile:", ""); 
      if(verbose) cout << " >>> Analysing a local sample: " << theSample << endl;
      TString sampleChain = TString(sampleName);
      Int_t nFiles = sampleChain.CountChar('&') + 1;
      if(verbose) cout << " [Info] Number of samples: " << nFiles << endl;
      if(sampleName.Contains("&")) {
        sampleName = TString(theSample(0, theSample.First('&'))); // For output file
        sampleName = TString(sampleName( sampleName.Last('/')+1, sampleName.Sizeof() )).ReplaceAll(".root", "").ReplaceAll("Tree_", "").ReplaceAll("_*", "").ReplaceAll("*", "").ReplaceAll(" ", "");
      }
      else sampleName = TString( theSample(theSample.Last('/')+1, theSample.Sizeof())).ReplaceAll(".root", "").ReplaceAll("Tree_", "").ReplaceAll("_*", "").ReplaceAll("*", "").ReplaceAll(" ", "");
      for(Int_t k = 0; k < nFiles; k++){
        if(sampleChain.Contains("&")){
          theSample  = TString(sampleChain(0,sampleChain.First('&')));
          sampleChain= TString(sampleChain(sampleChain.First('&')+1, sampleChain.Sizeof()));
        }
        else theSample  = sampleChain;
        theSample.ReplaceAll(" ", "");
        TString searchsample = TString( theSample(theSample.Last('/') + 1, theSample.Sizeof())).ReplaceAll(".root", "");
        theSample = TString(theSample(0, theSample.Last('/')));
        vector<TString> tempFiles = GetAllFiles(theSample, searchsample);
        Files.insert(Files.end(), tempFiles.begin(), tempFiles.end());
      }
      
      GetCount(Files, G_IsData);
      xsec = uxsec;
      G_Event_Weight = xsec/Count;
      if(SumOfWeights != Count){ // is aMCatNLO
        G_IsMCatNLO = true;
        if(verbose) cout << " >>> This is an aMCatNLO sample!!" << endl;
        G_Event_Weight = xsec/SumOfWeights;
      }
      cout << "Eventweight: " << G_Event_Weight << endl;
      cout << "Count: " << Count << endl;
      cout << "xsec: " << xsec << endl;
    }
    else if(sampleName.BeginsWith("Scan:")){ // T2tt sample
      stopMass = GetStopMass(options);
      lspMass  = GetLspMass(options);
      theSample = sampleName.ReplaceAll("Scan:", "");
      if(verbose) cout << " >>> Analysing a scan... : " << theSample << endl;
      std::vector<TString> tempFiles = dm->GetRealDataFiles(theSample);
      Files.insert(Files.end(), (tempFiles).begin(), (tempFiles).end());
      GetCount(Files);
      sampleName = Form("T2tt_mStop%i_mLsp%i",int(stopMass)+10*int(stopMass-int(stopMass)), int(lspMass)+10*int(lspMass-int(lspMass)));

      if(options.Contains("FastSim")) G_IsFastSim = true; else G_IsFastSim = false;

      xsec = GetStopXSec(stopMass);
      if(options.Contains("Dilep") || options.Contains("dilep")) xsec *= (3*0.108)*(3*0.108); // Dileptonic sample

      if(options.Contains("NormFile:")){
        TString CountFileName = options(options.Index("NormFile:")+9, options.Sizeof());
        CountFileName = CountFileName(0, CountFileName.Index(".root")+5);
        Count = GetSMSnorm(CountFileName, stopMass, lspMass); 
      }
      else Count = GetSMSnorm(Files, stopMass, lspMass);

      Bool_t doISRweights = G_IsFastSim || options.Contains("NormFile:");
      NormISRweights = GetISRweight(Files, stopMass, lspMass, doISRweights);
      G_Event_Weight = xsec/Count;
    } 
    else{ // Use dataset manager
      Float_t sumNorm = 1; long double totalXSec = 0; long double totalNorm = 0;
      TString sampleChain = TString(sampleName);
      if(sampleName.Contains("&")) sampleName = TString(sampleName(0, sampleName.First('&'))); // For output file
      sampleName.ReplaceAll(" ", "");
      Int_t nFiles = sampleChain.CountChar('&') + 1;
      if(verbose) cout << " [Info] Number of samples: " << nFiles << endl;
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
        //if(uxsec != 1) xsec    = uxsec;
      }
      if(verbose) cout << " [Info] Total number of files: " << Files.size() << endl;
      GetCount(Files);
      if(options.Contains("ISR") || options.Contains("isr")) NormISRweights = GetISRweight(Files, stopMass, lspMass);
      if(SumOfWeights != Count){ // is aMCatNLO
        G_IsMCatNLO = true;
        if(verbose) cout << " >>> This is an aMCatNLO sample!!" << endl;
        G_Event_Weight = xsec/SumOfWeights;
      }
      else{  G_Event_Weight = xsec/Count;}
    }
    if(sampleName.Contains("FastSim") || options.Contains("FastSim")) G_IsFastSim = true;
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
  if(options == "Semi")            sampleName += "Semi";
  if(options.Contains("Unfolding")) sampleName = "UNF_" + sampleName;
  
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
        command = Form("root -l -b -q \'RunAnalyserPAF.C(\"%s\", \"%s\", %i, %lli, %i, %i, \"%s\")\'", orig_sampleName.Data(), Selection.Data(), nSlots, nEvents, firstEvent, i, options.Data());
        cout << command << endl;
        os << command << endl;
        gSystem->Exec("sleep 10");
        gSystem->Exec("resetpaf -a");
      }
      return;
    }
    cout << Form("\033[0;97m >>> The sample is going to be divided in %i chunks!! \033[0m\n\n", nChunks);
    for(Int_t i = 0; i < nChunks; i++){
      firstEvent = (nTrueEntries/nChunks)*i+1;
      nEvents = nTrueEntries/nChunks;
      if(i == nChunks - 1) nEvents = nTrueEntries-firstEvent;
      RunAnalyserPAF(orig_sampleName, Selection, nSlots, nEvents, firstEvent, i, options);
      gSystem->Exec("sleep 10");
      gSystem->Exec("resetpaf -a");
    }
    cout << "\033[1;31m >>> Merging trees... \n\033[0m";
    
    TString haddCommand = "hadd " + (nukeIt ? TString("-f ") : TString("") ) + outputDir + "/Tree_" + sampleName + ".root " + outputDir + "/Tree_" + sampleName + "_[0-9].root " + ((nChunks > 10) ? outputDir + "/Tree_" + sampleName + TString("_[0-9][0-9].root") : "" );

    if (options.Contains("makeHadd")) gSystem->Exec(haddCommand);
    
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
  if(sel == ittDMSelec || sel == iTWSelec || sel ==iTWTTbarSelec){
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
  myProject->SetInputParam("stopMass"       , int(stopMass)    );
  myProject->SetInputParam("lspMass"        , int(lspMass)     );
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
  else if (sel == iWWSelec  )    myProject->AddSelectorPackage("WWAnalysis");
  else if (sel == iHWWSelec )    myProject->AddSelectorPackage("HWWAnalysis");
  else if (sel == iWZSelec  )    myProject->AddSelectorPackage("WZAnalysis");
  else if (sel == iTWTTbarSelec) myProject->AddSelectorPackage("TWTTbarAnalysis");
  else                         PAF_FATAL("RunAnalyserPAF", "No selector defined for this analysis!!!!");
  
  // Additional packages
  //----------------------------------------------------------------------------
  myProject->AddPackage("ElecScaleClass");
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


