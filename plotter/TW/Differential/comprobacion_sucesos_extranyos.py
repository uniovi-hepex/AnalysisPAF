import ROOT as r
import sys,os,copy,array
import varList as vl

fullpath = vl.GetLastFolder(vl.storagepath)
init     = "Tree_UNF_"
end      = ".root"
vname    = "LeadingJetPt"


tw_unf       = fullpath + init + "TW" + end
tbarw_unf    = fullpath + init + "TbarW" + end
twnfh_unf    = fullpath + init + "TW_noFullyHadr" + end
tbarwnfh_unf = fullpath + init + "TbarW_noFullyHadr" + end


cadena = r.TChain("Mini1j1t", "cadena")
cadena.Add(tw_unf)
cadena.Add(tbarw_unf)
cadena.Add(twnfh_unf)
cadena.Add(tbarwnfh_unf)

#specialweight = vl.n_tw/vl.sigma_tw/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
#tw_unf.Draw(      vnamegen + '>>hGen1',    genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
#specialweight = vl.n_tbarw/vl.sigma_tw/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
#tbarw_unf.Draw(   vnamegen + '>>hGen2',    genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
#specialweight = vl.n_twnohad/vl.sigma_twnohad/(vl.n_tw/vl.sigma_tw + vl.n_twnohad/vl.sigma_twnohad)
#twnfh_unf.Draw(   vnamegen + '>>hGen1noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))
#specialweight = vl.n_tbarwnohad/vl.sigma_twnohad/(vl.n_tbarw/vl.sigma_tw + vl.n_tbarwnohad/vl.sigma_twnohad)
#tbarwnfh_unf.Draw(vnamegen + '>>hGen2noF', genCut + '*' + str(specialweight) + "*(abs(TGen{vr}) < 1000)".format(vr = vname))

#hGen1.Add(hGen2)
#hGen1noF.Add(hGen2noF)
#hGen1.Add(hGen1noF)

output = []

nevent    = 0
neventden = [0,0,0]
foundjets = 1
for event in cadena:
    if ((int(ord(event.Tpassgen)) == 1) and (event.TGenLeadingJetPt < 90) and (event.TLeadingJetPtJERUp > 125) and (event.TLeadingJetPtJERUp < 1000) and (event.TGenLeadingJetPt < 1000)):
    #if ((int(ord(event.TpassrecoJERUp)) == 1) and (int(ord(event.Tpassgen)) == 1) and (event.TGenLeadingJetPt < 90) and (event.TLeadingJetPtJERUp > 125) and (event.TLeadingJetPtJERUp < 1000) and (event.TGenLeadingJetPt < 1000)):
        output.append("\n### Found jet number {n}".format(n = foundjets))
        output[-1] += "\n> DR = {vl}".format(vl = r.TMath.Sqrt((event.TGenLeadingJetEta - event.TLeadingJetEtaJERUp)**2 + (event.TGenLeadingJetPhi - event.TLeadingJetPhiJERUp)**2))
        output[-1] += "\n> Generated information."
        output[-1] += "\n    - pT  = {vl} GeV".format(vl = event.TGenLeadingJetPt)
        output[-1] += "\n    - E   = {vl} GeV".format(vl = event.TGenLeadingJetE)
        output[-1] += "\n    - eta = {vl}".format(vl = event.TGenLeadingJetEta)
        output[-1] += "\n    - phi = {vl}".format(vl = event.TGenLeadingJetPhi)
        output[-1] += "\n\n> Reconstructed information."
        output[-1] += "\n    - pT  = {vl} GeV".format(vl = event.TLeadingJetPtJERUp)
        output[-1] += "\n    - E   = {vl} GeV".format(vl = event.TLeadingJetEJERUp)
        output[-1] += "\n    - eta = {vl}".format(vl = event.TLeadingJetEtaJERUp)
        output[-1] += "\n    - phi = {vl}".format(vl = event.TLeadingJetPhiJERUp)
        foundjets += 1

    if ((int(ord(event.Tpassgen)) == 1) and (event.TGenLeadingJetPt < 90) and (event.TGenLeadingJetPt < 1000)):
        if   event.TGenLeadingJetPt > 70: neventden[2] += 1
        elif event.TGenLeadingJetPt > 50: neventden[1] += 1
        else:                             neventden[0] += 1
    nevent += 1
    if (nevent % 10000) == 0: print "Event number", nevent

for el in output: print el
print "nevs denominador", neventden
