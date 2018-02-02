
#root -l -b -q 'RunAnalyserPAF.C("T2tt_175_1_FS","StopTop",30, -2)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_200_50_FS_summer","StopTop",40)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_225_50_FS_summer","StopTop",40)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_250_50_FS_summer","StopTop",40)'
#root -l -b -q 'RunAnalyserPAF.C("Scan:T2tt_mStop160to210mLSP1to20", "StopTop", 20, -4, 0, 1.0, "T2tt:[175,1]")'
#root -l -b -q 'RunAnalyserPAF.C("TTJets_MLM"   ,"StopTop", 20, -4)'
#root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg"   ,"StopTop", 30, -10)'

#root -l -b -q 'RunAnalyserPAF.C("T2tt_200_50_FS_xqcut20","StopTop",30)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_225_50_FS_xqcut20","StopTop",30)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_250_50_FS_xqcut20","StopTop",30)'
#root -l -b -q 'RunAnalyserPAF.C("T2tt_175_1_FS_xqcut20","StopTop",30)'

#rootx 'RunAnalyserPAF.C("TTbar_Powheg_mtop1665", "StopTop", 30, -4)'
rootx 'RunAnalyserPAF.C("TTbar_Powheg_mtop1715", "StopTop", 30, -4)'
rootx 'RunAnalyserPAF.C("TTbar_Powheg_mtop1735", "StopTop", 30, -4)'
rootx 'RunAnalyserPAF.C("TTbar_Powheg_mtop1785", "StopTop", 30, -4)'


