#include "fake_rates.h"

#ifndef t4Functions_C
#define t4Functions_C 1

//Float_t FRweight(Int_t nfakes, Float_t *pt, Float_t *eta, Int_t *id);
//Float_t FRweight(Int_t nfakes, Float_t pt[], Float_t eta[], Int_t id[]){

TString t4ChannelLabelsWithFakes = "2LSS, *, >= 3L, *, 2LSS + #tau, *, 2LOS+tau, * ";
TString t4ChannelLabels = "2LSS, >= 3L, 2LSS + #tau, 2LOS+tau";
Int_t GetChannelWithFakes(Int_t TChannel){
  enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, TotalDefinedChannels};

  Int_t ch = 0;
  if     (TChannel == i2lss)            ch = 1;
  else if(TChannel == i2lss_fake)       ch = 2;
  else if(TChannel == iTriLep)          ch = 3;
  else if(TChannel == iTriLep_fake)     ch = 4;
  else if(TChannel == iSS1tau)          ch = 5;
  else if(TChannel == i1Tau_emufakeSS)  ch = 6;
  else if(TChannel == iOS1tau)          ch = 7;
  else if(TChannel == i1Tau_emufakeOS)  ch = 8;
  return ch;
}

Int_t GetChannel(Int_t TChannel){
  enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, TotalDefinedChannels};

  Int_t ch = 0;
  if     (TChannel == i2lss   || TChannel == i2lss_fake)            ch = 1;
  else if(TChannel == iTriLep || TChannel == iTriLep_fake)          ch = 2;
  else if(TChannel == iSS1tau || TChannel == i1Tau_emufakeSS)       ch = 3;
  else if(TChannel == iOS1tau || TChannel == i1Tau_emufakeOS)       ch = 4;
  return ch;
}

Int_t IsFake(Int_t TChannel){
  enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, TotalDefinedChannels};

  Int_t ch = 0;
  if     (TChannel == i2lss)            ch = 0;
  else if(TChannel == i2lss_fake)       ch = 1;
  else if(TChannel == iTriLep)          ch = 0;
  else if(TChannel == iTriLep_fake)     ch = 1;
  else if(TChannel == iSS1tau)          ch = 0;
  else if(TChannel == i1Tau_emufakeSS)  ch = 1;
  else if(TChannel == iOS1tau)          ch = 0;
  else if(TChannel == i1Tau_emufakeOS)  ch = 1;
  return ch;
}

Float_t _FW = 0;
Float_t FRweight(Int_t nfakes, Float_t pt1, Float_t pt2, Float_t pt3, Float_t eta1, Float_t eta2, Float_t eta3, Int_t id1, Int_t id2, Int_t id3){
  Float_t weight = 0; Float_t f = 1;
//  Float_t pt[3] = {pt1, pt2, pt3};
//  Float_t eta[3] = {eta1, eta2, eta3};
//  Int_t id[3] = {id1, id2, id3};
  //cout << "Llego aqui..." << endl;
  if(nfakes == 0) weight = 0; 
//  if(nfakes == 3){
//    cout << "id1  = " << id1  << ", id2  = " << id2 <<  ", id3  = " << id3  << endl;
//    cout << "pt1  = " << pt1  << ", pt2  = " << pt2 <<  ", pt3  = " << pt3  << endl;
//    cout << "eta1 = " << eta1 << ", eta2 = " << eta2 << ", eta3 = " << eta3 << endl;
//  }
  else{
    weight = -1;
    //cout << "nfakes = " << nfakes << endl;
    if     (nfakes == 1){ 
      if(pt1 < 20) cout << "WARNING: pt1 = " << pt1 << endl;
      f = t4fake(pt1, eta1, id1);
      weight *= -f/(1-f);
    }
    else if(nfakes == 2){ 
      if(pt1 < 20) cout << "WARNING: pt1 = " << pt1 << endl;
      if(pt2 < 20) cout << "WARNING: pt2 = " << pt2 << endl;
      f = t4fake(pt1, eta1, id1); weight *= -f/(1-f);
      f = t4fake(pt2, eta2, id2); weight *= -f/(1-f);
    }
    else if(nfakes == 3){ 
      if(pt1 < 20) cout << "WARNING: pt1 = " << pt1 << endl;
      if(pt2 < 20) cout << "WARNING: pt2 = " << pt2 << endl;
      if(pt3 < 20) cout << "WARNING: pt3 = " << pt3 << endl;
      f = t4fake(pt1, eta1, id1); weight *= -f/(1-f);
      f = t4fake(pt2, eta2, id2); weight *= -f/(1-f);
      f = t4fake(pt3, eta3, id3); weight *= -f/(1-f);
    }
  }
  //cout << "[" << nfakes << "] weight = " << weight << endl;
  return weight;
}

/*Float_t FRweight(Int_t nfakes, Float_t pt, Float_t pt2, Float_t eta, Int_t id){
  Float_t weight = 0; Float_t f = 1;
  if(nfakes == 0) weight = 0; 
  else{
    weight = -1;
    if(nfakes == 2){
      Float_t *ppt = &pt;
      cout << "pt[0] = " << pt << ", pt[1] = " << ppt[1] << endl;
    }
    for(Int_t i = 0; i < nfakes; i++){
      f = t4fake(pt, eta, id);
      weight *= -f/(1-f);
    }
  }
  return weight;
}*/

Float_t FRweightError(Int_t nfakes, Float_t *pt, Float_t *eta, Int_t* id){
  Float_t weight = 0; Float_t f = 1;
  if(nfakes == 0) weight = 0; 
  else{
    weight = -1;
    for(Int_t i = 0; i < nfakes; i++){
      f = t4fake_err(pt[i], eta[i], id[i]);
      weight *= -f/(1-f);
    }
  }
  return weight;
}


#endif
