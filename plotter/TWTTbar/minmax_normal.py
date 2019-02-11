import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
import sys, os, copy, math
from ROOT import *
import copy
from multiprocessing import Pool
gROOT.SetBatch(1) #evita que salgan ventanas aparte
#===================================


def normal(varName, kind = 'folded'):
    #open the signal histogram in root of tw+ttbar powheg
    if kind=='folded':         
        tmptfile = r.TFile.Open('temp/{var}/ClosureTest_fold_DR_unif_{var}.root'.format(var = varName))        
        tmptfile1 = r.TFile.Open('temp/{var}/ClosureTest_fold_DS_unif_{var}.root'.format(var = varName))
    elif kind=='unfolded': 
        tmptfile = r.TFile.Open('temp/{var}/ClosureTest_DR_unif_{var}.root'.format(var = varName))       
        tmptfile1 = r.TFile.Open('temp/{var}/ClosureTest_DS_unif_{var}.root'.format(var = varName))
    else : 
        print 'The kind of histogram chosen is not valid'
        #continue
    true = copy.deepcopy(tmptfile.Get('tW'))
    true.SetLineWidth(3)
    true.SetLineColor(bp.colorMap[3]) 
    
    #normalisation
    MAX=true.GetMaximum()  
    T=true.Integral()
    #true.Scale(1/MAX)
    true.Scale(1/T)
     
    #Saving the plot
    #ttbar normal
    ttbar = copy.deepcopy(tmptfile.Get('t#bar{t}'))
    Tttbar=ttbar.Integral()
    ttbar.SetLineWidth(3)
    ttbar.SetLineColor(bp.colorMap[4])
    #ttbar.Scale(1/MAX)
    ttbar.Scale(1/Tttbar)
    #set legend for the histogram
    ttbar.GetXaxis().SetTitle('m_{b\\ell}^{minimax} (GeV)')
    ttbar.GetYaxis().SetTitle('u.a.')
    
    #tW normal
    tW = copy.deepcopy(tmptfile1.Get('tW'))
    tW.SetLineWidth(3)
    tW.SetLineColor(bp.colorMap[2])
    TtW=tW.Integral()
    #tW.Scale(1/MAX)
    tW.Scale(1/TtW)
    
    
    if kind=='folded' :     plot = bp.beautifulUnfoldingPlots('{var}_norm_fold_'.format(var = varName))
    elif kind=='unfolded':  plot = bp.beautifulUnfoldingPlots('{var}_norm_'.format(var = varName))
    
    
    plot.doFit          = False
    plot.plotspath      = "results/discriminante/"
    legloc = "TR"
    
    
    plot.addHisto(ttbar, 'L,same',     't#bar{t} Powheg',   'L', 'mc')
    plot.addHisto(true, 'L,same',     'tW DR',   'L', 'mc')
    plot.addHisto(tW, 'L,same',     'tW DS',   'L', 'mc')
    
    
    plot.saveCanvas(legloc)    
    tmptfile.Close()
        
variables = ['M_bl_minmax_opt']
    
for vr in variables: 
    varName = vr;
    normal(varName, kind= 'unfolded')


print "> Done!", "\n"


