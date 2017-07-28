import os
import math

fil = open('rawSysts.tex')

theLines = {}

statBin = 0
CI = ''
xsec = ''
statUnc = ''
totalUnc = ''

fsrUnc = 0
sclUnc = 0
muoUnc = 0

for lin in fil.readlines():
    if not '&' in lin: continue
    if 'C.I.' in lin   : CI = lin  ; continue
    if 'sigma' in lin: xsec = lin; continue
    name = lin.split('&')[0]
    val  = lin.split('&')[1]
    if 'nan' in val              : continue
    if 'statbin'     in name  : statBin  = statBin + float(val)*float(val); continue
    if 'Stat. unc.'  in name  : statUnc  = lin; continue
    if 'Total unc.' in name  : totalUnc = lin; continue
    if 'fsr'         in name  : fsrUnc   = fsrUnc  + float(val)*float(val); continue
    if 'cale'        in name  : sclUnc   = sclUnc  + float(val)*float(val); continue
    if 'MuonEff'     in name  : muoUnc   = muoUnc  + float(val)*float(val); print name; print muoUnc; continue
    name = name.replace(' ','')
    theLines[name] = val.replace(' ','')

theLines['MC Statistics'] = str( math.sqrt(statBin) )
theLines['FSR'          ] = str( math.sqrt(fsrUnc ) )
theLines['ME Scale'     ] = str( math.sqrt(sclUnc ) )
theLines['Muon Eff'     ] = str( math.sqrt(muoUnc ) )


print theLines

# make the table great again

#for key,val in sorted(theLines.items(), key = lambda x : float(x[1])):
#    print '{:>30} & {:>1.1f} \\\\'.format(key,float(val))

stat  = float(statUnc.split('&')[1])
tota  = float(totalUnc.split('&')[1])
lumi  = float(theLines['Lumi'])

sys   = math.sqrt( tota**2 - lumi**2 - stat**2 )


out ='''  \hline \hline Source & Uncertainty (\%) \\ \hline                                                       
         Trigger efficiencies  & {trig:>1.1f} \\                                                                   
            Muon efficiencies  & {muon:>1.1f} \\                                                                   
        Electron efficiencies  & {elec:>1.1f} \\                                                                   
             Jet energy scale  & {jes:>1.1f} \\                                                                   
        Jet energy resolution  & {jer:>1.1f} \\                                                                   
                    b tagging  & {btag:>1.1f} \\                                                                   
                   Mistagging  & {mtag:>1.1f} \\                                                                   
                       Pileup  & {pu:>1.1f} \\ \hline                                                            
  $\mu_\mathrm{{R}}$ and $\mu_\mathrm{{F}}$ scale & {me:>1.1f} \\                                                    
             Underlying event  & {ue:>1.1f} \\                                                                   
               ME/PS matching  & {hdmp:>1.1f} \\                                                                   
      Initial state radiation  & {isr:>1.1f} \\                                                                   
        Final state radiation  & {fsr:>1.1f} \\                                                                   
            Color reconnection & {color:>1.1f} \\                                                                      
                          PDF  & {pdf:>1.1f} \\                                                                   
                        DR-DS  & {dr:>1.1f} \\\hline                                                             
                           VV  & {vv:>1.1f} \\                                                                   
                   Drell--Yan  & {dy:>1.1f} \\                                                                   
            Non-\PW/Z leptons  & {fks:>1.1f} \\                                                                   
                    $\ttbar$V  & {ttv:>1.1f} \\\hline                                                             
                MC statistics  & {mcstat:>1.1f} \\\hline                                                 
             Total systematic  & \multirow{{2}}{{*}}{{{sys:>1.1f}}}  \\                                                 
   (no integrated luminosity)  &     \\                                                                   
        Integrated luminosity  & {lumi:>1.1f} \\ \hline                                                            
                  Statistical  & {stat:>1.1f} \\ \hline                                                            
                         Total & {tota:>1.1f} \\ \hline \hline'''.format(trig  = float(theLines['Trig'])               ,
                                                                         muon  = math.sqrt(muoUnc )             ,
                                                                         elec  = float(theLines['ElecEff'])            ,
                                                                         jes   = float(theLines['JES']),
                                                                         jer   = float(theLines['JER']),
                                                                         btag  = float(theLines['Btag']),
                                                                         mtag  = float(theLines['Mistag']),  
                                                                         pu    = float(theLines['PU']),
                                                                         color = float(theLines['ColorReconnection']),
                                                                         me    = math.sqrt(sclUnc ),
                                                                         ue    = float(theLines['ue']),
                                                                         hdmp  = float(theLines['hdamp']), 
                                                                         fsr   = math.sqrt(fsrUnc ),
                                                                         isr   = float(theLines['isr']), 
                                                                         pdf   = float(theLines['pdf']), 
                                                                         dr    = float(theLines['DiagramSubtraction']),
                                                                         vv    = float(theLines['VV']), 
                                                                         dy    = float(theLines['DY']), 
                                                                         fks   = float(theLines['NonWZ']), 
                                                                         ttv   = float(theLines['ttV']), 
                                                                         mcstat= math.sqrt(statBin),
                                                                         sys   = sys                            ,
                                                                         lumi  = float(theLines['Lumi'])               ,
                                                                         stat  = stat,                            
                                                                         tota  = tota)                           

print out
