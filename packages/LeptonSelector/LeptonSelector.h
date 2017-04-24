#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "LeptonSF.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "LinkDef.h"

class LeptonSelector : public PAFChainItemSelector{

  public:

    LeptonSelector(); 
    virtual ~LeptonSelector() {delete LepSF;}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();

    std::vector<Lepton> selLeptons;
    std::vector<Lepton> looseLeptons;
    std::vector<Lepton> genLeptons;
    std::vector<Lepton> vetoLeptons;

  protected:

    LeptonSF *LepSF;
    Bool_t gIsData;
    Bool_t gIsFastSim;
    Int_t gSelection;
    TString localPath;

    // LepGood
    Lepton tL;
    Int_t nLep;
    TLorentzVector tP; 
    Float_t pt;
    Float_t eta;
    Int_t 	charge; 
    Int_t 	type;
    Int_t 	tightVar;
    Int_t 	mediumMuonId;
    Float_t etaSC;
    Float_t RelIso03;
    Float_t RelIso04;
    Float_t ptRel;
    Float_t ptRatio;
    Float_t miniIso;
    Float_t sigmaIEtaIEta;
    Float_t dEtaSC;
    Float_t dPhiSC;
    Float_t HoE;
    Float_t eImpI;
    Int_t   lostHits;
    Int_t   convVeto;
    Float_t dxy;
    Float_t dz; 
    Float_t sip;
    Float_t SF;
    Float_t MVATTH;
    Int_t	TightCharge;
    Float_t MVAID;
    Float_t jetBTagCSV;
    Float_t SegComp;
    Int_t isGlobalMuon;
    Int_t isTrackerMuon;
    
    // genLeptons
    Int_t ngenLep;
    Int_t ngenLepFromTau;
    Int_t gpdgMId;

    Int_t nLeptonsFromTau;
    Int_t nSelLeptons;
    Int_t nGenLeptons;
    Int_t nVetoLeptons;
    Int_t nLooseLeptons;

    void GetLeptonVariables(Int_t i);
    void GetGenLeptonVariables(Int_t i);
    void GetGenLepFromTauVariables(Int_t i);
    Bool_t getSIPcut(Float_t cut); 
    Bool_t getGoodVertex(Int_t wp); 
    Bool_t getRelIso03POG(Int_t wp); 
    Bool_t getRelIso04POG(Int_t wp);
    Bool_t getminiRelIso(Int_t wp);
    Bool_t getMuonId(Int_t wp);
    Bool_t getElecMVA(Int_t wp);
    Bool_t getElecMVAId(Int_t wp, Lepton lep); 
    Bool_t getElecCutBasedId(Int_t wp); 
    Bool_t getMultiIso(Int_t wp);
    Bool_t isGoodLepton(Lepton lep);
    Bool_t isLooseLepton(Lepton lep);
    Bool_t isVetoLepton(Lepton lep);

  ClassDef(LeptonSelector, 0);
};
