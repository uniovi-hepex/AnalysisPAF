import ROOT as r
import varList as vl
import sys, os
from multiprocessing import Pool
r.gROOT.SetBatch(True)
vl.SetUpWarnings()

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

if (len(sys.argv) > 2):
    nCores      = int(sys.argv[2])
    print ('> Parallelization will be done with ' + str(nCores) + ' cores')
else:
    print '> Sequential execution mode chosen'
    nCores      = 1


cardTemp = 'temp/{varr}{sys2}/forCards_{varr}{sys}_{bin}.root'
if not os.path.isdir('results/comparison'):
    os.system('mkdir -p results/comparison')


def GiveMeMyPlots(var):
    histos = { }
    if not os.path.isdir('results/comparison/{varr}'.format(varr = var)):
        os.system('mkdir -p results/comparison/{varr}'.format(varr = var))
    for sys in sysList:
        hist = r.TH1F('h%s'%sys, '', int(vl.nBinsInBDT * (len(vl.varList[var]['recobinning']) - 1)), 0.5, int(vl.nBinsInBDT * (len(vl.varList[var]['recobinning']) - 1)) + 0.5)
        for bin in range(1, len(vl.varList[var]['recobinning'])):
            tfile   = r.TFile.Open(cardTemp.format(varr = var, sys = '_'+sys if (sys != '' and sys != 'tW') else '', bin=bin, sys2 = '_' if (sys == '' or sys == 'tW') else '_'+sys))
            if 'DS' in sys or 'tW' in sys: subHist = tfile.Get('tW_%d'%bin)
            else: subHist = tfile.Get('ttbar_%d'%bin)
            for bin2 in range(1, vl.nBinsInBDT + 1):
                hist.SetBinContent( bin2 + vl.nBinsInBDT*(bin - 1), subHist.GetBinContent(bin2))
            tfile.Close()
        histos[sys] = hist
        if sys == '' or sys == 'tW': continue
        histos[''].SetLineColor(r.kBlack)
        histos['tW'].SetLineColor(r.kBlack)
        r.gStyle.SetOptStat(0)
        c = r.TCanvas()
        
        textSize = 0.022
        legWidth = 0.12
        height = .17
        corner = 'TL'
        if corner == "TR":
            (x1,y1,x2,y2) = (0.97-legWidth if self.doWide else .85-legWidth, .9 - height, .90, .93)
        elif corner == "TC":
            (x1,y1,x2,y2) = (.5, .9 - height, .55+legWidth, .93)
        elif corner == "TL":
            (x1,y1,x2,y2) = (.2, .9 - height, .25+legWidth, .91)
        elif corner == "BR":
            (x1,y1,x2,y2) = (.85-legWidth, .16 + height, .90, .15)
        elif corner == "BC":
            (x1,y1,x2,y2) = (.5, .16 + height, .5+legWidth, .15)
        elif corner == "BL":
            (x1,y1,x2,y2) = (.2, .16 + height, .2+legWidth, .15)
        
        (x1,y1,x2,y2) = (.7, .5, .9, .7)
        leg = r.TLegend(x1,y1,x2,y2)
        leg.SetTextFont(42)
        leg.SetTextSize(textSize)
        leg.SetBorderSize(0)
        leg.SetFillColor(10)
        
        if 'DS' in sys or 'tW' in sys:
            histos['tW'].GetXaxis().SetTitle( vl.varList[var]['xaxis'] )
            histos['tW'].GetYaxis().SetTitle( 'Events' )
            histos['tW'].Draw()
            histos[sys].SetLineColor(r.kRed)
            histos[sys].Draw('same')
            leg.AddEntry( histos['tW'], 'tW nominal', 'L')
            leg.AddEntry( histos[sys], sys, 'L')
            l1 = r.TLatex(0.7, 0.85,'#scale[0.6]{KS test: %4.4f}'%histos['tW'].KolmogorovTest( histos[sys] ))
            l2 = r.TLatex(0.7, 0.8 ,'#scale[0.6]{KS test (toys): %4.4f}'%histos['tW'].KolmogorovTest( histos[sys],'X'))
            l3 = r.TLatex(0.7, 0.75,'#scale[0.6]{#chi^{2} test: %4.4f}'%histos['tW'].Chi2Test( histos[sys],'WW' ))
            #l3 = r.TLatex(0.6, 0.7 ,'#chi^{2} test: %4.2f'%histos['tW'].Chi2Test( histos[sys],'WWCHI2' ))
        else:
            histos[''].GetXaxis().SetTitle( vl.varList[var]['xaxis'] )
            histos[''].GetYaxis().SetTitle( 'Events' )
            histos[''].Draw()
            histos[sys].SetLineColor(r.kRed)
            histos[sys].Draw('same')
            leg.AddEntry( histos[''], 'ttbar nominal', 'L')
            leg.AddEntry( histos[sys], sys, 'L')
            l1 = r.TLatex(0.7, 0.85,'#scale[0.6]{KS test: %4.4f}'%histos[''].KolmogorovTest( histos[sys] ))
            l2 = r.TLatex(0.7, 0.8 ,'#scale[0.6]{KS test (toys): %4.4f}'%histos[''].KolmogorovTest( histos[sys],'X'))
            l3 = r.TLatex(0.7, 0.75,'#scale[0.6]{#chi^{2} test: %4.4f}'%histos[''].Chi2Test( histos[sys],'WW' ))
            #l3 = r.TLatex(0.6, 0.7 ,'#chi^{2} test: %4.2f'%histos[''].Chi2Test( histos[sys],'WWCHI2' ))
        
        l1.SetNDC(True)
        l2.SetNDC(True)
        l3.SetNDC(True)

        l1.Draw('same')
        l2.Draw('same')
        l3.Draw('same')

        leg.Draw('same')
        
        c.SaveAs('results/comparison/{varr}_{syst}.png'.format(varr = var, syst = sys))
        del leg
        del c
    return

tasks   = []

if varName == 'All':
    for var in vl.varList['Names']['Variables']:
        tasks.append( var )
else:
    tasks = [varName]


pool    = Pool(nCores)
pool.map(GiveMeMyPlots, tasks)
pool.close()
pool.join()
del pool

