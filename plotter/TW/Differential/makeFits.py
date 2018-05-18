import ROOT  as r
import os,sys
import varList, tdrstyle
from array import array
from multiprocessing import Pool

r.gROOT.SetBatch(True)
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
    binning = varList.varList[varName]['recobinning']

    if syst == '':
        cardList = [ 'datacard_{var}_{idx}.txt'.format(var = varName, idx=idx) for idx in range(1, len(binning))]
    else:
        cardList = [ 'datacard_{var}_{sys}_{idx}.txt'.format(var = varName, sys=syst, idx=idx) for idx in range(1, len(binning))]


    os.system('cd temp; combineCards.py {allCards} > {outCard}; cd -'.format(allCards = ' '.join(cardList),
                                                                             outCard  = 'datacard_{var}_{sys}.txt'.format(var=varName,sys=syst)))

    physicsModel = 'text2workspace.py -m 125 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel '
    for idx in range(1,len(binning)):
        physicsModel = physicsModel + "--PO 'map=.*/tW_{idx}:r_tW_{idx}[1,0,10]' ".format(idx=idx)
    physicsModel = physicsModel + 'temp/datacard_{var}_{sys}.txt -o temp/comb_fit_{var}_{sys}.root'.format(var=varName,sys=syst)

    if not os.path.isdir('temp/{var}_{sys}/fitdiagnostics'.format(var=varName,sys=syst)):
        os.system('mkdir -p temp/{var}_{sys}/fitdiagnostics'.format(var=varName,sys=syst))

    os.system(physicsModel)
    os.system('combine -M FitDiagnostics --out temp/{var}_{sys}/fitdiagnostics  temp/comb_fit_{var}_{sys}.root --saveWorkspace -n {var}_{sys}'.format(var=varName,sys=syst))

    # Ahora recogemos la virutilla
    tfile     = r.TFile.Open('temp/{var}_{sys}/fitdiagnostics/fitDiagnostics{var}_{sys}.root'.format(var=varName,sys=syst))
    tfile2    = r.TFile.Open('higgsCombine{var}_{sys}.FitDiagnostics.mH120.root'.format(var=varName,sys=syst))
    fitResult = tfile.Get('fit_s')
    covar     = fitResult.correlationHist()

    # Tambien necesitamos el workspace
    w=tfile2.Get('w')
    poiList = r.RooArgList('poiList')
    for i in range(1,len(binning)):
        var = w.var('r_tW_%d'%i)
        poiList.add(var)
    cov = fitResult.reducedCovarianceMatrix(poiList)

    results = {}
    count = 0 
    for var in fitResult.floatParsFinal():
        if count == fitResult.floatParsFinal().getSize()-1: break
        count = count + 1
        if not 'r_tW' in var.GetName(): continue
        results[var.GetName()] = [ var.getVal(), var.getErrorLo(), var.getErrorHi() ]

    tfile2.Close()
    tfile.Close()

    # Put results into histos
    outHisto = r.TH1F('hFitResult_%s_%s'%(varName,syst), '', len(binning)-1,
                      array('d', varList.varList[varName]['recobinning']))
    for i in range( 1, len(binning)):
        if syst == '':
            card = r.TFile.Open('temp/forCards_%s_%d.root'%(varName,i))
        else:
            card = r.TFile.Open('temp/forCards_%s_%s_%d.root'%(varName,syst,i))
        results['r_tW_%d'%i][0]  = results['r_tW_%d'%i][0] * card.Get('tW_%d'%i).Integral()
        results['r_tW_%d'%i][1]  = results['r_tW_%d'%i][1] * card.Get('tW_%d'%i).Integral()
        results['r_tW_%d'%i][2]  = results['r_tW_%d'%i][2] * card.Get('tW_%d'%i).Integral()
        outHisto.SetBinContent(i, results['r_tW_%d'%i][0])
        card.Close() 

    errors = outHisto.Clone('hFitResult_forPlotting_%s_%s'%(varName,syst))
    for i in range( 1, len(binning)):
        #upVar = outHisto.GetBinContent(i) + results['r_tW_%d'%i][2]
        #dnVar = outHisto.GetBinContent(i) + results['r_tW_%d'%i][1]
        #print 'El 1', results['r_tW_%d'%i][1], 'El 2', results['r_tW_%d'%i][2]
        #errors.SetBinContent(i, ( upVar + dnVar ) / 2)
        #errors.SetBinError  (i, abs( upVar - dnVar) / 2)
        errors.SetBinContent(i, results['r_tW_%d'%i][2])
        errors.SetBinError  (i, results['r_tW_%d'%i][2])

    # Put covariance matrix into yield parametrization instead of
    # cross section parametrization
    # Also the thing shouuld be in a th2
    hCov    = r.TH2F('hCovar_%s_%s'%(varName,syst),'', len(binning)-1, -0.5, len(binning)-1.5, len(binning)-1,
                     -0.5, len(binning)-1.5)

    for i in range(1, len(binning)):
        for j in range(1, len(binning)):
            if syst == '':
                cardx = r.TFile.Open('temp/forCards_%s_%d.root'%(varName,i))
                cardy = r.TFile.Open('temp/forCards_%s_%d.root'%(varName,j))
            else:
                cardx = r.TFile.Open('temp/forCards_%s_%s_%d.root'%(varName,syst,i))
                cardy = r.TFile.Open('temp/forCards_%s_%s_%d.root'%(varName,syst,j))

            normx = cardx.Get('tW_%d'%i).Integral() 
            normy = cardy.Get('tW_%d'%j).Integral() 
        
            cov[i-1][j-1] = cov[i-1][j-1] * normx * normy
            
            hCov.SetBinContent( hCov.GetBin(i,j), cov[i-1][j-1] )
        
            cardx.Close()
            cardy.Close()

    outFile = r.TFile.Open('temp/fitOutput_%s.root'%varName,'recreate' if syst == '' else 'update')
    outHisto.Write()
    errors.Write()
    hCov.Write()
    outFile.Close()
    print '\n> Syst.', syst, 'of the variable', varName, 'fitted!\n'



# el fit nominal tiene que ir primero y por separado SIEMPRE (porque es el que crea el file de output)
makeFit((varName,''))


tasks = []
from varList import systMap
for sys in systMap:
    tasks.append( (varName, sys) )

#pool = Pool(nCores)
pool = Pool(1)
# thing in paralel wont work because its trying to modify many
# pool should deliver the histograms and they should be stored afterwards
pool.map(makeFit, tasks)
pool.close()
pool.join()

print '> Fitting procedure for variable', varName, 'succesfully completed'

# outHisto.SetMarkerStyle(r.kFullCircle)
# errors.SetFillColor(r.kBlue)
# errors.Draw('e2')
# outHisto.Draw('P,same')
# raw_input('asdf')

