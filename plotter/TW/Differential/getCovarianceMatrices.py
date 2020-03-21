import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
from   multiprocessing import Pool
from   array import array
import sys, os, copy, math, shutil, CMS_lumi, tdrstyle
#===================================

r.gROOT.SetBatch(True)
CMS_lumi.writeExtraText = 1
plotsoutputpath = "results/CovMat"
if not os.path.isdir(plotsoutputpath):
    os.system('mkdir -p ' + plotsoutputpath)
scaleval        = 1/vl.Lumi/1000 if vl.doxsec else 1

def printCovarianceMatrix(tsk):
    var, ty, fidornot, normornot = tsk
    if ty == 'unfolded' and not fidornot: tfile = r.TFile("temp/{vr}_/unfOutput_{vr}.root".format(vr = var), "read")
    elif fidornot and not normornot:      tfile = r.TFile("temp/{vr}_/fidOutput_{vr}.root".format(vr = var), "read")
    elif normornot and fidornot:          tfile = r.TFile("temp/{vr}_/fidOutput_{vr}norm.root".format(vr = var), "read")
    elif not fidornot and normornot:      tfile = r.TFile("temp/{vr}_/unfOutput_{vr}.root".format(vr = var), "read")
    else:                                 tfile = r.TFile("temp/{vr}_/cutOutput_{vr}.root".format(vr = var), "read")
    
    if ty == "folded": nominal = copy.deepcopy(tfile.Get("data_").Clone("nominal"))
    else:              nominal = copy.deepcopy(tfile.Get(var).Clone("nominal"))
    if vl.doxsec and ty == 'folded': nominal.Scale(scaleval)
    
    varMat  = {}
    for key in tfile.GetListOfKeys():
        if key.GetName() == "data_" or "asimov" in key.GetName() or key.GetName().replace("data_", "").replace(var + "_", '') in vl.varList['Names']['colorSysts'] or key.GetName() == var or key.GetName() == "nom0" or key.GetName() == "nom1":
            continue # Colour envelope is applied in any case
        elif key.GetName() == "CovMat" and ty == "unfolded":
            varMat["unfCovMat"] = copy.deepcopy(key.ReadObj())
        else:
            if "Down" in key.GetName() or "Up" not in key.GetName(): continue
            #elif "Up" not in key.GetName():
                #print key.GetName()
                #varMat[key.GetName().replace("data_", "").replace(var + "_", "")] = copy.deepcopy(ep.getCovarianceFromVar(nominal, key.ReadObj(), var, ty))
            else:
                tmphistoup = copy.deepcopy(key.ReadObj().Clone("tmphistoup"))

                for key2 in tfile.GetListOfKeys():
                    if key2.GetName() == key.GetName().replace("Up", "Down"):
                        tmphistodw = copy.deepcopy(key2.ReadObj().Clone("tmphistodw"))
                
                varsup  = [abs(tmphistoup.GetBinContent(binin) - nominal.GetBinContent(binin)) for binin in range(1, nominal.GetNbinsX() + 1)]
                varsdw  = [abs(tmphistodw.GetBinContent(binin) - nominal.GetBinContent(binin)) for binin in range(1, nominal.GetNbinsX() + 1)]
                varsfin = [(varsup[binin] + varsdw[binin])/2 for binin in range(nominal.GetNbinsX())]
                for binin in range(1, nominal.GetNbinsX() + 1):
                    tmphistoup.SetBinContent(binin, nominal.GetBinContent(binin) + varsfin[binin - 1])
                del varsup, varsdw, varsfin, tmphistodw
                varMat[key.GetName().replace("data_", "").replace(var + "_", "")] = copy.deepcopy(ep.getCovarianceFromVar(nominal, tmphistoup, var, ty))
                del tmphistoup
    
    
    colup = copy.deepcopy(nominal.Clone("ColorRUp"))
    coldn = copy.deepcopy(nominal.Clone("ColorRDown"))
    
    for bin in range(1, nominal.GetNbinsX() + 1): # Colour envelope is applied in any case
        tmpuncUp   = nominal.GetBinContent(bin)
        tmpuncDown = nominal.GetBinContent(bin)
        tmpunc     = 0.
        for key in tfile.GetListOfKeys():
            if key.GetName().replace("data_", "").replace(var+"_", "") in vl.varList['Names']['colorSysts']:
                tmpunc  = key.ReadObj().GetBinContent(bin)
                if tmpunc > tmpuncUp:
                    tmpuncUp    = tmpunc
                elif tmpunc < tmpuncDown:
                    tmpuncDown  = tmpunc
        colup.SetBinContent(bin, tmpuncUp)
        coldn.SetBinContent(bin, tmpuncDown)
    
    varsup  = [abs(colup.GetBinContent(binin) - nominal.GetBinContent(binin)) for binin in range(1, nominal.GetNbinsX() + 1)]
    varsdw  = [abs(coldn.GetBinContent(binin) - nominal.GetBinContent(binin)) for binin in range(1, nominal.GetNbinsX() + 1)]
    varsfin = [(varsup[binin] + varsdw[binin])/2 for binin in range(nominal.GetNbinsX())]
    for binin in range(1, nominal.GetNbinsX() + 1):
        colup.SetBinContent(binin, nominal.GetBinContent(binin) + varsfin[binin - 1])
    del varsup, varsdw, varsfin
    
    varMat["ColorRUp"]   = ep.getCovarianceFromVar(nominal, colup, var, ty)
    #varMat["ColorRDown"] = ep.getCovarianceFromVar(nominal, coldn, var, ty)
    
    binning  = array('f', vl.varList[var]['recobinning'] if ty == "folded" else vl.varList[var]['genbinning'])
    
    if fidornot: # We must calculate the (complete) covariance matrix for the statistical uncertainties for this case, as it is not directly providen by doFiducial.py nor TUnfold.
        ffid = r.TFile.Open("temp/Fiducial_/unfOutput_Fiducial.root",       "read")
        fvar = r.TFile.Open("temp/" + var + "_/unfOutput_" + var + ".root", "read")
        newmat = r.TH2F('newmat', '', nominal.GetNbinsX(), binning, nominal.GetNbinsX(), binning)
        
        dxs = copy.deepcopy(fvar.Get(var).Clone("dxs"))
        covm = copy.deepcopy(fvar.Get("CovMat").Clone("covm"))
        for key in ffid.GetListOfKeys():
            if key.GetName() != "Fiducial": continue
            dfid = r.TH1D(fvar.Get(var))
            for bin in range(1, dfid.GetNbinsX() + 1):
                dfid.SetBinContent(bin, key.ReadObj().GetBinContent(1))
                dfid.SetBinError(bin,   key.ReadObj().GetBinError(1))
        #print "\nFiducial"
        for bin1 in range(1, nominal.GetNbinsX() + 1):
            for bin2 in range(1, nominal.GetNbinsX() + 1):
                goodunc = (covm.GetBinContent(bin1, bin2) / dfid.GetBinContent(bin1)**2 +
                           dxs.GetBinContent(bin1) * dxs.GetBinContent(bin2) * dfid.GetBinError(bin1)**2 / dfid.GetBinContent(bin1)**4 -
                           dxs.GetBinContent(bin2) / dfid.GetBinContent(bin1)**3 * sum([covm.GetBinContent(bin1, j) for j in range(1, dxs.GetNbinsX() + 1)]) -
                           dxs.GetBinContent(bin1) / dfid.GetBinContent(bin1)**3 * sum([covm.GetBinContent(bin2, j) for j in range(1, dxs.GetNbinsX() + 1)]) )
                #if bin1 == bin2: print "bin", bin1, goodunc
                newmat.SetBinContent(bin1, bin2, goodunc)
        
        if normornot:
            newmat.Scale(1, "width"); # THIS IS A COVARIANCE!! Constants that apply to the random variable apply SQUARED to the covariance (and variance)! NOTE: when you have a 2D histo, the scale is already done "two times".
            #print "\nFiducialnorm"
        varMat["statistical"] = copy.deepcopy(newmat.Clone("statistical"))
        #for bin in range(1, nominal.GetNbinsX() + 1):
            #print "bin", bin, newmat.GetBinContent(bin, bin)
        ffid.Close(); fvar.Close(); del ffid, fvar, dfid, dxs, newmat;
    
    if ty == "folded":
        newmat = r.TH2F('newmat', '', nominal.GetNbinsX(), binning, nominal.GetNbinsX(), binning)
        for bin in range(1, nominal.GetNbinsX() + 1):
            newmat.SetBinContent(bin, bin, nominal.GetBinError(bin)**2)
        varMat["statistical"] = copy.deepcopy(newmat.Clone("statistical"))
        del newmat;
    idnx = 0
    finalmat = r.TH2F('finalmat', '', nominal.GetNbinsX(), binning, nominal.GetNbinsX(), binning)
    for key in varMat:
        idnx += 1
        #print idnx, key
        #print "finalmat.GetNbinsX()", finalmat.GetNbinsX()
        #print "varMat[key].GetNbinsX()", varMat[key].GetNbinsX()
        finalmat.Add(varMat[key])
    
    #for bin in range(1, nominal.GetNbinsX() + 1): print "matfinal bin", bin, ":", finalmat.GetBinContent(bin, bin)
    
    if normornot and not fidornot: finalmat.Scale(1, "width")

    if fidornot and ("unfCovMat" in varMat or "statistical" not in varMat): raise RuntimeError("You are calculating the covariance matrix of some fiducial results but you do not have the statistical uncertainties covariance matrix!!!!")
    
    tdrstyle.setTDRStyle()
    finalmat.SetStats(False)
    finalmat.SetXTitle(vl.varList[var]['xaxis'])
    finalmat.SetYTitle(vl.varList[var]['xaxis'])
    finalmat.SetMarkerColor(r.kRed)
    finalmat.SetMarkerColor(r.kRed)
    if 'covtxtsizeunf' in vl.varList[var] and ty == "unfolded" and not fidornot: finalmat.SetMarkerSize(vl.varList[var]['covtxtsizeunf'])
    elif 'covtxtsizefol' in vl.varList[var] and ty == "folded": finalmat.SetMarkerSize(vl.varList[var]['covtxtsizefol'])
    elif 'covtxtsizefidnorm' in vl.varList[var] and fidornot and normornot: finalmat.SetMarkerSize(vl.varList[var]['covtxtsizefidnorm'])
    
    fcovmat = r.TFile("temp/{vr}_/CovMat_{vr}_{t}{lodefid}{lodenorm}.root".format(vr = var, t = ty, lodefid = "_fiducial"*(fidornot), lodenorm = "norm"*(normornot)), "recreate")
    finalmat.Write()
    fcovmat.Close()
    
    r.gStyle.SetPaintTextFormat("4.5f" if not normornot else "4.8f")
    r.gStyle.SetPadRightMargin(0.17)
    r.gStyle.SetPadTopMargin(0.05)
    r.gStyle.SetPadBottomMargin(0.1)
    r.gStyle.SetPadLeftMargin(0.12)
    finalmat.GetYaxis().SetTitleOffset(1.5)
    finalmat.GetXaxis().SetTitleOffset(1.1)
    finalmat.GetXaxis().SetTitleFont(43)
    finalmat.GetXaxis().SetTitleSize(22)
    finalmat.GetXaxis().SetLabelFont(43)
    finalmat.GetXaxis().SetLabelSize(22)
    finalmat.GetYaxis().SetTitleFont(43)
    finalmat.GetYaxis().SetTitleSize(22)
    finalmat.GetYaxis().SetLabelFont(43)
    finalmat.GetYaxis().SetLabelSize(22)
    r.gStyle.SetLabelFont(43, "XYZ")
    r.gStyle.SetLabelSize(22, "XYZ")
    c = r.TCanvas('c', "", 200, 10, 600, 600)
    finalmat.Draw("colz text{s}".format(s = vl.varList[var]['covtxtangle'] if 'covtxtangle' in vl.varList[var] and ty == "folded" else vl.varList[var]['covtxtangleunf'] if ('covtxtangleunf' in vl.varList[var] and ty == "unfolded" and not fidornot) else vl.varList[var]['covtxtangleunffid'] if ('covtxtangleunffid' in vl.varList[var] and fidornot and not normornot) else vl.varList[var]['covtxtangleunffidnorm'] if ('covtxtangleunffidnorm' in vl.varList[var] and normornot) else ""))
    r.gPad.Update()
    CMS_lumi.lumi_13TeV = ""
    #CMS_lumi.extraText  = 'Simulation Supplementary'
    CMS_lumi.extraText  = 'Simulation Preliminary'
    CMS_lumi.lumi_sqrtS = ''
    #CMS_lumi.cmsTextSize += 0.1
    CMS_lumi.CMS_lumi(r.gPad, 0, 0, 0.05)
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}{lodefid}{lodenorm}.png" .format(vr = var, t = ty, lodefid = "_fiducial"*(fidornot), lodenorm = "norm"*(normornot)))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}{lodefid}{lodenorm}.pdf" .format(vr = var, t = ty, lodefid = "_fiducial"*(fidornot), lodenorm = "norm"*(normornot)))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}{lodefid}{lodenorm}.eps" .format(vr = var, t = ty, lodefid = "_fiducial"*(fidornot), lodenorm = "norm"*(normornot)))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}{lodefid}{lodenorm}.root".format(vr = var, t = ty, lodefid = "_fiducial"*(fidornot), lodenorm = "norm"*(normornot)))
    c.Close(); tfile.Close()
    del c, nominal, colup, coldn, tfile, varMat, finalmat, binning
    return


if __name__=="__main__":
    if (len(sys.argv) > 1):
        varName     = sys.argv[1]
        print "> Chosen variable:", varName, "\n"
        if (len(sys.argv) > 2):
            nCores      = int(sys.argv[2])
            print ('> Parallelization will be done with ' + str(nCores) + ' cores')
        else:
            print '> Sequential execution mode chosen'
            nCores      = 1
    else:
        print "> Default choice of variable and minitrees\n"
        varName     = 'LeadingLepEta'
        nCores      = 1

    vl.SetUpWarnings()
    if nCores == 1:
        if varName != "All":
            printCovarianceMatrix((varName, 'folded',   False, False))
            printCovarianceMatrix((varName, 'unfolded', False, False))
            printCovarianceMatrix((varName, 'unfolded', True,  False))
            printCovarianceMatrix((varName, 'unfolded', True,  True))
            printCovarianceMatrix((varName, 'unfolded', False,  True))
        else:
            for var in vl.varList['Names']['Variables']:
                if "Fiducial" in var: continue
                printCovarianceMatrix((var, 'folded',   False, False))
                printCovarianceMatrix((var, 'unfolded', False, False))
                printCovarianceMatrix((var, 'unfolded', True,  False))
                printCovarianceMatrix((var, 'unfolded', True,  True))
                printCovarianceMatrix((var, 'unfolded', False,  True))
    else:
        tasks = []
        if varName == "All":
            for var in vl.varList['Names']['Variables']:
                if "Fiducial" in var: continue
                for el in ['folded', 'unfolded']:
                    tasks.append( (var, el, False, False) )
                    if el == "unfolded": 
                        tasks.append( (var, el, True, False))
                        tasks.append( (var, el, True, True) )
                        tasks.append( (var, el, False, True) )
        else:
            tasks.append((varName, 'folded',   False, False))
            tasks.append((varName, 'unfolded', False, False))
            tasks.append((varName, 'unfolded', True,  False))
            tasks.append((varName, 'unfolded', True,  True))
            tasks.append((varName, 'unfolded', False,  True))
        pool = Pool(nCores)
        pool.map(printCovarianceMatrix, tasks)
        pool.close()
        pool.join()
        del pool
