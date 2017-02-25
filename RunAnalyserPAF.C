////////////////////////////////////////////////////////////////////////////////
//
//  RunAnalyserPAF("sampleName", "TheAnalysis", nSlots);
//  RunAnalyserPAF("sampleName", "TheAnalysis", nSlots, nEvents = 0, weight = 1);
//
//  To do: Automatic run on all _ext files
//
////////////////////////////////////////////////////////////////////////////////
R__LOAD_LIBRARY(DatasetManager/DatasetManager.C+)
#include "DatasetManager/DatasetManager.h"
#include "TLorentzVector.h"

Bool_t IsMCatNLO(TString sampleName);
enum sel{iStopSelec, iTopSelec, iTWSelec, iWWSelec, ittDMSelec, ittHSelec};

void RunAnalyserPAF(TString sampleName  = "TTbar_Madgraph", TString Selection = "StopDilep", Int_t nSlots = 1,
			Long64_t nEvents = 0, Float_t ThisWeight = 1.0,
			Int_t stopMass = 0, Int_t lspMass  = 0) {

  TString WorkingDir = gSystem->WorkingDirectory();

  // VARIABLES TO BE USED AS PARAMETERS...
  Float_t G_Event_Weight  = 1.0;         
  Bool_t  G_IsData        = false;       
  Bool_t  G_IsMCatNLO     = false;
  Bool_t  G_DoSystematics = false;
  Bool_t  G_IsFastSim     = false;

  // Selection
  Int_t sel = 0;
  if     (Selection == "StopDilep" || Selection == "stop"    ) sel = iStopSelec;
  else if(Selection == "Top"       || Selection == "TOP"     ) sel = iTopSelec;
  else if(Selection == "TW"        || Selection == "tW"      ) sel = iTWSelec;
  else if(Selection == "ttDM"      || Selection == "ttMET"   ) sel = ittDMSelec;
  else if(Selection == "ttH"       || Selection == "TTH"     ) sel = ittHSelec;
  else if(Selection == "WW"                                  ) sel = iWWSelec;
  else{ cout << "WRONG SELECTION!!!!" << endl; return;}
  cout << " >>>>>>>>>>>>>>>>>>>>> Analysis: " << Selection << endl;

  // PAF mode
  //----------------------------------------------------------------------------
  cout << endl; 
  PAFIExecutionEnvironment* pafmode = 0;
  if      (nSlots <=1 ) pafmode = new PAFSequentialEnvironment();
  else if (nSlots <=64) pafmode = new PAFPROOFLiteEnvironment(nSlots);
  else                  pafmode = new PAFPoDEnvironment(nSlots);
  PAFProject* myProject = new PAFProject(pafmode); // Create PAF Project whith that environment

  // INPUT DATA SAMPLE
  //----------------------------------------------------------------------------
  TString dataPath = "/pool/ciencias/"; // Base path to input files
  TString userhome = "/mnt_pool/fanae105/user/$USER/";
  DatasetManager* dm = DatasetManager::GetInstance();

  // Tab in the spreadsheet https://docs.google.com/spreadsheets/d/1b4qnWfZrimEGYc1z4dHl21-A9qyJgpqNUbhOlvCzjbE
  dm->SetTab("DR80XSummer16asymptoticMiniAODv2_2");

  // Deal with data samples
  if(sampleName == "DoubleEG" || sampleName == "DoubleMuon" || sampleName == "MuonEG" || sampleName.BeginsWith("Single")){
    cout << "   + Data..." << endl;
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
      cout << "   + Looking for " << asample << " trees..." << endl;
      myProject->AddDataFiles(dm->GetRealDataFiles(asample));
    }
    G_Event_Weight = 1.;
    G_IsData = true;
  }
  else{ // Deal with MC samples
    G_IsData = false; 
    TString theSample = "";
		if(sampleName.BeginsWith("LocalFile:")){ // LocalFile
			theSample = sampleName.ReplaceAll("LocalFile:", ""); 
			cout << " >>>>>>>>>>>>>>>>>>>>> Analysing a local sample: " << theSample << endl;
			myProject->AddDataFile(theSample);
			sampleName = TString( theSample(theSample.Last('/')+1, theSample.Sizeof())).ReplaceAll(".root", "").ReplaceAll("Tree_", "");
			G_Event_Weight = ThisWeight;
		}
		else if(sampleName.BeginsWith("T2tt:")){ // T2tt sample
			theSample = sampleName.ReplaceAll("T2tt:", "");
			cout << " >>>>>>>>>>>>>>>>>>>>> Analysing a T2tt sample: " << theSample << endl;
			myProject->AddDataFiles(dm->GetRealDataFiles(theSample));
			sampleName = Form("T2tt_mStop%i_mLsp%i",stopMass, lspMass);
			G_IsFastSim = true;
			G_Event_Weight = ThisWeight;
		} 
		else{ // Use dataset manager
			cout << " >>>>>>>>>>>>>>>>>>>>> Numbers from spreadsheet! "  << endl;
			Float_t xsec = 1; Float_t sumNorm = 1; long double totalXSec = 0; long double totalNorm = 0;
			TString sampleChain = TString(sampleName);  
			if(sampleName.Contains("&")) sampleName = TString(sampleName(0, sampleName.First('&'))); // For output file
			Int_t nFiles = sampleChain.CountChar('&') + 1;
			for(Int_t k = 0; k < nFiles; k++){
				if(sampleChain.Contains("&")){
					theSample  = TString(sampleChain(0,sampleChain.First('&')));
					sampleChain= TString(sampleChain(sampleChain.First('&')+1, sampleChain.Sizeof()));
				}
				else theSample  = sampleChain;
				dm->LoadDataset(theSample);
				myProject->AddDataFiles(dm->GetFiles()); 
				xsec    = dm->GetCrossSection();
				sumNorm = IsMCatNLO(theSample) ? dm->GetSumWeights() : dm->GetEventsInTheSample();
        cout << Form("\033[1;30m=================================================\033[0m\n",theSample.Data(), xsec, sumNorm);
        cout << Form("\033[1;32m #### Sample  = %s \033[0m\n", theSample.Data());
        cout << Form("\033[1;32m #### XSec    = %f \033[0m\n", xsec);
        cout << Form("\033[1;32m #### nEvents = %f \033[0m\n", sumNorm);
				totalNorm += sumNorm*sumNorm;
				totalXSec += sumNorm*xsec;
			}
			cout << Form("\033[1;30m=================================================\033[0m\n",theSample.Data(), xsec, sumNorm);
			if(IsMCatNLO(sampleName)){
				G_IsMCatNLO = true;
				cout << " >>>>>>>>>>>>>>>>>>>>> This is a aMCatNLO sample" << endl;
			}
			G_Event_Weight = totalXSec/totalNorm;
		}
		if(nEvents == 0) nEvents = dm->GetEventsInTheSample();
	}

	// Output file name
	//----------------------------------------------------------------------------
	TString outputDir = "./temp";
	if(sampleName.BeginsWith("T2tt")) outputDir += "/T2tt/";
	gSystem->mkdir(outputDir, kTRUE);
	TString outputFile = outputDir + "/Tree_" + sampleName + ".root";

	if(outputFile.Contains("_ext2")) outputFile.ReplaceAll("_ext2",""); 
	if(outputFile.Contains("_ext"))  outputFile.ReplaceAll("_ext",""); 

	cout << " >>>>>>>>>>>>>>>>>>>>> Output file = " << outputFile << endl;
	myProject->SetOutputFile(outputFile);

	// Parameters for the analysis
	//----------------------------------------------------------------------------
	// COMMON PARAMETERS
	myProject->SetInputParam("sampleName",    sampleName       );
	myProject->SetInputParam("IsData",        G_IsData         );
	myProject->SetInputParam("weight",        G_Event_Weight   );
	myProject->SetInputParam("IsMCatNLO",     G_IsMCatNLO      );  
	myProject->SetInputParam("iSelection",    sel              );
	myProject->SetInputParam("WorkingDir",    WorkingDir       );

	// EXTRA PARAMETERS
	myProject->SetInputParam("IsFastSim"    , G_IsFastSim);
	myProject->SetInputParam("stopMass"     , stopMass         );
	myProject->SetInputParam("lspMass"      , lspMass          );
	myProject->SetInputParam("doSyst"       , G_DoSystematics  ); 

	if(nEvents != 0) myProject->SetNEvents(nEvents);

	// Name of analysis class
	//----------------------------------------------------------------------------
	myProject->AddSelectorPackage("LeptonSelector");
	myProject->AddSelectorPackage("JetSelector");
	myProject->AddSelectorPackage("EventBuilder");
	if      (sel == 0)  myProject->AddSelectorPackage("StopAnalysis");
	else if (sel == 1)  myProject->AddSelectorPackage("StopAnalysis");
	else                myProject->AddSelectorPackage("CreateMiniTree");

	// Additional packages
	//----------------------------------------------------------------------------
	myProject->AddPackage("Lepton");
	myProject->AddPackage("Jet");
	myProject->AddPackage("mt2");
	myProject->AddPackage("Functions");
	myProject->AddPackage("LeptonSF");
	myProject->AddPackage("BTagSFUtil");
	myProject->AddPackage("PUWeight");

	// Let's rock!
	//----------------------------------------------------------------------------
	myProject->Run();
}

Bool_t IsMCatNLO(TString sampleName){
	if(	sampleName.Contains("amcatnlo")  ||
			sampleName.Contains("aMCatNLO")  || 
			sampleName.BeginsWith("TTW") || 
			sampleName.BeginsWith("TTZ") ||  
			sampleName.BeginsWith("WWZ") || 
			sampleName.BeginsWith("WZZ") || 
			sampleName.BeginsWith("ZZZ") )
		return true;
	else return false;
}
