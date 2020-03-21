import ROOT as r
import sys, os, shutil, CMS_lumi, tdrstyle
import varList as vl
from array  import array

r.gROOT.SetBatch(True)
CMS_lumi.writeExtraText = 1


#############################
print("\n===== Unfolding procedures: Response matrices & ROOT files production =====")
print("> Setting binning, paths, and other details...")
# ---------------------------------------------------------------- PRELIMINARIES
vl.SetUpWarnings()
minipath = vl.minipath
if (len(sys.argv) > 1):
  print("    - Manual minitrees' folder input!\n")
  if (sys.argv[1] == "last"):
    minipath  = vl.GetLastFolder(vl.storagepath)
  else:
    minipath  = vl.storagepath + sys.argv[1] + "/"

print "    - The minitrees' path is:\n", minipath
plotsoutputpath  = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
matrixoutputpath = "./temp/"

#nominal_weight = "(TWeight)"
nominal_weight = "(TWeight_TopPtUp)"
genCut      = "TWeight_normal * (Tpassgen == 1)"
recoCut     = "TWeight_normal * (Tpassreco == 1)"
Cut         = "TWeight * (" + nominal_weight + " / TWeight) * (Tpassgen == 1)"
fiduCut     = "TWeight * (" + nominal_weight + " / TWeight) * ((Tpassreco == 1) && (Tpassgen == 0))"

markersize  = 0.8

VarNames    = vl.varList["Names"]["Variables"]
nvars       = len(VarNames)

SysList     = vl.varList["Names"]["ExpSysts"]
nsys        = len(SysList)

purities    = []
stabilities = []

def GetResponseMatrix(t1, t2, vname, nxb, xb, nyb, yb, sys = "", nomtree = None, t3 = None):
  '''This function obtains the response matrix combining information of two trees.'''
  
  vnamegen    = vl.varList[vname]['var_gen'] # Generation name (without any kind of systematic thing)
  vnamereco   = vl.varList[vname]['var'] # Reconstruction name (can have a systematic-name tail)
  vnametitle  = vname # Name to be shown as the title (and name) of the histogram
  tmpcut      = Cut   # cuts
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   = vnamereco.replace(vname, vname + sys) if "Fiducial" not in vname else vnamereco.replace("LeadingLepPt", "LeadingLepPt" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys, 1) if "TopPt" not in sys else "TWeight * (Tpassgen == 1)" if "Down" in sys else "TWeight_TopPtUp * (TWeight_TopPtUp / TWeight) * (Tpassgen == 1)"
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling" and not "twttbar" in vname.lower()):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  elif (sys == "modeling" and "twttbar" in vname.lower()):
    if isinstance(t1, list):
      vnametitle  = vnametitle.replace("_", "") + "_" + t1[0].GetName()*(t1[0].GetName() != "Mini1j1t") + t3.GetName()*(t1[0].GetName() == "Mini1j1t")
    else:
      vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()   *(t1.GetName() != "Mini1j1t")    + t3.GetName()*(t1.GetName() == "Mini1j1t")
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  if not isinstance(t1, list):
    hGen1 = r.TH1F('hGen1', '', nxb, xb)
    hGen2 = r.TH1F('hGen2', '', nxb, xb)
    t1.Draw(vnamegen + '>>hGen1', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Draw(vnamegen + '>>hGen2', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    hGen1.Add(hGen2)
    if "twttbar" in vname.lower():
      hGen3 = r.TH1F('hGen3', '', nxb, xb)
      t3.Draw(vnamegen + '>>hGen3', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      hGen1.Add(hGen3)
      del hGen3
    del hGen2
  else:
    hGen1 = r.TH1F('hGen1', '', nxb, xb)
    hGen2 = r.TH1F('hGen2', '', nxb, xb)
    hGen1noF = r.TH1F('hGen1noF', '', nxb, xb)
    hGen2noF = r.TH1F('hGen2noF', '', nxb, xb)
    hGen1extra = r.TH1F('hGen1extra', '', nxb, xb)
    hGen2extra = r.TH1F('hGen2extra', '', nxb, xb)
    hGen1noFextra = r.TH1F('hGen1noFextra', '', nxb, xb)
    hGen2noFextra = r.TH1F('hGen2noFextra', '', nxb, xb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamegen + '>>hGen1', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t1[0].Draw(vnamegen + '>>hGen1extra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Draw(vnamegen + '>>hGen2', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2[0].Draw(vnamegen + '>>hGen2extra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamegen + '>>hGen1noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t1[1].Draw(vnamegen + '>>hGen1noFextra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Draw(vnamegen + '>>hGen2noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2[1].Draw(vnamegen + '>>hGen2noFextra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    hGen1.Add(hGen2)
    hGen1noF.Add(hGen2noF)
    hGen1.Add(hGen1noF)
    hGen1extra.Add(hGen2extra)
    hGen1noFextra.Add(hGen2noFextra)
    hGen1extra.Add(hGen1noFextra)
    if "twttbar" in vname.lower():
      hGen3 = r.TH1F('hGen3', '', nxb, xb)
      t3.Draw(vnamegen + '>>hGen3', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      hGen1.Add(hGen3)
      del hGen3
    del hGen2, hGen1noF, hGen2noF
    del hGen2extra, hGen1noFextra, hGen2noFextra
    
  hGen  = r.TH2F('hGen', '', nxb, xb, nyb, yb)
  
  for i in range(0, nxb+2):
    for j in range(0, nyb+2):
      hGen.SetBinContent(i, j, hGen1.GetBinContent(i))
      hGen.SetBinError(i, j, hGen1.GetBinError(i))
  
  if not isinstance(t1, list):
    h1    = r.TH2F('h1', "", nxb, xb, nyb, yb)
    h2    = r.TH2F('h2', '', nxb, xb, nyb, yb)
    
    t1.Project('h1', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Project('h2', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    h1.Add(h2)
    del h2
    if "twttbar" in vname.lower():
      h3    = r.TH2F('h3', '', nxb, xb, nyb, yb)
      t3.Project('h3', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))   # ERROR EN LO DE IRENE
      h1.Add(h3)
      del h3
  else:
    h1    = r.TH2F('h1',    "", nxb, xb, nyb, yb)
    h2    = r.TH2F('h2',    "", nxb, xb, nyb, yb)
    h1noF = r.TH2F('h1noF', "", nxb, xb, nyb, yb)
    h2noF = r.TH2F('h2noF', '', nxb, xb, nyb, yb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Project('h1', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Project('h2', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Project('h1noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Project('h2noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))

    #### PRUEBINA
    #specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    #t1[0].Project('h1', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname)    + "*(TLeadingJetPtJERUp < 1000)")
    #specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    #t2[0].Project('h2', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname)    + "*(TLeadingJetPtJERUp < 1000)")
    #specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    #t1[1].Project('h1noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname) + "*(TLeadingJetPtJERUp < 1000)")
    #specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    #t2[1].Project('h2noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname) + "*(TLeadingJetPtJERUp < 1000)")


    h1.Add(h2)
    h1noF.Add(h2noF)
    h1.Add(h1noF)
    if "twttbar" in vname.lower():
      h3    = r.TH2F('h3', "", nxb, xb, nyb, yb)
      t3.Project('h3', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
      h1.Add(h3)
      del h3
    del h2, h1noF, h2noF

  #### COMPROBACIOIS
  #if (sys == "JERUp"):
    #print "Comp.: numerador - valores"
    #print "Bin 1:", h1.GetBinContent( h1.GetXaxis().FindBin(40), h1.GetYaxis().FindBin(140)), "+-", h1.GetBinError( h1.GetXaxis().FindBin(40), h1.GetYaxis().FindBin(140))
    #print "Bin 2:", h1.GetBinContent( h1.GetXaxis().FindBin(60), h1.GetYaxis().FindBin(140)), "+-", h1.GetBinError( h1.GetXaxis().FindBin(60), h1.GetYaxis().FindBin(140))
    #print "Bin 3:", h1.GetBinContent( h1.GetXaxis().FindBin(80), h1.GetYaxis().FindBin(140)), "+-", h1.GetBinError( h1.GetXaxis().FindBin(80), h1.GetYaxis().FindBin(140))
    #print "\nComp.: denominador - valores"
    #print "Bin 1:", hGen.GetBinContent(hGen.GetXaxis().FindBin(40), hGen.GetYaxis().FindBin(140)), "+-", hGen.GetBinError( hGen.GetXaxis().FindBin(40), hGen.GetYaxis().FindBin(140))
    #print "Bin 2:", hGen.GetBinContent(hGen.GetXaxis().FindBin(60), hGen.GetYaxis().FindBin(140)), "+-", hGen.GetBinError( hGen.GetXaxis().FindBin(60), hGen.GetYaxis().FindBin(140))
    #print "Bin 3:", hGen.GetBinContent(hGen.GetXaxis().FindBin(80), hGen.GetYaxis().FindBin(140)), "+-", hGen.GetBinError( hGen.GetXaxis().FindBin(80), hGen.GetYaxis().FindBin(140))
    #print "\nCociente (x100)"
    #print "Bin 1:", 100 * h1.GetBinContent( h1.GetXaxis().FindBin(40), h1.GetYaxis().FindBin(140)) / hGen.GetBinContent(hGen.GetXaxis().FindBin(40), hGen.GetYaxis().FindBin(140))
    #print "Bin 2:", 100 * h1.GetBinContent( h1.GetXaxis().FindBin(60), h1.GetYaxis().FindBin(140)) / hGen.GetBinContent(hGen.GetXaxis().FindBin(60), hGen.GetYaxis().FindBin(140))
    #print "Bin 3:", 100 * h1.GetBinContent( h1.GetXaxis().FindBin(80), h1.GetYaxis().FindBin(140)) / hGen.GetBinContent(hGen.GetXaxis().FindBin(80), hGen.GetYaxis().FindBin(140))


  if (sys == ""):
    if not isinstance(t1, list):
      hReco1 = r.TH1F('hReco1', '', nyb, yb)
      hReco2 = r.TH1F('hReco2', '', nyb, yb)
      t1.Draw(vnamereco + '>>hReco1', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      t2.Draw(vnamereco + '>>hReco2', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      hReco1.Add(hReco2)
      del hReco2
      if "twttbar" in vname.lower():
        hReco3 = r.TH1F('hReco3', '', nyb, yb)
        t3.Draw(vnamereco + '>>hReco3', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
        hReco1.Add(hReco3)
        del hReco3
    else:
      hReco1 = r.TH1F('hReco1', '', nyb, yb)
      hReco2 = r.TH1F('hReco2', '', nyb, yb)
      hReco1noF = r.TH1F('hReco1noF', '', nyb, yb)
      hReco2noF = r.TH1F('hReco2noF', '', nyb, yb)
      
      specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
      t1[0].Draw(vnamereco + '>>hReco1', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
      t2[0].Draw(vnamereco + '>>hReco2', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
      t1[1].Draw(vnamereco + '>>hReco1noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
      t2[1].Draw(vnamereco + '>>hReco2noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      
      hReco1.Add(hReco2)
      hReco1noF.Add(hReco2noF)
      hReco1.Add(hReco1noF)
      del hReco2, hReco1noF, hReco2noF
      if "twttbar" in vname.lower():
        hReco3 = r.TH1F('hReco3', '', nyb, yb)
        t3.Draw(vnamereco + '>>hReco3', recoCut   + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
        hReco1.Add(hReco3)
        del hReco3
    
    tmppur  = []
    tmpstab = []

    ## BUENO
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

    ### EL QUE QUIEREN
    #for i in range(1, nxb+1):
      #sumstab = 0
      #for j in range(1, nyb+1):
        #sumstab += h1.GetBinContent(i, j)
      #try: tmpstab.append(sumstab/hGen1extra.GetBinContent(i))
      #except ZeroDivisionError: tmpstab.append(0)
    #stabilities.append(tmpstab)
    #del tmpstab
    #for j in range(1, nyb+1):
      #sumpur  = 0
      #for i in range(1, nxb+1):
        #sumpur  += h1.GetBinContent(i, j)
      #try: tmppur.append(sumpur/hReco1.GetBinContent(j))
      #except ZeroDivisionError: tmppur.append(0)
    #purities.append(tmppur)
    #del tmppur, hReco1


  
  if not isinstance(t1, list):
    if ('fsr' in t1.GetName() or 'FSR' in t1.GetName() or 'isr' in t1.GetName() or 'ISR' in t1.GetName()):
      h1nom = r.TH2F('h1nom', "", nxb, xb, nyb, yb)
      h2nom = r.TH2F('h2nom', '', nxb, xb, nyb, yb)
      nomtree[0].Project('h1nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Project('h2nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      h1nom.Add(h2nom)
      del h2nom
      hGen1nom = r.TH1F('hGen1nom', '', nxb, xb)
      hGen2nom = r.TH1F('hGen2nom', '', nxb, xb)

      nomtree[0].Draw(vnamegen + '>>hGen1nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Draw(vnamegen + '>>hGen2nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))

      hGen1nom.Add(hGen2nom)
      del hGen2nom
      if "twttbar" in vname.lower():
        h3nom = r.TH2F('h3nom', '', nxb, xb, nyb, yb)
        hGen3nom = r.TH1F('hGen3nom', '', nxb, xb)
        nomtree[2].Project('h3nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
        nomtree[2].Draw(vnamegen + '>>hGen3nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
        h1nom.Add(h3nom)
        hGen1nom.Add(hGen3nom)
        del h3nom, hGen3nom
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
      del hGennom, h1nom, hGen1nom
    elif ("mtop" in t1.GetName()):
      h1nom = r.TH2F('h1nom', "", nxb, xb, nyb, yb)
      h2nom = r.TH2F('h2nom', '', nxb, xb, nyb, yb)
      nomtree[0].Project('h1nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Project('h2nom', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      h1nom.Add(h2nom)
      del h2nom
      hGen1nom = r.TH1F('hGen1nom', '', nxb, xb)
      hGen2nom = r.TH1F('hGen2nom', '', nxb, xb)

      nomtree[0].Draw(vnamegen + '>>hGen1nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Draw(vnamegen + '>>hGen2nom', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))

      hGen1nom.Add(hGen2nom)
      del hGen2nom
      
      hGennom = r.TH2F('Gennom', '', nxb, xb, nyb, yb)

      for i in range(0, nxb+2):
        for j in range(0, nyb+2):
          hGennom.SetBinContent(i, j, hGen1nom.GetBinContent(i))
          hGennom.SetBinError(i, j, hGen1nom.GetBinError(i))
      
      h1.Add(h1nom, -1)
      hGen.Add(hGennom, -1)
      
      h1nom.Add(h1, r.Double(1./3.))
      hGennom.Add(hGen, r.Double(1./3.))
      
      del h1, hGen
      h1 = h1nom.Clone("h1")
      #h1.SetTitle("Response matrix - " + vnametitle)
      hGen = hGennom.Clone("hGen")
      del hGennom, h1nom, hGen1nom
      
  
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
  
  h1.SetXTitle(vl.varList[vname]['xaxis'] + " [particle level]")
  h1.SetYTitle(vl.varList[vname]['xaxis'] + " [detector level]")
  h1.SetName("R" + vnametitle)
  return h1


def GetFiducialHisto(t1, t2, vname, nyb, yb, sys = "", nomtree = None, t3 = None):
  '''This function obtains the fiducial histograms combining info. of two trees.'''
  vnametitle  = vname   # Name to be shown as the title (and name) of the histogram
  vnamereco   = vl.varList[vname]['var'] # Reconstruction name (can have a systematic-name tail)
  tmpcut      = fiduCut # cuts
  
  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   = vnamereco.replace(vname, vname + sys) if "Fiducial" not in vname else vnamereco.replace("LeadingLepPt", "LeadingLepPt" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    #tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys, 1) if "TopPt" not in sys else "TWeight * ((Tpassreco == 1) && (Tpassgen == 0))"
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys, 1) if "TopPt" not in sys else "TWeight * ((Tpassreco == 1) && (Tpassgen == 0))" if "Down" in sys else "TWeight_TopPtUp * (TWeight_TopPtUp / TWeight) * ((Tpassreco == 1) && (Tpassgen == 0))"
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling" and not "twttbar" in vname.lower()):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  elif (sys == "modeling" and "twttbar" in vname.lower()):
    if isinstance(t1, list):
      vnametitle  = vnametitle.replace("_", "") + "_" + t1[0].GetName()*(t1[0].GetName() != "Mini1j1t") + t3.GetName()*(t1[0].GetName() == "Mini1j1t")
    else:
      vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()*(t1.GetName() != "Mini1j1t") + t3.GetName()*(t1.GetName() == "Mini1j1t")
  else:
    vnametitle  = vnametitle.replace("_", "")
  
  if not isinstance(t1, list):
    h1 = r.TH1F('h1', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2 = r.TH1F('h2', '',                                    nyb, yb)
    
    t1.Draw(vnamereco + '>>h1', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Draw(vnamereco + '>>h2', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    
    h1.Add(h2)
    del h2
    if "twttbar" in vname.lower():
      h3 = r.TH1F('h3', '', nyb, yb)
      t3.Draw(vnamereco + '>>h3', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      h1.Add(h3)
      del h3
  else:
    h1 = r.TH1F('h1', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2 = r.TH1F('h2', '',                                    nyb, yb)
    h1noF = r.TH1F('h1noF', "Fiducial histogram - T" + vnametitle, nyb, yb)
    h2noF = r.TH1F('h2noF', '',                                    nyb, yb)
    
    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamereco + '>>h1', tmpcut + '*' + str(specialweight))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Draw(vnamereco + '>>h2', tmpcut + '*' + str(specialweight))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamereco + '>>h1noF', tmpcut + '*' + str(specialweight))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Draw(vnamereco + '>>h2noF', tmpcut + '*' + str(specialweight))

    h1.Add(h2)
    h1noF.Add(h2noF)
    h1.Add(h1noF)
    if "twttbar" in vname.lower():
      h3 = r.TH1F('h3', '', nyb, yb)
      t3.Draw(vnamereco + '>>h3', tmpcut)
      h1.Add(h3)
      del h3
    del h2, h1noF, h2noF
  
  if not isinstance(t1, list):
    if ('fsr' in t1.GetName() or 'FSR' in t1.GetName() or 'isr' in t1.GetName() or 'ISR' in t1.GetName()):
      h1nom = r.TH1F('h1nom', "", nyb, yb)
      h2nom = r.TH1F('h2nom', '', nyb, yb)
      nomtree[0].Draw(vnamereco + '>>h1nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Draw(vnamereco + '>>h2nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      h1nom.Add(h2nom)
      del h2nom
      
      if "twttbar" in vname.lower():
        h3nom = r.TH1F('h3nom', '', nyb, yb)
        nomtree[2].Draw(vnamereco + '>>h3nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
        h1nom.Add(h3nom)
        del h3nom
      
      h1.Add(h1nom, -1)
      h1nom.Add(h1, r.Double(1/r.TMath.Sqrt(2)))
      del h1
      h1 = h1nom.Clone("h1")
      h1.SetTitle("Fiducial histogram - T" + vnametitle)
      del h1nom
    
    elif ("mtop" in t1.GetName()):
      h1nom = r.TH1F('h1nom', "", nyb, yb)
      h2nom = r.TH1F('h2nom', '', nyb, yb)
      nomtree[0].Draw(vnamereco + '>>h1nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      nomtree[1].Draw(vnamereco + '>>h2nom', tmpcut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      h1nom.Add(h2nom)
      del h2nom
      
      h1.Add(h1nom, -1)
      h1nom.Add(h1, r.Double(1./3.))
      del h1
      h1 = h1nom.Clone("h1")
      h1.SetTitle("Fiducial histogram - T" + vnametitle)
      del h1nom
  
  h1.SetXTitle("Fiducial events")
  h1.SetYTitle("Events")
  h1.SetName("F" + vnametitle)
  
  return h1


def PrintResponseMatrix(htemp, vname, nxb, xb, xmin, xmax, nyb, yb, ymin, ymax, pur = None, stab = None):
  '''This function prints the response matrix of a given histogram.'''
  tdrstyle.setTDRStyle()
  if not os.path.exists(plotsoutputpath + vname):
    os.makedirs(plotsoutputpath + vname)
  if not os.path.isfile(plotsoutputpath + vname + "/index.php"):
    shutil.copy2(plotsoutputpath + "index.php", plotsoutputpath + vname + "/index.php")
  vnametitle = htemp.GetName()[1:]
  htemp.Scale(100)
  htemp.SetStats(False)
  r.gStyle.SetLabelFont(43, "XYZ")
  r.gStyle.SetLabelSize(22, "XYZ")
  htemp.GetYaxis().SetTitleOffset(1.4)
  htemp.GetXaxis().SetTitleOffset(1.2)
  htemp.GetXaxis().SetTitleFont(43)
  htemp.GetXaxis().SetTitleSize(22)
  htemp.GetXaxis().SetLabelFont(43)
  htemp.GetXaxis().SetLabelSize(22)
  htemp.GetYaxis().SetTitleFont(43)
  htemp.GetYaxis().SetTitleSize(22)
  htemp.GetYaxis().SetLabelFont(43)
  htemp.GetYaxis().SetLabelSize(22)
  #c = r.TCanvas('c', "", 700, 500)
  c = r.TCanvas('c', "", 600, 600)
  plot = c.GetPad(0);
  #plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.0475); plot.SetRightMargin(0.1); plot.SetLeftMargin(0.12); plot.SetBottomMargin(0.1)
  if 'resptxtsize' in vl.varList[vname]: htemp.SetMarkerSize(vl.varList[vname]['resptxtsize'])
  else:                                  htemp.SetMarkerSize(markersize)
  htemp.SetMarkerColor(r.kRed)
  htemp.Draw("colz text e")
  r.gStyle.SetPaintTextFormat("4.3f")
  CMS_lumi.lumi_13TeV = ""
  #CMS_lumi.extraText  = 'Simulation Supplementary'
  CMS_lumi.extraText  = 'Simulation Preliminary'
  CMS_lumi.lumi_sqrtS = ''
  #CMS_lumi.cmsTextSize += 0.1
  CMS_lumi.CMS_lumi(r.gPad, 0, 0, 0.05)
  r.gStyle.SetLabelFont(43, "XYZ")
  r.gStyle.SetLabelSize(22, "XYZ")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".png")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".eps")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".pdf")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + ".root")
  c.Close(); del c, plot
  
  #c = r.TCanvas('c', "", 700, 500)
  c = r.TCanvas('c', "", 600, 600)
  plot = c.GetPad(0);
  #plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.0475); plot.SetRightMargin(0.1); plot.SetLeftMargin(0.12); plot.SetBottomMargin(0.1)
  htemp.Draw("colz")
  CMS_lumi.CMS_lumi(r.gPad, 0, 0, 0.05)
  r.gStyle.SetLabelFont(43, "XYZ")
  r.gStyle.SetLabelSize(22, "XYZ")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.png")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.eps")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.pdf")
  c.SaveAs(plotsoutputpath + vname + "/R_T" + vnametitle + "_ensinnumerines.root")
  c.Close(); del c, plot
  
  
  #hX          = r.TProfile('hX', '', nxb, xb)
  #hY          = r.TProfile('hY', '', nyb, yb)
  #htemp.ProfileX("hX", 1, -1, "s")
  #htemp.ProfileY("hY", 1, -1, "s")
  
  #c       = r.TCanvas('c', "X-Profiled response matrix - T" + vnametitle, 600, 600)
  #hX.SetStats(False)
  #hX.SetXTitle("X/Gen. value")
  #hX.SetYTitle("Mean Y/Reco. value")
  #hX.Draw()
  #c.SaveAs(plotsoutputpath + vname + "/P_X_T" + vnametitle + ".png")
  #c.Close(); del c, hX
  
  #c = r.TCanvas('c', "Y-Profiled response matrix - T" + vnametitle, 600, 600)
  #hY.SetStats(False)
  #hY.SetXTitle("Y/Reco. value")
  #hY.SetYTitle("Mean X/Gen. value")
  #hY.Draw()
  #r.gStyle.SetPaintTextFormat("4.1f")
  #c.SaveAs(plotsoutputpath + vname + "/P_Y_T" + vnametitle + ".png")
  #c.Close(); del c, hY
  
  if pur == None: return
  
  hStab   = r.TH1F('hStab', '', nxb, xb)
  for i in range(1, hStab.GetNbinsX() + 1):
    hStab.SetBinContent(i, stab[i - 1])
  hPur    = r.TH1F('hPur',  '', nyb, yb)
  for j in range(1, hPur.GetNbinsX() + 1):
    hPur.SetBinContent(j, pur[j - 1])
  
  c = r.TCanvas('c', "Purities and stabilities of " + vnametitle)
  plot = c.GetPad(0);
  #plot.SetPad(0.0, 0.23, 1.0, 1.0);
  plot.SetTopMargin(0.06); plot.SetRightMargin(0.05); plot.SetLeftMargin(0.1); plot.SetBottomMargin(0.12)
  
  hPur.SetXTitle(vl.varList[vname]['xaxis'])
  hPur.SetStats(False)
  hPur.SetYTitle("Purities and stabilities")
  hStab.SetLineColor(r.kBlue)
  hPur.SetLineColor(r.kRed)
  hPur.SetMaximum(1.)
  hPur.SetMinimum(0.)
  #hPur.SetMinimum(0.6)
  hPur.GetXaxis().SetTitleFont(43)
  hPur.GetXaxis().SetTitleSize(22)
  hPur.GetXaxis().SetLabelFont(43)
  hPur.GetXaxis().SetLabelSize(22)
  hPur.GetXaxis().SetLabelOffset(0.007)
  hPur.GetXaxis().SetTitleOffset(1.2)
  hPur.GetYaxis().SetTitleFont(43)
  hPur.GetYaxis().SetTitleSize(22)
  hPur.GetYaxis().SetLabelFont(43)
  hPur.GetYaxis().SetLabelSize(22)
  hPur.GetYaxis().SetLabelOffset(0.007)
  hPur.GetYaxis().SetTitleOffset(1.2)
  hPur.Draw('')
  hStab.Draw('same')
  r.gPad.Update()
  
  textSize      = 0.035
  (x1,y1,x2,y2) = (.75, .45, .85, .65)
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
  c.Close(); del c, hStab, hPur, plot
  return


def PrintFiducialHisto(htemp, vname):
  '''This function prints the fiducial histo of a variable.'''
  if not os.path.exists(plotsoutputpath + vname):
    os.makedirs(plotsoutputpath + vname)
  if not os.path.isfile(plotsoutputpath + vname + "/index.php"):
    shutil.copy2(plotsoutputpath + "index.php", plotsoutputpath + vname + "/index.php")
  vnametitle = htemp.GetName()[1:]
  #c = r.TCanvas('c', "Fiducial histogram - T" + vnametitle, 200, 10, 700, 500)
  c = r.TCanvas('c', "Fiducial histogram - T" + vnametitle, 600, 600)
  #htemp.SetStats(False)
  htemp.Draw()
  r.gStyle.SetPaintTextFormat("4.1f")
  r.gPad.Update()
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".png")
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".pdf")
  c.SaveAs(plotsoutputpath + vname + "/F_T" + vnametitle + ".root")
  c.Close(); del c
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
nxbins  = [int(i/2)  for i in nybins]  ### IMPORTANT!!!! The relation 1(gen):2(reco) in the number of bins MUST be preserved
for i in range(len(nxbins)):
  if nxbins[i] == 0: nxbins[i] = 1

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
fTW_mtopUp        = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_mtop1755.root")
fTW_mtopDown      = r.TFile.Open(minipath + "Tree_UNF_TW_noFullyHadr_mtop1695.root")

fTbarW            = r.TFile.Open(minipath + "Tree_UNF_TbarW.root")
fTbarW_noFully    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr.root")
fTbarW_DSUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_DS.root")
fTbarW_fsrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrUp.root")
fTbarW_fsrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_fsrDown.root")
fTbarW_isrUp      = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrUp.root")
fTbarW_isrDown    = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_isrDown.root")
fTbarW_MEUp       = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleUp.root")
fTbarW_MEDown     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_MEscaleDown.root")
fTbarW_mtopUp     = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_mtop1755.root")
fTbarW_mtopDown   = r.TFile.Open(minipath + "Tree_UNF_TbarW_noFullyHadr_mtop1695.root")

#fTTbar            = r.TFile.Open(minipath + "Tree_UNF_TTbar2L_powheg_0.root")
fTTbar_fsrUp      = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_fsrUp.root")
fTTbar_fsrDown    = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_fsrDown.root")
fTTbar_isrUp      = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_isrUp.root")
fTTbar_isrDown    = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_isrDown.root")   # WE DO NOT HAVE ME VARIATIONS
#fTTbar_MEUp       = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_MEscaleUp.root")
#fTTbar_MEDown     = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_MEscaleDown.root")
fTTbar_hDampUp    = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_hdampUp.root")
fTTbar_hDampDown  = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_hdampDown.root")
fTTbar_UEUp       = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_ueUp.root")
fTTbar_UEDown     = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_ueDown.root")
fTTbar_erdON      = r.TFile.Open(minipath + "Tree_UNF_TTbar_Powheg_erdON.root")   # WE STILL NEED THE OTHER COLOUR REC. VARIATIONS!

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
treeTW_mtopUp     = fTW_mtopUp.Get('Mini1j1t')
treeTW_mtopUp.SetName("mtopUp")
treeTW_mtopDown   = fTW_mtopDown.Get('Mini1j1t')
treeTW_mtopDown.SetName("mtopDown")


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
treeTbarW_mtopUp  = fTbarW_mtopUp.Get('Mini1j1t')
treeTbarW_mtopUp.SetName("mtopUp")
treeTbarW_mtopDown= fTbarW_mtopDown.Get('Mini1j1t')
treeTbarW_mtopDown.SetName("mtopDown")

#treeTTbar         = fTTbar.Get('Mini1j1t')
treeTTbar = 0
treeTTbar_fsrUp   = fTTbar_fsrUp.Get('Mini1j1t')
treeTTbar_fsrUp.SetName("fsrUp")
treeTTbar_fsrDown = fTTbar_fsrDown.Get('Mini1j1t')
treeTTbar_fsrDown.SetName("fsrDown")
treeTTbar_isrUp   = fTTbar_isrUp.Get('Mini1j1t')
treeTTbar_isrUp.SetName("isrUp")
treeTTbar_isrDown = fTTbar_isrDown.Get('Mini1j1t')
treeTTbar_isrDown.SetName("isrDown")
#treeTTbar_MEUp    = fTTbar_MEUp.Get('Mini1j1t')
#treeTTbar_MEUp.SetName("MEUp")
#treeTTbar_MEDown  = fTTbar_MEDown.Get('Mini1j1t')
#treeTTbar_MEDown.SetName("MEDown")
treeTTbar_hDampUp = fTTbar_hDampUp.Get('Mini1j1t')
treeTTbar_hDampUp.SetName("hDampUp")
treeTTbar_hDampDown = fTTbar_hDampDown.Get('Mini1j1t')
treeTTbar_hDampDown.SetName("hDampDown")
treeTTbar_UEUp    = fTTbar_UEUp.Get('Mini1j1t')
treeTTbar_UEUp.SetName("UEUp")
treeTTbar_UEDown  = fTTbar_UEDown.Get('Mini1j1t')
treeTTbar_UEDown.SetName("UEDown")
treeTTbar_erdON   = fTTbar_erdON.Get('Mini1j1t')
treeTTbar_erdON.SetName("erdON")


print "\n> Drawing matrices and writing ROOT file (old one will be overwritten!)."
print "The matrices will be saved in " + str(matrixoutputpath)
print "The plots will be drawn in " + str(plotsoutputpath)
#f       = r.TFile(matrixoutputpath + "UnfoldingInfo.root", "recreate")
#for i in range(nvars):
  #print("\n    - Drawing and saving the response matrices of the variable "+ VarNames[i] + " ...")
  ## Nominal response matrices
  #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i], purities[i], stabilities[i])
  #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  ## JES, JER and weight-related systematics response matrices
  #for j in range(nsys):
    ##if j != 2: continue
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], SysList[j], t3 = treeTTbar)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], SysList[j], t3 = treeTTbar)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
  
  ## Modeling systematics response matrices
  #htemp = GetResponseMatrix(treeTW_DSUp,    treeTbarW_DSUp,       VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  #htemp = GetFiducialHisto(treeTW_DSUp,     treeTbarW_DSUp,       VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  ##htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_fsrUp)
  #htemp = GetResponseMatrix(treeTW_fsrUp,   treeTbarW_fsrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_fsrUp)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_fsrUp,    treeTbarW_fsrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_fsrUp)
  #htemp = GetFiducialHisto(treeTW_fsrUp,    treeTbarW_fsrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_fsrUp)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  ##htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_fsrDown)
  #htemp = GetResponseMatrix(treeTW_fsrDown, treeTbarW_fsrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_fsrDown)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_fsrDown,  treeTbarW_fsrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_fsrDown)
  #htemp = GetFiducialHisto(treeTW_fsrDown,  treeTbarW_fsrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_fsrDown)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  ##htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_isrDown)
  #htemp = GetResponseMatrix(treeTW_isrDown, treeTbarW_isrDown,  VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_isrDown)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_isrDown,  treeTbarW_isrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_isrDown)
  #htemp = GetFiducialHisto(treeTW_isrDown,  treeTbarW_isrDown,  VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_isrDown)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  ##htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_isrUp)
  #htemp = GetResponseMatrix(treeTW_isrUp,   treeTbarW_isrUp,    VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_isrUp)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_isrUp,    treeTbarW_isrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW, treeTbarW, treeTTbar], t3 = treeTTbar_isrUp)
  #htemp = GetFiducialHisto(treeTW_isrUp,    treeTbarW_isrUp,    VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar_isrUp)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  ##htemp = GetResponseMatrix(treeTW_MEUp,    treeTbarW_MEUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_MEUp)
  #htemp = GetResponseMatrix(treeTW_MEUp,    treeTbarW_MEUp,     VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_MEUp,     treeTbarW_MEUp,     VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_MEUp)
  #htemp = GetFiducialHisto(treeTW_MEUp,     treeTbarW_MEUp,     VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  ##htemp = GetResponseMatrix(treeTW_MEDown,  treeTbarW_MEDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_MEDown)
  #htemp = GetResponseMatrix(treeTW_MEDown,  treeTbarW_MEDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  ##htemp = GetFiducialHisto(treeTW_MEDown,   treeTbarW_MEDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_MEDown)
  #htemp = GetFiducialHisto(treeTW_MEDown,   treeTbarW_MEDown,   VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  ## MTOP
  #htemp = GetResponseMatrix(treeTW_mtopUp,  treeTbarW_mtopUp,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  #htemp = GetFiducialHisto(treeTW_mtopUp, treeTbarW_mtopUp, VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  #htemp = GetResponseMatrix(treeTW_mtopDown,  treeTbarW_mtopDown,   VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar)
  #htemp.Write()
  #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
  #htemp = GetFiducialHisto(treeTW_mtopDown, treeTbarW_mtopDown, VarNames[i], nybins[i], VarBins_Y[i], "modeling", [treeTW_noFully, treeTbarW_noFully, treeTTbar], t3 = treeTTbar)
  #htemp.Write()
  #PrintFiducialHisto(htemp, VarNames[i])
  
  #if "twttbar" in VarNames[i].lower():
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_UEDown)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_UEDown)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
    
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_UEUp)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_UEUp)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
    
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_erdON)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_erdON)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
    
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_hDampDown)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_hDampDown)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
    
    #htemp = GetResponseMatrix([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nxbins[i], VarBins_X[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_hDampUp)
    #htemp.Write()
    #PrintResponseMatrix(htemp, VarNames[i], nxbins[i], VarBins_X[i], xmin[i], xmax[i], nybins[i], VarBins_Y[i], ymin[i], ymax[i])
    #htemp = GetFiducialHisto([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[i], nybins[i], VarBins_Y[i], "modeling", t3 = treeTTbar_hDampUp)
    #htemp.Write()
    #PrintFiducialHisto(htemp, VarNames[i])
    
#f.Write()
#f.Close()
  
print("> Done!")

######################################################################################## lo del overlap


def GetOverlap(t1, t2, vname, nxb, xb, nyb, yb):
  '''This function used to obtain the response matrix combining information of two trees.
  Now it does not.'''

  vnamegen   = vl.varList[vname]['var_gen'] # Generation name (without any kind of systematic thing)
  vnamereco  = vl.varList[vname]['var']     # Reconstruction name (can have a systematic-name tail)
  vnametitle = vname # Name to be shown as the title (and name) of the histogram
  tmpcut     = Cut   # cuts
  sys        = ""

  if (sys in SysList[:3]):
    tmpcut      = tmpcut.replace("Tpassreco", "Tpassreco" + sys)
    vnamereco   = vnamereco.replace(vname, vname + sys) if "Fiducial" not in vname else vnamereco.replace("LeadingLepPt", "LeadingLepPt" + sys)
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys in SysList[3:]):
    tmpcut      = tmpcut.replace("TWeight", "TWeight_" + sys, 1) if "TopPt" not in sys else "TWeight * (Tpassgen == 1)" if "Down" in sys else "TWeight_TopPtUp * (TWeight_TopPtUp / TWeight) * (Tpassgen == 1)"
    vnametitle  = vnametitle.replace("_", "") + "_" + sys
  elif (sys == "modeling" and not "twttbar" in vname.lower()):
    vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()
  elif (sys == "modeling" and "twttbar" in vname.lower()):
    if isinstance(t1, list):
      vnametitle  = vnametitle.replace("_", "") + "_" + t1[0].GetName()*(t1[0].GetName() != "Mini1j1t") + t3.GetName()*(t1[0].GetName() == "Mini1j1t")
    else:
      vnametitle  = vnametitle.replace("_", "") + "_" + t1.GetName()   *(t1.GetName() != "Mini1j1t")    + t3.GetName()*(t1.GetName() == "Mini1j1t")
  else:
    vnametitle  = vnametitle.replace("_", "")

  hGen1 = r.TH1F('hGen1', '', nxb, xb)
  hGen2 = r.TH1F('hGen2', '', nxb, xb)

  if not isinstance(t1, list):
    t1.Draw(vnamegen + '>>hGen1', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Draw(vnamegen + '>>hGen2', genCut + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    hGen1.Add(hGen2)
    del hGen2
  else:
    hGen1noF = r.TH1F('hGen1noF', '', nxb, xb)
    hGen2noF = r.TH1F('hGen2noF', '', nxb, xb)
    hGen1extra = r.TH1F('hGen1extra', '', nxb, xb)
    hGen2extra = r.TH1F('hGen2extra', '', nxb, xb)
    hGen1noFextra = r.TH1F('hGen1noFextra', '', nxb, xb)
    hGen2noFextra = r.TH1F('hGen2noFextra', '', nxb, xb)

    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamegen + '>>hGen1', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t1[0].Draw(vnamegen + '>>hGen1extra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Draw(vnamegen + '>>hGen2', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2[0].Draw(vnamegen + '>>hGen2extra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamegen + '>>hGen1noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t1[1].Draw(vnamegen + '>>hGen1noFextra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Draw(vnamegen + '>>hGen2noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2[1].Draw(vnamegen + '>>hGen2noFextra', genCut + "/TWeight_normal*" + recoCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    hGen1.Add(hGen2)
    hGen1noF.Add(hGen2noF)
    hGen1.Add(hGen1noF)
    hGen1extra.Add(hGen2extra)
    hGen1noFextra.Add(hGen2noFextra)
    hGen1extra.Add(hGen1noFextra)
    del hGen2, hGen1noF, hGen2noF
    del hGen2extra, hGen1noFextra, hGen2noFextra

  # corte gen: hGen1

  h1    = r.TH2F('h1',    "", nxb, xb, nyb, yb)
  h2    = r.TH2F('h2',    "", nxb, xb, nyb, yb)
  if not isinstance(t1, list):
    t1.Project('h1', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Project('h2', vnamereco + ":" + vnamegen, tmpcut + "*(abs(TGen{vr}) < 1000)*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    h1.Add(h2)
    del h2
  else:
    h1noF = r.TH2F('h1noF', "", nxb, xb, nyb, yb)
    h2noF = r.TH2F('h2noF', '', nxb, xb, nyb, yb)

    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Project('h1', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Project('h2', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Project('h1noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Project('h2noF', vnamereco + ":" + vnamegen, tmpcut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt") + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") + sys * ("JER" in sys or "JES" in sys) if "Fiducial" not in vname else "LeadingLepPt"))

    h1.Add(h2)
    h1noF.Add(h2noF)
    h1.Add(h1noF)
    del h2, h1noF, h2noF



  if not isinstance(t1, list):
    hReco1 = r.TH1F('hReco1', '', nyb, yb)
    hReco2 = r.TH1F('hReco2', '', nyb, yb)
    t1.Draw(vnamereco + '>>hReco1', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    t2.Draw(vnamereco + '>>hReco2', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    hReco1.Add(hReco2)
    del hReco2
    if "twttbar" in vname.lower():
      hReco3 = r.TH1F('hReco3', '', nyb, yb)
      t3.Draw(vnamereco + '>>hReco3', recoCut + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
      hReco1.Add(hReco3)
      del hReco3
  else:
    hReco1 = r.TH1F('hReco1', '', nyb, yb)
    hReco2 = r.TH1F('hReco2', '', nyb, yb)
    hReco1noF = r.TH1F('hReco1noF', '', nyb, yb)
    hReco2noF = r.TH1F('hReco2noF', '', nyb, yb)

    specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[0].Draw(vnamereco + '>>hReco1', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[0].Draw(vnamereco + '>>hReco2', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
    t1[1].Draw(vnamereco + '>>hReco1noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))
    specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
    t2[1].Draw(vnamereco + '>>hReco2noF', recoCut + '*' + str(specialweight) + "*(abs(T{vr}) < 1000)".format(vr = vname.replace("ATLAS", "") if "Fiducial" not in vname else "LeadingLepPt"))

    hReco1.Add(hReco2)
    hReco1noF.Add(hReco2noF)
    hReco1.Add(hReco1noF)
    del hReco2, hReco1noF, hReco2noF

  hGen  = r.TH2F('hGen', '', nxb, xb, nyb, yb)

  for i in range(0, nxb+2):
    for j in range(0, nyb+2):
      hGen.SetBinContent(i, j, hGen1.GetBinContent(i))
      hGen.SetBinError(i, j, hGen1.GetBinError(i))

  print "valor2:", h1.GetBinContent(1,1)/hReco1.GetBinContent(1)

  h1.Divide(hGen)
  del hGen, hGen1

  print "valor:",  h1.GetBinContent(1, 1)

  ## Fixing the over and underflow bins to one.
  for i in range(1, 1+1):
    tmpsum = 0.
    for j in range(1, 1+1):
      tmpsum += h1.GetBinContent(i, j)
    h1.SetBinContent(i, 0, 0.)
    h1.SetBinContent(i, nyb + 1, 1 - tmpsum)
    #h1.SetBinContent(i, nyb + 1, 0)
  for j in range(0, 1+2):
    h1.SetBinContent(0, j, 0.)
    h1.SetBinContent(nxb + 1, j, 0.)

  print "valor 1,2:", h1.GetBinContent(1,2)
  return


GetOverlap([treeTW, treeTW_noFully], [treeTbarW, treeTbarW_noFully], VarNames[-1], nxbins[-1], VarBins_X[-1], nybins[-1], VarBins_Y[-1])



