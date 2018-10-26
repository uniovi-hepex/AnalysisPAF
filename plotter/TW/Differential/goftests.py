import ROOT as r
import varList as vl
import beautifulUnfoldingPlots as bp
import sys, os, copy
r.gROOT.SetBatch(True)
vl.SetUpWarnings()

print "\nREMEMBER TO RUN THIS WITH A CMSSW INSTALLATION (or with ROOT, Numpy, and Scipy)\n"
import numpy as np
from scipy.stats import chi2

sysList = ['', 'tW'] + [syst for syst in vl.systMap]

if (len(sys.argv) > 1):
    varName     = sys.argv[1]
    if sys.argv[1] == 'All':
        print "> All the variables will be considered"
    else:
        print "> Chosen variable:", varName, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'

pathtothings = "temp/"
outputpath   = "results/comparison"

if not os.path.isdir(outputpath):
    os.system('mkdir -p ' + outputpath)


def GiveMeMyGOFTests(var):
    f1 = r.TFile.Open(pathtothings + "{vr}_/unfOutput_{vr}.root".format(vr = var), "read")
    f2 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_{vr}.root".format(vr = var), "read")
    f3 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_DS_{vr}.root".format(vr = var), "read")
    f4 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_aMCatNLO_{vr}.root".format(vr = var), "read")
    
    hData     = copy.deepcopy(f1.Get(var).Clone("hData"))
    hDataUp   = copy.deepcopy(f1.Get("nom0").Clone("hDataUp"))
    hDataDown = copy.deepcopy(f1.Get("nom1").Clone("hDataDown"))
    hDR       = copy.deepcopy(f2.Get("tW").Clone("hDR"))
    hDS       = copy.deepcopy(f3.Get("tW").Clone("hDS"))
    haMCatNLO = copy.deepcopy(f4.Get("tW").Clone("haMCatNLO"))
    
    f1.Close(); f2.Close(); f3.Close(); f4.Close()
    
    hData.SetFillColor(r.kWhite)
    hData.SetLineWidth(2)
    hData.SetLineColor(r.kBlack)
    hData.SetLineStyle(2)
    for bin in range(1, hData.GetNbinsX() + 1):
        hData.SetBinError(bin, max( [hDataUp.GetBinError(bin), hDataDown.GetBinError(bin)] ))
    
    hDR.SetFillColor(r.kWhite)
    hDR.SetLineWidth(2)
    hDR.SetLineColor(r.kRed)
    
    hDS.SetFillColor(r.kWhite)
    hDS.SetLineWidth(2)
    hDS.SetLineColor(r.kGreen)
    hDS.GetXaxis().SetTitle(vl.varList[var]['xaxis'])
    
    haMCatNLO.SetFillColor(r.kWhite)
    haMCatNLO.SetLineWidth(2)
    haMCatNLO.SetLineColor(r.kBlue)
    
    
    plot           = bp.beautifulUnfoldingPlots("GOF_{vr}".format(vr = var))
    plot.doRatio   = False
    plot.doFit     = False
    plot.plotspath = outputpath + "/"
    
    plot.addHisto(hDS,       'hist',      'tW DS',      'L', 'mc')
    plot.addHisto(hDR,       'hist,same', 'tW Powheg',  'L', 'mc')
    plot.addHisto(haMCatNLO, 'hist,same', 'tW aMC@NLO', 'L', 'mc')
    plot.addHisto(hData,     'hist,same', 'Data',       'L', 'mc')
    
    l1 = r.TLatex(0.65, 0.700, '#scale[0.4]{DR - KS: %4.7f}'%hData.KolmogorovTest(hDR))
    l2 = r.TLatex(0.65, 0.675, '#scale[0.4]{DR - KS (toys): %4.7f}'%hData.KolmogorovTest(hDR,'X'))
    l3 = r.TLatex(0.65, 0.650, '#scale[0.4]{DR - #chi^{2}: %4.7f}'%hData.Chi2Test(hDR,'WW'))
    l4 = r.TLatex(0.65, 0.625, '#scale[0.4]{DS - KS: %4.7f}'%hData.KolmogorovTest(hDS))
    l5 = r.TLatex(0.65, 0.600, '#scale[0.4]{DS - KS (toys): %4.7f}'%hData.KolmogorovTest(hDS,'X'))
    l6 = r.TLatex(0.65, 0.575, '#scale[0.4]{DS - #chi^{2}: %4.7f}'%hData.Chi2Test(hDS,'WW'))
    l7 = r.TLatex(0.65, 0.550, '#scale[0.4]{aMC@NLO - KS: %4.7f}'%hData.KolmogorovTest(haMCatNLO))
    l8 = r.TLatex(0.65, 0.525, '#scale[0.4]{aMC@NLO - KS (toys): %4.7f}'%hData.KolmogorovTest(haMCatNLO,'X'))
    l9 = r.TLatex(0.65, 0.500, '#scale[0.4]{aMC@NLO - #chi^{2}: %4.7f}'%hData.Chi2Test(haMCatNLO,'WW'))
    
    l1.SetNDC(True); l2.SetNDC(True); l3.SetNDC(True); l4.SetNDC(True); l5.SetNDC(True); l6.SetNDC(True)
    l7.SetNDC(True); l8.SetNDC(True); l9.SetNDC(True);

    l1.Draw('same'); l2.Draw('same'); l3.Draw('same'); l4.Draw("same"); l5.Draw("same"); l6.Draw("same")
    l7.Draw("same"); l8.Draw("same"); l9.Draw("same")
    
    plot.saveCanvas("TR")
    del plot
    
    print 'DR - KS:', hData.KolmogorovTest(hDR)
    print 'DR - KS (toys):', hData.KolmogorovTest(hDR,'X')
    print 'DR - chi2:', hData.Chi2Test(hDR,'WW')
    print 'DS - KS:', hData.KolmogorovTest(hDS)
    print 'DS - KS (toys):', hData.KolmogorovTest(hDS,'X')
    print 'DS - chi2:', hData.Chi2Test(hDS,'WW')
    print 'aMC@NLO - KS:', hData.KolmogorovTest(haMCatNLO)
    print 'aMC@NLO - KS (toys):', hData.KolmogorovTest(haMCatNLO,'X')
    print 'aMC@NLO - chi2:', hData.Chi2Test(haMCatNLO,'WW')
    return


def Chi2TestForMultivarNormal(w1, V1, w2, V2):
    if len(w1) != len(w2): raise RuntimeError("The length of both arrays is not the same.")
    if V1.shape != V2.shape: raise RuntimeError("The shape of the covariance matrices is not the same.")
    if V1.shape[0] != V1.shape[0] or V2.shape[1] != V2.shape[1]: raise RuntimeError("One (or both) covariance matrix is (are) not squared.")
    if len(w1) != V1.shape[0] or len(w2) != V2.shape[0]: raise RuntimeError("The length of one (or both) array is not the same as the number of lines or columns of its respective covariance matrix.")
    
    dof = len(w1) - 1
    invV1 = np.linalg.inv(V1); invV2 = np.linalg.inv(V2)
    W1 = sum(w1); W2 = sum(w2)
    
    # 1) Calculation of MLE of the vector of probabilities of each bin
    p    = np.linalg.inv(W1**2 * invV1 + W2**2 * invV2).dot( W1 * invV1.dot(w1) + W2 * invV2.dot(w2) )
    
    # 2) Calculation of the Pearson's chi-squared test statistic
    est  = ((w1 - W1 * p).transpose()).dot(invV1).dot(w1 - W1 * p) + ((w2 - W2 * p).transpose()).dot(invV2).dot(w2 - W2 * p)
    
    # 3) Calculation of the corresponding p-value
    pval = 1 - chi2.cdf(est, dof)
    return pval, est


def GiveMeMyGoodGOFTests(var):
    f1 = r.TFile.Open(pathtothings + "{vr}_/unfOutput_{vr}.root".format(vr = var), "read")
    f2 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_{vr}.root".format(vr = var), "read")
    f3 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_DS_{vr}.root".format(vr = var), "read")
    f4 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_aMCatNLO_{vr}.root".format(vr = var), "read")
    f5 = r.TFile.Open(pathtothings + "{vr}_/CovMat_{vr}.root".format(vr = var), "read")
    
    hData     = copy.deepcopy(f1.Get(var).Clone("hData"))
    hDataUp   = copy.deepcopy(f1.Get("nom0").Clone("hDataUp"))
    hDataDown = copy.deepcopy(f1.Get("nom1").Clone("hDataDown"))
    hDataCov  = copy.deepcopy(f5.Get("finalmat").Clone("hDataCov"))
    hDR       = copy.deepcopy(f2.Get("tW").Clone("hDR"))
    hDS       = copy.deepcopy(f3.Get("tW").Clone("hDS"))
    haMCatNLO = copy.deepcopy(f4.Get("tW").Clone("haMCatNLO"))
    
    f1.Close(); f2.Close(); f3.Close(); f4.Close(); f5.Close()
    
    hData.SetFillColor(r.kWhite)
    hData.SetLineWidth(2)
    hData.SetLineColor(r.kBlack)
    hData.SetLineStyle(2)
    for bin in range(1, hData.GetNbinsX() + 1):
        hData.SetBinError(bin, max( [hDataUp.GetBinError(bin), hDataDown.GetBinError(bin)] ))
    
    hDR.SetFillColor(r.kWhite)
    hDR.SetLineWidth(2)
    hDR.SetLineColor(r.kRed)
    
    hDS.SetFillColor(r.kWhite)
    hDS.SetLineWidth(2)
    hDS.SetLineColor(r.kGreen)
    hDS.GetXaxis().SetTitle(vl.varList[var]['xaxis'])
    
    haMCatNLO.SetFillColor(r.kWhite)
    haMCatNLO.SetLineWidth(2)
    haMCatNLO.SetLineColor(r.kBlue)
    
    covmat = np.zeros( (hData.GetNbinsX(), hData.GetNbinsX()) )
    
    for i in range(1, hData.GetNbinsX() + 1):
        for j in range(1, hData.GetNbinsX() + 1):
            covmat[i-1, j-1] = hDataCov.GetBinContent(i, j)
    
    DRcovmat       = np.diag([hDR.GetBinError(bin)**2       for bin in range(1, hDR.GetNbinsX() + 1)])
    DScovmat       = np.diag([hDS.GetBinError(bin)**2       for bin in range(1, hDS.GetNbinsX() + 1)])
    aMCatNLOcovmat = np.diag([haMCatNLO.GetBinError(bin)**2 for bin in range(1, haMCatNLO.GetNbinsX() + 1)])
    
    coses = {}
    coses["DR"] = {
        "hist"   : copy.deepcopy(hDR),
        "covmat" : copy.deepcopy(DRcovmat)
    }
    coses["DS"] = {
        "hist"   : copy.deepcopy(hDS),
        "covmat" : copy.deepcopy(DScovmat)
    }
    coses["aMCatNLO"] = {
        "hist"   : copy.deepcopy(haMCatNLO),
        "covmat" : copy.deepcopy(aMCatNLOcovmat)
    }
    
    vdata = [hData.GetBinContent(bin) for bin in range(1, hData.GetNbinsX() + 1)]
    
    for key in coses:
        vtemp = [coses[key]["hist"].GetBinContent(bin) for bin in range(1, coses[key]["hist"].GetNbinsX() + 1)]
        pv, es = Chi2TestForMultivarNormal(vdata, covmat, vtemp, coses[key]["covmat"])
        coses[key]["p-value"]   = pv
        coses[key]["statistic"] = es
    
    
    plot           = bp.beautifulUnfoldingPlots("GOF_{vr}".format(vr = var))
    plot.doRatio   = False
    plot.doFit     = False
    plot.plotspath = outputpath + "/"
    
    plot.addHisto(hDS,       'hist',      'tW DS',      'L', 'mc')
    plot.addHisto(hDR,       'hist,same', 'tW Powheg',  'L', 'mc')
    plot.addHisto(haMCatNLO, 'hist,same', 'tW aMC@NLO', 'L', 'mc')
    plot.addHisto(hData,     'hist,same', 'Data',       'L', 'mc')
    
    l1 = r.TLatex(0.65, 0.650, '#scale[0.4]{DR - p-val.: %4.7f}'%coses["DR"]["p-value"])
    l2 = r.TLatex(0.65, 0.625, '#scale[0.4]{DR - stat.: %4.7f}'%coses["DR"]["statistic"])
    l3 = r.TLatex(0.65, 0.600, '#scale[0.4]{DS - p-val.: %4.7f}'%coses["DS"]["p-value"])
    l4 = r.TLatex(0.65, 0.575, '#scale[0.4]{DS - stat.: %4.7f}'%coses["DS"]["statistic"])
    l5 = r.TLatex(0.65, 0.550, '#scale[0.4]{aMC@NLO - p-val.: %4.7f}'%coses["aMCatNLO"]["p-value"])
    l6 = r.TLatex(0.65, 0.525, '#scale[0.4]{aMC@NLO - stat.: %4.7f}'%coses["aMCatNLO"]["statistic"])
    
    l1.SetNDC(True); l2.SetNDC(True); l3.SetNDC(True); l4.SetNDC(True); l5.SetNDC(True); l6.SetNDC(True)

    l1.Draw('same'); l2.Draw('same'); l3.Draw('same'); l4.Draw('same'); l5.Draw('same'); l6.Draw('same')
    
    plot.saveCanvas("TR")
    del plot
    
    print 'DR - p-val.:',      coses["DR"]["p-value"]
    print 'DR - stat.:',       coses["DR"]["statistic"]
    print 'DS - p-val.:',      coses["DS"]["p-value"]
    print 'DS - stat.:',       coses["DS"]["statistic"]
    print 'aMC@NLO - p-val.:', coses["aMCatNLO"]["p-value"]
    print 'aMC@NLO - stat.:',  coses["aMCatNLO"]["statistic"]
    return

tasks   = []

if varName == 'All':
    for var in vl.varList['Names']['Variables']:
        tasks.append( var )
else:
    tasks = [varName]

for el in tasks:
    GiveMeMyGoodGOFTests(el)

