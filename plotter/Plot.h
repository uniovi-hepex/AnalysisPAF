#ifndef Plot_h
#define Plot_h 1

#include "Histo.h"
#include "Looper.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TString.h"
#include <iostream>
#include "TResultsTable.h"

//##################################################
//# Configuration
//##################################################
// Control booleans

// Defaul cases and paths
const TString DefaultPlotfolder = "./Plots/";
const TString DefaultLimitFolder = "./Datacards/";
const Float_t DefaultLumi = 35.87; //fb-1


class Plot {
public:
  bool verbose         = false;
  bool doSys           = true;
  bool doData          = true;
  bool doYieldsInLeg   = true;
  bool doSingleLep     = false;
  bool doStackOverflow = true;
  bool doSignal        = true;
  bool doSetLogy       = true;
  bool doStatUncInDatacard = true;

  std::vector<Histo*> VBkgs;
  std::vector<Histo*> VSignals;
  std::vector<Histo*> VSignalsErr;
  std::vector<Histo*> VData;
  std::vector<Histo*> VSyst;
  std::vector<Histo*> VSumHistoSystUp;
  std::vector<Histo*> VSumHistoSystDown;
  std::vector<TString> VSystLabel;
  std::vector<TString> VTagSamples;
  std::vector<TString> VTagDataSamples;
  std::vector<TString> VTagProcesses;
  std::vector<TString> VTagOptions;
  Histo* hData = NULL;
  THStack* hStack = NULL;
  Histo* hAllBkg = NULL;
 // Histo* hSignal = NULL; // For a default signal if needed

  Int_t nBkgs = 0;
  TPad* plot = NULL; TPad* pratio = NULL;
  TLatex* texlumi = NULL;
  TLatex* texcms = NULL;
  TLatex* texchan = NULL;
  TH1F* hratio = NULL;
  Float_t* TotalSysUp = NULL;
  Float_t* TotalSysDown = NULL;
  TString sys = "0";
  Float_t sys_lumi = 0.026;

	Plot(){
		plotFolder = DefaultPlotfolder; 
    limitFolder = DefaultLimitFolder; 
    Lumi = DefaultLumi;
    nSignalSamples = 0;
        }
        Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Double_t bin0 = 0, Double_t binN = 0, TString tit = "title", TString xtit = "VAR"){
    var    = variable;
    cut    = (cuts);
    chan   = channel;
    nb     = nbins;
    x0     = bin0;
    xN     = binN;
    title = tit;
    xtitle = xtit;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));

    plotFolder = DefaultPlotfolder;
    limitFolder = DefaultLimitFolder; 
    Lumi = DefaultLumi;
    VBkgs = std::vector<Histo*>();
    VSignals = std::vector<Histo*>();
    VSignalsErr = std::vector<Histo*>();
    VData = std::vector<Histo*>();
    VSyst = std::vector<Histo*>();
		VSumHistoSystUp = std::vector<Histo*>();
		VSumHistoSystDown =  std::vector<Histo*>();
    VSystLabel = std::vector<TString>();
    VTagSamples = std::vector<TString>();
    VTagDataSamples = std::vector<TString>();
    VTagProcesses = std::vector<TString>();
    VTagOptions = std::vector<TString>();
    hData = NULL;
    hStack = NULL;
    hAllBkg = NULL;
    //hSignal = NULL; 

    plot = NULL; pratio = NULL;
    texlumi = NULL;
    texcms = NULL;
    texchan = NULL;
    hratio = NULL;
    TotalSysUp = NULL;
    TotalSysDown = NULL;
    nSignalSamples = 0;
  }
	virtual ~Plot(){
		//if(plot) delete plot;
		//if(pratio) delete pratio;
//		if(texlumi) delete texlumi;
//		if(texcms) delete texcms;
//		if(texchan) delete texchan;
		VData.clear();
		VBkgs.clear();
		VSignals.clear();
		VSignalsErr.clear();
		VSyst.clear();
		VSumHistoSystUp.clear();
		VSumHistoSystDown.clear();
		VSystLabel.clear();
		VTagSamples.clear();
		VTagDataSamples.clear();
		VTagProcesses.clear();
		if(hratio) delete hratio;
		if(TotalSysUp) delete TotalSysUp;
		if(TotalSysDown) delete TotalSysDown;
    if(hData && doData) delete hData;
    if(hStack) delete hStack;
    if(hAllBkg) delete hAllBkg;
};            // Destructor

	void AddSample(TString p = "TTbar_Powheg", TString pr = "ttbar", Int_t type = -1, Int_t color = 0, TString tsys = "0", TString options = "");

	// ######### Methods ########
  Histo* GetH(TString sample = "TTbar_Powheg", TString s = "0", Int_t type = itBkg);
  TCanvas *SetCanvas();
  TLegend* SetLegend();
  void SetTexChan(TString cuts); // To be updated
  void SetHRatio(); // To be updated

  void SetData();
  void GetStack();
  void GetAllBkg();
  void AllBkgSyst();
  
  void SetPlotStyle();

  void SetGoF(TString thegof="chi2");
  
  void DrawStack(TString tag, bool sav);
  void DrawComp(TString tag = "0", bool sav = 1, bool doNorm = 0, TString style = "pe");
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
  Float_t GetLumiUnc(){ return sys_lumi;}
  TString GetGoF(){ return gof;}
  void SetVar(TString variable){ var = variable; if(varname == "") varname = variable;}
  void SetVarName(TString variable){ varname = variable;}
  void SetChan(TString ch){chan = ch;}
  void SetLumi(Float_t lum){Lumi = lum;} 
  void SetLumiUnc(Float_t lum){sys_lumi = lum;} 
  void SetPlotFolder(TString f){plotFolder = f;} 
  void SetLimitFolder(TString f){limitFolder = f;}   
	void SetCut(TString cuts){cut = (cuts);}
	void SetBins(Int_t nbins, Double_t bin0, Double_t binN){
		nb = nbins; x0 = bin0; xN = binN;
  }
	void SetTitle(TString tit){title = tit;}
	void SetTitleX(TString xtit){xtitle = xtit;}


  void AddToHistos(Histo* p);
	void AddVarHistos(TString sys);
	void AddSystematic(TString s);
  void AddToSystematicLabels(TString sys){
    sys.ReplaceAll("Up", ""); sys.ReplaceAll("Down", "");
    for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++){
      if(VSystLabel.at(i) == sys) return;
    }
    VSystLabel.push_back(sys);
  }
  void GroupSystematics();
  void AddSumHistoSystematicUp(Histo* hsys){hsys->SetStyle(); VSumHistoSystUp.push_back(hsys);}
  void AddSumHistoSystematicDown(Histo* hsys){hsys->SetStyle(); VSumHistoSystDown.push_back(hsys);}
  void IncludeBkgSystematics();

  void SetPath(TString p){ path = p; if(pathSignal == "") pathSignal = path; if(pathData == "") pathData = path;}
  void SetPathToHeppyTrees(TString p){ pathToHeppyTrees = p;}
  void SetPathSignal(TString p){ pathSignal = p; }
  void SetPathData(TString p){ pathData = p; }
  void SetTreeName(TString p){ treeName = p;}
  void SetOutputName(TString p){ outputName = p;}
  void SetYieldsTableName(TString p){ YieldsTableName = p;}
  TString GetOutputName(){ return outputName;}
  TString GetPathToHeppyTrees(){ return pathToHeppyTrees;}

  Histo* GetHistoNormLHE(TString sampleName);

  void PrintSamples();
  void PrintSystematics(); 
  void PrintYields(TString cuts = "", TString labels = "", TString channels = "", TString options = "");
	void PrintSystYields();
	Float_t GetYield(TString pr = "ttbar", TString systag = "0");
	Histo* GetHisto(TString pr = "ttbar", TString systag = "0");
  Histo* GetSymmetricHisto(TString pr = "ttbar", TString systag = "0");
  void AddSymmetricHisto(TString pr, TString systag);
  Float_t GetData();
  Histo* GetHData();
  Float_t GetTotalSystematic(TString pr);
  Int_t GetColorOfProcess(TString pr);
  Plot* NewPlot(TString newVar = "", TString newCut = "", TString newChan = "", Int_t newnbins = -1, Float_t newbin0 = -999, Float_t newbinN = -999, TString newtitle = "", TString newXtitle = "");

  void SetRatioMin(Float_t r){ RatioMin = r;}
  void SetRatioMax(Float_t r){ RatioMax = r;}
  void SetScaleMax(Float_t s){ ScaleMax = s;}
  void SetScaleLog(Float_t s){ ScaleLog = s;}
  void SetPlotMinimum(Float_t p){ PlotMinimum = p;}
  void ScaleProcess(TString process, Float_t factor = 1);
  void ScaleSignal(Float_t factor = 1);
  void ScaleSys(TString processSys, Float_t factor = 1);
  void SetTableFormats(TString t){ tableFormats = t;}
  void SetLoopOptions(TString t){LoopOptions = t;}
  void SetRatioOptions(TString t){RatioOptions = t;}

  void SetSignalProcess(TString p){ SignalProcess = p;}
  void SetSignalStyle(TString p){ SignalStyle = p;} 
  TString GetSignalProcess(){ return SignalProcess;}

protected: 
  TString pathToHeppyTrees = "";
  TString path = "";
  TString pathSignal = "";
  TString pathData = "";
  TString treeName = "";
  TString outputName = "";
  TString YieldsTableName = "yields";
  TString tableFormats = "%1.2f";
  TString gof = "";

  Int_t nSignalSamples;
  
  // Maximum and minimum value of the ratio plot
  Float_t RatioMin = 0.8;
  Float_t RatioMax = 1.2;
  
  // Factor to multiply the maximum of the plot to set the maximum
  Float_t ScaleMax = 1.2;
  Float_t ScaleLog = 500;
  Float_t PlotMinimum = 0.;

  TString varname = "";
	TString var;
  TString chan;
  TString signal;
  TString cut;
  Int_t nb; Double_t x0; Double_t xN;
  TString  title;
  TString xtitle;
 
  TString SignalProcess;
  TString SignalStyle = "";
  TString LoopOptions = "";
  TString RatioOptions = "";

  TString SystVar;
  Int_t iS;
  Float_t Lumi;
  TString plotFolder;
  TString limitFolder;
  Bool_t ShowSystematics = false;
};

#endif
