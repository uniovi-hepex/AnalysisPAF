#===============================================================================
#
#                       Unfolding of tW - related variables (with profiling now)
#
#===============================================================================

logpath="/nfs/fanae/user/vrbouza/Documents/TFM/Queue_logs/"
ext="Unfolding"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd ..
source RemakeLibraries.sh
cd Differential
python getMatrices.py $4

if [ "$1" == "All" ]; then
    echo "> We are going to do all the unfolding procedures for all the variables! And profiling too!! Yey!!!"
else
    echo "> The variable to be unfolded is"
    echo $1
    echo " "
fi

echo "> Creating job ..."
Job=$(qsub -q proof -l walltime=06:00:00,nodes=1:ppn=$2 -o $logpath -e $logpath -d $workingpath -F "$1 $2 $3 $4" ExecuteUnfoldingWithProfiling.sh)
echo $Job
echo "> Job created!"
