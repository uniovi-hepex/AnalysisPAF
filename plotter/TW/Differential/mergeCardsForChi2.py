import ROOT as r

sysList = ['','_QCDbasedCRTuneerdON','_PowhegerdON','_GluonMoveCRTuneerdON','_GluonMoveCRTune']

cardTemp = 'temp/LeadingLepPt{sys2}/forCards_LeadingLepPt{sys}_{bin}.root'

histos = { }

for sys in sysList: 
    
    hist = r.TH1F('h%s'%sys,'',80,0.5,79.5)
    print hist

    for bin in range(1,9):
        tfile = r.TFile.Open(cardTemp.format(sys=sys,bin=bin,sys2='_' if sys == '' else sys))
        subHist = tfile.Get('ttbar_%d'%bin)
        for bin2 in range(1,11):
            print bin2 + 10*(bin-1)
            hist.SetBinContent( bin2 + 10*(bin-1), subHist.GetBinContent(bin2))
        tfile.Close()

    histos[sys] = hist


histos[''].SetLineColor(r.kBlack)

r.gStyle.SetOptStat(0)


for sys in sysList:
    c = r.TCanvas()
    if sys == '': continue
    histos[''].Draw()
    histos[sys].SetLineColor(r.kRed)
    histos[sys].Draw('same')

    l1 = r.TLatex(0.6,0.8,'KS test: %4.2f'%histos[''].KolmogorovTest( histos[sys] ))
    l2 = r.TLatex(0.6,0.75,'KS test (toys): %4.2f'%histos[''].KolmogorovTest( histos[sys],'X'))
    l3 = r.TLatex(0.6,0.7,'#chi^{2} test: %4.2f'%histos[''].Chi2Test( histos[sys],'WW' ))
    
    l1.SetNDC(True)    
    l2.SetNDC(True)
    l3.SetNDC(True)

    l1.Draw('same')
    l2.Draw('same')
    l3.Draw('same')

    c.SaveAs('results/crap/%s.pdf'%sys)

    raw_input('press key to continue')
