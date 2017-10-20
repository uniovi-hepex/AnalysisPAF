R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PlotterFunctions.C"

TString GetYields(TString cut);
TString NameOfTree = "tree";
TString pathToTree = "/pool/ciencias/userstorage/juanr/tttt/oct20_prueba/"; //oct19/";
TString CutNames[] = {"CRZ", "CRW", "SR1",  "SR2", "SR3", "SR4", "SR5", "SR6", "SR7", "SR8"}; Int_t nCutNames = 10;
TString Processes[] = {"ttW", "ttZ", "ttH", "ttVV", "Conv", "Rare SM", "Flips", "Fakes", "AllBkg", "Data", "tttt"}; Int_t nProcesses = 11;

TString baseline = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && !TIsOnZ && TPassTrigger && TPassMETFilters ";
TString CRZ      = "TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 &&  TIsOnZ && TPassTrigger && TPassMETFilters ";
TString CRW      = baseline + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets <= 5";
TString SR1      = baseline + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 6";
TString SR2      = baseline + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets == 7";
TString SR3      = baseline + "&& TNSelLeps == 2 && TNBtags == 2 && TNJets >= 8";
TString SR4      = baseline + "&& TNSelLeps == 2 && TNBtags == 3 && (TNJets == 5 || TNJets == 6)";
TString SR5      = baseline + "&& TNSelLeps == 2 && TNBtags == 3 && TNJets >= 7";
TString SR6      = baseline + "&& TNSelLeps == 2 && TNBtags >= 4 && TNJets >= 5";
TString SR7      = baseline + "&& TNSelLeps >= 3 && TNBtags == 2 && TNJets >= 5";
TString SR8      = baseline + "&& TNSelLeps >= 3 && TNBtags >= 3 && TNJets >= 4";

TString NoFake   = Form("TChannel == %i || TChannel == %i || TChannel == %i", i2lss, iTriLep, iSS1tau);

void YieldTable(){
  TString cutname;
  TString s;
  vector<TString> st;

  Int_t nrows = nCutNames; Int_t ncol = nProcesses;
  TResultsTable t(nrows, ncol, 0); 
  t.SetRowTitleHeader("Yields");
  t.SetFormatNum("%2.2f");
  for(Int_t i = 0; i < nrows; i++) t.SetRowTitle(i, CutNames[i]);
  for(Int_t i = 0; i < ncol;  i++) t.SetColumnTitle(i, Processes[i]);

  TString pr = "    ttW, ttZ, ttH, ttVV, Conv, Rare, Flip, Fake, All, Data, tttt"; 
  cout << pr << endl;
  for(Int_t i = 0; i < nCutNames; i++){
    cutname = CutNames[i];
    s = GetYields(cutname);
    st = TStringToVector(s);
    cout << Form("[%s] ", cutname.Data()) << s << endl;
    for(Int_t j = 0; j < nProcesses; j++) t[i][j] = st.at(j).Atof();
  }

  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  t.SaveAs("table.txt");
 gApplication->Terminate();
}

//void GetYields(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString varName){
//void GetYields(TString var, TString cut, TString Xtitle, TString varName){
TString GetYields(TString cutName){
  TString cut = "";
  if     (cutName == "CRW" ) cut = CRW ;
  else if(cutName == "CRZ" ) cut = CRZ ;
  else if(cutName == "SR1" ) cut = SR1 ;
  else if(cutName == "SR2" ) cut = SR2 ;
  else if(cutName == "SR3" ) cut = SR3 ;
  else if(cutName == "SR4" ) cut = SR4 ;
  else if(cutName == "SR5" ) cut = SR5 ;
  else if(cutName == "SR6" ) cut = SR6 ;
  else if(cutName == "SR7" ) cut = SR7 ;
  else if(cutName == "SR8" ) cut = SR8 ;
  else cut = baseline;


  TString var = "TChannel"; 
  TString chan = NoFake; TString Xtitle = "Counts";
  Float_t binN = 20; Float_t bin0 = 1; Int_t nbins = 1;
  TString varName = "prueba";
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  //p->verbose = true;
  if(varName != "") p->SetVarName(varName);
  TString cutForFakes = cut ;
  cutForFakes.ReplaceAll("TNSelLeps", "(TNSelLeps + TNFakeableLeps)");

  p->SetWeight("TWeight");
  p->AddSample("TTWToLNu",                                         "ttW",   itBkg,    kGreen+4); // ttW
  p->AddSample("TTZToLLNuNu, TTZToLL_M1to10",                      "ttZ",   itBkg,    kTeal+2);  // ttZ
  p->AddSample("TTHNonbb",                                         "ttH",   itBkg,    kTeal+2);  // ttH
  p->AddSample("TTZH, TTHH, TTZZ, TTWZ, TTTW, TTTJ, TTWH, TTWW",         "ttVV",  itBkg,    kTeal+2);  // ttVV
  p->AddSample("TGJets, TTGJets, WGToLNuG, ZGTo2LG",                "X+#gamma", itBkg, kViolet+2);  // X+gamma //"WZG_amcatnlo" "WWG_amcatnlo"
  p->AddSample("WZTo3LNu, WWTo2L2Nu, WpWpJJ, WWTo2L2Nu_DoubleScat, WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo, tZq_ll",    "Rare SM",  itBkg, kMagenta-7); // RareSM

  // Nonprompt from MC
  //p->AddSample("WJetsToLNu_MLM, DYJetsToLL_M5to50_MLM, DYJetsToLL_M50_MLM",                                 "Nonprompt", itBkg, kGray);
  //p->AddSample("TTbar_Powheg, TW_noFullyHadr, TbarW_noFullyHadr, T_tch, Tbar_tch, TToLeptons_sch_amcatnlo", "Nonprompt", itBkg);

  // Nonprompt from data
  p->SetCut(cutForFakes);
  p->SetChan("IsFake(TChannel)");
  p->SetWeight("FRweight(TNFakeableLeps, TFLep0Pt, TFLep1Pt, TFLep0Eta, TFLep1Eta, TFLep0Id, TFLep1Id)");
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",     "Nonprompt", itBkg, kGray, "0", "noScaleLumi");
  
  p->SetWeight("-1*TWeight*FRweight(TNFakeableLeps, TFLep0Pt, TFLep1Pt, TFLep0Eta, TFLep1Eta, TFLep0Id, TFLep1Id)");
  p->AddSample("WZTo3LNu, WpWpJJ,WWTo2L2Nu_DoubleScat, WWW, WWZ, WZZ, ZZZ, ZZTo4L, VHToNonbb_amcatnlo,TTZToLLNuNu, TTZToLL_M1to10, TTHNonbb, tZq_ll,TTWToLNu", "Nonprompt", itBkg, kGray, "0");

  // Charge misID
  p->SetCut(cut);
  p->SetChan(Form("TChannel == %i",i2LOS));
  p->SetWeight("FlipWeight(TLep0Pt, TLep0Eta, TLep0Id, TLep1Pt, TLep1Eta, TLep1Id)");
  p->AddSample("MuonEG, DoubleEG, DoubleMuon",     "Flips", itBkg, kGray, "0", "noScaleLumi");

  p->SetWeight("TWeight"); p->SetChan(chan);
  p->AddSample("TTTT",                                             "tttt",  itSignal, kRed+1);   // tttt signal
	p->AddSample("MuonEG, DoubleEG, DoubleMuon",                     "Data",  itData);             // Data

  Float_t ttW(0), ttZ(0), ttH(0), ttVV(0), conv(0), rare(0), flips(0), fakes(0), bkg(0), data(0), tttt(0);

  ttW  = p->GetYield("ttW");
  ttZ  = p->GetYield("ttZ");
  ttH  = p->GetYield("ttH");
  ttVV = p->GetYield("ttVV");
  conv = p->GetYield("X+#gamma");
  rare = p->GetYield("Rare SM");
  fakes = p->GetYield("Nonprompt");
  flips = p->GetYield("Flips");
  bkg = ttW + ttZ + ttH + conv + rare + fakes + ttVV + flips;
  data = p->GetYield("Data");
  tttt = p->GetYield("tttt");

  TString s = Form("%2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f", ttW, ttZ, ttH, ttVV, conv, rare, flips, fakes, bkg, data, tttt);

  delete p;

  return s;
}
  //p->SetWeight("TWeight*FlipWeight(Alt$(TLep_Pt[0], 0.), Alt$(TLep_Eta[0], 0.), Alt$(TLep_pdgId[0], 0), Alt$(TLep_Pt[1],0.), Alt$(TLep_Eta[1], 0.), Alt$(TLep_pdgId[1], 0))");
  //p->SetWeight("FRweight(TNFakeableLeps, Alt$(TFLep_Pt[0], 0.), Alt$(TFLep_Pt[1],0.), Alt$(TFLep_Pt[2],0.), Alt$(TFLep_Eta[0],0.), Alt$(TFLep_Eta[1],0.), Alt$(TFLep_Eta[2],0.), Alt$(TFLep_pdgId[0], 0.), Alt$(TFLep_pdgId[1],0.), Alt$(TFLep_pdgId[2],0.))");
