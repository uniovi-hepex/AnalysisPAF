R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName="", TString cutName="", TString outFolder="", Float_t yMax=2000, Float_t yMin=1);
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* binN, TString Xtitle, TString varName="", TString cutName="", TString outFolder="", Float_t yMax=2000, Float_t yMin=1);
TString NameOfTree = "medium"; //"nolepMVA", "medium", "top"
bool doMC = false;
TString pathToTree = "";

bool doSync = false;
bool doLog;
// Baseline
//TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && !TIsOnZ";
//TString CRZ      = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && TNTaus == 0 && TIsOnZ";

TString AR           = "TIsSR == 1 && TNTightLeps < 3";
TString SR           = "TIsSR == 1 && TNTightLeps == 3";
TString SRmmm        = "TIsSR == 1 && TNTightLeps == 3 && TChannel == 3";
TString SRmme        = "TIsSR == 1 && TNTightLeps == 3 && TChannel == 2";
TString SRmee        = "TIsSR == 1 && TNTightLeps == 3 && TChannel == 1";
TString SReee        = "TIsSR == 1 && TNTightLeps == 3 && TChannel == 0";


TString CRDY      = "TMath::Abs(TMll - 91.1876)< 15. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30";
TString CRTT      = "TMath::Abs(TMll - 91.1876)> 5. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags > 0 && TMET > 30";

TString CRDYmmm      = "TMath::Abs(TMll - 91.1876)< 15. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30 && TChannel == 3";
TString CRTTmmm      = "TMath::Abs(TMll - 91.1876)> 5. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags > 0 && TMET > 30 && TChannel == 3";
TString CRDYmme      = "TMath::Abs(TMll - 91.1876)< 15. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30 && TChannel == 2";
TString CRTTmme      = "TMath::Abs(TMll - 91.1876)> 5. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags > 0 && TMET > 30 && TChannel == 2";
TString CRDYmee      = "TMath::Abs(TMll - 91.1876)< 15. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30 && TChannel == 1";
TString CRTTmee      = "TMath::Abs(TMll - 91.1876)> 5. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags > 0 && TMET > 30 && TChannel == 1";
TString CRDYeee      = "TMath::Abs(TMll - 91.1876)< 15. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30 && TChannel == 0";
TString CRTTeee      = "TMath::Abs(TMll - 91.1876)> 5. && TNTightLeps == 3 && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20 && TM3l > 100 && TMinMll > 4 && TNBtags > 0 && TMET > 30 && TChannel == 0";

TString newCRDY      = "TIsNewCRDY == 1 && TNTightLeps == 3";
TString newCRTT      = "TIsNewCRTT == 1 && TNTightLeps == 3";

TString SRpt20        = "TIsSR == 1 && TNTightLeps == 3 && TLep_PtZ2 > 20";
TString CRDYpt20      = "TIsCRDY == 1 && TNTightLeps == 3 && TLep_PtZ2 > 20";
TString CRTTpt20      = "TIsCRTT == 1 && TNTightLeps == 3 && TLep_PtZ2 > 20";
TString newCRDYpt20      = "TIsNewCRDY == 1 && TNTightLeps == 3 && TLep_PtZ2 > 20";
TString newCRTTpt20      = "TIsNewCRTT == 1 && TNTightLeps == 3 && TLep_PtZ2 > 20";
Float_t yMax;
Float_t yMin;

void DrawPlots(TString cutName, TString treeName = ""){
  
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="carlosec") pathToTree ="/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/";
//  if(username=="carlosec") pathToTree ="/pool/ciencias/userstorage/carlosec/WZTrees_20_09_2017/"; //Testing the plotter
  if(treeName != "") NameOfTree = treeName;
  if(treeName == "veryTightMC"){
    NameOfTree = "veryTight";
    doMC = true;
  }

  TString cut;
  if     (cutName == "SR" ){ cut = SR ; yMax = 1200; yMin = 1;}
  else if(cutName == "CRDY" ){ cut = CRDY ; yMax = 600; yMin = 1;}
  else if(cutName == "CRTT" ){ cut = CRTT ; yMax = 120; yMin = 1;}
  else if(cutName == "CRDYmmm" ){ cut = CRDYmmm ; yMax = 300; yMin = 1;}
  else if(cutName == "CRTTmmm" ){ cut = CRTTmmm ; yMax = 60; yMin = 1;}
  else if(cutName == "CRDYmme" ){ cut = CRDYmme ; yMax = 300; yMin = 1;}
  else if(cutName == "CRTTmme" ){ cut = CRTTmme ; yMax = 60; yMin = 1;}
  else if(cutName == "CRDYmee" ){ cut = CRDYmee ; yMax = 200; yMin = 1;}
  else if(cutName == "CRTTmee" ){ cut = CRTTmee ; yMax = 30; yMin = 1;}
  else if(cutName == "CRDYeee" ){ cut = CRDYeee ; yMax = 150; yMin = 1;}
  else if(cutName == "CRTTeee" ){ cut = CRTTeee ; yMax = 30; yMin = 1;}
  else if(cutName == "NEWCRDY" ){ cut = newCRDY ; yMax = 1000; yMin = 1;}
  else if(cutName == "NEWCRTT" ){ cut = newCRTT ; yMax = 1000; yMin = 1;}
  else if(cutName == "SR20" ){ cut = SRpt20 ; yMax = 1000; yMin = 1;}
  else if(cutName == "CRDY20" ){ cut = CRDYpt20 ; yMax = 1000; yMin = 1;}
  else if(cutName == "CRTT20" ){ cut = CRTTpt20 ; yMax = 1000; yMin = 1;}
  else if(cutName == "NEWCRDY20" ){ cut = newCRDYpt20 ; yMax = 1000; yMin = 1;}
  else if(cutName == "NEWCRTT20" ){ cut = newCRTTpt20 ; yMax = 1000; yMin = 1;}
  else if(cutName == "SRmmm" ){ cut = SRmmm ; yMax = 300; yMin = 1;}
  else if(cutName == "SReee" ){ cut = SReee ; yMax = 200; yMin = 1;}
  else if(cutName == "SRmme" ){ cut = SRmme ; yMax = 350; yMin = 1;}
  else if(cutName == "SRmee" ){ cut = SRmee ; yMax = 200; yMin = 1;}
  else if (cutName == "AR"){
    cut = AR;
    doMC = true;
    yMax = 1200;
    yMin = 1;
  }
  else {cout << "Wrong name!!" << endl; return;}

//  DrawPlot("TM3l",  cut, "All", 21, 80, 300, "M_{3l} [GeV]", cutName);

  doLog = true;
//  DrawPlot("TMET",  cut, "All", 11, 30, 140, "E_{T}^{miss} [GeV]", "TMET_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TMET",  cut, "All", 11, 30, 140, "E_{T}^{miss} [GeV]", "TMET_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/temp/", yMax, yMin);
//  DrawPlot("TM3l",  cut, "All", 20, 100,300, "M_{3l} [GeV]", "TM3l_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TMET",  cut, "All", 39, 30, 420, "E_{T}^{miss} [GeV]", cutName);
//  Float_t M3lBins[] = {0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000};
//  DrawPlot("TM3l",  cut, "All", 13, M3lBins, "M_{3l} [GeV]", cutName);
//  DrawPlot("TMtWZ",  cut, "All", 13, M3lBins, "M_{WZ} [GeV]", cutName);
//  DrawPlot("TMtW",  cut, "All", 10 , 0 , 200 , "M_{T}^{W} [GeV]", "TMtW_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TMll",  cut, "All", 8, 70 , 110 , "M^{Z} [GeV]", "TMll_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtZ1",  cut, "All", 10,0 , 200 , "p_{T}^{lepZ1} [GeV]", "TLep_PtZ1_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtZ2",  cut, "All", 10,0 , 200 , "p_{T}^{lepZ2} [GeV]", "TLep_PtZ2_log",  cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtW",  cut, "All", 10,0 , 200 , "p_{T}^{lepW} [GeV]", "TLep_PtW_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);

//  DrawPlot("TLep_EtaZ1",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepZ1} [GeV]", "TLep_EtaZ1_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_EtaZ2",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepZ2} [GeV]", "TLep_EtaZ2_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_EtaW",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepW} [GeV]", "TLep_EtaW_log", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);

    doLog = false;
//  DrawPlot("TMET",  cut, "All", 11, 30, 140, "E_{T}^{miss} [GeV]", "TMET", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//    DrawPlot("TM3l",  cut, "All", 20, 100,300, "M_{3l} [GeV]", "TM3l", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TMET",  cut, "All", 39, 30, 420, "E_{T}^{miss} [GeV]", cutName);
//  Float_t M3lBins[] = {0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000};
//    DrawPlot("TM3l",  cut, "All", 13, M3lBins, "M_{3l} [GeV]", cutName);
//  DrawPlot("TMtWZ",  cut, "All", 13, M3lBins, "M_{WZ} [GeV]", cutName);
//  DrawPlot("TMtW",  cut, "All", 10 , 0 , 200 , "M_{T}^{W} [GeV]", "TMtW", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TMll",  cut, "All", 8, 70 , 110 , "M^{Z} [GeV]", "TMll", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtZ1",  cut, "All", 10,0 , 200 , "p_{T}^{lepZ1} [GeV]", "TLep_PtZ1", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtZ2",  cut, "All", 10,0 , 200 , "p_{T}^{lepZ2} [GeV]", "TLep_PtZ2",  cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_PtW",  cut, "All", 10,0 , 200 , "p_{T}^{lepW} [GeV]", "TLep_PtW", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);

//  DrawPlot("TLep_EtaZ1",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepZ1} [GeV]", "TLep_EtaZ1", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
//  DrawPlot("TLep_EtaZ2",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepZ2} [GeV]", "TLep_EtaZ2", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);
  DrawPlot("TLep_EtaW",  cut, "All", 10,-2.5 , 2.5 , "#eta^{lepW} [GeV]", "TLep_EtaW", cutName, "/mnt_pool/ciencias_users/user/carlosec/www/" + treeName + "_v2/" + cutName + "/", yMax, yMin);


  gApplication->Terminate();
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName, TString cutName, TString outFolder, Float_t yMax, Float_t yMin){
  gSystem->Exec("cp /nfs/fanae/user/carlosec/www/index.php " + outFolder);
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  std::cout << outFolder << std::endl;
  if(outFolder!="") p->SetPlotFolder(outFolder);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
  p->doData = true;
  if(varName != "") p->SetVarName(varName);
  p->SetOutputName(var);
  //std::cout  << cutName << NameOfTree<< "...............\n";
  std::cout << "_______________________________ADD HISTO M______________________________________________" << std::endl;
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_el_data_comb", "mvaMFRhistoEl"); //Book Elec Fake Rate
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_mu_data_comb", "mvaMFRhistoMu"); //Book Muon Fake Rate
  std::cout << "_______________________________ADD HISTO VT______________________________________________" << std::endl;
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpV_el_data_comb", "mvaVTFRhistoEl"); //Book Elec Fake Rate
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpV_mu_data_comb", "mvaVTFRhistoMu"); //Book Muon Fake Rate
  std::cout << "_______________________________ADD SAMPLES______________________________________________" << std::endl;
  if (NameOfTree == "top" ){//Load MC FAKES
    p->SetWeight("TWeight");
    p->SetCut(cut); //Allow Conversions
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut ); //Veto Conversions
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0"); //Add the MC fakes
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "tt + Single t", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM_0, DYJetsToLL_M50_MLM_1, DYJetsToLL_M50_MLM_2, DYJetsToLL_M50_MLM_3, WJetsToLNu_MLM", "V + Jets", itBkg, kOrange);
    p->SetCut(cut); //Add the MC fakes
  	p->AddSample("DoubleMuon, DoubleEG, MuonEG, SingleMuon, SingleElec",                     "Data",  itData);             // Data
  }

  else if (NameOfTree == "veryTight"){//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut);
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "Data",  itData);             // Data
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ  
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
    cut.ReplaceAll("TNTightLeps", "TNFOLeps");
    p->SetCut(cut); //Data fakes, relax tight to FO
    p->SetWeight("GetFRweightlepMVA3lep(GetFRweightMVAVT(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAVT(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAVT(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), 1)/35980"); //Fake rates for the fakerate god  
    //DATA FAKES
    p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "non-Pr",  itBkg, kBlue,"0", "fakes"); 
    //PROMPT SUB
    p->SetWeight("TWeight*GetFRweightlepMVA3lep(GetFRweightMVAVT(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAVT(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAVT(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), -1)"); //Fake rates for the promptsub god  
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut);
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
  }
  else {//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut);
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "Data",  itData);             // Data
    p->SetCut(cut);
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ  
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample(" tZq_ll_1, tZq_ll_2, tZq_ll_3",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    cut.ReplaceAll("TNTightLeps", "TNFOLeps");
    p->SetCut(cut); //Data fakes, relax tight to FO
    p->SetWeight("GetFRweightlepMVA3lep(GetFRweightMVAM(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAM(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAM(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), 1)/35980"); //Fake rates for the fakerate god  
    //DATA FAKES
    p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "non-Pr",  itBkg, kBlue,"0", "FakeLep"); 
    //PROMPT SUB
    p->SetWeight("TWeight*GetFRweightlepMVA3lep(GetFRweightMVAM(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAM(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAM(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), -1)"); //Fake rates for the promptsub god  
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample(" tZq_ll_1, tZq_ll_2, tZq_ll_3",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
  }
  std::cout << "_______________________________DO THE PLOTS______________________________________________" << std::endl;
  p->SetPlotMaximum(10000000*doLog + yMax);
  p->SetPlotMinimum(1*doLog+yMin);
  p->SetSignalProcess("WZ");
  //p->ScaleSignal(10);
  p->SetSignalStyle("Fill");
  p->SetRatioOptions("");
  p->SetRatioMin(0.5);
  p->SetRatioMax(1.5);

  //p->AddSystematic("stat");
  cout << "Selection = " << varName << endl;
  cout << "Corresponding to cut: " << cut << endl;
  p->PrintYields();
  p->PrintSamples();
  p->doSetLogy = doLog;
  //p->DrawStack("0", 1);
  //p->doSetLogy = true;
  p->DrawStack("log", 1);
  //p->PrintSystYields();
  delete p;
}


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins,  Float_t* binN, TString Xtitle, TString varName, TString cutName, TString outFolder){
  Plot* p = new Plot(var, cut, chan, nbins, binN, "Title", Xtitle);
  if(outFolder!="") p->SetPlotFolder(outFolder);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
//  p->doData = false;
  if(varName != "") p->SetVarName(varName);
  p->SetOutputName(var);
  std::cout  << cutName << NameOfTree<< "...............\n";
  p->SetPlotFolder("./Plots/" + NameOfTree + "/" + varName + "/");
  std::cout << "_______________________________ADD HISTO M______________________________________________" << std::endl;
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_el_data_comb", "mvaMFRhistoEl"); //Book Elec Fake Rate
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpM_mu_data_comb", "mvaMFRhistoMu"); //Book Muon Fake Rate
  std::cout << "_______________________________ADD HISTO VT______________________________________________" << std::endl;
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpV_el_data_comb", "mvaVTFRhistoEl"); //Book Elec Fake Rate
  AddFRhisto("fakerate_light_EWKino_Moriond2017_36p5fb.root", "FR_susy_wpV_mu_data_comb", "mvaVTFRhistoMu"); //Book Muon Fake Rate
  std::cout << "_______________________________ADD SAMPLES______________________________________________" << std::endl;
  if (NameOfTree == "top"){//Load MC FAKES
    p->SetCut(cut ); //Allow Conversions
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
  
    p->SetCut(cut + "&& TConvNumber == 0"); //Veto Conversions
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "Data",  itData);             // Data
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L_0, ZZTo4L_1, ZZTo4L_2, ZZTo4L_3",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut); //Add the MC fakes
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3,tZq_ll_4, tZq_ll_5, tZq_ll_6, tZq_ll_7,tZq_ll_8, tZq_ll_9", "tZq", itBkg, kRed);

    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "tt + Single t", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "V + Jets", itBkg, kOrange);
  }

  else if (NameOfTree == "veryTight"){//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut);
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "Data",  itData);             // Data
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ  
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L_0, ZZTo4L_1, ZZTo4L_2, ZZTo4L_3",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3,tZq_ll_4, tZq_ll_5, tZq_ll_6, tZq_ll_7,tZq_ll_8, tZq_ll_9",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "non-Pr", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "non-Pr", itBkg, kOrange);
    cut.ReplaceAll("TNTightLeps", "TNFOLeps");
    p->SetCut(cut + " && TConvNumber == 0"); //Data fakes, relax tight to FO
    p->SetWeight("GetFRweightlepMVA3lep(GetFRweightMVAVT(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAVT(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAVT(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), 1)"); //Fake rates for the fakerate god  
    //DATA FAKES
    p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "non-Pr",  itBkg, kBlue,"0", "fakes"); 
    //PROMPT SUB
    p->SetWeight("TWeight*GetFRweightlepMVA3lep(GetFRweightMVAVT(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAVT(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAVT(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), -1)"); //Fake rates for the promptsub god  
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L_0, ZZTo4L_1, ZZTo4L_2, ZZTo4L_3",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3,tZq_ll_4, tZq_ll_5, tZq_ll_6, tZq_ll_7,tZq_ll_8, tZq_ll_9",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "non-Pr", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "non-Pr", itBkg, kOrange);
  }
  else {//Load DATA FAKES, APPLY FAKE RATE FILES!!!
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut);
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "Data",  itData);             // Data
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ  
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo,WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L_0, ZZTo4L_1, ZZTo4L_2, ZZTo4L_3",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3,tZq_ll_4, tZq_ll_5, tZq_ll_6, tZq_ll_7,tZq_ll_8, tZq_ll_9",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "non-Pr", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "non-Pr", itBkg, kOrange);
    cut.ReplaceAll("TNTightLeps", "TNFOLeps");
    p->SetCut(cut + " && TConvNumber == 0"); //Data fakes, relax tight to FO
    p->SetWeight("GetFRweightlepMVA3lep(GetFRweightMVAM(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAM(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAM(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), 1)"); //Fake rates for the fakerate god  
    //DATA FAKES
    p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleElec, SingleMuon",                     "non-Pr",  itBkg, kBlue,"0", "fakes"); 
    //PROMPT SUB
    p->SetWeight("TWeight*GetFRweightlepMVA3lep(GetFRweightMVAM(TLep_PtZ1, TLep_EtaZ1, TLep_IsTightZ1, TLep_pdgIdZ1), GetFRweightMVAM(TLep_PtZ2, TLep_EtaZ2, TLep_IsTightZ2, TLep_pdgIdZ2), GetFRweightMVAM(TLep_PtW, TLep_EtaW, TLep_IsTightW, TLep_pdgIdW), -1)"); //Fake rates for the promptsub god  
    p->SetCut(cut);
    p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG, WZG_amcatnlo, WWG_amcatnlo",                "X+#gamma", itBkg, kViolet+2);  // X+gamma 
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("WZTo3LNu",                                                                   "WZ",       itSignal, kYellow);    // WZ
    p->AddSample("WWW, WWZ, WZZ, ZZZ, VHToNonbb_amcatnlo, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat",    "VVV/VV",  itBkg, kGreen-9); // RareSM
    p->AddSample("ZZTo4L_0, ZZTo4L_1, ZZTo4L_2, ZZTo4L_3",    "ZZ",  itBkg, kCyan-5); // RareSM
    p->AddSample("tZq_ll_0, tZq_ll_1, tZq_ll_2, tZq_ll_3,tZq_ll_4, tZq_ll_5, tZq_ll_6, tZq_ll_7,tZq_ll_8, tZq_ll_9",    "tZq",  itBkg, kGreen); // RareSM
    p->AddSample("TTZToLL_M1to10,TTZToLLNuNu,TTWToLNu,TTTT,TTHNonbb",    "ttX",  itBkg, kRed-10); // RareSM
    p->SetCut(cut + " && TConvNumber == 0");
    p->AddSample("TTbar_Powheg, TW_noFullyHadr,T_tch,Tbar_tch,TbarW_noFullyHadr,TToLeptons_sch_amcatnlo", "non-Pr", itBkg, kRed);
    p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM, WJetsToLNu_MLM", "non-Pr", itBkg, kOrange);
  }
  std::cout << "_______________________________DO THE PLOTS______________________________________________" << std::endl;

  p->SetPlotMaximum(1000000);
  p->SetSignalProcess("WZ");
  //p->ScaleSignal(10);
  p->SetSignalStyle("Fill");
  p->SetRatioOptions("");
  p->SetRatioMin(0.5);
  p->SetRatioMax(1.5);

  //p->AddSystematic("stat");
  cout << "Selection = " << varName << endl;
  cout << "Corresponding to cut: " << cut << endl;
  p->PrintYields();
  //p->PrintSamples();
  p->doSetLogy = true;
  //p->DrawStack("0", 1);
  //p->doSetLogy = true;
  p->DrawStack("log", 1);
  //p->PrintSystYields();
  delete p;
}

