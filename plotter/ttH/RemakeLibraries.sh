cd ..

rm Looper_* Histo_* Plot_* TResultsTable_* Datacard_*

source /cms/slc6_amd64_gcc530/external/gcc/5.3.0/etc/profile.d/init.sh
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/init.sh
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/dependencies-setup.sh
source /cms/slc6_amd64_gcc530/external/cmake/3.5.2/etc/profile.d/init.sh
source /opt/root6/bin/thisroot.sh

root -b -l -q 'ttH/CreateLibraries.C'

cd ttH
