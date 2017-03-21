R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Looper.h"
#include "Plot.h"


TString path = "/nfs/fanae/user/dani/CERN/AnalysisPAF/WW_temp/"; 

//selección de eventos ne las mismas funciones que plotean
//=======================================================================================

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag );

void MakePlots_WW(){
  
   Draw("TMT", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 && !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "M_T [GeV]", "0jets");
   Draw("TMll", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 && !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "InvMass [GeV]", "0jets");
   Draw("TMET", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "MET [GeV]", "0jets");
   Draw("TPtdil", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "Ptll [GeV]", "0jets");
   Draw("TJet_Eta[0]", "TMll > 20 && TNJets == 0  && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 25, -5, 5, "Jet_Eta", "0jets");
    
   //Draw("TMT", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "M_T [GeV]", "0jets");
   //Draw("TMT", "TMll > 20 && TNJets == 1 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "M_T [GeV]", "1jet");
   //Draw("TMT", "TMll > 20 && TNJets >= 2 && TNBtags >= 1 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "M_T [GeV]", ">2jets");
   
   //Draw("TLep_Pt[0]", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "Lep_Pt [GeV]", "0jets");
   //Draw("TLep_Pt[0]", "TMll > 20 && TNJets >= 2 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "Lep_Pt [GeV]", "1jets");
   //Draw("TLep_Pt[0]", "TMll > 20 && TNJets >= 2 && TNBtags >= 1 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "Lep_Pt [GeV]", ">2jets");
  
   //Draw("TLep_Eta[0]", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 25, -3, 3, "Lep_Eta", "0jets");
   //Draw("TJet_Phi[0]", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 20, -4, 4, "Jet_Phi", "0jets");
   //Draw("TJet_Phi[0]", "TMll > 20 && TNJets == 1 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 20, -4, 4, "Jet_Phi", "1jets");
	
   //Draw("TNJets", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 5, 0, 5, "MET [GeV]", "0jets");
   //Draw("TNJets", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 30", "ElMu", 40, 0, 400, "NJets", "0");
   //Draw("TNBtags", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 30", "ElMu", 40, 0, 400, "NbJets", "0");
   //Draw("TLep_Pt[0]", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 50", "ElMu", 40, 0, 400, "Ptll [GeV]", "0");
   
  
   //Draw("TMT2", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 30 && TMT2 > 10", "ElMu", 40, 0, 400, "MT2 [GeV]", "0");
   //Draw("TMT", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 30", "ElMu", 40, 0, 400, "MT [GeV]", "0");
   //Draw("TMTprime", "TNJets == 0 && TNBtags == 0 && TMET > 20 && TPtdil > 30", "ElMu", 40, 0, 400, "MT [GeV]", "0");
}


void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag){
	
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);

	p->SetPath(path);
	p->verbose = true;
	p->SetTreeName("tree");
	
	p->AddSample("ZZ", "VV + TTV", itBkg, kGreen, 0.50);
	p->AddSample("WZ", "VV + TTV", itBkg);


	p->AddSample("TW", "ttbar + TW", itBkg, kRed, 0.50);
	p->AddSample("TbarW", "ttbar + TW", itBkg);
	p->AddSample("Tbar_Powheg", "ttbar + TW", itBkg);
	//p->AddSample("TTbar_Powheg", "ttbar + TW", itBkg, kRed, 0.50); esta es la vieja (no incluye jets < 15 )
	
	p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kYellow, 0.50);
	p->AddSample("DYJetsToLL_M5to50_MLM", "DY", itBkg);
	p->AddSample("WJetsToLNu_MLM", "Wjets", itBkg, kMagenta, 0.50);
	p->AddSample("TTWToLNu", "VV + TTV", itBkg, kGreen, 0.5);
	p->AddSample("TTZToQQ", "VV + TTV", itBkg);
	p->AddSample("TTGJets", "VV + TTV", itBkg);
	p->AddSample("TTWToQQ", "VV + TTV", itBkg);
	
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);
	
	p->AddSample("WWTo2L2Nu", "WW", itBkg, kBlue+1, 0.50);
	p->AddSample("GGWWTo2L2Nu", "WW", itBkg);
	
	//p->AddSample("WW", "WW", itBkg, kBlue+1, 0.50);
	
	//p->AddSystematicLabel("ue");
	//p->AddSystematicLabel("fsr");
	//p->AddSystematicLabel("isr");
	//p->AddSystematicLabel("nlo");
	//p->AddSystematic("JES");
	//p->AddSystematic("LepEff");
	//p->AddSystematic("Btag");
	//p->AddSystematic("MisTag");
  
	//p->PrintYields();
	p->doSetLogy = false;
	p->DrawStack(tag, 1);
	//p->doSetLogy = true;
	//p->DrawStack(tag + "_log", 1);
	delete p;
};
