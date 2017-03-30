#===============================================================================
#
#                         Analysis of the ttH process
#
#===============================================================================

# 1st line of records: gae031

if [ "$1" == "an" ]; then
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH ANALYSIS EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
    echo ""
    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Setting up the environment"
    source pre_start.sh

    echo "%%%%%> DONE"

    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting analysis"
    cd ../..
    if [ "$2" == "test" ]; then
        root -l -b -q "RunAnalyserPAF.C(\"TestHeppy\"						               , \"ttH\", 1)"
    elif [ "$2" == "opt" ]; then
    	echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToLNu_ext1 & TTWToLNu_ext2\"      , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext & TTZToLLNuNu_ext2\" , \"ttH\", 60)" #25 - 60s, 40 - 57s, 64 - 62s
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"ttH\", 60)" #25 - 80s, 50 - 65s, 64 - 66s
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"ttH\", 55)" # 45 - 34s, 55 - 23s
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"ttH\", 25)" # 25 - 35s 30 - 35s
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"ttH\", 35)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWW\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"ttH\", 60)" # 55 - 122s, 64 - 132s
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"ttH\", 30)" # 35 - 38s, 45 - 40s, 30 - 41s

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"ttH\", 40)" # 50 - 69s, 64 - 74s, 64 - 74s
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_aMCatNLO\"				     , \"ttH\", 60)" # 64 - 312s
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext\" , \"ttH\", 20)" # 25 - 21s, 30 - 22s
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_aMCatNLO\"						     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TW & TW_ext\"								         , \"ttH\", 20)" # 25 - 24s, 30 - 26s
        root -l -b -q "RunAnalyserPAF.C(\"TbarW & TbarW_ext\"							     , \"ttH\", 20)" # 25 - 25s, 30 - 26s
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"ttH\", 60)" # 40 - 35s, 50 - 36s
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"ttH\", 45)" # 25 - 27s, 35 - 26s
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu_amcatnlo\"			             , \"ttH\", 20)" # 25 - 24s, 30 - 25s
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"ttH\", 20)" # 25 - 22s, 20 - 24s
        root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"ttH\", 25)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"ttH\", 50)" # 25 - 33s, 35 - 33s

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"ttH\", 60)" #~100s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"ttH\", 60)" #~246s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"ttH\", 60)" #~187s
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"ttH\", 60)" #~160s
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"ttH\", 60)" #~227s

    else
      echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToLNu_ext1 & TTWToLNu_ext2\"      , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext & TTZToLLNuNu_ext2\" , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"ttH\", $2)"

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_aMCatNLO\"				     , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext\" , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_aMCatNLO\"						     , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TW & TW_ext\"								         , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TbarW & TbarW_ext\"							     , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu_amcatnlo\"				           , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"ttH\", $2)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"ttH\", $2)"

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"ttH\", $2)"
    fi
    cd plotter/ttH
elif [ "$1" == "pl" ]; then
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ttH PLOTTER EXECUTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
  echo ""
  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Setting up the environment"
  source pre_start.sh

  echo "%%%%%> DONE"

  echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting analysis"
  cd ..
  root -l -b -q "ttH/DrawPlots.C(\"Muon\")"
  root -l -b -q "ttH/DrawPlots.C(\"Elec\")"
  root -l -b -q "ttH/DrawPlots.C(\"ElMu\")"
  root -l -b -q "ttH/DrawPlots.C(\"2lSS\")"
  root -l -b -q "ttH/DrawPlots.C(\"3l\")"
  root -l -b -q "ttH/DrawPlots.C(\"4l\")"
  root -l -b -q "ttH/DrawPlots.C(\"All\")"
  cd ttH
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
