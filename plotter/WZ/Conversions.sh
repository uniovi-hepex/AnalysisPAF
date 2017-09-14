# ====================
# CONVERSIONS
# ====================
root -l -b -q 'RunAnalyserPAF.C("TGJets & TGJets_ext"  , "WZ",16, -4)'
root -l -b -q 'RunAnalyserPAF.C("TTGJets & TTGJets_ext", "WZ",30   )'
root -l -b -q 'RunAnalyserPAF.C("WGToLNuG"             , "WZ",16, -4)'
root -l -b -q 'RunAnalyserPAF.C("ZGTo2LG"              , "WZ",16, -4)'
root -l -b -q 'RunAnalyserPAF.C("WZG_amcatnlo"   , "WZ",16, -4)' # WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8
root -l -b -q 'RunAnalyserPAF.C("WWG_amcatnlo"   , "WZ",16)' # WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8
