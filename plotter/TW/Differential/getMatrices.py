import ROOT as r
import sys, os, shutil
import varList as vl
from array  import array

r.gROOT.SetBatch(True)


#############################
print("\n===== Unfolding procedures: Response matrices & ROOT files production =====")
print("> Setting binning, paths, and other details...")
# ---------------------------------------------------------------- PRELIMINARIES
vl.SetUpWarnings()
storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
minipath    = "../../../TW_temp/"
if (len(sys.argv) > 1):
  print("    - Manual minitrees' folder input!\n")
  if (sys.argv[1] == "last"):
    minipath  = vl.GetLastFolder(storagepath)
  else:
    minipath  = storagepath + sys.argv[1] + "/"

print "    - The minitrees' path is:\n", minipath
plotsoutputpath  = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
matrixoutputpath = "./temp/"

genCut      = "TWeight_normal * (Tpassgen == 1)"
recoCut     = "TWeight_normal * (Tpassreco == 1)"
Cut         = "TWeight * (Tpassgen == 1)"
fiduCut     = "TWeight * ((Tpassreco == 1) && (Tpassgen == 0))"

markersize  = 0.8

VarNames    = vl.varList["Names"]["Variables"]
nvars       = len(VarNames)

SysList     = vl.varList["Names"]["ExpSysts"]
nsys        = len(SysList)

purities    = []
stabilities = []

def GetResponseMatrix(t1, t2, vname, nxb, xb, nyb, yb, sys = "", nomtree = None):
  '''This function obtains the response matrix combining information of two trees.'''
  
  vnamegen    = vl.varList[vname]['var_gen'] # Generation name (without any kind of systematic thing)
  vnamereco   = vl.varList[vname]['var'] # Reconstruction name (can have a systematic-name tail)
  vnametitle  = vname # Name to be shown as the title (and name) of the histogram
  tmpcut      = Cut   # cuts
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   = vnamereco.replace(vname, vname + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling"):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  if not isinstance(t1, list):
    hGen1 = r.TH1F('hGen1', '', nxb, xb)
    hGen2 = r.TH1F('hGen2', '', nxb, xb)
    t1.Draw(vnamegen + '>>hGen1', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    t2.Draw(vnamegen + '>>hGen2', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    hGen1.Add(hGen2)
    del hGen2
  else:
    hGen1 = r.TH1F('hGen1', '', nxb, xb)
    hGen2 = r.TH1F('hGen2', '', nxb, xb)
    hGen1noF = r.TH1F('hGen1noF', '', nxb, xb)
    hGen2noF = r.TH1F('hGen2noF', '', nxb, xb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamegen + '>>hGen1', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    t2[0].Draw(vnamegen + '>>hGen2', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamegen + '>>hGen1noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    t2[1].Draw(vnamegen + '>>hGen2noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
    hGen1.Add(hGen2)
    hGen1noF.Add(hGen2noF)
    hGen1.Add(hGen1noF)
    del hGen2, hGen1noF, hGen2noF
    
    
  hGen  = r.TH2F('Gen', '', nxb, xb, nyb, yb)
  
  for i in range(0, nxb+2):
    for j in range(0, nyb+2):
      hGen.SetBinContent(i, j, hGen1.GetBinContent(i))
      hGen.SetBinError(i, j, hGen1.GetBinError(i))
  
  if not isinstance(t1, list):
    h1    = r.TH2F('h1', "", nxb, xb, nyb, yb)
    h2    = r.TH2F('h2', '', nxb, xb, nyb, yb)
    
    t1.Project('h1', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    t2.Project('h2', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    h1.Add(h2)
    del h2
  else:
    h1    = r.TH2F('h1', "", nxb, xb, nyb, yb)
    h2    = r.TH2F('h2', "", nxb, xb, nyb, yb)
    h1noF = r.TH2F('h1noF', "", nxb, xb, nyb, yb)
    h2noF = r.TH2F('h2noF', '', nxb, xb, nyb, yb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Project('h1', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    t2[0].Project('h2', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Project('h1noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    t2[1].Project('h2noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
    h1.Add(h2)
    h1noF.Add(h2noF)
    h1.Add(h1noF)
    del h2, h1noF, h2noF
    
  if (sys == ""):
    if not isinstance(t1, list):
      hReco1 = r.TH1F('hReco1', '', nyb, yb)
      hReco2 = r.TH1F('hReco2', '', nyb, yb)
      t1.Draw(vnamereco + '>>hReco1', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname))
      t2.Draw(vnamereco + '>>hReco2', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname))
      hReco1.Add(hReco2)
      del hReco2
    else:
      hReco1 = r.TH1F('hReco1', '', nyb, yb)
      hReco2 = r.TH1F('hReco2', '', nyb, yb)
      hReco1noF = r.TH1F('hReco1noF', '', nyb, yb)
      hReco2noF = r.TH1F('hReco2noF', '', nyb, yb)
      
      specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
      t1[0].Draw(vnamereco + '>>hReco1', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname))
      t2[0].Draw(vnamereco + '>>hReco2', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname))
      specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
      t1[1].Draw(vnamereco + '>>hReco1noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname))
      t2[1].Draw(vnamereco + '>>hReco2noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname))
      
      hReco1.Add(hReco2)
      hReco1noF.Add(hReco2noF)
      hReco1.Add(hReco1noF)
      del hReco2, hReco1noF, hReco2noF
    
    tmppur  = []
    tmpstab = []
    for i in range(1, nxb+1):
      sumstab = 0
      for j in range(1, nyb+1):
        sumstab += h1.GetBinContent(i, j)
      try: tmpstab.append(sumstab/hGen1.GetBinContent(i))
      except ZeroDivisionError: tmpstab.append(0)
    stabilities.append(tmpstab)
    del tmpstab
    for j in range(1, nyb+1):
      sumpur  = 0
      for i in range(1, nxb+1):
        sumpur  += h1.GetBinContent(i, j)
      try: tmppur.append(sumpur/hReco1.GetBinContent(j))
      except ZeroDivisionError: tmppur.append(0)
    purities.append(tmppur)
    del tmppur, hReco1
  
  if not isinstance(t1, list):
    if ('fsr' in t1.GetName() or 'FSR' in t1.GetName() or 'isr' in t1.GetName() or 'ISR' in t1.GetName()):
      h1nom = r.TH2F('h1nom', "", nxb, xb, nyb, yb)
      h2nom = r.TH2F('h2nom', '', nxb, xb, nyb, yb)
      nomtree[0].Project('h1nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
      nomtree[1].Project('h2nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname))
      h1nom.Add(h2nom)
      del h2nom
      hGen1nom = r.TH1F('hGen1nom', '', nxb, xb)
      hGen2nom = r.TH1F('hGen2nom', '', nxb, xb)

      nomtree[0].Draw(vnamegen + '>>hGen1nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
      nomtree[1].Draw(vnamegen + '>>hGen2nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname))

      hGen1nom.Add(hGen2nom)
      del hGen2nom
      hGennom = r.TH2F('Gennom', '', nxb, xb, nyb, yb)

      for i in range(0, nxb+2):
        for j in range(0, nyb+2):
          hGennom.SetBinContent(i, j, hGen1nom.GetBinContent(i))
          hGennom.SetBinError(i, j, hGen1nom.GetBinError(i))

      h1.Add(h1nom, -1)
      hGen.Add(hGennom, -1)
      h1nom.Add(h1, r.Double(1/r.TMath.Sqrt(2)))
      hGennom.Add(hGen, r.Double(1/r.TMath.Sqrt(2)))
      del h1, hGen
      h1 = h1nom.Clone("h1")
      #h1.SetTitle("Response matrix - " + vnametitle)
      hGen = hGennom.Clone("hGen")
      del hGennom, h1nom
  
  h1.Divide(hGen)
  del hGen, hGen1
  
  # Fixing the over and underflow bins to one.
  for i in range(1, nxb+1):
    tmpsum = 0.
    for j in range(1, nyb+1):
      tmpsum += h1.GetBinContent(i, j)
    h1.SetBinContent(i, 0, 0.)
    h1.SetBinContent(i, nyb + 1, 1 - tmpsum)
    #h1.SetBinContent(i, nyb + 1, 0)
  for j in range(0, nyb+2):
    h1.SetBinContent(0, j, 0.)
    h1.SetBinContent(nxb + 1, j, 0.)
  
  h1.SetXTitle(vl.varList[vname]['xaxis'] + " [gen]")
  h1.SetYTitle(vl.varList[vname]['xaxis'] + " [reco]")
  h1.SetName("R" + vnametitle)
  return h1


def GetFiducialHisto(t1, t2, vname, nyb, yb, sys = "", nomtree = None):
  '''This function obtains the fiducial histograms combining info. of two trees.'''
  
  vnametitle  = vname   # Name to be shown as the title (and name) of the histogram
  vnamereco   = vl.varList[vname]['var'] # Reconstruction name (can have a systematic-name tail)
  tmpcut      = fiduCut # cuts
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   = vnamereco.replace(vname, vname + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling"):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  if not isinstance(t1, list):
    h1 = r.TH1F('h1', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2 = r.TH1F('h2', '',                                    nyb, yb)
    
    t1.Draw(vnamereco + '>>h1', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname))
    t2.Draw(vnamereco + '>>h2', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname))

    h1.Add(h2)
    del h2
  else:
    h1 = r.TH1F('h1', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2 = r.TH1F('h2', '',                                    nyb, yb)
    h1noF = r.TH1F('h1noF', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2noF = r.TH1F('h2noF', '',                                    nyb, yb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamereco + '>>h1', tmpcut + '*' + str(specialweight))
    t2[0].Draw(vnamereco + '>>h2', tmpcut + '*' + str(specialweight))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamereco + '>>h1noF', tmpcut + '*' + str(specialweight))
    t2[1].Draw(vnamereco + '>>h2noF', tmpcut + '*' + str(specialweight))

    h1.Add(h2)
    h1noF.Add(h2noF)
    h1.Add(h1noF)
    del h2, h1noF, h2noF
  
  if not isinstance(t1, list):
    if ('fsr' in t1.GetName() or 'FSR' in t1.GetName() or 'isr' in t1.GetName() or 'ISR' in t1.GetName()):
      h1nom = r.TH1F('h1nom', "", nyb, yb)
      h2nom = r.TH1F('h2nom', '', nyb, yb)
      nomtree[0].Draw(vnamereco + '>>h1nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname))
      nomtree[1].Draw(vnamereco + '>>h2nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname))
      h1nom.Add(h2nom)
      del h2nom
      
      h1.Add(h1nom, -1)
      h1nom.Add(h1, r.Double(1/r.TMath.Sqrt(2)))
      del h1
      h1 = h1nom.Clone("h1")
      h1.SetTitle("Fiducial histogram - T" + vnametitle)
      del h1nom
  
  h1.SetXTitle("Fiducial events")
  h1.SetYTitle("Events")
  h1.SetName("F" + vnametitle)
  
  return h1


def PrintResponseMatrix(htemp, vname, nxb, xb, xmin, xmax, nyb, yb, ymin, ymax, prof = 0, pur = None, stab = None):
  '''This function prints the response matrix of a given histogram.'''
  if not os.path.exists(plotsoutputpath + vname):
    os.makedirs(plotsoutputpath + vname)
  if not os.path.isfile(plotsoutputpath + vname + "/index.php"):
    shutil.copy2(plotsoutputpath + "index.php", plotsoutputpath + vname + "/index.php")
  vnametitle = htemp.GetName()[1:]
  htemp.Scale(100)
  htemp.SetStats(False)
  c = r.TCanvas('c', "", 700, 500)
  plot = c.GetPad(0);
  plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.06); plot.SetRightMargin(0.08); plot.SetLeftMargin(0.075)
  htemp.SetMarkerSize(markersize)
  htemp.SetMarkerColor(r.kRed)
  htemp.Draw("colz text e")
  r.gStyle.SetPaintTextFormat("4.3f")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".png")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".eps")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".pdf")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".root")
  del c, plot
  
  c = r.TCanvas('c', "", 700, 500)
  plot = c.GetPad(0);
  plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.06); plot.SetRightMargin(0.08); plot.SetLeftMargin(0.075)
  htemp.Draw("colz")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.png")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.eps")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.pdf")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.root")
  del c, plot
  
  if (prof == 0):
    return
  
  hX          = r.TProfile('hX', '', nxb, xb)
  hY          = r.TProfile('hY', '', nyb, yb)
  htemp.ProfileX("hX", 1, -1, "s")
  htemp.ProfileY("hY", 1, -1, "s")
  
  c       = r.TCanvas('c', "X-Profiled response matrix - T" + vnametitle)
  hX.SetStats(False)
  hX.SetXTitle("X/Gen. value")
  hX.SetYTitle("Mean Y/Reco. value")
  hX.Draw()
  c.SaveAs(plotsoutputpath + vname + "/P_X_T" + vnametitle + ".png")
  del c, hX
  
  c = r.TCanvas('c', "Y-Profiled response matrix - T" + vnametitle)
  hY.SetStats(False)
  hY.SetXTitle("Y/Reco. value")
  hY.SetYTitle("Mean X/Gen. value")
  hY.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  c.SaveAs(plotsoutputpath + vname + "/P_Y_T" + vnametitle + ".png")
  del c, hY
  
  hStab   = r.TH1F('hStab', '', nxb, xb)
  for i in range(1, hStab.GetNbinsX() + 1):
    hStab.SetBinContent(i, stab[i - 1])
  hPur    = r.TH1F('hPur',  '', nyb, yb)
  for j in range(1, hPur.GetNbinsX() + 1):
    hPur.SetBinContent(j, pur[j - 1])
  
  c = r.TCanvas('c', "Purities and stabilities of " + vnametitle)
  plot = c.GetPad(0);
  #plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.06); plot.SetRightMargin(0.05); plot.SetLeftMargin(0.075)
  
  hPur.SetXTitle(vl.varList[vname]['xaxis'])
  hPur.SetStats(False)
  hPur.SetYTitle("Purities and stabilities")
  hStab.SetLineColor(r.kBlue)
  hPur.SetLineColor(r.kRed)
  hPur.SetMaximum(1.)
  hPur.SetMinimum(0.)
  hPur.Draw('')
  hStab.Draw('same')
  r.gPad.Update()
  
  textSize      = 0.022
  legWidth      = 0.05
  height        = .11
  (x1,y1,x2,y2) = (.85-legWidth, .95 - height, .90, .92)
  l             = r.TLegend(x1, y1, x2, y2);
  l.AddEntry(hStab, 'Stability')
  l.AddEntry(hPur, 'Purity')
  l.SetTextFont(42)
  l.SetTextSize(textSize)
  l.SetBorderSize(0)
  l.SetFillColor(10)
  l.Draw()
  r.gPad.Update()
  
  c.SaveAs(plotsoutputpath + vname + "/PurStab_" + vnametitle + ".png")
  c.SaveAs(plotsoutputpath + vname + "/PurStab_" + vnametitle + ".pdf")
  c.SaveAs(plotsoutputpath + vname + "/PurStab_" + vnametitle + ".root")
  del c, hStab, hPur, plot
  return


def PrintFiducialHisto(htemp, vname):
  '''This function prints the fiducial histo of a variable.'''
  if not os.path.exists(plotsoutputpath + vname):
    os.makedirs(plotsoutputpath + vname)
  if not os.path.isfile(plotsoutputpath + vname + "/index.php"):
    shutil.copy2(plotsoutputpath + "index.php", plotsoutputpath + vname + "/index.php")
  vnametitle = htemp.GetName()[1:]
  c = r.TCanvas('c', "Fiducial histogram - T" + vnametitle, 200, 10, 700, 500)
  #htemp.SetStats(False)
  htemp.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".png")
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".pdf")
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".root")
  del c
  return


# ---------------------------------------------------------------- BINNING IMPORTING FROM varList
VarBins_X = []
VarBins_Y = []

for i in range(len(VarNames)):
  VarBins_X.append(array('f', vl.varList[VarNames[i]]['genbinning']))
  VarBins_Y.append(array('f', vl.varList[VarNames[i]]['recobinning']))

xmin    = [int(round(i[0]))  for i in VarBins_X]
xmax    = [int(round(i[-1])) for i in VarBins_Y]
ymin    = xmin
ymax    = xmax
nybins  = [len(i)-1  for i in VarBins_Y]
nxbins  = [int(i/2) for i in nybins]  ### IMPORTANT!!!! The relation 1(gen):2(reco) in the number of bins MUST be preserved


# ---------------------------------------------------------------- INFO IMPORTING FROM MINITREES
print("\n> Importing minitrees' information...")
fTW               = r.TFile.Open(minipath + "Tree_UNF_TW.root")
fTW_noFully       = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr.root")
fTW_DSUp          = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_DS.root")
fTW_fsrUp         = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_fsrUp.root")
fTW_fsrDown       = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_fsrDown.root")
fTW_isrUp         = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_isrUp.root")
fTW_isrDown       = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_isrDown.root")
fTW_MEUp          = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_MEscaleUp.root")
fTW_MEDown        = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_MEscaleDown.root")
fTW_PSUp          = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_PSscaleUp.root")
fTW_PSDown        = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_PSscaleDown.root")

fTbarW            = r.TFile.Open(minipath + "Tree_UNF_TbarW.root")
fTbarW_noFully    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr.root")
fTbarW_DSUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_DS.root")
fTbarW_fsrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrUp.root")
fTbarW_fsrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrDown.root")
fTbarW_isrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrUp.root")
fTbarW_isrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrDown.root")
fTbarW_MEUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleUp.root")
fTbarW_MEDown     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleDown.root")
fTbarW_PSUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_PSscaleUp.root")
fTbarW_PSDown     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_PSscaleDown.root")

treeTW            = fTW.Get('Mini1j1t')
treeTW_noFully    = fTW_noFully.Get('Mini1j1t')
treeTW_DSUp       = fTW_DSUp.Get('Mini1j1t')
treeTW_DSUp.SetName("DSUp")
treeTW_fsrUp      = fTW_fsrUp.Get('Mini1j1t')
treeTW_fsrUp.SetName("fsrUp")
treeTW_fsrDown    = fTW_fsrDown.Get('Mini1j1t')
treeTW_fsrDown.SetName("fsrDown")
treeTW_isrUp      = fTW_isrUp.Get('Mini1j1t')
treeTW_isrUp.SetName("isrUp")
treeTW_isrDown    = fTW_isrDown.Get('Mini1j1t')
treeTW_isrDown.SetName("isrDown")
treeTW_MEUp       = fTW_MEUp.Get('Mini1j1t')
treeTW_MEUp.SetName("tWMEUp")
treeTW_MEDown     = fTW_MEDown.Get('Mini1j1t')
treeTW_MEDown.SetName("tWMEDown")
treeTW_PSUp       = fTW_PSUp.Get('Mini1j1t')
treeTW_PSUp.SetName("tWPSUp")
treeTW_PSDown     = fTW_PSDown.Get('Mini1j1t')
treeTW_PSDown.SetName("tWPSDown")

treeTbarW         = fTbarW.Get('Mini1j1t')
treeTbarW_noFully = fTbarW_noFully.Get('Mini1j1t')
treeTbarW_DSUp    = fTbarW_DSUp.Get('Mini1j1t')
treeTbarW_DSUp.SetName("DSUp")
treeTbarW_fsrUp   = fTbarW_fsrUp.Get('Mini1j1t')
treeTbarW_fsrUp.SetName("fsrUp")
treeTbarW_fsrDown = fTbarW_fsrDown.Get('Mini1j1t')
treeTbarW_fsrDown.SetName("fsrDown")
treeTbarW_isrUp   = fTbarW_isrUp.Get('Mini1j1t')
treeTbarW_isrUp.SetName("isrUp")
treeTbarW_isrDown = fTbarW_isrDown.Get('Mini1j1t')
treeTbarW_isrDown.SetName("isrDown")
treeTbarW_MEUp    = fTbarW_MEUp.Get('Mini1j1t')
treeTbarW_MEUp.SetName("tWMEUp")
treeTbarW_MEDown  = fTbarW_MEDown.Get('Mini1j1t')
treeTbarW_MEDown.SetName("tWMEDown")
treeTbarW_PSUp    = fTbarW_PSUp.Get('Mini1j1t')
treeTbarW_PSUp.SetName("tWPSUp")
treeTbarW_PSDown  = fTbarW_PSDown.Get('Mini1j1t')
treeTbarW_PSDown.SetName("tWPSDown")


print "\n> Drawing matrices and writing ROOT file (old one will be overwritten!)."
print "The matrices will be saved in " + str(matrixoutputpath)
print "The plots will be drawn in " + str(plotsoutputpath)
f       = r.TFile(matrixoutputpath + "UnfoldingInfo.root", "recreate")
for i in range(nvars):
  print("\n    - Drawing and saving the response matrices of the variable "+ VarNames[i] + " ...")
  # Normal response matrices
  htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  #htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i], 1, purities[i], stabilities[i])
  htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i])
  #htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  
  # JES, JER and weight-related systematics response matrices
  for j in range(nsys):
    htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j])
    #htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], SysList[j])
    #htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintFiducialHisto(htemp, VarNames[i])
  
  # Modeling systematics response matrices
  htemp = GetResponseMatrix(treeTW_DSUp,    treeTbarW_DSUp,       VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_DSUp,     treeTbarW_DSUp,       VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrUp,    treeTbarW_fsrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrDown,  treeTbarW_fsrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrDown,  treeTbarW_isrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrUp,    treeTbarW_isrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_MEUp,    treeTbarW_MEUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_MEUp,     treeTbarW_MEUp,     VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_MEDown,  treeTbarW_MEDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_MEDown,   treeTbarW_MEDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_PSUp,    treeTbarW_PSUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_PSUp,     treeTbarW_PSUp,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_PSDown,  treeTbarW_PSDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_PSDown,   treeTbarW_PSDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])

f.Write()
f.Close()
  
print("> Done!")
