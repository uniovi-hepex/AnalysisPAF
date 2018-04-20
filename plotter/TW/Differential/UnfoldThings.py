import unfoldTW as unf
import sys

print "===== Unfolding procedure\n"
if (len(sys.argv) > 1):
    varName = sys.argv[1]
    print "> Variable to be unfolded:\n", varName
    if (len(sys.argv) > 2):
        pathtothings    = sys.argv[2]
        print "> Special path to things chose:\n", pathtothings
    else:
        pathtothings    = 'temp/'
    
else:
    print "> Default variable and path chosen\n"
    varName = 'LeadingJetPt'


print "> Beginning unfolding"
a = unf.Unfolder(varName, pathtothings + 'cardFile_' + varName + '.root', pathtothings + 'UnfoldingInfo.root')
a.plotspath = "results"
a.prepareNominalHelper()
a.doLCurveScan()
#a.doTauScan()
a.doScanPlots()
a.doNominalPlot()
a.doUnfoldingForAllNuis()

a   = None
print "> Unfolding done!"
