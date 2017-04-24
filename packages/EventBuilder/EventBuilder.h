#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include "LeptonSF.h"
#include "PUWeight.h"

class EventBuilder : public PAFChainItemSelector{

  public:

    std::vector<Float_t> CountLHE;
    std::vector<Float_t> LHEWeights;

    EventBuilder();
    virtual ~EventBuilder() {delete fPUWeight, fPUWeightUp, fPUWeightDown, TriggSF;}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();

    //Bool_t PassHLT_Elec; Bool_t PassHLT_Muon; Bool_t PassHLT_ElMu;
    Bool_t METfilters;
    Bool_t passTrigger;
    Bool_t isSS;
    Bool_t gIsFastSim;

    //Float_t  TrigSFElec;      Float_t  TrigSFMuon;      Float_t  TrigSFElMu;
    //Float_t  TrigSFElec_Up;   Float_t  TrigSFElMu_Up;   Float_t  TrigSFMuon_Up;
    //Float_t  TrigSFElec_Down; Float_t  TrigSFMuon_Down; Float_t  TrigSFElMu_Down;

    Float_t TriggerSF; Float_t TriggerSF_Up; Float_t TriggerSF_Down; Float_t TriggerSF_err;
    Float_t PUSF;      Float_t PUSF_Up;      Float_t PUSF_Down;

    Float_t NormWeight; // Nominal
    Float_t Weight;  // CrossSection/NumberOfGenEvents
    Float_t genWeight;   // For aMCatNLO samples

    Float_t nTrueInt;
    Int_t   gChannel;
    std::vector<Lepton> selLeptons;

  protected:

    LeptonSF *TriggSF;
    PUWeight *fPUWeight;
    PUWeight *fPUWeightUp;
    PUWeight *fPUWeightDown;

    Bool_t PassesMETfilters();

    Bool_t TrigElMu();
    Bool_t TrigElEl();
    Bool_t TrigMuMu();
    Bool_t Trig3l4l();

    Bool_t PassesDoubleElecTrigger();
    Bool_t PassesDoubleMuonTrigger();
    Bool_t PassesElMuTrigger();
    Bool_t PassesSingleElecTrigger();
    Bool_t PassesSingleMuonTrigger();
    Bool_t PassesDoubleMuonHTTrigger();
    Bool_t PassesDoubleElecHTTrigger();
    Bool_t PassesElMuHTTrigger();
    Bool_t PassesThreelFourlTrigger();

    Bool_t gIsSingleMuon;
    Bool_t gIsSingleElec;
    Bool_t gIsDoubleMuon;
    Bool_t gIsDoubleElec;
    Bool_t gIsMuonEG;
    Bool_t  gIsData;
    Int_t run;
    Int_t   gSelection;
    TString gSampleName;
    TString gPathToHeppyTrees;
    Bool_t  gIsMCatNLO;

    void SetCountLHE();

    ClassDef(EventBuilder, 0);
};
