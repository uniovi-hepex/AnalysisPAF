#include "StopTopAnalysis.h"

ClassImp(StopTopAnalysis);
StopTopAnalysis::StopTopAnalysis() : PAFChainItemSelector() {
  fTree = 0;
  fSumISRJets = 0;  
  fISRJets = 0;  
  ResetVariables();
  IsT2ttScan = false;
}

void StopTopAnalysis::Summary(){}

void StopTopAnalysis::ResetVariables(){
  TrigSF = 0; TrigSFerr = 0; PUSF = 0; PUSF_Up = 0; PUSF_Down = 0; genWeight = 0;
  gChannel = 0; passMETfilters = 0; passTrigger = 0; isSS = 0;  NormWeight = 0; TWeight = 0;
  TMT2 = 0; TMll = 0;  TPfMET = 0; TMHT = 0; TMET = 0; TMET_Phi = 0; TgenMET = 0; TNJets = 0; TNBtags = 0; THT = 0; 
  TMETMuScaleUp = 0; TMETMuScaleDown = 0; TMETElScaleUp = 0; TMETElScaleDown = 0; 
  TMT2MuScaleUp = 0; TMT2MuScaleDown = 0; TMT2ElScaleUp = 0; TMT2ElScaleDown = 0; 
  TMET_noSUSY = 0; TMET_Phi_noSUSY = 0; TMT2_noSUSY = 0; TMT2_METres = 0; TMET_METres = 0;
  TgenMET_noSUSY = 0; TgenMT2_noSUSY = 0; TMETunclUp = 0; TMETunclDown = 0; TMT2unclUp = 0; TMT2unclDown = 0;
  TNLooseLeps = 0; TNSelLeps = 0; TChannel = 0; TDeltaPhi = 0; TDeltaEta = 0; TBtagSFFS = 0;
  TNJetsJESUp = 0; TNJetsJESDown = 0; TNJetsJERUp = 0; TNBtagsJESUp = 0; TNBtagsJESDown = 0;
  TBtagPt = 0;
  TNBtagsBtagUp = 0; TNBtagsBtagDown = 0; TNBtagsMisTagUp = 0; TNBtagsMisTagDown = 0;
  THTJESUp = 0; THTJESDown = 0; TNISRJets = 0; TNVert = 0; FSSF = 0; FSSFerr = 0;
  TMETJERUp = 0; TMETJESUp = 0; TMETJESDown = 0; TMT2JERUp = 0; TMT2JESUp = 0; TMT2JESDown = 0;
  TWeight_LepEffUp = 0; TWeight_LepEffDown = 0; TWeight_TrigUp = 0; TWeight_TrigDown = 0;
  TWeight_FSUp = 0; TWeight_FSDown = 0; TWeight_PUDown = 0; TWeight_PUUp = 0;
  TWeight_PolR = 0; TWeight_PolL = 0;
  TIsSS = false; TNLHEWeight = 0; TISRweight = 0; TISRweightUp = 0; TISRweightDown = 0; TWeight_ISRUp = 0; TWeight_ISRDown = 0;
  TWeight_UnclMETUp   = 0;  TWeight_UnclMETDown = 0;
  TLep0_Pt = 0; TLep0_Eta = 0; TLep0_Phi = 0; TLep0_E = 0; TLep0_Charge = 0; TLep0_Id = 0; TgenLep0_Mid = 0; 
  TLep0_PtMuScaleUp = 0; TLep0_PtMuScaleDown = 0; TLep0_PtElScaleUp = 0; TLep0_PtElScaleDown = 0; TLep0_GenMatch = 0; TLep1_GenMatch = 0;
  TLep1_PtMuScaleUp = 0; TLep1_PtMuScaleDown = 0; TLep1_PtElScaleUp = 0; TLep1_PtElScaleDown = 0;
  TLep1_Pt = 0; TLep1_Eta = 0; TLep1_Phi = 0; TLep1_E = 0; TLep1_Charge = 0; TLep1_Id = 0; TgenLep1_Mid = 0; TgenLep0_Ch = 0; TgenLep1_Ch = 0; 
  TgenMETPhi = 0; TNgenLeps = 0; TEventTruth = 0;
  TgenLep0_Pt = 0; TgenLep0_Eta = 0; TgenLep1_Pt = 0; TgenLep1_Eta = 0; TgenLep0_Id = 0; TgenLep1_Id = 0;
  TgenLep0_Phi = 0; TgenLep1_Phi = 0; TgenLep1_M = 0; TgenLep0_M = 0;
  TgenDeltaPhi = 0; TgenDeltaEta = 0; TgenMT2 = 0; TNgenJets = 0;  TgenHT = 0;
  TgentopDeltaPhi = 0; TgentopDeltaR = 0;
  TMETMETDown = 0; TMETMETUp = 0; TMT2METUp = 0; TMT2METDown = 0;
  TPassTrigger = 0; TPassMETfilters = 0;
  for(Int_t i = 0; i < 20; i++){
    TJet_Pt[i] = 0;
    TJet_Eta[i] = 0;
    TJet_Phi[i] = 0;
    TJet_E[i] = 0;
    TJet_isBJet[i] = 0;
    TJetJESUp_Pt[i] = 0;
    TJetJESDown_Pt[i] = 0;
    TJetJER_Pt[i] = 0;
    TgenJet_Pt[i] = 0;
    TgenJet_Eta[i] = 0;
  }
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
  TgenTop1Pt  = 0;   TgenTop2Pt  = 0;
  TgenTop1Eta = 0;   TgenTop2Eta = 0;
  TgenTop1Phi = 0;   TgenTop2Phi = 0;
  TgenTop1M   = 0;   TgenTop2M   = 0;
  TgenNeu1Pt  = 0; TgenNeu2Pt  = 0;
  TgenNeu1Eta = 0; TgenNeu2Eta = 0;
  TgenNeu1Phi = 0; TgenNeu2Phi = 0;
  TgenNeu1M   = 0; TgenNeu2M   = 0;
  TgenW1Pt  = 0; TgenW2Pt  = 0;
  TgenW1Eta = 0; TgenW2Eta = 0;
  TgenW1Phi = 0; TgenW2Phi = 0;
  TgenW1M   = 0; TgenW2M   = 0;
  TgenStop1Pt  = 0; TgenStop2Pt  = 0;
  TgenStop1Eta = 0; TgenStop2Eta = 0;
  TgenStop1Phi = 0; TgenStop2Phi = 0;
  TgenStop1M   = 0; TgenStop2M   = 0;
  TgenLsp1Pt  = 0; TgenLsp2Pt  = 0;
  TgenLsp1Eta = 0; TgenLsp2Eta = 0;
  TgenLsp1Phi = 0; TgenLsp2Phi = 0;
  TgenLsp1M   = 0; TgenLsp2M   = 0;
  TgenStop1ID = 0; TgenStop2ID = 0;
  TgenTop1ID  = 0; TgenTop2ID  = 0;

  selLeptons.clear(); //looseLeptons.clear();
  genLeptons.clear();
  selJets.clear(); Jets15.clear();
  genJets.clear();
}

void StopTopAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  if(gSampleName.Contains("T2tt")){
    gStopMass = GetParam<Int_t>("stopMass");
    gLspMass  = GetParam<Int_t>("lspMass");
    NormISRweights = GetParam<Float_t>("NormISRweights");
    IsT2ttScan = true;
  }
//  if(gSampleName.BeginsWith("T2tt")) NormISRweights = GetParam<Float_t>("NormISRweights");
  fTree = CreateTree("tree","Created with PAF");
  fSumISRJets = CreateH1F("SumISRweights", "SumISRweights", 1 , 0, 2); 
  fISRJets =    CreateH1F(   "ISRweights",    "ISRweights", 6 , 0, 6); 
  Float_t ptbins[] = {0, 30, 50, 70, 90, 110, 130, 150, 170, 200, 250, 300, 400};
  fTopPt12   =  CreateH2F(   "TopPt2D",    "TopPt2D", 12, ptbins, 12, ptbins); 
  Float_t DPhiBins[] = {0, 0.4, 0.8, 1.2, 1.5, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3.2}; Int_t nDPhiBins = 11;
  Float_t DEtaBins[] = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.3, 1.6, 2, 2.5, 3.2}; Int_t nDEtaBins = 10;
  fDeltaPhi     = CreateH1F("hDeltaPhi", "hDeltaPhi", nDPhiBins, DPhiBins);
  fDeltaEta     = CreateH1F("hDeltaEta", "hDeltaEta", nDEtaBins, DEtaBins);
  fDeltaPhiEta  = CreateH2F("hDeltaPhiEta", "hDeltaPhiEta", nDPhiBins, DPhiBins, nDEtaBins, DEtaBins);
  fTopPtSumOfWeights = CreateH1F("hSumOfTopPtWeights", "hSumOfTopPtWeights", 1, 0, 2);
  fMetResSumOfWeights = CreateH1F("hMetResSumOfWeights", "hMetResSumOfWeights", 1, 0, 2);
  fNormMetRes = 0.92631;

  gIsLHE = false; gIsTTbar = false;
  if(gSampleName == "TTbar_Powheg" || gSampleName.Contains("xqcut20")) gIsLHE = true;
  if(gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  
  gDoISR = false;
  if(gSampleName.Contains("T2tt")) gDoISR = true;

  TNLHEWeight = 254; 
  SetLeptonVariables();
  SetJetVariables();
  SetEventVariables();

  selLeptons  = std::vector<Lepton>();
  //looseLeptons = std::vector<Lepton>();
  genLeptons = std::vector<Lepton>();
  selJets = std::vector<Jet>();
  Jets15  = std::vector<Jet>();
  genJets = std::vector<Jet>();
}


void StopTopAnalysis::InsideLoop(){
  if(IsT2ttScan) if(fabs(fabs((Get<Int_t>("GenSusyMStop"))- gStopMass)) > 1 || fabs((fabs(Get<Int_t>("GenSusyMNeutralino")) - gLspMass)) > 1) return;

  ResetVariables();

  selLeptons  = GetParam<vector<Lepton>>("looseLeptons");
  //looseLeptons = GetParam<vector<Lepton>>("looseLeptons");
  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  Jets15      = GetParam<vector<Jet>>("Jets15");
  genJets     = GetParam<vector<Jet>>("genJets");

  // Weights and SFs
  NormWeight   = GetParam<Float_t>("NormWeight");
  TrigSF       = GetParam<Float_t>("TriggerSF");
  TrigSFerr    = GetParam<Float_t>("TriggerSFerr");
  PUSF         = GetParam<Float_t>("PUSF");
  PUSF_Up      = GetParam<Float_t>("PUSF_Up");
  PUSF_Down    = GetParam<Float_t>("PUSF_Down");
  BtagSFFS     = GetParam<Float_t>("BtagSFFS");
  FSSF         = GetParam<Float_t>("FSSF");
  FSSFerr      = GetParam<Float_t>("FSSFerr");
  gIsMCatNLO   = GetParam<Bool_t>("IsMCatNLO");

  if(gIsMCatNLO) genWeight = Get<Float_t>("genWeight");
  else           genWeight = 1;

  // Event variables
  gChannel        = GetParam<Int_t>("gChannel");
  passMETfilters  = GetParam<Bool_t>("METfilters");
  passTrigger     = GetParam<Bool_t>("passTrigger");
  isSS            = GetParam<Bool_t>("isSS");

  // Leptons and Jets
  GetLeptonVariables(selLeptons);
  GetJetVariables(selJets, Jets15);
  GetMET();
  GetMHT();
  GetGenInfo();
  SetMetNoNeutralinos();

  if(gIsTTbar) fTopPtSumOfWeights->Fill(1, GetTopPtWeight(TgenTop1Pt, TgenTop2Pt));
  fSumISRJets->Fill(1, getISRJetsWeight(TNISRJets)); 
  fTopPt12->Fill(TgenTop1Pt, TgenTop2Pt);

  if(IsT2ttScan || gDoISR) SetPolWeights();


  // =================== Tree at gen level here! ==================
//  if(TNgenLeps >= 2){
//    fDeltaPhi->Fill(TgenDeltaPhi);
//    fDeltaEta->Fill(TgenDeltaEta);
//    fDeltaPhiEta->Fill(TgenDeltaPhi, TgenDeltaEta);
//    TChannel = GetDileptonicChannel(genLeptons);
//    fTree->Fill();
//  }
//  return;

  if(gIsTTbar){
    if(genLeptons.size() < 2) return;
  }
  // =================== Tree at reco level here! ==================
  if(TNSelLeps >= 2 || TNSelLepsMuScaleUp >= 2 || TNSelLepsMuScaleDown >= 2 || TNSelLepsElScaleUp >= 2 || TNSelLepsElScaleDown >= 2){ // 2 leptons, OS
    // TPassTrigger && TPassMETfilters && !TIsSS

    if(gIsTTbar) fMetResSumOfWeights->Fill(1, GetMetResolutionSF(TMET));
    Float_t ElecSF = 1; Float_t MuonSF = 1;
    Float_t ElecSFUp = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
    //lepSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
    //lepSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
    //lepSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
    if(TChannel == iElec){
      ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
      ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
    }
    else if(TChannel == iMuon){
      MuonSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
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
      else if(selLeptons.at(0).isElec){
        ElecSF   *= selLeptons.at(0).GetSF( 0);
        ElecSFUp *= selLeptons.at(0).GetSF( 1);
        ElecSFDo *= selLeptons.at(0).GetSF(-1);
      }
      if(selLeptons.at(1).isMuon){
        MuonSF   *= selLeptons.at(1).GetSF( 0);
        MuonSFUp *= selLeptons.at(1).GetSF( 1);
        MuonSFDo *= selLeptons.at(1).GetSF(-1);
      }
      else if(selLeptons.at(1).isElec){
        ElecSF   *= selLeptons.at(1).GetSF( 0);
        ElecSFUp *= selLeptons.at(1).GetSF( 1);
        ElecSFDo *= selLeptons.at(1).GetSF(-1);
      }
    }
    if(gIsFastSim){
      //TrigSF *= 0.98; // trigg efficiency for FastSim
      PUSF = 1;
      PUSF_Up  = 1;
      PUSF_Down = 1;
    }
    else FSSF = 1;
    Float_t MuonSF_diffUp   = fabs(MuonSFUp - MuonSF);
    Float_t MuonSF_diffDown = fabs(MuonSF - MuonSFDo);
    MuonSFUp = MuonSF + TMath::Sqrt(0.005*0.005+0.005*0.005 + 0.01*0.01 + MuonSF_diffUp*MuonSF_diffUp);
    MuonSFDo = MuonSF - TMath::Sqrt(0.005*0.005+0.005*0.005 + 0.01*0.01 + MuonSF_diffDown*MuonSF_diffDown);
    TWeight             = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*FSSF;
    TWeight_ElecEffUp   = NormWeight*ElecSFUp*MuonSF*TrigSF*PUSF*FSSF;
    TWeight_ElecEffDown = NormWeight*ElecSFDo*MuonSF*TrigSF*PUSF*FSSF;
    TWeight_MuonEffUp   = NormWeight*ElecSF*MuonSFUp*TrigSF*PUSF*FSSF;
    TWeight_MuonEffDown = NormWeight*ElecSF*MuonSFDo*TrigSF*PUSF*FSSF;
    TWeight_TrigUp      = NormWeight*ElecSF*MuonSF*(TrigSF+TrigSFerr)*PUSF;
    TWeight_TrigDown    = NormWeight*ElecSF*MuonSF*(TrigSF-TrigSFerr)*PUSF;
    TWeight_FSUp        = NormWeight*ElecSF*MuonSF*TrigSF*(FSSF+FSSFerr)*PUSF*FSSF;
    TWeight_FSDown      = NormWeight*ElecSF*MuonSF*TrigSF*(FSSF-FSSFerr)*PUSF*FSSF;
    TWeight_PUDown      = NormWeight*ElecSF*MuonSF*TrigSF*PUSF_Up*FSSF;
    TWeight_PUUp        = NormWeight*ElecSF*MuonSF*TrigSF*PUSF_Down*FSSF;
    TWeight_ISRUp       = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*TISRweightUp*FSSF;
    TWeight_ISRDown     = NormWeight*ElecSF*MuonSF*TrigSF*PUSF*TISRweightDown*FSSF;
    TWeight_UnclMETUp   = TWeight*GetMetUnclWeightUp(  TMET);
    TWeight_UnclMETDown = TWeight*GetMetUnclWeightDown(TMET);
    if(gIsData) TWeight = 1;

    // Event Selection
    // ===================================================================================================================
    if((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20){ // mll > 20 GeV
      if(gChannel == iElMu || (TMath::Abs((selLeptons.at(0).p + selLeptons.at(1).p).M() - 91) > 15)  ){ //  Z Veto in ee, µµ
        //if(TNJets > 1 || TNJetsJESUp > 1 || TNJetsJESDown > 1 || TNJetsJERUp > 1){ //At least 2 jets
        // if(TNBtags > 0 || TNBtagsBtagUp > 0 || TNBtagsBtagDown > 0 || TNBtagsMisTagUp > 0 || TNBtagsMisTagDown > 0 || TNBtagsJESUp > 0 || TNBtagsJESDown > 0){ // At least 1 b-tag
        fTree->Fill();
        // }
        // }
      }
    }
  }
  //if(TNgenLeps >= 2) fTree->Fill();
}


//#####################################################################
// Functions
//------------------------------------------------------------------

void StopTopAnalysis::SetLeptonVariables(){
  fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  fTree->Branch("TLep0_Pt",     &TLep0_Pt,     "TLep0_Pt/F");
  fTree->Branch("TLep0_Eta",     &TLep0_Eta,     "TLep0_Eta/F");
  fTree->Branch("TLep0_Phi",     &TLep0_Phi,     "TLep0_Phi/F");
  fTree->Branch("TLep0_E" ,     &TLep0_E ,     "TLep0_E/F");
  fTree->Branch("TLep0_Id" ,     &TLep0_Id ,     "TLep0_Id/I");
  fTree->Branch("TLep0_Charge", &TLep0_Charge, "TLep0_Charge/F");
  fTree->Branch("TLep0_GenMatch", &TLep0_GenMatch, "TLep0_GenMatch/I");
  fTree->Branch("TLep1_Pt",     &TLep1_Pt,     "TLep1_Pt/F");
  fTree->Branch("TLep1_Eta",     &TLep1_Eta,     "TLep1_Eta/F");
  fTree->Branch("TLep1_Phi",     &TLep1_Phi,     "TLep1_Phi/F");
  fTree->Branch("TLep1_E" ,     &TLep1_E ,     "TLep1_E/F");
  fTree->Branch("TLep1_Charge",  &TLep1_Charge, "TLep1_Charge/F");
  fTree->Branch("TLep1_Id" ,     &TLep1_Id ,     "TLep1_Id/I");
  fTree->Branch("TLep1_GenMatch", &TLep1_GenMatch, "TLep1_GenMatch/I");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  fTree->Branch("TPassTrigger",  &TPassTrigger,  "TPassTrigger/I");
  fTree->Branch("TPassMETfilters",  &TPassMETfilters,  "TPassMETfilters/I");
  fTree->Branch("TMll",      &TMll,      "TMll/F");
  fTree->Branch("TMT2",      &TMT2,      "TMT2/F");
  fTree->Branch("TDeltaPhi",      &TDeltaPhi,      "TDeltaPhi/F");
  fTree->Branch("TDeltaEta",      &TDeltaEta,      "TDeltaPhi/F");

  if(gIsData) return;
  fTree->Branch("TNSelLepsMuScaleUp",     &TNSelLepsMuScaleUp,     "TNSelLepsMuScaleUp/I");
  fTree->Branch("TNSelLepsMuScaleDown",   &TNSelLepsMuScaleDown,   "TNSelLepsMuScaleDown/I");
  fTree->Branch("TNSelLepsElScaleUp",     &TNSelLepsElScaleUp,     "TNSelLepsElScaleUp/I");
  fTree->Branch("TNSelLepsElScaleDown",   &TNSelLepsElScaleDown,   "TNSelLepsElScaleDown/I");
  fTree->Branch("TLep0_PtMuScaleUp",     &TLep0_PtMuScaleUp,     "TLep0_PtMuScaleUp/F");
  fTree->Branch("TLep0_PtMuScaleDown",   &TLep0_PtMuScaleDown,   "TLep0_PtMuScaleDown/F");
  fTree->Branch("TLep0_PtElScaleUp",     &TLep0_PtElScaleUp,     "TLep0_PtElScaleUp/F");
  fTree->Branch("TLep0_PtElScaleDown",   &TLep0_PtElScaleDown,   "TLep0_PtElScaleDown/F");
  fTree->Branch("TLep1_PtMuScaleUp",     &TLep1_PtMuScaleUp,     "TLep1_PtMuScaleUp/F");
  fTree->Branch("TLep1_PtMuScaleDown",   &TLep1_PtMuScaleDown,   "TLep1_PtMuScaleDown/F");
  fTree->Branch("TLep1_PtElScaleUp",     &TLep1_PtElScaleUp,     "TLep1_PtElScaleUp/F");
  fTree->Branch("TLep1_PtElScaleDown",   &TLep1_PtElScaleDown,   "TLep1_PtElScaleDown/F");
  fTree->Branch("TMT2MuScaleUp",      &TMT2MuScaleUp,      "TMT2MuScaleUp/F");
  fTree->Branch("TMT2MuScaleDown",    &TMT2MuScaleDown,    "TMT2MuScaleDown/F");
  fTree->Branch("TMT2ElScaleUp",      &TMT2ElScaleUp,      "TMT2ElScaleUp/F");
  fTree->Branch("TMT2ElScaleDown",    &TMT2ElScaleDown,    "TMT2ElScaleDown/F");
  fTree->Branch("TNgenLeps",     &TNgenLeps,     "TNgenLeps/I");
  fTree->Branch("TgenLep0_Pt",     &TgenLep0_Pt,     "TgenLep0_Pt/F");
  fTree->Branch("TgenLep0_Eta",     &TgenLep0_Eta,     "TgenLep0_Eta/F");
  fTree->Branch("TgenLep0_Id",     &TgenLep0_Id,     "TgenLep0_Id/I");
  fTree->Branch("TgenLep1_Id",     &TgenLep1_Id,     "TgenLep1_Id/I");
  fTree->Branch("TgenLep1_Pt",     &TgenLep1_Pt,     "TgenLep1_Pt/F");
  fTree->Branch("TgenLep1_Eta",     &TgenLep1_Eta,     "TgenLep1_Eta/F");
  fTree->Branch("TgenLep0_Mid" ,    &TgenLep0_Mid ,    "TgenLep0_Mid/I");
  fTree->Branch("TgenLep1_Mid" ,     &TgenLep1_Mid ,     "TgenLep1_Mid/I");
  fTree->Branch("TgenDeltaPhi",      &TgenDeltaPhi,      "TgenDeltaPhi/F");
  fTree->Branch("TgenDeltaEta",      &TgenDeltaEta,      "TgenDeltaPhi/F");
  fTree->Branch("TgenMT2",      &TgenMT2,      "TgenMT2/F");
}

void StopTopAnalysis::SetJetVariables(){
  fTree->Branch("TNJets",        &TNJets,         "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
  fTree->Branch("TJet_isBJet",   TJet_isBJet,       "TJet_isBJet[TNJets]/I");
  fTree->Branch("TJet_Pt",       TJet_Pt,           "TJet_Pt[TNJets]/F");
  fTree->Branch("TJet_Eta",      TJet_Eta,           "TJet_Eta[TNJets]/F");
  fTree->Branch("TJet_Phi",      TJet_Phi,           "TJet_Phi[TNJets]/F");
  fTree->Branch("TJet_E",        TJet_E,            "TJet_E[TNJets]/F");
  fTree->Branch("THT",          &THT,          "THT/F");
  fTree->Branch("TBtagPt",        &TBtagPt,        "TBtagPt/F");

  if(gIsData) return;
  fTree->Branch("TNJetsJESUp",      &TNJetsJESUp,         "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",    &TNJetsJESDown,         "TNJetsJESDown/I");
  fTree->Branch("TNBtagsJESUp",      &TNBtagsJESUp,         "TNBtagsJESUp/I");
  fTree->Branch("TNBtagsJESDown",    &TNBtagsJESDown,         "TNBtagsJESDown/I");
  fTree->Branch("TNJetsJERUp",        &TNJetsJERUp,         "TNJetsJERUp/I");

  fTree->Branch("TNBtagsBtagUp",           &TNBtagsBtagUp,   "TNBtagsBtagUp/I");
  fTree->Branch("TNBtagsBtagDown",         &TNBtagsBtagDown, "TNBtagsBtagDown/I");
  fTree->Branch("TNBtagsMisTagUp",     &TNBtagsMisTagUp,   "TNBtagsMisTagUp/I");
  fTree->Branch("TNBtagsMisTagDown",   &TNBtagsMisTagDown, "TNBtagsMisTagDown/I");

  fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJERUp]/F");

  fTree->Branch("TNgenJets",        &TNgenJets,       "TNgenJets/I");
  fTree->Branch("TNgenBtags",       &TNgenBtags,      "TNgenBtags/I");
  fTree->Branch("TgenJet_Pt",       TgenJet_Pt,       "TgenJet_Pt[TNJets]/F");
  fTree->Branch("TgenJet_Eta",      TgenJet_Eta,      "TgenJet_Eta[TNJets]/F");
  fTree->Branch("TgenHT",           &TgenHT,          "TgenHT/F");
}

void StopTopAnalysis::SetEventVariables(){
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
  fTree->Branch("TBtagSFFS",    &TBtagSFFS,    "TBtagSFFS/F");
  fTree->Branch("TMET",         &TMET,         "TMET/F");
  fTree->Branch("TMHT",         &TMHT,         "TMHT/F");
  fTree->Branch("TPfMET",         &TPfMET,         "TPfMET/F");
  fTree->Branch("TNVert",         &TNVert,         "TNVert/I");
  fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  fTree->Branch("TIsSS",        &TIsSS,        "TIsSS/B");  
  fTree->Branch("TMET_METres",      &TMET_METres,         "TMET_METres/F");
  fTree->Branch("TMT2_METres",      &TMT2_METres,      "TMT2_METres/F");

  if(gIsData) return;
  fTree->Branch("TMETMuScaleUp",         &TMETMuScaleUp,         "TMETMuScaleUp/F");
  fTree->Branch("TMETMuScaleDown",       &TMETMuScaleDown,       "TMETMuScaleDown/F");
  fTree->Branch("TMETElScaleUp",         &TMETElScaleUp,         "TMETElScaleUp/F");
  fTree->Branch("TMETElScaleDown",       &TMETElScaleDown,       "TMETElScaleDown/F");
  fTree->Branch("TgenMET",         &TgenMET,         "TgenMET/F");
  fTree->Branch("TISRweight",      &TISRweight,      "TISRweight/F");
  fTree->Branch("TWeight_MuonEffUp",      &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fTree->Branch("TWeight_MuonEffDown",    &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fTree->Branch("TWeight_ElecEffUp",      &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fTree->Branch("TWeight_ElecEffDown",    &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",        &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");
  fTree->Branch("TWeight_ISRUp",        &TWeight_ISRUp,        "TWeight_ISRUp/F");
  fTree->Branch("TWeight_ISRDown",        &TWeight_ISRDown,        "TWeight_ISRDown/F");
  fTree->Branch("TWeight_UnclMETUp",        &TWeight_UnclMETUp,        "TWeight_UnclMETUp/F");
  fTree->Branch("TWeight_UnclMETDown",        &TWeight_UnclMETDown,        "TWeight_UnclMETDown/F");
  fTree->Branch("TWeight_FSUp",        &TWeight_FSUp,        "TWeight_FSUp/F");
  fTree->Branch("TWeight_FSDown",        &TWeight_FSDown,        "TWeight_FSDown/F");
  fTree->Branch("TWeight_PolR",        &TWeight_PolR,        "TWeight_PolR/F");
  fTree->Branch("TWeight_PolL",        &TWeight_PolL,        "TWeight_PolL/F");

  //fTree->Branch("TNLHEWeight",        &TNLHEWeight,         "TNLHEWeight/I");
  if(gIsLHE) fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fTree->Branch("TMETJERUp",    &TMETJERUp,    "TMETJERUp/F");
  fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");
  fTree->Branch("TMT2JERUp",    &TMT2JERUp,    "TMT2JERUp/F");
  fTree->Branch("TMT2JESUp",    &TMT2JESUp,    "TMT2JESUp/F");
  fTree->Branch("TMT2JESDown",  &TMT2JESDown,  "TMT2JESDown/F");
  fTree->Branch("TMETMETUp",    &TMETMETUp,    "TMETMETUp/F");
  fTree->Branch("TMETMETDown",  &TMETMETDown,  "TMETMETDown/F");
  fTree->Branch("TMT2METUp",    &TMT2METUp,    "TMT2METUp/F");
  fTree->Branch("TMT2METDown",  &TMT2METDown,  "TMT2METDown/F");

  fTree->Branch("TMETunclUp",    &TMETunclUp,    "TMETunclUp/F");
  fTree->Branch("TMETunclDown",  &TMETunclDown,  "TMETunclDown/F");
  fTree->Branch("TMT2unclUp",    &TMT2unclUp,    "TMT2unclUp/F");
  fTree->Branch("TMT2unclDown",  &TMT2unclDown,  "TMT2unclDown/F");

  fTree->Branch("TgenWeight"  , &TgenWeight,  "TgenWeight/F" ); 
  fTree->Branch("TEventTruth"  , &TEventTruth,  "TEventTruth/I" ); 
  fTree->Branch("TgenMETPhi"  , &TgenMETPhi , "TgenMETPhi/F" ); 
  fTree->Branch("TgenTop1Pt"  , &TgenTop1Pt , "TgenTop1Pt/F" );
  fTree->Branch("TgenTop1Eta" , &TgenTop1Eta, "TgenTop1Eta/F"); 
  fTree->Branch("TgenTop1Phi" , &TgenTop1Phi, "TgenTop1Phi/F"); 
  fTree->Branch("TgenTop1M"   , &TgenTop1M  , "TgenTop1M/F"  ); 
  fTree->Branch("TgenTop2Pt"  , &TgenTop2Pt , "TgenTop2Pt/F" ); 
  fTree->Branch("TgenTop2Eta" , &TgenTop2Eta, "TgenTop2Eta/F"); 
  fTree->Branch("TgenTop2Phi" , &TgenTop2Phi, "TgenTop2Phi/F"); 
  fTree->Branch("TgenTop2M"   , &TgenTop2M  , "TgenTop2M/F"  ); 

  fTree->Branch("TnGenW",         &TnGenW,         "TnGenW/I");
  fTree->Branch("TnGenStop",         &TnGenStop,         "TnGenStop/I");
  fTree->Branch("TnGenLsp",         &TnGenLsp,         "TnGenLsp/I");
  fTree->Branch("TnGenNeutrinos",         &TnGenNeutrinos,         "TnGenNeutrinos/I");
  fTree->Branch("TgenNeu1Pt"  , &TgenNeu1Pt , "TgenNeu1Pt/F" );
  fTree->Branch("TgenNeu1Eta" , &TgenNeu1Eta, "TgenNeu1Eta/F"); 
  fTree->Branch("TgenNeu1Phi" , &TgenNeu1Phi, "TgenNeu1Phi/F"); 
  fTree->Branch("TgenNeu1M"   , &TgenNeu1M  , "TgenNeu1M/F"  ); 
  fTree->Branch("TgenNeu2Pt"  , &TgenNeu2Pt , "TgenNeu2Pt/F" ); 
  fTree->Branch("TgenNeu2Eta" , &TgenNeu2Eta, "TgenNeu2Eta/F"); 
  fTree->Branch("TgenNeu2Phi" , &TgenNeu2Phi, "TgenNeu2Phi/F"); 
  fTree->Branch("TgenNeu2M"   , &TgenNeu2M  , "TgenNeu2M/F"  ); 
  fTree->Branch("TgenW1Pt"  , &TgenW1Pt , "TgenW1Pt/F" );
  fTree->Branch("TgenW1Eta" , &TgenW1Eta, "TgenW1Eta/F"); 
  fTree->Branch("TgenW1Phi" , &TgenW1Phi, "TgenW1Phi/F"); 
  fTree->Branch("TgenW1M"   , &TgenW1M  , "TgenW1M/F"  ); 
  fTree->Branch("TgenW2Pt"  , &TgenW2Pt , "TgenW2Pt/F" ); 
  fTree->Branch("TgenW2Eta" , &TgenW2Eta, "TgenW2Eta/F"); 
  fTree->Branch("TgenW2Phi" , &TgenW2Phi, "TgenW2Phi/F"); 
  fTree->Branch("TgenW2M"   , &TgenW2M  , "TgenW2M/F"  ); 
  fTree->Branch("TgenStop1Pt"  , &TgenStop1Pt , "TgenStop1Pt/F" );
  fTree->Branch("TgenStop1Eta" , &TgenStop1Eta, "TgenStop1Eta/F"); 
  fTree->Branch("TgenStop1Phi" , &TgenStop1Phi, "TgenStop1Phi/F"); 
  fTree->Branch("TgenStop1M"   , &TgenStop1M  , "TgenStop1M/F"  ); 
  fTree->Branch("TgenStop2Pt"  , &TgenStop2Pt , "TgenStop2Pt/F" ); 
  fTree->Branch("TgenStop2Eta" , &TgenStop2Eta, "TgenStop2Eta/F"); 
  fTree->Branch("TgenStop2Phi" , &TgenStop2Phi, "TgenStop2Phi/F"); 
  fTree->Branch("TgenStop2M"   , &TgenStop2M  , "TgenStop2M/F"  ); 
  fTree->Branch("TgenLsp1Pt"  , &TgenLsp1Pt , "TgenLsp1Pt/F" );
  fTree->Branch("TgenLsp1Eta" , &TgenLsp1Eta, "TgenLsp1Eta/F"); 
  fTree->Branch("TgenLsp1Phi" , &TgenLsp1Phi, "TgenLsp1Phi/F"); 
  fTree->Branch("TgenLsp1M"   , &TgenLsp1M  , "TgenLsp1M/F"  ); 
  fTree->Branch("TgenLsp2Pt"  , &TgenLsp2Pt , "TgenLsp2Pt/F" ); 
  fTree->Branch("TgenLsp2Eta" , &TgenLsp2Eta, "TgenLsp2Eta/F"); 
  fTree->Branch("TgenLsp2Phi" , &TgenLsp2Phi, "TgenLsp2Phi/F"); 
  fTree->Branch("TgenLsp2M"   , &TgenLsp2M  , "TgenLsp2M/F"  ); 

  fTree->Branch("TMET_noSUSY", &TMET_noSUSY, "TMET_noSUSY/F");
  fTree->Branch("TMT2_noSUSY", &TMT2_noSUSY, "TMT2_noSUSY/F");
  fTree->Branch("TgenMET_noSUSY", &TgenMET_noSUSY, "TgenMET_noSUSY/F");
  fTree->Branch("TgenMT2_noSUSY", &TgenMT2_noSUSY, "TgenMT2_noSUSY/F");
}

void StopTopAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons){
  float pt; float ptUp; float ptDown; float scale; int id;
  int nMuon = 0; int nElec = 0; int nMuonUp = 0; int nMuonDown = 0; int nElecUp = 0; int nElecDown = 0;
  TNSelLeps = 0; TNSelLepsMuScaleUp = 0; TNSelLepsMuScaleDown = 0; TNSelLepsElScaleUp = 0; TNSelLepsElScaleDown = 0;
  for(int i = 0; i < (int) selLeptons.size(); i++){
    pt    = selLeptons.at(i).Pt();
    scale = selLeptons.at(i).GetEnergyUnc();
    id    = selLeptons.at(i).isMuon ? 13 : 11;
    ptUp  = pt*(1+scale);
    ptDown= pt*(1-scale);
    if(id == 13){
      if(pt     > 20) nMuon++;
      if(ptUp   > 20) nMuonUp++;
      if(ptDown > 20) nMuonDown++;
    }
    else{
      if(pt     > 20) nElec++;
      if(ptUp   > 20) nElecUp++;
      if(ptDown > 20) nElecDown++;
    }
  }
  TNSelLeps            = nMuon     + nElec;
  TNSelLepsMuScaleUp   = nMuonUp   + nElec; 
  TNSelLepsMuScaleDown = nMuonDown + nElec;
  TNSelLepsElScaleUp   = nMuon     + nElecUp;
  TNSelLepsElScaleDown = nMuon     + nElecDown;
  TNLooseLeps = selLeptons.size();
  //if(TNSelLeps >= 2 || TNSelLepsMuScaleUp >= 2 || TNSelLepsMuScaleDown >= 2 || TNSelLepsElScaleUp >= 2 || TNSelLepsElScaleDown >= 2) TNLooseLeps = 2;

  if(TNLooseLeps < 2) TChannel = -1;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) TChannel = iElMu;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) TChannel = iElMu;
  else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) TChannel = iMuon;
  else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) TChannel = iElec;
  Int_t Index0 = 0; Int_t Index1 = 1;
  if(TChannel == iElMu && selLeptons.at(0).isElec){ Index1 = 0; Index0 = 1;};

  if(TNLooseLeps > 0){
    scale = selLeptons.at(0).GetEnergyUnc();
    TLep0_Pt            = selLeptons.at(0).Pt();    
    TLep0_PtMuScaleUp   = selLeptons.at(0).isMuon? TLep0_Pt*(1 + scale) : TLep0_Pt;
    TLep0_PtMuScaleDown = selLeptons.at(0).isMuon? TLep0_Pt*(1 - scale) : TLep0_Pt;
    TLep0_PtElScaleUp   = selLeptons.at(0).isElec? TLep0_Pt*(1 + scale) : TLep0_Pt;
    TLep0_PtElScaleDown = selLeptons.at(0).isElec? TLep0_Pt*(1 - scale) : TLep0_Pt;
    TLep0_Eta    = selLeptons.at(0).Eta();    
    TLep0_Phi    = selLeptons.at(0).Phi();    
    TLep0_E      = selLeptons.at(0).E();    
    TLep0_Charge = selLeptons.at(0).charge;    
    TLep0_Id     = selLeptons.at(0).isElec ? 11 : 13;
    TLep0_GenMatch = selLeptons.at(0).GetGenMatch();
  }
  if(TNLooseLeps > 1){
    scale = selLeptons.at(1).GetEnergyUnc();
    TLep1_Pt     = selLeptons.at(1).Pt();    
    TLep1_PtMuScaleUp   = selLeptons.at(1).isMuon? TLep1_Pt*(1 + scale) : TLep1_Pt;
    TLep1_PtMuScaleDown = selLeptons.at(1).isMuon? TLep1_Pt*(1 - scale) : TLep1_Pt;
    TLep1_PtElScaleUp   = selLeptons.at(1).isElec? TLep1_Pt*(1 + scale) : TLep1_Pt;
    TLep1_PtElScaleDown = selLeptons.at(1).isElec? TLep1_Pt*(1 - scale) : TLep1_Pt;
    TLep1_Eta    = selLeptons.at(1).Eta();    
    TLep1_Phi    = selLeptons.at(1).Phi();    
    TLep1_E      = selLeptons.at(1).E();    
    TLep1_Charge = selLeptons.at(1).charge;    
    TLep1_Id     = selLeptons.at(1).isElec ? 11 : 13;
    TLep1_GenMatch = selLeptons.at(1).GetGenMatch();
    TMll = (selLeptons.at(0).p + selLeptons.at(1).p).M();      
    TDeltaPhi = (selLeptons.at(0).p.DeltaPhi(selLeptons.at(1).p));
    TDeltaEta = TMath::Abs(TMath::Abs(selLeptons.at(0).p.Eta()) - TMath::Abs(selLeptons.at(1).p.Eta()));
  }
  TPassTrigger = passTrigger;
  TPassMETfilters = passMETfilters;
  if(gIsData) return;  
}

void StopTopAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TBtagPt = 0;
  TBtagSFFS   = BtagSFFS;
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0; TNBtagsBtagUp = 0; TNBtagsBtagDown = 0;
  TNBtagsMisTagUp = 0;  TNBtagsMisTagDown = 0;
  for(Int_t i = 0; i < 20; i++){
    if(i < TNJets){
      TJet_Pt[i]     = selJets.at(i).Pt();
      TJet_Eta[i]    = selJets.at(i).Eta();
      TJet_Phi[i]    = selJets.at(i).Phi();
      TJet_E[i]      = selJets.at(i).E();
      TJet_isBJet[i] = selJets.at(i).isBtag;
      if(selJets.at(i).isBtag){
            TNBtags++;
            if(TBtagPt == 0) TBtagPt = selJets.at(i).Pt();
      }
    }
    else{
      TJet_Pt[i]     = 0;
      TJet_Eta[i]    = 0;
      TJet_Phi[i]    = 0;
      TJet_E[i]      = 0;
      TJet_isBJet[i] = 0;
    }
    THT += TJet_Pt[i];
  }
  if(gIsData) return;  // For systematics...
  TNISRJets = Int_t(Get<Float_t>("nISRJet30"));
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
  THTJESUp = 0; THTJESDown = 0;
  for(Int_t i = 0; i < (Int_t) cleanedJets15.size(); i++){
    if(cleanedJets15.at(i).pTJESUp > ptCut){
      TNJetsJESUp++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESUp++;
      TJetJESUp_Pt[i] = cleanedJets15.at(i).pTJESUp;
      THTJESUp += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJESDown > ptCut){
      TNJetsJESDown++;
      if(cleanedJets15.at(i).isBtag) TNBtagsJESDown++;
      TJetJESDown_Pt[i] = cleanedJets15.at(i).pTJESDown;
      THTJESDown += TJetJESUp_Pt[i];
    }
    if(cleanedJets15.at(i).pTJERUp > ptCut){
      TNJetsJERUp++;
      TJetJER_Pt[i] = cleanedJets15.at(i).pTJERUp;
    }
  }
}

void StopTopAnalysis::GetMET(){
  TMET        = Get<Float_t>("met_pt");
  TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
  TNVert      = Get<Int_t>("nVert");
  TIsSS       = isSS;
  TMET_METres = TMET*GetMetResolutionSF(TMET/fNormMetRes);
  TPfMET = TMET;
  if(TNLooseLeps>1){
    TMT2        = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET,        TMET_Phi);
    TMT2_METres = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET_METres, TMET_Phi);
  }
  if(gIsData) return;
  TMETMuScaleUp   = LepScaleToMET(selLeptons, TMET, TMET_Phi, 13,  1);
  TMETMuScaleDown = LepScaleToMET(selLeptons, TMET, TMET_Phi, 13, -1);
  TMETElScaleUp   = LepScaleToMET(selLeptons, TMET, TMET_Phi, 11,  1);
  TMETElScaleDown = LepScaleToMET(selLeptons, TMET, TMET_Phi, 11, -1);
  if(TNLooseLeps>1){
    TMT2MuScaleUp   = getMT2llLepScale(selLeptons.at(0), selLeptons.at(1), TMETMuScaleUp, TMET_Phi, 13,  1);
    TMT2MuScaleDown = getMT2llLepScale(selLeptons.at(0), selLeptons.at(1), TMETMuScaleUp, TMET_Phi, 13, -1);
    TMT2ElScaleUp   = getMT2llLepScale(selLeptons.at(0), selLeptons.at(1), TMETMuScaleUp, TMET_Phi, 11,  1);
    TMT2ElScaleDown = getMT2llLepScale(selLeptons.at(0), selLeptons.at(1), TMETMuScaleUp, TMET_Phi, 11, -1);
  }

  TgenMET     = Get<Float_t>("met_genPt");
  TMETMETUp = TMET; TMETMETDown = TMET;
  TMT2METDown = TMT2; TMT2METUp = TMT2;
  TMETJERUp   = GetParam<Float_t>("MET_JERUp");
  TMETJESUp   = GetParam<Float_t>("MET_JESUp");
  TMETJESDown = GetParam<Float_t>("MET_JESDown");
  if(gIsFastSim){
    TMET = (TMET + TgenMET)/2;
    Float_t difMET = TMath::Abs(TMET-TgenMET)/2;
    TMETMETUp = TMET + difMET;
    TMETMETDown = TMET - difMET;
    TMETJERUp   = JERtoMET(selJets, TMET, TMET_Phi);
    TMETJESUp   = JEStoMET(selJets, TMET, TMET_Phi,  1);
    TMETJESDown = JEStoMET(selJets, TMET, TMET_Phi, -1);
  }
  if(TNLooseLeps > 1){
    TMT2JERUp   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJERUp,   TMET_Phi);
    TMT2JESUp   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESUp,   TMET_Phi);
    TMT2JESDown = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETJESDown, TMET_Phi);
    if(gIsFastSim){
      TMT2METUp     = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETMETUp,   TMET_Phi);
      TMT2METDown   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETMETDown, TMET_Phi);
    }
  }
  TMETunclUp = TMET*GetMetUnclWeightUp(TMET); TMETunclDown = TMET*GetMetUnclWeightDown(TMET);
  if(TNLooseLeps > 1){
    TMT2unclUp     = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETunclUp,   TMET_Phi);
    TMT2unclDown   = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMETunclDown, TMET_Phi);
  }
  TISRweight     = 1;
  if(gDoISR) TISRweight = getISRJetsWeight(TNISRJets)/NormISRweights;
  Float_t diff   = TMath::Abs(TISRweight - 1)/2;
  TISRweightUp   = TISRweight + diff;
  TISRweightDown = TISRweight - diff;
  Int_t nLHEweights = Get<Int_t>("nLHEweight");
 // if(gIsLHE) for(Int_t i = 0; i < 9; i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
  if(gIsLHE) for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);
}

void StopTopAnalysis::GetGenInfo(){
  if(gIsData) return; 
  TgenMETPhi = Get<Float_t>("met_genPhi");
  TgenWeight = genWeight;
  TEventTruth = kLGMgood;
  if(TLep0_GenMatch == kLGMflip     || TLep1_GenMatch == kLGMflip    ) TEventTruth = kLGMflip;
  if(TLep0_GenMatch == kLGMconv     || TLep1_GenMatch == kLGMconv    ) TEventTruth = kLGMconv;
  if(TLep0_GenMatch == kLGMother    || TLep1_GenMatch == kLGMother   ) TEventTruth = kLGMother;
  if(TLep0_GenMatch == kLGMfake     || TLep1_GenMatch == kLGMfake    ) TEventTruth = kLGMfake;
//  if(TLep0_GenMatch == kLGMtoGenB   || TLep1_GenMatch == kLGMtoGenB  ) TEventTruth = kLGMtoGenB;
//  if(TLep0_GenMatch == kLGMtoGenLep && TLep1_GenMatch == kLGMtoGenLep) TEventTruth = kLGMtoGenLep;

  TLorentzVector top1; 
  TLorentzVector top2; 
  if (Get<Int_t>("nGenTop") > 1){
    TgenTop1Pt  = Get<Float_t>("GenTop_pt"  , 0);   TgenTop2Pt  = Get<Float_t>("GenTop_pt"  , 1);
    TgenTop1Eta = Get<Float_t>("GenTop_eta" , 0);   TgenTop2Eta = Get<Float_t>("GenTop_eta" , 1);
    TgenTop1Phi = Get<Float_t>("GenTop_phi" , 0);   TgenTop2Phi = Get<Float_t>("GenTop_phi" , 1);
    TgenTop1M   = Get<Float_t>("GenTop_mass", 0);   TgenTop2M   = Get<Float_t>("GenTop_mass", 1);
    TgenTop1ID  = Get<Int_t>("GenTop_pdgId", 0);    TgenTop2ID  = Get<Int_t>("GenTop_pdgId", 1);
  }
  top1.SetPtEtaPhiM(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M);
  top2.SetPtEtaPhiM(TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M);
  TgentopDeltaPhi = top1.DeltaPhi(top2);
  TgentopDeltaR   = top1.DeltaR(top2);

  TgenMET     = Get<Float_t>("met_genPt");
  TNgenLeps = genLeptons.size();
  if(TNgenLeps >= 2){
    TgenLep0_Pt     = genLeptons.at(0).Pt();    
    TgenLep0_Eta    = genLeptons.at(0).Eta();    
    TgenLep0_Phi    = genLeptons.at(0).Phi();    
    TgenLep0_M      = genLeptons.at(0).p.M();    
    TgenLep0_Id     = genLeptons.at(0).isMuon? 13 : 11;
    TgenLep0_Mid    = genLeptons.at(0).Mid;
    TgenLep0_Ch     = genLeptons.at(0).charge;    
    TgenLep1_Pt     = genLeptons.at(1).Pt();    
    TgenLep1_Eta    = genLeptons.at(1).Eta();    
    TgenLep1_Phi    = genLeptons.at(1).Phi();    
    TgenLep1_M      = genLeptons.at(1).p.M();    
    TgenLep1_Id     = genLeptons.at(1).isMuon? 13 : 11;
    TgenLep1_Mid    = genLeptons.at(1).Mid;
    TgenLep1_Ch     = genLeptons.at(1).charge;    
    TgenDeltaPhi = (genLeptons.at(0).p.DeltaPhi(genLeptons.at(1).p));
    TgenDeltaEta = TMath::Abs(TMath::Abs(genLeptons.at(0).p.Eta()) - TMath::Abs(genLeptons.at(1).p.Eta()));
    TgenMT2 = getMT2ll(genLeptons.at(0), genLeptons.at(1), TgenMET,     Get<Float_t>("met_genPhi"));
  }
  TNgenJets = genJets.size();
  TNgenBtags= 2; // Has to be modified
  for(Int_t i = 0; i < 20; i++){
    if(i < TNgenJets){
      TgenJet_Pt[i]     = genJets.at(i).Pt();
      TgenJet_Eta[i]    = genJets.at(i).Eta();
    }
    else{
      TgenJet_Pt[i]     = 0;
      TgenJet_Eta[i]    = 0;
    }
    TgenHT += TgenJet_Pt[i];
  }

  TnGenW = 0; TnGenStop = 0; TnGenLsp = 0; TnGenNeutrinos = 0;

  Int_t nGenPart = Get<Int_t>("nGenPart");
  Int_t id = 0; Int_t mid = 0;
  for(Int_t i = 0; i < nGenPart; i++){
    id = TMath::Abs(Get<Int_t>("GenPart_pdgId",i));
    //>>> This is a neutrino
    if(id == 12 || id == 14 || id == 16){
      mid = TMath::Abs(Get<Int_t>("GenPart_motherId",i));
      if(mid == 23 || mid == 24){ // From W/Z
        TnGenNeutrinos++;
        if(TnGenNeutrinos == 1){
          TgenNeu1Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenNeu1Eta = Get<Float_t>("GenPart_eta" , i);
          TgenNeu1Phi = Get<Float_t>("GenPart_phi" , i);
          TgenNeu1M   = Get<Float_t>("GenPart_mass", i);
        }
        else if(TnGenNeutrinos >=2){
          TgenNeu2Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenNeu2Eta = Get<Float_t>("GenPart_eta" , i);
          TgenNeu2Phi = Get<Float_t>("GenPart_phi" , i);
          TgenNeu2M   = Get<Float_t>("GenPart_mass", i);
        }
      }
    }
 
    //>>> This is a W
    if(id == 24){
      mid = TMath::Abs(Get<Int_t>("GenPart_motherId",i));
      if(mid == 6 || mid == 1000006){ // From top/stop
        TnGenW++;
        if(TnGenW == 1){
          TgenW1Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenW1Eta = Get<Float_t>("GenPart_eta" , i);
          TgenW1Phi = Get<Float_t>("GenPart_phi" , i);
          TgenW1M   = Get<Float_t>("GenPart_mass", i);
        }
        else if(TnGenW >=2){
          TgenW2Pt  = Get<Float_t>("GenPart_pt"  , i);   
          TgenW2Eta = Get<Float_t>("GenPart_eta" , i);
          TgenW2Phi = Get<Float_t>("GenPart_phi" , i);
          TgenW2M   = Get<Float_t>("GenPart_mass", i);
        }
      }
    }
    //>>> This is a stop 
    else if(id == 1000006){ 
      TnGenStop++;
      if(TnGenStop == 1){
        TgenStop1Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenStop1Eta = Get<Float_t>("GenPart_eta" , i);
        TgenStop1Phi = Get<Float_t>("GenPart_phi" , i);
        TgenStop1M   = Get<Float_t>("GenPart_mass", i);
        TgenStop1ID  = Get<Int_t>("GenPart_pdgId",i);
      }
      else if(TnGenStop >=2){
        TgenStop2Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenStop2Eta = Get<Float_t>("GenPart_eta" , i);
        TgenStop2Phi = Get<Float_t>("GenPart_phi" , i);
        TgenStop2M   = Get<Float_t>("GenPart_mass", i);
        TgenStop2ID   = Get<Int_t>("GenPart_pdgId",i);
      }
    }
    //>>> This is a neutralino
    else if(id == 1000022){
      TnGenLsp++;
      if(TnGenLsp == 1){
        TgenLsp1Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenLsp1Eta = Get<Float_t>("GenPart_eta" , i);
        TgenLsp1Phi = Get<Float_t>("GenPart_phi" , i);
        TgenLsp1M   = Get<Float_t>("GenPart_mass", i);
      }
      else if(TnGenLsp >=2){
        TgenLsp2Pt  = Get<Float_t>("GenPart_pt"  , i);   
        TgenLsp2Eta = Get<Float_t>("GenPart_eta" , i);
        TgenLsp2Phi = Get<Float_t>("GenPart_phi" , i);
        TgenLsp2M   = Get<Float_t>("GenPart_mass", i);
      }
    }
  }
  return; 
}

void StopTopAnalysis::SetMetNoNeutralinos(){
  TLorentzVector Chi1; TLorentzVector Chi2;
  TLorentzVector SUSYMET; TLorentzVector tmet; TLorentzVector tgenmet;
  Float_t TgenMET_Phi_noSUSy;
  Chi1.SetPtEtaPhiM(TgenNeu1Pt, TgenNeu1Eta, TgenNeu1Phi, TgenNeu1M);
  Chi2.SetPtEtaPhiM(TgenNeu2Pt, TgenNeu2Eta, TgenNeu2Phi, TgenNeu2M);
  TLorentzVector METnu = (-Chi1 - Chi2);
//  SUSYMET.SetPtEtaPhiM((Chi1 + Chi2).Pt(), 0, (Chi1 + Chi2).Phi(), 0);

  Chi1.SetPtEtaPhiM(TgenLsp1Pt, TgenLsp1Eta, TgenLsp1Phi, TgenLsp1M);
  Chi2.SetPtEtaPhiM(TgenLsp2Pt, TgenLsp2Eta, TgenLsp2Phi, TgenLsp2M);
  SUSYMET = Chi1 + Chi2; 
  tmet.SetPtEtaPhiM(TMET, 0, TMET_Phi, 0);
  tgenmet.SetPtEtaPhiM(TgenMET, 0, Get<Float_t>("met_genPhi"), 0);
  TMET_noSUSY = (tmet + SUSYMET).Et();
  TgenMET_noSUSY = (tgenmet + SUSYMET).Et();
  TgenMET_Phi_noSUSy = (tgenmet - ( - SUSYMET)).Phi();
  TMET_Phi_noSUSY = (tmet - ( - SUSYMET)).Phi();

//  TMET_noSUSY = METnu.Et();
//  TMET_Phi_noSUSY = METnu.Phi();
//  TgenMET_noSUSY = METnu.Pt();
//  TgenMET_Phi_noSUSy = METnu.Phi();
  if(TNLooseLeps >= 2)
    TMT2_noSUSY = getMT2ll(selLeptons.at(0), selLeptons.at(1), TMET_noSUSY, TMET_Phi_noSUSY);
  if(TNgenLeps >= 2)
    TgenMT2_noSUSY = getMT2ll(genLeptons.at(0), genLeptons.at(1), TgenMET_noSUSY,  TgenMET_Phi_noSUSy);

}

void StopTopAnalysis::GetMHT(){
  std::vector<TLorentzVector> particles;
  Float_t minDeltaR = 0.4;
  TLorentzVector pMHT;
  TLorentzVector p;
  Int_t n; Int_t i = 0;
  p.SetPtEtaPhiM(Get<Float_t>("LepGood_pt", 0), Get<Float_t>("LepGood_eta", 0), Get<Float_t>("LepGood_phi", 0), Get<Float_t>("LepGood_mass", 0));
  particles.push_back(p);
  // Leptons...
  n = Get<Int_t>("nLepGood");
  for(i = 1; i < n; i++){
    p.SetPtEtaPhiM(Get<Float_t>("LepGood_pt", i), Get<Float_t>("LepGood_eta", i), Get<Float_t>("LepGood_phi", i), Get<Float_t>("LepGood_mass", i));
    if(Cleaning(p, particles, minDeltaR))  particles.push_back(p);
  }

  // Central jets...
  n = Get<Int_t>("nJet");
  for(i = 0; i < n; i++){
    p.SetPtEtaPhiM(Get<Float_t>("Jet_pt",i), Get<Float_t>("Jet_eta",i), Get<Float_t>("Jet_phi", i), Get<Float_t>("Jet_mass",i));
    if(Cleaning(p, particles, minDeltaR))  particles.push_back(p);
  }
  // Fwd jets...
  n = Get<Int_t>("nJetFwd");
  for(i = 0; i < n; i++){
    p.SetPxPyPzE(Get<Float_t>("JetFwd_px",i), Get<Float_t>("JetFwd_py",i), Get<Float_t>("JetFwd_pz", i), Get<Float_t>("JetFwd_energy",i));
    if(Cleaning(p, particles, minDeltaR))  particles.push_back(p);
  }

  n = Get<Int_t>("nLepOther");
  for(i = 0; i < n; i++){
    p.SetPtEtaPhiM(Get<Float_t>("LepOther_pt",i), Get<Float_t>("LepOther_eta",i), Get<Float_t>("LepOther_phi", i), Get<Float_t>("LepOther_mass",i));
    if(Cleaning(p, particles, minDeltaR))  particles.push_back(p);
  }

  n = Get<Int_t>("nTauGood");
  for(i = 0; i < n; i++){
    p.SetPtEtaPhiM(Get<Float_t>("TauGood_pt",i), Get<Float_t>("TauGood_eta",i), Get<Float_t>("TauGood_phi", i), Get<Float_t>("TauGood_mass",i));
    if(Cleaning(p, particles, minDeltaR))  particles.push_back(p);
  }

/*  n = Get<Int_t>("nTauOther");
  for(i = 0; i < n; i++){
    p.SetPxPyPzE(Get<Float_t>("JetFwd_px",i), Get<Float_t>("JetFwd_py",i), Get<Float_t>("JetFwd_pz", i), Get<Float_t>("JetFwd_energy",i));
    pMHT += p;
  }*/
  pMHT = particles.at(0);
  Int_t npart = particles.size();
  for(i = 1; i < npart; i++) pMHT += particles.at(i);
  TMHT = pMHT.Et();
}

void StopTopAnalysis::SetPolWeights(){
  TWeight_PolR = 1; TWeight_PolL = 1;
  TLorentzVector stop1; TLorentzVector top1; TLorentzVector lep1;
  TLorentzVector stop2; TLorentzVector top2; TLorentzVector lep2;
  
  // 1 --> elec/muon, antitop, antistop
  // 2 --> ppositron/antimuon, top, stop
  if(TgenLep0_Ch == -1){ // 1 
    lep1.SetPtEtaPhiM(TgenLep0_Pt, TgenLep0_Eta, TgenLep0_Phi, TgenLep0_M);
    lep2.SetPtEtaPhiM(TgenLep1_Pt, TgenLep1_Eta, TgenLep1_Phi, TgenLep1_M);
  }
  else{ // 2
    lep2.SetPtEtaPhiM(TgenLep0_Pt, TgenLep0_Eta, TgenLep0_Phi, TgenLep0_M);
    lep1.SetPtEtaPhiM(TgenLep1_Pt, TgenLep1_Eta, TgenLep1_Phi, TgenLep1_M);
  }
  if(TgenTop1ID == -6){
    top1.SetPtEtaPhiM(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M);
    top2.SetPtEtaPhiM(TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M);
  }
  else{
    top2.SetPtEtaPhiM(TgenTop1Pt, TgenTop1Eta, TgenTop1Phi, TgenTop1M);
    top1.SetPtEtaPhiM(TgenTop2Pt, TgenTop2Eta, TgenTop2Phi, TgenTop2M);
  }
  if(TgenStop1ID == -1000006){
    stop1.SetPtEtaPhiM(TgenStop1Pt, TgenStop1Eta, TgenStop1Phi, TgenStop1M);
    stop2.SetPtEtaPhiM(TgenStop2Pt, TgenStop2Eta, TgenStop2Phi, TgenStop2M);
  }
  else{
    stop2.SetPtEtaPhiM(TgenStop1Pt, TgenStop1Eta, TgenStop1Phi, TgenStop1M);
    stop1.SetPtEtaPhiM(TgenStop2Pt, TgenStop2Eta, TgenStop2Phi, TgenStop2M);
  }

  TWeight_PolR = GetWeightPolRight(stop1, top1, lep1) * GetWeightPolRight(stop2, top2, lep2); 
  TWeight_PolL = GetWeightPolLetf( stop1, top1, lep1) * GetWeightPolLetf( stop2, top2, lep2); 
}
