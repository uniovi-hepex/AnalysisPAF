import ROOT as r
import math
import copy

def quadSum( elements):
    return math.sqrt( sum( map( lambda x : x*x, elements)))

def propagateQuantity( nom, varDict ):
    tot = 0 
    for key in varDict: 
        if 'Down' in key: continue
        if not 'Up' in key: 
            raise RuntimeError('Use case is not supported yet (compatibility mode with JER')

        if key.replace('Up','Down') in varDict:
            tot = tot + max( map(lambda x : x*x, [nom - varDict[key], nom - varDict[key.replace('Up','Down')]]))
        else:
            tot = tot + max( map(lambda x : x*x, [nom - varDict[key]]))

    return math.sqrt(tot)


def propagateHisto( nom, varDict ):
    out = nom.Clone(nom.GetName() + '_unc')

    for bin in range(1, out.GetNbinsX()+1):
        err     = out.GetBinError(bin)
        cont    = out.GetBinContent(bin)
        tmpDict = dict([ (key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
        out.SetBinError( bin, quadSum([propagateQuantity(cont,tmpDict),err]))
    return out



def maxRelativeError(histo):
    return max( [histo.GetBinError(bin) / histo.GetBinContent(bin) for bin in range(1, histo.GetNbinsX()+1)])

def relativeErrorHist(histo):
    relErr = histo.Clone( histo.GetName() + 'relErr')
    for bin in range(1, histo.GetNbinsX()+1):
        relErr.SetBinContent(bin, histo.GetBinError(bin) / histo.GetBinContent(bin) )
        relErr.SetBinError(bin,0)
    return relErr

def maximumHisto(histo1, histo2):
    maxHist = histo1.Clone( histo1.GetName() + 'max' ) 
    for bin in range(1, maxHist.GetNbinsX()+1):
        maxHist.SetBinContent( bin, max(histo1.GetBinContent(bin), histo2.GetBinContent(bin)))
    return maxHist

def getUncList(nom, varDict):
    medDict = [('Fit',nom)]
    outDict = []
    for var in varDict:
        hist = nom.Clone( nom.GetName() + var)
        for bin in range(1, nom.GetNbinsX()+1):
            hist.SetBinError( bin, abs(nom.GetBinContent(bin) - varDict[var].GetBinContent(bin)) )
        medDict.append( (var,hist) )
    
    kk = lambda x : maxRelativeError(x[1])
    medDict.sort(key = lambda x : maxRelativeError(x[1]), reverse=True )
    for key in medDict:
        if 'Down' in key[0]: continue
        down = None
        for key2 in medDict:
            if key2[0] == key[0].replace('Up','Down'):
                down = key2
        if down:
            hist = maximumHisto(key[1], key2[1])
            outDict.append( (key[0].replace('Up',''), hist) )
        else:
            outDict.append( (key[0].replace('Up',''), key[1]) )
        
        

    return map( lambda x : (x[0], relativeErrorHist(x[1])), outDict)
