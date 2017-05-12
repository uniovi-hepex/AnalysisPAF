///////////////////////////////////////////////////////////////////////////////////////////
//
//  Tau Selector: create vectors with selected Taus, veto (or fakeable) Taus...
//
//  All SFs and variables are within the Tau definition
//
//  To do: definition of veto Taus
//
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "TauSelector.h"

ClassImp(TauSelector);
TauSelector::TauSelector() : PAFChainItemSelector() {}
void TauSelector::Summary() {}

void TauSelector::Initialise() {
  // Initialise TauSelector
  gIsData       = GetParam<Bool_t>("IsData");
  gSelection    = GetParam<Int_t>("iSelection");
  localPath     = GetParam<TString>("WorkingDir");
  
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  selTaus   = std::vector<Lepton>();
  vetoTaus  = std::vector<Lepton>();
  looseTaus = std::vector<Lepton>();
}

//################################################################
//## Definition of wps...
//################################################################

// (Fill if necessary)


//################################################################
//## Tau definitions for each analysis
//################################################################
//
// Use the functions above to define your objects
//
// =============================================================== SELECTED TAUS
Bool_t TauSelector::isGoodTau(Lepton tau){

  //if(gSelection == i4tSelec) looseLeptons  = GetParam<std::vector<Lepton>>("selLeptons"  );
  //else                       looseLeptons  = GetParam<std::vector<Lepton>>("looseLeptons");
  looseLeptons  = GetParam<std::vector<Lepton>>("looseLeptons");
  

  if (gSelection == ittHSelec) {
    //      Selection of taus for multilepton ttH analysis:
    // idDecayMode = 1, pt > 20, abs(eta) < 2.3, medium wp in idMVA, no one can
    // overlap within dR = 0.4 with loose electrons or muons.
    
    Bool_t passDecayMode; Bool_t passPt; Bool_t passEta; Bool_t passID;
    Bool_t passOverlap; Bool_t passAntiE; Bool_t passAntiMu;
    
    passDecayMode = (idDecayMode == 1);
    passPt        = (pt > 20         );
    passEta       = (abs(eta) < 2.3  );
    passID        = (idMVA >= 3      );
    passAntiE     = (idAntiE >= 4    );
    passAntiMu    = (idAntiMu >= 4   );
    
    passOverlap             = 1;
    for (Int_t i = 0; i < looseLeptons.size(); i++) {
      if (tau.p.DeltaR(looseLeptons[i].p) < 0.4)
        {
          passOverlap = 0;
          break;
        }
    }
    
    if (!passDecayMode || !passPt || !passEta || !passID || !passOverlap || !passAntiE || !passAntiMu) return false;
    return true;
  } 
  else if(gSelection == i4tSelec)
    {
      // idDecayMode = 1, pt > 20, abs(eta) < 2.3, loose wp in idMVA, dR>0.4 w.r.t. loose leptons 
      
      Bool_t passDecayMode (idDecayMode == 1);
      Bool_t passPt        (pt > 20         );
      Bool_t passEta       (abs(eta) < 2.3  );
      Bool_t passID        (idMVA >= 2      );
      Bool_t passAntiE     (idAntiE >= 4    );
      Bool_t passAntiMu    (idAntiMu > 1    );
      Bool_t passOverlap   (true            );
      
      for (Int_t i = 0; i < looseLeptons.size(); i++) {
        if (tau.p.DeltaR(looseLeptons[i].p) < 0.4)
          {
            passOverlap = false;
          }
      }
      
      if (!passDecayMode || !passPt || !passEta || !passID || !passOverlap || !passAntiE || !passAntiMu) return false;
      return true;
    }
  else {
    return false;
  }
}

// ============================================== VETO TAUS
Bool_t TauSelector::isVetoTau(Lepton tau){
  // (Fill if necessary)
  return false;
}

// ============================================== LOOSE TAUS
Bool_t TauSelector::isLooseTau(Lepton tau){
  // (Fill if necessary)
  return false;
}


///////////////////////////////////////////////////////////////////////////
// You do not want to change anything below this point
///////////////////////////////////////////////////////////////////////////

void TauSelector::InsideLoop(){
  // Clear vectors...
  selTaus.clear();
  vetoTaus.clear();
  looseTaus.clear();
  
  // Loop over the Taus and select
  nTau = Get<Int_t>("nTauGood");
  for(Int_t i = 0; i < nTau; i++) {
    GetTauVariables(i);
    tL = Lepton(tP,
                charge,
                type,
                decayMode,
                idDecayMode, 
                idMVA, 
                idAntiE, 
                idAntiMu
                );
    if(isGoodTau(tL)) {
      selTaus.push_back(tL);
    }
    if(isVetoTau(tL)) {
      vetoTaus.push_back(tL);
    }
    if(isLooseTau(tL)) {
      looseTaus.push_back(tL);
    }
  }
  
  nSelTaus    = selTaus.size();
  nVetoTaus   = vetoTaus.size();
  nLooseTaus  = vetoTaus.size();
  
  // Set params for the next selectors
  SetParam("selTaus",   selTaus );
  SetParam("vetoTaus",  vetoTaus);
  SetParam("looseTaus", looseTaus);
  SetParam("nSelTaus",  nSelTaus);
  SetParam("nVetoTaus", nVetoTaus);
  SetParam("nLooseTaus",nVetoTaus);
}


//################################################################
//## Get important variables
//################################################################
void TauSelector::GetTauVariables(Int_t i) { // Once per tau, get all the info
  tP.SetPtEtaPhiM(Get<Float_t>("TauGood_pt", i), Get<Float_t>("TauGood_eta", i), Get<Float_t>("TauGood_phi", i), Get<Float_t>("TauGood_mass", i));
  pt    = tP.Pt();
  eta   = tP.Eta();
  type  = 2;
  
  charge      = Get<Int_t>("TauGood_charge"     ,i);
  decayMode   = Get<Int_t>("TauGood_decayMode"  ,i);
  idDecayMode = Get<Int_t>("TauGood_idDecayMode",i);
  idMVA       = Get<Int_t>("TauGood_idMVA"      ,i);
  idAntiE     = Get<Int_t>("TauGood_idAntiE"    ,i);
  idAntiMu    = Get<Int_t>("TauGood_idAntiMu"   ,i);

  SetParam("charge"     , charge     );
  SetParam("decayMode"  , decayMode  );
  SetParam("idDecayMode", idDecayMode);
  SetParam("idMVA"      , idMVA      );
  SetParam("idAntiE"    , idAntiE    );
  SetParam("idAntiMu"   , idAntiMu   );

}
