# ====================
# PROMPT
# ====================
# ttX
root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext1 & TTWToLNu_ext2"     ,"WZ",   16)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2", "WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLL_M1to10", "WZ",8)' # TTZToLL_M-1to10_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("TTHNonbb"                          , "WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("tZq_ll"                            , "WZ", 1, -6)'
root -l -b -q 'RunAnalyserPAF.C("TTTT"   , "WZ",16, -4)'

#Di and tri bosons
root -l -b -q 'RunAnalyserPAF.C("ZZTo4L"   ,   "WZ", 16 )' #root -l -b -q 'RunAnalyserPAF.C("ZZ & ZZ_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("WZTo3LNu"   , "WZ", 16 )' #root -l -b -q 'RunAnalyserPAF.C("WZ & WZ_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu"   ,"WZ", 60)' #root -l -b -q 'RunAnalyserPAF.C("WW & WW_ext"   ,"WZ", 8)'
root -l -b -q 'RunAnalyserPAF.C("ZZZ"      ,"WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("WZZ"      ,"WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("WWZ"      ,"WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("WWW"      ,"WZ",  16)'
root -l -b -q 'RunAnalyserPAF.C("WpWpJJ"   ,"WZ", 60, -4)'
root -l -b -q 'RunAnalyserPAF.C("VHToNonbb_amcatnlo"   ,"WZ", 20)' # VHToNonbb M125 13TeV amcatnloFXFX madspin pythia8
root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu_DoubleScat"   ,"WZ", 20)' # WW DoubleScattering 13TeV-pythia8

