import ROOT as r
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl
import warnings as wr
from   multiprocessing import Pool
from   array import array
import sys, os, copy, math, shutil
#===================================

r.gROOT.SetBatch(True)
plotsoutputpath = "results/CovMat"
if not os.path.isdir(plotsoutputpath):
    os.system('mkdir -p ' + plotsoutputpath)
scaleval        = 1/vl.Lumi/1000 if vl.doxsec else 1


def printCovarianceMatrix(tsk):
    var, ty = tsk
    if ty == 'unfolded': tfile = r.TFile("temp/{vr}_/unfOutput_{vr}.root".format(vr = var), "read")
    else:                tfile = r.TFile("temp/{vr}_/cutOutput_{vr}.root".format(vr = var), "read")
    
    if ty == "folded": nominal = copy.deepcopy(tfile.Get("data_").Clone("nominal"))
    else:              nominal = copy.deepcopy(tfile.Get(var).Clone("nominal"))
    if vl.doxsec and ty == 'folded': nominal.Scale(scaleval)
    
    varMat  = {}
    for key in tfile.GetListOfKeys():
        if key.GetName() == "data_" or (ty == "folded" and "asimov" in key.GetName()) or key.GetName().replace("data_", "").replace(var + "_", '') in vl.varList['Names']['colorSysts'] or key.GetName() == var:
            continue # Colour envelope is applied in any case
        elif key.GetName() == "CovMat" and ty == "unfolded": varMat["unfCovMat"] = copy.deepcopy(key.ReadObj())
        else: varMat[key.GetName().replace("data_", "").replace(var + "_", "")] = copy.deepcopy(ep.getCovarianceFromVar(nominal, key.ReadObj(), var, ty))
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
    
    varMat["ColorRUp"]   = ep.getCovarianceFromVar(nominal, colup, var, ty)
    varMat["ColorRDown"] = ep.getCovarianceFromVar(nominal, coldn, var, ty)
    
    binning  = array('f', vl.varList[var]['recobinning'] if ty == "folded" else vl.varList[var]['genbinning'])
    finalmat = r.TH2F('finalmat', '', nominal.GetNbinsX(), binning, nominal.GetNbinsX(), binning)
    for key in varMat:
        finalmat.Add(varMat[key])
    
    finalmat.SetStats(False)
    finalmat.SetXTitle(vl.varList[var]['xaxis'])
    finalmat.SetYTitle(vl.varList[var]['xaxis'])
    
    fcovmat = r.TFile("temp/{vr}_/CovMat_{vr}.root".format(vr = var), "recreate")
    finalmat.Write()
    fcovmat.Close()
    
    r.gStyle.SetPaintTextFormat("4.5f")
    r.gStyle.SetPadRightMargin(0.13)
    r.gStyle.SetPadTopMargin(0.05)
    r.gStyle.SetPadLeftMargin(0.08)
    c = r.TCanvas('c', "", 200, 10, 700, 500)
    finalmat.Draw("colz text")
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}.png" .format(vr = var, t = ty))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}.pdf" .format(vr = var, t = ty))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}.eps" .format(vr = var, t = ty))
    c.SaveAs(plotsoutputpath + "/Cov_{vr}_{t}.root".format(vr = var, t = ty))
    tfile.Close()
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
            printCovarianceMatrix((varName, 'folded'))
            printCovarianceMatrix((varName, 'unfolded'))
        else:
            for var in vl.varList['Names']['Variables']:
                printCovarianceMatrix((var, 'folded'))
                printCovarianceMatrix((var, 'unfolded'))
    else:
        tasks = []
        if varName == "All":
            for var in vl.varList['Names']['Variables']:
                for el in ['folded', 'unfolded']:
                    tasks.append( (var, el) )
        else:
            tasks.append((varName, 'folded'))
            tasks.append((varName, 'unfolded'))
        pool = Pool(nCores)
        pool.map(printCovarianceMatrix, tasks)
        pool.close()
        pool.join()
        del pool
