# This script grants you the chance of doing a beautiful trip through more than four python scripts and unfold some variable(s) WITH PROFILING!!! :D
# ---------------------------------------------------------
# Preliminaries
# source ../pre_start.sh
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

# unfoldingvars=("E_LLB" "LeadingJetE" "MT_LLMETB" "M_LLB" "M_LeadingB" "M_SubLeadingB" 
#                "MET" "MET_Phi" "LeadingJetPt" "LeadingJetEta" "LeadingJetPhi" 
#                "LeadingLepE" "LeadingLepPt" "LeadingLepPhi" "LeadingLepEta" 
#                "SubLeadingLepE" "SubLeadingLepPt" "SubLeadingLepPhi" "SubLeadingLepEta" 
#                "DilepPt" "DilepJetPt" "DilepMETJetPt" "HTtot" 
#                "DilepMETJet1Pz" "LLMETBEta" "MSys" "Mll" "DPhiLL" "DPhiLeadJet" "DPhiSubLeadJet")

# unfoldingvars=("M_LeadingB" "M_SubLeadingB" "LeadingLepPt" "DilepMETJet1Pz" "LLMETBEta" "DPhiLL" "DPhiLeadJet" "DPhiSubLeadJet")
# unfoldingvars=("LeadingJetPt" "LeadingLepPt")


# unfoldingvarscut=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "DilepPt") # Variables single top 15-10-2018
# unfoldingvars=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "DilepPt" "Fiducial") # Variables single top 15-10-2018
unfoldingvarscut=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB") # Variables single top 15-10-2018
unfoldingvars=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "Fiducial") # Variables single top 15-10-2018

# unfoldingvarscut=("LeadingJetPt") # PRUEBA
# unfoldingvars=("LeadingJetPt" "Fiducial") # PRUEBA


# unfoldingvarscut=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB") # Variables single top 15-10-2018
# unfoldingvars=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "Fiducial" "FiducialtWttbar")
# unfoldingvarscut=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "MT_LLMETBATLAS" "M_LLBATLAS")
# unfoldingvars=("LeadingJetPt" "LeadingLepPt" "DPhiLL" "DilepMETJet1Pz" "MT_LLMETB" "M_LLB" "MT_LLMETBATLAS" "M_LLBATLAS" "Fiducial" "FiducialtWttbar")
# unfoldingvarscut=("MT_LLMETBATLAS" "M_LLBATLAS")
# unfoldingvars=("MT_LLMETBATLAS" "M_LLBATLAS")

uplimit=$((${#unfoldingvars[@]}-1))
uplimitcuts=$((${#unfoldingvarscut[@]}-1))


if [ "$variable" == "All" ]; then
    echo "> Beggining full unfolding procedure of all the variables"
    echo " "
    echo "> Creating (if they do not exist yet) the temporal and results folders"
    echo " "
    mkdir -p temp
    mkdir -p results

    if [ ! -e "temp/UnfoldingInfo.root" ]; then
        echo "%%%%% FATAL ERROR: the ROOT file with the histograms of the response matrix do not exist."
        echo "%%%%% This bash script will now end."
        return
    else
        echo "> ROOT file with the histograms of the response matrix found."
        echo " "
    fi
  
#     # 1) Get those histograms.
#     echo "> Obtaining histograms per BDT's bin with profiling..."
#     echo " "
#     python getFinalCards.py "All" $ncores $4

#     source ../pre_start.sh
    source ../pre_start.sh devel
#     for ((i=0; i<=$uplimit; i++)); do
#         # 2) Get the histograms for the closure test.
#         echo "> Obtaining histograms for closure test..."
#         echo " "
#         python getClosureHistos.py ${unfoldingvars[i]} $4
      # 3) Get the folded results.
#         echo "> Obtaining folded results and signal information..."
#         echo " "
#         python FinalExtracter.py ${unfoldingvars[i]} $4
#     done

#      source ../pre_start.sh devel
#      4) Do a proper unfolding as you were taught by your mother when you were a child.
     echo "> Unfolding all variables..."
     echo " "
     for ((i=0; i<=$uplimit; i++)); do
         python unfoldTW_cut.py ${unfoldingvars[i]}
     done

#     source ../pre_start.sh devel
#     5) Get fiducial results.
    echo "> Getting fiducial results..."
    echo " "
    for ((i=0; i<=$uplimitcuts; i++)); do
        python doFiducial.py ${unfoldingvarscut[i]}
    done

#     source ../pre_start.sh devel
    # 6) Obtain all covariance matrices.
#     echo "> Obtaining all covariance matrices..."
#     echo " "
#     python getCovarianceMatrices.py All $ncores
#
#     cd
#     source ./pre_start_CMS.sh
#     cd Documents/TFM/AnalysisPAF/plotter/TW/Differential
#     # 7) Do GOF tests.
#     echo "> Performing GOF tests..."
#     echo " "
#     python goftests.py "All"
    
#     # 8) Get a txt with all the results
#     echo "> Printing yields..."
#     echo " "
#     python getYields.py All $ncores

else
    echo "> Beggining full unfolding procedure of the variable"
    echo $variable
    echo " "
    echo "> Creating (if they do not exist yet) the temporal and results folders"
    echo " "
    mkdir -p temp
    mkdir -p results

    if [ ! -e "temp/UnfoldingInfo.root" ]; then
        echo "%%%%% FATAL ERROR: the ROOT file with the histograms of the response matrix do not exist."
        echo "%%%%% This bash script will now end."
        return
    else
        echo "> ROOT file with the histograms of the response matrix found."
        echo " "
    fi

    # 1) Get those histograms.
    echo "> Obtaining histograms..."
    echo " "
    python getFinalCards.py $variable 1 $4

    # 2) Get the histograms for the closure test.
    echo "> Obtaining histograms for closure test..."
    echo " "
    python getClosureHistos.py $variable $4

    # 3) Get the folded results.
    echo "> Obtaining folded results and signal information..."
    echo " "
    python FinalExtracter.py $variable $4
    
    source ../pre_start.sh devel
    # 4) Do a proper unfolding as you were taught by your mother when you were a child.
    echo "> Unfolding variable..."
    echo " "
    python unfoldTW_cut.py $variable
    
    # 5) Get fiducial results.
    echo "> Getting fiducial results..."
    echo " "
    python doFiducial.py $variable

    source ../pre_start
    # 6) Obtain the covariance matrix
    echo "> Obtaining the covariance matrix..."
    echo " "
    python getCovarianceMatrices.py $variable

    cd
    source ./pre_start_CMS.sh
    cd Documents/TFM/AnalysisPAF/plotter/TW/Differential
    # 7) Do GOF tests.
    echo "> Performing GOF tests..."
    echo " "
    python goftests.py $variable
    
#     # 8) Get a txt with all the results
#     echo "> Printing yields..."
#     echo " "
#     python getYields.py $variable
fi


if [ "$3" == "copy" ]; then
    cd ..
    source copier.sh "p"
    cd Differential
fi

echo "> Full unfolding procedure complete!"
