#===============================================================================
#
#                         Analysis of the ttH process
#
#===============================================================================
if [ "$1" == "an" ]; then
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH ANALYSIS EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Setting up the environment"
    source pre_start.sh

    echo "%%%%%> DONE"
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting analysis"
    cd ../..
    sel="ttH"
    if [ "$3" != "" ]; then
      sel=$3
    fi
    if [ "$2" == "test" ]; then
        root -l -b -q "RunAnalyserPAF.C(\"TestHeppy\"						               , \"$sel\", 1)"
    elif [ "$2" == "opt" ]; then
    	echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToLNu_ext1 & TTWToLNu_ext2\"      , \"$sel\", 40)" 
        # 25 - 50s
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2\", \"$sel\", 35)" 
        # 25 - 60s, 40 - 57s, 64 - 62s, 60 - 61s
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"$sel\", 45)" 
        # 25 - 80s, 50 - 65s, 64 - 66s, 60 - 66s
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"$sel\", 60)" 
        # 45 - 34s, 55 - 23s
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"$sel\", 35)" 
        # 25 - 35s, 30 - 35s, 25 - 47s
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"$sel\", 35)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWW\"									               , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"$sel\", 50)" 
        # 55 - 122s, 64 - 132s, 60 - 130s
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"$sel\", 40)" 
        # 35 - 38s, 45 - 40s, 30 - 41s **

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"$sel\", 45)" 
        # 50 - 69s, 64 - 74s, 64 - 74s, 40 - 76s
        root -l -b -q "RunAnalyserPAF.C(\"TTbar_Powheg\" 						           , \"$sel\", 45)" 
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2\"  , \"$sel\", 60)" 
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M5to50_MLM\"              , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_MLM & WJetsToLNu_MLM_ext2\"              , \"$sel\", 40)"
        root -l -b -q "RunAnalyserPAF.C(\"TW_ext\"								             , \"$sel\", 25)" 
        # 25 - 24s, 30 - 26s, 20 - 25s 
        root -l -b -q "RunAnalyserPAF.C(\"TbarW & TbarW_ext\"							     , \"$sel\", 25)" 
        # 25 - 25s, 30 - 26s, 20 - 26s 
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"$sel\", 40)" 
        # 40 - 35s, 50 - 36s, 60 -38s
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"$sel\", 40)" 
        # 25 - 27s, 35 - 26s, 45 - 27s
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"$sel\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"        			             , \"$sel\", 40)" 
        # 20 - 76s
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"$sel\", 30)" 
        # 25 - 22s, 20 - 24s
        root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"$sel\", 25)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"$sel\", 30)" 
        # 25 - 33s, 35 - 33s, 50 - 34s

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"$sel\", 60)" 
        # ~100s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"$sel\", 60)" 
        # ~246s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"$sel\", 60)" 
        # ~187s
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"$sel\", 60)" 
        # ~160s
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"$sel\", 60)" 
        # ~227s

    else
      echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToLNu_ext1 & TTWToLNu_ext2\"      , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext1 & TTZToLLNuNu_ext2\", \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWW\"									               , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"$sel\", $2)"

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTbar_Powheg\" 						           , \"$sel\", $2)" 
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_MLM_ext & DYJetsToLL_M50_MLM_ext2\"  , \"$sel\", $2)" 
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M5to50_MLM\"              , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_MLM & WJetsToLNu_MLM_ext2\"              , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TW_ext\"								             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TbarW_ext\"							             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"				                   , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"$sel\", $2)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"$sel\", $2)"

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"$sel\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"$sel\", $2)"
        
        
    fi
    cd plotter/ttH
elif [ "$1" == "pl" ]; then
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH PLOTTER EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting to plot"
  categ="0"
  if [ "$2" != "" ]; then
    categ=$2
  fi
  source PlotThings.sh "$categ"
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
