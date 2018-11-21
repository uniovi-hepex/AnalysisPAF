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
  leptonSFEWKM = new LeptonSF(localPath + "/InputFiles/");
  leptonSFEWKVT = new LeptonSF(localPath + "/InputFiles/");

  if (!gIsData){
    leptonSFtop->loadHisto(iMuonId,   iTight);          //Only top
    leptonSFtop->loadHisto(iMuonIsoTightId,   iTight);//Only top
    leptonSFtop->loadHisto(iElecId,   iTight);        //Only top
    leptonSFEWKM->loadHisto(iMuonEWKinoID);//Only MVA
    leptonSFEWKM->loadHisto(iMuonEWKinomvaM);//Only MVA
    leptonSFEWKM->loadHisto(iElecEWKinoID);//Only MVA
    leptonSFEWKM->loadHisto(iElecEWKinomvaM);//Only MVA
    leptonSFEWKVT->loadHisto(iMuonEWKinoID);//Only MVA
    leptonSFEWKVT->loadHisto(iMuonEWKinomvaVT);//Only MVA
    leptonSFEWKVT->loadHisto(iElecEWKinoID);//Only MVA
    leptonSFEWKVT->loadHisto(iElecEWKinomvaVT);//Only MVA
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
  genParticles= GetParam<vector<Lepton>>("genParticles");
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



  for (int wP = 0; wP < nWPoints; wP++){
    // Leptons and Jets
    //std::cout << "Create vectors\n";
      tightLeptons = {};
      fakeableLeptons = {};
      GetLeptonsByWP(wP);
    //std::cout << "Get Tight/FO\n";
      GetLeptonVariables(tightLeptons, fakeableLeptons, looseLeptons);
    //std::cout << "Get Lepton\n";
      GetJetVariables(selJets, Jets15);
    //std::cout << "Get Jet\n";
      GetGenJetVariables(genJets, mcJets);
    //std::cout << "Get GenVariables\n";

      GetMET();

    //std::cout << "Match gen/reco\n";
      //fakeableLeptons = getMatchGenSelLeptons(fakeableLeptons, genLeptons, 0.3, true); // Match gen and sel Leptons, require same Id
      //fakeableLeptons = getMatchGenSelLeptons(fakeableLeptons, genParticles, 0.3, false); // Match gen particles and sel Leptons, do not require same Id (allow for taus)

    if(TNFOLeps == 3 && TNOSSF > 0 && passTrigger && passMETfilters){ // trilepton event with OSSF + l, passes trigger and MET filters
      // Deal with weights:
    //std::cout << "Pass 3FO\n";
      Float_t lepSF   = fakeableLeptons.at(0).GetSF(0)*fakeableLeptons.at(1).GetSF(0)*fakeableLeptons.at(2).GetSF(0);
      Float_t ElecSF = 1; Float_t MuonSF = 1;
      Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
      Float_t stat = 0; 
      //For muons
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffsRun2
      //Additional 1% for ID + 0.5% for Isolation + 0.5% single muon triggers
  
      if(TChannel == iElElEl){
        ElecSF   = fakeableLeptons.at(0).GetSF( 0)*fakeableLeptons.at(1).GetSF( 0)*fakeableLeptons.at(2).GetSF( 0);
        ElecSFUp = fakeableLeptons.at(0).GetSF( 1)*fakeableLeptons.at(1).GetSF( 1)*fakeableLeptons.at(2).GetSF( 1);
        ElecSFDo = fakeableLeptons.at(0).GetSF(-1)*fakeableLeptons.at(1).GetSF(-1)*fakeableLeptons.at(2).GetSF(-1);
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
      }
      else if(TChannel == iMuMuMu){
        MuonSF   = fakeableLeptons.at(0).GetSF( 0)*fakeableLeptons.at(1).GetSF( 0)*fakeableLeptons.at(2).GetSF( 0);
        MuonSFUp = fakeableLeptons.at(0).GetSF( 1)*fakeableLeptons.at(1).GetSF( 1)*fakeableLeptons.at(2).GetSF( 1);
        MuonSFDo = fakeableLeptons.at(0).GetSF(-1)*fakeableLeptons.at(1).GetSF(-1)*fakeableLeptons.at(2).GetSF(-1);
        ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
      }
      else{
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
        for (int i = 0; i <3; i ++){
          if (fakeableLeptons.at(i).isMuon){
            MuonSF   *= fakeableLeptons.at(i).GetSF( 0);
            MuonSFUp *= fakeableLeptons.at(i).GetSF( 1);
            MuonSFDo *= fakeableLeptons.at(i).GetSF(-1);        
          }
          else{
            ElecSF   *= fakeableLeptons.at(i).GetSF( 0);
            ElecSFUp *= fakeableLeptons.at(i).GetSF( 1);
            ElecSFDo *= fakeableLeptons.at(i).GetSF(-1);        
          }
        }
      }
  
      TWeight = NormWeight*ElecSF*MuonSF*TrigSF*PUSF;
      TIsSR   = false;
      TIsCRDY = false;
      TIsCRTT = false;  
      TIsNewCRDY = false;
      TIsNewCRTT = false;  
      if(gIsData) TWeight = 1;
      // Event Selection
      // ===================================================================================================================
      TM3l  = -999;
      TMtWZ = -999;
      TMtW  = -999;
      TMll  = -999;
      TMZ1W = -999;
      TMZ2W = -999;
      std::cout << "ASSIGNS\n";
      std::vector<Lepton> tempLeps = AssignWZLeptons(fakeableLeptons);
      tempLeps = getMatchGenSelLeptons(tempLeps, tightLeptons, 0.4, true);

      lepZ1 = tempLeps.at(0);
      lepZ2 = tempLeps.at(1);
      lepW  = tempLeps.at(2);

      TLep_PtZ1 = lepZ1.Pt();
      TLep_EtaZ1 = lepZ1.Eta();
      TLep_PhiZ1 = lepZ1.Phi();
      TLep_EZ1 = lepZ1.E();
      TLep_ChargeZ1 = lepZ1.charge;
      TLep_IsTightZ1 =  (lepZ1.lepMatch == 0) ? 0 : 1;
      TLep_pdgIdZ1 = lepZ1.type;

      TLep_PtZ2 = lepZ2.Pt();
      TLep_EtaZ2 = lepZ2.Eta();
      TLep_PhiZ2 = lepZ2.Phi();
      TLep_EZ2 = lepZ2.E();
      TLep_ChargeZ2 = lepZ2.charge;
      TLep_IsTightZ2 =  (lepZ2.lepMatch == 0) ? 0 : 1;
      TLep_pdgIdZ2 = lepZ2.type;

      TLep_PtW = lepW.Pt();
      TLep_EtaW = lepW.Eta();
      TLep_PhiW = lepW.Phi();
      TLep_EW = lepW.E();
      TLep_ChargeW = lepW.charge;
      TLep_IsTightW =  (lepW.lepMatch == 0) ? 0 : 1;
      TLep_pdgIdW = lepW.type;

      std::cout << "REMATCHES\n";
      TLep_IsPromptW = lepW.idDecayMode;
      TLep_IsPromptZ2 = lepZ2.idDecayMode;
      TLep_IsPromptZ1 = lepZ1.idDecayMode;

      for(Int_t i = 0; i < TNFOLeps; i++){
        TLep_isConvVeto[i]  = tempLeps.at(i).isConvVeto;
      }
      TLorentzVector metVector = TLorentzVector();
      metVector.SetPtEtaPhiM(TMET, TMET_Phi, 0., 0.);
      TM3l = (lepZ1.p + lepZ2.p + lepW.p).M();
      TMtWZ = (lepZ1.p + lepZ2.p + lepW.p + metVector).Mt();

      TMtW  = (lepW.p + metVector).Mt();
      TMll  = (lepZ1.p + lepZ2.p).M();
      TMZ1W  = (lepZ1.p + lepW.p).M();
      TMZ2W  = (lepZ2.p + lepW.p).M();
      if(passesMCTruth(fakeableLeptons,1,3)){
        //std::cout << "Pass 3Tight, hasOS,passMC\n";
        if (lepZ1.Pt() > 25 && lepZ2.Pt() > 15 && lepW.Pt() > 20){//3 lepton, has OSSF, leptons assigned to W and Z. Fill histos from here onwards
        
          if(TMath::Abs(TMll - nomZmass)< 15. && TMinMll > 4. && TM3l > 100.  ){ //  Z window + exlcude low masses + M_3l selection 
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
        }
      }
    fTree[wP] -> Fill();  //Skimming for 3 FO; remember to use TNTightLeptons == 3 for plotting!!!
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

  iniTree->Branch("TLep_PtZ1",     &TLep_PtZ1,     "TLep_PtZ1/F");
  iniTree->Branch("TLep_PtZ2",     &TLep_PtZ2,     "TLep_PtZ2/F");
  iniTree->Branch("TLep_PtW",      &TLep_PtW,     "TLep_PtW/F");

  iniTree->Branch("TLep_EtaZ1",     &TLep_EtaZ1,     "TLep_EtaZ1/F");
  iniTree->Branch("TLep_EtaZ2",     &TLep_EtaZ2,     "TLep_EtaZ2/F");
  iniTree->Branch("TLep_EtaW",      &TLep_EtaW,     "TLep_EtaW/F");

  iniTree->Branch("TLep_PhiZ1",     &TLep_PhiZ1,     "TLep_PhiZ1/F");
  iniTree->Branch("TLep_PhiZ2",     &TLep_PhiZ2,     "TLep_PhiZ2/F");
  iniTree->Branch("TLep_PhiW",      &TLep_PhiW,     "TLep_PhiW/F");

  iniTree->Branch("TLep_EZ1",     &TLep_EZ1,     "TLep_EZ1/F");
  iniTree->Branch("TLep_EZ2",     &TLep_EZ2,     "TLep_EZ2/F");
  iniTree->Branch("TLep_EW",      &TLep_EW,     "TLep_EW/F");

  iniTree->Branch("TLep_ChargeZ1",     &TLep_ChargeZ1,     "TLep_ChargeZ1/F");
  iniTree->Branch("TLep_ChargeZ2",     &TLep_ChargeZ2,     "TLep_ChargeZ2/F");
  iniTree->Branch("TLep_ChargeW",      &TLep_ChargeW,     "TLep_ChargeW/F");

  iniTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  iniTree->Branch("TMll",        &TMll,      "TMll/F");
  iniTree->Branch("TMZ1W",        &TMZ1W,      "TMZ1W/F");
  iniTree->Branch("TMZ2W",        &TMZ2W,      "TMZ2W/F");
  iniTree->Branch("TM3l",        &TM3l,      "TM3l/F");
  iniTree->Branch("TMtW",        &TMtW,      "TMtW/F");
  iniTree->Branch("TMtWZ",        &TMtWZ,      "TMtWZ/F");
  iniTree->Branch("TNOSSF",      &TNOSSF,      "TNOSSF/I");
  iniTree->Branch("TMinMll",      &TMinMll,      "TMinMll/F");
  iniTree->Branch("TConvNumber",      &TConvNumber,      "TConvNumber/I");
  iniTree->Branch("TFakeNumber",      &TFakeNumber,      "TFakeNumber/I");

  iniTree->Branch("TLep_IsTightZ1",     &TLep_IsTightZ1,     "TLep_IsTightZ1/F");
  iniTree->Branch("TLep_IsTightZ2",     &TLep_IsTightZ2,     "TLep_IsTightZ2/F");
  iniTree->Branch("TLep_IsTightW",      &TLep_IsTightW,     "TLep_IsTightW/F");

  iniTree->Branch("TLep_IsPromptZ1",     &TLep_IsPromptZ1,     "TLep_IsPromptZ1/F");
  iniTree->Branch("TLep_IsPromptZ2",     &TLep_IsPromptZ2,     "TLep_IsPromptZ2/F");
  iniTree->Branch("TLep_IsPromptW",      &TLep_IsPromptW,     "TLep_IsPromptW/F");

  iniTree->Branch("TLep_pdgIdZ1",     &TLep_pdgIdZ1,     "TLep_pdgIdZ1/F");
  iniTree->Branch("TLep_pdgIdZ2",     &TLep_pdgIdZ2,     "TLep_pdgIdZ2/F");
  iniTree->Branch("TLep_pdgIdW",      &TLep_pdgIdW,     "TLep_pdgIdW/F");
  iniTree->Branch("TLep_isConvVeto",   &TLep_isConvVeto, "TLep_isConvVeto[TNFOLeps]/I");
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

Bool_t WZAnalysis::passesMCTruth(std::vector<Lepton> sLep, Int_t addConvs = 1, Int_t requiredLeps = 3){
  Int_t passes = 0;
  Int_t convs = 0;
  Int_t otherfakes = 0;
  for (int i = 0; i < sLep.size(); i++){
    if (gIsData) passes++;
    else{
      Int_t theID = sLep.at(i).idDecayMode;
      Int_t theGammaID = sLep.at(i).decayMode;
      if (theID != 0){//Is prompt (parent is not fake, might be gamma)
        passes++;
      }
      else if (theID == 0 && theGammaID == 1){//Is conversion (parent is a fake gamma)
        convs++;
      }
      else if (theID == 0 && theGammaID != 1){//Other fakes
        otherfakes++;
      } 
    }
  }
/*  if (!gIsData){
    for (int i = 0; i < sLep.size(); i++){
      //std::cout << "LepMatch " << sLep.at(i).lepMatch << std::endl;
      if (sLep.at(i).isPrompt > 0) passes++; //MC hard prompt lepton
      else if (sLep.at(i).lepMatch == 0) continue;//No matching gen/reco is a fail
      else {
        Lepton theLep = sLep.at(i);
        Lepton genMatch = *sLep.at(i).lepMatch;
        Int_t theTypepdgId = (theLep.type == 1) ? 11 : 13;
        Int_t theMotherId  = TMath::Abs(genMatch.decayMode);
        Int_t theGrandMotherId  = TMath::Abs(genMatch.idDecayMode);
        if (genMatch.isPrompt) passes++; //MC hard prompt lepton
        else if (((theTypepdgId == TMath::Abs(genMatch.type) || TMath::Abs(genMatch.type) == 15) && ( (theMotherId == 23 || theMotherId == 24 || theMotherId == 25) || (theMotherId == 15 && (theGrandMotherId == 23 || theGrandMotherId == 24 || theGrandMotherId == 25))))) passes++; //Either a decay from a tau, Z, W or H including tau decays from the later
        else if (genMatch.type == 22 || (theMotherId == 22 && TMath::Abs(genMatch.type) == theTypepdgId)) isConv++; //For conversions
      } 
    }
  }
  if (gIsData){ passes++;}*/
  TConvNumber = convs; //Used to cut conversions in non-conversion samples
  TFakeNumber = otherfakes;
  return (passes + addConvs*(convs+ otherfakes)) == requiredLeps; //Count Prompt leptons and conversions and add up to the required lepton number
}

//#####################################################################
// Get Variables
//------------------------------------------------------------------

void WZAnalysis::GetLeptonsByWP(Int_t wPValue){
  Int_t nFakeableLeptons = foLeptons.size();
  Int_t nTightLeptons = selLeptons.size();

  if (sWPoints[wPValue] == "top"){
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
    Int_t nFO = 0;
    for (int k = 0; k < nFakeableLeptons; k++){
      if (foLeptons.at(k).idMVA%10 > WPointVal[wPValue]){
        //std::cout << k << std::endl;
        fakeableLeptons.push_back(foLeptons.at(k));
        if (WPointVal[wPValue] == 5){//VT SF
          fakeableLeptons.back().SetSF(foLeptons.at(k).GetSF(0)*leptonSFEWKVT->GetLeptonSF(foLeptons.at(k).Pt(), foLeptons.at(k).Eta(), foLeptons.at(k).type));
        }
        else { //M SF
          fakeableLeptons.back().SetSF(foLeptons.at(k).GetSF(0)*leptonSFEWKM->GetLeptonSF(foLeptons.at(k).Pt(), foLeptons.at(k).Eta(), foLeptons.at(k).type));
        }
        nFO++;
      }
    }

    for (int k = 0; k < nTightLeptons; k++){
      if (selLeptons.at(k).idMVA%10 > WPointVal[wPValue]){
        tightLeptons.push_back(selLeptons.at(k));
        if (WPointVal[wPValue] == 5){//VT SF
          tightLeptons.back().SetSF(selLeptons.at(k).GetSF(0)*leptonSFEWKVT->GetLeptonSF(selLeptons.at(k).Pt(), selLeptons.at(k).Eta(), selLeptons.at(k).type));
        }
        else { //M SF
          tightLeptons.back().SetSF(selLeptons.at(k).GetSF(0)*leptonSFEWKM->GetLeptonSF(selLeptons.at(k).Pt(), selLeptons.at(k).Eta(), selLeptons.at(k).type));
        }
      }
    }
  }
}

void WZAnalysis::GetLeptonVariables(std::vector<Lepton> tightLeptons, std::vector<Lepton> foLeptons, std::vector<Lepton> looseLeptons){
  TNTightLeps = tightLeptons.size();
  Int_t nVetoLeptons = foLeptons.size();
  TNFOLeps = nVetoLeptons;
  //std::cout << "NLeps:" << TNFOLeps << ", NTight:" << TNTightLeps << std::endl;
  //Require exactly 3 leptons 
  if(TNFOLeps != 3 ) gChannel = -1;
  //Charge compatibility with WZ production
  else if(TMath::Abs(fakeableLeptons.at(0).charge + fakeableLeptons.at(1).charge + fakeableLeptons.at(2).charge) != 1) gChannel = -1;
  //Combinatory of posible final leptons
  else if(fakeableLeptons.at(0).isMuon && fakeableLeptons.at(1).isMuon && fakeableLeptons.at(2).isMuon) gChannel = iMuMuMu;
  else if(fakeableLeptons.at(0).isMuon && fakeableLeptons.at(1).isMuon && fakeableLeptons.at(2).isElec) gChannel = iElMuMu;
  else if(fakeableLeptons.at(0).isMuon && fakeableLeptons.at(1).isElec && fakeableLeptons.at(2).isMuon) gChannel = iElMuMu;
  else if(fakeableLeptons.at(0).isMuon && fakeableLeptons.at(1).isElec && fakeableLeptons.at(2).isElec) gChannel = iElElMu;
  else if(fakeableLeptons.at(0).isElec && fakeableLeptons.at(1).isMuon && fakeableLeptons.at(2).isMuon) gChannel = iElMuMu;
  else if(fakeableLeptons.at(0).isElec && fakeableLeptons.at(1).isMuon && fakeableLeptons.at(2).isElec) gChannel = iElElMu;
  else if(fakeableLeptons.at(0).isElec && fakeableLeptons.at(1).isElec && fakeableLeptons.at(2).isMuon) gChannel = iElElMu;
  else if(fakeableLeptons.at(0).isElec && fakeableLeptons.at(1).isElec && fakeableLeptons.at(2).isElec) gChannel = iElElEl;
  //std::cout << "Channel Set\n";
  TMinMll = 100000;
  TNOSSF = 0;
  for(Int_t i = 0; i < TNFOLeps; i++){
    for(Int_t j = i+1; j < TNFOLeps; j++){
      if (fakeableLeptons.at(j).isMuon && fakeableLeptons.at(i).isMuon && fakeableLeptons.at(i).charge*fakeableLeptons.at(j).charge == -1)           TNOSSF++;
      if (fakeableLeptons.at(j).isElec && fakeableLeptons.at(i).isElec && fakeableLeptons.at(i).charge*fakeableLeptons.at(j).charge == -1)           TNOSSF++;
      Float_t hypMll = (fakeableLeptons.at(j).p + fakeableLeptons.at(i).p).M();
      if (hypMll < TMinMll) TMinMll = hypMll;
    }
  }
  //std::cout << "Mass computed\n";
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
