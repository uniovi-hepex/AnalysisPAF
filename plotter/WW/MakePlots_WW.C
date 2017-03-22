R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)

#include "Histo.h"
#include "Looper.h"
#include "Plot.h"


const Int_t nVar = 10;
const Int_t nChan = 4;
Int_t NBins;
Int_t X0;
Int_t XN;

TString path = "/nfs/fanae/user/dani/CERN/AnalysisPAF/WW_temp/"; 
TString Var[nVar] = {"TMET", "TMll", "TMT", "TPtdil", "TLep_Pt[0]", "TJet_Pt[0]", "TLep_Pt[1]", "TJet_Pt[1]", "TNJets", "TNBtags"}; //TDeltaPhi está quitado no está en las leaves
TString Chan[nChan] = {"ElMu", "Muon", "Elec", "All"};
TString CR;
TString Cuts;

//ttbar control region
TString ttbarCut = "TNJets >= 2 && TNVetoJets >= 1 && !TIsSS";
//Same sign control region
TString SSCut = "TMET > 20 && TPtdil > 30 && TNVetoJets == 0 && TIsSS"; //dividirla en 0 jets y 1 jets
//WW signal control region
TString WWCut = "TMET > 20 && TPtdil > 30 && TNVetoJets == 0 && !TIsSS"; //dividirla en 0 jets y 1 jets 
//DY control region
TString DYCut = "TMET > 20 && TPtdil > 30 && TNVetoJets == 0 && !TIsSS"; //dividirla en 0 jets, 1 jets e inclusiva






void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder = "");
void LoopDrawCR(TString CR);

//selección en CR de control región en la función LoopDrawCR( ) que plotea regiones de control
//=======================================================================================================================


void LoopDrawCR(TString CR){
	
	if(CR == "SS") Cuts = SSCut;
	if(CR == "ttbar") Cuts = ttbarCut;
	if(CR == "OS") Cuts = OSCut;
	if(CR == "WW") Cuts = WWCut;
				
	for(Int_t i=0 ; i < nVar ; i++){
		if( i >= 0 && i <= 5){
			NBins = 40;
			X0 = 0;
			XN = 400;
		}
		else NBins = 4 , X0 = 0 , XN = 4;
			
		for(Int_t j=0; j < nChan ; j++){
			Draw(Var[i], Cuts + "&& TNJets == 0", Chan[j], NBins, X0, XN, Var[i], "0jets", CR+"/"+Chan[j]);
			if((CR == "SS") || (CR == "WW")){
				Draw(Var[i], Cuts + "&& TNJets == 1", Chan[j], NBins, X0, XN, Var[i], "1jets", CR+"/"+Chan[j]);
			}; 
		};		
	};	
};


//selección de eventos en la función MakePlots_WW() que plotea
//=======================================================================================


void MakePlots_WW(){
  
   //LoopDrawCR("SS");
   //LoopDrawCR("ttbar");
   //LoopDrawCR("OS");
   LoopDrawCR("WW");
   
   //Draw("TMT", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 && !TIsSS && TMET > 20", "ElMu", 30, 0, 300, "M_T [GeV]", "0jets");
   //Draw("TMll", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 && !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "InvMass [GeV]", "0jets");
   //Draw("TMET", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "MET [GeV]", "0jets");
   //Draw("TPtdil", "TMll > 20 && TNJets == 0 && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 40, 0, 400, "Ptll [GeV]", "0jets");
   //Draw("TChannel", "TMll > 20 && TNJets == 0  && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 4, 0, 4, "TChannel", "0jets");
}


void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder){
	
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
    p->SetPlotFolder(plotFolder);
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
