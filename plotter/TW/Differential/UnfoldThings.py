import unfoldTW as unf
import sys

print "===== Unfolding procedure\n"
if (len(sys.argv) > 1):
    varName = sys.argv[1]
    print "> Variable to be unfolded:", varName, "\n"
    if (len(sys.argv) > 2):
        pathtothings    = sys.argv[2]
        print "> Special path to things chose:", pathtothings, "\n"
    else:
        pathtothings    = 'temp/'
    
else:
    print "> Default variable and path chosen\n"
    varName       = 'LeadingJetPt'
    pathtothings  = 'temp/'


print "\n> Beginning unfolding...\n"
a = unf.Unfolder(varName, pathtothings + 'cardFile_' + varName + '.root', pathtothings + 'UnfoldingInfo.root')
a.plotspath       = "results/"
a.doSanityCheck   = True
a.doColorEnvelope = True
a.prepareNominalHelper()
a.doLCurveScan()
#a.doTauScan()
a.doScanPlots()
a.doNominalPlot()
#a.doUnfoldingForAllNuis()    # Symmetric uncertainties
a.doUnfoldingForAllNuis(True) # Asymmetric uncertainties

a = None
print "> Unfolding done!"
