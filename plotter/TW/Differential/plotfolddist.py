import ROOT  as r
import sys, copy, os
import tdrstyle, CMS_lumi
import varList as vl
import errorPropagator as ep
import beautifulUnfoldingPlots as bp

r.gROOT.SetBatch(True)
CMS_lumi.writeExtraText = 1
doSanityCheck           = True
labellegend             = 'Data'
#labellegend             = 'Pseudodata'

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
else:
    print "> Default choice of variable\n"
    varName     = 'LeadingLepEta'

print '\n> Plotting the nominal folded distribution with uncertainties for the variable', varName, '\n'
fitinfo     = r.TFile.Open('temp/{var}_/fitOutput_{var}.root'.format(var = varName), 'read')
if not fitinfo:
    raise RuntimeError('Post-fit info. of variable {var} has not been found. This might have happened because the fit did not converge: check fit procedure logs.'.format(var = varName))

listofkeys  = fitinfo.GetListOfKeys()
if 'hFitResult_%s_'%varName not in listofkeys:
    raise RuntimeError('Nominal histogram was not found.')
if 'hFitResult_forPlotting_%s_'%varName not in listofkeys:
    raise RuntimeError('Fit uncertainties from nominal values were not found.')

nominal     = r.TH1F()
errors      = r.TH1F()
nominal     = copy.deepcopy(fitinfo.Get('hFitResult_%s_'%varName))
errors      = copy.deepcopy(fitinfo.Get('hFitResult_forPlotting_%s_'%varName))

allHistos = {}
for nuis in vl.systMap:
    if 'hFitResult_%s_%s'%(varName, nuis) not in listofkeys:
        raise RuntimeError('Variations of the variable ' + varName + ' with the systematic ' + nuis + ' were not found.')
    allHistos[nuis] = copy.deepcopy(fitinfo.Get('hFitResult_%s_%s'%(varName, nuis)))
fitinfo.Close()

for bin in range(1, errors.GetNbinsX() + 1):
    nominal.SetBinError(bin, errors.GetBinContent(bin))

nominal_withErrors  = ep.propagateHistoAsym(nominal, allHistos, True)
plot                = bp.beautifulUnfoldingPlots(varName + '_folded')
plot.plotspath      = "results/"
nominal.SetMarkerStyle(r.kFullCircle)
nominal.GetXaxis().SetNdivisions(505, True)

nominal_withErrors[0].SetFillColorAlpha(r.kBlue,0.35)
nominal_withErrors[0].SetLineColor(r.kBlue)
nominal_withErrors[0].SetFillStyle(1001)

if doSanityCheck:
    print '> Adding generated distribution.'
    if not os.path.isfile('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName)):
        raise RuntimeError('The rootfile with the generated information does not exist')
    tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName))
    tru = copy.deepcopy(tmptfile.Get('tW'))
    tru.SetLineWidth(2)
    tru.SetLineColor(bp.colorMap[0])
    for bin in range(1, tru.GetNbinsX()): tru.SetBinError(bin, 0.)
    if nominal_withErrors[0].GetMaximum() <= tru.GetMaximum(): nominal_withErrors[0].SetMaximum(tru.GetMaximum() + 1000)
    plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
    plot.addHisto(tru, 'L,same', 'tW Powheg', 'L')
    plot.addHisto(nominal, 'P,E,same', labellegend, 'P')
    plot.saveCanvas('TC')
    tmptfile.Close()
else:
    plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
    plot.addHisto(nominal, 'P,same', labellegend, 'P')
    plot.saveCanvas('TC')
del plot


uncList = ep.getUncList(nominal, allHistos, True)[:5]
#print uncList
plot    = bp.beautifulUnfoldingPlots(varName + 'uncertainties_folded')
uncList[0][1].Draw()

if uncList[0][1].GetMaximum() < 0.5:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.5)
else:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.9)
for i in range(5):
    uncList[i][1].SetLineColor(bp.colorMap[i])
    uncList[i][1].SetLineWidth( 2 )
    plot.addHisto(uncList[i][1], 'H,same' if i else 'H', uncList[i][0], 'L')

plot.plotspath = "results/"
plot.saveCanvas('TC')
