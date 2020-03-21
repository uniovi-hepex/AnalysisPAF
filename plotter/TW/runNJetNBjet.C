#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, bool setLegendLeft=false );
// TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/jul25/";
TString pathToTree = "/pool/phedex/userstorage/sscruz/TW/aug21/";
TString NameOfTree = "Mini";


void runNJetNBjet() {
  DrawPlot("nJetsnBs( TNJets , TNBtags )", "(TIsSS == 0)", "ElMu", 7, -0.5, 6.5, "(Number of jets, number of b-tagged jets)");
  return;
}


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, bool setLegendLeft = false) {
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "");
//   p->verbose = false;
  p->verbose  = true;

  p->SetCanvasHeight(600);
  p->SetCanvasWidth(600);

  p->SetChLabel("e^{#pm}#mu^{#mp}");
//   p->SetChLabelPos(0.275, 0.89, -1);
  p->SetChLabelPos(0.34, 0.89, -1);
  p->SetLumi(35.864);

  vector<TString> labels = { "(0, 0)","(1, 0)","(1, 1)","(2, 0)","(2, 1)","(2, 2)", "(#geq3, #geq0)"};
  p->VBinLabels = labels;

//   p->chlabel="e^{#pm}#mu^{#mp}";
  p->SetChLabel("e^{#pm}#mu^{#mp}");
  p->SetTitleY("Events / bin");
  p->SetYaxisOffset(0.8);


  p->AddSample("TTbar_PowhegSemi"           , "Non-W/Z" , itBkg, 413);
  p->AddSample("WJetsToLNu_MLM"             , "Non-W/Z" , itBkg, 413);

  p->AddSample("TTWToLNu"   , "VV+t#bar{t}V", itBkg, 390);
  p->AddSample("TTWToQQ"    , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToQQ"    , "VV+t#bar{t}V", itBkg);
  p->AddSample("TTZToLLNuNu", "VV+t#bar{t}V", itBkg);
  p->AddSample("WZ"         , "VV+t#bar{t}V", itBkg);
  p->AddSample("WW"         , "VV+t#bar{t}V", itBkg);
  p->AddSample("ZZ"         , "VV+t#bar{t}V", itBkg);

  p->AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, 852); // kRed);
  p->AddSample("DYJetsToLL_M50_aMCatNLO"    , "DY", itBkg);// TColor::GetColor("#8ADCFF"));

  p->AddSample("TTbar_Powheg"               , "t#bar{t}", itBkg, 633);

  p->AddSample("TW"                         , "tW"      , itBkg, TColor::GetColor("#ffcc33")); // TColor::GetColor("#ffc878"));
  p->AddSample("TbarW"                      , "tW"      , itBkg);

  p->AddSample("MuonEG"    , "Data", itData);
  p->AddSample("SingleMuon", "Data", itData);
  p->AddSample("SingleElec", "Data", itData);

  p->AddSample("TTbar_Powheg_hdampUp"  , "t#bar{t}", itSys, 1,  "hdampUp"); 
  p->AddSample("TTbar_Powheg_hdampDown", "t#bar{t}", itSys, 1,  "hdampDown"); 

  p->AddSample("TTbar_Powheg_ueUp"   , "t#bar{t}", itSys, 1,  "ueUp"); 
  p->AddSample("TTbar_Powheg_ueDown" , "t#bar{t}", itSys, 1,  "ueDown"); 
  p->AddSample("TTbar_Powheg_isrUp"  , "t#bar{t}", itSys, 1,  "isrUp"); 
  p->AddSample("TTbar_Powheg_isrDown", "t#bar{t}", itSys, 1,  "isrDown"); 
  p->AddSample("TTbar_Powheg_fsrUp"  , "t#bar{t}", itSys, 1,  "fsrUp"); 
  p->AddSample("TTbar_Powheg_fsrDown", "t#bar{t}", itSys, 1,  "fsrDown"); 

  p->AddSample("TW_noFullyHadr_isrUp"  ,"tW", itSys, 1,  "isrUp");
  p->AddSample("TW_noFullyHadr_isrDown","tW", itSys, 1,  "isrDown");
  p->AddSample("TW_noFullyHadr_fsrUp"  ,"tW", itSys, 1,  "fsrUp");
  p->AddSample("TW_noFullyHadr_fsrDown","tW", itSys, 1,  "fsrDown");

  p->AddSample("TW_noFullyHadr_MEscaleUp"  ,"tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TbarW_noFullyHadr_isrUp"      ,"tW", itSys, 1,  "isrUp");
  p->AddSample("TbarW_noFullyHadr_isrDown"    ,"tW", itSys, 1,  "isrDown");
  p->AddSample("TbarW_noFullyHadr_fsrUp"      ,"tW", itSys, 1,  "fsrUp");
  p->AddSample("TbarW_noFullyHadr_fsrDown"    ,"tW", itSys, 1,  "fsrDown");
  p->AddSample("TbarW_noFullyHadr_MEscaleUp"  ,"tW", itSys, 1,  "tWMe_s_caleUp");
  p->AddSample("TbarW_noFullyHadr_MEscaleDown","tW", itSys, 1,  "tWMe_s_caleDown");

  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "ScaleDown"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfUp"); 
  p->AddSample("TTbar_Powheg", "t#bar{t}", itSys, 1, "pdfDown"); 

  p->AddLumiSyst(0.025);
  p->AddSystematic("stat,JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"); //,LepEff
  //p->AddSystematic("stat"); //,LepEff

  p->SetDataStyle("psameE1");
  p->SetSignalStyle("SM");
  p->doUncInLegend = true;
  p->SetRatioMin( 0.6 );
  p->SetRatioMax( 1.4 );
  p->SetPadPlotMargins("0.06, 0.1, 0.04, 0.1");
  p->SetPadRatioMargins("0.03, 0.4, 0.04, 0.1");
  p->SetTexChanSize(0.06);
  p->SetTextLumiPosX(0.69);
  p->SetYratioOffset(0.35);
  p->SetCenterYAxis(false);
  p->SetXaxisOffset(1.1);
  p->SetPlotMaximum(120000);
  p->ObliterateXErrorBars();

  p->SetCMSlabel("CMS");
  p->SetCMSmodeLabel("Preliminary");

  p->SetPlotFolder("/nfs/fanae/user/vrbouza/www/TFM/1j1t/control/");

  //p->SetLegendPosition(0.66, 0.65, 0.98, 0.85);
//   p->SetLegendPosition(0.4, 0.52, 0.63, 0.92);
  p->SetLegendPosition(0.58, 0.52, 0.81, 0.92);
  p->SetLegendTextSize(0.055);
  p->SetCMSlabel("CMS");

  p->doData        = true;
  p->doYieldsInLeg = false;
  p->doSetLogy     = false;
  p->doSignal      = false;

  if (setLegendLeft) p->SetLegendPosition("UL");
  p->DrawStack();

  delete p;
}


void runLooper() {
  Looper* p = new Looper(pathToTree, NameOfTree, "TMET", "TMET > 0", "ElMu", 30, 0, 300);
  Histo* h = p->GetHisto("TW");
  h->SetLineColor(kRed);
  cout << "nEntries = " << h->GetEntries() << endl;
  cout << "Yield = " << h->Integral()*35900 << endl;
  h->Draw();
}
