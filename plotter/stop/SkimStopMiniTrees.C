R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(TResultsTable.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "SkimMiniTrees.C"

TString path = "/pool/ciencias/userstorage/juanr/stop/apr2018/";
TString outdir = path + "/ElMu/";
TString samplesSS = "TTbar_Powheg,DYJetsToLL_M50_aMCatNLO, DYJetsToLL_M10to50_aMCatNLO,TW, TbarW, WZ, WW, ZZ, TTWToLNu, TTWToQQ, TTZToQQ, TTZToLLNuNu";
TString datasamples = "MuonEG, SingleMuon, SingleElec, DoubleEG, DoubleMuon";
TString AllSamples = "TTbar_Powheg_Semi, ZZ,  TTbar_Powheg, WW,  WZ,  TW,  TbarW,  TTWToLNu,  TTZToLLNuNu,  TTZToQQ,  TTWToQQ,  WGToLNuG,  DYJetsToLL_M5to50_MLM,  DYJetsToLL_M10to50_aMCatNLO,  DYJetsToLL_M50_aMCatNLO,  DYJetsToLL_M50_MLM,  DoubleEG,  DoubleMuon,  MuonEG,  SingleElec,  SingleMuon,  T2tt_mStop175_mLsp7,  TTbar_Powheg_ueUp,  TTbar_Powheg_ueDown,  TTbar_Powheg_hdampUp,  TTbar_Powheg_hdampDown,  TTbar_Powheg_isrUp,  TTbar_Powheg_isrDown,  TTbar_Powheg_mtop1715,  TTbar_Powheg_mtop1735,  TTbar_Powheg_erdON,  TTbar_GluonMoveCRTune,  TTbar_QCDbasedCRTune_erdON,  TTbar_GluonMoveCRTune_erdON,  TTbar_Powheg_fsrDown,  TTbar_Powheg_fsrUp";
TString AllSignals = "T2tt_mStop167_mLsp0, T2tt_mStop175_mLsp0, T2tt_mStop175_mLsp7, T2tt_mStop182_mLsp0, T2tt_mStop182_mLsp15, T2tt_mStop182_mLsp7, T2tt_mStop190_mLsp15, T2tt_mStop190_mLsp22, T2tt_mStop190_mLsp7, T2tt_mStop197_mLsp15, T2tt_mStop197_mLsp22, T2tt_mStop197_mLsp30, T2tt_mStop205_mLsp22, T2tt_mStop205_mLsp30, T2tt_mStop205_mLsp37, T2tt_mStop212_mLsp30, T2tt_mStop212_mLsp37, T2tt_mStop212_mLsp45, T2tt_mStop220_mLsp37, T2tt_mStop220_mLsp45, T2tt_mStop220_mLsp52, T2tt_mStop227_mLsp45, T2tt_mStop227_mLsp52, T2tt_mStop227_mLsp60, T2tt_mStop235_mLsp52, T2tt_mStop235_mLsp60, T2tt_mStop235_mLsp67, T2tt_mStop242_mLsp60, T2tt_mStop242_mLsp67, T2tt_mStop242_mLsp75"; 

TString cut = "TChannel == 1";

void SkimStopMiniTrees(){
//  SkimMiniTrees(path,                       AllSamples,  path + "/ElMu/",                        "TChannel == 1");
//  SkimMiniTrees(path + "/Full2016Dataset/", datasamples, path + "/ElMu/Full2016Dataset/",        "TChannel == 1");
  SkimMiniTrees(path + "/T2tt/",            AllSignals,  path + "/ElMu/T2tt/",                   "TChannel == 1");
//  SkimMiniTrees(path,                       samplesSS,   path + "/ElMu/SkimSS/",                 "TChannel == 1 && TIsSS");
//  SkimMiniTrees(path + "/Full2016Dataset/", datasamples, path + "/ElMu/SkimSS/Full2016Dataset/", "TChannel == 1 && TIsSS");
}


