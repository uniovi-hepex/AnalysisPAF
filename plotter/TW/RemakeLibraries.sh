cd ..

rm Looper_* Histo_* Plot_* TResultsTable_* Datacard_* TW/AdditionalStuff_*

source TW/pre_start.sh

root -b -l -q 'TW/CreateLibraries.C'

cd TW
