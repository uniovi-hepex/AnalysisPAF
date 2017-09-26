#include "WZAnalysis.h"

ClassImp(WZAnalysis);

bool GreaterThan(float i, float j){ return (i > j);}

WZAnalysis::WZAnalysis() : PAFChainItemSelector() {
  fhDummy = 0;
  passMETfilters = 0;
  passTrigger    = 0;
}
void WZAnalysis::Summary(){}

void WZAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  localPath    = GetParam<TString>("WorkingDir");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsTTbar     = false;
  gIsLHE       = false;
  if (gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  if (gSampleName == "TTbar_Powheg") gIsLHE = true;
  leptonSFtop = new LeptonSF(localPath + "/InputFiles/");
  leptonSFEWK = new LeptonSF(localPath + "/InputFiles/");

  if (!gIsData){
    leptonSFtop->loadHisto(iMuonId,   iTight);          //Only top
    leptonSFtop->loadHisto(iMuonIsoTightId,   iTight);//Only top
    leptonSFtop->loadHisto(iElecId,   iTight);        //Only top
    leptonSFEWK->loadHisto(iMuonEWKinoID);//Only MVA
    leptonSFEWK->loadHisto(iMuonEWKinomvaM);//Only MVA
    leptonSFEWK->loadHisto(iElecEWKinoID);//Only MVA
    leptonSFEWK->loadHisto(iElecEWKinomvaM);//Only MVA
  }
  makeTree = true;
  makeHistos = true;
  if(makeTree){
    for(int i = 0; i < nWPoints; i++){
      fTree[i]   = CreateTree(sWPoints[i],"Created with PAF");
      SetLeptonVariables(fTree[i]);
      SetJetVariables(fTree[i]);
      SetEventVariables(fTree[i]);
    }
  }
  InitHistos();
}

void WZAnalysis::InsideLoop(){
  // Vectors with the objects
  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  foLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  looseLeptons = GetParam<vector<Lepton>>("looseLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  selJetsJecUp   = GetParam<vector<Jet>>("selJetsJecUp");
  selJetsJecDown = GetParam<vector<Jet>>("selJetsJecDown");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  vetoJets    = GetParam<vector<Jet>>("vetoJets");
  genJets     = GetParam<vector<Jet>>("genJets");
  mcJets      = GetParam<vector<Jet>>("mcJets");
  // Weights and SFs
  NormWeight = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSFerr    = GetParam<Float_t>("TriggerSFerr");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");


  // Event variables
  gChannel       = GetParam<Int_t>("gChannel");
  passMETfilters = GetParam<Bool_t>("METfilters");
  passTrigger    = GetParam<Bool_t>("passTrigger");
  TEvtNum        = Get<ULong64_t>("evt");

  getMatchGenSelLeptons(selLeptons, genLeptons, 0.3); // Match gen and sel

  for (int wP = 0; wP < nWPoints; wP++){
    // Leptons and Jets

      tightLeptons = {};
      fakeableLeptons = {};
      GetLeptonsByWP(wP);
      GetLeptonVariables(tightLeptons, fakeableLeptons, looseLeptons);
      GetJetVariables(selJets, Jets15);
      GetGenJetVariables(genJets, mcJets);
      GetMET();
  
    if(TNTightLeps == 3 && passTrigger && passMETfilters){ // trilepton event with OSSF + l, passes trigger and MET filters
      // Deal with weights:
      Float_t lepSF   = tightLeptons.at(0).GetSF(0)*tightLeptons.at(1).GetSF(0)*tightLeptons.at(2).GetSF(0);
      Float_t ElecSF = 1; Float_t MuonSF = 1;
      Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
      Float_t stat = 0; 
      //For muons
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffsRun2
      //Additional 1% for ID + 0.5% for Isolation + 0.5% single muon triggers
  
      if(TChannel == iElElEl){
        ElecSF   = tightLeptons.at(0).GetSF( 0)*tightLeptons.at(1).GetSF( 0)*tightLeptons.at(2).GetSF( 0);
        ElecSFUp = tightLeptons.at(0).GetSF( 1)*tightLeptons.at(1).GetSF( 1)*tightLeptons.at(2).GetSF( 1);
        ElecSFDo = tightLeptons.at(0).GetSF(-1)*tightLeptons.at(1).GetSF(-1)*tightLeptons.at(2).GetSF(-1);
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
      }
      else if(TChannel == iMuMuMu){
        MuonSF   = tightLeptons.at(0).GetSF( 0)*tightLeptons.at(1).GetSF( 0)*tightLeptons.at(2).GetSF( 0);
        MuonSFUp = tightLeptons.at(0).GetSF( 1)*tightLeptons.at(1).GetSF( 1)*tightLeptons.at(2).GetSF( 1);
        MuonSFDo = tightLeptons.at(0).GetSF(-1)*tightLeptons.at(1).GetSF(-1)*tightLeptons.at(2).GetSF(-1);
        ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
      }
      else{
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
        for (int i = 0; i <3; i ++){
          if (tightLeptons.at(i).isMuon){
            MuonSF   *= tightLeptons.at(i).GetSF( 0);
            MuonSFUp *= tightLeptons.at(i).GetSF( 1);
            MuonSFDo *= tightLeptons.at(i).GetSF(-1);        
          }
          else{
            ElecSF   *= tightLeptons.at(i).GetSF( 0);
            ElecSFUp *= tightLeptons.at(i).GetSF( 1);
            ElecSFDo *= tightLeptons.at(i).GetSF(-1);        
          }
        }
      }
  
      TWeight             = NormWeight*ElecSF*MuonSF*TrigSF*PUSF;
      TIsSR   = false;
      TIsCRDY = false;
      TIsCRTT = false;  
      TIsNewCRDY = false;
      TIsNewCRTT = false;  
      if(gIsData) TWeight = 1;
      // Event Selection
      // ===================================================================================================================

      if(TNTightLeps == 3 && TNOSSF > 0 && passesMCTruth(tightLeptons, genLeptons)){
        std::vector<Lepton> tempLeps = AssignWZLeptons(tightLeptons);
        lepZ1 = tempLeps.at(0);
        lepZ2 = tempLeps.at(1);
        lepW  = tempLeps.at(2);
        TLorentzVector metVector = TLorentzVector();
        metVector.SetPtEtaPhiM(TMET, TMET_Phi, 0., 0.);
        TM3l = (lepZ1.p + lepZ2.p + lepW.p).M();
        TMtWZ = (lepZ1.p + lepZ2.p + lepW.p + metVector).Mt();
        TMtW  = (lepW.p + metVector).Mt();
        TMll  = (lepZ1.p + lepZ2.p).M();

        if (lepZ1.Pt() > 25 && lepZ2.Pt() > 10 && lepW.Pt() > 25){//3 lepton, has OSSF, leptons assigned to W and Z. Fill histos from here onwards
        
          if(TMath::Abs(TMll - nomZmass)< 15. && TMinMll > 4. && (lepZ1.p + lepZ2.p + lepW.p).M() > 100.  ){ //  Z window + exlcude low masses + M_3l selection 
            // The last two cuts define the Control/Signal regions
            
            // Signal Region
            if(TMET > 30.){   // MET > 30 always

              if(TNBtags == 0){ //Exactly 0 btags
                TIsSR   = true;
              }
              else if(TNBtags > 0 && (TNOSSF == 0 || (TNOSSF > 0 && TMath::Abs(TM3l - nomZmass) > 5))){ //1 or more btags
                TIsCRTT = true ;
              }
            }
            else if (TMET < 30. && TNBtags == 0){
              TIsCRDY = true ;
            }

          }
          if (TMath::Abs(TMll - nomZmass)< 15. && TMinMll > 4. && TMET < 30.){
            TIsNewCRDY = true;
          }
          if (TMath::Abs(TMll - nomZmass)> 15. && TMath::Abs(TM3l - nomZmass) > 5 &&  TMinMll > 4. && (lepZ1.p + lepZ2.p + lepW.p).M() > 100. && TMET > 30.){
            TIsNewCRTT = true;
          }
        fTree[wP] -> Fill();  
        }
      }   
    }
  }
}


//#####################################################################
// Initialize histograms/Minitrees
//------------------------------------------------------------------
void WZAnalysis::InitHistos(){
}

void WZAnalysis::SetLeptonVariables(TTree* iniTree){
  iniTree->Branch("TNFOLeps",     &TNFOLeps,     "TNFOLeps/I");
  iniTree->Branch("TNTightLeps",     &TNTightLeps,     "TNTightLeps/I");
  iniTree->Branch("TLep_Pt",     TLep_Pt,     "TLep_Pt[TNTightLeps]/F");
  iniTree->Branch("TLep_Eta",     TLep_Eta,     "TLep_Eta[TNTightLeps]/F");
  iniTree->Branch("TLep_Phi",     TLep_Phi,     "TLep_Phi[TNTightLeps]/F");
  iniTree->Branch("TLep_E" ,     TLep_E ,     "TLep_E[TNTightLeps]/F");
  iniTree->Branch("TLep_Charge",  TLep_Charge, "TLep_Charge[TNTightLeps]/F");
  iniTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  iniTree->Branch("TMll",        &TMll,      "TMll/F");
  iniTree->Branch("TM3l",        &TM3l,      "TM3l/F");
  iniTree->Branch("TMtW",        &TMtW,      "TMtW/F");
  iniTree->Branch("TMtWZ",        &TMtWZ,      "TMtWZ/F");
  iniTree->Branch("TNOSSF",      &TNOSSF,      "TNOSSF/I");
  iniTree->Branch("TMinMll",      &TMinMll,      "TMinMll/F");
}

void WZAnalysis::SetJetVariables(TTree* iniTree){
  iniTree->Branch("TNJets",           &TNJets,         "TNJets/I");
  iniTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
  iniTree->Branch("TJet_isBJet",       TJet_isBJet,       "TJet_isBJet[TNJets]/I");
  iniTree->Branch("TJet_Pt",           TJet_Pt,           "TJet_Pt[TNJets]/F");
  iniTree->Branch("TJet_Eta",           TJet_Eta,           "TJet_Eta[TNJets]/F");
  iniTree->Branch("TJet_Phi",           TJet_Phi,           "TJet_Phi[TNJets]/F");
  iniTree->Branch("TJet_E",            TJet_E,            "TJet_E[TNJets]/F");
  iniTree->Branch("THT",          &THT,          "THT/F");
}

void WZAnalysis::SetEventVariables(TTree* iniTree){
  iniTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  iniTree->Branch("TIsSR"  ,      &TIsSR  ,      "TIsSR/B"  );
  iniTree->Branch("TIsCRTT",      &TIsCRTT,      "TIsCRTT/B");
  iniTree->Branch("TIsCRDY",      &TIsCRDY,      "TIsCRDY/B");
  iniTree->Branch("TIsNewCRTT",      &TIsNewCRTT,      "TIsNewCRTT/B");
  iniTree->Branch("TIsNewCRDY",      &TIsNewCRDY,      "TIsNewCRDY/B");
  iniTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  iniTree->Branch("TMET",         &TMET,         "TMET/F");
  iniTree->Branch("TEvtNum",         &TEvtNum,         "TEvtNum/l");
  iniTree->Branch("TGenMET",         &TGenMET,         "TGenMET/F");
  iniTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
}

Bool_t WZAnalysis::passesMCTruth(std::vector<Lepton> sLep, std::vector<Lepton> gLep){
  Bool_t pass = true;
  if (!gIsData){
    for (int i = 0; i < sLep.size(); i++){
      if (sLep.at(i).lepMatch == -1) pass = false;
      else {
        Lepton theLep = sLep.at(i);
        Lepton genMatch = gLep.at(sLep.at(i).lepMatch);
        if (!genMatch.isPrompt && !theLep.isPrompt){
          pass = false;
        }
      } 
    }
  }
  return pass;
}

//#####################################################################
// Get Variables
//------------------------------------------------------------------

void WZAnalysis::GetLeptonsByWP(Int_t wPValue){
  Int_t nFakeableLeptons = foLeptons.size();
  Int_t nTightLeptons = selLeptons.size();

  if (wPValue == top){
    for (int k = 0; k < nTightLeptons; k++){ // No FO for top ID
      if (selLeptons.at(k).idMVA >= 10){
        tightLeptons.push_back(selLeptons.at(k));
        fakeableLeptons.push_back(selLeptons.at(k));
        tightLeptons.back().SetSF(selLeptons.at(k).GetSF(0)*leptonSFtop->GetLeptonSF(selLeptons.at(k).Pt(), selLeptons.at(k).Eta(), selLeptons.at(k).type));
        fakeableLeptons.back().SetSF(selLeptons.at(k).GetSF(0)*leptonSFtop->GetLeptonSF(selLeptons.at(k).Pt(), selLeptons.at(k).Eta(), selLeptons.at(k).type));
      }
    }
  }
  else {
    for (int k = 0; k < nFakeableLeptons; k++){
      if (foLeptons.at(k).idMVA%10 > wPValue){
        fakeableLeptons.push_back(foLeptons.at(k));
        fakeableLeptons.back().SetSF(foLeptons.at(k).GetSF(0)*leptonSFEWK->GetLeptonSF(foLeptons.at(k).Pt(), foLeptons.at(k).Eta(), foLeptons.at(k).type));
      }
    }
  
    for (int k = 0; k < nTightLeptons; k++){
      if (selLeptons.at(k).idMVA%10 > wPValue){
        tightLeptons.push_back(selLeptons.at(k));
        tightLeptons.back().SetSF(selLeptons.at(k).GetSF(0)*leptonSFtop->GetLeptonSF(selLeptons.at(k).Pt(), selLeptons.at(k).Eta(), selLeptons.at(k).type));
      }
    }
  }
}

void WZAnalysis::GetLeptonVariables(std::vector<Lepton> tightLeptons, std::vector<Lepton> foLeptons, std::vector<Lepton> looseLeptons){
  TNTightLeps = tightLeptons.size();
  Int_t nVetoLeptons = foLeptons.size();
  TNFOLeps = nVetoLeptons;
  for(Int_t i = 0; i < TNTightLeps; i++){
    TLep_Pt[i]     = tightLeptons.at(i).Pt();    
    TLep_Eta[i]    = tightLeptons.at(i).Eta();    
    TLep_Phi[i]    = tightLeptons.at(i).Phi();    
    TLep_E[i]      = tightLeptons.at(i).E();    
    TLep_Charge[i] = tightLeptons.at(i).charge;
  }
  //Require exactly 3 leptons 
  if(TNTightLeps != 3 ) gChannel = -1;
  //Charge compatibility with WZ production
  else if(TMath::Abs(tightLeptons.at(0).charge + tightLeptons.at(1).charge + tightLeptons.at(2).charge) != 1) gChannel = -1;
  //Combinatory of posible final leptons
  else if(tightLeptons.at(0).isMuon && tightLeptons.at(1).isMuon && tightLeptons.at(2).isMuon) gChannel = iMuMuMu;
  else if(tightLeptons.at(0).isMuon && tightLeptons.at(1).isMuon && tightLeptons.at(2).isElec) gChannel = iElMuMu;
  else if(tightLeptons.at(0).isMuon && tightLeptons.at(1).isElec && tightLeptons.at(2).isMuon) gChannel = iElMuMu;
  else if(tightLeptons.at(0).isMuon && tightLeptons.at(1).isElec && tightLeptons.at(2).isElec) gChannel = iElElMu;
  else if(tightLeptons.at(0).isElec && tightLeptons.at(1).isMuon && tightLeptons.at(2).isMuon) gChannel = iElMuMu;
  else if(tightLeptons.at(0).isElec && tightLeptons.at(1).isMuon && tightLeptons.at(2).isElec) gChannel = iElElMu;
  else if(tightLeptons.at(0).isElec && tightLeptons.at(1).isElec && tightLeptons.at(2).isMuon) gChannel = iElElMu;
  else if(tightLeptons.at(0).isElec && tightLeptons.at(1).isElec && tightLeptons.at(2).isElec) gChannel = iElElEl;
  TMinMll = 100000;
  TNOSSF = 0;
  for(Int_t i = 0; i < TNTightLeps; i++){
    for(Int_t j = i+1; j < TNTightLeps; j++){
      if (tightLeptons.at(j).isMuon && tightLeptons.at(i).isMuon && tightLeptons.at(i).charge*tightLeptons.at(j).charge == -1)           TNOSSF++;
      if (tightLeptons.at(j).isElec && tightLeptons.at(i).isElec && tightLeptons.at(i).charge*tightLeptons.at(j).charge == -1)           TNOSSF++;
      Float_t hypMll = (tightLeptons.at(j).p + tightLeptons.at(i).p).M();
      if (hypMll < TMinMll) TMinMll = hypMll;
    }
  }

  TChannel = gChannel;
  gChannel = gChannel -1; // gchannel used for chan index of histograms
}

void WZAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0;
  for(Int_t i = 0; i < TNJets; i++){
    TJet_Pt[i]     = selJets.at(i).Pt();
    TJet_Eta[i]    = selJets.at(i).Eta();
    TJet_Phi[i]    = selJets.at(i).Phi();
    TJet_E[i]      = selJets.at(i).E();
    TJet_isBJet[i] = selJets.at(i).isBtag;
    THT += TJet_Pt[i];
    if(selJets.at(i).isBtag)            TNBtags++;
  }
  SetParam("THT",THT);
}

void WZAnalysis::GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets){
  if(gIsData) return;
  nFiduJets = 0; nFidubJets = 0; 
  Int_t nGenJets = genJets.size();
  Int_t nmcJets = mcJets.size();
  for(Int_t i = 0; i < nGenJets; i++) if(genJets.at(i).p.Pt() > 30 && TMath::Abs(genJets.at(i).p.Eta()) < 2.4)                         nFiduJets++;
  for(Int_t i = 0; i <  nmcJets; i++) if(mcJets.at(i).p.Pt()  > 30 && TMath::Abs(mcJets.at(i).Eta())    < 2.4 && mcJets.at(i).isBtag)  nFidubJets++;
}

void WZAnalysis::GetMET(){
    TMET        = Get<Float_t>("met_pt");
    TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
    if(gIsData) return;
    TGenMET     = Get<Float_t>("met_genPt");
  if(gIsLHE)  for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

//#####################################################################
// Fill histograms/Minitrees
//------------------------------------------------------------------

void WZAnalysis::FillDYHistos(Int_t ch){
}

void WZAnalysis::FillHistos(Int_t ch, Int_t cut){
  if(!makeHistos) return;
  Int_t sys = 0;
}
