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
        root -l -b -q "RunAnalyserPAF.C(\"TTWToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TTZToLLNuNu_ext & TTZToLLNuNu_ext2\" , \"ttH\", 40)" #25 - 60s
        root -l -b -q "RunAnalyserPAF.C(\"TTZToQQ\"								             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WGToLNuG\"							             , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"ZGTo2LG\"								             , \"ttH\", 50)" #25 - 80s
        root -l -b -q "RunAnalyserPAF.C(\"TGJets & TGJets_ext\"							   , \"ttH\", 45)"
        root -l -b -q "RunAnalyserPAF.C(\"TTGJets_ext\"							 , \"ttH\", 30)" # 25 - 35s
        #root -l -b -q "RunAnalyserPAF.C(\"TTGJets & TTGJets_ext\"							 , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WpWpJJ\"								             , \"ttH\", 35)" # petó
        root -l -b -q "RunAnalyserPAF.C(\"ZZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WWZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZZ\"									               , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WW_ext\"								         , \"ttH\", 25)"
        #root -l -b -q "RunAnalyserPAF.C(\"WW & WW_ext\"								         , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"tZq_ll\"								             , \"ttH\", 64)" # 55 - 122s
        root -l -b -q "RunAnalyserPAF.C(\"TTTT\"								               , \"ttH\", 45)" # 35 - 38s

		echo "%%%%%> MC SAMPLES FOR CONTROL REGIONS"
        root -l -b -q "RunAnalyserPAF.C(\"TTJets_aMCatNLO\"						         , \"ttH\", 64)" # 50 - 69s
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M50_aMCatNLO\"				     , \"ttH\", 64)" # 64 - 312s
        root -l -b -q "RunAnalyserPAF.C(\"DYJetsToLL_M10to50_aMCatNLO & DYJetsToLL_M10to50_aMCatNLO_ext\" , \"ttH\", 30)" # 25 - 21s
        root -l -b -q "RunAnalyserPAF.C(\"WJetsToLNu_aMCatNLO\"						     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"TW & TW_ext\"								         , \"ttH\", 30)" # 25 - 24s
        root -l -b -q "RunAnalyserPAF.C(\"TbarW & TbarW_ext\"							     , \"ttH\", 30)" # 25 - 25s
        root -l -b -q "RunAnalyserPAF.C(\"T_tch\"								               , \"ttH\", 50)" # 40 - 35s
        root -l -b -q "RunAnalyserPAF.C(\"Tbar_tch\"							             , \"ttH\", 35)" # 25 - 27s
        root -l -b -q "RunAnalyserPAF.C(\"TToLeptons_sch_amcatnlo\"				     , \"ttH\", 25)"
        root -l -b -q "RunAnalyserPAF.C(\"WZTo3LNu\"							             , \"ttH\", 30)" # 25 - 24s
        root -l -b -q "RunAnalyserPAF.C(\"WWTo2L2Nu\"							             , \"ttH\", 30)" # 25 - 22s
        root -l -b -q "RunAnalyserPAF.C(\"ZZ_ext\"								         , \"ttH\", 25)"
        #root -l -b -q "RunAnalyserPAF.C(\"ZZ & ZZ_ext\"								         , \"ttH\", 25)"

		echo "%%%%%> MC SIGNAL SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"TTHNonbb\"                       	   , \"ttH\", 35)" # 25 - 33s

		echo "%%%%%> DATA SAMPLES"
        root -l -b -q "RunAnalyserPAF.C(\"MuonEG\"                             , \"ttH\", 64)" #~100s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleMuon\"                         , \"ttH\", 64)" #~246s
        root -l -b -q "RunAnalyserPAF.C(\"DoubleEG\"                           , \"ttH\", 64)" #~187s
        root -l -b -q "RunAnalyserPAF.C(\"SingleElec\"                    	   , \"ttH\", 64)" #~158s
        root -l -b -q "RunAnalyserPAF.C(\"SingleMuon\"                         , \"ttH\", 64)" #~227s

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
elif [ "$1" == "pl" ]; then
  root -l -b -q "DrawPlots.C(\"Muon\")"
  root -l -b -q "DrawPlots.C(\"Elec\")"
  root -l -b -q "DrawPlots.C(\"ElMu\")"
  root -l -b -q "DrawPlots.C(\"3l\")"
  root -l -b -q "DrawPlots.C(\"4l\")"
else
    echo "ERROR - No valid arguments given"
    echo "Please, execute this script with a valid argument"
fi
