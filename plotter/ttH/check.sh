source pre_start.sh
cd ../..

slash="/"
allok=0

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

samples=("Tree_TTWToLNu.root" "Tree_TTWToQQ.root" "Tree_TTZToLLNuNu.root" "Tree_TTZToQQ.root" "Tree_WGToLNuG.root" "Tree_ZGTo2LG.root" "Tree_TGJets.root" "Tree_TTGJets.root" 
  "Tree_WpWpJJ.root" "Tree_ZZZ.root" "Tree_WZZ.root" "Tree_WWZ.root" "Tree_WWW.root" "Tree_WW.root" "Tree_tZq_ll.root" "Tree_TTTT.root" "Tree_TTJets_aMCatNLO.root" "Tree_TTbar_Powheg.root" 
  "Tree_DYJetsToLL_M50_MLM.root" "Tree_DYJetsToLL_M5to50_MLM.root" 
  "Tree_WJetsToLNu_MLM.root" "Tree_TW.root" "Tree_TbarW.root" "Tree_T_tch.root" "Tree_Tbar_tch.root" "Tree_TToLeptons_sch_amcatnlo.root" "Tree_WZTo3LNu.root" "Tree_WWTo2L2Nu.root" 
  "Tree_ZZ.root" "Tree_TTHNonbb.root" "Tree_MuonEG.root" "Tree_DoubleMuon.root" "Tree_DoubleEG.root" "Tree_SingleElec.root" "Tree_SingleMuon.root")


runsamples=("TTWToLNu_ext1 & TTWToLNu_ext2" "TTWToQQ" "TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2" "TTZToQQ" "WGToLNuG" "ZGTo2LG" "TGJets & TGJets_ext" "TTGJets & TTGJets_ext" 
  "WpWpJJ" "ZZZ" "WZZ" "WWZ" "WWW" "WW & WW_ext" "tZq_ll" "TTTT" "TTJets_aMCatNLO" "TTbar_Powheg" 
  "DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2" "DYJetsToLL_M5to50_MLM" 
  "WJetsToLNu_MLM & WJetsToLNu_MLM_ext2" "TW_ext" "TbarW_ext" "T_tch" "Tbar_tch" "TToLeptons_sch_amcatnlo" "WZTo3LNu" "WWTo2L2Nu" 
  "ZZ & ZZ_ext" "TTHNonbb" "MuonEG" "DoubleMuon" "DoubleEG" "SingleElec" "SingleMuon")

plotspath="/nfs/fanae/user/vrbouza/Documents/TFG/AnalysisPAF/ttH_temp"
if [ "$3" != "" ]; then
  plotspath=$3
fi
sel="ttH"
if [ "$2" != "" ]; then
  sel=$2
fi
if [ "$1" == "" ]; then
  echo "ERROR - No valid arguments given"
  echo "Please, execute this script with a valid argument"
  cd plotter/ttH
  return
fi

echo " "
echo "Checking that all root files exist in..."
echo $plotspath
echo "...with selection..."
echo $sel
echo "...and with..."
echo $1
echo "...cores."
echo " "

path=""
uplimit=$((${#samples[@]}-1))
checker=0

ttz="Tree_TTZToLLNuNu1.root"
ttw="Tree_TTWToLNu1.root"
ttz=$plotspath$slash$ttz
ttw=$plotspath$slash$ttw


while [ $allok != ${#samples[@]} ]; do
  checker=$(($checker+1))
  allok=0
  if [ -e $ttz ]; then
    echo " "
    echo "%%%% => ROOT file misnamed. The sample with wrong name is:"
    echo ${samples[2]}
    echo "Renaming..."
    echo " "
    cd $plotspath
    cp -- "Tree_TTZToLLNuNu1.root" "Tree_TTZToLLNuNu.root"
    rm "Tree_TTZToLLNuNu1.root"
    cd $workingpath
  fi
  if [ -e $ttw ]; then
    echo " "
    echo "%%%% => ROOT file misnamed. The sample with wrong name is:"
    echo ${samples[0]}
    echo "Renaming..."
    echo " "
    cd $plotspath
    cp -- "Tree_TTWToLNu1.root" "Tree_TTWToLNu.root"
    rm "Tree_TTWToLNu1.root"
    cd $workingpath
  fi
  
  for ((i=0; i<=$uplimit; i++)); do
    unset path
    path=$plotspath$slash${samples[i]}
    if [ ! -e $path ]; then
      echo " "
      echo "%%%% => ROOT file not found. The sample that is missing is:"
      echo ${samples[i]}
      echo "Reanalyzing..."
      echo " "
      root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $1)"
      # if [ ${samples[i]} == "Tree_TTHNonbb.root" ]; then
      #   root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/HeppyTreesSummer16/v2/jet25/Tree_TTHNonbb_0.root\", \"$sel\", $1, 0, 0, 0.21510)"
      #   cd $plotspath
      #   cp Tree_TTHNonbb_0.root Tree_TTHNonbb.root
      #   rm Tree_TTHNonbb_0.root
      #   cd $workingpath
      # elif [ ${samples[i]} == "Tree_WJetsToLNu_aMCatNLO.root" ]; then
      #   root -l -b -q "RunAnalyserPAF.C(\"LocalFile:/pool/ciencias/HeppyTreesSummer16/v2/jet25/Tree_WJetsToLNu_aMCatNLO_0.root\", \"$sel\", $1, 0, 0, 61526.7)"
      #   cd $plotspath
      #   cp Tree_WJetsToLNu_aMCatNLO_0.root Tree_WJetsToLNu_aMCatNLO.root
      #   rm Tree_WJetsToLNu_aMCatNLO_0.root
      #   cd $workingpath
      # else
      #   root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $1)"
      # fi
      allok=$(($allok-8))
    fi
    allok=$(($allok+1))
  done
  if [ $checker == 10 ]; then
    echo " "
    echo "%%%% => ERROR: limit of iterations (10) reached. There has been a problem with the execution or the sample files."
    echo "%%%% => The bash script will now end."
    echo " "
    cd plotter/ttH
    exit
  fi
  sleep 5
done

echo "%%%% => All expected ROOT files are in the folder"
cd plotter/ttH
