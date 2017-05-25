#ifndef SFfor4t_H
#define SFfor4t_H 1

#include <math.h>
#include <TMath.h>

//class SFfot4t{ //  SFfor4t(){}; //  ~SFfor4t(){}; //};
  
float electronScaleFactorHighHT(float pt, float eta);
float electronScaleFactorLowHT(float pt, float eta);
float muonScaleFactor(float pt, float eta);
float electronGSF(float pt, float eta);
float trackingSF(float eta);
float electronScaleFactorHighHT_error(float pt, float eta);
float electronScaleFactorLowHT_error(float pt, float eta);
float muonScaleFactor_error(float pt, float eta);
float leptonScaleFactor(int pdgId, float pt, float eta, float ht);
float leptonScaleFactor_err(int pdgId, float pt, float eta, float ht);
float eventScaleFactor_leponly(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht);

double Ele23_Ele12_LeadingLeg_SF(double pt, double eta);
double Ele23_Ele12_TrailingLeg_SF(double pt, double eta);
double Ele23_Ele12_DZLeg(double eta1, double eta2);
double Mu17_Mu8orTkMu8_LeadingLeg_SF(double pt, double eta);
double Mu17_Mu8orTkMu8_DZLeg(double eta1, double eta2); 
double Mu23_Ele8_LeadingLeg_SF(double pt, double eta);
double Mu23_Ele8_TrailingLeg_SF(double pt, double eta);
double Mu8_Ele23_LeadingLeg_SF(double pt, double eta);
double MuX_EleY_DZLeg(double eta_e, double eta_mu);
double DoubleMu8_PFHT300_MuLeg_SF(double pt, double eta);
double DoubleEle8_PFHT300_EleLeg_SF(double pt, double eta);
double Mu8Ele8_PFHT300_MuLeg_SF(double pt, double eta);
double Mu8Ele8_PFHT300_EleLeg_SF(double pt, double eta);
double HTLeg_SF(int lep1_pdgId, double lep1_pt, int lep2_pdgId, double lep2_pt,  double HT);
double TotalTriggerSF(int lep1_pdgId, double lep1_pt, double lep1_eta, int lep2_pdgId, double lep2_pt, double lep2_eta, double HT);

#endif
