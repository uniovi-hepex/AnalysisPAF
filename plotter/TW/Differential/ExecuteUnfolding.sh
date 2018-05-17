# This script grants you the chance of doing a beautiful trip through more than four python scripts and unfold some variable(s).
# ---------------------------------------------------------
# Preliminaries
source ../pre_start.sh
if [ "$1" != "" ]; then
  variable=$1
else
  echo "> Default variable chosen."
  echo " "
  variable="LeadingJetPt"
fi
if [ $3 == "onlyunf" ]; then
  echo "> Beggining ONLY unfolding procedure of the variable"
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

  echo "> Unfolding chosen variable..."
  echo " "
  python UnfoldThings.py $variable
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
  echo "> Obtaining histograms per BDT's bin..."
  echo " "
  python getCards.py $variable

  # 3) Get the histograms for the closure test.
  echo "> Obtaining histograms for closure test..."
  echo " "
  python getClosureHistos.py $variable
  
  # 4) Do a fit in each bin of your histograms and for all the systematics.
  echo "> Performing fits to each bin of the BDT's distribution..."
  echo " "
  cd
  source pre_start_CMS.sh
  cd Documents/TFM/AnalysisPAF/plotter/TW/Differential/
  python fits.py $variable

  # 5) Do a proper unfolding as you were taught by your mother when you were a child.
  source ../pre_start.sh
  echo "> Unfolding chosen variable..."
  echo " "
  python UnfoldThings.py $variable
fi

if [ "$2" == "copy" ]; then
  cd ..
  source copier.sh "p"
  cd Differential
fi

echo "> Full unfolding procedure complete!"
