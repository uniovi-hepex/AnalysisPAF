#===============================================================================
#
#                         Analysis of the ttH process
#
#===============================================================================

logpath="/nfs/fanae/user/vrbouza/Documents/TFG/Executions/"
ext="NORMALlogs"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ "$1" == "an" ]; then
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH ANALYSIS EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Creating job..."
  An=$(qsub -q proof -l nodes=1:ppn=$2 -o $logpath -e $logpath -d $workingpath -F "an $2 $3" DottHAnalysis.sh)
  echo $An
  qsub -q proof -l nodes=1:ppn=$2 -o $logpath -e $logpath -d $workingpath -W depend=afterany:$An -F "$2 $3" check.sh
elif [ "$1" == "pl" ]; then
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH PLOTTER EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  categ="0"
  if [ "$2" != "" ]; then
    categ=$2
  else
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Remaking libraries..."  
    source RemakeLibraries.sh
  fi
  
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Creating job..."  
  qsub -o $logpath -e $logpath -d $workingpath -F "$categ" PlotThings.sh
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
