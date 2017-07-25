//#define do2j1t
//#define   doingTraining

#include "TWAnalysis.h"

ClassImp(TWAnalysis);

bool GreaterThan(float i, float j){ return (i > j);}

TWAnalysis::TWAnalysis() : PAFChainItemSelector() {
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
void TWAnalysis::Summary(){}

void TWAnalysis::Initialise(){
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gIsTTbar     = false;
  gIsTW        = false;
  gIsLHE       = false;

  if (gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  if (gSampleName.Contains("TW")    || gSampleName.Contains("TbarW") ) gIsTW    = false;
  if (gSampleName == "TTbar_Powheg")   gIsLHE = true;

  makeTree = true;
  makeHistos = true;
  if(makeTree){
    fTree       = CreateTree("MiniTree","Created with PAF");
    fMini       = CreateTree("Mini"    , "MiniMiniTree");
    fMini1j1t   = CreateTree("Mini1j1t", "MiniMiniTree");
    fMini2j1t   = CreateTree("Mini2j1t", "MiniMiniTree");
    SetLeptonVariables();
    SetJetVariables();
    SetEventVariables();
    SetTWVariables();
  }
  setTWBDT();
  InitHistos();

  genLeptons  = std::vector<Lepton>();
  selLeptons  = std::vector<Lepton>();
  vetoLeptons = std::vector<Lepton>();
  selJets = std::vector<Jet>();
  selJetsJecUp = std::vector<Jet>();
  selJetsJecDown = std::vector<Jet>();
  selJetsJER     = std::vector<Jet>();
  Jets15  = std::vector<Jet>();
  genJets = std::vector<Jet>();
  mcJets  = std::vector<Jet>();
  vetoJets = std::vector<Jet>();
}

void TWAnalysis::InsideLoop(){

  

  ReSetTWVariables();
  // Vectors with the objects
  genLeptons  = GetParam<vector<Lepton>>("genLeptons");
  selLeptons  = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons = GetParam<vector<Lepton>>("vetoLeptons");
  selJets     = GetParam<vector<Jet>>("selJets");
  selJetsJecUp   = GetParam<vector<Jet>>("selJetsJecUp");
  selJetsJecDown = GetParam<vector<Jet>>("selJetsJecDown");
  selJetsJER     = GetParam<vector<Jet>>("selJetsJER");
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
  BtagSF           = GetParam<Float_t>("BtagSF"          );
  BtagSFBtagUp     = GetParam<Float_t>("BtagSFBtagUp"    );
  BtagSFBtagDown   = GetParam<Float_t>("BtagSFBtagDown"  );
  BtagSFMistagUp   = GetParam<Float_t>("BtagSFMistagUp"  );
  BtagSFMistagDown = GetParam<Float_t>("BtagSFMistagDown");

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
  if(genLeptons.size() > 2 ){ // MIND THE POSSIBLE SKIM (on reco leptons) IN THE SAMPLE!!
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
  // if (gSelection == iTopSelec){
  if (gIsTTbar && genLeptons.size() < 2) return; // Dilepton selection for ttbar!!!
  // }
  // else if (gSelection == iTWSelec){
  //   if (gIsTW && genLeptons.size() < 2) return; // Dilepton selection for tw!!
  // }
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

    TWeight             = NormWeight*ElecSF  *MuonSF*TrigSF*PUSF   *  BtagSF;
    TWeight_ElecEffUp   = NormWeight*ElecSFUp*MuonSF*TrigSF*PUSF   *  BtagSF;
    TWeight_ElecEffDown = NormWeight*ElecSFDo*MuonSF*TrigSF*PUSF   *  BtagSF;
    TWeight_MuonEffUp   = NormWeight*ElecSF*
      (MuonSF+TMath::Sqrt(TMath::Power(MuonSFUp-MuonSF,2)+TMath::Power(MuonSF*0.0122,2)))*TrigSF*PUSF*BtagSF;
    TWeight_MuonEffDown = NormWeight*ElecSF*
      (MuonSF-TMath::Sqrt(TMath::Power(MuonSFDo-MuonSF,2)+TMath::Power(MuonSF*0.0122,2)))*TrigSF*PUSF*BtagSF;
    TWeight_TrigUp     = NormWeight*lepSF*(TrigSF+TrigSFerr)*PUSF  *  BtagSF;
    TWeight_TrigDown   = NormWeight*lepSF*(TrigSF-TrigSFerr)*PUSF  *  BtagSF;
    TWeight_PUDown     = NormWeight*lepSF*TrigSF*PUSF_Up           *  BtagSF;
    TWeight_PUUp       = NormWeight*lepSF*TrigSF*PUSF_Down         *  BtagSF          ;
    TWeight_BtagUp     =  NormWeight*ElecSF*MuonSF*TrigSF*PUSF     *  BtagSFBtagUp    ; 
    TWeight_BtagDown   =  NormWeight*ElecSF*MuonSF*TrigSF*PUSF     *  BtagSFBtagDown  ; 
    TWeight_MistagUp   =  NormWeight*ElecSF*MuonSF*TrigSF*PUSF     *  BtagSFMistagUp  ; 
    TWeight_MistagDown =  NormWeight*ElecSF*MuonSF*TrigSF*PUSF     *  BtagSFMistagDown; 



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

      TLeadingLepPt     = selLeptons.at(0).p.Pt();
      TLeadingLepEta    = selLeptons.at(0).p.Eta();
      TDilepPt          = (selLeptons.at(0).p + selLeptons.at(1).p).Pt();
      TSubLeadingLepPt  = selLeptons.at(1).p.Pt();
      TSubLeadingLepEta = selLeptons.at(1).p.Eta();
      TMll              = (selLeptons.at(0).p + selLeptons.at(1).p).M();


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
	  if (TNJets == 1 && TNBtags == 1){
	    if(isSS) fHSSyields[gChannel][0] -> Fill(i1j1b, TWeight);
	    else{      fHyields[gChannel][0] -> Fill(i1j1b, TWeight);
	      FillHistos(gChannel, i1j1b); }
	  }
	  if (TNJets == 2 && TNBtags == 1){
	    if(isSS) fHSSyields[gChannel][0] -> Fill(i2j1b, TWeight);
	    else{      fHyields[gChannel][0] -> Fill(i2j1b, TWeight);
	      FillHistos(gChannel, i2j1b); }
	  }
	  if (TNJets == 2 && TNBtags == 2){
	    if(isSS) fHSSyields[gChannel][0] -> Fill(i2j2b, TWeight);
	    else{      fHyields[gChannel][0] -> Fill(i2j2b, TWeight);
	      FillHistos(gChannel, i2j2b); }
	  }
        }
      }
      // Option 1: for signal extraction
      if(TChannel == iElMu){
	if (TNBtags > 0 || TNBtagsJESUp > 0 || TNBtagsJESDown > 0 || TNBtagsJERUp > 0) {
	  CalculateTWVariables();
	  fTree->Fill();
	  // cout << "Filling " << TWeight << endl;
	}
      }
      
      // Option 2: for nB,nJets plot
      if(TChannel == iElMu)
	fMini->Fill();

      // Option 3: for 1j1t plots
      if(TChannel == iElMu){   // MET > 40 in ee, µµ
	if (TNBtags == 1 || TNBtagsJESUp == 1 || TNBtagsJESDown == 1 || TNBtagsJERUp == 1) {
	  if (TNJets == 1 || TNJetsJESUp == 1 || TNJetsJESDown == 1 || TNJetsJERUp == 1){
	    CalculateTWVariables();
	    if (TNBtags == 1 && TNJets == 1) cout << "var is " << DilepMETJetPt << " " 
						  << " " << THTtot << " "<< DilepmetjetOverHT  << endl;
	    fMini1j1t->Fill();
	  }
	}
      }

      // Option 4: for 2j1t plots
      if(TChannel == iElMu){   // MET > 40 in ee, µµ
	if (TNBtags == 1 || TNBtagsJESUp == 1 || TNBtagsJESDown == 1 || TNBtagsJERUp == 1) {
	  if (TNJets == 2 || TNJetsJESUp == 2 || TNJetsJESDown == 2 || TNJetsJERUp == 2){
	    CalculateTWVariables();
	    fMini2j1t->Fill();
	  }
	}
      }

    }
  }
}


//#####################################################################
// Functions
//------------------------------------------------------------------
void TWAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
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

void TWAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
  TNJets = selJets.size(); THT = 0;
  TNBtags = 0;
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
  
  TNVetoJets = vetoJets.size();
  if (TNVetoJets > 0){
    TVetoJet1_Pt     = vetoJets.at(0).Pt();
    TVetoJet1_Eta    = vetoJets.at(0).Eta();
  }
  else{
    TVetoJet1_Pt     = -99.;   
    TVetoJet1_Eta    = -99.;
  }

  if (TNVetoJets > 1){
    TVetoJet2_Pt     = vetoJets.at(1).Pt();
    TVetoJet2_Eta    = vetoJets.at(1).Eta();
  }
  else{
    TVetoJet2_Pt     = -99.;   
    TVetoJet2_Eta    = -99.;
  }

  if (TNVetoJets > 2){
    TVetoJet3_Pt     = vetoJets.at(2).Pt();
    TVetoJet3_Eta    = vetoJets.at(2).Eta();
  }
  else{
    TVetoJet3_Pt     = -99.;   
    TVetoJet3_Eta    = -99.;
  }
  


  SetParam("THT",THT);

  if(gIsData) return;  // For systematics...
  // for(Int_t i = 0; i < TNJets; i++){
  //   if(selJets.at(i).isBtag_BtagUp    ) TNBtagsUp++;
  //   if(selJets.at(i).isBtag_BtagDown  ) TNBtagsDown++;
  //   if(selJets.at(i).isBtag_MisTagUp  ) TNBtagsMisTagUp++;
  //   if(selJets.at(i).isBtag_MisTagDown) TNBtagsMisTagDown++;
  // }
  TNJetsJESUp    = 0;
  TNJetsJESDown  = 0;
  TNBtagsJESUp    = 0;
  TNBtagsJESDown  = 0;
  TNBtagsJERUp     = 0;
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
    // if(cleanedJets15.at(i).pTJERUpUp > ptCut){
    //   TNJetsJERUp++;
    //   TJetJERUp_Pt[i] = cleanedJets15.at(i).pTJERUpUp;
    //   if(cleanedJets15.at(i).isBtag) TNBtagsJERUp++;

    // }
  }
  TNJetsJERUp = selJetsJER.size();
  for (auto& jet : selJetsJER){
    if (jet.isBtag) TNBtagsJERUp++;
  }

}

void TWAnalysis::GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets){
  if(gIsData) return;
  nFiduJets = 0; nFidubJets = 0; 
  Int_t nGenJets = genJets.size();
  Int_t nmcJets = mcJets.size();
  for(Int_t i = 0; i < nGenJets; i++) if(genJets.at(i).p.Pt() > 30 && TMath::Abs(genJets.at(i).p.Eta()) < 2.4)                         nFiduJets++;
  for(Int_t i = 0; i <  nmcJets; i++) if(mcJets.at(i).p.Pt()  > 30 && TMath::Abs(mcJets.at(i).Eta())    < 2.4 && mcJets.at(i).isBtag)  nFidubJets++;
}

void TWAnalysis::GetMET(){
    TMET        = Get<Float_t>("met_pt");
    TMET_Phi    = Get<Float_t>("met_phi");  // MET phi
    if(gIsData) return;
    TMETJESUp   = Get<Float_t>("met_jecUp_pt"  );
    TMETJESDown = Get<Float_t>("met_jecDown_pt");
    TMET_PhiJESUp   = Get<Float_t>("met_jecUp_phi"  );
    TMET_PhiJESDown = Get<Float_t>("met_jecDown_phi");
    Float_t  diff_MET_JER_phi = GetParam<Float_t>("diff_MET_JER_phi");
    Float_t  diff_MET_JER_pt  = GetParam<Float_t>("diff_MET_JER_pt");

      TLorentzVector diff_MET_JER; diff_MET_JER.SetPtEtaPhiM(diff_MET_JER_pt, 0.,diff_MET_JER_phi, 0.);
    TLorentzVector vMET; vMET.SetPtEtaPhiM(TMET, 0., TMET_Phi, 0);

    TMET_PhiJERUp     = (vMET + diff_MET_JER).Phi();
    TMETJERUp         = (vMET + diff_MET_JER).Pt();

    TGenMET     = Get<Float_t>("met_genPt");
  if(gIsLHE)  for(Int_t i = 0; i < Get<Int_t>("nLHEweight"); i++)   TLHEWeight[i] = Get<Float_t>("LHEweight_wgt", i);


}

void TWAnalysis::InitHistos(){
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

void TWAnalysis::FillDYHistos(Int_t ch){
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

void TWAnalysis::FillHistos(Int_t ch, Int_t cut){
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

void TWAnalysis::SetLeptonVariables(){

  


#ifndef do2j1t
#ifndef doingTraining
  // fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  // fTree->Branch("TLep_Pt",     TLep_Pt,     "TLep_Pt[TNSelLeps]/F");
  // fTree->Branch("TLep_Eta",     TLep_Eta,     "TLep_Eta[TNSelLeps]/F");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
  fTree->Branch("TIsSS",      &TIsSS,      "TIsSS/B");
#endif
  // fTree->Branch("TLep_Phi",     TLep_Phi,     "TLep_Phi[TNSelLeps]/F");
  // fTree->Branch("TLep_E" ,     TLep_E ,     "TLep_E[TNSelLeps]/F");
  // fTree->Branch("TLep_Charge",  TLep_Charge, "TLep_Charge[TNSelLeps]/F");
  // fTree->Branch("TNVetoLeps",     &TNVetoLeps,     "TNVetoLeps/I");
  // fTree->Branch("TMll",      &TMll,      "TMll/F");
#else 
  // fTree->Branch("TNSelLeps",     &TNSelLeps,     "TNSelLeps/I");
  // fTree->Branch("TLep_Pt",     TLep_Pt,     "TLep_Pt[TNSelLeps]/F");
  // fTree->Branch("TLep_Eta",     TLep_Eta,     "TLep_Eta[TNSelLeps]/F");
  // fTree->Branch("TLep_Phi",     TLep_Phi,     "TLep_Phi[TNSelLeps]/F");
  // fTree->Branch("TLep_E" ,     TLep_E ,     "TLep_E[TNSelLeps]/F");
  // fTree->Branch("TLep_Charge",  TLep_Charge, "TLep_Charge[TNSelLeps]/F");
  fTree->Branch("TChannel",      &TChannel,      "TChannel/I");
#endif
}

void TWAnalysis::SetJetVariables(){
#ifndef do2j1t

  fTree->Branch("TNJets",           &TNJets,         "TNJets/I");
  fTree->Branch("TNBtags",       &TNBtags,     "TNBtags/I");
#ifndef doingTraining
  fTree->Branch("TJet_Pt",           TJet_Pt,           "TJet_Pt[TNJets]/F");
  // fTree->Branch("TJet_Eta",           TJet_Eta,           "TJet_Eta[TNJets]/F");
  
  fTree->Branch("TJet2_Pt"       ,           &TJet2_Pt       ,       "TJet2_Pt/F"       );
  fTree->Branch("TJet2_PtJESUp"  ,           &TJet2_PtJESUp  ,       "TJet2_PtJESUp/F"  );
  fTree->Branch("TJet2_PtJESDown",           &TJet2_PtJESDown,       "TJet2_PtJESDown/F");
  fTree->Branch("TJet2_PtJERUp"  ,           &TJet2_PtJERUp  ,       "TJet2_PtJERUp/F"  );


  
  // fTree->Branch("TNVetoJets",           &TNVetoJets,         "TNVetoJets/I");

  // fTree->Branch("TVetoJet1_Pt", &TVetoJet1_Pt, "TVetoJet1_Pt/F");
  // fTree->Branch("TVetoJet2_Pt", &TVetoJet2_Pt, "TVetoJet2_Pt/F");
  // fTree->Branch("TVetoJet3_Pt", &TVetoJet3_Pt, "TVetoJet3_Pt/F");

  // fTree->Branch("TVetoJet1_Eta", &TVetoJet1_Eta, "TVetoJet1_Eta/F");
  // fTree->Branch("TVetoJet2_Eta", &TVetoJet2_Eta, "TVetoJet2_Eta/F");
  // fTree->Branch("TVetoJet3_Eta", &TVetoJet3_Eta, "TVetoJet3_Eta/F");

  fTree->Branch("TNJetsJESUp",           &TNJetsJESUp,         "TNJetsJESUp/I");
  fTree->Branch("TNJetsJESDown",           &TNJetsJESDown,         "TNJetsJESDown/I");
  fTree->Branch("TNJetsJERUp",           &TNJetsJERUp,         "TNJetsJERUp/I");


  fTree->Branch("TNBtagsJESUp"  ,  &TNBtagsJESUp  , "TNBtagsJESUp/I"  );
  fTree->Branch("TNBtagsJESDown",  &TNBtagsJESDown, "TNBtagsJESDown/I");
  fTree->Branch("TNBtagsJERUp"    ,  &TNBtagsJERUp    , "TNBtagsJERUp/I"    );
#endif
  // fTree->Branch("TJetJESUp_Pt",      TJetJESUp_Pt,      "TJetJESUp_Pt[TNJetsJESUp]/F");
  // fTree->Branch("TJetJESDown_Pt",    TJetJESDown_Pt,    "TJetJESDown_Pt[TNJetsJESDown]/F");
  // fTree->Branch("TJetJER_Pt",        TJetJER_Pt,        "TJetJER_Pt[TNJetsJER]/F");
  // fTree->Branch("THTJESUp",     &THTJESUp,     "THTJESUp/F");
  // fTree->Branch("THTJESDown",   &THTJESDown,   "THTJESDown/F");
#else

  fTree->Branch("TNJets"         , &TNJets         ,       "TNJets/I"                  );
  fTree->Branch("TNBtags"        , &TNBtags        ,       "TNBtags/I"                 );
  // fTree->Branch("TJet_isBJet"    ,  TJet_isBJet    ,       "TJet_isBJet[TNJets]/I"     );
  // fTree->Branch("TJet_Pt"        ,  TJet_Pt        ,       "TJet_Pt[TNJets]/F"         );
  // fTree->Branch("TJet_Eta"       ,  TJet_Eta       ,       "TJet_Eta[TNJets]/F"        );
  // fTree->Branch("TJet_Phi"       ,  TJet_Phi       ,       "TJet_Phi[TNJets]/F"        );
  // fTree->Branch("TJet_E"         ,  TJet_E         ,       "TJet_E[TNJets]/F"          );
  // fTree->Branch("TNVetoJets"     , &TNVetoJets     ,       "TNVetoJets/I"              );
  // fTree->Branch("TVetoJet_isBJet",  TVetoJet_isBJet,       "TVetoJet_isBJet[TNJets]/I" );
  // fTree->Branch("TVetoJet_Pt"    ,  TVetoJet_Pt    ,       "TVetoJet_Pt[TNJets]/F"     );
  // fTree->Branch("TVetoJet_Eta"   ,  TVetoJet_Eta   ,       "TVetoJet_Eta[TNJets]/F"    );
  // fTree->Branch("TVetoJet_Phi"   ,  TVetoJet_Phi   ,       "TVetoJet_Phi[TNJets]/F"    );
  // fTree->Branch("TVetoJet_E"     ,  TVetoJet_E     ,       "TVetoJet_E[TNJets]/F"      );

  // fTree->Branch("THT"            ,          &THT   ,       "THT/F"                     );


#endif

}

void TWAnalysis::SetEventVariables(){

#ifndef do2j1t
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");
#ifndef doingTraining
  fTree->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fTree->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fTree->Branch("TWeight_ElecEffUp",     &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fTree->Branch("TWeight_ElecEffDown",   &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fTree->Branch("TWeight_MuonEffUp",     &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fTree->Branch("TWeight_MuonEffDown",   &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fTree->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fTree->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fTree->Branch("TWeight_PUUp",          &TWeight_PUUp,        "TWeight_PUUp/F");
  fTree->Branch("TWeight_PUDown",        &TWeight_PUDown     ,        "TWeight_PUDown/F");
  fTree->Branch("TWeight_MistagUp"   ,   &TWeight_MistagUp   ,"TWeight_MistagUp/F"  );
  fTree->Branch("TWeight_MistagDown" ,   &TWeight_MistagDown ,"TWeight_MistagDown/F");
  fTree->Branch("TWeight_BtagUp"   ,     &TWeight_BtagUp     ,"TWeight_BtagUp/F"    );
  fTree->Branch("TWeight_BtagDown" ,     &TWeight_BtagDown   ,"TWeight_BtagDown/F"  );
   
  fTree->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
#endif
  // fTree->Branch("TMET",         &TMET,         "TMET/F");
  // fTree->Branch("TGenMET",         &TGenMET,         "TGenMET/F");
  // fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");
  // fTree->Branch("TMETJESUp",    &TMETJESUp,    "TMETJESUp/F");
  // fTree->Branch("TMETJESDown",  &TMETJESDown,  "TMETJESDown/F");

#else 
  fTree->Branch("TWeight",      &TWeight,      "TWeight/F");

  // fTree->Branch("TMET",         &TMET,         "TMET/F");
  // fTree->Branch("TMET_Phi",     &TMET_Phi,     "TMET_Phi/F");


#endif
}


void TWAnalysis::SetTWVariables()
{
  fMini->Branch("TChannel"     , &TChannel     , "TChannel/I"      );
  fMini->Branch("TIsSS"        , &TIsSS        , "TIsSS/B"         );
  fMini->Branch("TNJets"       , &TNJets       , "TNJets/I"        );
  fMini->Branch("TNJetsJESUp"  , &TNJetsJESUp  , "TNJetsJESUp/I"   );
  fMini->Branch("TNJetsJESDown", &TNJetsJESDown, "TNJetsJESDown/I" );
  fMini->Branch("TNBtags"       , &TNBtags       , "TNBtags/I"        );
  fMini->Branch("TNBtagsJESUp"  , &TNBtagsJESUp  , "TNBtagsJESUp/I"   );
  fMini->Branch("TNBtagsJESDown", &TNBtagsJESDown, "TNBtagsJESDown/I" );

  fMini->Branch("TWeight",      &TWeight,      "TWeight/F");
  fMini->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fMini->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fMini->Branch("TWeight_ElecEffUp",     &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fMini->Branch("TWeight_ElecEffDown",   &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fMini->Branch("TWeight_MuonEffUp",     &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fMini->Branch("TWeight_MuonEffDown",   &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fMini->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fMini->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fMini->Branch("TWeight_PUUp",          &TWeight_PUUp,        "TWeight_PUUp/F");
  fMini->Branch("TWeight_PUDown",        &TWeight_PUDown     ,        "TWeight_PUDown/F");
  fMini->Branch("TWeight_MistagUp"   ,   &TWeight_MistagUp   ,"TWeight_MistagUp/F"  );
  fMini->Branch("TWeight_MistagDown" ,   &TWeight_MistagDown ,"TWeight_MistagDown/F");
  fMini->Branch("TWeight_BtagUp"   ,     &TWeight_BtagUp     ,"TWeight_BtagUp/F"    );
  fMini->Branch("TWeight_BtagDown" ,     &TWeight_BtagDown   ,"TWeight_BtagDown/F"  );
  fMini->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");


  fMini->Branch("TLeadingLepPt",         &TLeadingLepPt      , "TLeadingLepPt/F"    );
  fMini->Branch("TLeadingLepEta",        &TLeadingLepEta     , "TLeadingLepEta/F"    );
  fMini->Branch("TDilepPt"       ,       &TDilepPt           , "TDilepPt/F"         );
  fMini->Branch("TSubLeadingLepPt",      &TSubLeadingLepPt      , "TSubLeadingLepPt/F"    );
  fMini->Branch("TSubLeadingLepEta",     &TSubLeadingLepEta     , "TSubLeadingLepEta/F"    );
  fMini->Branch("TMll"             ,     &TMll                  , "TMll/F"    );




  fMini1j1t->Branch("TChannel"     , &TChannel     , "TChannel/I"      );
  fMini1j1t->Branch("TIsSS"        , &TIsSS        , "TIsSS/B"         );
  fMini1j1t->Branch("TWeight",      &TWeight,      "TWeight/F");
  fMini1j1t->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fMini1j1t->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fMini1j1t->Branch("TWeight_ElecEffUp",     &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fMini1j1t->Branch("TWeight_ElecEffDown",   &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fMini1j1t->Branch("TWeight_MuonEffUp",     &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fMini1j1t->Branch("TWeight_MuonEffDown",   &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fMini1j1t->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fMini1j1t->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fMini1j1t->Branch("TWeight_PUUp",          &TWeight_PUUp,        "TWeight_PUUp/F");
  fMini1j1t->Branch("TWeight_PUDown",        &TWeight_PUDown     ,        "TWeight_PUDown/F");
  fMini1j1t->Branch("TWeight_MistagUp"   ,   &TWeight_MistagUp   ,"TWeight_MistagUp/F"  );
  fMini1j1t->Branch("TWeight_MistagDown" ,   &TWeight_MistagDown ,"TWeight_MistagDown/F");
  fMini1j1t->Branch("TWeight_BtagUp"   ,     &TWeight_BtagUp     ,"TWeight_BtagUp/F"    );
  fMini1j1t->Branch("TWeight_BtagDown" ,     &TWeight_BtagDown   ,"TWeight_BtagDown/F"  );
  fMini1j1t->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fMini1j1t->Branch("TNJets"       , &TNJets       , "TNJets/I"        );
  fMini1j1t->Branch("TNJetsJESUp"  , &TNJetsJESUp  , "TNJetsJESUp/I"   );
  fMini1j1t->Branch("TNJetsJESDown", &TNJetsJESDown, "TNJetsJESDown/I" );
  fMini1j1t->Branch("TNBtags"       , &TNBtags       , "TNBtags/I"        );
  fMini1j1t->Branch("TNBtagsJESUp"  , &TNBtagsJESUp  , "TNBtagsJESUp/I"   );
  fMini1j1t->Branch("TNBtagsJESDown", &TNBtagsJESDown, "TNBtagsJESDown/I" );

  fMini1j1t->Branch("TnLooseCentral"        , &nLooseCentral      , "TnLooseCentral/F"       );
  fMini1j1t->Branch("TnBTotal"              , &nBTotal            , "TnBTotal/F"             );
  fMini1j1t->Branch("TDilepMETJetPt"        , &DilepMETJetPt      , "TDilepMETJetPt/F"       );
  fMini1j1t->Branch("TTHTtot"               , &THTtot             , "TTHTtot/F"              );
  fMini1j1t->Branch("TTJet1_pt"             , &TJet1_pt           , "TTJet1_pt/F"            );
  fMini1j1t->Branch("TTJetLooseCentralpt"   , &TJetLooseCentralpt , "TTJetLooseCentralpt/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtot" , &DilepmetjetOverHT  , "TDilepMETJetPt_THTtot/F");
  fMini1j1t->Branch("TMSys"                 , &MSys               , "TMSys/F"                );
  fMini1j1t->Branch("TC_jll"                , &C_jll              , "TC_jll/F"               );
  fMini1j1t->Branch("THTLepOverHT"          , &HTLepOverHT        , "THTLepOverHT/F"         );
  fMini1j1t->Branch("TDilepJetPt"           , &DilepJetPt         , "TDilepJetPt/F"          );

  fMini1j1t->Branch("TnLooseCentralJESUp"        , &nLooseCentralJESUp      , "TnLooseCentralJESUp/F"       );
  fMini1j1t->Branch("TnBTotalJESUp"              , &nBTotalJESUp            , "TnBTotalJESUp/F"             );
  fMini1j1t->Branch("TDilepMETJetPtJESUp"        , &DilepMETJetPtJESUp      , "TDilepMETJetPtJESUp/F"       );
  fMini1j1t->Branch("TTHTtotJESUp"               , &THTtotJESUp             , "TTHTtotJESUp/F"              );
  fMini1j1t->Branch("TTJet1_ptJESUp"             , &TJet1_ptJESUp           , "TTJet1_ptJESUp/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJESUp"   , &TJetLooseCentralptJESUp , "TTJetLooseCentralptJESUp/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJESUp" , &DilepmetjetOverHTJESUp  , "TDilepMETJetPt_THTtotJESUp/F");
  fMini1j1t->Branch("TMSys"                      , &MSysJESUp               , "TMSysJESUp/F"                );
  fMini1j1t->Branch("TC_jllJESUp"                , &C_jllJESUp              , "TC_jllJESUp/F"               );
  fMini1j1t->Branch("THTLepOverHTJESUp"          , &HTLepOverHTJESUp        , "THTLepOverHTJESUp/F"         );
  fMini1j1t->Branch("TDilepJetPtJESUp"           , &DilepJetPtJESUp         , "TDilepJetPtJESUp/F"          );


  fMini1j1t->Branch("TnLooseCentralJESDown"        , &nLooseCentralJESDown      , "TnLooseCentralJESDown/F"       );
  fMini1j1t->Branch("TnBTotalJESDown"              , &nBTotalJESDown            , "TnBTotalJESDown/F"             );
  fMini1j1t->Branch("TDilepMETJetPtJESDown"        , &DilepMETJetPtJESDown      , "TDilepMETJetPtJESDown/F"       );
  fMini1j1t->Branch("TTHTtotJESDown"               , &THTtotJESDown             , "TTHTtotJESDown/F"              );
  fMini1j1t->Branch("TTJet1_ptJESDown"             , &TJet1_ptJESDown           , "TTJet1_ptJESDown/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJESDown"   , &TJetLooseCentralptJESDown , "TTJetLooseCentralptJESDown/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJESDown" , &DilepmetjetOverHTJESDown  , "TDilepMETJetPt_THTtotJESDown/F");
  fMini1j1t->Branch("TMSys"                      , &MSysJESDown               , "TMSysJESDown/F"                );
  fMini1j1t->Branch("TC_jllJESDown"                , &C_jllJESDown              , "TC_jllJESDown/F"               );
  fMini1j1t->Branch("THTLepOverHTJESDown"          , &HTLepOverHTJESDown        , "THTLepOverHTJESDown/F"         );
  fMini1j1t->Branch("TDilepJetPtJESDown"           , &DilepJetPtJESDown         , "TDilepJetPtJESDown/F"          );


  fMini1j1t->Branch("TnLooseCentralJERUp"        , &nLooseCentralJERUp      , "TnLooseCentralJERUp/F"       );
  fMini1j1t->Branch("TnBTotalJERUp"              , &nBTotalJERUp            , "TnBTotalJERUp/F"             );
  fMini1j1t->Branch("TDilepMETJetPtJERUp"        , &DilepMETJetPtJERUp      , "TDilepMETJetPtJERUp/F"       );
  fMini1j1t->Branch("TTHTtotJERUp"               , &THTtotJERUp             , "TTHTtotJERUp/F"              );
  fMini1j1t->Branch("TTJet1_ptJERUp"             , &TJet1_ptJERUp           , "TTJet1_ptJERUp/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJERUp"   , &TJetLooseCentralptJERUp , "TTJetLooseCentralptJERUp/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJERUp" , &DilepmetjetOverHTJERUp  , "TDilepMETJetPt_THTtotJERUp/F");
  fMini1j1t->Branch("TMSys"                      , &MSysJERUp               , "TMSysJERUp/F"                );
  fMini1j1t->Branch("TC_jllJERUp"                , &C_jllJERUp              , "TC_jllJERUp/F"               );
  fMini1j1t->Branch("THTLepOverHTJERUp"          , &HTLepOverHTJERUp        , "THTLepOverHTJERUp/F"         );
  fMini1j1t->Branch("TDilepJetPtJERUp"           , &DilepJetPtJERUp         , "TDilepJetPtJERUp/F"          );



  fMini2j1t->Branch("TChannel"     , &TChannel     , "TChannel/I"      );
  fMini2j1t->Branch("TIsSS"        , &TIsSS        , "TIsSS/B"         );
  fMini2j1t->Branch("TWeight",      &TWeight,      "TWeight/F");
  fMini2j1t->Branch("TWeight_LepEffUp",      &TWeight_LepEffUp,      "TWeight_LepEffUp/F");
  fMini2j1t->Branch("TWeight_LepEffDown",    &TWeight_LepEffDown,    "TWeight_LepEffDown/F");
  fMini2j1t->Branch("TWeight_ElecEffUp",     &TWeight_ElecEffUp,      "TWeight_ElecEffUp/F");
  fMini2j1t->Branch("TWeight_ElecEffDown",   &TWeight_ElecEffDown,    "TWeight_ElecEffDown/F");
  fMini2j1t->Branch("TWeight_MuonEffUp",     &TWeight_MuonEffUp,      "TWeight_MuonEffUp/F");
  fMini2j1t->Branch("TWeight_MuonEffDown",   &TWeight_MuonEffDown,    "TWeight_MuonEffDown/F");
  fMini2j1t->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fMini2j1t->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fMini2j1t->Branch("TWeight_PUUp",          &TWeight_PUUp,        "TWeight_PUUp/F");
  fMini2j1t->Branch("TWeight_PUDown",        &TWeight_PUDown     ,        "TWeight_PUDown/F");
  fMini2j1t->Branch("TWeight_MistagUp"   ,   &TWeight_MistagUp   ,"TWeight_MistagUp/F"  );
  fMini2j1t->Branch("TWeight_MistagDown" ,   &TWeight_MistagDown ,"TWeight_MistagDown/F");
  fMini2j1t->Branch("TWeight_BtagUp"   ,     &TWeight_BtagUp     ,"TWeight_BtagUp/F"    );
  fMini2j1t->Branch("TWeight_BtagDown" ,     &TWeight_BtagDown   ,"TWeight_BtagDown/F"  );
  fMini2j1t->Branch("TLHEWeight",        TLHEWeight,         "TLHEWeight[254]/F");
  fMini2j1t->Branch("TNJets"       , &TNJets       , "TNJets/I"        );
  fMini2j1t->Branch("TNJetsJESUp"  , &TNJetsJESUp  , "TNJetsJESUp/I"   );
  fMini2j1t->Branch("TNJetsJESDown", &TNJetsJESDown, "TNJetsJESDown/I" );
  fMini2j1t->Branch("TNBtags"       , &TNBtags       , "TNBtags/I"        );
  fMini2j1t->Branch("TNBtagsJESUp"  , &TNBtagsJESUp  , "TNBtagsJESUp/I"   );
  fMini2j1t->Branch("TNBtagsJESDown", &TNBtagsJESDown, "TNBtagsJESDown/I" );

  fMini2j1t->Branch( "TjetPtSubLeading_"  , &jetPtSubLeading_ , "jetPtSubLeading_/F");
  fMini2j1t->Branch( "TDR_L1_J1"       	, &TDR_L1_J1        , "TDR_L1_J1/F");     
  fMini2j1t->Branch( "TDR_L1L2_J1J2"      , &TDR_L1L2_J1J2    , "TDR_L1L2_J1J2/F");
  fMini2j1t->Branch( "TDR_L1L2_J1J2MET"   , &TDR_L1L2_J1J2MET , "TDR_L1L2_J1J2MET/F");

  fMini2j1t->Branch( "TjetPtSubLeading_JESUp"   , &jetPtSubLeading_JESUp , "jetPtSubLeading_JESUp/F");
  fMini2j1t->Branch( "TDR_L1_J1JESUp"       	, &TDR_L1_J1JESUp        , "TDR_L1_J1JESUp/F");     
  fMini2j1t->Branch( "TDR_L1L2_J1J2JESUp"       , &TDR_L1L2_J1J2JESUp    , "TDR_L1L2_J1J2JESUp/F");
  fMini2j1t->Branch( "TDR_L1L2_J1J2METJESUp"    , &TDR_L1L2_J1J2METJESUp , "TDR_L1L2_J1J2METJESUp/F");

  fMini2j1t->Branch( "TjetPtSubLeading_JESDown"   , &jetPtSubLeading_JESDown , "jetPtSubLeading_JESDown/F");
  fMini2j1t->Branch( "TDR_L1_J1JESDown"       	, &TDR_L1_J1JESDown        , "TDR_L1_J1JESDown/F");     
  fMini2j1t->Branch( "TDR_L1L2_J1J2JESDown"       , &TDR_L1L2_J1J2JESDown    , "TDR_L1L2_J1J2JESDown/F");
  fMini2j1t->Branch( "TDR_L1L2_J1J2METJESDown"    , &TDR_L1L2_J1J2METJESDown , "TDR_L1L2_J1J2METJESDown/F");

  fMini2j1t->Branch( "TjetPtSubLeading_JERUp"   , &jetPtSubLeading_JERUp , "jetPtSubLeading_JERUp/F");
  fMini2j1t->Branch( "TDR_L1_J1JERUp"       	, &TDR_L1_J1JERUp        , "TDR_L1_J1JERUp/F");     
  fMini2j1t->Branch( "TDR_L1L2_J1J2JERUp"       , &TDR_L1L2_J1J2JERUp    , "TDR_L1L2_J1J2JERUp/F");
  fMini2j1t->Branch( "TDR_L1L2_J1J2METJERUp"    , &TDR_L1L2_J1J2METJERUp , "TDR_L1L2_J1J2METJERUp/F");
 

#ifndef do2j1t

#ifndef doingTraining
  // fTree->Branch("TBDTada"             , &TBDTada            , "TBDTada/F"        );
  // fTree->Branch("TBDTadaJESUp"        , &TBDTadaJESUp       , "TBDTadaJESUp/F"   );
  // fTree->Branch("TBDTadaJESDown"      , &TBDTadaJESDown     , "TBDTadaJESDown/F" );
  // fTree->Branch("TBDTJadaERUp"        , &TBDTadaJERUp         , "TBDTadaJERUp/F"   );
  // fTree->Branch("TBDTgrad"            , &TBDTgrad           , "TBDTgrad/F"       );
  // fTree->Branch("TBDTgradJESUp"       , &TBDTgradJESUp      , "TBDTgradJESUp/F"  );
  // fTree->Branch("TBDTgradJESDown"     , &TBDTgradJESDown    , "TBDTgradJESDown/F");
  // fTree->Branch("TBDTgradJERUp"       , &TBDTgradJERUp        , "TBDTgradJERUp/F"  );
  fTree->Branch("TBDT"                , &TBDT               , "TBDT/F"           );
  fTree->Branch("TBDTJESUp"           , &TBDTJESUp          , "TBDTJESUp/F"      );
  fTree->Branch("TBDTJESDown"         , &TBDTJESDown        , "TBDTJESDown/F"    );
  fTree->Branch("TBDTJERUp"           , &TBDTJERUp            , "TBDTJERUp/F"      );
#endif
  //fTree->Branch("TBDTJER"         , &TBDTJER        , "TBDTJER/F"        );
  // for bdt training
  // fTree->Branch("DilepMETJetPt"   , &DilepMETJetPt  , "DilepMETJetPt/F"  );
  // fTree->Branch("DilepMETJet1Pz"  , &DilepMETJet1Pz , "DilepMETJet1Pz/F" );
  // fTree->Branch("nLooseCentral"   , &nLooseCentral  , "nLooseCentral/F"  );
  // fTree->Branch("nLooseFwd"       , &nLooseFwd      , "nLooseFwd/F"      );
  // fTree->Branch("nBTotal"         , &nBTotal        , "nBTotal/F"        );
  // fTree->Branch("MSys"            , &MSys           , "MSys/F"           );
  // fTree->Branch("TJetLoosept"     , &TJetLoosept    , "TJetLoosept/F"    ); // loose jet pt
  // fTree->Branch("TJetLooseCentralpt"     , &TJetLooseCentralpt    , "TJetLooseCentralpt/F"    ); // loose jet pt
  // fTree->Branch("C_jll"           , &C_jll          , "C_jll/F"          );
  // fTree->Branch("DilepJetPt"      , &DilepJetPt     , "DilepJetPt/F"     );
  // fTree->Branch("TJet1_pt"        , &TJet1_pt       , "TJet1_pt/F"       );
  // fTree->Branch("HTLepOverHT"     , &HTLepOverHT    , "HTLepOverHT/F"    );
  // fTree->Branch("THTtot"          , &THTtot         , "THTtot/F"         );
  // fTree->Branch("TDilepPt"        , &TDilepPt       , "TDilepPt/F"       );
  // fTree->Branch("THTtot2j"        , &THTtot2j       , "THTtot2j/F"       );
  // fTree->Branch("nBLooseCentral"  , &nBLooseCentral , "nBLooseCentral/F" );
  // fTree->Branch("nBLooseFwd"      , &nBLooseFwd     , "nBLooseFwd/F"     );

  // 2j1t
#ifndef doingTraining
  // fTree->Branch("TDilepMETPt"    ,  &TDilepMETPt     , "TDilepMETPt/F");
  // fTree->Branch("TETSys"         ,  &TETSys          , "TETSys/F");
  // fTree->Branch("TET_LLJetMET"   ,  &TET_LLJetMET    , "TET_LLJetMET/F");
  // fTree->Branch("TDPtL1_L2"      ,  &TDPtL1_L2       , "TDPtL1_L2/F");
  // fTree->Branch("TDPtJ2_L2"      ,  &TDPtJ2_L2       , "TDPtJ2_L2/F");
  // fTree->Branch("TDR_L1_J1"      ,  &TDR_L1_J1       , "TDR_L1_J1/F");
  // fTree->Branch("TDR_L1L2_J1J2"  ,  &TDR_L1L2_J1J2   , "TDR_L1L2_J1J2/F");
  // fTree->Branch("TDR_L1L2_J1J2MET" , &TDR_L1L2_J1J2MET, "TDR_L1L2_J1J2MET/F");
  // fTree->Branch("THtRejJ2"       , &THtRejJ2         , "THtRejJ2/F");
  // fTree->Branch("TBDT2j1tv1"     , &TBDT2j1tv1       , "TBDT2j1tv1/F");
  // fTree->Branch("TBDT2j1tv2"     , &TBDT2j1tv2       , "TBDT2j1tv2/F");
  // fTree->Branch("TBDT2j1tv3"     , &TBDT2j1tv3       , "TBDT2j1tv3/F");
  fTree->Branch("TBDT2j1tJESUp"  , &TBDT2j1tJESUp    , "TBDT2j1tJESUp/F");
  fTree->Branch("TBDT2j1tJESDown", &TBDT2j1tJESDown  , "TBDT2j1tJESDown/F");
  fTree->Branch("TBDT2j1tJERUp"    , &TBDT2j1tJERUp      , "TBDT2j1tJERUp/F");
  // fTree->Branch("TBDT2j1t_DR" ,  &TBDT2j1t_DR   , "TBDT2j1t_DR/F");
  // fTree->Branch("TBDT2j1t_ot" ,  &TBDT2j1t_ot   , "TBDT2j1t_ot/F");
  

#endif

#endif
  fTree->Branch("TBDT2j1t"       , &TBDT2j1t         , "TBDT2j1t/F");
}


void TWAnalysis::ReSetTWVariables()
{
  hasTW = false;

  DilepMETJetPt  = -99;
  Lep1METJetPt   = -99;
  DPtDilep_JetMET= -99;
  DPtDilep_MET   = -99;
  DPtLep1_MET    = -99;
  DilepMETJet1Pz = -99;
  nLooseCentral  = -99;
  nLooseFwd      = -99;
  nBLooseCentral = -99;
  nBLooseFwd     = -99;
  TJet2csv       = -99;
  MSys           = -99;
  TJetLoosept    = -99;
  TJetLooseCentralpt = -99;
  C_jll          = -99;
  DilepJetPt     = -99;

  TBDTada           = -199;
  TBDTadaJESUp      = -199;
  TBDTadaJESDown    = -199;
  TBDTgrad           = -199;
  TBDTgradJESUp      = -199;
  TBDTgradJESDown    = -199;
  TBDT           = -199;
  TBDTJESUp      = -199;
  TBDTJESDown    = -199;
  
}

void TWAnalysis::CalculateTWVariables()
{
  if (hasTW) return;
  hasTW = true;

  get20Jets();
  if (TNJets == 1 && TNBtags == 1){
    // cout << "1j1b NOM"<< endl;
    DilepMETJetPt    =  getDilepMETJetPt()   ;
    DilepJetPt       =  getDilepJetPt()      ;
    Lep1METJetPt     =  getLep1METJetPt()    ;
    DPtDilep_JetMET  =  getDPtDilep_JetMET() ;
    DPtDilep_MET     =  getDPtDilep_MET()    ;
    DPtLep1_MET      =  getDPtLep1_MET()     ;    
    DilepMETJet1Pz   =  getDilepMETJet1Pz()  ;

    TLorentzVector met; met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    C_jll = (selJets[0].p + selLeptons[0].p + selLeptons[1].p).Et() / (selJets[0].p + selLeptons[0].p + selLeptons[1].p).E();
    TJet1_pt           = selJets.at(0).p.Pt();
    nBTotal            = nBLooseCentral + nBLooseFwd;  
    MSys               = getSysM();
    THTtot             = getHTtot();
    DilepmetjetOverHT  = DilepMETJetPt/THTtot          ;
    HTLepOverHT        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/THTtot    ;

  }
  else{
    DilepMETJetPt    =  -99.;
    DilepJetPt       =  -99.;
    Lep1METJetPt     =  -99.;
    DPtDilep_JetMET  =  -99.;
    DPtDilep_MET     =  -99.;
    DPtLep1_MET      =  -99.;    
    DilepMETJet1Pz   =  -99.;
    C_jll            =  -99.;
    TJet1_pt         =  -99.;
    nBTotal          =  -99.;
    DilepmetjetOverHT=  -99.;
    HTLepOverHT      =  -99.;
    MSys             =  -99.;
    THTtot           =  -99.;
  }

  if (!gIsData){
    if (TNJetsJESUp == 1 && TNBtagsJESUp == 1){
      // cout << "1j1b Up" << endl;
      DilepMETJetPtJESUp   =  getDilepMETJetPt(1)   ;
      DilepJetPtJESUp      =  getDilepJetPt("JESUp")      ;
      Lep1METJetPtJESUp    =  getLep1METJetPt("JESUp")    ;
      DPtDilep_JetMETJESUp =  getDPtDilep_JetMET("JESUp") ;
      DPtDilep_METJESUp    =  getDPtDilep_MET("JESUp")    ;
      DPtLep1_METJESUp     =  getDPtLep1_MET("JESUp")     ;    
      DilepMETJet1PzJESUp  =  getDilepMETJet1Pz("JESUp")  ;
      TLorentzVector met; met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
      C_jllJESUp = (selJetsJecUp[0].p + selLeptons[0].p + selLeptons[1].p).Et() / (selJetsJecUp[0].p + selLeptons[0].p + selLeptons[1].p).E();
      TJet1_ptJESUp           = selJetsJecUp.at(0).p.Pt();
      nBTotalJESUp            = nBLooseCentralJESUp + nBLooseFwdJESUp;  
      MSysJESUp               = getSysM("JESUp");
      THTtotJESUp             = getHTtot("JESUp");
      DilepmetjetOverHTJESUp  = DilepMETJetPtJESUp/THTtotJESUp          ;
      HTLepOverHTJESUp        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/THTtotJESUp    ;
    }
    else{
      DilepMETJetPtJESUp   = -99.;
      DilepJetPtJESUp      = -99.;
      Lep1METJetPtJESUp    = -99.;
      DPtDilep_JetMETJESUp = -99.;
      DPtDilep_METJESUp    = -99.;
      DPtLep1_METJESUp     = -99.;
      DilepMETJet1PzJESUp  = -99.;
      C_jllJESUp           = -99.;
      TJet1_ptJESUp           = -99.;
      nBTotalJESUp            = -99.;
      DilepmetjetOverHTJESUp  = -99.;
      HTLepOverHTJESUp        = -99.;
      MSysJESUp               = -99.;
      THTtotJESUp             = -99.; 
    }
    
    if (TNJetsJESDown == 1 && TNBtagsJESDown == 1){
      // cout << "1j1b Down"<< endl;
      DilepMETJetPtJESDown   =  getDilepMETJetPt(-1)   ;
      DilepJetPtJESDown      =  getDilepJetPt("JESDown")      ;
      Lep1METJetPtJESDown    =  getLep1METJetPt("JESDown")    ;
      DPtDilep_JetMETJESDown =  getDPtDilep_JetMET("JESDown") ;
      DPtDilep_METJESDown    =  getDPtDilep_MET("JESDown")    ;
      DPtLep1_METJESDown     =  getDPtLep1_MET("JESDown")     ;    
      DilepMETJet1PzJESDown  =  getDilepMETJet1Pz("JESDown")  ;
      TLorentzVector met; met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
      C_jllJESDown = (selJetsJecDown[0].p + selLeptons[0].p + selLeptons[1].p).Et() / (selJetsJecDown[0].p + selLeptons[0].p + selLeptons[1].p).E();
      TJet1_ptJESDown           = selJetsJecDown.at(0).p.Pt();
      nBTotalJESDown            = nBLooseCentralJESDown + nBLooseFwdJESDown;  
      THTtotJESDown             = getHTtot("JESDown");
      DilepmetjetOverHTJESDown  = DilepMETJetPtJESDown/THTtotJESDown          ;
      HTLepOverHTJESDown        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/ THTtotJESDown    ;
      MSysJESDown               = getSysM("JESDown");

    }
    else{
      DilepMETJetPtJESDown   = -99.;
      DilepJetPtJESDown      = -99.;
      Lep1METJetPtJESDown    = -99.;
      DPtDilep_JetMETJESDown = -99.;
      DPtDilep_METJESDown    = -99.;
      DPtLep1_METJESDown     = -99.;
      DilepMETJet1PzJESDown  = -99.;
      C_jllJESDown             = -99.;
      TJet1_ptJESDown          = -99.;
      nBTotalJESDown           = -99.;
      DilepmetjetOverHTJESDown = -99.;
      HTLepOverHTJESDown       = -99.;
      MSysJESDown              = -99.;
      THTtotJESDown            = -99.;
    }
    
    
    if (TNJetsJERUp == 1 && TNBtagsJERUp == 1){
      // cout << "1j1b JERUp"<< endl;
      DilepMETJetPtJERUp   =  getDilepMETJetPt(-2)   ; // -2 is JERUp :D
      DilepJetPtJERUp      =  getDilepJetPt("JER")      ;
      Lep1METJetPtJERUp    =  getLep1METJetPt("JER")    ;
      DPtDilep_JetMETJERUp =  getDPtDilep_JetMET("JER") ;
      DPtDilep_METJERUp    =  getDPtDilep_MET("JER")    ;
      DPtLep1_METJERUp     =  getDPtLep1_MET("JER")     ;    
      DilepMETJet1PzJERUp  =  getDilepMETJet1Pz("JER")  ;
      TLorentzVector met; met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
      C_jllJERUp = (selJetsJER[0].p + selLeptons[0].p + selLeptons[1].p).Et() / (selJetsJER[0].p + selLeptons[0].p + selLeptons[1].p).E();
      TJet1_ptJERUp           = selJetsJER.at(0).p.Pt();
      nBTotalJERUp            = nBLooseCentralJERUp + nBLooseFwdJERUp;  
      THTtotJERUp             = getHTtot("JER");
      HTLepOverHTJERUp        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/ THTtotJERUp    ;
      DilepmetjetOverHTJERUp  = DilepMETJetPtJERUp/THTtotJERUp          ;
      MSysJERUp               = getSysM("JER");

    }
    else{
      DilepMETJetPtJERUp   = -99.;
      DilepJetPtJERUp      = -99.;
      Lep1METJetPtJERUp    = -99.;
      DPtDilep_JetMETJERUp = -99.;
      DPtDilep_METJERUp    = -99.;
      DPtLep1_METJERUp     = -99.;
      DilepMETJet1PzJERUp  = -99.;
      C_jllJERUp             = -99.;
      TJet1_ptJERUp          = -99.;
      nBTotalJERUp           = -99.;
      DilepmetjetOverHTJERUp = -99.;
      HTLepOverHTJERUp       = -99.;
      MSysJERUp              = -99.;
      THTtotJERUp            = -99.;
    }


  }
  // if (TNJets == 2 && TNBtags == 1){
  //   // b is the b jet, j is the light jet
  //   // DR_j_bllmet = getDR_j_bllmet(); 
  //   M_l1_j      = getm_l1_j();
  //   M_l2_j      = getm_l2_j();

  //   JetPt       = getJetPt();       
  //   BPt         = getBPt();
  //   DR_j_b      = getDRJetB();
    
  // }
  // else{
  //   DR_j_bllmet = -99.;    
  //   M_l1_j      = -99.;
  //   M_l2_j      = -99.;
  //   DilepPt     = -99.;
  //   JetPt       = -99.;
  //   BPt         = -99.;
  //   DR_j_b      = -99.;
  // }

  // if (TNJets == 1 && TNJetsJESUp == 1 && TNJetsJESDown == 1 && TNBtags == 1 && TNBtagsJESUp == 1 && TNBtagsJESDown == 1){
  //   cout << "nLooseCentral"      << "   "<<  nLooseCentralJESUp     << "   "<< nLooseCentral      << "   "<< nLooseCentralJESDown     << endl;
  //   cout << "nLooseFwd"          << "   "<<  nLooseFwdJESUp         << "   "<< nLooseFwd          << "   "<< nLooseFwdJESDown         << endl;
  //   cout << "nBTotal"            << "   "<<  nBTotalJESUp           << "   "<< nBTotal            << "   "<< nBTotalJESDown           << endl;
  //   cout << "DilepMETJetPt"      << "   "<<  DilepMETJetPtJESUp     << "   "<< DilepMETJetPt      << "   "<< DilepMETJetPtJESDown     << endl;
  //   cout << "THT"                << "   "<<  THTJESUp               << "   "<< THT                << "   "<< THTJESDown               << endl;
  //   cout << "TJet1_pt"           << "   "<<  TJet1_ptJESUp          << "   "<< TJet1_pt           << "   "<< TJet1_ptJESDown          << endl;
  //   cout << "TJetLoosept"        << "   "<<  TJetLooseptJESUp       << "   "<< TJetLoosept        << "   "<< TJetLooseptJESDown       << endl;
  //   cout << "DilepmetjetOverHT"  << "   "<<  DilepmetjetOverHTJESUp << "   "<< DilepmetjetOverHT  << "   "<< DilepmetjetOverHTJESDown << endl;
  //   cout << "MSys"               << "   "<<  MSysJESUp              << "   "<< MSys               << "   "<< MSysJESDown              << endl;
  //   cout << "C_jll"              << "   "<<  C_jllJESUp             << "   "<< C_jll              << "   "<< C_jllJESDown             << endl;
  //   cout << "HTLepOverHT"        << "   "<<  HTLepOverHTJESUp       << "   "<< HTLepOverHT        << "   "<< HTLepOverHTJESDown       << endl;
  //   cout << "DilepJetPt"         << "   "<<  DilepJetPtJESUp        << "   "<< DilepJetPt         << "   "<< DilepJetPtJESDown        << endl;

  // }

  // cout << "The Comp " << TNJets << " " << TNJetsJER << endl;
  // if (TNJets == 1 && TNJetsJER == 1 && TNBtags == 1 && TNBtagsJER == 1){
  //   cout << "nLooseCentral"      << "   "<< nLooseCentral      << "   "<< nLooseCentralJER     << endl;
  //   cout << "nLooseFwd"          << "   "<< nLooseFwd          << "   "<< nLooseFwdJER         << endl;
  //   cout << "nBTotal"            << "   "<< nBTotal            << "   "<< nBTotalJER           << endl;
  //   cout << "DilepMETJetPt"      << "   "<< DilepMETJetPt      << "   "<< DilepMETJetPtJER     << endl;
  //   cout << "THT"                << "   "<< THT                << "   "<< THTJER               << endl;
  //   cout << "TJet1_pt"           << "   "<< TJet1_pt           << "   "<< TJet1_ptJER          << endl;
  //   cout << "TJetLoosept"        << "   "<< TJetLoosept        << "   "<< TJetLooseptJER       << endl;
  //   cout << "DilepmetjetOverHT"  << "   "<< DilepmetjetOverHT  << "   "<< DilepmetjetOverHTJER << endl;
  //   cout << "MSys"               << "   "<< MSys               << "   "<< MSysJER              << endl;
  //   cout << "C_jll"              << "   "<< C_jll              << "   "<< C_jllJER             << endl;
  //   cout << "HTLepOverHT"        << "   "<< HTLepOverHT        << "   "<< HTLepOverHTJER       << endl;
  //   cout << "DilepJetPt"         << "   "<< DilepJetPt         << "   "<< DilepJetPtJER        << endl;

  // }

  TDilepPt                 = getDilepPt();      

  if (TNJets == 1 && TNBtags == 1 ){
    TBDTada = BDTada->EvaluateMVA("BDTada");
    TBDTgrad = BDTgrad->EvaluateMVA("BDTgrad");
    TBDT = BDT->EvaluateMVA("BDT");
  }
  else{
    TBDTada = -99.;    
    TBDTgrad = -99.;
    TBDT = -99.;
  }
  if (!gIsData){
    if (TNJetsJESUp == 1 && TNBtagsJESUp == 1){
      TBDTadaJESUp = BDTada_JESUp->EvaluateMVA("BDTada_JESUp");
      TBDTgradJESUp = BDTgrad_JESUp->EvaluateMVA("BDTgrad_JESUp");
      TBDTJESUp = BDT_JESUp->EvaluateMVA("BDT_JESUp");
    }
    else{
      TBDTadaJESUp = -99.;
      TBDTgradJESUp = -99.;
      TBDTJESUp = -99.;
    }
    
    if (TNJetsJESDown == 1 && TNBtagsJESDown == 1){
      TBDTadaJESDown = BDTada_JESDown->EvaluateMVA("BDTada_JESDown");
      TBDTgradJESDown = BDTgrad_JESDown->EvaluateMVA("BDTgrad_JESDown");
      TBDTJESDown = BDT_JESDown->EvaluateMVA("BDT_JESDown");
    }
    else{
      TBDTadaJESDown = -99.;
      TBDTgradJESDown = -99.;
      TBDTJESDown = -99.;
    }
   if (TNJetsJERUp == 1 && TNBtagsJERUp == 1){
      TBDTadaJERUp = BDTada_JER->EvaluateMVA("BDTada_JER");
      TBDTgradJERUp = BDTgrad_JER->EvaluateMVA("BDTgrad_JER");
      TBDTJERUp = BDT_JER->EvaluateMVA("BDT_JER");
    }
    else{
      TBDTadaJERUp = -99.;
      TBDTgradJERUp = -99.;
      TBDTJERUp = -99.;
    }
  }
  if (TNJets == 2 && TNBtags == 1){
    // cout << "Nominal " << endl;
    TDilepMETPt     = getDilepMETPt();
    TETSys          = getETSys(); // faltan los systs!
    TET_LLJetMET    = getET_LLJetMET(); // faltan los systs!
    THtRejJ2        = selLeptons.at(0).Pt() + selLeptons.at(1).Pt() +  selJets.at(0).Pt() + TMET;
    TDPtL1_L2       = selLeptons.at(0).Pt() - selLeptons.at(1).Pt();
    TDPtJ2_L2       = selJets.at(1).Pt()    - selLeptons.at(1).Pt();
    TDR_L1_J1       = getDeltaRLep1_Jet1();
    TDR_L1L2_J1J2   = getDeltaRDilep_Jets12();
    TDR_L1L2_J1J2MET= getDeltaRDilep_METJets12();
    LeadingLeptPt_    = selLeptons.at(0).Pt();
    LeadingLeptEta_   = selLeptons.at(0).Eta();
    jetPtSubLeading_  = selJets.at(1).Pt();
    jetEtaSubLeading_ = selJets.at(1).Eta();
    THTtot2j          = getHTtot2j();
    // cout << "evaluating 2j1b" << endl;
    TBDT2j1t   = BDT2j1t->EvaluateMVA("2j1b");
    // TBDT2j1tv1 = BDT2j1tv1->EvaluateMVA("2j1bv1");
    // TBDT2j1tv2 = BDT2j1tv2->EvaluateMVA("2j1bv2");
    // TBDT2j1tv3 = BDT2j1tv3->EvaluateMVA("2j1bv3");
    // TBDT2j1t_DR   = BDT2j1t_DR->EvaluateMVA("2j1b");
    // TBDT2j1t_ot   = BDT2j1t_ot->EvaluateMVA("2j1b");


  }
  else{
    TDilepMETPt      = -99;
    TETSys           = -99.;
    TET_LLJetMET     = -99.;
    TDPtL1_L2        = -99.;
    TDPtJ2_L2        = -99.;
    TDR_L1_J1        = -99.;
    TDR_L1L2_J1J2    = -99.;
    TDR_L1L2_J1J2MET = -99.;
    THtRejJ2         = -99.;
    THTtot2j         = -99.;
    TBDT2j1t         = -99.;       
    // TBDT2j1tv1       = -99.;       
    // TBDT2j1tv2       = -99.;       
    // TBDT2j1tv3       = -99.;       
    TBDT2j1t_DR   = -99.;
    TBDT2j1t_ot   = -99.;
  }
	
  if (!gIsData){

    if (TNJetsJESUp == 2 && TNBtagsJESUp == 1){
      // cout << "Up " << endl;
      TDilepMETPtJESUp       = getDilepMETPt(+1);
      TETSysJESUp            = getETSys(+1); // faltan los systs!
      TET_LLJetMETJESUp      = getET_LLJetMET(+1); // faltan los systs!
      THtRejJ2JESUp          = selLeptons.at(0).Pt() + selLeptons.at(1).Pt() +  selJetsJecUp.at(0).Pt() + TMETJESUp;
      TDPtL1_L2JESUp         = selLeptons.at(0).Pt() - selLeptons.at(1).Pt();
      TDPtJ2_L2JESUp         = selJetsJecUp.at(1).Pt()    - selLeptons.at(1).Pt();
      TDR_L1_J1JESUp         = getDeltaRLep1_Jet1(1);
      TDR_L1L2_J1J2JESUp     = getDeltaRDilep_Jets12(1);
      TDR_L1L2_J1J2METJESUp  = getDeltaRDilep_METJets12(1);
      LeadingLeptPt_JESUp    = selLeptons.at(0).Pt();
      LeadingLeptEta_JESUp   = selLeptons.at(0).Eta();
      jetPtSubLeading_JESUp  = selJetsJecUp.at(1).Pt();
      jetEtaSubLeading_JESUp = selJetsJecUp.at(1).Eta();
      THTtot2jJESUp          = getHTtot2j("JESUp");
      // cout << "evaluating 2j1b" << endl;
      TBDT2j1tJESUp          = BDT2j1tJESUp->EvaluateMVA("2j1b");
    }
    else{
      TDilepMETPtJESUp       = -99.;
      TETSysJESUp            = -99.;
      TET_LLJetMETJESUp      = -99.;
      THtRejJ2JESUp          = -99.;
      TDPtL1_L2JESUp         = -99.;
      TDPtJ2_L2JESUp         = -99.;
      TDR_L1_J1JESUp         = -99.;
      TDR_L1L2_J1J2JESUp     = -99.;
      TDR_L1L2_J1J2METJESUp  = -99.;
      LeadingLeptPt_JESUp    = -99.;
      LeadingLeptEta_JESUp   = -99.;
      jetPtSubLeading_JESUp  = -99.;
      jetEtaSubLeading_JESUp = -99.;
      THTtot2jJESUp          = -99.;
      TBDT2j1tJESUp          = -99.;   
    }
    
    if (TNJetsJESDown == 2 && TNBtagsJESDown == 1){
      // cout << "Down " << endl;
      TDilepMETPtJESDown       = getDilepMETPt(-1);
      TETSysJESDown            = getETSys(-1); // faltan los systs!
      TET_LLJetMETJESDown      = getET_LLJetMET(-1); // faltan los systs!
      THtRejJ2JESDown          = selLeptons.at(0).Pt() + selLeptons.at(1).Pt() +  selJetsJecDown.at(0).Pt() + TMETJESDown;
      TDPtL1_L2JESDown         = selLeptons.at(0).Pt() - selLeptons.at(1).Pt();
      TDPtJ2_L2JESDown         = selJetsJecDown.at(1).Pt()    - selLeptons.at(1).Pt();
      TDR_L1_J1JESDown         = getDeltaRLep1_Jet1(-1);
      TDR_L1L2_J1J2JESDown     = getDeltaRDilep_Jets12(-1);
      TDR_L1L2_J1J2METJESDown  = getDeltaRDilep_METJets12(-1);
      LeadingLeptPt_JESDown    = selLeptons.at(0).Pt();
      LeadingLeptEta_JESDown   = selLeptons.at(0).Eta();
      jetPtSubLeading_JESDown  = selJetsJecDown.at(1).Pt();
      jetEtaSubLeading_JESDown = selJetsJecDown.at(1).Eta();
      THTtot2jJESDown          = getHTtot2j("JESDown");
      // cout << "evaluating 2j1b" << endl;
      TBDT2j1tJESDown = BDT2j1tJESDown->EvaluateMVA("2j1b");
    }
    else{
      TDilepMETPtJESDown       = -99.;
      TETSysJESDown            = -99.;
      TET_LLJetMETJESDown      = -99.;
      THtRejJ2JESDown          = -99.;
      TDPtL1_L2JESDown         = -99.;
      TDPtJ2_L2JESDown         = -99.;
      TDR_L1_J1JESDown         = -99.;
      TDR_L1L2_J1J2JESDown     = -99.;
      TDR_L1L2_J1J2METJESDown  = -99.;
      LeadingLeptPt_JESDown    = -99.;
      LeadingLeptEta_JESDown   = -99.;
      jetPtSubLeading_JESDown  = -99.;
      jetEtaSubLeading_JESDown = -99.;
      THTtot2jJESDown          = -99.;
      TBDT2j1tJESDown          = -99.;   
    }

    if (TNJetsJERUp == 2 && TNBtagsJERUp == 1){
      // cout << "JER " << endl;
      TDilepMETPtJERUp       = getDilepMETPt(-2);
      // cout << "check A " << endl;
      TETSysJERUp            = getETSys(-2); // faltan los systs!
      // cout << "check B " << endl;
      TET_LLJetMETJERUp      = getET_LLJetMET(-2); // faltan los systs!
      // cout << "check C " << endl;
      THtRejJ2JERUp          = selLeptons.at(0).Pt() + selLeptons.at(1).Pt() +  selJetsJER.at(0).Pt() + TMETJERUp;
      // cout << "check D " << endl;
      TDPtL1_L2JERUp         = selLeptons.at(0).Pt() - selLeptons.at(1).Pt();
      // cout << "check E " << endl;
      TDPtJ2_L2JERUp         = selJetsJER.at(1).Pt() - selLeptons.at(1).Pt();
      // cout << "check F " << endl;
      TDR_L1_J1JERUp         = getDeltaRLep1_Jet1(-2);
      // cout << "check G " << endl;
      TDR_L1L2_J1J2JERUp     = getDeltaRDilep_Jets12(-2);
      // cout << "check H " << endl;
      TDR_L1L2_J1J2METJERUp  = getDeltaRDilep_METJets12(-2);
      // cout << "check I " << endl;
      LeadingLeptPt_JERUp    = selLeptons.at(0).Pt();
      // cout << "check J " << endl;
      LeadingLeptEta_JERUp   = selLeptons.at(0).Eta();
      // cout << "check K " << endl;
      jetPtSubLeading_JERUp  = selJetsJER.at(1).Pt();
      // cout << "check L " << endl;
      jetEtaSubLeading_JERUp = selJetsJER.at(1).Eta();
      THTtot2jJERUp          = getHTtot2j("JER");
      // cout << "evaluating 2j1b" << endl;
      TBDT2j1tJERUp = BDT2j1tJER->EvaluateMVA("2j1b");


    }
    else{
      TDilepMETPtJERUp       = -99.;
      TETSysJERUp            = -99.;
      TET_LLJetMETJERUp      = -99.;
      THtRejJ2JERUp          = -99.;
      TDPtL1_L2JERUp         = -99.;
      TDPtJ2_L2JERUp         = -99.;
      TDR_L1_J1JERUp         = -99.;
      TDR_L1L2_J1J2JERUp     = -99.;
      TDR_L1L2_J1J2METJERUp  = -99.;
      LeadingLeptPt_JERUp    = -99.;
      LeadingLeptEta_JERUp   = -99.;
      jetPtSubLeading_JERUp  = -99.;
      jetEtaSubLeading_JERUp = -99.;
      THTtot2jJERUp          = -99.;
      TBDT2j1tJERUp          = -99.;   
    }
  }

  // if (TNJetsJER == 2 && TNBtagsJER == 1 && TNJetsJESUp == 2 && TNBtagsJESUp == 1){
    
  //   cout << "TDilepMETPt       " << TDilepMETPt        << "  " << TDilepMETPtJER        << endl;
  //   cout << "TETSys            " << TETSys             << "  " << TETSysJER             << endl;
  //   cout << "TET_LLJetMET      " << TET_LLJetMET       << "  " << TET_LLJetMETJER       << endl;
  //   cout << "THtRejJ2          " << THtRejJ2           << "  " << THtRejJ2JER           << endl;
  //   cout << "TDPtL1_L2         " << TDPtL1_L2          << "  " << TDPtL1_L2JER          << endl;
  //   cout << "TDPtJ2_L2         " << TDPtJ2_L2          << "  " << TDPtJ2_L2JER          << endl;
  //   cout << "TDR_L1_J1         " << TDR_L1_J1          << "  " << TDR_L1_J1JER          << endl;
  //   cout << "TDR_L1L2_J1J2     " << TDR_L1L2_J1J2      << "  " << TDR_L1L2_J1J2JER      << endl;
  //   cout << "TDR_L1L2_J1J2MET  " << TDR_L1L2_J1J2MET   << "  " << TDR_L1L2_J1J2METJER   << endl;
  //   cout << "LeadingLeptPt_    " << LeadingLeptPt_     << "  " << LeadingLeptPt_JER     << endl;
  //   cout << "LeadingLeptEta_   " << LeadingLeptEta_    << "  " << LeadingLeptEta_JER    << endl;
  //   cout << "jetPtSubLeading_  " << jetPtSubLeading_   << "  " << jetPtSubLeading_JER   << endl;
  //   cout << "jetEtaSubLeading_ " << jetEtaSubLeading_  << "  " << jetEtaSubLeading_JER  << endl;
  //   cout << "THTtot2j          " << THTtot2j           << "  " << THTtot2jJER           << endl;

  // }


  if (TNJets > 1)        TJet2_Pt        = selJets.at(1).Pt();
  else                   TJet2_Pt        = -99.;
  if (TNJetsJESUp > 1)   TJet2_PtJESUp   = selJetsJecUp.at(1).Pt();
  else                   TJet2_PtJESUp   = -99.;
  if (TNJetsJESDown > 1) TJet2_PtJESDown = selJetsJecDown.at(1).Pt();
  else                   TJet2_PtJESDown = -99.;
  if (TNJetsJERUp > 1)   TJet2_PtJERUp   = selJetsJER.at(1).Pt();
  else                   TJet2_PtJERUp   = -99.;
 






  return;


}


void TWAnalysis::get20Jets()
{
  vector<float> looseJetPt;
  vector<float> looseJetPtJESUp;
  vector<float> looseJetPtJESDown;
  vector<float> looseJetPtJER;

  vector<float> looseJetCentralPt;
  vector<float> looseJetCentralPtJESUp;
  vector<float> looseJetCentralPtJESDown;
  vector<float> looseJetCentralPtJER;
  
  nLooseCentral  = 0.;   nLooseCentralJESUp  = 0.; nLooseCentralJESDown  = 0.;  nLooseCentralJERUp  = 0.; 
  nLooseFwd      = 0.;   nLooseFwdJESUp      = 0.; nLooseFwdJESDown      = 0.;  nLooseFwdJERUp      = 0.; 
  nBLooseCentral = 0.;   nBLooseCentralJESUp = 0.; nBLooseCentralJESDown = 0.;  nBLooseCentralJERUp = 0.; 
  nBLooseFwd     = 0.;   nBLooseFwdJESUp     = 0.; nBLooseFwdJESDown     = 0.;  nBLooseFwdJERUp     = 0.; 
  TJet2csv       = 0.;   TJet2csvJESUp       = 0.; TJet2csvJESDown       = 0.;  TJet2csvJERUp       = 0.; 
  TJetLoosept    = 0.;   TJetLooseptJESUp    = 0.; TJetLooseptJESDown    = 0.;  TJetLooseptJERUp    = 0.; 
  TJetLooseCentralpt           = 0.;  TJetLooseCentralptJESUp    = 0.;
  TJetLooseCentralptJESDown    = 0.;  TJetLooseCentralptJERUp      = 0.; 

  for (unsigned int j = 0; j < vetoJets.size(); ++j){
    if (vetoJets.at(j).p.Pt() > 20.){
      looseJetPt.push_back( vetoJets.at(j).p.Pt() );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
	nLooseCentral++;
	looseJetCentralPt.push_back(vetoJets.at(j).p.Pt());
      }
      else nLooseFwd++;
      if (vetoJets.at(j).isBtag){
	if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentral++;
	else nBLooseFwd++;
      }
    }

    if (vetoJets.at(j).pTJESUp > 20.){
      looseJetPtJESUp.push_back( vetoJets.at(j).pTJESUp );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
	nLooseCentralJESUp++;
	looseJetCentralPtJESUp.push_back(vetoJets.at(j).pTJESUp);
      }
      else nLooseFwdJESUp++;
      if (vetoJets.at(j).isBtag){
	if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJESUp++;
	else nBLooseFwdJESUp++;
      }
    }

    if (vetoJets.at(j).pTJESDown > 20.){
      looseJetPtJESDown.push_back( vetoJets.at(j).pTJESDown );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
	nLooseCentralJESDown++;
	looseJetCentralPtJESDown.push_back(vetoJets.at(j).pTJESDown);
      }
      else nLooseFwdJESDown++;
      if (vetoJets.at(j).isBtag){
	if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJESDown++;
	else nBLooseFwdJESDown++;
      }
    }

    if (vetoJets.at(j).pTJERUp > 20.){
      looseJetPtJER.push_back( vetoJets.at(j).pTJERUp );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
	nLooseCentralJERUp++;
	looseJetCentralPtJER.push_back(vetoJets.at(j).pTJERUp);
      }
      else nLooseFwdJERUp++;
      if (vetoJets.at(j).isBtag){
	if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJERUp++;
	else nBLooseFwdJERUp++;
      }
    }
    // if (vetoJets.at(j).pTJER > 20.){
    //   looseJetPtJER.push_back( vetoJets.at(j).pTJER );
    //   if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nLooseCentralJER++;
    //   else nLooseFwdJER++;
    //   if (vetoJets.at(j).isBtag){
    // 	if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJER++;
    // 	else nBLooseFwdJER++;
    //   }
    //}
  }

  std::sort( looseJetPt.begin()       , looseJetPt.end()       , GreaterThan);
  std::sort( looseJetPtJESUp.begin()  , looseJetPtJESUp.end()  , GreaterThan);
  std::sort( looseJetPtJESDown.begin(), looseJetPtJESDown.end(), GreaterThan);
  std::sort( looseJetPtJER.begin()    , looseJetPtJER.end()    , GreaterThan);

  std::sort( looseJetCentralPt.begin()       , looseJetCentralPt.end()       , GreaterThan);
  std::sort( looseJetCentralPtJESUp.begin()  , looseJetCentralPtJESUp.end()  , GreaterThan);
  std::sort( looseJetCentralPtJESDown.begin(), looseJetCentralPtJESDown.end(), GreaterThan);
  std::sort( looseJetCentralPtJER.begin()    , looseJetCentralPtJER.end()    , GreaterThan);

  
  if (nLooseFwd + nLooseCentral > 1){
    TJet2csv = TMath::Max( vetoJets.at(1).csv   , 0.);
    TJetLoosept = looseJetPt.at(1);
  }
  if (nLooseFwdJESUp + nLooseCentralJESUp > 1){
    TJet2csvJESUp = TMath::Max( vetoJets.at(1).csv   , 0.);
    TJetLooseptJESUp = looseJetPtJESUp.at(1);
  }
  if (nLooseFwdJESDown + nLooseCentralJESDown > 1){
    TJet2csvJESDown = TMath::Max( vetoJets.at(1).csv   , 0.);
    TJetLooseptJESDown = looseJetPtJESDown.at(1);
  }
  if (nLooseFwdJERUp + nLooseCentralJERUp > 1){
    TJet2csvJERUp = TMath::Max( vetoJets.at(1).csv   , 0.);
    TJetLooseptJERUp = looseJetPtJER.at(1);
  }


  if (nLooseCentral > 1)
    TJetLooseCentralpt = looseJetCentralPt.at(1);

  if (nLooseCentralJESUp > 1)
    TJetLooseCentralptJESUp = looseJetCentralPtJESUp.at(1);

  if (nLooseCentralJESDown > 1)
    TJetLooseCentralptJESDown = looseJetCentralPtJESDown.at(1);

  if (nLooseCentralJERUp > 1)
    TJetLooseCentralptJERUp = looseJetCentralPtJER.at(1);


  return;
}


Double_t TWAnalysis::getHTtot(const TString& sys)
{
  if (sys == "Norm")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMET + selJets.at(0).p.Pt();
  else if (sys == "JESUp")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJESUp + selJetsJecUp.at(0).p.Pt();
  else if (sys == "JESDown")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJESDown + selJetsJecDown.at(0).p.Pt();
  else if (sys == "JER")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJERUp     + selJetsJER.at(0).p.Pt();
  else{
    cout << "Unset systematic" << endl;
    return -99.;
  }
}


Double_t TWAnalysis::getHTtot2j(const TString& sys)
{
  if (sys == "Norm")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMET + selJets.at(0).p.Pt() + selJets.at(1).p.Pt();
  else if (sys == "JESUp")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJESUp + selJetsJecUp.at(0).p.Pt() + selJetsJecUp.at(1).p.Pt();
  else if (sys == "JESDown")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJESDown + selJetsJecDown.at(0).p.Pt() + selJetsJecDown.at(1).p.Pt();
  else if (sys == "JER")
    return selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt() + TMETJERUp + selJetsJER.at(0).p.Pt() + selJetsJER.at(1).p.Pt();
  else{
    cout << "Unset systematic" << endl;
    return -99.;
  }
}

// 1j1b
Double_t TWAnalysis::getDilepMETJetPt(int sys)
{
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons.at(0).p;                               // lep1
  vSystem[1] = selLeptons.at(1).p;                               // lep2
  if (sys == 0){
    vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
    vSystem[3] = selJets.at(0).p;                                    // jet 1
  }
  else if (sys > 0){
    vSystem[2].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
    vSystem[3] = selJetsJecUp.at(0).p;
  }
  else if (sys == -1){
    vSystem[2].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
    vSystem[3] = selJetsJecDown.at(0).p;
  }
  else if (sys == -2){
    vSystem[2].SetPtEtaPhiE(TMETJERUp,0,TMET_Phi,TMETJERUp); // met
    vSystem[3] = selJetsJER.at(0).p;
  }
  else{
    cout << "Wrong systematic in TWAnalysis::getDilepMETJetPt" << endl;
    return -9999.;
  }


  return getPtSys(vSystem,4);

}


Double_t TWAnalysis::getDilepMETPt(int sys)
{
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons.at(0).p;                               // lep1
  vSystem[1] = selLeptons.at(1).p;                               // lep2
  if (sys == 0){
    vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
  }
  else if (sys > 0){
    vSystem[2].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
  }
  else if (sys == -1){
    vSystem[2].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
  }
  else if (sys == -2){
    vSystem[2].SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp); // met
  }
  else{
    cout << "Wrong systematic in TWAnalysis::getDilepMETJetPt" << endl;
    return -9999.;
  }


  return getPtSys(vSystem,3);

}

Double_t TWAnalysis::getDilepJetPt(const TString& sys)
{
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  if (sys == "Norm")
    vSystem[2] = selJets[0].p;                                    // jet 1
  else if (sys == "JESUp")
    vSystem[2] = selJetsJecUp[0].p;                                    // jet 1
  else if (sys == "JESDown")
    vSystem[2] = selJetsJecDown[0].p;                                    // jet 1
  else if (sys == "JER")
    vSystem[2] = selJetsJER[0].p;                                    // jet 1
  else{
    cout << "Wrong systematic in TWAnalysis::getDilepJetPt" << endl;
    return -9999.;
  }


  return getPtSys(vSystem,3);

}

// 1j1b
Double_t TWAnalysis::getLep1METJetPt(const TString& sys)
{
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons[0].p;
  if (sys == "Norm"){
    vSystem[1].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    vSystem[2] = selJets[0].p;
  }
  else if (sys == "JESUp"){
    vSystem[1].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
    vSystem[2] = selJetsJecUp[0].p;
  }
  else if (sys == "JESDown"){
    vSystem[1].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
    vSystem[2] = selJetsJecDown[0].p;
  }  
  else if (sys == "JER"){
    vSystem[1].SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp); // met
    vSystem[2] = selJetsJER[0].p;
  }  
  else{
    cout << "Wrong systematic in TWAnalysis::getLep1METJetPt" << endl;
    return -9999.;
  }
  
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

Double_t TWAnalysis::getDilepMETJet1Pz(const TString& sys)
{
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons[0].p;                               // lep1
  vSystem[1] = selLeptons[1].p;                               // lep2
  if (sys == "Norm"){
    vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
    vSystem[3] = selJets[0].p;                                    // jet 1
  }
  else if (sys == "JESUp"){
    vSystem[2].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
    vSystem[3] = selJetsJecUp[0].p;                                    // jet 1
  }
  else if (sys == "JESDown"){
    vSystem[2].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
    vSystem[3] = selJetsJecDown[0].p;                                    // jet 1
  }
  else if (sys == "JER"){
    vSystem[2].SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp); // met
    vSystem[3] = selJetsJER[0].p;                                    // jet 1
  }
  else{
    cout << "Wrong systematic in TWAnalysis::getDilepMETJet1Pz" << endl;
    return -9999.;
  }

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
Double_t TWAnalysis::getDPtDilep_JetMET(const TString& sys)
{
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;
  TLorentzVector met;
  col1.push_back(selLeptons.at(0).p);
  col1.push_back(selLeptons.at(1).p);
  if (sys == "Norm"){
    col2.push_back(selJets.at(0).p);
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  }
  else if (sys == "JESUp"){
    col2.push_back(selJetsJecUp.at(0).p);
    met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
  }
  else if (sys == "JESDown"){
    col2.push_back(selJetsJecDown.at(0).p);
    met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
  }
  else if (sys == "JER"){
    col2.push_back(selJetsJER.at(0).p);
    met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
  }
  else{
    cout << "Wrong systematic in getDPtDilep_JetMET" << endl;
    return -9999.;
  }

  col2.push_back(met);
  return getDeltaPt(col1,col2);

}




Double_t TWAnalysis::getDPtL1_L2(const TString& sys)
{
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons.at(0).p);
  col2.push_back(selLeptons.at(1).p);

  return getDeltaPt(col1,col2);

}

// 1j1b
Double_t TWAnalysis::getDPtDilep_MET(const TString& sys)
{
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons[0].p);
  col1.push_back(selLeptons[1].p);

  TLorentzVector met;
  if (sys == "Norm")
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  else if (sys == "JESUp")
    met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
  else if (sys == "JESDown")
    met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
  else if (sys == "JER")
    met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
  else{
    cout << "Wrong systematic in TWAnalysis::getDPtDilep_MET" << endl;
    return -9999.;
  }
  
  col2.push_back(met);
  return getDeltaPt(col1,col2);

}

// 1j1b
Double_t TWAnalysis::getDPtLep1_MET(const TString& sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons[0].p);
  TLorentzVector met;
  if (sys == "Norm")
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
  else if (sys == "JESUp")
    met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
  else if (sys == "JESDown")
    met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
  else if (sys == "JER")
    met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
  else{
    cout << "Wrong systematic in TWAnalysis::getDPtDilep_MET" << endl;
    return -9999.;
  }
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

Double_t TWAnalysis::getSysM(const TString& sys)
{
  vector<TLorentzVector> col;
  TLorentzVector met;
  if (sys == "Norm"){
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    col.push_back(selJets.at(0).p);
  }
  else if (sys == "JESUp"){
    met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
    col.push_back(selJetsJecUp.at(0).p);
  }
  else if (sys == "JESDown"){
    met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
    col.push_back(selJetsJecDown.at(0).p);
  }
  else if (sys == "JER"){
    met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
    col.push_back(selJetsJER.at(0).p);
  }
  else{
    return -9999.;
  }

  col.push_back(met);
  col.push_back(selLeptons.at(0).p);
  col.push_back(selLeptons.at(1).p);
  return getM(col);

}


Double_t TWAnalysis::getM(vector<TLorentzVector> col)
{
  if (col.size() == 0){
    return -99.;
  }
  TLorentzVector v;
  v  = col[0];
  for (unsigned int i = 1; i < col.size(); ++i){
    v += col[i];
  }
  return v.M();  
}

Double_t TWAnalysis::getDilepPt()
{
  TLorentzVector vSystem[2];
  vSystem[0] = selLeptons[0].p;
  vSystem[1] = selLeptons[1].p;
  return getPtSys(vSystem,2);
}


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


Double_t TWAnalysis::getDeltaRDilep_METJets12(int sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons[0].p);
  col1.push_back(selLeptons[1].p);
  if (sys == 0){
    col2.push_back(selJets[0].p);
    col2.push_back(selJets[1].p);
    TLorentzVector met;
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    col2.push_back(met);
  }
  else if (sys > 0){
    col2.push_back(selJetsJecUp.at(0).p);
    col2.push_back(selJetsJecUp.at(1).p);
    TLorentzVector met;
    met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
    col2.push_back(met);
  }
  else if (sys == -1){
    col2.push_back(selJetsJecDown.at(0).p);
    col2.push_back(selJetsJecDown.at(1).p);
    TLorentzVector met;
    met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
    col2.push_back(met);
  }
  else if (sys == -2){
    col2.push_back(selJetsJER.at(0).p);
    col2.push_back(selJetsJER.at(1).p);
    TLorentzVector met;
    met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
    col2.push_back(met);
  }


  return getDeltaR(col1,col2);
}
Double_t TWAnalysis::getDeltaRDilep_Jets12(int sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
  col1.push_back(selLeptons.at(1).p);
  if (sys == 0){
    col2.push_back(selJets.at(0).p);
    col2.push_back(selJets.at(1).p);
  }
  else if (sys > 0){
    col2.push_back(selJetsJecUp.at(0).p);
    col2.push_back(selJetsJecUp.at(1).p);
  }
  else if (sys == -1){
    col2.push_back(selJetsJecDown.at(0).p);
    col2.push_back(selJetsJecDown.at(1).p);
  }
  else if (sys == -2){
    col2.push_back(selJetsJER.at(0).p);
    col2.push_back(selJetsJER.at(1).p);
  }
  else{
    cout << "Sys not set" << endl; return -999;
  }
  return getDeltaR(col1,col2);
}

Double_t TWAnalysis::getDeltaRLep1_Jet1(int sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
  if (sys == 0)
    col2.push_back(selJets.at(0).p);
  else if (sys > 0)
    col2.push_back(selJetsJecUp.at(0).p);
  else if (sys == -1)
    col2.push_back(selJetsJecDown.at(0).p);
  else if (sys == -2)
    col2.push_back(selJetsJER.at(0).p);
  else{
    cout << "Sys not set" << endl; return -999;
  }

  return getDeltaR(col1,col2);
}



Double_t TWAnalysis::getDeltaR(vector<TLorentzVector> col1, vector<TLorentzVector> col2){
  TLorentzVector v1, v2;
  if (col1.size()*col2.size() == 0){
    cout << "[TAT::getDeltaR] One of the collections is empty" << endl;
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
  return v1.DeltaR(v2);
}


void TWAnalysis::setTWBDT()
{
  // nominal
  BDTada = new TMVA::Reader();
  BDTada->AddVariable( "nLooseCentral"                 , &nLooseCentral      );
  BDTada->AddVariable( "nLooseFwd"                     , &nLooseFwd          ); 
  BDTada->AddVariable( "nBTotal"                       , &nBTotal            );  
  BDTada->AddVariable( "DilepMETJetPt"                 , &DilepMETJetPt      );
  BDTada->AddVariable( "THTtot"                        , &THTtot             );    
  BDTada->AddVariable( "TJet1_pt"                      , &TJet1_pt           );
  BDTada->AddVariable( "TJetLoosept"                   , &TJetLoosept        );
  BDTada->AddVariable( "DilepMETJetPt/THTtot"          , &DilepmetjetOverHT  );
  BDTada->AddVariable( "MSys"                          , &MSys               );
  BDTada->AddVariable( "C_jll"                         , &C_jll              ); 
  BDTada->AddVariable( "HTLepOverHT"                   , &HTLepOverHT        );
  BDTada->AddVariable( "DilepJetPt"                    , &DilepJetPt         );       

  BDTada->BookMVA("BDTada","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_AdaBoost_1000_0.1_3.weights.xml");
  
  BDTada_JESUp = new TMVA::Reader();
  BDTada_JESUp->AddVariable( "nLooseCentral"            , &nLooseCentralJESUp      );
  BDTada_JESUp->AddVariable( "nLooseFwd"                , &nLooseFwdJESUp          ); 
  BDTada_JESUp->AddVariable( "nBTotal"                  , &nBTotalJESUp            );  
  BDTada_JESUp->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESUp      );
  BDTada_JESUp->AddVariable( "THTtot"                   , &THTtotJESUp             );    
  BDTada_JESUp->AddVariable( "TJet1_pt"                 , &TJet1_ptJESUp           );
  BDTada_JESUp->AddVariable( "TJetLoosept"              , &TJetLooseptJESUp        );
  BDTada_JESUp->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESUp  );
  BDTada_JESUp->AddVariable( "MSys"                     , &MSysJESUp               );
  BDTada_JESUp->AddVariable( "C_jll"                    , &C_jllJESUp              ); 
  BDTada_JESUp->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESUp        );
  BDTada_JESUp->AddVariable( "DilepJetPt"               , &DilepJetPtJESUp         );       
  BDTada_JESUp->BookMVA("BDTada_JESUp","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_AdaBoost_1000_0.1_3.weights.xml");

  BDTada_JESDown = new TMVA::Reader();
  BDTada_JESDown->AddVariable( "nLooseCentral"            , &nLooseCentralJESDown      );
  BDTada_JESDown->AddVariable( "nLooseFwd"                , &nLooseFwdJESDown          ); 
  BDTada_JESDown->AddVariable( "nBTotal"                  , &nBTotalJESDown            );  
  BDTada_JESDown->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESDown      );
  BDTada_JESDown->AddVariable( "THTtot"                   , &THTtotJESDown             );    
  BDTada_JESDown->AddVariable( "TJet1_pt"                 , &TJet1_ptJESDown           );
  BDTada_JESDown->AddVariable( "TJetLoosept"              , &TJetLooseptJESDown        );
  BDTada_JESDown->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESDown  );
  BDTada_JESDown->AddVariable( "MSys"                     , &MSysJESDown               );
  BDTada_JESDown->AddVariable( "C_jll"                    , &C_jllJESDown              ); 
  BDTada_JESDown->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESDown        );
  BDTada_JESDown->AddVariable( "DilepJetPt"               , &DilepJetPtJESDown         );       
  BDTada_JESDown->BookMVA("BDTada_JESDown","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_AdaBoost_1000_0.1_3.weights.xml");

  BDTada_JER = new TMVA::Reader();
  BDTada_JER->AddVariable( "nLooseCentral"            , &nLooseCentralJERUp      );
  BDTada_JER->AddVariable( "nLooseFwd"                , &nLooseFwdJERUp          ); 
  BDTada_JER->AddVariable( "nBTotal"                  , &nBTotalJERUp            );  
  BDTada_JER->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJERUp      );
  BDTada_JER->AddVariable( "THTtot"                   , &THTtotJERUp             );    
  BDTada_JER->AddVariable( "TJet1_pt"                 , &TJet1_ptJERUp           );
  BDTada_JER->AddVariable( "TJetLoosept"              , &TJetLooseptJERUp        );
  BDTada_JER->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJERUp  );
  BDTada_JER->AddVariable( "MSys"                     , &MSysJERUp               );
  BDTada_JER->AddVariable( "C_jll"                    , &C_jllJERUp              ); 
  BDTada_JER->AddVariable( "HTLepOverHT"              , &HTLepOverHTJERUp        );
  BDTada_JER->AddVariable( "DilepJetPt"               , &DilepJetPtJERUp         );       
  BDTada_JER->BookMVA("BDTada_JER","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_AdaBoost_1000_0.1_3.weights.xml");


  // nominal
  BDTgrad = new TMVA::Reader();
  BDTgrad->AddVariable( "nLooseCentral"            , &nLooseCentral      );
  BDTgrad->AddVariable( "nLooseFwd"                , &nLooseFwd          ); 
  BDTgrad->AddVariable( "nBTotal"                  , &nBTotal            );  
  BDTgrad->AddVariable( "DilepMETJetPt"            , &DilepMETJetPt      );
  BDTgrad->AddVariable( "THTtot"                   , &THTtot             );    
  BDTgrad->AddVariable( "TJet1_pt"                 , &TJet1_pt           );
  BDTgrad->AddVariable( "TJetLoosept"              , &TJetLoosept        );
  BDTgrad->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHT  );
  BDTgrad->AddVariable( "MSys"                     , &MSys               );
  BDTgrad->AddVariable( "C_jll"                    , &C_jll              ); 
  BDTgrad->AddVariable( "HTLepOverHT"              , &HTLepOverHT        );
  BDTgrad->AddVariable( "DilepJetPt"               , &DilepJetPt         );       

  BDTgrad->BookMVA("BDTgrad","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");
  
  BDTgrad_JESUp = new TMVA::Reader();
  BDTgrad_JESUp->AddVariable( "nLooseCentral"            , &nLooseCentralJESUp      );
  BDTgrad_JESUp->AddVariable( "nLooseFwd"                , &nLooseFwdJESUp          ); 
  BDTgrad_JESUp->AddVariable( "nBTotal"                  , &nBTotalJESUp            );  
  BDTgrad_JESUp->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESUp      );
  BDTgrad_JESUp->AddVariable( "THTtot"                   , &THTtotJESUp             );    
  BDTgrad_JESUp->AddVariable( "TJet1_pt"                 , &TJet1_ptJESUp           );
  BDTgrad_JESUp->AddVariable( "TJetLoosept"              , &TJetLooseptJESUp        );
  BDTgrad_JESUp->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESUp  );
  BDTgrad_JESUp->AddVariable( "MSys"                     , &MSysJESUp               );
  BDTgrad_JESUp->AddVariable( "C_jll"                    , &C_jllJESUp              ); 
  BDTgrad_JESUp->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESUp        );
  BDTgrad_JESUp->AddVariable( "DilepJetPt"               , &DilepJetPtJESUp         );       
  BDTgrad_JESUp->BookMVA("BDTgrad_JESUp","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");

  BDTgrad_JESDown = new TMVA::Reader();
  BDTgrad_JESDown->AddVariable( "nLooseCentral"            , &nLooseCentralJESDown      );
  BDTgrad_JESDown->AddVariable( "nLooseFwd"                , &nLooseFwdJESDown          ); 
  BDTgrad_JESDown->AddVariable( "nBTotal"                  , &nBTotalJESDown            );  
  BDTgrad_JESDown->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESDown      );
  BDTgrad_JESDown->AddVariable( "THTtot"                   , &THTtotJESDown             );    
  BDTgrad_JESDown->AddVariable( "TJet1_pt"                 , &TJet1_ptJESDown           );
  BDTgrad_JESDown->AddVariable( "TJetLoosept"              , &TJetLooseptJESDown        );
  BDTgrad_JESDown->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESDown  );
  BDTgrad_JESDown->AddVariable( "MSys"                     , &MSysJESDown               );
  BDTgrad_JESDown->AddVariable( "C_jll"                    , &C_jllJESDown              ); 
  BDTgrad_JESDown->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESDown        );
  BDTgrad_JESDown->AddVariable( "DilepJetPt"               , &DilepJetPtJESDown         );       
  BDTgrad_JESDown->BookMVA("BDTgrad_JESDown","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");

  BDTgrad_JER = new TMVA::Reader();
  BDTgrad_JER->AddVariable( "nLooseCentral"            , &nLooseCentralJERUp      );
  BDTgrad_JER->AddVariable( "nLooseFwd"                , &nLooseFwdJERUp          ); 
  BDTgrad_JER->AddVariable( "nBTotal"                  , &nBTotalJERUp            );  
  BDTgrad_JER->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJERUp      );
  BDTgrad_JER->AddVariable( "THTtot"                   , &THTtotJERUp             );    
  BDTgrad_JER->AddVariable( "TJet1_pt"                 , &TJet1_ptJERUp           );
  BDTgrad_JER->AddVariable( "TJetLoosept"              , &TJetLooseptJERUp        );
  BDTgrad_JER->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJERUp  );
  BDTgrad_JER->AddVariable( "MSys"                     , &MSysJERUp               );
  BDTgrad_JER->AddVariable( "C_jll"                    , &C_jllJERUp              ); 
  BDTgrad_JER->AddVariable( "HTLepOverHT"              , &HTLepOverHTJERUp        );
  BDTgrad_JER->AddVariable( "DilepJetPt"               , &DilepJetPtJERUp         );       
  BDTgrad_JER->BookMVA("BDTgrad_JER","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun5/bdtForTWv2/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");


  ////////////////////////////////////////////////
  // new BDT
  ////////////////////////////////////////////////
  // nominal
  BDT = new TMVA::Reader();
  BDT->AddVariable( "nLooseCentral"            , &nLooseCentral      );
  BDT->AddVariable( "nBTotal"                  , &nBTotal            );  
  BDT->AddVariable( "DilepMETJetPt"            , &DilepMETJetPt      );
  BDT->AddVariable( "THTtot"                   , &THTtot             );    
  BDT->AddVariable( "TJet1_pt"                 , &TJet1_pt           );
  BDT->AddVariable( "TJetLooseCentralpt"       , &TJetLooseCentralpt );
  BDT->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHT  );
  BDT->AddVariable( "MSys"                     , &MSys               );
  BDT->AddVariable( "C_jll"                    , &C_jll              ); 
  BDT->AddVariable( "HTLepOverHT"              , &HTLepOverHT        );
  BDT->AddVariable( "DilepJetPt"               , &DilepJetPt         );       

  BDT->BookMVA("BDT","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jun16/bdtForTWv4/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");
  
  BDT_JESUp = new TMVA::Reader();
  BDT_JESUp->AddVariable( "nLooseCentral"            , &nLooseCentralJESUp      );
  BDT_JESUp->AddVariable( "nBTotal"                  , &nBTotalJESUp            );  
  BDT_JESUp->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESUp      );
  BDT_JESUp->AddVariable( "THTtot"                   , &THTtotJESUp             );    
  BDT_JESUp->AddVariable( "TJet1_pt"                 , &TJet1_ptJESUp           );
  BDT_JESUp->AddVariable( "TJetLooseCentralpt"       , &TJetLooseCentralptJESUp );
  BDT_JESUp->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESUp  );
  BDT_JESUp->AddVariable( "MSys"                     , &MSysJESUp               );
  BDT_JESUp->AddVariable( "C_jll"                    , &C_jllJESUp              ); 
  BDT_JESUp->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESUp        );
  BDT_JESUp->AddVariable( "DilepJetPt"               , &DilepJetPtJESUp         );       
  BDT_JESUp->BookMVA("BDT_JESUp","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jun16/bdtForTWv4/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");

  BDT_JESDown = new TMVA::Reader();
  BDT_JESDown->AddVariable( "nLooseCentral"            , &nLooseCentralJESDown      );
  BDT_JESDown->AddVariable( "nBTotal"                  , &nBTotalJESDown            );  
  BDT_JESDown->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJESDown      );
  BDT_JESDown->AddVariable( "THTtot"                   , &THTtotJESDown             );    
  BDT_JESDown->AddVariable( "TJet1_pt"                 , &TJet1_ptJESDown           );
  BDT_JESDown->AddVariable( "TJetLooseCentralpt"       , &TJetLooseCentralptJESDown );
  BDT_JESDown->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJESDown  );
  BDT_JESDown->AddVariable( "MSys"                     , &MSysJESDown               );
  BDT_JESDown->AddVariable( "C_jll"                    , &C_jllJESDown              ); 
  BDT_JESDown->AddVariable( "HTLepOverHT"              , &HTLepOverHTJESDown        );
  BDT_JESDown->AddVariable( "DilepJetPt"               , &DilepJetPtJESDown         );       
  BDT_JESDown->BookMVA("BDT_JESDown","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jun16/bdtForTWv4/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");

  BDT_JER = new TMVA::Reader();
  BDT_JER->AddVariable( "nLooseCentral"            , &nLooseCentralJERUp      );
  BDT_JER->AddVariable( "nBTotal"                  , &nBTotalJERUp            );  
  BDT_JER->AddVariable( "DilepMETJetPt"            , &DilepMETJetPtJERUp      );
  BDT_JER->AddVariable( "THTtot"                   , &THTtotJERUp             );    
  BDT_JER->AddVariable( "TJet1_pt"                 , &TJet1_ptJERUp           );
  BDT_JER->AddVariable( "TJetLooseCentralpt"       , &TJetLooseCentralptJERUp );
  BDT_JER->AddVariable( "DilepMETJetPt/THTtot"     , &DilepmetjetOverHTJERUp  );
  BDT_JER->AddVariable( "MSys"                     , &MSysJERUp               );
  BDT_JER->AddVariable( "C_jll"                    , &C_jllJERUp              ); 
  BDT_JER->AddVariable( "HTLepOverHT"              , &HTLepOverHTJERUp        );
  BDT_JER->AddVariable( "DilepJetPt"               , &DilepJetPtJERUp         );       
  BDT_JER->BookMVA("BDT_JER","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jun16/bdtForTWv4/weights/TMVAClassification_GradBoost_2000_0.01.weights.xml");


  BDT2j1t = new TMVA::Reader();
  BDT2j1t->AddVariable("jetPtSubLeading_"    , &jetPtSubLeading_  );
  BDT2j1t->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1         );
  BDT2j1t->AddVariable("deltaRL1L2_J1J2_"    , &TDR_L1L2_J1J2     );
  BDT2j1t->AddVariable("deltaRL1L2_metJ1J2_" , &TDR_L1L2_J1J2MET  );
  BDT2j1t->BookMVA("2j1b","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jul8/bdtForTWv5/weights/TMVAClassification_GradBoost_200_005_4.weights.xml");



  BDT2j1tJESUp = new TMVA::Reader();
  BDT2j1tJESUp->AddVariable("jetPtSubLeading_"       , &jetPtSubLeading_JESUp  );
  BDT2j1tJESUp->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1JESUp         );
  BDT2j1tJESUp->AddVariable("deltaRL1L2_J1J2_"       , &TDR_L1L2_J1J2JESUp     );
  BDT2j1tJESUp->AddVariable("deltaRL1L2_metJ1J2_"    , &TDR_L1L2_J1J2METJESUp  );
  BDT2j1tJESUp->BookMVA("2j1b","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jul8/bdtForTWv5/weights/TMVAClassification_GradBoost_200_005_4.weights.xml");

  
  BDT2j1tJESDown = new TMVA::Reader();
  BDT2j1tJESDown->AddVariable("jetPtSubLeading_"       , &jetPtSubLeading_JESDown  );
  BDT2j1tJESDown->AddVariable("deltaRL1_J1_"	       , &TDR_L1_J1JESDown         );
  BDT2j1tJESDown->AddVariable("deltaRL1L2_J1J2_"       , &TDR_L1L2_J1J2JESDown     );
  BDT2j1tJESDown->AddVariable("deltaRL1L2_metJ1J2_"    , &TDR_L1L2_J1J2METJESDown  );
  BDT2j1tJESDown->BookMVA("2j1b","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jul8/bdtForTWv5/weights/TMVAClassification_GradBoost_200_005_4.weights.xml");

  BDT2j1tJER = new TMVA::Reader();
  BDT2j1tJER->AddVariable("jetPtSubLeading_"       , &jetPtSubLeading_JERUp  );
  // BDT2j1tJER->AddVariable("jetEtaSubLeading_"      , &jetEtaSubLeading_JERUp ); 
  // BDT2j1tJER->AddVariable("ptL1L2_"	             , &TDilepPt );
  // BDT2j1tJER->AddVariable("ptsysL1L2met_"	     , &TDilepMETPtJERUp );
  // BDT2j1tJER->AddVariable("deltaPtl1_l2_"	     , &TDPtL1_L2JERUp );
  // BDT2j1tJER->AddVariable("deltaPtl2_J2_"	     , &TDPtJ2_L2JERUp );
  BDT2j1tJER->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1JERUp );
  BDT2j1tJER->AddVariable("deltaRL1L2_J1J2_"       , &TDR_L1L2_J1J2JERUp );
  BDT2j1tJER->AddVariable("deltaRL1L2_metJ1J2_"    , &TDR_L1L2_J1J2METJERUp );
  BDT2j1tJER->BookMVA("2j1b","/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/plotter/TW/StableWeights/jul8/bdtForTWv5/weights/TMVAClassification_GradBoost_200_005_4.weights.xml");
  // BDT2j1tJER->AddVariable("LeadingLeptPt_"         , &LeadingLeptPt_JERUp    );
  // BDT2j1tJER->AddVariable("LeadingLeptEta_"        , &LeadingLeptEta_JERUp   );






  // BDT2j1tv1 = new TMVA::Reader();
  // BDT2j1tv1->AddVariable("LeadingLeptPt_"      , &LeadingLeptPt_    );
  // BDT2j1tv1->AddVariable("jetPtSubLeading_"    , &jetPtSubLeading_  );
  // BDT2j1tv1->AddVariable("ptL1L2_"	     , &TDilepPt );
  // BDT2j1tv1->AddVariable("ptsysL1L2met_"	     , &TDilepMETPt );
  // BDT2j1tv1->AddVariable("Etsys_"		     , &TETSys );
  // BDT2j1tv1->AddVariable("EtsysRejJ2_"	     , &TET_LLJetMET );
  // BDT2j1tv1->AddVariable("Ht_"		     , &THTtot2j );
  // BDT2j1tv1->AddVariable("HtRejJ2_"	     , &THtRejJ2 );
  // BDT2j1tv1->AddVariable("deltaPtl1_l2_"	     , &TDPtL1_L2 );
  // BDT2j1tv1->AddVariable("deltaPtl2_J2_"	     , &TDPtJ2_L2 );
  // BDT2j1tv1->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1 );
  // BDT2j1tv1->AddVariable("deltaRL1L2_J1J2_"    , &TDR_L1L2_J1J2 );
  // BDT2j1tv1->AddVariable("deltaRL1L2_metJ1J2_" , &TDR_L1L2_J1J2MET );
  // BDT2j1tv1->BookMVA("2j1bv1","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun20/v1/TMVAClassification_BDT.weights.xml");


  // BDT2j1tv2 = new TMVA::Reader();
  // BDT2j1tv2->AddVariable("LeadingLeptPt_"      , &LeadingLeptPt_    );
  // BDT2j1tv2->AddVariable("jetPtSubLeading_"    , &jetPtSubLeading_  );
  // BDT2j1tv2->AddVariable("ptL1L2_"	     , &TDilepPt );
  // BDT2j1tv2->AddVariable("ptsysL1L2met_"	     , &TDilepMETPt );
  // BDT2j1tv2->AddVariable("Ht_"		     , &THTtot2j );
  // BDT2j1tv2->AddVariable("HtRejJ2_"	     , &THtRejJ2 );
  // BDT2j1tv2->AddVariable("deltaPtl1_l2_"	     , &TDPtL1_L2 );
  // BDT2j1tv2->AddVariable("deltaPtl2_J2_"	     , &TDPtJ2_L2 );
  // BDT2j1tv2->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1 );
  // BDT2j1tv2->AddVariable("deltaRL1L2_J1J2_"    , &TDR_L1L2_J1J2 );
  // BDT2j1tv2->AddVariable("deltaRL1L2_metJ1J2_" , &TDR_L1L2_J1J2MET );
  // BDT2j1tv2->BookMVA("2j1bv2","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun20/v2/TMVAClassification_BDT.weights.xml");

  // BDT2j1tv3 = new TMVA::Reader();
  // BDT2j1tv3->AddVariable("LeadingLeptPt_"      , &LeadingLeptPt_    );
  // BDT2j1tv3->AddVariable("jetPtSubLeading_"    , &jetPtSubLeading_  );
  // BDT2j1tv3->AddVariable("ptL1L2_"	     , &TDilepPt );
  // BDT2j1tv3->AddVariable("ptsysL1L2met_"	     , &TDilepMETPt );
  // BDT2j1tv3->AddVariable("Etsys_"		     , &TETSys );
  // BDT2j1tv3->AddVariable("EtsysRejJ2_"	     , &TET_LLJetMET );
  // BDT2j1tv3->AddVariable("deltaPtl1_l2_"	     , &TDPtL1_L2 );
  // BDT2j1tv3->AddVariable("deltaPtl2_J2_"	     , &TDPtJ2_L2 );
  // BDT2j1tv3->AddVariable("deltaRL1_J1_"	     , &TDR_L1_J1 );
  // BDT2j1tv3->AddVariable("deltaRL1L2_J1J2_"    , &TDR_L1L2_J1J2 );
  // BDT2j1tv3->AddVariable("deltaRL1L2_metJ1J2_" , &TDR_L1L2_J1J2MET );
  // BDT2j1tv3->BookMVA("2j1bv3","/nfs/fanae/user/sscruz/TW/AnalysisPAF/plotter/TW/StableWeights/jun20/v3/TMVAClassification_BDT.weights.xml");


}



Double_t TWAnalysis::getETSys(int sys)
{

  if (sys == 0)
    return selJets.at(0).p.Et() + selJets.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMET;
  else if (sys > 0)
    return selJetsJecUp.at(0).p.Et() + selJetsJecUp.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESUp;
  else if (sys == -1)
    return selJetsJecDown.at(0).p.Et() + selJetsJecDown.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESDown;
  else if (sys == -2){
    return selJetsJER.at(0).p.Et() + selJetsJER.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJERUp;
  }
  else 
    return -999;
}

Double_t TWAnalysis::getET_LLJetMET(int sys)
{
  if (sys == 0)
    return selJets.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMET;
  else if (sys > 0)
    return selJetsJecUp.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESUp;
  else if (sys == -1)
    return selJetsJecDown.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESDown;
  else if (sys == -2)
    return selJetsJER.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJERUp;
  else return -999;
}
