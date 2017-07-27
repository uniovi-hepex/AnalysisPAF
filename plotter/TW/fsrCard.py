import os
import ROOT as r
from ROOT import TH1F, TFile


#r.gROOT.LoadMacro('Datacard.C')


# d1 = r.Datacard("tW","ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, Mistag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER,pdf,Scale,tWMe_s_cale", "ElMu")
# d1.SetRootFileName("Datacards/forCards_With1j1t.root")
# d1.GetParamsFormFile()
# d1.SetNormUnc("NonWZ", 1.50)
# d1.SetNormUnc("DY"   , 1.10)
# d1.SetNormUnc("VV"   , 1.10)
# d1.SetNormUnc("ttV"  , 1.10)
# d1.SetLumiUnc(1.026)
# d1.PrintDatacard("datacard_With1j1t_ElMu.txt")


# for ShapVarWithEverything
SRs = [range(1,9),range(9,15),range(15,25)] # here we work in bins, not in numbers

os.system('cp Datacards/forCards_WithEverything.root Datacards/forCards_WithEverything_withFSR.root')
tf = TFile.Open('Datacards/forCards_WithEverything_withFSR.root', 'update')


ttbar        = tf.Get('ttbar')
ttbarFSRUp   = tf.Get('ttbar_fsrUp')
ttbarFSRDown = tf.Get('ttbar_fsrDown')
tw        = tf.Get('tW')
twFSRUp   = tf.Get('tW_fsrUp')
twFSRDown = tf.Get('tW_fsrDown')

index = 0
for region in SRs:
    index = index + 1
    ttbarReg_fsrUp   = ttbar.Clone( 'ttbar_fsrReg%dUp'%index)
    ttbarReg_fsrDown = ttbar.Clone( 'ttbar_fsrReg%dDown'%index)
    twReg_fsrUp      = tw   .Clone( 'tW_fsrReg%dUp'%index)
    twReg_fsrDown    = tw   .Clone( 'tW_fsrReg%dDown'%index)


    for bin in region:
        ttbarReg_fsrUp  .SetBinContent(bin, ttbarFSRUp  .GetBinContent(bin) )
        ttbarReg_fsrDown.SetBinContent(bin, ttbarFSRDown.GetBinContent(bin) )
        twReg_fsrUp     .SetBinContent(bin, twFSRUp     .GetBinContent(bin) ) 
        twReg_fsrDown   .SetBinContent(bin, twFSRDown   .GetBinContent(bin) )

    ttbarReg_fsrUp  .Write()
    ttbarReg_fsrDown.Write()
    twReg_fsrUp     .Write()
    twReg_fsrDown   .Write()



tf.Close()


