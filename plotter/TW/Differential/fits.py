import fitter
import ROOT
import array
import math


expected = True

count = 0
bins = [0,40,80,120]

histo = ROOT.TH1F("histo","",3, array.array('d',bins))


for binDn,binUp in zip(bins, bins[1:]):
    count = count + 1 
    fit = fitter.FittingSuite("~/TW_differential/AnalysisPAF/plotter/TW/inputs/forCards_Test_%d.root"%count, expected)
    fit.doAllCombFits()
    results = fit.results

    content = results['']
    err = 0
    for key in results: 
        if key == 'obs' or key == '' or 'statbin' in key or 'Down' in key: continue
        maxDiff = max(map(abs, [results['']-results[key], results['']-results[key.replace('Up','Down')]]))
        err = err + maxDiff**2
        #print key.replace('Up',''), maxDiff/results['']
    
    print '#',fit.pmap['']['tW'].Integral(), content, math.sqrt(err)

    histo.SetBinContent( histo.FindBin( (binDn+binUp)/2), fit.pmap['']['tW'].Integral()*content       )
    histo.SetBinError  ( histo.FindBin( (binDn+binUp)/2), fit.pmap['']['tW'].Integral()*math.sqrt(err))


out = ROOT.TFile('output.root','recreate')
histo.Write()
out.Close()
