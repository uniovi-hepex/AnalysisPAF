import ROOT     as r
import varList  as vl
import sys, os, math
from multiprocessing import Pool

storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
pathToTree  = ""
NameOfTree  = "Mini1j1t";
systlist    = "JES,Btag,Mistag,PU,ElecEff,MuonEff,Trig"


def getPostFit(task):
    indx, syst, dire = task
    print '\n > Obtaining post and prefit plots of bin number {ind} of the syst. {sys}'.format(ind = indx + 1, sys = syst)
    tfile = r.TFile.Open('temp/{var}_{syst}/fitdiagnostics/fitDiagnostics{var}_{syst}.root'.format(syst=syst,var=varName))
    print 'temp/{var}_{syst}/fitdiagnostics/fitDiagnostics{var}_{syst}.root'.format(syst=syst,var=varName)
    if not tfile:
        raise RuntimeError("No fitdiagnostics file %s"%'temp/{var}_{syst}/fitdiagnostics/fitDiagnostics{var}_{syst}.root'.format(syst=syst,var=varName))
    tdir   = tfile.Get('shapes_{dire}/ch{indx}'.format(dire=dire,indx=indx))
    tdirda = tfile.Get('shapes_prefit/ch{indx}'.format(dire=dire,indx=indx))
    
    doDY    = False
    dofakes = False
    doother = False
    ttbar  = tdir  .Get('ttbar_%d'%indx    )
    tW     = tdir  .Get('tW_%d'%indx       )
    DY     = tdir  .Get('DY_%d'%indx       )
    fakes  = tdir  .Get('Non-WorZ_%d'%indx )
    other  = tdir  .Get('VVttbarV_%d'%indx )
    gdata  = tdirda.Get('data'             )
    
    if DY:    doDY    = True
    if fakes: dofakes = True
    if other: doother = True
    
    data = r.TH1F('hdata','', tW.GetNbinsX(), tW.GetBinLowEdge(1), tW.GetBinLowEdge( tW.GetNbinsX())+tW.GetBinWidth(tW.GetNbinsX()))
    for i in range(1,vl.nBinsInBDT+1):
        x = r.Double(0)
        y = r.Double(0)
        gdata.GetPoint( i-1, x, y)
        data.SetBinContent(i, y)
        data.SetBinError(i, math.sqrt(y))
    
    p = r.PlotToPy(r.TString('theBDt_bin%d( TBDT )'%indx),
                   r.TString('(TIsSS == 0 && TNJets == 1  && TNBtags == 1 && %s >= %4.2f  && %s < %4.2f )'%(vl.varList[varName]['var'], binDn, vl.varList[varName]['var'], binUp)), r.TString('ElMu'), 
                   tW.GetNbinsX(), tW.GetBinLowEdge(1), tW.GetBinLowEdge( tW.GetNbinsX())+tW.GetBinWidth(tW.GetNbinsX()),
                   r.TString(varName + '_%d'%indx), 
                   r.TString('BDT disc. (bin %s)'%(str(indx))))

    p.SetPath(pathToTree); p.SetTreeName(NameOfTree);
    os.system('mkdir -p ./temp/{var}_/{dire}/'.format(var = varName,dire=dire))
    p.SetLimitFolder('./temp/{var}_/{dire}/'.format(var = varName,dire=dire));
    p.SetTitleY("Events")
    
    httbar = r.Histo(ttbar); httbar.SetProcess('t#bar{t}'    );  httbar.SetTag('t#bar{t}'    );  httbar.SetType( r.itBkg  ); httbar.SetColor( 633 )
    htW    = r.Histo(tW   ); htW   .SetProcess('tW'          );  htW   .SetTag('tW'          );  htW   .SetType( r.itBkg  ); htW   .SetColor( r.TColor.GetColor("#ffcc33") )
    if doDY:    hDY    = r.Histo(DY   ); hDY   .SetProcess('DY'          );  hDY   .SetTag('DY'          );  hDY   .SetType( r.itBkg  ); hDY   .SetColor( 852 )
    if dofakes: hfakes = r.Histo(fakes); hfakes.SetProcess('Non-W|Z'     );  hfakes.SetTag('Non-W|Z'     );  hfakes.SetType( r.itBkg  ); hfakes.SetColor( 413 )
    if doother: hother = r.Histo(other); hother.SetProcess('VV+t#bar{t}V');  hother.SetTag('VV+t#bar{t}V');  hother.SetType( r.itBkg  ); hother.SetColor( 390 )
    hData  = r.Histo(data ); hData .SetProcess('Data'        );  hData .SetTag('Data'        );  hData .SetType( r.itData ); hData .SetColor( r.kBlack )
    
    p.AddToHistos( httbar )
    p.AddToHistos( htW    )
    if dofakes: p.AddToHistos( hfakes )
    if doother: p.AddToHistos( hother )
    if doDY:    p.AddToHistos( hDY    )
    p.AddToHistos( hData  )
    
    p.doUncInLegend = True;
    p.SetRatioMin( 0.6 );
    p.SetRatioMax( 1.4 );
    p.SetPadPlotMargins(vl.margins)
    p.SetPadRatioMargins(vl.marginsratio)
    p.SetLegendPosition('DL')
    p.SetLegendTextSize(0.0275)
    
    p.SetCMSlabel("CMS");
    p.SetCMSmodeLabel("Preliminary");
    #p.SetLegendPosition(0.7, 0.45, 0.93, 0.92);
    p.SetPlotFolder("results/{dire}/".format(dire=dire));
    p.doYieldsInLeg = False;
    p.doSetLogy     = False;
    #p.doData        = False;
    p.doSignal      = False;
    p.SetTitleY('Events')
    
    p.NoShowVarName = True;
    p.SetOutputName("forCards_" + varName + '_' + syst+'_%d'%indx);
    p.DrawStack();
    del p
    
    tfile.Close()
    return



if __name__ == '__main__':
    vl.SetUpWarnings()
    r.gROOT.SetBatch(True)
    verbose = False
    print "===== BDT's histograms procedure with systs. profiling to be obtained after fitting\n"
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
    
    r.gROOT.LoadMacro('../../Histo.C+')
    r.gROOT.LoadMacro('../../Looper.C+')
    r.gROOT.LoadMacro('../../Plot.C+')
    r.gROOT.LoadMacro('../../PlotToPy.C+')
    r.gROOT.LoadMacro('../../Datacard.C+')
    r.gROOT.LoadMacro('../../PDFunc.C+')
    r.gROOT.LoadMacro('./temp/{var}_/'.format(var = varName) + varName + '.C+')
    print '> Succesfully loaded binning information from temp/' + varName + '.C', "\n"

    indx    = 0
    binning = vl.varList[varName]['recobinning']
    tasks   = []
    for binDn,binUp in zip(binning, binning[1:]):
        indx = indx+1
        for ty in ['prefit', 'fit_s']:
            tasks.append( ( indx, '', ty ) )
            for syst in vl.systMap:
                tasks.append( ( indx, syst, ty) )

            tasks.append( (indx, 'pdfUp'      , ty) )
            tasks.append( (indx, 'ttbarMEUp'  , ty) )
            tasks.append( (indx, 'pdfDown'    , ty) )
            tasks.append( (indx, 'ttbarMEDown', ty) )

    print "> Beginning to produce histograms", "\n"
    pool = Pool(nCores)
    pool.map(getPostFit, tasks)
    pool.close()
    pool.join()

    print "> Done!", "\n"
