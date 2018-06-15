import ROOT as r
import os
# Library of all the variables so everything is centralized and there are not confusions
# TODO: port the card producer to python so it can also use this

Lumi            = 35.9
nBinsInBDT      = 8

sigma_ttbar     = 831.76
sigma_dilep     = 88.28769753

nUEUp_ttbar     = 58953660
nUEUp_dilep     = 9907537
nUEDown_ttbar   = 58338240
nUEDown_dilep   = 9691700
nhDampUp_ttbar  = 58858606
nhDampUp_dilep  = 9672473
nhDampDown_ttbar= 58163976
nhDampDown_dilep= 9634312

margins         = "0.06, 0.1, 0.06, 0.1" # top, bottom, right, left
marginsratio    = "0.03, 0.4, 0.06, 0.1" # top, bottom, right, left

def GetLastFolder(stpth):
    savefolders   = next(os.walk(stpth))[1]
    saveyears     = map(int, [i[6:]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[6:]) == max(saveyears)]
    savemonths    = map(int, [i[3:5] for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[3:5]) == max(savemonths)]
    savedays      = map(int, [i[:2]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[:2]) == max(savedays)]
    return (stpth + savefolders[0] + "/")


# var           := name of the variable in the tree to make cards
# var_response  := name of the variable in the response matrix without the M
varList = {}
varList['LCurve'] = {
    'xaxis'       : 'log L_{1}',
    'yaxis'       : 'log \\frac{L_{2}}{\\tau^{2}}',
}

varList['Names'] = {
    'Variables'   : ["E_LLB", "LeadingJetE", "MT_LLMETB", "M_LLB", "M_LeadingB", "M_SubLeadingB", 
                     "MET", "MET_Phi", "LeadingJetPt", "LeadingJetEta", "LeadingJetPhi", 
                     "LeadingLepE", "LeadingLepPt", "LeadingLepPhi", "LeadingLepEta", 
                     "SubLeadingLepE", "SubLeadingLepPt", "SubLeadingLepPhi", "SubLeadingLepEta",
                     "DilepPt", "DilepJetPt", "DilepMETJetPt", "HTtot"],
    'ExpSysts'    : ["JESUp", "JESDown", "JERUp", "ElecEffUp", "ElecEffDown", "MuonEffUp",
                     "MuonEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "BtagUp",
                     "BtagDown", "MistagUp", "MistagDown"],
    'ttbarSysts'  : ["ttbarMEUp", "ttbarMEDown", "pdfUp", "pdfDown", "hDampUp", "hDampDown",
                     "UEUp", "UEDown"],
    'specialSysts': ["JERDown", "DSDown"],
    'colorSysts'  : ["GluonMoveCRTuneerdON", "PowhegerdON", "QCDbasedCRTuneerdON", "GluonMoveCRTune"],
}

varList['E_LLB'] = {
    'xaxis'       : 'E(\\ell_{1}, \\ell_{2}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 190, 330, 550, 700],
    'recobinning' : [0, 160, 220, 280, 340, 400, 450, 550, 700],
    'var'         : 'TE_LLB',
    'var_response': 'ELLB',
    'var_gen'     : 'TGenE_LLB',
}
varList['E_LLBuncertainties'] = {
    'xaxis'       : varList['E_LLB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingJetE'] = {
    'xaxis'       : 'E(j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 75, 275, 400],
    'recobinning' : [0., 40., 70., 120., 175., 275., 400.],
    'var'         : 'TLeadingJetE',
    'var_response': 'LeadingJetE',
    'var_gen'     : 'TGenLeadingJetE',
}
varList['LeadingJetEuncertainties'] = {
    'xaxis'       : varList['LeadingJetE']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['MT_LLMETB'] = {
    'xaxis'       : 'm_{T}(\\ell_{1}, \\ell_{2},\\slash{E}_{T}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 250., 450., 600.],
    'recobinning' : [0., 200., 270., 320., 380., 450., 600.],
    'var'         : 'TMT_LLMETB',
    'var_response': 'MTLLMETB',
    'var_gen'     : 'TGenMT_LLMETB',
}
varList['MT_LLMETBuncertainties'] = {
    'xaxis'       : varList['MT_LLMETB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LLB'] = {
    'xaxis'       : 'm(\\ell_{1}, \\ell_{2}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 125., 250., 350., 500.],
    'recobinning' : [0., 125., 170., 180., 200., 240., 290., 350., 500.],
    'var'         : 'TM_LLB',
    'var_response': 'MLLB',
    'var_gen'     : 'TGenM_LLB',
}
varList['M_LLBuncertainties'] = {
    'xaxis'       : varList['M_LLB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LeadingB'] = {
    'xaxis'       : 'm(\\ell_{1}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 80, 170, 300, 400],
    'recobinning' : [0., 45., 75., 105., 140., 190., 230., 300., 400.],
    'var'         : 'TM_LeadingB',
    'var_response': 'MLeadingB',
    'var_gen'     : 'TGenM_LeadingB',
}
varList['M_LeadingBuncertainties'] = {
    'xaxis'       : varList['M_LeadingB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['M_SubLeadingB'] = {
    'xaxis'       : 'm(\\ell_{2}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 70, 110, 200, 300],
    'recobinning' : [0., 50., 60., 80., 100., 125., 150., 200., 300.],
    'var'         : 'TM_SubLeadingB',
    'var_response': 'MSubLeadingB',
    'var_gen'     : 'TGenM_SubLeadingB',
}
varList['M_SubLeadingBuncertainties'] = {
    'xaxis'       : varList['M_SubLeadingB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['MET'] = {
    'xaxis'       : '\\slash{E}_{T} (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 50, 140, 200],
    'recobinning' : [0., 20., 35., 50., 70., 140., 200.],
    'var'         : 'TMET',
    'var_response': 'MET',
    'var_gen'     : 'TGenMET'
}
varList['METuncertainties'] = {
    'xaxis'       : varList['MET']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['MET_Phi'] = {
    'xaxis'       : '\\varphi(\\slash{E}_{T}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [-r.TMath.Pi(), -1.5, 0, 1.5, r.TMath.Pi()],
    'recobinning' : [-r.TMath.Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, r.TMath.Pi()],
    'var'         : 'TMET_Phi',
    'var_response': 'METPhi',
    'var_gen'     : 'TGenMET_Phi',
}
varList['MET_Phiuncertainties'] = {
    'xaxis'       : varList['MET_Phi']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingJetPt'] = {
    'xaxis'       : 'p_{T}(j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 75, 200, 300],
    'recobinning' : [0., 50., 75., 110., 150., 200., 300.],
    'var'         : 'TLeadingJetPt',
    'var_response': 'LeadingJetPt',
    'var_gen'     : 'TGenLeadingJetPt',
}
varList['LeadingJetPtuncertainties'] = {
    'xaxis'       : varList['LeadingJetPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingJetPt'] = {
    'xaxis'       : 'Gen Leading jet p_{T} (GeV)',
    'yaxis'       : 'Leading jet p_{T} (GeV)'
}

varList['LeadingJetEta'] = {
    'xaxis'       : '|\\eta|(j)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 0.6, 1.2, 1.8, 2.4],
    'recobinning' : [0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4],
    'var'         : 'abs(TLeadingJetEta)',
    'var_response': 'LeadingJetEta',
    'var_gen'     : 'abs(TGenLeadingJetEta)',
}
varList['LeadingJetEtauncertainties'] = {
    'xaxis'       : varList['LeadingJetEta']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingJetEta'] = {
    'xaxis'       : 'Gen Leading Jet \\eta',
    'yaxis'       : 'Leading Jet \\eta'
}

varList['LeadingJetPhi'] = {
    'xaxis'       : '\\varphi(j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [-r.TMath.Pi(), -1.5, 0, 1.5, r.TMath.Pi()],
    'recobinning' : [-r.TMath.Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, r.TMath.Pi()],
    'var'         : 'TLeadingJetPhi',
    'var_response': 'LeadingJetPhi',
    'var_gen'     : 'TGenLeadingJetPhi',
}
varList['LeadingJetPhiuncertainties'] = {
    'xaxis'       : varList['LeadingJetPhi']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepE'] = {
    'xaxis'       : 'E(\\ell_{1}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 70, 120, 250, 350],
    'recobinning' : [0., 40., 60., 80., 100., 120., 150., 250., 350.],
    'var'         : 'TLeadingLepE',
    'var_response': 'LeadingLepE',
    'var_gen'     : 'TGenLeadingLepE',
}
varList['LeadingLepEuncertainties'] = {
    'xaxis'       : varList['LeadingLepE']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepPt'] = {
    'xaxis'       : 'p_{T}(\\ell_{1}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 50, 90, 160, 250],
    'recobinning' : [0., 50., 65., 75., 90., 110., 130., 160., 250.],
    'var'         : 'TLeadingLepPt',
    'var_response': 'LeadingLepPt',
    'var_gen'     : 'TGenLeadingLepPt',
}
varList['LeadingLepPtuncertainties'] = {
    'xaxis'       : varList['LeadingLepPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingLepPt'] = {
    'xaxis'       : 'Gen Leading lep p_{T} (GeV)',
    'yaxis'       : 'Leading lep p_{T} (GeV)'
}

varList['LeadingLepPhi'] = {
    'xaxis'       : '\\varphi(\\ell_{1}) (rad)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [-r.TMath.Pi(), -1.5, 0, 1.5, r.TMath.Pi()],
    'recobinning' : [-r.TMath.Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, r.TMath.Pi()],
    'var'         : 'TLeadingLepPhi',
    'var_response': 'LeadingLepPhi',
    'var_gen'     : 'TGenLeadingLepPhi',
}
varList['LeadingLepPhiuncertainties'] = {
    'xaxis'       : varList['LeadingLepPhi']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepEta'] = {
    'xaxis'       : '|\\eta|(\\ell_{1})',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 0.6, 1.2, 1.8, 2.4],
    'recobinning' : [0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4],
    'var'         : 'abs(TLeadingLepEta)',
    'var_response': 'LeadingLepEta',
    'var_gen'     : 'abs(TGenLeadingLepEta)',
}
varList['LeadingLepEtauncertainties'] = {
    'xaxis'       : varList['LeadingLepEta']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingLepEta'] = {
    'xaxis'       : 'Gen Leading lep \\eta',
    'yaxis'       : 'Leading lep \\eta'
}

varList['SubLeadingLepE'] = {
    'xaxis'       : 'E(\\ell_{2}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 50, 100, 175, 250],
    'recobinning' : [0., 30., 50., 70., 90., 115., 140., 175., 250.],
    'var'         : 'TSubLeadingLepE',
    'var_response': 'SubLeadingLepE',
    'var_gen'     : 'TGenSubLeadingLepE',
}
varList['SubLeadingLepEuncertainties'] = {
    'xaxis'       : varList['SubLeadingLepE']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepPt'] = {
    'xaxis'       : 'p_{T}(\\ell_{2}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 30, 60, 100, 150],
    'recobinning' : [0., 30., 40., 50., 58., 68., 78., 100., 150.],
    'var'         : 'TSubLeadingLepPt',
    'var_response': 'SubLeadingLepPt',
    'var_gen'     : 'TGenSubLeadingLepPt',
}
varList['SubLeadingLepPtuncertainties'] = {
    'xaxis'       : varList['SubLeadingLepPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepPhi'] = {
    'xaxis'       : '\\varphi(\\ell_{2}) (rad)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [-r.TMath.Pi(), -1.5, 0, 1.5, r.TMath.Pi()],
    'recobinning' : [-r.TMath.Pi(), -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, r.TMath.Pi()],
    'var'         : 'TSubLeadingLepPhi',
    'var_response': 'SubLeadingLepPhi',
    'var_gen'     : 'TGenSubLeadingLepPhi',
}
varList['SubLeadingLepPhiuncertainties'] = {
    'xaxis'       : varList['SubLeadingLepPhi']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepEta'] = {
    'xaxis'       : '\\eta(\\ell_2)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 0.6, 1.2, 1.8, 2.4],
    'recobinning' : [0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4],
    'var'         : 'abs(TSubLeadingLepEta)',
    'var_response': 'SubLeadingLepEta',
    'var_gen'     : 'abs(TGenSubLeadingLepEta)',
}
varList['SubLeadingLepEtauncertainties'] = {
    'xaxis'       : varList['SubLeadingLepEta']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepPt'] = {
    'xaxis'       : 'p_{T}(\\ell_{1}, \\ell_{2}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 50, 100, 150, 200],
    'recobinning' : [0., 40., 60., 80., 90., 110., 130., 150., 200.],
    'var'         : 'TDilepPt',
    'var_response': 'DilepPt',
    'var_gen'     : 'TGenDilepPt',
}
varList['DilepPtuncertainties'] = {
    'xaxis'       : varList['DilepPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepJetPt'] = {
    'xaxis'       : 'p_{T}(\\ell_{1}, \\ell_{2}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 50, 90, 140, 500],
    'recobinning' : [0., 20., 30., 50., 60., 90., 100., 140., 200.],
    'var'         : 'TDilepJetPt',
    'var_response': 'DilepJetPt',
    'var_gen'     : 'TGenDilepJetPt',
}
varList['DilepJetPtuncertainties'] = {
    'xaxis'       : varList['DilepJetPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepMETJetPt'] = {
    'xaxis'       : 'p_{T}(\\ell_{1} ,\\ell_{2}, j,\\slash{E}_{T}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 20, 40, 80, 150],
    'recobinning' : [0., 20., 30., 40., 50., 60., 70., 80., 150.],
    'var'         : 'TDilepMETJetPt',
    'var_response': 'DilepMETJetPt',
    'var_gen'     : 'TGenDilepMETJetPt',
}
varList['DilepMETJetPtuncertainties'] = {
    'xaxis'       : varList['DilepMETJetPt']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['HTtot'] = {
    'xaxis'       : 'p_{T}(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T}) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 200, 300, 450, 600],
    'recobinning' : [0., 150., 200., 250., 300., 350., 400., 450., 600.],
    'var'         : 'THTtot',
    'var_response': 'HTtot',
    'var_gen'     : 'TGenHTtot',
}
varList['HTtotuncertainties'] = {
    'xaxis'       : varList['HTtot']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}


# Profiling things
systMap = {
    'fsrUp' : { 'TW'             : 'TW_noFullyHadr_fsrUp',
                'TbarW'          : 'TbarW_noFullyHadr_fsrUp', 
                'TTbar_Powheg'   : 'TTbar_Powheg_fsrUp'
                },
    'fsrDown' : { 'TW'           : 'TW_noFullyHadr_fsrDown',
                  'TbarW'        : 'TbarW_noFullyHadr_fsrDown', 
                  'TTbar_Powheg' : 'TTbar_Powheg_fsrDown'
                  },
    'isrUp' : { 'TW'             : 'TW_noFullyHadr_isrUp',
                'TbarW'          : 'TbarW_noFullyHadr_isrUp', 
                'TTbar_Powheg'   : 'TTbar_Powheg_isrUp'
                },
    'isrDown' : { 'TW'           : 'TW_noFullyHadr_isrDown',
                  'TbarW'        : 'TbarW_noFullyHadr_isrDown', 
                  'TTbar_Powheg' : 'TTbar_Powheg_isrDown'
                  },
    'tWMEUp': { 'TW'           : 'TW_noFullyHadr_MEscaleUp',
                  'TbarW'        : 'TbarW_noFullyHadr_MEscaleUp',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'tWMEDown': { 'TW'         : 'TW_noFullyHadr_MEscaleDown',
                  'TbarW'        : 'TbarW_noFullyHadr_MEscaleDown',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'tWPSUp': { 'TW'           : 'TW_noFullyHadr_PSscaleUp',
                  'TbarW'        : 'TbarW_noFullyHadr_PSscaleUp',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'tWPSDown': { 'TW'           : 'TW_noFullyHadr_PSscaleDown',
                    'TbarW'        : 'TbarW_noFullyHadr_PSscaleDown',
                    'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'DSUp'        : { 'TW'           : 'TW_noFullyHadr_DS',
                    'TbarW'        : 'TbarW_noFullyHadr_DS',
                    'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'hDampUp'   : { 'TW'           : 'TW',
                    'TbarW'        : 'TbarW',
                    'TTbar_Powheg' : 'TTbar_Powheg_hdampUp'
                  },
    'hDampDown'   : { 'TW'           : 'TW',
                      'TbarW'        : 'TbarW',
                      'TTbar_Powheg' : 'TTbar_Powheg_hdampDown'
                      },
    'UEUp'        : { 'TW'           : 'TW',
                      'TbarW'        : 'TbarW',
                      'TTbar_Powheg' : 'TTbar_Powheg_ueUp'
                      },
    'UEDown'      : { 'TW'           : 'TW',
                      'TbarW'        : 'TbarW',
                      'TTbar_Powheg' : 'TTbar_Powheg_ueDown'
                      },
    'GluonMoveCRTune' : { 'TW'           : 'TW',
                          'TbarW'        : 'TbarW',
                          'TTbar_Powheg' : 'TTbar_GluonMoveCRTune'
                          },
    'GluonMoveCRTuneerdON' : { 'TW'           : 'TW',
                                'TbarW'        : 'TbarW',
                                'TTbar_Powheg' : 'TTbar_GluonMoveCRTune_erdON'
                                },
    'PowhegerdON' : { 'TW'           : 'TW',
                       'TbarW'        : 'TbarW',
                       'TTbar_Powheg' : 'TTbar_Powheg_erdON'
                       },
    'QCDbasedCRTuneerdON' : { 'TW'           : 'TW',
                               'TbarW'        : 'TbarW',
                               'TTbar_Powheg' : 'TTbar_QCDbasedCRTune_erdON'
                               },
}

colorMap = {
    'Fit'                   : r.TColor.GetColor("#a6cee3"),
    'fsr'                   : r.TColor.GetColor("#1f77b4"),
    'isr'                   : r.TColor.GetColor("#b2df8a"),
    'tWME'                  : r.TColor.GetColor("#33a02c"),
    'tWPS'                  : r.TColor.GetColor("#fb9a99"),
    'DS'                    : r.TColor.GetColor("#e31a1c"),
    'hDamp'                 : r.TColor.GetColor("#fdbf6f"),
    'UE'                    : r.TColor.GetColor("#ff7f00"),
    'ttbarME'               : r.TColor.GetColor("#6a3d9a"),
    'pdf'                   : r.TColor.GetColor("#ffff99"),
    'ColorR'                : r.TColor.GetColor("#cab2d6"),
    'GluonMoveCRTune'       : r.TColor.GetColor("#cab2d6"),
    'GluonMoveCRTuneerdON'  : r.kMagenta,
    'PowhegerdON'           : r.kGray+2,
    'QCDbasedCRTuneerdON'   : r.kBlue,
}
