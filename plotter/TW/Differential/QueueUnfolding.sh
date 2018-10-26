#===============================================================================
#
#                       Unfolding of tW - related variables
#
#===============================================================================

logpath="/nfs/fanae/user/vrbouza/Documents/TFM/Queue_logs/"
ext="Unfolding"
logpath=$logpath$ext

workingpath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

unfoldingvars=("E_LLB" "LeadingJetE" "MT_LLMETB" "M_LLB" "M_LeadingB" "M_SubLeadingB" 
               "MET" "MET_Phi" "LeadingJetPt" "LeadingJetEta" "LeadingJetPhi" 
               "LeadingLepE" "LeadingLepPt" "LeadingLepPhi" "LeadingLepEta" 
               "SubLeadingLepE" "SubLeadingLepPt" "SubLeadingLepPhi" "SubLeadingLepEta" 
               "DilepPt" "DilepJetPt" "DilepMETJetPt" "HTtot")

uplimit=$((${#unfoldingvars[@]}-1))

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
  echo "> We are going to do all the unfolding procedures for all the variables!! Yey!"

  echo "> Creating jobs for all the queue unfolding procedures..."
  for ((i=0; i<=$uplimit; i++)); do
    Job=$(qsub -q proof -o $logpath -e $logpath -d $workingpath -F "${unfoldingvars[i]} $2" ExecuteUnfolding.sh)
    echo $Job
  done
  echo "> Jobs created!"
  
elif [ "$1" == "AllUnf" ]; then
  echo "> We are going to ONLY unfold all the variables!! Yey!"
  tres="onlyunf"
  echo "> Creating jobs for all the queue unfolding procedures..."
  for ((i=0; i<=$uplimit; i++)); do
    Job=$(qsub -q proof -o $logpath -e $logpath -d $workingpath -F "${unfoldingvars[i]} $2 ${tres}" ExecuteUnfolding.sh)
    echo $Job
  done
  echo "> Jobs created!"
  
else
  echo "> The variable to be unfolded is"
  echo $1
  echo " "

  echo "> Creating job for queue unfolding procedures..."
  Job=$(qsub -q proof -o $logpath -e $logpath -d $workingpath -F "$1 $2" ExecuteUnfolding.sh)
  echo $Job
  echo "> Job created!"
fi
