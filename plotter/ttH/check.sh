source pre_start.sh
cd ../..

slash="/"
allok=0

samples=("Tree_TTWToLNu1.root" "Tree_TTWToQQ.root" "Tree_TTZToLLNuNu.root" "Tree_TTZToQQ.root" "Tree_WGToLNuG.root" "Tree_ZGTo2LG.root" "Tree_TGJets.root" "Tree_TTGJets.root" 
  "Tree_WpWpJJ.root" "Tree_ZZZ.root" "Tree_WZZ.root" "Tree_WWZ.root" "Tree_WWW.root" "Tree_WW.root" "Tree_tZq_ll.root" "Tree_TTTT.root" "Tree_TTJets_aMCatNLO.root" "Tree_DYJetsToLL_M50_MLM.root" 
  "Tree_DYJetsToLL_M5to50_MLM.root" "Tree_DYJetsToLL_M50_aMCatNLO.root" "Tree_DYJetsToLL_M10to50_aMCatNLO.root" "Tree_WJetsToLNu_aMCatNLO.root" 
  "Tree_TW.root" "Tree_TbarW.root" "Tree_T_tch.root" "Tree_Tbar_tch.root" "Tree_TToLeptons_sch_amcatnlo.root" "Tree_WZTo3LNu_amcatnlo.root" "Tree_WWTo2L2Nu.root" "Tree_ZZ.root" 
  "Tree_TTHNonbb.root" "Tree_MuonEG.root" "Tree_DoubleMuon.root" "Tree_DoubleEG.root" "Tree_SingleElec.root" "Tree_SingleMuon.root")

runsamples=("TTWToLNu_ext1 & TTWToLNu_ext2" "TTWToQQ" "TTZToLLNuNu_ext & TTZToLLNuNu_ext2" "TTZToQQ" "WGToLNuG" "ZGTo2LG" "TGJets & TGJets_ext" "TTGJets & TTGJets_ext" 
  "WpWpJJ" "ZZZ" "WZZ" "WWZ" "WWW" "WW & WW_ext" "tZq_ll" "TTTT" "TTJets_aMCatNLO" "DYJetsToLL_M50_MLM" 
  "DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2" "DYJetsToLL_M50_aMCatNLO" "DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext" "WJetsToLNu_aMCatNLO" 
  "TW & TW_ext" "TbarW & TbarW_ext" "T_tch" "Tbar_tch" "TToLeptons_sch_amcatnlo" "WZTo3LNu_amcatnlo" "WWTo2L2Nu" "ZZ & ZZ_ext" 
  "TTHNonbb" "MuonEG" "DoubleMuon" "DoubleEG" "SingleElec" "SingleMuon")

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

echo
echo "Checking that all root files exist in..."
echo $plotspath
echo "...with selection..."
echo $sel
echo "...and with..."
echo $1
echo "...cores."
echo

path=""
while [ $allok != 36 ]; do
  allok=0
  for i in {0..35}; do
    unset path
    path=$plotspath$slash${samples[i]}
    if [ ! -e $path ]; then
      echo "%%%% => ROOT file not found. The sample that is missing is:"
      echo ${samples[i]}
      echo "Reanalyzing..."
      echo
      root -l -b -q "RunAnalyserPAF.C(\"${runsamples[i]}\", \"$sel\", $1)"
      allok=$(($allok-8))
    fi
    allok=$(($allok+1))
  done
done

echo "%%%% => All expected ROOT files are in the folder"
cd plotter/ttH
