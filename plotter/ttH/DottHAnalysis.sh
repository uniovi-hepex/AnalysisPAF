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
        root -l -b -q "RunAnalyserPAF.C(\"TestHeppy\"						            , \"ttH\", 1)"
    else
    	echo "%%%%%> MC SAMPLES FOR COMPARISON WITH DATA"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToLNu_ext2\"						        , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext\"						      , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext2\"					       , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								          , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"TGJets\"								           , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TGJets_ext\"							        , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"TTGJets\"								         , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets_ext\"							        , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									            , \"ttH\", $2)"	# Rare
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									            , \"ttH\", $2)"	# Rare
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									            , \"ttH\", $2)"	# Rare
        #root -l -b -q "RunAnalyserPAF.C(\"WW\"									             , \"ttH\", $2)"	# Rare
        root -l -b -q "RunAnalyserPAF.C(\"WW_ext\"								          , \"ttH\", $2)"	# Rare
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								          , \"ttH\", $2)"	# Rare
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								            , \"ttH\", $2)"	# Rare

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						      , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_aMCatNLO\"				  , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M10to50_aMCatNLO_ext\" , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_MLM\"						      , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_MLM_ext2\"					   , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"TW\"									             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TW_ext\"								          , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"TbarW\"								           , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TbarW_ext\"							          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								            , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				  , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"							          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							          , \"ttH\", $2)"
        #root -l -b -q "RunAnalyserPAF.C(\"ZZ\"									             , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"ZZ_ext\"								          , \"ttH\", $2)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	, \"ttH\", $2)"

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                          , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                      , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                        , \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	, \"ttH\", $2)"
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                      , \"ttH\", $2)"
    fi
    cd plotter/ttH
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
