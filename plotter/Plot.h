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
#include "TGaxis.h"
#include "TExec.h"
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
  bool doLegend        = true;
  bool doUncInLegend   = false;
  bool doExternalSyst  = false;
  bool NoShowVarName   = false;
  bool resizemtop      = false;

  std::vector<Histo*> VBkgs;
  std::vector<Histo*> VSignals;
  std::vector<Histo*> VSignalsErr;
  std::vector<Histo*> VSignalsStack;
  std::vector<Histo*> VData;
  std::vector<Histo*> VSyst;
  std::vector<Histo*> VOther;
  std::vector<Histo*> VSumHistoSystUp;
  std::vector<Histo*> VSumHistoSystDown;
  std::vector<TString> VSystLabel;
  std::vector<TString> VTagSamples;
  std::vector<TString> VTagDataSamples;
  std::vector<TString> VTagProcesses;
  std::vector<TString> VTagOptions;
  std::vector<TString> VBinLabels;
  Histo* hData = NULL;
  THStack* hStack = NULL;
  Histo* hAllBkg = NULL;
 // Histo* hSignal = NULL; // For a default signal if needed

  Int_t nBkgs = 0;
  TPad* plot = NULL; TPad* pratio = NULL;
  TLatex* texlumi = NULL;
  TLatex* texcms = NULL;
  TLatex* texPrelim = NULL;
  TLatex* texchan = NULL;
  TH1F* hratio = NULL;
  Float_t* TotalSysUp = NULL;
  Float_t* TotalSysDown = NULL;
  TString sys = "0";
  Float_t sys_lumi = 0.025;

//  Plot(){
//    plotFolder = DefaultPlotfolder; 
//    limitFolder = DefaultLimitFolder; 
//    Lumi = DefaultLumi;
//    nSignalSamples = 0;
//  }
  Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t bin0 = 0, Float_t binN = 0, TString tit = "title", TString xtit = "VAR"){
    var    = variable;
    cut    = (cuts);
    chan   = channel;
    nb     = nbins;
    x0     = bin0;
    xN     = binN;
    title = tit;
    xtitle = xtit;
    ytitle = xtit;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));
    Init();
  }

  Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t* bins = 0, TString tit = "title", TString xtit = "VAR"){
    var    = variable;
    cut    = (cuts);
    chan   = channel;
    nb     = nbins;
    x0     = 0;
    xN     = 0;
    vbins  = bins;
    title = tit;
    xtitle = xtit;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));
    Init();
  }
  Plot(){
    var  = ""; cut  = ""; chan = ""; 
    nb   = 1; x0   = 0;  xN   = 0; vbins = nullptr; 
    title = "";
    xtitle = ""; varname = ""; 
    Init();
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
	  VBinLabels.clear();
	  if(f)       delete f;
	};            // Destructor

  void Init(){
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
    VBinLabels  = std::vector<TString>();
    hData = NULL;
    hStack = NULL;
    hAllBkg = NULL;
    //hSignal = NULL; 
    plot = NULL; pratio = NULL;
    texlumi = NULL;
    texcms = NULL;
    texPrelim = NULL;
    texchan = NULL;
    texchansize = 0.05;
    hratio = NULL;
    TotalSysUp = NULL;
    TotalSysDown = NULL;
    nSignalSamples = 0;
    fLegX1 = 0.66;  fLegY1 = 0.42; fLegX2 = 0.98;  fLegY2 = 0.92;
    LegendTextSize  = 0.065;
    SignalDrawStyle = "hist";
    CMSlabel = "CMS";
    CMSmodeLabel = "Preliminary";
    textForLumi = "%2.1f fb^{-1} (13 TeV)";
    texlumiX = 0.73; texlumiY = 0.97; texlumiSize = 0.05;
    texPrelimX = 0.15;
    texPrelimY =  0.83;
    texPrelimSize =  22;
    texCMSX =  0.15;
    texCMSY =  0.89;
    texCMSsize =  22;
    RatioErrorColor = kGray+2;
    RatioErrorStyle = 3444;
    StackErrorColor = kGray+2;
    StackErrorStyle = 3444;
    weight = "TWeight";
    systematics = "";
    
    heightcanvas = 600;
    widthcanvas  = 800;

    ytitle = "";
    xtitleOffset = 1.2;
    xtitleDivisions = 510;
    xtitleSize = 0.16;
    ytitleSize = 0.06;
    ytitleLabelSize = 0.05;
    ytitleOffset = 0.67;
    ytitleDivisions = 505;
    xAxisLabelSize = 0.16;
    xAxisTitleSize = 0.16;
    RatioYtitle = "Data/MC";
    yRatioTitleSize = 0.12;
    yRatioTitleLabelSize = 0.12;
    yRatioTitleOffset = 0.26;
    NCols = 1;
    doDecimalsInXAxis = true;
    Xerrorbars        = true;

    dataStyle = "psameE1X0";
    dataTag   = "Data";

    chX = 0.15; chY = 0.81; chSize = 22; chlabel = ""; f = 0;
    RatioMin = 0.8; RatioMax = 1.2;
    ScaleMax = 1.2; ScaleLog = 500;
    PlotMinimum = -999; PlotMaximum = -999;

    kPadPlotLimits   = "0.0, 0.23, 1.0, 1.0";
    kPadRatioLimits  = "0.0, 0.0, 1.0, 0.29";
    kPadPlotMargins  = "0.06, 0.1, 0.02, 0.1"; // top, bottom, right, left
    kPadRatioMargins = "0.03, 0.4, 0.02, 0.1"; // top, bottom, right, left

    kPadPlotSetGrid  = false; kPadRatioSetGrid = true;
    kTextLumiX = 0.73; kTextLumiY = 0.97; kTextLumiSize = 0.05;
    kTextCMSX = 0.15; kTextCMSY = 0.89; kTextCMSsize = 0.06;
    kTextPrelimX = 0.15; kTextPrelimY = 0.83; kTextPrelimSize = 0.052;
    
    centerYaxis = true;
  }


	
public:
	void AddSample(TString p = "TTbar_Powheg", TString pr = "", Int_t type = itBkg, Int_t color = 0, TString tsys = "0", TString options = "");
  void Multiloop(TString p = "TTbar_Powheg", TString pr = "", Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void PrepareHisto(vector<Histo*> vH, TString name, TString pr, Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void PrepareHisto(Histo* h, TString name, TString pr, Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void AddHistoFromFile(TString filename, TString name, int type = 0, int color = 0, TString sys = "", TString pathInRootfile = "");
  void AddHistoFromFileTGraph(TString filename, TString name, int type = 0, int color = 0, TString sys = "", TString pathInRootfile = "");
  void Group(Histo* h);

	// ######### Methods ########
  Histo* GetH(TString sample = "TTbar_Powheg", TString s = "0", Int_t type = itBkg);
  TCanvas *SetCanvas();
  TLegend* SetLegend();
  virtual void SetLegendPosition(TString);
  virtual void SetLegendPosition(float x1 = 0.70, float y1 = 0.65, float x2 = 0.93, float y2 = 0.93){ fLegX1 = x1; fLegY1 = y1; fLegX2 = x2; fLegY2 = y2;}
  virtual void SetTexChan();
  virtual void SetTexChanSize(Float_t siz = 0.05) {texchansize = siz;};
  void SetPad(TPad* pad, TString limits, TString margins, bool doGrid);
  virtual void SetHRatio(TH1F* h = nullptr); // To be updated
  virtual void SetYaxis(TAxis *a = nullptr);
  virtual void SetAxis(TAxis *a, TString tit = "", Float_t titSize = 0.07, Float_t titOffset = 0.5, Int_t nDiv = 510, Float_t labelSize = 0);

  void SetData();
  void GetStack();
  Float_t GetAllBkg(Int_t ibin = -1);
  void AllBkgSyst();
  
  Histo* GetAllBkgClone(TString newname = "NewHisto_AllBkg");
  virtual void SetGoF(TString thegof="chi2");
  
  virtual void DrawStack(TString tag = "");
  void DrawComp(TString tag = "", bool doNorm = 0, TString options = "");
  virtual void SaveHistograms();

  int GetNbins(){  return nb;}
  Float_t GetBin0(){  return x0;}
  Float_t GetBinN(){  return xN;}
  virtual TString GetVar(){  return var;}
  virtual TString GetCut(){  return cut;}
  virtual TString GetChan(){ return chan;}
  virtual TString GetSignal(){ return signal;}
  virtual Float_t GetLumi(){ return Lumi;}
  virtual Float_t GetLumiUnc(){ return sys_lumi;}
  virtual TString GetGoF(){ return gof;}
  virtual void SetVar(TString variable){ var = variable; if(varname == "") varname = variable;}
  virtual void SetVarName(TString variable){ varname = variable;}
  virtual void SetChan(TString ch){chan = ch;}
  virtual void SetLumi(Float_t lum){Lumi = lum;} 
  virtual void SetLumiUnc(Float_t lum){sys_lumi = lum;} 
  virtual void SetPlotFolder(TString f){plotFolder = f;} 
  virtual void SetLimitFolder(TString f){limitFolder = f;}   
	virtual void SetCut(TString cuts){cut = (cuts);}
	virtual void SetBins(Int_t nbins, Double_t bin0, Double_t binN){ nb = nbins; x0 = bin0; xN = binN;}
	virtual void SetBins(Histo* h){ nb = h->GetXaxis()->GetNbins(); x0 = h->GetXaxis()->GetBinLowEdge(1); xN = h->GetXaxis()->GetBinLowEdge(nb+1);}
	virtual void SetBins(TH1* h){ nb = h->GetXaxis()->GetNbins(); x0 = h->GetXaxis()->GetBinLowEdge(1); xN = h->GetXaxis()->GetBinLowEdge(nb+1);}
	void SetTitle(TString tit){title = tit;}
	virtual void SetTitleX(TString xtit, Float_t size = -999){xtitle = xtit; if(size != -999) xtitle = size;}
	virtual void SetTitleY(TString ytit, Float_t size = -999){ytitle = ytit; if(size != -999) ytitleSize = size;}
  void SetDecimalsInXAxis(Bool_t x){doDecimalsInXAxis = x;}
	void AddLumiSyst( float);
	void AddNormSyst( TString, TString, float);

  void SetDataTag(TString t){dataTag = t;}
  void SetDataStyle(TString t){dataStyle = t;}
  virtual void SetYaxisDivisions(Int_t div){ytitleDivisions = div;}
  virtual void SetXaxisDivisions(Int_t div){xtitleDivisions = div;}
  virtual void SetXaxisOffset(Float_t Offset, Float_t size = -999){ xtitleOffset = Offset; if(size != -999) ytitleLabelSize = size;}
  virtual void SetYaxisOffset(Float_t Offset, Float_t size = -999){ ytitleOffset = Offset; if(size != -999) ytitleLabelSize = size;}
  virtual void SetYratioOffset(Float_t offset, Float_t size = -999){ yRatioTitleOffset = offset; if(size != -999) yRatioTitleSize = size;}
  virtual void SetBinLabels(TString t, char separator = ','){VBinLabels = TStringToVector(t, separator);}
  void ObliterateXErrorBars() {Xerrorbars = false;}
  void SetXaxisLabelSize(float x){xAxisLabelSize = x;}
  void SetXaxisTitleSize(float x){xAxisTitleSize = x;}
  void SetTextLumiPosX(Float_t x){texlumiX = x;}
  void SetTextLumiPosY(Float_t x){texlumiY = x;}
  void SetNColumns(Int_t nc){NCols = nc;}
  void doResizemtop(Bool_t doornot){resizemtop = doornot;}

  void AddToHistos(Histo* p);
	void AddVarHistos(TString sys);
	void AddSystematic(TString s, TString pr = "");
  void AddStatError(TString process = "");
  void AddToSystematicLabels(TString sys){
    if(sys.Contains(",")){
      sys.ReplaceAll(" ", "");
      vector<TString> v = TStringToVector(sys, ',');
      for(Int_t i = 0; i < (Int_t) v.size(); i++) AddToSystematicLabels(v.at(i));
      return;
    }
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

public:
  virtual void SetPath(TString p){ path = p; if(pathSignal == "") pathSignal = path; if(pathData == "") pathData = path;}
  virtual void SetPathToHeppyTrees(TString p){ pathToHeppyTrees = p;}
  virtual void SetPathSignal(TString p){ pathSignal = p; }
  virtual void SetPathData(TString p){ pathData = p; }
  virtual void SetTreeName(TString p){ treeName = p;}
  virtual void SetOutputName(TString p){ outputName = p;}
  virtual void SetYieldsTableName(TString p){ YieldsTableName = p;}
  virtual TString GetOutputName(){ return outputName;}
  virtual TString GetPathToHeppyTrees(){ return pathToHeppyTrees;}
  virtual TString GetPath(){ return path;}

  virtual void PrintSamples();
  void PrintSystematics(); 
  virtual void PrintYields(TString cuts = "", TString labels = "", TString channels = "", TString options = "");
  virtual void PrintBinsYields(TString options = "txt,tex,html");
	virtual void PrintSystYields();
	virtual Float_t GetYield(TString pr = "ttbar", TString systag = "0", Int_t ibin = -1);
	virtual Float_t GetBinYield(TString pr = "ttbar", Int_t bin = 1, TString systag = "0");
  virtual Histo* GetHisto(TString pr = "ttbar", TString systag = "0");
  virtual Int_t GetProcessType(TString pr, TString systag = "");
  virtual Int_t GetProcessIndex(TString pr, TString systag = "");
  virtual void RemoveProcess(TString pr, TString systag = "");
  virtual Bool_t Exists(TString pr, TString systag = "");
  virtual Bool_t RenameProcess(TString pr, TString newname, TString systag = "");
  virtual void GroupProcesses(TString pr, TString newProcess);
  void AddUncToHisto(TString pr, TString sys, TString pr_new, TString sys_new);
  Histo* GetHistoError(TString pr, TString sys);
  virtual void AddNormUnc(TString pr, Float_t systUp = 1, Float_t systDown = - 99);
	virtual void SetLineStyle(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetLineStyle(s);}
	virtual void SetLineColor(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetLineColor(s); h->SetFillColor(s);}
	virtual void SetMarkerColor(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetMarkerColor(s);}
	virtual void SetMarkerStyle(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetMarkerStyle(s);}
  virtual Histo* GetSymmetricHisto(TString pr = "ttbar", TString systag = "0");
  virtual void AddSymmetricHisto(TString pr, TString systag);
  virtual Float_t GetData(Int_t ibin = -1);
  virtual Histo* GetHData();
  virtual Float_t GetTotalSystematic(TString pr);
  virtual Int_t GetColorOfProcess(TString pr);
  virtual Plot* NewPlot(TString newVar = "", TString newCut = "", TString newChan = "", Int_t newnbins = -1, Float_t newbin0 = -999, Float_t newbinN = -999, TString newtitle = "", TString newXtitle = "");
  virtual void RemoveSystematic(TString sys);
  virtual void UseEnvelope(TString pr, TString tags, TString newname = "");

  virtual void SetCanvasHeight(Float_t r) { heightcanvas = r;}
  virtual void SetCanvasWidth(Float_t r)  { widthcanvas = r;}
  
  void SetHistoTotalSyst(Histo* h){ hAllBkg = h->CloneHisto("AllBkg"); hAllBkg->SetDirectory(0); doExternalSyst = true;}
  virtual void SetWeight(TString t){ weight = t;}
  virtual void SetSystematics(TString t){ weight = t;}
  virtual void SetRatioMin(Float_t r){ RatioMin = r;}
  virtual void SetRatioMax(Float_t r){ RatioMax = r;}
  virtual void SetScaleMax(Float_t s){ ScaleMax = s;}
  virtual void SetScaleLog(Float_t s){ ScaleLog = s;}
  virtual void SetPlotMinimum(Float_t p){ PlotMinimum = p;}
  virtual void SetPlotMaximum(Float_t p){ PlotMaximum = p;}
  virtual void ScaleProcess(TString process, Float_t factor = 1);
  virtual void ScaleProcessBin(TString pt, Float_t SF = 1, Int_t ibin = 0);
  virtual void ScaleSignal(Float_t factor = 1);
  virtual void ScaleSys(TString processSys, TString sys = "", Float_t factor = 1);
  virtual void SetTableFormats(TString t){ tableFormats = t;}
  virtual void SetLoopOptions(TString t){LoopOptions = t;}
  virtual void SetRatioYtitle(TString t){RatioYtitle = t;}
  virtual void SetSignalDrawStyle(TString t){ SignalDrawStyle = t;}
  virtual void SetLegendTextSize(Float_t t){ LegendTextSize = t;} 
  virtual void SetRatioErrorColor(Int_t b){ RatioErrorColor = b;}
  virtual void SetRatioErrorStyle(Int_t b){ RatioErrorStyle = b;}
  virtual void SetStackErrorColor(Int_t b){ StackErrorColor = b;}
  virtual void SetStackErrorStyle(Int_t b){ StackErrorStyle = b;}
  void SetPadPlotLimits(  TString t){ kPadPlotLimits = t;}
  void SetPadRatioLimits( TString t){ kPadRatioLimits = t;}
  void SetPadPlotMargins( TString t){ kPadPlotMargins = t;}
  void SetPadRatioMargins(TString t){ kPadRatioMargins = t;}
  virtual void SetCenterYAxis(Bool_t doitornot){ centerYaxis = doitornot;};

  virtual void SetSignalProcess(TString p){ SignalProcess = p;}
  virtual void SetSignalStyle(TString p){ SignalStyle = p;} 
  TString GetSignalProcess(){ return SignalProcess;}

  virtual void SetCMSlabel(TString t){ CMSlabel = t;}
  virtual void SetCMSmodeLabel(TString t){ CMSmodeLabel = t;}
  virtual void SetChLabel(TString t){chlabel= t;}
  virtual void SetCMSLabelPos(float x, float y, float s = -1){texCMSX = x; texCMSY = y; if(s!= -1) texCMSsize = s;}
  virtual void SetCMSmodeLabelPos(float x, float y, float s = -1){texPrelimX = x; texPrelimY = y; if(s!= -1) texPrelimSize = s;}
  virtual void SetChLabelPos(float x, float y, float s = -1){chX = x; chY = y; if(s!= -1) chSize = s;}
  virtual void SetTextForLumi(TString t, float X = -1, float Y = -1, float s = -1){textForLumi = t; if(X!=-1) texlumiX = X; if(Y!=-1) texlumiY = Y; if(s!=-1) texlumiSize = s;}
  
  virtual void AddPlotFromHyperlooper(Hyperlooper *HyperLoop, TString plotname);
  void Reset();

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
  TString weight;
  TString systematics;
  
  TFile *f;

  Int_t nSignalSamples;
  
  // Maximum and minimum value of the ratio plot
  Float_t RatioMin;
  Float_t RatioMax;
  
  // Factor to multiply the maximum of the plot to set the maximum
  Float_t ScaleMax;
  Float_t ScaleLog;
  Float_t PlotMinimum;
  Float_t PlotMaximum;

  TString varname = "";
	TString var;
  TString chan;
  TString signal;
  TString cut;
  Int_t nb; Double_t x0; Double_t xN; Float_t *vbins;
  TString  title;
  TString xtitle;
  TString ytitle;
  Float_t xtitleOffset;
  Int_t   xtitleDivisions;
  Float_t xtitleSize;
  Float_t ytitleSize;
  Float_t ytitleLabelSize;
  Float_t ytitleOffset;
  Float_t xAxisLabelSize;
  Float_t xAxisTitleSize;
  TString yAxisTitleStyle;
  Int_t   ytitleDivisions;
  Float_t chX;
  Float_t chY;
  Float_t chSize;
  
  Bool_t doDecimalsInXAxis;
  
  TExec *ex1;
  TExec *ex2;
  Bool_t  Xerrorbars;
  Float_t heightcanvas;
  Float_t widthcanvas;
  
  TString dataStyle;
  TString dataTag;
  TString SignalProcess;
  TString LoopOptions = "";
  TString RatioYtitle;
  Float_t yRatioTitleSize;
  Float_t yRatioTitleLabelSize;
  Float_t yRatioTitleOffset;
  TString SignalStyle = "";
  TString SignalDrawStyle;
  TString CMSlabel;
  TString CMSmodeLabel;
  TString textForLumi;
  float texlumiX; float texlumiY; float texlumiSize;
  float texPrelimX; // 0.15
  float texPrelimY; // 0.83
  float texPrelimSize; // 22
  float texCMSX; // 0.15
  float texCMSY; // 0.89
  float texCMSsize; // 22
  float texchansize; // 22
  Int_t  RatioErrorColor;
  Int_t  RatioErrorStyle;
  Int_t  StackErrorColor;
  Int_t  StackErrorStyle;
  Int_t  NCols;

  TString SystVar;
  Int_t iS;
  Float_t Lumi;
  TString plotFolder;
  TString limitFolder;
  Bool_t ShowSystematics = false;
  TString chlabel;
  Bool_t centerYaxis; // 22

  float fLegX1; float fLegY1; float fLegX2; float fLegY2; Float_t LegendTextSize;
  TString kPadPlotLimits; TString kPadRatioLimits; TString kPadPlotMargins; TString kPadRatioMargins;
  Bool_t kPadPlotSetGrid; Bool_t kPadRatioSetGrid;
  Float_t kTextLumiX; Float_t kTextLumiY; Float_t kTextLumiSize;
  Float_t kTextCMSX; Float_t kTextCMSY; Float_t kTextCMSsize;
  Float_t kTextPrelimX; Float_t kTextPrelimY; Float_t kTextPrelimSize;
};

class MultiPlot : public Plot{
  public:
  MultiPlot(){};

  protected:
  Int_t nDist;
  Int_t nSamples;

  // nDistributions
  vector<TString> Dname;
  vector<TString> Dvar;
  vector<TString> Dcut;
  vector<TString> Dchan;
  vector<TString> Dweight;

  //vector<TString> DSyst;
  vector<Int_t>   Dnbins;
  vector<Float_t> Dbin0;
  vector<Float_t> DbinN;
  vector<Float_t*> Dbins;

  // nSamples
  vector<Hyperlooper*> HL; 
  vector<Int_t> VTypes;
  vector<Int_t> VColors;
  vector<TString> VSystL;
  vector<TString> VWeight;
  vector<TString> VOptions;
  vector<TString> VPaths;
  // VSamples, VProcesses --> From Plot

  public:
  void AddDistribution(TString name, TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t *bins = nullptr);
  void AddSample(TString sample, TString process = "", Int_t type = itBkg, Int_t color = 1, TString syst = "", TString weight = "TWeight", TString options = "");
  void AddHyperlooper(TString sample, TString process, Int_t type, Int_t color, TString syst, TString weight = "TWeight", TString options = "", TString path = "");
  void Loop();
  void SetPlot(TString name, TString xtit = "", TString ytit = "", TString seltext = "", Float_t ratioMax = 1.2, Float_t rationMin = -999);
};

#endif
