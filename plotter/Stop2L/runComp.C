R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);
//TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/StopTrees/Baseline/";
TString pathToTree = "/nfs/fanae/user/juanr/AnalysisPAF/Stop_temp/";
TString NameOfTree = "tree";

void runComp(){
  DrawPlot("TMET", "TMET > 0 && TNJets > 1 && TNBtags > 0", "ElMu", 30, 0, 300, "MET [GeV]");
}

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle){
  Plot* p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetPathSignal(pathToTree + "T2tt/");
  p->verbose = true;

/*	p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, 1, 0.05);
	p->AddSample("TTbar_PowhegLHE", "ttbar_JESUp", itSignal, kRed-2, 0.05, "JESUp");
	p->AddSample("TTbar_PowhegLHE", "ttbar_JESDown", itSignal, kRed+1, 0.05, "JESDown");
	p->AddSample("TTbar_PowhegLHE", "ttbar_BtagUp", itSignal, kBlue, 0.05, "BtagUp");
	p->AddSample("TTbar_PowhegLHE", "ttbar_BtagDown", itSignal, kBlue+3, 0.05, "BtagDown");
	p->AddSample("TTbar_PowhegLHE", "ttbar_MisTagUp", itSignal, kGreen, 0.05, "MisTagUp");
	p->AddSample("TTbar_PowhegLHE", "ttbar_MisTagDown", itSignal, kGreen-3, 0.05, "MisTagDown");
*/

	p->AddSample("TTbar_PowhegLHE", "ttbar", itSignal, 1, 0.05 );
	p->AddSample("TTbar_PowhegLHE", "ttbar_LepEffUp", itSignal, kPink, 0.05, "LepEffUp");
	p->AddSample("TTbar_PowhegLHE", "ttbar_LepEffDown", itSignal, kRed+1, 0.05, "LepEffDown");
	p->AddSample("TTbar_PowhegLHE", "ttbar_PUUp", itSignal, kCyan, 0.05, "PUUp");
	p->AddSample("TTbar_PowhegLHE", "ttbar_PUDown", itSignal, kBlue, 0.05, "PUDown");
	//p->AddSample("WW", "VV", itSignal, 1, 0.05 );
	//p->AddSample("WW", "VV_LepEffUp", itSignal, kRed, 0.05, "LepEffUp");
	//p->AddSample("WW", "VV_LepEffDown", itSignal, kBlue, 0.05, "LepEffDown");

  //p->PrintYields();
  p->DrawComp("0", 1);
  p->PrintSystYields();
  delete p;

}

