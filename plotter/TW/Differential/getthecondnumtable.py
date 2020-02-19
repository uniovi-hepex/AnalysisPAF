import getLaTeXtable as tex
import varList       as vl

tex.getcondnumLaTeXtable(vl.varList["Names"]["Variables"][:-1], txttablespath = "/nfs/fanae/user/vrbouza/www/TFM/Unfolding/Results/CondN", path = "./results/tables", addasimov = False)
