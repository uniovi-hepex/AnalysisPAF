# This script makes a copy of the full content of ../../PROCESS_temp into ~/Storage/PROCESS/MiniTrees/DD_MM_YYYY
#
#

plotspath="/nfs/fanae/user/vrbouza/www/TFM/Unfolding/Results/"
indstr="index.php"
crdplstr="Cardplots/"
compstr="comparison/"
cnstr="CondN/"
covstr="CovMat/"
wwwpath="/nfs/fanae/user/vrbouza/www/index.php"

if [ "$1" == "p" ]; then
  echo "===> Copying unfolding results!"
  echo " "
  echo "Plots will be copied into..."
  echo $plotspath
  
  echo " "
  echo "Copying plots..."
  cp -R Differential/results/. $plotspath
  cp $wwwpath $plotspath$indstr
  cp $wwwpath $plotspath$compstr$indstr
  cp $wwwpath $plotspath$cnstr$indstr
  cp $wwwpath $plotspath$covstr$indstr
  echo " "
  echo "Done!"
  return
elif [ "$1" != "" ]; then
  d=$1
else
  d=$(date +%d)
fi
if [ "$2" != "" ]; then
  m=$2
else
  m=$(date +%m)
fi
if [ "$3" != "" ]; then
  y=$3
else
  y=$(date +%Y)
fi

lowerbar="_"
slash="/"
savefolder=$d$lowerbar$m$lowerbar$y
storagepath="/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
savepath=$storagepath$savefolder

echo "===> Copying minitrees!"
echo " "
echo "Files will be copied into..."
echo $savepath
mkdir -p $savepath

echo " "
echo "Copying files (note that this process can last even minutes depending on the size of the minitrees)..."
savepath=$savepath$slash
cp -R ../../TW_temp/. $savepath

echo " "
echo "Done!"

