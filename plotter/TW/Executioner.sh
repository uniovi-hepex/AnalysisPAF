if [ "$1" == "an" ]; then
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TW ANALYSIS EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Setting up the environment"
  source pre_start.sh

  echo "%%%%%> DONE"
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting analysis"
  
  cd ../..
  
  sel="TW"
  
  if [ "$3" != "" ]; then
    sel=$3
  fi
  
  runsamples=("TTbar_Powheg" "TW_ext" "TbarW_ext" "DYJetsToLL_M50_aMCatNLO" "DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext" "WJetsToLNu_aMCatNLO" 
    "WJetsToLNu_MLM & WJetsToLNu_MLM_ext2" "ZZ & ZZ_ext" "WW & WW_ext" "WZ & WZ_ext" "TTWToLNu_ext1 & TTWToLNu_ext2" "TTZToQQ" "TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2" "TTWToQQ" "TTGJets" 
    "MuonEG" "SingleElec" "SingleMuon")
  runsamples_syst=("TTbar_Powheg_ueUp" "TTbar_Powheg_ueDown" "TTbar_Powheg_isrUp_ext" "TTbar_Powheg_isrDown" "TTbar_Powheg_fsrUp" "TTbar_Powheg_fsrDown" 
    "TTbar_Powheg_Herwig_ext2" "TTJets_aMCatNLO" "TTbar_Powheg_hdampUp & TTbar_Powheg_hdampUp_ext" "TTbar_Powheg_hdampDown & TTbar_Powheg_hdampDown_ext" "TW_DS" "TbarW_DS" 
    "TW_noFullyHadr_isrUp" "TW_noFullyHadr_isrDown" "TW_noFullyHadr_fsrUp" "TW_noFullyHadr_fsrDown" "TW_noFullyHadr_MEscaleUp" "TW_noFullyHadr_MEscaleDown" "TbarW_noFullyHadr_isrUp" 
    "TbarW_noFullyHadr_isrDown" "TbarW_noFullyHadr_fsrUp" "TbarW_noFullyHadr_fsrDown" "TbarW_noFullyHadr_MEscaleUp" "TbarW_noFullyHadr_MEscaleDown" 
    "TTbar_Powheg_mtop1665" "TTbar_Powheg_mtop1695_backup & TTbar_Powheg_mtop1695_ext & TTbar_Powheg_mtop1695_ext2" "TTbar_Powheg_mtop1735" 
    "TTbar_Powheg_mtop1755 & TTbar_Powheg_mtop1755_ext & TTbar_Powheg_mtop1755_ext2" "TTbar_Powheg_mtop1785" "TTbar_Powheg_erdON & TTbar_Powheg_erdON_ext" 
    "TTbar_QCDbasedCRTune_erdON & TTbar_QCDbasedCRTune_erdON_ext" "TTbar_GluonMoveCRTune" "TTbar_GluonMoveCRTune_erdON" "TW_noFullyHadr_mtop1695" "TW_noFullyHadr_mtop1755" 
    "TbarW_noFullyHadr_mtop1695" "TbarW_noFullyHadr_mtop1755" "TW_noFullyHadr_herwigpp" "TbarW_noFullyHadr_herwigpp")
  
  uplimit=$((${#runsamples[@]}-1))
  uplimit_syst=$((${#runsamples_syst[@]}-1))
  
  ncores=("30" "20" "20" "20" "20" "8" 
    "8" "8" "8" "8" "8" "8" "8" "8" "5" 
    "30" "30" "30")
  ncores_syst=("20" "20" "20" "20" "20" "20" 
    "20" "20" "20" "20" "20" "20" 
    "20" "20" "20" "20" "30" "30" "20" 
    "20" "20" "20" "30" "30" 
    "20" "20" "20" 
    "20" "20" "20" 
    "20" "20" "20" "20" "20" 
    "20" "20" "20" "20")
  
  if [ "$2" == "opt" ]; then
    
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Running general-purpose samples..."
    
    for ((i=0; i<=$uplimit; i++)); do
      root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", ${ncores[i]})"
      resetpaf -a
    done
    
#     echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Running samples for systematic uncertanties..."
#     
#     for ((i=0; i<=$uplimit; i++)); do
#       root -l -b -q "RunAnalyserPAF.C(\"${runsamples_syst[i]}\", \"$sel\", ${ncores_syst[i]})"
#       resetpaf -a
#     done
#     
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree.root\", \"$sel\", 20, 0, 0, 831.76)"
#     resetpaf -a
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TW.root\", \"$sel\", 20, 0, 0, 35.85)"
#     resetpaf -a
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TbarW.root\", \"$sel\", 20, 0, 0, 35.85)"
#     resetpaf -a
  
  else
    
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Running general-purpose samples..."
    
    for ((i=0; i<=$uplimit; i++)); do
      root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $2)"
      resetpaf -a
    done
    
#     echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Running samples for systematic uncertanties..."
#     
#     for ((i=0; i<=$uplimit; i++)); do
#       root -l -b -q "RunAnalyserPAF.C(\"${runsamples_syst[i]}\", \"$sel\", $2)"
#       resetpaf -a
#     done
#     
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree.root\", \"$sel\", $2, 0, 0, 831.76)"
#     resetpaf -a
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TW.root\", \"$sel\", $2, 0, 0, 35.85)"
#     resetpaf -a
#     root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TbarW.root\", \"$sel\", $2, 0, 0, 35.85)"
#     resetpaf -a
  fi
  
elif [ "$1" == "pl" ]; then
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TW PLOTTER EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  echo ""
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Remaking libraries..."  
  source RemakeLibraries.sh
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting to plot"
  source PlotThings.sh
  
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi

cd plotter/TW
