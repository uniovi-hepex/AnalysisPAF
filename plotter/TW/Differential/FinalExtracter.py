import ROOT as r 
import copy
import errorPropagator as ep
import beautifulUnfoldingPlots as bp
import varList as vl

def getXsecForSys(syst, tfile):
    data = copy.deepcopy(tfile.Get('data_obs').Clone('data_%s'%syst))


    sysString = ('_' + syst) if syst != '' else ''
    ttbar  = tfile.Get('ttbar' + sysString );
    if not ttbar: 
        ttbar = tfile.Get('ttbar')
    dy     = tfile.Get('DY'    + sysString ); 
    if not dy   : 
        dy    = tfile.Get('DY'   )
    fakes  = tfile.Get('Fakes' + sysString ); 
    if not fakes: 
        fakes = tfile.Get('Fakes')
    vvttv  = tfile.Get('VVttV' + sysString ); 
    if not vvttv: 
        vvttv = tfile.Get('VVttV')

    data.Add( ttbar , -1 )
    data.Add( dy    , -1 )
    data.Add( fakes , -1 )
    data.Add( vvttv , -1 ) 

    return data

def getXSecForNomSys(process, tfile, size):
    
    dataUp = copy.deepcopy(tfile.Get('data_obs').Clone('data_%s_Up'%process))
    dataDn = copy.deepcopy(tfile.Get('data_obs').Clone('data_%s_Dn'%process))



    ttbar  = tfile.Get('ttbar'  );
    dy     = tfile.Get('DY'     ); 
    fakes  = tfile.Get('Fakes'  ); 
    vvttv  = tfile.Get('VVttV'  ); 


    eval(process).Scale( 1 + size)

    dataUp.Add( ttbar , -1 )
    dataUp.Add( dy    , -1 )
    dataUp.Add( fakes , -1 )
    dataUp.Add( vvttv , -1 ) 

    eval(process).Scale( 1 / (1+size)**2)

    dataDn.Add( ttbar , -1 )
    dataDn.Add( dy    , -1 )
    dataDn.Add( fakes , -1 )
    dataDn.Add( vvttv , -1 ) 



    return dataUp, dataDn


tfile = r.TFile.Open('temp/forCards_LeadingLepPt.root')

sysList = ['ttbar_ElMu_statbin1Up','ttbar_ElMu_statbin1Down','ttbar_ElMu_statbin2Up','ttbar_ElMu_statbin2Down','ttbar_ElMu_statbin3Up','ttbar_ElMu_statbin3Down','ttbar_ElMu_statbin4Up','ttbar_ElMu_statbin4Down','ttbar_ElMu_statbin5Up','ttbar_ElMu_statbin5Down','ttbar_ElMu_statbin6Up','ttbar_ElMu_statbin6Down','JERUp','hdampUp','hdampDown','UEUp','UEDown','isrUp','isrDown','fsrUp','fsrDown','PowhegerdON','JERDown','JESUp','JESDown','BtagUp','BtagDown','MistagUp','MistagDown','PUUp','PUDown','ElecEffUp','ElecEffDown','MuonEffUp','MuonEffDown','TrigUp','TrigDown'] # ,'GluonMoveCRTune','GluonMoveCRTune_GluonMoveCRTune_TLeadingJetPt','GluonMoveCRTuneerdON','GluonMoveCRTune_erdON_GluonMoveCRTuneerdON_TLeadingJetPt ''QCDbasedCRTuneerdON','QCDbasedCRTune_erdON_QCDbasedCRTuneerdON_TLeadingJetPt',


nom = getXsecForSys('',tfile)
variations = {} 
for syst in sysList: 
    variations[syst] = getXsecForSys(syst,tfile)

for proc, size in zip( ['ttbar','dy','fakes','vvttv'], [0.03,0.5,0.5,0.5]):
    up, dn = getXSecForNomSys(proc, tfile, size)
    variations[proc + '_Up'] = up
    variations[proc + '_Down'] = dn


tfile.Close()


nominal_withErrors = ep.propagateHistoAsym( nom, variations, False)
plot                = bp.beautifulUnfoldingPlots('LeadingLepPt_' + '_folded')
plot.plotspath      = "results/"
nom.SetMarkerStyle(r.kFullCircle)
nom.GetXaxis().SetNdivisions(505, True)

nominal_withErrors[0].SetFillColorAlpha(r.kBlue,0.35)
nominal_withErrors[0].SetLineColor(r.kBlue)
nominal_withErrors[0].SetFillStyle(1001)
plot.addHisto(nominal_withErrors, 'hist', 'Syst. unc.', 'F')
plot.addHisto(nom, 'P,E,same', 'Data', 'P')
plot.saveCanvas('TC')

c = r.TCanvas()
nom.Draw()
variations['ttbar_Up'].Draw('same')
raw_input('kk')
del c 

uncList = ep.getUncList(nom, variations, False)[:5]

plot    = bp.beautifulUnfoldingPlots('LeadingLepPt_' + 'uncertainties_folded')
uncList[0][1].Draw()

if uncList[0][1].GetMaximum() < 0.5:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.5)
else:
    uncList[0][1].GetYaxis().SetRangeUser(0, 0.9)
for i in range(5):
    #uncList[i][1].SetLineColor(bp.colorMap[i])
    uncList[i][1].SetLineColor(vl.colorMap[uncList[i][0]])
    uncList[i][1].SetLineWidth( 2 )
    plot.addHisto(uncList[i][1], 'H,same' if i else 'H', uncList[i][0], 'L')


incmax  = []
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    if nominal_withErrors[1].GetBinError(bin) > nominal_withErrors[0].GetBinContent(bin):
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[0].GetBinContent(bin)]))
    else:
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[1].GetBinError(bin)]))

maxinctot = 0
hincmax   = copy.deepcopy(nom.Clone('hincmax'))
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    hincmax.SetBinContent(bin, incmax[bin-1] / hincmax.GetBinContent(bin))
    hincmax.SetBinError(bin, 0.)
    if (hincmax.GetBinContent(bin) > maxinctot): maxinctot = hincmax.GetBinContent(bin)

hincmax.SetLineColor(r.kBlack)
hincmax.SetLineWidth( 2 )
hincmax.SetFillColorAlpha(r.kBlue, 0.)
plot.addHisto(hincmax, 'hist,same', 'Total', 'L')


plot.plotspath = "results/"
plot.saveCanvas('TC')



raw_input('')

out = r.TFile.Open('mierdas.root','recreate')
nom.Write()
for syst in sysList:
    variations[syst].Write()
out.Close()
