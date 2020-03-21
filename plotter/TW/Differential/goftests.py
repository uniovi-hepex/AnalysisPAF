import ROOT as r
import varList as vl
import beautifulUnfoldingPlots as bp
import sys, os, copy
r.gROOT.SetBatch(True)
vl.SetUpWarnings()

print "\nREMEMBER TO RUN THIS WITH A CMSSW INSTALLATION (or with ROOT, Numpy, and Scipy)\n"
import numpy as np
from scipy.stats import chisquare, chi2

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


def GiveMeMyGOFTests(var): # Old code!
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
    
    l1 = r.TLatex(0.65, 0.700, '#scale[0.4]{DR - KS: %4.10f}'%hData.KolmogorovTest(hDR))
    l2 = r.TLatex(0.65, 0.675, '#scale[0.4]{DR - KS (toys): %4.10f}'%hData.KolmogorovTest(hDR,'X'))
    l3 = r.TLatex(0.65, 0.650, '#scale[0.4]{DR - #chi^{2}: %4.10f}'%hData.Chi2Test(hDR,'WW'))
    l4 = r.TLatex(0.65, 0.625, '#scale[0.4]{DS - KS: %4.10f}'%hData.KolmogorovTest(hDS))
    l5 = r.TLatex(0.65, 0.600, '#scale[0.4]{DS - KS (toys): %4.10f}'%hData.KolmogorovTest(hDS,'X'))
    l6 = r.TLatex(0.65, 0.575, '#scale[0.4]{DS - #chi^{2}: %4.10f}'%hData.Chi2Test(hDS,'WW'))
    l7 = r.TLatex(0.65, 0.550, '#scale[0.4]{aMC@NLO - KS: %4.10f}'%hData.KolmogorovTest(haMCatNLO))
    l8 = r.TLatex(0.65, 0.525, '#scale[0.4]{aMC@NLO - KS (toys): %4.10f}'%hData.KolmogorovTest(haMCatNLO,'X'))
    l9 = r.TLatex(0.65, 0.500, '#scale[0.4]{aMC@NLO - #chi^{2}: %4.10f}'%hData.Chi2Test(haMCatNLO,'WW'))
    
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
    #print "VAINAS RARAS"
    #escalaobtusa = 10000
    #escalaobtusa = 1
    #w1 = ww1*escalaobtusa ; w2 = ww2*escalaobtusa ; V1 = VV1*escalaobtusa**2; V2 = VV2*escalaobtusa **2
    if len(w1) != len(w2): raise RuntimeError("The length of both arrays is not the same.")
    if V1.shape != V2.shape: raise RuntimeError("The shape of the covariance matrices is not the same.")
    if V1.shape[0] != V1.shape[0] or V2.shape[1] != V2.shape[1]: raise RuntimeError("One (or both) covariance matrix is (are) not squared.")
    if len(w1) != V1.shape[0] or len(w2) != V2.shape[0]: raise RuntimeError("The length of one (or both) array is not the same as the number of lines or columns of its respective covariance matrix.")
    
    dof = len(w1) - 1
    #V1 = np.diag([ V1[el, el] for el in range(len(w1)) ]) ### FORCED DIAGONALISATION
    invV1 = np.linalg.inv(V1); invV2 = np.linalg.inv(V2)
    W1 = sum(w1); W2 = sum(w2)
    print "W1:",  W1
    print "W2:",  W2
    print "dof:", dof
    print "pruebina:", invV1.dot(V1)[2, 2]
    # 1) Calculation of MLE of the vector of probabilities of each bin
    p    = np.linalg.inv(W1**2 * invV1 + W2**2 * invV2).dot( W1 * invV1.dot(w1) + W2 * invV2.dot(w2) )
    print "el p esti", p
    # 2) Calculation of the Pearson's chi-squared test statistic
    est  = ((w1 - W1 * p).dot(invV1)).dot((w1 - W1 * p).transpose()) + ((w2 - W2 * p).dot(invV2)).dot((w2 - W2 * p).transpose())
    
    # 3) Calculation of the corresponding p-value
    pval = 1 - chi2.cdf(est, dof)
    return pval, est


def GiveMeMyGoodGOFTests(tsk):
    var, ty = tsk
    print "\n====== Performing tests for variable", var, "and with type", ty, "\n"
    if ty != "fidunorm":
        f1 = r.TFile.Open(pathtothings + "{vr}_/unfOutput_{vr}.root".format(vr = var),            "read")
        f2 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_{vr}.root".format(vr = var),          "read")
        f3 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_DS_{vr}.root".format(vr = var),       "read")
        f4 = r.TFile.Open(pathtothings + "{vr}_/ClosureTest_aMCatNLO_{vr}.root".format(vr = var), "read")
        f5 = r.TFile.Open(pathtothings + "{vr}_/CovMat_{vr}_unfolded.root".format(vr = var),      "read")
    else:
        f1 = r.TFile.Open(pathtothings + "{vr}_/fidOutput_{vr}norm.root".format(vr = var),                "read")
        f5 = r.TFile.Open(pathtothings + "{vr}_/CovMat_{vr}_unfolded_fiducialnorm.root".format(vr = var), "read")
    
    hData     = copy.deepcopy(f1.Get(var).Clone("hData"))
    hDataCov  = copy.deepcopy(f5.Get("finalmat").Clone("hDataCov"))
    if ty != "fidunorm":
        hDR       = copy.deepcopy(f2.Get("tW").Clone("hDR"))
        hDS       = copy.deepcopy(f3.Get("tW").Clone("hDS"))
        haMCatNLO = copy.deepcopy(f4.Get("tW").Clone("haMCatNLO"))
        f1.Close(); f2.Close(); f3.Close(); f4.Close(); f5.Close()
    else:
        hDR       = copy.deepcopy(f1.Get("tru").Clone("hDR"))
        hDS       = copy.deepcopy(f1.Get("hDS").Clone("hDS"))
        haMCatNLO = copy.deepcopy(f1.Get("aMCatNLO").Clone("haMCatNLO"))
        f1.Close(); f5.Close()
    
    hData.SetFillColor(r.kWhite)
    hData.SetLineWidth(2)
    hData.SetLineColor(r.kBlack)
    hData.SetLineStyle(2)
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
    
    covmat = np.zeros( (hData.GetNbinsX(), hData.GetNbinsX()), dtype = np.double )
    
    for i in range(1, hData.GetNbinsX() + 1):
        for j in range(1, hData.GetNbinsX() + 1):
            #if i != j: continue
            covmat[i-1, j-1] = hDataCov.GetBinContent(i, j)
    
    DRcovmat       = np.diag(np.array([hDR.GetBinError(bin)**2       for bin in range(1, hDR.GetNbinsX() + 1)], dtype = np.double))
    DScovmat       = np.diag(np.array([hDS.GetBinError(bin)**2       for bin in range(1, hDS.GetNbinsX() + 1)], dtype = np.double))
    aMCatNLOcovmat = np.diag(np.array([haMCatNLO.GetBinError(bin)**2 for bin in range(1, haMCatNLO.GetNbinsX() + 1)], dtype = np.double))
    
    #covmat         *= 10000**2
    #DRcovmat       *= 10000**2
    #DScovmat       *= 10000**2
    #aMCatNLOcovmat *= 10000**2
    #hDR.Scale(10000)
    #hDS.Scale(10000)
    #haMCatNLO.Scale(10000)
    #hData.Scale(10000)
    #print "\ncovmat de DR\n", DRcovmat
    
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
    
    #for i in range(1, coses["DR"]["hist"].GetNbinsX() + 1):
        #print coses["DR"]["hist"].GetBinContent(
    
    vdata = np.array([hData.GetBinContent(bin) for bin in range(1, hData.GetNbinsX() + 1)], dtype = np.double)
    
    #print "\nWOLOLO\n", coses["aMCatNLO"]["covmat"]
    
    #covmatdiagonal = np.diag(np.array(np.linalg.eig(covmat)[0], dtype = np.double))
    #matriztransf   = np.linalg.eig(covmat)[1]
    #res2           = matriztransf.dot(vdata)
    #res2DR         = matriztransf.dot(np.array([coses["DR"]["hist"].GetBinContent(bin) for bin in range(1, coses["DR"]["hist"].GetNbinsX() + 1)], dtype = np.double))
    #covmatres2DR   = (np.linalg.inv(matriztransf).dot(coses["DR"]["covmat"])).dot(matriztransf)
    #res2DS         = matriztransf.dot(np.array([coses["DS"]["hist"].GetBinContent(bin) for bin in range(1, coses["DS"]["hist"].GetNbinsX() + 1)], dtype = np.double))
    #covmatres2DS   = (np.linalg.inv(matriztransf).dot(coses["DS"]["covmat"])).dot(matriztransf)
    #res2aMC        = matriztransf.dot(np.array([coses["aMCatNLO"]["hist"].GetBinContent(bin) for bin in range(1, coses["aMCatNLO"]["hist"].GetNbinsX() + 1)], dtype = np.double))
    #covmatres2aMC  = (np.linalg.inv(matriztransf).dot(coses["aMCatNLO"]["covmat"])).dot(matriztransf)
    
    #hData2 = copy.deepcopy(hData.Clone("hData2"))
    #hDR2   = copy.deepcopy(coses["DR"]["hist"].Clone("hDR2"))
    #hDS2   = copy.deepcopy(coses["DS"]["hist"].Clone("hDS2"))
    #haMC2  = copy.deepcopy(coses["aMCatNLO"]["hist"].Clone("haMC2"))
    
    #print res2
    #print res2DR
    #print res2DS
    #print "WOLOLO", covmatdiagonal
    #print covmatres2DR
    #print covmatres2DS
    #print covmatres2aMC
    
    #for bin in range(1, hData2.GetNbinsX() + 1):
        #hData2.SetBinContent(bin, res2[bin - 1])
        #hData2.SetBinError(bin, r.TMath.Sqrt(covmatdiagonal[bin-1, bin-1]))
        #hDR2.SetBinContent(bin, res2DR[bin - 1])
        #hDR2.SetBinError(bin, r.TMath.Sqrt(covmatres2DR[bin-1, bin-1]))
        ##print "dif. DR y datos al cuadrao", (hDR2.GetBinContent(bin) - hData2.GetBinContent(bin))**2
        ##print "cov. de datos en este bin", covmatdiagonal[bin-1, bin-1]
        ##print "Cov. de DR en este bin", covmatres2DR[bin-1, bin-1]
        #hDS2.SetBinContent(bin, res2DS[bin - 1])
        #hDS2.SetBinError(bin, r.TMath.Sqrt(covmatres2DS[bin-1, bin-1]))
        #haMC2.SetBinContent(bin, res2aMC[bin - 1])
        #haMC2.SetBinError(bin, r.TMath.Sqrt(covmatres2aMC[bin-1, bin-1]))
    
    #print "\nPRUEBINA", ty
    #print 'DR - p-val.:',        hData2.KolmogorovTest(hDR2)
    ##print 'DR - p-val. (toys):', hData2.KolmogorovTest(hDR2,'X')
    #print 'DR - p-val. (chi2):', hData2.Chi2Test(hDR2,'WW')
    #print 'DS - p-val.:',        hData2.KolmogorovTest(hDS2)
    ##print 'DS - p-val. (toys):', hData2.KolmogorovTest(hDS2,'X')
    #print 'DS - p-val. (chi2):', hData2.Chi2Test(hDS2,'WW')
    #print 'aMCatNLO - p-val.:',        hData2.KolmogorovTest(haMC2)
    ##print 'aMCatNLO - p-val. (toys):', hData2.KolmogorovTest(haMC2,'X')
    #print 'aMCatNLO - p-val. (chi2):', hData2.Chi2Test(haMC2,'WW')
    #print "\n"
    #print "\nPRUEBINA PERO CON LO DE ANTES"
    #print 'DR - p-val.:',        hData.KolmogorovTest(hDR)
    ##print 'DR - p-val. (toys):', hData.KolmogorovTest(hDR,'X')
    #print 'DR - p-val. (chi2):', hData.Chi2Test(hDR,'WW')
    #print 'DS - p-val.:',        hData.KolmogorovTest(hDS)
    ##print 'DS - p-val. (toys):', hData.KolmogorovTest(hDS,'X')
    #print 'DS - p-val. (chi2):', hData.Chi2Test(hDS,'WW')
    #print 'aMCatNLO - p-val.:',        hData.KolmogorovTest(haMCatNLO)
    ##print 'aMCatNLO - p-val. (toys):', hData.KolmogorovTest(haMCatNLO,'X')
    #print 'aMCatNLO - p-val. (chi2):', hData.Chi2Test(haMCatNLO,'WW')
    #print "\n"
    
    #print "Cosa de Serchio pa DR\n"
    #estad = ((res2 - res2DR).dot(np.linalg.inv(covmatdiagonal))).dot((res2 - res2DR).transpose())
    #print "estad.:", estad
    #print "p-valor:", 1 - chi2.cdf(estad, hData.GetNbinsX() - 1)
    
    #print "\nLo mismo pero no en lo de aquello"
    #estad = ((vdata - np.array([coses["DR"]["hist"].GetBinContent(bin) for bin in range(1, coses["DR"]["hist"].GetNbinsX() + 1)], dtype = np.double)).dot(np.linalg.inv(covmat))).dot((vdata - np.array([coses["DR"]["hist"].GetBinContent(bin) for bin in range(1, coses["DR"]["hist"].GetNbinsX() + 1)], dtype = np.double)).transpose())
    #print "estad.:", estad
    #print "p-valor:", 1 - chi2.cdf(estad, hData.GetNbinsX() - 1)
    
    #sys.exit()
    
    ## Valores de atlas manualmente: (E(b))
    #ATLASdata = np.array([ 0.00438, 0.00613, 0.00474, 0.00252, 0.00103 ], dtype = np.double)
    #ATLASmc   = np.array([ 0.0054, 0.0075, 0.005, 0.003, 0.0006 ], dtype = np.double)
    #ATLAScov  = np.diag(([ 0.41, 0.34, 0.44, 0.53, 0.18 ] * ATLASdata)**2)
    
    #pv, es = Chi2TestForMultivarNormal(ATLASdata, ATLAScov, ATLASmc, ATLAScov)
    #print "pv:", pv, "est", es
    #sys.exit()
    
    
    for key in coses:
        vtemp = np.array([coses[key]["hist"].GetBinContent(bin) for bin in range(1, coses[key]["hist"].GetNbinsX() + 1)], dtype = np.double)
        print "\ncomprobacion de lo que-y metemos al alg."
        print "covmat datos bin 1", covmat[0,0]
        print "covmat datos bin 2", covmat[1,1]
        print "covmat datos bin 3", covmat[2,2]
        print "covmat datos bin 4", covmat[3,3]
        print "datos", vdata
        print "covmat MC bin 1", coses[key]["covmat"][0,0]
        print "covmat MC bin 2", coses[key]["covmat"][1,1]
        print "covmat MC bin 3", coses[key]["covmat"][2,2]
        print "covmat MC bin 4", coses[key]["covmat"][3,3]
        print "mc", vtemp
        pv, es = Chi2TestForMultivarNormal(vdata, covmat, vtemp, coses[key]["covmat"])
        coses[key]["p-value"]   = pv
        coses[key]["statistic"] = es
    
    
    plot           = bp.beautifulUnfoldingPlots("GOF_{vr}{s}".format(vr = var, s = "_fidunorm"*(ty == "fidunorm")))
    plot.doRatio   = False
    plot.doFit     = False
    plot.plotspath = outputpath + "/"
    
    hDS.GetYaxis().SetRangeUser(0, 1.1 * max( [hDS.GetMaximum(), hDR.GetMaximum(), haMCatNLO.GetMaximum(), hData.GetMaximum()] ))
    
    plot.addHisto(hDS,       'hist',      'tW DS',      'L', 'mc')
    plot.addHisto(hDR,       'hist,same', 'tW Powheg',  'L', 'mc')
    plot.addHisto(haMCatNLO, 'hist,same', 'tW aMC@NLO', 'L', 'mc')
    plot.addHisto(hData,     'hist,same', 'Data',       'L', 'mc')
    
    l1 = r.TLatex(0.65, 0.650, '#scale[0.4]{DR - p-val.:                %4.10f}'%coses["DR"]["p-value"])
    l2 = r.TLatex(0.65, 0.625, '#scale[0.4]{DR - stat.:                 %4.10f}'%coses["DR"]["statistic"])
    l3 = r.TLatex(0.65, 0.600, '#scale[0.4]{DS - p-val.:                %4.10f}'%coses["DS"]["p-value"])
    l4 = r.TLatex(0.65, 0.575, '#scale[0.4]{DS - stat.:                 %4.10f}'%coses["DS"]["statistic"])
    l5 = r.TLatex(0.65, 0.550, '#scale[0.4]{aMC@NLO - p-val.: %4.10f}'%coses["aMCatNLO"]["p-value"])
    l6 = r.TLatex(0.65, 0.525, '#scale[0.4]{aMC@NLO - stat.:  %4.10f}'%coses["aMCatNLO"]["statistic"])
    
    l1.SetNDC(True); l2.SetNDC(True); l3.SetNDC(True); l4.SetNDC(True); l5.SetNDC(True); l6.SetNDC(True)

    l1.Draw('same'); l2.Draw('same'); l3.Draw('same'); l4.Draw('same'); l5.Draw('same'); l6.Draw('same')
    
    plot.saveCanvas("TR")
    del plot
    
    if not os.path.isdir(vl.gofpath): os.system("mkdir -p " + vl.gofpath)
    outfile = open(vl.gofpath + "/" + var + "_goftests_" + ty + ".txt", "w")

    outtxt = ""
    outtxt += "Results of GOF tests\n"
    outtxt += "Variable: {vr} \n".format(vr = var)
    outtxt += "=========================================\n"

    for key in ["DR", "DS", "aMCatNLO"]:
        outtxt += key + " / p-value: "        + str(coses[key]["p-value"])   + "\n"
        outtxt += key + " / test statistic: " + str(coses[key]["statistic"]) + "\n"

    outfile.write(outtxt)
    outfile.close(); del outfile;


    print '\nDR - p-val.:',    coses["DR"]["p-value"]
    print 'DR - stat.:',       coses["DR"]["statistic"]
    print 'DS - p-val.:',      coses["DS"]["p-value"]
    print 'DS - stat.:',       coses["DS"]["statistic"]
    print 'aMC@NLO - p-val.:', coses["aMCatNLO"]["p-value"]
    print 'aMC@NLO - stat.:',  coses["aMCatNLO"]["statistic"], "\n"
    del coses
    return


tasks = []
if varName == 'All':
    for var in vl.varList['Names']['Variables']:
        if "Fiducial" in var: continue
        for t in ["unfolding", "fidunorm"]:
            tasks.append( (var, t) )
else:
    tasks = [(varName, "unfolding"), (varName, "fidunorm")]

for el in tasks:
    GiveMeMyGoodGOFTests(el)

