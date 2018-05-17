# This script grants you the chance of doing a beautiful trip through more than four python scripts and unfold some variable(s) WITH PROFILING!!! :D
# ---------------------------------------------------------
# Preliminaries
source ../pre_start.sh
if [ "$1" != "" ]; then
  variable=$1
else
  echo "> Default variable chosen."
  echo " "
  variable="LeadingLepEta"
fi

if [ "$2" != "" ]; then
  ncores=$2
else
  ncores=1
fi

unfoldingvars=("E_LLB" "LeadingJetE" "MT_LLMETB" "M_LLB" "M_LeadingB" "M_SubLeadingB" 
               "MET" "MET_Phi" "LeadingJetPt" "LeadingJetEta" "LeadingJetPhi" 
               "LeadingLepE" "LeadingLepPt" "LeadingLepPhi" "LeadingLepEta" 
               "SubLeadingLepE" "SubLeadingLepPt" "SubLeadingLepPhi" "SubLeadingLepEta" 
               "DilepPt" "DilepJetPt" "DilepMETJetPt" "HTtot")

uplimit=$((${#unfoldingvars[@]}-1))


if [ "$variable" == "All" ]; then
  echo "> Beggining full unfolding procedure of all the variables"
  echo " "
  echo "> Creating (if they do not exist yet) the temporal and results folders"
  echo " "
  mkdir -p temp
  mkdir -p results
  mkdir -p results/Cardplots

  if [ ! -e "temp/UnfoldingInfo.root" ]; then
    echo "%%%%% FATAL ERROR: the ROOT file with the histograms of the response matrix do not exist."
    echo "%%%%% This bash script will now end."
    exit
  else
    echo "> ROOT file with the histograms of the response matrix found."
    echo " "
  fi
  
  
  for ((i=0; i<=$uplimit; i++)); do
    # 1) Get the binning in order to obtain the BDT histograms.
    echo "> Obtaining BDT's binning..."
    echo " "
    python getBinning.py ${unfoldingvars[i]} 0

    # 2) Get those histograms.
    echo "> Obtaining histograms per BDT's bin with profiling..."
    echo " "
    python getCardsWithProfiling.py $ncores ${unfoldingvars[i]}
    
    # 3) Get the histograms for the closure test.
    echo "> Obtaining histograms for closure test..."
    echo " "
    python getClosureHistos.py ${unfoldingvars[i]}
  done

  cd
  source pre_start_CMS.sh
  cd Documents/TFM/AnalysisPAF/plotter/TW/Differential/
  for ((i=0; i<=$uplimit; i++)); do
    # 4) Do a fit in each bin of your histograms and for all the systematics.
    echo "> Performing fits to each bin of the BDT's distribution with profiling..."
    echo " "
    python makeFits.py $ncores ${unfoldingvars[i]}
  done

  
  source ../pre_start.sh
  for ((i=0; i<=$uplimit; i++)); do
    # 5) Plot the folded distributions
    echo "> Plotting the folding distributions after fits..."
    echo " "
    python plotfolddist.py ${unfoldingvars[i]}
  # # 6) Do a proper unfolding as you were taught by your mother when you were a child.
  # echo "> Unfolding chosen variable..."
  # echo " "
  # python UnfoldThings.py ${unfoldingvars[i]}
  done
else
  echo "> Beggining full unfolding procedure of the variable"
  echo $variable
  echo " "
  echo "> Creating (if they do not exist yet) the temporal and results folders"
  echo " "
  mkdir -p temp
  mkdir -p results
  mkdir -p results/Cardplots

  if [ ! -e "temp/UnfoldingInfo.root" ]; then
    echo "%%%%% FATAL ERROR: the ROOT file with the histograms of the response matrix do not exist."
    echo "%%%%% This bash script will now end."
    exit
  else
    echo "> ROOT file with the histograms of the response matrix found."
    echo " "
  fi

  # 1) Get the binning in order to obtain the BDT histograms.
  echo "> Obtaining BDT's binning..."
  echo " "
  python getBinning.py $variable 0

  # 2) Get those histograms.
  echo "> Obtaining histograms per BDT's bin with profiling..."
  echo " "
  python getCardsWithProfiling.py $ncores $variable

  # 3) Get the histograms for the closure test.
  echo "> Obtaining histograms for closure test..."
  echo " "
  python getClosureHistos.py $variable

  # 4) Do a fit in each bin of your histograms and for all the systematics.
  echo "> Performing fits to each bin of the BDT's distribution with profiling..."
  echo " "
  cd
  source pre_start_CMS.sh
  cd Documents/TFM/AnalysisPAF/plotter/TW/Differential/
  python makeFits.py $ncores $variable

  source ../pre_start.sh
  # 5) Plot the folded distributions
  echo "> Plotting the folding distributions after fits..."
  echo " "
  python plotfolddist.py $variable
  
  # # 6) Do a proper unfolding as you were taught by your mother when you were a child.
  # echo "> Unfolding chosen variable..."
  # echo " "
  # python UnfoldThings.py $variable
fi

if [ "$3" == "copy" ]; then
  cd ..
  source copier.sh "p"
  cd Differential
fi

echo "> Full unfolding procedure complete!"
