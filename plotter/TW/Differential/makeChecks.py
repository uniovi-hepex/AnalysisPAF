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

if not os.path.isdir('results/fiduchecks/{var}'.format(var = varName)):
    os.system('mkdir -p results/fiduchecks/{var}'.format(var = varName))
    os.system('mkdir -p results/fiduchecks/Fiducial')
    os.system('cp /pool/ciencias_users/user/vrbouza/www/index.php results/fiduchecks/index.php')
    os.system('cp /pool/ciencias_users/user/vrbouza/www/index.php results/fiduchecks/{var}/index.php'.format(var = varName))
    os.system('cp /pool/ciencias_users/user/vrbouza/www/index.php results/fiduchecks/Fiducial/index.php')
    
print "\n> Beginning to make these checks...\n"
ffid = r.TFile.Open("temp/{var}_/fidOutput_{var}.root".format(var = varName), "read")

htmpnom = copy.deepcopy(ffid.Get(varName).Clone("nom"))
htmpnom.SetLineColor(r.kBlack)
htmpnom.SetLineWidth(2)
htmpnom.GetXaxis().SetTitle( vl.varList[varName]['xaxis'] )

for key in ffid.GetListOfKeys():
    if key.GetName() == varName or "nom" in key.GetName() or "Down" in key.GetName(): continue
    sys = key.GetName().replace(varName + "_", "")
    if "Up" not in key.GetName() and "Down" not in key.GetName(): continue
    htmpup = copy.deepcopy(key.ReadObj().Clone(sys))
    htmpdw = copy.deepcopy(ffid.Get(key.GetName().replace("Up", "Down")).Clone(sys.replace("Up", "Down")))
    htmpup.SetLineColor(r.kBlue); htmpdw.SetLineColor(r.kRed);
    htmpup.SetLineWidth(2); htmpdw.SetLineWidth(2)

    c = r.TCanvas()
    htmpnom.Draw()
    htmpup.Draw("same"); htmpdw.Draw("same")
    c.SaveAs('results/fiduchecks/{var}/{var}_{syst}.png'.format(var = varName, syst = sys.replace("Up", "")));
    del c, htmpup, htmpdw

ffid.Close(); del ffid, htmpnom


ffid = r.TFile.Open("temp/{var}_/unfOutput_{var}.root".format(var = varName), "read")

htmpnom = copy.deepcopy(ffid.Get(varName).Clone("nom"))
htmpnom.SetLineColor(r.kBlack)
htmpnom.SetLineWidth(2)
htmpnom.GetXaxis().SetTitle( vl.varList[varName]['xaxis'] )

for key in ffid.GetListOfKeys():
    if key.GetName() == varName or "nom" in key.GetName() or "Down" in key.GetName(): continue
    sys = key.GetName().replace(varName + "_", "")
    if "Up" not in key.GetName() and "Down" not in key.GetName(): continue
    htmpup = copy.deepcopy(key.ReadObj().Clone(sys))
    htmpdw = copy.deepcopy(ffid.Get(key.GetName().replace("Up", "Down")).Clone(sys.replace("Up", "Down")))
    htmpup.SetLineColor(r.kBlue); htmpdw.SetLineColor(r.kRed);
    htmpup.SetLineWidth(2); htmpdw.SetLineWidth(2)

    c = r.TCanvas()
    htmpnom.Draw()
    htmpup.Draw("same"); htmpdw.Draw("same")
    c.SaveAs('results/fiduchecks/{var}/{var}_unf_{syst}.png'.format(var = varName, syst = sys.replace("Up", "")));
    del c, htmpup, htmpdw

ffid.Close(); del ffid, htmpnom


ffid = r.TFile.Open("temp/Fiducial_/unfOutput_Fiducial.root", "read")

htmpnom = copy.deepcopy(ffid.Get("Fiducial").Clone("nom"))
htmpnom.SetLineColor(r.kBlack)
htmpnom.SetLineWidth(2)
htmpnom.GetXaxis().SetTitle( vl.varList["Fiducial"]['xaxis'] )
htmpnom.GetYaxis().SetRangeUser( 0.10, 0.50 )

for key in ffid.GetListOfKeys():
    if key.GetName() == "Fiducial" or "nom" in key.GetName() or "Down" in key.GetName(): continue
    sys = key.GetName().replace("Fiducial_", "")
    if "Up" not in key.GetName() and "Down" not in key.GetName(): continue
    htmpup = copy.deepcopy(key.ReadObj().Clone(sys))
    htmpdw = copy.deepcopy(ffid.Get(key.GetName().replace("Up", "Down")).Clone(sys.replace("Up", "Down")))
    htmpup.SetLineColor(r.kBlue); htmpdw.SetLineColor(r.kRed);
    htmpup.SetLineWidth(2); htmpdw.SetLineWidth(2)

    c = r.TCanvas()
    htmpnom.Draw()
    htmpup.Draw("same"); htmpdw.Draw("same")
    c.SaveAs('results/fiduchecks/Fiducial/Fiducial_{syst}.png'.format(syst = sys.replace("Up", "")));
    del c, htmpup, htmpdw

ffid.Close(); del ffid, htmpnom




print "> Checks done!"
