#ifndef TRIGGER_SF_H
#define TRIGGER_SF_H 1

#include <math.h>
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
