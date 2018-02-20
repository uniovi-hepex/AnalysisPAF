source pre_start.sh

cd ../..

slash="/"
allok=0

# Minimum size (in bytes!):
minimumsize=50000

init="Tree_"
final=".root"

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Number of total samples: 60

samples=("TTbar_Powheg" "TW" "TbarW" "DYJetsToLL_M50_aMCatNLO" "DYJetsToLL_M10to50_aMCatNLO" "WJetsToLNu_aMCatNLO" 
  "WJetsToLNu_MLM" "ZZ" "WW" "WZ" "TTWToLNu" "TTZToQQ" "TTZToLLNuNu" "TTWToQQ" "TTGJets" 
  "MuonEG" "SingleElec" "SingleMuon" 
  "TTbar_Powheg_ueUp" "TTbar_Powheg_ueDown" "TTbar_Powheg_isrUp" "TTbar_Powheg_isrDown" "TTbar_Powheg_fsrUp" "TTbar_Powheg_fsrDown" 
  "TTbar_Powheg_Herwig_ext2" "TTJets_aMCatNLO" "TTbar_Powheg_hdampUp" "TTbar_Powheg_hdampDown" "TW_DS" "TbarW_DS" 
  "TW_noFullyHadr_isrUp" "TW_noFullyHadr_isrDown" "TW_noFullyHadr_fsrUp" "TW_noFullyHadr_fsrDown" "TW_noFullyHadr_MEscaleUp" "TW_noFullyHadr_MEscaleDown" "TbarW_noFullyHadr_isrUp" 
  "TbarW_noFullyHadr_isrDown" "TbarW_noFullyHadr_fsrUp" "TbarW_noFullyHadr_fsrDown" "TbarW_noFullyHadr_MEscaleUp" "TbarW_noFullyHadr_MEscaleDown" 
  "TTbar_Powheg_mtop1665" "TTbar_Powheg_mtop1695_backup" "TTbar_Powheg_mtop1735" 
  "TTbar_Powheg_mtop1755" "TTbar_Powheg_mtop1785" "TTbar_Powheg_erdON" 
  "TTbar_QCDbasedCRTune_erdON" "TTbar_GluonMoveCRTune" "TTbar_GluonMoveCRTune_erdON" "TW_noFullyHadr_mtop1695" "TW_noFullyHadr_mtop1755" 
  "TbarW_noFullyHadr_mtop1695" "TbarW_noFullyHadr_mtop1755" "TW_noFullyHadr_herwigpp" "TbarW_noFullyHadr_herwigpp" 
  "treeProducerSusyMultilepton_tree" "treeProducerSusyMultilepton_tree_TW" "treeProducerSusyMultilepton_tree_TbarW")


runsamples=("TTbar_Powheg" "TW_ext" "TbarW_ext" "DYJetsToLL_M50_aMCatNLO" "DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext" "WJetsToLNu_aMCatNLO" 
  "WJetsToLNu_MLM & WJetsToLNu_MLM_ext2" "ZZ & ZZ_ext" "WW & WW_ext" "WZ & WZ_ext" "TTWToLNu_ext1 & TTWToLNu_ext2" "TTZToQQ" "TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2" "TTWToQQ" "TTGJets" 
  "MuonEG" "SingleElec" "SingleMuon" 
  "TTbar_Powheg_ueUp" "TTbar_Powheg_ueDown" "TTbar_Powheg_isrUp_ext" "TTbar_Powheg_isrDown" "TTbar_Powheg_fsrUp" "TTbar_Powheg_fsrDown" 
  "TTbar_Powheg_Herwig_ext2" "TTJets_aMCatNLO" "TTbar_Powheg_hdampUp & TTbar_Powheg_hdampUp_ext" "TTbar_Powheg_hdampDown & TTbar_Powheg_hdampDown_ext" "TW_DS" "TbarW_DS" 
  "TW_noFullyHadr_isrUp" "TW_noFullyHadr_isrDown" "TW_noFullyHadr_fsrUp" "TW_noFullyHadr_fsrDown" "TW_noFullyHadr_MEscaleUp" "TW_noFullyHadr_MEscaleDown" "TbarW_noFullyHadr_isrUp" 
  "TbarW_noFullyHadr_isrDown" "TbarW_noFullyHadr_fsrUp" "TbarW_noFullyHadr_fsrDown" "TbarW_noFullyHadr_MEscaleUp" "TbarW_noFullyHadr_MEscaleDown" 
  "TTbar_Powheg_mtop1665" "TTbar_Powheg_mtop1695_backup & TTbar_Powheg_mtop1695_ext & TTbar_Powheg_mtop1695_ext2" "TTbar_Powheg_mtop1735" 
  "TTbar_Powheg_mtop1755 & TTbar_Powheg_mtop1755_ext & TTbar_Powheg_mtop1755_ext2" "TTbar_Powheg_mtop1785" "TTbar_Powheg_erdON & TTbar_Powheg_erdON_ext" 
  "TTbar_QCDbasedCRTune_erdON & TTbar_QCDbasedCRTune_erdON_ext" "TTbar_GluonMoveCRTune" "TTbar_GluonMoveCRTune_erdON" "TW_noFullyHadr_mtop1695" "TW_noFullyHadr_mtop1755" 
  "TbarW_noFullyHadr_mtop1695" "TbarW_noFullyHadr_mtop1755" "TW_noFullyHadr_herwigpp" "TbarW_noFullyHadr_herwigpp" 
  "LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree.root" "LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TW.root" 
  "LocalFile:/pool/ciencias/userstorage/sscruz/TW/bfrag/treeProducerSusyMultilepton_tree_TbarW.root")

plotspath="/nfs/fanae/user/vrbouza/Documents/TFM/AnalysisPAF/TW_temp"
if [ "$3" != "" ]; then
  plotspath=$3
fi

sel="TW"

if [ "$2" != "" ]; then
  sel=$2
fi

if [ "$1" == "" ]; then
  echo "ERROR - No valid arguments given"
  echo "Please, execute this script with a valid argument"
  cd plotter/TW
  return
fi

echo " "
echo "Checking that all root files exist in..."
echo $plotspath
echo "...with selection..."
echo $sel
echo "...with at least..."
echo $minimumsize
echo "...bytes of size. If they do not fulfil these requirements, they will be reanalysed with..."
echo $1
echo "...cores."
echo " "

path=""
uplimit=$((${#samples[@]}-1))
checker=0
actualsize=0

while [ $allok != ${#samples[@]} ]; do
  checker=$(($checker+1))
  allok=0
  for ((i=0; i<=$uplimit; i++)); do
    unset path
    unset actualsize
    
    path=$plotspath$slash$init${samples[i]}$final
    
    if [ ! -e $path ]; then
      echo " "
      echo "%%%% => ROOT file not found. The sample that is missing is:"
      echo ${samples[i]}
      echo "Reanalysing..."
      echo " "
      root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $1)"
      resetpaf -a
      allok=$(($allok-8))
    fi
    
    if [ -e $path ]; then
      actualsize=$(wc -c <"$path")
      if [ $actualsize -ge $minimumsize ]; then
        echo " "
        echo "%%%% => ROOT file with..."
        echo $actualsize
        echo "...bytes of size, which are lower than the minimum. This sample is:"
        echo ${samples[i]}
        echo "Reanalysing..."
        echo " "
        root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $1)"
        resetpaf -a
        allok=$(($allok-8))
      fi
    fi
    
    allok=$(($allok+1))
  done
  if [ $checker == 10 ]; then
    echo " "
    echo "%%%% => ERROR: limit of iterations (10) reached. There has been a problem with the execution or the sample files."
    echo "%%%% => The bash script will now end."
    echo " "
    cd plotter/TW
    exit
  fi
  sleep 5
done

echo "%%%% => All expected ROOT files are in the folder"
cd plotter/TW
