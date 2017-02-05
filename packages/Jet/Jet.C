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

/*
Bool_t  Jet::GetBtag(Int_t iSys){
	switch(iSys){
		case iNom:          return isBtag;
		case iBtagUp:       return isBtagUp;
		case iBtagDown:     return isBtagDown;
		case iBtagMisUp:    return isBtagMisUp;
		case iBtagMisDown:  return isBtagMisDown;
		default: return isBtag;
	}
}

Float_t Jet::GetPt(Int_t iSys){
	switch(iSys){
		case iNom:     return p.Pt();
		case iJESUp:   return pTJESUp;
		case iJESDown: return pTJESDown;
		case iJERUp:   return pTJERUp;
		case iJERDown: return pTJERDown;
		default: return p.Pt():
	}
}

void  Jet::SetIsBtag(Bool_t val, Int_t iSys){
	switch(iSys){
		case iNom:          isBtag        = val;
		case iBtagUp:       isBtagUp      = val;
		case iBtagDown:     isBtagDown    = val;
		case iBtagMisUp:    isBtagMisUp   = val;
		case iBtagMisDown:  isBtagMisDown = val;
		default: isBtag = val;
	}
}

void  Jet::SetPt(Float_t pt, Int_t iSys){
	switch(iSys){
		case iNom:     p.SetPtEtaPhiE(pt, p.Eta(), p.Phi(), p.Energy());
		case iJESUp:   pTJESUp    = pt;
		case iJESDown: pTJESDown  = pt; 
		case iJERUp:   pTJERUp    = pt;
		case iJERDown: pTJERDown  = pt;
		default: return p.Pt():
	}
}
*/
