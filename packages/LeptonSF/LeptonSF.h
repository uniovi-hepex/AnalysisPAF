#ifndef LEPTONSF_H
#define LEPTONSF_H 1

#include <iostream>
#include "TFile.h"
#include "TH2F.h"
#include "Lepton.h"
#include "Functions.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TSystem.h"

//const TString path_to_SF_histos = gSystem->WorkingDirectory() + TString("/InputFiles/");

const Float_t lumiBCDEF = 19.706;
const Float_t lumiGH    = 16.1454;

class LeptonSF {
 public:
  LeptonSF(TString path = "~/AnalyserPAF/InputFiles/");
  ~LeptonSF() {}
  void loadHisto(Int_t iHisto, Int_t wp = -1);
  Float_t GetTrackerMuonSF(Float_t eta);
  Float_t GetLeptonSF(Float_t pt, Float_t ieta, Int_t type = 0);
  Float_t GetLeptonSFerror(Float_t pt, Float_t ieta, Int_t type = 0);
  TGraphAsymmErrors* LoadTrackerMuonSF(const char* file, const char* histo);
  TString path_to_SF_histos;
  
  // Trigger SFs
  Float_t GetTrigDoubleMuSF(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigDoubleElSF(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigElMuSF    (Float_t eta1, Float_t eta2) const;
  // Trigger SF errors
  Float_t GetTrigDoubleMuSF_err(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigDoubleElSF_err(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigElMuSF_err    (Float_t eta1, Float_t eta2) const;
  
  std::vector<Int_t> loadedHistos;

 protected:
  TH2D* GetHistogramFromFileD(const char* file, const char* histo, const char* hname);
  TH2F* GetHistogramFromFileF(const char* file, const char* histo, const char* hname) const;
  
 private:
  // Muon SFs
  TGraphAsymmErrors*  fMuonTrackerSF;    
  TH2D*  fMuonIdSFSUSY;     
  TH2D*  fMuonIdSF_BCDEF;     
  TH2D*  fMuonIdSF_GH;     
  TH2D*  fMuonIsoSF_BCDEF;     
  TH2D*  fMuonIsoSF_GH;     
  TH2D*  fMuonIsoSFSUSY;     
  TH2D*  fMuonIP2DSF;     
  TH2D*  fMuonSIP3DSF;     
  TH2D*  fMuonIsoFastSim;
  TH2D*  fMuonIdFastSim;
  TH2D*  fElecFastSim;
  TH2D*  fMuonlepMVA2lSSttH;
  TH2D*  fMuonlepMVA3l4lttH;
  TH2D*  fMuonLooseTracksttH;
  TH2D*  fMuonLooseMiniIsottH;
  TH2D*  fMuonTightIP2DttH;
 
  // Elec SFs
  TH2D*  fElecTrackerSF;
  TH2D*  fElecIdSF;
  TH2D*  fElecIsoSF; 
  TH2D*  fElecIP2DSF;     
  TH2D*  fElecSIP3DSF;
  TH2D*  fEleclepMVA2lSSttH;
  TH2D*  fEleclepMVA3l4lttH;
  TH2D*  fElecTightIP2DM17ttH;
  TH2D*  fElecMini4M17ttH;
  TH2D*  fElecConvVetoM17ttH;

  // Trigger SFs
  TH2F *fDoubleMuSF;
  TH2F *fDoubleElSF;
  TH2F *fMuEGSF;
};
#endif
