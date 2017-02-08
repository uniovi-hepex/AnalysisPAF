#include "LeptonSF.h"

#include <iostream>

LeptonSF::LeptonSF():
	fMuonTrackerSF(0),  // Muon Reco
	fMuonIdSF(0),       // Muon Id
	fMuonIsoSF(0),      // Muon Iso
	fMuonIP2DSF(0),     // Muon IP2d
	fMuonSIP3DSF(0),    // Muon SIP

	fElecTrackerSF(0),  // Electron Reco
	fElecIdSF(0),       // Electron Id (+Iso)
	fElecIsoSF(0),      // Electron Iso
	fElecIP2DSF(0),     // Electron IP2D
	fElecSIP3DSF(0),    // Electron SIP3D

	fDoubleMuSF(0),     // Trigger Double Muon
	fDoubleElSF(0),     // Trigger Double Elec
	fMuEGSF(0){         // Trigger Elec-Muon
};



void LeptonSF::loadHisto(Int_t iHisto, Int_t wp){
  TString filename = ""; TString histoname = "";
	switch(iHisto){
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Muons 
		case iMuonReco:
      filename = ""; histoname = "";
			fMuonTrackerSF = LoadTrackerMuonSF(filename, histoname);
		case iMuonId:
      if     (wp == iLoose){  filename = "SUS_MuonLooseIdM17"; histoname = "SF";}
      else if(wp == iMedium){ filename = "SUS_MuonMediumIdM17"; histoname = "SF";}
      else if(wp == iTight){  filename = ""; histoname = "";}
			fMuonIdSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fMuonIdSF"); 
		case iMuonIso:
      if     (wp == iLoose){  filename = ""; histoname = "";}
      else if(wp == iMedium){ filename = ""; histoname = "";}
      else if(wp == iTight){  filename = ""; histoname = "";}
      else if(wp == iVeryTight){  filename = "SUS_MuonVTMultiIsovMediumM17"; histoname = "SF";}
			fMuonIsoSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fMuonIsoSF"); 
		case iMuonIP2D:
      if     (wp == iLoose){  filename = ""; histoname = "";}
      else if(wp == iTight){  filename = "SUS_MuonTIP2DvMediumM17"; histoname = "SF";}
			fMuonIP2DSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fMuonIP2DSF"); 
		case iMuonSIP3D:
      filename = "SUS_MuonSIPb4vMediumM17"; histoname = "SF";
			fMuonSIP3DSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fMuonSIP3DSF"); 
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Electrons
		case iElecReco:
      filename = "ElecRecoM17"; histoname = "EGamma_SF2D";
			fElecTrackerSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fElecTrackerSF"); 
		case iElecId:
      if     (wp == iVeto){   filename = "ElecVetoCBidM17";   histoname = "EGamma_SF2D";}
      else if(wp == iLoose){  filename = "ElecLooseCBidM17";  histoname = "EGamma_SF2D";}
      else if(wp == iMedium){ filename = "ElecMediumCBidM17"; histoname = "EGamma_SF2D";}
      else if(wp == iTight){  filename = "ElecTightCBidM17";  histoname = "EGamma_SF2D";}
      filename = ""; histoname = "";
			fElecIdSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fElecIdSF"); 
		case iElecIso:
      filename = ""; histoname = "";
			fElecIsoSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fElecIsoSF"); 
		case iElecIP2D:
      if     (wp == iLoose){  filename = ""; histoname = "";}
      else if(wp == iTight){  filename = ""; histoname = "";}
			fElecIP2DSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fElecIP2DSF"); 
		case iElecSIP3D:
      filename = ""; histoname = "";
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Triggers
			fElecSIP3DSF = GetHistogramFromFileD(path_to_SF_histos + filename, histoname, "fElecSIP3DSF"); 
		case iTrigDoubleMuon:
      filename = ""; histoname = "";
			fDoubleMuSF = GetHistogramFromFileF(path_to_SF_histos + filename, histoname, "fDoubleMuSF"); 
		case iTrigDoubleElec:
      filename = ""; histoname = "";
			fDoubleElSF = GetHistogramFromFileF(path_to_SF_histos + filename, histoname, "fDoubleElSF"); 
		case iTrigElMu:
      filename = ""; histoname = "";
			fMuEGSF = GetHistogramFromFileF(path_to_SF_histos + filename, histoname, "fMuEGSF"); 
	}
   
  loadedHistos.push_back(iHisto);
}

// Get global Lepton SF
Float_t LeptonSF::GetLeptonSF(Float_t pt, Float_t ieta){
	Float_t eta = TMath::Abs(ieta);
  if(pt < 200) pt = 199;
	Int_t nSFs = loadedHistos.size();
	Float_t SF = 1;
	for(Int_t i = 0; i < nSFs; i++){
		switch(i){
			case iMuonReco:  SF *= GetTrackerMuonSF(eta); 
			case iMuonId:    SF *= fMuonIdSF      ->GetBinContent(fMuonIdSF     ->FindBin(eta,pt));
			case iMuonIso:   SF *= fMuonIsoSF     ->GetBinContent(fMuonIsoSF    ->FindBin(eta,pt));
			case iMuonIP2D:  SF *= fMuonIP2DSF    ->GetBinContent(fMuonIP2DSF   ->FindBin(eta,pt));
			case iMuonSIP3D: SF *= fMuonSIP3DSF   ->GetBinContent(fMuonSIP3DSF  ->FindBin(eta,pt));
			case iElecReco:  SF *= fElecTrackerSF ->GetBinContent(fElecTrackerSF->FindBin(eta,50));
			case iElecId:    SF *= fElecIdSF      ->GetBinContent(fElecIdSF     ->FindBin(eta,pt));
			case iElecIso:   SF *= fElecIsoSF     ->GetBinContent(fElecIsoSF    ->FindBin(eta,pt));
			case iElecIP2D:  SF *= fElecIP2DSF    ->GetBinContent(fElecIP2DSF   ->FindBin(eta,pt));
			case iElecSIP3D: SF *= fElecSIP3DSF   ->GetBinContent(fElecSIP3DSF  ->FindBin(eta,pt));
		} 
	}
  return SF;
}

// Get global error for Lepton SF
Float_t LeptonSF::GetLeptonSFerror(Float_t pt, Float_t ieta){
	Float_t eta = TMath::Abs(ieta);
  if(pt < 200) pt = 199;
	Int_t nSFs = loadedHistos.size();
	Float_t err = 0;
	for(Int_t i = 0; i < nSFs; i++){
		switch(i){
			case iMuonReco:  err += 0;
			case iMuonId:    err += p2(fMuonIdSF      ->GetBinError(fMuonIdSF     ->FindBin(eta,pt)));
			case iMuonIso:   err += p2(fMuonIsoSF     ->GetBinError(fMuonIsoSF    ->FindBin(eta,pt)));
			case iMuonIP2D:  err += p2(fMuonIP2DSF    ->GetBinError(fMuonIP2DSF   ->FindBin(eta,pt)));
			case iMuonSIP3D: err += p2(fMuonSIP3DSF   ->GetBinError(fMuonSIP3DSF  ->FindBin(eta,pt)));
			case iElecReco:  err += p2(fElecTrackerSF ->GetBinError(fElecTrackerSF->FindBin(eta,pt)));
			case iElecId:    err += p2(fElecIdSF      ->GetBinError(fElecIdSF     ->FindBin(eta,pt)));
			case iElecIso:   err += p2(fElecIsoSF     ->GetBinError(fElecIsoSF    ->FindBin(eta,pt)));
			case iElecIP2D:  err += p2(fElecIP2DSF    ->GetBinError(fElecIP2DSF   ->FindBin(eta,pt)));
			case iElecSIP3D: err += p2(fElecSIP3DSF   ->GetBinError(fElecSIP3DSF  ->FindBin(eta,pt)));
		} 
	}
  return TMath::Sqrt(err);
}

// Functions to load the histograms

TH2D* LeptonSF::GetHistogramFromFileD(const char* filename, const char* histoname, const char* newhname) {
  TFile* file  = TFile::Open(filename);
  if (!file) {
    std::cerr << "ERROR[LeptonSF]: Could not load file" << std::endl
	  << "     " << filename << std::endl;
    return 0;
  }
  TH2D* h = (TH2D*) file->Get(histoname)->Clone(newhname);
  if (!h) {
    std::cerr << "ERROR[LeptonSF]: Could not find histogram " 
	      << histoname << std::endl;
    return 0;
  }
  h->SetDirectory(0);
  file->Close();
  return h;
}

TH2F* LeptonSF::GetHistogramFromFileF(const char* filename, const char* histoname, const char* newhname) const {
  TFile* file  = TFile::Open(filename);
  if (!file) {
      std::cerr << "ERROR[LeptonSF]: Could not load file" << std::endl
		<< "                 " << filename << std::endl;
      return 0;
  }
  TH2F* h = (TH2F*) file->Get(histoname)->Clone(newhname);
  if (!h) {
    std::cerr << "ERROR[LeptonSF]: Could not find histogram " 
	      << histoname << std::endl;
    return 0;
    }
  h->SetDirectory(0);
  file->Close();
 
  return h;
}

// -----------------------------------------
TGraphAsymmErrors* LeptonSF::LoadTrackerMuonSF(const char* file, const char* histo){
 TFile *f = TFile::Open(file);
 f->GetObject(histo, fMuonTrackerSF);
 return fMuonTrackerSF;
}

Float_t LeptonSF::GetTrackerMuonSF(Float_t eta){
	Float_t val = 0; Float_t xlow = 0; Float_t xhigh = 0;
	for(Int_t i = 0; i < fMuonTrackerSF->GetN(); i++){
		xlow  = fMuonTrackerSF->GetX()[i] - fMuonTrackerSF->GetErrorXlow(i);
		xhigh = fMuonTrackerSF->GetX()[i] + fMuonTrackerSF->GetErrorXhigh(i);
		if(xlow <= eta && xhigh > eta) val = fMuonTrackerSF->GetY()[i];
	}
	return val;
}

///////////////////////////////////////////////////
// Trigger SFs
///////////////////////////////////////////////////

Float_t LeptonSF::GetTrigDoubleMuSF(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	eta1 = TMath::Abs(eta1);
	eta2 = TMath::Abs(eta2);
	return fDoubleMuSF->GetBinContent(fDoubleMuSF->FindBin(eta1, eta2));
}
Float_t LeptonSF::GetTrigDoubleElSF(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	eta1 = TMath::Abs(eta1);
	eta2 = TMath::Abs(eta2);
	return fDoubleElSF->GetBinContent(fDoubleElSF->FindBin(eta1, eta2));
}
Float_t LeptonSF::GetTrigElMuGSF(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	eta1 = TMath::Abs(eta1);
	eta2 = TMath::Abs(eta2);
	return fMuEGSF->GetBinContent(fMuEGSF->FindBin(eta1, eta2) );
}

// Trigger SF errors
Float_t LeptonSF::GetTrigDoubleMuSF_err(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	return fDoubleMuSF->GetBinError(fDoubleMuSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
}
Float_t LeptonSF::GetTrigDoubleElSF_err(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	return fDoubleElSF->GetBinError(fDoubleElSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
}
Float_t LeptonSF::GetTrigElMuSF_err(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
	return fMuEGSF->GetBinError(fMuEGSF->FindBin(TMath::Abs(eta1),TMath::Abs(eta2)));
}
