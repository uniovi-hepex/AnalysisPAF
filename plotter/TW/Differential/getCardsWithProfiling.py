import ROOT
import varList
import sys, os
from multiprocessing import Pool
print "===== BDT's histograms procedure with systs. profiling\n"

storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
pathToTree  = ""
NameOfTree = "Mini1j1t";


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
        if sys.argv[2] == 'last':
            pathToTree    = varList.GetLastFolder(storagepath)
        else:
            pathToTree    = storagepath + sys.argv[3] + "/"
    else:
        pathToTree  = "../../../TW_temp/"
    print "> Minitrees will be read from:", pathToTree, "\n"
else:
    print "> Default choice of variable and minitrees\n"
    varName     = 'LeadingLepEta'
    pathToTree  = "../../../TW_temp/"


ROOT.gROOT.SetBatch(True)
ROOT.gROOT.LoadMacro('../../Histo.C+')
ROOT.gROOT.LoadMacro('../../Looper.C+')
ROOT.gROOT.LoadMacro('../../Plot.C+')
ROOT.gROOT.LoadMacro('../../PlotToPy.C+')
ROOT.gROOT.LoadMacro('../../Datacard.C+')
ROOT.gROOT.LoadMacro('../../PDFunc.C+')

ROOT.gROOT.LoadMacro('temp/' + varName + '.C+')
print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"


def getCardsNominal(task):
    binDn, binUp, indx, asimov = task

    p = ROOT.PlotToPy(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), varList.nBinsInBDT, ROOT.Double(0.5), ROOT.Double(varList.nBinsInBDT + 0.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString('BDT disc. (bin %s)'%(str(indx))))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/");
    p.SetPathSignal(pathToTree);
    
    p.AddSample("TW",                           "tW_%d"%indx,    ROOT.itBkg, ROOT.TColor.GetColor("#ffcc33"))
    p.AddSample("TbarW",                        "tW_%d"%indx,    ROOT.itBkg);
    
    p.AddSample("TTbar_PowhegSemi",             "Fakes_%d"%indx, ROOT.itBkg, 413)
    p.AddSample("WJetsToLNu_MLM",               "Fakes_%d"%indx, ROOT.itBkg)
    
    p.AddSample("WZ",                           "VVttV_%d"%indx, ROOT.itBkg, 390);
    p.AddSample("WW",                           "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("ZZ",                           "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToLNu",                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToQQ" ,                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToQQ" ,                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToLLNuNu",                  "VVttV_%d"%indx, ROOT.itBkg);

    p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,    ROOT.itBkg, 852)
    p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,    ROOT.itBkg);
    
    p.AddSample("TTbar_Powheg",                 "ttbar_%d"%indx, ROOT.itBkg, 633)
    
    p.AddSample("TW",                           "tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSample("TbarW",                        "tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSample("TTbar_Powheg",                 "ttbar_%d"%indx, ROOT.itSys, 1, "JERUp");
    p.AddSymmetricHisto("ttbar_%d"%indx,  "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");

    p.AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig")

    if not asimov:
        p.AddSample("MuonEG",                       "Data",  ROOT.itData);
        p.AddSample("SingleMuon",                   "Data",  ROOT.itData);
        p.AddSample("SingleElec",                   "Data",  ROOT.itData);
    else:
        hData=ROOT.Histo(p.GetHisto('tW_%d'%indx).Clone("Data"))
        for proc in ['ttbar_%d'%indx,"VVttV_%d"%indx, "DY_%d"%indx, "Fakes_%d"%indx]:
            hData.Add( p.GetHisto(proc) )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(ROOT.itData)
        hData.SetColor(ROOT.kBlack)
        p.AddToHistos(hData)
    
    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/Cardplots/");
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    p.SetTitleY(ROOT.TString(varList.varList[varName]['yaxis']))
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.DrawStack();
    p.SetOutputName("forCards_" + varName + '_%d'%indx);
    p.SaveHistograms();
    del p
    
    card = ROOT.Datacard('tW_%d'%indx, 'ttbar_{idx},DY_{idx},VVttV_{idx},Fakes_{idx}'.format(idx=indx) , "JES, Btag, Mistag, PU, ElecEff, MuonEff, Trig, JER", "ElMu_%d"%indx)
    card.SetRootFileName('temp/forCards_' + varName + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Fakes_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc("VVttV_%d"%indx, 1.50);
    card.SetNormUnc("ttbar_%d"%indx, 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/datacard_" + varName + '_%d'%indx);
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/datacard_' + varName + '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine: nuLine = nuLine.replace('-', '0')
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/datacard_' + varName + '_%d.txt'%indx,'w')
    outCarta.write(out)


def getCardsSyst(task):
    binDn, binUp, indx, asimov, syst = task

    p = ROOT.PlotToPy(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), varList.nBinsInBDT, ROOT.Double(0.5), ROOT.Double(varList.nBinsInBDT+0.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/");
    p.SetPathSignal(pathToTree);
    
    p.AddSample(varList.systMap[syst]["TW"],           "tW_%d"%indx,    ROOT.itBkg, ROOT.TColor.GetColor("#ffcc33"))
    p.AddSample(varList.systMap[syst]["TbarW"],        "tW_%d"%indx,    ROOT.itBkg);
    
    p.AddSample("TTbar_PowhegSemi",                    "Fakes_%d"%indx, ROOT.itBkg, 413)
    p.AddSample("WJetsToLNu_MLM",                      "Fakes_%d"%indx, ROOT.itBkg)
    
    p.AddSample("WZ",                                  "VVttV_%d"%indx, ROOT.itBkg, 390);
    p.AddSample("WW",                                  "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("ZZ",                                  "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToLNu",                            "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToQQ" ,                            "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToQQ" ,                            "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToLLNuNu",                         "VVttV_%d"%indx, ROOT.itBkg);

    p.AddSample("DYJetsToLL_M10to50_aMCatNLO",         "DY_%d"%indx,    ROOT.itBkg, 852)
    p.AddSample("DYJetsToLL_M50_aMCatNLO",             "DY_%d"%indx,    ROOT.itBkg);
    
    p.AddSample(varList.systMap[syst]["TTbar_Powheg"], "ttbar_%d"%indx, ROOT.itBkg, 633 , '')
    
    p.AddSample(varList.systMap[syst]["TW"],           "tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSample(varList.systMap[syst]["TbarW"],        "tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSample(varList.systMap[syst]["TTbar_Powheg"], "ttbar_%d"%indx, ROOT.itSys, 1, "JERUp");
    p.AddSymmetricHisto("ttbar_%d"%indx,  "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");
    
    p.AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig")

    if not asimov:
        p.AddSample("MuonEG",                       "Data",  ROOT.itData);
        p.AddSample("SingleMuon",                   "Data",  ROOT.itData);
        p.AddSample("SingleElec",                   "Data",  ROOT.itData);
    else: 
        # get asimov from the nominal one
        tfile = ROOT.TFile.Open("temp/forCards_" + varName + '_%d.root'%indx)
        if not tfile:
            raise RuntimeError("No nominal card in place")
        hData = ROOT.Histo( tfile.Get('data_obs') )
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(ROOT.itData)
        hData.SetColor(ROOT.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    
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
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.SaveHistograms();
    del p
    
    card = ROOT.Datacard('tW_%d'%indx, 'ttbar_{idx},DY_{idx},VVttV_{idx},Fakes_{idx}'.format(idx=indx) , "JES, Btag, Mistag, PU, ElecEff, MuonEff, Trig, JER", "ElMu_%d"%indx)
    card.SetRootFileName('temp/forCards_' + varName  + '_' + syst  + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Fakes_%d"%indx, 1.50)
    card.SetNormUnc("DY_%d"%indx   , 1.50)
    card.SetNormUnc("VVttV_%d"%indx, 1.50);
    card.SetNormUnc("ttbar_%d"%indx, 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/datacard_" + varName + '_' + syst + '_%d'%indx);
    del card
    
    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/datacard_' + varName + '_' + syst +  '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        if 'rate' in nuLine and '-' in nuLine: nuLine = nuLine.replace('-', '0')
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/datacard_' + varName + '_' + syst + '_%d.txt'%indx,'w')
    outCarta.write(out)


def getCardsPdf(task):

    binDn, binUp, indx, asimov, syst = task

    p = ROOT.PlotToPy(ROOT.TString('theBDt_bin%d( TBDT )'%indx), ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), varList.nBinsInBDT, ROOT.Double(0.5), ROOT.Double(varList.nBinsInBDT+0.5), ROOT.TString(varName + '_%d'%indx), ROOT.TString(''))
    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    p.SetLimitFolder("temp/");
    p.SetPathSignal(pathToTree);
    
    p.AddSample("WZ",                           "VVttV_%d"%indx, ROOT.itBkg, 390);
    p.AddSample("WW",                           "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("ZZ",                           "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToLNu",                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTWToQQ" ,                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToQQ" ,                     "VVttV_%d"%indx, ROOT.itBkg);
    p.AddSample("TTZToLLNuNu",                  "VVttV_%d"%indx, ROOT.itBkg);

    p.AddSample("DYJetsToLL_M10to50_aMCatNLO",  "DY_%d"%indx,    ROOT.itBkg, 852)
    p.AddSample("DYJetsToLL_M50_aMCatNLO",      "DY_%d"%indx,    ROOT.itBkg);
    p.AddSample("TW",                           "tW_%d"%indx,    ROOT.itBkg, 2)
    p.AddSample("TbarW",                        "tW_%d"%indx,    ROOT.itBkg);
    p.AddSample("TTbar_PowhegSemi",             "Fakes_%d"%indx, ROOT.itBkg, 413)
    p.AddSample("WJetsToLNu_MLM",               "Fakes_%d"%indx, ROOT.itBkg)

    p.AddSample("TW",   "tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSample("TbarW","tW_%d"%indx,    ROOT.itSys, 1, "JERUp");
    p.AddSymmetricHisto("tW_%d"%indx,  "JERUp");

    p.AddSystematic("JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig")

    pdf = ROOT.PDFToPy(pathToTree, "TTbar_Powheg", NameOfTree, ROOT.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && T%s >= %4.2f  && T%s < %4.2f )'%(varName, binDn, varName, binUp)), ROOT.TString('ElMu'), ROOT.TString('theBDt_bin%d( TBDT )'%indx), varList.nBinsInBDT,ROOT.Double(0.5), ROOT.Double(varList.nBinsInBDT+0.5))

    if 'pdfUp'==syst: 
        histo = pdf.GetSystHisto("up","pdf").CloneHisto()
    elif 'pdfDown'==syst: 
        histo = pdf.GetSystHisto("Down","pdf").CloneHisto()
    elif 'MEUp'==syst: 
        histo = pdf.GetSystHisto("up","ME").CloneHisto()
    elif 'MEDown'==syst: 
        histo = pdf.GetSystHisto("Down","ME").CloneHisto()
    else:
        raise RuntimeError("Systematic %s is not of 'pdf' type"%syst)

    p.PrepareHisto(histo, 'TTbar_Powheg', 'ttbar_%d'%indx, ROOT.itBkg,633)

    # now get systematic variations from nominal
    for s in 'JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig,JER'.split(','):
        tfile = ROOT.TFile.Open("temp/forCards_" + varName + '_%d.root'%indx)
        nom   = tfile.Get('ttbar_%d'%indx)
        nomUp = tfile.Get('ttbar_%d_%sUp'%(indx,s))
        nomDn = tfile.Get('ttbar_%d_%sDown'%(indx,s))

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

        p.PrepareHisto( histoUp, "TTbar_Powheg", 'ttbar_%d'%indx, ROOT.itSys, 0, s+"Up")
        p.PrepareHisto( histoDn, "TTbar_Powheg", 'ttbar_%d'%indx, ROOT.itSys, 0, s+"Down")

        p.AddToSystematicLabels(s)

    if not asimov:
        p.AddSample("MuonEG",                       "Data",  ROOT.itData);
        p.AddSample("SingleMuon",                   "Data",  ROOT.itData);
        p.AddSample("SingleElec",                   "Data",  ROOT.itData);
    else: 
        # get asimov from the nominal one
        tfile = ROOT.TFile.Open("temp/forCards_" + varName + '_%d.root'%indx)
        if not tfile: 
            raise RuntimeError("No nominal card in place")
        hData = ROOT.Histo( tfile.Get('data_obs') ) 
        hData.SetProcess("Data")
        hData.SetTag("Data")
        hData.SetType(ROOT.itData)
        hData.SetColor(ROOT.kBlack)
        p.AddToHistos(hData)

    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    
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
    p.SetOutputName("forCards_" + varName + '_' + syst + '_%d'%indx);
    p.SaveHistograms();
    del p

    card = ROOT.Datacard('tW_%d'%indx, 'ttbar_{idx},DY_{idx},VVttV_{idx},Fakes_{idx}'.format(idx=indx) , "JES, Btag, Mistag, PU, ElecEff, MuonEff, Trig, JER", "ElMu_%d"%indx)
    card.SetRootFileName('temp/forCards_' + varName  + '_' + syst  + '_%d'%indx)
    card.GetParamsFormFile()
    card.SetNormUnc("Fakes_%d"%indx   , 1.50)
    card.SetNormUnc("DY_%d"%indx      , 1.50)
    card.SetNormUnc("VVttV_%d"%indx   , 1.50);
    card.SetNormUnc("ttbar_%d"%indx   , 1.06);
    card.SetLumiUnc(1.025)
    card.PrintDatacard("temp/datacard_" + varName + '_' + syst + '_%d'%indx);
    del card

    # All this crap so i dont have to tamper with the DataCard.C
    out = ''
    datacarta = open('temp/datacard_' + varName + '_' + syst +  '_%d.txt'%indx,'r')
    for lin in datacarta.readlines():
        nuLine = lin
        if 'process' in nuLine: nuLine = nuLine.replace('-1', '-%d'%indx)
        out = out + nuLine
    datacarta.close()
    outCarta = open('temp/datacard_' + varName + '_' + syst + '_%d.txt'%indx,'w')
    outCarta.write(out)


if __name__ == '__main__':
    indx    = 0
    binning = varList.varList[varName]['recobinning']
    print "> Beginning to produce histograms", "\n"
    tasks   = []
    asimov  = True
    print '> Creating nominal rootfiles with histograms and datacards'
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        tasks.append( (binDn, binUp, indx, asimov) )

    pool    = Pool(nCores)
    pool.map(getCardsNominal,tasks)
    pool.close()
    pool.join()
    del pool
    
    print '> Creating rootfiles with histograms and datacards for all systematics'
    tasksSyst = []
    indx    = 0
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        for syst in varList.systMap:
            if 'pdf' in syst or 'ME' in syst: continue # these boys are handled differently
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
        tasksPdf.append( (binDn, binUp, indx, asimov, 'MEUp') )
        tasksPdf.append( (binDn, binUp, indx, asimov, 'pdfDown') )
        tasksPdf.append( (binDn, binUp, indx, asimov, 'MEDown') )

    pool    = Pool(nCores)
    pool.map(getCardsPdf, tasksPdf)
    pool.close()
    pool.join()


    print "> Done!", "\n"
