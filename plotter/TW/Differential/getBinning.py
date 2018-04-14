import ROOT
import numpy as np
import varList

nq = 10
xq=np.zeros(nq)
yq=np.zeros(nq)
tf = ROOT.TFile('/nfs/fanae/user/sscruz/TW_jun4/AnalysisPAF/TW_temp/Tree_TW.root') # cambiar por ttbar dileptonico
tree = tf.Mini1j1t

Base='''
#ifndef ADDSTUFF_H
#define ADDSTUFF_H
#include<iostream>
using namespace std;
'''


for i in range(0,nq): 
    xq[i] = float(i+1)/nq

varName = 'LeadingJetPt'
var     = varList.varList[varName]['var']
bins    = varList.varList[varName]['binning']

count = 0
c = ROOT.TCanvas()

for binDn,binUp in zip(bins,bins[1:]):
    count = count + 1
    varBin = "{var} > {binDn} && {var} < {binUp}".format(var=var, binUp=binUp, binDn=binDn)
    tree.Draw("TBDT","TWeight*(TNJets == 1 && TNBtags == 1 && TChannel == 1 && %s)"%varBin)
    c.Update()
    print "TBDT","TWeight*(TNJets == 1 && TNBtags == 1 && TChannel == 1 && %s)"%varBin
    raw_input('wait')
    tree.GetHistogram().GetQuantiles(nq,yq,xq)

    Base = Base + '''\n /////////////////////// \n 
Float_t theBDt_bin%d(Double_t BDT){
  if      (BDT < %f   ) return 1;
'''%(count, yq[0])

    subBin = 2

    for i in range(1,nq-1): 
        Base = Base + '  else if (BDT < %f) return %d;\n'%(yq[i],subBin)
        subBin = subBin + 1

    Base = Base + '  else                       return %d;\n} \n'%subBin

Base = Base + '#endif'

output = open(varName+'.C','w')
output.write(Base)
output.close()

ROOT.gROOT.LoadMacro(varName+'.C+')

for k in range(1, len(bins)):
    if not hasattr(ROOT, 'theBDt_bin%d'%k): print 'Something went wrong'
    else: print 'Bin', k, 'ok'

