import ROOT
import math
import copy
from fitter import FittingSuite
###############################################################################

fit = FittingSuite("~/TW_differential/AnalysisPAF/plotter/TW/inputs/forCards_input.root",True)

histos = fit.pmap

results = {}

for key in histos:
    if key=='obs': continue
    result = copy.deepcopy(histos['obs']['data'])
    for key2 in histos[key]:
        if key2=='tW' : continue
        result.Add( histos[key][key2],-1)

    results[key] = result

histo = copy.deepcopy(results[''])
for bin in range(1, histo.GetNbinsX()+1):
    err = 0
    content = histo.GetBinContent(bin)
    for key in results:
        if 'Down' in key: continue
        maxDiff = max( map( abs, [results[key].GetBinContent(bin)-content, results[key.replace('Up','Down')].GetBinContent(bin)-content]))
        err = err + maxDiff**2
    histo.SetBinError( bin, math.sqrt(err))

out = ROOT.TFile.Open('classic.root','recreate')
histo.Write()
out.Close()
