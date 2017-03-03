R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Plot.h"


TString path = "/nfs/fanae/user/dani/CERN/AnalyserPAF/WW_temp/"; 

//selección de eventos ne las mismas funciones que plotean
//=======================================================================================

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag );

void MakePlots_WW(){
  //Draw("TMET", "TNJets == 0 && TNBtags == 0 && TMET > 20", "ElMu", 40, 0, 400, "MET [GeV]", "0");
  //Draw("TNJets", "TNJets == 0 && TNBtags == 0 && TMET > 20", "ElMu", 40, 0, 400, "NJets", "0");
  Draw("TMll", "TNJets == 0 && TNBtags == 0 && TMET > 20", "ElMu", 40, 0, 400, "InvMass", "0");
}



void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag){
	
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);

	p->SetPath(path);
	
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed, 0.50);
	p->AddSample("ZZ", "VV", itBkg, kGreen, 0.50);
	p->AddSample("WZ", "VV", itBkg);


	p->AddSample("TW", "TW", itBkg, kGreen+3, 0.50);
	p->AddSample("TbarW", "TW", itBkg);
	//p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kYellow, 0.50);
	//p->AddSample("DYJetsToLL_M5to50_MLM", "DY", itBkg);
	p->AddSample("WJetsToLNu_MLM", "Wjets", itBkg, kMagenta, 0.50);
	p->AddSample("TTWToLNu", "TTV", itBkg, kOrange-3, 0.5);
	p->AddSample("TTZToQQ", "TTV", itBkg);
	p->AddSample("TTGJets", "TTV", itBkg);
	p->AddSample("TTWToQQ", "TTV", itBkg);
	//p->AddSample("MuonEG", "Data", itData);
	//p->AddSample("SingleMuon", "Data", itData);
	//p->AddSample("SingleElec", "Data", itData);
	//p->AddSample("DoubleEG", "Data", itData);
	//p->AddSample("DoubleMuon", "Data", itData);
	
	p->AddSample("WW", "WW", itBkg, kBlue+1, 0.50);
	p->AddSample("WWTo2L2Nu", "WW", itBkg);
	p->AddSample("GGWWTo2L2Nu", "WW", itBkg);

	p->doSetLogy = false;
	p->DrawStack("nombre que quiero", 1);
	p->doSetLogy = true;
	p->DrawStack(tag + "_log", 1);
	delete p;
};
