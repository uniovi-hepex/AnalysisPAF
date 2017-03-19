#include "TWAnalysis.h"

ClassImp(TWAnalysis);
TWAnalysis::TWAnalysis() : PAFChainItemSelector() {

  TWeight         = 0.;
  DilepMETJetPt   = 0.;
  Lep1METJetPt    = 0.;
  DPtDilep_JetMET = 0.;
  DPtDilep_MET    = 0.;
  DPtLep1_MET     = 0.;
  TWeight         = 0.;

  
}
void TWAnalysis::Summary(){}

void TWAnalysis::Initialise(){

  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsTTbar     = false;
  gIsTW        = false;
  if (gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  if (gSampleName.Contains("TW")    || gSampleName.Contains("TbarW") ) gIsTW    = false;


  genLeptons  = std::vector<Lepton>();
  selLeptons  = std::vector<Lepton>();
  selJets = std::vector<Jet>();

  fTW1j1b = CreateTree("1j1b","Created with PAF");
  SetTWVariables();
}

void TWAnalysis::InsideLoop(){
  // Vectors with the objects

  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  GetVarsFromTop();

  

  // Leptons and Jets

  if (gSelection == iTopSelec){
    if (gIsTTbar && genLeptons.size() < 2) return; // Dilepton selection for ttbar!!!
  }
  else if (gSelection == iTWSelec){
    if (gIsTW && genLeptons.size() < 2) return; // Dilepton selection for tw!!
  }

  if (!TIsOSDilep) return;

  Float_t NormWeight = GetParam<Float_t>("NormWeight");
  Float_t lepSF      = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
  Float_t TrigSF     = GetParam<Float_t>("TriggerSF"); TrigSF =1.; // for the moment trigsf = 1
  Float_t PUSF       = GetParam<Float_t>("PUSF");
  TWeight            = NormWeight*lepSF*TrigSF*PUSF;
  
    
  // for the 1b1j
  if (TNJets != 1) return;
  if (TNBtags != 1) return;
  
  CalculateTWVariables();
  fTW1j1b->Fill();

  return;
}


//#####################################################################
// Functions
//------------------------------------------------------------------



void TWAnalysis::GetMET(){
    TMET        = Get<Float_t>("met_pt");
    TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
}


void TWAnalysis::GetVarsFromTop(){
  TIsOSDilep = GetParam<Bool_t>("TIsOSDilep");
  TNJets     = GetParam<Int_t> ("TNJets");
  TNBtags    = GetParam<Int_t> ("TNBtags");
}


// 1j1b
Double_t TWAnalysis::getDilepMETJetPt()
{
  cout << "here" << endl;
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
  cout << "kkkk " << selJets.size() << endl;
  vSystem[3] = selJets[0].p;                                    // jet 1
  return getPtSys(vSystem,4);

}

// 1j1b
Double_t TWAnalysis::getLep1METJetPt()
{
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons[0].p;
  vSystem[1].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  vSystem[2] = selJets[0].p;
  return getPtSys(vSystem,3);
}



Double_t TWAnalysis::getPtSys(TLorentzVector* vSystem, int nSystem)
{
  TLorentzVector vSyst;
  vSyst = vSystem[0];
  for (int i = 1; i < nSystem; ++i){
    vSyst += vSystem[i];
  }
  return vSyst.Pt();
}

// 1j1b
Double_t TWAnalysis::getDPtDilep_JetMET()
{
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons[0].p);
  col1.push_back(selLeptons[1].p);

  col2.push_back(selJets[0].p);
  TLorentzVector met;
  met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  col2.push_back(met);
  return getDeltaPt(col1,col2);

}

// 1j1b
Double_t TWAnalysis::getDPtDilep_MET()
{
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons[0].p);
  col1.push_back(selLeptons[1].p);

  TLorentzVector met;
  met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  col2.push_back(met);
  return getDeltaPt(col1,col2);

}

// 1j1b
Double_t TWAnalysis::getDPtLep1_MET()
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons[0].p);
  TLorentzVector met;
  met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  col2.push_back(met);
  return getDeltaPt(col1,col2);

}

Double_t TWAnalysis::getDeltaPt(vector<TLorentzVector> col1, vector<TLorentzVector> col2){
  TLorentzVector v1, v2;
  if (col1.size()*col2.size() == 0){
    cout << "[TAT::getDeltaPt] One of the collections is empty" << endl;
    return -99.;
  }
  v1 = col1[0];
  v2 = col2[0];
  for (unsigned int i = 1; i < col1.size(); ++i){
    v1 += col1[i];
  }
  for (unsigned int i = 1; i < col2.size(); ++i){
    v2 += col2[i];
  }
  return (v1-v2).Pt();
}

// Double_t TWAnalysis::getM_L1J1()
// {
//   vector<TLorentzVector>  col;
//   col.push_back(selLeptons[0].p);
//   col.push_back(selJets[0].p);
//   return getM(col);
// }

// Double_t TWAnalysis::getM_L1J2()
// {
//   vector<TLorentzVector>  col;
//   col.push_back(selLeptons[0].p);
//   col.push_back(selJets[1].p);
//   return getM(col);
// }
// Double_t TWAnalysis::getM_L2J1()
// {
//   vector<TLorentzVector>  col;
//   col.push_back(selLeptons[1].p);
//   col.push_back(selJets[0].p);
//   return getM(col);
// }
// Double_t TWAnalysis::getM_L2J2()
// {
//   vector<TLorentzVector>  col;
//   col.push_back(selLeptons[1].p);
//   col.push_back(selJets[1].p);
//   return getM(col);
// }
// Double_t TWAnalysis::getM_L2J1J2()
// {
//   vector<TLorentzVector>  col;
//   col.push_back(selLeptons[1].p);
//   col.push_back(selJets[0].p);
//   col.push_back(selJets[1].p);
//   return getM(col);
// }

// Double_t TWAnalysis::getM(vector<TLorentzVector> col)
// {
//   if (col.size() == 0){
//     cout << "[TAT::getM] One of the collections is empty" << endl;
//     return -99.;
//   }
//   TLorentzVector v;
//   v  = col[0];
//   for (unsigned int i = 1; i < col.size(); ++i){
//     v += col[i];
//   }
//   return v.M();  
// }

// Double_t TWAnalysis::getDilepPt()
// {
//   TLorentzVector vSystem[2];
//   vSystem[0] = selLeptons[0].p;
//   vSystem[1] = selLeptons[1].p;
//   return getPtSys(vSystem,2);
// }


// Double_t TWAnalysis::getDPtLep1_Jet1()
// {
//   vector<TLorentzVector> col1;
//   vector<TLorentzVector> col2;

//   col1.push_back(selLeptons[0].p);
//   TLorentzVector met;
//   met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
//   col2.push_back(met);
//   return getDeltaPt(col1,col2);
// }


// Double_t TWAnalysis::getDeltaRDilep_METJets12()
// {
//   vector<TLorentzVector>  col1;
//   vector<TLorentzVector>  col2;
//   col1.push_back(selLeptons[0].p);
//   col1.push_back(selLeptons[1].p);
//   col2.push_back(selJets[0].p);
//   col2.push_back(selJets[1].p);
//   TLorentzVector met;
//   met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
//   col2.push_back(met);
//   return getDeltaR(col1,col2);
// }
// Double_t TWAnalysis::getDeltaRDilep_Jets12()
// {
//   vector<TLorentzVector>  col1;
//   vector<TLorentzVector>  col2;
//   col1.push_back(selLeptons[0].p);
//   col1.push_back(selLeptons[1].p);
//   col2.push_back(selJets[0].p);
//   col2.push_back(selJets[1].p);
//   return getDeltaR(col1,col2);
// }

// Double_t TWAnalysis::getDeltaRLep1_Jet1()
// {
//   vector<TLorentzVector>  col1;
//   vector<TLorentzVector>  col2;
//   col1.push_back(selLeptons[0].p);
//   col2.push_back(selJets[0].p);
//   return getDeltaR(col1,col2);
// }



// Double_t TWAnalysis::getDeltaR(vector<TLorentzVector> col1, vector<TLorentzVector> col2){
//   TLorentzVector v1, v2;
//   if (col1.size()*col2.size() == 0){
//     cout << "[TAT::getDeltaR] One of the collections is empty" << endl;
//     return -99.;
//   }
//   v1 = col1[0];
//   v2 = col2[0];
//   for (unsigned int i = 1; i < col1.size(); ++i){
//     v1 += col1[i];
//   }
//   for (unsigned int i = 1; i < col2.size(); ++i){
//     v2 += col2[i];
//   }
//   return v1.DeltaR(v2);
// }



void TWAnalysis::SetTWVariables()
{
  fTW1j1b->Branch("TWeight"         , &TWeight        , "TWeight/F"      );
  fTW1j1b->Branch("DilepMETJetPt"   , &DilepMETJetPt  , "DilepMETJetPt/F"  );
  fTW1j1b->Branch("Lep1METJetPt"    , &Lep1METJetPt   , "Lep1METJetPt/F"   );
  fTW1j1b->Branch("DPtDilep_JetMET" , &DPtDilep_JetMET, "DPtDilep_JetMET/F");
  fTW1j1b->Branch("DPtDilep_MET"    , &DPtDilep_MET   , "DPtDilep_MET/F"   );
  fTW1j1b->Branch("DPtLep1_MET"     , &DPtLep1_MET    , "DPtLep1_MET/F"    ); 

  

  
  
}


void TWAnalysis::CalculateTWVariables()
{
  DilepMETJetPt    =  getDilepMETJetPt();
  Lep1METJetPt     =  getLep1METJetPt();
  DPtDilep_JetMET  =  getDPtDilep_JetMET();
  DPtDilep_MET     =  getDPtDilep_MET();
  DPtLep1_MET      =  getDPtLep1_MET();    
  
  

}
