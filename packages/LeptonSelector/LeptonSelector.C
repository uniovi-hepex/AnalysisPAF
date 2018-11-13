///////////////////////////////////////////////////////////////////////////////////////////
//
//  Lepton Selector: create vectors with selected leptons, veto (or fakeable) leptons...
//
//  All SFs and variables are within the Lepton definition
//
//  To do: definition of veto leptons
//
//
///////////////////////////////////////////////////////////////////////////////////////////


#include "LeptonSelector.h"

ClassImp(LeptonSelector);
LeptonSelector::LeptonSelector() : PAFChainItemSelector() {}
void LeptonSelector::Summary(){}

void LeptonSelector::Initialise(){
  // Initialise LeptonSelector
  gIsData        = GetParam<Bool_t>("IsData");
  gIsFastSim     = GetParam<Bool_t>("IsFastSim");
  gSelection     = GetParam<Int_t>("iSelection");
  gOptions       = GetParam<TString>("_options");
  gIs2017 = false;
  if(gOptions.Contains("2017")) gIs2017 = true;
  localPath      = GetParam<TString>("WorkingDir");
  LepSF = new LeptonSF(localPath + "/InputFiles/", gOptions);
  ElecScale = new ElecScaleClass(localPath + "/InputFiles/ElecScale.dat");

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // Select SFs for each analysis !!!!!!
  if(gSelection == iStopSelec){
    LepSF->loadHisto(iTrigDoubleMuon);
    LepSF->loadHisto(iTrigDoubleElec);
    LepSF->loadHisto(iTrigElMu);
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonIdSUSY,   iMedium);
    LepSF->loadHisto(iMuonIsoSUSY,  iWPforStop);
    LepSF->loadHisto(iElecIdSUSY,   iWPforStop);
    LepSF->loadHisto(iElecIsoSUSY,   iWPforStop);
    if(gIsFastSim){
      LepSF->loadHisto(iElecIdFastSimStop);
      LepSF->loadHisto(iElecIsoFastSimStop);
      LepSF->loadHisto(iMuonIdFastSimStop);
      LepSF->loadHisto(iMuonIsoFastSimStop);
    }
  }
  else if(gSelection == ittHSelec){
    LepSF->loadHisto(iMuonlepMVA2lSSttH);
    LepSF->loadHisto(iEleclepMVA2lSSttH);
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iMuonLooseTracksttH);
    LepSF->loadHisto(iMuonLooseMiniIsottH);
    LepSF->loadHisto(iMuonTightIP2DttH);
    LepSF->loadHisto(iElecTightIP2DM17ttH);
    LepSF->loadHisto(iElecMini4M17ttH);
    LepSF->loadHisto(iElecConvVetoM17ttH);
  }
  else if(gSelection == i4tSelec || gSelection == iWZSelec){
    // Los tenemos en funciones, no en histogramas, entonces nos la pela de cargar histogramas
  }

  else if(gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iTWTTbarSelec || gSelection == iWWSelec){
    LepSF->loadHisto(iTrigDoubleMuon);
    LepSF->loadHisto(iTrigDoubleElec);
    LepSF->loadHisto(iTrigElMu);
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonId,   iTight);
    LepSF->loadHisto(iMuonIsoTightId,   iTight);
    LepSF->loadHisto(iElecId,   iTight);
  }
  else if(gSelection == iStopTopSelec){
    LepSF->loadHisto(iTrigDoubleMuon);
    LepSF->loadHisto(iTrigDoubleElec);
    LepSF->loadHisto(iTrigElMu);
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonId,   iTight);
    LepSF->loadHisto(iMuonIsoTightId,   iTight);
    LepSF->loadHisto(iElecId,   iTight);
    if(gIsFastSim){
      LepSF->loadHisto(iElecFastSim);
      LepSF->loadHisto(iMuonFastSim);
    }
  }
  else std::cout << ">>>>>>>>>>>> WRONG SELECTION!!!!!!!!" << std::endl;
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  selLeptons   = std::vector<Lepton>();
  vetoLeptons   = std::vector<Lepton>();
  looseLeptons   = std::vector<Lepton>();
}

//################################################################
//## Definition of wps...
//################################################################
Bool_t LeptonSelector::getSIPcut(Float_t cut){
  if(sip > cut) return false;
  return true;
}

Bool_t LeptonSelector::getGoodVertex(Int_t wp){
  if (gSelection == ittHSelec || gSelection == i4tSelec || gSelection == iWZSelec) {
    if (wp == iTight || wp == iMedium || wp == iLoose){
      DumpVar(evt, "dxy", dxy, dxy < 0.05);
      DumpVar(evt, "dz",  dxy, dz  < 0.1);
      if (dxy >= 0.05 || dz >= 0.1) return false;
    }
  } else {
    if(type == 1){ //electrons
      if(wp == iTight){
        if(etaSC <= 1.479 && ((dxy >= 0.05) || (dz  >= 0.10))) return false;
        if(etaSC >  1.479 && ((dxy >= 0.10) || (dz  >= 0.20))) return false;
      }
    }
    else{ // muons
      if(wp == iMedium && (dxy > 0.2  || dz > 0.5)) return false;
      if(wp == iTight  && (dxy > 0.05 || dz > 0.1)) return false;
    }
  }
  return true;
}

Bool_t LeptonSelector::getRelIso03POG(Int_t wp){
  if(type == 1){ // electrons
    if(wp == iWPforStop && RelIso03 > 0.12) return false;
    if(etaSC <= 1.479){
      if(wp == iVeto   && RelIso03 > 0.1750) return false;
      if(wp == iLoose  && RelIso03 > 0.0994) return false;
      if(wp == iMedium && RelIso03 > 0.0695) return false;
      if(wp == iTight  && RelIso03 > 0.0588) return false;
    }
    else if(etaSC > 1.479){
      if(wp == iVeto   && RelIso03 > 0.1590) return false;
      if(wp == iLoose  && RelIso03 > 0.1070) return false;
      if(wp == iMedium && RelIso03 > 0.0821) return false;
      if(wp == iTight  && RelIso03 > 0.0571) return false;
    }
  }
  else{ // muons
    if(wp == iLoose  && RelIso03 > 0.10) return false; 
    if(wp == iTight  && RelIso03 > 0.05) return false; 
    if(wp == iWPforStop && RelIso03 > 0.12) return false; 
    if(wp == iLooseWPforStop && RelIso03 > 0.4) return false; 
  }
  return true;
}

Bool_t LeptonSelector::getRelIso04POG(Int_t wp){ // wps for muons
  if(type == 1) return false; // electrons
  if(wp == iLoose  && RelIso04 > 0.25) return false;
  if(wp == iTight  && RelIso04 > 0.15) return false;
  return true;
}

Bool_t LeptonSelector::getminiRelIso(Int_t wp) {
  if (wp == iTight || wp == iMedium || wp == iLoose) {
    if (miniIso > 0.4) return false;
  }
  return true;
}

Bool_t LeptonSelector::getMuonId(Int_t wp){
  if(wp == iTight   && !tightVar)     return false;
  if(wp == iMedium  && !mediumMuonId) return false;
  return true;
}

Bool_t LeptonSelector::getElecMVA(Int_t wp){
  Float_t point = 0; Float_t aeta = TMath::Abs(etaSC);
  if(wp == iVeryLoose){
    if(aeta < 0.8){
      if     (pt > 10 && pt < 15) point = -0.86;
      else if(pt > 15 && pt < 25) point = -0.86 + (-0.96+0.86)/10*(pt-15);
      else if(pt > 25           ) point = -0.96;
    }
    else if(aeta < 1.479){
      if     (pt > 10 && pt < 15) point = -0.85;
      else if(pt > 15 && pt < 25) point = -0.85 + (-0.96+0.85)/10*(pt-15);
      else if(pt > 25           ) point = -0.96;
    }
    else if(aeta < 2.5){
      if     (pt > 10 && pt < 15) point = -0.81;
      else if(pt > 15 && pt < 25) point = -0.81 + (-0.95+0.81)/10*(pt-15);
      else if(pt > 25           ) point = -0.95;
    }
  }
  if(wp == iLoose){
    if(aeta < 0.8){
      if     (pt > 10 && pt < 15) point = -0.48;
      else if(pt > 15 && pt < 25) point = -0.48 + (-0.85+0.48)/10*(pt-15);
      else if(pt > 25           ) point = -0.85;
    }
    else if(aeta < 1.479){
      if     (pt > 10 && pt < 15) point = -0.67;
      else if(pt > 15 && pt < 25) point = -0.67 + (-0.91+0.67)/10*(pt-15);
      else if(pt > 25           ) point = -0.91;
    }
    else if(aeta < 2.5){
      if     (pt > 10 && pt < 15) point = -0.49;
      else if(pt > 15 && pt < 25) point = -0.49 + (-0.83+0.49)/10*(pt-15);
      else if(pt > 25           ) point = -0.83;
    }
  }
 
  else if(wp == iTight){
    if(aeta < 0.8){
      if     (pt > 10 && pt < 15) point = 0.77;
      else if(pt > 15 && pt < 25) point = 0.77 + (0.52-0.77)/10*(pt-15);
      else if(pt > 25           ) point = 0.52;
    }
    else if(aeta < 1.479){
      if     (pt > 10 && pt < 15) point = 0.56;
      else if(pt > 15 && pt < 25) point = 0.56 + (0.11-0.56)/10*(pt-15);
      else if(pt > 25           ) point = 0.11;
    }
    else if(aeta < 2.5){
      if     (pt > 10 && pt < 15) point = 0.48;
      else if(pt > 15 && pt < 25) point = 0.48 + (-0.01-0.48)/10*(pt-15);
      else if(pt > 25           ) point = -0.01;
    }
  }
  return ( MVAID > point); 
}

Bool_t LeptonSelector::getElecMVAId(Int_t wp, Lepton lep) {
  if (wp == iTight) {
    Float_t   A = -0.86+(-0.85 + 0.86)*(abs(eta) > 0.8)+(-0.81 + 0.86)*(abs(eta) > 1.479);
    Float_t  B = -0.96+(-0.96 + 0.96)*(abs(eta) > 0.8)+(-0.95 + 0.96)*(abs(eta) > 1.479);
    if (pt > 10) {
      if (!(MVAID > min( A , max( B , A+(B-A)/10*(pt-15))) )) return false;
    }
    if (abs(eta) < 0.8) {
      if (sigmaIEtaIEta > 0.011)             return false;
      if (HoE > 0.10)                       return false;
      if (dEtaSC > 0.01)                     return false;
      if (dPhiSC > 0.04)                     return false;
      if (eImpI < -0.05 || eImpI > 0.0010)   return false;
    }
    else if ((abs(eta) < 1.479) && (abs(eta) >= 0.8)){
      if (sigmaIEtaIEta > 0.011)             return false;
      if (HoE > 0.10)                       return false;
      if (dEtaSC > 0.01)                     return false;
      if (dPhiSC > 0.04)                     return false;
      if (eImpI < -0.05 || eImpI > 0.0010)   return false;
    }
    else if (abs(eta) >= 1.479) {
      if (sigmaIEtaIEta > 0.030)             return false;
      if (HoE > 0.07)                        return false;
      if (dEtaSC > 0.008)                   return false;
      if (dPhiSC > 0.07)                     return false;
      if (eImpI < -0.05 || eImpI > 0.005)   return false;
    }
    if (convVeto == 0)     return false;
    if (lostHits != 0)     return false;
  }

  if (wp == iMedium) {
    if (isGoodLepton(lep)) {
      Float_t   A = -0.86+(-0.85 + 0.86)*(abs(eta) > 0.8)+(-0.81 + 0.86)*(abs(eta) > 1.479);
      Float_t  B = -0.96+(-0.96 + 0.96)*(abs(eta) > 0.8)+(-0.95 + 0.96)*(abs(eta) > 1.479);
      if (pt > 10) {
        if (!(MVAID > min( A , max( B , A+(B-A)/10*(pt-15))) )) return false;
      }
    }
    if (abs(eta) < 0.8) {
      if (!isGoodLepton(lep)) {
        if (MVAID < 0)                          return false;
      }
      if (sigmaIEtaIEta > 0.011)              return false;
      if (HoE > 0.10)                        return false;
      if (dEtaSC > 0.01)                      return false;
      if (dPhiSC > 0.04)                      return false;
      if (eImpI < -0.05 || eImpI > 0.0010)    return false;
    }
    else if ((abs(eta) < 1.479) && (abs(eta) >= 0.8)) {
      if (!isGoodLepton(lep)) {
        if (MVAID < 0)                          return false;
      }
      if (sigmaIEtaIEta > 0.011)              return false;
      if (HoE > 0.10)                        return false;
      if (dEtaSC > 0.01)                      return false;
      if (dPhiSC > 0.04)                      return false;
      if (eImpI < -0.05 || eImpI > 0.0010)    return false;
    }
    else if (abs(eta) >= 1.479) {
      if (!isGoodLepton(lep)) {
        if (MVAID < 0.7)                        return false;
      }
        if (sigmaIEtaIEta > 0.030)            return false;
        if (HoE > 0.07)                       return false;
        if (dEtaSC > 0.008)                  return false;
        if (dPhiSC > 0.07)                    return false;
        if (eImpI < -0.05 || eImpI > 0.005)  return false;
    }
    if (lostHits != 0)                     return false;
  }

  if (wp == iLoose) {
    Float_t   A = -0.86+(-0.85 + 0.86)*(abs(eta) > 0.8)+(-0.81 + 0.86)*(abs(eta) > 1.479);
    Float_t  B = -0.96+(-0.96 + 0.96)*(abs(eta) > 0.8)+(-0.95 + 0.96)*(abs(eta) > 1.479);
    if (pt > 10) {
      if (!(MVAID > min( A , max( B , A+(B-A)/10*(pt-15))) )) return false;
    }
    if (lostHits > 2)     return false;
  }
  return true;
}

Bool_t LeptonSelector::getElecCutBasedId(Int_t wp){
  if(gSelection == i4tSelec){
    if(TMath::Abs(etaSC) < 1.479){ // central
      if(TMath::Abs(sigmaIEtaIEta) > 0.011 ) return false;
      if(TMath::Abs(dEtaSC) > 0.01  ) return false;
      if(TMath::Abs(dPhiSC) > 0.04 ) return false;
      if(HoE > 0.10 ) return false;
      if(eImpI < -0.05 || eImpI > 0.01 ) return false;
    }
    else if(TMath::Abs(etaSC) < 2.5){ // fwd
      if(TMath::Abs(sigmaIEtaIEta) > 0.03 ) return false;
      if(TMath::Abs(dEtaSC) > 0.008  ) return false;
      if(TMath::Abs(dPhiSC) > 0.07 ) return false;
      if(HoE > 0.07 ) return false;
      if(eImpI < -0.05 || eImpI > 0.005) return false;
    }
    return true;
  }
  else if(gSelection == iWZSelec)
    {
      return true; // Selección de ID dentro del paquete de analisis
    }
  else{
    if(wp == iTight   && tightVar < 3)     return false;
    if(wp == iMedium  && tightVar < 2)     return false;
    if(wp == iLoose   && tightVar < 1)     return false;
    if(wp == iVeto    && tightVar < 0)     return false;
    if(wp == iWPforStop){ // Spring 15
      if(TMath::Abs(etaSC) < 1.479){ // central
        if(TMath::Abs(sigmaIEtaIEta) > 0.0101 ) return false;
        if(TMath::Abs(dEtaSC) > 0.00926  ) return false;
        if(TMath::Abs(dPhiSC) > 0.0336 ) return false;
        if(HoE > 0.0597 ) return false;
        if(TMath::Abs(eImpI) > 0.012 ) return false;
        if(lostHits         > 2      ) return false;
      }
      else{ // forward
        if(TMath::Abs(sigmaIEtaIEta) > 0.0279 ) return false;
        if(TMath::Abs(dEtaSC) > 0.00724  ) return false;
        if(TMath::Abs(dPhiSC) > 0.0918 ) return false;
        if(HoE > 0.0615 ) return false;
        if(TMath::Abs(eImpI) > 0.00999 ) return false;
        if(lostHits         > 1      ) return false;
      }
    }
    if (convVeto  == 0   ) return false;
    return true;
  }
}

Bool_t LeptonSelector::getElecCutBasedId2017(Int_t wp){
  // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Cut_Based_Electron_ID_for_Run_2
  float b_full5x5_sigmaIetaIeta, b_absdEtaSeed, b_absdPhiIn, b_C0, b_relIsoWithEA, b_abs1oEm1op, b_missinnerhits; // eta < 1.479
  float e_full5x5_sigmaIetaIeta, e_absdEtaSeed, e_absdPhiIn, e_C0, e_relIsoWithEA, e_abs1oEm1op, e_missinnerhits; // eta > 1.479

  float aetaSC = TMath::Abs(etaSC); float rho = Get<Float_t>("rho");
  if(wp == iTight){
    b_full5x5_sigmaIetaIeta = aetaSC < 1.479 ? 0.0104  : 0.0305;
    b_absdEtaSeed           = aetaSC < 1.479 ? 0.00353 : 0.00567;
    b_absdPhiIn             = aetaSC < 1.479 ? 0.0499  : 0.0165;
    b_C0                    = aetaSC < 1.479 ? 0.026   : 0.026;
    b_relIsoWithEA          = aetaSC < 1.479 ? 0.0361  : 0.094;
    b_abs1oEm1op            = aetaSC < 1.479 ? 0.0278  : 0.0158;
    b_missinnerhits         = aetaSC < 1.479 ? 1       : 1;
  }
  float b_effC0 = aetaSC < 1.479 ? b_C0 + 1.12/energy + 0.0368*rho/energy : b_C0 + 0.50/energy + 0.2010*rho/energy;

  if(TMath::Abs(sigmaIEtaIEta) > b_full5x5_sigmaIetaIeta)   return false;
  if(TMath::Abs(dEtaSC)        > b_absdEtaSeed  )           return false;
  if(TMath::Abs(dPhiSC)        > b_absdPhiIn )              return false;
  if(HoE                       > b_effC0)                   return false;
  if(TMath::Abs(RelIso03)      > b_relIsoWithEA)            return false;
  if(TMath::Abs(eImpI)         > b_abs1oEm1op)              return false;
  if(lostHits                  > b_missinnerhits)           return false;
  if(!convVeto) return false;

  /*
    if(wp == iTight   && tightVar < 3)     return false;
    if(wp == iMedium  && tightVar < 2)     return false;
    if(wp == iLoose   && tightVar < 1)     return false;
    if(wp == iVeto    && tightVar < 0)     return false;
    */
  return true;
}


Bool_t LeptonSelector::getMultiIso(Int_t wp){
  if(wp == iVeryTight) return (miniIso < 0.09 && (ptRatio > 0.84 || ptRel > 7.2));
  if(wp == iTight    ){ 
    DumpVar(evt, "miniIso", miniIso, miniIso < 0.12);
    DumpVar(evt, "(ptRatio > 0.80 || ptRel > 7.2)", ptRatio, (ptRatio > 0.80 || ptRel > 7.2));
    return (miniIso < 0.12 && (ptRatio > 0.80 || ptRel > 7.2));
  }
  if(wp == iMedium   ){ 
    DumpVar(evt, "miniIso", miniIso, miniIso < 0.16);
    DumpVar(evt, "(ptRatio > 0.76 || ptRel > 7.2)", ptRatio, (ptRatio > 0.76 || ptRel > 7.2));
    return (miniIso < 0.16 && (ptRatio > 0.76 || ptRel > 7.2));
  }
  if(wp == iLoose    ) return miniIso < 0.40;
  //if(wp == iLoose    ) return (miniIso < 0.20 && (ptRatio > 0.69 || ptRel > 6.0));
  if(wp == iVeto     ){
    if(type == 1)      return (miniIso < 0.40 && (ptRatio > 0.80 || ptRel > 7.2));
    else               return (miniIso < 0.40 && (ptRatio > 0.76 || ptRel > 7.2));
  }    
  return true;
}


Int_t LeptonSelector::getSUSYMVAId(Lepton lep, Int_t ty){//ty = 1 for FO, ty = 2 for tight
  if (ty == 1){//Fakeable Objects
    if (lep.isMuon){
        if (ptRatio > 0.30 && jetBTagCSV < 0.3) return 7; //Only for FO
        else if (MVASUSY >  0.65) return 7; //Extra Tight
        else if (MVASUSY >  0.45) return 6; //Very Tight
        else if (MVASUSY >  0.15) return 5; //Tight
        else if (MVASUSY > -0.20) return 4; //Medium
        else if (MVASUSY > -0.60) return 3; //Loose
        else if (MVASUSY > -0.90) return 2; //Very Loose
    }
    else if (lep.isElec){
        if (ptRatio > 0.30 && jetBTagCSV < 0.3 && ( (MVAID > 0.0)*(lep.p.Eta() < 0.8) || (MVAID > 0.0)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (MVAID > 0.3)*(lep.p.Eta() > 1.479) )) return 7; //Only for FO
        else if (MVASUSY >  0.85) return 7; //Extra Tight
        else if (MVASUSY >  0.75) return 6; //Very Tight
        else if (MVASUSY >  0.65) return 5; //Tight
        else if (MVASUSY >  0.50) return 4; //Medium
        else if (MVASUSY >  0.25) return 3; //Loose
        else if (MVASUSY > -0.30) return 2; //Very Loose
    }
  }

  else if (ty == 2){//Tight leptonMVA leptons
    if (lep.isMuon){
        if      (MVASUSY >  0.65) return 7; //Extra Tight
        else if (MVASUSY >  0.45) return 6; //Very Tight
        else if (MVASUSY >  0.15) return 5; //Tight
        else if (MVASUSY > -0.20) return 4; //Medium
        else if (MVASUSY > -0.60) return 3; //Loose
        else if (MVASUSY > -0.90) return 2; //Very Loose
    }
    else if (lep.isElec){
        if      (MVASUSY >  0.85) return 7; //Extra Tight
        else if (MVASUSY >  0.75) return 6; //Very Tight
        else if (MVASUSY >  0.65) return 5; //Tight
        else if (MVASUSY >  0.50) return 4; //Medium
        else if (MVASUSY >  0.25) return 3; //Loose
        else if (MVASUSY > -0.30) return 2; //Very Loose
    }
  }
  return 1;
}

//################################################################
//## Lepton definitions for each analysis
//################################################################
//
// Use the functions above to define your objects
//
//=============================================================== SELECTED LEPTONS
Bool_t LeptonSelector::isGoodLepton(Lepton lep){
  Bool_t passId; Bool_t passIso;
  if(gSelection == iStopSelec){
    // Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4
    // Medium Muon ID, RelIso POG, tightIP2D, SIP3D > 4
    if(lep.isMuon){
      passId  = getMuonId(iMedium);
      passIso = getRelIso03POG(iWPforStop);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iWPforStop);
      passIso = getRelIso03POG(iWPforStop);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
    else return false;
  }
  else if(gSelection == iTopSelec || gSelection == iStopTopSelec){
    // Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4
    // Tight Muon ID, RelIso POG, tightIP2D, SIP3D > 4
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight) && lostHits <= 1;
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 18 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && ( (lep.isElec && getGoodVertex(iTight)) || (lep.isMuon && getGoodVertex(iMedium) ))) return true;
    else return false;
  }
  else if(gSelection == iTWSelec || gSelection == iTWTTbarSelec){
    // Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4
    // Tight Muon ID, RelIso POG, tightIP2D, SIP3D > 4
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight) && lostHits <= 1;
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && ( (lep.isElec && getGoodVertex(iTight)) || (lep.isMuon && getGoodVertex(iMedium) ))) return true;
    else return false;
  }
  else if(gSelection == iWWSelec){
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight);
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
    else return false;
    return false;
  }
  else if(gSelection == iHWWSelec){
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight);
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 15) return false;
    if(TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
    else return false;
    return false;
  }
  else if(gSelection == i4tSelec){
    if(lep.isMuon){
      //DumpVar(evt, "!isGlobalMuon && !isTrackerMuon", isGlobalMuon || isTrackerMuon, isGlobalMuon || isTrackerMuon);
      //DumpVar(evt, "getMuonId(iMedium)", getMuonId(iMedium), getMuonId(iMedium));
      if(lep.p.Pt() < 20) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.4) return false;
      if(!isGlobalMuon && !isTrackerMuon) return false; 
      if(!getMuonId(iMedium)) return false;
      if(!getMultiIso(iMedium)) return false;
    }
    if(lep.isElec){
      //DumpVar(evt, "getElecCutBasedId(iLoose)", getElecCutBasedId(iLoose), getElecCutBasedId(iLoose));
      //DumpVar(evt, "convVeto", convVeto, convVeto);
      //DumpVar(evt, "lostHits", lostHits, lostHits == 0);
      //DumpVar(evt, "getElecMVA(iTight)", getElecMVA(iTight), getElecMVA(iTight));
      if(lep.p.Pt() < 20) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.5) return false;
      if(!getElecCutBasedId(iLoose)) return false; // Electron selection criteria used for the emulation
      if(!convVeto) return false;
      if(lostHits != 0) return false;
      if(!getElecMVA(iTight)) return false;
      if(!getMultiIso(iTight)) return false;
    }
    //if(!getminiRelIso(iLoose)) return false;
    //DumpVar(evt, "getSIPcut(4)", sip, getSIPcut(4)); 
    //DumpVar(evt, "TightCharge == 2", TightCharge, TightCharge == 2); 
    if(!getGoodVertex(iTight)) return false;
    if(!getSIPcut(4)) return false;
    if(TightCharge != 2) return false;
    return true;
  }
  else if(gSelection == iWZSelec){ // Fakeable Objects for the WZ analysis. MVA WP (and related selection criteria) are defined inside the proper analysis
    Bool_t isMVALepton = true;    
    if(lep.isMuon){
      if(!isVetoLepton(lep)) isMVALepton = false;
      if(TightCharge != 2) isMVALepton = false;
      // Added MVA selection in the analysis
    }
    if(lep.isElec){
      if(!isVetoLepton(lep)) isMVALepton = false;
      if(TightCharge != 2) isMVALepton = false;
      if(!convVeto) isMVALepton = false;
      // Added MVA selection in the analysis
    }

    Bool_t isTopLepton = true;
    //Top/Stop ID    
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight) && lostHits <= 1;
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) isTopLepton = false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) isTopLepton = false;
    if(passId && passIso && ( (lep.isElec && getGoodVertex(iTight)) || (lep.isMuon && getGoodVertex(iMedium) ))){
      isTopLepton = true;
    }
    else isTopLepton = false;

    if (isTopLepton && isMVALepton){
      lepMVASUSYId = getSUSYMVAId(lep, 2) + 10; //This is a terrible but quick way to do it
      return true;
    }
    else if (isTopLepton && !(isMVALepton)){
      lepMVASUSYId = 10; //This is a terrible but quick way to do it
      return true;  
    }
    else if ( (!isTopLepton) && isMVALepton){
      lepMVASUSYId = getSUSYMVAId(lep, 2); //This is a terrible but quick way to do it
      return true;    
    }
    else return false;
  }
  else if (gSelection == ittHSelec ) {
    //   Tight muons for multilepton ttH Analysis:
    // abs(eta)<0.4, Pt>15, abs(dxy)<0.05cm, abs(dz)<0.1cm, SIP3D<8, Imini<0.4,
    // isLooseMuon==1,jetCSV<0.8484,isMediumMuon==1,tight-charge,lepMVA>0.90.
    //
    //   Tight electrons for multilepton ttH Analysis:
    // abs(eta)<0.5, Pt>15, abs(dxy)<0.05cm, abs(dz)<0.1cm, SIP3D<8, Imini<0.4,
    // jetCSV<0.8484,lepMVA>0.90,missinghits==0,conversion rej..
    // Furthermore, 3 regions in eta-phi space are defined: 0-0.8-1.479-2.5,
    // where: MVA ID>(0,0,0.7), sigmaietaieta<(0.011,0.011,0.031),
    // HoverE<(0.10,0.10,0.07), Deltaetain<(0.01,0.01,0.008),
    // Deltaphiin<(0.04,0.04,0.07),-0.05<1/E-1/p<(0.01,0.01,0.005)
    //
    Bool_t passVertex; Bool_t passEta; Bool_t passPt; Bool_t passSIP;
    Bool_t passCSV; Bool_t passLepMVA; Bool_t passElecCutBasedId;
    Bool_t passptRatio;
    
    if (lep.isMuon) {
      passEta            = (abs(eta) < 2.4);
      passPt              = (pt > 10);
      passVertex         = getGoodVertex(iTight);
      passSIP             = getSIPcut(8);
      passIso             = getminiRelIso(iTight);
      passCSV             = (jetBTagCSV < 0.8484);
      passId             = mediumMuonId;
      passLepMVA         = (MVATTH > 0.90);

      passptRatio         = 1;
      passElecCutBasedId = 1;
    }
    if (lep.isElec) {
      passEta            = (abs(eta) < 2.5);
      passPt              = (pt > 10);
      passVertex         = getGoodVertex(iTight);
      passSIP             = getSIPcut(8);
      passIso             = getminiRelIso(iTight);
      passElecCutBasedId = getElecMVAId(iTight,lep);
      passptRatio         = 1;
      passCSV             = (jetBTagCSV < 0.8484);
      passLepMVA         = (MVATTH > 0.90);

      passId             = 1;
    }
    if (!passEta || !passPt || !passVertex || !passSIP || !passIso || !passCSV || !passId || !passLepMVA || !passptRatio || !passElecCutBasedId) return false;
  return true;
  }
  return true;
}

//============================================== VETO LEPTONS
Bool_t LeptonSelector::isVetoLepton(Lepton lep){
  Bool_t passId; Bool_t passIso;
  if(gSelection == iStopSelec || gSelection == iStopTopSelec){
    if(lep.isMuon){
      passId = true;
      passIso = getRelIso03POG(iLooseWPforStop);
    }
    else{
      passId = getElecCutBasedId(iVeto);
      passIso = getRelIso03POG(iLooseWPforStop);
    } 
    return passId && passIso && getGoodVertex(iTight) && getSIPcut(4);
  }
  else if(gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iTWTTbarSelec){
    return true;
  }
  else if((gSelection == iWWSelec) || (gSelection == iHWWSelec)){
    return true;
  }
  else if(gSelection == i4tSelec){
    if(lep.isMuon){
      if(lep.p.Pt() < 20) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.4) return false;
      if(!isGlobalMuon && !isTrackerMuon) return false; 
      if(!getMuonId(iMedium)) return false;
      if(!getMultiIso(iLoose)) return false;
    }
    if(lep.isElec){
      if(lep.p.Pt() < 20) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.5) return false;
      if(!getElecCutBasedId(iLoose)) return false; // Electron selection criteria used for the emulation
      if(!convVeto) return false;
      if(lostHits != 0) return false;
        if(!getElecMVA(iLoose)) return false;
      if(!getMultiIso(iLoose)) return false;
    }
    //if(!getminiRelIso(iLoose)) return false;
    if(!getGoodVertex(iTight)) return false;
    if(!getSIPcut(4)) return false;
    if(TightCharge != 2) return false;
    return true;
  }
  else if(gSelection == iWZSelec){ // Fakeable Objects for the WZ analysis
    if(lep.isMuon){
      if(lep.p.Pt() < 10) return false;
      if(!isLooseLepton(lep)) return false;
      if(!mediumMuonId) return false;

    }
    if(lep.isElec){
      if(lep.p.Pt() < 10) return false;
      if(!isLooseLepton(lep)) return false;
      if(lostHits > 0) return false;
      if((sigmaIEtaIEta > 0.011 )*(lep.p.Eta() < 0.8) || (sigmaIEtaIEta > 0.011)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (sigmaIEtaIEta > 0.030)*(lep.p.Eta() > 1.479)) return false;
      if((HoE > 0.1)*(lep.p.Eta() < 0.8) || (HoE > 0.1)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (HoE > 0.07)*(lep.p.Eta() > 1.479)) return false;
      if((dEtaSC > 0.01)*(lep.p.Eta() < 0.8) || (dEtaSC > 0.01)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (dEtaSC > 0.008)*(lep.p.Eta() > 1.479)) return false;
      if((dPhiSC > 0.04)*(lep.p.Eta() < 0.8) || (dPhiSC > 0.04)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (dPhiSC > 0.07)*(lep.p.Eta() > 1.479)) return false;
      if((eImpI<-0.05) || (eImpI > 0.01)*(lep.p.Eta() < 0.8) || (eImpI > 0.01)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (eImpI > 0.005)*(lep.p.Eta() > 1.479)) return false;

    }
    lepMVASUSYId = getSUSYMVAId(lep, 1);
    if (lepMVASUSYId < 0) return false;
    return true;
  }
  else if(gSelection == ittHSelec){
    //   Fakeable muons for multilepton ttH Analysis:
    // Tight muons without medium muon ID, tight charge and lepton MVA cuts.
    //
    //   Fakeable electrons for multilepton ttH Analysis:
    // Tight electrons without tight charge, conv. rej., lepton MVA cuts and
    // with ptratio > 0.5, if the electron fails tight selection (otherwise
    // w/o cut in ptratio) and, in this case too, with <0.3 jet CSV.
    //
    Bool_t passVertex; Bool_t passEta; Bool_t passPt; Bool_t passSIP;
    Bool_t passCSV; Bool_t passLepMVA; Bool_t passElecCutBasedId;
    Bool_t passptRatio; Bool_t passSegComp;
    if (lep.isMuon) {
      passEta           = (abs(eta) < 2.4);
      passPt             = (pt > 10);
      passVertex        = getGoodVertex(iMedium);
      passSIP            = getSIPcut(8);
      passIso            = getminiRelIso(iLoose);
      if (!isGoodLepton(lep)) {
        passCSV             = (jetBTagCSV < 0.3);
        passptRatio         = (ptRatio > 0.5);
        passSegComp         = (SegComp > 0.3);
      } else {
        passCSV             = (jetBTagCSV < 0.8484);
        passptRatio         = 1;
        passSegComp         = 1;
      }
      passId             = 1;
      passLepMVA         = 1;
      passElecCutBasedId = 1;
    }
    if (lep.isElec) {
      passEta            = (abs(eta) < 2.5);
      passPt              = (pt > 10);
      passVertex         = getGoodVertex(iMedium);
      passSIP             = getSIPcut(8);
      passIso             = getminiRelIso(iLoose);
      passElecCutBasedId = getElecMVAId(iLoose,lep);
      if (!isGoodLepton(lep)) {
        passptRatio         = (ptRatio > 0.5);
        passCSV             = (jetBTagCSV < 0.3);
      } else {
        passptRatio         = 1;
        passCSV             = (jetBTagCSV < 0.8484);
      }

      passLepMVA        = 1;
      passId            = 1;
      passSegComp        = 1;
    }
    if (!passEta || !passPt || !passVertex || !passSIP || !passIso || !passCSV || !passId || !passLepMVA || !passptRatio || !passElecCutBasedId || !passSegComp) return false;
    return true;
  }
  return false;
}

//============================================== Loose leptons (or other)
Bool_t LeptonSelector::isLooseLepton(Lepton lep){
  Bool_t passId; Bool_t passIso;
  if(gSelection == i4tSelec){
    if(lep.isMuon){
      if(lep.p.Pt() < 15) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.4) return false;
      if(!isGlobalMuon && !isTrackerMuon) return false; 
      // Smallest muon Id: Loose Id!! 
      if(!getMultiIso(iLoose)) return false;
    }
    if(lep.isElec){
      if(lep.p.Pt() < 15) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.5) return false;
      if(!getElecCutBasedId(iLoose)) return false;
      if(!convVeto) return false;
      if(lostHits > 2) return false;
      if(!getElecMVA(iLoose)) return false;
      if(!getMultiIso(iLoose)) return false;
    }
    //if(!getminiRelIso(iLoose)) return false;
    if(!getGoodVertex(iTight)) return false;
    return true;
  }
  else if(gSelection == iWZSelec){ // Loose leptons are common to all tight WP
    if(lep.isMuon){
      if(lep.p.Pt() < 5) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.4) return false;
      if(!getGoodVertex(iLoose)) return false;
      if(!getSIPcut(8)) return false;
      if(!getminiRelIso(iLoose)) return false;
    }
    if(lep.isElec){
      if(lep.p.Pt() < 7) return false;
      if(TMath::Abs(lep.p.Eta()) > 2.5) return false;
      if(!getGoodVertex(iLoose)) return false;    
      if(!getSIPcut(8)) return false;
      if(!getminiRelIso(iLoose)) return false;
      if(!getElecMVAId(iLoose,lep)) return false;  
      if((MVAID < -0.70)*(lep.p.Eta() < 0.8) || (MVAID < -0.83)*(lep.p.Eta() < 1.479)*(lep.p.Eta() > 0.8) || (MVAID < -0.92)*(lep.p.Eta() > 1.479)) return false;
      if(lostHits > 2) return false;
    }
    return true;
  }
  else if(gSelection == iTopSelec || gSelection == iStopTopSelec || gSelection == iTWSelec || gSelection == iTWTTbarSelec){
    // Same as good lepton but no looser cut on pT
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight) && lostHits <= 1;
      if(gIs2017)  passId = getElecCutBasedId2017(iTight);
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 18 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && ( (lep.isElec && getGoodVertex(iTight)) || (lep.isMuon && getGoodVertex(iMedium) ))) return true;
    else return false;
  }
  if(gSelection == ittHSelec){
    //   Loose muons for multilepton ttH Analysis:
    // Fakeable muons without jetCSV cut and with pt>5.
    //
    //   Loose electrons for multilepton ttH Analysis:
    // Fakeable electrons with Nmissinghits<2 and pt>7 and without jetCSV,
    // ptratio, 1/E-1/p, deltaPhiin, deltaEtain, H/E, sigmaietaieta cuts
    //
    Bool_t passVertex; Bool_t passEta; Bool_t passPt; Bool_t passSIP;
    Bool_t passCSV; Bool_t passLepMVA; Bool_t passElecCutBasedId; 
    Bool_t passptRatio;
    if (lep.isMuon) {
      passEta            = (abs(eta) < 2.4);
      passPt              = (pt > 5);
      passVertex         = getGoodVertex(iLoose);
      passSIP             = getSIPcut(8);
      passIso             = getminiRelIso(iLoose);

      passCSV             = 1;
      passId             = 1;
      passLepMVA         = 1;
      passptRatio         = 1;
      passElecCutBasedId = 1;
    }
    if (lep.isElec) {
      passEta            = (abs(eta) < 2.5);
      passPt              = (pt > 7);
      passVertex         = getGoodVertex(iLoose);
      passSIP             = getSIPcut(8);
      passIso             = getminiRelIso(iLoose);
      passElecCutBasedId = getElecMVAId(iLoose,lep);

      passptRatio         = 1;
      passCSV             = 1;
      passLepMVA         = 1;
      passId             = 1;
    }
    if (!passEta || !passPt || !passVertex || !passSIP || !passIso || !passCSV || !passId || !passLepMVA || !passptRatio || !passElecCutBasedId) return false;
    return true;
  }
    return true;
}


///////////////////////////////////////////////////////////////////////////
// You do not want to change anything below this point
///////////////////////////////////////////////////////////////////////////

void LeptonSelector::InsideLoop(){
  evt = Get<ULong64_t>("evt");
  // Clear vectors...
  selLeptons.clear();
  looseLeptons.clear();
  genLeptons.clear();
  vetoLeptons.clear();
  vGenBquarks.clear();
  nLeptonsFromTau = 0;
  // Loop over the leptons and select
  nLep     = Get<Int_t>("nLepGood");
  DumpEvent(evt, "========================================");
  DumpEvent(evt, Form("Event: %i", evt));
  DumpEvent(evt, Form("N Leptons: %i", nLep));
  if(!gIsData) GetGenBquarks();

  // Loop over the gen leptons and get gen info...
  if(!gIsData){
    ngenLep         = Get<Int_t>("ngenLep");
    ngenLepFromTau  = Get<Int_t>("ngenLepFromTau");
    for(Int_t i = 0; i < ngenLep; i++){
      GetGenLeptonVariables(i);
      if(gpdgMId == 23 || gpdgMId == 24 || gpdgMId == 25){
        tL = Lepton(tP, charge, type);
        tL.Mid = gpdgMId;
        //if(tL.p.Pt() > 20 && TMath::Abs(tL.p.Eta()) < 2.4) genLeptons.push_back(tL);
        genLeptons.push_back(tL);
      }
    }
    for(Int_t i = 0; i < ngenLepFromTau; i++){
      GetGenLepFromTauVariables(i);
      if(gpdgMId == 23 || gpdgMId == 24 || gpdgMId == 25){
        tL = Lepton(tP, charge, type);
        tL.Mid = 15;
        nLeptonsFromTau++;
        //if(tL.p.Pt() > 20 && TMath::Abs(tL.p.Eta()) < 2.4) genLeptons.push_back(tL);
        genLeptons.push_back(tL);
      }
    }
  }

  // Loop over reco leptons
  for(Int_t i = 0; i < nLep; i++){
    GetLeptonVariables(i);
    tL = Lepton(tP, charge, type);
    if(tL.isMuon){
      tL.SetIso(RelIso04);
      tL.SetEnergyUnc(GetMuonEnergyScale());
    }
    else{
      tL.SetIso(RelIso03);
      tL.SetR9(R9);
      tL.SetEnergyUnc(ElecScale->GetUnc(tL.Pt(), tL.Eta(), tL.GetR9()));
    }

    // Set status: Good, conv, flip, fake
    if(!gIsData){
      tL.SetGenMatch(kLGMgood);
      if(abs(mcMatchPDGID) != abs(pdgid))            tL.SetGenMatch(kLGMother);
      if(mcPromptGamma == 1)                         tL.SetGenMatch(kLGMconv);
//      if(IsMatchedDeltaR(tL.p, genLeptons,  0.4))    tL.SetGenMatch(kLGMtoGenLep);
//      if(IsMatchedDeltaR(tL.p, vGenBquarks, 0.3))    tL.SetGenMatch(kLGMtoGenB);
      if( (tL.GetGenMatch() == kLGMtoGenLep || tL.GetGenMatch() == kLGMgood) && charge*mcMatchPDGID > 0 && abs(mcMatchPDGID) == abs(pdgid))  tL.SetGenMatch(kLGMflip); // flip and no fake
      if(mcMatchID == 0 || mcMatchID == -99)         tL.SetGenMatch(kLGMfake);
//      if(!mcPrompt)   tL.SetGenMatch(kLGMfake);
    }

    if(isGoodLepton(tL)){
      DumpEvent(evt, Form(" >>> Lepton %i (pt = %g, eta = %g, type = %i):    PASA", i, tP.Pt(), tP.Eta(), type));
      //if(1){
      tL.SetSF(   LepSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
      tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta, tL.type) );
      if(gSelection == iWZSelec) tL.idMVA = lepMVASUSYId;
      selLeptons.push_back(tL);
    }
    else DumpEvent(evt, Form(" >>> Lepton %i (pt = %g, eta = %g, type = %i): NO PASA", i, tP.Pt(), tP.Eta(), type));
    if(isVetoLepton(tL)){ // If you need to veto extra leptons...
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      if(gSelection == i4tSelec){
        if(!isGoodLepton(tL)) vetoLeptons.push_back(tL);
      }
      else if(gSelection == iWZSelec) tL.idMVA = lepMVASUSYId;
      else vetoLeptons.push_back(tL);
    }
    if(isLooseLepton(tL)){ // A loose category... used in ttH, for example
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      if(gSelection == i4tSelec){
        if(!isGoodLepton(tL)) looseLeptons.push_back(tL);
      }
      else if(gSelection == iStopTopSelec){
        tL.SetSF(   LepSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
        tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta, tL.type) );
        looseLeptons.push_back(tL);
      }
      else looseLeptons.push_back(tL);
    }
  }

  if(gSelection == iStopSelec || gSelection == iStopTopSelec){ // Adding leptons for the discarded collection
    nLep     = Get<Int_t>("nDiscLep");
    for(Int_t i = 0; i < nLep; i++){
      GetDiscLeptonVariables(i);
      tL = Lepton(tP, charge, type);
      if(tL.isMuon) tL.SetIso(RelIso04);
      else          tL.SetIso(RelIso03);
      tL.SetGenMatch(kLGMgood);

      if(!gIsData){
        tL.SetGenMatch(kLGMgood);
        if(abs(mcMatchPDGID) != abs(pdgid))            tL.SetGenMatch(kLGMother);
        if(mcPromptGamma == 1)                         tL.SetGenMatch(kLGMconv);
        if(mcMatchID == 0 || mcMatchID == -99)         tL.SetGenMatch(kLGMfake);
//        if(!mcPrompt)   tL.SetGenMatch(kLGMfake);
       // if(IsMatchedDeltaR(tL.p, vGenBquarks, 0.4))    tL.SetGenMatch(kLGMtoGenB);
       // if(IsMatchedDeltaR(tL.p, genLeptons,  0.4))    tL.SetGenMatch(kLGMtoGenLep);
        if( (tL.GetGenMatch() == kLGMtoGenLep || tL.GetGenMatch() == kLGMgood) && charge*mcMatchPDGID > 0 && abs(mcMatchPDGID) == abs(pdgid))  tL.SetGenMatch(kLGMflip); // flip and no fake
      }

      if(isGoodLepton(tL)){
        tL.SetSF(   LepSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
        tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta, tL.type) );
        selLeptons.push_back(tL);
      }
      if(isVetoLepton(tL)){ // If you need to veto extra leptons...
        //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
        vetoLeptons.push_back(tL);
      }
      if(isLooseLepton(tL)){ // A loose category... used in ttH, for example
        //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
        tL.SetSF(   LepSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
        tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta, tL.type) );
        looseLeptons.push_back(tL);
      }
    }
  }

  nSelLeptons   = selLeptons.size();
  nVetoLeptons   = vetoLeptons.size();
  nLooseLeptons = looseLeptons.size();
  nGenLeptons    = genLeptons.size();

  //=== Trigger SF
  TriggerSF = 1; TriggerSFerr = 0;
  if(gSelection == iTopSelec || gSelection == iStopTopSelec || gSelection == iTWSelec || gSelection == iTWTTbarSelec || gSelection == iStopSelec){
    if(nSelLeptons >= 2){
      if     (selLeptons.at(0).isMuon && selLeptons.at(1).isMuon){
        TriggerSF = LepSF->GetTrigDoubleMuSF(    selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
        TriggerSFerr = LepSF->GetTrigDoubleMuSF_err(selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
      }
      else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec){
        TriggerSF = LepSF->GetTrigDoubleElSF(    selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
        TriggerSFerr = LepSF->GetTrigDoubleElSF_err(selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
      }
      else{
        TriggerSF = LepSF->GetTrigElMuSF(        selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
        TriggerSFerr = LepSF->GetTrigElMuSF_err(    selLeptons.at(0).p.Eta(), selLeptons.at(1).p.Eta());
      }
    }
  }

  //=== FullSim/FastSim SF
  FSSF = 1; FSSFerr = 0; Float_t id;
  if(gIsFastSim && nSelLeptons >= 1){
    id = 11; if(selLeptons.at(0).isMuon) id = 13;
    FSSF    = LepSF->GetFSSF(selLeptons.at(0).p.Pt(), selLeptons.at(0).p.Eta(), id);
    FSSFerr = LepSF->GetFSSFerr(selLeptons.at(0).p.Pt(), selLeptons.at(0).p.Eta(), id);
  }

  DumpEvent(evt, "========================================");

 selLeptons   = SortLeptonsByPt(selLeptons);
 vetoLeptons  = SortLeptonsByPt(vetoLeptons);
 looseLeptons = SortLeptonsByPt(looseLeptons);
 genLeptons   = SortLeptonsByPt(genLeptons);

  // Set params for the next selectors
  SetParam("selLeptons",  selLeptons );
  SetParam("vetoLeptons", vetoLeptons);
  SetParam("looseLeptons", looseLeptons);
  SetParam("genLeptons",  genLeptons );
  SetParam("nLeptonsFromTau", nLeptonsFromTau);
  SetParam("nGenLeptons", nGenLeptons);
  SetParam("nSelLeptons", nSelLeptons);
  SetParam("nVetoLeptons", nVetoLeptons);
  SetParam("nLooseLeptons", nVetoLeptons);

  SetParam("TriggerSF",    TriggerSF);
  SetParam("TriggerSFerr", TriggerSFerr);
  SetParam("FSSF",    FSSF);
  SetParam("FSSFerr", FSSFerr);
}

//################################################################
//## Get important variables
//################################################################
void LeptonSelector::GetLeptonVariables(Int_t i){ // Once per muon, get all the info
  //tP.SetPxPyPzE(Get<Float_t>("LepGood_px", i), Get<Float_t>("LepGood_py", i), Get<Float_t>("LepGood_pz", i), Get<Float_t>("LepGood_energy", i));
  tP.SetPtEtaPhiM(Get<Float_t>("LepGood_pt", i), Get<Float_t>("LepGood_eta", i), Get<Float_t>("LepGood_phi", i), Get<Float_t>("LepGood_mass", i));
  pt             = tP.Pt();
  eta           = tP.Eta();
  energy        = tP.Energy();
  charge         = Get<Int_t>("LepGood_charge", i);
  type           = TMath::Abs(Get<Int_t>("LepGood_pdgId",i)) == 11 ? 1 : 0;
  pdgid          = Get<Int_t>("LepGood_pdgId",i);
  tightVar       = Get<Int_t>("LepGood_tightId", i);
  mediumMuonId   = Get<Int_t>("LepGood_mediumMuonId",i);
  etaSC         = TMath::Abs(Get<Float_t>("LepGood_etaSc",i));
  RelIso03       = Get<Float_t>("LepGood_relIso03",i);
  RelIso04       = Get<Float_t>("LepGood_relIso04",i);
  ptRel         = Get<Float_t>("LepGood_jetPtRelv2",i);
  ptRatio       = Get<Float_t>("LepGood_jetPtRatiov2",i);
  miniIso       = Get<Float_t>("LepGood_miniRelIso",i);
  dxy           = TMath::Abs(Get<Float_t>("LepGood_dxy", i));
  dz            = TMath::Abs(Get<Float_t>("LepGood_dz", i));
  sigmaIEtaIEta = Get<Float_t>("LepGood_sigmaIEtaIEta", i);
  dEtaSC         = Get<Float_t>("LepGood_dEtaScTrkIn", i);
  dPhiSC         = Get<Float_t>("LepGood_dPhiScTrkIn", i);
  HoE            = Get<Float_t>("LepGood_hadronicOverEm", i);
  eImpI          = Get<Float_t>("LepGood_eInvMinusPInv", i);
  lostHits       = Get<Int_t>("LepGood_lostHits", i);
  convVeto       = Get<Int_t>("LepGood_convVeto", i);
  sip           = Get<Float_t>("LepGood_sip3d",i);
  MVATTH        = Get<Float_t>("LepGood_mvaTTH",i);
  MVASUSY        = Get<Float_t>("LepGood_mvaSUSY",i);
  TightCharge    = Get<Int_t>("LepGood_tightCharge",i);
  jetBTagCSV    = Get<Float_t>("LepGood_jetBTagCSV",i);
  SegComp        = Get<Float_t>("LepGood_segmentCompatibility",i);
  R9            = Get<Float_t>("LepGood_r9",i);
  
  if(!gIsData){
    mcPrompt      = Get<Int_t>("LepGood_mcPrompt", i);
    mcMatchID     = Get<Int_t>("LepGood_mcMatchId", i);
    mcPromptGamma = Get<Int_t>("LepGood_mcPromptGamma", i);
    mcMatchPDGID  = Get<Int_t>("LepGood_mcMatchPdgId", i);
  }
  
  if (gSelection != iTWSelec && gSelection != iTWTTbarSelec){
    MVAID         = Get<Float_t>("LepGood_mvaIdSpring16GP",i);
    isGlobalMuon  = Get<Int_t>("LepGood_isGlobalMuon",i); 
    isTrackerMuon = Get<Int_t>("LepGood_isTrackerMuon",i); 
  }
  else {
    MVAID         = 0.;
    isGlobalMuon  = 0.;
    isTrackerMuon = 0.;
  }
}

void LeptonSelector::GetDiscLeptonVariables(Int_t i){ // Once per muon, get all the info
  tP.SetPxPyPzE(Get<Float_t>("DiscLep_px", i), Get<Float_t>("DiscLep_py", i), Get<Float_t>("DiscLep_pz", i), Get<Float_t>("DiscLep_energy", i));
  pt            = tP.Pt();
  eta           = tP.Eta();
  energy        = tP.Energy();
  charge        = Get<Int_t>("DiscLep_charge", i);
  type          = TMath::Abs(Get<Int_t>("DiscLep_pdgId",i)) == 11 ? 1 : 0;
  pdgid         = Get<Int_t>("DiscLep_pdgId",i);
  tightVar      = Get<Int_t>("DiscLep_tightId", i);
  mediumMuonId  = Get<Int_t>("DiscLep_mediumMuonId",i);
  etaSC         = TMath::Abs(Get<Float_t>("DiscLep_etaSc",i));
  RelIso03      = Get<Float_t>("DiscLep_relIso03",i);
  RelIso04      = Get<Float_t>("DiscLep_relIso04",i);
  ptRel         = Get<Float_t>("DiscLep_jetPtRelv2",i);
  ptRatio       = Get<Float_t>("DiscLep_jetPtRatiov2",i);
  miniIso       = Get<Float_t>("DiscLep_miniRelIso",i);
  dxy           = TMath::Abs(Get<Float_t>("DiscLep_dxy", i));
  dz            = TMath::Abs(Get<Float_t>("DiscLep_dz", i));
  sigmaIEtaIEta = Get<Float_t>("DiscLep_sigmaIEtaIEta", i);
  dEtaSC        = Get<Float_t>("DiscLep_dEtaScTrkIn", i);
  dPhiSC        = Get<Float_t>("DiscLep_dPhiScTrkIn", i);
  HoE           = Get<Float_t>("DiscLep_hadronicOverEm", i);
  eImpI         = Get<Float_t>("DiscLep_eInvMinusPInv", i);
  lostHits      = Get<Int_t>("DiscLep_lostHits", i);
  convVeto      = Get<Int_t>("DiscLep_convVeto", i);
  sip           = Get<Float_t>("DiscLep_sip3d",i);
  MVATTH        = Get<Float_t>("DiscLep_mvaTTH",i);       //*
  MVASUSY       = Get<Float_t>("DiscLep_mvaSUSY",i);       //*
  TightCharge   = Get<Int_t>("DiscLep_tightCharge",i);      //*
  jetBTagCSV    = Get<Float_t>("DiscLep_jetBTagCSV",i);   //*
  SegComp       = Get<Float_t>("DiscLep_segmentCompatibility",i);   //*
  
  if (gSelection == iTWSelec || gSelection == iTWTTbarSelec) {
    MVAID         = 0.;
    isGlobalMuon  = 0.;
    isTrackerMuon = 0.;
  }
  else {
    MVAID         = Get<Float_t>("DiscLep_mvaIdSpring16GP",i);
    isGlobalMuon  = Get<Int_t>("DiscLep_isGlobalMuon",i);
    isTrackerMuon = Get<Int_t>("DiscLep_isTrackerMuon",i);
    mcPrompt      = Get<Int_t>("DiscLep_mcPrompt", i);
    mcMatchID     = Get<Int_t>("DiscLep_mcMatchId", i);
    mcPromptGamma = Get<Int_t>("DiscLep_mcPromptGamma", i);
    mcMatchPDGID  = Get<Int_t>("DiscLep_mcMatchPdgId", i);
  }
}
void LeptonSelector::GetGenLeptonVariables(Int_t i){
  tP.SetPtEtaPhiM(Get<Float_t>("genLep_pt", i), Get<Float_t>("genLep_eta", i), Get<Float_t>("genLep_phi", i), Get<Float_t>("genLep_mass", i));
  charge = Get<Int_t>("genLep_charge", i);
  gpdgMId = TMath::Abs(Get<Int_t>("genLep_motherId", i));
  type = TMath::Abs(Get<Int_t>("genLep_pdgId",i)) == 11 ? 1 : 0;
}

void LeptonSelector::GetGenLepFromTauVariables(Int_t i){
  tP.SetPtEtaPhiM(Get<Float_t>("genLepFromTau_pt", i), Get<Float_t>("genLepFromTau_eta", i), Get<Float_t>("genLepFromTau_phi", i), Get<Float_t>("genLepFromTau_mass", i));
  charge = Get<Int_t>("genLepFromTau_charge", i);
  gpdgMId = TMath::Abs(Get<Int_t>("genLepFromTau_grandmotherId", i));
  type = TMath::Abs(Get<Int_t>("genLepFromTau_pdgId",i)) == 11 ? 1 : 0;
}

void LeptonSelector::GetGenBquarks(){
  Int_t nPart = Get<Int_t>("nGenPart"); 
  TLorentzVector p; int pdg;
  for(Int_t i = 0; i < nPart; i++){
    pdg = Get<Int_t>("GenPart_pdgId", i);
    if(TMath::Abs(pdg) != 5) continue; // b quarks
    p.SetPtEtaPhiM(Get<Float_t>("GenPart_pt", i), Get<Float_t>("GenPart_eta", i), Get<Float_t>("GenPart_phi", i), Get<Float_t>("GenPart_mass", i));
    vGenBquarks.push_back(p);
  }
}
