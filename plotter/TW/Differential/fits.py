import fitter, varList
import ROOT
import array, math, sys
import beautifulUnfoldingPlots

print "===== Fitting procedure\n"
expected    = True

if (len(sys.argv) > 1):
    name = sys.argv[1]
    print "> Variable to be unfolded:\n", name
else:
    print "> Default variable chosen\n"
    name        = 'LeadingJetPt'

bins        = varList.varList[name]['recobinning']
count       = 0
histo       = ROOT.TH1F("histo", "", len(bins)-1, array.array('d',bins))
allResults  = {}


for binDn,binUp in zip(bins, bins[1:]):
    print "\n/////////// FITTING BIN NUMBER", count, "///////////\n"
    count   = count + 1
    fit     = fitter.FittingSuite("temp/forCards_%s_%d.root"%(name,count), expected)
    fit.VarName = name
    fit.doAllCombFits()
    results = fit.results
    allResults[(binDn,binUp)] = results
    err     = 0

    # Add uncertainty of systematics
    for key in results: 
        if key == 'obs' or key == '' or 'statbin' in key or 'Down' in key: continue
        maxDiff = max(map(abs, [results[''][0]-results[key], results[''][0]-results[key.replace('Up','Down')]]))
        err = err + maxDiff**2

    # Add uncertainty of the fit
        err = err + max(map(abs,[results[''][0]-results[''][1],results[''][0]-results[''][2]]))**2
    
    histo.SetBinContent( histo.FindBin( (binDn+binUp)/2), results[''][0]       )
    histo.SetBinError  ( histo.FindBin( (binDn+binUp)/2), math.sqrt(err)       )


print '> Saving folded distribution in rootfile\n'
out = ROOT.TFile('temp/output_%s.root'%name, 'recreate')
histo.Write()
out.Close()


print "> Treating unc. histograms and constructing dummy card for Pietro's code\n"
histoSyst = {}
for key in allResults[(bins[0],bins[1])]: 
    if key == 'obs' or 'statbin' in key: continue
    histoSyst[key] =  ROOT.TH1F("tW" + ("_" + key if key != "" else ''), "", len(bins) - 1, array.array('d', bins))

    for binDn,binUp in zip(bins, bins[1:]):
        if key != '':
            histoSyst[key].SetBinContent( histoSyst[key].FindBin((binUp+binDn)/2), allResults[(binDn,binUp)][key])
        else:
            histoSyst[key].SetBinContent( histoSyst[key].FindBin((binUp+binDn)/2), allResults[(binDn,binUp)][key][0])
            histoSyst[key].SetBinError  ( histoSyst[key].FindBin((binUp+binDn)/2), max( map(abs, [allResults[(binDn,binUp)][key][0]-allResults[(binDn,binUp)][key][1],allResults[(binDn,binUp)][key][0]-allResults[(binDn,binUp)][key][2]])))

outCard = open('dummy_card_template.txt').read()
print outCard
outCard = outCard.format(obs=histoSyst[''].Integral(),tW=histoSyst[''].Integral(),ref=name)
out = open('temp/dummy_card_%s.txt'%name, 'w')
out.write(outCard)

cardInput = ROOT.TFile.Open('temp/cardFile_{ref}.root'.format(ref=name), 'recreate')

for key in histoSyst: histoSyst[key].Write()
cardInput.Close()


print "> Saving folded-space histogram of the variable"
plot = beautifulUnfoldingPlots.beautifulUnfoldingPlots(name)
plot.addHisto(histo, 'hist', 'Folded distribution', 'L')
plot.plotspath  = "results/"
plot.saveCanvas('TR','_folded')

print "> Fitting procedure done!"
