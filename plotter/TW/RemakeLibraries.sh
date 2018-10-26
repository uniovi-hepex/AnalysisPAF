cd ..

rm Looper_* Histo_* Plot_C* TResultsTable_* TW/AdditionalStuff_* PlotToPy_* PlotToPyC_* PDFunc_* Datacard_*

source TW/pre_start.sh

root -b -l -q 'TW/CreateLibraries.C'

cd TW
