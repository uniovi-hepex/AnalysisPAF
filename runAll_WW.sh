# Start root 6
source /cms/slc6_amd64_gcc530/external/gcc/5.3.0/etc/profile.d/init.sh; 
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/init.sh; 
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/dependencies-setup.sh; 
source /cms/slc6_amd64_gcc530/external/cmake/3.5.2/etc/profile.d/init.sh;source /opt/root6/bin/thisroot.sh

# Start PoD
source /opt/PoD/PoD_env.sh
# Start PAF
source /opt/PAF/PAF_setup.sh

resetpaf

#root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg", "WW", 16)' #//la nueva hay que poner la de 15jets
#(haciendo la trampa de cambiar el datasets) root -l -b -q 'RunAnalyserPAF.C("Tbar_Powheg", "WW", 40, -4)'


root -l -b -q 'RunAnalyserPAF.C("Tbar_Powheg", "WW", 40)'
#root -l -b -q 'RunAnalyserPAF.C("TTbar_Powheg", "WW", 16)'

root -l -b -q 'RunAnalyserPAF.C("TW_ext", "WW", 10)'
root -l -b -q 'RunAnalyserPAF.C("TbarW_ext", "WW", 8)'
#root -l -b -q 'RunAnalyserPAF.C("ZZ_ext", "WW", 16)'
#root -l -b -q 'RunAnalyserPAF.C("WW_ext", "WW", 20)'  #esta está incluida en la WWTo2L2Nu de debajo. No es necesario correrla.
#root -l -b -q 'RunAnalyserPAF.C("WWTo2L2Nu", "WW", 4)'
#root -l -b -q 'RunAnalyserPAF.C("GGWWTo2L2Nu", "WW", 16)'
#root -l -b -q 'RunAnalyserPAF.C("WZ_ext", "WW", 16)'

root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M50_MLM_ext", "WW", 16)'
#root -l -b -q 'RunAnalyserPAF.C("DYJetsToLL_M5to50_MLM",  "WW", 16)'

#root -l -b -q 'RunAnalyserPAF.C("WJetsToLNu_MLM & WJetsToLNu_MLM_ext2", "WW", 16)'

#root -l -b -q 'RunAnalyserPAF.C("TTWToLNu_ext2", "WW", 8)'
#root -l -b -q 'RunAnalyserPAF.C("TTZToQQ", "WW", 16)'
root -l -b -q 'RunAnalyserPAF.C("TTGJets_ext", "WW", 4)'   
#root -l -b -q 'RunAnalyserPAF.C("TTWToQQ", "WW", 16)'

#root -l -b -q 'RunAnalyserPAF.C("ggHWWTo2L2Nu", "WW", 16)'

#muestras datos
#root -l -b -q 'RunAnalyserPAF.C("MuonEG", "WW", 16)'                             
#root -l -b -q 'RunAnalyserPAF.C("DoubleMuon", "WW", 16)'                             
#root -l -b -q 'RunAnalyserPAF.C("DoubleEG", "WW", 16)'
#root -l -b -q 'RunAnalyserPAF.C("SingleMuon", "WW", 16)'                      
#root -l -b -q 'RunAnalyserPAF.C("SingleElec", "WW", 16)'


