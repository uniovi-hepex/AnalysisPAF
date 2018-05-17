import ROOT  as r
import sys, copy
import tdrstyle, errorPropagator, varList, CMS_lumi
from beautifulUnfoldingPlots import beautifulUnfoldingPlots, colorMap

CMS_lumi.writeExtraText = 1
r.gROOT.SetBatch(True)

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
else:
    print "> Default choice of variable\n"
    varName     = 'LeadingLepEta'

print '> Plotting the nominal folded distribution with uncertainties for the variable', varName
fitinfo     = r.TFile.Open('temp/fitOutput_%s.root'%varName, 'read')
listofkeys  = fitinfo.GetListOfKeys()
nominal     = r.TH1F()
errors      = r.TH1F()
if 'hFitResult_%s_'%varName not in listofkeys:
    raise RuntimeError('Nominal histogram was not found.')
if 'hFitResult_forPlotting_%s_'%varName not in listofkeys:
    raise RuntimeError('Fit uncertainties from nominal values were not found.')
nominal     = copy.deepcopy(fitinfo.Get('hFitResult_%s_'%varName))
errors      = copy.deepcopy(fitinfo.Get('hFitResult_forPlotting_%s_'%varName))

allHistos = {}
for nuis in varList.systMap:
    if 'hFitResult_%s_%s'%(varName, nuis) not in listofkeys:
        raise RuntimeError('Variations of the variable ' + varName + ' with the systematic ' + nuis + ' were not found.')
    allHistos[nuis] = copy.deepcopy(fitinfo.Get('hFitResult_%s_%s'%(varName, nuis)))
fitinfo.Close()


for bin in range(1, errors.GetNbinsX() + 1):
    nominal.SetBinError(bin, errors.GetBinContent(bin))

nominal_withErrors = errorPropagator.propagateHistoAsym(nominal, allHistos, True)
plot = beautifulUnfoldingPlots(varName + '_folded')
plot.plotspath  = "results/"
nominal.SetMarkerStyle(r.kFullCircle)
nominal.GetXaxis().SetNdivisions(505, True)

nominal_withErrors[0].SetFillColorAlpha(r.kBlue,0.35)
nominal_withErrors[0].SetLineColor(r.kBlue)
nominal_withErrors[0].SetFillStyle(1001)
plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
plot.addHisto(nominal, 'P,same', 'Pseudodata', 'P')
plot.saveCanvas('TR')
del plot


uncList = errorPropagator.getUncList(nominal, allHistos, True)[:5]
#print uncList
plot    = beautifulUnfoldingPlots(varName + 'uncertainties_folded')
uncList[0][1].Draw()

if uncList[0][1].GetMaximum() < 0.5:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.5)
else:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.75)
for i in range(5):
    uncList[i][1].SetLineColor(colorMap[i])
    uncList[i][1].SetLineWidth( 2 )
    plot.addHisto(uncList[i][1], 'H,same' if i else 'H', uncList[i][0], 'L')

plot.plotspath = "results/"
plot.saveCanvas('TC')
