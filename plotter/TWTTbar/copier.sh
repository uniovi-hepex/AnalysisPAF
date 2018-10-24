# This script makes a copy of the full content of ../../PROCESS_temp into ~/Storage/PROCESS/MiniTrees/YYYY_MM_DD
#
#

plotspath="/nfs/fanae/user/DIRECCIÓNDEIRENEQUE-YPRESTE"
indstr="index.php"
cnstr="CondN/"
covstr="CovMat/"
indexpath="~/www/index.php"

if [ "$1" == "p" ]; then
  echo "===> Copying unfolding results!"
  echo " "
  echo "Plots will be copied into..."
  echo $plotspath
  
  echo " "
  echo "Copying plots..."
  cp -R results/. $plotspath
  cp $indexpath $plotspath$indstr
  cp $indexpath $plotspath$covstr$indstr
  cp $indexpath $plotspath$cnstr$indstr
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
savefolder=$y$lowerbar$m$lowerbar$d
storagepath="/nfs/fanae/user/vrbouza/Storage/TWTTbar/MiniTrees/"
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

