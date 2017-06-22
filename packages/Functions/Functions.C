#include "Functions.h"
#include "mt2.h"

// ==================================================================
// ==================== Kinematic variables ========================
// ==================================================================
Float_t getDPhiClosestJet(vector<Jet> vJet, TLorentzVector lep){
  Float_t minDphi = 9999.;
  for (UInt_t i=0; i<vJet.size(); i++) {
    if (minDphi > TMath::Abs(lep.DeltaPhi(vJet[i].p))) minDphi = TMath::Abs(lep.DeltaPhi(vJet[i].p));
  }
  return minDphi;
}

Float_t getDeltaPhillJet(Lepton lep1, Lepton lep2, Jet jet){
  TLorentzVector dilep = lep1.p+lep2.p;
  return TMath::Abs(dilep.DeltaPhi(jet.p));
}

Float_t getHT(vector<Jet> jet){
  Float_t ht(0);
  for (UInt_t i=0; i < jet.size(); i++) ht += jet.at(i).p.Pt();
  return ht;
}

Float_t getMHT(vector<Lepton> lepton, vector<Jet> jet) { // (~MET but with selected objects)
  Float_t mht = 0;
  TLorentzVector vectemp(0.,0.,0.,0.);
  for (UInt_t i = 0; i < jet.size(); i++)    vectemp += jet.at(i).p;
  for (UInt_t i = 0; i < lepton.size(); i++) vectemp += lepton.at(i).p;
  mht = vectemp.Pt();
  
  return mht;
}

Float_t getMETLD(Float_t MET, Float_t MHT) {
  Float_t metld;
  metld = MET * 0.00397 + MHT * 0.00265;
  
  return metld;
}

Float_t getMT(TLorentzVector v1, TLorentzVector v2){
  Float_t pt1 = v1.Pt();
  Float_t pt2 = v2.Pt();
  Float_t dphi = v1.DeltaPhi(v2);
  return TMath::Sqrt(2*pt1*pt2*(1-TMath::Cos(dphi)));
}

Float_t getMinDPhiMetJets(vector<Jet> vjets, Float_t met, Float_t met_phi){
  Float_t deltaphi = 999;
  Float_t minDphi  = 999;
  TLorentzVector pmet;
  pmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  for(Int_t i = 0; i < vjets.size(); i++){
    deltaphi = pmet.DeltaPhi(vjets[i].p);
    if(minDphi > deltaphi) minDphi = deltaphi;
  }
  return minDphi;
}

Float_t getDelPhi(TLorentzVector v1, TLorentzVector v2){ return v1.DeltaPhi(v2);}
Float_t getDelEta(TLorentzVector v1, TLorentzVector v2){ return TMath::Abs(v1.Eta() - v2.Eta());}

Float_t getMT2(TLorentzVector plep1, TLorentzVector plep2, TLorentzVector pmet, Float_t mass){
  double pa[3]; double pb[3]; double pmiss[3];
  pmiss[0] = 0.; // irrelevant
  pmiss[1] = pmet.Px(); pmiss[2] = pmet.Py();
  pa[0] = 0.; pa[1] = plep1.Px(); pa[2] = plep1.Py();
  pb[0] = 0.; pb[1] = plep2.Px(); pb[2] = plep2.Py();
  mt2 MT2bisect;
  MT2bisect.set_momenta(pa, pb, pmiss);
  MT2bisect.set_mn(mass); // testmass
  Float_t MT2 = MT2bisect.get_mt2();
  return MT2;
}

Float_t getMT2ll(Lepton l1, Lepton l2, Float_t met, Float_t met_phi){
  TLorentzVector pmet;
  pmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  Float_t MT2 = getMT2(l1.p, l2.p, pmet, 0);
  return MT2;
}

Float_t getMeff(Lepton l1, Lepton l2, vector<Jet> vjets, Float_t met){
  return l1.p.Pt() + l2.p.Pt() + met;
} 

TLorentzVector getPtllb(Lepton l1, Lepton l2, Float_t met, Float_t met_phi){
  TLorentzVector pmet; pmet.SetPtEtaPhiM(met, 0., met_phi, 0.);
  TLorentzVector pl1 = l1.p; TLorentzVector pl2 = l2.p;
  return pl1 + pl2 + pmet;
}

Bool_t Cleaning(Jet j, vector<Lepton> vLep, Float_t minDR){
  Int_t nLeps = vLep.size();
  for(Int_t i = 0; i < nLeps; i++){
    if(j.p.DeltaR(vLep[i].p) < minDR) return false;
  }
  return true;
}

// ==================================================================
// ========================== Systematics ===========================
// ==================================================================
Float_t JEStoMET(vector<Jet> vjets, Float_t met, Float_t met_phi, Int_t dir){
  if(dir == 0){
    std::cout << "[ERROR] Wrong value in JEStoMET" << std::endl;
    return 0;
  }
  TLorentzVector vmet    = TLorentzVector();
  vmet.SetPtEtaPhiM(met, 0, met_phi, 0);
  TLorentzVector nomJets = TLorentzVector(0,0,0,0);
  TLorentzVector varJets = TLorentzVector(0,0,0,0);
  TLorentzVector temp = TLorentzVector(0,0,0,0);

  for(Int_t i = 0; i < (Int_t) vjets.size(); i++){
    nomJets += vjets[i].p;
    if(dir < 0) temp.SetPtEtaPhiM(vjets[i].pTJESDown, vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    else if(dir > 0) temp.SetPtEtaPhiM(vjets[i].pTJESUp,   vjets[i].p.Eta(), vjets[i].p.Phi(), vjets[i].p.M());
    varJets += temp;
  }
  return (vmet+nomJets-varJets).Pt();
}

Float_t getJERscale(Float_t jet_eta, Int_t dir = 0){
  // SFs for 80DX https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
  Float_t eta = TMath::Abs(jet_eta);
  Float_t SF = 1; Float_t err = 0;
  if     (eta < 0.5){SF = 1.109; err = 0.008; } 
  else if(eta < 0.8){SF = 1.138; err = 0.013; }
  else if(eta < 1.1){SF = 1.114; err = 0.013; }
  else if(eta < 1.3){SF = 1.123; err = 0.024; }
  else if(eta < 1.7){SF = 1.084; err = 0.011; }
  else if(eta < 1.9){SF = 1.082; err = 0.035; }
  else if(eta < 2.1){SF = 1.140; err = 0.047; }
  else if(eta < 2.3){SF = 1.067; err = 0.053; }
  else if(eta < 2.5){SF = 1.177; err = 0.041; }
  else if(eta < 2.8){SF = 1.364; err = 0.039; }
  else if(eta < 3.0){SF = 1.857; err = 0.071; }
  else if(eta < 3.2){SF = 1.328; err = 0.022; }
  else              {SF = 1.160; err = 0.029;  }
  if     (dir == +1) return SF + err;
  else if(dir == -1) return SF - err;
  return SF;
}

Bool_t ByPt( Jet jet1, Jet jet2){
  return (jet1.p.Pt() > jet2.p.Pt());
}


Float_t getJERscale(Jet jet, Int_t dir = 0){ return getJERscale(jet.p.Eta(), dir);}

Float_t getJetPtErr(Jet jet, Float_t rho){
  // From https://github.com/cms-jet/JRDatabase/blob/master/textFiles/Spring16_25nsV6_MC/Spring16_25nsV6_MC_PtResolution_AK4PF.txt
  Float_t pt = jet.p.Pt(); Float_t eta = TMath::Abs(jet.p.Eta());
  float ptmax = -1.; float A = -1; float B = -1; float C = -1; float D = -1;
  if      (eta < -3.2 && rho < 6.73) {ptmax =  299.824   ; A =        2.373 ;B =         0.322 ; C =      0.09776   ; D =     -0.8119;	      }
  else if (eta < -3.2 && rho < 12.51){ptmax =    292.5   ; A =         3.01 ;B =        0.2254 ; C =      0.08868   ; D =     -0.5196;	      }
  else if (eta < -3.2 && rho < 18.29){ptmax =      295   ; A =        3.544 ;B =        0.1874 ; C =    5.446e-05   ; D =     -0.2277;	      }
  else if (eta < -3.2 && rho < 24.08){ptmax =    299.5   ; A =        3.381 ;B =         1.114 ; C =       0.1021   ; D =      -1.365;	      }
  else if (eta < -3.2)               {ptmax =    297.9   ; A =       -15.14 ;B =         15.21 ; C =       0.1039   ; D =      -1.971;	      }
  else if (eta < -3.  && rho < 6.73) {ptmax =    299.8   ; A =        4.089 ;B =        0.9805 ; C =       0.1314   ; D =      -1.351;	      }
  else if (eta < -3.  && rho < 12.51){ptmax =    308.3   ; A =     0.006269 ;B =         4.498 ; C =       0.1352   ; D =      -1.848;	      }
  else if (eta < -3.  && rho < 18.29){ptmax =    302.3   ; A =        6.387 ;B =     3.303e-05 ; C =       0.1413   ; D =       -1.41;	      }
  else if (eta < -3.  && rho < 24.08){ptmax =    302.7   ; A =        7.178 ;B =      2.53e-05 ; C =       0.1344   ; D =      -1.247;	      }
  else if (eta < -3. )               {ptmax =    304.7   ; A =        7.821 ;B =    -6.745e-06 ; C =       0.1408   ; D =      -1.492;	      }
  else if (eta < -2.8 && rho < 6.73) {ptmax =  444.6     ; A =      4.479   ;B =      0.2032   ; C =  2.553e-05     ; D =   -0.2419  ;		      }
  else if (eta < -2.8 && rho < 12.51){ptmax =    448     ; A =      5.199   ;B =      0.2912   ; C =    6.7e-05     ; D =   -0.4054  ;		      }
  else if (eta < -2.8 && rho < 18.29){ptmax =  457.3     ; A =      5.741   ;B =       0.573   ; C =    0.07489     ; D =   -0.8183  ;		      }
  else if (eta < -2.8 && rho < 24.08){ptmax =  451.7     ; A =      6.859   ;B =      0.3026   ; C =  1.662e-06     ; D =   -0.4132  ;		      }
  else if (eta < -2.8)               {ptmax =  459.2     ; A =      8.357   ;B =      0.1995   ; C =  1.461e-05     ; D =   -0.2497  ;		      }
  else if (eta < -2.5 && rho < 6.73) {ptmax =    640.6   ; A =        4.518 ;B =         0.222 ; C =    2.174e-06   ; D =     -0.3943;	      }
  else if (eta < -2.5 && rho < 12.51){ptmax =    637.1   ; A =        5.323 ;B =        0.2489 ; C =    9.607e-06   ; D =     -0.4586;	      }
  else if (eta < -2.5 && rho < 18.29){ptmax =    637.9   ; A =        6.237 ;B =        0.2938 ; C =    6.557e-07   ; D =     -0.5231;	      }
  else if (eta < -2.5 && rho < 24.08){ptmax =    646.7   ; A =        7.236 ;B =        0.2539 ; C =    1.237e-06   ; D =     -0.4661;	      }
  else if (eta < -2.5)               {ptmax =    647.2   ; A =        7.935 ;B =        0.3382 ; C =    1.379e-06   ; D =     -0.5699;	      }
  else if (eta < -2.3 && rho < 6.73) {ptmax =    653.4   ; A =        4.156 ;B =        0.3477 ; C =    1.346e-06   ; D =     -0.6215;	      }
  else if (eta < -2.3 && rho < 12.51){ptmax =    651.1   ; A =         4.91 ;B =        0.3751 ; C =    8.908e-07   ; D =     -0.6407;	      }
  else if (eta < -2.3 && rho < 18.29){ptmax =    650.5   ; A =        5.655 ;B =        0.3878 ; C =    3.564e-06   ; D =      -0.652;	      }
  else if (eta < -2.3 && rho < 24.08){ptmax =    649.1   ; A =        6.178 ;B =         0.469 ; C =    3.809e-06   ; D =     -0.7149;	      }
  else if (eta < -2.3)               {ptmax =    846.7   ; A =        6.981 ;B =        0.4503 ; C =    2.365e-07   ; D =     -0.6961;	      }
  else if (eta < -2.1 && rho < 6.73) {ptmax =    885.3   ; A =        3.541 ;B =        0.4416 ; C =      0.02533   ; D =     -0.7289;	      }
  else if (eta < -2.1 && rho < 12.51){ptmax =    872.5   ; A =        4.338 ;B =        0.4735 ; C =      0.02318   ; D =     -0.7473;	      }
  else if (eta < -2.1 && rho < 18.29){ptmax =    873.5   ; A =        4.667 ;B =        0.6482 ; C =      0.03168   ; D =     -0.8764;	      }
  else if (eta < -2.1 && rho < 24.08){ptmax =    873.3   ; A =        5.345 ;B =        0.6433 ; C =      0.02504   ; D =     -0.8451;	      }
  else if (eta < -2.1)               {ptmax =    873.7   ; A =        6.278 ;B =        0.5782 ; C =      0.01875   ; D =     -0.7908;	      }
  else if (eta < -1.9 && rho < 6.73) {ptmax =    888.5   ; A =        2.827 ;B =        0.6968 ; C =      0.03702   ; D =     -0.9243;	      }
  else if (eta < -1.9 && rho < 12.51){ptmax =     1170   ; A =        3.742 ;B =        0.6879 ; C =      0.03564   ; D =      -0.907;	      }
  else if (eta < -1.9 && rho < 18.29){ptmax =     1167   ; A =        4.675 ;B =         0.561 ; C =      0.02864   ; D =      -0.799;	      }
  else if (eta < -1.9 && rho < 24.08){ptmax =     1172   ; A =         4.66 ;B =         0.963 ; C =      0.03609   ; D =       -1.01;	      }
  else if (eta < -1.9)               {ptmax =     1169   ; A =        5.783 ;B =        0.6706 ; C =      0.02924   ; D =     -0.8551;	      }
  else if (eta < -1.7 && rho < 6.73) {ptmax =     1199   ; A =        2.729 ;B =        0.7823 ; C =      0.03672   ; D =     -0.9445;	      }
  else if (eta < -1.7 && rho < 12.51){ptmax =     1199   ; A =        3.918 ;B =        0.7173 ; C =      0.03579   ; D =     -0.9014;	      }
  else if (eta < -1.7 && rho < 18.29){ptmax =     1191   ; A =        4.776 ;B =        0.6319 ; C =      0.03041   ; D =       -0.83;	      }
  else if (eta < -1.7 && rho < 24.08){ptmax =     1199   ; A =        4.869 ;B =        0.9075 ; C =      0.03557   ; D =     -0.9694;	      }
  else if (eta < -1.7)               {ptmax =     1188   ; A =        6.051 ;B =        0.6433 ; C =      0.02608   ; D =     -0.8158;	      }
  else if (eta < -1.3 && rho < 6.73) {ptmax =     1625   ; A =      -0.9513 ;B =         1.458 ; C =      0.05725   ; D =      -1.169;	      }
  else if (eta < -1.3 && rho < 12.51){ptmax =     1634   ; A =      -0.1577 ;B =         1.736 ; C =      0.05719   ; D =       -1.22;	      }
  else if (eta < -1.3 && rho < 18.29){ptmax =     1637   ; A =        2.877 ;B =         1.574 ; C =      0.05651   ; D =      -1.177;	      }
  else if (eta < -1.3 && rho < 24.08){ptmax =     1630   ; A =        4.046 ;B =         1.541 ; C =      0.05654   ; D =      -1.162;	      }
  else if (eta < -1.3)               {ptmax =     1643   ; A =        5.064 ;B =         1.474 ; C =      0.05645   ; D =      -1.143;	      }
  else if (eta < -1.1 && rho < 6.73) {ptmax =     2080   ; A =        1.947 ;B =          0.79 ; C =      0.04998   ; D =     -0.9118;	      }
  else if (eta < -1.1 && rho < 12.51){ptmax =     2053   ; A =            3 ;B =        0.8215 ; C =      0.04957   ; D =     -0.9162;	      }
  else if (eta < -1.1 && rho < 18.29){ptmax =     2090   ; A =        3.555 ;B =        0.9421 ; C =      0.04979   ; D =      -0.958;	      }
  else if (eta < -1.1 && rho < 24.08){ptmax =     2096   ; A =        4.335 ;B =        0.9671 ; C =      0.04903   ; D =     -0.9581;	      }
  else if (eta < -1.1)               {ptmax =     2089   ; A =        5.391 ;B =        0.9144 ; C =      0.04856   ; D =     -0.9332;	      }
  else if (eta < -0.8 && rho < 6.73) {ptmax =     2150   ; A =        1.952 ;B =        0.5236 ; C =      0.03495   ; D =      -0.749;	      }
  else if (eta < -0.8 && rho < 12.51){ptmax =     2620   ; A =        2.861 ;B =        0.5957 ; C =       0.0363   ; D =     -0.7965;	      }
  else if (eta < -0.8 && rho < 18.29){ptmax =     2145   ; A =        3.523 ;B =        0.6874 ; C =      0.03809   ; D =     -0.8484;	      }
  else if (eta < -0.8 && rho < 24.08){ptmax =     2589   ; A =        4.347 ;B =        0.7109 ; C =      0.03763   ; D =     -0.8536;	      }
  else if (eta < -0.8)               {ptmax =     2626   ; A =        5.534 ;B =        0.6063 ; C =      0.03484   ; D =     -0.7873;	      }
  else if (eta < -0.5 && rho < 6.73) {ptmax =     2165   ; A =        1.803 ;B =        0.4178 ; C =      0.02287   ; D =     -0.6661;	      }
  else if (eta < -0.5 && rho < 12.51){ptmax =     2166   ; A =        2.926 ;B =        0.4496 ; C =      0.02451   ; D =     -0.6915;	      }
  else if (eta < -0.5 && rho < 18.29){ptmax =     2171   ; A =        3.715 ;B =        0.5099 ; C =      0.02577   ; D =     -0.7359;	      }
  else if (eta < -0.5 && rho < 24.08){ptmax =     2652   ; A =        4.665 ;B =        0.5057 ; C =       0.0266   ; D =     -0.7333;	      }
  else if (eta < -0.5)               {ptmax =     2668   ; A =         5.27 ;B =        0.5937 ; C =      0.02861   ; D =     -0.7887;	      }
  else if (eta <  0.0 && rho < 6.73) {ptmax =     3121   ; A =        1.584 ;B =        0.4016 ; C =       0.0187   ; D =     -0.6458;	      }
  else if (eta <  0.0 && rho < 12.51){ptmax =     2662   ; A =        2.687 ;B =        0.4743 ; C =      0.02212   ; D =     -0.7058;	      }
  else if (eta <  0.0 && rho < 18.29){ptmax =     2183   ; A =         3.63 ;B =        0.5071 ; C =      0.02452   ; D =     -0.7326;	      }
  else if (eta <  0.0 && rho < 24.08){ptmax =     2664   ; A =        4.515 ;B =        0.5392 ; C =      0.02569   ; D =      -0.754;	      }
  else if (eta <  0.0)               {ptmax =     2658   ; A =        5.124 ;B =        0.6185 ; C =      0.02731   ; D =     -0.8025;	      }
  else if (eta <  0.5 && rho < 6.73) {ptmax =  3121      ; A =     1.584    ;B =     0.4016    ; C =    0.0187      ; D =  -0.6458   ;		      }
  else if (eta <  0.5 && rho < 12.51){ptmax =  2662      ; A =     2.687    ;B =     0.4743    ; C =   0.02212      ; D =  -0.7058   ;		      }
  else if (eta <  0.5 && rho < 18.29){ptmax =  2183      ; A =      3.63    ;B =     0.5071    ; C =   0.02452      ; D =  -0.7326   ;		      }
  else if (eta <  0.5 && rho < 24.08){ptmax =  2664      ; A =     4.515    ;B =     0.5392    ; C =   0.02569      ; D =   -0.754   ;		      }
  else if (eta <  0.5)               {ptmax =  2658      ; A =     5.124    ;B =     0.6185    ; C =   0.02731      ; D =  -0.8025   ;		      }
  else if (eta <  0.8 && rho < 6.73) {ptmax =    2165    ; A =       1.803  ;B =       0.4178  ; C =     0.02287    ; D =    -0.6661 ;		      }
  else if (eta <  0.8 && rho < 12.51){ptmax =    2166    ; A =       2.926  ;B =       0.4496  ; C =     0.02451    ; D =    -0.6915 ;		      }
  else if (eta <  0.8 && rho < 18.29){ptmax =    2171    ; A =       3.715  ;B =       0.5099  ; C =     0.02577    ; D =    -0.7359 ;		      }
  else if (eta <  0.8 && rho < 24.08){ptmax =    2652    ; A =       4.665  ;B =       0.5057  ; C =      0.0266    ; D =    -0.7333 ;		      }
  else if (eta <  0.8)               {ptmax =    2668    ; A =        5.27  ;B =       0.5937  ; C =     0.02861    ; D =    -0.7887 ;		      }
  else if (eta <  1.1 && rho < 6.73) {ptmax =    2150    ; A =       1.952  ;B =       0.5236  ; C =     0.03495    ; D =     -0.749 ;		      }
  else if (eta <  1.1 && rho < 12.51){ptmax =    2620    ; A =       2.861  ;B =       0.5957  ; C =      0.0363    ; D =    -0.7965 ;		      }
  else if (eta <  1.1 && rho < 18.29){ptmax =    2145    ; A =       3.523  ;B =       0.6874  ; C =     0.03809    ; D =    -0.8484 ;		      }
  else if (eta <  1.1 && rho < 24.08){ptmax =    2589    ; A =       4.347  ;B =       0.7109  ; C =     0.03763    ; D =    -0.8536 ;		      }
  else if (eta <  1.1)               {ptmax =    2626    ; A =       5.534  ;B =       0.6063  ; C =     0.03484    ; D =    -0.7873 ;		      }
  else if (eta <  1.3 && rho < 6.73) {ptmax =    2080    ; A =       1.947  ;B =         0.79  ; C =     0.04998    ; D =    -0.9118 ;		      }
  else if (eta <  1.3 && rho < 12.51){ptmax =    2053    ; A =           3  ;B =       0.8215  ; C =     0.04957    ; D =    -0.9162 ;		      }
  else if (eta <  1.3 && rho < 18.29){ptmax =    2090    ; A =       3.555  ;B =       0.9421  ; C =     0.04979    ; D =     -0.958 ;		      }
  else if (eta <  1.3 && rho < 24.08){ptmax =    2096    ; A =       4.335  ;B =       0.9671  ; C =     0.04903    ; D =    -0.9581 ;		      }
  else if (eta <  1.3)               {ptmax =    2089    ; A =       5.391  ;B =       0.9144  ; C =     0.04856    ; D =    -0.9332 ;		      }
  else if (eta <  1.7 && rho < 6.73) {ptmax =    1625    ; A =     -0.9513  ;B =        1.458  ; C =     0.05725    ; D =     -1.169 ;		      }
  else if (eta <  1.7 && rho < 12.51){ptmax =    1634    ; A =     -0.1577  ;B =        1.736  ; C =     0.05719    ; D =      -1.22 ;		      }
  else if (eta <  1.7 && rho < 18.29){ptmax =    1637    ; A =       2.877  ;B =        1.574  ; C =     0.05651    ; D =     -1.177 ;		      }
  else if (eta <  1.7 && rho < 24.08){ptmax =    1630    ; A =       4.046  ;B =        1.541  ; C =     0.05654    ; D =     -1.162 ;		      }
  else if (eta <  1.7)               {ptmax =    1643    ; A =       5.064  ;B =        1.474  ; C =     0.05645    ; D =     -1.143 ;		      }
  else if (eta <  1.9 && rho < 6.73) {ptmax =    1199    ; A =       2.729  ;B =       0.7823  ; C =     0.03672    ; D =    -0.9445 ;		      }
  else if (eta <  1.9 && rho < 12.51){ptmax =    1199    ; A =       3.918  ;B =       0.7173  ; C =     0.03579    ; D =    -0.9014 ;		      }
  else if (eta <  1.9 && rho < 18.29){ptmax =    1191    ; A =       4.776  ;B =       0.6319  ; C =     0.03041    ; D =      -0.83 ;		      }
  else if (eta <  1.9 && rho < 24.08){ptmax =    1199    ; A =       4.869  ;B =       0.9075  ; C =     0.03557    ; D =    -0.9694 ;		      }
  else if (eta <  1.9)               {ptmax =    1188    ; A =       6.051  ;B =       0.6433  ; C =     0.02608    ; D =    -0.8158 ;		      }
  else if (eta <  2.1 && rho < 6.73) {ptmax =   888.5    ; A =       2.827  ;B =       0.6968  ; C =     0.03702    ; D =    -0.9243 ;		      }
  else if (eta <  2.1 && rho < 12.51){ptmax =    1170    ; A =       3.742  ;B =       0.6879  ; C =     0.03564    ; D =     -0.907 ;		      }
  else if (eta <  2.1 && rho < 18.29){ptmax =    1167    ; A =       4.675  ;B =        0.561  ; C =     0.02864    ; D =     -0.799 ;		      }
  else if (eta <  2.1 && rho < 24.08){ptmax =    1172    ; A =        4.66  ;B =        0.963  ; C =     0.03609    ; D =      -1.01 ;		      }
  else if (eta <  2.1)               {ptmax =    1169    ; A =       5.783  ;B =       0.6706  ; C =     0.02924    ; D =    -0.8551 ;		      }
  else if (eta <  2.3 && rho < 6.73) {ptmax =   885.3    ; A =       3.541  ;B =       0.4416  ; C =     0.02533    ; D =    -0.7289 ;		      }
  else if (eta <  2.3 && rho < 12.51){ptmax =   872.5    ; A =       4.338  ;B =       0.4735  ; C =     0.02318    ; D =    -0.7473 ;		      }
  else if (eta <  2.3 && rho < 18.29){ptmax =   873.5    ; A =       4.667  ;B =       0.6482  ; C =     0.03168    ; D =    -0.8764 ;		      }
  else if (eta <  2.3 && rho < 24.08){ptmax =   873.3    ; A =       5.345  ;B =       0.6433  ; C =     0.02504    ; D =    -0.8451 ;		      }
  else if (eta <  2.3)               {ptmax =   873.7    ; A =       6.278  ;B =       0.5782  ; C =     0.01875    ; D =    -0.7908 ;		      }
  else if (eta <  2.5 && rho < 6.73) {ptmax =   653.4    ; A =       4.156  ;B =       0.3477  ; C =   1.346e-06    ; D =    -0.6215 ;		      }
  else if (eta <  2.5 && rho < 12.51){ptmax =   651.1    ; A =        4.91  ;B =       0.3751  ; C =   8.908e-07    ; D =    -0.6407 ;		      }
  else if (eta <  2.5 && rho < 18.29){ptmax =   650.5    ; A =       5.655  ;B =       0.3878  ; C =   3.564e-06    ; D =     -0.652 ;		      }
  else if (eta <  2.5 && rho < 24.08){ptmax =   649.1    ; A =       6.178  ;B =        0.469  ; C =   3.809e-06    ; D =    -0.7149 ;		      }
  else if (eta <  2.5)               {ptmax =   846.7    ; A =       6.981  ;B =       0.4503  ; C =   2.365e-07    ; D =    -0.6961 ;		      }
  else if (eta <  2.8 && rho < 6.73) {ptmax =   640.6    ; A =       4.518  ;B =        0.222  ; C =   2.174e-06    ; D =    -0.3943 ;		      }
  else if (eta <  2.8 && rho < 12.51){ptmax =   637.1    ; A =       5.323  ;B =       0.2489  ; C =   9.607e-06    ; D =    -0.4586 ;		      }
  else if (eta <  2.8 && rho < 18.29){ptmax =   637.9    ; A =       6.237  ;B =       0.2938  ; C =   6.557e-07    ; D =    -0.5231 ;		      }
  else if (eta <  2.8 && rho < 24.08){ptmax =   646.7    ; A =       7.236  ;B =       0.2539  ; C =   1.237e-06    ; D =    -0.4661 ;		      }
  else if (eta <  2.8)               {ptmax =   647.2    ; A =       7.935  ;B =       0.3382  ; C =   1.379e-06    ; D =    -0.5699 ;		      }
  else if (eta <  3.0 && rho < 6.73) {ptmax =   444.6    ; A =       4.479  ;B =       0.2032  ; C =   2.553e-05    ; D =    -0.2419 ;		      }
  else if (eta <  3.0 && rho < 12.51){ptmax =     448    ; A =       5.199  ;B =       0.2912  ; C =     6.7e-05    ; D =    -0.4054 ;		      }
  else if (eta <  3.0 && rho < 18.29){ptmax =   457.3    ; A =       5.741  ;B =        0.573  ; C =     0.07489    ; D =    -0.8183 ;		      }
  else if (eta <  3.0 && rho < 24.08){ptmax =   451.7    ; A =       6.859  ;B =       0.3026  ; C =   1.662e-06    ; D =    -0.4132 ;		      }
  else if (eta <  3.0)               {ptmax =   459.2    ; A =       8.357  ;B =       0.1995  ; C =   1.461e-05    ; D =    -0.2497 ;		      }
  else if (eta <  3.2 && rho < 6.73) {ptmax = 299.8      ; A =     4.089    ;B =     0.9805    ; C =    0.1314      ; D =   -1.351   ;		      }
  else if (eta <  3.2 && rho < 12.51){ptmax = 308.3      ; A =  0.006269    ;B =      4.498    ; C =    0.1352      ; D =   -1.848   ;		      }
  else if (eta <  3.2 && rho < 18.29){ptmax = 302.3      ; A =     6.387    ;B =  3.303e-05    ; C =    0.1413      ; D =    -1.41   ;		      }
  else if (eta <  3.2 && rho < 24.08){ptmax = 302.7      ; A =     7.178    ;B =   2.53e-05    ; C =    0.1344      ; D =   -1.247   ;		      }
  else if (eta <  3.2)               {ptmax = 304.7      ; A =     7.821    ;B = -6.745e-06    ; C =    0.1408      ; D =   -1.492   ;		      }
  else if (eta <  4.7 && rho < 6.73) {ptmax =   299.8    ; A =       2.373  ;B =        0.322  ; C =     0.09776    ; D =    -0.8119 ;		      }
  else if (eta <  4.7 && rho < 12.51){ptmax =   292.5    ; A =        3.01  ;B =       0.2254  ; C =     0.08868    ; D =    -0.5196 ;		      }
  else if (eta <  4.7 && rho < 18.29){ptmax =     295    ; A =       3.544  ;B =       0.1874  ; C =   5.446e-05    ; D =    -0.2277 ;		      }
  else if (eta <  4.7 && rho < 24.08){ptmax =   299.5    ; A =       3.381  ;B =        1.114  ; C =      0.1021    ; D =     -1.365 ;		      }
  else                               {ptmax =   297.9    ; A =      -15.14  ;B =        15.21  ; C =      0.1039    ; D =     -1.971 ;                 }

  // this is the absolute resolution (squared), not sigma(pt)/pt
  // so have to multiply by pt^2, thats why D+2 instead of D
  float InvPerr2 =  (A*TMath::Abs(A)) + (B*B)*pow(pt, D+2) + (C*C)*pt*pt;
  return sqrt(InvPerr2);
}

Float_t getJetJERpt(Jet jet, Float_t rho){
  // Hybrid method https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
  Float_t jerScale(1.), genpt(1.), pt(1.), factor1(0.), factor2(0.), sigmaMC(1.), rdm(1.);
  jerScale     = getJERscale(jet, 0);    
  genpt = jet.mcp.Pt(); pt = jet.p.Pt();
  sigmaMC  = getJetPtErr(jet,rho)/pt;

  // Using JER SF 
  factor1  = TMath::Max(Float_t(0.0), genpt + jerScale*(pt - genpt))/pt;
  if(jet.mcp.DeltaR(jet.p) > 0.2 || TMath::Abs(pt - genpt) > 3*pt*sigmaMC) factor1 = -1; // Check matching
  // smearing...
  TRandom3 *fRand3 = new TRandom3(UInt_t(100000*pt));
  rdm  = fRand3->Gaus(0., sigmaMC ); //smear
  // cout << "random number is " << rdm << endl;
  //rdm  = fRand3->Gaus(1., TMath::Sqrt(jerScale*jerScale -1.) * sigmaMC ); //smear
  factor2 = 1 + rdm*TMath::Sqrt(TMath::Max(Float_t(0.0), jerScale*jerScale - 1));
  delete fRand3;
  if(factor1 == -1) return pt*factor2;
  return  pt*factor1;   
}

Float_t getISRJetsWeight(Int_t nISRJet){
  // https://indico.cern.ch/event/592621/contributions/2398559/attachments/1383909/2105089/16-12-05_ana_manuelf_isr.pdf --> Slide 8
  Float_t SF = 0;
  if     (nISRJet == 0) SF = 1;
  else if(nISRJet == 1) SF = 0.920;
  else if(nISRJet == 2) SF = 0.821;
  else if(nISRJet == 3) SF = 0.715;
  else if(nISRJet == 4) SF = 0.652;
  else if(nISRJet == 5) SF = 0.561;
  else                  SF = 0.511;
  return SF;
}

Jet GetMatchedJet(Jet origJet, std::vector<Jet> jetCollection, Float_t etaRange, Float_t ptRange){
  Float_t mieta = origJet.p.Eta();
  Float_t mipt  = origJet.p.Pt();
  Int_t nJet = jetCollection.size();
  Float_t eta = 0; Float_t pt = 0;
  for(Int_t i = 0; i < nJet; i++){
    eta = jetCollection.at(i).Eta();
    pt  = jetCollection.at(i).Pt();
    if(TMath::Abs(eta - mieta) > etaRange && TMath::Abs(pt - mipt) > ptRange) return jetCollection.at(i);
  }
  return Jet(TLorentzVector(0,0,0,0), 0, 0, 0);
}

// ==================================================================
// ========================== Other ================================
// ==================================================================
Float_t p2(Float_t x){
  return (x*x);
}

bool LepMomentumComparator(Lepton i, Lepton j){ 
  return (i.p.Pt()>j.p.Pt()); 
}

bool JetMomentumComparator(Jet i   , Jet    j){
  return (i.p.Pt()>j.p.Pt()); 
}

vector<Lepton> SortLeptonsByPt(vector<Lepton>& Leptons){
  vector<Lepton> theLep = Leptons;
  sort (theLep.begin(), theLep.end(), LepMomentumComparator);
  return theLep;
}

vector<Jet> SortJetsByPt(vector<Jet>& Jets){
  vector<Jet> theJet = Jets;
  sort (theJet.begin(), theJet.end(), JetMomentumComparator);
  return theJet;
}

// Playing with lepton masses...

Float_t ClosestMlltoZ(vector<Lepton> leptons){
  Int_t nLeps = leptons.size(); Float_t mll = 0; Float_t best_mll = 0;
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          mll = (leptons.at(i).p + leptons.at(j).p).M(); if(best_mll == 0) best_mll = mll; 
          if( TMath::Abs(mll - Zm) < TMath::Abs(best_mll - Zm) ) best_mll = mll;
        }
      }
    }
  } 
  return best_mll;
}

Bool_t hasOSSF(vector<Lepton> leptons){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          return true;
        }
      }
    }
  } 
  return false;
}

Bool_t has2OSSFwMlmm(vector<Lepton> lepton, Float_t mm) {
  if (lepton.size() < 4) return false;
  vector<TLorentzVector> OSSFpair;
  vector<UInt_t> OSSFpair1;
  vector<UInt_t> OSSFpair2;
  OSSFpair	  = vector<TLorentzVector>();
  OSSFpair1	  = vector<UInt_t>();
  OSSFpair2	  = vector<UInt_t>();
  
  for (UInt_t i = 0; i < lepton.size(); i++) {
    for (UInt_t j = i+1; j < lepton.size(); j++) {
      if (lepton.at(i).type != lepton.at(j).type)      continue;
      if (lepton.at(i).charge*lepton.at(j).charge > 0) continue;
      OSSFpair.push_back(lepton.at(i).p+lepton.at(j).p);
      OSSFpair1.push_back(i);
      OSSFpair2.push_back(j);
    }
  }
  
  if (OSSFpair.size() < 2) return false;
  for (UInt_t i = 0; i < OSSFpair.size(); i++) {
    for (UInt_t j = i+1; j < OSSFpair.size(); j++) {
      if ((OSSFpair1.at(i) == OSSFpair1.at(j)) || 
          (OSSFpair1.at(i) == OSSFpair2.at(j)) || 
          (OSSFpair2.at(i) == OSSFpair2.at(j)))         continue;
      if ((OSSFpair.at(i) + OSSFpair.at(j)).M() < mm)   return true;
    }
  }
  
  return false;
}


Bool_t IsOnZ(vector<Lepton> leptons){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 1){ // opposite sign
          if( TMath::Abs( (leptons.at(i).p + leptons.at(j).p).M() - Zm) < 15){ // on Z
            return true;
          }
        }
      }
    }
  } 
  return false;
}

Bool_t PassLowInvMass(vector<Lepton> leptons, Float_t Mll_max){
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).type == leptons.at(j).type){ // same flavour
        if(leptons.at(i).charge*leptons.at(j).charge < 0){ // opposite sign
          if( (leptons.at(i).p + leptons.at(j).p).M()  < Mll_max){
            return false;
          }
        }
      }
    }
  } 
  return true;
}

Bool_t PassesLowMassLimit(vector<Lepton> lepton, Float_t mm_max) {
  if(lepton.size() < 2) return false;
  for(UInt_t i = 0; i < lepton.size(); i++) {
    for(UInt_t j = i+1; j < lepton.size(); j++) {
      if((lepton.at(i).p + lepton.at(j).p).M() < mm_max) return false;
    }
  }
  return true;
}

Bool_t IsThereSSpair(vector<Lepton> leptons){
  //  return true;
  Int_t nLeps = leptons.size();
  if(nLeps < 2) return false;
  for(Int_t i = 0; i < nLeps; i++){
    for(Int_t j = 0; j < i; j++){
      if(leptons.at(i).charge*leptons.at(j).charge > 0){
        return true;
      }
    }
  }
  return false;
}

Bool_t IsThere3SS(vector<Lepton> leptons){
  Int_t nLeps = leptons.size();
  if(nLeps < 3) return false;
  for(Int_t i = 2; i < nLeps; i++){
    for(Int_t j = 1; j < i; j++){
      for(Int_t k = 0; k < j; k++){
        if( (leptons.at(i).charge == leptons.at(j).charge) && (leptons.at(k).charge == leptons.at(j).charge)){
          return true;
        }
      }
    }
  }
  return false;
}

Int_t getCS(vector<Lepton> lepton) { // Get the sum of charges of a vector of Lepton objects.
  Int_t cs = 0;
  for (UInt_t i = 0; i < lepton.size(); i++) cs += lepton.at(i).charge;
  
  return cs;
}

void co(TString out, TString co = "1;30"){
  std::cout << "\033[" << co << "m" << out << "\033[0m" << endl;
}

void DumpVar(Int_t evt, TString varname, Float_t val, Bool_t pass, Bool_t verbose){
  if(!verbose) return;
  Int_t ee; Int_t i = 0;
  for(Int_t i = 0; i < nExploredEvents; i++){
    ee = ExploredEvents[i];

    if(evt == ee){
      if(pass){
        if(verbose) cout << Form("[OK : %d] ", evt) << varname << Form(" = %2.5f\n", val);
      }
      if(!pass) cout << Form("[NO PASA : %d] ", evt) << varname << Form(" = %2.5f\n", val);
    } 
  }
}

void DumpEvent(Int_t evt, TString s, Bool_t verbose){
  if(!verbose) return;
  Int_t ee; Int_t i = 0;
  for(Int_t i = 0; i < nExploredEvents; i++){
    ee = ExploredEvents[i];
    if(evt == ee) cout << s << endl;
  }
}
