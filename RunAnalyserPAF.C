R__LOAD_LIBRARY(DatasetManager/DatasetManager.C+)
#include "DatasetManager/DatasetManager.h"

void RunAnalyserPAF(TString sampleName  = "TTbar_Madgraph", Int_t nSlots = 1,
			Long64_t nEvents = 0,
			Int_t stopMass = 0, Int_t lspMass  = 0,  Float_t  SusyWeight = 0.0) {
  
  // VARIABLES TO BE USED AS PARAMETERS...
  Float_t G_Event_Weight  = 1.0;         
  Bool_t  G_IsData        = false;       
  Bool_t  G_IsMCatNLO     = false;

  // PAF mode
  //----------------------------------------------------------------------------
  cout << endl; 
  PAFIExecutionEnvironment* pafmode = 0;
  if (nSlots <=1 ) {
    PAF_INFO("RunTree_ReReco", "Sequential mode chosen");
    pafmode = new PAFSequentialEnvironment();
  }
  else if (nSlots <=64) {
    PAF_INFO("RunTree_ReReco", "PROOF Lite mode chosen");
    pafmode = new PAFPROOFLiteEnvironment(nSlots);
  }
  else {
    PAF_INFO("RunTree_ReReco", "PoD mode chosen");
    pafmode = new PAFPoDEnvironment(nSlots);
  }

  // Create PAF Project whith that environment
  //----------------------------------------------------------------------------
  PAFProject* myProject = new PAFProject(pafmode);

  // Base path to input files
  //----------------------------------------------------------------------------
  TString dataPath = "/pool/ciencias/";

  // INPUT DATA SAMPLE
  //----------------------------------------------------------------------------
  TString userhome = "/mnt_pool/fanae105/user/$USER/";
  DatasetManager* dm = DatasetManager::GetInstance();
  dm->SetTab("DR80XasymptoticMiniAODv2_v2");
  //dm->SetTab("DR80XasymptoticMiniAODv2");

  // Deal with data samples
  if(sampleName == "DoubleEG" || sampleName == "DoubleMuon" || sampleName == "MuonEG" || sampleName.BeginsWith("Single")){
    cout << "   + Data..." << endl;
    TString datasuffix[] = { 
      //"Run2016B_PromptReco_v2", // 5.86
      //"Run2016C_PromptReco_v2", // 2.64
      //"Run2016D_PromptReco_v2", // 4.35
      //"Run2016G_PromptReco_v1", // 4.39
      "Run2016B_23Sep2016", // 5.86
      "Run2016C_23Sep2016", // 2.64
      "Run2016D_23Sep2016", // 4.35
      "Run2016E_23Sep2016",
      "Run2016F_23Sep2016",
      "Run2016G_23Sep2016",
      "Run2016H_PromptReco_v2",
      "Run2016H_PromptReco_v3",
    };
    const unsigned int nDataSamples = 8;
    for(unsigned int i = 0; i < nDataSamples; i++) {
      TString asample = Form("Tree_%s_%s",sampleName.Data(), datasuffix[i].Data());
      cout << "   + Looking for " << asample << " trees..." << endl;
      myProject->AddDataFiles(dm->GetRealDataFiles(asample));
    }
    G_Event_Weight = 1.;
    G_IsData = true;
  }
  else{ // Deal with MC samples
    G_IsData = false; //true;  // only for pseudodata
    dm->LoadDataset(sampleName);
    if(sampleName != "TestHeppy" && !sampleName.Contains("T2tt"))   myProject->AddDataFiles(dm->GetFiles());
    if(
	    sampleName == "TTWToLNu"  || 
	    sampleName == "TTWToQQ" || 
	    sampleName == "TTZToQQ" || 
	    sampleName == "WWZ" || 
	    sampleName == "WZZ" || 
			sampleName == "ZZZ" ||
			sampleName.Contains("aMCatNLO") ||
			sampleName.Contains("amcatnlo") ){
			G_Event_Weight = dm->GetCrossSection() / dm->GetSumWeights();
			cout << endl;
      cout << " weightSum(MC@NLO) = " << dm->GetSumWeights()     << endl;
    }
		else if(sampleName.BeginsWith("T2tt_mStop")){
			TString lp = "/pool/ciencias/HeppyTreesDR80X/v1/";
			cout << "Analyzing Stop Dilep sample" << endl;
			G_Event_Weight = SusyWeight;
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_0.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_1.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_2.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_3.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_4.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_5.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_6.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_7.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_8.root");
			myProject->AddDataFile(lp + "Tree_" + sampleName + "_9.root");
			sampleName = Form("T2ttDilep_mStop%i_mLsp%i",stopMass, lspMass);
		}
    else if(sampleName.BeginsWith("T2tt")){
      TString lp = "/pool/ciencias/HeppyTreesDR80X/v1/";
      cout << "Analyzing Stop sample" << endl;
      G_Event_Weight = SusyWeight;
      myProject->AddDataFile(lp + "Tree_" + sampleName + "_0.root");
      myProject->AddDataFile(lp + "Tree_" + sampleName + "_1.root");
			sampleName = Form("T2tt_mStop%i_mLsp%i",stopMass, lspMass);
		}
		else if(sampleName == "TestHeppy"){
			TString localpath="/nfs/fanae/user/juanr/tempTrees/";
			TString sample = "treeLooseElecs.root";
			myProject->AddDataFile(localpath + sample);
			G_Event_Weight = 1;
		}
		else {       
			G_Event_Weight = dm->GetCrossSection() / dm->GetEventsInTheSample();
		}
    
    if(nEvents == 0) nEvents = dm->GetEventsInTheSample();

    cout << endl;
    cout << " #===============================================" << endl;
    cout << " #     sampleName = " << sampleName                 << endl;
		cout << " #	     x-section = " << dm->GetCrossSection()      << endl;
		cout << " #	       nevents = " << dm->GetEventsInTheSample() << endl;
    cout << " # base file name = " << dm->GetBaseFileName()      << endl;
		cout << " #	        weight = " << G_Event_Weight	           << endl;
		cout << " #	        isData = " << G_IsData	                 << endl;
    cout << " #===============================================" << endl;
		cout << endl;
  }
  
	// Output file name
  //----------------------------------------------------------------------------
	TString outputDir = "./temp";
  if(sampleName.BeginsWith("T2tt")) outputDir += "/Susy";
	gSystem->mkdir(outputDir, kTRUE);
  TString outputFile = outputDir + "/Tree_" + sampleName + ".root";

  if(outputFile.Contains("_ext2")) outputFile.ReplaceAll("_ext2",""); 
  if(outputFile.Contains("_ext"))  outputFile.ReplaceAll("_ext",""); 

  PAF_INFO("RunTree_ReReco", Form("Output file = %s", outputFile.Data()));
  myProject->SetOutputFile(outputFile);

  if(sampleName.Contains("aMCatNLO") || sampleName.Contains("amcatnlo") ||
     sampleName == "TTWToLNu"       || sampleName == "TTWToQQ"          || 
     sampleName == "TTZToQQ"        || sampleName == "WWZ"              || 
     sampleName == "WZZ"            || sampleName == "ZZZ"           ){
    PAF_INFO("RunTree_ReReco", "This is a MC@NLO sample!");
    G_IsMCatNLO = true;
  }

  // Parameters for the analysis
  //----------------------------------------------------------------------------
  myProject->SetInputParam("sampleName",    sampleName       );
  myProject->SetInputParam("IsData",        G_IsData         );
  myProject->SetInputParam("weight",        G_Event_Weight   );
  myProject->SetInputParam("stopMass"     , stopMass         );
  myProject->SetInputParam("lspMass"      , lspMass          );
  myProject->SetInputParam("IsMCatNLO"    , G_IsMCatNLO      );  
  myProject->SetInputParam("iSelection"   , 1); // enum...

  if(nEvents != 0) myProject->SetNEvents(nEvents);

  // Name of analysis class
  //----------------------------------------------------------------------------
  myProject->AddSelectorPackage("LeptonSelector");
  //myProject->AddSelectorPackage("JetSelector");

  // Additional packages
  //----------------------------------------------------------------------------
  myProject->AddPackage("Lepton");
  myProject->AddPackage("LeptonSF");
  //myProject->AddPackage("Jet");
  //myProject->AddPackage("BTagSFUtil");
  //myProject->AddPackage("PUWeight");
  //myProject->AddPackage("mt2");

  // Let's rock!
  //----------------------------------------------------------------------------
  myProject->Run();
}

