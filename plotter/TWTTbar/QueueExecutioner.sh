#===============================================================================
#
#                         Analysis of the TWTTbar process
#
#===============================================================================

logpath="logs/"
ext="Execution"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

mkdir -p logs
mkdir -p logs/Execution

echo ""
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TW ANALYSIS EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
echo ""
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Creating jobs..."
An=$(qsub -l nodes=1:ppn=$1 -l walltime=20:00:00 -o $logpath -e $logpath -d $workingpath -F "an $1 $2 $3" Executioner.sh)
echo $An
qsub -l nodes=1:ppn=$1 -l walltime=20:00:00 -o $logpath -e $logpath -d $workingpath -W depend=afterany:$An -F "ch $1 $2 $3" Executioner.sh
