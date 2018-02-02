//#include "DrawPlots.C"
R__LOAD_LIBRARY(TResultsTable.C+)
#include "../TResultsTable.h"

TString path = "/pool/ciencias/userstorage/juanr/top/oct10/";
TString outputFolder = "/mnt_pool/ciencias_users/user/juanr/AnalysisPAF/plotter/top/xsec/";
double getYield(TString sample, TString chan = "ElMu", TString level = "2jets", TString syst = "0", bool isSS = false);
double yield(TString process, TString chan = "ElMu", TString level = "2jets", TString syst = "0", bool isSS = false);
double getStatError(TString sample, TString chan, TString level, TString syst = "0", bool isSS = false);


TH1F* loadDYHisto(TString lab = "MC", TString ch = "ElMu", TString lev = "2jets");

double GetDYDD(  TString ch = "ElMu", TString lev = "2jets", bool IsErr = false, bool docout = false);
double GetNonWDD(TString ch = "ElMu", TString lev = "2jets", bool IsErr = false, bool docout = false);
void PrintSFwithLevel();

double Lumi = 35851.45088;
TString Chan = "Elec";
TString Level = "1btag";

TH1F* loadDYHisto(TString lab, TString ch, TString lev){
  TString PlotName = "H_DY_InvMass";
  if(Chan != "ElMu") PlotName = "H_DY_InvMass_SF";
	TFile* f1; TH1F* h; TH1F* h1; TH1F* h2;
  if(lab != "MC" && lab != "DY" && lab != "data" && lab != "Data"){
    f1 = TFile::Open(path + "Tree_" + lab + ".root");
    f1->GetObject(PlotName + "_" + ch + "_" + lev, h);
    if(!lab.Contains("Single") && !lab.Contains("Double") && !lab.Contains("MuonEG")) h->Scale(Lumi);
  }
  else if(lab == "MC" || lab == "DY"){
    //h1 = loadDYHisto("DYJetsToLL_M5to50_MLM", ch, lev);
    h  = loadDYHisto("DYJetsToLL_M50_aMCatNLO", ch, lev);
    //h->Add(h1);
  }
  else{
    if     (ch == "ElMu"){
      h  = loadDYHisto("MuonEG", ch, lev);
      h1 = loadDYHisto("SingleElec", ch, lev);
      h2 = loadDYHisto("SingleMuon", ch, lev);
      h->Add(h1); h->Add(h2);
    }
    else if(ch == "Elec"){
      h  = loadDYHisto("DoubleEG", ch, lev);
      h1 = loadDYHisto("SingleElec", ch, lev);
      h->Add(h1);
    }
    else if(ch == "Muon"){
      h  = loadDYHisto("DoubleMuon", ch, lev);
      h1 = loadDYHisto("SingleMuon", ch, lev);
      h->Add(h1);
    }
  }
  h ->SetDirectory(0);
  return h;
}

void PrintSFwithLevel(){
  TResultsTable t(3, 3, true);
  t.SetFormatNum("%1.3f");
  t.SetRowTitleHeader(" ");

  t.SetColumnTitle(0, "$e^{+}e^{-}$");
  t.SetColumnTitle(1, "$\\mu^{+}\\mu^{-}$");
  t.SetColumnTitle(2, "$e^{\\pm}\\mu^{\\mp}$");

  t.SetRowTitle(0, "$\\geq$ 1 b tag");
  t.SetRowTitle(1, "$\\geq$ 2 jets");
  t.SetRowTitle(2, "Dilepton");

  t[0][0].SetContent(GetDYDD("Elec", "1btag", 0,0)); t[0][0].SetError(GetDYDD("Elec", "1btag", 1,0));
  t[0][1].SetContent(GetDYDD("Muon", "1btag", 0,0)); t[0][1].SetError(GetDYDD("Muon", "1btag", 1,0));
  t[0][2].SetContent(GetDYDD("ElMu", "1btag", 0,0)); t[0][2].SetError(GetDYDD("ElMu", "1btag", 1,0));
  t[1][0].SetContent(GetDYDD("Elec", "2jets", 0,0)); t[1][0].SetError(GetDYDD("Elec", "2jets", 1,0));
  t[1][1].SetContent(GetDYDD("Muon", "2jets", 0,0)); t[1][1].SetError(GetDYDD("Muon", "2jets", 1,0));
  t[1][2].SetContent(GetDYDD("ElMu", "2jets", 0,0)); t[1][2].SetError(GetDYDD("ElMu", "2jets", 1,0));
  t[2][0].SetContent(GetDYDD("Elec", "dilepton", 0,0)); t[1][0].SetError(GetDYDD("Elec", "dilepton", 1,0));
  t[2][1].SetContent(GetDYDD("Muon", "dilepton", 0,0)); t[1][1].SetError(GetDYDD("Muon", "dilepton", 1,0));
  t[2][2].SetContent(GetDYDD("ElMu", "dilepton", 0,0)); t[1][2].SetError(GetDYDD("ElMu", "dilepton", 1,0));

  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  t.SaveAs(outputFolder + "/DYDD_AllLevels.tex");
  t.SaveAs(outputFolder + "/DYDD_AllLevels.html");
  t.SaveAs(outputFolder + "/DYDD_AllLevels.txt");
}

double GetDYDD(TString ch, TString lev, bool IsErr, bool docout){
  Chan = ch;
//  ch = Chan; lev = Level;
  double Re     = 0; double N_ine  = 0; double N_oute = 0; double k_lle = 0; double SFel = 0;
	double R_erre = 0; double N_in_erre = 0; double N_out_erre = 0; double k_ll_erre = 0; double SFel_err = 0;

  double Rm     = 0; double N_inm  = 0; double N_outm = 0; double k_llm = 0; double SFmu = 0;
	double R_errm = 0; double N_in_errm = 0; double N_out_errm = 0; double k_ll_errm = 0; double SFmu_err = 0;

  double N_inem = 0; double N_in_errem = 0; double SFem = 0; double SFem_err = 0;

	Double_t nout_ee(0.),nin_ee(0.),nout_err_ee(0.),nin_err_ee(0.);
	Double_t nout_mm(0.),nin_mm(0.),nout_err_mm(0.),nin_err_mm(0.);
  
  TH1F* DYmm   = loadDYHisto("DY",  "Muon", lev);
  TH1F* DYee   = loadDYHisto("DY",  "Elec", lev);
  TH1F* Datamm = loadDYHisto("Data","Muon", lev);
  TH1F* Dataee = loadDYHisto("Data","Elec", lev);
  TH1F* Dataem = loadDYHisto("Data","ElMu", lev);

  Int_t low_in = DYmm->FindBin(76. );
  Int_t  up_in = DYmm->FindBin(106.);

  Double_t t1(0.),t2(0.);

  Int_t nbins = DYmm->GetNbinsX();
  nin_mm = DYmm->IntegralAndError(low_in, up_in, nin_err_mm); 
  nin_ee = DYee->IntegralAndError(low_in, up_in, nin_err_ee); 
  nout_mm = DYmm->IntegralAndError(0,low_in-1,nout_err_mm) + DYmm->IntegralAndError(up_in+1,nbins+2,t1);
  nout_ee = DYee->IntegralAndError(0,low_in-1,nout_err_ee) + DYee->IntegralAndError(up_in+1,nbins+2,t2);
  
	nout_err_mm += t1;
	nout_err_ee += t2;

/*
//-------------------------
  nout_mm = DYmm->Integral(0,200) - nin_mm;
  nout_ee = DYee->Integral(0,200) - nin_ee;
	nin_err_mm = TMath::Sqrt(nin_mm);
	nin_err_ee = TMath::Sqrt(nin_ee);
	nout_err_mm = TMath::Sqrt(nout_mm);
	nout_err_ee = TMath::Sqrt(nout_ee);
//-------------------------
 */

  Rm     = nout_mm/nin_mm;
  R_errm = (nout_err_mm/nout_mm + nin_err_mm/nin_mm)*Rm;
  Re     = nout_ee/nin_ee;
  R_erre = (nout_err_ee/nout_ee + nin_err_ee/nin_ee)*Re;

  N_inm  = Datamm->IntegralAndError(low_in, up_in, N_in_errm);
  N_ine  = Dataee->IntegralAndError(low_in, up_in, N_in_erre);
  N_inem = Dataem->IntegralAndError(low_in, up_in, N_in_errem);

  k_llm = TMath::Sqrt(nin_mm / nin_ee  );
  k_lle = TMath::Sqrt(nin_ee / nin_mm  );

	k_ll_errm = 0.5*(nin_err_mm/nin_mm + nin_err_ee/nin_ee)*k_llm;
	k_ll_erre = 0.5*(nin_err_ee/nin_ee + nin_err_mm/nin_mm)*k_lle;

	N_outm = Rm * (N_inm - 0.5 * N_inem * k_llm);
	N_oute = Re * (N_ine - 0.5 * N_inem * k_lle);

	Double_t dA(0.),dB(0.),dC(0.),dD(0.);

/*
//-------------------------
  dA = N_outm*R_errm/Rm;
  dB = Rm*N_in_errm;
  dC = Rm/2*k_llm*N_in_errem;
  dD = Rm/2*k_ll_errm*N_inem;
  N_out_errm = dA + dB + dC + dD;

  dA = N_oute*R_erre/Re;
  dB = Re*N_in_erre;
  dC = Re/2*k_lle*N_in_errem;
  dD = Re/2*k_ll_erre*N_inem;
  N_out_erre = dA + dB + dC + dD;
//-------------------------
*/

  double tmperr1; double tmperr2;
	tmperr1 =  N_in_erre/N_ine;
	tmperr2 =  N_in_errem/N_inem + k_ll_erre/k_lle;
	N_out_erre = R_erre/Re*(tmperr1 + 0.5*tmperr2) * N_oute;

	tmperr1 =  N_in_errm/N_inm;
	tmperr2 =  N_in_errem/N_inem + k_ll_errm/k_llm;
	N_out_errm = R_errm/Rm*(tmperr1 + 0.5*tmperr2) * N_outm;

  SFmu   = N_outm / nout_mm;
  SFel   = N_oute / nout_ee;
  SFem   = TMath::Sqrt(SFel * SFmu);

  SFmu_err = (N_out_errm/N_outm + nout_err_mm/nout_mm) * SFmu;
  SFel_err = (N_out_erre/N_oute + nout_err_ee/nout_ee) * SFel;
  SFem_err = 0.5*SFem*(SFel_err/SFel + SFmu_err/SFmu);

  if(docout){
    TResultsTable a(7+2, 3, true);
    a.SetFormatNum("%1.3f");
    a.SetRowTitleHeader("Level: " + lev);
    a.AddVSeparation(3); a.AddVSeparation(4); 
     a.AddVSeparation(5);

    a.SetColumnTitle(0, "$e^{+}e^{-}$");
    a.SetColumnTitle(1, "$\\mu^{+}\\mu^{-}$");
    a.SetColumnTitle(2, "$e^{\\pm}\\mu^{\\mp}$");

    a.SetRowTitle(0, "$N_{in}$ (MC)");
    a.SetRowTitle(1, "$n_{out}$ (MC)");
    a.SetRowTitle(2, "$R_{out/in}$  (MC)");
    a.SetRowTitle(3, "$k_{ll}$");
    a.SetRowTitle(4, "$N_{in} (D)$");
    a.SetRowTitle(5, "$N_{out}$");
    a.SetRowTitle(6, "SF(D/MC)");
      a.SetRowTitle(7, "Drell-Yan (MC)");
      a.SetRowTitle(8, "Drell-Yan (D)");

    a[0][0].SetContent(nin_ee); a[0][0].SetError(nin_err_ee);
    a[0][1].SetContent(nin_mm); a[0][1].SetError(nin_err_mm);

    a[1][0].SetContent(nout_ee); a[1][0].SetError(nout_err_ee);
    a[1][1].SetContent(nout_mm); a[1][1].SetError(nout_err_mm);

    a[2][0].SetContent(Re); a[2][0].SetError(R_erre);
    a[2][1].SetContent(Rm); a[2][1].SetError(R_errm);

    a[3][0].SetContent(k_lle); a[3][0].SetError(k_ll_erre);
    a[3][1].SetContent(k_llm); a[3][1].SetError(k_ll_errm);

    a[4][0].SetContent(N_ine); a[4][0].SetError(N_in_erre);
    a[4][1].SetContent(N_inm); a[4][1].SetError(N_in_errm);
    a[4][2].SetContent(N_inem); a[4][2].SetError(N_in_errem);

    a[5][0].SetContent(N_oute); a[5][0].SetError(N_out_erre);
    a[5][1].SetContent(N_outm); a[5][1].SetError(N_out_errm);

    a[6][0].SetContent(SFel); a[6][0].SetError(SFel_err);
    a[6][1].SetContent(SFmu); a[6][1].SetError(SFmu_err);
    a[6][2].SetContent(SFem); a[6][2].SetError(SFem_err);

        a[7][0].SetContent(yield("DY", "Elec", lev)); a[7][0].SetError(getStatError("DY", "Elec", lev));
        a[7][1].SetContent(yield("DY", "Muon", lev)); a[7][1].SetError(getStatError("DY", "Muon", lev));
        a[7][2].SetContent(yield("DY", "ElMu", lev)); a[7][2].SetError(getStatError("DY", "ElMu", lev));

        a[8][0].SetContent(yield("DY", "Elec", lev)*SFel); a[8][0].SetError(getStatError("DY", "Elec", lev)*SFel);
        a[8][1].SetContent(yield("DY", "Muon", lev)*SFmu); a[8][1].SetError(getStatError("DY", "Muon", lev)*SFmu);
        a[8][2].SetContent(yield("DY", "ElMu", lev)*SFem); a[8][2].SetError(getStatError("DY", "ElMu", lev)*SFem); 

    a.SetDrawHLines(false); a.SetDrawVLines(true); a.Print();
    a.SaveAs(outputFolder + "/DYDD_" + lev + ".tex");
    a.SaveAs(outputFolder + "/DYDD_" + lev + ".html");
    a.SaveAs(outputFolder + "/DYDD_" + lev + ".txt");
  }
  if(IsErr){
    if     (ch == "ElMu") return SFem_err;
    else if(ch == "Elec") return SFel_err;
    else if(ch == "Muon") return SFmu_err;
    //else if(ch == "Muon") return 0.96;
  }
  else{
    if     (ch == "ElMu") return SFem;
    else if(ch == "Elec") return SFel;
    else if(ch == "Muon") return SFmu;
  }
  cout << " DY SF -----> Wrong input values!!" << endl;
  return 1.;
}

double GetNonWDD(TString ch, TString lev, bool IsErr, bool docout){
//  ch = Chan;
//  lev = Level;
  double fake; double efake;
  double fakeSS; double dataSS; double bkgSS;
  double efakeSS; double edataSS; double ebkgSS;
  double R; double eR;
  double DDfake; double eDDfake; 
  double SF; double eSF; 

  fake   = yield("fake", ch, lev, "0", 0);
  efake   = getStatError("fake", ch, lev, "0", 0);

	fakeSS = yield("fake", ch, lev, "0", 1);
	dataSS = yield("data", ch, lev, "0", 1);
	bkgSS  = yield("bkg",  ch, lev, "0", 1) - fakeSS;  // prompt MC in SS
  
	efakeSS = getStatError("fake", ch, lev, "0", 1);
	edataSS = getStatError("data", ch, lev, "0", 1);
	ebkgSS  = getStatError("bkg",  ch, lev, "0", 1) - efakeSS;
  
	R = fake/fakeSS; 
  eR = R*(efake/fake + efakeSS/fakeSS);

  DDfake = R*(dataSS - bkgSS);
  eDDfake = DDfake*(eR/R  + (edataSS/dataSS - ebkgSS/bkgSS));

  SF = DDfake/fake;
  eSF = SF*(eDDfake/DDfake + efake/fake);

	if(docout){
    TResultsTable a(12, 3, true);
    a.SetFormatNum("%1.1f");
    a.SetRowTitleHeader("Source");
    a.AddVSeparation(4); a.AddVSeparation(6); a.AddVSeparation(7); a.AddVSeparation(10);

    a.SetColumnTitle(0, "$e^{+}e^{-}$");
    a.SetColumnTitle(1, "$\\mu^{+}\\mu^{-}$");
    a.SetColumnTitle(2, "$e^{\\pm}\\mu^{\\mp}$");

    a.SetRowTitle(0, "\\ttbar dilepton (SS)");
    a.SetRowTitle(1, "Drell-Yan (SS)");
    a.SetRowTitle(2, "Single top quark (SS)");
    a.SetRowTitle(3, "Dibosons (SS)");
    a.SetRowTitle(4, "ttV (SS)");
    a.SetRowTitle(5, "Total background (SS)");
    a.SetRowTitle(6, "Data (SS)");
    a.SetRowTitle(7, "SS data - bkg");
    a.SetRowTitle(8, "Non W/Z lep (SS)");
    a.SetRowTitle(9, "Non W/Z lep (OS)");
    a.SetRowTitle(10, "R (OS/SS)");
    a.SetRowTitle(11, "Non W/Z estimation");

    TString channels[3] = {"Elec", "Muon", "ElMu"}; 
    for(Int_t i = 0; i < 3; i++){
      ch = channels[i];

      fake   = yield("fake", ch, lev, "0", 0);
      efake   = getStatError("fake", ch, lev, "0", 0);

      fakeSS = yield("fake", ch, lev, "0", 1);
      dataSS = yield("data", ch, lev, "0", 1);
      bkgSS  = yield("bkg",  ch, lev, "0", 1) - fakeSS;  // prompt MC in SS

      efakeSS = getStatError("fake", ch, lev, "0", 1);
      edataSS = getStatError("data", ch, lev, "0", 1);
      ebkgSS  = getStatError("bkg",  ch, lev, "0", 1) - efakeSS;

      R = fake/fakeSS; 
      eR = R*(efake/fake + efakeSS/fakeSS);

      DDfake = R*(dataSS - bkgSS);
      eDDfake = DDfake*(eR/R  + (edataSS/dataSS - ebkgSS/bkgSS));

      SF = DDfake/fake;
      eSF = SF*(eDDfake/DDfake + efake/fake);

      a[0][i].SetContent(yield("ttbar", ch, lev, "0", 1)); 
      a[1][i].SetContent(yield("DY", ch, lev, "0", 1)); 
      a[2][i].SetContent(yield("tW", ch, lev, "0", 1)); 
      a[3][i].SetContent(yield("VV", ch, lev, "0", 1)); 
      a[4][i].SetContent(yield("ttV", ch, lev, "0", 1)); 
      a[5][i].SetContent(bkgSS);  a[5][i].SetError(ebkgSS);
      a[6][i].SetContent(dataSS); a[6][i].SetError(edataSS);
      a[7][i].SetContent(dataSS - bkgSS);
      a[8][i].SetContent(fakeSS);   a[8][i].SetError(efakeSS);
      a[9][i].SetContent(fake);   a[9][i].SetError(efake);
      a[10][i].SetContent(R);  a[10][i].SetError(eR);
      a[11][i].SetContent(DDfake);  a[11][i].SetError(eDDfake);

      a.SetDrawHLines(true); a.SetDrawVLines(true); a.Print();
      a.SaveAs(outputFolder + "/NonWDD_" + lev + ".tex");
      a.SaveAs(outputFolder + "/NonWDD_" + lev + ".html");
      a.SaveAs(outputFolder + "/NonWDD_" + lev + ".txt");
    }
	}

	if(IsErr) return eDDfake;
	else      return DDfake;
}

void GetDataDriven(TString theChan = ""){
  if(theChan == "") theChan = Chan;
  cout << " ------ Drell-Yan DD (R out/in) ------" << endl;
	cout << endl;
	GetDYDD(theChan, "dilepton", 0, 1);
	cout << endl;
	GetDYDD(theChan, "MET", 0, 1);
	cout << endl;
	GetDYDD(theChan, "2jets", 0, 1);
	cout << endl;
	GetDYDD(theChan, "1btag", 0, 1);
	cout << endl;
	cout << endl;
  cout << " ------ NonW leptons estimate ------" << endl;
	cout << endl;
  GetNonWDD(theChan, "dilepton", 0, 1);
	cout << endl;
  GetNonWDD(theChan, "2jets", 0, 1);
	cout << endl;
  GetNonWDD(theChan, "1btag", 0, 1);
	cout << endl;
}




//======================================================================================================================================================
//======================================================================================================================================================
//======================================================================================================================================================

double getYield(TString sample, TString chan, TString level, TString syst, bool isSS){
  if(chan == "All")   return getYield(sample, "ElMu", level, syst) + getYield(sample, "Muon", level, syst) + getYield(sample, "Elec", level, syst);
  if(chan == "sameF") return getYield(sample, "Muon", level, syst) + getYield(sample, "Elec", level, syst);

  int bin = 0;
  if(level == "dilepton") bin = 1;
  if(level == "ZVeto")    bin = 2;
  if(level == "MET")      bin = 3;
  if(level == "2jets")    bin = 4;
  if(level == "1btag")    bin = 5;
  if(level == "DYVeto")    bin = 6;
  TH1F* h;
  TFile* inputfile;
  inputfile = TFile::Open(path +    "/Tree_" + sample + ".root");
  TString prefix = "H_Yields_"; if(isSS) prefix = "H_SSYields_";
  if(syst == "0")  inputfile->GetObject(prefix + chan, h);
  else             inputfile->GetObject(prefix + chan + "_" + syst, h);
  double y = h->GetBinContent(bin);
  if(!sample.Contains("Double") && !sample.Contains("Single") && !sample.Contains("MuonEG")) return y*Lumi;
  else return y;
}

double yield(TString process, TString chan, TString level, TString syst, bool isSS){
  double y = 0;
  if       (process == "ttbar"  ) y = getYield("TTbar_Powheg", chan, level, syst, isSS);
  else if  (process == "tW"     ) y = getYield("TbarW", chan, level, syst, isSS) + getYield("TW", chan, level, syst, isSS);
  //else if  (process == "DY"     ) y = getYield("DYJetsToLL_M5to50_MLM", chan, level, syst, isSS) + getYield("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS);
  else if  (process == "DY"     ) y = getYield("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS);
  else if  (process == "VV"     ) y = getYield("WW", chan, level, syst, isSS) + getYield("WZ", chan, level, syst, isSS) + getYield("ZZ", chan, level, syst, isSS);
  else if  (process == "fake"||process == "NonW/Z" ||process=="NonW")  y = getYield("WJetsToLNu_MLM", chan, level, syst, isSS) + getYield("TTbar_Powheg_Semilep", chan, level, syst, isSS);
  else if  (process == "ttV"     ) y = getYield("TTWToLNu", chan, level, syst, isSS) + getYield("TTZToQQ", chan, level, syst, isSS) + getYield("TTZToLLNuNu", chan, level, syst, isSS) + getYield("TTWToQQ", chan, level, syst, isSS);
  else if  (process == "bkg"    ) {y = yield("ttbar", chan, level, syst, isSS) + yield("tW", chan, level, syst, isSS) + yield("DY", chan, level, syst, isSS) + yield("VV", chan, level, syst, isSS) + yield("fake", chan, level, syst, isSS) + yield("ttV", chan, level, syst, isSS); return y;}

  else if  (process == "data" || process == "Data"   ){
    if     (chan == "Elec") y = getYield("SingleElec", chan, level, "0", isSS) + getYield("DoubleEG", chan, level, "0", isSS);
    else if(chan == "ElMu") y = getYield("SingleElec", chan, level, "0", isSS) + getYield("SingleMuon", chan, level, "0", isSS) + getYield("MuonEG", chan, level, "0", isSS);
    else if(chan == "Muon") y = getYield("SingleMuon", chan, level, "0", isSS) + getYield("DoubleMuon", chan, level, "0", isSS);
  }
  else y = getYield(process, chan, level, syst, isSS);
  return y;
}

double getStatError(TString sample, TString chan, TString level, TString syst, bool isSS){
  if(chan == "All")   return getStatError(sample, "ElMu", level, syst, isSS) + getStatError(sample, "Muon", level, syst, isSS) + getStatError(sample, "Elec", level, syst, isSS);
  if(chan == "sameF") return getStatError(sample, "Muon", level, syst, isSS) + getStatError(sample, "Elec", level, syst, isSS);

  if       (sample == "ttbar"  ) return getStatError("TTbar_Powheg", chan, level, syst, isSS);
  else if  (sample == "tW"     ) return getStatError("TbarW", chan, level, syst, isSS) + getStatError("TW", chan, level, syst, isSS);
 // else if  (sample == "DY"   ) return getStatError("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS) + getStatError("DYJetsToLL_M5to50_MLM", chan, level, syst, isSS);
  else if  (sample == "DY"   ) return getStatError("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS);
  else if  (sample == "VV"   ) return getStatError("WW", chan, level, syst, isSS) + getStatError("WZ", chan, level, syst, isSS) + getStatError("ZZ", chan, level, syst);
  else if  (sample == "ttV"  ) return getStatError("TTWToLNu", chan, level, syst, isSS) + getStatError("TTZToQQ", chan, level, syst, isSS) + getStatError("TTZToLLNuNu", chan, level, syst, isSS) + getStatError("TTWToQQ", chan, level, syst, isSS);
  else if  (sample == "fake"||sample == "NonW/Z" ||sample =="NonW")  return getStatError("WJetsToLNu_MLM", chan, level, syst, isSS) + getStatError("TTbar_Powheg_Semilep", chan, level, syst, isSS);
  else if  (sample == "bkg"  ) return  getStatError("tW", chan, level, syst, isSS) + getStatError("DY", chan, level, syst, isSS) + getStatError("VV", chan, level, syst, isSS) + getStatError("fake", chan, level, syst, isSS) + getStatError("ttV", chan, level, syst, isSS);
  else if  (sample == "data" || sample == "Data"   ){
    if     (chan == "Elec") return getStatError("SingleElec", chan, level, "0", isSS) + getStatError("DoubleEG", chan, level, "0", isSS);
    else if(chan == "Muon") return getStatError("SingleElec", chan, level, "0", isSS) + getStatError("SingleMuon", chan, level, "0", isSS) + getStatError("MuonEG", chan, level, "0", isSS);
    else if(chan == "ElMu") return getStatError("SingleMuon", chan, level, "0", isSS) + getStatError("DoubleMuon", chan, level, "0", isSS);
  }

  int bin = 0; if(level == "dilepton") bin = 1; if(level == "ZVeto") bin = 2;  if(level == "MET") bin = 3; if(level == "2jets") bin = 4; if(level == "1btag") bin = 5; if(level == "DYVeto") bin = 6;
  TH1F* h; TFile* inputfile;
  inputfile = TFile::Open(path + "/Tree_" + sample + ".root");
  TString prefix = "H_Yields_"; if(isSS) prefix = "H_SSYields_";
  if(syst == "0")  inputfile->GetObject(prefix + chan, h);
  else             inputfile->GetObject(prefix + chan + "_" + syst, h);
  double err = h->GetBinError(bin);
  delete inputfile;
  if(!sample.Contains("Double") && !sample.Contains("Single") && !sample.Contains("MuonEG")) return err*Lumi;
  else return err;
}

