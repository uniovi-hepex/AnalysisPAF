import ROOT as r
import math, copy
from array import array
import varList as vl
###############################################################################

def quadSum(elements):
    return math.sqrt( sum( map( lambda x : x*x, elements)))


def GetMaxUnc(nominal, uncUp, uncDown):
    return max( map(lambda x : x*x, [nominal - uncUp, nominal - uncDown]))


def propagateQuantity(nom, varDict, case = 0):
    tot = 0.
    if case == 0:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key:
                raise RuntimeError('Use case is not supported yet. Syst %s is not supported'%key)

            if key.replace('Up','Down') in varDict:
                tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
            else:
                tot = tot + max( map(lambda x : x*x, [nom - varDict[key]]))
            
            #if key.replace('Up','Down') in varDict:
                #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                    #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])/2
                #else:
                    #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
            #else:
                #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                    #tot = tot + max( map(lambda x : x*x, [nom - varDict[key]]))/2
                #else:
                    #tot = tot + max( map(lambda x : x*x, [nom - varDict[key]]))
    elif case > 0:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key and key not in vl.varList['Names']['colorSysts']:
                raise RuntimeError('Use case is not supported yet. Syst %s is not supported'%key)

            if key.replace('Up','Down') in varDict:
                if (nom - varDict[key] > 0 and nom - varDict[key.replace('Up', 'Down')] > 0):
                    continue
                elif (nom - varDict[key] < 0 and nom - varDict[key.replace('Up', 'Down')] < 0):
                    tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
                    #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                        #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])/2
                    #else:
                        #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
                else:
                    if (nom - varDict[key] < 0):
                        #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]/2
                        #else:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                        tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                    else:
                        #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]/2
                        #else:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]
                        tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]
    else:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key and key not in vl.varList['Names']['colorSysts']:
                raise RuntimeError('Use case is not supported yet. Syst %s is not supported'%key)

            if key.replace('Up','Down') in varDict:
                if (nom - varDict[key] < 0 and nom - varDict[key.replace('Up', 'Down')] < 0): 
                    continue
                elif (nom - varDict[key] > 0 and nom - varDict[key.replace('Up', 'Down')] > 0):
                    #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                        #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])/2
                    #else:
                        #tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
                    tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
                else:
                    if (nom - varDict[key] > 0):
                        #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]/2
                        #else:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                        tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                    else:
                        #if 'fsr' in key or 'FSR' in key or 'isr' in key or 'ISR' in key:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]/2
                        #else:
                            #tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]
                        tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]
    return math.sqrt(tot)


def propagateHisto( nom, varDict, doEnv = False ):
    out = nom.Clone(nom.GetName() + '_unc')
    
    if doEnv:
        for bin in range(1, out.GetNbinsX() + 1):
            err     = out.GetBinError(bin)          # <== Fit unc. taken here
            cont    = out.GetBinContent(bin)
            tmpDict = dict([ (key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            tmpunc  = 0.
            for (key, histo) in varDict.iteritems():
                if key in vl.varList['Names']['colorSysts']:
                    if tmpunc < GetMaxUnc(cont, tmpDict[key], tmpDict[key]):
                        tmpunc = GetMaxUnc(cont, tmpDict[key], tmpDict[key])
                    del tmpDict[key]
            out.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict), err, math.sqrt(tmpunc)]))
    else:
        for bin in range(1, out.GetNbinsX() + 1):
            err     = out.GetBinError(bin)          # <== Fit unc. taken here
            cont    = out.GetBinContent(bin)
            tmpDict = dict([ (key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            out.SetBinError( bin, quadSum([propagateQuantity(cont, tmpDict), err]))
    return out


def propagateHistoAsym(nom, varDict, doEnv = False, doAsimov = False):
    outUp   = nom.Clone(nom.GetName() + '_uncUp')
    outDown = nom.Clone(nom.GetName() + '_uncDown')
    
    if doAsimov:
        if 'asimov' not in varDict: raise RuntimeError("Asimov variation has not been found in the dictionary provided to the error propagation procedure.")
        hasimov = copy.deepcopy(varDict['asimov'])
    
    if doEnv:
        for bin in range(1, nom.GetNbinsX() + 1):
            err         = outUp.GetBinError(bin)    # <==  Fit unc. (or stat. and unf. uncs.) taken here
            cont        = outUp.GetBinContent(bin)
            tmpDict     = dict([(key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            tmpuncUp    = 0.
            tmpuncDown  = 0.
            tmpunc      = 0.
            valasimov   = 0.
            for (key, histo) in varDict.iteritems():
                if key in vl.varList['Names']['colorSysts']:
                    tmpunc  = tmpDict[key] - cont
                    if tmpunc > tmpuncUp:
                        tmpuncUp    = tmpunc
                    elif tmpunc < tmpuncDown:
                        tmpuncDown  = tmpunc
                    del tmpDict[key]
            if doAsimov: valasimov = hasimov.GetBinError(bin)
            if 'asimov' in varDict: del tmpDict['asimov']
            outUp.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, +1), err, tmpuncUp, valasimov]))
            outDown.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, -1), err, abs(tmpuncDown), valasimov]))
    else:
        for bin in range(1, nom.GetNbinsX() + 1):
            err     = outUp.GetBinError(bin)    # <==  Fit unc. taken here
            cont    = outUp.GetBinContent(bin)
            tmpDict = dict([(key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            if doAsimov: valasimov = hasimov.GetBinError(bin)
            if 'asimov' in varDict: del tmpDict['asimov']
            outUp.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, +1), err, valasimov]))
            outDown.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, -1), err, valasimov]))
    return [outUp, outDown]


def maxRelativeError(histo):
    try:
        return max( [histo.GetBinError(bin) / histo.GetBinContent(bin) for bin in range(1, histo.GetNbinsX()+1)])
    except ZeroDivisionError:
        raise RuntimeError('There is (at least) one bin with zero contents')


def relativeErrorHist(histo):
    relErr = histo.Clone( histo.GetName() + 'relErr')
    for bin in range(1, histo.GetNbinsX()+1):
        relErr.SetBinContent(bin, histo.GetBinError(bin) / histo.GetBinContent(bin) )
        relErr.SetBinError(bin, 0)
    return relErr

def MeanUncertaintyHisto(histo):
    try:
        return vl.mean( [histo.GetBinError(bin) / histo.GetBinContent(bin) for bin in range(1, histo.GetNbinsX()+1)])
    except ZeroDivisionError:
        raise RuntimeError('There is (at least) one bin with zero contents')

def maximumHisto(histo1, histo2):
    maxHist = histo1.Clone( histo1.GetName() + 'max' ) 
    for bin in range(1, maxHist.GetNbinsX()+1):
        maxHist.SetBinContent( bin, max(histo1.GetBinContent(bin), histo2.GetBinContent(bin)))
    return maxHist


def getUncList(nom, varDict, doEnv = False, doFit = True, doAsimov = False):
    if doFit:    medDict = [('Fit', nom)]
    else:        medDict = [('Stat.', nom)]
    if doAsimov: medDict.append(( 'asimov', varDict['asimov'])) # Note: the asimov histogram must arrive here with the
                                                                # proper uncs. in the bin errors and the nominal contents.
    outDict = []
    histUp  = [[] for i in range(nom.GetNbinsX())]
    histDown= [[] for i in range(nom.GetNbinsX())]
    for var in varDict:
        if var == 'asimov': continue
        hist    = nom.Clone(nom.GetName() + var)
        variat  = 0.
        for bin in range(1, nom.GetNbinsX() + 1):
            variat = nom.GetBinContent(bin) - varDict[var].GetBinContent(bin)
            #if 'fsr' in var or 'FSR' in var or 'isr' in var or 'ISR' in var: variat/(2**(1/2))
            if doEnv and var in vl.varList['Names']['colorSysts']:
                if (variat >= 0):
                    if abs(variat) >= nom.GetBinContent(bin): histDown[bin - 1].append(nom.GetBinContent(bin))
                    else:                                     histDown[bin - 1].append(abs(variat))
                else: histUp[bin - 1].append(abs(variat))
            else:
                if abs(variat) >= nom.GetBinContent(bin): hist.SetBinError(bin, nom.GetBinContent(bin))
                else:                                     hist.SetBinError(bin, abs(variat))
        if not doEnv or var not in vl.varList['Names']['colorSysts']:
            medDict.append( (var,hist) )
    
    if doEnv:
        finalhistUp     = nom.Clone(nom.GetName() + 'ColorRUp')
        finalhistDown   = nom.Clone(nom.GetName() + 'ColorRDown')
        for bin in range(1, nom.GetNbinsX() + 1):
            finalhistUp.SetBinError(bin, max(histUp[bin - 1] + [0]))
            finalhistDown.SetBinError(bin, max(histDown[bin - 1] + [0]))
        medDict.append(('ColorRUp', finalhistUp))
        medDict.append(('ColorRDown', finalhistDown))
    
    medDict.sort(key = lambda x : MeanUncertaintyHisto(x[1]), reverse = True)
    #medDict.sort(key = lambda x : maxRelativeError(x[1]), reverse = True)
    #medDict.sort(key = lambda x : relativeErrorHist(x[1]).GetBinContent(1), reverse = True)
    
    for key in medDict:
        done = False
        for i in range(len(outDict)):
            if key[0].replace('Up', '').replace('Down', '') == outDict[i][0]: done = True
        if done: continue
        if 'Down' in key[0]:
            for key2 in medDict:
                if key2[0] == key[0].replace('Down','Up'):
                    down = key2
            hist = maximumHisto(key[1], key2[1])
            outDict.append( (key[0].replace('Down','').replace('_',''), hist) )
        elif 'Up' in key[0]:
            for key2 in medDict:
                if key2[0] == key[0].replace('Up','Down'):
                    down = key2
            if key[0].replace('Up', 'Down') in medDict:
                hist = maximumHisto(key[1], key2[1])
                outDict.append( (key[0].replace('Up','').replace('_',''), hist) )
            else:
                outDict.append( (key[0].replace('Up','').replace('_',''), key[1]) )
        else: # We expect only the fit unc. to arrive here
            outDict.append( (key[0].replace('_',''), key[1]) )
    
    return map( lambda x : (x[0], relativeErrorHist(x[1])), outDict)


def SetTheStatsUncs(histo):
    ''' Function that sets the uncs. of the given histogram as the expecteds
    from its contents.'''
    for bin in range(1, histo.GetNbinsX() + 1):
        histo.SetBinError(bin, math.sqrt(histo.GetBinContent(bin)))
    
    return


def SetTheUncsFromHere(histo, hlist, SetStatUncs = False):
    ''' Function that returns to you a histogram with the quadratic sum
    of the uncertainties corresponding to the bin uncertainties of the 
    histograms that are given in a list, in addition to the ones from the
    given histogram.'''
    
    tmperr = 0
    for i in range(1, histo.GetNbinsX() + 1):
        if SetStatUncs: tmperr = quadSum([math.sqrt(hel.GetBinContent(bin)) for hel in hlist] + math.sqrt(histo.GetBinContent(bin)))
        else:           tmperr = quadSum([hel.GetBinError(bin) for hel in hlist] + histo.GetBinError(bin))
        
        histo.SetBinError(bin, tmperr)
    
    return


def getCovarianceFromVar(nom, var, name, ty = "folded", doCorr = False):
    nbins   = nom.GetXaxis().GetNbins()
    binning = array('f', vl.varList[name]['recobinning'] if ty == "folded" else vl.varList[name]['genbinning'])
    if ty == "folded" and vl.doxsec: var.Scale(1/vl.Lumi/1000)
    cov     = r.TH2F(var.GetName().replace("data_", "").replace(name+"_", ''), '', nbins, binning, nbins, binning)
    for x in range(nbins):
        for y in range(nbins):
            bin = cov.GetBin(x + 1, y + 1)
            cov.SetBinContent(bin, (nom.GetBinContent(x + 1) - var.GetBinContent(x + 1)) * (nom.GetBinContent(y + 1) - var.GetBinContent(y + 1)))

    return cov
