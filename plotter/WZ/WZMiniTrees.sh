# ====================
# FAKES
# ====================

# DY, WJets
root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2"            , "WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM"                           , "WZ", 20)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2", "WZ", 20)'

# ttbar, single-t
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"           ,"WZ", 30)'
root -l -b -q 'RunAnalyserPAF.C("T_tch"                  ,"WZ", 30)'
root -l -b -q 'RunAnalyserPAF.C("Tbar_tch"               ,"WZ", 30)'
root -l -b -q 'RunAnalyserPAF.C("TToLeptons_sch_amcatnlo","WZ", 30)'
root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr & TbarW_noFullyHadr_ext & TbarW_noFullyHadr_ext2"   ,"WZ", 30)'
root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr & TW_noFullyHadr_ext & TW_noFullyHadr_ext2"            ,"WZ", 30)'


# Inclusive samples, not dileptonic...

# ====================
# PROMPT
# ====================
# ttX
root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext1 & TTWToLNu_ext2"     ,"WZ",   8)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2", "WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLL_M1to10", "WZ",8)' # TTZToLL_M-1to10_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("TTHNonbb"                          , "WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("tZq_ll"                            , "WZ", 1, -6)'

#Di and tri bosons
root -l -b -q 'RunAnalyserPAF.C("ZZTo4L"   ,   "WZ", 8 )' #root -l -b -q 'RunAnalyserPAF.C("ZZ & ZZ_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("WZTo3LNu"   , "WZ", 8 )' #root -l -b -q 'RunAnalyserPAF.C("WZ & WZ_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu"   ,"WZ", 30)' #root -l -b -q 'RunAnalyserPAF.C("WW & WW_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("ZZZ"      ,"WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("WZZ"      ,"WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("WWZ"      ,"WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("WWW"      ,"WZ",  8)'
root -l -b -q 'RunAnalyserPAF.C("WpWpJJ"   ,"WZ", 30, -4)'
root -l -b -q 'RunAnalyserPAF.C("VHToNonbb_amcatnlo"   ,"WZ", 5)' # VHToNonbb M125 13TeV amcatnloFXFX madspin pythia8
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu_DoubleScat"   ,"WZ", 5)' # WW DoubleScattering 13TeV-pythia8

# ====================
# CONVERSIONS
# ====================
root -l -b -q 'RunAnalyserPAF.C("TGJets & TGJets_ext"  , "WZ",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("TTGJets & TTGJets_ext", "WZ",30   )'
root -l -b -q 'RunAnalyserPAF.C("WGToLNuG"             , "WZ",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("ZGTo2LG"              , "WZ",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("WZG_amcatnlo"   , "WZ",8, -4)' # WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("WWG_amcatnlo"   , "WZ",8)' # WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8

# ====================
# SIGNAL
# ====================
root -l -b -q 'RunAnalyserPAF.C("TTTT"   , "WZ",8, -4)'

# ====================
# DATA
# ====================
root -l -b -q 'RunAnalyserPAF.C("MuonEG"    , "WZ",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "WZ",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleEG"  , "WZ",40)'

