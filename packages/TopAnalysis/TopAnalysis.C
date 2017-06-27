#include "TopAnalysis.h"

ClassImp(TopAnalysis);

bool GreaterThan(float i, float j){ return (i > j);}

TopAnalysis::TopAnalysis() : PAFChainItemSelector() {
  fTree = 0;
  fhDummy = 0;
  passMETfilters = 0;
  passTrigger    = 0;
  isSS           = 0;

  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
  
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
          fHSSyields[ch][sys]   = 0;
        }
      }
    }
  }
}
void TopAnalysis::Summary(){}

void TopAnalysis::Initialise(){
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
    fTree   = CreateTree("MiniTree","Created with PAF");
    SetLeptonVariables();
    SetJetVariables();
    SetEventVariables();
  }
  InitHistos();

  // genLeptons  = std::vector<Lepton>();
  // selLeptons  = std::vector<Lepton>();
  // vetoLeptons = std::vector<Lepton>();
  // selJets = std::vector<Jet>();
  // selJetsJecUp = std::vector<Jet>();
  // selJetsJecDown = std::vector<Jet>();
  // Jets15  = std::vector<Jet>();
  // genJets = std::vector<Jet>();
  // mcJets  = std::vector<Jet>();
  // vetoJets = std::vector<Jet>();
}

void TopAnalysis::InsideLoop(){
  // Vectors with the objects
  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
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
  isSS           = GetParam<Bool_t>("isSS");
  // Leptons and Jets
  GetLeptonVariables(selLeptons, vetoLeptons);
  GetJetVariables(selJets, Jets15);
  GetGenJetVariables(genJets, mcJets);
  GetMET();
  fhDummy->Fill(1);

  // Number of events in fiducial region
  if(genLeptons.size() >= 2){ // MIND THE POSSIBLE SKIM (on reco leptons) IN THE SAMPLE!!
    Int_t GenChannel = -1;
    if(genLeptons.at(0).isElec && genLeptons.at(0).isMuon) GenChannel = iElMu; 
    if(genLeptons.at(0).isMuon && genLeptons.at(0).isElec) GenChannel = iElMu; 
    if(genLeptons.at(0).isMuon && genLeptons.at(0).isMuon) GenChannel = iMuon; 
    if(genLeptons.at(0).isElec && genLeptons.at(0).isElec) GenChannel = iElec; 
    if( ( (genLeptons.at(0).p.Pt() > 25 && genLeptons.at(1).p.Pt() > 20) || (genLeptons.at(0).p.Pt() > 20 && genLeptons.at(1).p.Pt() > 25) )
        && (TMath::Abs(genLeptons.at(0).p.Eta() < 2.4) && TMath::Abs(genLeptons.at(1).p.Eta() < 2.4)) 
        && ( (genLeptons.at(0).p + genLeptons.at(1).p).M() > 20 ) ){
      fHFiduYields[GenChannel-1][0] -> Fill(idilepton);
      if(GenChannel == iElMu || (TMath::Abs((genLeptons.at(0).p + genLeptons.at(1).p).M() - 91) > 15) ){
        fHFiduYields[GenChannel-1][0] -> Fill(iZVeto);
        if(GenChannel == iElMu || TGenMET > 40){   // MET > 40 in ee, µµ
          fHFiduYields[GenChannel-1][0] -> Fill(iMETcut);
          if(nFiduJets >= 2){ //At least 2 jets
            fHFiduYields[GenChannel-1][0] -> Fill(i2jets);
            if(nFidubJets >= 1){ // At least 1 b-tag
              fHFiduYields[GenChannel-1][0] -> Fill(i1btag);
            }
          }
        }
      }
    }
  }
  //if((Int_t) genLeptons.size() >=2 && TNSelLeps >= 2 && passTrigger && passMETfilters){ // dilepton event, 2 leptons // && !isSS
  if (gSelection == iTopSelec){
    if (gIsTTbar && genLeptons.size() < 2) return; // Dilepton selection for ttbar!!!
  }
  if(TNSelLeps >= 2 && passTrigger && passMETfilters){ // dilepton event, 2 leptons // && !isSS
    // Deal with weights:
    Float_t lepSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
    Float_t ElecSF = 1; Float_t MuonSF = 1;
    Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
    if(TChannel == iElec){
      ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
      ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
    }
    else if(TChannel == iMuon){
      MuonSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      MuonSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
    }
    else{
      if(selLeptons.at(0).isMuon){
        MuonSF   *= selLeptons.at(0).GetSF( 0);
        MuonSFUp *= selLeptons.at(0).GetSF( 1);
        MuonSFDo *= selLeptons.at(0).GetSF(-1);
      }
      else{
        ElecSF   *= selLeptons.at(0).GetSF( 0);
        ElecSFUp *= selLeptons.at(0).GetSF( 1);
        ElecSFDo *= selLeptons.at(0).GetSF(-1);
      }
      if(selLeptons.at(1).isMuon){
        MuonSF   *= selLeptons.at(1).GetSF( 0);
        MuonSFUp *= selLeptons.at(1).GetSF( 1);
        MuonSFDo *= selLeptons.at(1).GetSF(-1);
      }
      else{
        ElecSF   *= selLeptons.at(1).GetSF( 0);
        ElecSFUp *= selLeptons.at(1).GetSF( 1);
        ElecSFDo *= selLeptons.at(1).GetSF(-1);
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
    if(gIsData) TWeight = 1;
    // Event Selection
    // ===================================================================================================================
    if((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20 && selLeptons.at(0).p.Pt() > 25){ // mll > 20 GeV, dilepton, leading lepton pT > 25 GeV
      if(isSS) fHSSyields[gChannel][0] -> Fill(idilepton, TWeight);
      else{    
        fHyields[gChannel][0] -> Fill(idilepton, TWeight);
        FillHistos(gChannel, idilepton);
        FillDYHistos(gChannel);
      }

      if(TChannel == iElMu || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  ){ //  Z Veto in ee, µµ
        if(isSS) fHSSyields[gChannel][0] -> Fill(iZVeto, TWeight);
        else{      fHyields[gChannel][0] -> Fill(iZVeto, TWeight);
          FillHistos(gChannel, iZVeto);}

        if(TChannel == iElMu || TMET > 40){   // MET > 40 in ee, µµ
          if(isSS) fHSSyields[gChannel][0] -> Fill(iMETcut, TWeight);
          else{      fHyields[gChannel][0] -> Fill(iMETcut, TWeight);
            FillHistos(gChannel, iMETcut);}

          if(TNJets > 1){ //At least 2 jets
            if(isSS) fHSSyields[gChannel][0] -> Fill(i2jets, TWeight);
            else{      fHyields[gChannel][0] -> Fill(i2jets, TWeight);
              FillHistos(gChannel, i2jets); }

            if(TNBtags > 0){ // At least 1 b-tag
              if(isSS) fHSSyields[gChannel][0] -> Fill(i1btag, TWeight);
              else{      fHyields[gChannel][0] -> Fill(i1btag, TWeight);
                FillHistos(gChannel, i1btag); }

            }
          }
        }
      }
      if(TChannel == iElMu){// || ((TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15))){
        if(TChannel == iElMu || TMET > 40){   // MET > 40 in ee, µµ
          if (TNBtags > 0 || TNBtagsBtagUp > 0 || TNBtagsBtagDown > 0 || TNBtagsMisTagUp > 0 || TNBtagsMisTagDown > 0 || TNBtagsJESUp > 0 || TNBtagsJESDown > 0){
            fTree->Fill();
          }
        }
      }
    }   
  }
}


//#####################################################################
// Functions
//------------------------------------------------------------------
void TopAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
  TNSelLeps = selLeptons.size();
  Int_t nVetoLeptons = VetoLeptons.size();
  TNVetoLeps = (nVetoLeptons == 0) ? TNSelLeps : nVetoLeptons;
  for(Int_t i = 0; i < TNSelLeps; i++){
    TLep_Pt[i]     = selLeptons.at(i).Pt();    
    TLep_Eta[i]    = selLeptons.at(i).Eta();    
    TLep_Phi[i]    = selLeptons.at(i).Phi();    
    TLep_E[i]      = selLeptons.at(i).E();    
    TLep_Charge[i] = selLeptons.at(i).charge;    
  }
  if(TNSelLeps < 2) gChannel = -1;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) gChannel = iElMu;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) gChannel = iElMu;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) gChannel = iMuon;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) gChannel = iElec;
  if(TNSelLeps > 1) TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();      
  TChannel = gChannel;
  TIsSS = isSS;
  gChannel = gChannel -1; // gchannel used for chan index of histograms
  
  bool TIsOSDilep = false;
  if (TNSelLeps >= 2)
    TIsOSDilep = passTrigger && passMETfilters && (!isSS) && ((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20) && selLeptons.at(0).p.Pt() > 25;
  else
    TIsOSDilep = false;

  SetParam("TIsOSDilep",TIsOSDilep);
}

void TopAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
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

void TopAnalysis::GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets){
  if(gIsData) return;
  nFiduJets = 0; nFidubJets = 0; 
  Int_t nGenJets = genJets.size();
  Int_t nmcJets = mcJets.size();
  for(Int_t i = 0; i < nGenJets; i++) if(genJets.at(i).p.Pt() > 30 && TMath::Abs(genJets.at(i).p.Eta()) < 2.4)                         nFiduJets++;
  for(Int_t i = 0; i <  nmcJets; i++) if(mcJets.at(i).p.Pt()  > 30 && TMath::Abs(mcJets.at(i).Eta())    < 2.4 && mcJets.at(i).isBtag)  nFidubJets++;
}

void TopAnalysis::GetMET(){
    TMET        = Get<Float_t>("met_pt");
    TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
    if(gIsData) return;
    TMETJESUp   = Get<Float_t>("met_jecUp_pt"  );
    TMETJESDown = Get<Float_t>("met_jecDown_pt");
    TGenMET     = Get<Float_t>("met_genPt");
  if(gIsLHE)  for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

void TopAnalysis::InitHistos(){
  fhDummy = CreateH1F("fhDummy", "fhDummy", 1, 0, 2);
  for(Int_t ch = 0; ch < nChannels; ch++){
    fHyields[ch][0]     = CreateH1F("H_Yields_"+gChanLabel[ch],"", nLevels, -0.5, nLevels-0.5);
    fHFiduYields[ch][0]     = CreateH1F("H_FiduYields_"+gChanLabel[ch],"", nLevels, -0.5, nLevels-0.5);
    fHSSyields[ch][0]   = CreateH1F("H_SSYields_"+gChanLabel[ch],"", nLevels, -0.5, nLevels-0.5);
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
  }
}

void TopAnalysis::FillDYHistos(Int_t ch){
  Int_t sys = 0;
  Int_t cut;
  Float_t EventWeight = TWeight;
  cut = idilepton;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  cut = iZVeto;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  //  if(TMET > 40){
  cut = iMETcut;
  fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
  //  }
  if(TNJets > 1){
    cut = i2jets;
    fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
    if(TNBtags > 0){
      cut = i1btag;
      fHDYInvMass[ch][cut][sys]       -> Fill(TMll, EventWeight);
    }
  }
}

void TopAnalysis::FillHistos(Int_t ch, Int_t cut){
  if(!makeHistos) return;
  Int_t sys = 0;
  Float_t EventWeight = TWeight;

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
  if(TNJets == 4) fHNBtagsNJets[ch][cut][sys]   -> Fill(TNBtags+10, EventWeight);
}

void TopAnalysis::SetLeptonVariables(){
  fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
  fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  fTree->Branch("TLep_Pt",     TLep_Pt,     "TLep_Pt[TNSelLeps]/F");
  fTree->Branch("TLep_Eta",     TLep_Eta,     "TLep_Eta[TNSelLeps]/F");
  fTree->Branch("TLep_Phi",     TLep_Phi,     "TLep_Phi[TNSelLeps]/F");
  fTree->Branch("TLep_E" ,     TLep_E ,     "TLep_E[TNSelLeps]/F");
  fTree->Branch("TLep_Charge",  TLep_Charge, "TLep_Charge[TNSelLeps]/F");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  fTree->Branch("TIsSS",      &TIsSS,      "TIsSS/B");
  fTree->Branch("TMll",      &TMll,      "TMll/F");
}

void TopAnalysis::SetJetVariables(){
  fTree->Branch("TNJets",           &TNJets,         "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
  fTree->Branch("TJet_isBJet",       TJet_isBJet,       "TJet_isBJet[TNJets]/I");
  fTree->Branch("TJet_Pt",           TJet_Pt,           "TJet_Pt[TNJets]/F");
  fTree->Branch("TJet_Eta",           TJet_Eta,           "TJet_Eta[TNJets]/F");
  fTree->Branch("TJet_Phi",           TJet_Phi,           "TJet_Phi[TNJets]/F");
  fTree->Branch("TJet_E",            TJet_E,            "TJet_E[TNJets]/F");

  fTree->Branch("TNJetsJESUp",           &TNJetsJESUp,         "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",           &TNJetsJESDown,         "TNJetsJESDown/I");
  fTree->Branch("TNJetsJERUp",           &TNJetsJERUp,         "TNJetsJERUp/I");

  fTree->Branch("TNBtagsBtagUp",     &TNBtagsBtagUp,   "TNBtagsBtagUp/I");
  fTree->Branch("TNBtagsBtagDown",   &TNBtagsBtagDown, "TNBtagsBtagDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TNBtagsJESUp",   &TNBtagsJESUp, "TNBtagsJESUp/I");
  fTree->Branch("TNBtagsJESDown",  &TNBtagsJESDown, "TNBtagsJESDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJERUp]/F");

  fTree->Branch("THT",          &THT,          "THT/F");
  fTree->Branch("THTJESUp",     &THTJESUp,     "THTJESUp/F");
  fTree->Branch("THTJESDown",   &THTJESDown,   "THTJESDown/F");
}

void TopAnalysis::SetEventVariables(){
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fTree->Branch("TWeight_ElecEffUp",      &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fTree->Branch("TWeight_ElecEffDown",    &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fTree->Branch("TWeight_MuonEffUp",      &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fTree->Branch("TWeight_MuonEffDown",    &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");

  fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fTree->Branch("TMET",         &TMET,         "TMET/F");
  fTree->Branch("TGenMET",         &TGenMET,         "TGenMET/F");
  fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
}
