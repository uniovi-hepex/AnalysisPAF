#===============================================================================
#
#                       Unfolding of tW - related variables (with profiling now)
#
#===============================================================================

logpath="logs/"
ext="Unfolding"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

source RemakeLibraries.sh
python getMatrices.py $4

mkdir -p logs
mkdir -p logs/Unfolding

if [ "$1" == "All" ]; then
    echo "> We are going to do all the unfolding procedures for all the variables! And profiling too!! Yey!!!"
else
    echo "> The variable to be unfolded is"
    echo $1
    echo " "
fi

echo "> Creating job ..."
Job=$(qsub -q proof -l walltime=06:00:00,nodes=1:ppn=$2 -o $logpath -e $logpath -d $workingpath -F "$1 $2 $3 $4" ExecuteUnfolding.sh)
echo $Job
echo "> Job created!"
