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
//    a->DrawSSCR("nombre");
//
//======================================================================================================

R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)

class SSCR{

  public:
    SSCR(TString path = "", TString basecut = "", TString sys = ""){
      if(path    == "") pathToTree = "/pool/ciencias/userstorage/juanr/stop/jan5/";
      if(basecut == "") BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
      if(sys     == "") systematics = "stat";
      NameOfTree = "tree";
      SScut      = "TNJets >= 2 && TNBtags >= 1 && TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
      var        = "TMT2"; 
      chan       = "ElMu"; 
      set        = false;
      ngbins = 21;
      gbins[0]  = 0;   gbins[1]  = 5;   gbins[2]  = 10;  gbins[3]  = 15;  gbins[4] = 20; 
      gbins[5]  = 25;  gbins[6]  = 30;  gbins[7]  = 35;  gbins[8]  = 40;  gbins[9] = 45; 
      gbins[10] = 50;  gbins[11] = 55;  gbins[12] = 60;  gbins[13] = 65;  gbins[14] = 70; 
      gbins[15] = 75;  gbins[16] = 80;  gbins[17] = 85;  gbins[18] = 90;  gbins[19] = 95; 
      gbins[20] = 100; gbins[21] = 200; 
     /* ngbins = 20;
      gbins[0]  = 0;   gbins[1]  = 5;   gbins[2]  = 10;  gbins[3]  = 15;  gbins[4] = 20; 
      gbins[5]  = 25;  gbins[6]  = 30;  gbins[7]  = 35;  gbins[8]  = 40;  gbins[9] = 45; 
      gbins[10] = 50;  gbins[11] = 55;  gbins[12] = 60;  gbins[13] = 65;  gbins[14] = 70; 
      gbins[15] = 75;  gbins[16] = 80;  gbins[17] = 100;  gbins[18] = 120;  gbins[19] = 140; gbins[20] = 200; */
    };
    ~SSCR(){if(p) delete p; if(g) delete g;}

    void SetSF();
    void PrintSStable(TString name = "");
    Float_t GetFakeSF();
    Float_t GetFakeSFerr();
    void DrawSSCR(TString name = "");
    void SetPathToTree(TString t){ pathToTree = t;}
    void SetNameOfTree(TString t){ NameOfTree = t;}
    void SetBaselineCut(TString t){ BaselineCut = t;}
    void SetSystematics(TString t){ systematics = t;}
    void SetSScut(TString t){ SScut = t;}
    void SetVar(TString t){ var = t;}
    void SetChan(TString t){ chan = t;}
    void SetPlotFolder(TString t){ plotfolder = t;}

  protected:
    Float_t gbins[22];
    Int_t ngbins;

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
  if(name == "") name = "SSCR";
  if(plotfolder != "") gSystem->mkdir(plotfolder, kTRUE);
  p->SetPlotFolder(plotfolder);
  p->SetLegendTextSize(0.045);
  p->SetLegendPosition(0.70, 0.70, 0.93, 0.93);
  p->SetRatioErrorColor(kTeal-2);
  p->SetRatioErrorStyle(3244);
  p->SetStackErrorStyle(3244);
  p->SetRatioMin(0); p->SetRatioMax(5);
  p->doSetLogy = false;
  p->SetOutputName(name);
  p->DrawStack();
  p->SetOutputName(name+"_log");
  p->doSetLogy = true;
  p->DrawStack();
}


// =============================================================================================================
// Calculate all the numbers
// =============================================================================================================

void SSCR::SetSF(){
  if(set) return;
  set = true;
  SetPlots();
  SetNumbers();
  SetErrors();
}

void SSCR::SetPlots(){
  set = true;
  g = new Plot(var, BaselineCut, chan, ngbins, gbins, "Title", "M_{T2} [GeV]");
  g->SetPath(pathToTree); g->SetTreeName(NameOfTree);
  g->AddSample("WJetsToLNu_MLM, TTbar_Powheg_Semi, WGToLNuG", "Nonprompt", itBkg, kGray+1, "0");
  g->AddSystematic(systematics);
  g->AddNormUnc("Nonprompt", 0.30);

  p = new Plot(var, SScut, chan, ngbins, gbins, "Title2", "M_{T2} [GeV]");
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->SetPathData(  pathToTree + "/Full2016Dataset/");
  p->verbose = false;
  p->SetVarName("MT2");

  p->AddSample("WZ, WW, ZZ",                                           "VV",        itBkg, kYellow-10);
	p->AddSample("TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu",              "ttV",       itBkg, kOrange-3);
	p->AddSample("DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO", "DY",        itBkg, kAzure-8);
	p->AddSample("TW, TbarW",                                            "tW",        itBkg, kMagenta);
	p->AddSample("WJetsToLNu_MLM,TTbar_Powheg_Semi,WGToLNuG",            "Nonprompt", itBkg, kGray+1, "0");
	p->AddSample("TTbar_Powheg",                                         "ttbar",     itBkg, kRed+1, "0");
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data",      itData);

  p->AddSystematic(systematics);
  p->AddNormUnc("ttbar", 0.05);
  p->AddNormUnc("VV", 0.30);
  p->AddNormUnc("ttV", 0.30);
  p->AddNormUnc("DY", 0.15);
  p->AddNormUnc("tW", 0.30);
  p->AddNormUnc("Nonprompt", 0.50);

}

void SSCR::SetNumbers(){
  FakeOS       = g->GetYield("Nonprompt");
  Prompt       = p->GetYield("VV") + p->GetYield("ttW") + p->GetYield("DY") + p->GetYield("tW") + p->GetYield("ttbar");
  FakeSS       = p->GetYield("Nonprompt");
  data         = p->GetYield("Data");
  OSSS         = FakeOS/FakeSS;
  FakeDD       = (data-Prompt)*OSSS;
  SF           = FakeDD/FakeOS;
}

void SSCR::SetErrors(){
  FakeOS_err   = TMath::Abs(FakeOS - g->GetYield("Nonprompt", "stat"));
  Prompt_err   = TMath::Sqrt( (p->GetYield("VV")    - p->GetYield("VV"   ,"stat"))*(p->GetYield("VV") - p->GetYield("VV"   ,"stat"))   + 
                              (p->GetYield("ttW")   - p->GetYield("ttW"  ,"stat"))*(p->GetYield("ttW")   - p->GetYield("ttW"  ,"stat")) +
                              (p->GetYield("DY")    - p->GetYield("DY"   ,"stat"))*(p->GetYield("DY")    - p->GetYield("DY"   ,"stat")) +
                              (p->GetYield("tW")    - p->GetYield("tW"   ,"stat"))*(p->GetYield("tW")    - p->GetYield("tW"   ,"stat")) +
                              (p->GetYield("ttbar") - p->GetYield("ttbar","stat"))*(p->GetYield("ttbar") - p->GetYield("ttbar","stat")) );
  FakeSS_err   = TMath::Abs(FakeSS - p->GetYield("Nonprompt", "stat"));
  data_err     = TMath::Sqrt(p->GetYield("Data"));
  OSSS_err     = OSSS*(fabs(FakeOS_err/FakeOS) + fabs(FakeSS_err/FakeSS));
  //(data-Prompt)*OSSS;
  FakeDD_err   = TMath::Sqrt(FakeDD*fabs(OSSS_err/OSSS)*FakeDD*fabs(OSSS_err/OSSS) +
                (FakeDD-(data+data_err-Prompt)*OSSS)*(FakeDD-(data+data_err-Prompt)*OSSS) +
                (FakeDD-((data-(Prompt+Prompt_err))*OSSS))*(FakeDD-((data-(Prompt+Prompt_err))*OSSS)) );
  SF_err       = SF*(fabs(FakeDD_err/FakeDD) + fabs(FakeOS_err/FakeOS));
}



















/*    cout << "=======>  Control region with SS dileptons  <======="     << endl;
    cout << "----------------------------------------------------"     << endl;
    cout << "# Fake estimate (W+Jets + semileptonic tt) = " << Prompt  << endl;
    cout << "# Prompt background                        = " << FakeSS  << endl;
    cout << "# Data                                     = " << data    << endl;
    cout << "# OS/SS prediction                         = " << OSSS    << endl;
    cout << "# Fake rate estimate                       = " << FakeDD  << endl;
    cout << "----------------------------------------------------"     << endl;
    cout << "# Scale Factor                             = " << SF      << endl;
    cout << "===================================================="     << endl;*/
