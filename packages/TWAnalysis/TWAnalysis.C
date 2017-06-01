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
  selJets     = std::vector<Jet>();
  Jets15      = std::vector<Jet>();
  vetoJets    = std::vector<Jet>();
  fTW1j1b = CreateTree("1j1b","Created with PAF");
  SetTWVariables();
  SetHistos();
  setBDT();
}

void TWAnalysis::SetHistos()
{

  fHGenB_eta = CreateH1F("HGenB_eta","", 100, -5., 5.);
  fHGenB_pt  = CreateH1F("HGenB_pt" ,"", 100, 0., 600.);
  fHGenB_RecoJetDR   = CreateH1F("fHGenB_RecoJetDR" , "", 50, 0., 5.); 
  fHNGenB_unmatched  = CreateH1F("fHNGenB_unmatched", "",  5, -0.5,4.5);

  fHGenB_eta_pt  = CreateH2F("HGenB_pt_eta" ,"", 20, -5., 5.,20, 0., 600.);

  fHNCSV  = CreateH1F("fHCSV", "",  100, -12.,1.4);

}

void TWAnalysis::setBDT()
{
  return;
  Float_t nb = float(nBLooseCentral + nBLooseFwd);
  Float_t pz = TMath::Abs(DilepMETJet1Pz);
  Float_t nlooseCentral = nLooseCentral;   // tmva doesnt like integers
  Float_t nlooseFwd     = nLooseFwd;

  BDT = new TMVA::Reader();
  BDT->AddVariable( "MSys"                       , &MSys            );
  BDT->AddVariable( "DilepMETJetPt"              , &DilepMETJetPt   );
  BDT->AddVariable( "Lep1METJetPt"               , &Lep1METJetPt    );           
  BDT->AddVariable( "DPtDilep_JetMET"            , &DPtDilep_JetMET );
  BDT->AddVariable( "DPtDilep_MET"               , &DPtDilep_MET    );         
  BDT->AddVariable( "DPtLep1_MET"                , &DPtLep1_MET     );           
  BDT->AddVariable( "TMath::Abs(DilepMETJet1Pz)" , &pz	      );
  BDT->AddVariable( "nLooseCentral"              , &nlooseCentral   );
  BDT->AddVariable( "nLooseFwd"                  , &nlooseFwd       );        
  BDT->AddVariable( "TJet2csv"                   , &TJet2csv        );           
  BDT->AddVariable( "nBLooseCentral + nBLooseFwd", &nb              );   
  
  BDT->BookMVA("BDTG","/mnt_pool/fanae105/user/sscruz/TW/AnalysisPAF/plotter/TW/dark-matter/weights/TMVAClassification_BDTG.weights.xml");

}

void TWAnalysis::InsideLoop(){
  // Vectors with the objects

  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  vetoJets    = GetParam<vector<Jet>>("vetoJets");
  GetVarsFromTop();
  for (unsigned int k = 0; k < selJets.size(); ++k){
    fHNCSV->Fill( selJets[k].csv );
  }
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
  if (TNJets  == 1 && TNBtags == 1){
    CalculateTWVariables();
    GetGenStuff();
  }

  if (GetParam<Bool_t>("TWEventToBeFilled"))
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
  THT        = GetParam<Float_t>("THT");
}


// 1j1b
Double_t TWAnalysis::getDilepMETJetPt()
{
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
  vSystem[3] = selJets[0].p;                                    // jet 1
  return getPtSys(vSystem,4);

}

Double_t TWAnalysis::getDilepJetPt()
{
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  vSystem[2] = selJets[0].p;                                    // jet 1
  return getPtSys(vSystem,3);

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

Double_t TWAnalysis::getDilepMETJet1Pz()
{
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
  vSystem[3] = selJets[0].p;                                    // jet 1
  return getPzSys(vSystem,4);

}

Double_t TWAnalysis::getPzSys(TLorentzVector* vSystem, int nSystem){
 TLorentzVector vSyst;
  vSyst = vSystem[0];
  for (int i = 1; i < nSystem; ++i){
    vSyst += vSystem[i];
  }
  return vSyst.Pz();
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

Double_t TWAnalysis::getSysM()
{
  vector<TLorentzVector> col;
  TLorentzVector met;
  met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  col.push_back(met);
  col.push_back(selJets[0].p);
  col.push_back(selLeptons[0].p);
  col.push_back(selLeptons[1].p);
  return getM(col);

}


Double_t TWAnalysis::getM(vector<TLorentzVector> col)
{
  if (col.size() == 0){
    cout << "[TAT::getM] One of the collections is empty" << endl;
    return -99.;
  }
  TLorentzVector v;
  v  = col[0];
  for (unsigned int i = 1; i < col.size(); ++i){
    v += col[i];
  }
  return v.M();  
}

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

  // for BDT
  fTW1j1b->Branch("DilepMETJetPt"   , &DilepMETJetPt  , "DilepMETJetPt/F"  );
  fTW1j1b->Branch("Lep1METJetPt"    , &Lep1METJetPt   , "Lep1METJetPt/F"   );
  fTW1j1b->Branch("DPtDilep_JetMET" , &DPtDilep_JetMET, "DPtDilep_JetMET/F");
  fTW1j1b->Branch("DPtDilep_MET"    , &DPtDilep_MET   , "DPtDilep_MET/F"   );
  fTW1j1b->Branch("DPtLep1_MET"     , &DPtLep1_MET    , "DPtLep1_MET/F"    ); 
  fTW1j1b->Branch("DilepMETJet1Pz"  , &DilepMETJet1Pz , "DilepMETJet1Pz/F" );
  fTW1j1b->Branch("nLooseCentral"   , &nLooseCentral  , "nLooseCentral/I"  );
  fTW1j1b->Branch("nLooseFwd"       , &nLooseFwd      , "nLooseFwd/I"      );
  fTW1j1b->Branch("nBLooseCentral"  , &nBLooseCentral , "nBLooseCentral/I" );
  fTW1j1b->Branch("nBLooseFwd"      , &nBLooseFwd     , "nBLooseFwd/I"     );
  fTW1j1b->Branch("TJet2csv"        , &TJet2csv       , "TJet2csv/F"       );
  fTW1j1b->Branch("MSys"            , &MSys           , "MSys/F"           );
  fTW1j1b->Branch("THT"             , &THT            , "THT/F"            );
  fTW1j1b->Branch("TJet1_pt"        , &TJet1_pt       , "TJet1_pt/F"       );
  fTW1j1b->Branch("TJet2pt"         , &TJet2pt        , "TJet2pt/F"        ); // loose jet pt
  fTW1j1b->Branch("TLep1_pt"        , &TLep1_pt       , "TLep1_pt/F"       );
  fTW1j1b->Branch("TLep2_pt"        , &TLep2_pt       , "TLep2_pt/F"       );
  fTW1j1b->Branch("TMET"            , &TMET           , "TMET/F"           );
  fTW1j1b->Branch("C_jll"           , &C_jll          , "C_jll/F"          );
  fTW1j1b->Branch("DilepJetPt"      , &DilepJetPt     , "DilepJetPt/F"     );
  
  // for DNN
  // fTW1j1b->Branch("TJet1_Dphi"      , &TJet1_Dphi     , "TJet1_Dphi/F"    );    
  // fTW1j1b->Branch("TJet1_pz"        , &TJet1_pz       , "TJet1_pz/F"      );
  // fTW1j1b->Branch("TJet1_E"         , &TJet1_E        , "TJet1_E/F"       );
  // fTW1j1b->Branch("TLep1_pt"        , &TLep1_pt       , "TLep1_pt/F"      );
  // fTW1j1b->Branch("TLep1_Dphi"       , &TLep1_Dphi      , "TLep1_Dphi/F"  );
  // fTW1j1b->Branch("TLep1_pz"        , &TLep1_pz       , "TLep1_pz/F"      );
  // fTW1j1b->Branch("TLep1_E"         , &TLep1_E        , "TLep1_E/F"       );
  // fTW1j1b->Branch("TLep2_pt"        , &TLep2_pt       , "TLep2_pt/F"      );
  // fTW1j1b->Branch("TLep2_Dphi"       , &TLep2_Dphi      , "TLep2_Dphi/F"  );
  // fTW1j1b->Branch("TLep2_pz"        , &TLep2_pz       , "TLep2_pz/F"      );
  // fTW1j1b->Branch("TLep2_E"         , &TLep2_E        , "TLep2_E/F"       );
  // fTW1j1b->Branch("TMET"            , &TMET           , "TMET/F"          );

  // fTW1j1b->Branch("TBDT"            , &TBDT           , "TBDT/F"          );
}


void TWAnalysis::CalculateTWVariables()
{
  DilepMETJetPt    =  getDilepMETJetPt()   ;
  DilepJetPt       =  getDilepJetPt()      ;
  Lep1METJetPt     =  getLep1METJetPt()    ;
  DPtDilep_JetMET  =  getDPtDilep_JetMET() ;
  DPtDilep_MET     =  getDPtDilep_MET()    ;
  DPtLep1_MET      =  getDPtLep1_MET()     ;    
  DilepMETJet1Pz   =  getDilepMETJet1Pz()  ;
  get20Jets();
  MSys             = getSysM();

  TLorentzVector met; 
  met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);

  TJet1_pt  = selJets[0].p.Pt();
  TJet1_Dphi = met.DeltaPhi(selJets[0].p);
  TJet1_pz  = selJets[0].p.Pz();
  TJet1_E   = selJets[0].p.E();
  TLep1_pt  = selLeptons[0].p.Pt();
  TLep1_Dphi = met.DeltaPhi(selLeptons[0].p);
  TLep1_pz  = selLeptons[0].p.Pz();
  TLep1_E   = selLeptons[0].p.E();
  TLep2_pt  = selLeptons[0].p.Pt();
  TLep2_Dphi = met.DeltaPhi(selLeptons[0].p);
  TLep2_pz  = selLeptons[0].p.Pz();
  TLep2_E   = selLeptons[0].p.E();

  C_jll = (selJets[0].p + selLeptons[0].p + selLeptons[1].p).Et() / (selJets[0].p + selLeptons[0].p + selLeptons[1].p).E();
  TBDT = -1.;
  // TBDT = BDT->EvaluateMVA("BDTG");
  return;


}


void TWAnalysis::GetGenStuff()
{
  // get bjets
  vector<TLorentzVector> genB; 
  for (int gen = 0; gen < Get<Int_t>("nGenPart"); ++gen){
    if (TMath::Abs(Get<Int_t>("GenPart_pdgId",gen)) == 5){
      TLorentzVector b;
      b.SetPtEtaPhiM(Get<Float_t>("GenPart_pt",gen),
		     Get<Float_t>("GenPart_eta",gen),
		     Get<Float_t>("GenPart_phi",gen),
		     Get<Float_t>("GenPart_mass",gen));
      genB.push_back(b);
    }
  }
  // match to leading jet (which a bjet0
  if (TNJets == 1 && TNBtags == 1){
    int n = 0;
    for (unsigned int k = 0; k < genB.size(); ++k){
      TLorentzVector b = genB[k];
      if (b.DeltaR(selJets[0].p) > 0.3){
	n++;
	// this b is not matched to anyhting
	fHGenB_pt->Fill(b.Pt());
	fHGenB_eta->Fill(b.Eta());
	fHGenB_eta_pt->Fill(b.Eta(),b.Pt());
	fHGenB_RecoJetDR->Fill( b.DeltaR(selJets[0].p));
	// 
	
      }
    }
    fHNGenB_unmatched->Fill(n);
  }
  return;
  
}


void TWAnalysis::get20Jets()
{
  nLooseCentral  = 0.;
  nLooseFwd      = 0.;
  nBLooseCentral = 0.;
  nBLooseFwd     = 0.;
  TJet2csv       = 0.;
  TJet2pt        = 0.;
  
  for (unsigned int j = 0; j < vetoJets.size(); ++j){
    if (vetoJets[j].p.Pt() < 20.) continue;
    if (TMath::Abs(vetoJets[j].p.Eta()) < 2.4) nLooseCentral++;
    else nLooseFwd++;
    if (!vetoJets[j].isBtag) continue;
    if (TMath::Abs(vetoJets[j].p.Eta()) < 2.4) nBLooseCentral++;
    else nBLooseFwd++;
  }
  if (nLooseFwd + nLooseCentral > 1){
    TJet2csv = TMath::Max( vetoJets[1].csv   , 0.);
    TJet2pt = TMath::Max( vetoJets[1].p.Pt(), 0.);
  }

  return;
}
