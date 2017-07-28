//#pragma once
#ifndef ADDSTUFF_H
#define ADDSTUFF_H
#include<iostream>

using namespace std;

// Float_t ShapVar(Double_t nJet, Double_t nBTag){
//   if (nJet == 1 && nBTag == 1) return 0;
//   if (nJet == 2 && nBTag == 1) return 1;
//   if (nJet == 2 && nBTag == 2) return 2;
//   else return -1;
// }

// Float_t ShapVarWithTwoBDTs(

Float_t theBDt(Double_t BDT){
  // if (BDT < -1)     { cout << "[W]: BDT < -1" << endl; return -9999;  }
  // else if (BDT >  1){ cout << "[W]: BDT >  1" << endl; return -9999;  }
  if      (BDT < -0.531448  ) return 1; 
  else if (BDT < -0.396646  ) return 2; 
  else if (BDT < -0.282771  ) return 3; 
  else if (BDT < -0.184872  ) return 4; 
  else if (BDT < -0.0977834 ) return 5; 
  else if (BDT < -0.0118005 ) return 6; 
  else if (BDT < 0.0867415  ) return 7; 
  else if (BDT < 0.207723   ) return 8; 
  else if (BDT < 0.332052   ) return 9; 
  else                       return 10;

}


Float_t the2j1tBDt(Double_t BDT){
  // if (BDT < -1)     { cout << "[W]: BDT < -1" << endl; return -9999;  }
  // else if (BDT >  1){ cout << "[W]: BDT >  1" << endl; return -9999;  }
  // if      (BDT < -0.856689) return 1;
  // else if (BDT < -0.559669) return 2;
  // else if (BDT < -0.22675 ) return 3;
  // else if (BDT < 0.0977021) return 4;
  // else if (BDT < 0.398956 ) return 5;
  // else                      return 6;

  // if       (BDT < -0.868962)      return 1;
  // else if (BDT < -0.602492) return 2;
  // else if (BDT < -0.2555  ) return 3;
  // else if (BDT < 0.0812049) return 4;
  // else if (BDT < 0.399538 ) return 5;
  // else                      return 6;

  // if      ( BDT < -0.22298  ) return 1;
  // else if ( BDT < -0.0288331) return 2;
  // else if ( BDT < 0.149631  ) return 3;
  // else if ( BDT < 0.352233  ) return 4;
  // else if ( BDT < 0.562698  ) return 5;
  // else                        return 6;


  if      ( BDT < -0.176522  ) return 1;
  else if ( BDT < -0.0798808 ) return 2;
  else if ( BDT < -0.00888648) return 3;
  else if ( BDT < 0.0427275  ) return 4;
  else if ( BDT < 0.108243   ) return 5;
  else                         return 6;


}


Float_t JetPtBinning( Double_t jetpt)
{
  cout << "Cuidado!!!! Has cambiado el binning de todo?" << endl;
  if      (jetpt < 50.)  return 0;
  else if (jetpt < 70.)  return 1;
  else if (jetpt < 90.)  return 2;
  else if (jetpt < 110.) return 3;
  else return 4;

}


Float_t ShapVar(Double_t nJet, Double_t nBtag)
{
  // entre 0 y 2 (3 bines)
  if (nJet == 2 && nBtag == 2) return 0;
  else if (nJet == 2 && nBtag == 1) return 1;
  else if (nJet == 1 && nBtag == 1) return 2;
  else return -1;
}


Float_t Case2( Double_t TNJets , Double_t TNBtags , Double_t TBDT )
{
  // entre 0 y 10 (11 bines)
  if (TNJets == 2 && TNBtags == 1) return 0; // 0
  else if (TNJets == 1 && TNBtags == 1){
    return theBDt(TBDT); // entre 1 y 10
  }
  else return -1;

}

Float_t Case4( Double_t TNJets , Double_t TNBtags , Double_t TBDT , Double_t TJet2_Pt )
{
  // entre 0 y 18 (19 bines)
  if (TNJets == 2 && TNBtags == 2)
    return JetPtBinning(TJet2_Pt);        // entre 0 y 4
  else if (TNJets == 2 && TNBtags == 1)
    return 5;                             // 5
  else if (TNJets == 1 && TNBtags == 1)
    return theBDt( TBDT )+5;                 // entre 6 y 15 (BDT entre 1 y 10)
  else return -1;

}

Float_t ShapVarWith1j1tBDt(Double_t nJet, Double_t nBtag, Double_t bdt)
{
  // entre 0 y 9 (0 bines)
  if (nJet == 1 && nBtag == 1) return theBDt(bdt)-1;  // entre 0 y 9
  else return -1;

}

Float_t FitWYieldsOneBDT(Double_t nJet, Double_t nBtag, Double_t bdt)
{
  // entre 0 y 11 (12 bines)
  if (nJet == 2 && nBtag == 2) return 0;
  else if (nJet == 2 && nBtag == 1) return 1;
  else if (nJet == 1 && nBtag == 1) return theBDt(bdt)+1;  // entre 2 y 11
  else return -1;

}

Float_t ShapVarWithBothBDts(Double_t nJet, Double_t nBtag, Double_t bdt, Double_t bdt2j1t)
{
  // entre 0 y 15 (16 bines)
  if (nJet == 2 && nBtag == 1) return the2j1tBDt( bdt2j1t )-1; // the2j1t esta entre 1 y 6 (entre 0 y 5) 
  else if (nJet == 1 && nBtag == 1) return theBDt(bdt)+5;  // entre 6 y 15 (thBDt esta entre 1 y 10)
  else return -1;

}

Float_t FitWYieldsTwoBDTs(Double_t nJet, Double_t nBtag, Double_t bdt, Double_t bdt2j1t)
{
  // entre 0 y 16 (17 bines)
  if (nJet == 2 && nBtag == 2)      return 0;
  else if (nJet == 2 && nBtag == 1) return the2j1tBDt( bdt2j1t ); // the2j1t esta entre 1 y 6 (entre 1 y 6) 
  else if (nJet == 1 && nBtag == 1) return theBDt(bdt)+6;  // entre 7 y 16 (thBDt esta entre 1 y 10)
  else return -1;

}

Float_t ShapVarWith1j1tBDtAndJetPt(Double_t nJet, Double_t nBtag, Double_t bdt, Double_t bdt2j1t, Double_t jetpt)
{
  // entre 0 y 20 (21 bines)
  if (nJet == 2 && nBtag == 2) return JetPtBinning(jetpt);        // entre 0 y 4
  else if (nJet == 2 && nBtag == 1) return the2j1tBDt( bdt2j1t )+4; // the2j1t esta entre 1 y 6  => entre 5 y 10
  else if (nJet == 1 && nBtag == 1) return theBDt(bdt)+10;  // entre 11 y 20 (thBDt esta entre 1 y 10) 
  else return -1;

}




// Float_t ShapVarWithBDt(Double_t nJet, Double_t nBTag, Double_t BDT){
//   if (nJet == 2 && nBTag == 1) return 0;
//   else if (nJet == 2 && nBTag == 2) return 1;
//   else if (nJet == 1 && nBTag == 1){
//     if (BDT < -1)     { cout << "[W]: BDT < -1" << endl; return -9999;  }
//     else if (BDT >  1){ cout << "[W]: BDT >  1" << endl; return -9999;  }
//       if      (BDT < -0.501254 ) return 2; 
//       else if (BDT < -0.328117 ) return 3; 
//       else if (BDT < -0.207006 ) return 4; 
//       else if (BDT < -0.108441 ) return 5; 
//       else if (BDT < -0.0307518) return 6; 
//       else if (BDT < 0.0479999 ) return 7; 
//       else if (BDT < 0.138156  ) return 8; 
//       else if (BDT < 0.244131  ) return 9; 
//       else if (BDT < 0.352559  ) return 10; 
//       else                      return 11;
//   }
//   else return -1;

//     // if (BDT < -0.268576)                            return 2;
//     // else if (BDT < -0.210725 && BDT > -0.268576)    return 3;
//     // else if (BDT < -0.167467 && BDT > -0.210725)    return 4;
//     // else if (BDT < -0.119859 && BDT > -0.167467)    return 5;
//     // else if (BDT < -0.0533192 && BDT > -0.119859)   return 6;
//     // else if (BDT < -0.0047638 && BDT > -0.0533192)  return 7;
//     // else if (BDT <  0.0385243 && BDT > -0.00476387) return 8;
//     // else if (BDT <  0.0678366 && BDT > 0.0385243)   return 9;
//     // else if (BDT <  0.0990841 && BDT > 0.0678366)   return 10;
//     // else    return 11;

// }


// Float_t ShapVarWithNJets(Double_t nJet, Double_t nBTag, Int_t njet){
//   if (nJet == 2 && nBTag == 1)      return 0;
//   else if (nJet == 2 && nBTag == 2) return 1;
//   else if (nJet == 1 && nBTag == 1){
//     if (njet == 0)                  return 2;
//     else if (njet == 1)             return 3;
//     else                            return 4;
//   } 
//   else return -1;
  
// }  

// Float_t ShapVarWithBDtada(Double_t nJet, Double_t nBTag, Double_t BDTada){
//   if (nJet == 2 && nBTag == 1) return 0;
//   else if (nJet == 2 && nBTag == 2) return 1;
//   else if (nJet == 1 && nBTag == 1){
//     if (BDTada < -0.6) return 2;
//     else if (BDTada > -0.6 && BDTada < -0.4) return 3;
//     else if (BDTada > -0.4 && BDTada < -0.2) return 4;
//     else if (BDTada > -0.2 && BDTada <  0. ) return 5;
//     else if (BDTada >  0.  && BDTada <  0.2) return 6;
//     else if (BDTada >  0.2 && BDTada <  0.4) return 7;
//     else if (BDTada >  0.4 && BDTada <  0.6) return 8;
//     else                                return 9;
//   }
//   else
//     return -1;
// }


// Float_t ShapVarWithBDtgrad(Double_t nJet, Double_t nBTag, Double_t BDTgrad){
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

// Float_t ShapVarWithLoosePt(Float_t nJet, Float_t nBTag, Float_t TJetLoosept)
// {
//   if (nJet == 2 && nBTag == 1)     return 0;
//   else if (nJet == 2 && nBTag == 2) return 1;
//   else if (nJet == 1 && nBTag == 1){
//     if (TJetLoosept < 10)                     return 2;
//     else if (TJetLoosept > 10 && TJetLoosept < 25) return 3;
//     else if (TJetLoosept > 30 && TJetLoosept < 40) return 4;
//     else if (TJetLoosept > 40 && TJetLoosept < 50) return 5;
//     else if (TJetLoosept > 50 && TJetLoosept < 60) return 6;
//     else if (TJetLoosept > 60 && TJetLoosept < 70) return 7;
//     else if (TJetLoosept > 70 && TJetLoosept < 80) return 8;
//     else                                           return 9;

//   }
//   else return -1;

// }

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


Float_t nJetsnBs(float TNJets, float TNBtags)
{
  if (TNJets < 3)
    return TNJets + TNJets*(TNJets-1)/2 + TNBtags;
  else return 6;
}





#endif
