'''
Library of all the variables so everything is centralised and there are not confusions
'''

import ROOT as r
import warnings as wr
import os

# === ESSENTIAL PARAMETERS OF THE ANALYSIS. CHANGING THIS APPLIES TO EVERYTHING. ===
nBinsInBDT  = 4         # Number of bins in the BDT disc. distribution
nuncs       = 6         # Number of uncs. shown in the relative uncertainty plots
asimov      = False     # Use of Asimov dataset or data
unifttbar   = True      # Equally distributed ttbar or tW in the BDT disc. distr.
doxsec      = True      # Show events or diff. cross section in final results
doReg       = False     # Do or not do regularisation
doArea      = False     # Do or not do area constraint

# === OTHER IMPORTANT DEFINITIONS ===
Lumi        = 35.864    # In femtobarns
uncLumi     = 0.025     # In %

sigma_ttbar   = 831.76
sigma_dilep   = 88.28769753
sigma_tw      = 35.85
sigma_twnohad = 19.4674104

n_ttbar                    = 77229341
n_dilep                    = 79092400

nUEUp_ttbar                = 58953660
nUEUp_dilep                = 9907537
nUEDown_ttbar              = 58338240
nUEDown_dilep              = 9691700
nhDampUp_ttbar             = 58858606
nhDampUp_dilep             = 9672473
nhDampDown_ttbar           = 58163976
nhDampDown_dilep           = 9634312

nGluonMoveCRTune_ttbar     = 59037234
nGluonMoveCRTune_dilep     = 9862990
nPowhegerdON_ttbar         = 59882210
nPowhegerdON_dilep         = 9537400
nQCDbasedCRTuneerdON_ttbar = 59620206
nQCDbasedCRTuneerdON_dilep = 9816448

n_tw                       = 6952830
n_twnohad                  = 11345619
n_tbarw                    = 6933094
n_tbarwnohad               = 11408144

plotlimits   = tuple([float(i) for i in "0.00, 0.23, 1.00, 1.00".split(',')]) # xlow, ylow, xup, yup
ratiolimits  = tuple([float(i) for i in "0.00, 0.05, 1.00, 0.29".split(',')]) # xlow, ylow, xup, yup
margins      = "0.06, 0.1, 0.06, 0.1" # top, bottom, right, left
marginsratio = "0.03, 0.4, 0.06, 0.1" # top, bottom, right, left
legpos       = (0.82, 0.65, 0.93, 0.93)

if asimov: labellegend = 'Pseudodata'
else:      labellegend = 'Data'

storagepath = "/nfs/fanae/user/vrbouza/Storage/TWTTbar/MiniTrees/"

def GetLastFolder(stpth):
    savefolders   = next(os.walk(stpth))[1]
    saveyears     = map(int, [i[6:]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[6:]) == max(saveyears)]
    savemonths    = map(int, [i[3:5] for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[3:5]) == max(savemonths)]
    savedays      = map(int, [i[:2]  for i in savefolders])
    savefolders   = [i for i in savefolders if int(i[:2]) == max(savedays)]
    return (stpth + savefolders[0] + "/")

def SetUpWarnings():
    wr.simplefilter("always", UserWarning)
    wr.filterwarnings(action = 'ignore', category = RuntimeWarning, message = 'TClass::Init:0: RuntimeWarning: no dictionary for class')
    return

def mean(numbers):
    return float(sum(numbers)) / max(len(numbers), 1)

# === DICTIONARIES ===
# var           := name of the variable in the tree to make cards
# var_response  := name of the variable in the response matrix without the M
varList = {}
varList['LCurve'] = {
    'xaxis'       : 'log L_{1}',
    'yaxis'       : 'log \\frac{L_{2}}{\\tau^{2}}',
}

varList['Names'] = {
    #'Variables'   : ["E_LLB", "LeadingJetE", "MT_LLMETB", "M_LLB", "M_LeadingB", "M_SubLeadingB", 
                     #"MET", "MET_Phi", "LeadingJetPt", "LeadingJetEta", "LeadingJetPhi", 
                     #"LeadingLepE", "LeadingLepPt", "LeadingLepPhi", "LeadingLepEta", 
                     #"SubLeadingLepE", "SubLeadingLepPt", "SubLeadingLepPhi", "SubLeadingLepEta", 
                     #"DilepPt", "DilepJetPt", "DilepMETJetPt", "HTtot", 
                     #"DilepMETJet1Pz", "LLMETBEta", "MSys", "Mll", "DPhiLL", "DPhiLeadJet", "DPhiSubLeadJet"], # Nuevinas
    #'Variables'   : ["M_LeadingB", "M_SubLeadingB", "LeadingLepPt", "LLMETBEta", "DilepMETJet1Pz", "DPhiLL", "DPhiLeadJet", "DPhiSubLeadJet"], # Nuevinas
    #'Variables'   : ["LeadingJetPt", "LeadingLepPt"],
    #'Variables'   : ["LeadingLepPt", "LeadingLepEta"],
    'Variables'   : ["LeadingLepPt", "LeadingJetPt", "DPhiLL", "DilepMETJet1Pz", "MT_LLMETB", "M_LLB"], # Variables single top 15-10-2018
    'ExpSysts'    : ["JESUp", "JESDown", "JERUp", "ElecEffUp", "ElecEffDown", "MuonEffUp", #   DO NOT MOVE THE FIRST THREE TO OTHER
                     "MuonEffDown", "TrigUp", "TrigDown", "PUUp", "PUDown", "BtagUp",      # POSITION: it will affect the calculus
                     "BtagDown", "MistagUp", "MistagDown"],                                # of the response matrices.
    'ttbarSysts'  : ["ttbarMEUp", "ttbarMEDown", "pdfUp", "pdfDown", "hDampUp", "hDampDown", "UEUp", "UEDown"],
    'specialSysts': ["JERDown", "DSDown"],
    'colorSysts'  : ["GluonMoveCRTuneerdON", "PowhegerdON", "QCDbasedCRTuneerdON", "GluonMoveCRTune"],
    'NormSysts'   : ["ttbarUp", "ttbarDown", "Non-WorZUp", "Non-WorZDown", "DYUp", "DYDown", "VVttbarVUp", "VVttbarVDown"],
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
    #'xaxis'       : 'm_{T}(\\ell_{1}, \\ell_{2},\\slash{E}_{T}, j) (GeV)',
    'xaxis'       : 'm_{T}(e^{\\pm}, \\mu^{\\mp}, \\slash{E}_{T}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 225., 325., 425., 500.],
    'recobinning' : [0., 225., 250., 275., 300., 325., 350., 425., 500.],
    'var'         : 'min(TMT_LLMETB, 499.)',
    'var_response': 'MTLLMETB',
    'var_gen'     : 'min(TGenMT_LLMETB, 499.)',
    'legpos'      : (0.6, 0.61, 0.71, 0.93),
    'legpos_foldas':"BR",
    'legpos_fold' : "BL",
    'uncleg_fold' : "TL",
    'uncleg_unf'  : "TC",
}
varList['MT_LLMETBuncertainties'] = {
    'xaxis'       : varList['MT_LLMETB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LLB'] = {
    #'xaxis'       : 'm(\\ell_{1}, \\ell_{2}, j) (GeV)',
    'xaxis'       : 'm(e^{#pm}, #mu^{#mp}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 125., 250., 350., 400.],
    'recobinning' : [0., 125., 170., 180., 200., 240., 290., 350., 400.],
    'var'         : 'min(TM_LLB, 399.)',
    'var_response': 'MLLB',
    'var_gen'     : 'min(TGenM_LLB, 399.)',
    'uncleg_fold' : "TC",
}
varList['M_LLBuncertainties'] = {
    'xaxis'       : varList['M_LLB']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['M_LeadingB'] = {
    'xaxis'       : 'm(\\ell_{1}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 75., 175., 275., 400.],
    'recobinning' : [0., 75., 95., 115., 135., 175., 225., 275., 400.],
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
    'genbinning'  : [0., 60., 100., 150., 300.],
    'recobinning' : [0., 60., 70., 80., 90., 100., 125., 150., 300.],
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
    #'genbinning'  : [0, 75, 200, 300],
    #'recobinning' : [0., 50., 75., 110., 150., 200., 300.],
   #'genbinning'  : [0., 75., 140., 200., 300.],                            # binning presentado en singletop
   #'recobinning' : [0., 60., 80., 105., 120., 140., 170., 210., 300.],
#    'genbinning'  : [0., 60., 110., 150., 300.],
#    'recobinning' : [0., 60., 75., 90., 110., 125., 150., 175., 300.],
    'genbinning'  : [0., 60., 90., 120., 150.],
    'recobinning' : [0., 60., 70., 80, 90., 100., 110., 120., 150.],
    'var'         : 'min(TLeadingJetPt, 149.)',
    'var_response': 'LeadingJetPt',
    'var_gen'     : 'min(TGenLeadingJetPt, 149.)',
    'uncleg_fold' : "TL",
    'uncleg_unf'  : "TC",
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
    #'xaxis'       : 'p_{T}(\\ell_{1}) (GeV)',
    'xaxis'       : 'Leading lepton p_{T} (GeV)',
    'yaxis'       : 'd#sigma [pb]',
   #'genbinning'  : [0, 50, 120, 160, 250],                           # binning presentado en singletop (junio/julio)
   #'recobinning' : [0, 50, 65, 85, 97, 110, 145, 180, 250],
    'genbinning'  : [0., 50., 90., 125., 150.],
    'recobinning' : [0., 50., 60., 70., 80., 90., 105., 125., 150.],  # antes de 15-10-2018
    'var'         : 'min(TLeadingLepPt, 149.)',
    'var_response': 'LeadingLepPt',
    'var_gen'     : 'min(TGenLeadingLepPt, 149.)',
    'uncleg_fold' : "TL",
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
    #'genbinning'  : [0., 0.5, 1., 1.6, 2.4],
    #'recobinning' : [0., 0.25, 0.5, 0.75, 1., 1.3, 1.6, 2., 2.4],
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
    'genbinning'  : [0., 40., 80., 120., 200.],
    'recobinning' : [0., 20., 30., 40., 50., 60., 80., 120., 200.],
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
    'genbinning'  : [0., 20., 40., 70., 150.],
    'recobinning' : [0., 10., 20., 30., 40., 50., 60., 70., 150.],
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


varList['DilepMETJet1Pz'] = {
    'xaxis'       : 'p_{Z} (e^{\\pm}, \\mu^{\\mp}, j) (GeV)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 100., 200., 350., 450.],
    'recobinning' : [0., 50., 100., 150., 200., 250., 300., 350., 450.],
    'var'         : 'min(abs(TDilepMETJet1Pz), 449.)',
    'var_response': 'DilepMETJet1Pz',
    'var_gen'     : 'min(abs(TGenDilepMETJet1Pz), 449.)',
    'legpos'      : (0.6, 0.61, 0.71, 0.93),
    'legpos_fold':  "TC",
    'legpos_foldas':"BL",
    'uncleg_fold' : "TC",
}
varList['DilepMETJet1Pzuncertainties'] = {
    'xaxis'       : varList['DilepMETJet1Pz']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['LLMETBEta'] = {
    'xaxis'       : '\\eta(\\ell_{1}, \\ell_{2}, j)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 1.25, 2.5, 3.75, 5.],
    'recobinning' : [0., 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 5.],
    'var'         : 'abs(TLLMETBEta)',
    'var_response': 'LLMETBEta',
    'var_gen'     : 'abs(TGenLLMETBEta)',
}
varList['LLMETBEtauncertainties'] = {
    'xaxis'       : varList['LLMETBEta']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['MSys'] = {
    'xaxis'       : 'm(\\ell_{1}, \\ell_{2}, j,\\slash{E}_{T})',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 225., 325., 425., 700.],
    'recobinning' : [0., 225., 250., 275., 300., 325., 350., 425., 700.],
    'var'         : 'TMSys',
    'var_response': 'MSys',
    'var_gen'     : 'TGenMSys',
}
varList['MSysuncertainties'] = {
    'xaxis'       : varList['MSys']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['Mll'] = {
    'xaxis'       : 'm(\\ell_{1}, \\ell_{2})',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., 50., 100., 150., 300.],
    'recobinning' : [0., 25., 45., 60., 75., 100., 125., 150., 300.],
    'var'         : 'TMll',
    'var_response': 'Mll',
    'var_gen'     : 'TGenMll',
}
varList['Mlluncertainties'] = {
    'xaxis'       : varList['Mll']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DPhiLL'] = {
    #'xaxis'       : '\\Delta \\varphi(\\ell_{1}, \\ell_{2}) (rad)',
    'xaxis'       : '\\Delta \\varphi(e^{\\pm}, \\mu^{\\mp}) (rad)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0., .75, 1.5, 2.25, r.TMath.Pi()],
    'recobinning' : [0., .35, .85, 1.25, 1.65, 2.05, 2.45, 2.85, r.TMath.Pi()],
    'var'         : 'abs(TDPhiLL)',
    'var_response': 'DPhiLL',
    'var_gen'     : 'abs(TGenDPhiLL)',
    #'legpos'      : (0.82, 0.14, 0.93, 0.47),
    'legpos'      : (0.15, 0.47, 0.26, 0.81),
    'legpos_fold' : "TL",
    'legpos_unf'  : "TL",
    'legpos_foldas':"TL",
    'legpos_unfas': "TL",
}
varList['DPhiLLuncertainties'] = {
    'xaxis'       : varList['DPhiLL']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DPhiLeadJet'] = {
    'xaxis'       : '\\Delta \\varphi(\\ell_{1}, j) (rad)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 1., 1.75, 2.5, r.TMath.Pi()],
    'recobinning' : [0, .5, 1., 1.5, 1.75, 2., 2.5, 2.85, r.TMath.Pi()],
    'var'         : 'abs(TDPhiLeadJet)',
    'var_response': 'DPhiLeadJet',
    'var_gen'     : 'abs(TGenDPhiLeadJet)',
}
varList['DPhiLeadJetuncertainties'] = {
    'xaxis'       : varList['DPhiLeadJet']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['DPhiSubLeadJet'] = {
    'xaxis'       : '\\Delta \\varphi(\\ell_{2}, j) (rad)',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 1., 1.75, 2.5, r.TMath.Pi()],
    'recobinning' : [0, .5, 1., 1.5, 1.75, 2., 2.5, 2.85, r.TMath.Pi()],
    'var'         : 'abs(TDPhiSubLeadJet)',
    'var_response': 'DPhiSubLeadJet',
    'var_gen'     : 'abs(TGenDPhiSubLeadJet)',
}
varList['DPhiSubLeadJetuncertainties'] = {
    'xaxis'       : varList['DPhiSubLeadJet']['xaxis'],
    'yaxis'       : 'Relative uncertainty'
}

varList['nLooseCentral'] = {
    'xaxis'       : '#(j_{loose})',
    'yaxis'       : 'd#sigma [pb]',
    'genbinning'  : [0, 2, 4, 6],
    'recobinning' : [0, 1, 2, 3, 4, 5, 6],
    'var'         : 'TnLooseCentral',
    'var_response': 'nLooseCentral',
    'var_gen'     : 'TnSergioLooseCentralJets',
}
varList['nLooseCentraluncertainties'] = {
    'xaxis'       : varList['nLooseCentral']['xaxis'],
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

NewColorMap = {
    'JES'                   : r.TColor.GetColor("#a6cee3"),
    'fsr'                   : r.TColor.GetColor("#1f77b4"),
    'isr'                   : r.TColor.GetColor("#b2df8a"),
    'tWME'                  : r.TColor.GetColor("#33a02c"),
    'tWPS'                  : r.TColor.GetColor("#fb9a99"),
    'DS'                    : r.TColor.GetColor("#e31a1c"),
    'hDamp'                 : r.TColor.GetColor("#fdbf6f"),
    'UE'                    : r.TColor.GetColor("#ff7f00"),
    'ttbarME'               : r.TColor.GetColor("#6a3d9a"),
    'pdf'                   : r.kOrange-6,
    'ColorR'                : r.TColor.GetColor("#cab2d6"),
    'JER'                   : r.kAzure-6,
    'ElecEff'               : r.kMagenta,
    'MuonEff'               : r.kGray+2,
    'Trig'                  : r.kBlue,
    'PU'                    : r.kRed,
    'Btag'                  : r.kGreen+4,
    'Mistag'                : r.kYellow-6,
    'ttbar'                 : r.kTeal-7,
    'Non-WorZ'              : r.kViolet-2,
    'DY'                    : r.kPink+1,
    'VVttbarV'              : r.kSpring-9,
    'Stat.'                 : r.kGreen+1,
    'Lumi'                  : r.kPink-5,
    'asimov'                : r.kTeal,
}
