import ROOT as r
import sys, os, copy, array
import varList as vl
import beautifulUnfoldingPlots as bp
from multiprocessing import Pool

r.gROOT.SetBatch(True)

pathtothings = "temp/"
#pathtothings = "temp_backup_2019_12_18_nuevobinningjetptSOLOESA/"
outpath      = "/nfs/fanae/user/vrbouza/www/2020_01_30_unc_variations/"
responsesourcepath = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/"
#varname = "LeadingLepPt"

groups    = ["Non-WorZ", "VVttbarV", "DY", "ttbar", "tW"]
variables = ["LeadingLepPt", "LeadingJetPt", "DPhiLL", "DilepMETJet1Pz", "MT_LLMETB", "M_LLB", "Fiducial"]
#variables = ["LeadingJetPt", "Fiducial"]

variations = ["Btag", "ElecEff", "JES", "Mistag", "MuonEff", "PU", "TopPt", "Trig", "JER", "UE", "hDamp", "isr", "pdf", "tWME", "ttbarME", "fsr", "ColourQCDbasedCRTuneerdON", "ColourGluonMoveCRTuneerdON", "ColourPowhegerdON", "ColourGluonMoveCRTune", "ttbar", "Non-WorZ", "VVttbarV", "DY", "mtop"]

colour_syst    = ["ColourQCDbasedCRTuneerdON", "ColourGluonMoveCRTuneerdON", "ColourPowhegerdON", "ColourGluonMoveCRTune"]
colour_colours = [r.kRed, r.kBlue, r.kGreen, r.kViolet]

### ACORDASE DEL CASO ESPECIAL DE ME!!!

colours = {
    "DY":       852,
    "Non-WorZ": 413,
    "VVttbarV": 390,
    "tW":       r.TColor.GetColor("#ffcc33"),
    "ttbar":    633
}

maxima = {
    "LeadingLepPt"  : 3100,
    "LeadingJetPt"  : 3100,
    "DPhiLL"        : 2000,
    "DilepMETJet1Pz": 3000,
    "M_LLB"         : 3100,
    "MT_LLMETB"     : 2800
}

maximaunf = {
    "LeadingLepPt"  : 0.006,
    "LeadingJetPt"  : 0.008,
    "DPhiLL"        : 0.5,
    "DilepMETJet1Pz": 0.0014,
    "M_LLB"         : 0.0025,
    "MT_LLMETB"     : 0.003
}

maximafold = {
    "LeadingLepPt"  : 1000,
    "LeadingJetPt"  : 1000,
    "DPhiLL"        : 1000,
    "DilepMETJet1Pz": 1000,
    "M_LLB"         : 1000,
    "MT_LLMETB"     : 1000
}

def mkdir(path):
    if not os.path.isdir(path):
        os.system("mkdir -p " + path)
        os.system("cp ~/www/index.php " + path + "/")


def damelasmiovariaciones(tsk):
    variacion, ty = tsk
    #for varname in vl.varList["Names"]["Variables"]:
    if ty == "folded":
        damelasmiovariacionesprofolded(variacion)
        return

    for varname in variables:
        print "\n> Producing for variable", varname, "and variation", variacion, "for type", ty
        if "Fiducial" in varname: continue
        fullpath = pathtothings + varname + "_/" + "forBeautifulPlots_{var}.root".format(var = varname) * (ty == "mcdata") + (ty == "unf") * "normOutput_{var}.root".format(var = varname)
        cutout   = r.TFile.Open(fullpath, "READ")

        #binning = array.array("f", vl.varList[varname]["recobinning"])

        textSize = 0.035 if "unc" in varname else 0.0435
        if "unc" in varname: legWidth = 0.13
        else:                legWidth = 0.2
        height = (.18 + textSize * max(2 - 3, 0))

        corner = "TR"

        if isinstance(corner, str):
            if corner == "TR":
                (x1,y1,x2,y2) = ( .72 - legWidth, .9 - height, .735,         .9)
            elif corner == "TC":
                (x1,y1,x2,y2) = (.5,              .9 - height, .55+legWidth, .9)
            elif corner == "TL":
                (x1,y1,x2,y2) = (.18,             .78 - height, .18+legWidth, .78)
            elif corner == "MR":
                (x1,y1,x2,y2) = (.85 - legWidth,  .3 + height, .90,          .3)
            elif corner == "MC":
                (x1,y1,x2,y2) = (.5,              .3 + height, .5+legWidth,  .3)
            elif corner == "ML":
                (x1,y1,x2,y2) = (.18,             .3 + height, .18+legWidth, .3)
            elif corner == "BR":
                (x1,y1,x2,y2) = (.85 - legWidth,  .1 + height, .90,          .1)
            elif corner == "BC":
                (x1,y1,x2,y2) = (.5,              .1 + height, .5+legWidth,  .1)
            elif corner == "BL":
                (x1,y1,x2,y2) = (.18,             .1 + height, .18+legWidth,  .1)

        histolist = []

        if ty == "unf":
            plot = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_values_unf") if "Colour" not in variacion else "ColourReconnection_values_unf")
            plot.doRatio       = False
            plot.doFit         = False
            plot.doPreliminary = False
            plot.noCMS         = True
            plot.doSupplementary = False
            plot.plotspath     = outpath + "/{var}/".format(var = varname)

            legloc = ""

            if "legpos_unf" in vl.varList[varname]: legloc = vl.varList[varname]["legpos_unf"]
            else:                                   legloc = "TR"

            legloc = "TR"

            found = False
            nominal  = r.TH1F()
            for key in cutout.GetListOfKeys():
                if key.GetName() == varname + "_{vr}Up".format(vr = variacion) and "Colour" not in variacion:
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}Up".format(vr = variacion)).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(r.kBlue)
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', variacion + "_Up", 'PEL', variacion + "_Up")
                elif key.GetName() == varname + "_{vr}Down".format(vr = variacion) and "Colour" not in variacion:
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}Down".format(vr = variacion)).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(r.kRed)
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', variacion + "_Down", 'PEL', variacion + "_Down")
                elif (key.GetName() == varname + "_{vr}".format(vr = colour_syst[0].replace("Colour", ""))) and "Colour" in variacion:   ### Colour reconnection
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}".format(vr = colour_syst[0].replace("Colour", ""))).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(colour_colours[0])
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', colour_syst[0].replace("Colour", "") + "_", 'PEL', colour_syst[0].replace("Colour", "") )
                elif (key.GetName() == varname + "_{vr}".format(vr = colour_syst[1].replace("Colour", ""))) and "Colour" in variacion:
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}".format(vr = colour_syst[1].replace("Colour", ""))).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(colour_colours[1])
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', colour_syst[1].replace("Colour", ""), 'PEL', colour_syst[1].replace("Colour", ""))
                elif (key.GetName() == varname + "_{vr}".format(vr = colour_syst[2].replace("Colour", ""))) and "Colour" in variacion:
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}".format(vr = colour_syst[2].replace("Colour", ""))).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(colour_colours[2])
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', colour_syst[2].replace("Colour", ""), 'PEL', colour_syst[2].replace("Colour", ""))
                elif (key.GetName() == varname + "_{vr}".format(vr = colour_syst[3].replace("Colour", ""))) and "Colour" in variacion:
                    histolist.append(r.TH1F())
                    histolist[-1] = copy.deepcopy(cutout.Get(varname + "_{vr}".format(vr = colour_syst[3].replace("Colour", ""))).Clone())
                    found = True
                    #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                    #histolist[-1].GetYaxis().SetRangeUser(0, maximaunf[varname])
                    histolist[-1].SetMaximum(maximaunf[varname])
                    histolist[-1].SetLineColor(colour_colours[3])
                    histolist[-1].SetFillStyle(0)
                    plot.addHisto(histolist[-1], 'P,E,same', colour_syst[3].replace("Colour", "") + "_", 'PEL', colour_syst[3].replace("Colour", ""))
                elif key.GetName() == varname:
                    nominal = copy.deepcopy(cutout.Get(varname).Clone("nominal"))
                    #nominal.GetYaxis().SetRangeUser(0, maximaunf[varname])
                    nominal.SetMaximum(maximaunf[varname])
                    nominal.SetLineColor(r.kBlack)
                    nominal.SetFillStyle(0)
                    plot.addHisto(nominal, 'P,E,same', "Nominal", 'PEL', "Nominal")

            r.gPad.RedrawAxis();
            plot.saveCanvas(legloc)
            del plot

            legloc = "TC"

            histnom  = copy.deepcopy(cutout.Get(varname).Clone("histnom"))
            unctotUp = copy.deepcopy(cutout.Get("nom0").Clone("unctotUp"))
            unctotDn = copy.deepcopy(cutout.Get("nom1").Clone("unctotDn"))

            unctotmax= copy.deepcopy(unctotUp.Clone("unctotmax"))
            for bin in range(1, unctotmax.GetNbinsX() + 1):
                unctotmax.SetBinError(bin, max(unctotUp.GetBinError(bin), unctotDn.GetBinError(bin)))
                #print "unctotmax bin", bin, ":", unctotmax.GetBinError(bin)

            unctotmaxrel = copy.deepcopy(cutout.Get("hincmax").Clone())
            uncsysrel    = copy.deepcopy(cutout.Get("hincsyst").Clone())

            plotrel = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_uncrel_unf") if "Colour" not in variacion else "ColourReconnection_uncrel_unf")
            plotrel.doRatio         = False
            plotrel.doFit           = False
            plotrel.doPreliminary   = False
            plotrel.noCMS           = True
            plotrel.doSupplementary = False
            plotrel.plotspath       = outpath + "/{var}/".format(var = varname)

            unchistolist = []

            unctotmaxrel.SetFillStyle(0)
            uncsysrel.SetFillStyle(0)
            plotrel.addHisto(unctotmaxrel, 'H,same', "Total",            "L")
            plotrel.addHisto(uncsysrel,    'H,same', "Total systematic", "L")
            for minivar in histolist:
                unchistolist.append(copy.deepcopy(unctotmaxrel.Clone(minivar.GetName().replace(varname + "_", "") + "_uncrel")))
                for bin in range(1, unchistolist[-1].GetNbinsX() + 1):
                    unchistolist[-1].SetBinContent(bin, ( abs(minivar.GetBinContent(bin) - histnom.GetBinContent(bin)) if (minivar.GetBinContent(bin) > histnom.GetBinContent(bin)) or (minivar.GetBinContent(bin) < histnom.GetBinContent(bin) and minivar.GetBinContent(bin) >= 0) else histnom.GetBinContent(bin) ) / histnom.GetBinContent(bin) )
                    unchistolist[-1].SetBinError(bin, 0)
                    unchistolist[-1].SetLineWidth(1)
                    if "Down" in minivar.GetName(): unchistolist[-1].SetLineColor(r.kRed)
                    elif "Colour" not in variacion: unchistolist[-1].SetLineColor(r.kBlue)
                    elif "Colour" in variacion    : unchistolist[-1].SetLineColor(minivar.GetLineColor())
                    #print "histo", minivar.GetName(), "bin:", bin, ":", unchistolist[-1].GetBinContent(bin)
                unchistolist[-1].SetFillStyle(0)
                plotrel.addHisto(unchistolist[-1], 'H,same', minivar.GetName().replace(varname + "_", ""), 'L')


            if len(unchistolist) == 2:
                unchistolist.append(copy.deepcopy(unchistolist[-1].Clone("Average")))
                for bin in range(1, unctotmaxrel.GetNbinsX() + 1):
                    unchistolist[-1].SetBinContent(bin, (unchistolist[-1].GetBinContent(bin) + unchistolist[-3].GetBinContent(bin))/2)
                unchistolist[-1].SetLineColor(r.kGreen)
                plotrel.addHisto(unchistolist[-1], 'H,same', "Average", 'L')

            r.gPad.RedrawAxis();
            plotrel.saveCanvas(legloc)
            del plotrel

            plotabs = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_uncabs_unf") if "Colour" not in variacion else "ColourReconnection_uncabs_unf")
            plotabs.doRatio         = False
            plotabs.doFit           = False
            plotabs.doPreliminary   = False
            plotabs.noCMS           = True
            plotabs.doSupplementary = False
            plotabs.plotspath       = outpath + "/{var}/".format(var = varname)

            unctotmaxabs = copy.deepcopy(unctotmaxrel.Clone("unctotmaxabs"))
            uncsysabs    = copy.deepcopy(uncsysrel.Clone("uncsysabs"))

            absmaximum = 0

            for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
                unctotmaxabs.SetBinContent(bin, unctotmaxabs.GetBinContent(bin) * histnom.GetBinContent(bin))
                uncsysabs.SetBinContent(bin,    uncsysabs.GetBinContent(bin)    * histnom.GetBinContent(bin))
                if unctotmaxabs.GetBinContent(bin) > absmaximum: absmaximum = unctotmaxabs.GetBinContent(bin)

            unctotmaxabs.SetMaximum(absmaximum * 1.1)

            unctotmaxabs.GetYaxis().SetTitle("Absolute uncertainty (pb/GeV)")

            unctotmaxabs.SetFillStyle(0)
            uncsysabs.SetFillStyle(0)
            plotabs.addHisto(unctotmaxabs, 'H,same', "Total", "L")
            plotabs.addHisto(uncsysabs,    'H,same', "Total systematic",     "L")

            unchistoabslist = []

            for minivar in unchistolist:
                unchistoabslist.append(minivar.Clone(minivar.GetName() + "_abs"))
                for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
                    unchistoabslist[-1].SetBinContent(bin, unchistoabslist[-1].GetBinContent(bin) * histnom.GetBinContent(bin))
                    #print "histo", minivar.GetName(), "bin:", bin, ":", unchistoabslist[-1].GetBinContent(bin)
                    #print "nominal bin:", bin, ":", histnom.GetBinContent(bin)
                unchistoabslist[-1].SetFillStyle(0)
                plotabs.addHisto(unchistoabslist[-1], 'H,same', minivar.GetName().replace(varname + "_", ""), 'L')

            if len(unchistoabslist) == 2:
                unchistoabslist.append(copy.deepcopy(unchistoabslist[-1].Clone("Average")))
                for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
                    unchistoabslist[-1].SetBinContent(bin, (unchistoabslist[-1].GetBinContent(bin) + unchistoabslist[-3].GetBinContent(bin))/2)
                unchistoabslist[-1].SetLineColor(r.kGreen)
                plotabs.addHisto(unchistoabslist[-1], 'H,same', "Average", 'L')

            r.gPad.RedrawAxis();
            plotabs.saveCanvas(legloc)
            del plotabs

        else:
            leg = r.TLegend(x1,y1,x2,y2)
            leg.SetTextFont(42)
            leg.SetTextSize(textSize)
            leg.SetBorderSize(0)
            leg.SetFillColor(10)
            leg.SetFillStyle(0); # transparent legend!
            stack = r.THStack("stack", "")

            for gr in groups:
                histolist.append(r.TH1F())
                found = False
                for key in cutout.GetListOfKeys():
                    if key.GetName() == gr + "_{vr}".format(vr = variacion.replace("Colour", "")):
                        histolist[-1] = copy.deepcopy(cutout.Get(gr + "_{vr}".format(vr = variacion.replace("Colour", ""))).Clone())
                        found = True
                if not found: histolist[-1] = copy.deepcopy(cutout.Get(gr).Clone())
                leg.AddEntry(histolist[-1], gr, "f")
                histolist[-1].SetFillColor(colours[gr])
                histolist[-1].SetLineWidth(0)
                stack.Add(histolist[-1])

            #stack.GetYaxis().SetRangeUser(0, maxima[varname])
            stack.SetMaximum(maxima[varname])

            datos = r.TH1F(cutout.Get("data_obs").Clone("datos"))
            leg.AddEntry(datos, "Data", "P")

            c = r.TCanvas("c", "", 600, 600)

            c.Divide(1, 2)
            c.GetPad(1).SetPad(*vl.plotlimits)
            c.GetPad(2).SetPad(*vl.ratiolimits)
            c.GetPad(1).SetTopMargin(0.08)
            c.GetPad(1).SetRightMargin(0.03)
            c.GetPad(1).SetLeftMargin(0.16)
            c.GetPad(1).SetBottomMargin(0.025)
            c.GetPad(2).SetBottomMargin(0.35)
            c.GetPad(2).SetBottomMargin(0.375)
            c.GetPad(2).SetRightMargin(0.03)
            c.GetPad(2).SetLeftMargin(0.16)

            c.cd(1)

            stack.Draw("Ehist")
            datos.Draw("Psame")
            leg.Draw("same")

            c.cd(2)

            hratio = copy.deepcopy(datos.Clone("ratio"))
            hratio.Divide(stack.GetStack().Last())

            unity = copy.deepcopy(datos.Clone("unity"))
            for bin in range(1, unity.GetNbinsX() + 1): unity.SetBinContent(bin, 1.0)
            unity.SetLineStyle(2)
            unity.SetLineColor(r.kBlack)

            hratio.GetXaxis().SetTitle(vl.varList[varname.replace('_folded', '').replace('_asimov', '').replace("_fiducial", "").replace('_norm', '').replace("norm", "")]['xaxis'])
            hratio.GetXaxis().SetTitleFont(43)
            hratio.GetXaxis().SetTitleSize(22)
            hratio.GetXaxis().SetTitleOffset(4)
            hratio.GetXaxis().SetLabelFont(43)
            hratio.GetXaxis().SetLabelSize(22)
            hratio.GetXaxis().SetLabelOffset(0.007)
            hratio.GetXaxis().SetNdivisions(510, True)

            hratio.GetYaxis().SetRangeUser(0.5, 1.5)
            hratio.SetLineColor(r.kBlack)
            hratio.GetYaxis().SetTitle('Data/MC')
            hratio.GetYaxis().SetTitleFont(43)
            hratio.GetYaxis().SetTitleSize(22)
            hratio.GetYaxis().SetTitleOffset(2.2)
            hratio.GetYaxis().CenterTitle(True)
            hratio.GetYaxis().SetLabelFont(43)
            hratio.GetYaxis().SetLabelSize(22)
            hratio.GetYaxis().SetLabelOffset(0.007)
            hratio.GetYaxis().SetNdivisions(505, True)
            hratio.Draw("sameL")

            unity.Draw("samehist")


            mkdir(outpath + "{var}".format(var = varname))
            c.SaveAs(outpath + "/{var}/{var}_{vr}{t}.png".format (var = varname, vr = ("norm" if (variacion == "ttbarUp" or variacion == "VVttbarVUp" or variacion == "DYUp" or variacion == "Non-WorZUp" or variacion == "ttbarDown" or variacion == "VVttbarVDown" or variacion == "DYDown" or variacion == "Non-WorZDown") else "") + variacion, t = "_unf" if ty == "unf" else "_mcdata"))
            c.SaveAs(outpath + "/{var}/{var}_{vr}{t}.pdf".format (var = varname, vr = ("norm" if (variacion == "ttbarUp" or variacion == "VVttbarVUp" or variacion == "DYUp" or variacion == "Non-WorZUp" or variacion == "ttbarDown" or variacion == "VVttbarVDown" or variacion == "DYDown" or variacion == "Non-WorZDown") else "") + variacion, t = "_unf" if ty == "unf" else "_mcdata"))
            c.SaveAs(outpath + "/{var}/{var}_{vr}{t}.root".format(var = varname, vr = ("norm" if (variacion == "ttbarUp" or variacion == "VVttbarVUp" or variacion == "DYUp" or variacion == "Non-WorZUp" or variacion == "ttbarDown" or variacion == "VVttbarVDown" or variacion == "DYDown" or variacion == "Non-WorZDown") else "") + variacion, t = "_unf" if ty == "unf" else "_mcdata"))
            c.SaveAs(outpath + "/{var}/{var}_{vr}{t}.C".format   (var = varname, vr = ("norm" if (variacion == "ttbarUp" or variacion == "VVttbarVUp" or variacion == "DYUp" or variacion == "Non-WorZUp" or variacion == "ttbarDown" or variacion == "VVttbarVDown" or variacion == "DYDown" or variacion == "Non-WorZDown") else "") + variacion, t = "_unf" if ty == "unf" else "_mcdata"))
            c.Close()
            del c

        cutout.Close()
    return



def damelasmiovariacionesprofolded(variacion):
    for varname in variables:
        print "\n> Producing for variable", varname, "and variation", variacion, "for type folded"
        if "Fiducial" in varname: continue
        fullpath = pathtothings + varname + "_/" + "cutOutput_{var}.root".format(var = varname)
        cutout   = r.TFile.Open(fullpath, "READ")

        textSize = 0.035 if "unc" in varname else 0.0435
        if "unc" in varname: legWidth = 0.13
        else:                legWidth = 0.2
        height = (.18 + textSize * max(2 - 3, 0))

        corner = "TR"

        if isinstance(corner, str):
            if corner == "TR":
                (x1,y1,x2,y2) = ( .72 - legWidth, .9 - height, .735,         .9)
            elif corner == "TC":
                (x1,y1,x2,y2) = (.5,              .9 - height, .55+legWidth, .9)
            elif corner == "TL":
                (x1,y1,x2,y2) = (.18,             .78 - height, .18+legWidth, .78)
            elif corner == "MR":
                (x1,y1,x2,y2) = (.85 - legWidth,  .3 + height, .90,          .3)
            elif corner == "MC":
                (x1,y1,x2,y2) = (.5,              .3 + height, .5+legWidth,  .3)
            elif corner == "ML":
                (x1,y1,x2,y2) = (.18,             .3 + height, .18+legWidth, .3)
            elif corner == "BR":
                (x1,y1,x2,y2) = (.85 - legWidth,  .1 + height, .90,          .1)
            elif corner == "BC":
                (x1,y1,x2,y2) = (.5,              .1 + height, .5+legWidth,  .1)
            elif corner == "BL":
                (x1,y1,x2,y2) = (.18,             .1 + height, .18+legWidth,  .1)

        histolist = []

        plot = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_values_folded") if "Colour" not in variacion else "ColourReconnection_values_folded")
        plot.doRatio       = False
        plot.doFit         = False
        plot.doPreliminary = False
        plot.noCMS         = True
        plot.doSupplementary = False
        plot.plotspath     = outpath + "/{var}/folded_distributions/".format(var = varname)

        legloc = ""

        if "legpos_unf" in vl.varList[varname]: legloc = vl.varList[varname]["legpos_unf"]
        else:                                   legloc = "TR"

        legloc = "TR"

        found = False
        nominal  = r.TH1F()
        for key in cutout.GetListOfKeys():
            if key.GetName() == "data_{vr}Up".format(vr = variacion) and "Colour" not in variacion:
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}Up".format(vr = variacion)).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(r.kBlue)
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', variacion + "_Up", 'PEL', variacion + "_Up")
            elif key.GetName() == "data_{vr}Down".format(vr = variacion) and "Colour" not in variacion:
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}Down".format(vr = variacion)).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(r.kRed)
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', variacion + "_Down", 'PEL', variacion + "_Down")
            elif (key.GetName() == "data_{vr}".format(vr = colour_syst[0].replace("Colour", ""))) and "Colour" in variacion:   ### Colour reconnection
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}".format(vr = colour_syst[0].replace("Colour", ""))).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(colour_colours[0])
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', colour_syst[0].replace("Colour", ""), 'PEL', colour_syst[0].replace("Colour", "") )
            elif (key.GetName() == "data_{vr}".format(vr = colour_syst[1].replace("Colour", ""))) and "Colour" in variacion:
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}".format(vr = colour_syst[1].replace("Colour", ""))).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(colour_colours[1])
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', colour_syst[1].replace("Colour", ""), 'PEL', colour_syst[1].replace("Colour", ""))
            elif (key.GetName() == "data_{vr}".format(vr = colour_syst[2].replace("Colour", ""))) and "Colour" in variacion:
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}".format(vr = colour_syst[2].replace("Colour", ""))).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(colour_colours[2])
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', colour_syst[2].replace("Colour", ""), 'PEL', colour_syst[2].replace("Colour", ""))
            elif (key.GetName() == "data_{vr}".format(vr = colour_syst[3].replace("Colour", ""))) and "Colour" in variacion:
                histolist.append(r.TH1F())
                histolist[-1] = copy.deepcopy(cutout.Get("data_{vr}".format(vr = colour_syst[3].replace("Colour", ""))).Clone())
                found = True
                #print "\nMASIMU:", histolist[-1].GetMaximum() + histolist[-1].GetMaximum()*0.075
                #histolist[-1].GetYaxis().SetRangeUser(0, maximafold[varname])
                histolist[-1].SetMaximum(maximafold[varname])
                histolist[-1].SetLineColor(colour_colours[3])
                histolist[-1].SetFillStyle(0)
                histolist[-1].SetMarkerSize(0)
                plot.addHisto(histolist[-1], 'P,E,same', colour_syst[3].replace("Colour", "") + "_", 'PEL', colour_syst[3].replace("Colour", ""))
            elif key.GetName() == "data_":
                nominal = copy.deepcopy(cutout.Get("data_").Clone("nominal"))
                #nominal.GetYaxis().SetRangeUser(0, maximafold[varname])
                nominal.SetMaximum(maximafold[varname])
                nominal.SetLineColor(r.kBlack)
                nominal.SetFillStyle(0)
                nominal.SetMarkerSize(0)
                plot.addHisto(nominal, 'P,E,same', "Nominal", 'PEL', "Nominal")

        r.gPad.RedrawAxis();
        plot.saveCanvas(legloc)
        del plot

        legloc = "TC"

        histnom  = copy.deepcopy(cutout.Get("data_").Clone("histnom"))
        unctotUp = copy.deepcopy(cutout.Get("nom0").Clone("unctotUp"))
        unctotDn = copy.deepcopy(cutout.Get("nom1").Clone("unctotDn"))

        unctotmax= copy.deepcopy(unctotUp.Clone("unctotmax"))
        for bin in range(1, unctotmax.GetNbinsX() + 1):
            unctotmax.SetBinError(bin, max(unctotUp.GetBinError(bin), unctotDn.GetBinError(bin)))
            #print "unctotmax bin", bin, ":", unctotmax.GetBinError(bin)

        unctotmaxrel = copy.deepcopy(cutout.Get("hincmax").Clone())
        uncsysrel    = copy.deepcopy(cutout.Get("hincsyst").Clone())

        plotrel = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_uncrel_folded") if "Colour" not in variacion else "ColourReconnection_uncrel_folded")
        plotrel.doRatio         = False
        plotrel.doFit           = False
        plotrel.doPreliminary   = False
        plotrel.noCMS           = True
        plotrel.doSupplementary = False
        plotrel.plotspath       = outpath + "/{var}/folded_distributions/".format(var = varname)

        unchistolist = []

        unctotmaxrel.SetFillStyle(0)
        uncsysrel.SetFillStyle(0)
        plotrel.addHisto(unctotmaxrel, 'H,same', "Total",            "L")
        plotrel.addHisto(uncsysrel,    'H,same', "Total systematic", "L")
        for minivar in histolist:
            unchistolist.append(copy.deepcopy(unctotmaxrel.Clone(minivar.GetName().replace("data_", "") + "_uncrel")))
            for bin in range(1, unchistolist[-1].GetNbinsX() + 1):
                unchistolist[-1].SetBinContent(bin, ( abs(minivar.GetBinContent(bin) - histnom.GetBinContent(bin)) if (minivar.GetBinContent(bin) > histnom.GetBinContent(bin)) or (minivar.GetBinContent(bin) < histnom.GetBinContent(bin) and minivar.GetBinContent(bin) >= 0) else histnom.GetBinContent(bin) ) / histnom.GetBinContent(bin) )
                unchistolist[-1].SetBinError(bin, 0)
                unchistolist[-1].SetLineWidth(1)
                unchistolist[-1].SetMarkerSize(0)
                if "Down" in minivar.GetName(): unchistolist[-1].SetLineColor(r.kRed)
                elif "Colour" not in variacion: unchistolist[-1].SetLineColor(r.kBlue)
                elif "Colour" in variacion:     unchistolist[-1].SetLineColor(minivar.GetLineColor())
                #print "histo", minivar.GetName(), "bin:", bin, ":", unchistolist[-1].GetBinContent(bin)
            unchistolist[-1].SetFillStyle(0)
            plotrel.addHisto(unchistolist[-1], 'H,same', minivar.GetName().replace("data_", ""), 'L')


        if len(unchistolist) == 2:
            unchistolist.append(copy.deepcopy(unchistolist[-1].Clone("Average")))
            for bin in range(1, unctotmaxrel.GetNbinsX() + 1):
                unchistolist[-1].SetBinContent(bin, (unchistolist[-1].GetBinContent(bin) + unchistolist[-3].GetBinContent(bin))/2)
            unchistolist[-1].SetLineColor(r.kGreen)
            plotrel.addHisto(unchistolist[-1], 'H,same', "Average", 'L')

        r.gPad.RedrawAxis();
        plotrel.saveCanvas(legloc)
        del plotrel

        plotabs = bp.beautifulUnfoldingPlots((varname + "_" + ("norm" if (variacion == "ttbar" or variacion == "VVttbarV" or variacion == "DY" or variacion == "Non-WorZ") else "") + variacion + "_uncabs_folded") if "Colour" not in variacion else "ColourReconnection_uncabs_folded")
        plotabs.doRatio         = False
        plotabs.doFit           = False
        plotabs.doPreliminary   = False
        plotabs.noCMS           = True
        plotabs.doSupplementary = False
        plotabs.plotspath       = outpath + "/{var}/folded_distributions/".format(var = varname)

        unctotmaxabs = copy.deepcopy(unctotmaxrel.Clone("unctotmaxabs"))
        uncsysabs    = copy.deepcopy(uncsysrel.Clone("uncsysabs"))

        absmaximum = 0

        for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
            unctotmaxabs.SetBinContent(bin, unctotmaxabs.GetBinContent(bin) * histnom.GetBinContent(bin))
            uncsysabs.SetBinContent(bin,    uncsysabs.GetBinContent(bin)    * histnom.GetBinContent(bin))
            if unctotmaxabs.GetBinContent(bin) > absmaximum: absmaximum = unctotmaxabs.GetBinContent(bin)

        unctotmaxabs.SetMaximum(absmaximum * 1.1)

        unctotmaxabs.GetYaxis().SetTitle("Absolute uncertainty (pb/GeV)")

        unctotmaxabs.SetFillStyle(0)
        uncsysabs.SetFillStyle(0)
        plotabs.addHisto(unctotmaxabs, 'H,same', "Total", "L")
        plotabs.addHisto(uncsysabs,    'H,same', "Total systematic",     "L")

        unchistoabslist = []

        for minivar in unchistolist:
            unchistoabslist.append(minivar.Clone(minivar.GetName() + "_abs"))
            for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
                unchistoabslist[-1].SetBinContent(bin, unchistoabslist[-1].GetBinContent(bin) * histnom.GetBinContent(bin))
                #print "histo", minivar.GetName(), "bin:", bin, ":", unchistoabslist[-1].GetBinContent(bin)
                #print "nominal bin:", bin, ":", histnom.GetBinContent(bin)
            unchistoabslist[-1].SetFillStyle(0)
            plotabs.addHisto(unchistoabslist[-1], 'H,same', minivar.GetName().replace(varname + "_", ""), 'L')

        if len(unchistoabslist) == 2:
            unchistoabslist.append(copy.deepcopy(unchistoabslist[-1].Clone("Average")))
            for bin in range(1, unctotmaxabs.GetNbinsX() + 1):
                unchistoabslist[-1].SetBinContent(bin, (unchistoabslist[-1].GetBinContent(bin) + unchistoabslist[-3].GetBinContent(bin))/2)
            unchistoabslist[-1].SetLineColor(r.kGreen)
            plotabs.addHisto(unchistoabslist[-1], 'H,same', "Average", 'L')

        r.gPad.RedrawAxis();
        plotabs.saveCanvas(legloc)
        del plotabs
        cutout.Close()
    return


def CopyResponseMatrices(variacion):
    #for varname in vl.varList["Names"]["Variables"]:
    for varname in variables:
        if "Fiducial" in varname: continue
        mkdir(outpath + "/{var}/response_matrices".format(var = varname))

        os.system("cp -r " + responsesourcepath + "/" + varname + "/R_T" + varname.replace("_", "") + "_" + variacion + "Up.* " + outpath + "/{var}/response_matrices".format(var = varname) + "/")
        if ("JER" not in variacion) and ("DS" not in variacion):
            os.system("cp -r " + responsesourcepath + "/" + varname + "/R_T" + varname.replace("_", "") + "_" + variacion + "Down.* " + outpath + "/{var}/response_matrices".format(var = varname) + "/")



tasks     = []
copytasks = []
for var in variations:
    if "ttbar" not in var and "VVttbarV" not in var and "DY" not in var and "Non-WorZ" not in var and "Colour" not in var and "hDamp" not in var and "UE" not in var and "pdf" not in var and "ttbarME" not in var: copytasks.append(var)
    if "Colour" not in var:
        tasks.append((var + "Up", "mcdata"))
        if "ttbarME" not in var:
            tasks.append((var, "unf"))
        if "tWME" not in var:
            tasks.append((var, "folded"))
        if ("JER" not in var) and ("DS" not in var):
            tasks.append((var + "Down", "mcdata"))
    else:
        tasks.append((var, "mcdata"))
        if var == "ColourQCDbasedCRTuneerdON":
            tasks.append((var, "unf"))
            tasks.append((var, "folded"))

#tasks = [("JES", "unf")]

if len(sys.argv) > 1:
    nCores = int(sys.argv[1])
    pool   = Pool(nCores)
    pool.map(damelasmiovariaciones, tasks)
    pool.map(CopyResponseMatrices, copytasks)
    pool.close()
    pool.join()
else:
    for tsk in tasks:     damelasmiovariaciones(tsk)
    for tsk in copytasks: CopyResponseMatrices(tsk)


