import ROOT as r
import sys, os, shutil
import varList
from math   import pi
from array  import array

r.gROOT.SetBatch(True)


#############################
print("\n===== Unfolding procedures: Response matrices & ROOT files production =====")
print("> Setting binning, paths, and other details...")
# ---------------------------------------------------------------- PRELIMINARIES
storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
minipath    = "../../../TW_temp/"
if (len(sys.argv) > 1):
  print("    - Manual minitrees' folder input!\n")
  if (sys.argv[1] == "last"):
    minipath  = varList.GetLastFolder(storagepath)
  else:
    minipath  = storagepath + sys.argv[1] + "/"

print "    - The minitrees' path is:\n", minipath
plotsoutputpath  = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
matrixoutputpath = "./temp/"
genCut      = "TWeight_normal * (Tpassgen == 1)"
recoCut     = "TWeight_normal * (Tpassreco == 1)"
Cut         = "TWeight * (Tpassgen == 1)"
fiduCut     = "TWeight * ((Tpassreco == 1) && (Tpassgen == 0))"

VarNames    = varList.varList["Names"]["Variables"]
nvars       = len(VarNames)

SysList     = varList.varList["Names"]["ExpSysts"]
nsys        = len(SysList)

purities    = []
stabilities = []

def GetResponseMatrix(t1, t2, vname, nxb, xb, nyb, yb, sys = ""):
  '''This function obtains the response matrix combining info. of two trees.'''
  hGen1 = r.TH1F('hGen1', '', nxb, xb)
  hGen2 = r.TH1F('hGen2', '', nxb, xb)
  
  vnamegen    = vname # Generation name (without any kind of systematic thing)
  vnamereco   = vname # Reconstruction name (can have a systematic-name tail)
  vnametitle  = vname # Name to be shown as the title (and name) of the histogram
  tmpcut      = Cut   # cuts
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   += sys
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling"):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  t1.Draw('TGen'+vnamegen+'>>hGen1', genCut)
  t2.Draw('TGen'+vnamegen+'>>hGen2', genCut)

  hGen1.Add(hGen2)
  hGen2 = None
  hGen  = r.TH2F('Gen', '', nxb, xb, nyb, yb)
  
  for i in range(0, nxb+2):
    for j in range(0, nyb+2):
      hGen.SetBinContent(i, j, hGen1.GetBinContent(i))
  
  h1    = r.TH2F('h1', "Response matrix - " + vnametitle, nxb, xb, nyb, yb)
  h2    = r.TH2F('h2', '',                                nxb, xb, nyb, yb)
  
  t1.Project('h1', "T" + vnamereco + ":TGen" + vnamegen, tmpcut)
  t2.Project('h2', "T" + vnamereco + ":TGen" + vnamegen, tmpcut)
  h1.Add(h2)
  h2      = None
  if (sys == ""):
    hReco1 = r.TH1F('hReco1', '', nyb, yb)
    hReco2 = r.TH1F('hReco2', '', nyb, yb)
    t1.Draw('T' + vnamereco + '>>hReco1', recoCut)
    t2.Draw('T' + vnamereco + '>>hReco2', recoCut)
    hReco1.Add(hReco2)
    hReco2  = None
    
    tmppur  = []
    tmpstab = []
    for i in range(1, nxb+1):
      sumstab = 0
      for j in range(1, nyb+1):
        sumstab += h1.GetBinContent(i, j)
      try: tmpstab.append(sumstab/hGen1.GetBinContent(i))
      except ZeroDivisionError: tmpstab.append(0)
    stabilities.append(tmpstab)
    tmpstab = None
    for j in range(1, nyb+1):
      sumpur  = 0
      for i in range(1, nxb+1):
        sumpur  += h1.GetBinContent(i, j)
      try: tmppur.append(sumpur/hReco1.GetBinContent(j))
      except ZeroDivisionError: tmppur.append(0)
    purities.append(tmppur)
    tmppur  = None
    hReco1  = None
  
  hGen1 = None
  
  #print "PRINTING INFO RELATED TO UNDER/OVERFLOW: GENINFO"
  #for i in range(nxb+2):
    #print(str(i) + ", 0 : " + str(hGen.GetBinContent(i, 0)))
  #for i in range(nxb+2):
    #print(str(i) + ", nybins+2 : "+ str(hGen.GetBinContent(i, nyb+2)))
  
  #for j in range(nyb+2):
    #print("0, " + str(j) + " : " + str(hGen.GetBinContent(0, j)))
  #for j in range(nyb+2):
    #print("nxbins+2, " +  str(j) + " : " + str(hGen.GetBinContent(nxb+2, j)))
  
  #print "PRINTING INFO RELATED TO UNDER/OVERFLOW: NUMINFO"
  #for i in range(nxb+2):
    #print(str(i) +", 0 : "+ str(h1.GetBinContent(i, 0)))
  #for i in range(nxb+2):
    #print(str(i)+ ", nybins+2 : "+ str(h1.GetBinContent(i, nyb+2)))
  
  #for j in range(nyb+2):
    #print("0, " + str(j)+ " : " +str(h1.GetBinContent(0, j)))
  #for j in range(nyb+2):
    #print("nxbins+2, "+ str(j)+" : "+ str(h1.GetBinContent(nxb+2, j)))
  
  h1.Divide(hGen)
  hGen  = None
  
  #print "PRINTING INFO RELATED TO UNDER/OVERFLOW: RESPONSEMATRIXINFO"
  #for i in range(nxb+2):
    #print(str(i) + ", 0 : " + str(h1.GetBinContent(i, 0)))
  #for i in range(nxb+2):
    #print(str(i) + ", nybins+2 : "+  str(h1.GetBinContent(i, nyb+2)))
  
  #for j in range(nyb+2):
    #print("0, " + str(j) + " : "+ str(h1.GetBinContent(0, j)))
  #for j in range(nyb+2):
    #print("nxbins+2, " +  str(j) + " : " +str(h1.GetBinContent(nxb+2, j)))
  
  #print "PRINTING SUMS OF COLUMNS"
  #for i in range(nxb+2):
    #tmpsum  = 0
    #for j in range(nyb+2):
      #tmpsum += h1.GetBinContent(i, j)
    #print(str(i) + " : " + str(tmpsum))
    
  
  #print "PRINTING SUMS OF LINES"
  #for j in range(nyb+2):
    #tmpsum  = 0
    #for i in range(nxb+2):
      #tmpsum += h1.GetBinContent(i, j)
    #print(str(j) + " : " + str(tmpsum))
    
  
  
  # Lower cutoff for the values of the matrices
  for i in range(1, nxb+1):
    for j in range(1, nyb+1):
      if (h1.GetBinContent(i, j) < 1e-4):
        h1.SetBinContent(i, j, 0)
  
  # Fixing the over and underflow bins to one.
  for i in range(1, nxb+1):
    tmpsum = 0.
    for j in range(1, nyb+1):
      tmpsum += h1.GetBinContent(i, j)
    h1.SetBinContent(i, 0, 0.)
    h1.SetBinContent(i, nyb + 1, 1 - tmpsum)
  for j in range(0, nyb+2):
    h1.SetBinContent(0, j, 0.)
    h1.SetBinContent(nxb+1, j, 0.)
  
  h1.SetXTitle("Generated events")
  h1.SetYTitle("Reconstructed events")
  h1.SetName("R" + vnametitle)
  return h1


def GetFiducialHisto(t1, t2, vname, nyb, yb, sys = ""):
  '''This function obtains the fiducial histograms combining info. of two trees.'''
  
  vnametitle  = vname   # Name to be shown as the title (and name) of the histogram
  vnamereco   = vname   # Reconstruction name (can have a systematic-name tail)
  tmpcut      = fiduCut # cuts
  
  h1 = r.TH1F('h1', "Fiducial histogram - T" + vnametitle, nyb, yb)
  h2 = r.TH1F('h2', '',                                    nyb, yb)
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   += sys
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling"):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  t1.Draw('T' + vnamereco + '>>h1', tmpcut)
  t2.Draw('T' + vnamereco + '>>h2', tmpcut)

  h1.Add(h2)
  h2 = None
  
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
  c = r.TCanvas('c', "Response matrix - T" + vnametitle, 200, 10, 700, 500)
  htemp.Scale(100)
  htemp.SetStats(False)
  htemp.Draw("colz text")
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".png")
  c     = None
  
  if (prof == 0):
    return
  
  hX          = r.TProfile('hX', '', nxb, xb)
  hY          = r.TProfile('hY', '', nyb, yb)
  htemp.ProfileX("hX", 1, -1, "s")
  htemp.ProfileY("hY", 1, -1, "s")
  
  c       = r.TCanvas('c', "X-Profiled response matrix - T" + vnametitle)
  hX.SetStats(False)
  hX.SetXTitle("Stabilities per bin")
  hX.SetYTitle("Mean Y/Reco. value")
  hX.Draw()
  for i in range(1, htemp.GetNbinsX()+1):
    hX.GetXaxis().SetBinLabel(i, "S = " + str(round(stab[i-1], 2)))
    #.ChangeLabel(i, -1, -1, -1, -1, -1, "S = " + str(round(stab[i], 2)))
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  secaxis = r.TGaxis(c.GetUxmin(), c.GetUymax(), c.GetUxmax(), c.GetUymax(), xmin, xmax, 510, "-")
  secaxis.Draw()
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/P_X_T" + vnametitle + ".png")
  c       = None
  hX      = None
  secaxis = None
  
  c = r.TCanvas('c', "Y-Profiled response matrix - T" + vnametitle)
  hY.SetStats(False)
  hY.SetXTitle("Purities per bin")
  hY.SetYTitle("Mean X/Gen. value")
  hY.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  for i in range(1, htemp.GetNbinsY() + 1):
    hY.GetXaxis().SetBinLabel(i, "P = " + str(round(pur[i-1], 2)))
    #secaxis.ChangeLabel(i, -1, -1, -1, -1, -1, "P = " + str(round(pur[i], 2)))
  r.gPad.Update()
  secaxis = r.TGaxis(c.GetUxmin(), c.GetUymax(), c.GetUxmax(), c.GetUymax(), ymin, ymax, 510, "-")
  secaxis.Draw()
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/P_Y_T" + vnametitle + ".png")
  c       = None
  hY      = None
  secaxis = None
  
  hStab   = r.TH1F('hStab', '', nxb, xb)
  for i in range(1, hStab.GetNbinsX() + 1):
    hStab.SetBinContent(i, stab[i - 1])
  hPur    = r.TH1F('hPur',  '', nyb, yb)
  for j in range(1, hPur.GetNbinsX() + 1):
    hPur.SetBinContent(j, pur[j - 1])
  
  c = r.TCanvas('c', "Purities and stabilities of " + vnametitle)
  hPur.SetXTitle(vnametitle)
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
  height        = .15
  (x1,y1,x2,y2) = (.85-legWidth, .9 - height, .90, .89)
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
  c     = None
  hStab = None
  hPur  = None
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
  c     = None
  return


# ---------------------------------------------------------------- BINNING IMPORTING FROM varList
VarBins_X = []
VarBins_Y = []

#print varList.varList[VarNames[3]]['genbinning']

for i in range(len(VarNames)):
  VarBins_X.append(array('f', varList.varList[VarNames[i]]['genbinning']))
  VarBins_Y.append(array('f', varList.varList[VarNames[i]]['recobinning']))

xmin    = [int(round(i[0]))  for i in VarBins_X]
xmax    = [int(round(i[-1])) for i in VarBins_Y]
ymin    = xmin
ymax    = xmax
nybins  = [len(i)-1  for i in VarBins_Y]
nxbins  = [int(i/2) for i in nybins]  ### IMPORTANT!!!! The relation 1(gen):2(reco) in the number of bins MUST be preserved


# ---------------------------------------------------------------- INFO IMPORTING FROM MINITREES
print("\n> Importing minitrees' information...")
fTW               = r.TFile.Open(minipath + "Tree_UNF_TW.root")
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
treeTW_MEUp.SetName("MEUp")
treeTW_MEDown     = fTW_MEDown.Get('Mini1j1t')
treeTW_MEDown.SetName("MEDown")
treeTW_PSUp       = fTW_PSUp.Get('Mini1j1t')
treeTW_PSUp.SetName("PSUp")
treeTW_PSDown     = fTW_PSDown.Get('Mini1j1t')
treeTW_PSDown.SetName("PSDown")

treeTbarW         = fTbarW.Get('Mini1j1t')
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
treeTbarW_MEUp.SetName("MEUp")
treeTbarW_MEDown  = fTbarW_MEDown.Get('Mini1j1t')
treeTbarW_MEDown.SetName("MEDown")
treeTbarW_PSUp    = fTbarW_PSUp.Get('Mini1j1t')
treeTbarW_PSUp.SetName("PSUp")
treeTbarW_PSDown  = fTbarW_PSDown.Get('Mini1j1t')
treeTbarW_PSDown.SetName("PSDown")


print "\n> Drawing matrices and writing ROOT file (old one will be overwritten!)."
print "The matrices will be saved in " + str(matrixoutputpath)
print "The plots will be drawn in " + str(plotsoutputpath)
f       = r.TFile(matrixoutputpath + "UnfoldingInfo.root", "recreate")
for i in range(nvars):
  print("\n    - Drawing and saving the response matrices of the variable "+ VarNames[i] + " ...")
  # Normal response matrices
  htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i], 1, purities[i], stabilities[i])
  htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  
  # JES, JER and weight-related systematics response matrices
  for j in range(nsys):
    htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintFiducialHisto(htemp, VarNames[i])
  
  # Modeling systematics response matrices
  htemp = GetResponseMatrix(treeTW_DSUp,    treeTbarW_DSUp,       VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_DSUp,     treeTbarW_DSUp,       VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrUp,    treeTbarW_fsrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrDown,  treeTbarW_fsrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrDown,  treeTbarW_isrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrUp,    treeTbarW_isrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling")
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

treeTW            = None
treeTW_DSUp       = None
treeTW_fsrDown    = None
treeTW_fsrUp      = None
treeTW_isrDown    = None
treeTW_isrUp      = None
treeTW_MEDown     = None
treeTW_MEUp       = None
treeTW_PSDown     = None
treeTW_PSUp       = None

treeTbarW         = None
treeTbarW_DSUp    = None
treeTbarW_fsrDown = None
treeTbarW_fsrUp   = None
treeTbarW_isrDown = None
treeTbarW_isrUp   = None
treeTbarW_MEDown  = None
treeTbarW_MEUp    = None
treeTbarW_PSDown  = None
treeTbarW_PSUp    = None

fTW               = None
fTW_DSUp          = None
fTW_fsrUp         = None
fTW_fsrDown       = None
fTW_isrUp         = None
fTW_isrDown       = None
fTW_MEUp          = None
fTW_MEDown        = None
fTW_PSUp          = None
fTW_PSDown        = None

fTbarW            = None
fTbarW_DSUp       = None
fTbarW_fsrUp      = None
fTbarW_fsrDown    = None
fTbarW_isrUp      = None
fTbarW_isrDown    = None
fTbarW_MEUp       = None
fTbarW_MEDown     = None
fTbarW_PSUp       = None
fTbarW_PSDown     = None

htemp             = None

f.Write()
f.Close()
  
print("> Done!")
  
