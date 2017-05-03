source /cms/slc6_amd64_gcc530/external/gcc/5.3.0/etc/profile.d/init.sh
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/init.sh
source /cms/slc6_amd64_gcc530/external/python/2.7.11-giojec2/etc/profile.d/dependencies-setup.sh
source /cms/slc6_amd64_gcc530/external/cmake/3.5.2/etc/profile.d/init.sh
source /opt/root6/bin/thisroot.sh

cd ..

root -l -b -q "ttH/DrawPlots.C(\"Muon\" , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"Elec\" , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"ElMu\" , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"2lSS\" , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"3l\"   , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"4l\"   , \"$1\")"
root -l -b -q "ttH/DrawPlots.C(\"All\"  , \"$1\")"

cd ttH
