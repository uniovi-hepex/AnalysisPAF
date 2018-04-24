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
Bool_t doFakesFromData = true;

//============================================================== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/apr2018/";
TString NameOfTree = "tree";
TString outputFolder = "./output/mt2b100/";
Float_t gbins26[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200}; Int_t ngbins26 = 26;
Float_t gbins21[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200}; Int_t ngbins21 = 21;
Float_t gbins20[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100}; Int_t ngbins20 = 20;
Float_t gbins19[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,100,200}; Int_t ngbins19 = 19;
Float_t gbins25[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,200}; Int_t ngbins25 = 25;
// Float_t gbins20[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,   90,   100,    110,    120,200}; Int_t ngbins20 = 20;
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


Float_t binsMT2deta[]    = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta = 29;
Float_t binsMT2deta_24[] = {0,20,30,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta_24 = 24;
Float_t binsMT2deta_20[] = {0,5,10,15,20,40,60,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta_20 = 20;
Float_t binsMT2deta_17[] = {0,20,40,60,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta_17 = 17;
Float_t binsMT2deta_18[] = {0,20,40,60,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.0, 202.4}; Int_t nbinsmt2deta_18 = 18;
Float_t binsMT2metdeta[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.4, 200.8, 201.2, 201.6, 202.0, 202.4,205, 210}; Int_t nbinsmt2metdeta = 29;
Float_t binsMT2metdeta2[] = {0,20,40,60,80,100,200,200.4, 200.8, 201.2, 201.6, 202.0, 202.4,205, 210}; Int_t nbinsmt2metdeta2 = 14;

Float_t binsMT2deta_31[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200,200.05,200.1, 200.15, 200.2, 200.3, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta_31 = 31;
//Float_t binsMT2deta_21[] = {0,20,30,40,50,60,70,75,80,85,90,95,100,200,200.1, 200.2, 200.4, 200.6, 200.8, 201.2, 201.6, 202.4}; Int_t nbinsmt2deta_21 = 21;

//============================================================== Selection
//TString BaselineCut = "!TIsSS && TPassTrigger && TPassMETfilters"; // && TNVetoLeps < 3 
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25 && TMT2 > 0";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString DefaultOptions = "FullSim,ttbar,pdf"; //"ttbar,FullSim,xqcut,dileptonic,inclusive";
TString IsPrompt = "(!TIsSS && TEventTruth !=2)";
TString IsFakeSS = "(TIsSS && TEventTruth == 2)";
//============================================================== Functions
void CreatePlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins, TString Xtitle, TString name, TString options="");
void AddSamples(TString options = "ttbar,FullSim");
Float_t GetFSRsf(Float_t n, Float_t v);
void SetPaths();
void ScaleFakes();

//============================================================== Systematic uncertainties
TString bkgSyst  = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,UnclMET";
TString expSyst  = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,UnclMET";
TString T2ttSyst = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,UnclMET";
TString FSSyst   = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,JER,MET";


const int nSig = 30;
TString signalNames[nSig] = {"T2tt_mStop167_mLsp0", "T2tt_mStop175_mLsp0", "T2tt_mStop182_mLsp0", "T2tt_mStop175_mLsp7", "T2tt_mStop182_mLsp7", "T2tt_mStop190_mLsp7", "T2tt_mStop182_mLsp15", "T2tt_mStop190_mLsp15", "T2tt_mStop197_mLsp15", "T2tt_mStop190_mLsp22", "T2tt_mStop197_mLsp22", "T2tt_mStop205_mLsp22", "T2tt_mStop197_mLsp30", "T2tt_mStop205_mLsp30", "T2tt_mStop212_mLsp30", "T2tt_mStop205_mLsp37", "T2tt_mStop212_mLsp37", "T2tt_mStop220_mLsp37", "T2tt_mStop212_mLsp45", "T2tt_mStop220_mLsp45", "T2tt_mStop227_mLsp45", "T2tt_mStop220_mLsp52", "T2tt_mStop227_mLsp52", "T2tt_mStop235_mLsp52", "T2tt_mStop227_mLsp60", "T2tt_mStop235_mLsp60", "T2tt_mStop242_mLsp60", "T2tt_mStop235_mLsp67", "T2tt_mStop242_mLsp67", "T2tt_mStop242_mLsp75"};
TString signalPrNames[nSig] = {"T2tt_167p5_1p0", "T2tt_175p0_1p0", "T2tt_182p5_1p0", "T2tt_175p0_7p5", "T2tt_182p5_7p5", "T2tt_190p0_7p5", "T2tt_182p5_15p0", "T2tt_190p0_15p0", "T2tt_197p5_15p0", "T2tt_190p0_22p5", "T2tt_197p5_22p5", "T2tt_205p0_22p5", "T2tt_197p5_30p0", "T2tt_205p0_30p0", "T2tt_212p5_30p0", "T2tt_205p0_37p5", "T2tt_212p5_37p5", "T2tt_220p0_37p5", "T2tt_212p5_45p0", "T2tt_220p0_45p0", "T2tt_227p5_45p0", "T2tt_220p0_52p5", "T2tt_227p5_52p5", "T2tt_235p0_52p5", "T2tt_227p5_60p0", "T2tt_235p0_60p0", "T2tt_242p5_60p0", "T2tt_235p0_67p5", "T2tt_242p5_67p5", "T2tt_242p5_75p0"};


//============================================================== SaveFiles (Main function)
void SaveFiles(TString options = ""){
    CreatePlot("TChannel", BaselineCut, "ElMu", 1, 0, 10,  0,    "Yields",       "CutAndCount", options);
//      CreatePlot("TMT2",     BaselineCut + " && TMT2 > 0", "ElMu", 21, 0, 105, 0, "M_{T2} [GeV]", "MT2"        , options);
//    CreatePlot("TMT2",     BaselineCut + " && TMT2 == 0", "ElMu", 1, 0, 1, 0, "M_{T2} [GeV]", "MT2_eq0"        , options);
//    CreatePlot("TMT2",     BaselineCut, "ElMu", 21, 0, 105, 0, "M_{T2} [GeV]", "MT2_fullShape"        , options);
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

  // PDF uncertainties on signal
  cout << ">>> CREATING PDF histograms from T2tt_225_50_FS_xqcut20..." << endl;
  PDFunc* pdfSignal = new PDFunc(pathToTree + "T2tt/", "T2tt_225_50_FS_xqcut20", "tree", cut+" && !TIsSS", chan, var, nbins, bin0, binN); // Get pdf variations from this sample
  Histo* pdfNom  = pdfSignal->GetSystHisto("nom","ME")->CloneHisto("pdfNom");
  Histo* pdfUp   = pdfSignal->GetSystHisto("up",   "ME")->CloneHisto();
  Histo* pdfDo   = pdfSignal->GetSystHisto("down", "ME")->CloneHisto();
  Histo* pdfDifUp = new Histo("pdfDifUp", "pdfDifUp", nbins, bin0, binN); // Two histograms with up and down variations
  Histo* pdfDifDo = new Histo("pdfDifDo", "pdfDifDo", nbins, bin0, binN);

  Histo* hu; Histo* hd; float nn; float vvu; float vvd; float varUp; float varDo; float nom;
  Int_t nSign = p->VSignals.size(); 
  TString prname; TString sampname;
  for(int i = 0; i < nSign; i++){
    prname   = p->VSignals.at(i)->GetProcess();
    hu = p->VSignals.at(i)->CloneHisto(prname+"_MEScaleUp");
    hd = p->VSignals.at(i)->CloneHisto(prname+"_MEScaleDo");
    for(int k = 1; k <= nbins; k++){
      nn = pdfNom->GetBinContent(k);
      vvu = pdfUp->GetBinContent(k);
      vvd = pdfDo->GetBinContent(k);
      nom = hu->GetBinContent(k);
      varUp = nom*(1+fabs(nn-vvu)/nn);
      varDo = nom*(1-fabs(nn-vvd)/nn);
      hu->SetBinContent(k, varUp);
      hd->SetBinContent(k, varDo);
    }
    hu->SetDirectory(0); hd->SetDirectory(0);
    p->PrepareHisto(hu, prname+Form("%i_Up",i), prname, itSys, 0, "SignalScaleUp");
    p->PrepareHisto(hd->CloneHisto(Form("%s_Do%i",prname.Data(),i)), prname+Form("%i_Do",i), prname, itSys, 0, "SignalScaleDown");
  }
  p->AddToSystematicLabels("SignalScale");

  // Scale fsr
  Float_t n  = p->GetYield("ttbar"); 
  Float_t vu = p->GetYield("ttbar", "fsrUp");
  Float_t vd = p->GetYield("ttbar", "fsrDown");
  p->ScaleSys("ttbar", "fsrUp",   GetFactorFSR(n, vu));
  p->ScaleSys("ttbar", "fsrDown", GetFactorFSR(n, vd));
  
  if(name != "") p->SetVarName(name + "_" + chan);
  p->doData = true;
  p->SetYieldsTableName("StopYields"); 
  p->PrintSystYields();
  p->SetTableFormats("%2.0f");
  p->PrintBinsYields("tex, txt, html");
  p->SaveHistograms();
  delete p;
}


void AddSamples(TString options){
  //>>> Default options
  if(options == "") options = "FullSim"; //options = "ttbar,FullSim,xqcut,inclusive,dilep";
  cout << "[SaveFiles] Options: " << options << endl;

  p->SetCut(BaselineCut + " && " + IsPrompt);

  //>>> Bkgs and data
  p->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10, bkgSyst);
  p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3,  bkgSyst);
  p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8,   bkgSyst);
  p->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta,   bkgSyst);
  if(doTopPtReweight)  p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
  if(doMetResReweight) p->SetWeight("GetMetResNorm(TMET)*TWeight");
  p->AddSample("TTbar_Powheg",                                         "ttbar",     itBkg, kRed+1,     expSyst);
  p->SetWeight("TWeight");
  p->SetCut(BaselineCut + " && !TIsSS");
  p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  if(doFakesFromData){
    p->SetPath(pathToTree + "/SkimSS/Full2016Dataset/"); 
    p->SetWeight("TWeight");
    p->SetCut(BaselineCut + " && TIsSS");
    p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Nonprompt", itBkg, 1, bkgSyst, "noScaleLumi,noSys"); // Data SS

/*    Histo* hDataSS = (p->GetHisto("Nonprompt"))->CloneHisto("HistNonpromptData");
    vector<TString> VbkgSyst = TStringToVector(bkgSyst);
    for(Int_t i = 0; i < (int) VbkgSyst.size(); i++){
      TString iS = VbkgSyst.at(i);
      p->PrepareHisto(hDataSS, "Nonprompt"+iS, "Nonprompt", itSys, 1, iS+"Up");
      p->PrepareHisto(hDataSS, "Nonprompt"+iS, "Nonprompt", itSys, 1, iS+"Down");
    }*/

    p->SetPath(pathToTree + "/SkimSS/");
    p->SetCut(BaselineCut + "&& TIsSS && TEventTruth != 2"); // Prompt of flips, same-sign
    p->SetWeight("(-1*TWeight)"); // Substraction

    p->AddSample("TTbar_Powheg",  "Nonprompt",    itBkg, 1, bkgSyst);
    p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Nonprompt",  itBkg, 1, bkgSyst);
    p->AddSample("TW, TbarW, WZ, WW, ZZ, TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",  "Nonprompt", itBkg, 1, bkgSyst);
  }
  else  p->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG",          "Nonprompt", itBkg, kGray,      bkgSyst);

  //>>> ttbar systematics
  if(options.Contains("ttbar")){
    // TopPt
    p->SetPath(pathToTree);
    p->SetCut(BaselineCut + " && " + IsPrompt);
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

    // FSR
    p->SetCut(BaselineCut + " && !TIsSS");
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
    p->SetCut(BaselineCut + " && !TIsSS");
    p->SetPathSignal(pathToTree + "T2tt/");

    //p->AddSample("T2tt_200_50_FS_summer", "SFS_200_50", itSignal, kCyan, T2ttSyst);
    //p->AddSample("T2tt_225_50_FS_summer", "SFS_225_50", itSignal, kPink, T2ttSyst);
    //p->AddSample("T2tt_250_50_FS_summer", "SFS_250_50", itSignal, 1,     T2ttSyst);
    //p->AddSample("T2tt_200_50_FS_xqcut20", "SFS_200_50_xqcut20", itSignal, kCyan, T2ttSyst);
    p->AddSample("T2tt_225_50_FS_xqcut20", "SFS_225_50_xqcut20", itSignal, kPink, T2ttSyst);
    //_p->AddSample("T2tt_250_50_FS_xqcut20", "SFS_250_50_xqcut20", itSignal, 1,     T2ttSyst);


    for(int i = 0; i < nSig; i++){
      p->SetWeight("TWeight*TISRweight");
      p->AddSample(signalNames[i], signalPrNames[i], itSignal, 1, T2ttSyst); 
      p->SetWeight("TWeight*(3*TISRweight-1)/2");
      p->AddSample(signalNames[i], signalPrNames[i], itSys, 1, "SignalISRUp"); 
      p->SetWeight("TWeight*(TISRweight+1)/2");
      p->AddSample(signalNames[i], signalPrNames[i], itSys, 1, "SignalISRDown"); 
    }
    p->SetWeight("TWeight");
  }
}

void SetPaths(){
  TString outdir = "output/mt2/";
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = true;
  p->doData = true;
  p->SetLimitFolder(outdir);
  p->SetPlotFolder(outdir);
}

void ScaleFakes(){
  if(doFakesFromData){
/*    p->AddNormUnc("PromptSub_ttbar", 0.06);
    p->AddNormUnc("PromptSub_DY", 0.15);
    p->AddNormUnc("PromptSub_Other", 0.30);
    p->GroupProcesses("PromptSub_ttbar,PromptSub_DY,PromptSub_Other", "Nonprompt");*/
    p->ScaleProcess("PromptSub", transferFactor);
    p->ScaleProcess("Nonprompt", transferFactor);
  }
  else{
    Float_t fakeSF = 1.5;
    p->ScaleProcess("Nonprompt", fakeSF);
  }
}

