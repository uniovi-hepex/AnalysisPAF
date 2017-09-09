R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(CrossSection.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString NameOfTree = "tree";
TString BaselineCut = "TNJets > 1 && TNBtags > 0 && !TIsSS && TNVetoLeps < 3";

TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/may22/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/mar16/Baseline/";
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/TopLikeObjects/";

void PrintSystTableTop(){
  Plot* p = new Plot("TChannel", BaselineCut, "ElMu", 1, 0, 10, "Channel", "SystTableTop");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->verbose = false;
  //p->doData = false;
  //p->doStackSignal = true;

  p->AddSample("WZ, WW, ZZ",                                            "VV",    itBkg, kYellow-10, 0.30);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToQQ",                   "ttV",   itBkg, kOrange-3,  0.30);
	p->AddSample("TW, TbarW",                                             "tW",    itBkg, kMagenta,   0.30);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO",  "DY",    itBkg, kAzure-8,   0.15);
  //p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM,",          "DY",    itBkg, kAzure-8,   0.15);
  p->AddSample("TTbar_Powheg",                                          "ttbar", itBkg, kRed+1,     0.00);
  p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",  "Data",  itData);

  p->AddSample("TTbar_Powheg_ueUp",      "ttbar", itSys, 1, 0, "ueUp"     );
  p->AddSample("TTbar_Powheg_ueDown",    "ttbar", itSys, 1, 0, "ueDown"   );
  p->AddSample("TTbar_Powheg_hdampUp",   "ttbar", itSys, 1, 0, "hdampUp"  );
  p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, 0, "hdampDown");
  p->AddSample("TTbar_Powheg_isrUp",     "ttbar", itSys, 1, 0, "isrUp"    );
  p->AddSample("TTbar_Powheg_isrDown",   "ttbar", itSys, 1, 0, "isrDown"  );
  p->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, 0, "fsrUp"    );
  p->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, 0, "fsrDown"  );

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "ScaleUp"    );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "ScaleDown"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "pdfUp"      );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "pdfDown"    );
  
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "JERUp"      );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "JERDown"    );

  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "LepEffUp"  );
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "LepEffDown");

//  p->AddSample("TTJets_aMCatNLO", "ttbar", itSys, 1, 0, "nloUp");
//  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "nloDown");
//  p->AddSample("TTbar_Powheg_Herwig", "ttbar", itSys, 1, 0, "hadUp");
//  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, 0, "hadDown");

  p->AddSystematic("JES,Btag,MisTag,PU,stat");
  p->SetSignalStyle("xsec");
  p->SetTableFormats("2.1");

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
  x->SwitchLabel("LepEff", "Lepton efficiencies");
  x->SwitchLabel("Trig", "Trigger efficiencies");
  x->SwitchLabel("PU", "Pile-Up");
  x->SwitchLabel("Scale", "ME scales");
  x->SwitchLabel("pdf", "PDF");
  x->SwitchLabel("stat", "MC stat");

  // Scale FSR unc by sqrt
  Float_t diff = TMath::Abs(x->GetYield("ttbar") - x->GetUnc("FSR scale"));
  x->SetUnc("FSR scale", x->GetYield("ttbar")-diff/TMath::Sqrt(2));
  x->SetUnc("FSR scale", x->GetYield("ttbar"));
  x->SetMembers();

  x->SetOutputFolder("./Stop2L/");

  x->SetTableName("UncertaintiesTop");
  x->SetYieldsTableName("Yields");

  x->PrintSystematicTable("html,tex,txt");
  //x->PrintYieldsTable("html,tex,txt");

}

