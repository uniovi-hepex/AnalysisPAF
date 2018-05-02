#include "LeptonSF.h"

#include "PAF.h"

#include <iostream>

LeptonSF::LeptonSF(TString path, TString options):
  fMuonTrackerSF(0),  // Muon Reco
  fMuonIdSF_BCDEF(0), // Muon Id
  fMuonIdSF_GH(0),    // Muon Id
  fMuonIdSFSUSY(0),   // Muon Id
  fMuonIsoSF_BCDEF(0),// Muon Iso
  fMuonIsoSF_GH(0),   // Muon Iso
  fMuonIsoSFSUSY(0),  // Muon Iso
  fMuonIP2DSF(0),     // Muon IP2d
  fMuonSIP3DSF(0),    // Muon SIP
  fMuonFastSim(0),
  fMuonIsoFastSimStop(0),
  fMuonIdFastSimStop(0),
  fMuonlepMVA2lSSttH(0),    // Muon ttH LepMVA 2lSS
  fMuonlepMVA3l4lttH(0),    // Muon ttH LepMVA 3l, 4l
  fMuonLooseTracksttH(0),   // Muon ttH Loose Tracks
  fMuonLooseMiniIsottH(0),  // Muon ttH Loose MiniIso
  fMuonTightIP2DttH(0),     // Muon ttH Tight IP2D


  fElecTrackerSF(0),  // Electron Reco
  fElecIdSF(0),       // Electron Id (+Iso)
  fElecIsoSF(0),      // Electron Iso
  fElecIP2DSF(0),     // Electron IP2D
  fElecSIP3DSF(0),    // Electron SIP3D
  fElecFastSim(0),
  fElecIsoFastSimStop(0),
  fElecIdFastSimStop(0),
  fEleclepMVA2lSSttH(0),    // Elec ttH LepMVA 2lSS
  fEleclepMVA3l4lttH(0),    // Elec ttH LepMVA 3l, 4l
  fElecTightIP2DM17ttH(0),  // Elec ttH Tight IP2D
  fElecMini4M17ttH(0),      // Elec ttH Mini4 
  fElecConvVetoM17ttH(0),   // Elec ttH ConvVeto 

  fDoubleMuSF(0),     // Trigger Double Muon
  fDoubleElSF(0),     // Trigger Double Elec
  fMuEGSF(0){         // Trigger Elec-Muon
  path_to_SF_histos = path;
  if(options.Contains("2017")) gIs2017 = true; 
  else gIs2017 = false;
};





void LeptonSF::loadHisto(Int_t iHisto, Int_t wp){
  TString filename = ""; TString histoname = "";
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Muons 
  if(iHisto == iMuonReco){
    filename = "Tracking_EfficienciesAndSF_BCDEFGH"; histoname = "ratio_eff_eta3_dr030e030_corr";
    fMuonTrackerSF = LoadTrackerMuonSF(path_to_SF_histos + filename + ".root", histoname);
  }
  else if(iHisto == iMuonIdSUSY){
    if     (wp == iLoose){  filename = "SUS_MuonLooseIdM17"; histoname = "SF";}
    else if(wp == iMedium){ filename = "SUS_MuonMediumIdM17"; histoname = "SF";}
    else if(wp == iTight){  filename = ""; histoname = "";}
    fMuonIdSFSUSY = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSFSUSY"); 
  }
  else if(iHisto == iMuonId){
    if(wp == iLoose){
      filename = "MuonSFId_BCDEF"; histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_BCDEF"); 
      filename = "MuonSFId_GH"; histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_GH"); 
    }
    if(wp == iMedium){
      filename = "MuonSFId_BCDEF"; histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_BCDEF"); 
      filename = "MuonSFId_GH"; histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_GH"); 
    }
    if(wp == iTight){
      filename = "MuonSFId_BCDEF"; histoname = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_BCDEF"); 
      filename = "MuonSFId_GH"; histoname = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio";
      fMuonIdSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdSF_GH"); 
    }
  }
  else if(iHisto == iMuonIsoTightId){
    if(wp == iLoose){
      filename = "MuonSFIso_BCDEF"; histoname = "LooseISO_TightID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_BCDEF"); 
      filename = "MuonSFIso_GH"; histoname = "LooseISO_TightID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_GH"); 
    }
    if(wp == iTight){
      filename = "MuonSFIso_BCDEF"; histoname = "TightISO_TightID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_BCDEF"); 
      filename = "MuonSFIso_GH"; histoname = "TightISO_TightID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_GH"); 
    }
  }
  else if(iHisto == iMuonIsoMediumId){
    if(wp == iLoose){
      filename = "MuonSFIso_BCDEF"; histoname = "LooseISO_MediumID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_BCDEF"); 
      filename = "MuonSFIso_GH"; histoname = "LooseISO_MediumD_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_GH"); 
    }
    if(wp == iTight){
      filename = "MuonSFIso_BCDEF"; histoname = "TightISO_MediumID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_BCDEF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_BCDEF"); 
      filename = "MuonSFIso_GH"; histoname = "TightISO_MediumID_pt_eta/pt_abseta_ratio";
      fMuonIsoSF_GH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSF_GH"); 
    }
  }
  else if(iHisto == iMuonIsoSUSY){
    if     (wp == iLoose){  filename = ""; histoname = "";}
    else if(wp == iMedium){ filename = ""; histoname = "";}
    else if(wp == iTight){  filename = ""; histoname = "";}
    else if(wp == iVeryTight){  filename = "SUS_MuonVTMultiIsovMediumM17"; histoname = "SF";}
    else if(wp == iWPforStop){  filename = "SUS_MuonRelIso03b012"; histoname = "pt_abseta_ratio";}
    fMuonIsoSFSUSY = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoSFSUSY"); 
  }
  else if(iHisto == iMuonIP2D){
    if     (wp == iLoose){  filename = ""; histoname = "";}
    else if(wp == iTight){  filename = "SUS_MuonTIP2DvMediumM17"; histoname = "SF";}
    fMuonIP2DSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIP2DSF"); 
  }
  else if(iHisto == iMuonSIP3D){
    filename = "SUS_MuonSIPb4vMediumM17"; histoname = "SF";
    fMuonSIP3DSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonSIP3DSF"); 
  }
  else if(iHisto == iMuonFastSim){
    filename = "FullSimFastSim_Muon"; histoname = "SF";
    fMuonFastSim = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonFastSim"); 
  }
  else if(iHisto == iMuonIsoFastSimStop){
    filename = "Fast_vs_Full_mu_mediumID_iso012"; histoname = "histo2D";
    fMuonIsoFastSimStop = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIsoFastSimStop"); 
  }
  else if(iHisto == iMuonIdFastSimStop){
    filename = "Fast_vs_Full_mu_mediumID"; histoname = "histo2D";
    fMuonIdFastSimStop = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonIdFastSimSrop"); 
  }
  else if(iHisto == iMuonlepMVA2lSSttH){
    filename = "lepMVAEffSF_m_2lss"; histoname = "sf";
    fMuonlepMVA2lSSttH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonlepMVA2lSSttH");
  }
  else if(iHisto == iMuonlepMVA3l4lttH){
    filename = "lepMVAEffSF_m_3l"; histoname = "sf";
    fMuonlepMVA3l4lttH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonlepMVA3l4lttH");
  }
  else if(iHisto == iMuonLooseTracksttH){
    filename = "MuonSFTTH_TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta"; histoname = "SF";
    fMuonLooseTracksttH   = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonLooseTracksttH");
  }
  else if(iHisto == iMuonLooseMiniIsottH){
    filename = "MuonSFTTH_TnP_NUM_MiniIsoLoose_DENOM_LooseID_VAR_map_pt_eta"; histoname = "SF";
    fMuonLooseMiniIsottH  = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonLooseMiniIsottH");
  }
  else if(iHisto == iMuonTightIP2DttH){
    filename = "MuonSFTTH_TnP_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta"; histoname = "SF";
    fMuonTightIP2DttH     = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fMuonTightIP2DttH");
  }


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Electrons
  else if(iHisto == iElecReco){
    filename = "ElecRecoM17"; histoname = "EGamma_SF2D";
    fElecTrackerSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecTrackerSF"); 
  }
  else if(iHisto == iElecId){
    if     (gIs2017){
      if(wp == iTight){  filename = "ElecTightCBid94X";  histoname = "EGamma_SF2D";}
    }
    else{
      if     (wp == iVeto){   filename = "ElecVetoCBidM17";   histoname = "EGamma_SF2D";}
      else if(wp == iLoose){  filename = "ElecLooseCBidM17";  histoname = "EGamma_SF2D";}
      else if(wp == iMedium){ filename = "ElecMediumCBidM17"; histoname = "EGamma_SF2D";}
      else if(wp == iTight){  filename = "ElecTightCBidM17";  histoname = "EGamma_SF2D";}
    }
    fElecIdSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIdSF"); 
  }
  else if(iHisto == iElecIdSUSY){
    filename = "SUS_electonSFs"; 
    if(wp == iWPforStop) histoname = "GsfElectronToCutBasedSpring15T";
    fElecIdSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIdSF"); 
  }
  else if(iHisto == iElecIsoSUSY){
    filename = "SUS_electonSFs"; 
    if(wp == iWPforStop) histoname = "CutBasedStopsDileptonToRelIso012";
    fElecIsoSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIsoSF"); 
  }
  else if(iHisto == iElecIP2D){
    if     (wp == iLoose){  filename = ""; histoname = "";}
    else if(wp == iTight){  filename = ""; histoname = "";}
    fElecIP2DSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIP2DSF"); 
  }
  else if(iHisto == iElecSIP3D){
    filename = ""; histoname = "";
    fElecSIP3DSF = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecSIP3DSF"); 
  }
  else if(iHisto == iElecFastSim){
    filename = "FullSimFastSim_Elec"; histoname = "SF";
    fElecFastSim = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecFastSim"); 
  }
  else if(iHisto == iElecIsoFastSimStop){
    filename = "Fast_vs_Full_el_tightCB_iso012"; histoname = "histo2D";
    fElecIsoFastSimStop = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIsoFastSimStop"); 
  }
  else if(iHisto == iElecIdFastSimStop){
    filename = "Fast_vs_Full_el_tightCB"; histoname = "histo2D";
    fElecIdFastSimStop = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecIdFastSimStop"); 
  }
  else if(iHisto == iEleclepMVA2lSSttH){
    filename = "lepMVAEffSF_e_2lss"; histoname = "sf";
    fEleclepMVA2lSSttH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fEleclepMVA2lSSttH");
  }
  else if(iHisto == iEleclepMVA3l4lttH){
    filename = "lepMVAEffSF_e_3l"; histoname = "sf";
    fEleclepMVA3l4lttH    = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fEleclepMVA3l4lttH");
  }
  else if(iHisto == iElecTightIP2DM17ttH){
    filename = "ElecSFTTH_Moriond17"; histoname = "GsfElectronToMVAVLooseFOIDEmuTightIP2D";
    fElecTightIP2DM17ttH  = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecTightIP2DM17ttH");
  }
  else if(iHisto == iElecMini4M17ttH){    
    filename = "ElecSFTTH_Moriond17"; histoname = "MVAVLooseElectronToMini4";
    fElecMini4M17ttH      = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecMini4M17ttH");
  }
  else if(iHisto == iElecConvVetoM17ttH){
    filename = "ElecSFTTH_Moriond17"; histoname = "MVAVLooseElectronToConvVetoIHit1";
    fElecConvVetoM17ttH   = GetHistogramFromFileD(path_to_SF_histos + filename + ".root", histoname, "fElecConvVetoM17ttH");
  }


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Triggers
  else if(iHisto == iTrigDoubleMuon){
    //filename = "TriggerSF_mumu2016_pt"; histoname = "lepton_pt_2D_sf";
    filename = "triggerSummary_mumu"; histoname = "scalefactor_eta2d_with_syst";
    fDoubleMuSF = GetHistogramFromFileF(path_to_SF_histos + filename + ".root", histoname, "fDoubleMuSF"); 
  }
  else if(iHisto == iTrigDoubleElec){
    //filename = "TriggerSF_ee2016_pt"; histoname = "lepton_pt_2D_sf";
    filename = "triggerSummary_ee"; histoname = "scalefactor_eta2d_with_syst";
    fDoubleElSF = GetHistogramFromFileF(path_to_SF_histos + filename + ".root", histoname, "fDoubleElSF"); 
  }
  else if(iHisto == iTrigElMu){
    //filename = "TriggerSF_emu2016_pt"; histoname = "lepton_pt_2D_sf";
    filename = "triggerSummary_emu"; histoname = "scalefactor_eta2d_with_syst";
    fMuEGSF = GetHistogramFromFileF(path_to_SF_histos + filename + ".root", histoname, "fMuEGSF"); 
  }
  PAF_INFO("Lepton SF", Form("Loaded histogram %s from file %s%s.root", histoname.Data(), path_to_SF_histos.Data(), filename.Data()));
  loadedHistos.push_back(iHisto);
}

// Get global Lepton SF
Float_t LeptonSF::GetLeptonSF(Float_t pt, Float_t ieta, Int_t type){
  Float_t eta = TMath::Abs(ieta);
  Int_t nSFs = loadedHistos.size();
  Float_t SF = 1; Int_t id; Float_t pr = 1;
  //if(type == 1) cout << "================================= new Elec! [pt,eta] = [" << pt << "," << eta << "]..." << endl;
  //if(type == 0) cout << "================================= new Muon! [pt,eta] = [" << pt << "," << eta << "]..." << endl;
  if(pt <= 20) pt = 20.01;
  for(Int_t i = 0; i < nSFs; i++){
    pr = 1; 
    id = loadedHistos[i];
    if(type == 0){
      if(pt > 120) pt = 119;
      if (((id == iMuonlepMVA2lSSttH) || (id == iMuonlepMVA3l4lttH)) && (pt >= 100)) pt = 99;
      if     (id == iMuonReco)    pr = GetTrackerMuonSF(eta); 
      else if(id == iMuonIdSUSY)  pr = fMuonIdSFSUSY      ->GetBinContent(fMuonIdSFSUSY     ->FindBin(pt,eta));
      else if(id == iMuonId)      pr = (fMuonIdSF_BCDEF->GetBinContent(fMuonIdSF_BCDEF->FindBin(pt,eta))*lumiBCDEF + fMuonIdSF_GH->GetBinContent(fMuonIdSF_GH->FindBin(pt,eta))*lumiGH)/(lumiBCDEF+lumiGH);
      else if(id == iMuonIsoSUSY) pr = fMuonIsoSFSUSY     ->GetBinContent(fMuonIsoSFSUSY    ->FindBin(pt,eta));
      else if(id == iMuonIsoMediumId || id == iMuonIsoTightId)
        pr = (fMuonIsoSF_BCDEF->GetBinContent(fMuonIsoSF_BCDEF->FindBin(pt,eta))*lumiBCDEF + fMuonIsoSF_GH->GetBinContent(fMuonIsoSF_GH->FindBin(pt,eta))*lumiGH)/(lumiBCDEF+lumiGH);
      else if(id == iMuonIP2D)    pr = fMuonIP2DSF    ->GetBinContent(fMuonIP2DSF   ->FindBin(pt,eta));
      else if(id == iMuonSIP3D)   pr = fMuonSIP3DSF   ->GetBinContent(fMuonSIP3DSF  ->FindBin(pt,eta));
      else if(id == iMuonIdFastSimStop)   pr = fMuonIdFastSimStop    ->GetBinContent(fMuonIdFastSimStop  ->FindBin(pt,eta));
      else if(id == iMuonIsoFastSimStop)  pr = fMuonIsoFastSimStop   ->GetBinContent(fMuonIsoFastSimStop  ->FindBin(pt,eta));
      else if(id == iMuonlepMVA2lSSttH)   pr = fMuonlepMVA2lSSttH   ->GetBinContent(fMuonlepMVA2lSSttH  ->FindBin(pt,eta));
      else if(id == iMuonlepMVA3l4lttH)   pr = fMuonlepMVA3l4lttH   ->GetBinContent(fMuonlepMVA3l4lttH  ->FindBin(pt,eta));
      else if(id == iMuonLooseTracksttH)  pr = fMuonLooseTracksttH  ->GetBinContent(fMuonLooseTracksttH ->FindBin(pt,eta));
      else if(id == iMuonLooseMiniIsottH) pr = fMuonLooseMiniIsottH ->GetBinContent(fMuonLooseMiniIsottH->FindBin(pt,eta));
      else if(id == iMuonTightIP2DttH)    pr = fMuonTightIP2DttH    ->GetBinContent(fMuonTightIP2DttH   ->FindBin(pt,eta));
    }
    else if(type == 1){ 
      if(pt > 200) pt = 199;
      if (((id == iEleclepMVA2lSSttH) || (id == iEleclepMVA3l4lttH)) && (pt >= 100)) pt = 99;
      if(id == iElecReco)    pr = fElecTrackerSF ->GetBinContent(fElecTrackerSF->FindBin(eta,50));
      else if(id == iElecIdSUSY)      pr = fElecIdSF      ->GetBinContent(fElecIdSF     ->FindBin(pt,eta));
      else if(id == iElecIsoSUSY) pr = fElecIsoSF     ->GetBinContent(fElecIsoSF    ->FindBin(pt,eta));
      else if(id == iElecId)      pr = fElecIdSF      ->GetBinContent(fElecIdSF     ->FindBin(eta,pt));
      else if(id == iElecIP2D)    pr = fElecIP2DSF    ->GetBinContent(fElecIP2DSF   ->FindBin(eta,pt));
      else if(id == iElecSIP3D)   pr = fElecSIP3DSF   ->GetBinContent(fElecSIP3DSF  ->FindBin(eta,pt));
      else if(id == iElecIsoFastSimStop)   pr = fElecIsoFastSimStop ->GetBinContent(fElecIsoFastSimStop  ->FindBin(pt, eta));
      else if(id == iElecIdFastSimStop)   pr = fElecIdFastSimStop ->GetBinContent(fElecIdFastSimStop  ->FindBin(pt, eta));
      else if(id == iEleclepMVA2lSSttH)   pr = fEleclepMVA2lSSttH    ->GetBinContent(fEleclepMVA2lSSttH    ->FindBin(pt,eta));
      else if(id == iEleclepMVA3l4lttH)   pr = fEleclepMVA3l4lttH    ->GetBinContent(fEleclepMVA3l4lttH    ->FindBin(pt,eta));
      else if(id == iElecTightIP2DM17ttH) pr = fElecTightIP2DM17ttH  ->GetBinContent(fElecTightIP2DM17ttH  ->FindBin(pt,eta));
      else if(id == iElecMini4M17ttH)     pr = fElecMini4M17ttH      ->GetBinContent(fElecMini4M17ttH      ->FindBin(pt,eta));
      else if(id == iElecConvVetoM17ttH)  pr = fElecConvVetoM17ttH   ->GetBinContent(fElecConvVetoM17ttH   ->FindBin(pt,eta));
    }
    SF *= pr;
    //cout << "   " << SFTString[id] << ", SF = " << pr << ", total SF = " << SF << endl;
  }
  return SF;
}

// Get global error for Lepton SF
Float_t LeptonSF::GetLeptonSFerror(Float_t pt, Float_t ieta, Int_t type){
  Float_t eta = TMath::Abs(ieta);
  float t = 0;
  Int_t nSFs = loadedHistos.size();
  Float_t err = 0; Int_t id;
  if(pt <= 20) pt = 20.01;
  for(Int_t i = 0; i < nSFs; i++){
    id = loadedHistos[i];
    if(type == 0){
      if(pt > 120) pt = 119;
      if (((id == iMuonlepMVA2lSSttH) || (id == iMuonlepMVA3l4lttH)) && (pt >= 100)) pt = 99;
      if     (id == iMuonReco)    err += 0;
      else if(id == iMuonIdSUSY)  err += p2(fMuonIdSFSUSY      ->GetBinError(fMuonIdSFSUSY     ->FindBin(pt,eta)));
      else if(id == iMuonId)      err += p2( (fMuonIdSF_BCDEF->GetBinError(fMuonIdSF_BCDEF->FindBin(pt,eta))*lumiBCDEF + fMuonIdSF_GH->GetBinError(fMuonIdSF_GH->FindBin(pt,eta))*lumiGH)/(lumiBCDEF+lumiGH) );
      else if(id == iMuonIsoSUSY) err += p2(fMuonIsoSFSUSY ->GetBinError(fMuonIsoSFSUSY->FindBin(pt,eta)));
      else if(id == iMuonIsoMediumId || id == iMuonIsoTightId)
        err += p2( (fMuonIsoSF_BCDEF->GetBinError(fMuonIsoSF_BCDEF->FindBin(pt,eta))*lumiBCDEF + fMuonIsoSF_GH->GetBinError(fMuonIsoSF_GH->FindBin(pt,eta))*lumiGH)/(lumiBCDEF+lumiGH) );
      else if(id == iMuonIP2D)    err += p2(fMuonIP2DSF    ->GetBinError(fMuonIP2DSF   ->FindBin(pt,eta)));
      else if(id == iMuonSIP3D)   err += p2(fMuonSIP3DSF   ->GetBinError(fMuonSIP3DSF  ->FindBin(pt,eta)));
      else if(id == iMuonIdFastSimStop)   err += p2(fMuonIdFastSimStop   ->GetBinError(fMuonIdFastSimStop  ->FindBin(pt,eta)));
      else if(id == iMuonIsoFastSimStop)  err += p2(fMuonIsoFastSimStop   ->GetBinError(fMuonIsoFastSimStop  ->FindBin(pt,eta)));
      else if(id == iMuonlepMVA2lSSttH)   err += p2(fMuonlepMVA2lSSttH    ->GetBinError(fMuonlepMVA2lSSttH  ->FindBin(pt,eta)));
      else if(id == iMuonlepMVA3l4lttH)   err += p2(fMuonlepMVA3l4lttH    ->GetBinError(fMuonlepMVA3l4lttH  ->FindBin(pt,eta)));
      else if(id == iMuonLooseTracksttH)  err += p2(fMuonLooseTracksttH   ->GetBinError(fMuonLooseTracksttH ->FindBin(pt,eta)));
      else if(id == iMuonLooseMiniIsottH) err += p2(fMuonLooseMiniIsottH  ->GetBinError(fMuonLooseMiniIsottH->FindBin(pt,eta)));
      else if(id == iMuonTightIP2DttH)    err += p2(fMuonTightIP2DttH     ->GetBinError(fMuonTightIP2DttH   ->FindBin(pt,eta)));
    }
    else if(type == 1){ 
      if(pt > 200) pt = 199;
      if (((id == iEleclepMVA2lSSttH) || (id == iEleclepMVA3l4lttH)) && (pt >= 100)) pt = 99;
      if(id == iElecReco)    err += p2(fElecTrackerSF ->GetBinError(fElecTrackerSF->FindBin(eta,50)));
      else if(id == iElecIdSUSY)      err += p2(fElecIdSF      ->GetBinError(fElecIdSF     ->FindBin(pt,eta)));
      else if(id == iElecIsoSUSY) err += p2(fElecIsoSF     ->GetBinError(fElecIsoSF    ->FindBin(pt,eta)));
      else if(id == iElecId)      err += p2(fElecIdSF      ->GetBinError(fElecIdSF     ->FindBin(eta,pt)));
      else if(id == iElecIP2D)    err += p2(fElecIP2DSF    ->GetBinError(fElecIP2DSF   ->FindBin(eta,pt)));
      else if(id == iElecSIP3D)   err += p2(fElecSIP3DSF   ->GetBinError(fElecSIP3DSF  ->FindBin(eta,pt)));
      else if(id == iElecIsoFastSimStop)   err += p2(fElecIsoFastSimStop ->GetBinError(fElecIsoFastSimStop  ->FindBin(pt, eta)));
      else if(id == iElecIdFastSimStop)   err += p2(fElecIdFastSimStop ->GetBinError(fElecIdFastSimStop  ->FindBin(pt, eta)));
      else if(id == iEleclepMVA2lSSttH)   err += p2(fEleclepMVA2lSSttH    ->GetBinError(fEleclepMVA2lSSttH  ->FindBin(pt,eta)));
      else if(id == iEleclepMVA3l4lttH)   err += p2(fEleclepMVA3l4lttH    ->GetBinError(fEleclepMVA3l4lttH  ->FindBin(pt,eta)));
      else if(id == iElecTightIP2DM17ttH) err += p2(fElecTightIP2DM17ttH  ->GetBinError(fElecTightIP2DM17ttH->FindBin(pt,eta)));
      else if(id == iElecMini4M17ttH)     err += p2(fElecMini4M17ttH      ->GetBinError(fElecMini4M17ttH    ->FindBin(pt,eta)));
      else if(id == iElecConvVetoM17ttH)  err += p2(fElecConvVetoM17ttH   ->GetBinError(fElecConvVetoM17ttH ->FindBin(pt,eta)));
    }
  }
  return TMath::Sqrt(err);
}

// Functions to load the histograms

TH2D* LeptonSF::GetHistogramFromFileD(const char* filename, const char* histoname, const char* newhname) {
  TFile* file  = TFile::Open(filename);
  if (!file) {
    PAF_FATAL("LeptonSF", Form("Could not load file\"%s\"", filename));
    return 0;
  }
  TH2D* h = (TH2D*) file->Get(histoname)->Clone(newhname);
  if (!h) {
    PAF_ERROR("LeptonSF", Form("Could not find histogram \"%s\"", histoname));
    return 0;
  }
  h->SetDirectory(0);
  file->Close();
  return h;
}

TH2F* LeptonSF::GetHistogramFromFileF(const char* filename, const char* histoname, const char* newhname) const {
  TFile* file  = TFile::Open(filename);
  if (!file) {
    PAF_FATAL("LeptonSF", Form("Could not load file\"%s\"", filename));
    return 0;
  }
  TH2F* h = (TH2F*) file->Get(histoname)->Clone(newhname);
  if (!h) {
    PAF_ERROR("LeptonSF", Form("Could not find histogram \"%s\"", histoname));
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
Float_t LeptonSF::GetTrigElMuSF(Float_t eta1, Float_t eta2) const { // binned in eta1, eta2
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

///////////////////////////////////////////////////
// FullSim/FastSim SF
///////////////////////////////////////////////////
Float_t LeptonSF::GetFSSF(Float_t pt, Float_t eta, Int_t id){
  Float_t SF = 1;
  if(pt >= 200) pt = 199; eta = TMath::Abs(eta); id  = TMath::Abs(id);
  if(id == 13) SF = fMuonFastSim   ->GetBinContent(fMuonFastSim  ->FindBin(pt,eta));
  else         SF = fElecFastSim ->GetBinContent(fElecFastSim  ->FindBin(pt, eta));
  return SF;
}

Float_t LeptonSF::GetFSSFerr(Float_t pt, Float_t eta, Int_t id){
  Float_t SFerr = 0;
  if(pt >= 200) pt = 199; eta = TMath::Abs(eta); id  = TMath::Abs(id);
  if(id == 13) SFerr = fMuonFastSim   ->GetBinError(fMuonFastSim  ->FindBin(pt,eta));
  else         SFerr = fElecFastSim ->GetBinError(fElecFastSim  ->FindBin(pt, eta));
  return SFerr;
}
