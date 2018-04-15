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
