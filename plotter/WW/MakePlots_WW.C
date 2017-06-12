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
//const Int_t nChan = 4;
const Int_t nChan = 1;
Int_t NBins;
Int_t X0;
Int_t XN;


TString Path = "/nfs/fanae/user/dani/CERN/AnalysisPAF/WW_temp/NewSamplesSyst/"; 
TString Var[nVar] = {"TMET", "TMll", "TMT", "TPtdil", "TLep_Pt[0]", "TJet_Pt[0]", "TLep_Pt[1]", "TJet_Pt[1]", "TNJets", "TNBtags", "TDeltaPhi"};
//TString Chan[nChan] = {"ElMu", "Muon", "Elec", "All"};
TString Chan[nChan] = {"ElMu"};
TString CR;
TString Cuts;
TString XLegend;
TString Nobtag0jets    = "TMll > 12 && TMET > 20 && TPtdil > 30  && !TIsSS && TNJets == 0 && TNVetoJets == 0";

//ttbar control region
TString ttbarCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS"; // >= 2 jet y al menos un b-jets
//Same sign control region
TString SSCut = "TMET > 20 && TPtdil > 30 && TNBtags == 0 && TIsSS"; //se dividirá en 0 jets y 1 jets
//WW signal control region
TString WWCut = "TMll > 12 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS"; //idem
//DY control region de 0 jets y 1 jets
TString DYCut = "TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS"; //idem

//===============  regiones inclusivas ============== (inclusiva en < 2 jets sin b-jets)
//DY control region inclusiva
TString DYCutInc = "!TIsSS && TMET > 20 && TPtdil > 30 && TNJets < 2 && TNBtags == 0"; 
//WW control region inclusiva
TString WWCutInc = "!TIsSS && TMET > 20 && TPtdil > 30 && TNJets < 2 && TNBtags == 0";
//SS control region inclusiva
TString SSCutInc = "TIsSS && TMET > 20 && TPtdil > 30 && TNJets < 2 && TNBtags == 0";


void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder = "");
void LoopDrawCR(TString CR);
void xsec();

//selección en CR de control región en la función LoopDrawCR( ) que plotea regiones de control
//=======================================================================================================================


void LoopDrawCR(TString CR){
	 
	if(CR == "CRSS") Cuts = SSCut;
	if(CR == "CRttbar") Cuts = ttbarCut;
	if(CR == "CRWW") Cuts = WWCut;
	if(CR == "CRDY") Cuts = DYCut;
	if(CR == "CRDYInc") Cuts = DYCutInc;
	if(CR == "CRWWInc") Cuts = WWCutInc;
	if(CR == "CRSSInc") Cuts = SSCutInc;
				
	for(Int_t i=0 ; i < nVar ; i++){
		if     (Var[i] == "TMET")        NBins = 25 , X0 = 0 , XN = 250 , XLegend = "MET [GeV]";
		else if(Var[i] == "TMll")        NBins = 34 , X0 = 0 , XN = 340 , XLegend = "InvMass [GeV]";
		else if(Var[i] == "TMT")         NBins = 30 , X0 = 0 , XN = 300 , XLegend = "M_{T} [GeV]";
		else if(Var[i] == "TPtdil")      NBins = 20 , X0 = 0 , XN = 200 , XLegend = "P^{ll}_{T} [GeV]";
		else if(Var[i] == "TLep_Pt[0]")  NBins = 20 , X0 = 0 , XN = 200 , XLegend = "Lep_Pt_0 [GeV]";
		else if(Var[i] == "TLep_Pt[1]")  NBins = 20 , X0 = 0 , XN = 200 , XLegend = "Lep_Pt_1 [GeV]";
		else if(Var[i] == "TJet_Pt[0]")  NBins = 20 , X0 = 0 , XN = 200 , XLegend = "Jet_Pt_0 [GeV]";
		else if(Var[i] == "TJet_Pt[1]")  NBins = 15 , X0 = 0 , XN = 150 , XLegend = "Jet_Pt_1 [GeV]";
		else if(Var[i] == "TNJets")      NBins = 5  , X0 = 0 , XN = 5   , XLegend = "Number Jets";
		else if(Var[i] == "TNBtags")     NBins = 5  , X0 = 0 , XN = 5   , XLegend = "Number b-Jets";
		else if(Var[i] == "DeltaPhi")    NBins = 16 , X0 = -4 , XN = 4  , XLegend = "Delta Phi";
			
        	//en principio la CRDYInc tiene que tener el pico del Z dentro y el path a su carpeta cambia solo.
		if((CR == "CRDYInc") || (CR == "CRSSInc") || (CR == "CRWWInc")){
			for(Int_t j=0; j < nChan ; j++){
				Draw(Var[i], Cuts, Chan[j], NBins, X0, XN, XLegend, "Inclusiva (0jets or 1jets)", CR+"/"+Chan[j]);
			}
		}
		else if((CR == "CRSS") || (CR == "CRWW") || (CR == "CRDY")){
			for(Int_t j=0; j < nChan ; j++){
				Draw(Var[i], Cuts + "&& TNJets == 0 && TNVetoJets == 0", Chan[j], NBins, X0, XN, XLegend, "0jets", CR+"/"+Chan[j]);
				Draw(Var[i], Cuts + "&& TNJets == 1", Chan[j], NBins, X0, XN, XLegend, "1jets", CR+"/"+Chan[j]);
			}		 
		}
		else if(CR == "CRttbar"){
			for(Int_t j=0; j < nChan ; j++){
                Draw(Var[i], Cuts, Chan[j], NBins, X0, XN, XLegend, ">=1btag >=2jets", CR+"/"+Chan[j]);
            }
		}
	}		
};	


void Draw(TString var, TString cuts, TString chan, Int_t nbins, Int_t x0, Int_t xN, TString xlabel, TString tag, TString plotFolder){
	
	Plot* p = new Plot(var, cuts, chan, nbins, x0, xN, "Title", xlabel);
    	p->SetPlotFolder(plotFolder);	
	p->SetPath(Path);
	if (CR == "CRDYInc") p->SetPath("/nfs/fanae/user/dani/CERN/AnalysisPAF/WW_temp/DYInc(picoZdentro)/"); 
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
	
	p->AddSample("ggHWWTo2L2Nu", "H-WW", itSignal, kGreen);
	//esta es la señal
	
   	//=================================================================

	p->AddSystematic("JES,Btag,MisTag,PU,stat");
  
	p->PrintYields();
	p->SetSignalStyle("H");
	p->doSetLogy = false;
	p->DrawStack(tag, 1);
	//p->doSetLogy = true;
	//p->DrawStack(tag + "_log", 1);
	delete p;
};


void xsec(){
  	Plot* p = new Plot("TNJets", Nobtag0jets, "ElMu", 1, 0, 10, "Channel", "xsec");
	p->SetPath(Path);
	p->SetTreeName("tree");
	if (CR == "CRDYInc") p->SetPath("/nfs/fanae/user/dani/CERN/AnalysisPAF/WW_temp/DYInc(picoZdentro)/"); 
	p->verbose = true;
	p->SetLumi(35.85145088);

        //====================================================================
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
	p->AddSample("TTbar_Powheg", "ttbar + TW", itBkg, kRed); 
	p->AddSample("WWTo2L2Nu", "WW", itBkg, kBlue+1);
	p->AddSample("GGWWTo2L2Nu", "WW", itBkg);
  	p->AddSample("MuonEG, DoubleEG, DoubleMuon, SingleMuon, SingleElec",  "Data",  itData);
	//=======================================================================================

  	p->AddSample("WWTo2L2Nu", "WW", itSys, 1, "ScaleUp"    );
  	p->AddSample("WWTo2L2Nu", "WW", itSys, 1, "ScaleDown"  );
  	p->AddSample("WWTo2L2Nu", "WW", itSys, 1, "pdfUp"      );
  	p->AddSample("WWTo2L2Nu", "WW", itSys, 1, "pdfDown"    );

  	p->AddSample("GGWWTo2L2Nu", "WW", itSys, 1, "ScaleUp"    );
  	p->AddSample("GGWWTo2L2Nu", "WW", itSys, 1, "ScaleDown"  );
  	p->AddSample("GGWWTo2L2Nu", "WW", itSys, 1, "pdfUp"      );
  	p->AddSample("GGWWTo2L2Nu", "WW", itSys, 1, "pdfDown"    );



  	//=======================================================================================
 	
	p->AddSystematic("JES,Btag,MisTag,PU,stat,MuonEff,ElecEff");
 	p->SetSignalStyle("xsec");
  	p->SetTableFormats("%2.4f");
	p->PrintYields("", "", "", "txt, tex, html");

	
	CrossSection *x = new CrossSection(p, "WW");
  	
	x->SetTheoXsec(120.3);
  	x->SetChannelTag("ElMu");

  	x->SetEfficiencySyst("JES, Btag, MisTag, ElecEff, MuonEff, LepEff, Trig, PU, JER");
  	x->SetAcceptanceSyst("stat, ue, nlo, hdamp, scale, pdf, isr, fsr, q2, ME");

  	x->SwitchLabel("VV + TTV", "Dibosons y TTV");
  	x->SwitchLabel("DY", "Drell-Yan");
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
  	x->SetUnc("Wjets", 0.30);
  	x->SetUnc("Dibosons y TTV", 0.30);
  	x->SetUnc("ttbar + TW", 0.30);
  	x->SetMembers();

  	x->SetOutputFolder("/nfs/fanae/user/dani/CERN/AnalysisPAF/plotter/WWplots/CrossSectionWW/");

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


void MakePlots_WW(){
  
//==========================//Loops a todas las control region. 
   //LoopDrawCR("CRSS");
   //LoopDrawCR("CRttbar");
   //LoopDrawCR("CRWW");
   //LoopDrawCR("CRDY");
   
//los Loops a las regiones control Inclusivas:
   
   //LoopDrawCR("CRDYInc");
   //LoopDrawCR("CRWWInc");
   //LoopDrawCR("CRSSInc");
   
//=========================//Plots simples por cortes (algunos ejemplos);=====================================//      
   //Draw("TMT", "TMll > 20 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 30, 0, 300, "MT [GeV]", "0 jets", "./WWplots/");
   //Draw("TMll", "TMll > 12 && TMET > 20 && TPtdil > 30 && TNBtags == 0 && !TIsSS && TNJets == 0 && TNVetoJets == 0", "ElMu", 34, 0, 340, "Mll [GeV]", "0 jets", "./WWplots/");
   //Draw("TChannel", "TMll > 20 && TNJets == 0  && TNVetoJets == 0 && TPtdil > 30 &&  !TIsSS && TMET > 20", "ElMu", 4, 0, 4, "TChannel", "0jets");


//=======================Plot Cross Section ==================================================================//

   //xsec();



};


