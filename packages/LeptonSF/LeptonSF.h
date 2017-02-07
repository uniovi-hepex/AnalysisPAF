#ifndef LEPTONSF_H
#define LEPTONSF_H 1

#include <iostream>
#include "TFile.h"
#include "TH2F.h"
#include "Lepton.h"
#include "Functions.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"

const TString path_to_SF_histos = "$WDIR/AnalyserPAF/InputFiles/";

class LeptonSF {
 public:
  LeptonSF();
  ~LeptonSF() {}
  void loadHisto(Int_t iHisto, Int_t wp = -1);
  Float_t GetTrackerMuonSF(Float_t eta);
	Float_t GetLeptonSF(Float_t pt, Float_t ieta);
	Float_t GetLeptonSFerror(Float_t pt, Float_t ieta);
	TGraphAsymmErrors* LoadTrackerMuonSF(const char* file, const char* histo);
	Float_t p2(Float_t var);
  
  // Trigger SFs
  float GetDoubleMuSF(float eta1, float eta2) const { // binned in eta1, eta2
    eta1 = TMath::Abs(eta1);
    eta2 = TMath::Abs(eta2);
    return fDoubleMuSF->GetBinContent(fDoubleMuSF->FindBin(eta1, eta2));
    //return fDoubleMuSF->GetBinContent(fDoubleMuSF->FindBin(eta1, eta2)) - fDoubleMuSF->GetBinError(fDoubleMuSF->FindBin(eta1, eta2));
  }
  float GetDoubleElSF(float eta1, float eta2) const { // binned in eta1, eta2
    eta1 = TMath::Abs(eta1);
    eta2 = TMath::Abs(eta2);
    return fDoubleElSF->GetBinContent(fDoubleElSF->FindBin(eta1, eta2));
    //return fDoubleElSF->GetBinContent(fDoubleElSF->FindBin(eta1, eta2)) - fDoubleElSF->GetBinError(fDoubleElSF->FindBin(eta1, eta2));
  }
  float GetMuEGSF    (float eta1, float eta2) const { // binned in eta1, eta2
    eta1 = TMath::Abs(eta1);
    eta2 = TMath::Abs(eta2);
    return fMuEGSF->GetBinContent(fMuEGSF->FindBin(eta1, eta2) );
    //return fMuEGSF->GetBinContent(fMuEGSF->FindBin(eta1, eta2)) - fMuEGSF->GetBinError(fMuEGSF->FindBin(eta1, eta2));
  }
  
  // Trigger SF errors
  float GetDoubleMuSF_err(float eta1, float eta2) const { // binned in eta1, eta2
    return fDoubleMuSF->GetBinError(fDoubleMuSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
  }
  float GetDoubleElSF_err(float eta1, float eta2) const { // binned in eta1, eta2
    return fDoubleElSF->GetBinError(fDoubleElSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
  }
  float GetMuEGSF_err(float eta1, float eta2) const { // binned in eta1, eta2
    return fMuEGSF->GetBinError(fMuEGSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
  }
  
	std::vector<Int_t> loadedHistos;

 protected:
  TH2D* GetHistogramFromFileD(const char* file, const char* histo, const char* hname);
  TH2F* GetHistogramFromFileF(const char* file, const char* histo, const char* hname) const;
  
 private:
  // Muon SFs
  TGraphAsymmErrors*  fMuonTrackerSF;    
  TH2D*  fMuonIdSF;     
  TH2D*  fMuonIsoSF;     
  TH2D*  fMuonIP2DSF;     
  TH2D*  fMuonSIP3DSF;     
 
  // Elec SFs
  TH2D*  fElecTrackerSF;
  TH2D*  fElecIdSF;
  TH2D*  fElecIsoSF; 
  TH2D*  fElecIP2DSF;     
  TH2D*  fElecSIP3DSF;     

  // Trigger SFs
  TH2F *fDoubleMuSF;
  TH2F *fDoubleElSF;
  TH2F *fMuEGSF;
};
#endif
