#ifndef CrossSection_h
#define CrossSection_h 1

#include "Histo.h"
#include "Plot.h"


class CrossSection{
  // Inputs needed:
  // - NData
  // - N expected and label
  // - All bkgs, unc, labels
  // - List of unc (acceptance/efficiency) and label
  // Optional
  //    - BR
  //    - Do fiducial?
  //    - Theoretical xsec + number of gen events
  //    - Tag for level
  //    - Tag for channel

  public:

    CrossSection(){}; // void constructor
    CrossSection(Plot* p, TString sig = ""){  // From a Plot
      /* Takes data, signal and background from a plot
       * Also de systematic uncertainties but you can modify the name of the systematics
       */
      if(sig == "") signalTag = p->GetSignalProcess();
      else signalTag = sig;
      Int_t nBkgs = p->VBkgs.size(); 
      for(Int_t i = 0; i < nBkgs; i++){
        if(signalTag ==  p->VBkgs.at(i)->GetProcess()) continue;
        AddBkgTag(p->VBkgs.at(i)->GetProcess());
        AddBkg(p->VBkgs.at(i)->GetYield());
        AddBkgUnc(1);
        AddBkgStatUnc( (p->GetYield(p->VBkgs.at(i)->GetProcess(), "stat") - p->VBkgs.at(i)->GetYield())/p->VBkgs.at(i)->GetYield() );
      }
      Int_t nSys = p->VSystLabel.size();
      for(Int_t i = 0; i < nSys; i++){
        AddSysTag(p->VSystLabel.at(i));
        AddSysVar(p->GetYield(signalTag, p->VSystLabel.at(i)));
      }
      y = p->GetYield(signalTag, "0");
      y_staterr = TMath::Abs(y - p->GetYield(signalTag, "stat"));
      Lumi = p->GetLumi();
      LumiUnc = p->GetLumiUnc();
      NData = p->GetData(); // Set dataaaaaaaa
      // External inputs not provided by plot: thxsec
      //    Optional inputs: nSimulatedSignal, nFiducialSignal, BR, level tag, channel tag
    }; 
    CrossSection(Int_t nbkg, TString *bkgtag, Float_t *bkg, Float_t *bkg_unc, Int_t nsys, TString *systag, Float_t *var, Float_t signal_yield = 1, Float_t data = 1, Float_t lum = 0, Float_t lum_unc = 0, Bool_t *isEff = 0){ // From yields
      Int_t nBkgs = nbkg;
      for(Int_t i = 0; i < nBkgs; i++){
        AddBkgTag(bkgtag[i]);
        AddBkg(bkg[i]);
        AddBkgUnc(bkg_unc[i]);
      }
      Int_t nSys = nsys;
      for(Int_t i = 0; i < nSys; i++){
        AddSysTag(systag[i]);
        AddSysVar(var[i]);
        if(isEff) AddUncTagEff(isEff[i]);
      }
      y = signal_yield; NData = data;
      Lumi = lum; LumiUnc = lum_unc;
    }; 
    //CrossSection(){}; // From data, bkg and acceptance
    ~CrossSection(){}; // Destructor

    Bool_t IsFiducial = false;
 
    void AddBkg(Float_t v){ BkgYield.push_back(v);}
    void AddBkgUnc(Float_t v){ BkgUnc.push_back(v);}
    void AddBkgStatUnc(Float_t v){ BkgStatUnc.push_back(v);}

    void AddBkgTag(TString t){ BkgTags.push_back(t);}
    void AddSysTag(TString t){ SysTags.push_back(t); IsEffic.push_back(false);}
    void AddSysVar(Float_t v){ SysVar.push_back(v);}
    void AddUncTagEff(Bool_t v){ IsEffic.push_back(v);}
    void SetUnc(TString source, Float_t value);
    void SetYield(TString bkg, Float_t value);
    Float_t GetYield(TString bkg);
    Float_t GetUnc(TString source);
    void SetEfficiencySyst(TString s);
    void SetAcceptanceSyst(TString s);

    void SetLevelTag(TString t){ level = t;}
    void SetChannelTag(TString t){ channel = t;}
    void SetNSimulatedEvents(Float_t n){ nSimulatedSignal = n;}
    void SetNFiducialEvents(Float_t n){ nFiducialSignal = n;}
    void SetNData(Float_t n){ NData = n;}
    void SetLumi(Float_t l){ Lumi = l;}
    void SetLumiUnc(Float_t l){ LumiUnc = l;}
    void SetTheoXsec(Float_t s){ thxsec = s;}
    void SetBR(Float_t b){ BR = b;}
    void SetSignalYield(Float_t n){ y = n;}
    void SetOutputFolder(TString t){ outputFolder = t;}
    void SetTableName(TString t){ tableName = t;}
    void SetXsecTableName(TString t){ xsecvalTableName = t;}
    void SetYieldsTableName(TString t){ yieldsTableName = t;}

    void SetMembers();

    Float_t GetXSec(Float_t data, Float_t bkg, Float_t y){
      if(IsFiducial) return(data-bkg)/y*thxsec*(nFiducialSignal/nSimulatedSignal);
      return (data-bkg)/y*thxsec;
    }
    void PrintSystematicTable(TString options = "");
    void PrintYieldsTable(TString options = "");
    void PrintCrossSection(TString options = "");
    void SwitchLabel(TString oldLabel, TString newLabel);

  protected:

    // Inputs...
    vector<TString> SysTags  = vector<TString>();
    vector<Bool_t>  IsEffic  = vector<Bool_t>();  // Does affect to Efficiency? Or acceptance?
    vector<Float_t> SysVar   = vector<Float_t>(); // (relative)
    vector<TString> BkgTags  = vector<TString>();
    vector<Float_t> BkgUnc   = vector<Float_t>(); // (relative)
    vector<Float_t> BkgStatUnc = vector<Float_t>(); // (relative)
    vector<Float_t> BkgYield = vector<Float_t>(); // Already scaled to lumi
    Int_t nBkgs = 0;
    Int_t nSyst = 0;
    Float_t y = 0; // Nominal signal yield
    Float_t NData = 0;
    Float_t Lumi = 0; // luminosity
    Float_t LumiUnc = 0; // (relative)
    Float_t thxsec = 0;
    Bool_t notSet = true;

    // Optional inputs...
    Float_t nSimulatedSignal = 0; // or sum of weights...
    Float_t nFiducialSignal = 0; // or sum of weights...
    Float_t BR = 0;
    TString level = "";
    TString channel = "";
    TString signalTag = "";
    TString outputFolder = "./";
    TString tableName = "xsec";
    TString yieldsTableName = "xsec_yields";
    TString xsecvalTableName = "xsecval";

    // Derived quantities
    Float_t NBkg = 0;
    Float_t acceptance = 0;
    Float_t efficiency = 0;
    Float_t TotalAcceptance = 0;
    Float_t xsec = 0; // nominal value

    Float_t y_err = 0; 
    Float_t y_staterr = 0; 
    Float_t NData_err = 0;
    Float_t NBkg_err = 0;
    Float_t NBkg_staterr = 0;
    Float_t xsec_total_err = 0;
    Float_t xsec_stat_err = 0;
    Float_t xsec_syst_err = 0;
    Float_t xsec_lumi_err = 0;
    Float_t acc_err = 0;
    Float_t eff_err = 0;
};

#endif
