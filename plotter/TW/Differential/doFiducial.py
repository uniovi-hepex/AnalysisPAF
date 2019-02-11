'''
Piece of art for doing fiducial vainas

'''

import ROOT as r
import beautifulUnfoldingPlots as bp
import errorPropagator as ep
import varList as vl
import warnings as wr
import os, sys, math, array, copy

vl.SetUpWarnings()
r.gROOT.SetBatch(True)
verbose = False
print "===== Unfolding procedure\n"
if (len(sys.argv) > 1):
    varName = sys.argv[1]
    print "> Variable to be unfolded:", varName, "\n"
    if (len(sys.argv) > 2):
        pathtothings    = sys.argv[2]
        print "> Special path to things chosen:", pathtothings, "\n"
    else:
        pathtothings    = 'temp/{var}_/'.format(var = varName)
    
else:
    print "> Default variable and path chosen\n"
    varName       = 'LeadingLepEta'
    pathtothings  = 'temp/{var}_/'.format(var = varName)

def BibhuFunction(dv, df, dfinal, covm):
    for key in dv:
        print key
        for bin in range(1, dv[key].GetNbinsX() + 1):
            goodunc = r.TMath.Sqrt( dv[key].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**2 +
                                    dv[key].GetBinContent(bin)**2 * df[key.replace(varName, "Fiducial")].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**4 -
                                    2 * dv[key].GetBinContent(bin) / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**3 * sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)]) )
            
            dfinal[key].SetBinError(bin, goodunc)
            print goodunc, dfinal[key].GetBinContent(bin)
    return




print "\n> Beginning to produce the fiducial vainas...\n"
ffid = r.TFile.Open("temp/Fiducial_/unfOutput_Fiducial.root")
fvar = r.TFile.Open("temp/" + varName + "_/unfOutput_" + varName + ".root")


dirfid = {}; dirvar = {};
for key in ffid.GetListOfKeys():
    if "Fiducial" not in key.GetName(): continue
    htmp = r.TH1D(fvar.Get(varName))
    for bin in range(1, htmp.GetNbinsX() + 1):
        htmp.SetBinContent(bin, key.ReadObj().GetBinContent(1))
        htmp.SetBinError(bin,   key.ReadObj().GetBinError(1))
    dirfid[key.GetName()] = copy.deepcopy(htmp.Clone(key.GetName()))
    del htmp

for key in fvar.GetListOfKeys():
    if varName not in key.GetName(): continue
    dirvar[key.GetName()] = copy.deepcopy(key.ReadObj())

covmat = copy.deepcopy(fvar.Get("CovMat").Clone("covmat"))

ffid.Close(); fvar.Close(); del ffid, fvar

dirfinal = {}
for key in dirvar:
    name    = "Fiducial" if key == varName else "Fiducial" + key.replace(varName, "")
    dirfinal[key] = copy.deepcopy(dirvar[key].Clone())
    dirfinal[key].Divide(dirfid[name])


BibhuFunction(dirvar, dirfid, dirfinal, covmat)
del dirfid, dirvar

nominal   = copy.deepcopy(dirfinal[varName])
allHistos = { key.replace(varName + "_", "") : dirfinal[key] for key in dirfinal}
del allHistos[varName]

#### FOR ASIMOV FIDUCIAL RESULTS -- NOT IN USE!!
#if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var)):
    #raise RuntimeError('The rootfile with the generated information does not exist')
#tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = self.var))
##tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_recobinning_{var}.root'.format(var = self.var))
#tru = copy.deepcopy(tmptfile.Get('tW'))
##tru2 = copy.deepcopy(tmptfile2.Get('tW'))
##tru2.Scale(vl.Lumi*1000)
#for bin in range(1, allHistos['asimov'].GetNbinsX() + 1): allHistos['asimov'].SetBinError(bin,   abs(allHistos['asimov'].GetBinContent(bin) - tru.GetBinContent(bin)))
#tmptfile.Close()
#del tru

savetfile = r.TFile("temp/{var}_/fidOutput_{var}.root".format(var = varName), "recreate")
nominal.Write()
for key in allHistos: allHistos[key].Write()
savetfile.Close()

nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, True, False)
plot               = bp.beautifulUnfoldingPlots(varName + "_fiducial")
plot.doRatio       = True
plot.doFit         = False
plot.plotspath     = 'results/'

nominal.SetMarkerStyle(r.kFullCircle)
nominal.SetLineColor(r.kBlack)
nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
nominal_withErrors[0].SetLineColor(r.kBlue)
nominal_withErrors[0].SetFillStyle(1001)

savetfile2 = r.TFile("temp/{var}_/fidOutput_{var}.root".format(var = varName), "update")
nom0 = copy.deepcopy(nominal_withErrors[0].Clone("nom0"))
nom1 = copy.deepcopy(nominal_withErrors[1].Clone("nom1"))
nom0.Write()
nom1.Write()
savetfile2.Close()
del nom0,nom1

#############################
print "\nLOS RESULTAOS - {uno} - {dos}".format(uno = "DATOS", dos = varName)
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    print "Bin", bin, "(abs.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin), 4), "-", round(nominal_withErrors[1].GetBinError(bin), 4), ") pb"
    print "Bin", bin, "(rel.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), "-", round(nominal_withErrors[1].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), ") pb\n"
print "\n"
#############################

if "legpos_fid" in vl.varList[varName]: legloc = vl.varList[varName]["legpos_fid"]
else:                                   legloc = "TR"


if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information does not exist.')
tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = varName))
tru = copy.deepcopy(tmptfile.Get('tW').Clone('tru'))
tru.SetLineWidth(2)
tru.SetLineColor(bp.colorMap[0])
tmptfile11 = r.TFile.Open('temp/Fiducial_/ClosureTest_Fiducial.root')
htmp   = r.TH1F(tru)
for bin in range(1, tru.GetNbinsX() + 1):
    htmp.SetBinContent(bin, tmptfile11.Get('tW').GetBinContent(1))
    htmp.SetBinError(bin, tmptfile11.Get('tW').GetBinError(1))
tru.Divide(htmp)
tmptfile.Close(); tmptfile11.Close(); del tmptfile, tmptfile11, htmp

if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist.')
tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName))
aMCatNLO = copy.deepcopy(tmptfile2.Get('tW').Clone('aMCatNLO'))
aMCatNLO.SetLineWidth(2)
aMCatNLO.SetLineColor(r.kAzure)
tmptfile22 = r.TFile.Open('temp/Fiducial_/ClosureTest_aMCatNLO_Fiducial.root')
htmp   = r.TH1F(aMCatNLO)
for bin in range(1, tru.GetNbinsX() + 1):
    htmp.SetBinContent(bin, tmptfile22.Get('tW').GetBinContent(1))
    htmp.SetBinError(bin, tmptfile22.Get('tW').GetBinError(1))
aMCatNLO.Divide(htmp)
tmptfile2.Close(); tmptfile22.Close(); del tmptfile2, tmptfile22, htmp

if not os.path.isfile('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information with the DS variation does not exist.')
tmptfile3 = r.TFile.Open('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = varName))
hDS = copy.deepcopy(tmptfile3.Get('tW').Clone('hDS'))
hDS.SetLineWidth(2)
hDS.SetLineColor(r.kGreen)
tmptfile33 = r.TFile.Open('temp/Fiducial_/ClosureTest_DS_Fiducial.root')
htmp   = r.TH1F(hDS)
for bin in range(1, tru.GetNbinsX() + 1):
    htmp.SetBinContent(bin, tmptfile33.Get('tW').GetBinContent(1))
    htmp.SetBinError(bin, tmptfile33.Get('tW').GetBinError(1))
hDS.Divide(htmp)
tmptfile3.Close(); tmptfile33.Close(); del tmptfile3, tmptfile33, htmp

plot.addHisto(nominal_withErrors, 'hist',   'Total unc.',   'F', 'unc')
plot.addHisto(tru,                'L,same', 'tW Powheg',    'L', 'mc')
plot.addHisto(aMCatNLO,           'L,same', 'tW aMCatNLO',  'L', 'mc')
plot.addHisto(hDS,                'L,same', 'tW DS',        'L', 'mc')
plot.addHisto(nominal,            'P,E,same',vl.labellegend,'P', 'data')
plot.saveCanvas(legloc)

#plot.addHisto(nominal_withErrors, 'E2',     'Total unc.',   'F')
#plot.addHisto(nominal, 'P,same',vl.labellegend,'P', 'data')
#plot.saveCanvas(legloc)

del plot

plot2       = bp.beautifulUnfoldingPlots(varName + 'uncertainties_fiducial')
plot2.doFit = False
uncList     = ep.getUncList(nominal, allHistos, True, False, False)
print 'Full uncertainties list (ordered by impact):', uncList
uncList     = uncList[:vl.nuncs]

incmax  = []
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    if nominal_withErrors[1].GetBinError(bin) > nominal_withErrors[0].GetBinContent(bin):
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[0].GetBinContent(bin)]))
    else:
        incmax.append(max([nominal_withErrors[0].GetBinError(bin), nominal_withErrors[1].GetBinError(bin)]))

incsyst  = []
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    if math.sqrt(nominal_withErrors[1].GetBinError(bin)**2 - nominal.GetBinError(bin)**2) > nominal_withErrors[0].GetBinContent(bin):
        incsyst.append(max([math.sqrt(nominal_withErrors[0].GetBinError(bin)**2 - nominal.GetBinError(bin)**2),
                            nominal_withErrors[0].GetBinContent(bin)]))
    else:
        incsyst.append(max([math.sqrt(nominal_withErrors[0].GetBinError(bin)**2 - nominal.GetBinError(bin)**2), 
                            math.sqrt(nominal_withErrors[1].GetBinError(bin)**2 - nominal.GetBinError(bin)**2)]))

maxinctot = 0
hincmax   = nominal_withErrors[0].Clone('hincmax')
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    hincmax.SetBinContent(bin, incmax[bin-1] / hincmax.GetBinContent(bin))
    hincmax.SetBinError(bin, 0)
    if (hincmax.GetBinContent(bin) > maxinctot): maxinctot = hincmax.GetBinContent(bin)

hincsyst  = copy.deepcopy(nominal.Clone('hincsyst'))
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    hincsyst.SetBinContent(bin, incsyst[bin-1] / hincsyst.GetBinContent(bin))
    hincsyst.SetBinError(bin, 0.)

hincmax.SetLineColor(r.kBlack)
hincmax.SetLineWidth( 2 )
hincmax.SetFillColorAlpha(r.kBlue, 0)
hincsyst.SetLineColor(r.kBlack)
hincsyst.SetLineWidth( 2 )
hincsyst.SetLineStyle( 3 )
hincsyst.SetFillColorAlpha(r.kBlue, 0.)

#if (maxinctot >= 0.9):
    #if maxinctot >= 5:
        #uncList[0][1].GetYaxis().SetRangeUser(0, 5)
    #else:
        #uncList[0][1].GetYaxis().SetRangeUser(0, maxinctot + 0.1)
    
#else:
    #uncList[0][1].GetYaxis().SetRangeUser(0, 0.9)

uncList[0][1].GetYaxis().SetRangeUser(0, 1.4)

for i in range(vl.nuncs):
    uncList[i][1].SetLineColor( vl.NewColorMap[uncList[i][0]] )
    uncList[i][1].SetLineWidth( 2 )
    if "Stat" in uncList[i][0]:
        uncList[i][1].SetLineColor(r.kBlack)
        uncList[i][1].SetLineStyle( 2 )
    elif "Lumi" in uncList[i][0]:
        uncList[i][1].SetLineColor(r.kBlack)
        uncList[i][1].SetLineStyle( 4 )
    
    plot2.addHisto(uncList[i][1], 'H,same' if i else 'H',uncList[i][0],'L')

plot2.addHisto(hincsyst, 'hist,same', 'Syst.', 'L')
plot2.addHisto(hincmax,  'hist,same', 'Total', 'L')
plot2.plotspath = 'results/'

if "uncleg_fid" in vl.varList[varName]: unclegpos = vl.varList[varName]["uncleg_fid"]
else:                                   unclegpos = "TR"

plot2.saveCanvas(unclegpos)
del plot2


print "> Fiducial vainas done!"
