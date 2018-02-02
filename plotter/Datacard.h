#ifndef datac_h
#define datac_h 1

#include "TH1F.h"
#include "TFile.h"
#include "TString.h"
#include <iostream>


TString _separation("##-----------\n");

class Datacard{

  public:
    // Give signal name, background names and nuisances.
    // + By defauls: shapes. If so:
    //    - Give the path and name to the .root file
    //    - Execute GetParamsFromFile(). It gets rates and uncertainties from root file...
    //    - Use SetNormUnc(TString process, Float_t unc) to se normalization uncertainties for all processes
    //    - Adjust systematics and other parameters
    // + If it's not a shape datacard:
    //    - Blablabla
    // + If you want to combine channels:
    //    - Blablabla

    Datacard(TString sigName, TString bkgNames, TString nuiNames, TString bin = "");
    Datacard(TString sigName, vector<TString> bkgNames, vector<TString> nuiNames, TString bin = "");

    ~Datacard(){};

    Bool_t IsShapeDatacard;
    Bool_t doBeestonBarlow;

    void SetSignalName(TString sn){ signalName = sn;}
    void SetPathToFile(TString g){pathToFile = g; if(!pathToFile.EndsWith("/")) pathToFile += "/";}
    void SetRootFileName(TString g){rootFileName = g; if(!g.EndsWith(".root")) rootFileName += ".root";}
    void SetCardName(TString g){cardName = g; if(!g.EndsWith(".txt")) cardName += ".txt";}

    void InitialiseParams();
    void SetParams();
    Float_t GetYieldFromFile(TString histoname);
    void GetParamsFormFile(TString options = "");

    void SetStatDatacard(Bool_t activate = true);
    void SetStatProcess(TString p, Float_t activate = 1);
    void SetNuisanceProcess(TString p, TString nuisname, Float_t activate = 1);
    void SetNuisancesToProcess(TString p, TString nuisance, Float_t val = 1);
    void SetNuisancesToAllProcesses(TString nuisance, Float_t val = 1);
    void SetRateProcess(TString p, Float_t v = 0);
    void SetNormUnc(TString p, Float_t v = 1);
    void SetRateSignal(Float_t v){ SetRateProcess(signalName, v);} 
    void SetRateData(Int_t v){ dataRate = v;}
    void SetNHistoBins(Int_t n){ nHistoBins = n;}
    void SetLumiUnc(Float_t l){ lumiUnc = l;}

    Int_t GetNProcesses(){ return nProcesses;}
    Int_t GetNNuisances(){ return nNuisances;}
    Int_t GetNHistoBins(){ return nHistoBins;}
    TString GetBinName(){ return binName;}
    Int_t GetDataRate(){ return dataRate;}
    Float_t GetLumiUnc(){ return lumiUnc;}
    TString GetProcessName(Int_t i){ return processes.at(i);}
    TString GetNuisName(Int_t i){ return nuisName.at(i);}
    vector<TString> GetVectorProcesses() { return processes;}
    vector<TString> GetVectorNuisName(){ return nuisName;}
    Float_t *GetArrayRates(){return rate;}
    Bool_t  *GetDoLumi(){return doLumi;}
    Bool_t  *GetDoStat(){return doStat;}
    Float_t GetNormUnc(Int_t i){ return normUnc[i];}
    Float_t *GetDoNuis(Int_t i){ return doNuis[i];}
    Bool_t IsShape(Int_t i){ return isShape[i];}
    void SetShape(TString name, Bool_t activate = true){ for(Int_t i = 0; i < nNuisances; i++){ if(name == nuisName.at(i)) isShape[i] = activate;}}
    void AddNuis(TString nuis){nuisName.push_back(nuis); SetParams();}

    TString GetFileString();
    void PrintDatacard(TString fname = "");
    std::vector<TString> TStringToVector(TString t, char separator = ',');

  private:
    TString signalName;
    TString binName;
    vector<TString> processes; // List with the name of the processes
    vector<TString> nuisName;  // Name of all nuisance parameters
    Bool_t  *isShape;  
    Float_t *rate;
    Float_t *normUnc;
    Bool_t  *doLumi;
    Bool_t  *doStat;
    Float_t **doNuis;
    Int_t dataRate;
    Float_t lumiUnc;

    Int_t   nProcesses;
    Int_t   nNuisances;
    Int_t   nHistoBins;
    TString pathToFile; // to root file
    TString rootFileName;
    TString cardName; // output file
};


//======================================================================
// DataLine Class
//======================================================================
// It's a line with a title and some elements...
// You can sum them to obtain same title and more elements
class DataLine{
  public:
    DataLine(Int_t d, TString tit = "");
    DataLine(){};
    ~DataLine(){};
    TString GetLine();
    TString GetElement(Int_t i ){ return pos[i];} 
    Int_t GetDim(){ return dim;}
    TString GetTitle(){return title;}
    void SetTitle(TString t){ title = t;}
    void SetElement(Int_t i, TString g = ""){pos[i] = g;}
    void SetStatus(Bool_t s = true){ use = s;}

    void SetElementsFromArray(Float_t *f);
    void SetElementsFromBool(Bool_t *f);
    void SetElementsFromVectorTString(std::vector<TString> t);

    DataLine operator+(DataLine dl);

  protected:
    Int_t dim; 
    TString title;
    TString *pos;
    Bool_t use;
};

//======================================================================
// Text Datacard Class
//======================================================================
// The text that is printed into a txt
// You can sum them to create combined datacards
class TextDatacard{

  public:
    // General constructor
    TextDatacard(Int_t nPro, Int_t nNui, Int_t nHistoB = 1, Int_t nChan = 1);

    // Constructor from a datacard --> nominal constructor
    TextDatacard(Datacard *datacard);

    TextDatacard(){};
    ~TextDatacard(){};

    // Initialise all the data members
    void Init(){
      doBeestonBarlow = true;
      shapes = "";
      obs_bin     = DataLine(nChannels,  "bin");
      observation = DataLine(nChannels,  "observation");
      pro_bin     = DataLine(nProcesses, "bin");
      pro_name    = DataLine(nProcesses, "process");
      pro_num     = DataLine(nProcesses, "process");
      rates       = DataLine(nProcesses, "rate");
      Lumi        = DataLine(nProcesses, "Lumi lnN");
      processNorm = new DataLine[nProcesses];
      statUnc     = new DataLine[nHistoBins*nProcesses];  
      systUnc     = new DataLine[nNuisances];
      for(Int_t i = 0; i < nProcesses; i++)            processNorm[i] = DataLine(nProcesses);
      for(Int_t i = 0; i < nNuisances; i++)            systUnc[i]     = DataLine(nProcesses);
      totalStatDatacard = "";
      for(Int_t i = 0; i < nProcesses*nHistoBins; i++) statUnc[i]     = DataLine(nProcesses);
    }

    TString GetText();
    void PrintDatacard(TString f = "");
    TString GetStatUncData(Int_t nExtraPos = 0);

    TextDatacard operator+(TextDatacard d);
    TextDatacard operator+(Datacard d){ return *this +  TextDatacard(&d);}

  protected:
    Bool_t doBeestonBarlow;

    Int_t nProcesses;
    Int_t nNuisances;
    Int_t nHistoBins;
    Int_t nChannels;  

    TString shapes;
    DataLine obs_bin;
    DataLine observation;
    DataLine pro_bin;
    DataLine pro_name;
    DataLine pro_num;
    DataLine rates;
    DataLine Lumi;
    DataLine *processNorm;   // nProcesses
    DataLine *statUnc;       // nProcesses*nHistoBins
    DataLine *systUnc;       // nNuisances
    TString totalStatDatacard;
};



#endif
