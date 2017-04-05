#include "Plot.C"

const TString pathToMiniTrees = "";

const Int_t nChannels = 3;
TString channels[nChannels] = {"ElMu", "Muon", "Elec"};


Plot* pDY[3];
Plot* pData[3];

GetDYSF(Plot* p){

  pDY[] = new Plot("TMll", cut, chan, 400, 0, 400, "Title", "Invariant Mass");
}

TH1F* loadDYHisto(TString lab, TString ch, TString lev){
	TFile* f1; TFile* f2;

	TH1F*  h1; TH1F*  h2;
	if(lab == "MC" || lab == "DY"){
		f1 = TFile::Open(path + "Tree_DYJetsToLL_M50_aMCatNLO.root");
		f2 = TFile::Open(path + "Tree_DYJetsToLL_M10to50_aMCatNLO.root"); 
	}
	else{
		f1 = TFile::Open(path + "Tree_Data_SingleElec.root");
		f2 = TFile::Open(path + "Tree_Data_SingleMu.root"); 
	}
	f1->GetObject("H_DY_InvMass_" + ch + "_" + lev, h1);
	f2->GetObject("H_DY_InvMass_" + ch + "_" + lev, h2);
	h1->Add(h2);
  if(lab == "MC" || lab == "DY") h1->Scale(Lumi);
  h1->SetDirectory(0);
  f1->Close(); f2->Close(); delete f1; delete f2;
  return h1;
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
  
  TH1F* DYmm   = loadDYHisto("DY","Muon", lev);
  TH1F* DYee   = loadDYHisto("DY","Elec", lev);
  TH1F* Datamm = loadDYHisto("Da","Muon", lev);
  TH1F* Dataee = loadDYHisto("Da","Elec", lev);
  TH1F* Dataem = loadDYHisto("Da","ElMu", lev);

  Int_t low_in = DYmm->FindBin(76. );
  Int_t  up_in = DYmm->FindBin(106.);

  Double_t t1(0.),t2(0.);

  nin_mm = DYmm->IntegralAndError(low_in, up_in, nin_err_mm); 
  nin_ee = DYee->IntegralAndError(low_in, up_in, nin_err_ee); 
  nout_mm = DYmm->IntegralAndError(0,low_in-1,nout_err_mm) + DYmm->IntegralAndError(up_in+1,201,t1);
  nout_ee = DYee->IntegralAndError(0,low_in-1,nout_err_ee) + DYee->IntegralAndError(up_in+1,201,t2);
  
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

/*
  cout << " ------ Drell-Yan DD (R out/in) ------" << endl;
	cout << endl;
	GetDYDD(Chan, "dilepton", 0, 1);
	cout << endl;
	GetDYDD(Chan, "MET", 0, 1);
	cout << endl;
	GetDYDD(Chan, "2jets", 0, 1);
	cout << endl;
	cout << endl;
*/


