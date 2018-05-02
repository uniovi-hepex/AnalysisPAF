#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
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

void run1j1tPlots(TString pathToTree  = "../TW_temp/"){
//   DrawPlot("TnBTotal - 1"           , StandardCut, "ElMu",  3, -0.5, 2.5, "Number of b-tagged loose jets"         , false);
//   DrawPlot("TnLooseCentral - 1"     , StandardCut, "ElMu",  5, -0.5, 4.5, "Number of loose jets"                  , false);
//   DrawPlot("TDilepMETJetPt_THTtot"  , StandardCut, "ElMu", 20,  0.,   1., "p_{T}(e#mu j met) / H_{T}"             , false, "units,0.05");
//   DrawPlot("TC_jll"                 , StandardCut, "ElMu", 20,  0.,   1., "C_{je#mu}"                             , true , "units,0.05");
//   DrawPlot("TTJetLooseCentralpt"    , StandardCut, "ElMu", 30,  0.,  30., "Loose jet p_{T} [GeV]"                 , false, "gev");
//   DrawPlot("TMSys"                  , StandardCut, "ElMu", 20, 50., 750., "m_{sys} [GeV]"                         , false, "gev");
//   DrawPlot("THTLepOverHT"           , StandardCut, "ElMu", 20,  0.,   1., "H_{T}+lep / H_{T}"                     , false, "units,0.05", 1e4);
  
  DrawPlot("TMET"                   , StandardCut, "ElMu", 20,  0., 500., "#slash{E}_{T} [GeV]"                   , false, "gev");
  DrawPlot("TMET_Phi"               , StandardCut, "ElMu", 20,-TMath::Pi(),TMath::Pi(),"#varphi(#slash{E}_{T}) [GeV]", false, "gev");
  DrawPlot("TM_LeadingB"            , StandardCut, "ElMu", 20,  0., 500., "m^{l_{1}b} [GeV]"                      , false, "gev");
  DrawPlot("TM_SubLeadingB"         , StandardCut, "ElMu", 20,  0., 500., "m^{l_{2}b} [GeV]"                      , false, "gev");
  DrawPlot("TM_LLB"                 , StandardCut, "ElMu", 20,  0., 800., "m^{llb} [GeV]"                         , false, "gev");
  DrawPlot("TMT_LLMETB"             , StandardCut, "ElMu", 20,  0., 800., "m_T^{ll #nu #nu b} [GeV]"              , false, "gev");
  DrawPlot("TE_LLB"                 , StandardCut, "ElMu", 20,  0., 1000, "E^{llb} [GeV]"                         , false, "gev");
  DrawPlot("TDilepPt"               , StandardCut, "ElMu", 20,  0., 500., "p_T^{ll} [GeV]"                        , false, "gev");
  DrawPlot("TDilepJetPt"            , StandardCut, "ElMu", 25,  0., 250., "p_{T}^{e#mu, j} [GeV]"                 , false, "gev");
  DrawPlot("TDilepMETJetPt"         , StandardCut, "ElMu", 20,  0., 500., "p_{T}^{e#mu, j, MET} [GeV]"            , false, "gev");
  DrawPlot("THTtot"                 , StandardCut, "ElMu", 20, 70., 550., "H_{T} [GeV]"                           , false, "gev");
  DrawPlot("TLeadingJetPt"          , StandardCut, "ElMu", 20,  0., 500., "p_{T}^{j} [GeV]"                       , false, "gev");
  DrawPlot("TLeadingJetE"           , StandardCut, "ElMu", 20,  0., 500., "E^{j} [GeV]"                           , false, "gev");
  DrawPlot("TLeadingJetPhi"         , StandardCut, "ElMu", 20,  -TMath::Pi(), TMath::Pi(), "#varphi^{j} [GeV]"    , false, "gev");
  DrawPlot("TLeadingJetEta"         , StandardCut, "ElMu", 20,  -2.4, 2.4, "#eta^{j} [GeV]"                       , false, "gev");
  DrawPlot("TLeadingLepPt"          , StandardCut, "ElMu", 20,  0., 500., "p_{T}^{l_{1}} [GeV]"                   , false, "gev");
  DrawPlot("TLeadingLepE"           , StandardCut, "ElMu", 20,  0., 500., "E^{l_{1}} [GeV]"                       , false, "gev");
  DrawPlot("TLeadingLepPhi"         , StandardCut, "ElMu", 20,  -TMath::Pi(), TMath::Pi(), "#varphi^{l_{1}} [GeV]", false, "gev");
  DrawPlot("TLeadingLepEta"         , StandardCut, "ElMu", 20,  -2.4, 2.4, "#eta^{l_{1}} [GeV]"                   , false, "gev");
  DrawPlot("TSubLeadingLepPt"       , StandardCut, "ElMu", 20,  0., 500., "p_{T}^{l_{2}} [GeV]"                   , false, "gev");
  DrawPlot("TSubLeadingLepE"        , StandardCut, "ElMu", 20,  0., 500., "E^{l_{2}} [GeV]"                       , false, "gev");
  DrawPlot("TSubLeadingLepPhi"      , StandardCut, "ElMu", 20,  -TMath::Pi(), TMath::Pi(), "#varphi^{l_{2}} [GeV]", false, "gev");
  DrawPlot("TSubLeadingLepEta"      , StandardCut, "ElMu", 20,  -2.4, 2.4, "#eta^{l_{2}} [GeV]"                   , false, "gev");
  
  Float_t RB_TMET[]             = {0., 25., 50., 75., 100., 125., 150., 175., 1000.};
  Float_t RB_TMET_Phi[]         = {(Float_t) -TMath::Pi(), -2., -1.5, -.75, 0., .75, 1.5, 2., (Float_t) TMath::Pi()};
  Float_t RB_TM_LeadingB[]      = {0., 50., 100., 150., 200., 250., 300., 350., 1000.};
  Float_t RB_TM_SubLeadingB[]   = {0., 25., 50., 75., 100., 125., 150., 175., 200., 250., 1000.};
  Float_t RB_TM_LLB[]           = {0., 50., 100., 150., 200., 250., 300., 350, 400, 450, 1000.};
  Float_t RB_TMT_LLMETB[]       = {0., 100., 150., 200., 250., 300., 350., 400., 450., 500., 1000.};
  Float_t RB_TE_LLB[]           = {0., 150., 200., 250., 300., 350., 400., 500., 600., 700., 1000.};
  Float_t RB_TDilepPt[]         = {0., 25., 50., 75., 100., 125., 150., 175., 500.};
  Float_t RB_TDilepJetPt[]      = {0., 20., 40., 60., 80., 100., 120., 140., 500.};
  Float_t RB_TDilepMETJetPt[]   = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 500.};
  Float_t RB_THTtot[]           = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TLeadingJetPt[]    = {0., 50., 70., 90., 110., 130., 150., 170., 500.};
  Float_t RB_TLeadingJetE[]     = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TLeadingJetPhi[]   = {(Float_t) -TMath::Pi(), -2., -1.5, -.75, 0., .75, 1.5, 2., (Float_t) TMath::Pi()};
  Float_t RB_TLeadingJetEta[]   = {-2.4, -1.75, -1.25, -.5, 0., .5, 1.25, 1.75, 2.4};
  Float_t RB_TLeadingLepPt[]    = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TLeadingLepE[]     = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TLeadingLepPhi[]   = {(Float_t) -TMath::Pi(), -2., -1.5, -.75, 0., .75, 1.5, 2., (Float_t) TMath::Pi()};
  Float_t RB_TLeadingLepEta[]   = {-2.4, -1.75, -1.25, -.5, 0., .5, 1.25, 1.75, 2.4};
  Float_t RB_TSubLeadingLepPt[] = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TSubLeadingLepE[]  = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 1000.};
  Float_t RB_TSubLeadingLepPhi[]= {(Float_t) -TMath::Pi(), -2., -1.5, -.75, 0., .75, 1.5, 2., (Float_t) TMath::Pi()};
  Float_t RB_TSubLeadingLepEta[]= {-2.4, -1.75, -1.25, -.5, 0., .5, 1.25, 1.75, 2.4};
  
  DrawCustomPlot("TMET"                   , StandardCut, "ElMu", 8,  RB_TMET             , "#slash{E}_{T} [GeV]"       , false, "gev");
  DrawCustomPlot("TMET_Phi"               , StandardCut, "ElMu", 8,  RB_TMET_Phi         , "#varphi(#slash{E}_T) [GeV]", false, "gev");
  DrawCustomPlot("TM_LeadingB"            , StandardCut, "ElMu", 8,  RB_TM_LeadingB      , "m^{l_{1}b} [GeV]"          , false, "gev");
  DrawCustomPlot("TM_SubLeadingB"         , StandardCut, "ElMu", 10, RB_TM_SubLeadingB   , "m^{l_{2}b} [GeV]"          , false, "gev");
  DrawCustomPlot("TM_LLB"                 , StandardCut, "ElMu", 10, RB_TM_LLB           , "m^{llb} [GeV]"             , false, "gev");
  DrawCustomPlot("TMT_LLMETB"             , StandardCut, "ElMu", 10, RB_TMT_LLMETB       , "m_T^{ll #nu #nu b} [GeV]"  , false, "gev");
  DrawCustomPlot("TE_LLB"                 , StandardCut, "ElMu", 10, RB_TE_LLB           , "E^{llb} [GeV]"             , false, "gev");
  DrawCustomPlot("TDilepPt"               , StandardCut, "ElMu", 8,  RB_TDilepPt         , "p_T^{ll} [GeV]"            , false, "gev");
  DrawCustomPlot("TDilepJetPt"            , StandardCut, "ElMu", 8,  RB_TDilepJetPt      , "p_{T}^{e#mu, j} [GeV]"     , false, "gev");
  DrawCustomPlot("TDilepMETJetPt"         , StandardCut, "ElMu", 10, RB_TDilepMETJetPt   , "p_{T}^{e#mu, j, MET} [GeV]", false, "gev");
  DrawCustomPlot("THTtot"                 , StandardCut, "ElMu", 10, RB_THTtot           , "H_{T} [GeV]"               , false, "gev");
  DrawCustomPlot("TLeadingJetPt"          , StandardCut, "ElMu", 8,  RB_TLeadingJetPt    , "p_{T}^{j} [GeV]"           , false, "gev");
  DrawCustomPlot("TLeadingJetE"           , StandardCut, "ElMu", 10, RB_TLeadingJetE     , "E^{j} [GeV]"               , false, "gev");
  DrawCustomPlot("TLeadingJetPhi"         , StandardCut, "ElMu", 8,  RB_TLeadingJetPhi   , "#varphi^{j} [GeV]"         , false, "gev");
  DrawCustomPlot("TLeadingJetEta"         , StandardCut, "ElMu", 8,  RB_TLeadingJetEta   , "#eta^{j} [GeV]"            , false, "gev");
  DrawCustomPlot("TLeadingLepPt"          , StandardCut, "ElMu", 10, RB_TLeadingLepPt    , "p_{T}^{l_{1}} [GeV]"       , false, "gev");
  DrawCustomPlot("TLeadingLepE"           , StandardCut, "ElMu", 10, RB_TLeadingLepE     , "E^{l_{1}} [GeV]"           , false, "gev");
  DrawCustomPlot("TLeadingLepPhi"         , StandardCut, "ElMu", 8,  RB_TLeadingLepPhi   , "#varphi^{l_{1}} [GeV]"     , false, "gev");
  DrawCustomPlot("TLeadingLepEta"         , StandardCut, "ElMu", 8,  RB_TLeadingLepEta   , "#eta^{l_{1}} [GeV]"        , false, "gev");
  DrawCustomPlot("TSubLeadingLepPt"       , StandardCut, "ElMu", 10, RB_TSubLeadingLepPt , "p_{T}^{l_{2}} [GeV]"       , false, "gev");
  DrawCustomPlot("TSubLeadingLepE"        , StandardCut, "ElMu", 10, RB_TSubLeadingLepE  , "E^{l_{2}} [GeV]"           , false, "gev");
  DrawCustomPlot("TSubLeadingLepPhi"      , StandardCut, "ElMu", 8,  RB_TSubLeadingLepPhi, "#varphi^{l_{2}} [GeV]"     , false, "gev");
  DrawCustomPlot("TSubLeadingLepEta"      , StandardCut, "ElMu", 8,  RB_TSubLeadingLepEta, "#eta^{l_{2}} [GeV]"        , false, "gev");
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
  p->SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
  
  
  // Adding general-purpose samples --- Beware of the ORDER of the samples!!!!!!
  p->AddSample("TTbar_PowhegSemi"             , "Non-W/Z"     , itBkg, 413);
  p->AddSample("WJetsToLNu_MLM"               , "Non-W/Z"     , itBkg);
  
  p->AddSample("TTWToLNu"                     , "VV+t#bar{t}V", itBkg, 390);
  p->AddSample("TTWToQQ"                      , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToQQ"                      , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToLLNuNu"                  , "VV+t#bar{t}V", itBkg);
  p->AddSample("WZ"                           , "VV+t#bar{t}V", itBkg);
  p->AddSample("WW"                           , "VV+t#bar{t}V", itBkg);
  p->AddSample("ZZ"                           , "VV+t#bar{t}V", itBkg);
  
  p->AddSample("DYJetsToLL_M10to50_aMCatNLO"  , "DY"          , itBkg, 852); // kRed);
  p->AddSample("DYJetsToLL_M50_aMCatNLO"      , "DY"          , itBkg);
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itBkg, 633);

  p->AddSample("TW"                           , "tW"          , itBkg, TColor::GetColor("#ffcc33"));
  p->AddSample("TbarW"                        , "tW"          , itBkg);
  
//   p->AddSample("MuonEG"                       , "Data"        , itData);
//   p->AddSample("SingleMuon"                   , "Data"        , itData);
//   p->AddSample("SingleElec"                   , "Data"        , itData);
  
  
  // Adding samples for systematic uncertanties
  p->AddSample("TW"                           , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TW_noFullyHadr_isrUp"         , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown"       , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"         , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown"       , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TW_noFullyHadr_MEscaleUp"     , "tW"          , itSys, 1, "MEUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown"   , "tW"          , itSys, 1, "MEDown");
  p->AddSample("TW_noFullyHadr_PSscaleUp"     , "tW"          , itSys, 1, "PSUp");
  p->AddSample("TW_noFullyHadr_PSscaleDown"   , "tW"          , itSys, 1, "PSDown");

  p->AddSample("TbarW"                        , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TbarW_noFullyHadr_isrUp"      , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  , "tW"          , itSys, 1, "MEUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown", "tW"          , itSys, 1, "MEDown");
  p->AddSample("TbarW_noFullyHadr_PSscaleUp"  , "tW"          , itSys, 1, "PSUp");
  p->AddSample("TbarW_noFullyHadr_PSscaleDown", "tW"          , itSys, 1, "PSDown");
  
  p->AddSample("TW_noFullyHadr_DS"            , "tW"          , itSys, 1, "DSUp");
  p->AddSample("TbarW_noFullyHadr_DS"         , "tW"          , itSys, 1, "DSUp");
  p->AddSymmetricHisto("tW",  "DSUp");
  p->AddSymmetricHisto("tW",  "JERUp");
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "JERUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleDown");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfDown");
  p->AddSample("TTbar_Powheg_hdampUp"         , "t#bar{t}"    , itSys, 1, "hdampUp");
  p->AddSample("TTbar_Powheg_hdampDown"       , "t#bar{t}"    , itSys, 1, "hdampDown");
  p->AddSample("TTbar_Powheg_ueUp"            , "t#bar{t}"    , itSys, 1, "ueUp");
  p->AddSample("TTbar_Powheg_ueDown"          , "t#bar{t}"    , itSys, 1, "ueDown");
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

  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
//   p->AddSystematic("stat");
  
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

  p->DrawStack();
//   cout << "Done drawing stack!" << endl;
  
  p->PrintSystematics();
  p->PrintSystYields();
  
  // Debugging thing
//   Double_t nVVttV = p->GetHisto("VV+t#bar{t}V","")->GetBinContent(2);
//   Double_t nDY    = p->GetHisto("DY","")->GetBinContent(2);
//   Double_t nttbar = p->GetHisto("t#bar{t}","")->GetBinContent(2);
//   Double_t nNonWZ = p->GetHisto("Non-W/Z","")->GetBinContent(2);
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
  p->SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
  
  // Adding general-purpose samples --- Beware of the ORDER of the samples!!!!!!
  p->AddSample("TTbar_PowhegSemi"             , "Non-W/Z"     , itBkg, 413);
  p->AddSample("WJetsToLNu_MLM"               , "Non-W/Z"     , itBkg);
  
  p->AddSample("TTWToLNu"                     , "VV+t#bar{t}V", itBkg, 390);
  p->AddSample("TTWToQQ"                      , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToQQ"                      , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToLLNuNu"                  , "VV+t#bar{t}V", itBkg);
  p->AddSample("WZ"                           , "VV+t#bar{t}V", itBkg);
  p->AddSample("WW"                           , "VV+t#bar{t}V", itBkg);
  p->AddSample("ZZ"                           , "VV+t#bar{t}V", itBkg);
  
  p->AddSample("DYJetsToLL_M10to50_aMCatNLO"  , "DY"          , itBkg, 852); // kRed);
  p->AddSample("DYJetsToLL_M50_aMCatNLO"      , "DY"          , itBkg);
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itBkg, 633);

  p->AddSample("TW"                           , "tW"          , itBkg, TColor::GetColor("#ffcc33"));
  p->AddSample("TbarW"                        , "tW"          , itBkg);
  
//   p->AddSample("MuonEG"                       , "Data"        , itData);
//   p->AddSample("SingleMuon"                   , "Data"        , itData);
//   p->AddSample("SingleElec"                   , "Data"        , itData);
  
  
  // Adding samples for systematic uncertanties
  p->AddSample("TW"                           , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TW_noFullyHadr_isrUp"         , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown"       , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"         , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown"       , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TW_noFullyHadr_MEscaleUp"     , "tW"          , itSys, 1, "MEUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown"   , "tW"          , itSys, 1, "MEDown");
  p->AddSample("TW_noFullyHadr_PSscaleUp"     , "tW"          , itSys, 1, "PSUp");
  p->AddSample("TW_noFullyHadr_PSscaleDown"   , "tW"          , itSys, 1, "PSDown");

  p->AddSample("TbarW"                        , "tW"          , itSys, 1, "JERUp");
  p->AddSample("TbarW_noFullyHadr_isrUp"      , "tW"          , itSys, 1, "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    , "tW"          , itSys, 1, "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      , "tW"          , itSys, 1, "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    , "tW"          , itSys, 1, "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  , "tW"          , itSys, 1, "MEUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown", "tW"          , itSys, 1, "MEDown");
  p->AddSample("TbarW_noFullyHadr_PSscaleUp"  , "tW"          , itSys, 1, "PSUp");
  p->AddSample("TbarW_noFullyHadr_PSscaleDown", "tW"          , itSys, 1, "PSDown");
  
  p->AddSample("TW_noFullyHadr_DS"            , "tW"          , itSys, 1, "DSUp");
  p->AddSample("TbarW_noFullyHadr_DS"         , "tW"          , itSys, 1, "DSUp");
  p->AddSymmetricHisto("tW",  "DSUp");
  p->AddSymmetricHisto("tW",  "JERUp");
  
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "JERUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleDown");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfUp");
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfDown");
  p->AddSample("TTbar_Powheg_hdampUp"         , "t#bar{t}"    , itSys, 1, "hdampUp");
  p->AddSample("TTbar_Powheg_hdampDown"       , "t#bar{t}"    , itSys, 1, "hdampDown");
  p->AddSample("TTbar_Powheg_ueUp"            , "t#bar{t}"    , itSys, 1, "ueUp");
  p->AddSample("TTbar_Powheg_ueDown"          , "t#bar{t}"    , itSys, 1, "ueDown");
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

  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
//   p->AddSystematic("stat");
  
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
  p->DrawStack();
//   p->PrintSystematics();
//   p->PrintSystYields();
  delete p;
}

// void runLooper(){
//   Looper* p = new Looper(pathToTree, NameOfTree, "TMET", "TMET > 0", "ElMu", 30, 0, 300);
//   Histo* h = p->GetHisto("TW");
//   h->SetLineColor(kRed);
//   cout << "nEntries = " << h->GetEntries() << endl;
//   cout << "Yield = " << h->Integral()*35900 << endl;
//   h->Draw();
// }
