R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle, TString name = "");
TString NameOfTree = "MiniTree";

TString dilepton = "!TIsSS";
TString jets2    = "!TIsSS && TNJets >= 2";
TString btag1    = "!TIsSS && TNJets >= 2 && TNBtags >= 1";

TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
TString bweightUp = "TWeight*GetFSR_BtagSF_Up(TBtagPt)";
TString bweightDo = "TWeight*GetFSR_BtagSF_Down(TBtagPt)";

MultiPlot* mp = new MultiPlot();

Float_t GetErr(Float_t v, Float_t n);
TString PrintVar(Float_t v, Float_t n);
void SetMultiPlot();
void SetVals(Int_t i);


//TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/FSRtrees/";
//TString pathToTree = "/pool/ciencias/userstorage/juanr/top/nov15/";
TString pathToTree = "/pool/ciencias/userstorage/juanr/top/FSRStudies/";

Float_t n[3], u[3], d[3], uJ[3], dJ[3], uB[3], dB[3];
enum lev{idilep, i2jet, i1btag};
TString level[3] = {"dileptons", ">= 2 jets", ">= 1 btag"};

void FSRStudies(){

  SetMultiPlot();

  mp->SetPlot("dilepton"); SetVals(0);
  mp->SetPlot("2jet");     SetVals(1);
  mp->SetPlot("1btag");    SetVals(2);

  for(Int_t i = 0; i < 3; i++){
    cout << "\n" << endl;
    cout << "========== LEVEL " << level[i] << "==========" << endl;
    cout << " >>> NO CORRECTION " << endl;
    cout << "Yield nominal: " << n[i] << endl;
    cout << "FSR up:        " << PrintVar(u[i], n[i]);
    cout << "FSR down:      " << PrintVar(d[i], n[i]);
    if(i > 0){
      cout << "\n >>> JEC CORRECTION" << endl;
      cout << "FSR up:        " << PrintVar(uJ[i], n[i]);
      cout << "FSR down:      " << PrintVar(dJ[i], n[i]);
    }
    if(i > 1){
      cout << "\n >>> JEC + BTAG CORRECTIONS" << endl;
      cout << "FSR up:        " << PrintVar(uB[i], n[i]);
      cout << "FSR down:      " << PrintVar(dB[i], n[i]);
    }
  }
  return;
}

TString PrintVar(Float_t v, Float_t n){
  TString t;
  t = Form("%1.0f (%1.2f", v, GetErr(v, n)); 
  t += " %)\n";
  //cout << t << endl;
  return t;
}

void SetVals(Int_t i){
  n[i]  = mp->GetYield("tt");
  u[i]  = mp->GetYield("tt_fsru");
  d[i]  = mp->GetYield("tt_fsrd");
  uJ[i] = mp->GetYield("tt_JECu");
  dJ[i] = mp->GetYield("tt_JECd");
  uB[i] = mp->GetYield("tt_Btagu");
  dB[i] = mp->GetYield("tt_Btagd");
}

Float_t GetErr(Float_t v, Float_t n){
  return fabs(v-n)/n*100;
}

void SetMultiPlot(){
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
  AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");

  mp->SetPath(pathToTree);
  mp->SetTreeName(NameOfTree);
  mp->SetPlotFolder("./test/");

  TString chan = "ElMu";
  TString weight = "TWeight";
  TString var = "TChannel";

  mp->AddDistribution("dilepton", var, dilepton, chan, 1, 0, 10);
  mp->AddDistribution("2jet",     var, jets2, chan, 1, 0, 10);
  mp->AddDistribution("1btag",    var, btag1, chan, 1, 0, 10);

  // Nominal
  mp->AddSample("TTbar_Powheg",                 "tt",    itBkg, kRed+1,  "",        "TWeight");

  // FSR out of the box
  mp->AddSample("TTbar_Powheg_fsrUp",         "tt_fsru",    itBkg, kRed+1, "", "TWeight");
  mp->AddSample("TTbar_Powheg_fsrDown",       "tt_fsrd",    itBkg, kRed+1, "", "TWeight");

  // JEC corrections
  mp->AddSample("TTbar_Powheg_fsrUp_JEC",   "tt_JECu",    itBkg, kRed+1,  "", "TWeight");
  mp->AddSample("TTbar_Powheg_fsrDown_JEC", "tt_JECd",    itBkg, kRed+1,  "", "TWeight");

  // JEC and btag corrections
  mp->AddSample("TTbar_Powheg_fsrUp_JEC",   "tt_Btagu",    itBkg, kRed+1,  "", bweightUp);
  mp->AddSample("TTbar_Powheg_fsrDown_JEC", "tt_Btagd",    itBkg, kRed+1,  "", bweightDo);
}
