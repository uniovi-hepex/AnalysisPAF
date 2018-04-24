//======================================================================================================
//  # Description:
//    Class to calculate a SF to scale fake rate estimate from MC
//    By default: MT2 shape.
//    Binning not configured to universal use...
//
//  # Usage:
//    SSCR *a = new SSCR();
//    a->SetPathToTree("Path_To_Mini_Trees");
//    Float_t NompromptSF = a->GetFakeSF();
//    Float_t NompromptSF_err = a->GetFakeSFerr();
//    a->SetPlotFolder(dir);
//    a->DrawSSCR("nom");
//
//  # Obtain the mt2 histogram to use in the analysis:
//    SSCR *a = new SSCR();
//    Histo* h   = a->GetHistogram();
//    Histo* hUp = a->GetHistogram("up");
//    Histo* hDo = a->GetHistogram("down");
//    Histo* hStatUp = a->GetHistogram("statup");
//    Histo* hStatDo = a->GetHistogram("statdown");
//
//======================================================================================================

R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)

// OS_fakes:MC / SS_fakes_MC using also events with mt2 = 0
float transferFactor     = 1.22;
float transferFactor_unc = 0.13;

class SSCR{

  TString systunc = "Trig,JES,Btag,MisTag,ElecEff,MuonEff,PU,MuScale,ElScale,unclMET";

  public:
    SSCR(TString path = "", TString basecut = "", TString sys = ""){
      pathToTree  = (path    != "")? path    : "/pool/ciencias/userstorage/juanr/stop/apr2018/";
      BaselineCut = (basecut != "")? basecut : "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25 && TMT2 > 0";
      systematics = (sys     != "")? sys     : "stat";
      NameOfTree = "tree";
      SScut      = BaselineCut; SScut.ReplaceAll("!TIsSS", "TIsSS");//"TNJets >= 2 && TNBtags >= 1 && TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25 && TMT2 > 0";
      var        = "TMT2"; 
      chan       = "ElMu"; 
      set        = false;
      setHisto   = false;
      nbins = 21;
      bin0 = 0; binN = 0;
      gbins[0]  = 0;   gbins[1]  = 5;   gbins[2]  = 10;  gbins[3]  = 15;  gbins[4] = 20; 
      gbins[5]  = 25;  gbins[6]  = 30;  gbins[7]  = 35;  gbins[8]  = 40;  gbins[9] = 45; 
      gbins[10] = 50;  gbins[11] = 55;  gbins[12] = 60;  gbins[13] = 65;  gbins[14] = 70; 
      gbins[15] = 75;  gbins[16] = 80;  gbins[17] = 85;  gbins[18] = 90;  gbins[19] = 95; 
      gbins[20] = 100; gbins[21] = 105; 
     /* nbins = 20;
      gbins[0]  = 0;   gbins[1]  = 5;   gbins[2]  = 10;  gbins[3]  = 15;  gbins[4] = 20; 
      gbins[5]  = 25;  gbins[6]  = 30;  gbins[7]  = 35;  gbins[8]  = 40;  gbins[9] = 45; 
      gbins[10] = 50;  gbins[11] = 55;  gbins[12] = 60;  gbins[13] = 65;  gbins[14] = 70; 
      gbins[15] = 75;  gbins[16] = 80;  gbins[17] = 100;  gbins[18] = 120;  gbins[19] = 140; gbins[20] = 200; */
    };
    ~SSCR(){if(p) delete p; if(g) delete g;}

    //static void IncludeFakes(Plot* p, TString name = "Nonpropmt", Int_t color = kGray+2, TString var = "", TString cut = "", TString chan = "", TString path = "", int nb = 0, float b0 = 0, float bN = 0);
    static void IncludeFakes(Plot* p, TString name = "Nonprompt", Int_t color = kGray+2);

    Histo* GetHistogram(TString dir = "");
    void SetHistos();
    void SetSF();
    void PrintSStable(TString name = "");
    Float_t GetFakeSF();
    Float_t GetFakeSFerr();
    void DrawSSCR(TString name = "");
    void DrawOS(TString name = "");
    void SetPathToTree(TString t){ pathToTree = t;}
    void SetNameOfTree(TString t){ NameOfTree = t;}
    void SetBaselineCut(TString t){ BaselineCut = t;}
    void SetSystematics(TString t){ systematics = t;}
    void SetSScut(TString t){ SScut = t;}
    void SetVar(TString t){ var = t;}
    void SetChan(TString t){ chan = t;}
    void SetPlotFolder(TString t){ plotfolder = t;}
    void SetBins(Int_t nb, Float_t b0, Float_t bN){nbins = nb; bin0 = b0; binN = bN;};

  protected:
    Float_t gbins[22];
    Int_t nbins;
    Float_t bin0;
    Float_t binN;

    TString pathToTree;
    TString NameOfTree;
    TString BaselineCut;
    TString SScut;
    TString var;
    TString chan;
    TString systematics;
    TString plotfolder;

    Float_t FakeOS; Float_t FakeOS_err;
    Float_t Prompt; Float_t Prompt_err;
    Float_t FakeSS; Float_t FakeSS_err;
    Float_t OSSS;   Float_t OSSS_err;
    Float_t FakeDD; Float_t FakeDD_err;
    Float_t SF;     Float_t SF_err;
    Float_t data;   Float_t data_err;
    Bool_t set;
    Plot* p;
    Plot* g;
    Plot* ph;
    Histo* h;
    Histo* hUp;
    Histo* hDo;
    Histo* hStatUp;
    Histo* hStatDo;
    Bool_t setHisto;

    void SetPlots();
    void SetNumbers();
    void SetErrors();
};

Float_t SSCR::GetFakeSF(){
  if(!set) SetSF(); 
  return SF;
}

Float_t SSCR::GetFakeSFerr(){
  if(!set) SetSF(); 
  return SF_err;
}

//====================================================================
// Print a table
//====================================================================

void SSCR::PrintSStable(TString name){
  if(!set) SetSF(); 
  if(name == "") name = "SSCRtable";
  if(plotfolder == "") plotfolder = "./";

  TResultsTable t(7, 1, 1); // One column, has errors
  t.SetRowTitleHeader("Fake rate normalization from same-sign CR");

  t.SetRowTitle(0, "Fake rate estimate from MC (OS)");
  t[0][0].SetContent(FakeOS); t[0][0].SetError(FakeOS_err);
  t.SetRowTitle(1, "Fake rate estimate from MC (SS)");
  t[1][0].SetContent(FakeSS); t[1][0].SetError(FakeSS_err);
  t.SetRowTitle(2, "Prompt background estimate from MC");
  t[2][0].SetContent(Prompt); t[2][0].SetError(Prompt_err);
  t.SetRowTitle(3, "Observed data in same-sign region");
  t[3][0].SetContent(data); t[3][0].SetError(data_err);
  t.SetRowTitle(4, "OS/SS prediction from fakes MC");
  t[4][0].SetContent(OSSS); t[4][0].SetError(OSSS_err);

  t.AddVSeparation(4);
  t.SetRowTitle(5, "Fake rate estimate from data");
  t[5][0].SetContent(FakeDD); t[5][0].SetError(FakeDD_err);

  t.AddVSeparation(5);
  t.SetRowTitle(6, "Scale factor");
  t[6][0].SetContent(SF); t[6][0].SetError(SF_err);

  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  t.SaveAs(gSystem->ExpandPathName(plotfolder + "/" + name + ".tex"));
  t.SaveAs(gSystem->ExpandPathName(plotfolder + "/" + name + ".html"));
  t.SaveAs(gSystem->ExpandPathName(plotfolder + "/" + name + ".txt"));
}

//====================================================================
// Draw a plot 
//====================================================================

void SSCR::DrawSSCR(TString name){
  if(!set) SetSF(); 
  cout << ">> SET!\n";
  if(name == "") name = "SSCR";
  if(plotfolder != "") gSystem->mkdir(plotfolder, kTRUE);
  p->doSignal = false;
  p->SetTitleY("Events / 5 GeV");
  p->doYieldsInLeg = false;
  p->SetPlotFolder(plotfolder);
  p->SetLegendTextSize(0.045);
  p->SetLegendPosition(0.70, 0.45, 0.93, 0.93);
  p->SetRatioErrorColor(kTeal-2);
  p->SetRatioErrorStyle(3244);
  p->SetStackErrorStyle(3244);
  p->SetRatioMin(0); p->SetRatioMax(2.5);
  cout << ">> Drawing..." << endl;
  p->doSetLogy = false;
  p->SetOutputName(name);
  p->DrawStack();
  p->SetOutputName(name+"_log");
  p->doSetLogy = true;
  p->DrawStack();
}

void SSCR::DrawOS(TString name){
  // Draw in the OS region:
  // - Estimate using SS data + OS/SS factor
  // - Fakes from MC using MC truth
  if(!set) SetSF(); 
  if(name == "") name = "SSCR_OS";
  g->doSignal = false;
  g->doData = true;
  cout << "Applying OS/SS SF: " << OSSS << endl;
  g->ScaleProcess("Fakes from data", OSSS);
    g->SetDataTag("Fakes from SS data");
    g->SetDataStyle("hist");
    g->PrepareHisto(g->VSignals.at(0), "PromptSubs", "Fakes from data", itData, 1);
  g->SetTitleY("Events / 5 GeV");
  g->doYieldsInLeg = false;
  g->SetPlotFolder(plotfolder);
  g->SetLegendTextSize(0.045);
  g->SetLegendPosition(0.70, 0.45, 0.93, 0.93);
  g->SetRatioErrorColor(kTeal-2);
  g->SetRatioErrorStyle(3244);
  g->SetStackErrorStyle(3244);
  g->SetRatioMin(0); g->SetRatioMax(2.5);
//  g->SetPlotMaximum(160);
//  g->SetRatioYtitle("S/B");
  //g->SetSignalStyle("pe");
  //g->VSignals.at(0)->SetMarkerStyle(20);
  //g->VSignals.at(0)->SetMarkerSize(1.2);
  cout << ">> Drawing..." << endl;
  g->doSetLogy = false;
  g->SetOutputName(name);
  g->DrawStack();
}




// =============================================================================================================
// Calculate all the numbers
// =============================================================================================================

void SSCR::SetHistos(){
  if(setHisto) return;
  if(bin0 == binN) ph = new Plot(var, SScut, chan, nbins, gbins);
  else             ph = new Plot(var, SScut, chan, nbins, bin0, binN);

  ph->SetTreeName(NameOfTree);
  ph->SetPath(pathToTree + "/Full2016Dataset/"); 
  ph->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "data", itBkg, 1, "", "noScaleLumi"); // Data SS

  ph->SetPath(pathToTree); 
  ph->SetCut(SScut + " && (TEventTruth == 0 || TEventTruth == 1 || TEventTruth == 3)"); // Prompt or flips
  ph->SetWeight("(-1*TWeight)");
  ph->AddSample("TTbar_Powheg",  "ttbar",    itBkg, 1, systunc);
  ph->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",  itBkg, 1, systunc);
  ph->AddSample("TW, TbarW, WZ, WW, ZZ, TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",  "Other", itBkg, 1, systunc);

  ph->AddSystematic("stat");
  ph->AddNormUnc("ttbar", 0.06);
  ph->AddNormUnc("DY", 0.15);
  ph->AddNormUnc("Other", 0.30);

//  ph->ScaleProcess();
  
  h       = ph->GetAllBkgClone("Fakes");
  hUp     = ph->GetAllBkgClone("FakesUp");
  hDo     = ph->GetAllBkgClone("FakesDown");
  hStatUp = ph->GetAllBkgClone("FakesStatUp");
  hStatDo = ph->GetAllBkgClone("FakesStatDown");
  for(Int_t i = 1; i <= nbins; i++){
    hUp->SetBinContent(i, hUp->GetBinContent(i) + hUp->GetBinError(i)); 
    hDo->SetBinContent(i, hDo->GetBinContent(i) - hDo->GetBinError(i)); 
  }

  // Scale for OS/SS transfer factor
  h      ->Scale(transferFactor);
  hUp    ->Scale(transferFactor);
  hDo    ->Scale(transferFactor);
  hStatUp->Scale(transferFactor+transferFactor_unc);
  hStatDo->Scale(transferFactor-transferFactor_unc);

  setHisto = true;
}

void SSCR::SetSF(){
  if(set) return;
  set = true;
  cout << ">> Setting plots..." << endl;
  SetPlots();
  cout << ">> Setting numbers..." << endl;
  SetNumbers();
  cout << ">> Setting errors..." << endl;
  SetErrors();
}

void SSCR::SetPlots(){
  set = true;
  g = new Plot(var, BaselineCut, chan, nbins, gbins, "Title", "M_{T2} [GeV]");
  g->SetPath(pathToTree); g->SetTreeName(NameOfTree);
  g->SetPathData(  pathToTree + "/Full2016Dataset/");

  // Fakes from OS MC
  g->SetCut(BaselineCut + " && (TEventTruth == 2)");
  g->AddSample("TTbar_Powheg_Semi", "t#bar{t} semilep", itBkg, kRed-7, systunc);
	g->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Other fakes",        itBkg, kTeal+6, systunc);
	g->AddSample("TTbar_Powheg",     "Other fakes",     itBkg, kPink-9, systunc);
	g->AddSample("TW, TbarW, WW, ZZ, WZ, WGToLNuG",          "Other fakes",        itBkg, kTeal+6, systunc);

  //>>> Fakes from Data SS
  g->SetCut(SScut);
  g->SetPathSignal(  pathToTree + "/Full2016Dataset/");
	g->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Fakes from data",      itSignal, 1, "", "noScaleLumi"); // Data SS
  g->SetLoopOptions("");

  //>>> Prompt substraction
  g->SetPathSignal(  pathToTree );
  g->SetCut(SScut + " && (TEventTruth == 0 || TEventTruth == 1 || TEventTruth == 3)"); g->SetWeight("(-1*TWeight)");
	g->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "Fakes from data",       itSignal, 1);
	g->AddSample("TTbar_Powheg",     "Fakes from data",    itSignal, 1);
	g->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Fakes from data",  itSignal, 1);
	g->AddSample("TW, TbarW, WZ, WW, ZZ",                                "Fakes from data", itSignal, 1);

  g->AddSystematic("stat");


  p = new Plot(var, SScut, chan, nbins, gbins, "Title2", "M_{T2} [GeV]");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = false;
  p->SetVarName("MT2");


  // Prompt SS
  p->SetCut(SScut + " && (TEventTruth == 0 || TEventTruth == 1)");
  p->AddSample("WZ, WW, ZZ",                                           "Prompt",       itBkg, kYellow-10, systunc);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "Prompt",       itBkg, 1, systunc);
	p->AddSample("TTbar_Powheg",      "Prompt", itBkg, kPink-9, systunc);
	p->AddSample("TTbar_Powheg_Semi",  "Prompt", itBkg, kRed-7, systunc);

  // Flips SS from tt, tW, DY 
  p->SetCut(SScut + " && TEventTruth == 3");
	p->AddSample("TTbar_Powheg",     "Flips",     itBkg, kAzure+5, systunc);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Flips",        itBkg, kAzure-8, systunc);
	p->AddSample("TW, TbarW, WZ, WW, ZZ",                                "Flips",        itBkg, kViolet-5, systunc);

  // Fakes SS from tt, tW, DY
  p->SetCut(SScut + " && (TEventTruth == 2)");// && (TEventTruth != 0 && TEventTruth != 1 && TEventTruth != 3)"); // && (TEventTruth != 3)
	p->AddSample("TTbar_Powheg",     "Other fakes",     itBkg, kRed+1, systunc);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "Other fakes",        itBkg, kGray+3, systunc);
	p->AddSample("TW, TbarW, WW",                                        "Other fakes",        itBkg, kGray+3, systunc);
	p->AddSample("WGToLNuG",               "Other fakes", itBkg, kGray+3, systunc);
	p->AddSample("TTbar_Powheg_Semi",      "t#bar{t} semilep", itBkg, kRed-7, systunc);
  //p->AddSample("WJetsToLNu_MLM",         "W+Jets", itBkg, kGray, systunc);


  p->SetCut(SScut);
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData); // Data SS

  p->AddSystematic("stat");
//  p->AddNormUnc("t#bar{t} dilep", 0.06);
//  p->AddNormUnc("Nonprompt", 0.60);

}

void SSCR::SetNumbers(){
  FakeOS       = g->GetYield("t#bar{t} semilep") + g->GetYield("Other fakes");
  Prompt       = p->GetYield("Prompt") + p->GetYield("Flips");
  FakeSS       = p->GetYield("t#bar{t} semilep") + p->GetYield("Other fakes");
  data         = p->GetYield("Data");
  OSSS         = FakeOS/FakeSS;
  FakeDD       = (data-Prompt)*OSSS;
  SF           = FakeDD/FakeOS;
}


void SSCR::SetErrors(){
  FakeOS_err   = TMath::Sqrt( (FakeOS - g->GetYield("t#bar{t} semilep", "stat"))*(FakeOS - g->GetYield("t#bar{t} semilep", "stat")) + (FakeOS - g->GetYield("Other fakes", "stat"))*(FakeOS - g->GetYield("Other fakes", "stat")) );
  Prompt_err   = TMath::Sqrt( (p->GetYield("Prompt") - p->GetYield("Prompt", "stat"))*(p->GetYield("Prompt") - p->GetYield("Prompt", "stat")) + (p->GetYield("Flips") - p->GetYield("Flips", "stat"))*(p->GetYield("Flips") - p->GetYield("Flips", "stat")) );
  FakeSS_err   = TMath::Sqrt( (FakeSS - p->GetYield("t#bar{t} semilep", "stat"))*(FakeSS - p->GetYield("t#bar{t} semilep", "stat")) + (FakeSS - p->GetYield("Other fakes", "stat"))*(FakeSS - p->GetYield("Other fakes", "stat")) );
  data_err     = TMath::Sqrt(p->GetYield("Data"));
  OSSS_err     = OSSS*(fabs(FakeOS_err/FakeOS) + fabs(FakeSS_err/FakeSS));
  //(data-Prompt)*OSSS;
  FakeDD_err   = TMath::Sqrt(FakeDD*fabs(OSSS_err/OSSS)*FakeDD*fabs(OSSS_err/OSSS) +
                (FakeDD-(data+data_err-Prompt)*OSSS)*(FakeDD-(data+data_err-Prompt)*OSSS) +
                (FakeDD-((data-(Prompt+Prompt_err))*OSSS))*(FakeDD-((data-(Prompt+Prompt_err))*OSSS)) );
  SF_err       = SF*(fabs(FakeDD_err/FakeDD) + fabs(FakeOS_err/FakeOS));
}


Histo* SSCR::GetHistogram(TString dir){
  SetHistos();
  if(dir.Contains("stat") || dir.Contains("Stat") || dir.Contains("STAT")){
    if     (dir == "Up" || dir == "up" || dir == "UP") return hStatUp;
    else if(dir == "Do" || dir == "do" || dir == "DO" || dir == "down" || dir == "Down" || dir == "DOWN") return hStatDo;
  }
  else{
    if     (dir == "Up" || dir == "up" || dir == "UP") return hUp;
    else if(dir == "Do" || dir == "do" || dir == "DO" || dir == "down" || dir == "Down" || dir == "DOWN") return hDo;
  }
  return h;
}

void SSCR::IncludeFakes(Plot* p, TString name, Int_t color){
  TString var  = p->GetVar();
  TString cut  = p->GetCut();
  TString chan = p->GetChan();
  TString path = p->GetPath();
  Int_t   nb   = p->GetNbins();
  Float_t b0   = p->GetBin0();
  Float_t bN   = p->GetBinN();

  cout << "  >> Include fakes for: \n"
       << "     variable:     " << var  << "\n"
       << "     cut     :     " << cut  << "\n"
       << "     channel :     " << chan << "\n"
       << "     bins    :     " << Form("[%i,%1.1f,%1.1f]\n", nb, b0, bN)
       << "     path    :     " << path << "\n";

  SSCR* a = new SSCR(path, cut);
  if(var != "")  a->SetVar(var);
  if(chan != "") a->SetChan(chan);
  if(nb != 0)  a->SetBins(nb, b0, bN);
  p->PrepareHisto(a->GetHistogram(),   "Nonprompt_h",       name, itBkg, color);
  p->PrepareHisto(a->GetHistogram(),   "Nonprompt_hUp",     name, itSys, color, name + "SystUp");
  p->PrepareHisto(a->GetHistogram(),   "Nonprompt_hDo",     name, itSys, color, name + "SystDown");
  p->PrepareHisto(a->GetHistogram(),   "Nonprompt_hStatUp", name, itSys, color, name + "StatUp");
  p->PrepareHisto(a->GetHistogram(),   "Nonprompt_hStatDo", name, itSys, color, name + "StatDown");

}

void SSCRrun(){
  SSCR* a = new SSCR();
  a->DrawSSCR("SSCR_temp");
  a->DrawOS("OS_temp");
  a->PrintSStable("table_temp");
}
