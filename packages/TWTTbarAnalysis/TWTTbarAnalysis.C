#include "TWTTbarAnalysis.h"
ClassImp(TWTTbarAnalysis);

bool GreaterThan(float i, float j){ return (i > j);} // implementa la funcion 'mayor que'

//#####################################################################
// Core PAF methods
//---------------------------------------------------------------------

TWTTbarAnalysis::TWTTbarAnalysis() : PAFChainItemSelector() {
  fhDummy        = 0;
  passMETfilters = 0;
  passTrigger    = 0;
  isSS           = 0;
  
  for(Int_t i = 0; i < 254; i++) TLHEWeight[i] = 0;
}

void TWTTbarAnalysis::Initialise(){            //=============== Initialise
  gIsData      = GetParam<Bool_t>("IsData");
  gSelection   = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gDoSyst      = GetParam<Bool_t>("doSyst");
  gPar         = GetParam<TString>("_options");
  if (gPar.Contains("Semi")) {
    cout << "> Running the semileptonic ttbar sample" << endl;
  }
  gIsTTbar     = false;
  gIsTW        = false;
  gIsLHE       = false;

  gIsFSRUp = false; gIsFSRDown = false;
  if     (gSampleName.Contains("TTbar_Powheg") && gSampleName.Contains("fsrUp"))   gIsFSRUp = true;
  else if(gSampleName.Contains("TTbar_Powheg") && gSampleName.Contains("fsrDown")) gIsFSRDown = true;

  if (gSampleName.Contains("TTbar") || gSampleName.Contains("TTJets")) gIsTTbar = true;
  if (gSampleName.Contains("TW")    || gSampleName.Contains("TbarW") ) gIsTW    = true;
  if (gSampleName == "TTbar_Powheg")   gIsLHE = true;
  
  // Esto en python seria media linea :D
  if (gSampleName.Contains("_")){
    TObjArray *tx = gSampleName.Tokenize("_");
    if (((TObjString*) tx->Last())->GetString().IsDigit()) gIsLHE = true;
  }
  
  fhDummy = CreateH1F("fhDummy", "fhDummy", 1, 0, 2);
  
  fMini1j1t   = CreateTree("Mini1j1t", "MiniMiniTree");
  SetTWTTbarVariables();
  
  genLeptons      = std::vector<Lepton>();
  selLeptons      = std::vector<Lepton>();
  vetoLeptons     = std::vector<Lepton>();
  selJets         = std::vector<Jet>();
  selJetsJecUp    = std::vector<Jet>();
  selJetsJecDown  = std::vector<Jet>();
  selJetsJER      = std::vector<Jet>();
  Jets15          = std::vector<Jet>();
  genJets         = std::vector<Jet>();
  mcJets          = std::vector<Jet>();
  vetoJets        = std::vector<Jet>();
  
  SergioJets      = std::vector<Jet>();
  SergioLooseCentralJets = std::vector<Jet>();
  SergioLooseFwdJets     = std::vector<Jet>();
  SergioLeps      = std::vector<Lepton>();
}

void TWTTbarAnalysis::InsideLoop() {            //=============== InsideLoop
  ReSetTWTTbarVariables();
  // Vectors with the objects
  genLeptons        = GetParam<vector<Lepton>>("genLeptons");
  selLeptons        = GetParam<vector<Lepton>>("selLeptons");
  vetoLeptons       = GetParam<vector<Lepton>>("vetoLeptons");
  selJets           = GetParam<vector<Jet>>("selJets");
  selJetsJecUp      = GetParam<vector<Jet>>("selJetsJecUp");
  selJetsJecDown    = GetParam<vector<Jet>>("selJetsJecDown");
  selJetsJER        = GetParam<vector<Jet>>("selJetsJER");
  Jets15            = GetParam<vector<Jet>>("Jets15");
  vetoJets          = GetParam<vector<Jet>>("vetoJets");
  genJets           = GetParam<vector<Jet>>("genJets");
  mcJets            = GetParam<vector<Jet>>("mcJets");
  // Weights and SFs
  NormWeight        = GetParam<Float_t>("NormWeight");
  TrigSF            = GetParam<Float_t>("TriggerSF");
  TrigSFerr         = GetParam<Float_t>("TriggerSFerr");
  PUSF              = GetParam<Float_t>("PUSF");
  PUSF_Up           = GetParam<Float_t>("PUSF_Up");
  PUSF_Down         = GetParam<Float_t>("PUSF_Down");
  BtagSF            = GetParam<Float_t>("BtagSF"          );
  BtagSFBtagUp      = GetParam<Float_t>("BtagSFBtagUp"    );
  BtagSFBtagDown    = GetParam<Float_t>("BtagSFBtagDown"  );
  BtagSFMistagUp    = GetParam<Float_t>("BtagSFMistagUp"  );
  BtagSFMistagDown  = GetParam<Float_t>("BtagSFMistagDown");

  // Event variables
  gChannel          = GetParam<Int_t>("gChannel");
  passMETfilters    = GetParam<Bool_t>("METfilters");
  passTrigger       = GetParam<Bool_t>("passTrigger");
  isSS              = GetParam<Bool_t>("isSS");
  
  // Leptons and Jets
  GetLeptonVariables(selLeptons, vetoLeptons);
  GetJetVariables(selJets, Jets15);
  if (gPar.Contains("Unfolding")) {
    GetGenLepVariables();
    GetGenMET();
  }
  GetGenJetVariables(genJets, mcJets);
  GetMET();
  fhDummy->Fill(1);

  if (gPar.Contains("Semi")) {
    if (gIsTTbar && genLeptons.size() > 1 ) return;
  } else {
    if (gIsTTbar && genLeptons.size() < 2 ) return; // Dilepton selection for ttbar!!!
  }
  
  TWeight_normal = NormWeight;
  
  if ((gPar.Contains("Unfolding")) && (nSergioLeps >= 2) && (nSergioJets == 2) && (nSergiobJets == 2)) { // Checking if we pass the selection with gen things
    if(SergioLeps.at(0).isElec && SergioLeps.at(1).isMuon) GenChannel = iElMu; // ...but first, let's redefine the GenChannel to get it right
    if(SergioLeps.at(0).isMuon && SergioLeps.at(1).isElec) GenChannel = iElMu;
    if(SergioLeps.at(0).isMuon && SergioLeps.at(1).isMuon) GenChannel = iMuon;
    if(SergioLeps.at(0).isElec && SergioLeps.at(1).isElec) GenChannel = iElec;
    TGenIsSS            = (SergioLeps.at(0).charge * SergioLeps.at(1).charge) > 0;
    
    TGenM_LeadingB     = (SergioJets.at(0).p + SergioLeps.at(0).p).M();
    TGenM_SubLeadingB   = (SergioJets.at(0).p + SergioLeps.at(1).p).M();
    //nuevas variables para la variable de ATLAS
    TGenM_LeadingBj2    = (SergioJets.at(1).p + SergioLeps.at(0).p).M();
    TGenM_SubLeadingBj2 = (SergioJets.at(1).p + SergioLeps.at(1).p).M();
    if(GreaterThan(TGenM_LeadingB,TGenM_SubLeadingBj2)) {
        if(GreaterThan(TGenM_SubLeadingB,TGenM_LeadingBj2)) {
            if(!GreaterThan(TGenM_LeadingB,TGenM_SubLeadingB)) {TGenM_bjetlepton_minmax = TGenM_LeadingB;}
            else                                               {TGenM_bjetlepton_minmax = TGenM_SubLeadingB;}
        }
        else {
            if(!GreaterThan(TGenM_LeadingB,TGenM_LeadingBj2))  {TGenM_bjetlepton_minmax = TGenM_LeadingB; }
            else                                               {TGenM_bjetlepton_minmax = TGenM_LeadingBj2;}
        }
    }
    else {
        if(GreaterThan(TGenM_SubLeadingB,TGenM_LeadingBj2)) {
            if(!GreaterThan(TGenM_SubLeadingBj2,TGenM_SubLeadingB)) {TGenM_bjetlepton_minmax = TGenM_SubLeadingBj2;}
            else                                                    {TGenM_bjetlepton_minmax = TGenM_SubLeadingB;}
        }
        else {
            if(!GreaterThan(TGenM_SubLeadingBj2,TGenM_LeadingBj2))  {TGenM_bjetlepton_minmax = TGenM_SubLeadingBj2;}
            else                                                    {TGenM_bjetlepton_minmax = TGenM_LeadingBj2;}
        }  
    }
    TGenE_LLB           = (SergioJets.at(0).p + SergioLeps.at(0).p + SergioLeps.at(1).p).E();
    TGenMT_LLMETB       = (SergioJets.at(0).p + SergioLeps.at(0).p + SergioLeps.at(1).p + SergioMET).Mt();
    TGenM_LLB           = (SergioJets.at(0).p + SergioLeps.at(0).p + SergioLeps.at(1).p).M();
    TGenDilepPt         = (SergioLeps.at(0).p + SergioLeps.at(1).p).Pt();
    TGenDilepJetPt      = (SergioLeps.at(0).p + SergioLeps.at(1).p + SergioJets.at(0).p).Pt();
    TGenDilepMETJetPt   = (SergioLeps.at(0).p + SergioLeps.at(1).p + SergioJets.at(0).p + SergioMET).Pt();
    TGenHTtot           = (SergioLeps.at(0).Pt() + SergioLeps.at(1).Pt() + SergioJets.at(0).Pt() + SergioMET.Pt());
    TGenDilepMETJet1Pz  = (SergioLeps.at(0).p + SergioLeps.at(1).p + SergioJets.at(0).p + SergioMET).Pz();
    TGenLLMETBEta       = (SergioLeps.at(0).p + SergioLeps.at(1).p + SergioJets.at(0).p + SergioMET).Eta();
    TGenMSys            = (SergioLeps.at(0).p + SergioLeps.at(1).p + SergioJets.at(0).p + SergioMET).M();
    TGenMll             = (SergioLeps.at(0).p + SergioLeps.at(1).p).M();
    TGenDPhiLL          = SergioLeps.at(0).p.DeltaPhi(SergioLeps.at(1).p);
    TGenDPhiLeadJet     = SergioLeps.at(0).p.DeltaPhi(SergioJets.at(0).p);
    TGenDPhiSubLeadJet  = SergioLeps.at(1).p.DeltaPhi(SergioJets.at(0).p);
    
    if (((SergioLeps.at(0).p + SergioLeps.at(1).p).M() > 20) && (SergioLeps.at(0).p.Pt() > 25) && 
        (GenChannel == iElMu || GenChannel == iMuon || GenChannel == iElec) && !TGenIsSS && (nSergioLooseCentralJets == 2)) {
      if (GenChannel == iMuon || GenChannel == iElec) {
        if ((TGenMET > 20) && (abs(TGenMll-90.19)>15)) {Tpassgen = 1;}   
      }
      else {Tpassgen = 1;}
    }
  }
  // Checking if we pass the selection with reco things
  if ((TNSelLeps >= 2) && passTrigger && passMETfilters && ((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20) && 
      (selLeptons.at(0).p.Pt() > 25) && (TChannel == iElMu || TChannel == iElec || TChannel == iMuon) && (!isSS)) {
    Float_t lepSF     = selLeptons.at(0).GetSF( 0) * selLeptons.at(1).GetSF( 0);
    Float_t ElecSF    = 1; Float_t MuonSF   = 1;
    Float_t ElecSFUp  = 1; Float_t ElecSFDo = 1; Float_t MuonSFUp = 1; Float_t MuonSFDo = 1;
    if (TChannel == iElec) {
      ElecSF   = selLeptons.at(0).GetSF( 0)*selLeptons.at(1).GetSF( 0);
      ElecSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      ElecSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      MuonSFUp = 1; MuonSFDo = 1; MuonSF = 1;
    }
    else if (TChannel == iMuon) {
      MuonSFUp = selLeptons.at(0).GetSF( 1)*selLeptons.at(1).GetSF( 1);
      MuonSFDo = selLeptons.at(0).GetSF(-1)*selLeptons.at(1).GetSF(-1);
      ElecSFUp = 1; ElecSFDo = 1; ElecSF = 1;
    }
    else {
      if(selLeptons.at(0).isMuon) {
        MuonSF   *= selLeptons.at(0).GetSF( 0);
        MuonSFUp *= selLeptons.at(0).GetSF( 1);
        MuonSFDo *= selLeptons.at(0).GetSF(-1);
      }
      else {
        ElecSF   *= selLeptons.at(0).GetSF( 0);
        ElecSFUp *= selLeptons.at(0).GetSF( 1);
        ElecSFDo *= selLeptons.at(0).GetSF(-1);
      }
      if (selLeptons.at(1).isMuon) {
        MuonSF   *= selLeptons.at(1).GetSF( 0);
        MuonSFUp *= selLeptons.at(1).GetSF( 1);
        MuonSFDo *= selLeptons.at(1).GetSF(-1);
      }
      else {
        ElecSF   *= selLeptons.at(1).GetSF( 0);
        ElecSFUp *= selLeptons.at(1).GetSF( 1);
        ElecSFDo *= selLeptons.at(1).GetSF(-1);
      }
    }
    
    TWeight               = NormWeight * ElecSF   * MuonSF * TrigSF * PUSF * BtagSF;
    TWeight_ElecEffUp     = NormWeight * ElecSFUp * MuonSF * TrigSF * PUSF * BtagSF;
    TWeight_ElecEffDown   = NormWeight * ElecSFDo * MuonSF * TrigSF * PUSF * BtagSF;
    TWeight_MuonEffUp     = NormWeight * ElecSF   * (MuonSF+TMath::Sqrt(TMath::Power(MuonSFUp-MuonSF,2)+TMath::Power(MuonSF*0.0122,2)))*TrigSF*PUSF*BtagSF;
    TWeight_MuonEffDown   = NormWeight * ElecSF   * (MuonSF-TMath::Sqrt(TMath::Power(MuonSFDo-MuonSF,2)+TMath::Power(MuonSF*0.0122,2)))*TrigSF*PUSF*BtagSF;
    TWeight_TrigUp        = NormWeight * lepSF    * (TrigSF+TrigSFerr) * PUSF   * BtagSF;
    TWeight_TrigDown      = NormWeight * lepSF    * (TrigSF-TrigSFerr) * PUSF   * BtagSF;
    TWeight_PUDown        = NormWeight * lepSF    * TrigSF * PUSF_Up   * BtagSF;
    TWeight_PUUp          = NormWeight * lepSF    * TrigSF * PUSF_Down * BtagSF;
    TWeight_BtagUp        = NormWeight * ElecSF   * MuonSF * TrigSF * PUSF * BtagSFBtagUp    ;
    TWeight_BtagDown      = NormWeight * ElecSF   * MuonSF * TrigSF * PUSF * BtagSFBtagDown  ;
    TWeight_MistagUp      = NormWeight * ElecSF   * MuonSF * TrigSF * PUSF * BtagSFMistagUp  ;
    TWeight_MistagDown    = NormWeight * ElecSF   * MuonSF * TrigSF * PUSF * BtagSFMistagDown;

    if(gIsData) TWeight = 1;
    
    CalculateTWTTbarVariables(); 
    
    if ((TNJets == 2) && (TNBtags == 2) && (nLooseCentral == 2)) {
      if (TChannel == iMuon || TChannel == iElec) {
          if ((TMET > 20) && (abs(TMll-90.19)>15)) {Tpassreco = 1;}   
      }
      else {Tpassreco = 1;}
    }
    if ((TNJetsJESUp == 1) && (TNBtagsJESUp == 1) && (nLooseCentralJESUp == 1)) {
      TpassrecoJESUp = 1;
    }
    if ((TNJetsJESDown == 1) && (TNBtagsJESDown == 1) && (nLooseCentralJESDown == 1)) {
      TpassrecoJESDown = 1;
    }
    if ((TNJetsJERUp == 1) && (TNBtagsJERUp == 1) && (nLooseCentralJERUp == 1)) {
      TpassrecoJERUp = 1;
    }
  }
  
  //   Setting maximum value of the unfolding candidate variables. 
  if (TMET                 >= 200)         TMET                 = 199.999;
  if (TM_LeadingB          >= 400)         TM_LeadingB          = 399.999;
  if (TM_SubLeadingB       >= 300)         TM_SubLeadingB       = 299.999;
  if (TM_LeadingBj2        >= 400)         TM_LeadingBj2        = 399.999;
  if (TM_SubLeadingBj2     >= 300)         TM_SubLeadingBj2     = 299.999;
  if (TM_bjetlepton_minmax >= 420)         TM_bjetlepton_minmax = 419.999;
  if (TM_LLB               >= 400)         TM_LLB               = 399.999;
  if (TMT_LLMETB           >= 500)         TMT_LLMETB           = 499.999;
  if (TE_LLB               >= 700)         TE_LLB               = 699.999;
  if (TDilepPt             >= 200)         TDilepPt             = 199.999;
  if (DilepJetPt           >= 200)         DilepJetPt           = 199.999;
  if (DilepMETJetPt        >= 150)         DilepMETJetPt        = 149.999;
  if (THTtot               >= 600)         THTtot               = 599.999;
  if (TLeadingJetPt        >= 300)         TLeadingJetPt        = 299.999;
  if (TLeadingJetE         >= 400)         TLeadingJetE         = 399.999;
  if (TLeadingJetPhi       >= TMath::Pi()) TLeadingJetPhi       = 3.14;
  if (TLeadingJetEta       >= 2.4)         TLeadingJetEta       = 2.39999;
  if (TLeadingLepPt        >= 300)         TLeadingLepPt        = 299.999;
  if (TLeadingLepE         >= 350)         TLeadingLepE         = 349.999;
  if (TLeadingLepPhi       >= TMath::Pi()) TLeadingLepPhi       = 3.14;
  if (TLeadingLepEta       >= 2.4)         TLeadingLepEta       = 2.39999;
  if (TSubLeadingLepPt     >= 150)         TSubLeadingLepPt     = 149.999;
  if (TSubLeadingLepE      >= 250)         TSubLeadingLepE      = 249.999;
  if (TSubLeadingLepPhi    >= TMath::Pi()) TSubLeadingLepPhi    = 3.14;
  if (TSubLeadingLepEta    >= 2.4)         TSubLeadingLepEta    = 2.39999;
  if (DilepMETJet1Pz       >= 450)         DilepMETJet1Pz       = 449.999;
  if (TLLMETBEta           >= 5)           TLLMETBEta           = 4.999;
  if (MSys                 >= 700)         MSys                 = 699.999;
  if (TMll                 >= 300)         TMll                 = 299.999;
  if (TDPhiLL              >= TMath::Pi()) TDPhiLL              = 3.14;
  if (TDPhiLeadJet         >= TMath::Pi()) TDPhiLeadJet         = 3.14;
  if (TDPhiSubLeadJet      >= TMath::Pi()) TDPhiSubLeadJet      = 3.14;
  
  if (TMET                 < 0)            TMET                 = 0;
  if (TM_LeadingB          < 0)            TM_LeadingB          = 0;
  if (TM_SubLeadingB       < 0)            TM_SubLeadingB       = 0;
  if (TM_LeadingBj2        < 0)            TM_LeadingBj2        = 0;
  if (TM_SubLeadingBj2     < 0)            TM_SubLeadingBj2     = 0;
  if (TM_bjetlepton_minmax < 0)            TM_bjetlepton_minmax = 0;
  if (TM_LLB               < 0)            TM_LLB               = 0;
  if (TMT_LLMETB           < 0)            TMT_LLMETB           = 0;
  if (TE_LLB               < 0)            TE_LLB               = 0;
  if (TDilepPt             < 0)            TDilepPt             = 0;
  if (DilepJetPt           < 0)            DilepJetPt           = 0;
  if (DilepMETJetPt        < 0)            DilepMETJetPt        = 0;
  if (THTtot               < 0)            THTtot               = 0;
  if (TLeadingJetPt        < 0)            TLeadingJetPt        = 0;
  if (TLeadingJetE         < 0)            TLeadingJetE         = 0;
  if (TLeadingJetPhi       < -TMath::Pi()) TLeadingJetPhi       = -3.14;
  if (TLeadingJetEta       < -2.4)         TLeadingJetEta       = -2.39999;
  if (TLeadingLepPt        < 0)            TLeadingLepPt        = 0;
  if (TLeadingLepE         < 0)            TLeadingLepE         = 0;
  if (TLeadingLepPhi       < -TMath::Pi()) TLeadingLepPhi       = -3.14;
  if (TLeadingLepEta       < -2.4)         TLeadingLepEta       = -2.39999;
  if (TSubLeadingLepPt     < 0)            TSubLeadingLepPt     = 0;
  if (TSubLeadingLepE      < 0)            TSubLeadingLepE      = 0;
  if (TSubLeadingLepPhi    < -TMath::Pi()) TSubLeadingLepPhi    = -3.14;
  if (TSubLeadingLepEta    < -2.4)         TSubLeadingLepEta    = -2.39999;
  if (DilepMETJet1Pz       < -450)         DilepMETJet1Pz       = -449.999;
  if (TLLMETBEta           < -5)           TLLMETBEta           = -4.999;
  if (MSys                 < 0)            MSys                 = 0;
  if (TMll                 < 0)            TMll                 = 0;
  if (TDPhiLL              < -TMath::Pi()) TDPhiLL              = -3.14;
  if (TDPhiLeadJet         < -TMath::Pi()) TDPhiLeadJet         = -3.14;
  if (TDPhiSubLeadJet      < -TMath::Pi()) TDPhiSubLeadJet      = -3.14;
  
  
  if (TMETJESUp                 >= 200)         TMETJESUp                 = 199.999;
  if (TM_LeadingBJESUp          >= 400)         TM_LeadingBJESUp          = 399.999;
  if (TM_SubLeadingBJESUp       >= 300)         TM_SubLeadingBJESUp       = 299.999;
  if (TM_LLBJESUp               >= 400)         TM_LLBJESUp               = 399.999;
  if (TMT_LLMETBJESUp           >= 500)         TMT_LLMETBJESUp           = 499.999;
  if (TE_LLBJESUp               >= 700)         TE_LLBJESUp               = 699.999;
  if (TDilepPtJESUp             >= 200)         TDilepPtJESUp             = 199.999;
  if (DilepJetPtJESUp           >= 200)         DilepJetPtJESUp           = 199.999;
  if (DilepMETJetPtJESUp        >= 150)         DilepMETJetPtJESUp        = 149.999;
  if (THTtotJESUp               >= 600)         THTtotJESUp               = 599.999;
  if (TLeadingJetPtJESUp        >= 150)         TLeadingJetPtJESUp        = 149.999;
  if (TLeadingJetEJESUp         >= 400)         TLeadingJetEJESUp         = 399.999;
  if (TLeadingJetPhiJESUp       >= TMath::Pi()) TLeadingJetPhiJESUp       = 3.14;
  if (TLeadingJetEtaJESUp       >= 2.4)         TLeadingJetEtaJESUp       = 2.39999;
  if (TLeadingLepPtJESUp        >= 150)         TLeadingLepPtJESUp        = 149.999;
  if (TLeadingLepEJESUp         >= 350)         TLeadingLepEJESUp         = 349.999;
  if (TLeadingLepPhiJESUp       >= TMath::Pi()) TLeadingLepPhiJESUp       = 3.14;
  if (TLeadingLepEtaJESUp       >= 2.4)         TLeadingLepEtaJESUp       = 2.39999;
  if (TSubLeadingLepPtJESUp     >= 150)         TSubLeadingLepPtJESUp     = 149.999;
  if (TSubLeadingLepEJESUp      >= 250)         TSubLeadingLepEJESUp      = 249.999;
  if (TSubLeadingLepPhiJESUp    >= TMath::Pi()) TSubLeadingLepPhiJESUp    = 3.14;
  if (TSubLeadingLepEtaJESUp    >= 2.4)         TSubLeadingLepEtaJESUp    = 2.39999;
  if (DilepMETJet1PzJESUp       >= 450)         DilepMETJet1PzJESUp       = 449.999;
  if (TLLMETBEtaJESUp           >= 5)           TLLMETBEtaJESUp           = 4.999;
  if (MSysJESUp                 >= 700)         MSysJESUp                 = 699.999;
  if (TMllJESUp                 >= 300)         TMllJESUp                 = 299.999;
  if (TDPhiLLJESUp              >= TMath::Pi()) TDPhiLLJESUp              = 3.14;
  if (TDPhiLeadJetJESUp         >= TMath::Pi()) TDPhiLeadJetJESUp         = 3.14;
  if (TDPhiSubLeadJetJESUp      >= TMath::Pi()) TDPhiSubLeadJetJESUp      = 3.14;
    
  if (TMETJESUp                 < 0)            TMETJESUp                 = 0;
  if (TM_LeadingBJESUp          < 0)            TM_LeadingBJESUp          = 0;
  if (TM_SubLeadingBJESUp       < 0)            TM_SubLeadingBJESUp       = 0;
  if (TM_LLBJESUp               < 0)            TM_LLBJESUp               = 0;
  if (TMT_LLMETBJESUp           < 0)            TMT_LLMETBJESUp           = 0;
  if (TE_LLBJESUp               < 0)            TE_LLBJESUp               = 0;
  if (TDilepPtJESUp             < 0)            TDilepPtJESUp             = 0;
  if (DilepJetPtJESUp           < 0)            DilepJetPtJESUp           = 0;
  if (DilepMETJetPtJESUp        < 0)            DilepMETJetPtJESUp        = 0;
  if (THTtotJESUp               < 0)            THTtotJESUp               = 0;
  if (TLeadingJetPtJESUp        < 0)            TLeadingJetPtJESUp        = 0;
  if (TLeadingJetEJESUp         < 0)            TLeadingJetEJESUp         = 0;
  if (TLeadingJetPhiJESUp       < -TMath::Pi()) TLeadingJetPhiJESUp       = -3.14;
  if (TLeadingJetEtaJESUp       < -2.4)         TLeadingJetEtaJESUp       = -2.39999;
  if (TLeadingLepPtJESUp        < 0)            TLeadingLepPtJESUp        = 0;
  if (TLeadingLepEJESUp         < 0)            TLeadingLepEJESUp         = 0;
  if (TLeadingLepPhiJESUp       < -TMath::Pi()) TLeadingLepPhiJESUp       = -3.14;
  if (TLeadingLepEtaJESUp       < -2.4)         TLeadingLepEtaJESUp       = -2.39999;
  if (TSubLeadingLepPtJESUp     < 0)            TSubLeadingLepPtJESUp     = 0;
  if (TSubLeadingLepEJESUp      < 0)            TSubLeadingLepEJESUp      = 0;
  if (TSubLeadingLepPhiJESUp    < -TMath::Pi()) TSubLeadingLepPhiJESUp    = -3.14;
  if (TSubLeadingLepEtaJESUp    < -2.4)         TSubLeadingLepEtaJESUp    = -2.39999;
  if (DilepMETJet1PzJESUp       < -450)         DilepMETJet1PzJESUp       = -449.999;
  if (TLLMETBEtaJESUp           < -5)           TLLMETBEtaJESUp           = -4.999;
  if (MSysJESUp                 < 0)            MSysJESUp                 = 0;
  if (TMllJESUp                 < 0)            TMllJESUp                 = 0;
  if (TDPhiLLJESUp              < -TMath::Pi()) TDPhiLLJESUp              = -3.14;
  if (TDPhiLeadJetJESUp         < -TMath::Pi()) TDPhiLeadJetJESUp         = -3.14;
  if (TDPhiSubLeadJetJESUp      < -TMath::Pi()) TDPhiSubLeadJetJESUp      = -3.14;
  
  
  if (TMETJESDown                 >= 200)         TMETJESDown                 = 199.999;
  if (TM_LeadingBJESDown          >= 400)         TM_LeadingBJESDown          = 399.999;
  if (TM_SubLeadingBJESDown       >= 300)         TM_SubLeadingBJESDown       = 299.999;
  if (TM_LLBJESDown               >= 400)         TM_LLBJESDown               = 399.999;
  if (TMT_LLMETBJESDown           >= 500)         TMT_LLMETBJESDown           = 499.999;
  if (TE_LLBJESDown               >= 700)         TE_LLBJESDown               = 699.999;
  if (TDilepPtJESDown             >= 200)         TDilepPtJESDown             = 199.999;
  if (DilepJetPtJESDown           >= 200)         DilepJetPtJESDown           = 199.999;
  if (DilepMETJetPtJESDown        >= 150)         DilepMETJetPtJESDown        = 149.999;
  if (THTtotJESDown               >= 600)         THTtotJESDown               = 599.999;
  if (TLeadingJetPtJESDown        >= 150)         TLeadingJetPtJESDown        = 149.999;
  if (TLeadingJetEJESDown         >= 400)         TLeadingJetEJESDown         = 399.999;
  if (TLeadingJetPhiJESDown       >= TMath::Pi()) TLeadingJetPhiJESDown       = 3.14;
  if (TLeadingJetEtaJESDown       >= 2.4)         TLeadingJetEtaJESDown       = 2.39999;
  if (TLeadingLepPtJESDown        >= 150)         TLeadingLepPtJESDown        = 149.999;
  if (TLeadingLepEJESDown         >= 350)         TLeadingLepEJESDown         = 349.999;
  if (TLeadingLepPhiJESDown       >= TMath::Pi()) TLeadingLepPhiJESDown       = 3.14;
  if (TLeadingLepEtaJESDown       >= 2.4)         TLeadingLepEtaJESDown       = 2.39999;
  if (TSubLeadingLepPtJESDown     >= 150)         TSubLeadingLepPtJESDown     = 149.999;
  if (TSubLeadingLepEJESDown      >= 250)         TSubLeadingLepEJESDown      = 249.999;
  if (TSubLeadingLepPhiJESDown    >= TMath::Pi()) TSubLeadingLepPhiJESDown    = 3.14;
  if (TSubLeadingLepEtaJESDown    >= 2.4)         TSubLeadingLepEtaJESDown    = 2.39999;
  if (DilepMETJet1PzJESDown       >= 450)         DilepMETJet1PzJESDown       = 449.999;
  if (TLLMETBEtaJESDown           >= 5)           TLLMETBEtaJESDown           = 4.999;
  if (MSysJESDown                 >= 700)         MSysJESDown                 = 699.999;
  if (TMllJESDown                 >= 300)         TMllJESDown                 = 299.999;
  if (TDPhiLLJESDown              >= TMath::Pi()) TDPhiLLJESDown              = 3.14;
  if (TDPhiLeadJetJESDown         >= TMath::Pi()) TDPhiLeadJetJESDown         = 3.14;
  if (TDPhiSubLeadJetJESDown      >= TMath::Pi()) TDPhiSubLeadJetJESDown      = 3.14;
    
  if (TMETJESDown                 < 0)            TMETJESDown                 = 0;
  if (TM_LeadingBJESDown          < 0)            TM_LeadingBJESDown          = 0;
  if (TM_SubLeadingBJESDown       < 0)            TM_SubLeadingBJESDown       = 0;
  if (TM_LLBJESDown               < 0)            TM_LLBJESDown               = 0;
  if (TMT_LLMETBJESDown           < 0)            TMT_LLMETBJESDown           = 0;
  if (TE_LLBJESDown               < 0)            TE_LLBJESDown               = 0;
  if (TDilepPtJESDown             < 0)            TDilepPtJESDown             = 0;
  if (DilepJetPtJESDown           < 0)            DilepJetPtJESDown           = 0;
  if (DilepMETJetPtJESDown        < 0)            DilepMETJetPtJESDown        = 0;
  if (THTtotJESDown               < 0)            THTtotJESDown               = 0;
  if (TLeadingJetPtJESDown        < 0)            TLeadingJetPtJESDown        = 0;
  if (TLeadingJetEJESDown         < 0)            TLeadingJetEJESDown         = 0;
  if (TLeadingJetPhiJESDown       < -TMath::Pi()) TLeadingJetPhiJESDown       = -3.14;
  if (TLeadingJetEtaJESDown       < -2.4)         TLeadingJetEtaJESDown       = -2.39999;
  if (TLeadingLepPtJESDown        < 0)            TLeadingLepPtJESDown        = 0;
  if (TLeadingLepEJESDown         < 0)            TLeadingLepEJESDown         = 0;
  if (TLeadingLepPhiJESDown       < -TMath::Pi()) TLeadingLepPhiJESDown       = -3.14;
  if (TLeadingLepEtaJESDown       < -2.4)         TLeadingLepEtaJESDown       = -2.39999;
  if (TSubLeadingLepPtJESDown     < 0)            TSubLeadingLepPtJESDown     = 0;
  if (TSubLeadingLepEJESDown      < 0)            TSubLeadingLepEJESDown      = 0;
  if (TSubLeadingLepPhiJESDown    < -TMath::Pi()) TSubLeadingLepPhiJESDown    = -3.14;
  if (TSubLeadingLepEtaJESDown    < -2.4)         TSubLeadingLepEtaJESDown    = -2.39999;
  if (DilepMETJet1PzJESDown       < -450)         DilepMETJet1PzJESDown       = -449.999;
  if (TLLMETBEtaJESDown           < -5)           TLLMETBEtaJESDown           = -4.999;
  if (MSysJESDown                 < 0)            MSysJESDown                 = 0;
  if (TMllJESDown                 < 0)            TMllJESDown                 = 0;
  if (TDPhiLLJESDown              < -TMath::Pi()) TDPhiLLJESDown              = -3.14;
  if (TDPhiLeadJetJESDown         < -TMath::Pi()) TDPhiLeadJetJESDown         = -3.14;
  if (TDPhiSubLeadJetJESDown      < -TMath::Pi()) TDPhiSubLeadJetJESDown      = -3.14;
  
  
  if (TMETJERUp                 >= 200)         TMETJERUp                 = 199.999;
  if (TM_LeadingBJERUp          >= 400)         TM_LeadingBJERUp          = 399.999;
  if (TM_SubLeadingBJERUp       >= 300)         TM_SubLeadingBJERUp       = 299.999;
  if (TM_LLBJERUp               >= 400)         TM_LLBJERUp               = 399.999;
  if (TMT_LLMETBJERUp           >= 500)         TMT_LLMETBJERUp           = 499.999;
  if (TE_LLBJERUp               >= 700)         TE_LLBJERUp               = 699.999;
  if (TDilepPtJERUp             >= 200)         TDilepPtJERUp             = 199.999;
  if (DilepJetPtJERUp           >= 200)         DilepJetPtJERUp           = 199.999;
  if (DilepMETJetPtJERUp        >= 150)         DilepMETJetPtJERUp        = 149.999;
  if (THTtotJERUp               >= 600)         THTtotJERUp               = 599.999;
  if (TLeadingJetPtJERUp        >= 150)         TLeadingJetPtJERUp        = 149.999;
  if (TLeadingJetEJERUp         >= 400)         TLeadingJetEJERUp         = 399.999;
  if (TLeadingJetPhiJERUp       >= TMath::Pi()) TLeadingJetPhiJERUp       = 3.14;
  if (TLeadingJetEtaJERUp       >= 2.4)         TLeadingJetEtaJERUp       = 2.39999;
  if (TLeadingLepPtJERUp        >= 150)         TLeadingLepPtJERUp        = 149.999;
  if (TLeadingLepEJERUp         >= 350)         TLeadingLepEJERUp         = 349.999;
  if (TLeadingLepPhiJERUp       >= TMath::Pi()) TLeadingLepPhiJERUp       = 3.14;
  if (TLeadingLepEtaJERUp       >= 2.4)         TLeadingLepEtaJERUp       = 2.39999;
  if (TSubLeadingLepPtJERUp     >= 150)         TSubLeadingLepPtJERUp     = 149.999;
  if (TSubLeadingLepEJERUp      >= 250)         TSubLeadingLepEJERUp      = 249.999;
  if (TSubLeadingLepPhiJERUp    >= TMath::Pi()) TSubLeadingLepPhiJERUp    = 3.14;
  if (TSubLeadingLepEtaJERUp    >= 2.4)         TSubLeadingLepEtaJERUp    = 2.39999;
  if (DilepMETJet1PzJERUp       >= 450)         DilepMETJet1PzJERUp       = 449.999;
  if (TLLMETBEtaJERUp           >= 5)           TLLMETBEtaJERUp           = 4.999;
  if (MSysJERUp                 >= 700)         MSysJERUp                 = 699.999;
  if (TMllJERUp                 >= 300)         TMllJERUp                 = 299.999;
  if (TDPhiLLJERUp              >= TMath::Pi()) TDPhiLLJERUp              = 3.14;
  if (TDPhiLeadJetJERUp         >= TMath::Pi()) TDPhiLeadJetJERUp         = 3.14;
  if (TDPhiSubLeadJetJERUp      >= TMath::Pi()) TDPhiSubLeadJetJERUp      = 3.14;
    
  if (TMETJERUp                 < 0)            TMETJERUp                 = 0;
  if (TM_LeadingBJERUp          < 0)            TM_LeadingBJERUp          = 0;
  if (TM_SubLeadingBJERUp       < 0)            TM_SubLeadingBJERUp       = 0;
  if (TM_LLBJERUp               < 0)            TM_LLBJERUp               = 0;
  if (TMT_LLMETBJERUp           < 0)            TMT_LLMETBJERUp           = 0;
  if (TE_LLBJERUp               < 0)            TE_LLBJERUp               = 0;
  if (TDilepPtJERUp             < 0)            TDilepPtJERUp             = 0;
  if (DilepJetPtJERUp           < 0)            DilepJetPtJERUp           = 0;
  if (DilepMETJetPtJERUp        < 0)            DilepMETJetPtJERUp        = 0;
  if (THTtotJERUp               < 0)            THTtotJERUp               = 0;
  if (TLeadingJetPtJERUp        < 0)            TLeadingJetPtJERUp        = 0;
  if (TLeadingJetEJERUp         < 0)            TLeadingJetEJERUp         = 0;
  if (TLeadingJetPhiJERUp       < -TMath::Pi()) TLeadingJetPhiJERUp       = -3.14;
  if (TLeadingJetEtaJERUp       < -2.4)         TLeadingJetEtaJERUp       = -2.39999;
  if (TLeadingLepPtJERUp        < 0)            TLeadingLepPtJERUp        = 0;
  if (TLeadingLepEJERUp         < 0)            TLeadingLepEJERUp         = 0;
  if (TLeadingLepPhiJERUp       < -TMath::Pi()) TLeadingLepPhiJERUp       = -3.14;
  if (TLeadingLepEtaJERUp       < -2.4)         TLeadingLepEtaJERUp       = -2.39999;
  if (TSubLeadingLepPtJERUp     < 0)            TSubLeadingLepPtJERUp     = 0;
  if (TSubLeadingLepEJERUp      < 0)            TSubLeadingLepEJERUp      = 0;
  if (TSubLeadingLepPhiJERUp    < -TMath::Pi()) TSubLeadingLepPhiJERUp    = -3.14;
  if (TSubLeadingLepEtaJERUp    < -2.4)         TSubLeadingLepEtaJERUp    = -2.39999;
  if (DilepMETJet1PzJERUp       < -450)         DilepMETJet1PzJERUp       = -449.999;
  if (TLLMETBEtaJERUp           < -5)           TLLMETBEtaJERUp           = -4.999;
  if (MSysJERUp                 < 0)            MSysJERUp                 = 0;
  if (TMllJERUp                 < 0)            TMllJERUp                 = 0;
  if (TDPhiLLJERUp              < -TMath::Pi()) TDPhiLLJERUp              = -3.14;
  if (TDPhiLeadJetJERUp         < -TMath::Pi()) TDPhiLeadJetJERUp         = -3.14;
  if (TDPhiSubLeadJetJERUp      < -TMath::Pi()) TDPhiSubLeadJetJERUp      = -3.14;
  
  
  if (TGenMET              >= 200)         TGenMET              = 199.999;
  if (TGenM_LeadingB       >= 400)         TGenM_LeadingB       = 399.999;
  if (TGenM_SubLeadingB    >= 300)         TGenM_SubLeadingB    = 299.999;
  if (TGenM_LLB            >= 400)         TGenM_LLB            = 399.999;
  if (TGenM_LeadingBj2     >= 400)         TGenM_LeadingBj2     = 399.999;
  if (TGenM_SubLeadingBj2  >= 300)         TGenM_SubLeadingBj2  = 299.999;
  if (TGenM_bjetlepton_minmax>=420)        TGenM_bjetlepton_minmax=419.999;
  if (TGenMT_LLMETB        >= 500)         TGenMT_LLMETB        = 499.999;
  if (TGenE_LLB            >= 700)         TGenE_LLB            = 699.999;
  if (TGenDilepPt          >= 200)         TGenDilepPt          = 199.999;
  if (TGenDilepJetPt       >= 200)         TGenDilepJetPt       = 199.999;
  if (TGenDilepMETJetPt    >= 150)         TGenDilepMETJetPt    = 149.999;
  if (TGenHTtot            >= 600)         TGenHTtot            = 599.999;
  if (TGenLeadingJetPt     >= 300)         TGenLeadingJetPt     = 299.999;
  if (TGenLeadingJetE      >= 400)         TGenLeadingJetE      = 399.999;
  if (TGenLeadingJetPhi    >= TMath::Pi()) TGenLeadingJetPhi    = 3.14;
  if (TGenLeadingJetEta    >= 2.4)         TGenLeadingJetEta    = 2.39999;
  if (TGenLeadingLepPt     >= 300)         TGenLeadingLepPt     = 299.999;
  if (TGenLeadingLepE      >= 350)         TGenLeadingLepE      = 349.999;
  if (TGenLeadingLepPhi    >= TMath::Pi()) TGenLeadingLepPhi    = 3.14;
  if (TGenLeadingLepEta    >= 2.4)         TGenLeadingLepEta    = 2.39999;
  if (TGenSubLeadingLepPt  >= 150)         TGenSubLeadingLepPt  = 149.999;
  if (TGenSubLeadingLepE   >= 250)         TGenSubLeadingLepE   = 249.999;
  if (TGenSubLeadingLepPhi >= TMath::Pi()) TGenSubLeadingLepPhi = 3.14;
  if (TGenSubLeadingLepEta >= 2.4)         TGenSubLeadingLepEta = 2.39999;
  if (TGenDilepMETJet1Pz   >= 450)         TGenDilepMETJet1Pz   = 449.999;
  if (TGenLLMETBEta        >= 5)           TGenLLMETBEta        = 4.999;
  if (TGenMSys             >= 700)         TGenMSys             = 699.999;
  if (TGenMll              >= 300)         TGenMll              = 299.999;
  if (TGenDPhiLL           >= TMath::Pi()) TGenDPhiLL           = 3.14;
  if (TGenDPhiLeadJet      >= TMath::Pi()) TGenDPhiLeadJet      = 3.14;
  if (TGenDPhiSubLeadJet   >= TMath::Pi()) TGenDPhiSubLeadJet   = 3.14;
  
  if (TGenMET              < 0)            TGenMET              = 0;
  if (TGenM_LeadingB       < 0)            TGenM_LeadingB       = 0;
  if (TGenM_SubLeadingB    < 0)            TGenM_SubLeadingB    = 0;
  if (TGenM_LLB            < 0)            TGenM_LLB            = 0;
  if (TGenM_LeadingBj2     < 0)            TGenM_LeadingBj2     = 0;
  if (TGenM_SubLeadingBj2  < 0)            TGenM_SubLeadingBj2  = 0;
  if (TGenM_bjetlepton_minmax<0)           TGenM_bjetlepton_minmax=0;
  if (TGenMT_LLMETB        < 0)            TGenMT_LLMETB        = 0;
  if (TGenE_LLB            < 0)            TGenE_LLB            = 0;
  if (TGenDilepPt          < 0)            TGenDilepPt          = 0;
  if (TGenDilepJetPt       < 0)            TGenDilepJetPt       = 0;
  if (TGenDilepMETJetPt    < 0)            TGenDilepMETJetPt    = 0;
  if (TGenHTtot            < 0)            TGenHTtot            = 0;
  if (TGenLeadingJetPt     < 0)            TGenLeadingJetPt     = 0;
  if (TGenLeadingJetE      < 0)            TGenLeadingJetE      = 0;
  if (TGenLeadingJetPhi    < -TMath::Pi()) TGenLeadingJetPhi    = -3.14;
  if (TGenLeadingJetEta    < -2.4)         TGenLeadingJetEta    = -2.39999;
  if (TGenLeadingLepPt     < 0)            TGenLeadingLepPt     = 0;
  if (TGenLeadingLepE      < 0)            TGenLeadingLepE      = 0;
  if (TGenLeadingLepPhi    < -TMath::Pi()) TGenLeadingLepPhi    = -3.14;
  if (TGenLeadingLepEta    < -2.4)         TGenLeadingLepEta    = -2.39999;
  if (TGenSubLeadingLepPt  < 0)            TGenSubLeadingLepPt  = 0;
  if (TGenSubLeadingLepE   < 0)            TGenSubLeadingLepE   = 0;
  if (TGenSubLeadingLepPhi < -TMath::Pi()) TGenSubLeadingLepPhi = -3.14;
  if (TGenSubLeadingLepEta < -2.4)         TGenSubLeadingLepEta = -2.39999;
  if (TGenDilepMETJet1Pz   < -450)         TGenDilepMETJet1Pz   = -449.999;
  if (TGenLLMETBEta        < -5)           TGenLLMETBEta        = -4.999;
  if (TGenMSys             < 0)            TGenMSys             = 0;
  if (TGenMll              < 0)            TGenMll              = 0;
  if (TGenDPhiLL           < -TMath::Pi()) TGenDPhiLL           = -3.14;
  if (TGenDPhiLeadJet      < -TMath::Pi()) TGenDPhiLeadJet      = -3.14;
  if (TGenDPhiSubLeadJet   < -TMath::Pi()) TGenDPhiSubLeadJet   = -3.14;
  
  //   Setting dummy value for gen events that don't pass the reco selection for
  // unfolding procedures.
  if (gPar.Contains("Unfolding")) {
    if (Tpassgen && !Tpassreco) {
        TMET              = 99999;
        TMET_Phi          = 99999;
        TM_LeadingB       = 99999;
        TM_SubLeadingB    = 99999;
        TM_LLB            = 99999;
        TM_LeadingBj2     = 99999;
        TM_SubLeadingBj2  = 99999;
        TM_bjetlepton_minmax=99999;
        TMT_LLMETB        = 99999;
        TE_LLB            = 99999;
        TDilepPt          = 99999;
        DilepJetPt        = 99999;
        DilepMETJetPt     = 99999;
        THTtot            = 99999;
        TLeadingJetPt     = 99999;
        TLeadingJetE      = 99999;
        TLeadingJetPhi    = 99999;
        TLeadingJetEta    = 99999;
        TLeadingLepPt     = 99999;
        TLeadingLepE      = 99999;
        TLeadingLepPhi    = 99999;
        TLeadingLepEta    = 99999;
        TSubLeadingLepPt  = 99999;
        TSubLeadingLepE   = 99999;
        TSubLeadingLepPhi = 99999;
        TSubLeadingLepEta = 99999;
        DilepMETJet1Pz    = 99999;
        TLLMETBEta        = 99999;
        MSys              = 99999;
        TMll              = 99999;
        TDPhiLL           = 99999;
        TDPhiLeadJet      = 99999;
        TDPhiSubLeadJet   = 99999;
    }
    if (Tpassgen && !TpassrecoJESUp) {
        TMETJESUp              = 99999;
        TMET_PhiJESUp          = 99999;
        TM_LeadingBJESUp       = 99999;
        TM_SubLeadingBJESUp    = 99999;
        TM_LLBJESUp            = 99999;
        TMT_LLMETBJESUp        = 99999;
        TE_LLBJESUp            = 99999;
        TDilepPtJESUp          = 99999;
        DilepJetPtJESUp        = 99999;
        DilepMETJetPtJESUp     = 99999;
        THTtotJESUp            = 99999;
        TLeadingJetPtJESUp     = 99999;
        TLeadingJetEJESUp      = 99999;
        TLeadingJetPhiJESUp    = 99999;
        TLeadingJetEtaJESUp    = 99999;
        TLeadingLepPtJESUp     = 99999;
        TLeadingLepEJESUp      = 99999;
        TLeadingLepPhiJESUp    = 99999;
        TLeadingLepEtaJESUp    = 99999;
        TSubLeadingLepPtJESUp  = 99999;
        TSubLeadingLepEJESUp   = 99999;
        TSubLeadingLepPhiJESUp = 99999;
        TSubLeadingLepEtaJESUp = 99999;
        DilepMETJet1PzJESUp    = 99999;
        TLLMETBEtaJESUp        = 99999;
        MSysJESUp              = 99999;
        TMllJESUp              = 99999;
        TDPhiLLJESUp           = 99999;
        TDPhiLeadJetJESUp      = 99999;
        TDPhiSubLeadJetJESUp   = 99999;
    }
    if (Tpassgen && !TpassrecoJESDown) {
        TMETJESDown              = 99999;
        TMET_PhiJESDown          = 99999;
        TM_LeadingBJESDown       = 99999;
        TM_SubLeadingBJESDown    = 99999;
        TM_LLBJESDown            = 99999;
        TMT_LLMETBJESDown        = 99999;
        TE_LLBJESDown            = 99999;
        TDilepPtJESDown          = 99999;
        DilepJetPtJESDown        = 99999;
        DilepMETJetPtJESDown     = 99999;
        THTtotJESDown            = 99999;
        TLeadingJetPtJESDown     = 99999;
        TLeadingJetEJESDown      = 99999;
        TLeadingJetPhiJESDown    = 99999;
        TLeadingJetEtaJESDown    = 99999;
        TLeadingLepPtJESDown     = 99999;
        TLeadingLepEJESDown      = 99999;
        TLeadingLepPhiJESDown    = 99999;
        TLeadingLepEtaJESDown    = 99999;
        TSubLeadingLepPtJESDown  = 99999;
        TSubLeadingLepEJESDown   = 99999;
        TSubLeadingLepPhiJESDown = 99999;
        TSubLeadingLepEtaJESDown = 99999;
        DilepMETJet1PzJESDown    = 99999;
        TLLMETBEtaJESDown        = 99999;
        MSysJESDown              = 99999;
        TMllJESDown              = 99999;
        TDPhiLLJESDown           = 99999;
        TDPhiLeadJetJESDown      = 99999;
        TDPhiSubLeadJetJESDown   = 99999;
    }
    if (Tpassgen && !TpassrecoJERUp) {
        TMETJERUp              = 99999;
        TMET_PhiJERUp          = 99999;
        TM_LeadingBJERUp       = 99999;
        TM_SubLeadingBJERUp    = 99999;
        TM_LLBJERUp            = 99999;
        TMT_LLMETBJERUp        = 99999;
        TE_LLBJERUp            = 99999;
        TDilepPtJERUp          = 99999;
        DilepJetPtJERUp        = 99999;
        DilepMETJetPtJERUp     = 99999;
        THTtotJERUp            = 99999;
        TLeadingJetPtJERUp     = 99999;
        TLeadingJetEJERUp      = 99999;
        TLeadingJetPhiJERUp    = 99999;
        TLeadingJetEtaJERUp    = 99999;
        TLeadingLepPtJERUp     = 99999;
        TLeadingLepEJERUp      = 99999;
        TLeadingLepPhiJERUp    = 99999;
        TLeadingLepEtaJERUp    = 99999;
        TSubLeadingLepPtJERUp  = 99999;
        TSubLeadingLepEJERUp   = 99999;
        TSubLeadingLepPhiJERUp = 99999;
        TSubLeadingLepEtaJERUp = 99999;
        DilepMETJet1PzJERUp    = 99999;
        TLLMETBEtaJERUp        = 99999;
        MSysJERUp              = 99999;
        TMllJERUp              = 99999;
        TDPhiLLJERUp           = 99999;
        TDPhiLeadJetJERUp      = 99999;
        TDPhiSubLeadJetJERUp   = 99999;
    }
  }
  if (Tpassreco || Tpassgen) {
  
  //if (Tpassgen || Tpassreco || TpassrecoJESUp || TpassrecoJESDown || TpassrecoJERUp 
      //|| (TNJets == 2 && TNBtags == 2) || (TNJetsJESUp == 1 && TNBtagsJESUp == 1) 
      //|| (TNJetsJESDown == 1 && TNBtagsJESDown == 1) || (TNJetsJERUp == 1 && TNBtagsJERUp == 1))  { // If needed, filling.
    fMini1j1t->Fill();
  }
}


void TWTTbarAnalysis::Summary(){}            //=============== Summary


//#####################################################################
// Functions
//---------------------------------------------------------------------
void TWTTbarAnalysis::GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons){
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
    TIsOSDilep = passTrigger && passMETfilters && (!isSS) && ((selLeptons.at(0).p + selLeptons.at(1).p).M() > 20) && selLeptons.at(0).Pt() > 25;
  else
    TIsOSDilep = false;
  
  if (TNSelLeps >= 1) {
    TLeadingLepPt          = selLeptons.at(0).Pt();
    TLeadingLepE           = selLeptons.at(0).E();
    TLeadingLepPhi         = selLeptons.at(0).Phi();
    TLeadingLepEta         = selLeptons.at(0).Eta();
    TLeadingLepPtJESUp     = selLeptons.at(0).Pt();
    TLeadingLepEJESUp      = selLeptons.at(0).E();
    TLeadingLepPhiJESUp    = selLeptons.at(0).Phi();
    TLeadingLepEtaJESUp    = selLeptons.at(0).Eta();
    TLeadingLepPtJESDown   = selLeptons.at(0).Pt();
    TLeadingLepEJESDown    = selLeptons.at(0).E();
    TLeadingLepPhiJESDown  = selLeptons.at(0).Phi();
    TLeadingLepEtaJESDown  = selLeptons.at(0).Eta();
    TLeadingLepPtJERUp     = selLeptons.at(0).Pt();
    TLeadingLepEJERUp      = selLeptons.at(0).E();
    TLeadingLepPhiJERUp    = selLeptons.at(0).Phi();
    TLeadingLepEtaJERUp    = selLeptons.at(0).Eta();
    if (TNSelLeps >= 2) {
      TSubLeadingLepPt          = selLeptons.at(1).Pt();
      TSubLeadingLepE           = selLeptons.at(1).E();
      TSubLeadingLepPhi         = selLeptons.at(1).Phi();
      TSubLeadingLepEta         = selLeptons.at(1).Eta();
      TSubLeadingLepPtJESUp     = selLeptons.at(1).Pt();
      TSubLeadingLepEJESUp      = selLeptons.at(1).E();
      TSubLeadingLepPhiJESUp    = selLeptons.at(1).Phi();
      TSubLeadingLepEtaJESUp    = selLeptons.at(1).Eta();
      TSubLeadingLepPtJESDown   = selLeptons.at(1).Pt();
      TSubLeadingLepEJESDown    = selLeptons.at(1).E();
      TSubLeadingLepPhiJESDown  = selLeptons.at(1).Phi();
      TSubLeadingLepEtaJESDown  = selLeptons.at(1).Eta();
      TSubLeadingLepPtJERUp     = selLeptons.at(1).Pt();
      TSubLeadingLepEJERUp      = selLeptons.at(1).E();
      TSubLeadingLepPhiJERUp    = selLeptons.at(1).Phi();
      TSubLeadingLepEtaJERUp    = selLeptons.at(1).Eta();
    }
  }
  
  SetParam("TIsOSDilep",TIsOSDilep);
}


void TWTTbarAnalysis::GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut){
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
  
  if (TNJets > 0){
    TLeadingJetPt  = selJets.at(0).Pt();
    TLeadingJetE   = selJets.at(0).E();
    TLeadingJetPhi = selJets.at(0).Phi();
    TLeadingJetEta = selJets.at(0).Eta();
    TLeadingJetCSV = selJets.at(0).csv;
  }
  if (TNJets > 1){
    TSubLeadingJetPt  = selJets.at(1).Pt();
    TSubLeadingJetEta = selJets.at(1).Eta();
    TSubLeadingJetCSV = selJets.at(1).csv;
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

  if(gIsData) return;
  
  TNJetsJESUp     = 0;
  TNJetsJESDown   = 0;
  TNBtagsJESUp    = 0;
  TNBtagsJESDown  = 0;
  TNBtagsJERUp    = 0;
  TNJetsJERUp     = 0;

  TNJetsJESUp   = selJetsJecUp.size();
  TNJetsJESDown = selJetsJecDown.size();
  TNJetsJERUp   = selJetsJER.size();
  
  for (auto& jet : selJetsJecUp) {
    THTJESUp += jet.pTJESUp;
    if (jet.isBtag) TNBtagsJESUp++;
  }
  for (auto& jet : selJetsJecDown) {
    THTJESDown += jet.pTJESDown;
    if (jet.isBtag) TNBtagsJESDown++;
  }
  for (auto& jet : selJetsJER) {
    if (jet.isBtag) TNBtagsJERUp++;
  }
  
  if (!gIsData) {
    if (TNJetsJESUp >= 1) {
      TLeadingJetPtJESUp  = selJetsJecUp.at(0).Pt();
      TLeadingJetEJESUp   = selJetsJecUp.at(0).E();
      TLeadingJetPhiJESUp = selJetsJecUp.at(0).Phi();
      TLeadingJetEtaJESUp = selJetsJecUp.at(0).Eta();
    }
    if (TNJetsJESDown >= 1) {
      TLeadingJetPtJESDown  = selJetsJecDown.at(0).Pt();
      TLeadingJetEJESDown   = selJetsJecDown.at(0).E();
      TLeadingJetPhiJESDown = selJetsJecDown.at(0).Phi();
      TLeadingJetEtaJESDown = selJetsJecDown.at(0).Eta();
    }
    if (TNJetsJERUp >= 1) {
      TLeadingJetPtJERUp  = selJetsJER.at(0).Pt();
      TLeadingJetEJERUp   = selJetsJER.at(0).E();
      TLeadingJetPhiJERUp = selJetsJER.at(0).Phi();
      TLeadingJetEtaJERUp = selJetsJER.at(0).Eta();
    }
  }
}


void TWTTbarAnalysis::GetGenJetVariables(std::vector<Jet> genJets, std::vector<Jet> mcJets){
  if(gIsData) return;
  nFiduJets = 0; nFidubJets = 0; 
  Int_t nGenJets = genJets.size();
  Int_t nmcJets = mcJets.size();
  for (Int_t i = 0; i < nGenJets; i++) if(genJets.at(i).p.Pt() > 30 && TMath::Abs(genJets.at(i).p.Eta()) < 2.4)                         nFiduJets++;
  for (Int_t i = 0; i <  nmcJets; i++) if(mcJets.at(i).p.Pt()  > 30 && TMath::Abs(mcJets.at(i).Eta())    < 2.4 && mcJets.at(i).isBtag)  nFidubJets++;

  if (! gPar.Contains("Unfolding")) return;
  
  nSergioGenJets = Get<Int_t>("nGenJetSergio");
  nSergiobJets = 0; nSergioJets = 0; nSergioLooseCentralbJets = 0; nSergioLooseFwdJets = 0;
  
  for (Int_t i = 0; i < nSergioGenJets; i++) {
    tpJ.SetPtEtaPhiM(Get<Float_t>("GenJetSergio_pt",i), Get<Float_t>("GenJetSergio_eta",i), Get<Float_t>("GenJetSergio_phi",i), Get<Float_t>("GenJetSergio_mass",i));
    tJ = Jet(tpJ, 0, 1, 0);
    tJ.isBtag = (TMath::Abs(Get<Int_t>("GenJetSergio_pdgId", i)) == 5);
    if (tJ.p.Pt() > 20) {
      if (TMath::Abs(tJ.p.Eta()) < 2.4 && Cleaning(tJ, SergioLeps, 0.4)) {
        SergioLooseCentralJets.push_back(tJ);
        if (tJ.isBtag) nSergioLooseCentralbJets++;
        if (tJ.p.Pt() > 30) {
          SergioJets.push_back(tJ);
          if (tJ.isBtag) nSergiobJets++;
        }
      }
      else if (TMath::Abs(tJ.p.Eta()) < 4.7) {
        SergioLooseFwdJets.push_back(tJ);
      }
    }
  }
  
  nSergioJets             = SergioJets.size();
  nSergioLooseCentralJets = SergioLooseCentralJets.size();
  nSergioLooseFwdJets     = SergioLooseFwdJets.size();
  
  if (nSergioLooseCentralJets >= 2) {
    TGenSubLeadingLooseCentralJetPt   = SergioLooseCentralJets.at(1).Pt();
    TGenSubLeadingLooseCentralJetE    = SergioLooseCentralJets.at(1).E();
    TGenSubLeadingLooseCentralJetPhi  = SergioLooseCentralJets.at(1).Phi();
    TGenSubLeadingLooseCentralJetEta  = SergioLooseCentralJets.at(1).Eta();
  }
  if (nSergioJets >= 1) {
    TGenLeadingJetPt   = SergioJets.at(0).Pt();
    TGenLeadingJetE    = SergioJets.at(0).E();
    TGenLeadingJetPhi  = SergioJets.at(0).Phi();
    TGenLeadingJetEta  = SergioJets.at(0).Eta();
  }
  if (nSergioLooseFwdJets >= 1) {
    TGenLeadingLooseFwdJetPt  = SergioLooseFwdJets.at(0).Pt();
    TGenLeadingLooseFwdJetE   = SergioLooseFwdJets.at(0).E();
    TGenLeadingLooseFwdJetPhi = SergioLooseFwdJets.at(0).Phi();
    TGenLeadingLooseFwdJetEta = SergioLooseFwdJets.at(0).Eta();
  }
}


void TWTTbarAnalysis::GetGenLepVariables() {
  if(gIsData) return;
  nSergioGenLeps = Get<Int_t>("nGenLepSergio");
  
  for (Int_t i = 0; i < nSergioGenLeps; i++) {
    tpL.SetPtEtaPhiM(Get<Float_t>("GenLepSergio_pt",i), Get<Float_t>("GenLepSergio_eta",i), Get<Float_t>("GenLepSergio_phi",i), Get<Float_t>("GenLepSergio_mass",i));
    tL = Lepton(tpL, Get<Int_t>("GenLepSergio_charge", i), (TMath::Abs(Get<Int_t>("GenLepSergio_pdgId",i)) == 11) ? 1 : 0);
    if (tL.p.Pt() > 20 && TMath::Abs(tL.p.Eta()) < 2.4) {
      if (tL.isMuon) SergioLeps.push_back(tL);
      else if (tL.isElec  && (TMath::Abs(tL.p.Eta()) < 1.4442 || TMath::Abs(tL.p.Eta()) > 1.566)) SergioLeps.push_back(tL);
    }
  }
  
  nSergioLeps = SergioLeps.size();
  
  if (nSergioLeps >= 1) {
    TGenLeadingLepPt   = SergioLeps.at(0).Pt();
    TGenLeadingLepE    = SergioLeps.at(0).E();
    TGenLeadingLepPhi  = SergioLeps.at(0).Phi();
    TGenLeadingLepEta  = SergioLeps.at(0).Eta();
    if (nSergioLeps >= 2) {
      TGenSubLeadingLepPt   = SergioLeps.at(1).Pt();
      TGenSubLeadingLepE    = SergioLeps.at(1).E();
      TGenSubLeadingLepPhi  = SergioLeps.at(1).Phi();
      TGenSubLeadingLepEta  = SergioLeps.at(1).Eta();
    }
  }
}

float TWTTbarAnalysis::getTopPtRW() {
  if (!gIsTTbar) return 1;
  
  float SF = 1;

  if (Get<Int_t>("nGenTop") > 1){
    float pt1 = Get<Float_t>("GenTop_pt"  , 0);
    float pt2 = Get<Float_t>("GenTop_pt"  , 1);
    SF *= TMath::Exp(0.0615-0.0005 * pt1);
    SF *= TMath::Exp(0.0615-0.0005 * pt2);
    return TMath::Sqrt(SF);      
  }
  else{
    cout << "Error Error Error get top pt rw" << endl;
    return -1;
  }
}

void TWTTbarAnalysis::GetMET(){
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

void TWTTbarAnalysis::GetGenMET() {
  if(gIsData) return;
  
  nSergioGenMET = Get<Int_t>("nGenMetSergio");
  for (Int_t i = 0; i < nSergioGenMET; i++) {
    tMET.SetPtEtaPhiE(Get<Float_t>("GenMetSergio_pt", i), 0, Get<Float_t>("GenMetSergio_phi", i), Get<Float_t>("GenMetSergio_pt", i));
    SergioMET += tMET;
  }
  
  TGenMET     = SergioMET.Pt();
  TGenMET_Phi = SergioMET.Phi();
}


void TWTTbarAnalysis::SetTWTTbarVariables() {
  // Minitree 1j1t
  fMini1j1t->Branch("TChannel",              &TChannel,              "TChannel/I");
  fMini1j1t->Branch("TIsSS",                 &TIsSS,                 "TIsSS/B");
  fMini1j1t->Branch("TWeight",               &TWeight,               "TWeight/F");
  fMini1j1t->Branch("TWeight_ElecEffUp",     &TWeight_ElecEffUp,     "TWeight_ElecEffUp/F");
  fMini1j1t->Branch("TWeight_ElecEffDown",   &TWeight_ElecEffDown,   "TWeight_ElecEffDown/F");
  fMini1j1t->Branch("TWeight_MuonEffUp",     &TWeight_MuonEffUp,     "TWeight_MuonEffUp/F");
  fMini1j1t->Branch("TWeight_MuonEffDown",   &TWeight_MuonEffDown,   "TWeight_MuonEffDown/F");
  fMini1j1t->Branch("TWeight_TrigUp",        &TWeight_TrigUp,        "TWeight_TrigUp/F");
  fMini1j1t->Branch("TWeight_TrigDown",      &TWeight_TrigDown,      "TWeight_TrigDown/F");
  fMini1j1t->Branch("TWeight_PUUp",          &TWeight_PUUp,          "TWeight_PUUp/F");
  fMini1j1t->Branch("TWeight_PUDown",        &TWeight_PUDown,        "TWeight_PUDown/F");
  fMini1j1t->Branch("TWeight_BtagUp",        &TWeight_BtagUp,        "TWeight_BtagUp/F");
  fMini1j1t->Branch("TWeight_BtagDown",      &TWeight_BtagDown,      "TWeight_BtagDown/F");
  fMini1j1t->Branch("TWeight_MistagUp",      &TWeight_MistagUp,      "TWeight_MistagUp/F");
  fMini1j1t->Branch("TWeight_MistagDown",    &TWeight_MistagDown,    "TWeight_MistagDown/F");
  fMini1j1t->Branch("TLHEWeight",            TLHEWeight,             "TLHEWeight[254]/F");

  fMini1j1t->Branch("TNJets"       ,         &TNJets,                "TNJets/I"        );
  fMini1j1t->Branch("TNJetsJESUp"  ,         &TNJetsJESUp,           "TNJetsJESUp/I"   );
  fMini1j1t->Branch("TNJetsJESDown",         &TNJetsJESDown,         "TNJetsJESDown/I" );
  fMini1j1t->Branch("TNJetsJERUp",           &TNJetsJERUp,           "TNJetsJERUp/I" );
  fMini1j1t->Branch("TNBtags"       ,        &TNBtags,               "TNBtags/I"        );
  fMini1j1t->Branch("TNBtagsJESUp"  ,        &TNBtagsJESUp,          "TNBtagsJESUp/I"   );
  fMini1j1t->Branch("TNBtagsJESDown",        &TNBtagsJESDown,        "TNBtagsJESDown/I" );
  fMini1j1t->Branch("TNBtagsJERUp",          &TNBtagsJERUp,          "TNBtagsJERUp/I" );
  fMini1j1t->Branch("TnLooseCentral"        ,&nLooseCentral      ,   "TnLooseCentral/I"       );
  fMini1j1t->Branch("TnLooseCentralJESUp"   ,&nLooseCentralJESUp  ,  "TnLooseCentralJESUp/I"       );
  fMini1j1t->Branch("TnLooseCentralJESDown" ,&nLooseCentralJESDown,  "TnLooseCentralJESDown/I"       );
  fMini1j1t->Branch("TnLooseCentralJERUp"   ,&nLooseCentralJERUp  ,  "TnLooseCentralJERUp/I"       );
  fMini1j1t->Branch("TnBLooseCentral"       ,&nBLooseCentral      ,  "TnBLooseCentral/I"       );
  fMini1j1t->Branch("TnBLooseCentralJESUp"  ,&nBLooseCentralJESUp  , "TnBLooseCentralJESUp/I"       );
  fMini1j1t->Branch("TnBLooseCentralJESDown",&nBLooseCentralJESDown, "TnBLooseCentralJESDown/I");
  fMini1j1t->Branch("TnBLooseCentralJERUp"  ,&nBLooseCentralJERUp  , "TnBLooseCentralJERUp/I");
  fMini1j1t->Branch("TnLooseFwd"            ,&nLooseFwd            , "TnLooseFwd/I"          );
  fMini1j1t->Branch("TnLooseFwdJESUp"       ,&nLooseFwdJESUp        ,"TnLooseFwdJESUp/I"     );
  fMini1j1t->Branch("TnLooseFwdJESDown"     ,&nLooseFwdJESDown      ,"TnLooseFwdJESDown/I"   );
  fMini1j1t->Branch("TnLooseFwdJERUp"       ,&nLooseFwdJERUp        ,"TnLooseFwdJERUp/I"     );

  fMini1j1t->Branch("TDilepMETJetPt"        , &DilepMETJetPt      , "TDilepMETJetPt/F"       );
  fMini1j1t->Branch("TTJet1_pt"             , &TJet1_pt           , "TTJet1_pt/F"            );
  fMini1j1t->Branch("TTJetLooseCentralpt"   , &TJetLooseCentralpt , "TTJetLooseCentralpt/F"  );
  fMini1j1t->Branch("TTJetLooseFwdpt"       , &TJetLooseFwdpt     , "TTJetLooseFwdpt/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtot" , &DilepmetjetOverHT  , "TDilepMETJetPt_THTtot/F");
  fMini1j1t->Branch("TMSys"                 , &MSys               , "TMSys/F"                );
  fMini1j1t->Branch("TC_jll"                , &C_jll              , "TC_jll/F"               );
  fMini1j1t->Branch("THTLepOverHT"          , &HTLepOverHT        , "THTLepOverHT/F"         );
  fMini1j1t->Branch("TDilepJetPt"           , &DilepJetPt         , "TDilepJetPt/F"          );
  fMini1j1t->Branch("TDilepMETJet1Pz",        &DilepMETJet1Pz,      "DilepMETJet1Pz/F"       );
  fMini1j1t->Branch("TLLMETBEta",             &TLLMETBEta,          "TLLMETBEta/F"           );
  fMini1j1t->Branch("TMll",                   &TMll,                "TMll/F"           );

  fMini1j1t->Branch("TDilepMETJetPtJESUp"        , &DilepMETJetPtJESUp      , "TDilepMETJetPtJESUp/F"       );
  fMini1j1t->Branch("TTHTtotJESUp"               , &THTtotJESUp             , "TTHTtotJESUp/F"              );
  fMini1j1t->Branch("TTJet1_ptJESUp"             , &TJet1_ptJESUp           , "TTJet1_ptJESUp/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJESUp"   , &TJetLooseCentralptJESUp , "TTJetLooseCentralptJESUp/F"  );
  fMini1j1t->Branch("TTJetLooseFwdptJESUp"       , &TJetLooseFwdptJESUp     , "TTJetLooseFwdptJESUp/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJESUp" , &DilepmetjetOverHTJESUp  , "TDilepMETJetPt_THTtotJESUp/F");
  fMini1j1t->Branch("TMSysJESUp"                 , &MSysJESUp               , "TMSysJESUp/F"                );
  fMini1j1t->Branch("TC_jllJESUp"                , &C_jllJESUp              , "TC_jllJESUp/F"               );
  fMini1j1t->Branch("THTLepOverHTJESUp"          , &HTLepOverHTJESUp        , "THTLepOverHTJESUp/F"         );
  fMini1j1t->Branch("TDilepJetPtJESUp"           , &DilepJetPtJESUp         , "TDilepJetPtJESUp/F"          );
  fMini1j1t->Branch("TDilepMETJet1PzJESUp",        &DilepMETJet1PzJESUp,      "DilepMETJet1PzJESUp/F"       );
  fMini1j1t->Branch("TLLMETBEtaJESUp",             &TLLMETBEtaJESUp,          "TLLMETBEtaJESUp/F"           );
  fMini1j1t->Branch("TMllJESUp",                   &TMllJESUp,                "TMllJESUp/F"                 );

  fMini1j1t->Branch("TDilepMETJetPtJESDown"        , &DilepMETJetPtJESDown      , "TDilepMETJetPtJESDown/F"       );
  fMini1j1t->Branch("TTHTtotJESDown"               , &THTtotJESDown             , "TTHTtotJESDown/F"              );
  fMini1j1t->Branch("TTJet1_ptJESDown"             , &TJet1_ptJESDown           , "TTJet1_ptJESDown/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJESDown"   , &TJetLooseCentralptJESDown , "TTJetLooseCentralptJESDown/F"  );
  fMini1j1t->Branch("TTJetLooseFwdptJESDown"       , &TJetLooseFwdptJESDown     , "TTJetLooseFwdptJESDown/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJESDown" , &DilepmetjetOverHTJESDown  , "TDilepMETJetPt_THTtotJESDown/F");
  fMini1j1t->Branch("TMSysJESDown"                 , &MSysJESDown               , "TMSysJESDown/F"                );
  fMini1j1t->Branch("TC_jllJESDown"                , &C_jllJESDown              , "TC_jllJESDown/F"               );
  fMini1j1t->Branch("THTLepOverHTJESDown"          , &HTLepOverHTJESDown        , "THTLepOverHTJESDown/F"         );
  fMini1j1t->Branch("TDilepJetPtJESDown"           , &DilepJetPtJESDown         , "TDilepJetPtJESDown/F"          );
  fMini1j1t->Branch("TDilepMETJet1PzJESDown",        &DilepMETJet1PzJESDown,      "DilepMETJet1PzJESDown/F"       );
  fMini1j1t->Branch("TLLMETBEtaJESDown",             &TLLMETBEtaJESDown,          "TLLMETBEtaJESDown/F"           );
  fMini1j1t->Branch("TMllJESDown",                   &TMllJESDown,                "TMllJESDown/F"                 );

  fMini1j1t->Branch("TDilepMETJetPtJERUp"        , &DilepMETJetPtJERUp      , "TDilepMETJetPtJERUp/F"       );
  fMini1j1t->Branch("TTHTtotJERUp"               , &THTtotJERUp             , "TTHTtotJERUp/F"              );
  fMini1j1t->Branch("TTJet1_ptJERUp"             , &TJet1_ptJERUp           , "TTJet1_ptJERUp/F"            );
  fMini1j1t->Branch("TTJetLooseCentralptJERUp"   , &TJetLooseCentralptJERUp , "TTJetLooseCentralptJERUp/F"  );
  fMini1j1t->Branch("TTJetLooseFwdptJERUp"       , &TJetLooseFwdptJERUp     , "TTJetLooseFwdptJERUp/F"  );
  fMini1j1t->Branch("TDilepMETJetPt_THTtotJERUp" , &DilepmetjetOverHTJERUp  , "TDilepMETJetPt_THTtotJERUp/F");
  fMini1j1t->Branch("TMSysJERUp"                 , &MSysJERUp               , "TMSysJERUp/F"                );
  fMini1j1t->Branch("TC_jllJERUp"                , &C_jllJERUp              , "TC_jllJERUp/F"               );
  fMini1j1t->Branch("THTLepOverHTJERUp"          , &HTLepOverHTJERUp        , "THTLepOverHTJERUp/F"         );
  fMini1j1t->Branch("TDilepJetPtJERUp"           , &DilepJetPtJERUp         , "TDilepJetPtJERUp/F"          );
  fMini1j1t->Branch("TDilepMETJet1PzJERUp",        &DilepMETJet1PzJERUp,      "DilepMETJet1PzJERUp/F"       );
  fMini1j1t->Branch("TLLMETBEtaJERUp",             &TLLMETBEtaJERUp,          "TLLMETBEtaJERUp/F"           );
  fMini1j1t->Branch("TMllJERUp",                   &TMllJERUp,                "TMllJERUp/F"                 );
  
  // Things added by me (WOLOLO):
  fMini1j1t->Branch("TMET",                    &TMET,                       "TMET/F");
  fMini1j1t->Branch("TMET_Phi",                &TMET_Phi,                   "TMET_Phi/F");
  fMini1j1t->Branch("TM_LeadingB" ,            &TM_LeadingB,                "TM_LeadingB/F");
  fMini1j1t->Branch("TM_SubLeadingB",          &TM_SubLeadingB,             "TM_SubLeadingB/F");
  fMini1j1t->Branch("TM_LeadingBj2",           &TM_LeadingBj2,              "TM_LeadingBj2/F"); // nueva 
  fMini1j1t->Branch("TM_SubLeadingBj2",        &TM_SubLeadingBj2,           "TM_SubLeadingBj2/F"); // nueva
  fMini1j1t->Branch("TM_bjetlepton_minmax",    &TM_bjetlepton_minmax,       "TM_bjetlepton_minmax/F"); // variable de atlas
  fMini1j1t->Branch("TE_LLB",                  &TE_LLB,                     "TE_LLB/F");
  fMini1j1t->Branch("TMT_LLMETB",              &TMT_LLMETB,                 "TMT_LLMETB/F");
  fMini1j1t->Branch("TM_LLB",                  &TM_LLB,                     "TM_LLB/F");
  fMini1j1t->Branch("THTtot",                  &THTtot,                     "THTtot/F");
  fMini1j1t->Branch("TDilepPt",                &TDilepPt,                   "TDilepPt/F");
  fMini1j1t->Branch("TLeadingJetPt",           &TLeadingJetPt,              "TLeadingJetPt/F");
  fMini1j1t->Branch("TLeadingJetE",            &TLeadingJetE,               "TLeadingJetE/F");
  fMini1j1t->Branch("TLeadingJetPhi",          &TLeadingJetPhi,             "TLeadingJetPhi/F");
  fMini1j1t->Branch("TLeadingJetEta",          &TLeadingJetEta,             "TLeadingJetEta/F");
  fMini1j1t->Branch("TLeadingLepPt",           &TLeadingLepPt,              "TLeadingLepPt/F");
  fMini1j1t->Branch("TLeadingLepE",            &TLeadingLepE,               "TLeadingLepE/F");
  fMini1j1t->Branch("TLeadingLepPhi",          &TLeadingLepPhi,             "TLeadingLepPhi/F");
  fMini1j1t->Branch("TLeadingLepEta",          &TLeadingLepEta,             "TLeadingLepEta/F");
  fMini1j1t->Branch("TSubLeadingLepPt",        &TSubLeadingLepPt,           "TSubLeadingLepPt/F");
  fMini1j1t->Branch("TSubLeadingLepE",         &TSubLeadingLepE,            "TSubLeadingLepE/F");
  fMini1j1t->Branch("TSubLeadingLepPhi",       &TSubLeadingLepPhi,          "TSubLeadingLepPhi/F");
  fMini1j1t->Branch("TSubLeadingLepEta",       &TSubLeadingLepEta,          "TSubLeadingLepEta/F");
  fMini1j1t->Branch("TWeight_normal",          &TWeight_normal,             "TWeight_normal/F");
  fMini1j1t->Branch("Tpassreco",               &Tpassreco,                  "Tpassreco/B");
  fMini1j1t->Branch("TDPhiLL",                 &TDPhiLL,                    "TDPhiLL/F");
  fMini1j1t->Branch("TDPhiLeadJet",            &TDPhiLeadJet,               "TDPhiLeadJet/F");
  fMini1j1t->Branch("TDPhiSubLeadJet",         &TDPhiSubLeadJet,            "TDPhiSubLeadJet/F");
  
  fMini1j1t->Branch("TMETJESUp",               &TMETJESUp,                  "TMETJESUp/F");
  fMini1j1t->Branch("TMET_PhiJESUp",           &TMET_PhiJESUp,              "TMET_PhiJESUp/F");
  fMini1j1t->Branch("TM_LeadingBJESUp" ,       &TM_LeadingBJESUp,           "TM_LeadingBJESUp/F");
  fMini1j1t->Branch("TM_SubLeadingBJESUp",     &TM_SubLeadingBJESUp,        "TM_SubLeadingBJESUp/F");
  fMini1j1t->Branch("TE_LLBJESUp",             &TE_LLBJESUp,                "TE_LLBJESUp/F");
  fMini1j1t->Branch("TMT_LLMETBJESUp",         &TMT_LLMETBJESUp,            "TMT_LLMETBJESUp/F");
  fMini1j1t->Branch("TM_LLBJESUp",             &TM_LLBJESUp,                "TM_LLBJESUp/F");
  fMini1j1t->Branch("THTtotJESUp",             &THTtotJESUp,                "THTtotJESUp/F");
  fMini1j1t->Branch("TDilepPtJESUp",           &TDilepPtJESUp,              "TDilepPtJESUp/F");
  fMini1j1t->Branch("TLeadingJetPtJESUp",      &TLeadingJetPtJESUp,         "TLeadingJetPtJESUp/F");
  fMini1j1t->Branch("TLeadingJetEJESUp",       &TLeadingJetEJESUp,          "TLeadingJetEJESUp/F");
  fMini1j1t->Branch("TLeadingJetPhiJESUp",     &TLeadingJetPhiJESUp,        "TLeadingJetPhiJESUp/F");
  fMini1j1t->Branch("TLeadingJetEtaJESUp",     &TLeadingJetEtaJESUp,        "TLeadingJetEtaJESUp/F");
  fMini1j1t->Branch("TLeadingLepPtJESUp",      &TLeadingLepPtJESUp,         "TLeadingLepPtJESUp/F");
  fMini1j1t->Branch("TLeadingLepEJESUp",       &TLeadingLepEJESUp,          "TLeadingLepEJESUp/F");
  fMini1j1t->Branch("TLeadingLepPhiJESUp",     &TLeadingLepPhiJESUp,        "TLeadingLepPhiJESUp/F");
  fMini1j1t->Branch("TLeadingLepEtaJESUp",     &TLeadingLepEtaJESUp,        "TLeadingLepEtaJESUp/F");
  fMini1j1t->Branch("TSubLeadingLepPtJESUp",   &TSubLeadingLepPtJESUp,      "TSubLeadingLepPtJESUp/F");
  fMini1j1t->Branch("TSubLeadingLepEJESUp",    &TSubLeadingLepEJESUp,       "TSubLeadingLepEJESUp/F");
  fMini1j1t->Branch("TSubLeadingLepPhiJESUp",  &TSubLeadingLepPhiJESUp,     "TSubLeadingLepPhiJESUp/F");
  fMini1j1t->Branch("TSubLeadingLepEtaJESUp",  &TSubLeadingLepEtaJESUp,     "TSubLeadingLepEtaJESUp/F");
  fMini1j1t->Branch("TpassrecoJESUp",          &TpassrecoJESUp,             "TpassrecoJESUp/B");
  fMini1j1t->Branch("TDPhiLLJESUp",            &TDPhiLLJESUp,               "TDPhiLLJESUp/F");
  fMini1j1t->Branch("TDPhiLeadJetJESUp",       &TDPhiLeadJetJESUp,          "TDPhiLeadJetJESUp/F");
  fMini1j1t->Branch("TDPhiSubLeadJetJESUp",    &TDPhiSubLeadJetJESUp,       "TDPhiSubLeadJetJESUp/F");
  
  fMini1j1t->Branch("TMETJESDown",             &TMETJESDown,                "TMETJESDown/F");
  fMini1j1t->Branch("TMET_PhiJESDown",         &TMET_PhiJESDown,            "TMET_PhiJESDown/F");
  fMini1j1t->Branch("TM_LeadingBJESDown" ,     &TM_LeadingBJESDown,         "TM_LeadingBJESDown/F");
  fMini1j1t->Branch("TM_SubLeadingBJESDown",   &TM_SubLeadingBJESDown,      "TM_SubLeadingBJESDown/F");
  fMini1j1t->Branch("TE_LLBJESDown",           &TE_LLBJESDown,              "TE_LLBJESDown/F");
  fMini1j1t->Branch("TMT_LLMETBJESDown",       &TMT_LLMETBJESDown,          "TMT_LLMETBJESDown/F");
  fMini1j1t->Branch("TM_LLBJESDown",           &TM_LLBJESDown,              "TM_LLBJESDown/F");
  fMini1j1t->Branch("THTtotJESDown",           &THTtotJESDown,              "THTtotJESDown/F");
  fMini1j1t->Branch("TDilepPtJESDown",         &TDilepPtJESDown,            "TDilepPtJESDown/F");
  fMini1j1t->Branch("TLeadingJetPtJESDown",    &TLeadingJetPtJESDown,       "TLeadingJetPtJESDown/F");
  fMini1j1t->Branch("TLeadingJetEJESDown",     &TLeadingJetEJESDown,        "TLeadingJetEJESDown/F");
  fMini1j1t->Branch("TLeadingJetPhiJESDown",   &TLeadingJetPhiJESDown,      "TLeadingJetPhiJESDown/F");
  fMini1j1t->Branch("TLeadingJetEtaJESDown",   &TLeadingJetEtaJESDown,      "TLeadingJetEtaJESDown/F");
  fMini1j1t->Branch("TLeadingLepPtJESDown",    &TLeadingLepPtJESDown,       "TLeadingLepPtJESDown/F");
  fMini1j1t->Branch("TLeadingLepEJESDown",     &TLeadingLepEJESDown,        "TLeadingLepEJESDown/F");
  fMini1j1t->Branch("TLeadingLepPhiJESDown",   &TLeadingLepPhiJESDown,      "TLeadingLepPhiJESDown/F");
  fMini1j1t->Branch("TLeadingLepEtaJESDown",   &TLeadingLepEtaJESDown,      "TLeadingLepEtaJESDown/F");
  fMini1j1t->Branch("TSubLeadingLepPtJESDown", &TSubLeadingLepPtJESDown,    "TSubLeadingLepPtJESDown/F");
  fMini1j1t->Branch("TSubLeadingLepEJESDown",  &TSubLeadingLepEJESDown,     "TSubLeadingLepEJESDown/F");
  fMini1j1t->Branch("TSubLeadingLepPhiJESDown",&TSubLeadingLepPhiJESDown,   "TSubLeadingLepPhiJESDown/F");
  fMini1j1t->Branch("TSubLeadingLepEtaJESDown",&TSubLeadingLepEtaJESDown,   "TSubLeadingLepEtaJESDown/F");
  fMini1j1t->Branch("TpassrecoJESDown",        &TpassrecoJESDown,           "TpassrecoJESDown/B");
  fMini1j1t->Branch("TDPhiLLJESDown",          &TDPhiLLJESDown,             "TDPhiLLJESDown/F");
  fMini1j1t->Branch("TDPhiLeadJetJESDown",     &TDPhiLeadJetJESDown,        "TDPhiLeadJetJESDown/F");
  fMini1j1t->Branch("TDPhiSubLeadJetJESDown",  &TDPhiSubLeadJetJESDown,     "TDPhiSubLeadJetJESDown/F");
  
  fMini1j1t->Branch("TMETJERUp",               &TMETJERUp,                  "TMETJERUp/F");
  fMini1j1t->Branch("TMET_PhiJERUp",           &TMET_PhiJERUp,              "TMET_PhiJERUp/F");
  fMini1j1t->Branch("TM_LeadingBJERUp" ,       &TM_LeadingBJERUp,           "TM_LeadingBJERUp/F");
  fMini1j1t->Branch("TM_SubLeadingBJERUp",     &TM_SubLeadingBJERUp,        "TM_SubLeadingBJERUp/F");
  fMini1j1t->Branch("TE_LLBJERUp",             &TE_LLBJERUp,                "TE_LLBJERUp/F");
  fMini1j1t->Branch("TMT_LLMETBJERUp",         &TMT_LLMETBJERUp,            "TMT_LLMETBJERUp/F");
  fMini1j1t->Branch("TM_LLBJERUp",             &TM_LLBJERUp,                "TM_LLBJERUp/F");
  fMini1j1t->Branch("THTtotJERUp",             &THTtotJERUp,                "THTtotJERUp/F");
  fMini1j1t->Branch("TDilepPtJERUp",           &TDilepPtJERUp,              "TDilepPtJERUp/F");
  fMini1j1t->Branch("TLeadingJetPtJERUp",      &TLeadingJetPtJERUp,         "TLeadingJetPtJERUp/F");
  fMini1j1t->Branch("TLeadingJetEJERUp",       &TLeadingJetEJERUp,          "TLeadingJetEJERUp/F");
  fMini1j1t->Branch("TLeadingJetPhiJERUp",     &TLeadingJetPhiJERUp,        "TLeadingJetPhiJERUp/F");
  fMini1j1t->Branch("TLeadingJetEtaJERUp",     &TLeadingJetEtaJERUp,        "TLeadingJetEtaJERUp/F");
  fMini1j1t->Branch("TLeadingLepPtJERUp",      &TLeadingLepPtJERUp,         "TLeadingLepPtJERUp/F");
  fMini1j1t->Branch("TLeadingLepEJERUp",       &TLeadingLepEJERUp,          "TLeadingLepEJERUp/F");
  fMini1j1t->Branch("TLeadingLepPhiJERUp",     &TLeadingLepPhiJERUp,        "TLeadingLepPhiJERUp/F");
  fMini1j1t->Branch("TLeadingLepEtaJERUp",     &TLeadingLepEtaJERUp,        "TLeadingLepEtaJERUp/F");
  fMini1j1t->Branch("TSubLeadingLepPtJERUp",   &TSubLeadingLepPtJERUp,      "TSubLeadingLepPtJERUp/F");
  fMini1j1t->Branch("TSubLeadingLepEJERUp",    &TSubLeadingLepEJERUp,       "TSubLeadingLepEJERUp/F");
  fMini1j1t->Branch("TSubLeadingLepPhiJERUp",  &TSubLeadingLepPhiJERUp,     "TSubLeadingLepPhiJERUp/F");
  fMini1j1t->Branch("TSubLeadingLepEtaJERUp",  &TSubLeadingLepEtaJERUp,     "TSubLeadingLepEtaJERUp/F");
  fMini1j1t->Branch("TpassrecoJERUp",          &TpassrecoJERUp,             "TpassrecoJERUp/B");
  fMini1j1t->Branch("TDPhiLLJERUp",            &TDPhiLLJERUp,               "TDPhiLLJERUp/F");
  fMini1j1t->Branch("TDPhiLeadJetJERUp",       &TDPhiLeadJetJERUp,          "TDPhiLeadJetJERUp/F");
  fMini1j1t->Branch("TDPhiSubLeadJetJERUp",    &TDPhiSubLeadJetJERUp,       "TDPhiSubLeadJetJERUp/F");
  
  if (gPar.Contains("Unfolding")) {
    fMini1j1t->Branch("Tpassgen",                &Tpassgen,                   "Tpassgen/B");
    fMini1j1t->Branch("TnSergioJets",            &nSergioJets,                "TnSergioJets/I");
    fMini1j1t->Branch("TnSergiobJets",           &nSergiobJets,               "TnSergiobJets/I");
    fMini1j1t->Branch("TnSergioLooseCentralJets",&nSergioLooseCentralJets,    "TnSergioLooseCentralJets/I");
    fMini1j1t->Branch("TnSergioLooseCentralbJets",&nSergioLooseCentralbJets,  "TnSergioLooseCentralbJets/I");
    fMini1j1t->Branch("TnSergioLooseFwdJets",    &nSergioLooseFwdJets,        "TnSergioLooseFwdJets/I");
    fMini1j1t->Branch("TGenChannel",             &GenChannel,                 "GenChannel/I");
    fMini1j1t->Branch("TGenIsSS",                &TGenIsSS,                   "TGenIsSS/B");
    fMini1j1t->Branch("TGenMET",                 &TGenMET,                    "TGenMET/F");
    fMini1j1t->Branch("TGenMET_Phi",             &TGenMET_Phi,                "TGenMET_Phi/F");
    fMini1j1t->Branch("TGenM_LeadingB" ,         &TGenM_LeadingB,             "TGenM_LeadingB/F");
    fMini1j1t->Branch("TGenM_SubLeadingB",       &TGenM_SubLeadingB,          "TGenM_SubLeadingB/F");
    fMini1j1t->Branch("TGenE_LLB",               &TGenE_LLB,                  "TGenE_LLB/F");
    fMini1j1t->Branch("TGenM_LeadingBj2" ,       &TGenM_LeadingBj2,           "TGenM_LeadingBj2/F");
    fMini1j1t->Branch("TGenM_SubLeadingBj2",     &TGenM_SubLeadingBj2,        "TGenM_SubLeadingBj2/F");
    fMini1j1t->Branch("TGenM_bjetlepton_minmax", &TGenM_bjetlepton_minmax,    "TGenM_bjetlepton_minmax/F");
    fMini1j1t->Branch("TGenMT_LLMETB",           &TGenMT_LLMETB,              "TGenMT_LLMETB/F");
    fMini1j1t->Branch("TGenM_LLB",               &TGenM_LLB,                  "TGenM_LLB/F");
    fMini1j1t->Branch("TGenDilepPt",             &TGenDilepPt,                "TGenDilepPt/F");
    fMini1j1t->Branch("TGenDilepJetPt",          &TGenDilepJetPt,             "TGenDilepJetPt/F");
    fMini1j1t->Branch("TGenDilepMETJetPt",       &TGenDilepMETJetPt,          "TGenDilepMETJetPt/F");
    fMini1j1t->Branch("TGenHTtot",               &TGenHTtot,                  "TGenHTtot/F");
    fMini1j1t->Branch("TGenLeadingJetPt",        &TGenLeadingJetPt,           "TGenLeadingJetPt/F");
    fMini1j1t->Branch("TGenLeadingJetE",         &TGenLeadingJetE,            "TGenLeadingJetE/F");
    fMini1j1t->Branch("TGenLeadingJetPhi",       &TGenLeadingJetPhi,          "TGenLeadingJetPhi/F");
    fMini1j1t->Branch("TGenLeadingJetEta",       &TGenLeadingJetEta,          "TGenLeadingJetEta/F");
    fMini1j1t->Branch("TGenSubLeadingLooseCentralJetPt", &TGenSubLeadingLooseCentralJetPt, "TGenSubLeadingLooseCentralJetPt/F");
    fMini1j1t->Branch("TGenSubLeadingLooseCentralJetE",  &TGenSubLeadingLooseCentralJetE,  "TGenSubLeadingLooseCentralJetE/F");
    fMini1j1t->Branch("TGenSubLeadingLooseCentralJetPhi",&TGenSubLeadingLooseCentralJetPhi,"TGenSubLeadingLooseCentralJetPhi/F");
    fMini1j1t->Branch("TGenSubLeadingLooseCentralJetEta",&TGenSubLeadingLooseCentralJetEta,"TGenSubLeadingLooseCentralJetEta/F");
    fMini1j1t->Branch("TGenLeadingLooseFwdJetPt", &TGenLeadingLooseFwdJetPt,  "TGenLeadingLooseFwdJetPt/F");
    fMini1j1t->Branch("TGenLeadingLooseFwdJetE",  &TGenLeadingLooseFwdJetE,   "TGenLeadingLooseFwdJetE/F");
    fMini1j1t->Branch("TGenLeadingLooseFwdJetPhi",&TGenLeadingLooseFwdJetPhi, "TGenLeadingLooseFwdJetPhi/F");
    fMini1j1t->Branch("TGenLeadingLooseFwdJetEta",&TGenLeadingLooseFwdJetEta, "TGenLeadingLooseFwdJetEta/F");
    fMini1j1t->Branch("TGenLeadingLepPt",        &TGenLeadingLepPt,           "TGenLeadingLepPt/F");
    fMini1j1t->Branch("TGenLeadingLepE",         &TGenLeadingLepE,            "TGenLeadingLepE/F");
    fMini1j1t->Branch("TGenLeadingLepPhi",       &TGenLeadingLepPhi,          "TGenLeadingLepPhi/F");
    fMini1j1t->Branch("TGenLeadingLepEta",       &TGenLeadingLepEta,          "TGenLeadingLepEta/F");
    fMini1j1t->Branch("TGenSubLeadingLepPt",     &TGenSubLeadingLepPt,        "TGenSubLeadingLepPt/F");
    fMini1j1t->Branch("TGenSubLeadingLepE",      &TGenSubLeadingLepE,         "TGenSubLeadingLepE/F");
    fMini1j1t->Branch("TGenSubLeadingLepPhi",    &TGenSubLeadingLepPhi,       "TGenSubLeadingLepPhi/F");
    fMini1j1t->Branch("TGenSubLeadingLepEta",    &TGenSubLeadingLepEta,       "TGenSubLeadingLepEta/F");
    fMini1j1t->Branch("TGenDilepMETJet1Pz",      &TGenDilepMETJet1Pz,         "TGenDilepMETJet1Pz/F");
    fMini1j1t->Branch("TGenLLMETBEta",           &TGenLLMETBEta,              "TGenLLMETBEta/F");
    fMini1j1t->Branch("TGenMSys",                &TGenMSys,                   "TGenMSys/F");
    fMini1j1t->Branch("TGenMll",                 &TGenMll,                    "TGenMll/F");
    fMini1j1t->Branch("TGenDPhiLL",              &TGenDPhiLL,                 "TGenDPhiLL/F");
    fMini1j1t->Branch("TGenDPhiLeadJet",         &TGenDPhiLeadJet,            "TGenDPhiLeadJet/F");
    fMini1j1t->Branch("TGenDPhiSubLeadJet",      &TGenDPhiSubLeadJet,         "TGenDPhiSubLeadJet/F");
  }
}


void TWTTbarAnalysis::ReSetTWTTbarVariables() {
  hasTW = false;
  
  DilepMETJetPt  = -99;
  Lep1METJetPt   = -99;
  DPtDilep_JetMET= -99;
  DPtDilep_MET   = -99;
  DPtLep1_MET    = -99;
  DilepMETJet1Pz = -99;
  DilepMETJet1PzJESUp = -99;
  DilepMETJet1PzJESDown = -99;
  DilepMETJet1PzJERUp = -99;
  nLooseCentral  = -99;
  nLooseCentralJESUp  = -99;
  nLooseCentralJESDown  = -99;
  nLooseCentralJERUp  = -99;
  nLooseFwd        = -99;
  nLooseFwdJESUp   = -99;
  nLooseFwdJESDown = -99;
  nLooseFwdJERUp   = -99;
  nBLooseCentral = -99;
  nBLooseCentralJESUp = -99;
  nBLooseFwdJESDown   = -99;
  nBLooseCentralJERUp = -99;
  TJet2csv       = -99;
  MSys           = -99;
  MSysJESUp      = -99;
  MSysJESDown    = -99;
  MSysJERUp      = -99;
  TMll           = -99;
  TMllJESUp      = -99;
  TMllJESDown    = -99;
  TMllJERUp      = -99;
  TJetLoosept    = -99;
  TJetLooseCentralpt = -99;
  C_jll          = -99;
  DilepJetPt     = -99;
  
  // ADDED
  TLeadingJetE    = -99;
  TLeadingJetPt   = -99;
  TLeadingJetPhi  = -99;
  TLeadingJetEta  = -99;
  TLeadingJetEJESUp    = -99;
  TLeadingJetPtJESUp   = -99;
  TLeadingJetPhiJESUp  = -99;
  TLeadingJetEtaJESUp  = -99;
  TLeadingJetEJESDown    = -99;
  TLeadingJetPtJESDown   = -99;
  TLeadingJetPhiJESDown  = -99;
  TLeadingJetEtaJESDown  = -99;
  TLeadingJetEJERUp    = -99;
  TLeadingJetPtJERUp   = -99;
  TLeadingJetPhiJERUp  = -99;
  TLeadingJetEtaJERUp  = -99;
  TLeadingLepE    = -99;
  TLeadingLepPt   = -99;
  TLeadingLepPhi  = -99;
  TLeadingLepEta  = -99;
  TLeadingLepEJESUp    = -99;
  TLeadingLepPtJESUp   = -99;
  TLeadingLepPhiJESUp  = -99;
  TLeadingLepEtaJESUp  = -99;
  TLeadingLepEJESDown    = -99;
  TLeadingLepPtJESDown   = -99;
  TLeadingLepPhiJESDown  = -99;
  TLeadingLepEtaJESDown  = -99;
  TLeadingLepEJERUp    = -99;
  TLeadingLepPtJERUp   = -99;
  TLeadingLepPhiJERUp  = -99;
  TLeadingLepEtaJERUp  = -99;
  TSubLeadingLepE    = -99;
  TSubLeadingLepPt   = -99;
  TSubLeadingLepPhi  = -99;
  TSubLeadingLepEta  = -99;
  TSubLeadingLepEJESUp    = -99;
  TSubLeadingLepPtJESUp   = -99;
  TSubLeadingLepPhiJESUp  = -99;
  TSubLeadingLepEtaJESUp  = -99;
  TSubLeadingLepEJESDown    = -99;
  TSubLeadingLepPtJESDown   = -99;
  TSubLeadingLepPhiJESDown  = -99;
  TSubLeadingLepEtaJESDown  = -99;
  TSubLeadingLepEJERUp    = -99;
  TSubLeadingLepPtJERUp   = -99;
  TSubLeadingLepPhiJERUp  = -99;
  TSubLeadingLepEtaJERUp  = -99;
  TDilepPt        = -99;
  TM_LeadingB     = -99;
  TM_SubLeadingB  = -99;
  TM_LeadingBj2        = -99; //nueva
  TM_SubLeadingBj2     = -99; //nueva 
  TM_bjetlepton_minmax = -99; //variable atlas
  TE_LLB          = -99;
  TMT_LLMETB      = -99;
  TM_LLB          = -99;
  TLLMETBEta      = -99;
  TM_LeadingBJESUp     = -99;
  TM_SubLeadingBJESUp  = -99;
  TE_LLBJESUp          = -99;
  TMT_LLMETBJESUp      = -99;
  TM_LLBJESUp          = -99;
  TDilepPtJESUp        = -99;
  TLLMETBEtaJESUp      = -99;
  TM_LeadingBJESDown     = -99;
  TM_SubLeadingBJESDown  = -99;
  TE_LLBJESDown          = -99;
  TMT_LLMETBJESDown      = -99;
  TM_LLBJESDown          = -99;
  TDilepPtJESDown        = -99;
  TLLMETBEtaJESDown      = -99;
  TM_LeadingBJERUp     = -99;
  TM_SubLeadingBJERUp  = -99;
  TE_LLBJERUp          = -99;
  TMT_LLMETBJERUp      = -99;
  TM_LLBJERUp          = -99;
  TDilepPtJERUp        = -99;
  TLLMETBEtaJERUp      = -99;
  
  TDPhiLL              = -99.;
  TDPhiLLJESUp         = -99.;
  TDPhiLLJESDown       = -99.;
  TDPhiLLJERUp         = -99.;
  TDPhiLeadJet         = -99.;
  TDPhiLeadJetJESUp    = -99.;
  TDPhiLeadJetJESDown  = -99.;
  TDPhiLeadJetJERUp    = -99.;
  TDPhiSubLeadJet      = -99.;
  TDPhiSubLeadJetJESUp = -99.;
  TDPhiSubLeadJetJESDown= -99.;
  TDPhiSubLeadJetJERUp = -99.;
  
  TGenLeadingJetE    = -99;
  TGenLeadingJetPt   = -99;
  TGenLeadingJetPhi  = -99;
  TGenLeadingJetEta  = -99;
  TGenLeadingLooseFwdJetPt   = -99.;
  TGenLeadingLooseFwdJetE    = -99.;
  TGenLeadingLooseFwdJetPhi  = -99.;
  TGenLeadingLooseFwdJetEta  = -99.;
  TGenSubLeadingLooseCentralJetPt  = -99.;
  TGenSubLeadingLooseCentralJetE   = -99.;
  TGenSubLeadingLooseCentralJetPhi = -99.;
  TGenSubLeadingLooseCentralJetEta = -99.;
  TGenLeadingLepE    = -99;
  TGenLeadingLepPt   = -99;
  TGenLeadingLepPhi  = -99;
  TGenLeadingLepEta  = -99;
  TGenSubLeadingLepE    = -99;
  TGenSubLeadingLepPt   = -99;
  TGenSubLeadingLepPhi  = -99;
  TGenSubLeadingLepEta  = -99;
  TGenM_LeadingB     = -99;
  TGenM_SubLeadingB  = -99;
  TGenE_LLB          = -99;
  TGenM_LeadingBj2   = -99;
  TGenM_SubLeadingBj2= -99;
  TGenM_bjetlepton_minmax=-99;
  TGenMT_LLMETB      = -99;
  TGenM_LLB          = -99;
  TGenDilepJetPt     = -99;
  TGenDilepMETJetPt  = -99;
  TGenHTtot          = -99;
  TGenDilepMETJet1Pz = -99;
  TGenLLMETBEta      = -99;
  TGenMSys           = -99;
  TGenDPhiLL         = -99.;
  TGenDPhiLeadJet    = -99.;
  TGenDPhiSubLeadJet = -99.;
  Tpassreco          = 0;
  TpassrecoJESUp     = 0;
  TpassrecoJESDown   = 0;
  TpassrecoJERUp     = 0;
  Tpassgen           = 0;
  TGenIsSS           = 0;
  SergioJets.clear();
  SergioLooseCentralJets.clear();
  SergioLooseFwdJets.clear();
  SergioLeps.clear();
  SergioMET.SetXYZT(0, 0, 0, 0);
  tpL.SetXYZT(0, 0, 0, 0);
  tpJ.SetXYZT(0, 0, 0, 0);
  tMET.SetXYZT(0, 0, 0, 0);
  TIsFid          = false;
  GenChannel      = -1;
  TGenMET         = -99;
  TGenMET_Phi     = -99;
  nSergioJets     = -99;
  nSergiobJets    = -99;
  nSergioLooseCentralJets= -99;
  nSergioLooseCentralbJets= -99;
  nSergioLooseFwdJets= -99;
  nSergioLeps     = -99;
  nSergioGenJets  = -99;
  nSergioGenLeps  = -99;
  nSergioGenMET   = -99;
  TMET            = -99;
  TMET_Phi        = -99;
  TMETJESUp       = -99;
  TMET_PhiJESUp   = -99;
  TMETJESDown     = -99;
  TMET_PhiJESDown = -99;
  TMETJERUp       = -99;
  TMET_PhiJERUp   = -99;
  TWeight_normal  = -99;
}


void TWTTbarAnalysis::CalculateTWTTbarVariables() {
  if (hasTW) return;
  hasTW = true;

  get20Jets();
  TLorentzVector met;
  if (TNJets == 2 && TNBtags == 2) {
    // cout << "1j1b NOM"<< endl;
    met.SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    
    DilepMETJetPt     =  getDilepMETJetPt()   ;
    DilepJetPt        =  getDilepJetPt()      ;
    Lep1METJetPt      =  getLep1METJetPt()    ;
    DPtDilep_JetMET   =  getDPtDilep_JetMET() ;
    DPtDilep_MET      =  getDPtDilep_MET()    ;
    DPtLep1_MET       =  getDPtLep1_MET()     ;
    DilepMETJet1Pz    =  getDilepMETJet1Pz()  ;

    C_jll = (selJets.at(0).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et()) / (selJets.at(0).p.E() + selLeptons.at(0).p.E() + selLeptons.at(1).p.E());
    TJet1_pt          = selJets.at(0).Pt();
    MSys              = getSysM();
    THTtot            = getHTtot();
    DilepmetjetOverHT = DilepMETJetPt/THTtot;
    HTLepOverHT       = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/THTtot;
    TDilepPt          = (selLeptons.at(0).p + selLeptons.at(1).p).Pt();
    TMll              = (selLeptons.at(0).p + selLeptons.at(1).p).M();
    
    TM_LeadingB       = (selJets.at(0).p + selLeptons.at(0).p).M(); //M(b1,l1)
    TM_SubLeadingB    = (selJets.at(0).p + selLeptons.at(1).p).M(); //M(b1,l2)
    //nuevas variables para la variable de ATLAS
    TM_LeadingBj2     = (selJets.at(1).p + selLeptons.at(0).p).M(); //M(b2,l1)
    TM_SubLeadingBj2  = (selJets.at(1).p + selLeptons.at(1).p).M(); //M(b2,l2)
    if(GreaterThan(TM_LeadingB,TM_SubLeadingBj2)) {
        if(GreaterThan(TM_SubLeadingB,TM_LeadingBj2)) {
            if(!GreaterThan(TM_LeadingB,TM_SubLeadingB))       {TM_bjetlepton_minmax = TM_LeadingB;}
            else                                               {TM_bjetlepton_minmax = TM_SubLeadingB;}
        }
        else {
            if(!GreaterThan(TM_LeadingB,TM_LeadingBj2))       {TM_bjetlepton_minmax = TM_LeadingB; }
            else                                               {TM_bjetlepton_minmax = TM_LeadingBj2;}
        }
    }
    else {
        if(GreaterThan(TM_SubLeadingB,TM_LeadingBj2)) {
            if(!GreaterThan(TM_SubLeadingBj2,TM_SubLeadingB)) {TM_bjetlepton_minmax = TM_SubLeadingBj2;}
            else                                               {TM_bjetlepton_minmax = TM_SubLeadingB;}
        }
        else {
            if(!GreaterThan(TM_SubLeadingBj2,TM_LeadingBj2)) {TM_bjetlepton_minmax = TM_SubLeadingBj2;}
            else                                               {TM_bjetlepton_minmax = TM_LeadingBj2;}
        }  
    }
    TE_LLB            = (selJets.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).E();
    TMT_LLMETB        = (selJets.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Mt();
    TM_LLB            = (selJets.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).M();
    TLLMETBEta        = (selJets.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Eta();
    TDPhiLL           = getDPhiLep1_Lep2();
    TDPhiLeadJet      = getDPhiLep1_Jet1();
    TDPhiSubLeadJet   = getDPhiLep2_Jet1();
    
  }
  else {
    DilepMETJetPt    = -99.;
    DilepJetPt       = -99.;
    Lep1METJetPt     = -99.;
    DPtDilep_JetMET  = -99.;
    DPtDilep_MET     = -99.;
    DPtLep1_MET      = -99.;
    DilepMETJet1Pz   = -99.;
    C_jll            = -99.;
    TJet1_pt         = -99.;
    DilepmetjetOverHT= -99.;
    HTLepOverHT      = -99.;
    MSys             = -99.;
    THTtot           = -99.;
    TDilepPt         = -99.;
    TMll             = -99.;
    TM_LeadingB      = -99.;
    TM_SubLeadingB   = -99.;
    TE_LLB           = -99.;
    TM_LeadingBj2    = -99.;
    TM_SubLeadingBj2 = -99.;
    TM_bjetlepton_minmax= -99.;
    TMT_LLMETB       = -99.;
    TM_LLB           = -99.;
    TLLMETBEta       = -99.;
    TDPhiLL          = -99.;
    TDPhiLeadJet     = -99.;
    TDPhiSubLeadJet  = -99.;
  }

  if (!gIsData) {
    if (TNJetsJESUp == 1 && TNBtagsJESUp == 1) {
      // cout << "1j1b Up" << endl;
      met.SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp);
      
      DilepMETJetPtJESUp   =  getDilepMETJetPt(1)   ;
      DilepJetPtJESUp      =  getDilepJetPt("JESUp")      ;
      Lep1METJetPtJESUp    =  getLep1METJetPt("JESUp")    ;
      DPtDilep_JetMETJESUp =  getDPtDilep_JetMET("JESUp") ;
      DPtDilep_METJESUp    =  getDPtDilep_MET("JESUp")    ;
      DPtLep1_METJESUp     =  getDPtLep1_MET("JESUp")     ;
      DilepMETJet1PzJESUp  =  getDilepMETJet1Pz("JESUp")  ;
      
      C_jllJESUp = (selJetsJecUp.at(0).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et()) / (selJetsJecUp.at(0).p.E() + selLeptons.at(0).p.E() + selLeptons.at(1).p.E());
      TJet1_ptJESUp           = selJetsJecUp.at(0).Pt();
      MSysJESUp               = getSysM("JESUp");
      THTtotJESUp             = getHTtot("JESUp");
      DilepmetjetOverHTJESUp  = DilepMETJetPtJESUp/THTtotJESUp          ;
      TDilepPtJESUp           = (selLeptons.at(0).p + selLeptons.at(1).p).Pt();
      HTLepOverHTJESUp        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/THTtotJESUp    ;
      
      TM_LeadingBJESUp        = (selJetsJecUp.at(0).p + selLeptons.at(0).p).M();
      TM_SubLeadingBJESUp     = (selJetsJecUp.at(0).p + selLeptons.at(1).p).M();
      TE_LLBJESUp             = (selJetsJecUp.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).E();
      TMT_LLMETBJESUp         = (selJetsJecUp.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Mt();
      TM_LLBJESUp             = (selJetsJecUp.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).M();
      TLLMETBEtaJESUp         = (selJetsJecUp.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Eta();
      TMllJESUp               = (selLeptons.at(0).p + selLeptons.at(1).p).M();
      TDPhiLLJESUp            = getDPhiLep1_Lep2();
      TDPhiLeadJetJESUp       = getDPhiLep1_Jet1("JESUp");
      TDPhiSubLeadJetJESUp    = getDPhiLep2_Jet1("JESUp");
    }
    else {
      DilepMETJetPtJESUp    = -99.;
      DilepJetPtJESUp       = -99.;
      Lep1METJetPtJESUp     = -99.;
      DPtDilep_JetMETJESUp  = -99.;
      DPtDilep_METJESUp     = -99.;
      DPtLep1_METJESUp      = -99.;
      DilepMETJet1PzJESUp   = -99.;
      C_jllJESUp            = -99.;
      TJet1_ptJESUp         = -99.;
      DilepmetjetOverHTJESUp= -99.;
      HTLepOverHTJESUp      = -99.;
      MSysJESUp             = -99.;
      TDilepPtJESUp         = -99.;
      THTtotJESUp           = -99.;
      TM_LeadingBJESUp      = -99.;
      TM_SubLeadingBJESUp   = -99.;
      TE_LLBJESUp           = -99.;
      TMT_LLMETBJESUp       = -99.;
      TM_LLBJESUp           = -99.;
      TLLMETBEtaJESUp       = -99.;
      TMllJESUp             = -99.;
      TDPhiLLJESUp          = -99.;
      TDPhiLeadJetJESUp     = -99.;
      TDPhiSubLeadJetJESUp  = -99.;
    }
    
    if (TNJetsJESDown == 1 && TNBtagsJESDown == 1) {
      // cout << "1j1b Down"<< endl;
      met.SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown);
      
      DilepMETJetPtJESDown   =  getDilepMETJetPt(-1)   ;
      DilepJetPtJESDown      =  getDilepJetPt("JESDown")      ;
      Lep1METJetPtJESDown    =  getLep1METJetPt("JESDown")    ;
      DPtDilep_JetMETJESDown =  getDPtDilep_JetMET("JESDown") ;
      DPtDilep_METJESDown    =  getDPtDilep_MET("JESDown")    ;
      DPtLep1_METJESDown     =  getDPtLep1_MET("JESDown")     ;
      DilepMETJet1PzJESDown  =  getDilepMETJet1Pz("JESDown")  ;
      
      C_jllJESDown = (selJetsJecDown.at(0).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et()) / (selJetsJecDown.at(0).p.E() + selLeptons.at(0).p.E() + selLeptons.at(1).p.E());
      TJet1_ptJESDown           = selJetsJecDown.at(0).p.Pt();
      THTtotJESDown             = getHTtot("JESDown");
      DilepmetjetOverHTJESDown  = DilepMETJetPtJESDown/THTtotJESDown          ;
      TDilepPtJESDown           = (selLeptons.at(0).p + selLeptons.at(1).p).Pt();
      HTLepOverHTJESDown        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/ THTtotJESDown    ;
      MSysJESDown               = getSysM("JESDown");
      
      TM_LeadingBJESDown        = (selJetsJecDown.at(0).p + selLeptons.at(0).p).M();
      TM_SubLeadingBJESDown     = (selJetsJecDown.at(0).p + selLeptons.at(1).p).M();
      TE_LLBJESDown             = (selJetsJecDown.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).E();
      TMT_LLMETBJESDown         = (selJetsJecDown.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Mt();
      TM_LLBJESDown             = (selJetsJecDown.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).M();
      TLLMETBEtaJESDown         = (selJetsJecDown.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Eta();
      TMllJESDown               = (selLeptons.at(0).p + selLeptons.at(1).p).M();
      TDPhiLLJESDown            = getDPhiLep1_Lep2();
      TDPhiLeadJetJESDown       = getDPhiLep1_Jet1("JESDown");
      TDPhiSubLeadJetJESDown    = getDPhiLep2_Jet1("JESDown");
    }
    else {
      DilepMETJetPtJESDown    = -99.;
      DilepJetPtJESDown       = -99.;
      Lep1METJetPtJESDown     = -99.;
      DPtDilep_JetMETJESDown  = -99.;
      DPtDilep_METJESDown     = -99.;
      DPtLep1_METJESDown      = -99.;
      DilepMETJet1PzJESDown   = -99.;
      C_jllJESDown            = -99.;
      TJet1_ptJESDown         = -99.;
      DilepmetjetOverHTJESDown= -99.;
      HTLepOverHTJESDown      = -99.;
      MSysJESDown             = -99.;
      TDilepPtJESDown         = -99.;
      THTtotJESDown           = -99.;
      TM_LeadingBJESDown      = -99.;
      TM_SubLeadingBJESDown   = -99.;
      TE_LLBJESDown           = -99.;
      TMT_LLMETBJESDown       = -99.;
      TM_LLBJESDown           = -99.;
      TLLMETBEtaJESDown       = -99.;
      TMllJESDown             = -99.;
      TDPhiLLJESDown          = -99.;
      TDPhiLeadJetJESDown     = -99.;
      TDPhiSubLeadJetJESDown  = -99.;
    }
    
    if (TNJetsJERUp == 1 && TNBtagsJERUp == 1) {
      met.SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp);
      
      DilepMETJetPtJERUp   =  getDilepMETJetPt(-2)   ; // -2 is JERUp :D
      DilepJetPtJERUp      =  getDilepJetPt("JER")      ;
      Lep1METJetPtJERUp    =  getLep1METJetPt("JER")    ;
      DPtDilep_JetMETJERUp =  getDPtDilep_JetMET("JER") ;
      DPtDilep_METJERUp    =  getDPtDilep_MET("JER")    ;
      DPtLep1_METJERUp     =  getDPtLep1_MET("JER")     ;
      DilepMETJet1PzJERUp  =  getDilepMETJet1Pz("JER")  ;
      
      C_jllJERUp = (selJetsJER.at(0).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et()) / (selJetsJER.at(0).p.E() + selLeptons.at(0).p.E() + selLeptons.at(1).p.E());
      TJet1_ptJERUp           = selJetsJER.at(0).p.Pt();
      THTtotJERUp             = getHTtot("JER");
      HTLepOverHTJERUp        = (selLeptons.at(0).p.Pt() + selLeptons.at(1).p.Pt())/ THTtotJERUp    ;
      TDilepPtJERUp           = (selLeptons.at(0).p + selLeptons.at(1).p).Pt();
      DilepmetjetOverHTJERUp  = DilepMETJetPtJERUp/THTtotJERUp          ;
      MSysJERUp               = getSysM("JER");

      TM_LeadingBJERUp        = (selJetsJER.at(0).p + selLeptons.at(0).p).M();
      TM_SubLeadingBJERUp     = (selJetsJER.at(0).p + selLeptons.at(1).p).M();
      TE_LLBJERUp             = (selJetsJER.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).E();
      TMT_LLMETBJERUp         = (selJetsJER.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Mt();
      TM_LLBJERUp             = (selJetsJER.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p).M();
      TLLMETBEtaJERUp         = (selJetsJER.at(0).p + selLeptons.at(0).p + selLeptons.at(1).p + met).Eta();
      TMllJERUp               = (selLeptons.at(0).p + selLeptons.at(1).p).M();
      TDPhiLLJERUp            = getDPhiLep1_Lep2();
      TDPhiLeadJetJERUp       = getDPhiLep1_Jet1("JER");
      TDPhiSubLeadJetJERUp    = getDPhiLep2_Jet1("JER");
    }
    else {
      DilepMETJetPtJERUp    = -99.;
      DilepJetPtJERUp       = -99.;
      Lep1METJetPtJERUp     = -99.;
      DPtDilep_JetMETJERUp  = -99.;
      DPtDilep_METJERUp     = -99.;
      DPtLep1_METJERUp      = -99.;
      DilepMETJet1PzJERUp   = -99.;
      C_jllJERUp            = -99.;
      TJet1_ptJERUp         = -99.;
      DilepmetjetOverHTJERUp= -99.;
      HTLepOverHTJERUp      = -99.;
      MSysJERUp             = -99.;
      THTtotJERUp           = -99.;
      TDilepPtJERUp         = -99.;
      TM_LeadingBJERUp      = -99.;
      TM_SubLeadingBJERUp   = -99.;
      TE_LLBJERUp           = -99.;
      TMT_LLMETBJERUp       = -99.;
      TM_LLBJERUp           = -99.;
      TLLMETBEtaJERUp       = -99.;
      TMllJERUp             = -99.;
      TDPhiLLJERUp          = -99.;
      TDPhiLeadJetJERUp     = -99.;
      TDPhiSubLeadJetJERUp  = -99.;
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
  }

  if (!gIsData){
    if (TNJetsJESUp == 2 && TNBtagsJESUp == 1){
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
    }
    
    if (TNJetsJESDown == 2 && TNBtagsJESDown == 1){
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
    }

    if (TNJetsJERUp == 2 && TNBtagsJERUp == 1){
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
    }
  }

  if (TNJets > 1)        TJet2_Pt        = selJets.at(1).Pt();
  else                   TJet2_Pt        = -99.;

  if (!gIsData){
    if (TNJetsJESUp > 1)   TJet2_PtJESUp   = selJetsJecUp.at(1).Pt();
    else                   TJet2_PtJESUp   = -99.;
    if (TNJetsJESDown > 1) TJet2_PtJESDown = selJetsJecDown.at(1).Pt();
    else                   TJet2_PtJESDown = -99.;
    if (TNJetsJERUp > 1)   TJet2_PtJERUp   = selJetsJER.at(1).Pt();
    else                   TJet2_PtJERUp   = -99.;
  }
  return;
}


void TWTTbarAnalysis::get20Jets() {
  vector<float> looseJetPt;
  vector<float> looseJetPtJESUp;
  vector<float> looseJetPtJESDown;
  vector<float> looseJetPtJER;

  vector<float> looseJetCentralPt;
  vector<float> looseJetCentralPtJESUp;
  vector<float> looseJetCentralPtJESDown;
  vector<float> looseJetCentralPtJER;
  
  vector<float> looseJetFwdPt;
  vector<float> looseJetFwdPtJESUp;
  vector<float> looseJetFwdPtJESDown;
  vector<float> looseJetFwdPtJER;
  
  nLooseCentral  = 0.; nLooseCentralJESUp  = 0.; nLooseCentralJESDown  = 0.; nLooseCentralJERUp  = 0.;
  nLooseFwd      = 0.; nLooseFwdJESUp      = 0.; nLooseFwdJESDown      = 0.; nLooseFwdJERUp      = 0.;
  nBLooseCentral = 0.; nBLooseCentralJESUp = 0.; nBLooseCentralJESDown = 0.; nBLooseCentralJERUp = 0.;
  TJet2csv       = 0.; TJet2csvJESUp       = 0.; TJet2csvJESDown       = 0.; TJet2csvJERUp       = 0.;
  TJetLoosept    = 0.; TJetLooseptJESUp    = 0.; TJetLooseptJESDown    = 0.; TJetLooseptJERUp    = 0.;
  TJetLooseCentralpt        = 0.; TJetLooseCentralptJESUp = 0.;
  TJetLooseCentralptJESDown = 0.; TJetLooseCentralptJERUp = 0.;
  TJetLooseFwdpt        = 0.; TJetLooseFwdptJESUp = 0.;
  TJetLooseFwdptJESDown = 0.; TJetLooseFwdptJERUp = 0.;

  for (unsigned int j = 0; j < vetoJets.size(); ++j){
    if (vetoJets.at(j).p.Pt() > 20.){
      looseJetPt.push_back( vetoJets.at(j).p.Pt() );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
        nLooseCentral++;
        looseJetCentralPt.push_back(vetoJets.at(j).p.Pt());
      }
      else {
        nLooseFwd++;
        looseJetFwdPt.push_back(vetoJets.at(j).p.Pt());
      }
      if (vetoJets.at(j).isBtag){
    if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentral++;
      }
    }

    if (vetoJets.at(j).pTJESUp > 20.){
      looseJetPtJESUp.push_back( vetoJets.at(j).pTJESUp );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
        nLooseCentralJESUp++;
        looseJetCentralPtJESUp.push_back(vetoJets.at(j).pTJESUp);
      }
      else {
        nLooseFwdJESUp++;
        looseJetFwdPtJESUp.push_back(vetoJets.at(j).pTJESUp);
      }
      if (vetoJets.at(j).isBtag){
        if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJESUp++;
      }
    }

    if (vetoJets.at(j).pTJESDown > 20.){
      looseJetPtJESDown.push_back( vetoJets.at(j).pTJESDown );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
        nLooseCentralJESDown++;
        looseJetCentralPtJESDown.push_back(vetoJets.at(j).pTJESDown);
      }
      else {
        nLooseFwdJESDown++;
        looseJetFwdPtJESDown.push_back(vetoJets.at(j).pTJESDown);
      }
      if (vetoJets.at(j).isBtag){
        if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJESDown++;
      }
    }

    if (vetoJets.at(j).pTJERUp > 20.){
      looseJetPtJER.push_back( vetoJets.at(j).pTJERUp );
      if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4){
        nLooseCentralJERUp++;
        looseJetCentralPtJER.push_back(vetoJets.at(j).pTJERUp);
      }
      else {
        nLooseFwdJERUp++;
        looseJetFwdPtJER.push_back(vetoJets.at(j).pTJERUp);
      }
      if (vetoJets.at(j).isBtag){
        if (TMath::Abs(vetoJets.at(j).p.Eta()) < 2.4) nBLooseCentralJERUp++;
      }
    }
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
    TJet2csv = TMath::Max( vetoJets.at(1).csv   , (Float_t) 0.);
    TJetLoosept = looseJetPt.at(1);
  }
  if (nLooseFwdJESUp + nLooseCentralJESUp > 1){
    TJet2csvJESUp = TMath::Max( vetoJets.at(1).csv   , (Float_t) 0.);
    TJetLooseptJESUp = looseJetPtJESUp.at(1);
  }
  if (nLooseFwdJESDown + nLooseCentralJESDown > 1){
    TJet2csvJESDown = TMath::Max( vetoJets.at(1).csv   , (Float_t) 0.);
    TJetLooseptJESDown = looseJetPtJESDown.at(1);
  }
  if (nLooseFwdJERUp + nLooseCentralJERUp > 1){
    TJet2csvJERUp = TMath::Max( vetoJets.at(1).csv   , (Float_t) 0.);
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

  if (nLooseFwd > 1)
    TJetLooseFwdpt = looseJetFwdPt.at(1);

  if (nLooseFwdJESUp > 1)
    TJetLooseFwdptJESUp = looseJetFwdPtJESUp.at(1);

  if (nLooseFwdJESDown > 1)
    TJetLooseFwdptJESDown = looseJetFwdPtJESDown.at(1);

  if (nLooseFwdJERUp > 1)
    TJetLooseFwdptJERUp = looseJetFwdPtJER.at(1);

  return;
}


Double_t TWTTbarAnalysis::getHTtot(const TString& sys) {
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


Double_t TWTTbarAnalysis::getHTtot2j(const TString& sys) {
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


Double_t TWTTbarAnalysis::getDilepMETJetPt(int sys) {
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
    cout << "Wrong systematic in TWTTbarAnalysis::getDilepMETJetPt" << endl;
    return -9999.;
  }
  return getPtSys(vSystem,4);
}


Double_t TWTTbarAnalysis::getDilepMETPt(int sys) {
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
    cout << "Wrong systematic in TWTTbarAnalysis::getDilepMETJetPt" << endl;
    return -9999.;
  }

  return getPtSys(vSystem,3);
}


Double_t TWTTbarAnalysis::getDilepJetPt(const TString& sys) {
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons.at(0).p;                               // lep1
  vSystem[1] = selLeptons.at(1).p;                               // lep2
  if (sys == "Norm")
    vSystem[2] = selJets.at(0).p;                                    // jet 1
  else if (sys == "JESUp")
    vSystem[2] = selJetsJecUp.at(0).p;                                    // jet 1
  else if (sys == "JESDown")
    vSystem[2] = selJetsJecDown.at(0).p;                                    // jet 1
  else if (sys == "JER")
    vSystem[2] = selJetsJER.at(0).p;                                    // jet 1
  else{
    cout << "Wrong systematic in TWTTbarAnalysis::getDilepJetPt" << endl;
    return -9999.;
  }

  return getPtSys(vSystem,3);
}


Double_t TWTTbarAnalysis::getLep1METJetPt(const TString& sys) {
  TLorentzVector vSystem[3];
  vSystem[0] = selLeptons.at(0).p;
  if (sys == "Norm"){
    vSystem[1].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET);
    vSystem[2] = selJets.at(0).p;
  }
  else if (sys == "JESUp"){
    vSystem[1].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
    vSystem[2] = selJetsJecUp.at(0).p;
  }
  else if (sys == "JESDown"){
    vSystem[1].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
    vSystem[2] = selJetsJecDown.at(0).p;
  }  
  else if (sys == "JER"){
    vSystem[1].SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp); // met
    vSystem[2] = selJetsJER.at(0).p;
  }  
  else{
    cout << "Wrong systematic in TWTTbarAnalysis::getLep1METJetPt" << endl;
    return -9999.;
  }
  
  return getPtSys(vSystem,3);
}


Double_t TWTTbarAnalysis::getPtSys(TLorentzVector* vSystem, int nSystem) {
  TLorentzVector vSyst;
  vSyst = vSystem[0];
  for (int i = 1; i < nSystem; ++i){
    vSyst += vSystem[i];
  }
  return vSyst.Pt();
}


Double_t TWTTbarAnalysis::getDilepMETJet1Pz(const TString& sys) {
  TLorentzVector vSystem[4];
  vSystem[0] = selLeptons.at(0).p;                               // lep1
  vSystem[1] = selLeptons.at(1).p;                               // lep2
  if (sys == "Norm"){
    vSystem[2].SetPtEtaPhiE(TMET,0,TMET_Phi,TMET); // met
    vSystem[3] = selJets.at(0).p;                                    // jet 1
  }
  else if (sys == "JESUp"){
    vSystem[2].SetPtEtaPhiE(TMETJESUp,0,TMET_PhiJESUp,TMETJESUp); // met
    vSystem[3] = selJetsJecUp.at(0).p;                                    // jet 1
  }
  else if (sys == "JESDown"){
    vSystem[2].SetPtEtaPhiE(TMETJESDown,0,TMET_PhiJESDown,TMETJESDown); // met
    vSystem[3] = selJetsJecDown.at(0).p;                                    // jet 1
  }
  else if (sys == "JER"){
    vSystem[2].SetPtEtaPhiE(TMETJERUp,0,TMET_PhiJERUp,TMETJERUp); // met
    vSystem[3] = selJetsJER.at(0).p;                                    // jet 1
  }
  else{
    cout << "Wrong systematic in TWTTbarAnalysis::getDilepMETJet1Pz" << endl;
    return -9999.;
  }

  return getPzSys(vSystem,4);
}


Double_t TWTTbarAnalysis::getPzSys(TLorentzVector* vSystem, int nSystem) {
 TLorentzVector vSyst;
  vSyst = vSystem[0];
  for (int i = 1; i < nSystem; ++i){
    vSyst += vSystem[i];
  }
  return vSyst.Pz();
}


Double_t TWTTbarAnalysis::getDPtDilep_JetMET(const TString& sys) {
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


Double_t TWTTbarAnalysis::getDPtL1_L2(const TString& sys) {
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons.at(0).p);
  col2.push_back(selLeptons.at(1).p);

  return getDeltaPt(col1,col2);

}


Double_t TWTTbarAnalysis::getDPtDilep_MET(const TString& sys) {
  vector<TLorentzVector> col1;
  vector<TLorentzVector> col2;

  col1.push_back(selLeptons.at(0).p);
  col1.push_back(selLeptons.at(1).p);

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
    cout << "Wrong systematic in TWTTbarAnalysis::getDPtDilep_MET" << endl;
    return -9999.;
  }
  
  col2.push_back(met);
  return getDeltaPt(col1,col2);
}


Double_t TWTTbarAnalysis::getDPtLep1_MET(const TString& sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
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
    cout << "Wrong systematic in TWTTbarAnalysis::getDPtDilep_MET" << endl;
    return -9999.;
  }
  col2.push_back(met);
  return getDeltaPt(col1,col2);
}


Double_t TWTTbarAnalysis::getDeltaPt(vector<TLorentzVector> col1, vector<TLorentzVector> col2){
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


Double_t TWTTbarAnalysis::getDPhiLep1_Lep2()
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
  col2.push_back(selLeptons.at(1).p);
  return getDeltaPhi(col1,col2);
}


Double_t TWTTbarAnalysis::getDPhiLep1_Jet1(const TString& sys)
{
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
  if (sys == "Norm")
    col2.push_back(selJets.at(0).p);
  else if (sys == "JESUp")
    col2.push_back(selJetsJecUp.at(0).p);
  else if (sys == "JESDown")
    col2.push_back(selJetsJecDown.at(0).p);
  else if (sys == "JER")
    col2.push_back(selJetsJER.at(0).p);
  else{
    cout << "Wrong systematic in TWTTbarAnalysis::getDPhiLep1_Jet1" << endl;
    return -9999.;
  }
  return getDeltaPhi(col1,col2);
}


Double_t TWTTbarAnalysis::getDPhiLep2_Jet1(const TString& sys) {
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(1).p);
  if (sys == "Norm")
    col2.push_back(selJets.at(0).p);
  else if (sys == "JESUp")
    col2.push_back(selJetsJecUp.at(0).p);
  else if (sys == "JESDown")
    col2.push_back(selJetsJecDown.at(0).p);
  else if (sys == "JER")
    col2.push_back(selJetsJER.at(0).p);
  else{
    cout << "Wrong systematic in TWTTbarAnalysis::getDPhiLep1_Jet1" << endl;
    return -9999.;
  }
  return getDeltaPhi(col1,col2);
}


Double_t TWTTbarAnalysis::getDeltaPhi(vector<TLorentzVector> col1, vector<TLorentzVector> col2) {
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
  return v1.DeltaPhi(v2);
}


Double_t TWTTbarAnalysis::getSysM(const TString& sys) {
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


Double_t TWTTbarAnalysis::getM(vector<TLorentzVector> col) {
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


Double_t TWTTbarAnalysis::getDilepPt() {
  TLorentzVector vSystem[2];
  vSystem[0] = selLeptons.at(0).p;
  vSystem[1] = selLeptons.at(1).p;
  return getPtSys(vSystem,2);
}


Double_t TWTTbarAnalysis::getDeltaRDilep_METJets12(int sys) {
  vector<TLorentzVector>  col1;
  vector<TLorentzVector>  col2;
  col1.push_back(selLeptons.at(0).p);
  col1.push_back(selLeptons.at(1).p);
  if (sys == 0){
    col2.push_back(selJets.at(0).p);
    col2.push_back(selJets.at(1).p);
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


Double_t TWTTbarAnalysis::getDeltaRDilep_Jets12(int sys) {
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


Double_t TWTTbarAnalysis::getDeltaRLep1_Jet1(int sys) {
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


Double_t TWTTbarAnalysis::getDeltaR(vector<TLorentzVector> col1, vector<TLorentzVector> col2) {
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


Double_t TWTTbarAnalysis::getETSys(int sys) {
  if (sys == 0) {
    return selJets.at(0).p.Et() + selJets.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMET;
  }
  else if (sys > 0) {
    return selJetsJecUp.at(0).p.Et() + selJetsJecUp.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESUp;
  }
  else if (sys == -1) {
    return selJetsJecDown.at(0).p.Et() + selJetsJecDown.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESDown;
  }
  else if (sys == -2) {
    return selJetsJER.at(0).p.Et() + selJetsJER.at(1).p.Et() + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJERUp;
  }
  else {
    return -999;
  }
}


Double_t TWTTbarAnalysis::getET_LLJetMET(int sys) {
  if (sys == 0) {
    return selJets.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMET;
  }
  else if (sys > 0) {
    return selJetsJecUp.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESUp;
  }
  else if (sys == -1) {
    return selJetsJecDown.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJESDown;
  }
  else if (sys == -2) {
    return selJetsJER.at(0).p.Et()  + selLeptons.at(0).p.Et() + selLeptons.at(1).p.Et() + TMETJERUp;
  }
  else {
    return -999;
  }
}
