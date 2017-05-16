//#include "DrawPlots.C"

TString path = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/may10/";
double getYield(TString sample, TString chan = "ElMu", TString level = "2jets", TString syst = "0", bool isSS = false);
double yield(TString process, TString chan = "ElMu", TString level = "2jets", TString syst = "0", bool isSS = false);
double getStatError(TString sample, TString chan, TString level, TString syst = "0", bool isSS = false);


TH1F* loadDYHisto(TString lab = "MC", TString ch = "ElMu", TString lev = "2jets");

double GetDYDD(  TString ch = "ElMu", TString lev = "2jets", bool IsErr = false, bool docout = false);
double GetNonWDD(TString ch = "ElMu", TString lev = "2jets", bool IsErr = false, bool docout = false);

double Lumi = 35851.45088;
TString Chan = "ElMu";
TString Level = "1btag";

TH1F* loadDYHisto(TString lab, TString ch, TString lev){
	TFile* f1; TH1F* h; TH1F* h1; TH1F* h2;
  if(lab != "MC" && lab != "DY" && lab != "data" && lab != "Data"){
    f1 = TFile::Open(path + "Tree_" + lab + ".root");
    f1->GetObject("H_DY_InvMass_" + ch + "_" + lev, h);
    if(!lab.Contains("Single") && !lab.Contains("Double") && !lab.Contains("MuonEG")) h->Scale(Lumi);
  }
  else if(lab == "MC" || lab == "DY"){
    h1 = loadDYHisto("DYJetsToLL_M10to50_aMCatNLO", ch, lev);
    h  = loadDYHisto("DYJetsToLL_M50_aMCatNLO", ch, lev);
    h->Add(h1);
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

double GetDYDD(TString ch, TString lev, bool IsErr, bool docout){
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
   cout << " DY DD estimation for " << lev << endl;
   cout << "=============================================================================" << endl;
   cout << "                  |       El/El      |       Mu/Mu      |       El/Mu      ||" << endl;
	 cout << "-----------------------------------------------------------------------------" << endl;
	 cout<< Form(" n_in (MC)        | %7.1f +/- %4.1f | %7.1f +/- %4.1f |                  ||", nin_ee, nin_err_ee, nin_mm, nin_err_mm) << endl;
	 cout<< Form(" n_out (MC)       | %7.1f +/- %4.1f | %7.1f +/- %4.1f |                  ||", nout_ee, nout_err_ee, nout_mm, nout_err_mm) << endl;
	 cout<< Form(" R(Nout/Nin)(MC)  | %6.3f +/- %4.3f | %6.3f +/- %4.3f |                  ||", Re, R_erre, Rm, R_errm) << endl;
	 cout<< Form(" k_ll             | %6.3f +/- %4.3f | %6.3f +/- %4.3f |                  ||", k_lle, k_ll_erre, k_llm, k_ll_errm) << endl;
	 cout<< Form(" N_in (D)         | %7.1f +/- %4.1f | %7.1f +/- %4.1f | %5.1f +/- %4.1f   ||",	N_ine, N_in_erre, N_inm, N_in_errm, N_inem, N_in_errem) << endl;
	 cout<< "-----------------------------------------------------------------------------" <<  endl;
	 cout<< Form(" N_out            | %7.1f +/- %4.1f | %7.1f +/- %4.1f |                  ||",	 N_oute, N_out_erre, N_outm, N_out_errm) << endl;
	 cout<< "-----------------------------------------------------------------------------" <<  endl;
	 cout<< Form(" SF (D/MC)        | %6.3f +/- %4.3f | %6.3f +/- %4.3f | %6.3f +/- %4.3f ||", SFel, SFel_err, SFmu, SFmu_err, SFem, SFem_err) << endl;
	 cout<< "-----------------------------------------------------------------------------" <<  endl;
	 cout<< Form(" Drell-Yan (MC)   | %7.2f +/- %4.2f | %7.2f +/- %4.2f | %7.2f +/- %4.2f ||", yield("DY", "Elec", lev), getStatError("DY", "Elec", lev), yield("DY", "Muon", lev), getStatError("DY", "Muon", lev), yield("DY", "ElMu", lev), getStatError("DY", "ElMu", lev)) << endl;
	 cout<< Form(" Drell-Yan (D)    | %7.2f +/- %4.2f | %7.2f +/- %4.2f | %7.2f +/- %4.2f ||", yield("DY", "Elec", lev)*SFel, getStatError("DY", "Elec", lev)*SFel, yield("DY", "Muon", lev)*SFmu, getStatError("DY", "Muon", lev)*SFmu, yield("DY", "ElMu", lev)*SFem, getStatError("DY", "ElMu", lev)*SFem) << endl;
   cout << "=============================================================================" << endl;
	 cout<< endl;
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
	double fake   = yield("fake", ch, lev, "0", 0);
	double efake   = getStatError("fake", ch, lev, "0", 0);

/*	if(ch == "Muon"){
		if(IsErr) return efake;
		else      return  fake;
  }*/

	double fakeSS = yield("fake", ch, lev, "0", 1);
	double dataSS = yield("data", ch, lev, "0", 1);
	double bkgSS  = yield("bkg",  ch, lev, "0", 1) - fakeSS;  // prompt MC in SS
  
	double efakeSS = getStatError("fake", ch, lev, "0", 1);
	double edataSS = getStatError("data", ch, lev, "0", 1);
	double ebkgSS  = getStatError("bkg",  ch, lev, "0", 1) - efakeSS;
  
	double  R = fake/fakeSS; 
  double eR = R*(efake/fake + efakeSS/fakeSS);

  double  DDfake = R*(dataSS - bkgSS);
  double eDDfake = DDfake*(eR/R  + (edataSS/dataSS - ebkgSS/bkgSS));

  double  SF = DDfake/fake;
  double eSF = SF*(eDDfake/DDfake + efake/fake);

	if(docout){
   cout << "Yields for same-sign events after " << lev << " level of selection in " << ch << " channel:\n";
   cout << "===========================" << endl;
   cout << " tW           : " << yield("tW", ch, lev, "0", 1) << endl;
   cout << " Drell-Yan    : " << yield("DY", ch, lev, "0", 1) << endl;
   cout << " Dibosons     : " << yield("VV", ch, lev, "0", 1) << endl;
   cout << " ttV          : " << yield("ttV", ch, lev, "0", 1) << endl;
   cout << " ttbar dilep  : " << yield("ttbar", ch, lev, "0", 1) << endl;
   cout << "---------------------------" << endl;
   cout << " ttbar semilep: " << yield("TTbar_Powheg_Semilep", ch, lev, "0", 1) << endl;
   cout << " WJets        : " << yield("WJetsToLNu_MLM", ch, lev, "0", 1) << endl;
   cout << "===========================" << endl;
   cout << endl;
   cout <<      " NonW leptons DD estimate for " << lev << endl;
   cout <<      "==================================================================" << endl;
	 cout << Form(" MC fake estimate    (OS WJets + ttbar semilep)   = %2.2f ± %0.2f", fake, efake) << endl;
	 cout <<      "------------------------------------------------------------------" << endl;
	 cout << Form(" MC fake SS (WJets + ttbar semilep)   = %2.2f ± %0.2f", fakeSS, efakeSS)  << endl;
   cout << Form(" R = fakeOS/fakeSS                    = %2.2f ± %0.2f", R, eR)            << endl;
	 cout << Form(" MC prompt SS (other sources)         = %2.2f ± %0.2f", bkgSS , ebkgSS )  << endl;
	 cout << Form(" Data SS events                       = %2.2f ± %0.2f", dataSS , edataSS )  << endl;
	 cout <<      "------------------------------------------------------------------" << endl;
   cout << Form(" DD fake estimate = R(DataSS-bkgSS)   = %2.2f ± %0.2f", DDfake , eDDfake )  << endl;
 //  cout << Form(" Scake Factor (DD/MC)                 = %2.2f ± %0.2f", SF , eSF )  << endl;
   cout <<      "==================================================================" << endl;
   cout << endl;
	}

	if(IsErr) return eDDfake;
	else      return DDfake;
}

void GetDataDriven(){
  cout << " ------ Drell-Yan DD (R out/in) ------" << endl;
	cout << endl;
	GetDYDD(Chan, "dilepton", 0, 1);
	cout << endl;
	GetDYDD(Chan, "MET", 0, 1);
	cout << endl;
	GetDYDD(Chan, "2jets", 0, 1);
	cout << endl;
	GetDYDD(Chan, "1btag", 0, 1);
	cout << endl;
	cout << endl;
  cout << " ------ NonW leptons estimate ------" << endl;
	cout << endl;
  GetNonWDD(Chan, "dilepton", 0, 1);
	cout << endl;
  GetNonWDD(Chan, "2jets", 0, 1);
	cout << endl;
  GetNonWDD(Chan, "1btag", 0, 1);
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
  else if  (process == "DY"     ) y = getYield("DYJetsToLL_M10to50_aMCatNLO", chan, level, syst, isSS) + getYield("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS);
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
  else if  (sample == "DY"   ) return getStatError("DYJetsToLL_M50_aMCatNLO", chan, level, syst, isSS) + getStatError("DYJetsToLL_M10to50_aMCatNLO", chan, level, syst, isSS);
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

