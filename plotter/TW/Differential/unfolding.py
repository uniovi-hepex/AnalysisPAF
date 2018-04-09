#from numpy              import *
#from scipy              import *
#import matplotlib.pyplot as plt
import ROOT as r
import sys
from math   import *
from array  import array

r.gROOT.SetBatch(True)

#############################
print("\n===== Unfolding procedures - Response matrices & ROOT files production =====")
print("> Setting binning, paths, and other details...")
# ---------------------------------------------------------------- PRELIMINARIES
minipath    = "../../../TW_temp/"
outputpath  = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
genCut      = "TWeight_normal * (Tpassgen == 1)"
Cut         = "TWeight * ((Tpassreco == 1) && (Tpassgen == 1))"

VarNames    = ["E_LLB", "LeadingJetE", "MT_LLMETB", "M_LLB", "M_LeadingB", "M_SubLeadingB", 
               "MET", "MET_Phi", "LeadingJetPt", "LeadingJetEta", "LeadingJetPhi", 
               "LeadingLepE", "LeadingLepPt", "LeadingLepPhi", "LeadingLepEta", 
               "SubLeadingLepE", "SubLeadingLepPt", "SubLeadingLepPhi", "SubLeadingLepEta",
               "DilepPt", "DilepJetPt", "DilepMETJetPt", "HTtot"]
nvars       = len(VarNames)

SysList     = ["JESUp", "JESDown", "JERUp", "ElecEffUp", "ElecEffDown", "MuonEffUp",
               "MuonEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "BtagUp",
               "BtagDown", "MistagUp", "MistagDown", "FragUp", "FragDown", "PetersonFrag",
               "semilepbrUp", "semilepbrDown"]
nsys        = len(SysList)

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
    vnametitle  += sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys)
    vnametitle  += sys
  elif (sys == "modeling"):
    vnametitle  += t1.GetName()
  
  t1.Draw('TGen'+vnamegen+'>>hGen1', genCut)
  t2.Draw('TGen'+vnamegen+'>>hGen2', genCut)

  hGen1.Add(hGen2)
  hGen2 = None
  hGen  = r.TH2F('Gen', '', nxb, xb, nyb, yb)
  
  for i in range(1, nxb+1):
    for j in range(1, nyb+1):
      hGen.SetBinContent(i, j, hGen1.GetBinContent(i))
  
  hGen1 = None
  h1    = r.TH2F('h1', "Response matrix - " + vnametitle, nxb, xb, nyb, yb)
  h2    = r.TH2F('h2', '',                                nxb, xb, nyb, yb)
  
  t1.Project('h1', "T" + vnamereco + ":TGen" + vnamegen, tmpcut)
  t2.Project('h2', "T" + vnamereco + ":TGen" + vnamegen, tmpcut)
  
  h1.Add(h2)
  h1.Divide(hGen)
  h2    = None
  hGen  = None
  
  h1.SetXTitle("Generated events")
  h1.SetYTitle("Reconstructed events")
  h1.SetName(vnametitle)
  
  return h1


def PrintResponseMatrix(htemp, vname, nxb, xmin, xmax, nyb, ymin, ymax, prof = 0):
  '''This function prints the response matrix of a given histogram.'''
  c = r.TCanvas('c', "Response matrix - T" + vname, 200, 10, 700, 500)
  htemp.Scale(100)
  htemp.SetStats(False)
  htemp.Draw("colz text")
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(outputpath + "R_T" + vname + ".png")
  c     = None
  
  if (prof == 0):
    return
  
  hX          = r.TProfile('hX', '', nxb, xmin, xmax)
  hY          = r.TProfile('hY', '', nyb, ymin, ymax)
  htemp.ProfileX("hX", 1, -1, "s")
  htemp.ProfileY("hY", 1, -1, "s")
  htemp = None
  
  c = r.TCanvas('c', "X-Profiled response matrix - T" + vname, 200, 10, 700, 500)
  hX.SetStats(False)
  hX.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(outputpath + "P_X_T" + vname + ".png")
  c     = None
  hX    = None
  
  c = r.TCanvas('c', "Y-Profiled response matrix - T" + vname, 200, 10, 700, 500)
  hY.SetStats(False)
  hY.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(outputpath + "P_Y_T" + vname + ".png")
  c     = None
  hY    = None
  
  return

# ---------------------------------------------------------------- BINNING SETTING
# ------------------------------------------> Generation / X axis
VarBins_X         = []
B_E_LLB           = array('f', [0, 150, 200, 250, 300, 350, 400, 500, 600, 700, 1000])
VarBins_X.append(B_E_LLB)
B_LeadingJetE     = array('f', [0, 75, 125, 175, 250, 320, 420, 500])
VarBins_X.append(B_LeadingJetE)
B_MT_LLMETB       = array('f', [0, 150, 200, 250, 300, 350, 400, 450, 500, 800])
VarBins_X.append(B_MT_LLMETB)
B_M_LLB           = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 800])
VarBins_X.append(B_M_LLB)
B_M_LeadingB      = array('f', [0, 75, 125, 175, 225, 300, 500])
VarBins_X.append(B_M_LeadingB)
B_M_SubLeadingB   = array('f', [0, 25, 50, 75, 100, 125, 150, 175, 500])
VarBins_X.append(B_M_SubLeadingB)

B_MET             = array('f', [0, 25, 50, 75, 100, 125, 500])
VarBins_X.append(B_MET)
B_MET_Phi         = array('f', [-pi, -2, -1, 0, 1, 2, pi])
VarBins_X.append(B_MET_Phi)
B_LeadingJetPt    = array('f', [30, 50, 70, 90, 110, 130, 250])
VarBins_X.append(B_LeadingJetPt)
B_LeadingJetEta   = array('f', [-2.4, -1, 0, 1, 2.4])
VarBins_X.append(B_LeadingJetEta)
B_LeadingJetPhi   = array('f', [-pi, -2, -1, 0, 1, 2, pi])
VarBins_X.append(B_LeadingJetPhi)

B_LeadingLepE     = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500])
VarBins_X.append(B_LeadingLepE)
B_LeadingLepPt    = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500])
VarBins_X.append(B_LeadingLepPt)
B_LeadingLepPhi   = array('f', [-pi, -2, -1, 0, 1, 2, pi])
VarBins_X.append(B_LeadingLepPhi)
B_LeadingLepEta   = array('f', [-2.4, -1, 0, 1, 2.4])
VarBins_X.append(B_LeadingLepEta)

B_SubLeadingLepE  = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500])
VarBins_X.append(B_SubLeadingLepE)
B_SubLeadingLepPt = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500])
VarBins_X.append(B_SubLeadingLepPt)
B_SubLeadingLepPhi= array('f', [-pi, -2, -1, 0, 1, 2, pi])
VarBins_X.append(B_SubLeadingLepPhi)
B_SubLeadingLepEta= array('f', [-2.4, -1, 0, 1, 2.4])
VarBins_X.append(B_SubLeadingLepEta)

B_DilepPt         = array('f', [0, 25, 50, 75, 100, 125, 150, 500])
VarBins_X.append(B_DilepPt)
B_DilepJetPt      = array('f', [0, 20, 40, 60, 80, 100, 120, 140, 250])
VarBins_X.append(B_DilepJetPt)
B_DilepMETJetPt   = array('f', [0, 10, 20, 30, 40, 50, 60, 70, 80, 250])
VarBins_X.append(B_DilepMETJetPt)
B_HTtot           = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 550])
VarBins_X.append(B_HTtot)


# ------------------------------------------> Reconstruction / Y axis
VarBins_Y         = []
B_E_LLB           = array('f', [0, 150, 250, 350, 500, 700, 1000])
VarBins_Y.append(B_E_LLB)
B_LeadingJetE     = array('f', [0, 100, 200, 300, 420, 500])
VarBins_Y.append(B_LeadingJetE)
B_MT_LLMETB       = array('f', [0, 150, 250, 350, 500, 800])
VarBins_Y.append(B_MT_LLMETB)
B_M_LLB           = array('f', [0, 50, 150, 250, 400, 800])
VarBins_Y.append(B_M_LLB)
B_M_LeadingB      = array('f', [0, 75, 175, 300, 500])
VarBins_Y.append(B_M_LeadingB)
B_M_SubLeadingB   = array('f', [0, 25, 75, 125, 175, 500])
VarBins_Y.append(B_M_SubLeadingB)

B_MET             = array('f', [0, 25, 75, 125, 500])
VarBins_Y.append(B_MET)
B_MET_Phi         = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_Y.append(B_MET_Phi)
B_LeadingJetPt    = array('f', [30, 70, 110, 150, 250])
VarBins_Y.append(B_LeadingJetPt)
B_LeadingJetEta   = array('f', [-2.4, 0, 2.4])
VarBins_Y.append(B_LeadingJetEta)
B_LeadingJetPhi   = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_Y.append(B_LeadingJetPhi)

B_LeadingLepE     = array('f', [0, 100, 200, 300, 400, 500])
VarBins_Y.append(B_LeadingLepE)
B_LeadingLepPt    = array('f', [0, 100, 200, 300, 400, 500])
VarBins_Y.append(B_LeadingLepPt)
B_LeadingLepPhi   = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_Y.append(B_LeadingLepPhi)
B_LeadingLepEta   = array('f', [-2.4, 0, 2.4])
VarBins_Y.append(B_LeadingLepEta)

B_SubLeadingLepE  = array('f', [0, 100, 200, 300, 400, 500])
VarBins_Y.append(B_SubLeadingLepE)
B_SubLeadingLepPt = array('f', [0, 100, 200, 300, 400, 500])
VarBins_Y.append(B_SubLeadingLepPt)
B_SubLeadingLepPhi= array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_Y.append(B_SubLeadingLepPhi)
B_SubLeadingLepEta= array('f', [-2.4, 0, 2.4])
VarBins_Y.append(B_SubLeadingLepEta)

B_DilepPt         = array('f', [0, 50, 100, 150, 500])
VarBins_Y.append(B_DilepPt)
B_DilepJetPt      = array('f', [0, 50, 100, 150, 250])
VarBins_Y.append(B_DilepJetPt)
B_DilepMETJetPt   = array('f', [0, 20, 40, 60, 80, 250])
VarBins_Y.append(B_DilepMETJetPt)
B_HTtot           = array('f', [0, 100, 200, 300, 400, 550])
VarBins_Y.append(B_HTtot)


# ------------------------------------------> Number of bins & limits
xmin    = [0,     0,    0,    0,    0,    0,
           0,   -pi,   30, -2.4,  -pi,
           0,     0,    -pi,   -2.4,
           0,     0,    -pi,   -2.4,
           0,     0,    0,    0]
xmax    = [1000,  500,  800,  800,  500,  500,
           500,   pi,   250,  2.4,    pi,
           500,   500,  pi,    2.4,
           500,   500,  pi,    2.4,
           500,   250,  250,   550]
ymin    = xmin
ymax    = xmax
nxbins  = [10, 7,  9,  9, 6, 8,
           6,  6,  6,  4, 6,
           10, 10, 6,  4,
           10, 10, 6,  4,
           7,  8,  9,  9]
nybins  = [6,  5,  5,  5, 4, 5,
           4,  4,  4,  2, 4,
           5,  5,  4,  2,
           5,  5,  4,  2,
           4,  4,  5,  5]


print("\n> Importing minitrees' information...")
fTW               = r.TFile.Open(minipath + "Tree_TW.root")
fTW_DS            = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_DS.root")
fTW_fsrUp         = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_fsrUp.root")
fTW_fsrDown       = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_fsrDown.root")
fTW_isrUp         = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_isrUp.root")
fTW_isrDown       = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_isrDown.root")
fTW_MEUp          = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_MEscaleUp.root")
fTW_MEDown        = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_MEscaleDown.root")
fTW_PSUp          = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_PSscaleUp.root")
fTW_PSDown        = r.TFile.Open(minipath + "Tree_TW_noFullyHadr_PSscaleDown.root")

fTbarW            = r.TFile.Open(minipath + "Tree_TbarW.root")
fTbarW_DS         = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_DS.root")
fTbarW_fsrUp      = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_fsrUp.root")
fTbarW_fsrDown    = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_fsrDown.root")
fTbarW_isrUp      = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_isrUp.root")
fTbarW_isrDown    = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_isrDown.root")
fTbarW_MEUp       = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_MEscaleUp.root")
fTbarW_MEDown     = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_MEscaleDown.root")
fTbarW_PSUp       = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_PSscaleUp.root")
fTbarW_PSDown     = r.TFile.Open(minipath + "Tree_TbarW_noFullyHadr_PSscaleDown.root")

treeTW            = fTW.Get('Mini1j1t')
treeTW_DS         = fTW_DS.Get('Mini1j1t')
treeTW_DS.SetName("DS")
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
treeTbarW_DS      = fTbarW_DS.Get('Mini1j1t')
treeTbarW_DS.SetName("DS")
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


print("\n> Drawing response matrices in "+str(outputpath))
for i in range(nvars):
  print("\n    - Drawing the variable "+VarNames[i]+" ...")
  htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = None


print("\n> Writing ROOT file (old one will be overwritten!) with the matrices and other information in "+str(outputpath)+" ...")

f       = r.TFile(outputpath + "UnfoldingInfo.root", "recreate")
for i in range(nvars):
  # Normal response matrices
  htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  
  # JES, JER and weight-related systematics response matrices
  for j in range(nsys):
    htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
  
  # Modeling systematics response matrices
  htemp = GetResponseMatrix(treeTW_DS,      treeTbarW_DS,       VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_MEUp,    treeTbarW_MEUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_MEDown,  treeTbarW_MEDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_PSUp,    treeTbarW_PSUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  htemp = GetResponseMatrix(treeTW_PSDown,  treeTbarW_PSDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()

treeTW            = None
treeTW_DS         = None
treeTW_fsrDown    = None
treeTW_fsrUp      = None
treeTW_isrDown    = None
treeTW_isrUp      = None
treeTW_MEDown     = None
treeTW_MEUp       = None
treeTW_PSDown     = None
treeTW_PSUp       = None

treeTbarW         = None
treeTbarW_DS      = None
treeTbarW_fsrDown = None
treeTbarW_fsrUp   = None
treeTbarW_isrDown = None
treeTbarW_isrUp   = None
treeTbarW_MEDown  = None
treeTbarW_MEUp    = None
treeTbarW_PSDown  = None
treeTbarW_PSUp    = None

fTW               = None
fTW_DS            = None
fTW_fsrUp         = None
fTW_fsrDown       = None
fTW_isrUp         = None
fTW_isrDown       = None
fTW_MEUp          = None
fTW_MEDown        = None
fTW_PSUp          = None
fTW_PSDown        = None

fTbarW            = None
fTbarW_DS         = None
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
  
