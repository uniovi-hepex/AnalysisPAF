import ROOT as r
import sys
import os
import shutil
from math   import *
from array  import array

r.gROOT.SetBatch(True)


def GetLastFolder(stpth):
    savefolders   = next(os.walk(stpth))[1]
    saveyears     = map(int, [i[6:]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[6:]) == max(saveyears)]
    savemonths    = map(int, [i[3:5] for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[3:5]) == max(savemonths)]
    savedays      = map(int, [i[:2]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[:2]) == max(savedays)]
    return (stpth + savefolders[0] + "/")

#############################
print("\n===== Unfolding procedures: Response matrices & ROOT files production =====")
print("> Setting binning, paths, and other details...")
# ---------------------------------------------------------------- PRELIMINARIES
storagepath = "/nfs/fanae/user/vrbouza/Storage/TW/MiniTrees/"
minipath    = "../../../TW_temp/"
if (len(sys.argv) > 1):
  print("    - Manual minitrees' folder input!\n")
  if (sys.argv[1] == "last"):
    minipath  = GetLastFolder(storagepath)
  else:
    minipath  = storagepath + sys.argv[1] + "/"
print "    - The minitrees' path is:\n", minipath
plotsoutputpath  = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
matrixoutputpath = "./temp/"
genCut      = "TWeight_normal * (Tpassgen == 1)"
recoCut     = "TWeight_normal * (Tpassreco == 1)"
#Cut         = "TWeight * ((Tpassreco == 1) && (Tpassgen == 1))"
Cut         = "TWeight * (Tpassgen == 1)"
fiduCut     = "TWeight * ((Tpassreco == 1) && (Tpassgen == 0))"

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
  
  t1.Draw('T' + vnamereco + '>>h1', tmpcut)
  t2.Draw('T' + vnamereco + '>>h2', tmpcut)

  h1.Add(h2)
  h2 = None
  
  h1.SetXTitle("Fiducial events")
  h1.SetYTitle("Events")
  h1.SetName("F" + vnametitle)
  
  return h1


def PrintResponseMatrix(htemp, vname, nxb, xmin, xmax, nyb, ymin, ymax, prof = 0, pur = None, stab = None):
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
  
  hX          = r.TProfile('hX', '', nxb, xmin, xmax)
  hY          = r.TProfile('hY', '', nyb, ymin, ymax)
  htemp.ProfileX("hX", 1, -1, "s")
  htemp.ProfileY("hY", 1, -1, "s")
  
  c       = r.TCanvas('c', "X-Profiled response matrix - T" + vnametitle)
  hX.SetStats(False)
  hX.SetXTitle("Stabilities per bin")
  hX.SetYTitle("Events")
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
  hY.SetYTitle("Events")
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


# ---------------------------------------------------------------- BINNING SETTING
# ------------------------------------------> Generation / X axis
VarBins_X         = []
B_E_LLB           = array('f', [0, 200, 350, 500, 700, 1000])
VarBins_X.append(B_E_LLB)
B_LeadingJetE     = array('f', [0, 100, 200, 300, 420, 1000])
VarBins_X.append(B_LeadingJetE)
B_MT_LLMETB       = array('f', [0, 150, 250, 350, 500, 1000])
VarBins_X.append(B_MT_LLMETB)
B_M_LLB           = array('f', [0, 50, 150, 250, 400, 1000])
VarBins_X.append(B_M_LLB)
B_M_LeadingB      = array('f', [0, 75, 175, 300, 1000])
VarBins_X.append(B_M_LeadingB)
B_M_SubLeadingB   = array('f', [0, 25, 75, 125, 175, 1000])
VarBins_X.append(B_M_SubLeadingB)

B_MET             = array('f', [0, 25, 75, 125, 1000])
VarBins_X.append(B_MET)
B_MET_Phi         = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_X.append(B_MET_Phi)
B_LeadingJetPt    = array('f', [0, 50, 100, 150, 500])
VarBins_X.append(B_LeadingJetPt)
B_LeadingJetEta   = array('f', [-2.4, -1.2, 0, 1.2, 2.4])
VarBins_X.append(B_LeadingJetEta)
B_LeadingJetPhi   = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_X.append(B_LeadingJetPhi)

B_LeadingLepE     = array('f', [0, 100, 200, 300, 400, 1000])
VarBins_X.append(B_LeadingLepE)
B_LeadingLepPt    = array('f', [0, 100, 200, 300, 400, 1000])
VarBins_X.append(B_LeadingLepPt)
B_LeadingLepPhi   = array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_X.append(B_LeadingLepPhi)
B_LeadingLepEta   = array('f', [-2.4, -1.2, 0, 1.2, 2.4])
VarBins_X.append(B_LeadingLepEta)

B_SubLeadingLepE  = array('f', [0, 100, 200, 300, 400, 1000])
VarBins_X.append(B_SubLeadingLepE)
B_SubLeadingLepPt = array('f', [0, 100, 200, 300, 400, 1000])
VarBins_X.append(B_SubLeadingLepPt)
B_SubLeadingLepPhi= array('f', [-pi, -1.5, 0, 1.5, pi])
VarBins_X.append(B_SubLeadingLepPhi)
B_SubLeadingLepEta= array('f', [-2.4, -1.2, 0, 1.2, 2.4])
VarBins_X.append(B_SubLeadingLepEta)

B_DilepPt         = array('f', [0, 50, 100, 150, 500])
VarBins_X.append(B_DilepPt)
B_DilepJetPt      = array('f', [0, 50, 100, 150, 500])
VarBins_X.append(B_DilepJetPt)
B_DilepMETJetPt   = array('f', [0, 20, 40, 60, 80, 500])
VarBins_X.append(B_DilepMETJetPt)
B_HTtot           = array('f', [0, 100, 200, 300, 400, 1000])
VarBins_X.append(B_HTtot)


# ------------------------------------------> Reconstruction / Y axis: DOUBLE of nxbins
VarBins_Y         = []
B_E_LLB           = array('f', [0, 150, 200, 250, 300, 350, 400, 500, 600, 700, 1000])
VarBins_Y.append(B_E_LLB)
B_LeadingJetE     = array('f', [0, 75, 125, 175, 250, 320, 420, 1000])
VarBins_Y.append(B_LeadingJetE)
B_MT_LLMETB       = array('f', [0, 100, 150, 200, 250, 300, 350, 400, 450, 500, 1000])
VarBins_Y.append(B_MT_LLMETB)
B_M_LLB           = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_M_LLB)
B_M_LeadingB      = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 1000])
VarBins_Y.append(B_M_LeadingB)
B_M_SubLeadingB   = array('f', [0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 1000])
VarBins_Y.append(B_M_SubLeadingB)

B_MET             = array('f', [0, 25, 50, 75, 100, 125, 150, 175, 1000])
VarBins_Y.append(B_MET)
B_MET_Phi         = array('f', [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi])
VarBins_Y.append(B_MET_Phi)
B_LeadingJetPt    = array('f', [0, 50, 70, 90, 110, 130, 150, 170, 500])
VarBins_Y.append(B_LeadingJetPt)
B_LeadingJetEta   = array('f', [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4])
VarBins_Y.append(B_LeadingJetEta)
B_LeadingJetPhi   = array('f', [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi])
VarBins_Y.append(B_LeadingJetPhi)

B_LeadingLepE     = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_LeadingLepE)
B_LeadingLepPt    = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_LeadingLepPt)
B_LeadingLepPhi   = array('f', [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi])
VarBins_Y.append(B_LeadingLepPhi)
B_LeadingLepEta   = array('f', [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4])
VarBins_Y.append(B_LeadingLepEta)

B_SubLeadingLepE  = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_SubLeadingLepE)
B_SubLeadingLepPt = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_SubLeadingLepPt)
B_SubLeadingLepPhi= array('f', [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi])
VarBins_Y.append(B_SubLeadingLepPhi)
B_SubLeadingLepEta= array('f', [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4])
VarBins_Y.append(B_SubLeadingLepEta)

B_DilepPt         = array('f', [0, 25, 50, 75, 100, 125, 150, 175, 500])
VarBins_Y.append(B_DilepPt)
B_DilepJetPt      = array('f', [0, 20, 40, 60, 80, 100, 120, 140, 500])
VarBins_Y.append(B_DilepJetPt)
B_DilepMETJetPt   = array('f', [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 500])
VarBins_Y.append(B_DilepMETJetPt)
B_HTtot           = array('f', [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000])
VarBins_Y.append(B_HTtot)

# ------------------------------------------> Number of bins & limits
xmin    = [B_E_LLB[0],            B_LeadingJetE[0],       B_MT_LLMETB[0],         B_M_LLB[0],           B_M_LeadingB[0],    B_M_SubLeadingB[0],
           B_MET[0],              B_MET_Phi[0],           B_LeadingJetPt[0],      B_LeadingJetEta[0],   B_LeadingJetPhi[0],
           B_LeadingLepE[0],      B_LeadingLepPt[0],      B_LeadingLepPhi[0],     B_LeadingLepEta[0],
           B_SubLeadingLepE[0],   B_SubLeadingLepPt[0],   B_SubLeadingLepPhi[0],  B_SubLeadingLepEta[0],
           B_DilepPt[0],          B_DilepJetPt[0],        B_DilepMETJetPt[0],     B_HTtot[0]]
xmax    = [B_E_LLB[-1],           B_LeadingJetE[-1],      B_MT_LLMETB[-1],        B_M_LLB[-1],          B_M_LeadingB[-1],   B_M_SubLeadingB[-1],
           B_MET[-1],             B_MET_Phi[-1],          B_LeadingJetPt[-1],     B_LeadingJetEta[-1],  B_LeadingJetPhi[-1],
           B_LeadingLepE[-1],     B_LeadingLepPt[-1],     B_LeadingLepPhi[-1],    B_LeadingLepEta[-1],
           B_SubLeadingLepE[-1],  B_SubLeadingLepPt[-1],  B_SubLeadingLepPhi[-1], B_SubLeadingLepEta[-1],
           B_DilepPt[-1],         B_DilepJetPt[-1],       B_DilepMETJetPt[-1],    B_HTtot[-1]]
ymin    = xmin
ymax    = xmax
nybins  = [len(B_E_LLB)-1,           len(B_LeadingJetE)-1,      len(B_MT_LLMETB)-1,        len(B_M_LLB)-1,          len(B_M_LeadingB)-1,   len(B_M_SubLeadingB)-1,
           len(B_MET)-1,             len(B_MET_Phi)-1,          len(B_LeadingJetPt)-1,     len(B_LeadingJetEta)-1,  len(B_LeadingJetPhi)-1,
           len(B_LeadingLepE)-1,     len(B_LeadingLepPt)-1,     len(B_LeadingLepPhi)-1,    len(B_LeadingLepEta)-1,
           len(B_SubLeadingLepE)-1,  len(B_SubLeadingLepPt)-1,  len(B_SubLeadingLepPhi)-1, len(B_SubLeadingLepEta)-1,
           len(B_DilepPt)-1,         len(B_DilepJetPt)-1,       len(B_DilepMETJetPt)-1,    len(B_HTtot)-1]
nxbins  = [int(i/2) for i in nybins]  ### IMPORTANT!!!! The relation 1(gen):2(reco) in the number of bins MUST be preserved


print("\n> Importing minitrees' information...")
fTW               = r.TFile.Open(minipath + "Tree_UNF_TW.root")
fTW_DS            = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_DS.root")
fTW_fsrUp         = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_fsrUp.root")
fTW_fsrDown       = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_fsrDown.root")
fTW_isrUp         = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_isrUp.root")
fTW_isrDown       = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_isrDown.root")
fTW_MEUp          = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_MEscaleUp.root")
fTW_MEDown        = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_MEscaleDown.root")
fTW_PSUp          = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_PSscaleUp.root")
fTW_PSDown        = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_PSscaleDown.root")

fTbarW            = r.TFile.Open(minipath + "Tree_UNF_TbarW.root")
fTbarW_DS         = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_DS.root")
fTbarW_fsrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrUp.root")
fTbarW_fsrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrDown.root")
fTbarW_isrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrUp.root")
fTbarW_isrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrDown.root")
fTbarW_MEUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleUp.root")
fTbarW_MEDown     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleDown.root")
fTbarW_PSUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_PSscaleUp.root")
fTbarW_PSDown     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_PSscaleDown.root")

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


print "\n> Drawing matrices and writing ROOT file (old one will be overwritten!)."
print "The matrices will be saved in " + str(matrixoutputpath)
print "The plots will be drawn in " + str(plotsoutputpath)
f       = r.TFile(matrixoutputpath + "UnfoldingInfo.root", "recreate")
for i in range(nvars):
  print("\n    - Drawing and saving the response matrices of the variable "+ VarNames[i] + " ...")
  # Normal response matrices
  htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i], 1, purities[i], stabilities[i])
  htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i])
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  
  # JES, JER and weight-related systematics response matrices
  for j in range(nsys):
    htemp = GetResponseMatrix(treeTW, treeTbarW, VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
    htemp = GetFiducialHisto(treeTW, treeTbarW, VarNames[i], nybins[i], VarBins_Y[i], SysList[j])
    htemp.Write()
    PrintFiducialHisto(htemp, VarNames[i])
  
  # Modeling systematics response matrices
  htemp = GetResponseMatrix(treeTW_DS,      treeTbarW_DS,       VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_DS,       treeTbarW_DS,       VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrUp,    treeTbarW_fsrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_fsrDown,  treeTbarW_fsrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrDown,  treeTbarW_isrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_isrUp,    treeTbarW_isrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_MEUp,    treeTbarW_MEUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_MEUp,     treeTbarW_MEUp,     VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_MEDown,  treeTbarW_MEDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_MEDown,   treeTbarW_MEDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_PSUp,    treeTbarW_PSUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_PSUp,     treeTbarW_PSUp,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])
  htemp = GetResponseMatrix(treeTW_PSDown,  treeTbarW_PSDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintResponseMatrix(htemp, VarNames[i], nxbins[i], xmin[i], xmax[i], nybins[i], ymin[i], ymax[i])
  htemp = GetFiducialHisto(treeTW_PSDown,   treeTbarW_PSDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling")
  htemp.Write()
  PrintFiducialHisto(htemp, VarNames[i])

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
  
