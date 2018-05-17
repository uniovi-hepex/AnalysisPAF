import ROOT as r
import math, copy
import varList

def quadSum( elements):
    return math.sqrt( sum( map( lambda x : x*x, elements)))


def GetMaxUnc(nominal, uncUp, uncDown):
    return max( map(lambda x : x*x, [nominal - uncUp, nominal - uncDown]))


def propagateQuantity(nom, varDict, case = 0):
    tot = 0.
    if case == 0:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key:
                print 'Key that raised the error:', key
                raise RuntimeError('Use case is not supported yet.')

            if key.replace('Up','Down') in varDict:
                tot = tot + GetMaxUnc(nom, varDict[key], varDict[key.replace('Up', 'Down')])
            else:
                tot = tot + max( map(lambda x : x*x, [nom - varDict[key]]))
    elif case > 0:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key and key not in varList.varList['Names']['colorSysts']:
                print 'Key that raised the error:', key
                raise RuntimeError('Use case is not supported yet.')

            if key.replace('Up','Down') in varDict:
                if (nom - varDict[key] < 0):
                    tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                else:
                    tot = tot + map(lambda x : x*x, [nom - varDict[key.replace('Up', 'Down')]])[0]
    else:
        for key in varDict: 
            if 'Down' in key: continue
            if not 'Up' in key and key not in varList.varList['Names']['colorSysts']:
                print 'Key that raised the error:', key
                raise RuntimeError('Use case is not supported yet.')

            if key.replace('Up','Down') in varDict:
                if (nom - varDict[key] > 0):
                    tot = tot + map(lambda x : x*x, [nom - varDict[key]])[0]
                else:
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
                if key in varList.varList['Names']['colorSysts']:
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


def propagateHistoAsym(nom, varDict, doEnv = False):
    outUp   = nom.Clone(nom.GetName() + '_uncUp')
    outDown = nom.Clone(nom.GetName() + '_uncDown')
    
    if doEnv:
        for bin in range(1, nom.GetNbinsX() + 1):
            err         = outUp.GetBinError(bin)    # <==  Fit unc. taken here
            cont        = outUp.GetBinContent(bin)
            tmpDict     = dict([(key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            tmpuncUp    = 0.
            tmpuncDown  = 0.
            tmpunc      = 0.
            for (key, histo) in varDict.iteritems():
                if key in varList.varList['Names']['colorSysts']:
                    tmpunc  = tmpDict[key] - cont
                    if tmpunc > tmpuncUp:
                        tmpuncUp    = tmpunc
                    elif tmpunc < tmpuncDown:
                        tmpuncDown  = tmpunc
                    del tmpDict[key]
            outUp.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, +1), err/2, tmpuncUp]))
            outDown.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, -1), err/2, abs(tmpuncDown)]))
    else:
        for bin in range(1, nom.GetNbinsX() + 1):
            err     = outUp.GetBinError(bin)    # <==  Fit unc. taken here
            cont    = outUp.GetBinContent(bin)
            tmpDict = dict([(key, histo.GetBinContent(bin)) for (key, histo) in varDict.iteritems()])
            outUp.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, +1), err/2]))
            outDown.SetBinError(bin, quadSum([propagateQuantity(cont, tmpDict, -1), err/2]))
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
        relErr.SetBinError(bin,0)
    return relErr


def maximumHisto(histo1, histo2):
    maxHist = histo1.Clone( histo1.GetName() + 'max' ) 
    for bin in range(1, maxHist.GetNbinsX()+1):
        maxHist.SetBinContent( bin, max(histo1.GetBinContent(bin), histo2.GetBinContent(bin)))
    return maxHist


def getUncList(nom, varDict, doEnv = False):
    medDict = [('Fit', nom)]
    outDict = []
    histUp  = [[] for i in range(nom.GetNbinsX())]
    histDown= [[] for i in range(nom.GetNbinsX())]
    for var in varDict:
        hist    = nom.Clone(nom.GetName() + var)
        variat  = 0.
        for bin in range(1, nom.GetNbinsX() + 1):
            variat = nom.GetBinContent(bin) - varDict[var].GetBinContent(bin)
            if doEnv and var in varList.varList['Names']['colorSysts']:
                if (variat >= 0): histUp[bin - 1].append(abs(variat))
                else: histDown[bin - 1].append(abs(variat))
            else:
                hist.SetBinError(bin, abs(variat))
        if not doEnv or var not in varList.varList['Names']['colorSysts']:
            medDict.append( (var,hist) )
    
    if doEnv:
        finalhistUp     = nom.Clone(nom.GetName() + 'ColorRUp')
        finalhistDown   = nom.Clone(nom.GetName() + 'ColorRDown')
        for bin in range(1, nom.GetNbinsX() + 1):
            finalhistUp.SetBinError(bin, max(histUp[bin - 1] + [0]))
            finalhistDown.SetBinError(bin, max(histDown[bin - 1] + [0]))
        medDict.append(('ColorRUp', finalhistUp))
        medDict.append(('ColorRDown', finalhistDown))
    
    #kk = lambda x : maxRelativeError(x[1])
    medDict.sort(key = lambda x : maxRelativeError(x[1]), reverse = True)
    #for key in medDict:
        #print key
        #if 'Down' in key[0]: continue
        #down = None
        #for key2 in medDict:
            #if key2[0] == key[0].replace('Up','Down'):
                #down = key2
        #if down:
            #hist = maximumHisto(key[1], key2[1])
            #outDict.append( (key[0].replace('Up',''), hist) )
        #else:
            #outDict.append( (key[0].replace('Up',''), key[1]) )
    for key in medDict:
        #print key
        done = False
        for i in range(len(outDict)):
            if key[0].replace('Up', '').replace('Down', '') == outDict[i][0]: done = True
        if done: continue
        if 'Down' in key[0]:
            for key2 in medDict:
                if key2[0] == key[0].replace('Down','Up'):
                    down = key2
            hist = maximumHisto(key[1], key2[1])
            outDict.append( (key[0].replace('Down',''), hist) )
        elif 'Up' in key[0]:
            for key2 in medDict:
                if key2[0] == key[0].replace('Up','Down'):
                    down = key2
            if key[0].replace('Up', 'Down') in medDict:
                hist = maximumHisto(key[1], key2[1])
                outDict.append( (key[0].replace('Up',''), hist) )
            else:
                outDict.append( (key[0].replace('Up',''), key[1]) )
        else: # We expect only the fit unc. to arrive here
            outDict.append( (key[0], key[1]) )
    
    return map( lambda x : (x[0], relativeErrorHist(x[1])), outDict)
