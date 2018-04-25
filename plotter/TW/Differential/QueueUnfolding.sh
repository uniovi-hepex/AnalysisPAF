#===============================================================================
#
#                       Unfolding of tW - related variables
#
#===============================================================================

logpath="/nfs/fanae/user/vrbouza/Documents/TFM/Unfolding_logs/"
ext="UNF"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! -e "temp/UnfoldingInfo.root" ]; then
  echo "%%%%% FATAL ERROR: the ROOT file with the histograms of the response matrix do not exist."
  echo "%%%%% Please, execute getMatrices.py correctly to obtain the response matrix before beginning the unfolding procedure."
  echo "%%%%% This bash script will now end."
  exit
else
  echo "> ROOT file with the histograms of the response matrix found."
  echo " "
fi

echo "> The variable to be unfolded is"
echo $1
echo " "

echo "> Creating job for queue unfolding..."
Job=$(qsub -q proof -o $logpath -e $logpath -d $workingpath -F "$1 $2" ExecuteUnfolding.sh)
echo $Job
echo "> Job created!"
