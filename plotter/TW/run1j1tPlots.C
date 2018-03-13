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
TString NameOfTree = "Mini1j1t";
TString pathToTree = "../TW_temp/";


void run1j1tPlots(){
  DrawPlot("TnBTotal - 1 "          , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu",  3, -0.5, 2.5, "Number of b-tagged loose jets", false);
  DrawPlot("TnLooseCentral - 1 "    , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu",  5, -0.5, 4.5, "Number of loose jets"         , false);
  DrawPlot("TDilepMETJetPt_THTtot " , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0.,   1., "p_{T}(e#mu j met) / H_{T}"    , false, "units,0.05");
  DrawPlot("TC_jll "                , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0.,   1., "C_{je#mu}"                    , true , "units,0.05");
  DrawPlot("TTJet1_pt "             , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 25,  0., 250., "Leading jet p_{T} [GeV]"      , false, "gev");
  DrawPlot("TDilepMETJetPt "        , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 200., "p_{T}(e#mu j met) [GeV]"      , false, "gev");
  DrawPlot("TTHTtot "               , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20, 70., 550., "H_{T} [GeV]"                  , false, "gev");
  DrawPlot("TTJetLooseCentralpt "   , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 30,  0.,  30., "Loose jet p_{T} [GeV]"        , false, "gev");
  DrawPlot("TMSys "                 , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20, 50., 750., "m_{sys} [GeV]"                , false, "gev");
  DrawPlot("THTLepOverHT "          , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0.,   1., "H_{T}+lep / H_{T}"            , false, "units,0.05", 1e4);
  DrawPlot("TDilepJetPt "           , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 25,  0., 250., "p_{T}^{e#mu, j} [GeV]"        , false, "gev");
  DrawPlot("TLeadingJetE"           , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 500., "E^{j} [GeV]"                  , false, "gev");
  DrawPlot("TMET"                   , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 500., "#cancel{E}_{T} [GeV]"         , false, "gev");
  DrawPlot("TM_LeadingB"            , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 500., "m^{l_{1}b} [GeV]"             , false, "gev");
  DrawPlot("TM_SubLeadingB"         , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 500., "m^{l_{2}b} [GeV]"             , false, "gev");
  DrawPlot("TM_LLB"                 , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 800., "m^{llb} [GeV]"                , false, "gev");
  DrawPlot("TMT_LLMETB"             , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 800., "m_T^{ll #nu #nu b} [GeV]"     , false, "gev");
  DrawPlot("TE_LLB"                 , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 1000, "E^{llb} [GeV]"                , false, "gev");
  DrawPlot("TPt_LL"                 , "(TNJets == 1) && (TNBtags == 1) && (TIsSS == 0) ", "ElMu", 20,  0., 500., "p_T^{ll} [GeV]"               , false, "gev");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, bool setLegendLeft = false, TString yAxisTitleStyle="", float max = 0.){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  
  
  // Initial configuration
  
  p->SetPath(pathToTree); 
  p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
  p->verbose  = false;
//   p->verbose  = true;
  // p->SetVarName("forFit");
  TGaxis::SetMaxDigits(3);
  p->SetChLabel("e^{#pm}#mu^{#mp} + 1j1b");
  
  
  // Adding general-purpose samples
  
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
  p->AddSample("TTbar_PowhegSemi"             , "Non-W/Z"     , itBkg, 413);
  p->AddSample("WJetsToLNu_MLM"               , "Non-W/Z"     , itBkg, 413);
  p->AddSample("TW"                           , "tW"          , itBkg, TColor::GetColor("#ffcc33")); 
  p->AddSample("TbarW"                        , "tW"          , itBkg);

  p->AddSample("MuonEG"                       , "Data"        , itData);
  p->AddSample("SingleMuon"                   , "Data"        , itData);
  p->AddSample("SingleElec"                   , "Data"        , itData);
  
  
  // Adding samples for systematic uncertanties
  
  p->AddSample("TTbar_Powheg_hdampUp"         , "t#bar{t}"    , itSys, 1,  "hdampUp");
  p->AddSample("TTbar_Powheg_hdampDown"       , "t#bar{t}"    , itSys, 1,  "hdampDown");

  p->AddSample("TTbar_Powheg_ueUp"            , "t#bar{t}"    , itSys, 1,  "ueUp");
  p->AddSample("TTbar_Powheg_ueDown"          , "t#bar{t}"    , itSys, 1,  "ueDown");
  p->AddSample("TTbar_Powheg_isrUp"           , "t#bar{t}"    , itSys, 1,  "isrUp");
  p->AddSample("TTbar_Powheg_isrDown"         , "t#bar{t}"    , itSys, 1,  "isrDown");
  p->AddSample("TTbar_Powheg_fsrUp"           , "t#bar{t}"    , itSys, 1,  "fsrUp");
  p->AddSample("TTbar_Powheg_fsrDown"         , "t#bar{t}"    , itSys, 1,  "fsrDown");

  p->AddSample("TW_noFullyHadr_isrUp"         ,"tW"           , itSys, 1,  "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown"       ,"tW"           , itSys, 1,  "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"         ,"tW"           , itSys, 1,  "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown"       ,"tW"           , itSys, 1,  "fsrDown");

  p->AddSample("TW_noFullyHadr_MEscaleUp"     ,"tW"           , itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown"   ,"tW"           , itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp"      ,"tW"           , itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    ,"tW"           , itSys, 1,  "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      ,"tW"           , itSys, 1,  "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    ,"tW"           , itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  ,"tW"           , itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW"           , itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg"                 , "t#bar{t}"    , itSys, 1, "pdfDown"); 
  
  
  // Other settings
  p->doUncInLegend=true;
  p->SetRatioMin( 0.6 );
  p->SetRatioMax( 1.4 );
  
  if (max > 0) p->SetPlotMaximum(max);

  p->SetCMSlabel("CMS");

  p->SetLegendPosition(0.7, 0.45, 0.93, 0.92);

  p->SetPlotFolder("/nfs/fanae/user/vrbouza/www/TFM/1j1t/");

  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  p->AddSystematic("stat");

  p->doYieldsInLeg=false;
  p->doSetLogy = false;
  p->doData = true;
  
  if (setLegendLeft) p->SetLegendPosition("UL");
  p->AddLumiSyst(0.025);
  
  // Drawing and computations

  p->DrawStack("ElMu_0_log");
//   cout << "Done drawing stack!" << endl;
  
  p->PrintSystematics();
  p->PrintSystYields();

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
