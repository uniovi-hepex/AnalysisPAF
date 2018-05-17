#===============================================================================
#
#                       Unfolding of tW - related variables (with profiling now)
#
#===============================================================================

logpath="/nfs/fanae/user/vrbouza/Documents/TFM/Queue_logs/"
ext="Unfolding"
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

cd ..
source RemakeLibraries.sh
cd Differential


if [ "$1" == "All" ]; then
  echo "> We are going to do all the unfolding procedures for all the variables! And profiling too!! Yey!!!"
else
  echo "> The variable to be unfolded is"
  echo $1
  echo " "
fi


echo "> Creating job ..."
Job=$(qsub -q proof -l walltime=06:00:00,nodes=1:ppn=$2 -o $logpath -e $logpath -d $workingpath -F "$1 $2 $3" ExecuteUnfoldingWithProfiling.sh)
echo $Job
echo "> Job created!"
