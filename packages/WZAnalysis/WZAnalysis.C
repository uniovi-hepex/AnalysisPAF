#include "WZAnalysis.h"

ClassImp(WZAnalysis);

bool GreaterThan(float i, float j){ return (i > j);}

WZAnalysis::WZAnalysis() : PAFChainItemSelector() {
  //fTree = {0};
  fhDummy = 0;
  passMETfilters = 0;
  passTrigger    = 0;

/*  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
  
  for(Int_t ch = 0; ch < nChannels; ch++){
    for(Int_t cut = 0; cut < nLevels; cut++){
      for(Int_t sys = 1; sys < nSysts; sys++){
        fHLHEweights[ch][cut][sys]  = 0;
        fHMET[ch][cut][sys]         = 0;
        fHLep0Eta[ch][cut][sys]     = 0;
        fHLep1Eta[ch][cut][sys]     = 0;
        fHDelLepPhi[ch][cut][sys]   = 0;
        fHHT[ch][cut][sys]          = 0;
        fHJet0Eta[ch][cut][sys]     = 0;
        fHJet1Eta[ch][cut][sys]     = 0;
        fHDYInvMass[ch][cut][sys]       = 0;
        fHInvMass[ch][cut][sys]       = 0;
        fHInvMass2[ch][cut][sys]      = 0;
        fHNBtagsNJets[ch][cut][sys]   = 0;
        fHNJets[ch][cut][sys]        = 0;
        fHNBtagJets[ch][cut][sys]    = 0;
        fHJet0Pt[ch][cut][sys]       = 0;
        fHJet1Pt[ch][cut][sys]       = 0;
        fHDiLepPt[ch][cut][sys]      = 0;
        fHLep0Iso[ch][cut][sys]       = 0;
        fHLep1Iso[ch][cut][sys]       = 0;
        fHLep0Pt[ch][cut][sys]       = 0;
        fHLep1Pt[ch][cut][sys]       = 0;
        fHJetCSV[ch][cut][sys]       = 0;
        fHJet0CSV[ch][cut][sys]       = 0;
        fHJet1CSV[ch][cut][sys]       = 0;
        fHvertices[ch][cut][sys]     = 0;
        if(cut == 0){
          fHyields[ch][sys]     = 0;
          fHFiduYields[ch][sys]     = 0;
        }
      }
    }
  }*/
}
void WZAnalysis::Summary(){}

void WZAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsTTbar     = false;
  gIsLHE       = false;
  if (gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  if (gSampleName == "TTbar_Powheg") gIsLHE = true;

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

  for (int wP = 0; wP < nWPoints; wP++){
    // Leptons and Jets

      tightLeptons = {};
      fakeableLeptons = {};

      GetLeptonsByWP(wP);
 
      GetLeptonVariables(tightLeptons, fakeableLeptons, looseLeptons);
      GetJetVariables(selJets, Jets15);
      GetGenJetVariables(genJets, mcJets);
      GetMET();
      //fhDummy->Fill(1);
  
  
    if(TNTightLeps == 3 && passTrigger && passMETfilters){ // trilepton event with OSSF + l, passes trigger and MET filters
      // Deal with weights:
      Float_t lepSF   = selLeptons.at(0).GetSF(0)*selLeptons.at(1).GetSF(0)*selLeptons.at(2).GetSF(0);
      Float_t ElecSF = 1; Float_t MuonSF = 1;
      Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
      Float_t stat = 0; 
      //For muons
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffsRun2
      //Additional 1% for ID + 0.5% for Isolation + 0.5% single muon triggers
  
      if(TChannel == iElElEl){
        ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0)*selLeptons.at(2).GetSF( 0);
        ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1)*selLeptons.at(2).GetSF( 1);
        ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1)*selLeptons.at(2).GetSF(-1);
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
      }
      else if(TChannel == iMuMuMu){
        MuonSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0)*selLeptons.at(2).GetSF( 0);
        MuonSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1)*selLeptons.at(2).GetSF( 1);
        MuonSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1)*selLeptons.at(2).GetSF(-1);
        ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
      }
      else{
        MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
        for (int i = 0; i <3; i ++){
          if (selLeptons.at(i).isMuon){
            MuonSF   *= selLeptons.at(i).GetSF( 0);
            MuonSFUp *= selLeptons.at(i).GetSF( 1);
            MuonSFDo *= selLeptons.at(i).GetSF(-1);        
          }
          else{
            ElecSF   *= selLeptons.at(i).GetSF( 0);
            ElecSFUp *= selLeptons.at(i).GetSF( 1);
            ElecSFDo *= selLeptons.at(i).GetSF(-1);        
          }
        }
      }
  
      TWeight             = NormWeight*ElecSF*MuonSF*TrigSF*PUSF;
      TWeight_ElecEffUp   = NormWeight*ElecSFUp*MuonSF*TrigSF*PUSF;
      TWeight_ElecEffDown = NormWeight*ElecSFDo*MuonSF*TrigSF*PUSF;
      TWeight_MuonEffUp   = NormWeight*ElecSF*MuonSFUp*TrigSF*PUSF;
      TWeight_MuonEffDown = NormWeight*ElecSF*MuonSFDo*TrigSF*PUSF;
      TWeight_TrigUp     = NormWeight*lepSF*(TrigSF+TrigSFerr)*PUSF;
      TWeight_TrigDown   = NormWeight*lepSF*(TrigSF-TrigSFerr)*PUSF;
      TWeight_PUDown     = NormWeight*lepSF*TrigSF*PUSF_Up;
      TWeight_PUUp       = NormWeight*lepSF*TrigSF*PUSF_Down;
      TIsSR   = false;
      TIsCRDY = false;
      TIsCRTT = false;  
      TIsNewCRDY = false;
      TIsNewCRTT = false;  
      if(gIsData) TWeight = 1;
      // Event Selection
      // ===================================================================================================================

      if(TNTightLeps == 3 && TNOSSF > 0){
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
          // fHyields[gChannel][0] -> Fill(itrilepton, TWeight);
          // FillHistos(gChannel, itrilepton);
          // FillDYHistos(gChannel);
        
          if(TMath::Abs(TMll - nomZmass)< 15. && TMinMll > 4. && (lepZ1.p + lepZ2.p + lepW.p).M() > 100.  ){ //  Z window + exlcude low masses + M_3l selection 
            // fHyields[gChannel][0] -> Fill(ionZ, TWeight);
            // FillHistos(gChannel, ionZ);
            // The last two cuts define the Control/Signal regions
            
            // Signal Region
            if(TMET > 30.){   // MET > 30 always
              // fHyields[gChannel][0] -> Fill(imet, TWeight);
              // FillHistos(gChannel, imet);
              if(TNBtags == 0){ //Exactly 0 btags
                //fHyields[gChannel][0] -> Fill(i0btag, TWeight);
                //FillHistos(gChannel, i0btag);
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
/*  fhDummy = CreateH1F("fhDummy", "fhDummy", 1, 0, 2);
  for(Int_t ch = 0; ch < nChannels; ch++){
    fHyields[ch][0]     = CreateH1F("H_Yields_"+gChanLabel[ch],"", nLevels, -0.5, nLevels-0.5);
    fHFiduYields[ch][0]     = CreateH1F("H_FiduYields_"+gChanLabel[ch],"", nLevels, -0.5, nLevels-0.5);
  }
  if(!makeHistos) return;
  for(Int_t ch = 0; ch < nChannels; ch++){
    for(Int_t cut = 0; cut < nLevels; cut++){
      fHLHEweights[ch][cut][0]  = CreateH1F("H_LHEweights"  +gChanLabel[ch]+"_"+sCut[cut],"LHEweights", nWeights, -0.5, nWeights - 0.5);
      fHMET[ch][cut][0]         = CreateH1F("H_MET_"        +gChanLabel[ch]+"_"+sCut[cut],"MET"       , 3000, 0,300);
      fHLep0Eta[ch][cut][0]     = CreateH1F("H_Lep0Eta_"    +gChanLabel[ch]+"_"+sCut[cut],"Lep0Eta"   , 50  ,0 ,2.5);
      fHLep1Eta[ch][cut][0]     = CreateH1F("H_Lep1Eta_"    +gChanLabel[ch]+"_"+sCut[cut],"Lep1Eta"   , 50  ,0 ,2.5);
      fHDelLepPhi[ch][cut][0]   = CreateH1F("H_DelLepPhi_"  +gChanLabel[ch]+"_"+sCut[cut],"DelLepPhi" , 100, -3.2, 3.2);
      fHHT[ch][cut][0]          = CreateH1F("H_HT_"         +gChanLabel[ch]+"_"+sCut[cut],"HT"        , 4700,30,500);
      fHJet0Eta[ch][cut][0]     = CreateH1F("H_Jet0Eta_"  +gChanLabel[ch]+"_"+sCut[cut],"Jet0Eta"   , 50,0,2.5);
      fHJet1Eta[ch][cut][0]     = CreateH1F("H_Jet1Eta_"  +gChanLabel[ch]+"_"+sCut[cut],"Jet1Eta"   , 50,0,2.5);
      fHDYInvMass[ch][cut][0]     = CreateH1F("H_DY_InvMass_"    +gChanLabel[ch]+"_"+sCut[cut],"InvMass"   ,  300,  0., 300.);
      fHInvMass[ch][cut][0]       = CreateH1F("H_InvMass_"    +gChanLabel[ch]+"_"+sCut[cut],"InvMass"   ,  300,  0., 300.);
      fHInvMass2[ch][cut][0]      = CreateH1F("H_InvMass2_"   +gChanLabel[ch]+"_"+sCut[cut],"InvMass2"  ,  400, 70., 110.);
      fHNBtagsNJets[ch][cut][0]   = CreateH1F("H_NBtagsNJets_"+gChanLabel[ch]+"_"+sCut[cut]  ,"NBtagsNJets"   ,15 , -0.5, 14.5);
      fHNJets[ch][cut][0]       = CreateH1F("H_NJets_"      +gChanLabel[ch]+"_"+sCut[cut],"NJets"     , 8 ,-0.5, 7.5);
      fHNBtagJets[ch][cut][0]   = CreateH1F("H_NBtagJets_"  +gChanLabel[ch]+"_"+sCut[cut],"NBtagJets" , 4 ,-0.5, 3.5);
      fHJet0Pt[ch][cut][0]      = CreateH1F("H_Jet0Pt_"     +gChanLabel[ch]+"_"+sCut[cut],"Jet0Pt"    , 2700,30,300);
      fHJet1Pt[ch][cut][0]      = CreateH1F("H_Jet1Pt_"     +gChanLabel[ch]+"_"+sCut[cut],"Jet1Pt"    , 2200,30,250);
      fHDiLepPt[ch][cut][0]     = CreateH1F("H_DiLepPt_"    +gChanLabel[ch]+"_"+sCut[cut],"DiLepPt"   , 1600,20,180);
      fHLep0Pt[ch][cut][0]      = CreateH1F("H_Lep0Pt_"     +gChanLabel[ch]+"_"+sCut[cut],"Lep0Pt"    , 1800,20,200);
      fHLep1Pt[ch][cut][0]      = CreateH1F("H_Lep1Pt_"     +gChanLabel[ch]+"_"+sCut[cut],"Lep1Pt"    , 1800,20,200);
      fHLep0Iso[ch][cut][0]      = CreateH1F("H_Lep0Iso_"     +gChanLabel[ch]+"_"+sCut[cut],"Leading lepton RelIso"    , 50,0,0.15);
      fHLep1Iso[ch][cut][0]      = CreateH1F("H_Lep1Iso_"     +gChanLabel[ch]+"_"+sCut[cut],"Subleading lepton RelIso"    , 50,0,0.15);
      fHJetCSV[ch][cut][0]  = CreateH1F("H_JetCSV_" +gChanLabel[ch]+"_"+sCut[cut],"CSV" , 100,0, 1.0);
      fHJet0CSV[ch][cut][0]  = CreateH1F("H_Jet0CSV_" +gChanLabel[ch]+"_"+sCut[cut],"Jet0CSV" , 100,0, 1.0);
      fHJet1CSV[ch][cut][0]  = CreateH1F("H_Jet1CSV_" +gChanLabel[ch]+"_"+sCut[cut],"Jet1CSV" , 100,0, 1.0);
      fHvertices[ch][cut][0]     = CreateH1F("H_Vtx_"+gChanLabel[ch]+"_"+sCut[cut],"", 51, -0.5, 50.5); 
    }
  }
  for(Int_t ch = 0; ch < nChannels; ch++){
    for(Int_t cut = 0; cut < nLevels; cut++){
      for(Int_t sys = 1; sys < nSysts; sys++){
        fHLHEweights[ch][cut][sys]  = CreateH1F("H_LHEweights"  +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"LHEweights", nWeights, -0.5, nWeights - 0.5);
        fHMET[ch][cut][sys]         = CreateH1F("H_MET_"        +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"MET"       , 3000, 0,300);
        fHLep0Eta[ch][cut][sys]     = CreateH1F("H_Lep0Eta_"    +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Lep0Eta"   , 50  ,0 ,2.5);
        fHLep1Eta[ch][cut][sys]     = CreateH1F("H_Lep1Eta_"    +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Lep1Eta"   , 50  ,0 ,2.5);
        fHDelLepPhi[ch][cut][sys]   = CreateH1F("H_DelLepPhi_"  +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"DelLepPhi" , 100,-3.2, 3.2);
        fHHT[ch][cut][sys]          = CreateH1F("H_HT_"         +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"HT"        , 4700,30,500);
        fHJet0Eta[ch][cut][sys]     = CreateH1F("H_Jet0Eta_"  +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet0Eta"   , 50,0,2.5);
        fHJet1Eta[ch][cut][sys]     = CreateH1F("H_Jet1Eta_"  +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet1Eta"   , 50,0,2.5);
        fHDYInvMass[ch][cut][sys]       = CreateH1F("H_DY_InvMass_"    +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"InvMass"   ,  300,  0., 300.);
        fHInvMass[ch][cut][sys]       = CreateH1F("H_InvMass_"    +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"InvMass"   ,  300,  0., 300.);
        fHInvMass2[ch][cut][sys]      = CreateH1F("H_InvMass2_"   +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"InvMass2"  ,  400, 70., 110.);
        fHNBtagsNJets[ch][cut][sys]   = CreateH1F("H_NBtagsNJets_"+gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys]  ,"NBtagsNJets"   ,15 , -0.5, 14.5);
        fHNJets[ch][cut][sys]        = CreateH1F("H_NJets_"      +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"NJets"     , 8 ,-0.5, 7.5);
        fHNBtagJets[ch][cut][sys]    = CreateH1F("H_NBtagJets_"  +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"NBtagJets" , 4 ,-0.5, 3.5);
        fHJet0Pt[ch][cut][sys]       = CreateH1F("H_Jet0Pt_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet0Pt"    , 2700,30,300);
        fHJet1Pt[ch][cut][sys]       = CreateH1F("H_Jet1Pt_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet1Pt"    , 2200,30,250);
        fHDiLepPt[ch][cut][sys]      = CreateH1F("H_DiLepPt_"    +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"DiLepPt"   , 1600,20,180);
        fHLep0Pt[ch][cut][sys]       = CreateH1F("H_Lep0Pt_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Lep0Pt"    , 1800,20,200);
        fHLep1Pt[ch][cut][sys]       = CreateH1F("H_Lep1Pt_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Lep1Pt"    , 1800,20,200);
        fHLep0Iso[ch][cut][sys]       = CreateH1F("H_Lep0Iso_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"RelIso"    , 50,0,0.15);
        fHLep1Iso[ch][cut][sys]       = CreateH1F("H_Lep1Iso_"     +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"RelIso"    , 50,0,0.15);
        fHJetCSV[ch][cut][sys]       = CreateH1F("H_JetCSV_AllJets_" +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"CSV" , 100,0, 1.0);
        fHJet0CSV[ch][cut][sys]       = CreateH1F("H_Jet0CSV_" +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet0CSV" , 100,0, 1.0);
        fHJet1CSV[ch][cut][sys]       = CreateH1F("H_Jet1CSV_" +gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"Jet1CSV" , 100,0, 1.0);
        fHvertices[ch][cut][sys]     = CreateH1F("H_Vtx_"+gChanLabel[ch]+"_"+sCut[cut]+"_"+gSys[sys],"", 51, -0.5, 50.5); 
      }
    }
  }*/
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

  iniTree->Branch("TNJetsJESUp",           &TNJetsJESUp,         "TNJetsJESUp/I");
  iniTree->Branch("TNJetsJESDown",           &TNJetsJESDown,         "TNJetsJESDown/I");
  iniTree->Branch("TNJetsJERUp",           &TNJetsJERUp,         "TNJetsJERUp/I");

  iniTree->Branch("TNBtagsBtagUp",     &TNBtagsBtagUp,   "TNBtagsBtagUp/I");
  iniTree->Branch("TNBtagsBtagDown",   &TNBtagsBtagDown, "TNBtagsBtagDown/I");
  iniTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  iniTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  iniTree->Branch("TNBtagsJESUp",   &TNBtagsJESUp, "TNBtagsJESUp/I");
  iniTree->Branch("TNBtagsJESDown",  &TNBtagsJESDown, "TNBtagsJESDown/I");

  iniTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  iniTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  iniTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJERUp]/F");

  iniTree->Branch("THT",          &THT,          "THT/F");
  iniTree->Branch("THTJESUp",     &THTJESUp,     "THTJESUp/F");
  iniTree->Branch("THTJESDown",   &THTJESDown,   "THTJESDown/F");
}

void WZAnalysis::SetEventVariables(TTree* iniTree){
  iniTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  iniTree->Branch("TIsSR"  ,      &TIsSR  ,      "TIsSR/B"  );
  iniTree->Branch("TIsCRTT",      &TIsCRTT,      "TIsCRTT/B");
  iniTree->Branch("TIsCRDY",      &TIsCRDY,      "TIsCRDY/B");
  iniTree->Branch("TIsNewCRTT",      &TIsNewCRTT,      "TIsNewCRTT/B");
  iniTree->Branch("TIsNewCRDY",      &TIsNewCRDY,      "TIsNewCRDY/B");
  iniTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  iniTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  iniTree->Branch("TWeight_ElecEffUp",      &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  iniTree->Branch("TWeight_ElecEffDown",    &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  iniTree->Branch("TWeight_MuonEffUp",      &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  iniTree->Branch("TWeight_MuonEffDown",    &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  iniTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  iniTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  iniTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  iniTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");

  iniTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  iniTree->Branch("TMET",         &TMET,         "TMET/F");
  iniTree->Branch("TGenMET",         &TGenMET,         "TGenMET/F");
  iniTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  iniTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  iniTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
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
      }
    }
  }
  else {
    for (int k = 0; k < nFakeableLeptons; k++){
      if (foLeptons.at(k).idMVA%10 > wPValue){
        fakeableLeptons.push_back(foLeptons.at(k));
      }
    }
  
    for (int k = 0; k < nTightLeptons; k++){
      if (selLeptons.at(k).idMVA%10 > wPValue){
        tightLeptons.push_back(selLeptons.at(k));
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
  TNBtags = 0; TNBtagsBtagUp = 0; TNBtagsBtagDown = 0;
  TNBtagsMisTagUp = 0;  TNBtagsMisTagDown = 0;
  THTJESUp = 0; THTJESDown = 0;
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

  if(gIsData) return;  // For systematics...
  for(Int_t i = 0; i < TNJets; i++){
    if(selJets.at(i).isBtag_BtagUp    ) TNBtagsBtagUp++;
    if(selJets.at(i).isBtag_BtagDown  ) TNBtagsBtagDown++;
    if(selJets.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
    if(selJets.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
  }
  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNBtagsJESUp    = 0;
  TNBtagsJESDown  = 0;
  TNJetsJERUp      = 0;  
  for(Int_t i = 0; i < (Int_t) cleanedJets15.size(); i++){
    if(cleanedJets15.at(i).pTJESUp > ptCut){
      THTJESUp += cleanedJets15.at(i).pTJESUp;
      TNJetsJESUp++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESUp++;
      TJetJESUp_Pt[i] = cleanedJets15.at(i).pTJESUp;
    }
    if(cleanedJets15.at(i).pTJESDown > ptCut){
      THTJESDown += cleanedJets15.at(i).pTJESDown;
      TNJetsJESDown++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESDown++;
      TJetJESDown_Pt[i] = cleanedJets15.at(i).pTJESDown;
    }
    if(cleanedJets15.at(i).pTJERUp > ptCut){
      TNJetsJERUp++;
      TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
    }
  }

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
    TMETJESUp   = Get<Float_t>("met_jecUp_pt"  );
    TMETJESDown = Get<Float_t>("met_jecDown_pt");
    TGenMET     = Get<Float_t>("met_genPt");
  if(gIsLHE)  for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

//#####################################################################
// Fill histograms/Minitrees
//------------------------------------------------------------------

void WZAnalysis::FillDYHistos(Int_t ch){
/*  Int_t sys = 0;
  Int_t cut;
  Float_t EventWeight = TWeight;
  cut = itrilepton;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  cut = ionZ;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  //  if(TMET > 40){
  cut = imet;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  //  }
  if(TNBtags == 0){
    cut = i0btag;
    fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  }*/
}

void WZAnalysis::FillHistos(Int_t ch, Int_t cut){
  if(!makeHistos) return;
  Int_t sys = 0;
/*  Float_t EventWeight = TWeight;

  fHMET[ch][cut][sys]         -> Fill(TMET, EventWeight);
  fHLep0Eta[ch][cut][sys]     -> Fill(TMath::Abs(TLep_Eta[0]), EventWeight);
  fHLep1Eta[ch][cut][sys]     -> Fill(TMath::Abs(TLep_Eta[1]), EventWeight);
  fHLep0Pt[ch][cut][sys]      -> Fill(TLep_Pt[0], EventWeight);
  fHLep1Pt[ch][cut][sys]      -> Fill(TLep_Pt[1], EventWeight);
  fHLep0Iso[ch][cut][sys]      -> Fill(selLeptons.at(0).GetIso(), EventWeight);
  fHLep1Iso[ch][cut][sys]      -> Fill(selLeptons.at(1).GetIso(), EventWeight);
  fHDiLepPt[ch][cut][sys]      -> Fill((selLeptons[0].p + selLeptons[1].p).Pt(), EventWeight);
  fHDelLepPhi[ch][cut][sys]   -> Fill(TMath::Abs(selLeptons[0].p.DeltaPhi(selLeptons[1].p)), EventWeight);
  fHHT[ch][cut][sys]          -> Fill(THT, EventWeight);
  if(TNJets > 0){ 
    fHJet0Eta[ch][cut][sys]     -> Fill(TMath::Abs(TJet_Eta[0]), EventWeight);
    fHJet0Pt [ch][cut][sys]     -> Fill(TJet_Pt[0], EventWeight);
    fHJet0CSV[ch][cut][sys]     -> Fill(selJets.at(0).csv, EventWeight);
  }
  if(TNJets > 1){
    fHJet1Eta[ch][cut][sys]     -> Fill(TMath::Abs(TJet_Eta[1]), EventWeight);
    fHJet1Pt [ch][cut][sys]     -> Fill(TJet_Pt[1], EventWeight);
    fHJet1CSV[ch][cut][sys]     -> Fill(selJets.at(1).csv, EventWeight);
  }
  fHInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  fHInvMass2[ch][cut][sys]      -> Fill(TMll, EventWeight);
  fHNJets[ch][cut][sys]        -> Fill(TNJets, EventWeight);
  fHNBtagJets[ch][cut][sys]    -> Fill(TNBtags, EventWeight);
  for(Int_t i = 0; i < (Int_t) Jets15.size(); i++){ 
    fHJetCSV[ch][cut][sys]       -> Fill(Jets15.at(i).csv, EventWeight);
  }
  fHvertices[ch][cut][sys]      -> Fill(Get<Int_t>("nVert"), EventWeight);
  if(TNJets == 0) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags,    EventWeight);
  if(TNJets == 1) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags+1,  EventWeight);
  if(TNJets == 2) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags+3,  EventWeight);
  if(TNJets == 3) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags+6,  EventWeight);
  if(TNJets == 4) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags+10, EventWeight);*/
}
