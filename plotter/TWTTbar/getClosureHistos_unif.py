import ROOT as r
import varList as vl
import sys, os
from array import array
print "===== Plots for closure test comparisons procedure\n"
vl.SetUpWarnings()

pathToTree  = ""
recocut     = "Tpassreco == 1"
gencut      = '1/TWeight*TWeight_normal*(Tpassgen == 1)'


pathToTree  = "../../TWTTbar_temp/";

NameOfTree = "Mini1j1t";

r.gROOT.SetBatch(True)
r.gROOT.LoadMacro('../Histo.C+')
r.gROOT.LoadMacro('../Looper.C+')
r.gROOT.LoadMacro('../Plot.C+')
r.gROOT.LoadMacro('../PlotToPyC.C+')



def closure(var, kind = 'folded', interf = 'DR'):
    
    
    if vl.doxsec: opts = 'noScaleLumi'
    else:         opts = ''

    if not os.path.isdir('temp/{var}'.format(var = varName)):
        os.system('mkdir -p temp/{var}'.format(var = varName))
    print "> Beginning to produce the histograms", "\n"
    
    bins = [0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 220., 240., 260., 280., 300., 320., 340., 360., 380., 400., 420.]
    binning = array('f', bins)
    
    if kind=='unfolded': 
        p = r.PlotToPyC(r.TString(vl.varList[varName]['var_gen']), r.TString(gencut), r.TString('All'), len(binning)-1, binning, r.TString('TGen' + varName), r.TString(''))
    elif kind=='folded': 
        p = r.PlotToPyC(r.TString(vl.varList[varName]['var']), r.TString(gencut), r.TString('All'), int(20), binning, r.TString('T' + varName), r.TString(''))   
        
    p.SetPath(pathToTree); 
    p.SetTreeName(NameOfTree);
    p.SetLimitFolder('temp/{var}/'.format(var = varName));
    p.SetPathSignal(pathToTree);
    p.verbose = True
    p.SetLumi(vl.Lumi)

    if kind=='unfolded' and interf=='DR': 
        #p.AddSample("UNF_TW",    "tW", r.itBkg, 2, '', opts)
        #p.AddSample("UNF_TbarW", "tW", r.itBkg, 2, '', opts)

        specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('UNF_TW',                     'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('UNF_TW_noFullyHadr',         'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('UNF_TbarW',                  'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('UNF_TbarW_noFullyHadr',      'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        p.SetWeight('TWeight')

        p.AddSample('UNF_TTbar2L_powheg_0' ,   't#bar{t}',      r.itBkg, 2, '', opts)
                
    elif kind=='unfolded' and interf=='DS':    
        p.AddSample("UNF_TW_noFullyHadr_DS",    "tW", r.itBkg, 2, '', opts)
        p.AddSample("UNF_TbarW_noFullyHadr_DS", "tW", r.itBkg, 2, '', opts)

        p.AddSample('UNF_TTbar2L_powheg_0' ,   't#bar{t}',      r.itBkg, 2, '', opts)
        
        #p.AddSample("TW",    "tW", r.itBkg, 2, '', opts)
        #p.AddSample("TbarW", "tW", r.itBkg, 2, '', opts)
    
    elif kind=='folded' and interf=='DR':  
        specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TW',                     'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TW_noFullyHadr',         'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TbarW',                  'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TbarW_noFullyHadr',      'tW',      r.itBkg, r.TColor.GetColor("#ffcc33"), '', opts)
        p.SetWeight('TWeight')

        #p.AddSample("TTbar_Powheg",       "t#bar{t}", r.itBkg, 2, '', opts)
        p.AddSample("TTbar2L_powheg",    "t#bar{t}", r.itBkg, 2, '', opts)

        #specialweight = vl.n_ttbar/vl.sigma_ttbar/(vl.n_ttbar/vl.sigma_ttbar + vl.n_dilep/vl.sigma_dilep)
        #p.SetWeight('TWeight*' + str(specialweight))
        #p.AddSample('TTbar_Powheg',          "t#bar{t}",    r.itBkg,                       633, '', opts)
        #specialweight = vl.n_dilep/vl.sigma_dilep/(vl.n_ttbar/vl.sigma_ttbar + vl.n_dilep/vl.sigma_dilep)
        #p.SetWeight('TWeight*' + str(specialweight))
        #p.AddSample('TTbar2L_powheg',        "t#bar{t}",    r.itBkg,                       633, '', opts)
        #p.SetWeight('TWeight')
    
    elif kind=='folded' and interf=='DS':  
        p.AddSample("TW_noFullyHadr_DS",    "tW", r.itBkg, 2, '', opts)
        p.AddSample("TbarW_noFullyHadr_DS", "tW", r.itBkg, 2, '', opts)

        p.AddSample("TTbar2L_powheg",    "t#bar{t}", r.itBkg, 2, '', opts)
         
    p.NoShowVarName = True;
    
    if kind=='unfolded' and interf=='DR':p.SetOutputName("ClosureTest_DR_unif_" + varName);
    elif kind=='unfolded' and interf=='DS':p.SetOutputName("ClosureTest_DS_unif_" + varName);
    elif kind=='folded' and interf=='DR':  p.SetOutputName("ClosureTest_fold_DR_unif_" + varName);
    elif kind=='folded' and interf=='DS':  p.SetOutputName("ClosureTest_fold_DS_unif_" + varName);
    p.SaveHistograms();
    del p
    
variables = ['M_bl_minmax_ATLAS', 'M_bl_minmax_opt']
    
for vr in variables: 
    varName = vr;
    closure(varName, kind= 'folded', interf = 'DR')
    closure(varName, kind= 'unfolded', interf = 'DR')
    closure(varName, kind= 'folded', interf = 'DS')
    closure(varName, kind= 'unfolded', interf = 'DS')

print "> Done!", "\n"
