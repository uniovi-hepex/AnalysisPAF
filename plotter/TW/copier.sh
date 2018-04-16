# This script makes a copy of the full content of ../../PROCESS_temp into ~/Storage/PROCESS/MiniTrees/DD_MM_YYYY
#
#
if [ "$1" != "" ]; then
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
