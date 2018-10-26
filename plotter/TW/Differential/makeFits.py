import ROOT  as r
import varList as vl
import beautifulUnfoldingPlots as bp
import tdrstyle
import os, sys
import warnings as wr
from array import array
from multiprocessing import Pool


def getBinFromLabel(hist, labx, laby):
    result = None
    for i in range(1, hist.GetXaxis().GetNbins()+1):
        if not hist.GetXaxis().GetBinLabel(i) == labx: continue
        for j in range(1, hist.GetYaxis().GetNbins()+1):
            if not hist.GetYaxis().GetBinLabel(j) == laby: continue
            if result != None:
                raise RuntimeError('Bin label (%s,%s) is repeated'%(labx,laby))
            result=hist.GetBinContent( hist.GetBin(i,j) )
    if result == None:
        raise RuntimeError('No such bin label (%s, %s) in histo'%(labx,laby))
    return result


def makeFit(task):
    varName, syst = task
    print '\n> Fitting syst.', syst, 'of the variable', varName, '\n'
    binning = vl.varList[varName]['recobinning']
    if syst == '':
        cardList = [ 'datacard_{var}_{idx}.txt'.format(var = varName, idx=idx) for idx in range(1, len(binning))]
    else:
        cardList = [ 'datacard_{var}_{sys}_{idx}.txt'.format(var = varName, sys=syst, idx=idx) for idx in range(1, len(binning))]

    if verbose: print 'Cardlist:', cardList
    os.system('cd temp/{var}_{sys}; combineCards.py {allCards} > {outCard}; cd -'.format(allCards = ' '.join(cardList), var = varName, sys = syst,
                                                                             outCard  = 'datacard_{var}_{sys}.txt'.format(var=varName,sys=syst)))
    if verbose: print 'cd temp/{var}_{sys}; combineCards.py {allCards} > {outCard}; cd -'.format(allCards = ' '.join(cardList), var = varName, sys = syst,
                                                                             outCard  = 'datacard_{var}_{sys}.txt'.format(var=varName,sys=syst))
    physicsModel = 'text2workspace.py -m 125 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel '
    
    for idx in range(1,len(binning)):
        physicsModel = physicsModel + "--PO 'map=.*/tW_{idx}:r_tW_{idx}[1,0,10]' ".format(idx=idx)
    physicsModel = physicsModel + 'temp/{var}_{sys}/datacard_{var}_{sys}.txt -o temp/{var}_{sys}/comb_fit_{var}_{sys}.root'.format(var=varName,sys=syst)

    if not os.path.isdir('temp/{var}_{sys}/fitdiagnostics'.format(var=varName,sys=syst)):
        os.system('mkdir -p temp/{var}_{sys}/fitdiagnostics'.format(var=varName,sys=syst))

    os.system(physicsModel)
    if verbose: print physicsModel
    #os.system('combine  -M FitDiagnostics --out temp/{var}_{sys}/fitdiagnostics  temp/{var}_{sys}/comb_fit_{var}_{sys}.root --saveWorkspace -n {var}_{sys} --X-rtd MINIMIZER_MaxCalls=5000000'.format(var=varName,sys=syst))
    if verbose: print 'combine  -M FitDiagnostics --out temp/{var}_{sys}/fitdiagnostics  temp/{var}_{sys}/comb_fit_{var}_{sys}.root --saveWorkspace -n {var}_{sys} --X-rtd MINIMIZER_analytic --X-rtd MINIMIZER_MaxCalls=5000000 --saveShapes'.format(var=varName,sys=syst)
    os.system('combine  -M FitDiagnostics --out temp/{var}_{sys}/fitdiagnostics  temp/{var}_{sys}/comb_fit_{var}_{sys}.root --saveWorkspace -n {var}_{sys} --X-rtd MINIMIZER_analytic --X-rtd MINIMIZER_MaxCalls=5000000 --saveShapes'.format(var=varName,sys=syst))

    # Ahora recogemos la virutilla
    tfile     = r.TFile.Open('temp/{var}_{sys}/fitdiagnostics/fitDiagnostics{var}_{sys}.root'.format(var=varName,sys=syst))
    tfile2    = r.TFile.Open('higgsCombine{var}_{sys}.FitDiagnostics.mH120.root'.format(var=varName,sys=syst))
    fitsb     = tfile.Get('tree_fit_sb')
    fitsb.GetEntry(0)
    fitstatus = fitsb.fit_status
    if fitstatus == -1:
        raise RuntimeError('Fit of variable {var} with syst. {sys} has not converged (fit status value: {fitv})'.format(var = varName, sys = syst, fitv = fitstatus))
    elif fitstatus != 0:
        wr.warn('Fit of variable {var} with syst. {sys} has a nonzero fit status value: {fitv}'.format(var = varName, sys = syst, fitv = fitstatus), UserWarning, stacklevel=2)
    
    fitResult = tfile.Get('fit_s')
    if verbose: fitResult.Print()
    covar     = fitResult.correlationHist('covar')

    # Tambien necesitamos el workspace
    w       = tfile2.Get('w')
    poiList = r.RooArgList('poiList')
    for i in range(1,len(binning)):
        var = w.var('r_tW_%d'%i)
        poiList.add(var)
    cov = fitResult.reducedCovarianceMatrix(poiList)

    results = {}
    count   = 0
    for var in fitResult.floatParsFinal():
        count = count + 1
        #if not 'r_tW' in var.GetName(): continue # se guarda todo
        results[var.GetName()] = [ var.getVal(), var.getErrorLo(), var.getErrorHi() ]
        if count == fitResult.floatParsFinal().getSize(): break

    print '> Plotting covariance matrix'
    c = r.TCanvas('c', '{var}_{sys} covariance matrix'.format(var = varName, sys = syst), 1200, 1200)
    c.SetTopMargin(0.06)
    c.SetRightMargin(0.06)
    covar.GetXaxis().SetTitle('Free parameters of the fit')
    covar.GetXaxis().SetLabelSize(0.01)
    covar.GetXaxis().SetTitleSize(0.01)
    covar.GetXaxis().SetTitleOffset(3)
    covar.GetYaxis().SetTitle('Free parameters of the fit')
    covar.GetYaxis().SetLabelSize(0.01)
    covar.GetYaxis().SetTitleSize(0.01)
    covar.GetYaxis().SetTitleOffset(3)
    covar.SetTitle('{var}_{sys} covariance matrix'.format(var = varName, sys = syst))
    covar.SetStats(False)
    covar.Draw('colz')
    if not os.path.isdir('results/covmatrices'):
        os.system('mkdir -p results/covmatrices')
    
    c.SaveAs('results/covmatrices/{var}_{sys}.pdf'.format(var = varName, sys = syst))
    c.SaveAs('results/covmatrices/{var}_{sys}.png'.format(var = varName, sys = syst))
    c.SaveAs('results/covmatrices/{var}_{sys}.root'.format(var = varName, sys = syst))
    del c
    tfile2.Close()
    tfile.Close()

    toKeep = []
    for p in [('r_tW','tW'), ('DY','DY'), ('VVttbarV','VV+ttV'), ('ttbar','t#bar{t}')]:
        graph = r.TGraphAsymmErrors(len(binning)-1)
        graph.SetName(p[0])
        for i in range(1, len(binning)):
            if '%s_%d'%(p[0],i) not in results:
                graph.SetPoint( i-1, (binning[i-1] + binning[i])/2, 0)
                graph.SetPointError( i-1, (binning[i] - binning[i-1])/2, (binning[i] - binning[i-1])/2, 0, 0)
            else:
                graph.SetPoint( i-1, (binning[i-1] + binning[i])/2, results['%s_%d'%(p[0],i)][0])
                graph.SetPointError( i-1, (binning[i] - binning[i-1])/2, (binning[i] - binning[i-1])/2,
                                    -results['%s_%d'%(p[0],i)][1], results['%s_%d'%(p[0],i)][2])
        toKeep.append( (graph, p[1]))

    plot = bp.beautifulUnfoldingPlots('srs_{var}_{sys}'.format(var = varName, sys = syst))
    plot.addHistoInPad( len(toKeep), toKeep[0][0], 'AP', toKeep[0][1],'')
    plot.addTLatex(0.7,1-0.2, toKeep[0][1])
    plot.plotspath  = "results/srs/"
    for p in range( 1, len(toKeep)):
        plot.addHistoInPad( p+1, toKeep[p][0], 'AP', toKeep[p][1],'')
        #toKeep[p][0].GetYaxis().SetTitle('Post/pre')
        #toKeep[p][0].GetYaxis().CenterTitle(True)

        plot.addTLatex(0.7,1-1.23*float(p)/(len(toKeep)+1)-0.2, toKeep[p][1])
    setattr(plot,'noCMS',True)
    plot.saveCanvas('TR', '',False)


    # Put results into histos
    outHisto = r.TH1F('hFitResult_%s_%s'%(varName,syst), '', len(binning)-1,
                      array('d', vl.varList[varName]['recobinning']))
    for i in range( 1, len(binning)):
        if syst == '':
            card = r.TFile.Open('temp/{var}_/forCards_{var}_{ind}.root'.format(var = varName, ind = i))
        else:
            card = r.TFile.Open('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root'.format(var = varName, sys = syst, ind = i))
        
        divquantity = 1
        if vl.doxsec: divquantity = vl.Lumi * 1000
        
        results['r_tW_%d'%i][0]  = results['r_tW_%d'%i][0] * card.Get('tW_%d'%i).Integral() / divquantity
        results['r_tW_%d'%i][1]  = results['r_tW_%d'%i][1] * card.Get('tW_%d'%i).Integral() / divquantity
        results['r_tW_%d'%i][2]  = results['r_tW_%d'%i][2] * card.Get('tW_%d'%i).Integral() / divquantity
        outHisto.SetBinContent(i, results['r_tW_%d'%i][0])
        card.Close()

    errors = outHisto.Clone('hFitResult_forPlotting_%s_%s'%(varName,syst))
    for i in range( 1, len(binning)):
        errors.SetBinContent(i, results['r_tW_%d'%i][2]) # Reminder: they are symmetric. Always.
        errors.SetBinError  (i, results['r_tW_%d'%i][2])

    # Put covariance matrix into yield parametrization instead of
    # cross section parametrization
    # Also the thing shouuld be in a th2
    hCov    = r.TH2F('hCovar_%s_%s'%(varName,syst),'', len(binning)-1, -0.5, len(binning)-1.5, len(binning)-1,
                     -0.5, len(binning)-1.5)

    for i in range(1, len(binning)):
        for j in range(1, len(binning)):
            if syst == '':
                cardx = r.TFile.Open('temp/{var}_/forCards_{var}_{ind}.root'.format(var = varName, ind = i))
                cardy = r.TFile.Open('temp/{var}_/forCards_{var}_{ind}.root'.format(var = varName, ind = j))
            else:
                cardx = r.TFile.Open('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root'.format(var = varName, sys = syst, ind = i))
                cardy = r.TFile.Open('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root'.format(var = varName, sys = syst, ind = j))
            
            scale = 1
            if vl.doxsec: scale = 1/vl.Lumi/1000
            
            normx = cardx.Get('tW_%d'%i).Integral() * scale
            normy = cardy.Get('tW_%d'%j).Integral() * scale
        
            cov[i-1][j-1] = cov[i-1][j-1] * normx * normy
            
            hCov.SetBinContent( hCov.GetBin(i,j), cov[i-1][j-1] )
        
            cardx.Close()
            cardy.Close()
    
    print '\n> Syst.', syst, 'of the variable', varName, 'fitted!\n'
    return [outHisto, errors, hCov]



if __name__ == '__main__':
    vl.SetUpWarnings()
    r.gROOT.SetBatch(True)
    verbose = False

    print "===== Fitting procedure with syst. profiling\n"

    if (len(sys.argv) > 1):
        nCores      = int(sys.argv[1])
        print ('> Parallelization will be done with ' + str(nCores) + ' cores')
    else:
        print '> Sequential execution mode chosen'
        nCores      = 1

    if (len(sys.argv) > 2):
        varName     = sys.argv[2]
    else:
        print "> Default choice of variable and minitrees\n"
        varName     = 'LeadingLepEta'
    
    tasks = []
    tasks.append((varName,''))
    for sys in vl.systMap:
        tasks.append( (varName, sys) )

    tasks.append( (varName, 'pdfUp'      ) )
    tasks.append( (varName, 'pdfDown'    ) )
    tasks.append( (varName, 'ttbarMEUp'  ) )
    tasks.append( (varName, 'ttbarMEDown') )
    
    if not os.path.isdir('results/srs'):
        os.system('mkdir -p results/srs')
    
    if verbose: print 'Jobs:', tasks
    pool = Pool(nCores)
    finalresults = pool.map(makeFit, tasks)
    pool.close()
    pool.join()

    print '> Saving fit results...'
    outFile = r.TFile.Open('temp/{var}_/fitOutput_{var}.root'.format(var = varName),'recreate')
    for el in finalresults:
        for subel in el:
            subel.Write()

    outFile.Close()
    print '> Fitting procedure for variable', varName, 'succesfully completed'
