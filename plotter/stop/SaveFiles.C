//##################################################################################################################
//
// Crate a .root files with all needed histograms to calculate limits
//
//    root -l -b -q 'SaveFiles.C("")'  
//
//##################################################################################################################
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "SSCR.C"
#include "PDFunc.C"

Bool_t doTopPtReweight = false;
Bool_t doMetResReweight = false;

//============================================================== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/jan5/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";
Float_t gbins26[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins26 = 26;
Float_t gbins21[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins21 = 21;
Float_t gbins25[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,200}; Int_t ngbins25 = 25;
Float_t gbins20[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,   90,   100,    110,    120,200}; Int_t ngbins20 = 20;
Float_t gbins19[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,   90,   100,            120,200}; Int_t ngbins19 = 19;
Float_t gbins16[] = {0,5,10,15,20,25,30,35,40,   50,   60,         80,   90,   100,    110,    120,200}; Int_t ngbins16 = 16;
Float_t gbins15[] = {0,5,10,15,20,25,30,35,40,   50,   60,         80,   90,   100,            120,200}; Int_t ngbins15 = 15;
Float_t gbins11[] = {0,  10,   20,   30,   40,   50,   60,         80,   90,   100,            120,200}; Int_t ngbins11 = 11;
Float_t gbins10[] = {0,  10,   20,   30,   40,   50,   60,         80,         100,            120,200}; Int_t ngbins10 = 10;
//Float_t gbins9[] =  {0,10,20,30,40,50,60,80,    120,200}; Int_t ngbins9 = 9;
Float_t gbins5[] =  {0,   20,   40,60,80,       200}; Int_t ngbins5 = 5;
//Float_t gbins4[] =  {0,   20,         80,   120,200}; Int_t ngbins4 = 4;
Float_t gbins3[] =  {0,   20,         80,       200}; Int_t ngbins3 = 3;
Float_t gbins2[] =  {0,   20,                   200}; Int_t ngbins2 = 2;

Float_t gbins5b[] =  {0,5, 40, 80, 100, 200}; Int_t ngbins5b = 5;
Float_t gbins6b[] =  {0, 10, 20,40, 80, 100, 200}; Int_t ngbins6b = 6;

TString FastSimSignals = "S_175_1, S_183_1, S_192_25, S_200_25, S_208_25, S_217_50, S_225_50, S_242_75, S_250_75, SD_160_1, SD_160_10, SD_160_20, SD_170_1, SD_170_10, SD_170_20, SD_175_1, SD_175_10, SD_175_20, SD_180_1, SD_180_10, SD_180_20, SD_190_1, SD_190_10, SD_190_20, SD_200_1, SD_200_10, SD_200_20, SD_210_1, SD_210_10, SD_210_20";
TString FullSimSignals = "SFS_175_1, SFS_200_50, SFS_225_50, SFS_250_50";
Plot   *p = nullptr;
PDFunc *pdf = nullptr;


Float_t binsMT2deta[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.4, 200.8, 201.2, 201.6, 202.0, 202.4,}; Int_t nbinsmt2deta = 27;
Float_t binsMT2metdeta[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.4, 200.8, 201.2, 201.6, 202.0, 202.4,205, 210}; Int_t nbinsmt2metdeta = 29;
Float_t binsMT2metdeta2[] = {0,20,40,60,80,100,200,200.4, 200.8, 201.2, 201.6, 202.0, 202.4,205, 210}; Int_t nbinsmt2metdeta2 = 14;

//============================================================== Selection
//TString BaselineCut = "!TIsSS && TPassTrigger && TPassMETfilters"; // && TNVetoLeps < 3 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString DefaultOptions = "ttbar,FullSim"; //"ttbar,FullSim,xqcut,dileptonic,inclusive";

//============================================================== Functions
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString Xtitle, TString name, TString options="");
void AddSamples(TString options = "ttbar,FullSim");
Float_t GetFSRsf(Float_t n, Float_t v);
void SetPaths();
void ScaleFakes();

//============================================================== Systematic uncertainties
TString bkgSyst  = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale";
TString expSyst  = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER";
TString T2ttSyst = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,UnclMET";
TString FSSyst   = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,MET";

//============================================================== SaveFiles (Main function)
void SaveFiles(TString options = ""){
    //CreatePlot("TChannel", BaselineCut, "ElMu", 1, 0, 10,  0,    "Yields",       "CutAndCount", options);
    TString channels = "ElMu"; //, Elec, Muon, All";
    vector<TString> vchan = TStringToVector(channels);
    TString chan;
    for(int i = 0; i < (int) vchan.size(); i++){
      chan = vchan.at(i);

      //CreatePlot("TMT2",     BaselineCut, chan, ngbins21, 1, 1, gbins21, "M_{T2} [GeV]", "MT2_21"        , options);
      //CreatePlot("GetMT2orDeltaEta(TMT2, TDeltaEta)",  BaselineCut, chan, nbinsmt2deta, 1, 1, binsMT2deta, "M_{T2} [GeV] / #Delta#eta", "MT2_DeltaEta" , options);
     // CreatePlot("GetMT2orDiPtorDeltaEtaBins(TMT2, GetDilepPtDPhi(TLep0_Pt, TLep1_Pt, TDeltaPhi), TDeltaEta)",  BaselineCut, chan, 14, 1, 15, 0, "", "MT2_DilepPt_DeltaEta" , options);
     // CreatePlot("GetMT2orMETorDeltaEta(TMT2, TMET, TDeltaEta)",  BaselineCut, chan, nbinsmt2metdeta, 1, 1, binsMT2metdeta, "M_{T2} [GeV] / #Delta#eta", "MT2_MET_DeltaEta" , options);
      CreatePlot("GetMT2orDeltaEtaBins(TMT2, TMET, TDeltaEta)",  BaselineCut, chan, 16, 1, 17, 0, "", "MT2_MET_DeltaEta3" , options);
//      CreatePlot("TDeltaEta",  BaselineCut, chan, 12, 0, 2.4, 0, "#Delta#eta", "DeltaEta" , options);
//      CreatePlot("TDeltaPhi",  BaselineCut, chan, 16, 0, 3.2, 0, "#Delta#phi", "DeltaPhi" , options);
//      CreatePlot("TMET",       BaselineCut, chan, 25, 0, 250, 0, "MET (GeV)",  "MET" , options);
     // Lep0_Eta, Lep1_Eta
   }


//    CreatePlot("TMET", BaselineCut, "ElMu", 20, 0, 200,  0,    "MET [GeV]",       "MET", "noooo");

//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins15, 1, 1, gbins15, "M_{T2} [GeV]", "MT2_15"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins10, 1, 1, gbins10, "M_{T2} [GeV]", "MT2_10"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins11, 1, 1, gbins11, "M_{T2} [GeV]", "MT2_11"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins25, 1, 1, gbins25, "M_{T2} [GeV]", "MT2_25"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins26, 1, 1, gbins26, "M_{T2} [GeV]", "MT2", options);
/*
    CreatePlot("TChannel", BaselineCut + " && TMT2 > 0", "ElMu", 1, 0, 3,  0,    "Yields",       "CutAndCount_MT2g0", options);
    CreatePlot("TChannel", BaselineCut + " && TMT2 > 0", "All", 3, 0, 3,  0,    "Yields",       "CutAndCount_All", options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins26, 1, 1, gbins26, "M_{T2} [GeV]", "MT2_26"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins20, 1, 1, gbins20, "M_{T2} [GeV]", "MT2_20"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins15, 1, 1, gbins15, "M_{T2} [GeV]", "MT2_15"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins10, 1, 1, gbins10, "M_{T2} [GeV]", "MT2_10"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins5, 1, 1, gbins5, "M_{T2} [GeV]", "MT2_5"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins3, 1, 1, gbins3, "M_{T2} [GeV]", "MT2_3"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins2, 1, 1, gbins2, "M_{T2} [GeV]", "MT2_2"        , options);
    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins5b, 1, 1, gbins5b, "M_{T2} [GeV]", "MT2_5b"        , options);
*/
//    CreatePlot("TDeltaPhi",     BaselineCut, "ElMu", 30, 0, 3.2, 0, "DeltaPhi", "DeltaPhi"        , options);
//    CreatePlot("TMT2",     BaselineCut, "Muon", ngbins26, 1, 1, gbins26, "M_{T2} [GeV]", "MT2_Muon"        , options);
//    CreatePlot("TMT2",     BaselineCut, "Elec", ngbins26, 1, 1, gbins26, "M_{T2} [GeV]", "MT2_Elec"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", ngbins26, 1, 1, gbins26, "M_{T2} [GeV]", "MT2_ElMu"        , options);
}



//=============================================================== Other options
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name, TString options){
  if(options == "") options = DefaultOptions;

  if(bin0 == binN){
    p = new Plot(var, cut, chan, nbins, bins, "Title", Xtitle);
    if(options.Contains("pdf")) pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bins);
  }
  else{
    p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
    if(options.Contains("pdf")) pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN);
  }
  p->verbose = false;
  SetPaths();
  AddSamples(options);
  ScaleFakes(); 

  Float_t n  = p->GetYield("ttbar"); 
  Float_t vu = p->GetYield("ttbar", "fsrUp");
  Float_t vd = p->GetYield("ttbar", "fsrDown");
  p->ScaleSys("ttbar", "fsrUp",   GetFactorFSR(n, vu));
  p->ScaleSys("ttbar", "fsrDown", GetFactorFSR(n, vd));
  
  if(name != "") p->SetVarName(name + "_" + chan);
  p->doData = true;
  p->PrintSystYields();
  p->SaveHistograms();
  delete p;
}


void AddSamples(TString options){
  //>>> Default options
  if(options == "") options = "ttbar,FullSim"; //options = "ttbar,FullSim,xqcut,inclusive,dilep";
  cout << "[SaveFiles] Options: " << options << endl;

  //>>> Bkgs and data
  p->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, bkgSyst);
  p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  bkgSyst);
  p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray,      bkgSyst);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   bkgSyst);
  p->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   bkgSyst);
  if(doTopPtReweight)  p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  if(doMetResReweight) p->SetWeight("GetMetResNorm(TMET)*TWeight");
  p->AddSample("TTbar_Powheg",                                         "ttbar",     itBkg, kRed+1,     expSyst);
  p->SetWeight("TWeight");
  p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  //>>> ttbar systematics
  if(options.Contains("ttbar")){
    // TopPt
    p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
    p->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtUp");
    p->SetWeight("TWeight");
    p->AddSample("TTbar_Powheg","ttbar", itSys, 1, "TopPtDown");

    p->AddSample("TTbar_Powheg_ueUp", "ttbar", itSys, 1, "ueUp");
    p->AddSample("TTbar_Powheg_ueDown", "ttbar", itSys, 1, "ueDown");
    p->AddSample("TTbar_Powheg_isrUp", "ttbar", itSys, 1, "isrUp");
    p->AddSample("TTbar_Powheg_isrDown", "ttbar", itSys, 1, "isrDown");
    p->AddSample("TTbar_Powheg_hdampUp", "ttbar", itSys, 1, "hdampUp");
    p->AddSample("TTbar_Powheg_hdampDown", "ttbar", itSys, 1, "hdampDown");

    //----------> Missing b-fragmentation??? 

    p->AddSample("TTbar_Powheg_mtop1715", "ttbar", itSys, 1, "topMassDown");
    p->AddSample("TTbar_Powheg_mtop1735", "ttbar", itSys, 1, "topMassUp");

    //>>> Uncl
    p->SetWeight("TWeight*GetMetUnclWeightUp(TMET)");
    p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "unclMETUp");
    p->SetWeight("TWeight*GetMetUnclWeightDown(TMET)");
    p->AddSample("TTbar_Powheg", "ttbar", itSys, 1, "unclMETDown");
    p->SetWeight("TWeight");

    //>>> Color Reconnection
    p->SetWeight("TWeight");
    p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
    p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
    p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
    p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
    p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");

    //>>> PDF and scale using LHE weights...
    if(options.Contains("pdf")){
      Histo* hPDFUp   =  pdf->GetSystHisto("up",   "pdf")->CloneHisto();
      Histo* hPDFDown =  pdf->GetSystHisto("down", "pdf")->CloneHisto();
      Histo* hMEUp    =  pdf->GetSystHisto("up",   "ME")->CloneHisto();
      Histo* hMEDown  =  pdf->GetSystHisto("down", "ME")->CloneHisto();

      p->PrepareHisto(hPDFUp,   "TTbar_Powheg", "ttbar", itSys, 0, "PDFUp");
      p->PrepareHisto(hPDFDown, "TTbar_Powheg", "ttbar", itSys, 0, "PDFDown");
      p->PrepareHisto(hMEUp,    "TTbar_Powheg", "ttbar", itSys, 0, "MEUp");
      p->PrepareHisto(hMEDown,  "TTbar_Powheg", "ttbar", itSys, 0, "MEDown");
      p->AddToSystematicLabels("PDF");
      p->AddToSystematicLabels("ME");
    }


    TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
    p->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrUp",     "ttbar", itSys, 1, "fsrUp"    );
    p->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrDown",   "ttbar", itSys, 1, "fsrDown"  );

    p->SetWeight("TWeight");
  }
  
  //>>> FulSim samples
  if(options.Contains("FullSim")){
    //p->AddSample("T2tt_175_1_FS",         "SFS_175_1",  itSignal, kCyan, expSyst);
    p->SetPathSignal(pathToTree + "T2tt/FullSimScan/");
    
    p->AddSample("T2tt_mStop227_mLsp52_0",  "T2tt_227p5_52p5_mod", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_0",  "T2tt_227p5_52p5_mod", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_0",  "T2tt_227p5_52p5_mod", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod", 3);
    p->AddSample("T2tt_mStop227_mLsp52_1",  "T2tt_227p5_52p5_mod2", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_1",  "T2tt_227p5_52p5_mod2", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_1",  "T2tt_227p5_52p5_mod2", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod2", 3);
    p->AddSample("T2tt_mStop227_mLsp52_2",  "T2tt_227p5_52p5_mod3", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_2",  "T2tt_227p5_52p5_mod3", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_2",  "T2tt_227p5_52p5_mod3", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod3", 3);

    p->AddSample("T2tt_mStop227_mLsp52_06",  "T2tt_227p5_52p5_mod06", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_06",  "T2tt_227p5_52p5_mod06", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_06",  "T2tt_227p5_52p5_mod06", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod06", 6);
    p->AddSample("T2tt_mStop227_mLsp52_16",  "T2tt_227p5_52p5_mod16", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_16",  "T2tt_227p5_52p5_mod16", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_16",  "T2tt_227p5_52p5_mod16", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod16", 6);
    p->AddSample("T2tt_mStop227_mLsp52_26",  "T2tt_227p5_52p5_mod26", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_26",  "T2tt_227p5_52p5_mod26", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_26",  "T2tt_227p5_52p5_mod26", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod26", 6);
    p->AddSample("T2tt_mStop227_mLsp52_36",  "T2tt_227p5_52p5_mod36", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_36",  "T2tt_227p5_52p5_mod36", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_36",  "T2tt_227p5_52p5_mod36", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod36", 6);
    p->AddSample("T2tt_mStop227_mLsp52_46",  "T2tt_227p5_52p5_mod46", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_46",  "T2tt_227p5_52p5_mod46", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_46",  "T2tt_227p5_52p5_mod46", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod46", 6);
    p->AddSample("T2tt_mStop227_mLsp52_56",  "T2tt_227p5_52p5_mod56", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52_56",  "T2tt_227p5_52p5_mod56", itSys, 1,     "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52_56",  "T2tt_227p5_52p5_mod56", itSys, 1,     "unclMETDown");
    p->ScaleProcess("T2tt_227p5_52p5_mod56", 6);

    p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan, T2ttSyst);
    p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink, T2ttSyst);
    p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1,     T2ttSyst);
    p->AddSample("T2tt_200_50_FS_xqcut20", "SFS_200_50_xqcut20", itSignal, kCyan, T2ttSyst);
    p->AddSample("T2tt_225_50_FS_xqcut20", "SFS_225_50_xqcut20", itSignal, kPink, T2ttSyst);
    p->AddSample("T2tt_250_50_FS_xqcut20", "SFS_250_50_xqcut20", itSignal, 1,     T2ttSyst);
    p->SetPathSignal(pathToTree + "T2tt/FullSimScan/");
    p->AddSample("T2tt_mStop167_mLsp0",   "T2tt_167p0_1p0",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop175_mLsp0",   "T2tt_175p0_1p0",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop175_mLsp7",   "T2tt_175p0_7p5",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop182_mLsp0",   "T2tt_182p5_1p0",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop182_mLsp7",   "T2tt_182p5_7p5",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop182_mLsp15",  "T2tt_182p5_15p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop190_mLsp7",   "T2tt_190p0_7p5",  itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop190_mLsp15",  "T2tt_190p0_15p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop190_mLsp22",  "T2tt_190p0_22p5", itSignal, 1,     expSyst);

    p->AddSample("T2tt_mStop197_mLsp15",  "T2tt_197p5_15p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop197_mLsp22",  "T2tt_197p5_22p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop197_mLsp30",  "T2tt_197p5_30p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop205_mLsp22",  "T2tt_205p0_22p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop205_mLsp30",  "T2tt_205p0_30p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop205_mLsp37",  "T2tt_205p0_37p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop212_mLsp30",  "T2tt_212p5_30p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop212_mLsp37",  "T2tt_212p5_37p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop212_mLsp45",  "T2tt_212p5_45p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop220_mLsp37",  "T2tt_220p0_37p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop220_mLsp45",  "T2tt_220p0_45p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop220_mLsp52",  "T2tt_220p0_52p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp45",  "T2tt_227p5_45p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp52",  "T2tt_227p5_52p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop227_mLsp60",  "T2tt_227p5_60p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop235_mLsp52",  "T2tt_235p0_52p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop235_mLsp60",  "T2tt_235p0_60p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop235_mLsp67",  "T2tt_235p0_67p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop242_mLsp60",  "T2tt_242p5_60p0", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop242_mLsp67",  "T2tt_242p5_67p5", itSignal, 1,     expSyst);
    p->AddSample("T2tt_mStop242_mLsp75",  "T2tt_242p5_75p0", itSignal, 1,     expSyst);

    //>>> Uncl MET for signal
    p->SetWeight("TWeight*GetMetUnclWeightUp(TMET)");
    p->AddSample("T2tt_mStop167_mLsp0",   "T2tt_167p0_1p0",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop175_mLsp0",   "T2tt_175p0_1p0",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop175_mLsp7",   "T2tt_175p0_7p5",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop182_mLsp0",   "T2tt_182p5_1p0",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop182_mLsp7",   "T2tt_182p5_7p5",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop182_mLsp15",  "T2tt_182p5_15p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop190_mLsp7",   "T2tt_190p0_7p5",  itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop190_mLsp15",  "T2tt_190p0_15p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop190_mLsp22",  "T2tt_190p0_22p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop197_mLsp15",  "T2tt_197p5_15p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop197_mLsp22",  "T2tt_197p5_22p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop197_mLsp30",  "T2tt_197p5_30p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop205_mLsp22",  "T2tt_205p0_22p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop205_mLsp30",  "T2tt_205p0_30p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop205_mLsp37",  "T2tt_205p0_37p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop212_mLsp30",  "T2tt_212p5_30p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop212_mLsp37",  "T2tt_212p5_37p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop212_mLsp45",  "T2tt_212p5_45p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop220_mLsp37",  "T2tt_220p0_37p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop220_mLsp45",  "T2tt_220p0_45p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop220_mLsp52",  "T2tt_220p0_52p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp45",  "T2tt_227p5_45p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp52",  "T2tt_227p5_52p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop227_mLsp60",  "T2tt_227p5_60p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop235_mLsp52",  "T2tt_235p0_52p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop235_mLsp60",  "T2tt_235p0_60p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop235_mLsp67",  "T2tt_235p0_67p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop242_mLsp60",  "T2tt_242p5_60p0", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop242_mLsp67",  "T2tt_242p5_67p5", itSys, 1,  "unclMETUp");
    p->AddSample("T2tt_mStop242_mLsp75",  "T2tt_242p5_75p0", itSys, 1,  "unclMETUp");

    p->SetWeight("TWeight*GetMetUnclWeightDown(TMET)");
    p->AddSample("T2tt_mStop167_mLsp0",   "T2tt_167p0_1p0",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop175_mLsp0",   "T2tt_175p0_1p0",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop175_mLsp7",   "T2tt_175p0_7p5",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop182_mLsp0",   "T2tt_182p5_1p0",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop182_mLsp7",   "T2tt_182p5_7p5",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop182_mLsp15",  "T2tt_182p5_15p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop190_mLsp7",   "T2tt_190p0_7p5",  itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop190_mLsp15",  "T2tt_190p0_15p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop190_mLsp22",  "T2tt_190p0_22p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop197_mLsp15",  "T2tt_197p5_15p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop197_mLsp22",  "T2tt_197p5_22p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop197_mLsp30",  "T2tt_197p5_30p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop205_mLsp22",  "T2tt_205p0_22p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop205_mLsp30",  "T2tt_205p0_30p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop205_mLsp37",  "T2tt_205p0_37p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop212_mLsp30",  "T2tt_212p5_30p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop212_mLsp37",  "T2tt_212p5_37p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop212_mLsp45",  "T2tt_212p5_45p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop220_mLsp37",  "T2tt_220p0_37p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop220_mLsp45",  "T2tt_220p0_45p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop220_mLsp52",  "T2tt_220p0_52p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop227_mLsp45",  "T2tt_227p5_45p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop227_mLsp52",  "T2tt_227p5_52p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop227_mLsp60",  "T2tt_227p5_60p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop235_mLsp52",  "T2tt_235p0_52p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop235_mLsp60",  "T2tt_235p0_60p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop235_mLsp67",  "T2tt_235p0_67p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop242_mLsp60",  "T2tt_242p5_60p0", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop242_mLsp67",  "T2tt_242p5_67p5", itSys, 1,  "unclMETDown");
    p->AddSample("T2tt_mStop242_mLsp75",  "T2tt_242p5_75p0", itSys, 1,  "unclMETDown");

    p->SetWeight("TWeight");
  }

  //>>> Full Sim xqcut = 20
    if(options.Contains("xqcut")){
    //p->AddSample("T2tt_175_1_FS_xqcut20",  "SFS_175_1_xqcut20",  itSignal, kCyan, expSyst); 
  }

  //>>> FastSim Inclusive grid
  if(options.Contains("incl")){
    p->SetWeight("TWeight*TBtagSFFS");
    //p->AddSample("T2tt_mStop175_mLsp1", "S_175_1", itSignal, kCyan); // In the dileptonic samples
    p->AddSample("T2tt_mStop183_mLsp1",  "S_183_1",  itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop192_mLsp25", "S_192_25", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop200_mLsp25", "S_200_25", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop208_mLsp25", "S_208_25", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop200_mLsp50", "S_200_50", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop217_mLsp50", "S_217_50", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop225_mLsp50", "S_225_50", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop233_mLsp50", "S_233_50", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop250_mLsp50", "S_250_50", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop242_mLsp75", "S_242_75", itSignal, kCyan, FSSyst); 
    p->AddSample("T2tt_mStop250_mLsp75", "S_250_75", itSignal, kCyan, FSSyst); 
    p->SetWeight("TWeight");
  }

  //>>> FastSim Dileptonic grid
  if(options.Contains("dilep")){
    p->SetWeight("TWeight*TBtagSFFS");
    p->AddSample("T2tt_mStop160_mLsp1" , "SD_160_1",  itSignal, 1, FSSyst); 
    //p->AddSample("T2tt_mStop160_mLsp10", "SD_160_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop160_mLsp20", "SD_160_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop170_mLsp1" , "SD_170_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop170_mLsp10", "SD_170_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop170_mLsp20", "SD_170_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop175_mLsp1" , "SD_175_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop175_mLsp10", "SD_175_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop175_mLsp20", "SD_175_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop180_mLsp1" , "SD_180_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop180_mLsp10", "SD_180_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop180_mLsp20", "SD_180_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop190_mLsp1" , "SD_190_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop190_mLsp10", "SD_190_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop190_mLsp20", "SD_190_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop200_mLsp1" , "SD_200_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop200_mLsp10", "SD_200_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop200_mLsp20", "SD_200_20", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop210_mLsp1" , "SD_210_1",  itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop210_mLsp20" ,"SD_210_20", itSignal, 1, FSSyst); 
    //p->AddSample("T2tt_mStop210_mLsp10", "SD_210_10", itSignal, 1, FSSyst); 
    p->AddSample("T2tt_mStop210_mLsp20", "SD_210_20", itSignal, 1, FSSyst); 
    p->SetWeight("TWeight");
  }

  //p->AddSystematic("Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,JER", FullSimSignals);
  //>>> Set systematics
  //if(options.Contains("AllSyst")){
  //  p->AddSystematic("Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU");
  //  p->AddSystematic("JER", "ttbar,"+FastSimSignals+","+FullSimSignals);
  //  p->AddSystematic("MET", FastSimSignals);
  //}

}

void SetPaths(){
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = false;
  p->doData = false;
  p->SetLimitFolder("output/Datacards_jan5/");
}

void ScaleFakes(){
  //SSCR *ss = new SSCR();
  //Float_t fakeSF = ss->GetFakeSF();
  Float_t fakeSF = 1.5;
  p->ScaleProcess("Nonprompt", fakeSF);
}

