#pragma once

#include "PAFChainItemSelector.h"
#include "Lepton.h"
#include "Jet.h"
#include "BTagSFUtil.h"
#include "Functions.h"
#include <iostream>
#include <vector>
#include "LinkDef.h"

class JetSelector : public PAFChainItemSelector{

  public:
    TString stringWP;
    JetSelector();
    virtual ~JetSelector() {}
    virtual void InsideLoop();
    void Initialise();
    void Summary();

    std::vector<Lepton> Leptons;
    std::vector<Jet> selJets;
    std::vector<Jet> Jets15;
    std::vector<Jet> genJets;
    std::vector<Jet> mcJets;
    std::vector<Jet> vetoJets;

    Int_t nSelJets;
    Int_t nGenJets;
    Int_t nVetoJets;
    Int_t nJets15;
    Int_t nBtagJets;

    Float_t minDR;
    Float_t jet_MaxEta;
    Float_t jet_MinPt;
    Float_t vetoJet_minPt;
    Float_t vetoJet_maxEta;

    Float_t MET_JESUp; 
    Float_t MET_JESDown;

  protected:

    BTagSFUtil *fBTagSFnom ;
    BTagSFUtil *fBTagSFbUp ;
    BTagSFUtil *fBTagSFbDo ;
    BTagSFUtil *fBTagSFlUp ;
    BTagSFUtil *fBTagSFlDo ;
    string MeasType;

    Bool_t gIsData;
    Int_t gSelection;

    // Jets
    Int_t nJet;
    Jet tJ;
    TLorentzVector tpJ; 
    TLorentzVector tmcJ; 
    Int_t jetId;
    Int_t flavmc;
    Float_t csv;
    Float_t pt; Float_t eta; Float_t rawPt; 
    Float_t pt_corrUp; Float_t pt_corrDown; 
    // genJet
    Int_t ngenJet;

    void GetJetVariables(Int_t i);
    void GetJetFwdVariables(Int_t i);
    void GetGenJetVariables(Int_t i);
    Bool_t IsBtag(Jet j); 
    void SetSystematics(Jet *j);
    Bool_t Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR = 0.4);

    ClassDef(JetSelector, 0);
};

