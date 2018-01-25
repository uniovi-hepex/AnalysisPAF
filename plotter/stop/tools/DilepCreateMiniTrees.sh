resetpaf -a
#root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"   ,"StopDilep", 30)'
#root -l -b -q 'RunAnalyserPAF.C("TTbar_PowhegLHE"   ,"StopDilep", 30)'
resetpaf -a
resetpaf -a

root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_aMCatNLO", "StopDilep", 5)'
root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2", "StopDilep", 5)'
root -l -b -q 'RunAnalyserPAF.C("ZZ_ext"   ,"StopDilep", 5)'
root -l -b -q 'RunAnalyserPAF.C("WW_ext"   ,"StopDilep", 5)'
root -l -b -q 'RunAnalyserPAF.C("WZ_ext"   ,"StopDilep", 5)'
resetpaf -a
resetpaf -a
root -l -b -q 'RunAnalyserPAF.C("TW_ext"   ,"StopDilep", 8)'
root -l -b -q 'RunAnalyserPAF.C("TbarW_ext","StopDilep", 8)'
resetpaf -a
resetpaf -a

root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext2"    ,"StopDilep", 4)'
root -l -b -q 'RunAnalyserPAF.C("TTZToQQ"   ,"StopDilep", 4)'
root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext", "StopDilep",4)'
root -l -b -q 'RunAnalyserPAF.C("TTWToQQ"   , "StopDilep",4)'
resetpaf -a
resetpaf -a

#root -l -b -q 'RunAnalyserPAF.C("TTGJets"   , "StopDilep",5)'

root -l -b -q 'RunAnalyserPAF.C("MuonEG"    , "StopDilep",40, -2)'
root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "StopDilep",50, -6)'
root -l -b -q 'RunAnalyserPAF.C("DoubleEG"  , "StopDilep",50, -6)'
resetpaf -a
resetpaf -a
root -l -b -q 'RunAnalyserPAF.C("SingleElec"  , "StopDilep",50)'
root -l -b -q 'RunAnalyserPAF.C("SingleMuon"  , "StopDilep",50)'
resetpaf -a
resetpaf -a

root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM",  "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2"  , "StopDilep",50, -6)'
resetpaf -a
resetpaf -a

root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext",  "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_aMCatNLO",  "StopDilep",30, -6)'
resetpaf -a
resetpaf -a


root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_ueUp"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_ueDown"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_isrUp_ext"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_isrDown"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_fsrUp"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_fsrDown"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_Herwig_ext2"  , "StopDilep",30)'
root -l -b -q 'RunAnalyserPAF.C("TTJets_aMCatNLO"  , "StopDilep",30)'
resetpaf -a
resetpaf -a
