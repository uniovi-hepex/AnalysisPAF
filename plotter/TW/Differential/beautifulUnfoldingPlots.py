import ROOT  as r
import tdrstyle, varList, CMS_lumi
from ROOT import TH1, TEfficiency, TFile, TCanvas, TAxis

CMS_lumi.writeExtraText = 1


colorMap = [
    r.TColor.GetColor("#e41a1c"),
    r.TColor.GetColor("#377eb8"),
    r.TColor.GetColor("#4daf4a"),
    r.TColor.GetColor("#984ea3"),
    r.TColor.GetColor("#ff7f00"),
    r.TColor.GetColor("#ffff33"),
    r.TColor.GetColor("#a65628"),
    r.TColor.GetColor("#f781bf"),
    ]


class beautifulUnfoldingPlots:
    def __init__(self, name):
        r.gROOT.SetBatch(True)
        self.name           = name
        self.inited         = False
        self.objectsInLeg   = []
        self.plotspath      = ""
        self.doWide         = False
        self.doRatio        = False


    def initCanvasAndAll(self):
        if self.inited: return
        self.inited = True
        tdrstyle.setTDRStyle()

        topSpamSize = 1.2
        doOfficialCMS = True

        plotformat = (1200,600) if self.doWide else (600,600)
        height = plotformat[1]+150 if self.doRatio else plotformat[1]

        self.canvas = r.TCanvas(self.name+"_canvas", self.name, plotformat[0], height)
        
        self.canvas.SetTopMargin(self.canvas.GetTopMargin()*topSpamSize)
        topsize = 0.12*600./height if self.doRatio else 0.06*600./height
        if doOfficialCMS: self.canvas.SetTopMargin(topsize*1.2 if self.doWide else topsize)
        self.canvas.SetWindowSize(plotformat[0] + (plotformat[0] - self.canvas.GetWw()), (plotformat[1]+150 + (plotformat[1]+150 - self.canvas.GetWh())))
    
    
    def addHisto(self, histos, options, name, legOptions):
        if not self.inited: self.initCanvasAndAll()
        
        if isinstance(histos, list):
            histo = histos[0]
        else:
            histo = histos
        
        self.canvas.cd()
        if isinstance(histos, list):
            print '> Drawing an asym.-unc. histogram with the following options:', options
            asymhisto   =   r.TGraphAsymmErrors(histo)
            for bin in range(asymhisto.GetN()):
                asymhisto.SetPointEYhigh(bin, histo.GetBinError(bin + 1))
                asymhisto.SetPointEYlow(bin, histos[1].GetBinError(bin + 1))
            asymhisto.GetXaxis().SetTitle( varList.varList[self.name.replace('_folded', '')]['xaxis'] )
            asymhisto.GetYaxis().SetTitle( varList.varList[self.name.replace('_folded', '')]['yaxis'] )
            
            if '_folded' in self.name:
                asymhisto.GetXaxis().SetRangeUser(histo.GetXaxis().GetBinLowEdge(1), histo.GetXaxis().GetBinUpEdge(histo.GetNbinsX()))
            
            asymhisto.GetXaxis().SetTitleFont(42)
            asymhisto.GetXaxis().SetTitleSize(0.05)
            asymhisto.GetXaxis().SetTitleOffset(1.1)
            asymhisto.GetXaxis().SetLabelFont(42)
            asymhisto.GetXaxis().SetLabelSize(0.05)
            asymhisto.GetXaxis().SetLabelOffset(0.007)
            asymhisto.GetYaxis().SetTitleFont(42)
            asymhisto.GetYaxis().SetTitleSize(0.05)
            asymhisto.GetYaxis().SetTitleOffset(0.4 if self.doWide else 2.0)
            asymhisto.GetYaxis().SetLabelFont(42)
            asymhisto.GetYaxis().SetLabelSize(0.05)
            asymhisto.GetYaxis().SetLabelOffset(0.007)
            asymhisto.SetMinimum(0.)
            asymhisto.GetXaxis().SetNdivisions(510, True)
            asymhisto.GetYaxis().SetNdivisions(510, True)
            asymhisto.Draw('a2')
            self.objectsInLeg.append( (asymhisto, name, legOptions) )
        else:
            histo.GetXaxis().SetTitle( varList.varList[self.name.replace('_folded', '')]['xaxis'] )
            histo.GetYaxis().SetTitle( varList.varList[self.name.replace('_folded', '')]['yaxis'] )

            histo.GetXaxis().SetTitleFont(42)
            histo.GetXaxis().SetTitleSize(0.05)
            histo.GetXaxis().SetTitleOffset(1.1)
            histo.GetXaxis().SetLabelFont(42)
            histo.GetXaxis().SetLabelSize(0.05)
            histo.GetXaxis().SetLabelOffset(0.007)
            histo.GetYaxis().SetTitleFont(42)
            histo.GetYaxis().SetTitleSize(0.05)
            histo.GetYaxis().SetTitleOffset(0.4 if self.doWide else 2.0)
            histo.GetYaxis().SetLabelFont(42)
            histo.GetYaxis().SetLabelSize(0.05)
            histo.GetYaxis().SetLabelOffset(0.007)
            
            histo.SetMinimum(0.)
            print '> Drawing a sym.-unc. histogram with the following options:', options
            histo.Draw(options)
            self.objectsInLeg.append( (histo, name, legOptions) )
    
    
    def saveCanvas(self, corner, suffix='', leg=True):
        self.canvas.cd()
        
        textSize = 0.055 if self.doRatio else 0.022
        legWidth = 0.12
        height = (.19 + textSize*max(len(self.objectsInLeg)-3,0))
        if corner == "TR":
            (x1,y1,x2,y2) = (0.97-legWidth if self.doWide else .85-legWidth, .9 - height, .90, .93)
        elif corner == "TC":
            (x1,y1,x2,y2) = (.5, .9 - height, .55+legWidth, .93)
        elif corner == "TL":
            (x1,y1,x2,y2) = (.2, .9 - height, .25+legWidth, .91)
        elif corner == "BR":
            (x1,y1,x2,y2) = (.85-legWidth, .16 + height, .90, .15)
        elif corner == "BC":
            (x1,y1,x2,y2) = (.5, .16 + height, .5+legWidth, .15)
        elif corner == "BL":
            (x1,y1,x2,y2) = (.2, .16 + height, .2+legWidth, .15)
        
        if leg:
            leg = r.TLegend(x1,y1,x2,y2)
            leg.SetTextFont(42)
            leg.SetTextSize(textSize)
            leg.SetBorderSize(0)
            leg.SetFillColor(10)
            for obj,name,opt in self.objectsInLeg:
                if opt:
                    leg.AddEntry( obj, name, opt)
            leg.Draw('same')

        CMS_lumi.lumi_13TeV = "%.1f fb^{-1}" %(varList.Lumi)
        CMS_lumi.extraText  = 'Preliminary'
        CMS_lumi.lumi_sqrtS = '#sqrt{s} = 13 TeV'
        CMS_lumi.CMS_lumi(r.gPad, 4, 0, -0.005 if self.doWide and self.doRatio else 0.01 if self.doWide else 0.05)

        self.canvas.SaveAs(self.plotspath + self.name + suffix +'.pdf')
        self.canvas.SaveAs(self.plotspath + self.name + suffix +'.png')

        self.canvas.IsA().Destructor(self.canvas)
