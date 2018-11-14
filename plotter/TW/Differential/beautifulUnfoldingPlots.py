import ROOT  as r
import tdrstyle, CMS_lumi
import copy
import varList as vl
###############################################################################

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
        #r.gROOT.SetBatch(True)
        self.name           = name
        self.inited         = False
        self.objectsInLeg   = []
        self.plotspath      = ""
        self.doWide         = False
        self.doRatio        = False
        self.isLCurve       = False
        self.doFit          = True


    def initCanvasAndAll(self):
        if self.inited: return
        self.inited     = True
        tdrstyle.setTDRStyle()

        topSpamSize     = 1.2
        doOfficialCMS   = True

        plotformat  = (1200,600) if self.doWide else (600,600)
        height      = plotformat[1]+150 if self.doRatio else plotformat[1]

        self.canvas = r.TCanvas(self.name+"_canvas", self.name, plotformat[0], height)
        
        self.canvas.SetTopMargin(self.canvas.GetTopMargin()*topSpamSize)
        topsize = 0.12*600./height if self.doRatio else 0.06*600./height
        if doOfficialCMS: self.canvas.SetTopMargin(topsize*1.2 if self.doWide else topsize)
        self.canvas.SetWindowSize(plotformat[0] + (plotformat[0] - self.canvas.GetWw()), (plotformat[1]+150 + (plotformat[1]+150 - self.canvas.GetWh())))

        if self.doRatio:
            self.canvas.Divide(1, 2)
            self.canvas.GetPad(1).SetPad(*vl.plotlimits)
            self.canvas.GetPad(2).SetPad(*vl.ratiolimits)
            self.canvas.GetPad(2).SetBottomMargin(0.3)
        return
    
    
    def addHisto(self, histos, options, name, legOptions, idname = '', padnum = 1):
        if not self.inited: self.initCanvasAndAll()
        if self.doRatio: self.canvas.cd(1)
        else:            self.canvas.cd()
        
        if isinstance(histos, list):
            histo = histos[0]
            asymhisto   =   r.TGraphAsymmErrors(histo)
            for bin in range(asymhisto.GetN()):
                asymhisto.SetPointEYhigh(bin, histo.GetBinError(bin + 1))
                asymhisto.SetPointEYlow(bin, histos[1].GetBinError(bin + 1))
            
            if self.name.replace('_folded', '').replace('_asimov', '') in vl.varList:
                asymhisto.GetXaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['xaxis'] )
                if not vl.doxsec: asymhisto.GetYaxis().SetTitle( 'Events' )
                else:             asymhisto.GetYaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['yaxis'] )
            
            asymhisto.GetXaxis().SetRangeUser(histo.GetXaxis().GetBinLowEdge(1), histo.GetXaxis().GetBinUpEdge(histo.GetNbinsX()))
            
            asymhisto.GetXaxis().SetTitleFont(43)
            asymhisto.GetXaxis().SetTitleSize(22)
            asymhisto.GetXaxis().SetTitleOffset(1.4)
            asymhisto.GetXaxis().SetLabelFont(43)
            asymhisto.GetXaxis().SetLabelSize(22)
            asymhisto.GetXaxis().SetLabelOffset(0.007)
            asymhisto.GetXaxis().SetNdivisions(510, True)
            
            asymhisto.GetYaxis().SetTitleFont(43)
            asymhisto.GetYaxis().SetTitleSize(22)
            asymhisto.GetYaxis().SetTitleOffset(0.5 if self.doWide else 1.8 if not self.doRatio else 2.2)
            asymhisto.GetYaxis().SetLabelFont(43)
            asymhisto.GetYaxis().SetLabelSize(22)
            asymhisto.GetYaxis().SetLabelOffset(0.007)
            asymhisto.GetYaxis().SetNdivisions(510, True)
            
            if 'nomin' not in options:
                asymhisto.SetMinimum(0.)
            else:
                options = options.replace('nomin', '')
            
            if self.doRatio:
                asymhisto.GetXaxis().SetLabelOffset(999)
                asymhisto.GetXaxis().SetLabelSize(0)
                asymhisto.GetXaxis().SetTitle(' ')
                
            print '> Drawing an asym.-unc. histogram with the following options:', options
            asymhisto.Draw('a2')
            self.objectsInLeg.append( (asymhisto, name, legOptions, idname) )
        else:
            histo = histos
            if self.name.replace('_folded', '').replace('_asimov', '') in vl.varList:
                histo.GetXaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['xaxis'] )
                if not vl.doxsec: histo.GetYaxis().SetTitle( 'Events' )
                else:             histo.GetYaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['yaxis'] )

            histo.GetXaxis().SetTitleFont(43)
            histo.GetXaxis().SetTitleSize(22)
            histo.GetXaxis().SetTitleOffset(1.7 if self.isLCurve else 1.5)
            histo.GetXaxis().SetLabelFont(43)
            histo.GetXaxis().SetLabelSize(13 if self.isLCurve else 22)
            histo.GetXaxis().SetLabelOffset(0.033 if self.isLCurve else 0.007)
            histo.GetXaxis().SetNdivisions(010 if self.isLCurve else 510, True)
            
            histo.GetYaxis().SetTitleFont(43)
            histo.GetYaxis().SetTitleSize(22)
            histo.GetYaxis().SetTitleOffset(0.5 if self.doWide else 1.8 if not self.doRatio else 2.2)
            histo.GetYaxis().SetLabelFont(43)
            histo.GetYaxis().SetLabelSize(13 if self.isLCurve else 22)
            histo.GetYaxis().SetLabelOffset(0.007)
            histo.GetYaxis().SetNdivisions(310 if self.isLCurve else 510, True)
            
            if 'nomin' not in options:
                histo.SetMinimum(0.)
            else:
                options = options.replace('nomin', '')
            
            if 'comp' in name: 
                histo.GetYaxis().SetRangeUser(0.9, 1.1)
            
            if self.isLCurve:
                for i in range(1, 25):
                    histo.GetXaxis().ChangeLabel(i, 45)
            
            if self.doRatio:
                histo.GetXaxis().SetLabelOffset(999)
                histo.GetXaxis().SetLabelSize(0)
                histo.GetXaxis().SetTitle(' ')
            
            print '> Drawing a sym.-unc. histogram with the following options:', options
            
            histo.Draw(options)
            self.objectsInLeg.append( (histo, name, legOptions, idname) )
        return
    
    
    def addHistoInPad(self, padnum, histos, options, name, legOptions):
        if not self.inited:
            self.initCanvasAndAll() # first one should be total number of supbad

        self.canvas.cd(padnum)
        histo = histos

        if self.name.replace('_folded', '').replace('_asimov', '') in vl.varList:
            histo.GetXaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['xaxis'] )
            if not vl.doxsec: histo.GetYaxis().SetTitle( 'Events' )
            else:             histo.GetYaxis().SetTitle( vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['yaxis'] )

        histo.GetXaxis().SetTitleFont(43)
        histo.GetXaxis().SetTitleSize(22)
        histo.GetXaxis().SetTitleOffset(1.4)
        histo.GetXaxis().SetLabelFont(43)
        histo.GetXaxis().SetLabelSize(22)
        histo.GetXaxis().SetLabelOffset(0.007)
        
        histo.GetYaxis().SetTitleFont(43)
        histo.GetYaxis().SetTitleSize(22)
        histo.GetYaxis().SetTitleOffset(0.5 if self.doWide else 1.8)
        histo.GetYaxis().SetLabelFont(43)
        histo.GetYaxis().SetLabelSize(22)
        histo.GetYaxis().SetLabelOffset(0.007)
        histo.Draw(options)
        return
    
    
    def addTLatex(self, x1, y1, text, padnum = 1):
        if padnum:
            self.canvas.cd(padnum)
        else:
            self.canvas.cd()

        la = r.TLatex( x1, y1, text)
        if not hasattr(self, 'tlatex'):
            setattr(self, 'tlatex', [la])
        else:
            self.tlatex.append(la)
        
        la.SetNDC()
        la.SetTextAlign(22)
        la.SetTextFont(43)
        la.SetTextSizePixels(22)
        la.SetTextAngle(0)
        
        la.Draw()
        return


    def saveCanvas(self, corner, suffix='', leg=True):
        if self.doRatio: self.canvas.cd(1)
        else:            self.canvas.cd()
        
        # Draw legend
        textSize = 0.022
        legWidth = 0.13
        height = (.18 + textSize*max(len(self.objectsInLeg)-3,0))
        if corner == "TR":
            (x1,y1,x2,y2) = (0.93-legWidth if self.doWide else .85-legWidth, .93 - height, .865,         .93)
        elif corner == "TC":
            (x1,y1,x2,y2) = (.5,                                             .93 - height, .55+legWidth, .93)
        elif corner == "TL":
            (x1,y1,x2,y2) = (.18,                                            .835 - height, .18+legWidth, .835)
        elif corner == "BR":
            (x1,y1,x2,y2) = (.85 - legWidth,                                 .16 + height, .90,          .16)
        elif corner == "BC":
            (x1,y1,x2,y2) = (.5,                                             .16 + height, .5+legWidth,  .16)
        elif corner == "BL":
            (x1,y1,x2,y2) = (.2,                                             .16 + height, .2+legWidth,  .16)
        
        if leg:
            leg = r.TLegend(x1,y1,x2,y2)
            leg.SetTextFont(42)
            leg.SetTextSize(textSize)
            leg.SetBorderSize(0)
            leg.SetFillColor(10)
            for obj,name,opt,idnam in self.objectsInLeg:
                if opt:
                    leg.AddEntry(obj, name, opt)
            leg.Draw('same')

        if not hasattr(self, 'noCMS'):
            CMS_lumi.lumi_13TeV = "%.1f fb^{-1}" %(vl.Lumi)
            CMS_lumi.extraText  = 'Preliminary'
            CMS_lumi.lumi_sqrtS = '#sqrt{s} = 13 TeV'
            CMS_lumi.CMS_lumi(r.gPad, 4, 10, -0.005 if self.doWide and self.doRatio else 0.01 if self.doWide else 0.05)
        
        # Draw ratio if needed
        if self.doRatio:
            print '> Plotting ratio between prediction(s) and results'
            if 'data' not in [it[3] for it in self.objectsInLeg]: raise RuntimeError('There is not data to make a ratio plot as demanded.')
            if [it[3] for it in self.objectsInLeg].count('data') > 1: raise RuntimeError('There are more than one data histograms attached: multiple ratios are not implemented.')
            if 'mc' not in [it[3] for it in self.objectsInLeg]: raise RuntimeError('There is not (at least) one prediction to make a ratio plot as demanded.')
            if 'unc' not in [it[3] for it in self.objectsInLeg]: raise RuntimeError('There is not histogram with the total uncertainties to make a ratio plot as demanded.')
            if [it[3] for it in self.objectsInLeg].count('unc') > 1: raise RuntimeError('There are more than one total uncertainty histograms attached. Please add only one.')
            # Obtaining resources
            ratiohistos= []
            totalunc   = None
            
            datavalues = copy.deepcopy([it[0] for it in self.objectsInLeg if it[3] == 'data'][0])
            fitunc     = copy.deepcopy(datavalues.Clone('fitunc'))
            
            for bin in range(1, datavalues.GetNbinsX() + 1):
                datavalues.SetBinError(bin, 0.)
            for it in self.objectsInLeg:
                if it[3] == 'mc':
                    ratiohistos.append(copy.deepcopy(it[0].Clone(it[1])))
                    ratiohistos[-1].Divide(datavalues)
                if it[3] == 'unc':
                    totalunc = copy.deepcopy(it[0].Clone(it[1]))
            xtemp = r.Double(0.)
            ytemp = r.Double(0.)
            for bin in range(1, datavalues.GetNbinsX() + 1):
                fitunc.SetBinError(bin, fitunc.GetBinError(bin)/fitunc.GetBinContent(bin))
                fitunc.SetBinContent(bin, 1.)
                totalunc.GetPoint(bin - 1, xtemp, ytemp)
                totalunc.SetPointEYhigh(bin - 1, totalunc.GetErrorYhigh(bin - 1)/ytemp)
                totalunc.SetPointEYlow(bin - 1,  totalunc.GetErrorYlow(bin - 1)/ytemp)
                totalunc.SetPoint(bin - 1, xtemp, 1.)
            
            # Setting options
            fitunc.SetFillColorAlpha(r.kAzure, 0.35)
            fitunc.SetLineColor(r.kBlack)
            fitunc.SetFillStyle(1001)
            fitunc.SetLineWidth(1)
            fitunc.SetMarkerSize(0.)
            
            totalunc.SetLineColor(r.kBlack)
            totalunc.SetFillStyle(1001)
            totalunc.SetLineWidth(1)
            
            #if self.doFit: totalunc.GetXaxis().SetRangeUser(fitunc.GetXaxis().GetBinLowEdge(1), fitunc.GetXaxis().GetBinUpEdge(fitunc.GetNbinsX()))
            totalunc.GetXaxis().SetRangeUser(fitunc.GetXaxis().GetBinLowEdge(1), fitunc.GetXaxis().GetBinUpEdge(fitunc.GetNbinsX()))
            #else:          totalunc.GetXaxis().SetRangeUser(datavalues.GetXaxis().GetBinLowEdge(1), datavalues.GetXaxis().GetBinUpEdge(datavalues.GetNbinsX()))
            
            totalunc.GetXaxis().SetTitle(vl.varList[self.name.replace('_folded', '').replace('_asimov', '')]['xaxis'])
            totalunc.GetXaxis().SetTitleFont(43)
            totalunc.GetXaxis().SetTitleSize(22)
            totalunc.GetXaxis().SetTitleOffset(4)
            totalunc.GetXaxis().SetLabelFont(43)
            totalunc.GetXaxis().SetLabelSize(22)
            totalunc.GetXaxis().SetLabelOffset(0.007)
            totalunc.GetXaxis().SetNdivisions(510, True)
            
            totalunc.GetYaxis().SetRangeUser(0, 2.5)
            totalunc.GetYaxis().SetTitle('Pred./Data')
            totalunc.GetYaxis().SetTitleFont(43)
            totalunc.GetYaxis().SetTitleSize(22)
            totalunc.GetYaxis().SetTitleOffset(0.5 if self.doWide else 1.8)
            totalunc.GetYaxis().CenterTitle(True)
            totalunc.GetYaxis().SetLabelFont(43)
            totalunc.GetYaxis().SetLabelSize(22)
            totalunc.GetYaxis().SetLabelOffset(0.007)
            totalunc.GetYaxis().SetNdivisions(510, True)
            
            # Drawing
            self.canvas.cd(2)
            totalunc.Draw('a2')
            fitunc.Draw('E2,L,same')
            for el in ratiohistos:
                el.Draw('L,same')
        
        
        # Save results
        self.canvas.SaveAs(self.plotspath + self.name + suffix + '.pdf')
        self.canvas.SaveAs(self.plotspath + self.name + suffix + '.png')
        self.canvas.SaveAs(self.plotspath + self.name + suffix + '.eps')
        self.canvas.SaveAs(self.plotspath + self.name + suffix + '.root')

        self.canvas.IsA().Destructor(self.canvas)
