#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(PDFunc.C+)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, bool setLegendLeft=false, TString yAxisTitleStyle="", float max = 0.);
void DrawCustomPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bins, TString Xtitle, bool setLegendLeft=false, TString yAxisTitleStyle="", float max = 0.);
TString NameOfTree  = "Mini1j1t";
TString StandardCut = "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0)";
TString pathToTree  = "../TW_temp/";
TString systlist    = "stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig";
Float_t sigmattbar     = 831.76;
Float_t sigmadilep     = 88.28769753;

Float_t nUEUpttbar     = 58953660;
Float_t nUEUpdilep     = 9907537;
Float_t nUEDownttbar   = 58338240;
Float_t nUEDowndilep   = 9691700;
Float_t nhDampUpttbar  = 58858606;
Float_t nhDampUpdilep  = 9672473;
Float_t nhDampDownttbar= 58163976;
Float_t nhDampDowndilep= 9634312;


void run1j1tPlots(TString pathToTree  = "../TW_temp/"){
//   DrawPlot("TnBTotal - 1"           , StandardCut, "ElMu",  3, -0.5, 2.5, "Number of b-tagged loose jets"         , false);
//   DrawPlot("TnLooseCentral - 1"     , StandardCut, "ElMu",  5, -0.5, 4.5, "Number of loose jets"                  , false);
//   DrawPlot("TDilepMETJetPt_THTtot"  , StandardCut, "ElMu", 20,  0.,   1., "p_{T}(e#mu j met) / H_{T}"             , false, "units,0.05");
//   DrawPlot("TC_jll"                 , StandardCut, "ElMu", 20,  0.,   1., "C_{je#mu}"                             , true , "units,0.05");
//   DrawPlot("TTJetLooseCentralpt"    , StandardCut, "ElMu", 30,  0.,  30., "Loose jet p_{T} (GeV)"                 , false, "gev");
//   DrawPlot("TMSys"                  , StandardCut, "ElMu", 20, 50., 750., "m_{sys} (GeV)"                         , false, "gev");
//   DrawPlot("THTLepOverHT"           , StandardCut, "ElMu", 20,  0.,   1., "H_{T}+lep / H_{T}"                     , false, "units,0.05", 1e4);
  
  DrawPlot("TMET"                   , StandardCut, "ElMu", 20,  0., 500., "\\slash{E}_{T} (GeV)"                   , false, "gev");
  DrawPlot("TMET_Phi"               , StandardCut, "ElMu", 20,-TMath::Pi(),TMath::Pi(),"\\varphi(\\slash{E}_{T}) (rad)", false, "gev");
  DrawPlot("TM_LeadingB"            , StandardCut, "ElMu", 20,  0., 500., "m(\\ell_{1}, j) (GeV)"                  , false, "gev");
  DrawPlot("TM_SubLeadingB"         , StandardCut, "ElMu", 20,  0., 500., "m(\\ell_{2}, j) (GeV)"                  , false, "gev");
  DrawPlot("TM_LLB"                 , StandardCut, "ElMu", 20,  0., 800., "m(\\ell_{1}, \\ell_{2}, j) (GeV)"        , false, "gev");
  DrawPlot("TMT_LLMETB"             , StandardCut, "ElMu", 20,  0., 800., "m_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T}} (GeV)", false, "gev");
  DrawPlot("TE_LLB"                 , StandardCut, "ElMu", 20,  0., 1000, "E(\\ell_{1}, \\ell_{2}, j) (GeV)"        , false, "gev");
  DrawPlot("TDilepPt"               , StandardCut, "ElMu", 20,  0., 500., "p_{T}(\\ell_{1}, \\ell_{2}) (GeV)"       , false, "gev");
  DrawPlot("TDilepJetPt"            , StandardCut, "ElMu", 25,  0., 250., "p_{T}(\\ell_{1}, \\ell_{2}, j) (GeV)"    , false, "gev");
  DrawPlot("TDilepMETJetPt"         , StandardCut, "ElMu", 20,  0., 500., "p_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T}} (GeV)", false, "gev");
  DrawPlot("THTtot"                 , StandardCut, "ElMu", 20, 70., 550., "H_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T})(GeV)" , false, "gev");
  DrawPlot("TLeadingJetPt"          , StandardCut, "ElMu", 20,  0., 500., "p_{T}(j) (GeV)"                        , false, "gev");
  DrawPlot("TLeadingJetE"           , StandardCut, "ElMu", 20,  0., 500., "E(j) (GeV)"                            , false, "gev");
  DrawPlot("TLeadingJetPhi"         , StandardCut, "ElMu", 20,-TMath::Pi(),TMath::Pi(), "\\varphi(j) (rad)"        , false, "gev");
  DrawPlot("abs(TLeadingJetEta)"    , StandardCut, "ElMu", 20,  0, 2.4, "|\\eta|(j) (GeV)"                         , false, "gev");
  DrawPlot("TLeadingLepPt"          , StandardCut, "ElMu", 20,  0., 500., "p_{T}(\\ell_{1}) (GeV)"                 , false, "gev");
  DrawPlot("TLeadingLepE"           , StandardCut, "ElMu", 20,  0., 500., "E(\\ell_{1}) (GeV)"                     , false, "gev");
  DrawPlot("TLeadingLepPhi"         , StandardCut, "ElMu", 20,-TMath::Pi(),TMath::Pi(), "\\varphi(\\ell_{1}) (rad)" , false, "gev");
  DrawPlot("abs(TLeadingLepEta)"    , StandardCut, "ElMu", 20,0, 2.4, "|\\eta|(\\ell_{1}) (GeV)"                    , false, "gev");
  DrawPlot("TSubLeadingLepPt"       , StandardCut, "ElMu", 20,  0., 500., "p_{T}(\\ell_{2}) (GeV)"                 , false, "gev");
  DrawPlot("TSubLeadingLepE"        , StandardCut, "ElMu", 20,  0., 500., "E(\\ell_{2}) (GeV)"                     , false, "gev");
  DrawPlot("TSubLeadingLepPhi"      , StandardCut, "ElMu", 20,-TMath::Pi(),TMath::Pi(), "\\varphi(\\ell_{2}) (rad)" , false, "gev");
  DrawPlot("abs(TSubLeadingLepEta)" , StandardCut, "ElMu", 20,  0, 2.4, "|\\eta|(\\ell_2) (GeV)"                    , false, "gev");
  
  Float_t RB_TMET[]             = {0., 20., 35., 50., 70., 140., 200.};
  Float_t RB_TMET_Phi[]         = {(Float_t) -TMath::Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, (Float_t) TMath::Pi()};
  Float_t RB_TM_LeadingB[]      = {0., 45., 75., 105., 140., 190., 230., 300., 400.};
  Float_t RB_TM_SubLeadingB[]   = {0., 50., 60., 80., 100., 125., 150., 200., 300.};
  Float_t RB_TM_LLB[]           = {0., 125., 170., 180., 200., 240., 290., 350., 500.};
  Float_t RB_TMT_LLMETB[]       = {0., 200., 270., 320., 380., 450., 600.};
  Float_t RB_TE_LLB[]           = {0., 160., 220., 280., 340., 400., 450., 550., 700.};
  Float_t RB_TDilepPt[]         = {0., 40., 60., 80., 90., 110., 130., 150., 200.};
  Float_t RB_TDilepJetPt[]      = {0., 20., 30., 50., 60., 90., 100., 140., 200.};
  Float_t RB_TDilepMETJetPt[]   = {0., 20., 30., 40., 50., 60., 70., 80., 150.};
  Float_t RB_THTtot[]           = {0., 150., 200., 250., 300., 350., 400., 450., 600.};
  Float_t RB_TLeadingJetPt[]    = {0., 50., 75., 110., 150., 200., 300.};
  Float_t RB_TLeadingJetE[]     = {0., 40., 70., 120., 175., 275., 400.};
  Float_t RB_TLeadingJetPhi[]   = {(Float_t) -TMath::Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, (Float_t) TMath::Pi()};
  Float_t RB_TLeadingJetEta[]   = {0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};
  Float_t RB_TLeadingLepPt[]    = {0., 50., 65., 75., 90., 110., 130., 160., 250.};
  Float_t RB_TLeadingLepE[]     = {0., 40., 60., 80., 100., 120., 150., 250., 350.};
  Float_t RB_TLeadingLepPhi[]   = {(Float_t) -TMath::Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, (Float_t) TMath::Pi()};
  Float_t RB_TLeadingLepEta[]   = {0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};
  Float_t RB_TSubLeadingLepPt[] = {0., 30., 40., 50., 58., 68., 78., 100., 150.};
  Float_t RB_TSubLeadingLepE[]  = {0., 30., 50., 70., 90., 115., 140., 175., 250.};
  Float_t RB_TSubLeadingLepPhi[]= {(Float_t) -TMath::Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, (Float_t) TMath::Pi()};
  Float_t RB_TSubLeadingLepEta[]= {0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};
  
  DrawCustomPlot("TMET"                  , StandardCut, "ElMu", sizeof(RB_TMET)/sizeof(*RB_TMET) - 1,  RB_TMET                                     ,   "\\slash{E}_{T} (GeV)"   , false, "gev");
  DrawCustomPlot("TMET_Phi"              , StandardCut, "ElMu", sizeof(RB_TMET_Phi)/sizeof(*RB_TMET_Phi) - 1,  RB_TMET_Phi         ,            "\\varphi(\\slash{E}_{T}) (rad)", false, "gev");
  DrawCustomPlot("TM_LeadingB"           , StandardCut, "ElMu", sizeof(RB_TM_LeadingB)/sizeof(*RB_TM_LeadingB) - 1,  RB_TM_LeadingB      ,             "m(\\ell_{1}, j) (GeV)"  , false, "gev");
  DrawCustomPlot("TM_SubLeadingB"        , StandardCut, "ElMu", sizeof(RB_TM_SubLeadingB)/sizeof(*RB_TM_SubLeadingB) - 1, RB_TM_SubLeadingB,           "m(\\ell_{2}, j) (GeV)"  , false, "gev");
  DrawCustomPlot("TM_LLB"                , StandardCut, "ElMu", sizeof(RB_TM_LLB)/sizeof(*RB_TM_LLB) - 1, RB_TM_LLB,                          "m(\\ell_{1}, \\ell_{2}, j) (GeV)", false, "gev");
  DrawCustomPlot("TMT_LLMETB"            , StandardCut, "ElMu", sizeof(RB_TMT_LLMETB)/sizeof(*RB_TMT_LLMETB) - 1,RB_TMT_LLMETB,"m_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T}} (GeV)", false, "gev");
  DrawCustomPlot("TE_LLB"                , StandardCut, "ElMu", sizeof(RB_TE_LLB)/sizeof(*RB_TE_LLB) - 1, RB_TE_LLB,                          "E(\\ell_{1}, \\ell_{2}, j) (GeV)", false, "gev");
  DrawCustomPlot("TDilepPt"              , StandardCut, "ElMu", sizeof(RB_TDilepPt)/sizeof(*RB_TDilepPt) - 1, RB_TDilepPt,                   "p_{T}(\\ell_{1}, \\ell_{2}) (GeV)", false, "gev");
  DrawCustomPlot("TDilepJetPt"           , StandardCut, "ElMu", sizeof(RB_TDilepJetPt)/sizeof(*RB_TDilepJetPt) - 1, RB_TDilepJetPt,       "p_{T}(\\ell_{1}, \\ell_{2}, j) (GeV)", false, "gev");
  DrawCustomPlot("TDilepMETJetPt"        , StandardCut, "ElMu", sizeof(RB_TDilepMETJetPt)/sizeof(*RB_TDilepMETJetPt) - 1,RB_TDilepMETJetPt,"p_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T}} (GeV)", false, "gev");
  DrawCustomPlot("THTtot"                , StandardCut, "ElMu", sizeof(RB_THTtot)/sizeof(*RB_THTtot) - 1, RB_THTtot     , "H_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T})(GeV)"  , false, "gev");
  DrawCustomPlot("TLeadingJetPt"         , StandardCut, "ElMu", sizeof(RB_TLeadingJetPt)/sizeof(*RB_TLeadingJetPt) - 1, RB_TLeadingJetPt   ,         "p_{T}(j) (GeV)"           , false, "gev");
  DrawCustomPlot("TLeadingJetE"          , StandardCut, "ElMu", sizeof(RB_TLeadingJetE)/sizeof(*RB_TLeadingJetE) - 1, RB_TLeadingJetE     ,          "E(j) (GeV)"               , false, "gev");
  DrawCustomPlot("TLeadingJetPhi"        , StandardCut, "ElMu", sizeof(RB_TLeadingJetPhi)/sizeof(*RB_TLeadingJetPhi) - 1, RB_TLeadingJetPhi   ,      "\\varphi(j) (rad)"        , false, "gev");
  DrawCustomPlot("abs(TLeadingJetEta)"   , StandardCut, "ElMu", sizeof(RB_TLeadingJetEta)/sizeof(*RB_TLeadingJetEta) - 1, RB_TLeadingJetEta   ,      "|\\eta|(j) (GeV)"         , false, "gev");
  DrawCustomPlot("TLeadingLepPt"         , StandardCut, "ElMu", sizeof(RB_TLeadingLepPt)/sizeof(*RB_TLeadingLepPt) - 1, RB_TLeadingLepPt    ,        "p_{T}(\\ell_{1}) (GeV)"   , false, "gev");
  DrawCustomPlot("TLeadingLepE"          , StandardCut, "ElMu", sizeof(RB_TLeadingLepE)/sizeof(*RB_TLeadingLepE) - 1, RB_TLeadingLepE     ,          "E(\\ell_{1}) (GeV)"       , false, "gev");
  DrawCustomPlot("TLeadingLepPhi"        , StandardCut, "ElMu", sizeof(RB_TLeadingLepPhi)/sizeof(*RB_TLeadingLepPhi) - 1, RB_TLeadingLepPhi   ,      "\\varphi(\\ell_{1}) (rad)", false, "gev");
  DrawCustomPlot("abs(TLeadingLepEta)"   , StandardCut, "ElMu", sizeof(RB_TLeadingLepEta)/sizeof(*RB_TLeadingLepEta) - 1, RB_TLeadingLepEta   ,      "|\\eta|(\\ell_{1}) (GeV)" , false, "gev");
  DrawCustomPlot("TSubLeadingLepPt"      , StandardCut, "ElMu", sizeof(RB_TSubLeadingLepPt)/sizeof(*RB_TSubLeadingLepPt) - 1, RB_TSubLeadingLepPt,  "p_{T}(\\ell_{2}) (GeV)"    , false, "gev");
  DrawCustomPlot("TSubLeadingLepE"       , StandardCut, "ElMu", sizeof(RB_TSubLeadingLepE)/sizeof(*RB_TSubLeadingLepE) - 1, RB_TSubLeadingLepE,    "E(\\ell_{2}) (GeV)"         , false, "gev");
  DrawCustomPlot("TSubLeadingLepPhi"     , StandardCut, "ElMu", sizeof(RB_TSubLeadingLepPhi)/sizeof(*RB_TSubLeadingLepPhi) - 1, RB_TSubLeadingLepPhi,"\\varphi(\\ell_{2}) (rad)", false, "gev");
  DrawCustomPlot("abs(TSubLeadingLepEta)", StandardCut, "ElMu", sizeof(RB_TSubLeadingLepEta)/sizeof(*RB_TSubLeadingLepEta) - 1, RB_TSubLeadingLepEta,"|\\eta|(\\ell_2) (GeV)"   , false, "gev");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, bool setLegendLeft = false, TString yAxisTitleStyle="", float max = 0.){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  
  // Initial configuration
  p->SetPath(pathToTree); 
  p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose  = false;
  p->verbose  = true;
  // p->SetVarName("forFit");
  TGaxis::SetMaxDigits(3);
//   p->SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
  p->SetTitleY("Events");
  Float_t specialweight = 0.;
  
  // Adding general-purpose samples --- Beware of the ORDER of the samples!!!!!!
  p->AddSample("TTbar_PowhegSemi"             , "Non-W|Z"     , itBkg, 413, systlist);
  p->AddSample("WJetsToLNu_MLM"               , "Non-W|Z"     , itBkg, 413, systlist);
  
  p->AddSample("TTWToLNu"                     , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTWToQQ"                      , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTZToQQ"                      , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTZToLLNuNu"                  , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("WZ"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("WW"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("ZZ"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  
//   p->AddSample("DYJetsToLL_M10to50_aMCatNLO"  , "DY"          , itBkg, 852, systlist);
//   p->AddSample("DYJetsToLL_M50_aMCatNLO"      , "DY"          , itBkg, 852, systlist);
  p->AddSample("DYJetsToLL_M5to50_MLM"        , "DY"          , itBkg, 852, systlist);
  p->AddSample("DYJetsToLL_M50_MLM"           , "DY"          , itBkg, 852, systlist);
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itBkg, 633, systlist);

  p->AddSample("TW"                           , "tW"          , itBkg, TColor::GetColor("#ffcc33"), systlist);
  p->AddSample("TbarW"                        , "tW"          , itBkg, TColor::GetColor("#ffcc33"), systlist);
  
  p->AddSample("MuonEG"                       , "Data"        , itData);
  p->AddSample("SingleMuon"                   , "Data"        , itData);
  p->AddSample("SingleElec"                   , "Data"        , itData);
  
  
  // Adding samples for systematic uncertanties
  p->AddSample("TW"                           , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TW_noFullyHadr_isrUp"         , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown"       , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"         , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown"       , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TW_noFullyHadr_MEscaleUp"     , "tW"          , itSys, 1, "tW_MEUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown"   , "tW"          , itSys, 1, "tW_MEDown");
  p->AddSample("TW_noFullyHadr_PSscaleUp"     , "tW"          , itSys, 1, "tW_PSUp");
  p->AddSample("TW_noFullyHadr_PSscaleDown"   , "tW"          , itSys, 1, "tW_PSDown");

  p->AddSample("TbarW"                        , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TbarW_noFullyHadr_isrUp"      , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  , "tW"          , itSys, 1, "tW_MEUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown", "tW"          , itSys, 1, "tW_MEDown");
  p->AddSample("TbarW_noFullyHadr_PSscaleUp"  , "tW"          , itSys, 1, "tW_PSUp");
  p->AddSample("TbarW_noFullyHadr_PSscaleDown", "tW"          , itSys, 1, "tW_PSDown");
  
  p->AddSample("TW_noFullyHadr_DS"            , "tW"          , itSys, 1, "DSUp");
  p->AddSample("TbarW_noFullyHadr_DS"         , "tW"          , itSys, 1, "DSUp");
  p->AddSymmetricHisto("tW",  "DSUp");
  p->AddSymmetricHisto("tW",  "JERUp");
  
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "JERUp");
  
  specialweight = nUEUpttbar/sigmattbar/(nUEUpttbar/sigmattbar + nUEUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_ueUp"            , "t#bar{t}"    , itSys, 1, "ueUp");
  specialweight = nUEUpdilep/sigmadilep/(nUEUpttbar/sigmattbar + nUEUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_ueUp"          , "t#bar{t}"    , itSys, 1, "ueUp");
  specialweight = nUEDownttbar/sigmattbar/(nUEDownttbar/sigmattbar + nUEDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_ueDown"          , "t#bar{t}"    , itSys, 1, "ueDown");
  specialweight = nUEDowndilep/sigmadilep/(nUEDownttbar/sigmattbar + nUEDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_ueDown"        , "t#bar{t}"    , itSys, 1, "ueDown");
  specialweight = nhDampUpttbar/sigmattbar/(nhDampUpttbar/sigmattbar + nhDampUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_hdampUp"         , "t#bar{t}"    , itSys, 1, "hdampUp");
  specialweight = nhDampUpdilep/sigmadilep/(nhDampUpttbar/sigmattbar + nhDampUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_hdampUp"       , "t#bar{t}"    , itSys, 1, "hdampUp");
  specialweight = nhDampDownttbar/sigmattbar/(nhDampDownttbar/sigmattbar + nhDampDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_hdampDown"       , "t#bar{t}"    , itSys, 1, "hdampDown");
  specialweight = nhDampDowndilep/sigmadilep/(nhDampDownttbar/sigmattbar + nhDampDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_hdampDown"     , "t#bar{t}"    , itSys, 1, "hdampDown");
  p->SetWeight("TWeight");
  
  p->AddSample("TTbar_Powheg_isrUp"           , "t#bar{t}"    , itSys, 1, "isrUp");
  p->AddSample("TTbar_Powheg_isrDown"         , "t#bar{t}"    , itSys, 1, "isrDown");
  p->AddSample("TTbar_Powheg_fsrUp"           , "t#bar{t}"    , itSys, 1, "fsrUp");
  p->AddSample("TTbar_Powheg_fsrDown"         , "t#bar{t}"    , itSys, 1, "fsrDown");
  p->AddSample("TTbar_GluonMoveCRTune"        , "t#bar{t}"    , itSys, 1, "GluonMoveCRTune");
  p->AddSample("TTbar_GluonMoveCRTune_erdON"  , "t#bar{t}"    , itSys, 1, "GluonMoveCRTune_erdON");
  p->AddSample("TTbar_Powheg_erdON"           , "t#bar{t}"    , itSys, 1, "Powheg_erdON");
  p->AddSample("TTbar_QCDbasedCRTune_erdON"   , "t#bar{t}"    , itSys, 1, "QCDbasedCRTune_erdON");
  p->UseEnvelope("t#bar{t}", "GluonMoveCRTune,GluonMoveCRTune_erdON,Powheg_erdON,QCDbasedCRTune_erdON", "ColorReconnection");
  p->AddSymmetricHisto("t#bar{t}",  "JERUp");
  
  pdf = PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, StandardCut, chan, var, nbins, bin0, binN);
  Histo* hPDFUp  = pdf.GetSystHisto("up","pdf")->CloneHisto();
  Histo* hPDFDown= pdf.GetSystHisto("Down","pdf")->CloneHisto();
  Histo* hMEUp   = pdf.GetSystHisto("up","ME")->CloneHisto();
  Histo* hMEDown = pdf.GetSystHisto("Down","ME")->CloneHisto();
  p->PrepareHisto(hPDFUp,   "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarPDFUp");
  p->PrepareHisto(hPDFDown, "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarPDFDown");
  p->PrepareHisto(hMEUp,    "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarMEUp");
  p->PrepareHisto(hMEDown,  "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarMEDown");
  p->AddToSystematicLabels("ttbarPDF");
  p->AddToSystematicLabels("ttbarME");
  
  // Other settings
  p->doUncInLegend=true;
  p->SetRatioMin( 0.6 );
  p->SetRatioMax( 1.4 );
  
  if (max > 0) p->SetPlotMaximum(max);

  p->SetCMSlabel("CMS");
  p->SetCMSmodeLabel("Preliminary");
  p->SetLegendPosition(0.7, 0.45, 0.93, 0.92);
  p->SetPlotFolder("/nfs/fanae/user/vrbouza/www/TFM/1j1t/");
  p->doYieldsInLeg  = false;
  p->doSetLogy      = false;
  p->doData         = true;
  p->doSignal       = false;
  
  if (setLegendLeft) p->SetLegendPosition("UL");
  p->AddLumiSyst(0.025);
  
  // Drawing and computations
  if (var.Contains("abs")) {
    p->NoShowVarName = true;
    p->SetOutputName(var.ReplaceAll("abs(","").ReplaceAll(")",""));
  }
  p->DrawStack();
//   cout << "Done drawing stack!" << endl;
  
  p->PrintSystematics();
  p->PrintSystYields();
  
  // Debugging thing
//   Double_t nVVttV = p->GetHisto("VV+t#bar{t}V","")->GetBinContent(2);
//   Double_t nDY    = p->GetHisto("DY","")->GetBinContent(2);
//   Double_t nttbar = p->GetHisto("t#bar{t}","")->GetBinContent(2);
//   Double_t nNonWZ = p->GetHisto("Non-W|Z","")->GetBinContent(2);
//   Double_t ntw    = p->GetHisto("tW","")->GetBinContent(2);
//   Double_t ndata  = p->GetHisto("Data","")->GetBinContent(2);
//   cout << nVVttV << endl;
//   cout << nDY << endl;
//   cout << nttbar << endl;
//   cout << nNonWZ << endl;
//   cout << ntw << endl;
//   cout << ndata << endl;
//   
  
  
  delete p;
}


void DrawCustomPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t* bins, TString Xtitle, bool setLegendLeft = false, TString yAxisTitleStyle="", float max = 0.){
  Plot* p = new Plot(var, cut, chan, nbins, bins, "Title", Xtitle);
  
  // Initial configuration
  p->SetPath(pathToTree); 
  p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose  = false;
  p->verbose  = true;
  // p->SetVarName("forFit");
  TGaxis::SetMaxDigits(3);
//   p->SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
  p->SetTitleY("Events");
  Float_t specialweight = 0.;
  
  // Adding general-purpose samples --- Beware of the ORDER of the samples!!!!!!
  p->AddSample("TTbar_PowhegSemi"             , "Non-W|Z"     , itBkg, 413, systlist);
  p->AddSample("WJetsToLNu_MLM"               , "Non-W|Z"     , itBkg, 413, systlist);
  
  p->AddSample("TTWToLNu"                     , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTWToQQ"                      , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTZToQQ"                      , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("TTZToLLNuNu"                  , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("WZ"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("WW"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  p->AddSample("ZZ"                           , "VV+t#bar{t}V", itBkg, 390, systlist);
  
//   p->AddSample("DYJetsToLL_M10to50_aMCatNLO"  , "DY"          , itBkg, 852, systlist);
//   p->AddSample("DYJetsToLL_M50_aMCatNLO"      , "DY"          , itBkg, 852, systlist);
  p->AddSample("DYJetsToLL_M5to50_MLM"        , "DY"          , itBkg, 852, systlist);
  p->AddSample("DYJetsToLL_M50_MLM"           , "DY"          , itBkg, 852, systlist);
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itBkg, 633, systlist);

  p->AddSample("TW"                           , "tW"          , itBkg, TColor::GetColor("#ffcc33"), systlist);
  p->AddSample("TbarW"                        , "tW"          , itBkg, TColor::GetColor("#ffcc33"), systlist);
  
  p->AddSample("MuonEG"                       , "Data"        , itData);
  p->AddSample("SingleMuon"                   , "Data"        , itData);
  p->AddSample("SingleElec"                   , "Data"        , itData);
  
  
  // Adding samples for systematic uncertanties
  p->AddSample("TW"                           , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TW_noFullyHadr_isrUp"         , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown"       , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"         , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown"       , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TW_noFullyHadr_MEscaleUp"     , "tW"          , itSys, 1, "tWMEUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown"   , "tW"          , itSys, 1, "tWMEDown");
  p->AddSample("TW_noFullyHadr_PSscaleUp"     , "tW"          , itSys, 1, "tWPSUp");
  p->AddSample("TW_noFullyHadr_PSscaleDown"   , "tW"          , itSys, 1, "tWPSDown");

  p->AddSample("TbarW"                        , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TbarW_noFullyHadr_isrUp"      , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  , "tW"          , itSys, 1, "tWMEUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown", "tW"          , itSys, 1, "tWMEDown");
  p->AddSample("TbarW_noFullyHadr_PSscaleUp"  , "tW"          , itSys, 1, "tWPSUp");
  p->AddSample("TbarW_noFullyHadr_PSscaleDown", "tW"          , itSys, 1, "tWPSDown");
  
  p->AddSample("TW_noFullyHadr_DS"            , "tW"          , itSys, 1, "DSUp");
  p->AddSample("TbarW_noFullyHadr_DS"         , "tW"          , itSys, 1, "DSUp");
  p->AddSymmetricHisto("tW",  "DSUp");
  p->AddSymmetricHisto("tW",  "JERUp");
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "JERUp");
  
  specialweight = nUEUpttbar/sigmattbar/(nUEUpttbar/sigmattbar + nUEUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_ueUp"            , "t#bar{t}"    , itSys, 1, "ueUp");
  specialweight = nUEUpdilep/sigmadilep/(nUEUpttbar/sigmattbar + nUEUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_ueUp"          , "t#bar{t}"    , itSys, 1, "ueUp");
  specialweight = nUEDownttbar/sigmattbar/(nUEDownttbar/sigmattbar + nUEDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_ueDown"          , "t#bar{t}"    , itSys, 1, "ueDown");
  specialweight = nUEDowndilep/sigmadilep/(nUEDownttbar/sigmattbar + nUEDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_ueDown"        , "t#bar{t}"    , itSys, 1, "ueDown");
  specialweight = nhDampUpttbar/sigmattbar/(nhDampUpttbar/sigmattbar + nhDampUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_hdampUp"         , "t#bar{t}"    , itSys, 1, "hdampUp");
  specialweight = nhDampUpdilep/sigmadilep/(nhDampUpttbar/sigmattbar + nhDampUpdilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_hdampUp"       , "t#bar{t}"    , itSys, 1, "hdampUp");
  specialweight = nhDampDownttbar/sigmattbar/(nhDampDownttbar/sigmattbar + nhDampDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar_Powheg_hdampDown"       , "t#bar{t}"    , itSys, 1, "hdampDown");
  specialweight = nhDampDowndilep/sigmadilep/(nhDampDownttbar/sigmattbar + nhDampDowndilep/sigmadilep);
  p->SetWeight("TWeight*" + to_string(specialweight));
  p->AddSample("TTbar2L_Powheg_hdampDown"     , "t#bar{t}"    , itSys, 1, "hdampDown");
  p->SetWeight("TWeight");
  
  p->AddSample("TTbar_Powheg_isrUp"           , "t#bar{t}"    , itSys, 1, "isrUp");
  p->AddSample("TTbar_Powheg_isrDown"         , "t#bar{t}"    , itSys, 1, "isrDown");
  p->AddSample("TTbar_Powheg_fsrUp"           , "t#bar{t}"    , itSys, 1, "fsrUp");
  p->AddSample("TTbar_Powheg_fsrDown"         , "t#bar{t}"    , itSys, 1, "fsrDown");
  p->AddSample("TTbar_GluonMoveCRTune"        , "t#bar{t}"    , itSys, 1, "GluonMoveCRTune");
  p->AddSample("TTbar_GluonMoveCRTune_erdON"  , "t#bar{t}"    , itSys, 1, "GluonMoveCRTune_erdON");
  p->AddSample("TTbar_Powheg_erdON"           , "t#bar{t}"    , itSys, 1, "Powheg_erdON");
  p->AddSample("TTbar_QCDbasedCRTune_erdON"   , "t#bar{t}"    , itSys, 1, "QCDbasedCRTune_erdON");
  p->UseEnvelope("t#bar{t}", "GluonMoveCRTune,GluonMoveCRTune_erdON,Powheg_erdON,QCDbasedCRTune_erdON", "ColorReconnection");
  p->AddSymmetricHisto("t#bar{t}",  "JERUp");

  pdf = PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, StandardCut, chan, var, nbins, bins);
  Histo* hPDFUp  = pdf.GetSystHisto("up","pdf")->CloneHisto();
  Histo* hPDFDown= pdf.GetSystHisto("Down","pdf")->CloneHisto();
  Histo* hMEUp   = pdf.GetSystHisto("up","ME")->CloneHisto();
  Histo* hMEDown = pdf.GetSystHisto("Down","ME")->CloneHisto();
  p->PrepareHisto(hPDFUp,   "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarPDFUp");
  p->PrepareHisto(hPDFDown, "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarPDFDown");
  p->PrepareHisto(hMEUp,    "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarMEUp");
  p->PrepareHisto(hMEDown,  "TTbar_Powheg", "t#bar{t}", itSys, 0, "ttbarMEDown");
  p->AddToSystematicLabels("ttbarPDF");
  p->AddToSystematicLabels("ttbarME");
  
  // Other settings
  p->doUncInLegend=true;
  p->SetRatioMin( 0.6 );
  p->SetRatioMax( 1.4 );
  
  if (max > 0) p->SetPlotMaximum(max);

  p->SetCMSlabel("CMS");
  p->SetCMSmodeLabel("Preliminary");
  p->SetLegendPosition(0.7, 0.45, 0.93, 0.92);
  p->SetPlotFolder("/nfs/fanae/user/vrbouza/www/TFM/1j1t/");
  p->doYieldsInLeg  = false;
  p->doSetLogy      = false;
  p->doData         = true;
  p->doSignal       = false;
  
  if (setLegendLeft) p->SetLegendPosition("UL");
  p->AddLumiSyst(0.025);
  
  
  // Drawing and computations
  p->SetOutputName("Custom");
  if (var.Contains("abs")) {
  p->NoShowVarName = true;
  p->SetOutputName("Custom_" + var.ReplaceAll("abs(","").ReplaceAll(")",""));
}
  p->DrawStack();
//   p->PrintSystematics();
//   p->PrintSystYields();
  delete p;
}
