import getLaTeXtable as tex
import varList       as vl

tex.getgoftestsLaTeXtable(vl.varList["Names"]["Variables"][:-1], txttablespath = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/Results/goftests", path = "./results/tables", ty = "unfolding")
tex.getgoftestsLaTeXtable(vl.varList["Names"]["Variables"][:-1], txttablespath = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/Results/goftests", path = "./results/tables", ty = "fidunorm")
