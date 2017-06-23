//#pragma once
#ifndef ADDSTUFF_H
#define ADDSTUFF_H
#include<iostream>

using namespace std;

Float_t ShapVar(Double_t nJet, Double_t nBTag){
  if (nJet == 1 && nBTag == 1) return 0;
  if (nJet == 2 && nBTag == 1) return 1;
  if (nJet == 2 && nBTag == 2) return 2;
  else return -1;
}


Float_t ShapVarWithBDt(Double_t nJet, Double_t nBTag, Double_t BDT){
  if (nJet == 2 && nBTag == 1) return 0;
  else if (nJet == 2 && nBTag == 2) return 1;
  else if (nJet == 1 && nBTag == 1){
    if (BDT < -1) cout << "[W]: BDT < -1" << endl;
    if (BDT >  1) cout << "[W]: BDT >  1" << endl;
    if (BDT < -0.268576)                            return 2;
    else if (BDT < -0.210725 && BDT > -0.268576)    return 3;
    else if (BDT < -0.167467 && BDT > -0.210725)    return 4;
    else if (BDT < -0.119859 && BDT > -0.167467)    return 5;
    else if (BDT < -0.0533192 && BDT > -0.119859)   return 6;
    else if (BDT < -0.0047638 && BDT > -0.0533192)  return 7;
    else if (BDT <  0.0385243 && BDT > -0.00476387) return 8;
    else if (BDT <  0.0678366 && BDT > 0.0385243)   return 9;
    else if (BDT <  0.0990841 && BDT > 0.0678366)   return 10;
    else    return 11;
  } 
  else return -1;
}

Float_t ShapVarWithNJets(Double_t nJet, Double_t nBTag, Int_t njet){
  if (nJet == 2 && nBTag == 1)      return 0;
  else if (nJet == 2 && nBTag == 2) return 1;
  else if (nJet == 1 && nBTag == 1){
    if (njet == 0)                  return 2;
    else if (njet == 1)             return 3;
    else                            return 4;
  } 
  else return -1;
  
}  

Float_t ShapVarWithBDtada(Double_t nJet, Double_t nBTag, Double_t BDTada){
  if (nJet == 2 && nBTag == 1) return 0;
  else if (nJet == 2 && nBTag == 2) return 1;
  else if (nJet == 1 && nBTag == 1){
    if (BDTada < -0.6) return 2;
    if (BDTada > -0.6 && BDTada < -0.4) return 3;
    if (BDTada > -0.4 && BDTada < -0.2) return 4;
    if (BDTada > -0.2 && BDTada <  0. ) return 5;
    if (BDTada >  0.  && BDTada <  0.2) return 6;
    if (BDTada >  0.2 && BDTada <  0.4) return 7;
    if (BDTada >  0.4 && BDTada <  0.6) return 8;
    else                                return 9;
  }
  else
    return -1;
}


Float_t ShapVarWithBDtgrad(Double_t nJet, Double_t nBTag, Double_t BDTgrad){
  if (nJet == 2 && nBTag == 1) return 0;
  else if (nJet == 2 && nBTag == 2) return 1;
  else if (nJet == 1 && nBTag == 1){
    if (BDTgrad < -0.6) return 2;
    if (BDTgrad > -0.6 && BDTgrad < -0.4) return 3;
    if (BDTgrad > -0.4 && BDTgrad < -0.2) return 4;
    if (BDTgrad > -0.2 && BDTgrad <  0. ) return 5;
    if (BDTgrad >  0.  && BDTgrad <  0.2) return 6;
    if (BDTgrad >  0.2 && BDTgrad <  0.3) return 7;
    if (BDTgrad >  0.3 && BDTgrad <  0.4) return 8;
    else                                  return 9;
  }
  else
    return -1;
}

Float_t ShapVarWithLoosePt(Float_t nJet, Float_t nBTag, Float_t TJetLoosept)
{
  if (nJet == 2 && nBTag == 1)     return 0;
  else if (nJet == 2 && nBTag == 2) return 1;
  else if (nJet == 1 && nBTag == 1){
    if (TJetLoosept < 10)                     return 2;
    if (TJetLoosept > 10 && TJetLoosept < 25) return 3;
    if (TJetLoosept > 30 && TJetLoosept < 40) return 4;
    if (TJetLoosept > 40 && TJetLoosept < 50) return 5;
    if (TJetLoosept > 50 && TJetLoosept < 60) return 6;
    if (TJetLoosept > 60 && TJetLoosept < 70) return 7;
    if (TJetLoosept > 70 && TJetLoosept < 80) return 8;
    if (TJetLoosept > 80)                     return 9;
  }
  else return -1;

}

// Float_t ShapVarWithTwoBDTs(Double_t nJet, Double_t nBTag, Double_t BDT1j1b, Double_t BDT2j1b){
//   else if (nJet == 2 && nBTag == 2) return 0;
//   else if (nJet == 1 && nBTag == 1){
//     if (BDT1j1b < -0.6)                   return 1;
//     if (BDT1j1b > -0.6 && BDT1j1b < -0.4) return 2;
//     if (BDT1j1b > -0.4 && BDT1j1b < -0.2) return 3;
//     if (BDT1j1b > -0.2 && BDT1j1b <  0. ) return 4;
//     if (BDT1j1b >  0.  && BDT1j1b <  0.2) return 5;
//     if (BDT1j1b >  0.2 && BDT1j1b <  0.3) return 6;
//     if (BDT1j1b >  0.3 && BDT1j1b <  0.4) return 7;
//     else                                  return 8;
//   }
//   if (nJet == 2 && nBTag == 1){
//     if (BDDT2
//   }
//   else
//     return -1;
// }


// Float_t FullExtraction(Double_t nJet, Double_t nBTag, Double_t BDTgrad){
//   if (nJet == 2 && nBTag == 1) return 0;
//   else if (nJet == 2 && nBTag == 2) return 1;
//   else if (nJet == 1 && nBTag == 1){
//     if (BDTgrad < -0.6) return 2;
//     if (BDTgrad > -0.6 && BDTgrad < -0.4) return 3;
//     if (BDTgrad > -0.4 && BDTgrad < -0.2) return 4;
//     if (BDTgrad > -0.2 && BDTgrad <  0. ) return 5;
//     if (BDTgrad >  0.  && BDTgrad <  0.2) return 6;
//     if (BDTgrad >  0.2 && BDTgrad <  0.3) return 7;
//     if (BDTgrad >  0.3 && BDTgrad <  0.4) return 8;
//     else                                  return 9;
//   }
//   else
//     return -1;
// }
#endif
