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
TString SScut = "TMT2 > 0 && TMET > 50 && TNJets > 1 && TNBtags > 0 && TIsSS && TNVetoLeps < 3";
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

TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/may10/";
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
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToQQ",                   "ttV",   itBkg, kOrange-3);
	p->AddSample("TW, TbarW",                                             "tW",    itBkg, kMagenta);
  p->AddSample("DYJetsToLL_M50_aMCatNLO",                               "DY",    itBkg, kAzure-8);
  //p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM,",            "DY",    itBkg, kAzure-8);
  p->AddSample("TTbar_Powheg",                                          "ttbar", itBkg, kRed+1);
  p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",  "Data",  itData);

  p->AddSample("TTbar_Powheg_ueUp",      "ttbar", itSys, 1, "ueUp"     );
  p->AddSample("TTbar_Powheg_ueDown",    "ttbar", itSys, 1, "ueDown"   );
  p->AddSample("TTbar_Powheg_hdampUp",   "ttbar", itSys, 1, "hdampUp"  );
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");
  p->AddSample("TTbar_Powheg_isrUp",     "ttbar", itSys, 1, "isrUp"    );
  p->AddSample("TTbar_Powheg_isrDown",   "ttbar", itSys, 1, "isrDown"  );
  p->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
  p->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleUp"    );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ScaleDown"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfUp"      );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "pdfDown"    );
  
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "JERUp"      );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "JERDown"    );

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ElecEffUp"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "ElecEffDown");

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "MuonEffUp"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "MuonEffDown");

//  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp");
//  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "nloDown");
//  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp");
//  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "hadDown");

  p->AddSystematic("JES,Btag,MisTag,PU,stat");
  p->SetSignalStyle("xsec");
  p->SetTableFormats("2.4");

//  p->PrintSystematics();
//  p->PrintSystYields();
//  p->PrintYields("", "", "", "");
//  p->PrintYields(dileptonSS + ", " + jets2SS + ", " + btag1SS, "Dilepton, 2jets, 1btag", "ElMu, ElMu, ElMu", "html");

  CrossSection *x = new CrossSection(p, "ttbar");
  x->SetTheoXsec(831.8);
  x->SetChannelTag("ElMu");
  x->SetLevelTag("1btag");

  x->SetEfficiencySyst("JES, Btag, MisTag, ElecEff, MuonEff, LepEff, Trig, PU, JER");
  x->SetAcceptanceSyst("stat, ue, nlo, hdamp, scale, pdf, isr, fsr, q2, ME");

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

  // Scale FSR unc by sqrt
  Float_t diff = TMath::Abs(x->GetYield("ttbar") - x->GetUnc("FSR scale"));
  x->SetUnc("FSR scale", x->GetYield("ttbar")-diff/TMath::Sqrt(2));
  x->SetUnc("FSR scale", x->GetYield("ttbar"));
  x->SetUnc("Drell-Yan", 0.15);
  x->SetUnc("NonWZ",     0.30);
  x->SetUnc("Dibosons",  0.30);
  x->SetUnc("tW",        0.30);
  x->SetUnc("ttV",       0.30);
  x->SetMembers();

  x->SetOutputFolder("./top/xsec/");

  x->SetTableName("xsec_unc_MC");
  x->SetYieldsTableName("yields_MC");
  x->SetXsecTableName("xsec_MC");

  x->PrintSystematicTable("html,tex,txt");
  x->PrintYieldsTable("html,tex,txt");
  x->PrintCrossSection("html");

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

  //x->SetBR();

  //p->ScaleProcess("DY", DYSF);
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

