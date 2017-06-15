R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(CrossSection.C+)
R__LOAD_LIBRARY(Datacard.C)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TResultsTable.h"
#include "Datacard.h"
#include <iostream>
#include <fstream>

const TString Signalmc[]  = {"TTHNonbb"};                                 // ttH
const TString TTWmc[] 	  = {"TTWToLNu", "TTWToQQ"};		                  // TTW
const TString TTZmc[] 	  = {"TTZToLLNuNu", "TTZToQQ"};                  // TTZ
const TString WZmc[] 	    = {"WZTo3LNu"};                                 // WZ
const TString Convsmc[]   = {"WGToLNuG", "ZGTo2LG", "TGJets", "TTGJets"}; // Convs
const TString Fakesmc[]   = {"TTbar_Powheg", "WJetsToLNu_MLM", "TW", 
                             "TbarW", "T_tch", "Tbar_tch", 
                             "TToLeptons_sch_amcatnlo", "DYJetsToLL_M50_MLM", 
                             "DYJetsToLL_M5to50_MLM", "WW", "WWTo2L2Nu"}; // Fakes (ttbar with Powheg)
/*const TString Fakesmc[]   = {"TTbar_aMCatNLO", "WJetsToLNu_MLM", "TW", 
                             "TbarW", "T_tch", "Tbar_tch", 
                             "TToLeptons_sch_amcatnlo", "DYJetsToLL_M50_MLM", 
                             "DYJetsToLL_M5to50_MLM", "WW", "WWTo2L2Nu"}; // Fakes (ttbar with aMC@NLO)*/
const TString Raresmc[]   = {"WWW", "WWZ", "WZZ", "ZZZ", "ZZ", "tZq_ll", 
                             "TTTT"};                                     // Rares
const TString Data[]      = {"MuonEG", "SingleMuon", "SingleElec", "DoubleEG", "DoubleMuon"};        // Data samples
UInt_t counter = 0;


void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "", TString tag = "0");


void DrawPlots(TString chan = "ElMu", TString tag = "0"){
  TString cut = "((TCat == 2 && (TChannel == 1 || TChannel == 2 || TChannel == 3)) || (TCat == 3 && TChannel == 5) || (TCat == 4 && TChannel == 6))";
  if (chan == "2lSS")     cut = "(TCat == 2 && (TChannel == 1 || TChannel == 2 || TChannel == 3))";
  else if (chan == "Elec" || chan == "Muon" || chan == "ElMu") cut = "(TCat == 2)";
  else if (chan == "3l")  cut = "(TCat == 3)";
  else if (chan == "4l")  cut = "(TCat == 4)";
  
  DrawPlot("TCat",                cut, chan, 3, 2, 5,       "Category",         "Categories", tag); // This one is only for getting yields and for the Datacard.
  DrawPlot("TnTightLepton",       cut, chan, 6, -0.5, 5.5,  "nTightLep (#)",    "nTightLepton", tag);
  DrawPlot("TnFakeableLepton",    cut, chan, 5, -0.5, 5.5,  "nFakeLep (#)",     "nFakeLepton", tag);
  DrawPlot("TnLooseLepton",       cut, chan, 5, -0.5, 5.5,  "nLooseLep (#)",    "nLooseLepton", tag);
  DrawPlot("TnTaus",              cut, chan, 3, -0.5, 2.5,  "nTaus (#)",        "nTaus", tag);
  DrawPlot("TnJets",              cut, chan, 10, -0.5, 9.5, "nJets (#)",        "nJets", tag);
  DrawPlot("TnMediumBTags",       cut, chan, 6, -0.5, 5.5,  "nMediumBTags (#)", "nMediumBTags", tag);
  DrawPlot("TnLooseBTags",        cut, chan, 6, -0.5, 5.5,  "nLooseBTags (#)",  "nLooseBTags", tag);
  DrawPlot("TPtLeading",          cut, chan, 10, 0, 200,    "Pt (GeV)",         "PtLeading", tag);
  DrawPlot("TPtSubLeading",       cut, chan, 10, 0, 200,    "Pt (GeV)",         "PtSubLeading", tag);
  DrawPlot("TPtSubSubLeading",    cut, chan, 10, 0, 200,    "Pt (GeV)",         "PtSubSubLeading", tag);
  DrawPlot("TPtSubSubSubLeading", cut, chan, 10, 0, 200,    "Pt (GeV)",         "PtSubSubSubLeading", tag);
  DrawPlot("TPtVector",           cut, chan, 10, 0, 200,    "Pt (GeV)",         "PtVector", tag);
  DrawPlot("TMET",                cut, chan, 10, 0, 400,    "MET (GeV)",        "MET", tag);
  DrawPlot("TMHT",                cut, chan, 10, 0, 400,    "MHT (GeV)",        "MHT", tag);
  DrawPlot("THT",                 cut, chan, 10, 0, 1000,   "HT (GeV)",         "HT", tag);
  DrawPlot("TMETLD",              cut, chan, 10, 0, 2,      "METLD (GeV)",      "METLD", tag);
  DrawPlot("TCS",                 cut, chan, 7, -3.5, 3.5,  "Sum of charges",   "CS", tag);
  DrawPlot("TMass",               cut, chan, 10, 0, 400,    "M_ll (GeV)",       "Mass", tag);
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name, TString tag = "0") {
  Plot* p = NULL;
  if (chan == "2lSS") {
    p     = new Plot(var, cut, "All", nbins, bin0, binN, "Title", Xtitle);
  }
  else {
    p     = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  }
  
  p->verbose  = true;
  
  
  // Determination of path and output path =====================================
  TString   outputpath = "/nfs/fanae/user/vrbouza/www/Results/";
  
  string    cline;
  ifstream  gitinfo("../.git/HEAD");
  getline(gitinfo,cline);
  TString   githead(cline);
  string    cpath = __FILE__ ;
  TString   path(cpath);
  path      = path(0,path.Index("AnalysisPAF/")+12);
  path      += "ttH_temp/";
  
  if (githead.Contains("lepidcomparison")) {
    if (counter == 0){
      cout << endl;
      cout << "+ Branch LEPIDCOMPARISON chosen" << endl;
      cout << endl;
    }
    outputpath  += "lepidcomparison/";
    path        += "lepidcomparison/";
    if      (tag == "top")  path += "top/";
    else if (tag == "Stop") path += "Stop/";
    else if (tag == "ttH")  path += "ttH/";
  }
  else if (githead.Contains("random")) {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch RANDOM chosen" << endl;
      cout << endl;
    }
    path        += "random/";
    outputpath  += "random/";
  }
  else if (githead.Contains("lepMVAcomparison")) {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch LEPMVACOMPARISON chosen" << endl;
      cout << endl;
    }
    path        += "lepMVAcomparison/";
    outputpath  += "lepMVAcomparison/";
    if      (tag == "et")     path += "extratight/";
    else if (tag == "vt")     path += "verytight/";
    else if (tag == "t")      path += "tight/";
    else if (tag == "m")      path += "medium/";
    else if (tag == "tth")    path += "tth/";
    else if (tag == "tth95")  path += "tth95/";
    else if (tag == "tth97")  path += "tth97/";
  }
  else if (githead.Contains("test2")) {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch TEST2 chosen" << endl;
      cout << endl;
    }
    outputpath  += "test2/";
  }
  else if (githead.Contains("test")) {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch TEST chosen" << endl;
      cout << endl;
    }
    outputpath  += "test/";
  }
  else if (githead.Contains("tauidcomparison")) {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch TAUIDCOMPARISON chosen" << endl;
      cout << endl;
    }
    path        += "tauidcomparison/";
    outputpath  += "tauidcomparison/";
    if      (tag == "tau")    path += "tau/";
    else if (tag == "tth")    path += "tth/";
  }
  else {
    if (counter == 0) {
      cout << endl;
      cout << "+ Branch MASTER chosen" << endl;
      cout << endl;
    }
  }
  
  if (counter == 0) {
    cout << "+ Path to the root files: " << path << endl;
    cout << "+ Output path: " << outputpath << endl;
    cout << endl;
  }
    
  p->SetPlotFolder(outputpath);
  p->SetPath(path);
  p->SetPathSignal(path);
  
  
  // Minitree settings =========================================================
  p->SetTreeName("MiniTree");
  if (chan == "Elec" || chan == "Muon" || chan == "ElMu") name = name+"_2lSS";
  p->SetVarName(name+"_"+chan);
  
    
  // Samples import ============================================================
  if (var != "TPtVector") {
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itBkg, kGreen-5);
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itBkg, kSpring+2);
    }
    /*
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "ScaleUp");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "ScaleUp");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "ScaleDown");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "ScaleDown");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "pdfUp");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "pdfUp");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "pdfDown");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "pdfDown");
    }*/
    
    for (UInt_t isample = 0; isample < sizeof(WZmc)/sizeof(*WZmc); isample++) {
  	  p->AddSample(WZmc[isample], "WZ", itBkg, kViolet+10);
    }
    for (UInt_t isample = 0; isample < sizeof(Convsmc)/sizeof(*Convsmc); isample++) {
  	  p->AddSample(Convsmc[isample], "Convs", itBkg, kYellow);
    }
    for (UInt_t isample = 0; isample < sizeof(Fakesmc)/sizeof(*Fakesmc); isample++) {
  	  p->AddSample(Fakesmc[isample], "Fakes", itBkg, kOrange-7);
    }
    for (UInt_t isample = 0; isample < sizeof(Raresmc)/sizeof(*Raresmc); isample++) {
  	  p->AddSample(Raresmc[isample], "Rares", itBkg, kAzure-9);
    }
    for (UInt_t isample = 0; isample < sizeof(Data)/sizeof(*Data); isample++) {
  	  p->AddSample(Data[isample], "Data", itData,kBlack);
    }
    if (counter == 0) {
      for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
  	    p->AddSample(Signalmc[isample], "ttH", itSignal, kRed);
      }
    } else {
      p->AddSample(Signalmc[0], "ttH", itBkg, kRed);
    }
    /*
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "NormttHUp");
    p->ScaleSys("ttH_NormttHUp", 1.058);
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "NormttHDown");
    p->ScaleSys("ttH_NormttHDown", 0.902);
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "ScaleUp");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "ScaleDown");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfUp");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfDown");
  */
  }
  else {
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itBkg, kGreen-5, "0", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itBkg, kSpring+2, "0", "AllInstances");
    }
    /*
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "ScaleUp", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "ScaleUp", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "ScaleDown", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "ScaleDown", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "pdfUp", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "pdfUp", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
      p->AddSample(TTWmc[isample], "TTW", itSys, 1, "pdfDown", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
  	  p->AddSample(TTZmc[isample], "TTZ", itSys, 1, "pdfDown", "AllInstances");
    }*/
    
    for (UInt_t isample = 0; isample < sizeof(WZmc)/sizeof(*WZmc); isample++) {
  	  p->AddSample(WZmc[isample], "WZ", itBkg, kViolet+10, "0", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(Convsmc)/sizeof(*Convsmc); isample++) {
  	  p->AddSample(Convsmc[isample], "Convs", itBkg, kYellow, "0", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(Fakesmc)/sizeof(*Fakesmc); isample++) {
  	  p->AddSample(Fakesmc[isample], "Fakes", itBkg, kOrange-7, "0", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(Raresmc)/sizeof(*Raresmc); isample++) {
  	  p->AddSample(Raresmc[isample], "Rares", itBkg, kAzure-9, "0", "AllInstances");
    }
    for (UInt_t isample = 0; isample < sizeof(Data)/sizeof(*Data); isample++) {
  	  p->AddSample(Data[isample], "Data", itData,kBlack, "0", "AllInstances");
    }
    if (counter == 0) {
      for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
  	    p->AddSample(Signalmc[isample], "ttH", itSignal, kRed, "0", "AllInstances");
      }
    } else {
      p->AddSample(Signalmc[0], "ttH", itBkg, kRed, "0", "AllInstances");
    }
    /*
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "NormttHUp", "AllInstances");
    p->ScaleSys("ttH_NormttHUp", 1.058);
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "NormttHDown", "AllInstances");
    p->ScaleSys("ttH_NormttHDown", 0.902);
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "ScaleUp", "AllInstances");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "ScaleDown", "AllInstances");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfUp", "AllInstances");
    p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfDown", "AllInstances");
    */
  }
  // Histogram settings ========================================================
  p->SetScaleMax(1.7);
  p->SetRatioMin(0);
  p->SetRatioMax(2);
  p->doSetLogy = false;
  
  // Errors ====================================================================
  p->AddSystematic("stat,Trig,PU,MuonEff,ElecEff,JES");
  
  // Yields table and cross section settings, histograms plotting ==============
  if (counter == 0) {
    // Yields
    p->SetTableFormats("%1.4f");
    p->SetYieldsTableName("Yields_"+chan+"_"+tag);
    p->PrintYields("","","","txt");
    
    
    p->SetLimitFolder(outputpath);
    p->SetOutputName("Histos_"+chan+"_"+tag);
    p->SaveHistograms();
    
    // Cross section
    p->SetSignalStyle("xsec");
    CrossSection *x = new CrossSection(p, "ttH");
    x->SetTheoXsec(0.2150955);
    x->SetChannelTag("chan");
    
    x->SetEfficiencySyst("Trig, PU, MuonEff, ElecEff, JES");
//    x->SetAcceptanceSyst("stat, Scale, pdf");
    x->SetAcceptanceSyst("stat");
    
    x->SetOutputFolder(outputpath);
    x->SetXsecTableName("Xsec_"+chan+"_"+tag);
    x->SetTableName("Xsec_unc_"+chan+"_"+tag);
    
    x->PrintSystematicTable("txt");
    x->PrintCrossSection("txt");
    
    // Datacard
    const TString Bkgs      = " TTW,  TTZ,    WZ,   Convs,  Fakes,  Rares";
    //const TString BkgsNorm  = "1.12, 1.10,   1.3,     1.3,    1.3,    1.3";
    const TString BkgsNorm  = "1.12, 1.10,   1.3,     1.3,    1.3,    1.3,   1.098";
    //const TString Sys       = "Trig, PU, MuonEff, ElecEff, JES, Scale, pdf";
    const TString Sys       = "Trig, PU, MuonEff, ElecEff, JES";
    Datacard *d = new Datacard("ttH",Bkgs,Sys,chan);
    
    d->SetPathToFile(outputpath);
    d->SetLumiUnc(1+2.6/100);
    d->SetNormUnc(BkgsNorm);
    d->SetRootFileName("Histos_"+chan+"_"+tag);
    d->GetParamsFormFile();
    d->PrintDatacard(outputpath+"Datacard_"+name+"_"+chan+"_"+tag+".txt");
  }
  else {
    p->DrawStack(tag, 1);
  }
  
  if (counter == 0) counter = 1;
  delete p;
}
