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
  gIsFastSim    = GetParam<Bool_t>("IsFastSim");
  gSelection    = GetParam<Int_t>("iSelection");
  localPath     = GetParam<TString>("WorkingDir");
  TauSF 		= new LeptonSF(localPath + "/InputFiles/");

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // Select SFs for each analysis !!!!!!
  if(gSelection == ittHSelec){
  
  } else std::cout << ">>>>>>>>>>>> WRONG SELECTION!!!!!!!!" << std::endl;
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
  selTaus 	= std::vector<Lepton>();
  vetoTaus 	= std::vector<Lepton>();
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
//=============================================================== SELECTED TAUS
Bool_t TauSelector::isGoodTau(Lepton tau){
  looseLeptons  = GetParam<std::vector<Lepton>>("looseLeptons"); 
  if (gSelection == ittHSelec) {
  	// 	Selection of taus for multilepton ttH analysis:
  	// idDecayMode = 1, pt > 20, abs(eta) < 2.3, medium wp in idMVA, no one can
  	// overlap within dR = 0.4 with loose electrons or muons.
  	
  	Bool_t passDecayMode; Bool_t passPt; Bool_t passEta; Bool_t passID;
  	Bool_t passOverlap;
	
	passDecayMode	= (idDecayMode == 1);
	passPt			= (pt > 20);
	passEta			= (abs(eta) < 2.3);
	passID			= (idMVA >= 3);
	
	passOverlap		= 1;
	for (Int_t i = 0; i < looseLeptons.size(); i++){
	  if (tau.p.DeltaR(looseLeptons[i].p) < 0.4) passOverlap = 0;
	}

  	if (!passDecayMode || !passPt || !passEta || !passID || !passOverlap) return false;
	return true;
  } else {
    return false;
  }
}

//============================================== VETO TAUS
Bool_t TauSelector::isVetoTau(Lepton tau){ 
  // (Fill if necessary)
  return false;
}

//============================================== LOOSE TAUS
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
  nTau     = Get<Int_t>("nTauGood"); 
  for(Int_t i = 0; i < nTau; i++){
    GetTauVariables(i);
    tL = Lepton(tP, charge, type);
    if(isGoodTau(tL)){
      tL.SetSF(   TauSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
      tL.SetSFerr(TauSF->GetLeptonSFerror(pt, eta, tL.type) );
      selTaus.push_back(tL);
    }
    if(isVetoTau(tL)){ // If you need to veto extra Taus...
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      vetoTaus.push_back(tL);
    }
    if(isLooseTau(tL)){ // A loose category... used in ttH, for example
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      looseTaus.push_back(tL); 
    }
  }


  nSelTaus 	  = selTaus.size();
  nVetoTaus   = vetoTaus.size();
  nLooseTaus  = vetoTaus.size();
  
  // Set params for the next selectors
  SetParam("selTaus",  	selTaus );
  SetParam("vetoTaus", 	vetoTaus);
  SetParam("looseTaus", looseTaus);
  SetParam("nSelTaus", 	nSelTaus);
  SetParam("nVetoTaus", nVetoTaus);
  SetParam("nLooseTaus",nVetoTaus);
}


//################################################################
//## Get important variables
//################################################################
void TauSelector::GetTauVariables(Int_t i){ // Once per tau, get all the info
  tP.SetPtEtaPhiM(Get<Float_t>("TauGood_pt", i), Get<Float_t>("TauGood_eta", i), Get<Float_t>("TauGood_phi", i), Get<Float_t>("TauGood_mass", i));
  pt 			= tP.Pt();
  eta 			= tP.Eta();
  charge 		= Get<Int_t>("TauGood_charge", i); 
  type 	= 2;
  
  idDecayMode	= Get<Int_t>("TauGood_idDecayMode",i);
  idMVA			= Get<Float_t>("TauGood_idMVA",i);
}


