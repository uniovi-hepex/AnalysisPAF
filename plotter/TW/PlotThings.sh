source pre_start.sh

cd ..

# root -l -b -q "TW/runControlPlots.C()"
# root -l -b -q "TW/runFitPlots.C()"
# root -l -b -q "TW/runNjetNBjet.C()"
root -l -b -q "TW/run1j1tPlots.C()"
# root -l -b -q "TW/run2j1bBDTPlots.C()"

cd TW
