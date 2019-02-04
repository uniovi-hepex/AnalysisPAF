import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
import sys, os, copy, math
#===================================

def Coherencia(varName):

    print 'Variable seleccionada'
    print varName
    tmptfile = r.TFile.Open('temp/{var}/forCards_{var}.root'.format(var = varName))
    tru = copy.deepcopy(tmptfile.Get('tW'))
    tru.Add(copy.deepcopy(tmptfile.Get('t#bar{t}')))
    tmptfile.Close()
    
    out = r.TFile.Open('temp/{var}/cutOutput_{var}.root'.format(var = varName))
    pseudo = copy.deepcopy(out.Get('data_asimov'))
    data = copy.deepcopy(out.Get('data_'))
    out.Close()
    
    hist   = []
    asimov = []
    datos  = []
    rel    = []
    rel1   = []
    
    for i in range(1, tru.GetNbinsX() + 1):
        hist.append(tru.GetBinContent(i))
        asimov.append(pseudo.GetBinContent(i))
        #datos.append(data.GetBinContent(i))
        rel.append(hist[i - 1]/asimov[i - 1])
        #rel1.append(hist[i]/datos[i])
    
    print 'histograma'
    print hist
    print 'pseudodatos'
    print asimov
    print 'relacion'
    print rel
    return hist,asimov,rel
    
varlist=vl.varList['Names']['Variables']  
for var in varlist:
    Coherencia(var)  
    
#Coherencia('LeadingLepPt')

