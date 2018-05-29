import ROOT     as r
import varList  as vl
import sys, os
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

r.gROOT.LoadMacro('./temp/{var}_/'.format(var = varName) + varName + '.C+')
print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"


def getCardsNominal(task):
    binDn, binUp, indx, asimov = task

    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT + 0.5), r.TString(varName + '_%d'%indx), r.TString('BDT disc. (bin %s)'%(str(indx))))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder('./temp/{var}_/'.format(var = varName));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    
    p.AddSample("TW",                           "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample("TbarW",                        "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist);
    
    p.AddSample("TTbar_PowhegSemi",             "Non-W|Z",      r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",               "Non-W|Z",      r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                           "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("WW",                           "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("ZZ",                           "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",                     "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,                     "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,                     "VV+t#bar{t}V", r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",                  "VV+t#bar{t}V", r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY",           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM",        "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",           "DY",           r.itBkg, 852, systlist);
    
    p.AddSample("TTbar_Powheg",                 "t#bar{t}",     r.itBkg, 633, systlist)
    
    p.AddSample("TW",                           "tW",           r.itSys, 1, "JERUp");
    p.AddSample("TbarW",                        "tW",           r.itSys, 1, "JERUp");
    p.AddSample("TTbar_Powheg",                 "t#bar{t}",     r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("t#bar{t}",  "JERUp");
    p.AddSymmetricHisto("tW",  "JERUp");

    #p.AddSystematic(systlist)

    if not asimov:
        p.AddSample("MuonEG",                       "Data",             r.itData);
        p.AddSample("SingleMuon",                   "Data",             r.itData);
        p.AddSample("SingleElec",                   "Data",             r.itData);
    else:
        hData=r.Histo(p.GetHisto('tW').Clone("Data"))
        for proc in ['t#bar{t}','VV+t#bar{t}V', "DY", "Non-W|Z"]:
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
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    p.SetTitleY(r.TString(vl.varList[varName]['yaxis']))
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.DrawStack();
    del p
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT + 0.5), r.TString(varName + '_%d'%indx), r.TString('BDT disc. (bin %s)'%(str(indx))))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder('./temp/{var}_/'.format(var = varName));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    
    p.AddSample("TW",                    "tW_%d"%indx,                         r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample("TbarW",                 "tW_%d"%indx,                         r.itBkg, r.TColor.GetColor("#ffcc33"), systlist);
    
    p.AddSample("TTbar_PowhegSemi",      "Non-WorZ_%d"%indx,                   r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",        "Non-WorZ_%d"%indx,                   r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("WW",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("ZZ",                    "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,              "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",           "VVttbarV_{ind}".format(ind = indx), r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM", "DY_%d"%indx,                         r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",    "DY_%d"%indx,                         r.itBkg, 852, systlist);
    
    p.AddSample("TTbar_Powheg",          "ttbar_{ind}".format(ind = indx),     r.itBkg, 633, systlist)
    
    p.AddSample("TW",                    "tW_%d"%indx,                         r.itSys, 1, "JERUp");
    p.AddSample("TbarW",                 "tW_%d"%indx,                         r.itSys, 1, "JERUp");
    p.AddSample("TTbar_Powheg",          "ttbar_{ind}".format(ind = indx),     r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("ttbar_{ind}".format(ind = indx),  "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not asimov:
        p.AddSample("MuonEG",            "Data",                               r.itData);
        p.AddSample("SingleMuon",        "Data",                               r.itData);
        p.AddSample("SingleElec",        "Data",                               r.itData);
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
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    p.SetTitleY(r.TString(vl.varList[varName]['yaxis']))
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.SaveHistograms();
    del p
    
    card = r.Datacard("tW_%d"%indx, "ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}".format(idx=indx), systlist + ', JER', "ElMu_%d"%indx)
    card.SetRootFileName('./temp/{var}_/forCards_'.format(var = varName) + varName + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx), 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_/datacard_".format(var = varName) + varName + '_%d'%indx);
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/{var}_/datacard_'.format(var = varName) + varName + '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine: nuLine = nuLine.replace('-', '0')
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/{var}_/datacard_'.format(var = varName) + varName + '_%d.txt'%indx,'w')
    outCarta.write(out)


def getCardsSyst(task):
    binDn, binUp, indx, asimov, syst = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    
    p.AddSample(vl.systMap[syst]["TW"],           "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample(vl.systMap[syst]["TbarW"],        "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    
    p.AddSample("TTbar_PowhegSemi",            "Non-W|Z",      r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",              "Non-W|Z",      r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("WW",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("ZZ",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",                 'VV+t#bar{t}V', r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY",           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM",        "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",           "DY",           r.itBkg, 852, systlist);
    
    if 'UEUp' in syst:
        specialweight = vl.nUEUp_ttbar/vl.sigma_ttbar/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itSys, 1, "JERUp");
        
        specialweight = vl.nUEUp_dilep/vl.sigma_dilep/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueUp',                 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_ueUp',                 't#bar{t}', r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('t#bar{t}',  "JERUp");
        p.SetWeight('TWeight')
    elif 'UEDown' in syst:
        specialweight = vl.nUEDown_ttbar/vl.sigma_ttbar/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itSys, 1, "JERUp")
        
        specialweight = vl.nUEDown_dilep/vl.sigma_dilep/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueDown',               't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_ueDown',               't#bar{t}', r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('t#bar{t}',  "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampUp' in syst:
        specialweight = vl.nhDampUp_ttbar/vl.sigma_ttbar/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itSys, 1, "JERUp")
        
        specialweight = vl.nhDampUp_dilep/vl.sigma_dilep/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampUp',              't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_hdampUp',              't#bar{t}', r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('t#bar{t}', "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampDown' in syst:
        specialweight = vl.nhDampDown_ttbar/vl.sigma_ttbar/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itSys, 1, "JERUp")
        
        specialweight = vl.nhDampDown_dilep/vl.sigma_dilep/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampDown',            't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_hdampDown',            't#bar{t}', r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('t#bar{t}',  "JERUp");
        p.SetWeight('TWeight')
    else:
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 't#bar{t}', r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('t#bar{t}',  "JERUp");


    p.AddSample(vl.systMap[syst]["TW"],           "tW",    r.itSys, 1, "JERUp");
    p.AddSample(vl.systMap[syst]["TbarW"],        "tW",    r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("tW",  "JERUp");
    
    if not asimov:
        p.AddSample("MuonEG",                       "Data",  r.itData);
        p.AddSample("SingleMuon",                   "Data",  r.itData);
        p.AddSample("SingleElec",                   "Data",  r.itData);
    else: 
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
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
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.DrawStack();
    del p
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    
    p.AddSample(vl.systMap[syst]["TW"],           "tW_%d"%indx,           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample(vl.systMap[syst]["TbarW"],        "tW_%d"%indx,           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    
    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,      r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,      r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM",        "DY_%d"%indx,           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",           "DY_%d"%indx,           r.itBkg, 852, systlist);
    
    if 'UEUp' in syst:
        specialweight = vl.nUEUp_ttbar/vl.sigma_ttbar/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp");
        
        specialweight = vl.nUEUp_dilep/vl.sigma_dilep/(vl.nUEUp_ttbar/vl.sigma_ttbar + vl.nUEUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueUp',                 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_ueUp',                 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'UEDown' in syst:
        specialweight = vl.nUEDown_ttbar/vl.sigma_ttbar/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        
        specialweight = vl.nUEDown_dilep/vl.sigma_dilep/(vl.nUEDown_ttbar/vl.sigma_ttbar + vl.nUEDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_ueDown',               'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_ueDown',               'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampUp' in syst:
        specialweight = vl.nhDampUp_ttbar/vl.sigma_ttbar/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        
        specialweight = vl.nhDampUp_dilep/vl.sigma_dilep/(vl.nhDampUp_ttbar/vl.sigma_ttbar + vl.nhDampUp_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampUp',              'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_hdampUp',              'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    elif 'hDampDown' in syst:
        specialweight = vl.nhDampDown_ttbar/vl.sigma_ttbar/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        
        specialweight = vl.nhDampDown_dilep/vl.sigma_dilep/(vl.nhDampDown_ttbar/vl.sigma_ttbar + vl.nhDampDown_dilep/vl.sigma_dilep)
        p.SetWeight('TWeight*' + str(specialweight))
        p.AddSample('TTbar2L_Powheg_hdampDown',            'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample('TTbar2L_Powheg_hdampDown',            'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");
        p.SetWeight('TWeight')
    else:
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itBkg, 633, systlist)
        p.AddSample(vl.systMap[syst]["TTbar_Powheg"], 'ttbar_{ind}'.format(ind = indx), r.itSys, 1, "JERUp")
        p.AddSymmetricHisto('ttbar_{ind}'.format(ind = indx),  "JERUp");


    p.AddSample(vl.systMap[syst]["TW"],           "tW_%d"%indx,    r.itSys, 1, "JERUp");
    p.AddSample(vl.systMap[syst]["TbarW"],        "tW_%d"%indx,    r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    if not asimov:
        p.AddSample("MuonEG",                       "Data",  r.itData);
        p.AddSample("SingleMuon",                   "Data",  r.itData);
        p.AddSample("SingleElec",                   "Data",  r.itData);
    else: 
        # get asimov from the nominal one"
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
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
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.SaveHistograms();
    del p
    
    card = r.Datacard('tW_%d'%indx, 'ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}'.format(idx=indx) , systlist + ', JER', "ElMu_%d"%indx)
    card.SetRootFileName("temp/{var}_{sys}/forCards_".format(var = varName, sys = syst) + varName  + '_' + syst  + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx), 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_".format(var = varName, sys = syst) + varName + '_' + syst + '_%d'%indx);
    del card
    
    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/{var}_{sys}/datacard_'.format(var = varName, sys = syst) + varName + '_' + syst +  '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine: nuLine = nuLine.replace('-', '0')
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_'.format(var = varName, sys = syst) + varName + '_' + syst + '_%d.txt'%indx,'w')
    outCarta.write(out)


def getCardsPdf(task):
    binDn, binUp, indx, asimov, syst = task
    if not os.path.isdir('temp/{var}_{sys}'.format(var = varName, sys = syst)):
        os.system('mkdir -p temp/{var}_{sys}'.format(var = varName, sys = syst))

    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    p.AddSample("TW",                          "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample("TbarW",                       "tW",           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist);
    
    p.AddSample("TTbar_PowhegSemi",            "Non-W|Z",      r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",              "Non-W|Z",      r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("WW",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("ZZ",                          'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,                    'VV+t#bar{t}V', r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",                 'VV+t#bar{t}V', r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY",           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM",        "DY",           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",           "DY",           r.itBkg, 852, systlist);

    p.AddSample("TW",                          "tW",           r.itSys, 1, "JERUp");
    p.AddSample("TbarW",                       "tW",           r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("tW",  "JERUp");
    
    pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), r.TString('theBDt_bin%d( TBDT )'%indx), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    pdf.verbose = False
    
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

    p.PrepareHisto(histo, 'TTbar_Powheg', 't#bar{t}', r.itBkg,633)

    # now get systematic variations from nominal
    for s in (systlist + ',JER').split(','):
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
        if not tfile: 
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        nom   = tfile.Get('ttbar_{ind}'.format(ind = indx))
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

        p.PrepareHisto( histoUp, "TTbar_Powheg", 't#bar{t}', r.itSys, 0, s+"Up")
        p.PrepareHisto( histoDn, "TTbar_Powheg", 't#bar{t}', r.itSys, 0, s+"Down")

        p.AddToSystematicLabels(s)
        del nom, nomUp, nomDn, histoUp, histoDn
        tfile.Close()

    if not asimov:
        p.AddSample("MuonEG",                       "Data",  r.itData);
        p.AddSample("SingleMuon",                   "Data",  r.itData);
        p.AddSample("SingleElec",                   "Data",  r.itData);
    else: 
        # get asimov from the nominal one
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
        hData = r.Histo( tfile.Get('data_obs') ) 
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)
        del hData
        tfile.Close()

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.DrawStack();
    del p
    del pdf
    del histo
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx), r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), vl.nBinsInBDT, float(0.5), float(vl.nBinsInBDT+0.5), r.TString(varName + '_%d'%indx), r.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/{var}_{sys}/".format(var = varName, sys = syst));
    p.SetPathSignal(pathToTree);
    p.SetTitleY("Events")
    
    p.AddSample("TW",                          "tW_%d"%indx,           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist)
    p.AddSample("TbarW",                       "tW_%d"%indx,           r.itBkg, r.TColor.GetColor("#ffcc33"), systlist);
    
    p.AddSample("TTbar_PowhegSemi",            "Non-WorZ_%d"%indx,      r.itBkg, 413, systlist)
    p.AddSample("WJetsToLNu_MLM",              "Non-WorZ_%d"%indx,      r.itBkg, 413, systlist)
    
    p.AddSample("WZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("WW",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("ZZ",                          'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToLNu",                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTWToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToQQ" ,                    'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);
    p.AddSample("TTZToLLNuNu",                 'VVttbarV_{ind}'.format(ind = indx), r.itBkg, 390, systlist);

    #p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,           r.itBkg, 852, systlist)
    #p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M5to50_MLM",        "DY_%d"%indx,           r.itBkg, 852, systlist);
    p.AddSample("DYJetsToLL_M50_MLM",           "DY_%d"%indx,           r.itBkg, 852, systlist);

    p.AddSample("TW",                          "tW_%d"%indx,           r.itSys, 1, "JERUp");
    p.AddSample("TbarW",                       "tW_%d"%indx,           r.itSys, 1, "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    pdf = r.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), r.TString('theBDt_bin%d( TBDT )'%indx), vl.nBinsInBDT,float(0.5), float(vl.nBinsInBDT+0.5))
    pdf.verbose = False
    
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
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
        if not tfile: 
            raise RuntimeError('Nominal card rootfile for variable {var} has not been found while considering syst. {sys}'.format(var = varName, sys = syst))
        nom   = tfile.Get('ttbar_{ind}'.format(ind = indx))
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

    if not asimov:
        p.AddSample("MuonEG",                       "Data",  r.itData);
        p.AddSample("SingleMuon",                   "Data",  r.itData);
        p.AddSample("SingleElec",                   "Data",  r.itData);
    else: 
        # get asimov from the nominal one
        tfile = r.TFile.Open("temp/{var}_/forCards_".format(var = varName) + varName + '_%d.root'%indx)
        hData = r.Histo( tfile.Get('data_obs') ) 
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(r.itData)
        hData.SetColor(r.kBlack)
        p.AddToHistos(hData)
        del hData
        tfile.Close()

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.SaveHistograms();
    del p
    del pdf
    del histo
    
    card = r.Datacard('tW_%d'%indx, 'ttbar_{idx},DY_{idx},VVttbarV_{idx},Non-WorZ_{idx}'.format(idx=indx) , systlist + ', JER', "ElMu_%d"%indx)
    card.SetRootFileName('temp/{var}_{sys}/forCards_'.format(var = varName, sys = syst) + varName  + '_' + syst  + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Non-WorZ_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx      , 1.50)
    card.SetNormUnc('VVttbarV_{ind}'.format(ind = indx), 1.50);
    card.SetNormUnc('ttbar_{ind}'.format(ind = indx)   , 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/{var}_{sys}/datacard_".format(var = varName, sys = syst) + varName + '_' + syst + '_%d'%indx);
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/{var}_{sys}/datacard_'.format(var = varName, sys = syst) + varName + '_' + syst +  '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/{var}_{sys}/datacard_'.format(var = varName, sys = syst) + varName + '_' + syst + '_%d.txt'%indx,'w')
    outCarta.write(out)


if __name__ == '__main__':
    indx    = 0
    binning = vl.varList[varName]['recobinning']
    print "> Beginning to produce histograms", "\n"
    tasks   = []
    asimov  = False
    print '> Creating nominal rootfiles with histograms and datacards'
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        tasks.append( (binDn, binUp, indx, asimov) )

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
            tasksSyst.append( (binDn, binUp, indx, asimov, syst) )
    
    pool    = Pool(nCores)
    pool.map(getCardsSyst, tasksSyst)
    pool.close()
    pool.join()
    
    tasksPdf = []
    indx = 0
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        tasksPdf.append( (binDn, binUp, indx, asimov, 'pdfUp') )
        tasksPdf.append( (binDn, binUp, indx, asimov, 'ttbarMEUp') )
        tasksPdf.append( (binDn, binUp, indx, asimov, 'pdfDown') )
        tasksPdf.append( (binDn, binUp, indx, asimov, 'ttbarMEDown') )

    pool    = Pool(nCores)
    pool.map(getCardsPdf, tasksPdf)
    pool.close()
    pool.join()


    print "> Done!", "\n"
