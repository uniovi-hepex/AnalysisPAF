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
const TString TTWmc[] 	  = {"TTWToLNu", "TTWToQQ"};		                  // ttW
const TString TTZmc[] 	  = {"TTZToLLNuNu", "TTZToQQ"};                  // ttZ
const TString WZmc[] 	    = {"WZTo3LNu"};                                 // WZ
const TString Convsmc[]   = {"WGToLNuG", "ZGTo2LG", "TGJets", "TTGJets"}; // Convs
const TString Fakesmc[]   = {"TTbar_Powheg", "WJetsToLNu_MLM", "TW", 
                             "TbarW", "T_tch", "Tbar_tch", 
                             "TToLeptons_sch_amcatnlo", "DYJetsToLL_M50_MLM", 
                             "DYJetsToLL_M5to50_MLM", "WW", "WWTo2L2Nu", "WpWpJJ"}; // Fakes (ttbar with Powheg)
/*const TString Fakesmc[]   = {"TTbar_aMCatNLO", "WJetsToLNu_MLM", "TW", 
                             "TbarW", "T_tch", "Tbar_tch", 
                             "TToLeptons_sch_amcatnlo", "DYJetsToLL_M50_MLM", 
                             "DYJetsToLL_M5to50_MLM", "WW", "WWTo2L2Nu", "WpWpJJ"}; // Fakes (ttbar with aMC@NLO)*/
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
  DrawPlot("TCat",                cut, chan, 1, 2, 5,       "Category",         "Categories", tag); // This one is only for obtaining the xsection.
  DrawPlot("TnTightLepton",       cut, chan, 6, -0.5, 5.5,  "nTightLep (#)",    "nTightLepton", tag);
  DrawPlot("TnFakeableLepton",    cut, chan, 6, -0.5, 5.5,  "nFakeLep (#)",     "nFakeLepton", tag);
  DrawPlot("TnLooseLepton",       cut, chan, 6, -0.5, 5.5,  "nLooseLep (#)",    "nLooseLepton", tag);
  DrawPlot("TnTaus",              cut, chan, 3, -0.5, 2.5,  "nTaus (#)",        "nTaus", tag);
  DrawPlot("TnJets",              cut, chan, 10, -0.5, 9.5, "nJets (#)",        "nJets", tag);
  DrawPlot("TnMediumBTags",       cut, chan, 6, -0.5, 5.5,  "nMediumBTags (#)", "nMediumBTags", tag);
  DrawPlot("TnLooseBTags",        cut, chan, 6, -0.5, 5.5,  "nLooseBTags (#)",  "nLooseBTags", tag);
  DrawPlot("TPtLeading",          cut, chan, 10, 0, 200,    "p_{T} (GeV)",      "PtLeading", tag);
  DrawPlot("TPtSubLeading",       cut, chan, 10, 0, 200,    "p_{T} (GeV)",      "PtSubLeading", tag);
  DrawPlot("TPtSubSubLeading",    cut, chan, 10, 0, 200,    "p_{T} (GeV)",      "PtSubSubLeading", tag);
  DrawPlot("TPtSubSubSubLeading", cut, chan, 10, 0, 200,    "p_{T} (GeV)",      "PtSubSubSubLeading", tag);
  DrawPlot("TPtVector",           cut, chan, 10, 0, 200,    "p_{T} (GeV)",      "PtVector", tag);
  DrawPlot("TMET",                cut, chan, 10, 0, 400,    "#slash{E}_{T} (GeV)","MET", tag);
  DrawPlot("TMHT",                cut, chan, 10, 0, 400,    "#slash{H}_{T} (GeV)","MHT", tag);
  DrawPlot("THT",                 cut, chan, 10, 0, 1000,   "H_{T} (GeV)",      "HT", tag);
  DrawPlot("TMETLD",              cut, chan, 10, 0, 2,      "#slash{E}_{T}LD (GeV)","METLD", tag);
  DrawPlot("TCS",                 cut, chan, 7, -3.5, 3.5,  "Sum of charges",   "CS", tag);
  DrawPlot("TMass",               cut, chan, 10, 0, 400,    "M_{ll} (GeV)",     "Mass", tag);
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
  if (counter < 2) {                                                               // FOR YIELDS, XSEC & DATACARDS
    for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {   // ttW
      p->AddSample(TTWmc[isample], "ttW", itBkg, kGreen-5);
      //p->AddSample(TTWmc[isample], "ttW", itSys, 1, "ScaleUp");
      //p->AddSample(TTWmc[isample], "ttW", itSys, 1, "ScaleDown");
      if (counter != 1) {
    	  p->AddSample(TTWmc[isample], "ttW", itSys, 1, "NormttWUp");
    	  p->AddSample(TTWmc[isample], "ttW", itSys, 1, "NormttWDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("ttW_NormttWUp",    1.12);
      p->ScaleSys("ttW_NormttWDown",  0.88);
    }
    /*p->AddSample("TTWToLNu",    "ttW", itSys, 1, "pdfUp", "HeppySampleName: TTWToLNu_ext1 & TTWToLNu_ext2");
    p->AddSample("TTWToQQ",     "ttW", itSys, 1, "pdfUp");
    p->AddSample("TTWToLNu",    "ttW", itSys, 1, "pdfDown", "HeppySampleName: TTWToLNu_ext1 & TTWToLNu_ext2");
    p->AddSample("TTWToQQ",     "ttW", itSys, 1, "pdfDown");*/
    
    
    for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {   // ttZ
      p->AddSample(TTZmc[isample], "ttZ", itBkg, kSpring+2);
      //p->AddSample(TTZmc[isample], "ttZ", itSys, 1, "ScaleUp");
      //p->AddSample(TTZmc[isample], "ttZ", itSys, 1, "ScaleDown");
      if (counter != 1) {
        p->AddSample(TTZmc[isample], "ttZ", itSys, 1, "NormttZUp");
        p->AddSample(TTZmc[isample], "ttZ", itSys, 1, "NormttZDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("ttZ_NormttZUp",    1.10);
      p->ScaleSys("ttZ_NormttZDown",  0.90);
    }
    /*p->AddSample("TTZToLLNuNu", "ttZ", itSys, 1, "pdfUp", "HeppySampleName: TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2");
    p->AddSample("TTZToQQ",     "ttZ", itSys, 1, "pdfUp");
    p->AddSample("TTZToLLNuNu", "ttZ", itSys, 1, "pdfDown", "HeppySampleName: TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2");
    p->AddSample("TTZToQQ",     "ttZ", itSys, 1, "pdfDown");*/
    
    
    for (UInt_t isample = 0; isample < sizeof(WZmc)/sizeof(*WZmc); isample++) {   // WZ
      p->AddSample(WZmc[isample], "WZ", itBkg, kViolet+10);
      if (counter != 1) {
        p->AddSample(WZmc[isample], "WZ", itSys, 1, "NormWZUp");
        p->AddSample(WZmc[isample], "WZ", itSys, 1, "NormWZDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("WZ_NormWZUp",    1.30);
      p->ScaleSys("WZ_NormWZDown",  0.70);
    }
    
    
    for (UInt_t isample = 0; isample < sizeof(Convsmc)/sizeof(*Convsmc); isample++) {
  	  p->AddSample(Convsmc[isample], "Convs", itBkg, kYellow);
      if (counter != 1) {
        p->AddSample(Convsmc[isample], "Convs", itSys, 1, "NormConvsUp");
        p->AddSample(Convsmc[isample], "Convs", itSys, 1, "NormConvsDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("Convs_NormConvsUp",    1.30);
      p->ScaleSys("Convs_NormConvsDown",  0.70);
    }
    
    
    for (UInt_t isample = 0; isample < sizeof(Fakesmc)/sizeof(*Fakesmc); isample++) {
  	  p->AddSample(Fakesmc[isample], "Fakes", itBkg, kOrange-7);
      if (counter != 1) {
        p->AddSample(Fakesmc[isample], "Fakes", itSys, 1, "NormFakesUp");
        p->AddSample(Fakesmc[isample], "Fakes", itSys, 1, "NormFakesDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("Fakes_NormFakesUp",    1.30);
      p->ScaleSys("Fakes_NormFakesDown",  0.70);
    }
    
    
    for (UInt_t isample = 0; isample < sizeof(Raresmc)/sizeof(*Raresmc); isample++) {
  	  p->AddSample(Raresmc[isample], "Rares", itBkg, kAzure-9);
      if (counter != 1) {
        p->AddSample(Raresmc[isample], "Rares", itSys, 1, "NormRaresUp");
        p->AddSample(Raresmc[isample], "Rares", itSys, 1, "NormRaresDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("Rares_NormRaresUp",    1.30);
      p->ScaleSys("Rares_NormRaresDown",  0.70);
    }
    
    
    for (UInt_t isample = 0; isample < sizeof(Data)/sizeof(*Data); isample++) {
  	  p->AddSample(Data[isample], "Data", itData, kBlack);
    }
    
    
    for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
	    p->AddSample(Signalmc[isample], "ttH", itSignal, kRed);
      //p->AddSample(Signalmc[isample], "ttH", itSys, 1, "ScaleUp");
      //p->AddSample(Signalmc[isample], "ttH", itSys, 1, "ScaleDown");
      if (counter != 1) {
        p->AddSample(Signalmc[isample], "ttH", itSys, 1, "NormttHUp");
        p->AddSample(Signalmc[isample], "ttH", itSys, 1, "NormttHDown");
      }
    }
    if (counter != 1) {
      p->ScaleSys("ttH_NormttHUp", 1.058);
      p->ScaleSys("ttH_NormttHDown", 0.902);
    }
    //p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfUp");
    //p->AddSample(Signalmc[0], "ttH", itSys, 1, "pdfDown");
  }
  else {
    if (var != "TPtVector") {
      for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
        p->AddSample(TTWmc[isample], "t#bar{t}W", itBkg, kGreen-5);
      }
      for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
    	  p->AddSample(TTZmc[isample], "t#bar{t}Z", itBkg, kSpring+2);
      }
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
      for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
        p->AddSample(Signalmc[isample], "t#bar{t}H", itBkg, kRed);
      }
    }
    else {
      for (UInt_t isample = 0; isample < sizeof(TTWmc)/sizeof(*TTWmc); isample++) {
        p->AddSample(TTWmc[isample], "t#bar{t}W", itBkg, kGreen-5, "0", "AllInstances");
      }
      for (UInt_t isample = 0; isample < sizeof(TTZmc)/sizeof(*TTZmc); isample++) {
    	  p->AddSample(TTZmc[isample], "t#bar{t}Z", itBkg, kSpring+2, "0", "AllInstances");
      }    
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
      for (UInt_t isample = 0; isample < sizeof(Signalmc)/sizeof(*Signalmc); isample++) {
        p->AddSample(Signalmc[isample], "t#bar{t}H", itBkg, kRed, "0", "AllInstances");
      }
    }
  }
  // Histogram settings ========================================================
  p->SetScaleMax(1.7);
  p->SetRatioMin(0);
  p->SetRatioMax(2);
  p->doSetLogy = false;
  
  // Errors ====================================================================
  p->AddSystematic("stat, Trig, PU, MuonEff, ElecEff, JES");
  
  // Yields table and cross section settings, histograms plotting ==============
  if (counter == 0) { // YIELDS & DATACARDS 
    // Yields
    p->SetTableFormats("%1.4f");
    p->SetYieldsTableName("Yields_"+chan+"_"+tag);
    p->PrintYields("","","","txt");
    
    p->SetLimitFolder(outputpath);
    p->SetOutputName("Histos_"+chan+"_"+tag);
    p->SaveHistograms();
    
    
    // Datacard
    const TString Bkgs      = " ttW,  ttZ,    WZ,   Convs,  Fakes,  Rares";
    //const TString BkgsNorm  = "1.12,  1.10,   1.3,  1.3,    1.3,    1.3,   1.098";
    const TString BkgsNorm  = "1.12,  1.10,   1.3,  1.3,    1.3,    1.3,   1";
    //const TString Sys       = "Trig, PU, MuonEff, ElecEff, JES, Scale, pdf";
    //const TString Sys       = "Trig, PU, MuonEff, ElecEff, JES, Scale, pdf, NormttH";
    const TString Sys       = "Trig, PU, MuonEff, ElecEff, JES, NormttH";
    Datacard *d = new Datacard("ttH",Bkgs,Sys,chan);
    
    d->SetPathToFile(outputpath);
    d->SetLumiUnc(1+2.6/100);
    d->SetNormUnc(BkgsNorm);
    d->SetRootFileName("Histos_"+chan+"_"+tag);
    d->GetParamsFormFile();
    d->PrintDatacard(outputpath+"Datacard_"+name+"_"+chan+"_"+tag+".txt");
  }
  else if (counter == 1) {// CROSS SECTION
    // Cross section
    p->SetSignalStyle("xsec");
    CrossSection *x = new CrossSection(p, "ttH");
    x->SetTheoXsec(0.2150955);
    x->SetChannelTag(chan);
    
    x->SetUnc("ttW",    0.12);
    x->SetUnc("ttZ",    0.10);
    x->SetUnc("WZ",     0.30);
    x->SetUnc("Convs",  0.30);
    x->SetUnc("Fakes",  0.30);
    x->SetUnc("Rares",  0.30);
    
    x->SetEfficiencySyst("Trig, PU, MuonEff, ElecEff, JES");
    //x->SetAcceptanceSyst("stat, Scale, pdf");
    x->SetAcceptanceSyst("stat");
    
    x->SetOutputFolder(outputpath);
    x->SetXsecTableName("Xsec_"+chan+"_"+tag);
    x->SetTableName("Xsec_unc_"+chan+"_"+tag);
    
    x->PrintSystematicTable("txt");
    x->PrintCrossSection("txt");
  }
  else {// PLOT - DRAWINGS
    p->SetCMSlabel("CMS Academic");
    p->DrawStack(tag, 1);
  }
  
  counter++;
  delete p;
}
