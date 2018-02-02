#ifndef StopFunctions_C
#define StopFunctions_C 0

Int_t IsEmuFromWW(Int_t mid1, Int_t mid2){
  Float_t w = 0;
  mid1 = TMath::Abs(mid1);
  mid2 = TMath::Abs(mid2);
  if( (mid1 + mid2) == (24*2) ) w = 1;
  return w;
}

Int_t IsEmuFromWTau(Int_t mid1, Int_t mid2){
  Float_t w = 0;
  mid1 = TMath::Abs(mid1);
  mid2 = TMath::Abs(mid2);
  if( (mid1 + mid2) == (24+15) ) w = 1;
  return w;
}

Int_t IsEmuFromTauTau(Int_t mid1, Int_t mid2){
  Float_t w = 0;
  mid1 = TMath::Abs(mid1);
  mid2 = TMath::Abs(mid2);
  if( (mid1 + mid2) == (15*2) ) w = 1;
  return w;
}


#endif
