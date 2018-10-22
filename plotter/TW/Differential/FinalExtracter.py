import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
import sys, os, copy, math
#===================================

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    print "> Chosen variable:", varName, "\n"
else:
    print "> Default choice of variable\n"
    varName     = 'LeadingLepEta'

r.gROOT.SetBatch(True)
procs = ['ttbar', 'DY', 'Non-WorZ', 'VVttbarV']
doSanityCheck = True

def getXsecForSys(syst, tfile):
    data = copy.deepcopy(tfile.Get('data_obs').Clone('data_%s'%syst))

    sysString = ('_' + syst) if (syst != '' and 'asimov' not in syst) else ''
    ttbar     = tfile.Get('ttbar' + sysString )
    dy        = tfile.Get('DY'    + sysString )
    fakes     = tfile.Get('Non-WorZ' + sysString )
    vvttbarv  = tfile.Get('VVttbarV' + sysString )
    
    if not ttbar:
        ttbar    = tfile.Get('ttbar')
    if not dy:
        dy       = tfile.Get('DY')
    if not fakes:
        fakes    = tfile.Get('Non-WorZ')
    if not vvttbarv:
        vvttbarv = tfile.Get('VVttbarV')
    
    if 'fsr' in syst or 'FSR' in syst or 'isr' in syst or 'ISR' in syst:
        ttbarclone = copy.deepcopy(ttbar.Clone("ttbarclone"))
        oldandgoodttbar = tfile.Get('ttbar')
        for bin in range(1, data.GetNbinsX() + 1):
            ttbarclone.SetBinContent(bin, oldandgoodttbar.GetBinContent(bin) + (ttbar.GetBinContent(bin) - oldandgoodttbar.GetBinContent(bin))/math.sqrt(2))
        data.Add(ttbarclone, -1)
        del ttbarclone, oldandgoodttbar
    else: data.Add( ttbar , -1 )
    data.Add(dy       , -1 )
    data.Add(fakes    , -1 )
    data.Add(vvttbarv , -1 )
    
    return data


def getXSecForNomSys(process, tfile, size):
    dataUp = copy.deepcopy(tfile.Get('data_obs').Clone('data_%sUp'%procs[procs2.index(proc)]))
    dataDn = copy.deepcopy(tfile.Get('data_obs').Clone('data_%sDown'%procs[procs2.index(proc)]))

    ttbar     = copy.deepcopy(tfile.Get('ttbar').Clone('ttbar'))
    dy        = copy.deepcopy(tfile.Get('DY').Clone('dy'))
    fakes     = copy.deepcopy(tfile.Get('Non-WorZ').Clone('fakes'))
    vvttbarv  = copy.deepcopy(tfile.Get('VVttbarV').Clone('vvttbarv'))
    
    eval(process).Scale( 1 + size)

    dataUp.Add( ttbar , -1 )
    dataUp.Add( dy    , -1 )
    dataUp.Add( fakes , -1 )
    dataUp.Add( vvttbarv , -1 )
    
    eval(process).Scale( 1 / (1+size)**2)

    dataDn.Add( ttbar , -1 )
    dataDn.Add( dy    , -1 )
    dataDn.Add( fakes , -1 )
    dataDn.Add( vvttbarv , -1 )
    
    del ttbar, dy, fakes, vvttbarv
    
    return dataUp, dataDn


def getLumiUnc(tfile):
    dataUp = copy.deepcopy(tfile.Get('data_obs').Clone('data_LumiUp'))
    dataDn = copy.deepcopy(tfile.Get('data_obs').Clone('data_LumiDown'))

    ttbar     = copy.deepcopy(tfile.Get('ttbar').Clone('ttbar'))
    dy        = copy.deepcopy(tfile.Get('DY').Clone('dy'))
    fakes     = copy.deepcopy(tfile.Get('Non-WorZ').Clone('fakes'))
    vvttbarv  = copy.deepcopy(tfile.Get('VVttbarV').Clone('vvttbarv'))
    
    ttbar.Scale(1 + vl.uncLumi)
    dy.Scale( 1 + vl.uncLumi)
    fakes.Scale( 1 + vl.uncLumi)
    vvttbarv.Scale( 1 + vl.uncLumi)

    dataUp.Add( ttbar , -1 )
    dataUp.Add( dy    , -1 )
    dataUp.Add( fakes , -1 )
    dataUp.Add( vvttbarv , -1 )
    
    ttbar.Scale( 1 / (1 + vl.uncLumi)**2)
    dy.Scale( 1 / (1 + vl.uncLumi)**2)
    fakes.Scale( 1 / (1 + vl.uncLumi)**2)
    vvttbarv.Scale( 1 / (1 + vl.uncLumi)**2)

    dataDn.Add( ttbar , -1 )
    dataDn.Add( dy    , -1 )
    dataDn.Add( fakes , -1 )
    dataDn.Add( vvttbarv , -1 )
    
    del ttbar, dy, fakes, vvttbarv
    
    return dataUp, dataDn


tfile   = r.TFile.Open('temp/{var}_/forCards_{var}.root'.format(var = varName), "read")
sysList = []

sysList += vl.varList['Names']['ExpSysts'] + vl.varList['Names']['ttbarSysts'] + vl.varList['Names']['colorSysts'] + vl.varList['Names']['specialSysts'] + ['DSUp', 'LumiUp', 'LumiDown', 'fsrUp', 'fsrDown', 'isrUp', 'isrDown', 'tWMEUp', 'tWMEDown'] + vl.varList['Names']['NormSysts']

nominal = getXsecForSys('', tfile)
variations = {}
for syst in sysList:
    variations[syst] = getXsecForSys(syst, tfile)

procs2 = ['ttbar','dy','fakes','vvttbarv']
for proc, size in zip(procs2, [0.04, 0.5, 0.5, 0.5]):
    dataUp, dataDn = getXSecForNomSys(proc, tfile, size)
    variations[procs[procs2.index(proc)] + 'Up']   = dataUp
    variations[procs[procs2.index(proc)] + 'Down'] = dataDn
    del dataUp, dataDn

dataUp, dataDn         = getLumiUnc(tfile)
variations['LumiUp']   = dataUp
variations['LumiDown'] = dataDn
tfile.Close()

if not vl.asimov:
    tfile      = r.TFile.Open('temp/{var}_/forCards_{var}_asimov.root'.format(var = varName), "read")
    asimov     = getXsecForSys('asimov', tfile)
    tfile.Close()
    

out = r.TFile.Open('temp/{var}_/cutOutput_{var}.root'.format(var = varName), 'recreate')
nominal.Write()

if not vl.asimov:
    asimov.Write()
for syst in sysList:
    variations[syst].Write()
out.Close()

scaleval = 1/vl.Lumi/1000 if vl.doxsec else 1
nominal.Scale(scaleval)
for key in variations:
    variations[key].Scale(scaleval)

nominal_withErrors  = ep.propagateHistoAsym(nominal, variations, True)
plot                = bp.beautifulUnfoldingPlots('{var}_folded'.format(var = varName))
plot.doRatio        = True
plot.doFit          = False
plot.plotspath      = "results/"

nominal.SetMarkerStyle(r.kFullCircle)
nominal.GetXaxis().SetNdivisions(505, True)

nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
nominal_withErrors[0].SetLineColor(r.kBlue)
nominal_withErrors[0].SetFillStyle(1001)

if "legpos_fold" in vl.varList[varName]: legloc = vl.varList[varName]["legpos_fold"]
else:                                    legloc = "TR"

if doSanityCheck:
    print '> Adding generated distribution with used software and others.'
    if not os.path.isfile('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName)):
        raise RuntimeError('The rootfile with the generated information does not exist')
    tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName))
    tru = copy.deepcopy(tmptfile.Get('tW'))
    tru.SetLineWidth(2)
    tru.SetLineColor(bp.colorMap[0])
    
    if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_recobinning_{var}.root'.format(var = varName)):
        raise RuntimeError('The rootfile with the generated aMCatNLO information does not exist')
    tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_recobinning_{var}.root'.format(var = varName))
    aMCatNLO = copy.deepcopy(tmptfile2.Get('tW'))
    aMCatNLO.SetLineWidth(2)
    aMCatNLO.SetLineColor(r.kAzure)
    
    if not os.path.isfile('temp/{var}_/ClosureTest_DS_recobinning_{var}.root'.format(var = varName)):
        raise RuntimeError('The rootfile with the generated DS variation information does not exist')
    tmptfile3 = r.TFile.Open('temp/{var}_/ClosureTest_DS_recobinning_{var}.root'.format(var = varName))
    hDS = copy.deepcopy(tmptfile3.Get('tW'))
    hDS.SetLineWidth(2)
    hDS.SetLineColor(r.kGreen)
    
    if nominal_withErrors[0].GetMaximum() <= tru.GetMaximum():      nominal_withErrors[0].SetMaximum(tru.GetMaximum())
    if nominal_withErrors[0].GetMaximum() <= aMCatNLO.GetMaximum(): nominal_withErrors[0].SetMaximum(aMCatNLO.GetMaximum())
    if nominal_withErrors[0].GetMaximum() <= hDS.GetMaximum():      nominal_withErrors[0].SetMaximum(hDS.GetMaximum())
    
    plot.addHisto(nominal_withErrors, 'hist',     'Total unc.',   'F', 'unc')
    plot.addHisto(tru,                'L,same',   'tW Powheg',    'L', 'mc')
    plot.addHisto(aMCatNLO,           'L,same',   'tW aMCatNLO',  'L', 'mc')
    plot.addHisto(hDS,                'L,same',   'tW DS',        'L', 'mc')
    plot.addHisto(nominal,            'P,E,same', vl.labellegend, 'P', 'data')
    plot.saveCanvas(legloc)
    tmptfile3.Close()
    tmptfile2.Close()
    tmptfile.Close()
else:
    plot.addHisto(nominal_withErrors, 'hist', 'Total unc.', 'F')
    plot.addHisto(nominal, 'P,same', vl.labellegend, 'P', 'data')
    plot.saveCanvas(legloc)
del plot


uncList = ep.getUncList(nominal, variations, True, False)
print "Full list of uncs.", uncList
uncList = uncList[:vl.nuncs]
plot    = bp.beautifulUnfoldingPlots('{var}uncertainties_folded'.format(var = varName))
uncList[0][1].Draw()

incmax  = []
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    if nominal_withErrors[1].GetBinError(bin) > nominal_withErrors[0].GetBinContent(bin):
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[0].GetBinContent(bin)]))
    else:
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[1].GetBinError(bin)]))

maxinctot = 0
hincmax   = copy.deepcopy(nominal.Clone('hincmax'))
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    hincmax.SetBinContent(bin, incmax[bin-1] / hincmax.GetBinContent(bin))
    hincmax.SetBinError(bin, 0.)
    if (hincmax.GetBinContent(bin) > maxinctot): maxinctot = hincmax.GetBinContent(bin)

hincmax.SetLineColor(r.kBlack)
hincmax.SetLineWidth( 2 )
hincmax.SetFillColorAlpha(r.kBlue, 0.)

#if (maxinctot >= 0.9):
    #if maxinctot >= 5:
        #uncList[0][1].GetYaxis().SetRangeUser(0., 5.)
    #else:
        #uncList[0][1].GetYaxis().SetRangeUser(0., maxinctot + 0.1)
#else:
    #uncList[0][1].GetYaxis().SetRangeUser(0., 0.9)

uncList[0][1].GetYaxis().SetRangeUser(0., 1.1)

for i in range(vl.nuncs):
    if 'statbin' not in uncList[i][0]: uncList[i][1].SetLineColor(vl.NewColorMap[uncList[i][0]])
    else:                              uncList[i][1].SetLineColor(r.kAzure)
    uncList[i][1].SetLineWidth( 2 )
    plot.addHisto(uncList[i][1], 'hist,same' if i else 'hist', uncList[i][0], 'L')

plot.addHisto(hincmax, 'hist,same', 'Total', 'L')
plot.plotspath = "results/"

if "uncleg_fold" in vl.varList[varName]: unclegpos = vl.varList[varName]["uncleg_fold"]
else:                                    unclegpos = "TR"

plot.saveCanvas(unclegpos)
del plot, variations, nominal, dataUp, dataDn

if not vl.asimov:
    tfile   = r.TFile.Open('temp/{var}_/forCards_{var}_goodasimov.root'.format(var = varName))
    nominal = getXsecForSys('', tfile)
    variations = {}
    for syst in sysList:
        variations[syst] = getXsecForSys(syst, tfile)

    procs2 = ['ttbar','dy','fakes','vvttbarv']
    for proc, size in zip(procs2, [0.04, 0.5, 0.5, 0.5]):
        dataUp, dataDn = getXSecForNomSys(proc, tfile, size)
        variations[procs[procs2.index(proc)] + 'Up']   = dataUp
        variations[procs[procs2.index(proc)] + 'Down'] = dataDn

    dataUp, dataDn         = getLumiUnc(tfile)
    variations['LumiUp']   = dataUp
    variations['LumiDown'] = dataDn
    tfile.Close()

    out = r.TFile.Open('temp/{var}_/cutOutput_{var}_goodasimov.root'.format(var = varName), 'recreate')
    nominal.Write()

    for syst in sysList:
        variations[syst].Write()
    out.Close()

    scaleval = 1/vl.Lumi/1000 if vl.doxsec else 1
    nominal.Scale(scaleval)
    for key in variations:
        variations[key].Scale(scaleval)

    nominal_withErrors  = ep.propagateHistoAsym(nominal, variations, True)
    plot                = bp.beautifulUnfoldingPlots('{var}_asimov_folded'.format(var = varName))
    plot.doRatio        = True
    plot.doFit          = False
    plot.plotspath      = "results/"

    nominal.SetMarkerStyle(r.kFullCircle)
    nominal.GetXaxis().SetNdivisions(505, True)

    nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
    nominal_withErrors[0].SetLineColor(r.kBlue)
    nominal_withErrors[0].SetFillStyle(1001)

    if "legpos_foldas" in vl.varList[varName]: legloc_as = vl.varList[varName]["legpos_foldas"]
    else:                                      legloc_as = "TR"

    if doSanityCheck:
        print '> Adding generated distribution with used software and others.'
        if not os.path.isfile('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName)):
            raise RuntimeError('The rootfile with the generated information does not exist')
        tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = varName))
        tru = copy.deepcopy(tmptfile.Get('tW'))
        tru.SetLineWidth(2)
        tru.SetLineColor(bp.colorMap[0])
        
        if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_recobinning_{var}.root'.format(var = varName)):
            raise RuntimeError('The rootfile with the generated aMCatNLO information does not exist')
        tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_recobinning_{var}.root'.format(var = varName))
        aMCatNLO = copy.deepcopy(tmptfile2.Get('tW'))
        aMCatNLO.SetLineWidth(2)
        aMCatNLO.SetLineColor(r.kAzure)
        
        if not os.path.isfile('temp/{var}_/ClosureTest_DS_recobinning_{var}.root'.format(var = varName)):
            raise RuntimeError('The rootfile with the generated DS variation information does not exist')
        tmptfile3 = r.TFile.Open('temp/{var}_/ClosureTest_DS_recobinning_{var}.root'.format(var = varName))
        hDS = copy.deepcopy(tmptfile3.Get('tW'))
        hDS.SetLineWidth(2)
        hDS.SetLineColor(r.kGreen)
        
        if nominal_withErrors[0].GetMaximum() <= tru.GetMaximum():      nominal_withErrors[0].SetMaximum(tru.GetMaximum())
        if nominal_withErrors[0].GetMaximum() <= aMCatNLO.GetMaximum(): nominal_withErrors[0].SetMaximum(aMCatNLO.GetMaximum())
        if nominal_withErrors[0].GetMaximum() <= hDS.GetMaximum():      nominal_withErrors[0].SetMaximum(hDS.GetMaximum())
        
        plot.addHisto(nominal_withErrors, 'hist',     'Total unc.',   'F', 'unc')
        plot.addHisto(tru,                'L,same',   'tW Powheg',    'L', 'mc')
        plot.addHisto(aMCatNLO,           'L,same',   'tW aMCatNLO',  'L', 'mc')
        plot.addHisto(hDS,                'L,same',   'tW DS',        'L', 'mc')
        plot.addHisto(nominal,            'P,E,same', "Pseudodata",   'P', 'data')
        plot.saveCanvas(legloc_as)
        tmptfile2.Close()
        tmptfile.Close()
    else:
        plot.addHisto(nominal_withErrors, 'hist', 'Total unc.', 'F')
        plot.addHisto(nominal, 'P,same', "Pseudodata", 'P', 'data')
        plot.saveCanvas(legloc_as)
    del plot


    uncList = ep.getUncList(nominal, variations, True, False)[:vl.nuncs]
    plot    = bp.beautifulUnfoldingPlots('{var}uncertainties_asimov_folded'.format(var = varName))
    uncList[0][1].Draw()

    incmax  = []
    for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
        if nominal_withErrors[1].GetBinError(bin) > nominal_withErrors[0].GetBinContent(bin):
            incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[0].GetBinContent(bin)]))
        else:
            incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[1].GetBinError(bin)]))

    maxinctot = 0
    hincmax   = copy.deepcopy(nominal.Clone('hincmax'))
    for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
        hincmax.SetBinContent(bin, incmax[bin-1] / hincmax.GetBinContent(bin))
        hincmax.SetBinError(bin, 0.)
        if (hincmax.GetBinContent(bin) > maxinctot): maxinctot = hincmax.GetBinContent(bin)

    hincmax.SetLineColor(r.kBlack)
    hincmax.SetLineWidth( 2 )
    hincmax.SetFillColorAlpha(r.kBlue, 0.)

    #if (maxinctot >= 0.9):
        #if maxinctot >= 5:
            #uncList[0][1].GetYaxis().SetRangeUser(0., 5.)
        #else:
            #uncList[0][1].GetYaxis().SetRangeUser(0., maxinctot + 0.1)
    #else:
        #uncList[0][1].GetYaxis().SetRangeUser(0., 0.9)

    uncList[0][1].GetYaxis().SetRangeUser(0., 1.1)
    
    for i in range(vl.nuncs):
        if 'statbin' not in uncList[i][0]: uncList[i][1].SetLineColor(vl.NewColorMap[uncList[i][0]])
        else:                              uncList[i][1].SetLineColor(r.kAzure)
        uncList[i][1].SetLineWidth( 2 )
        plot.addHisto(uncList[i][1], 'hist,same' if i else 'hist', uncList[i][0], 'L')

    plot.addHisto(hincmax, 'hist,same', 'Total', 'L')
    plot.plotspath = "results/"
    
    if "uncleg_foldas" in vl.varList[varName]: unclegpos_as = vl.varList[varName]["uncleg_foldas"]
    else:                                      unclegpos_as = "TR"
    
    plot.saveCanvas(unclegpos_as)
    del plot