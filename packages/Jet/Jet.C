#include "Jet.h"

void Jet::InitSyst(){
  pTJESUp   = p.Pt();
  pTJESDown = p.Pt();
  pTJERUp   = p.Pt();
  pTJERDown = p.Pt();
  isBtag_BtagUp     = isBtag;
  isBtag_BtagDown   = isBtag;
  isBtag_MisTagUp   = isBtag;
  isBtag_MisTagDown = isBtag;
}


