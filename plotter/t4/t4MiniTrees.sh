# ====================
# FAKES
# ====================
# DY, WJets
root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2", "4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM",  "4t",30)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2"  , "4t",50)'
# ttbar, single-t
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("T_tch"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("Tbar_tch"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TToLeptons_sch_amcatnlo"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TW"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("TbarW"   ,"4t", 30)'
# Inclusive samples, not dileptonic...

# ====================
# PROMPT
# ====================
# ttX
#root -l -b -q 'RunAnalyserPAF.C("TTZToQQ"   ,"4t", 8)'
#root -l -b -q 'RunAnalyserPAF.C("TTWToQQ"   , "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext1 & TTWToLNu_ext2"    ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext & TTZToLLNuNu_ext2", "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("TTHNonbb"   , "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("tZq_ll"   , "4t",40)'
# TTZToLL_M-1to10_TuneCUETP8M1_13TeV-amcatnlo-pythia8
#Di and tri bosons
#root -l -b -q 'RunAnalyserPAF.C("WW & WW_ext"   ,"4t", 8)'
#root -l -b -q 'RunAnalyserPAF.C("WZ & WZ_ext"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("ZZ & ZZ_ext"   ,"4t", 8)'
# ZZTo4L 13TeV powheg pythia8
root -l -b -q 'RunAnalyserPAF.C("WZTo3LNu"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu"   ,"4t", 30)'
root -l -b -q 'RunAnalyserPAF.C("ZZZ"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WZZ"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWZ"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWW"   ,"4t", 8)'
root -l -b -q 'RunAnalyserPAF.C("WpWpJJ"   ,"4t", 30)'
# WW DoubleScattering 13TeV-pythia8
# VHToNonbb M125 13TeV amcatnloFXFX madspin pythia8

# ====================
# CONVERSIONS
# ====================
root -l -b -q 'RunAnalyserPAF.C("TGJets & TGJets_ext"   , "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("TTGJets & TTGJets_ext"   , "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("WGToLNuG"   , "4t",8)'
root -l -b -q 'RunAnalyserPAF.C("ZGTo2LG"   , "4t",8)'
# WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8
# WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8

# ====================
# SIGNAL
# ====================
root -l -b -q 'RunAnalyserPAF.C("TTTT"   , "4t",8)'

# ====================
# DATA
# ====================
root -l -b -q 'RunAnalyserPAF.C("MuonEG"    , "4t",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "4t",40)'
root -l -b -q 'RunAnalyserPAF.C("DoubleEG"  , "4t",40)'


