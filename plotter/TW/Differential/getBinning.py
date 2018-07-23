import ROOT as r
import varList as vl
import sys, os
from array import array

r.gROOT.SetBatch(True)
vl.SetUpWarnings()
print "===== Setting binning of the BDT procedure\n"
print "> Preliminaries..."
nq          = vl.nBinsInBDT  # Number of bins in which to divide the BDT distribution.
xq          = array('d', [0]*nq)
yq          = array('d', [0]*nq)
storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
minipath    = ""
printthings = 1

print '> Number of bins of the BDT distribution:', nq

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    if (len(sys.argv) > 2):
        printthings = sys.argv[2]
        if (len(sys.argv) > 3):
            if sys.argv[3] == 'last':
                minipath    = vl.GetLastFolder(storagepath)
            else:
                minipath    = storagepath + sys.argv[3] + "/"
        else:
            minipath    = "../../../TW_temp/"
    else:
        minipath    = "../../../TW_temp/"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'
    minipath    = "../../../TW_temp/"

print '> Variable chosen:', varName, '\n'
print '> Minitrees path:', minipath, '\n'

if vl.unifttbar:
    print '> Uniform ttbar distribution of the BDT discriminant\n'
    tf    = r.TFile(minipath + 'Tree_TTbar_Powheg.root')
else:
    print '> Uniform tW distribution of the BDT discriminant\n'
    tf    = r.TFile(minipath + 'Tree_TW.root')


tree  = tf.Mini1j1t

Base  ='''
#ifndef ADDSTUFF_H
#define ADDSTUFF_H
#include<iostream>
using namespace std;
'''

for i in range(0,nq):
    xq[i] = float(i+1)/nq

var   = vl.varList[varName]['var']
bins  = vl.varList[varName]['recobinning']

count = 0
print "\n> Constructing C++ file of the binning"
for binDn,binUp in zip(bins, bins[1:]):
    c       = r.TCanvas()
    count   = count + 1
    varBin  = "{var} >= {binDn} && {var} < {binUp}".format(var=var, binUp=binUp, binDn=binDn)
    tree.Draw("TBDT","TWeight * (Tpassreco == 1 && %s)"%varBin)
    c.Update()
    if (printthings == 1):
      print "TBDT","(Tpassreco == 1 && %s)"%varBin
      raw_input('Copy this string into your .C DrawPlots-like file and press enter.')
    tree.GetHistogram().GetQuantiles(nq, yq, xq)
    del c
    Base = Base + '''\n /////////////////////// \n 
Float_t theBDt_bin%d(Double_t BDT){
  if      (BDT < %f   ) return 1;
'''%(count, yq[0])
    
    subBin = 2
    for i in range(1, nq-1): 
        Base = Base + '  else if (BDT < %f) return %d;\n'%(yq[i],subBin)
        subBin = subBin + 1
    
    Base = Base + '  else                       return %d;\n} \n'%subBin


Base = Base + '#endif'
print "> Saving file"
if not os.path.isdir('temp/{var}_'.format(var = varName)):
    os.system('mkdir -p temp/{var}_'.format(var = varName))
output = open('./temp/{var}_/'.format(var = varName) + varName + '.C', 'w')
output.write(Base)
output.close()

print "> Doing sanity checks..."
r.gROOT.LoadMacro('./temp/{var}_/'.format(var = varName) + varName + '.C+')

for k in range(1, len(bins)):
    if not hasattr(r, 'theBDt_bin%d'%k): print 'Something went wrong'
    else: print '    - Bin', k, 'OK'


print "> Sanity check correct\n"
print "> BDT binning done!"
