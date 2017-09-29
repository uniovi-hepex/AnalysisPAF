'''
 Read the datasets and get values of masses for the neutralino, stop and limits

 Author: Juan Rodrigo Gonzalez Fernandez
'''
import re
from ROOT import *
import numpy as np

# input amd output dir
inputdir = '/mnt_pool/fanae105/user/juanr/StopIndirect/'
outputdir = '/mnt_pool/fanae105/user/juanr/StopIndirect/plots/'

f = open(inputdir  + "AsymptoticLimits.txt", "r")
text = f.read()

# neutralino masses (GeV)
mneutralino = ["1.0", "12.5", "25.0", "37.5"]

# Matrices with values...
mstop = []; obs = []; l2sigmad = []; l2sigmau = []; l1sigmad = []; l1sigmau = []; lexp = [];

# Get the values:
textlines = text.split("\n")
for i in range(len(mneutralino)):
	nlin = -1
	vmt = []; vob = []; vexp2sd = []; vexp1sd = []; vexp1su = []; vexp2su = []; vexpc = [];
	for line in textlines:
		nlin += 1
		if ("mneutra"+mneutralino[i]) in line:
			#print line.strip(), "nlin = ", nlin
			mt = float((re.search("mtop(.+?)_", line.strip())).group(1))
			ob = float((re.search("r < (.*)" , textlines[nlin+4])).group(1))
			exp2sd = float((re.search("r < (.*)" , textlines[nlin+5])).group(1)) 
			exp1sd = float((re.search("r < (.*)" , textlines[nlin+6])).group(1)) 
			expc = float((re.search("r < (.*)" , textlines[nlin+7])).group(1)) 
			exp1su = float((re.search("r < (.*)" , textlines[nlin+8])).group(1)) 
			exp2su = float((re.search("r < (.*)" , textlines[nlin+9])).group(1))
			vmt.append(mt); vob.append(ob); vexp2sd.append(exp2sd); vexp1sd.append(exp1sd); vexpc.append(expc); vexp1su.append(exp1su); vexp2su.append(exp2su);
	mstop.append(vmt); obs.append(vob); l2sigmad.append(vexp2sd); l1sigmad.append(vexp1sd); l2sigmau.append(vexp2su); l1sigmau.append(vexp1su); lexp.append(vexpc);

# Draw the 95% CL limit plots
def draw(k):
   n = len(mstop[k]);
   xp = mstop[k]; op = obs[k]; ep = lexp[k]; y1minp = l1sigmad[k]; y1maxp = l1sigmau[k]; y2minp = l2sigmad[k]; y2maxp = l2sigmau[k];
   x = np.array(xp); o = np.array(op); e = np.array(ep); y1max = np.array(y1maxp); y1min = np.array(y1minp); y2max = np.array(y2maxp); y2min = np.array(y2minp)

   c1 = TCanvas("c1","CL",200,10,700,500);
   c1.SetGrid();
   # c1.DrawFrame(150,0.5,240,5);
   c1.DrawFrame(min(x)-2,min(y2maxp+y2minp)-0.2,max(x)+2,max(y2minp+y2maxp)+0.2);

   gr1min = TGraph(n,x,y1min);
   gr1max = TGraph(n,x,y1max);
   gr2min = TGraph(n,x,y2min);
   gr2max = TGraph(n,x,y2max);
   gro    = TGraph(n,x,o);
   gre    = TGraph(n,x,e);
   gh     = TGraph(n,x, np.linspace(0.999,1,n));
   gr1shade = TGraph(2*n);
   gr2shade = TGraph(2*n);
   for i in range(n):
      gr1shade.SetPoint(i,x[i],y1max[i]);
      gr1shade.SetPoint(n+i,x[n-i-1],y1min[n-i-1]);
      gr2shade.SetPoint(i,x[i],y2max[i]);
      gr2shade.SetPoint(n+i,x[n-i-1],y2min[n-i-1]);

   gr2shade.SetFillColor(5); #grshade.SetFillStyle(3013);
   gr2shade.Draw("f");
   gr1shade.SetFillColor(3);
   gr1shade.Draw("f");
   gh.SetLineWidth(2);
   gh.SetMarkerStyle(0);
   gh.SetLineColor(46);
   gh.SetLineStyle(2);
   gh.Draw("LP");
   gro.SetLineWidth(2);
   gro.SetMarkerStyle(20);
   gro.SetMarkerSize(0.7);
   gro.SetLineColor(1);
   gro.Draw("LP");
   gre.SetLineWidth(2);
   gre.SetMarkerStyle(0);
   gre.SetLineColor(1);
   gre.SetLineStyle(6);
   gre.SetTitle("fdasfasd")
   gre.Draw("LP");

   gre.SetTitle("Expected") 
   gro.SetTitle("Observed") 
   gr1shade.SetTitle("Expected 1#sigma") 
   gr2shade.SetTitle("Expected 2#sigma") 

   leg = TLegend(.1, .7, .3, .9)
   leg.AddEntry(gro)
   leg.AddEntry(gre)
   leg.AddEntry(gr1shade)
   leg.AddEntry(gr2shade)
   leg.SetFillColor(0)
   leg.Draw("same")

   gre.SetFillColor(0); gro.SetFillColor(0);
   gr1shade.SetLineColor(3); gr2shade.SetLineColor(5);

   Title = TLatex(); Title.DrawLatexNDC(.45, .94, "M_{#chi} = " + str(mneutralino[k]) + "GeV")
   Xaxis = TLatex(); Xaxis.DrawLatexNDC(0.8, 0.03, "M_{#tilde{t}} (GeV)")
   Yaxis = TLatex(); Yaxis.SetTextAngle(90); Yaxis.DrawLatexNDC(0.05, 0.15, "95% CL limit on signal strength")

   c1.Update();
   c1.Print(outputdir + "Asymptotic_mChi" + str(mneutralino[k]) + ".pdf", "pdf")
   c1.Print(outputdir + "Asymptotic_mChi" + str(mneutralino[k]) + ".png", "png")

   print "mneutralino = ", mneutralino[k]
   print "mstop = ", xp
   print "observed = ", o
   print "expected = ", e

# Execute
draw(0); draw(1); draw(2); draw(3);




