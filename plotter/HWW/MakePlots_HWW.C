R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(CrossSection.C+)

#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "TResultsTable.h"
#include "CrossSection.h"

const Int_t nVar = 11;
const Int_t nChan = 1;
Int_t NBins;
Int_t X0;
Int_t XN;


TString Path = "/nfs/fanae/user/dani/CERN/AnalysisPAF/HWW_temp/";
TString Var[nVar] = {"TMET", "TMll", "TMT", "TPtdil", "TLep0_Pt", "TJet_Pt[0]", "TLep1_Pt", "TJet_Pt[1]", "TNJets", "TNVetoJets", "TDeltaPhi"};
TString Chan[nChan] = {"ElMu"};
TString CR;
TString Cuts;
TString XLegend;

TString Nobtag0jets    = "TMT > 70 && TMT < 150 && TMath::Abs(TDeltaPhi) < 1.8 && TMll > 12 && TMET > 20 && TPtdil > 30 && !TIsSS && TNVetoJets == 0 && TNJets == 0";
TString Nobtag1jets    = "TMT > 70 && TMT < 150 && TMath::Abs(TDeltaPhi) < 1.8 && TMll > 12 && TMET > 20 && TPtdil > 30 && !TIsSS && TNVetoJets == 0 && TNJets == 1";

//HWW signal region
TString HWWCut = "TMT > 70 && TMT < 150 && TMath::Abs(TDeltaPhi) < 1.8 && TMll > 12 && TMET > 20 && TPtdil > 30 && !TIsSS && TNVetoJets == 0"; //se dividirá en 0 y 1 jet
void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder = "");
void LoopDrawCR(TString CR);

//selección en CR de control región en la función LoopDrawCR( ) que plotea regiones de control
//=======================================================================================================================


void LoopDrawCR(TString CR){

	if(CR == "CRHWW") Cuts = HWWCut;
				
		for(Int_t i=0 ; i < nVar ; i++){

			if     (Var[i] == "TMET")        NBins = 30 , X0 =  0, XN = 150 , XLegend = "MET [GeV]";  //NBins = 20 con un jet
			else if(Var[i] == "TMll")        NBins = 22 , X0 = 0 , XN = 220 , XLegend = "M_{ll} [GeV]";
			else if(Var[i] == "TMT")         NBins = 19 , X0 = 69 , XN = 151 , XLegend = "M_{T} [GeV]";
			else if(Var[i] == "TPtdil")      NBins = 18 , X0 = 31 , XN = 120 , XLegend = "P^{ll}_{T} [GeV]";
			else if(Var[i] == "TLep0_Pt")  NBins = 20 , X0 =  0, XN = 140 , XLegend = "Pt^{l_{0}} [GeV]"; //XN =140 con un jet
			else if(Var[i] == "TLep1_Pt")  NBins =  20, X0 = 0 , XN = 140 , XLegend = "Pt^{l_{1}} [GeV]";
			else if(Var[i] == "TJet_Pt[0]")  NBins = 25 , X0 = 0 , XN = 200 , XLegend = "Jet_Pt_0 [GeV]";
			else if(Var[i] == "TJet_Pt[1]")  NBins = 20 , X0 = 0 , XN = 150 , XLegend = "Jet_Pt_1 [GeV]";
			else if(Var[i] == "TNJets")      NBins = 5  , X0 = 0 , XN = 5   , XLegend = "Number Jets";
			else if(Var[i] == "TNVetoJets")     NBins = 5  , X0 = 0 , XN = 5   , XLegend = "Number b-Jets";
			else if(Var[i] == "DeltaPhi")    NBins = 16 , X0 = -4, XN = 4   , XLegend = "#Delta#phi";
			

			for(Int_t j=0; j < nChan ; j++){
				Draw(Var[i], Cuts + "&& TNJets == 0 && TNVetoJets == 0", Chan[j], NBins, X0, XN, XLegend, "0jets", CR+"/"+Chan[j]);
				Draw(Var[i], Cuts + "&& TNJets == 1", Chan[j], NBins, X0, XN, XLegend, "1jets", CR+"/"+Chan[j]);
			}		 
		}		
};	

void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder){
	
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
    	p->SetPlotFolder(plotFolder);	
	p->SetPath(Path);
	p->verbose = true;
	p->SetLumi(35.85145088);
	
	p->SetTreeName("tree");
	//===============================================================

	p->AddSample("WJetsToLNu_MLM", "Wjets", itBkg, kMagenta);
	
	p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kYellow);
	p->AddSample("DYJetsToLL_M5to50_MLM", "DY", itBkg);
	
	p->AddSample("ZZ", "VV + TTV", itBkg, kCyan);
	p->AddSample("WZ", "VV + TTV", itBkg);
	p->AddSample("TTWToLNu", "VV + TTV", itBkg);
	p->AddSample("TTZToQQ", "VV + TTV", itBkg);
	p->AddSample("TTWToQQ", "VV + TTV", itBkg);

	p->AddSample("TW", "ttbar + TW", itBkg, kRed);
	p->AddSample("TbarW", "ttbar + TW", itBkg);
	p->AddSample("TTbar_Powheg", "ttbar + TW", itBkg);
	
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);

	p->AddSample("WWTo2L2Nu", "WW", itBkg, kBlue+1);
	p->AddSample("GGWWTo2L2Nu", "WW", itBkg);
	
//======================================== para hacer plots logaritmicos invertir colocación ==============
/*	
	p->AddSample("TW", "ttbar + TW", itBkg, kRed);
	p->AddSample("TbarW", "ttbar + TW", itBkg);
	p->AddSample("TTbar_Powheg", "ttbar + TW", itBkg);

	p->AddSample("DYJetsToLL_M50_MLM", "DY", itBkg, kYellow);
	p->AddSample("DYJetsToLL_M5to50_MLM", "DY", itBkg);
	
	p->AddSample("MuonEG", "Data", itData);
	p->AddSample("SingleMuon", "Data", itData);
	p->AddSample("SingleElec", "Data", itData);
	p->AddSample("DoubleEG", "Data", itData);
	p->AddSample("DoubleMuon", "Data", itData);

	p->AddSample("WWTo2L2Nu", "WW", itBkg, kBlue+1);
	p->AddSample("GGWWTo2L2Nu", "WW", itBkg);

	p->AddSample("ZZ", "VV + TTV", itBkg, kCyan);
	p->AddSample("WZ", "VV + TTV", itBkg);
	p->AddSample("TTWToLNu", "VV + TTV", itBkg);
	p->AddSample("TTZToQQ", "VV + TTV", itBkg);
	p->AddSample("TTWToQQ", "VV + TTV", itBkg);

	p->AddSample("WJetsToLNu_MLM", "Wjets", itBkg, kMagenta);
*/
//=========================================================================
	//esta es la señal
	p->AddSample("ggHWWTo2L2Nu", "H-WW", itBkg, kGreen);

	//p->GetHisto("H-WW")->SetLineWidth(3);
	//p->ScaleSignal(50);


   	//=================================================================

	p->SetCMSlabel("CMS Academic");
	//p->AddSystematic("JES,Btag,MisTag,PU,stat");
  	p->AddSystematic("stat");

	p->PrintYields();
	//p->SetSignalStyle("BSM");
	//p->SetSignalDrawStyle("H");
	//p->SetSignalStyle("hist");
	p->doSetLogy = false;
	//if((var == "TMll") || (var == "TPtdil") || (var == "TMET") || (var == "TMT") ) p->SetPlotMaximum(1200);
	p->DrawStack(tag, 1);
	//p->doSetLogy = true;
	//p->DrawStack(tag + "_log", 1);
	delete p;
};

void xsec(){

  	Plot* p = new Plot("TMll", Nobtag1jets, "ElMu", 21, 11, 220, "Channel", "xsec");
	p->SetTreeName("tree");
	p->SetPath(Path);
	p->verbose = true;
	p->SetLumi(35.85145088);

        //====================================================================
	p->AddSample("WJetsToLNu_MLM", "Wjets", itBkg, kMagenta);
	p->AddSample("DYJetsToLL_M50_MLM, DYJetsToLL_M5to50_MLM", "DY", itBkg, kYellow);
	p->AddSample("ZZ, WZ", "VV", itBkg, kCyan);
	p->AddSample("TTWToLNu, TTZToQQ, TTWToQQ", "TTV", itBkg, kOrange);
	p->AddSample("TW, TbarW", "TW", itBkg, kRed-4);
	p->AddSample("TTbar_Powheg", "ttbar", itBkg, kRed);	
	p->AddSample("MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon", "Data", itData);
	p->AddSample("WWTo2L2Nu, GGWWTo2L2Nu", "WW", itBkg, kBlue+1);
	p->AddSample("ggHWWTo2L2Nu", "H-WW", itBkg, kGreen);
	//=======================================================================================


  	p->AddSample("ggHWWTo2L2Nu", "H-WW", itSys, 1, "ScaleUp"    );
  	p->AddSample("ggHWWTo2L2Nu", "H-WW", itSys, 1, "ScaleDown"  );
  	p->AddSample("ggHWWTo2L2Nu", "H-WW", itSys, 1, "pdfUp"      );
  	p->AddSample("ggHWWTo2L2Nu", "H-WW", itSys, 1, "pdfDown"    );

  	//=======================================================================================
 	
	p->AddSystematic("JES,Btag,MisTag,PU,stat,MuonEff,ElecEff");
 	p->SetSignalStyle("xsec");
  	p->SetTableFormats("%2.4f");
	p->PrintYields("", "", "", "txt, tex, html");
	
	p->SaveHistograms();
		
	CrossSection *x = new CrossSection(p, "H-WW");
  	
	x->SetTheoXsec(120.3);
  	x->SetChannelTag("ElMu");

  	x->SetEfficiencySyst("JES, Btag, MisTag, ElecEff, MuonEff, LepEff, Trig, PU, JER");
  	x->SetAcceptanceSyst("stat, ue, nlo, hdamp, scale, pdf, isr, fsr, q2, ME");

  	x->SwitchLabel("VV", "Dibosons");
	x->SwitchLabel("TTV", "ttV");
  	x->SwitchLabel("DY", "Drell-Yan");
	x->SwitchLabel("Wjets", "NonWZ");
	x->SwitchLabel("ttbar", "ttbar");
	x->SwitchLabel("TW", "tW");
	x->SwitchLabel("H-WW", "H-WW");	
	
  	x->SwitchLabel("JES", "Jet Energy Scale");
  	x->SwitchLabel("JER", "Jet Energy Resolution");
  	x->SwitchLabel("Btag", "b-tagging efficiency");
  	x->SwitchLabel("MisTag", "Mistagging efficiency");
  	x->SwitchLabel("ElecEff", "Electron efficiencies");
  	x->SwitchLabel("MuonEff", "Muon efficiencies");
  	x->SwitchLabel("Trig", "Trigger efficiencies");
  	x->SwitchLabel("PU", "Pile-Up");
  	x->SwitchLabel("Scale", "ME scales");
  	x->SwitchLabel("pdf", "PDF");
  	x->SwitchLabel("stat", "MC stat");

  	// Scale FSR unc by sqrt
  	x->SetUnc("Drell-Yan", 0.15);
  	x->SetUnc("NonWZ",     0.30);
  	x->SetUnc("Dibosons",  0.30);
  	x->SetUnc("tW",        0.30);
  	x->SetUnc("ttV",       0.30);
  	x->SetUnc("ttbar",     0.10);

  	x->SetMembers();

  	x->SetOutputFolder("/nfs/fanae/user/dani/CERN/AnalysisPAF/plotter/HWWplots/CrossSectionHWW/");

  	x->SetTableName("xsec_unc_MC");
  	x->SetYieldsTableName("yields_MC");
  	x->SetXsecTableName("xsec_MC");

  	x->PrintSystematicTable("html,tex,txt");
  	x->PrintYieldsTable("html,tex,txt");
  	x->PrintCrossSection("html,tex,txt");

  	delete p;
};




//selección de eventos en la función MakePlots_WW() que plotea
//=======================================================================================


void MakePlots_HWW(){
  
//==========================//Loops signal region //
   	LoopDrawCR("CRHWW");

//=========================//Plots simples por cortes (algunos ejemplos);=====================================// 
  
   	//Draw("TLep_Pt[0]", "!TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 50, 0, 250, "P^{T}_{l}", "0jetsBaseLine", "./HWWplots/");

	//Draw("TDeltaPhi", "TNJets == 1 && TNVetoJets == 0 && !TIsSS && TPtdil > 30 && TMET > 20", "ElMu", 8, -4, 4, "#Delta#phi_{ll}", "1jetsBaseline", "./HWWplots/");
	//Draw("TMT", "TNJets == 1 && TNVetoJets == 0 && !TIsSS && TPtdil > 30 && TMath::Abs(TDeltaPhi) < 1.8 && TMET > 20", "ElMu", 32, 0, 250, "M_{T}", "1jetsBaseline", "./HWWplots/");
	//Draw("TMll", "TMll > 12 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 35, 0, 340, "M_{ll}", "InvMass", "./CRHWW/");
	
	//Draw("TMT", "TNJets == 0 && TNVetoJets == 0 && !TIsSS", "ElMu", 25, 0, 200, "M_{ll}", "0jetsBaseline", "./HWWplots/");

   	//Draw("TPtdil", "TPtdil > 30 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 50, 0, 250, "P^{T}_{ll}", "0jets1Cut", "./HWWplots/");
	//Draw("TDeltaPhi", "TPtdil > 30 && TNJets == 0 && TNVetoJets == 0 && !TIsSS", "ElMu", 8, -4, 4, "#Delta#phi_{ll}", "0jets1Cut", "./HWWplots/");


   	//Draw("TMll", "TMT > 80 && TMath::Abs(TDeltaPhi) < 1.8 && TMll > 12 && TMll < 200 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 19, 11, 200, "M_{ll}", "Finalelection", "./HWWplots/");
	
	//Draw("TPtdil", "TMT > 80 && TMT < 250 && TMath::Abs(TDeltaPhi) < 1.8 && TMll > 12 && TMll < 200 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 19, 31, 140, "P_{T}^{ll}", "FinalSelection", "./HWWplots/");

 //xsec();

};


