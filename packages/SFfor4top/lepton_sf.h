#ifndef LEPTON_SF_H
#define LEPTON_SF_H 1

#include <math.h>

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

#endif
