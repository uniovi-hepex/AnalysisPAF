//#########################################################################################
//
// Print bin-by-bin yields and uncertainties
//
// root -l -b -1 'BinByBin.C'
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

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name = "BinByBin");

//=== Define paths and tree name
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/jan5/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";

//=== Constants...
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
Float_t gbins[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins = 26;
Float_t gbins21[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins21 = 21;
TString bkgSyst = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale";
TString expSyst = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER";

void BinByBin(TString chan = "ElMu"){
  TString cut = BaselineCut;
  //DrawPlot("TChannel",        cut, chan, 1, 0, 10, 0, "Counts", "Yields");
  DrawPlot("TMT2",        cut, chan, ngbins21, 0, 0, gbins21, "M_{T2} [GeV]", "MT2_21");
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
  p->SetOutputName("BinByBin");

  p->doYieldsInLeg = false;
  p->doSignal = true;

  //>>> Add all backgrounds and data
  p->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, bkgSyst);
  p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  bkgSyst);
  p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray,      bkgSyst);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   bkgSyst);
  p->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   bkgSyst);
  p->AddSample("TTbar_Powheg",                                         "ttbar",     itBkg, kRed+1,     expSyst);
  p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  //>>> Add signal points 
  p->AddSample("T2tt_200_50_FS_summer", "T2tt[200,50]", itSignal, kCyan, expSyst);
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50]", itSignal, kPink, expSyst);
  p->AddSample("T2tt_250_50_FS_summer", "T2tt[250,50]", itSignal, 1,     expSyst);

  //>> Add systematics
  p->AddSystematic("stat"); 
  p->AddNormUnc("tW",  0.30);
  p->AddNormUnc("ttV", 0.30);
  p->AddNormUnc("VV",  0.30);
  p->AddNormUnc("DY",  0.15);
  p->AddNormUnc("Nonprompt", 0.60);
  p->AddNormUnc("ttbar",     0.06);
  p->AddNormUnc("T2tt[200,50]",     0.15);
  p->AddNormUnc("T2tt[225,50]",     0.15);
  p->AddNormUnc("T2tt[250,50]",     0.15);

  //>>> Uncl MET for signal
  p->SetWeight("TWeight*GetMetUnclWeightUp(TMET)");
  p->AddSample("T2tt_200_50_FS_summer", "T2tt[200,50]", itSys, 1, "unclUp");
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50]", itSys, 1, "unclUp");
  p->AddSample("T2tt_250_50_FS_summer", "T2tt[250,50]", itSys, 1, "unclUp");
  p->SetWeight("TWeight*GetMetUnclWeightDown(TMET)");
  p->AddSample("T2tt_200_50_FS_summer", "T2tt[200,50]", itSys, 1, "unclDown");
  p->AddSample("T2tt_225_50_FS_summer", "T2tt[225,50]", itSys, 1, "unclDown");
  p->AddSample("T2tt_250_50_FS_summer", "T2tt[250,50]", itSys, 1, "unclDown");
  p->SetWeight("TWeight");



  //>>> UE, ISR, hdamp
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
  p->SetWeight("TWeight");

  //>>> Uncl
  p->SetWeight("TWeight*GetMetUnclWeightUp(TMET)");
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "unclUp");
  p->SetWeight("TWeight*GetMetUnclWeightDown(TMET)");
  p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "unclDown");
  p->SetWeight("TWeight");

  //>>> Top Mass
  p->AddSample("TTbar_Powheg_mtop1715", "ttbar", itSys, 1, "topMassDown");
  p->AddSample("TTbar_Powheg_mtop1735", "ttbar", itSys, 1, "topMassUp");

  //>>> Color Reconnection
  p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
  p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
  p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
  p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
  p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

  //>>> PDF and scale using LHE weights...
/*  PDFunc *pdf; 
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
*/
  //>>> Get the SF for nonprompt leptons background
  //SSCR* a = new SSCR();
  //a->SetPathToTree(pathToTree); a->SetNameOfTree(NameOfTree);
  //a->SetBaselineCut(cut); a->SetChan(chan);
  //Float_t fakeSS = a->GetFakeSF();
  Float_t fakeSS = 1.5;
  p->ScaleProcess("Nonprompt", fakeSS);


  // Scale FSR
  Float_t n  = p->GetYield("ttbar"); 
  Float_t vu = p->GetYield("ttbar", "fsrUp");
  Float_t vd = p->GetYield("ttbar", "fsrDown");
  p->ScaleSys("ttbar", "fsrUp",   GetFactorFSR(n, vu));
  p->ScaleSys("ttbar", "fsrDown", GetFactorFSR(n, vd));

  //>>> Print and save
  p->SetTableFormats("%2.0f");
  p->SetPlotFolder(outputFolder);
  p->doData = true;

  cout << "\n\n PRINTING BINS YIELDS \n\n";
  p->PrintBinsYields();

  delete p;
}

  //>>> Top pT
  //p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  //p->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtUp");
  //p->SetWeight("TWeight");
