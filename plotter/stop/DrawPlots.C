//#########################################################################################
//
// Drawing plots
// Select you option:
//   - Draw plots with data (control plots)
//   - Draw plots with S/B ratio and no data
//   - Print nonprompt lepton background estimate (table and plots)
//   - Print yields table
//
// HowTo:
// First, select your options below...
// root -l -b -1 'DrawPlots.C("ElMu")'
//
//#########################################################################################

R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"
#include "PlotterFunctions.C"

//=== Functions 
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins = 0, TString Xtitle = "", TString name = "");
void PrintNonWZestimate(TString chan = "ElMu", TString cut = "");
void PrintYieldTalbe(TString chan = "ElMu", TString cut = "");
void DrawSignalPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name);

//=== Define paths and tree name
TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/";
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/jan5/";
TString NameOfTree = "tree";
TString outputFolder = "./output/ControlPlots_topPtWeight/"; //_topPtWeight

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins = 21;
Float_t gbins5b[] =  {0,5, 40, 80, 100, 200}; Int_t ngbins5b = 5;
TString binLabelsSR = "150-200, > 200,  0.4, 0.8, 1.2, 1.6, 2.0, > 2.0, 20, 40, 60, 80, 100, > 100";

//=== Change here your options
Bool_t _PrintDataPlots   = true; Bool_t doAllSyst  = true;
Bool_t _PrintWZestimate  = false;
Bool_t _PrintYieldTable  = false;
Bool_t _PrintSignalPlots = false;
Bool_t doTopPtReweighting = false;
TString systematics = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU";

void DrawPlots(TString chan = "ElMu"){
  TString  cut = BaselineCut;
  // TString weight = "TWeight";
  //>>> if b-tag requirement --> weight = "TWeight*TBtagSFFS";

  if(_PrintDataPlots){ //>>> Plots with data (Control plots)
    cut = Dilepton;
//    DrawPlot("TNJets", cut, chan, 9, 0, 8, 0, "#Jets", "NJets");
//    DrawPlot("NBtagNJets(TNJets,TNBtags)", cut, chan, 16, 0, 15, 0, "[#Jets|#BJets]", "NBtagsNJets");
    cut = BaselineCut;
    DrawPlot("TMET",  cut + " && TMET > 100", chan, 10,    100,  400, 0, "MET [GeV]",                    "MET");
    //DrawPlot("TMT2",        cut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2_21");
//    DrawPlot("GetMT2orDeltaEtaBins(TMT2, TMET, TDeltaEta)", cut, chan, 14, 1, 15, 0, "Signal regions", "SignalRegions");
//    DrawPlot("TDeltaPhi",   cut, chan, 30,    0, 3.15, 0, "#Delta#varphi_{#font[12]{ll}} [rad]",   "DeltaPhi");
/*    DrawPlot("TLep0_Pt",    cut, chan, 25,   25,  250, 0, "Leading lepton p_{T} [GeV]",   "Lep0Pt");
    DrawPlot("TMll",        cut, chan, 40,    0,  600, 0, "M_{#font[12]{ll}} [GeV]",               "InvMass");
    DrawPlot("DilepPt(TLep0_Pt, TLep0_Eta, TLep0_Phi, TLep0_E, TLep1_Pt, TLep1_Eta, TLep1_Phi, TLep1_E)",    cut, chan, 30,    0,  300, 0, "p_{T}^{#font[12]{ll}} [GeV]",  "DilepPt");
    DrawPlot("TDeltaEta",   cut, chan, 30,    0,  2.4, 0, "#Delta#eta_{#font[12]{ll}} [rad]",      "DeltaEta");
//    DrawPlot("TLep1_Pt",    cut, chan, 30,    0,  200, 0, "Subleading lepton p_T [GeV]",  "Lep1Pt");
//    DrawPlot("TLep0_Eta",   cut, chan, 20, -2.4,  2.4, 0, "Leading lepton #eta [rad]",    "Lep0Eta");
//    DrawPlot("TLep1_Eta",   cut, chan, 20, -2.4,  2.4, 0, "Subleading lepton #eta [rad]", "Lep1Eta");
//    DrawPlot("TChannel",    cut, chan, 1,     0,   10, 0, "Events",                       "Events");
    //DrawPlot("TJet_Eta[0]", cut, chan, 20, -2.4,  2.4, 0, "Leading jet #eta [rad]",       "Jet0Eta");
    //DrawPlot("TJet_Eta[1]", cut, chan, 20, -2.4,  2.4, 0, "Subleading jet #eta [rad]",    "Jet1Eta");
    //DrawPlot("TJet_Pt[0]",  cut, chan, 27,   30,  300, 0, "Leading jet p_T [GeV]",        "Jet0Pt");
    //DrawPlot("TJet_Pt[1]",  cut, chan, 30,    0,  300, 0, "Subleading jet p_T [GeV]",     "Jet1Pt");
    DrawPlot("TNBtags", cut, chan, 5, 0, 4, 0, "#b jets", "NBtags");
*/  }
  
  //>>> Nonprompt estimate
  if(_PrintWZestimate)  PrintNonWZestimate(chan, cut);

  //>>> Table of yields
  if(_PrintYieldTable)  PrintYieldTalbe(chan, cut);

  //>>> Plot with S/B ratio
  if(_PrintSignalPlots){
    DrawSignalPlot("TMT2",        cut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2_21");
    //DrawSignalPlot("TMT2",        cut, chan, ngbins5b, 0, 0, gbins5b, "M_{T2} [GeV]", "MT2_5b");
    //DrawSignalPlot("TMET",        cut, chan, 40,    0,  400, 0, "MET [GeV]",                    "MET");
    //DrawSignalPlot("TDeltaPhi",   cut, chan, 30,    0, 3.15, 0, "#Delta#varphi_{#font[12]{ll}} [rad]",   "DeltaPhi");
    //DrawSignalPlot("TDeltaEta",   cut, chan, 30,    0,  2.4, 0, "#Delta#eta_{#font[12]{ll}} [rad]",      "DeltaEta");
  }
  return;
}


// #########################################################################
// ## Plots with Data
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name){
  Plot *p;
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = true;
  if(name != "") p->SetVarName(name + "_" + chan);

  //>>> Set style
  p->SetOutputName("Data");
  p->SetLegendTextSize(0.055);
  p->SetLegendPosition(0.60, 0.30, 0.93, 0.93);
  if(var.Contains("Eta" || var.Contains("eta"))){
    p->SetLegendTextSize(0.035);
    p->SetLegendPosition(0.70, 0.70, 0.93, 0.93);
  }
  else if(var == "TDeltaPhi"){
    p->SetLegendTextSize(0.035);
    p->SetLegendPosition(0.5, 0.70, 0.7, 0.93);
  }

  p->SetRatioErrorColor(kTeal-2);
  p->SetRatioErrorStyle(3244);
  p->SetStackErrorStyle(3244);
  p->doYieldsInLeg = false;
  p->doSignal = false;
  p->SetRatioMin(0.8); p->SetRatioMax(1.2);

  //>>> Add all backgrounds and data
  p->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, systematics);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  systematics);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   systematics);
	p->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   systematics);
	p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray+1,    systematics);
  if(doTopPtReweighting) p->SetWeight("TWeight*GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
	p->AddSample("TTbar_Powheg",                                         "ttbar",  itBkg, kRed+1,     systematics + ", JER");
  p->SetWeight("TWeight");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  //>>> Print axis labels for NBtagNJets
  if(name == "NBtagsNJets") p->SetBinLabels("[0|0], [1|0], [1|1], [2|0], [2|1], [2|2], [3|0], [3|1], [3|2], [3|3], [4|0], [4|1], [4|2], [4|3], [4|4], [>4|]");
  if(name == "SignalRegions") p->SetBinLabels(binLabelsSR);

  //>>> Add signal points 
  //p->AddSample("T2tt_mStop175_mLsp1",   "Stop[175, 1]",    itSignal, kGreen+1);
  //p->AddSample("T2tt_225_50_FS_summer", "Stop[225,50]", itSignal, kCyan); 

  //>> Add systematics
  p->AddSystematic("stat"); 
  p->AddNormUnc("ttbar", 0.05);
  p->AddNormUnc("VV", 0.30);
  p->AddNormUnc("ttV", 0.30);
  p->AddNormUnc("DY", 0.15);
  p->AddNormUnc("tW", 0.30);
  p->AddNormUnc("Nonprompt", 0.50);
  if(doAllSyst){

  if(doTopPtReweighting) p->SetWeight("TWeight*GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
    // ttbar modeling
    p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
    p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
    p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
    p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
    p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
    p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

    //>>> Color Reconnection
    p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
    p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
    p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
    p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
    p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

    //>>> PDF and scale using LHE weights...
    PDFunc *pdf;
    if(bin0 == binN) pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bins);
    else             pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN);
    Histo* hPDFUp   =  pdf->GetSystHisto("up",   "pdf")->CloneHisto();
    Histo* hPDFDown =  pdf->GetSystHisto("down", "pdf")->CloneHisto();
    Histo* hMEUp    =  pdf->GetSystHisto("up",   "ME")->CloneHisto();
    Histo* hMEDown  =  pdf->GetSystHisto("down", "ME")->CloneHisto();
    hPDFUp->SetProcess("ttbar"); hPDFDown->SetProcess("ttbar"); hMEUp->SetProcess("ttbar"); hMEDown->SetProcess("ttbar");
    hPDFUp->SetSysTag("PDFUp"); hPDFDown->SetSysTag("PDFDown"); hMEUp->SetSysTag("MEUp");  hMEDown->SetSysTag("MEDown");
    hPDFUp->SetTag("ttbar_PDFUp"); hPDFDown->SetTag("ttbar_PDFDown"); hMEUp->SetTag("ttbar_MEUp"); hMEDown->SetTag("ttbar_MEDown");
    hPDFUp->SetName("ttbar_PDFUp"); hPDFDown->SetName("ttbar_PDFDown"); hMEUp->SetName("ttbar_MEUp"); hMEDown->SetName("ttbar_MEDown");
    hPDFUp->SetType(itSys); hPDFDown->SetType(itSys); hMEUp->SetType(itSys); hMEDown->SetType(itSys);
    hPDFUp->SetStyle(); hPDFDown->SetStyle(); hMEUp->SetStyle(); hMEDown->SetStyle();
    p->AddToHistos(hPDFUp); p->AddToHistos(hPDFDown); p->AddToHistos(hMEUp); p->AddToHistos(hMEDown);

  p->SetWeight("TWeight");
  }

  //>>> Get the SF for nonprompt leptons background
 /// SSCR* a = new SSCR();
  //a->SetPathToTree(pathToTree); a->SetNameOfTree(NameOfTree);
  //a->SetBaselineCut(cut); a->SetChan(chan);
  //Float_t fakeSS = a->GetFakeSF();
  Float_t fakeSS = 1.5;
  p->ScaleProcess("Nonprompt", fakeSS);

  //>>> Print and save
  p->SetPlotFolder(outputFolder+"/ControlPlotsDilepton/");
  p->doData = true;
  p->doSetLogy = false;
  p->DrawStack();
  p->SetOutputName("Data_log");
  p->doSetLogy = true;
  p->doLegend = false;
  p->DrawStack();

  delete p;
}

// #########################################################################
// ## Non WZ same-sign estimate
void PrintNonWZestimate(TString chan, TString cut){
  SSCR* a = new SSCR();
  TString outname = "NonWZestimate";
  if(cut == "") cut = BaselineCut;
  a->SetPathToTree(pathToTree); a->SetNameOfTree(NameOfTree);
  a->SetBaselineCut(cut); a->SetChan(chan);
  a->SetPlotFolder(outputFolder+"NonWestimate/");
  a->DrawSSCR(outname + "_" + chan);
  a->PrintSStable(outname + "_" + chan + "_table");
}


// #########################################################################
// ## YIELDS
void PrintYieldTalbe(TString chan, TString cut){
  if(cut == "") cut = BaselineCut;
  TString var = "TChannel"; Int_t bin0 = 0; Int_t binN = 10; Int_t nbins = 1;
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", "Events");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = false;
  p->SetOutputName("Yields");

  //>>> Background samples
  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
	p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
	p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG", "Nonprompt", itBkg, kGray+1, "0");
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, "0");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);

  //>>> Signal samples
  p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kGray); 
  p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kViolet); 
  p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, kTeal-3); 
  p->SetPathSignal("/pool/ciencias/userstorage/juanr/stop/sep22/T2tt/ISR/");
  p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50_2", itSignal, kGray); 
  p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50_2", itSignal, kViolet); 
  p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50_2", itSignal, kTeal-3); 
  p->SetWeight("TWeight*TISRweight");
  p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50/ISRweight", itSignal, kGray); 
  p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50/ISRweight", itSignal, kViolet); 
  p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50/ISRweight", itSignal, kTeal-3); 
  p->SetWeight("TWeight");

  //>>> Systematics
  p->AddSystematic("stat"); // Only stat //p->AddSystematic("stat,JES,Btag,MisTag,LepEff,PU");
  if(doAllSyst){
    p->AddSystematic("Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,JER");
    //p->AddSystematic("MET", FastSimSignals);
    p->AddNormUnc("ttbar", 0.06);
    p->AddNormUnc("VV", 0.30);
    p->AddNormUnc("ttV", 0.30);
    p->AddNormUnc("DY", 0.15);
    p->AddNormUnc("tW", 0.30);
    p->AddNormUnc("Nonprompt", 0.50);
    p->AddNormUnc("SFS_200_50", 0.15);
    p->AddNormUnc("SFS_225_50", 0.15);
    p->AddNormUnc("SFS_250_50", 0.15);

    //>>> ISR, UE, hdamp
    p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
    p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
    p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
    p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
    p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
    p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

    //>>> FSR
    TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
    p->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
    p->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );

    //>>> Top pT
    p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
    p->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtUp");
    p->SetWeight("TWeight");

    //>>> TopMass
    p->AddSample("TTbar_Powheg_mtop1715", "ttbar", itSys, 1, "topMassDown");
    p->AddSample("TTbar_Powheg_mtop1735", "ttbar", itSys, 1, "topMassUp");
    
    //>>> Color Reconnection
    p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
    p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
    p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
    p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
    p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

    //>>> PDF and scale using LHE weights...
    PDFunc *pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN);
    Histo* hPDFUp   =  pdf->GetSystHisto("up",   "pdf")->CloneHisto();
    Histo* hPDFDown =  pdf->GetSystHisto("down", "pdf")->CloneHisto();
    Histo* hMEUp    =  pdf->GetSystHisto("up",   "ME")->CloneHisto();
    Histo* hMEDown  =  pdf->GetSystHisto("down", "ME")->CloneHisto();
    hPDFUp->SetProcess("ttbar"); hPDFDown->SetProcess("ttbar"); hMEUp->SetProcess("ttbar"); hMEDown->SetProcess("ttbar");
    hPDFUp->SetSysTag("PDFUp"); hPDFDown->SetSysTag("PDFDown"); hMEUp->SetSysTag("MEUp");  hMEDown->SetSysTag("MEDown");
    hPDFUp->SetTag("ttbar_PDFUp"); hPDFDown->SetTag("ttbar_PDFDown"); hMEUp->SetTag("ttbar_MEUp"); hMEDown->SetTag("ttbar_MEDown");
    hPDFUp->SetName("ttbar_PDFUp"); hPDFDown->SetName("ttbar_PDFDown"); hMEUp->SetName("ttbar_MEUp"); hMEDown->SetName("ttbar_MEDown");
    hPDFUp->SetType(itSys); hPDFDown->SetType(itSys); hMEUp->SetType(itSys); hMEDown->SetType(itSys);
    hPDFUp->SetStyle(); hPDFDown->SetStyle(); hMEUp->SetStyle(); hMEDown->SetStyle();
    p->AddToHistos(hPDFUp); p->AddToHistos(hPDFDown); p->AddToHistos(hMEUp); p->AddToHistos(hMEDown);
  }

  //>>> Nonprompt estimate
  //SSCR* a = new SSCR();
  //a->SetPathToTree(pathToTree); a->SetNameOfTree(NameOfTree);
  //a->SetBaselineCut(cut); a->SetChan(chan);
  //Float_t fakeSS = a->GetFakeSF();
  Float_t fakeSS = 1.5;
  p->ScaleProcess("Nonprompt", fakeSS);

  Float_t n  = p->GetYield("ttbar"); 
  Float_t vu = p->GetYield("ttbar", "fsrUp");
  Float_t vd = p->GetYield("ttbar", "fsrDown");
  p->ScaleSys("ttbar", "fsrUp",   GetFactorFSR(n, vu));
  p->ScaleSys("ttbar", "fsrDown", GetFactorFSR(n, vd));

  //>>> Options and printing
  p->SetPlotFolder(outputFolder+"Yields/");
  p->doData = false;
  p->SetYieldsTableName("StopYields"); 
  //p->PrintYields("", "", "", "tex, txt, html");
  p->PrintBinsYields("tex, txt, html");
  return; 
}

// #########################################################################
// ## SIGNAL PLOTS
void DrawSignalPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name){
  Plot *p;
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = true;
  if(name != "") p->SetVarName(name + "_" + chan);

  //>>> Setting style
  p->SetLegendTextSize(0.035);
  p->SetLegendPosition(0.70, 0.70, 0.93, 0.93);
  p->SetRatioErrorColor(kTeal-2);
  p->SetRatioErrorStyle(3244);
  p->SetStackErrorStyle(3244);

  //>>> Adding background and data samples
  p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow-10);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu", "ttV", itBkg, kOrange-3);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY", itBkg, kAzure-8);
	p->AddSample("TW, TbarW", "tW", itBkg, kMagenta);
	p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG", "Nonprompt", itBkg, kGray+1, "0");
  if(doTopPtReweighting) p->SetWeight("TWeight*GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)");
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed+1, "0");
  p->SetWeight("TWeight");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);

  //>>> Adding signal sample (only one at a time, for now...)
  //p->SetWeight("TWeight*TBtagSFFS");
  //p->AddSample("T2tt_mStop175_mLsp1",   "Stop[175, 1]",    itSignal, kGreen+1);
  p->AddSample("T2tt_225_50_FS_summer", "Stop[225,50]", itSignal, kCyan); 
  p->SetOutputName("Signal");

  //>>> Setting StoB ratio... NO DATA  
  p->SetSignalProcess("Stop[225,50]");
  p->SetSignalStyle("SM");
  p->SetRatioMin(0.0);
  p->SetRatioMax(0.1);
  //p->SetRatioOptions("S/B");
  p->doData = true;

  //>>> Add systematics
  p->AddSystematic("stat"); //p->AddSystematic("JES,Btag,MisTag,LepEff,PU");

  //>>> Scale nonprompt background 
  /* SSCR* a = new SSCR();
  a->SetPathToTree(pathToTree); a->SetNameOfTree(NameOfTree);
  a->SetBaselineCut(cut); a->SetChan(chan);
  Float_t fakeSS = a->GetFakeSF(); */
  Float_t fakeSS = 1.50;
  p->ScaleProcess("Nonprompt", fakeSS);

  //>>> Options and printing
  p->SetPlotFolder(outputFolder);
  p->doSetLogy = false;
  p->SetScaleMax(1.25);
  p->DrawStack();
  p->SetOutputName("Signal_log");
  p->SetScaleMax(1000);
  p->doSetLogy = true;
  p->DrawStack();

  delete p;
}
