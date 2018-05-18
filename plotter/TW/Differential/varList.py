from math   import pi
import os
# Library of all the variables so everything is centralized and theres no confusions
# TODO: port the card producer to python so it can also use this

Lumi        = 35.9
nBinsInBDT  = 10

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
    'yaxis'       : 'log #frac{L_{2}}{#tau^{2}}',
}

varList['Names'] = {
    'Variables'   : ["E_LLB", "LeadingJetE", "MT_LLMETB", "M_LLB", "M_LeadingB", "M_SubLeadingB", 
                     "MET", "MET_Phi", "LeadingJetPt", "LeadingJetEta", "LeadingJetPhi", 
                     "LeadingLepE", "LeadingLepPt", "LeadingLepPhi", "LeadingLepEta", 
                     "SubLeadingLepE", "SubLeadingLepPt", "SubLeadingLepPhi", "SubLeadingLepEta",
                     "DilepPt", "DilepJetPt", "DilepMETJetPt", "HTtot"],
    'ExpSysts'    : ["JESUp", "JESDown", "JERUp", "ElecEffUp", "ElecEffDown", "MuonEffUp",
                     "MuonEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "BtagUp",
                     "BtagDown", "MistagUp", "MistagDown", "FragUp", "FragDown", "PetersonFrag",
                     "semilepbrUp", "semilepbrDown"],
    'ttbarSysts'  : ["ScaleUp", "ScaleDown", "pdfUp", "pdfDown", "hdampUp", "hdampDown",
                     "ueUp", "ueDown", "isrUp", "isrDown", "fsrUp", "fsrDown", "ColorReconnectionUp",
                     "ColorReconnectionDown"],
    'specialSysts': ["JERDown", "DSDown"],
    'colorSysts'  : ["GluonMoveCRTuneerdON", "PowhegerdON", "QCDbasedCRTuneerdON", "GluonMoveCRTune"],
}

varList['E_LLB'] = {
    'xaxis'       : '#ell(1), #ell(2), j(1) energy [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 190, 330, 550, 1000],
    'recobinning' : [0., 160., 220., 280., 340., 400., 450., 550., 1000.],
    'var'         : 'TE_LLB',
    'var_response': 'ELLB',
}
varList['E_LLBuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2), j(1) energy [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingJetE'] = {
    'xaxis'       : 'j(1) energy [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 75, 275, 1000],
    'recobinning' : [0., 40., 70., 120., 175., 275., 1000.],
    'var'         : 'TLeadingJetE',
    'var_response': 'LeadingJetE',
}
varList['LeadingJetEuncertainties'] = {
    'xaxis'       : 'j(1) energy [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['MT_LLMETB'] = {
    'xaxis'       : '#ell(1), #ell(2), #slash{E}_{T}, j(1) m_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0., 250., 450., 1000.],
    'recobinning' : [0., 200., 270., 320., 380., 450., 1000.],
    'var'         : 'TMT_LLMETB',
    'var_response': 'MTLLMETB',
}
varList['MT_LLMETBuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2), #slash{E}_{T}, j(1) m_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LLB'] = {
    'xaxis'       : '#ell(1), #ell(2) m [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0., 125., 250., 350., 1000.],
    'recobinning' : [0., 125., 170., 180., 200., 240., 290., 350., 1000.],
    'var'         : 'TM_LLB',
    'var_response': 'MLLB',
}
varList['M_LLBuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2) m [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LeadingB'] = {
    'xaxis'       : '#ell(1), j(1) m [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 80, 170, 300, 1000],
    'recobinning' : [0, 45, 75, 105, 140, 190, 230, 300, 1000],
    'var'         : 'TM_LeadingB',
    'var_response': 'MLeadingB',
}
varList['M_LeadingBuncertainties'] = {
    'xaxis'       : '#ell(1), j(1) m [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['M_SubLeadingB'] = {
    'xaxis'       : '#ell(2), j(1) m [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 70, 110, 200, 1000],
    'recobinning' : [0., 50., 60., 80., 100., 125., 150., 200., 1000.],
    'var'         : 'TM_SubLeadingB',
    'var_response': 'MSubLeadingB',
}
varList['M_SubLeadingBuncertainties'] = {
    'xaxis'       : '#ell(2), j(1) m [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['MET'] = {
    'xaxis'       : '#slash{E}_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 50, 140, 1000],
    'recobinning' : [0, 20, 35, 50, 70, 140, 1000],
    'var'         : 'TMET',
    'var_response': 'MET',
}
varList['METuncertainties'] = {
    'xaxis'       : '#slash{E}_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['MET_Phi'] = {
    'xaxis'       : '#varphi(#slash{E}_{T}) [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [-pi, -1.5, 0, 1.5, pi],
    'recobinning' : [-pi, -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, pi],
    'var'         : 'TMET_Phi',
    'var_response': 'METPhi',
}
varList['MET_Phiuncertainties'] = {
    'xaxis'       : '#varphi(#slash{E}_{T}) [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingJetPt'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 75, 200, 500],
    'recobinning' : [0, 50, 75, 110, 150, 200, 500],
    'var'         : 'TLeadingJetPt',
    'var_response': 'LeadingJetPt',
}
varList['LeadingJetPtuncertainties'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingJetPt'] = {
    'xaxis'       : 'Gen Leading jet p_{T} [GeV]',
    'yaxis'       : 'Leading jet p_{T} [GeV]'
}

varList['LeadingJetEta'] = {
    'xaxis'       : 'Leading jet #eta',
    'yaxis'       : 'Events',
    'genbinning'  : [-2.4, -1.2, 0, 1.2, 2.4],
    'recobinning' : [-2.4, -1.6, -1.25, -.5, 0, .5, 1.25, 1.6, 2.4],
    'var'         : 'TLeadingJetEta',
    'var_response': 'LeadingJetEta',
}
varList['LeadingJetEtauncertainties'] = {
    'xaxis'       : 'Leading jet #eta',
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingJetEta'] = {
    'xaxis'       : 'Gen Leading Jet #eta',
    'yaxis'       : 'Leading Jet #eta'
}

varList['LeadingJetPhi'] = {
    'xaxis'       : '#varphi(j(1)) [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [-pi, -1.5, 0, 1.5, pi],
    'recobinning' : [-pi, -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, pi],
    'var'         : 'TLeadingJetPhi',
    'var_response': 'LeadingJetPhi',
}
varList['LeadingJetPhiuncertainties'] = {
    'xaxis'       : '#varphi(j(1)) [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepE'] = {
    'xaxis'       : 'Leading lep energy [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 70, 120, 250, 1000],
    'recobinning' : [0, 40, 60, 80, 100, 120, 150, 250, 1000],
    'var'         : 'TLeadingLepE',
    'var_response': 'LeadingLepE',
}
varList['LeadingLepEuncertainties'] = {
    'xaxis'       : 'Leading lep energy [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepPt'] = {
    'xaxis'       : 'Leading lep p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 50, 90, 160, 1000],
    'recobinning' : [0, 50, 65, 75, 90, 110, 130, 160, 1000],
    'var'         : 'TLeadingLepPt',
    'var_response': 'LeadingLepPt',
}
varList['LeadingLepPtuncertainties'] = {
    'xaxis'       : 'Leading lep p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingLepPt'] = {
    'xaxis'       : 'Gen Leading lep p_{T} [GeV]',
    'yaxis'       : 'Leading lep p_{T} [GeV]'
}

varList['LeadingLepPhi'] = {
    'xaxis'       : 'Leading lep #varphi [rad]',
    'yaxis'       : 'Events',
    'genbinning'  : [-pi, -1.5, 0, 1.5, pi],
    'recobinning' : [-pi, -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, pi],
    'var'         : 'TLeadingLepPhi',
    'var_response': 'LeadingLepPhi',
}
varList['LeadingLepPhiuncertainties'] = {
    'xaxis'       : 'Leading lep #varphi [rad]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepEta'] = {
    'xaxis'       : 'Leading lep #eta',
    'yaxis'       : 'Events',
    'genbinning'  : [-2.4, -1.2, 0, 1.2, 2.4],
    #'recobinning' : [-2.4, -2.1, -1.2, -.5, 0, .4, 1.25, 1.75, 2.4],
    'recobinning' : [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4],
    'var'         : 'TLeadingLepEta',
    'var_response': 'LeadingLepEta',
}
varList['LeadingLepEtauncertainties'] = {
    'xaxis'       : 'Leading lep #eta',
    'yaxis'       : 'Relative uncertainty'
}
varList['ResponseLeadingLepEta'] = {
    'xaxis'       : 'Gen Leading lep #eta',
    'yaxis'       : 'Leading lep #eta'
}

varList['SubLeadingLepE'] = {
    'xaxis'       : 'Subleading lep energy [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 50, 100, 175, 1000],
    'recobinning' : [0, 30, 50, 70, 90, 115, 140, 175, 1000],
    'var'         : 'TSubLeadingLepE',
    'var_response': 'SubLeadingLepE',
}
varList['SubLeadingLepEuncertainties'] = {
    'xaxis'       : 'Subleading lep energy [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepPt'] = {
    'xaxis'       : 'Subleading lep p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 30, 60, 100, 1000],
    'recobinning' : [0, 30, 40, 50, 58, 68, 78, 100, 1000],
    'var'         : 'TSubLeadingLepPt',
    'var_response': 'SubLeadingLepPt',
}
varList['SubLeadingLepPtuncertainties'] = {
    'xaxis'       : 'Subleading lep p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepPhi'] = {
    'xaxis'       : 'Subleading lep #varphi [rad]',
    'yaxis'       : 'Events',
    'genbinning'  : [-pi, -1.5, 0, 1.5, pi],
    'recobinning' : [-pi, -2.25, -1.5, -.75, 0, .75, 1.5, 2.25, pi],
    'var'         : 'TSubLeadingLepPhi',
    'var_response': 'SubLeadingLepPhi',
}
varList['SubLeadingLepPhiuncertainties'] = {
    'xaxis'       : 'Subleading lep #varphi [rad]',
    'yaxis'       : 'Relative uncertainty'
}

varList['SubLeadingLepEta'] = {
    'xaxis'       : 'Subleading lep #eta',
    'yaxis'       : 'Events',
    'genbinning'  : [-2.4, -1.2, 0, 1.2, 2.4],
    'recobinning' : [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4],
    'var'         : 'TSubLeadingLepEta',
    'var_response': 'SubLeadingLepEta',
}
varList['SubLeadingLepEtauncertainties'] = {
    'xaxis'       : 'Subleading lep #eta',
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepPt'] = {
    'xaxis'       : '#ell(1), #ell(2) p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 50, 100, 150, 500],
    'recobinning' : [0, 40, 60, 80, 90, 110, 130, 150, 500],
    'var'         : 'TDilepPt',
    'var_response': 'DilepPt',
}
varList['DilepPtuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2) p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepJetPt'] = {
    'xaxis'       : '#ell(1) ,#ell(2), j(1) p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 50, 90, 140, 500],
    'recobinning' : [0, 20, 30, 50, 60, 90, 100, 140, 500],
    'var'         : 'TDilepJetPt',
    'var_response': 'DilepJetPt',
}
varList['DilepJetPtuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2), j(1) p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['DilepMETJetPt'] = {
    'xaxis'       : '#ell(1) ,#ell(2), j(1), #slash{E}_{T} p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 20, 40, 80, 500],
    'recobinning' : [0, 20, 30, 40, 50, 60, 70, 80, 500],
    'var'         : 'TDilepMETJetPt',
    'var_response': 'DilepMETJetPt',
}
varList['DilepMETJetPtuncertainties'] = {
    'xaxis'       : '#ell(1), #ell(2), j(1), #slash{E}_{T} p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['HTtot'] = {
    'xaxis'       : '#ell(1) ,#ell(2), j(1), #slash{E}_{T} p_{T} [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 200, 300, 450, 1000],
    'recobinning' : [0, 150, 200, 250, 300, 350, 400, 450, 1000],
    'var'         : 'THTtot',
    'var_response': 'HTtot',
}
varList['HTtotuncertainties'] = {
    'xaxis'       : 'H_{T}(tot.) [GeV]',
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
    'MEUp': { 'TW'           : 'TW_noFullyHadr_MEscaleUp',
                  'TbarW'        : 'TbarW_noFullyHadr_MEscaleUp',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'MEDown': { 'TW'         : 'TW_noFullyHadr_MEscaleDown',
                  'TbarW'        : 'TbarW_noFullyHadr_MEscaleDown',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'PSUp': { 'TW'           : 'TW_noFullyHadr_PSscaleUp',
                  'TbarW'        : 'TbarW_noFullyHadr_PSscaleUp',
                  'TTbar_Powheg' : 'TTbar_Powheg'
                  },
    'PSDown': { 'TW'           : 'TW_noFullyHadr_PSscaleDown',
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
    # 'pdfUp' : { 'TW'           : 'TW',
    #             'TbarW'        : 'TbarW',
    #             'TTbar_Powheg' : 'TTbar_Powheg'
    #             },
    # 'pdfDown' : { 'TW'           : 'TW',
    #               'TbarW'        : 'TbarW',
    #               'TTbar_Powheg' : 'TTbar_Powheg'
    #               },
    # 'MEUp' : { 'TW'           : 'TW',
    #               'TbarW'        : 'TbarW',
    #               'TTbar_Powheg' : 'TTbar_Powheg'
    #             },
    # 'MEDown' : { 'TW'           : 'TW',
    #                 'TbarW'        : 'TbarW',
    #                 'TTbar_Powheg' : 'TTbar_Powheg'
    #                 },
    }

# systWeight = {
#     'fsrUp'                  : '',  
#     'fsrDown'                : '',
#     'isrUp'                  : '',
#     'isrDown'                : '',
#     'MEUp'               : '',
#     'MEDown'             : '',
#     'PSUp'               : '',
#     'PSDown'             : '',
#     'DSUp'                     : '',
#     'hDampUp'                : '',
#     'hDampDown'              : '',
#     'UEUp'                   : '',
#     'UEDown'                 : '',
#     'GluonMoveCRTune'        : '',
#     'GluonMoveCRTune_erdOn'  : '', 
#     'Powheg_erdON'           : '',
#     'QCDbasedCRTune_erdON'   : '',
#     'pdfUp'                  : 'pdfUp',
#     'pdfDown'                : 'pdfDown',
#     'scaleUp'                : 'ScaleUp',
#     'scaleDown'              : 'ScaleDown',
# }
