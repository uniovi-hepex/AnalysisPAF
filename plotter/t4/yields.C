R__LOAD_LIBRARY(plotter/Histo.C+)
R__LOAD_LIBRARY(plotter/Looper.C+)
R__LOAD_LIBRARY(plotter/Plot.C+)
R__LOAD_LIBRARY(plotter/TResultsTable.C+)
R__LOAD_LIBRARY(plotter/CrossSection.C+)
#include "../Histo.h"
#include "../Looper.h"
#include "../Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");

TString NameOfTree = "tree";

TString alwaystrue("1");
//TString presel("TMET>50 && TMT2>0 && isSS && TNVetLeps < 3");
TString presel("TMET>50 && TIsSS && TNVetLeps < 3"); // MT2 falta de los trees
TString SRs(presel+" && TNBtags>1");
TString CRs(presel+" && TNBtags <2 && TNJets>0");

//TNTaus    : TNTaus/I                                               *
//TTau_Pt   : TTau_Pt[TNTaus]/F                                      *
//TTau_Eta  : TTau_Eta[TNTaus]/F                                     *
//TTau_Charge : TTau_Charge[TNTaus]/F                                *
//TTau_DecayMode : TTau_DecayMode[TNTaus]/F                          *
//TTau_IdDecayMode : TTau_IdDecayMode[TNTaus]/F                      *
//TTau_IdMVA : TTau_IdMVA[TNTaus]/F                                  *
//TTau_IdAntiE : TTau_IdAntiE[TNTaus]/F                              *
//TTau_IdAntiMu : TTau_IdAntiMu[TNTaus]/F                            *
//TNFakeableLeps : TNFakeableLeps/I                                  *
//TNSelLeps : TNSelLeps/I                                            *
//TLep_Pt   : TLep_Pt[TNSelLeps]/F                                   *
//TLep_Eta  : TLep_Eta[TNSelLeps]/F                                  *
//TLep_Phi  : TLep_Phi[TNSelLeps]/F                                  *
//TLep_E    : TLep_E[TNSelLeps]/F                                    *
//TLep_Charge : TLep_Charge[TNSelLeps]/F                             *
//TFLep_Pt  : TFLep_Pt[TNFakeableLeps]/F                             *
//TFLep_Eta : TFLep_Eta[TNFakeableLeps]/F                            *
//TFLep_Phi : TFLep_Phi[TNFakeableLeps]/F                            *
//TFLep_E   : TFLep_E[TNFakeableLeps]/F                              *
//TFLep_Charge : TFLep_Charge[TNFakeableLeps]/F                      *
//TChannel  : TChannel/I                                             *
//TMll      : TMll/F                                                 *
//TM3l      : TM3l/F                                                 *
//TMZ       : TMZ/F                                                  *
//TNJets    : TNJets/I                                               *
//TNBtags   : TNBtags/I                                              *
//TJet_isBJet : TJet_isBJet[TNJets]/I                                *
//TJet_Pt   : TJet_Pt[TNJets]/F                                      *
//TJet_Eta  : TJet_Eta[TNJets]/F                                     *
//TJet_Phi  : TJet_Phi[TNJets]/F                                     *
//TJet_E    : TJet_E[TNJets]/F                                       *
//THT       : THT/F                                                  *
//TWeight   : TWeight/F                                              *
//TMET      : TMET/F                                                 *
//TMET_Phi  : TMET_Phi/F                                             *
//TIsOnZ    : TIsOnZ/I                                               *

TString pathToTree("./tttt_temp/");


void yields(TString plotsFolder=""){
  Plot* p = new Plot("TChannel", alwaystrue, "1", 1, 0, 10, "Channel", "xsec");
  TString username(gSystem->GetUserInfo(gSystem->GetUid())->fUser);
  if(username=="vischia") pathToTree="/pool/ciencias/userstorage/pietro/tttt/2l_skim_wmt2/tttt_temp/";
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
  p->SetPlotFolder(plotsFolder);
  //p->doData = false;
  //p->doStackSignal = true;

  // Diboson
  p->AddSample("ZZTo4L"   , "VV non-WZ", itBkg, kYellow-10, 0.50);
  p->AddSample("WWTo2L2Nu", "VV non-WZ", itBkg);  
  p->AddSample("WpWpJJ"   , "VV non-WZ", itBkg);
  
  // Triboson
  p->AddSample("ZZZ", "VVV", itBkg);
  p->AddSample("WZZ", "VVV", itBkg);
  p->AddSample("WWZ", "VVV", itBkg);
  p->AddSample("WWW", "VVV", itBkg);
  
  // ttV
  p->AddSample("TTZToLLNuNu1"  , "ttV", itBkg, kOrange-3, 0.5);
  p->AddSample("TTWToLNu1"     , "ttV", itBkg);
  p->AddSample("TTZToLL_M1to10", "ttV", itBkg);

  // ttH/tZq
  p->AddSample("tZq_ll",   "tZq", itBkg, kOrange-1, 0.5);
  //p->AddSample("TTHNonbb", "ttH/tZq", itBkg); // Must run from jet25, but need jet15

  // WZ
  p->AddSample("WZTo3LNu",     "WZ", itBkg);

  // tW
  p->AddSample("T_tch"                  ,"t/tW", itBkg, kMagenta, 0.2);
  p->AddSample("Tbar_tch"               ,"t/tW", itBkg);
  p->AddSample("TToLeptons_sch_amcatnlo","t/tW", itBkg);
  p->AddSample("TW_noFullyHadr"         ,"t/tW", itBkg);
  p->AddSample("TbarW_noFullyHadr"      ,"t/tW", itBkg);
  
  // tt
  p->AddSample("TTbar_Powheg", "tt", itBkg, kRed+1, 0.05);
  //p->AddSample("TTbar_Powheg_Semi", "NonWZ", itBkg, kGreen-2, 0.50);  

  // Conversions
  p->AddSample("TGJets"      , "Conversions", itBkg, kBlue-1, 0.5);
  p->AddSample("TTGJets"     , "Conversions", itBkg);
  p->AddSample("WGToLNuG"    , "Conversions", itBkg);
  p->AddSample("ZGTo2LG"     , "Conversions", itBkg);
  p->AddSample("WZG_amcatnlo", "Conversions", itBkg);
  p->AddSample("WWG_amcatnlo", "Conversions", itBkg);

  // V+jets
  p->AddSample("WJetsToLNu_MLM"       , "WJets", itBkg, kAzure-4, 0.50);
  p->AddSample("DYJetsToLL_M50_MLM"   , "DY"   , itBkg, kAzure-8, 0.50);
  p->AddSample("DYJetsToLL_M5to50_MLM", "DY"   , itBkg);
  // Substitute with DY/WJets  aMCatNLO?
  //p->AddSample("WJetsToLNu_aMCatNLO", "WJets", itBkg, kAzure-4, 0.50);
  //p->AddSample("DYJetsToLL_M50_aMCatNLO", "DY", itBkg, kAzure-8, 0.50);
  //p->AddSample("DYJetsToLL_M10to50_aMCatNLO",     "DY", itBkg);

  // Signal
  p->AddSample("TTTT"   , "TTTT", itSignal, kRed, 0.10);

  p->AddSample("DoubleEG"  , "Data", itData);
  p->AddSample("DoubleMuon", "Data", itData);
  p->AddSample("MuonEG"    , "Data", itData);
  //// p->AddSample("SingleMuon", "Data", itData);
  //// p->AddSample("SingleElec", "Data", itData);


/*
  p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, 0, "ueUp");
  p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, 0, "ueDown");
  p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, 0, "isrUp");
  p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, 0, "isrDown");
  p->AddSample("TTbar_Powheg_fsrUp", "ttbar", itSys, 1, 0, "fsrUp");
  p->AddSample("TTbar_Powheg_fsrDown", "ttbar", itSys, 1, 0, "fsrDown");
  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp");
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "nloDown");
  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp");
  p->AddSample("TTbar_PowhegLHE", "ttbar", itSys, 1, 0, "hadDown");
*/


  p->AddSystematic("stat");
  // p->AddSystematic("JES");
  //p->PrintYields(dilepton + ", " + jets2 + ", " + btag1, "Dilepton, 2jets, 1btag", "ElMu, ElMu, ElMu");
  //p->PrintYields("", "", "", "html");
  p->PrintYields(presel + ", " + SRs + ", " + CRs, "Preselection, Signal Regions, Control Regions", "   1,    1,    1", "html");
  p->PrintYields(presel + ", " + SRs + ", " + CRs, "Preselection, Signal Regions, Control Regions", "ElMu, ElMu, ElMu", "html");
  p->PrintYields(presel + ", " + SRs + ", " + CRs, "Preselection, Signal Regions, Control Regions", "MuMu, MuMu, MuMu", "html");
  p->PrintYields(presel + ", " + SRs + ", " + CRs, "Preselection, Signal Regions, Control Regions", "ElEl, ElEl, ElEl", "html");


  //CrossSection *x = new CrossSection(p, "ttbar");
  //x->SetTheoXsec(831.8);
  //x->SetChannelTag("ElMu");
  //x->SetLevelTag(">2jets");
  //x->PrintSystematicTable();
  //x->SetBR();


  //p->ScaleProcess("VV", 2.);
  //p->PrintYields();

  //p->PrintSamples();
  //p->PrintSystYields();
  //p->PrintYields("TMET > 50, TMET > 50, TMET > 50", "ElMu, Elec, Muon", "ElMu, Elec, Muon");
  //p->DrawStack("prueba", 1);
 
  //Plot* k = p->NewPlot("TMET", "TMET > 200");
  //k->PrintYields();
  //k->PrintSamples();
  //k->PrintSystYields();

  //delete p;
}

