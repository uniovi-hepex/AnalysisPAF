R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(CrossSection.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "GetDataDriven.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString NameOfTree = "MiniTree";
TString VarNBtagsNJets = "TNBtags + (TNJets == 1) + (TNJets == 2)*3 + (TNJets == 3)*6 + (TNJets >= 4)*10";

//TString dilepton = "TLep_Charge[0]*TLep_Charge[1] < 0";
//TString jets2    = "TLep_Charge[0]*TLep_Charge[1] < 0 && TNJets >= 2";
//TString btag1    = "TLep_Charge[0]*TLep_Charge[1] < 0 && TNJets >= 2 && TNBtags > 0";
TString dilepton = "!TIsSS";
TString jets1    = "!TIsSS && TNJets >= 1";
TString jets2    = "!TIsSS && TNJets >= 2";
TString btag1    = "!TIsSS && TNJets >= 2 && TNBtags > 0";
TString btag1jets0 = "";

TString dileptonSS = "TIsSS";
TString jets2SS    = "TIsSS && TNJets >= 2";
TString btag1SS    = "TIsSS && TNJets >= 2 && TNBtags > 0";

// Baseline

//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/may10/"; // For AN 18 may
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/jul25/";
TString pathToTree = "/pool/ciencias/userstorage/juanr/top/nov15/";
void xsec(){

  Float_t DYSF         = GetDYDD(  "ElMu", "1btag");
  Float_t NonWZ_DD     = GetNonWDD("ElMu", "1btag");
  Float_t DYSF_err     = GetDYDD(  "ElMu", "1btag", true)/DYSF;
  Float_t NonWZ_DD_err = GetNonWDD("ElMu", "1btag", true)/NonWZ_DD;

  Plot* p = new Plot("TChannel", btag1, "ElMu", 1, 0, 10, "Channel", "xsec");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = true;
  //p->doData = false;
  //p->doStackSignal = true;

  p->AddSample("WZ, WW, ZZ",                                            "VV",    itBkg, kYellow-10);
  //p->AddSample("TTbar_Powheg_Semilep, WJetsToLNu_aMCatNLO",             "NonWZ", itBkg, kGreen+1,   0.30);
  p->AddSample("TTbar_Powheg_Semilep, WJetsToLNu_MLM",                  "NonWZ", itBkg, kGreen+1);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",               "ttV",   itBkg, kOrange-3);
	p->AddSample("TW, TbarW",                                             "tW",    itBkg, kMagenta);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",                               "DY",    itBkg, kAzure-8);
  //p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM,",            "DY",    itBkg, kAzure-8);
  p->AddSample("TTbar_Powheg",                                          "ttbar", itBkg, kRed+1, "ElecEff,MuonEff,Trig,JES,Btag,MisTag,PU");
//TTbar_Powheg_mtop1755
//TTbar_Powheg_mtop1665
//TTbar_Powheg_mtop1695
//TTbar_Powheg_mtop1715
//TTbar_Powheg_mtop1735

  p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",  "Data",  itData);

  p->AddSample("TTbar_Powheg_ueUp",      "ttbar", itSys, 1, "ueUp"     );
  p->AddSample("TTbar_Powheg_ueDown",    "ttbar", itSys, 1, "ueDown"   );
  p->AddSample("TTbar_Powheg_hdampUp",   "ttbar", itSys, 1, "hdampUp"  );
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");
  p->AddSample("TTbar_Powheg_isrUp",     "ttbar", itSys, 1, "isrUp"    );
  p->AddSample("TTbar_Powheg_isrDown",   "ttbar", itSys, 1, "isrDown"  );

  TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
  p->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
  p->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
  p->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
  p->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );

  p->SetWeight("TWeight");
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleUp"    );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleDown"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfUp"      );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfDown"    );
  
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "JERUp"      );
//  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "JERDown"    );


  p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
  p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
  p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
  p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");

  p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");


  p->AddSystematic("stat");
  //p->AddSystematic("ElecEff,MuonEff,Trig,JES,Btag,MisTag,PU", "ttbar");
  p->SetSignalStyle("xsec");
  p->SetTableFormats("2.4");

//  p->PrintSystematics();
//  p->PrintSystYields();
//  p->PrintYields("", "", "", "");
//  p->PrintYields(dileptonSS + ", " + jets2SS + ", " + btag1SS, "Dilepton, 2jets, 1btag", "ElMu, ElMu, ElMu", "html");
  Float_t n = p->GetYield("ttbar");
  Float_t fsrUp = p->GetYield("ttbar", "fsrUp");
  Float_t fsrDo = p->GetYield("ttbar", "fsrDown");
  

  CrossSection *x = new CrossSection(p, "ttbar");
  x->SetTheoXsec(831.8);
  x->SetChannelTag("ElMu");
  x->SetLevelTag("1btag");
  x->SetBR(0.03263);
  x->SetNFiducialEvents(1.15245e+06);
  x->SetNSimulatedEvents(77229341);

  x->SetEfficiencySyst("JES, Btag, MisTag, ElecEff, MuonEff, LepEff, Trig, PU, JER");
  x->SetAcceptanceSyst("stat, ue, nlo, hdamp, scale, pdf, isr, fsr, q2, ME, CR");

  x->SwitchLabel("VV", "Dibosons");
  x->SwitchLabel("DY", "Drell-Yan");
  x->SwitchLabel("ue", "Underlying Event");
  x->SwitchLabel("fsr", "FSR scale");
  x->SwitchLabel("isr", "ISR scale");
  x->SwitchLabel("hdamp", "ME/PS matching (hdamp)");
  x->SwitchLabel("nlo", "NLO (Powheg vs aMCatNLO)");
  x->SwitchLabel("had", "Had. (Pythia8 vs Herwig++)");
  x->SwitchLabel("JES", "Jet Energy Scale");
  x->SwitchLabel("JER", "Jet Energy Resolution");
  x->SwitchLabel("Btag", "b-tagging efficiency");
  x->SwitchLabel("MisTag", "Mistagging efficiency");
  x->SwitchLabel("ElecEff", "Electron efficiencies");
  x->SwitchLabel("MuonEff", "Muon efficiencies");
  x->SwitchLabel("Trig", "Trigger efficiencies");
  x->SwitchLabel("PU", "Pile-Up");
  x->SwitchLabel("Scale", "ME scales");
  x->SwitchLabel("pdf", "PDF");
  x->SwitchLabel("stat", "MC stat");
  x->SwitchLabel("CR", "Color reconnection");

  Float_t y = x->GetYield("ttbar");
  // Scale FSR unc by sqrt
  Float_t diff = TMath::Abs(x->GetYield("ttbar") - x->GetUnc("FSR scale"));
  x->SetUnc("FSR scale", x->GetYield("ttbar")-diff/TMath::Sqrt(2));
  //x->SetUnc("FSR scale", x->GetYield("ttbar"));

  x->SetUnc("PDF", y- y*0.006); 
  x->SetUnc("ME scales", y- y*0.0014); 

  x->SetUnc("Drell-Yan", 0.15);
  x->SetUnc("NonWZ",     0.30);
  x->SetUnc("Dibosons",  0.30);
  x->SetUnc("tW",        0.30);
  x->SetUnc("ttV",       0.30);

  // Adding 0.5, 0.5, 1.0 muon efficiencies
  Float_t stat =  TMath::Abs(x->GetUnc("Muon efficiencies") - y)/y;
  Float_t MuonUnc = TMath::Sqrt(stat*stat + 0.005*0.005 + 0.005*0.005 + 0.01*0.01);
  MuonUnc = MuonUnc*y + y;
  x->SetUnc("Muon efficiencies",  MuonUnc);
  x->SetMembers();

  x->SetOutputFolder("./top/xsec/");

  // Cross section from MC.............................................
  x->SetTableName("xsec_unc_MC");
  x->SetYieldsTableName("yields_MC");
  x->SetXsecTableName("xsec_MC");
  x->PrintSystematicTable("html,tex,txt");
  x->PrintYieldsTable("html,tex,txt");
  x->PrintCrossSection("html");

  // Data driven........................................................
  x->SetYield("Drell-Yan", x->GetYield("Drell-Yan")*DYSF); 
  x->SetYield("NonWZ", NonWZ_DD); 
  Float_t DYunc   = x->GetUnc("Drell-Yan");
  Float_t NonWunc = x->GetUnc("NonWZ");
  DYunc   = TMath::Sqrt(DYunc*DYunc     + DYSF_err*DYSF_err);
  NonWunc = TMath::Sqrt(NonWunc*NonWunc + NonWZ_DD_err*NonWZ_DD_err);
  x->SetUnc("Drell-Yan", DYunc  );
  x->SetUnc("NonWZ",     NonWunc);

  x->SetMembers();

  x->SetTableName("xsec_unc_DD");
  x->SetYieldsTableName("yields_DD");
  x->SetXsecTableName("xsec_DD");

  x->PrintSystematicTable("html,tex,txt");
  x->PrintYieldsTable("html,tex,txt");
  x->PrintCrossSection("html,tex,txt");

}

