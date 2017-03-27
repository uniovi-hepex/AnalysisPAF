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

    echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Starting analysis"
    cd ../..
    if [ "$2" == "test" ]; then
        root -l -b -q "RunAnalyserPAF.C(\"TestHeppy\"						               , \"ttH\", 1)"
    elif [ "$2" == "opt" ]; then
    	echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext & TTZToLLNuNu_ext2\" , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"ttH\", 45)"
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets_ext\"							 , \"ttH\", 25)"
        #root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"ttH\", 35)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WW\"								         , \"ttH\", 25)"
        #root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"ttH\", 55)"
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"ttH\", 35)"

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"ttH\", 50)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_aMCatNLO\"				     , \"ttH\", 64)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext\" , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_aMCatNLO\"						     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TW & TW_ext\"								         , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TbarW & TbarW_ext\"							     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"ttH\", 40)"
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZ_ext\"								         , \"ttH\", 25)"
        #root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"ttH\", 25)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"ttH\", 25)"

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"ttH\", 64)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"ttH\", 64)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"ttH\", 64)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"ttH\", 64)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"ttH\", 64)"

    else
      echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
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
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"							             , \"ttH\", $2)"
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
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
