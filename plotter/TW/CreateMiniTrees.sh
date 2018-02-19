  cd ../..
  
  root6
  source /opt/PAF/PAF_setup.sh
  
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg","TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_ext"      ,"TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_ext"   ,"TW", 20)'
  resetpaf -a
  
  root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_aMCatNLO",  "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext"  , "TW",20)'
  resetpaf -a
  
  root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_aMCatNLO", "TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2", "TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("ZZ & ZZ_ext"   ,"TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("WW & WW_ext"   ,"TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("WZ & WZ_ext"   ,"TW", 8)'
  resetpaf -a
  
  root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext2"    ,"TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTZToQQ"   ,"TW", 8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2", "TW",8)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTWToQQ"   , "TW",8)'
  resetpaf -a
  
  root -l -b -q 'RunAnalyserPAF.C("TTGJets"   , "TW",5)'
  resetpaf -a
  
  root -l -b -q 'RunAnalyserPAF.C("MuonEG"    , "TW",10)'
  resetpaf -a
  
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("SingleElec"  , "TW",50)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("SingleMuon"  , "TW",50)'
  resetpaf -a

#  return
#  resetpaf -a
#  resetpaf -a

#  root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "TW",10)'
#  root -l -b -q 'RunAnalyserPAF.C("DoubleEG"  , "TW",10)'


  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_ueUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_ueDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_isrUp_ext"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_isrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_fsrUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_fsrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_Herwig_ext2"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTJets_aMCatNLO"  , "TW",20)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_hdampUp & TTbar_Powheg_hdampUp_ext"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_hdampDown & TTbar_Powheg_hdampDown_ext"  , "TW",20)'
  resetpaf -a



  root -l -b -q 'RunAnalyserPAF.C("TW_DS"   ,"TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_DS","TW", 20)'
  resetpaf -a



  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_isrUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_isrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_fsrUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_fsrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_MEscaleUp"     , "TW",30)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_MEscaleDown"   , "TW",30)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_isrUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_isrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_fsrUp"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_fsrDown"  , "TW",20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_MEscaleUp"     , "TW",30)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_MEscaleDown"   , "TW",30)'
  resetpaf -a



  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_mtop1665", "TW", 20)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_mtop1695_backup & TTbar_Powheg_mtop1695_ext & TTbar_Powheg_mtop1695_ext2", "TW", 20)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_mtop1735", "TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_mtop1755 & TTbar_Powheg_mtop1755_ext & TTbar_Powheg_mtop1755_ext2", "TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_mtop1785", "TW", 20)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg_erdON & TTbar_Powheg_erdON_ext", "TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_QCDbasedCRTune_erdON & TTbar_QCDbasedCRTune_erdON_ext", "TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_GluonMoveCRTune", "TW", 20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TTbar_GluonMoveCRTune_erdON", "TW", 20)'
  resetpaf -a



  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_mtop1695"   , "TW" ,20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_mtop1755"   , "TW" ,20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_mtop1695", "TW" ,20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_mtop1755", "TW" ,20)'
  resetpaf -a


  root -l -b -q 'RunAnalyserPAF.C("TW_noFullyHadr_herwigpp", "TW" ,20)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("TbarW_noFullyHadr_herwigpp", "TW" ,20)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree.root", "TW" ,20,0,0,831.76)'
  resetpaf -a

  root -l -b -q 'RunAnalyserPAF.C("LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TW.root", "TW", 20, 0,0, 35.85)'
  resetpaf -a
  root -l -b -q 'RunAnalyserPAF.C("LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TbarW.root", "TW", 20, 0,0, 35.85)'
  resetpaf -a
