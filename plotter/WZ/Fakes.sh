# ====================
# FAKES
# ====================

# DY, WJets
root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2"            , "WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM"                           , "WZ", 40)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2", "WZ", 60)'

# ttbar, single-t
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"           ,"WZ", 60)'
root -l -b -q 'RunAnalyserPAF.C("T_tch"                  ,"WZ", 60)'
root -l -b -q 'RunAnalyserPAF.C("Tbar_tch"               ,"WZ", 60)'
root -l -b -q 'RunAnalyserPAF.C("TToLeptons_sch_amcatnlo","WZ", 60)'
root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr & TbarW_noFullyHadr_ext & TbarW_noFullyHadr_ext2"   ,"WZ", 60)'
root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr & TW_noFullyHadr_ext & TW_noFullyHadr_ext2"            ,"WZ", 60)'
