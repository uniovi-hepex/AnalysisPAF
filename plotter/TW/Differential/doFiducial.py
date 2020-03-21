'''
Piece of art for doing fiducial vainas

'''

import ROOT as r
import beautifulUnfoldingPlots as bp
import errorPropagator as ep
import varList as vl
import warnings as wr
import os, sys, math, array, copy
import getLaTeXtable as tex

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
        #if key == "LeadingLepPt_asimov": print key
        
        for bin in range(1, dv[key].GetNbinsX() + 1):
            goodunc = r.TMath.Sqrt( dv[key].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**2 +
                                    dv[key].GetBinContent(bin)**2 * df[key.replace(varName, "Fiducial")].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**4 -
                                    2 * dv[key].GetBinContent(bin) / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**3 * sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)]) )
            
            dfinal[key].SetBinError(bin, goodunc)
            #if key == "LeadingLepPt_asimov":
                #print "dv[key].GetBinError(bin)**2", dv[key].GetBinError(bin)**2
                #print "df[key.replace(varName, Fiducial)].GetBinContent(bin)**2", df[key.replace(varName, "Fiducial")].GetBinContent(bin)**2
                #print "df[key.replace(varName, Fiducial)].GetBinContent(bin)**3", df[key.replace(varName, "Fiducial")].GetBinContent(bin)**3
                #print "df[key.replace(varName, Fiducial)].GetBinContent(bin)**4", df[key.replace(varName, "Fiducial")].GetBinContent(bin)**4
                #print "df[key.replace(varName, Fiducial)].GetBinError(bin)**2", df[key.replace(varName, "Fiducial")].GetBinError(bin)**2
                #print "sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)])", sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)])
                #print "dv[key].GetBinError(bin)**2 / df[key.replace(varName, Fiducial)].GetBinContent(bin)**2 + dv[key].GetBinContent(bin)**2 * df[key.replace(varName, Fiducial)].GetBinError(bin)**2 / df[key.replace(varName, Fiducial)].GetBinContent(bin)**4 - 2 * dv[key].GetBinContent(bin) / df[key.replace(varName, Fiducial)].GetBinContent(bin)**3 * sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)])", dv[key].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**2 + dv[key].GetBinContent(bin)**2 * df[key.replace(varName, "Fiducial")].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**4 - 2 * dv[key].GetBinContent(bin) / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**3 * sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)])
                #print "primero", dv[key].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**2 + dv[key].GetBinContent(bin)**2 * df[key.replace(varName, "Fiducial")].GetBinError(bin)**2 / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**4
                #print "segundo", 2 * dv[key].GetBinContent(bin) / df[key.replace(varName, "Fiducial")].GetBinContent(bin)**3 * sum([covm.GetBinContent(bin, j) for j in range(1, dv[key].GetNbinsX() + 1)])
                #print goodunc, dfinal[key].GetBinContent(bin)
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
    dirvar[key.GetName()] = copy.deepcopy(key.ReadObj().Clone(key.GetName()))

covmat = copy.deepcopy(fvar.Get("CovMat").Clone("covmat"))

ffid.Close(); fvar.Close(); del ffid, fvar

dirfinal = {}
for key in dirvar:
    name    = "Fiducial" if key == varName else "Fiducial" + key.replace(varName, "")
    dirfinal[key] = copy.deepcopy(dirvar[key].Clone())
    dirfinal[key].Divide(dirfid[name])


BibhuFunction(dirvar, dirfid, dirfinal, covmat)
del dirfid

nominal   = copy.deepcopy(dirfinal[varName])
allHistos = { key.replace(varName + "_", "") : dirfinal[key] for key in dirfinal}
del allHistos[varName]


savetfile = r.TFile("temp/{var}_/fidOutput_{var}.root".format(var = varName), "recreate")
nominal.Write()
for key in allHistos: allHistos[key].Write()
savetfile.Close(); del savetfile

nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, True, False, doSym = vl.doSym)
plot               = bp.beautifulUnfoldingPlots(varName + "_fiducial")
plot.doRatio       = True
plot.doFit         = False
plot.plotspath     = 'results/'
plot.doPreliminary = vl.doPre

nominal.SetMarkerStyle(r.kFullCircle)
nominal.SetLineColor(r.kBlack)
nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
nominal_withErrors[0].SetLineColor(0)
nominal_withErrors[0].SetFillStyle(1001)


savetfile2 = r.TFile("temp/{var}_/fidOutput_{var}.root".format(var = varName), "update")
nom0 = copy.deepcopy(nominal_withErrors[0].Clone("nom0"))
nom1 = copy.deepcopy(nominal_withErrors[1].Clone("nom1"))
nom0.Write()
nom1.Write()
savetfile2.Close(); del nom0,nom1,savetfile2

#############################
print "\nLOS RESULTAOS NORMALIZAOS A SECCION EF. FID. - {uno} - {dos}".format(uno = "DATOS", dos = varName)
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    print "Bin", bin, "(abs.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin), 4), "-", round(nominal_withErrors[1].GetBinError(bin), 4), ") pb"
    print "Bin", bin, "(rel.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), "-", round(nominal_withErrors[1].GetBinError(bin)/nominal.GetBinContent(bin)*100, 4), ") pb\n"
print "\n"
#############################

tex.saveLaTeXfromhisto(nominal, varName, path = vl.tablespath, errhisto = nominal_withErrors[0], ty = "unfolded_norm")


if "legpos_fid" in vl.varList[varName]: legloc = vl.varList[varName]["legpos_fid"]
else:                                   legloc = "TR"

yaxismax_fid = 1.4
if "yaxismax_fid" in vl.varList[varName]: yaxismax_fid = vl.varList[varName]["yaxismax_fid"]
else:                                     yaxismax_fid = 1.4

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
aMCatNLO.SetLineStyle(2)
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


savetfile2 = r.TFile("temp/{var}_/fidOutput_{var}.root".format(var = varName), "update")
tru.Write()
aMCatNLO.Write()
hDS.Write()
savetfile2.Close(); del savetfile2

plot.addHisto(nominal_withErrors, 'hist',   'Uncertainty',              'F', 'unc')
plot.addHisto(tru,                'L,same', 'tW Powheg DR + Pythia8',   'L', 'mc')
plot.addHisto(hDS,                'L,same', 'tW Powheg DS + Pythia8',   'L', 'mc')
plot.addHisto(aMCatNLO,           'L,same', 'tW aMC@NLO DR + Pythia8',  'L', 'mc')
plot.addHisto(nominal,            'P,E,same',vl.labellegend,          'PEL', 'data')
plot.saveCanvas(legloc)

#plot.addHisto(nominal_withErrors, 'E2',     'Uncertainty',   'F')
#plot.addHisto(nominal, 'P,same',vl.labellegend,'PE', 'data')
#plot.saveCanvas(legloc)

del plot

plot2       = bp.beautifulUnfoldingPlots(varName + 'uncertainties_fiducial')
plot2.doFit = False
plot2.doPreliminary = vl.doPre
uncListorig     = ep.getUncList(nominal, allHistos, True, False, False)
print 'Full uncertainties list (ordered by impact):', uncListorig
uncList     = uncListorig[:vl.nuncs]

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


actualindex = 0
for i in range(vl.nuncs):
    #if "Stat" in uncListorig[i][0]:
        #uncListorig[actualindex][1].SetLineColor(r.kBlack)
        #uncListorig[actualindex][1].SetLineStyle( 2 )
    if "Stat" in uncListorig[actualindex][0]:
        actualindex += 1
    uncListorig[actualindex][1].SetLineColor( vl.NewColorMap[uncListorig[actualindex][0]] )
    uncListorig[actualindex][1].SetLineWidth( 2 )
    if "Lumi" in uncListorig[actualindex][0]:
        uncListorig[actualindex][1].SetLineColor(r.kBlack)
        uncListorig[actualindex][1].SetLineStyle( 4 )
    if i == 0: uncListorig[actualindex][1].GetYaxis().SetRangeUser(0, yaxismax_fid)
    plot2.addHisto(uncListorig[actualindex][1], 'H,same' if i else 'H', uncListorig[actualindex][0],'L')
    actualindex += 1

for i in range(len(uncListorig)):
    if "Stat" in uncListorig[i][0]:
        uncListorig[i][1].SetLineColor(r.kBlack)
        uncListorig[i][1].SetLineStyle( 2 )
        plot2.addHisto(uncListorig[i][1], 'hist,same', 'Stat.', 'L')
plot2.addHisto(hincsyst, 'hist,same', 'Syst.', 'L')
plot2.addHisto(hincmax,  'hist,same', 'Total', 'L')
plot2.plotspath = "results/"

if "uncleg_fid" in vl.varList[varName]: unclegpos = vl.varList[varName]["uncleg_fid"]
else:                                   unclegpos = "TR"

plot2.saveCanvas(unclegpos)
del plot2


print "> Fiducial vainas done!"
print "> Now let's obtain the same plots normalised but also divided by the bin's width!!"

yaxismax_fidnorm = 1.4
if "yaxismax_fidnorm" in vl.varList[varName]: yaxismax_fidnorm = vl.varList[varName]["yaxismax_fidnorm"]
else:                                         yaxismax_fidnorm = 1.4

del nominal_withErrors, tru, aMCatNLO, hDS
nominal.Scale(1, "width")
for key in allHistos:
    allHistos[key].Scale(1, "width")


savetfile2 = r.TFile("temp/{var}_/fidOutput_{var}norm.root".format(var = varName), "recreate")
nominal.Write()
for key in allHistos: allHistos[key].Write()
savetfile2.Close(); del savetfile2


nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, True, False, doSym = vl.doSym)
plot               = bp.beautifulUnfoldingPlots(varName + "_fiducialnorm")
plot.doRatio       = True
plot.doFit         = False
plot.plotspath     = 'results/'
plot.doPreliminary = vl.doPre

nominal.SetMarkerStyle(r.kFullCircle)
nominal.SetLineColor(r.kBlack)
nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
nominal_withErrors[0].SetLineColor(0)
nominal_withErrors[0].SetFillStyle(1001)

if "yaxisuplimitunffidnorm" in vl.varList[varName]: plot.yaxisuplimit = vl.varList[varName]["yaxisuplimitunffidnorm"]

#############################
print "\nLOS RESULTAOS NORMALIZAOS A SEC. EF. FID. Y POR BIN- {uno} - {dos}".format(uno = "DATOS", dos = varName)
comprobasao = 0
for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    print "Bin", bin, "(abs.): (", round(nominal.GetBinContent(bin), 6), "+", round(nominal_withErrors[0].GetBinError(bin), 6), "-", round(nominal_withErrors[1].GetBinError(bin), 6), ") pb"
    print "Bin", bin, "(rel.): (", round(nominal.GetBinContent(bin), 6), "+", round(nominal_withErrors[0].GetBinError(bin)/nominal.GetBinContent(bin)*100, 6), "-", round(nominal_withErrors[1].GetBinError(bin)/nominal.GetBinContent(bin)*100, 6), ") pb\n"
    comprobasao += nominal.GetBinContent(bin) * nominal.GetBinWidth(bin)
print "Comprobasao:", comprobasao, "\n"
print "Underflow:", nominal.GetBinContent(0)
print "Overflow:",  nominal.GetBinContent(nominal.GetNbinsX() + 1)
print "\n"
#############################

tex.saveLaTeXfromhisto(nominal, varName, path = vl.tablespath, errhisto = nominal_withErrors[0], ty = "unfolded_binnorm")

if "legpos_fidbin" in vl.varList[varName]: legloc = vl.varList[varName]["legpos_fidbin"]
else:                                      legloc = "TR"

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
tru.Scale(1, "width")
tmptfile.Close(); tmptfile11.Close(); del tmptfile, tmptfile11, htmp

if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist.')
tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName))
aMCatNLO = copy.deepcopy(tmptfile2.Get('tW').Clone('aMCatNLO'))
aMCatNLO.SetLineWidth(2)
aMCatNLO.SetLineColor(r.kAzure)
aMCatNLO.SetLineStyle(2)
tmptfile22 = r.TFile.Open('temp/Fiducial_/ClosureTest_aMCatNLO_Fiducial.root')
htmp   = r.TH1F(aMCatNLO)
for bin in range(1, tru.GetNbinsX() + 1):
    htmp.SetBinContent(bin, tmptfile22.Get('tW').GetBinContent(1))
    htmp.SetBinError(bin, tmptfile22.Get('tW').GetBinError(1))
aMCatNLO.Divide(htmp)
aMCatNLO.Scale(1, "width")
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
hDS.Scale(1, "width")
tmptfile3.Close(); tmptfile33.Close(); del tmptfile3, tmptfile33, htmp

savetfile2 = r.TFile("temp/{var}_/fidOutput_{var}norm.root".format(var = varName), "update")
tru.Write()
aMCatNLO.Write()
hDS.Write()
savetfile2.Close()

plot.addHisto(nominal_withErrors, 'hist',    'Uncertainty',              'F', 'unc')
plot.addHisto(tru,                'L,same',  'tW Powheg DR + Pythia8',   'L', 'mc')
plot.addHisto(hDS,                'L,same',  'tW Powheg DS + Pythia8',   'L', 'mc')
plot.addHisto(aMCatNLO,           'L,same',  'tW aMC@NLO DR + Pythia8',  'L', 'mc')
plot.addHisto(nominal,            'P,E,same{s}'.format(s = ",X0" if "equalbinsunf" in vl.varList[varName.replace('_folded', '').replace('_asimov', '').replace("_fiducial", "").replace('norm', '')] else ""),vl.labellegend,           'PEL','data')
plot.saveCanvas(legloc)

#plot.addHisto(nominal_withErrors, 'E2',     'Uncertainty',   'F')
#plot.addHisto(nominal, 'P,same',vl.labellegend,'PE', 'data')
#plot.saveCanvas(legloc)
del plot

plot2       = bp.beautifulUnfoldingPlots(varName + 'uncertainties_fiducialnorm')
plot2.doFit = False
plot2.doPreliminary = vl.doPre
uncListorig = ep.getUncList(nominal, allHistos, True, False, False)
print 'Full uncertainties list (ordered by impact):', uncListorig
uncList     = uncListorig[:vl.nuncs]

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


hincmax.GetYaxis().SetRangeUser(0, yaxismax_fidnorm)
plot2.addHisto(hincmax,  'hist', 'Total', 'L')
plot2.addHisto(hincsyst, 'hist,same', 'Systematic', 'L')
actualindex = 0
isstat = False
for i in range(vl.nuncs):
    #if "Stat" in uncListorig[i][0]:
        #uncListorig[actualindex][1].SetLineColor(r.kBlack)
        #uncListorig[actualindex][1].SetLineStyle( 2 )
    #if "Stat" in uncListorig[actualindex][0]:
        #actualindex += 1
    uncListorig[actualindex][1].SetLineColor( vl.NewColorMap[uncListorig[actualindex][0]] )
    uncListorig[actualindex][1].SetLineWidth( 2 )
    if "Lumi" in uncListorig[actualindex][0]:
        uncListorig[actualindex][1].SetLineColor(r.kBlack)
        uncListorig[actualindex][1].SetLineStyle( 4 )
    if "Stat" in uncListorig[actualindex][0]:
        isstat = True
        uncListorig[actualindex][1].SetLineColor(r.kBlack)
        uncListorig[actualindex][1].SetLineStyle( 2 )
        plot2.addHisto(uncListorig[actualindex][1], 'hist,same', vl.SysNameTranslator[uncListorig[actualindex][0]], 'L')
        actualindex += 1
    elif (not isstat and actualindex == vl.nuncs - 1):
        isstat = True
        for j in range(len(uncListorig)):
            if "Stat" in uncListorig[j][0]:
                uncListorig[j][1].SetLineColor(r.kBlack)
                uncListorig[j][1].SetLineStyle( 2 )
                plot2.addHisto(uncListorig[j][1], 'hist,same', vl.SysNameTranslator[uncListorig[j][0]], 'L')
    else:
        plot2.addHisto(uncListorig[actualindex][1], 'H,same', vl.SysNameTranslator[uncListorig[actualindex][0]],'L')
    actualindex += 1

#for i in range(len(uncListorig)):
    #if "Stat" in uncListorig[i][0]:
        #uncListorig[i][1].SetLineColor(r.kBlack)
        #uncListorig[i][1].SetLineStyle( 2 )
        #plot2.addHisto(uncListorig[i][1], 'hist,same', 'Statistical', 'L')
plot2.plotspath = "results/"

if "uncleg_fidbin" in vl.varList[varName]: unclegpos = vl.varList[varName]["uncleg_fidbin"]
else:                                      unclegpos = "TR"

plot2.saveCanvas(unclegpos)
del plot2


print "> Fiducial vainas but also normalised per bin width done!"
print "> Now let's obtain the same plots only normalised to the bin width!!"
del nominal, nominal_withErrors, tru, aMCatNLO, hDS, allHistos

nominal   = copy.deepcopy(dirvar[varName])
allHistos = { key.replace(varName + "_", "") : dirvar[key] for key in dirvar}
del allHistos[varName]

nominal.Scale(1, "width")
for key in allHistos:
    allHistos[key].Scale(1, "width")

savetfile = r.TFile("temp/{var}_/normOutput_{var}.root".format(var = varName), "recreate")
nominal.Write()
for key in allHistos: allHistos[key].Write()
savetfile.Close()


nominal_withErrors = ep.propagateHistoAsym(nominal, allHistos, True, False, doSym = vl.doSym)
plot               = bp.beautifulUnfoldingPlots(varName + "_norm")
plot.doRatio       = True
plot.doFit         = False
plot.plotspath     = 'results/'
plot.doPreliminary = vl.doPre

if "yaxisuplimitunfnorm" in vl.varList[varName]: plot.yaxisuplimit = vl.varList[varName]["yaxisuplimitunfnorm"]

nominal.SetMarkerStyle(r.kFullCircle)
nominal.SetLineColor(r.kBlack)
nominal_withErrors[0].SetFillColorAlpha(r.kBlue, 0.35)
nominal_withErrors[0].SetLineColor(0)
nominal_withErrors[0].SetFillStyle(1001)


savetfile1 = r.TFile("temp/{var}_/normOutput_{var}.root".format(var = varName), "update")
nom0 = copy.deepcopy(nominal_withErrors[0].Clone("nom0"))
nom1 = copy.deepcopy(nominal_withErrors[1].Clone("nom1"))
nom0.Write()
nom1.Write()
savetfile1.Close()


#############################
print "\nLOS RESULTAOS NORMALIZAOS A ANCHO DEL BIN - {uno} - {dos}".format(uno = "DATOS", dos = varName)

comprobasao = 0

for bin in range(1, nominal_withErrors[0].GetNbinsX() + 1):
    print "Bin", bin, "(abs.): (", round(nominal.GetBinContent(bin), 5), "+", round(nominal_withErrors[0].GetBinError(bin), 5), "-", round(nominal_withErrors[1].GetBinError(bin), 5), ") pb"
    print "Bin", bin, "(rel.): (", round(nominal.GetBinContent(bin), 4), "+", round(nominal_withErrors[0].GetBinError(bin)/nominal.GetBinContent(bin)*100, 5), "-", round(nominal_withErrors[1].GetBinError(bin)/nominal.GetBinContent(bin)*100, 5), ") pb\n"
#############################

tex.saveLaTeXfromhisto(nominal, varName, path = vl.tablespath, errhisto = nominal_withErrors[0], ty = "unfolded_bin")


if "legpos_norm" in vl.varList[varName]: legloc = vl.varList[varName]["legpos_norm"]
else:                                    legloc = "TR"

yaxismax_norm = 1.4
if "yaxismax_norm" in vl.varList[varName]: yaxismax_norm = vl.varList[varName]["yaxismax_norm"]
else:                                      yaxismax_norm = 1.4

if not os.path.isfile('temp/{var}_/ClosureTest_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information does not exist.')
tmptfile = r.TFile.Open('temp/{var}_/ClosureTest_{var}.root'.format(var = varName))
tru = copy.deepcopy(tmptfile.Get('tW').Clone('tru'))
tru.SetLineWidth(2)
tru.SetLineColor(bp.colorMap[0])
tru.Scale(1, "width")
tmptfile.Close(); del tmptfile

if not os.path.isfile('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information from an aMCatNLO sample does not exist.')
tmptfile2 = r.TFile.Open('temp/{var}_/ClosureTest_aMCatNLO_{var}.root'.format(var = varName))
aMCatNLO = copy.deepcopy(tmptfile2.Get('tW').Clone('aMCatNLO'))
aMCatNLO.SetLineWidth(2)
aMCatNLO.SetLineColor(r.kAzure)
aMCatNLO.SetLineStyle(2)
aMCatNLO.Scale(1, "width")
tmptfile2.Close(); del tmptfile2

if not os.path.isfile('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = varName)):
    raise RuntimeError('The rootfile with the generated information with the DS variation does not exist.')
tmptfile3 = r.TFile.Open('temp/{var}_/ClosureTest_DS_{var}.root'.format(var = varName))
hDS = copy.deepcopy(tmptfile3.Get('tW').Clone('hDS'))
hDS.SetLineWidth(2)
hDS.SetLineColor(r.kGreen)
hDS.Scale(1, "width")
tmptfile3.Close(); del tmptfile3


savetfile2 = r.TFile("temp/{var}_/normOutput_{var}.root".format(var = varName), "update")
tru.Write()
aMCatNLO.Write()
hDS.Write()
savetfile2.Close(); del savetfile2

plot.addHisto(nominal_withErrors, 'hist',   'Uncertainty',              'F', 'unc')
plot.addHisto(tru,                'L,same', 'tW Powheg DR + Pythia8',   'L', 'mc')
plot.addHisto(hDS,                'L,same', 'tW Powheg DS + Pythia8',   'L', 'mc')
plot.addHisto(aMCatNLO,           'L,same', 'tW aMC@NLO DR + Pythia8',  'L', 'mc')
plot.addHisto(nominal,            'P,E,same',vl.labellegend,          'PEL', 'data')
plot.saveCanvas(legloc)

#plot.addHisto(nominal_withErrors, 'E2',     'Uncertainty',   'F')
#plot.addHisto(nominal, 'P,same',vl.labellegend,'PE', 'data')
#plot.saveCanvas(legloc)

del plot

plot2       = bp.beautifulUnfoldingPlots(varName + 'uncertainties_norm')
plot2.doFit = False
plot2.doPreliminary = vl.doPre
uncListorig     = ep.getUncList(nominal, allHistos, True, False, False)
print 'Full uncertainties list (ordered by impact):', uncListorig
uncList     = uncListorig[:vl.nuncs]

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


actualindex = 0
hincmax.GetYaxis().SetRangeUser(0, yaxismax_norm)
plot2.addHisto(hincmax,  'hist', 'Total', 'L')
plot2.addHisto(hincsyst, 'hist,same', 'Systematic', 'L')
#for i in range(vl.nuncs):
    ##if "Stat" in uncListorig[i][0]:
        ##uncListorig[actualindex][1].SetLineColor(r.kBlack)
        ##uncListorig[actualindex][1].SetLineStyle( 2 )
    #if "Stat" in uncListorig[actualindex][0]:
        #actualindex += 1
    #uncListorig[actualindex][1].SetLineColor( vl.NewColorMap[uncListorig[actualindex][0]] )
    #uncListorig[actualindex][1].SetLineWidth( 2 )
    #if "Lumi" in uncListorig[actualindex][0]:
        #uncListorig[actualindex][1].SetLineColor(r.kBlack)
        #uncListorig[actualindex][1].SetLineStyle( 4 )
    #if i == 0: uncListorig[actualindex][1].GetYaxis().SetRangeUser(0, yaxismax_fid)
    #plot2.addHisto(uncListorig[actualindex][1], 'H,same' if i else 'H', vl.SysNameTranslator[uncListorig[actualindex][0]],'L')
    #actualindex += 1

for i in range(vl.nuncs):
    #if "Stat" in uncListorig[i][0]:
        #uncListorig[actualindex][1].SetLineColor(r.kBlack)
        #uncListorig[actualindex][1].SetLineStyle( 2 )
    #if "Stat" in uncListorig[actualindex][0]:
        #actualindex += 1
    uncListorig[actualindex][1].SetLineColor( vl.NewColorMap[uncListorig[actualindex][0]] )
    uncListorig[actualindex][1].SetLineWidth( 2 )
    if "Lumi" in uncListorig[actualindex][0]:
        uncListorig[actualindex][1].SetLineColor(r.kBlack)
        uncListorig[actualindex][1].SetLineStyle( 4 )
    if "Stat" in uncListorig[actualindex][0]:
        isstat = True
        uncListorig[actualindex][1].SetLineColor(r.kBlack)
        uncListorig[actualindex][1].SetLineStyle( 2 )
        plot2.addHisto(uncListorig[actualindex][1], 'hist,same', vl.SysNameTranslator[uncListorig[actualindex][0]], 'L')
        actualindex += 1
    elif (not isstat and actualindex == vl.nuncs - 1):
        isstat = True
        for j in range(len(uncListorig)):
            if "Stat" in uncListorig[j][0]:
                uncListorig[j][1].SetLineColor(r.kBlack)
                uncListorig[j][1].SetLineStyle( 2 )
                plot2.addHisto(uncListorig[j][1], 'hist,same', vl.SysNameTranslator[uncListorig[j][0]], 'L')
    else:
        plot2.addHisto(uncListorig[actualindex][1], 'H,same', vl.SysNameTranslator[uncListorig[actualindex][0]],'L')
    actualindex += 1

#for i in range(len(uncListorig)):
    #if "Stat" in uncListorig[i][0]:
        #uncListorig[i][1].SetLineColor(r.kBlack)
        #uncListorig[i][1].SetLineStyle( 2 )
        #plot2.addHisto(uncListorig[i][1], 'hist,same', 'Stat.', 'L')
#plot2.addHisto(hincsyst, 'hist,same', 'Syst.', 'L')
#plot2.addHisto(hincmax,  'hist,same', 'Total', 'L')
plot2.plotspath = "results/"

if "uncleg_norm" in vl.varList[varName]: unclegpos = vl.varList[varName]["uncleg_norm"]
else:                                    unclegpos = "TR"

plot2.saveCanvas(unclegpos)
del plot2


savetfile3 = r.TFile("temp/{var}_/normOutput_{var}.root".format(var = varName), "update")
hincmax.Write()
hincsyst.Write()
savetfile3.Close(); del savetfile3
