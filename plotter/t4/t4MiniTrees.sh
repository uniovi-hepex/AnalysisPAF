# ====================
# FAKES
# ====================

# DY, WJets
root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2"            , "4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM"                           , "4t", 20)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2", "4t", 20)'

# ttbar, single-t
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"           ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("T_tch"                  ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("Tbar_tch"               ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TToLeptons_sch_amcatnlo","4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr & TbarW_noFullyHadr_ext & TbarW_noFullyHadr_ext2"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr & TW_noFullyHadr_ext & TW_noFullyHadr_ext2"            ,"4t", 30)'


# Inclusive samples, not dileptonic...

# ====================
# PROMPT
# ====================
# ttX
root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext1 & TTWToLNu_ext2"     ,"4t",   8)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2", "4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLL_M1to10", "4t",8)' # TTZToLL_M-1to10_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("TTHNonbb"                          , "4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("tZq_ll"                            , "4t", 1, -6)'

#Di and tri bosons
root -l -b -q 'RunAnalyserPAF.C("ZZTo4L"   ,   "4t", 8 )' #root -l -b -q 'RunAnalyserPAF.C("ZZ & ZZ_ext"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WZTo3LNu"   , "4t", 8 )' #root -l -b -q 'RunAnalyserPAF.C("WZ & WZ_ext"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu"   ,"4t", 30)' #root -l -b -q 'RunAnalyserPAF.C("WW & WW_ext"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("ZZZ"      ,"4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("WZZ"      ,"4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("WWZ"      ,"4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("WWW"      ,"4t",  8)'
root -l -b -q 'RunAnalyserPAF.C("WpWpJJ"   ,"4t", 30, -4)'
root -l -b -q 'RunAnalyserPAF.C("VHToNonbb_amcatnlo"   ,"4t", 5)' # VHToNonbb M125 13TeV amcatnloFXFX madspin pythia8
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu_DoubleScat"   ,"4t", 5)' # WW DoubleScattering 13TeV-pythia8

# ====================
# CONVERSIONS
# ====================
root -l -b -q 'RunAnalyserPAF.C("TGJets & TGJets_ext"  , "4t",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("TTGJets & TTGJets_ext", "4t",30   )'
root -l -b -q 'RunAnalyserPAF.C("WGToLNuG"             , "4t",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("ZGTo2LG"              , "4t",8, -4)'
root -l -b -q 'RunAnalyserPAF.C("WZG_amcatnlo"   , "4t",8, -4)' # WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("WWG_amcatnlo"   , "4t",8)' # WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8

# ====================
# SIGNAL
# ====================
root -l -b -q 'RunAnalyserPAF.C("TTTT"   , "4t",8, -4)'

# ====================
# DATA
# ====================
root -l -b -q 'RunAnalyserPAF.C("MuonEG"    , "4t",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "4t",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleEG"  , "4t",40)'

