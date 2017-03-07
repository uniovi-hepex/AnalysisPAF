#ifndef Plot_h
#define Plot_h 1

#include "Histo.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TString.h"
#include <iostream>

//##################################################
//# Configuration
//##################################################
// Control booleans

// Defaul cases and paths
const TString DefaultPlotfolder = "./Plots/";
const TString DefaultLimitFolder = "./Datacards/";
const Float_t DefaultLumi = 35.9; //fb-1


class Plot {
public:

	bool verbose = false;
	bool doSys       = true;
	bool doData      = true;
	bool doYieldsInLeg   = true;
	bool doSingleLep = false;
	bool doStackOverflow = true;
	bool doSignal = true;

  std::vector<Histo*> VBkgs;
  std::vector<Histo*> VSignals;
  std::vector<Histo*> VSignalsErr;
  std::vector<Histo*> VData;
  std::vector<Histo*> VSyst;
  std::vector<TString> VSystLabel;
  std::vector<TString> VTagSamples;
  std::vector<TString> VTagProcesses;

  Bool_t  doSetLogy   = true;
  Int_t nBkgs = 0;
	TPad* plot; TPad* pratio;
	//TLegend* leg;
	TLatex* texlumi;
	TLatex* texcms;
	TLatex* texchan;
	TH1F* hratio;
  TString sys = "0";
  Float_t* TotalSysUp;
  Float_t* TotalSysDown;
  TString  title;
  TString xtitle;

  Float_t sys_lumi = 0.10;

	Plot(){
		plotFolder = DefaultPlotfolder; 
		limitFolder = DefaultLimitFolder; 
		Lumi = DefaultLumi;
	}
	Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Double_t bin0 = 0, Double_t binN = 0, TString tit = "My plot", TString xtit = "VAR"){
		var    = variable;
		cut    = TCut(cuts);
		chan   = channel;
		nb     = nbins;
		x0     = bin0;
		xN     = binN;
		title  = tit;
		xtitle = xtit;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));

		plotFolder = DefaultPlotfolder; 
		limitFolder = DefaultLimitFolder; 
		Lumi = DefaultLumi;
	}
	virtual ~Plot(){
//		if(plot) delete plot;
//		if(hratio) delete hratio;
//		if(texlumi) delete texlumi;
//		if(texchan) delete texchan;
//		if(TotalSysUp) delete TotalSysUp;
//		if(TotalSysDown) delete TotalSysDown;
    //for(Int_t i = 0; i < (Int_t) VBkgs.size(); i++) delete VBkgs.at(i);
		VBkgs.clear();
		VSignals.clear();
		VSignalsErr.clear();
		VData.clear();
		VSyst.clear();
		VSystLabel.clear();
		VTagSamples.clear();
		VTagProcesses.clear();
};            // Destructor

	void AddSample(TString p = "TTbar_Powheg", TString pr = "ttbar", Int_t type = -1, Int_t color = 0, Float_t S = 1, TString tsys = "0");
	//void AddSample(TString p = "TTbar_Powheg", TString pr = "ttbar", Int_t type = -1, Int_t color = 0, Float_t S = 1, TString Syst);

	// ######### Methods ########
  Histo GetH(TString sample = "TTbar_Powheg", TString s = "0", Int_t type = itBkg);
  TCanvas *SetCanvas();
  TLegend* SetLegend();
  void SetTexChan(TString cuts); // To be updated
  void SetHRatio(); // To be updated

	Histo* SetData();
	THStack* GetStack();
	Histo* GetAllBkg();

  void SetPlotStyle();

	void DrawStack(TString tag, bool sav);
	void DrawComp(TString tag, bool sav);
  void SaveHistograms();
  TString GetStatUncDatacard(Int_t iSignal = 0);
	TString GetShapeUncLines();
  void MakeDatacard(TString tag, Int_t iSignal);
  void MakeAllDatacards(){ for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) MakeDatacard(VSignals.at(i)->GetTag(), i);};

  void MakeDatacardAllBins(TString tag, Int_t iSignal = 0); // Datacard "bin a bin"
  void MakeDatacardBin(Int_t bin, TString tag = "b", Int_t iSignal = 0);

  TString GetVar(){  return var;}
  TString GetChan(){ return chan;}
  TString GetSignal(){ return signal;}
  Float_t GetLumi(){ return Lumi;}
  void SetVar(TString variable){ var = variable; if(varname == "") varname = variable;}
  void SetVarName(TString variable){ varname = variable;}
  void SetChan(TString ch){chan = ch;}
  void SetLumi(Float_t lum){Lumi = lum;} 
  void SetPlotFolder(TString f){plotFolder = f;} 
  void SetLimitFolder(TString f){limitFolder = f;}   
	void SetCut(TString cuts){cut = TCut(cuts);}
	void SetBins(Int_t nbins, Double_t bin0, Double_t binN){
		nb = nbins; x0 = bin0; xN = binN;
  }
	void SetTitle(TString tit){title = tit;}
	void SetTitleX(TString xtit){xtitle = xtit;}


  void AddToHistos(Histo* p);
	void AddVarHistos(TString sys);
	void AddSystematic(TString s);
	void AddSystematicLabel(TString lab){ VSystLabel.push_back(lab);}
  void IncludeBkgSystematics();
  Histo* AllBkgSyst();

  void SetPath(TString p){ path = p; if(pathSignal == "") pathSignal = path;}
  void SetPathSignal(TString p){ pathSignal = p; }
  void SetTreeName(TString p){ treeName = p;}
  void SetOutputName(TString p){ outputName = p;}
  TString GetOutputName(){ return outputName;}

  void PrintSamples();
  void PrintSystematics(); 
  void PrintYields();
	void PrintSystYields();
	Float_t GetYield(TString pr = "ttbar", TString systag = "0");

protected: 
  TString path = "";
  TString pathSignal = "";
  TString treeName = "";
  TString outputName = "";
  
  TString varname = "";
	TString var;
  TString chan;
  TString signal;
  TCut cut;
  Int_t nb; Double_t x0; Double_t xN;

  TString SystVar;
  Int_t iS;
  Float_t Lumi;
  TString plotFolder;
  TString limitFolder;
  Bool_t ShowSystematics = false;
  Bool_t SavedHistos = false;
};

#endif
