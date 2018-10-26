import ROOT     as r
import varList  as vl
import warnings as wr
import sys, os, copy
from multiprocessing import Pool
print "===== BDT's histograms procedure with systs. profiling\n"

storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
pathToTree  = ""
NameOfTree  = "Mini1j1t";
systlist    = "JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"

if (len(sys.argv) > 1):
    nCores      = int(sys.argv[1])
    print ('> Parallelization will be done with ' + str(nCores) + ' cores')
else:
    print '> Sequential execution mode chosen'
    nCores      = 1

if (len(sys.argv) > 2):
    varName     = sys.argv[2]
    print "> Chosen variable:", varName, "\n"
    if (len(sys.argv) > 3):
        if sys.argv[3] == 'last':
            pathToTree    = vl.GetLastFolder(storagepath)
        else:
            pathToTree    = storagepath + sys.argv[3] + "/"
    else:
        pathToTree  = "../../../TW_temp/"
    print "> Minitrees will be read from:", pathToTree, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'
    pathToTree  = "../../../TW_temp/"


r.gROOT.SetBatch(True)
r.gROOT.LoadMacro('../../Histo.C+')
r.gROOT.LoadMacro('../../Looper.C+')
r.gROOT.LoadMacro('../../Plot.C+')
r.gROOT.LoadMacro('../../PlotToPy.C+')
r.gROOT.LoadMacro('../../Datacard.C+')
r.gROOT.LoadMacro('../../PDFunc.C+')

r.gROOT.LoadMacro('./temp/{var}_/{var}.C+'.format(var = varName))
print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"


def getCardsNominal(task):
    binDn, binUp, indx = task
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT + 0.5), r.TString(varName + '_%d'%indx), r.TString('BDT disc. (bin %s)'%(str(indx))))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder('./temp/{var}_/'.format(var = varName));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.SetLumi(vl.Lumi)
    
    p.AddSample("TTbar_Powheg",          "ttbar_{ind}".format(ind = indx),     r.itBkg, 633, systlist, opts)
    
    p.AddSample("TTbar_PowhegSemi",      "Non-WorZ_%d"%indx,                   r.itBkg, 413, systlist, opts)
    p.AddSample("WJetsToLNu_MLM",        "Non-WorZ_%d"%indx,                   r.itBkg, 413, systlist, opts)
    
    p.AddSample("WZ",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("WW",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("ZZ",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToLNu",              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToQQ" ,              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToQQ" ,              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToLLNuNu",           "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTGJets",               "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,                  r.itBkg, 852, systlist, opts)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,                  r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M5to50_MLM", "DY_%d"%indx,                         r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M50_MLM",    "DY_%d"%indx,                         r.itBkg, 852, systlist, opts)
    
    p.AddSample("TW",                    "tW_%d"%indx,                         r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    p.AddSample("TbarW",                 "tW_%d"%indx,                         r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    
    p.AddSample("TW",                    "tW_%d"%indx,                         r.itSys, 1, "JERUp", opts)
    p.AddSample("TbarW",                 "tW_%d"%indx,                         r.itSys, 1, "JERUp", opts)
    p.AddSample("TTbar_Powheg",          "ttbar_{ind}".format(ind = indx),     r.itSys, 1, "JERUp", opts)
    p.AddSymmetricHisto("ttbar_{ind}".format(ind = indx),  "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not vl.asimov:
        p.AddSample("MuonEG",            "Data",                               r.itData, 0, '', opts)
        p.AddSample("SingleMuon",        "Data",                               r.itData, 0, '', opts)
        p.AddSample("SingleElec",        "Data",                               r.itData, 0, '', opts)
    else:
        hData=r.Histo(p.GetHisto('tW_%d'%indx).Clone("Data"))
        for proc in ['ttbar_{ind}'.format(ind = indx),'VVttbarV_{ind}'.format(ind = indx), "DY_%d"%indx, "Non-WorZ_%d"%indx]:
            hData.Add( p.GetHisto(proc) )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)
    
    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92)
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    p.SetTitleY(r.TString(vl.varList[varName]['yaxis']))
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_{var}_{ind}".format(var = varName, ind = indx));
    p.SaveHistograms();
    del p
    if vl.asimov: del hData
    
    if uncorrelate:
        # Uncorrelation things
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, ind = indx), 'update')
        for key in tfile.GetListOfKeys():
            if 'JES' in key.GetName() and 'JES%i'%indx not in key.GetName(): key.SetName(key.GetName().replace('JES', 'JES%i'%indx))
        tfile.Close()
    
    if uncorrelate:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist.replace('JES', 'JES%i'%indx) + ', JER', "ElMu_%d"%indx) # For uncorrelation
    else:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx) # For correlation
    card.SetRootFileName('./temp/{var}_/forCards_{var}_{ind}'.format(var = varName, ind = indx))
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx), 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_/datacard_{var}_{ind}".format(var = varName, ind = indx));
    del card
    
    # All this crap so i dont have to tamper with the DataCard.C
    out     = ''
    toelimi = False
    datacarta = open('temp/{var}_/datacard_{var}_{ind}.txt'.format(var = varName, ind = indx), 'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine:
            numproc = nuLine.split()
            procind = [i for i,x in enumerate(numproc) if x == '-'][0]
            toelimi = True
            wr.warn('There are yields of (at least) one group of processes of variable {var} when considering the nominal variations and the bin {ind} that are zero. This process will be removed to avoid problems in the fit.'.format(var = varName, ind = indx), UserWarning, stacklevel=2)
            break
        out = out + nuLine
    
    datacarta.close()   # Because of reasons, after using one time the readlines() method, the file must be reoponed in order to use it again.
    datacarta = open('temp/{var}_/datacard_{var}_{ind}.txt'.format(var = varName, ind = indx), 'r')
    
    if toelimi:
        out     = ''
        nomproc = None
        nomprocs= None
        for lin in datacarta.readlines():
            nuLine = lin
            if 'bin' in nuLine and len(nuLine.split()) > 2: nuLine = ' '.join(nuLine.split()[:-1])
            if 'process' in nuLine: 
                if nomproc == None: 
                    nomproc = nuLine.split()[procind]
                    nomprocs= nuLine.split()[1:]
                nuLine = nuLine.replace('-1', '-%d'%indx).split(' ')
                del nuLine[procind]
                nuLine = '\n'*('tW' in nuLine[-2])+' '.join(nuLine)
            elif 'rate' in nuLine and '-' in nuLine:
                nuLine = nuLine.replace('- ', '')
            elif '#' not in nuLine and 'autoMCStats' not in nuLine and nomproc != None and ('shape' in nuLine or 'Lumi' in nuLine or (i in nuLine for i in nomprocs)):
                nuLine = nuLine.split(' ')
                del nuLine[procind + 1]
                nuLine = ' '.join(nuLine)
            
            if nomproc != None and nomproc in nuLine:
                continue
            else:
                out = out + nuLine
       
    outCarta = open('temp/{var}_/datacard_{var}_{ind}.txt'.format(var = varName, ind = indx), 'w')
    outCarta.write(out)
    outCarta.close()
    return


def getCardsSyst(task):
    binDn, binUp, indx, syst = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.SetLumi(vl.Lumi)
    
    if 'UEUp' in syst:                                                                                                 # For all samples: all the if statement
        specialweight = vl.nUEUp_ttbar/vl.sigma_ttbar/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nUEUp_dilep/vl.sigma_dilep/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueUp',                 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTbar2L_Powheg_ueUp',                 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'UEDown' in syst:
        specialweight = vl.nUEDown_ttbar/vl.sigma_ttbar/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nUEDown_dilep/vl.sigma_dilep/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueDown',               'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTbar2L_Powheg_ueDown',               'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampUp' in syst:
        specialweight = vl.nhDampUp_ttbar/vl.sigma_ttbar/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nhDampUp_dilep/vl.sigma_dilep/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampUp',              'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTbar2L_Powheg_hdampUp',              'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampDown' in syst:
        specialweight = vl.nhDampDown_ttbar/vl.sigma_ttbar/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nhDampDown_dilep/vl.sigma_dilep/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampDown',            'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTbar2L_Powheg_hdampDown',            'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'GluonMoveCRTune' in syst:
        specialweight = vl.nGluonMoveCRTune_ttbar/vl.sigma_ttbar/(vl.nGluonMoveCRTune_ttbar/vl.sigma_ttbar + vl.nGluonMoveCRTune_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nGluonMoveCRTune_dilep/vl.sigma_dilep/(vl.nGluonMoveCRTune_ttbar/vl.sigma_ttbar + vl.nGluonMoveCRTune_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTTo2L2Nu_GluonMoveCRTune',           'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTTo2L2Nu_GluonMoveCRTune',           'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'PowhegerdON' in syst:
        specialweight = vl.nPowhegerdON_ttbar/vl.sigma_ttbar/(vl.nPowhegerdON_ttbar/vl.sigma_ttbar + vl.nPowhegerdON_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nPowhegerdON_dilep/vl.sigma_dilep/(vl.nPowhegerdON_ttbar/vl.sigma_ttbar + vl.nPowhegerdON_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTTo2L2Nu_Powheg_erdON',            'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTTo2L2Nu_Powheg_erdON',            'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'QCDbasedCRTuneerdON' in syst:
        specialweight = vl.nQCDbasedCRTuneerdON_ttbar/vl.sigma_ttbar/(vl.nQCDbasedCRTuneerdON_ttbar/vl.sigma_ttbar + vl.nQCDbasedCRTuneerdON_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        
        specialweight = vl.nQCDbasedCRTuneerdON_dilep/vl.sigma_dilep/(vl.nQCDbasedCRTuneerdON_ttbar/vl.sigma_ttbar + vl.nQCDbasedCRTuneerdON_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTTo2L2Nu_QCDbasedCRTune_erdON',            'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample('TTTo2L2Nu_QCDbasedCRTune_erdON',            'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    else:
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist, opts)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp", opts)
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");

    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTGJets",                     "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",     "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M5to50_MLM",       "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M50_MLM",          "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    
    p.AddSample(vl.systMap[syst]["TW"],        "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    p.AddSample(vl.systMap[syst]["TbarW"],     "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    # NOTE: comment or uncomment the following three lines AND the whole if statement to consider less or more samples for syst. uncs.
    #p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)           # For fewer samples
    #p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp");             # For fewer samples
    #p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");                                                  # For fewer samples

    p.AddSample(vl.systMap[syst]["TW"],        "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSample(vl.systMap[syst]["TbarW"],     "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not vl.asimov:
        p.AddSample("MuonEG",                  "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleMuon",              "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleElec",              "Data",                              r.itData, 0, '', opts)
    else:
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, ind = indx))
        if not tfile:
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        hData = r.Histo( tfile.Get('data_obs') )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92)
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx));
    p.SaveHistograms();
    del p
    if vl.asimov:
        del hData
        tfile.Close()
    
    if uncorrelate:
        # Uncorrelation things
        tfile = r.TFile.Open("temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root".format(var = varName, sys = syst, ind = indx), 'update')
        for key in tfile.GetListOfKeys():
            if 'JES' in key.GetName() and 'JES%i'%indx not in key.GetName(): key.SetName(key.GetName().replace('JES', 'JES%i'%indx))
        tfile.Close()
    
    if uncorrelate:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist.replace('JES', 'JES%i'%indx) + ', JER', "ElMu_%d"%indx) # For uncorrelation
    else:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx) # For correlation
    card.SetRootFileName("temp/{var}_{sys}/forCards_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx))
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx), 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx))
    del card
    
    # All this crap so i dont have to tamper with the DataCard.C
    out     = ''
    toelimi = False
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine:
            numproc = nuLine.split()
            procind = [i for i,x in enumerate(numproc) if x == '-'][0]
            toelimi = True
            wr.warn('There are yields of (at least) one group of processes of variable {var} when considering the systematic {sys} and the bin {ind} that are zero. This process will be removed to avoid problems in the fit.'.format(var = varName, sys = syst, ind = indx), UserWarning, stacklevel=2)
            break
        out = out + nuLine
    
    datacarta.close()   # Because of reasons, after using one time the readlines() method, the file must be reoponed in order to use it again.
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    
    if toelimi:
        out     = ''
        nomproc = None
        nomprocs= None
        for lin in datacarta.readlines():
            nuLine = lin
            if 'bin' in nuLine and len(nuLine.split()) > 2: nuLine = ' '.join(nuLine.split()[:-1])
            if 'process' in nuLine: 
                if nomproc == None: 
                    nomproc = nuLine.split()[procind]
                    nomprocs= nuLine.split()[1:]
                nuLine = nuLine.replace('-1', '-%d'%indx).split(' ')
                del nuLine[procind]
                nuLine = '\n'*('tW' in nuLine[-2])+' '.join(nuLine)
            elif 'rate' in nuLine and '-' in nuLine:
                nuLine = nuLine.replace('- ', '')
            elif '#' not in nuLine and 'autoMCStats' not in nuLine and nomproc != None and ('shape' in nuLine or 'Lumi' in nuLine or (i in nuLine for i in nomprocs)):
                nuLine = nuLine.split(' ')
                del nuLine[procind + 1]
                nuLine = ' '.join(nuLine)
            
            if nomproc != None and nomproc in nuLine:
                continue
            else:
                out = out + nuLine
       
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'w')
    outCarta.write(out)
    outCarta.close()
    return


def getCardsPdf(task):
    binDn, binUp, indx, syst = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.SetLumi(vl.Lumi)
    
    ### Here follows an approximate approach to obtain ME and PDF uncs.
    pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), r.TString('theBDt_bin%d( TBDT )'%indx), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    pdf.verbose = False
    pdf.SetLumi(vl.Lumi * 1000)
    
    if 'pdfUp' == syst:
        histo = pdf.GetSystHisto("up","pdf").CloneHisto()
    elif 'pdfDown' == syst:
        histo = pdf.GetSystHisto("Down","pdf").CloneHisto()
    elif 'ttbarMEUp' == syst:
        histo = pdf.GetSystHisto("up","ME").CloneHisto()
    elif 'ttbarMEDown' == syst:
        histo = pdf.GetSystHisto("Down","ME").CloneHisto()
    else:
        raise RuntimeError("Systematic %s is not of 'pdf' type"%syst)
    p.PrepareHisto(histo, 'TTbar_Powheg', 'ttbar_{ind}'.format(ind = indx), r.itBkg,633)

    # now get systematic variations from nominal
    for s in (systlist + ',JER').split(','):
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, sys = syst, ind = indx))
        if not tfile:
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        nom   = tfile.Get('ttbar_{ind}'.format(ind = indx))
        if s == 'JES' and uncorrelate:
            nomUp = tfile.Get('ttbar_{ind}_{sys}{ind}Up'.format(ind = indx, sys = s))
            nomDn = tfile.Get('ttbar_{ind}_{sys}{ind}Down'.format(ind = indx, sys = s))
        else:
            nomUp = tfile.Get('ttbar_{ind}_{sys}Up'.format(ind = indx, sys = s))
            nomDn = tfile.Get('ttbar_{ind}_{sys}Down'.format(ind = indx, sys = s))

        # remove stats just in case
        for i in range(nom.GetNbinsX()):
            nom  .SetBinError(i+1,0.)
            nomUp.SetBinError(i+1,0.)
            nomDn.SetBinError(i+1,0.)

        nomUp.Divide(nom)
        nomDn.Divide(nom)

        histoUp = histo.CloneHisto()
        histoDn = histo.CloneHisto()

        histoUp.Multiply(nomUp)
        histoDn.Multiply(nomDn)

        p.PrepareHisto( histoUp, "TTbar_Powheg", 'ttbar_{ind}'.format(ind = indx), r.itSys, 0, s+"Up")
        p.PrepareHisto( histoDn, "TTbar_Powheg", 'ttbar_{ind}'.format(ind = indx), r.itSys, 0, s+"Down")

        p.AddToSystematicLabels(s)
        del nom, nomUp, nomDn, histoUp, histoDn
        tfile.Close()
    
    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTGJets",                     "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",     "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M5to50_MLM",       "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M50_MLM",          "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not vl.asimov:
        p.AddSample("MuonEG",                  "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleMuon",              "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleElec",              "Data",                              r.itData, 0, '', opts)
    else:
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, ind = indx))
        if not tfile:
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        hData = r.Histo( tfile.Get('data_obs') )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92)
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx));
    p.SaveHistograms();
    del p
    del pdf
    del histo
    if vl.asimov:
        del hData
        tfile.Close()
    
    if uncorrelate:
        # Uncorrelation things
        tfile = r.TFile.Open("temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root".format(var = varName, sys = syst, ind = indx), 'update')
        for key in tfile.GetListOfKeys():
            if 'JES' in key.GetName() and 'JES%i'%indx not in key.GetName(): key.SetName(key.GetName().replace('JES', 'JES%i'%indx))
        tfile.Close()
    
    if uncorrelate:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist.replace('JES', 'JES%i'%indx) + ', JER', "ElMu_%d"%indx) #For uncorrelation
    else:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx) # For correlation
    card.SetRootFileName('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}'.format(var = varName, sys = syst, ind = indx))
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx      , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx)   , 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx))
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out     = ''
    toelimi = False
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine:
            numproc = nuLine.split()
            procind = [i for i,x in enumerate(numproc) if x == '-'][0]
            toelimi = True
            wr.warn('There are yields of (at least) one group of processes of variable {var} when considering the systematic {sys} and the bin {ind} that are zero. This process will be removed to avoid problems in the fit.'.format(var = varName, sys = syst, ind = indx), UserWarning, stacklevel=2)
            break
        out = out + nuLine
    
    datacarta.close()   # Because of reasons, after using one time the readlines() method, the file must be reoponed in order to use it again.
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    
    if toelimi:
        out     = ''
        nomproc = None
        nomprocs= None
        for lin in datacarta.readlines():
            nuLine = lin
            if 'bin' in nuLine and len(nuLine.split()) > 2: nuLine = ' '.join(nuLine.split()[:-1])
            if 'process' in nuLine: 
                if nomproc == None: 
                    nomproc = nuLine.split()[procind]
                    nomprocs= nuLine.split()[1:]
                nuLine = nuLine.replace('-1', '-%d'%indx).split(' ')
                del nuLine[procind]
                nuLine = '\n'*('tW' in nuLine[-2])+' '.join(nuLine)
            elif 'rate' in nuLine and '-' in nuLine:
                nuLine = nuLine.replace('- ', '')
            elif '#' not in nuLine and 'autoMCStats' not in nuLine and nomproc != None and ('shape' in nuLine or 'Lumi' in nuLine or (i in nuLine for i in nomprocs)):
                nuLine = nuLine.split(' ')
                del nuLine[procind + 1]
                nuLine = ' '.join(nuLine)
            
            if nomproc != None and nomproc in nuLine:
                continue
            else:
                out = out + nuLine
       
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'w')
    outCarta.write(out)
    outCarta.close()
    return


def getGoodCardsPDFandME(task):
    binDn, binUp, indx, syst = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.SetLumi(vl.Lumi)
    
    subtasks = []
    subtasks.append((binDn, binUp, indx, syst, ''))
    for s in (systlist + ',JER').replace(' ','').split(','):
        subtasks.append( (binDn, binUp, indx, syst, s + 'Up') )
        if s != 'JER': subtasks.append( (binDn, binUp, indx, syst, s + 'Down') )
    
    goodresults = {}
    pool    = Pool(nCores)
    results = pool.map(GiveMeAPDForMEunc, subtasks)
    pool.close()
    pool.join()
    del pool
    
    for i in results:
        goodresults[i[1]] = i[0]
    
    p.PrepareHisto(goodresults[''],'TTbar_Powheg','ttbar_{ind}'.format(ind = indx), r.itBkg, 633)
    for key in goodresults:
        if key == '': continue
        p.PrepareHisto(goodresults[key],'TTbar_Powheg','ttbar_{ind}'.format(ind = indx), r.itSys, 0, key)
    
    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTGJets",                     "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",     "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M5to50_MLM",       "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M50_MLM",          "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not vl.asimov:
        p.AddSample("MuonEG",                  "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleMuon",              "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleElec",              "Data",                              r.itData, 0, '', opts)
    else:
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, ind = indx))
        if not tfile:
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        hData = r.Histo( tfile.Get('data_obs') )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92)
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx));
    p.SaveHistograms();
    del p
    if vl.asimov:
        del hData
        tfile.Close()
    
    if uncorrelate:
        # Uncorrelation things
        tfile = r.TFile.Open("temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root".format(var = varName, sys = syst, ind = indx), 'update')
        for key in tfile.GetListOfKeys():
            if 'JES' in key.GetName() and 'JES%i'%indx not in key.GetName(): key.SetName(key.GetName().replace('JES', 'JES%i'%indx))
        tfile.Close()
    
    if uncorrelate:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist.replace('JES', 'JES%i'%indx) + ', JER', "ElMu_%d"%indx) #For uncorrelation
    else:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx) # For correlation
    card.SetRootFileName('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}'.format(var = varName, sys = syst, ind = indx))
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx      , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx)   , 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx))
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out     = ''
    toelimi = False
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine:
            numproc = nuLine.split()
            procind = [i for i,x in enumerate(numproc) if x == '-'][0]
            toelimi = True
            wr.warn('There are yields of (at least) one group of processes of variable {var} when considering the systematic {sys} and the bin {ind} that are zero. This process will be removed to avoid problems in the fit.'.format(var = varName, sys = syst, ind = indx), UserWarning, stacklevel=2)
            break
        out = out + nuLine
    
    datacarta.close()   # Because of reasons, after using one time the readlines() method, the file must be reoponed in order to use it again.
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    
    if toelimi:
        out     = ''
        nomproc = None
        nomprocs= None
        for lin in datacarta.readlines():
            nuLine = lin
            if 'bin' in nuLine and len(nuLine.split()) > 2: nuLine = ' '.join(nuLine.split()[:-1])
            if 'process' in nuLine: 
                if nomproc == None: 
                    nomproc = nuLine.split()[procind]
                    nomprocs= nuLine.split()[1:]
                nuLine = nuLine.replace('-1', '-%d'%indx).split(' ')
                del nuLine[procind]
                nuLine = '\n'*('tW' in nuLine[-2])+' '.join(nuLine)
            elif 'rate' in nuLine and '-' in nuLine:
                nuLine = nuLine.replace('- ', '')
            elif '#' not in nuLine and 'autoMCStats' not in nuLine and nomproc != None and ('shape' in nuLine or 'Lumi' in nuLine or (i in nuLine for i in nomprocs)):
                nuLine = nuLine.split(' ')
                del nuLine[procind + 1]
                nuLine = ' '.join(nuLine)
            
            if nomproc != None and nomproc in nuLine:
                continue
            else:
                out = out + nuLine
       
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'w')
    outCarta.write(out)
    outCarta.close()
    return


def GiveMeAPDForMEunc(subtask):
    binDn, binUp, indx, syst, syst2 = subtask
    pdf = None
    del pdf
    if syst2 == '':
        pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), r.TString('theBDt_bin{ind}( TBDT )'.format(ind = indx)), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    elif 'JES' in syst2 or 'JER' in syst2:
        pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), r.TString('theBDt_bin{ind}( TBDT{sys} )'.format(ind = indx, sys = syst2)), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    else:
        pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})/TWeight*TWeight_{sys}'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp, sys = syst2)), r.TString('ElMu'), r.TString('theBDt_bin{ind}( TBDT )'.format(ind = indx)), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    
    pdf.SetLumi(vl.Lumi * 1000)
    pdf.verbose = False
    histo       = None
    del histo
    nam         = 'ttbar_{ind}'.format(ind = indx) + (syst2 != '') * '_{sys}'.format(sys = syst2)
    
    if 'pdfUp' == syst:
        histo = copy.deepcopy(pdf.GetSystHisto("up","pdf").CloneHisto(nam))
    elif 'pdfDown' == syst:
        histo = copy.deepcopy(pdf.GetSystHisto("Down","pdf").CloneHisto(nam))
    elif 'ttbarMEUp' == syst:
        histo = copy.deepcopy(pdf.GetSystHisto("up","ME").CloneHisto(nam))
    elif 'ttbarMEDown' == syst:
        histo = copy.deepcopy(pdf.GetSystHisto("Down","ME").CloneHisto(nam))
    else:
        raise RuntimeError("Systematic %s is not of 'pdf' type"%syst)
    del pdf
    
    histo.SetProcess("ttbar_{ind}".format(ind = indx))
    if syst2 == '': histo.SetType(r.itBkg)
    else:           histo.SetType(r.itSys)
    histo.SetLineColor(1)
    histo.SetLineWidth(1)
    histo.SetFillColor(633)
    return [histo, syst2, syst, indx]


def GiveMeTheUltimatePDFandMECards(task):
    binDn, binUp, indx, syst, dictwithvariations = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && {var} >= {bd}  && {var} < {bu})'.format(var = vl.varList[varName]['var'], bd = binDn, bu = binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.SetLumi(vl.Lumi)
    
    p.PrepareHisto(dictwithvariations[''],'TTbar_Powheg','ttbar_{ind}'.format(ind = indx), r.itBkg, 633)
    for key in dictwithvariations:
        if key == '': continue
        p.PrepareHisto(dictwithvariations[key],'TTbar_Powheg','ttbar_{ind}'.format(ind = indx), r.itSys, 0, key)
    
    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,                  r.itBkg, 413, systlist, opts)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist, opts)
    p.AddSample("TTGJets",                     "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist, opts)

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO", "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",     "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M5to50_MLM",       "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)
    p.AddSample("DYJetsToLL_M50_MLM",          "DY_%d"%indx,                        r.itBkg, 852, systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itBkg, r.TColor.GetColor("#ffcc33"), systlist, opts)

    p.AddSample("TW",                          "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSample("TbarW",                       "tW_%d"%indx,                        r.itSys, 1, "JERUp", opts)
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
    
    if not vl.asimov:
        p.AddSample("MuonEG",                  "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleMuon",              "Data",                              r.itData, 0, '', opts)
        p.AddSample("SingleElec",              "Data",                              r.itData, 0, '', opts)
    else:
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_{var}_{ind}.root".format(var = varName, ind = indx))
        if not tfile:
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        hData = r.Histo( tfile.Get('data_obs') )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92)
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx));
    p.SaveHistograms();
    del p
    if vl.asimov:
        del hData
        tfile.Close()
    
    if uncorrelate:
        # Uncorrelation things
        tfile = r.TFile.Open("temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root".format(var = varName, sys = syst, ind = indx), 'update')
        for key in tfile.GetListOfKeys():
            if 'JES' in key.GetName() and 'JES%i'%indx not in key.GetName(): key.SetName(key.GetName().replace('JES', 'JES%i'%indx))
        tfile.Close()
    
    if uncorrelate:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist.replace('JES', 'JES%i'%indx) + ', JER', "ElMu_%d"%indx) #For uncorrelation
    else:
        card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx) # For correlation
    
    card.SetRootFileName('temp/{var}_{sys}/forCards_{var}_{sys}_{ind}.root'.format(var = varName, sys = syst, ind = indx))
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx      , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx)   , 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_{var}_{sys}_{ind}".format(var = varName, sys = syst, ind = indx))
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out     = ''
    toelimi = False
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine:
            numproc = nuLine.split()
            procind = [i for i,x in enumerate(numproc) if x == '-'][0]
            toelimi = True
            wr.warn('There are yields of (at least) one group of processes of variable {var} when considering the systematic {sys} and the bin {ind} that are zero. This process will be removed to avoid problems in the fit.'.format(var = varName, sys = syst, ind = indx), UserWarning, stacklevel=2)
            break
        out = out + nuLine
    
    datacarta.close()   # Because of reasons, after using one time the readlines() method, the file must be reoponed in order to use it again.
    datacarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'r')
    
    if toelimi:
        out     = ''
        nomproc = None
        nomprocs= None
        for lin in datacarta.readlines():
            nuLine = lin
            if 'bin' in nuLine and len(nuLine.split()) > 2: nuLine = ' '.join(nuLine.split()[:-1])
            if 'process' in nuLine: 
                if nomproc == None: 
                    nomproc = nuLine.split()[procind]
                    nomprocs= nuLine.split()[1:]
                nuLine = nuLine.replace('-1', '-%d'%indx).split(' ')
                del nuLine[procind]
                nuLine = '\n'*('tW' in nuLine[-2])+' '.join(nuLine)
            elif 'rate' in nuLine and '-' in nuLine:
                nuLine = nuLine.replace('- ', '')
            elif '#' not in nuLine and 'autoMCStats' not in nuLine and nomproc != None and ('shape' in nuLine or 'Lumi' in nuLine or (i in nuLine for i in nomprocs)):
                nuLine = nuLine.split(' ')
                del nuLine[procind + 1]
                nuLine = ' '.join(nuLine)
            
            if nomproc != None and nomproc in nuLine:
                continue
            else:
                out = out + nuLine
       
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_{var}_{sys}_{ind}.txt'.format(var = varName, sys = syst, ind = indx), 'w')
    outCarta.write(out)
    outCarta.close()
    return



if __name__ == '__main__':
    vl.SetUpWarnings()
    print "> Beginning to produce histograms", "\n"
    indx        = 0
    binning     = vl.varList[varName]['recobinning']
    tasks       = []
    uncorrelate = False
    opts        = ''
    print '> Creating nominal rootfiles with histograms and datacards'
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        tasks.append( (binDn, binUp, indx) )

    pool    = Pool(nCores)
    pool.map(getCardsNominal, tasks)
    pool.close()
    pool.join()
    del pool
    
    print '> Creating rootfiles with histograms and datacards for all systematics'
    tasksSyst = []
    indx    = 0
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        for syst in vl.systMap:
            tasksSyst.append( (binDn, binUp, indx, syst) )
    
    pool    = Pool(nCores)
    pool.map(getCardsSyst, tasksSyst)
    pool.close()
    pool.join()
    del pool
    
    # Approximate procedure:
    #tasksPDF = []
    #indx     = 0
    #for binDn,binUp in zip(binning, binning[1:]):
        #indx = indx+1
        #tasksPDF.append( (binDn, binUp, indx, 'pdfUp'      ) )
        #tasksPDF.append( (binDn, binUp, indx, 'ttbarMEUp'  ) )
        #tasksPDF.append( (binDn, binUp, indx, 'pdfDown'    ) )
        #tasksPDF.append( (binDn, binUp, indx, 'ttbarMEDown') )
    #pool    = Pool(nCores)
    #pool.map(getCardsPdf, tasksPDF)
    #pool.close()
    #pool.join()
    #del pool
    
    # Exact procedure:
    #tasksPDF = []
    #indx     = 0
    #for binDn,binUp in zip(binning, binning[1:]):
        #indx = indx+1
        #if indx != 1: continue
        #tasksPDF.append( (binDn, binUp, indx, 'pdfUp'      ) )
        #tasksPDF.append( (binDn, binUp, indx, 'ttbarMEUp'  ) )
        #tasksPDF.append( (binDn, binUp, indx, 'pdfDown'    ) )
        #tasksPDF.append( (binDn, binUp, indx, 'ttbarMEDown') )
    #for i in tasksPDF:
        #getGoodCardsPDFandME(i)
    
    # Exact and ultimately faster procedure:
    tasksPDF = []
    PDFsysts = ['pdfUp', 'ttbarMEUp', 'pdfDown', 'ttbarMEDown']
    indx     = 0
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx + 1
        for subsyst in PDFsysts:
            tasksPDF.append((binDn, binUp, indx, subsyst, ''))
            for s in (systlist + ',JER').replace(' ','').split(','):
                tasksPDF.append( (binDn, binUp, indx, subsyst, s + 'Up') )
                if s != 'JER': tasksPDF.append( (binDn, binUp, indx, subsyst, s + 'Down') )
    
    print '\n > Beginning large execution'
    midpoint = int(round(len(tasksPDF)/2,0))
    
    pool    = Pool(nCores)
    results = pool.map(GiveMeAPDForMEunc, tasksPDF[:midpoint]) # Do the large calculation
    pool.close()
    pool.join()
    del pool
    
    pool    = Pool(nCores)
    results = results + pool.map(GiveMeAPDForMEunc, tasksPDF[midpoint:])
    pool.close()
    pool.join()
    del pool
    
    print '\n > Collecting results'
    goodresults = {}
    for syst in PDFsysts:
        goodresults[syst] = {}
        for ind in range(1, len(binning)+1):
            goodresults[syst][ind] = {}
    for item in results:
        goodresults[item[2]][item[3]][item[1]] = item[0] # Order: 1-syst, 2-bin, 3-subsyst
    
    print '\n > Obtaining cards'
    finaltasksPDF = []
    indx     = 0
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        for syst in PDFsysts:
            finaltasksPDF.append((binDn, binUp, indx, syst, goodresults[syst][indx]))
    
    pool    = Pool(nCores)
    pool.map(GiveMeTheUltimatePDFandMECards, finaltasksPDF) # Do the actual cards
    pool.close()
    pool.join()
    del pool
    
    print "> Done!", "\n"
