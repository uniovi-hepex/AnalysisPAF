from math   import pi
# Library of all the variables so everything is centralized and theres no confusions
# TODO: port the card producer to python so it can also use this

Lumi = 35.9

# var           := name of the variable in the tree to make cards
# var_response  := name of the variable in the response matrix without the M

varList = {}

varList['LCurve'] = {
    'xaxis'       : 'log L_{1}',
    'yaxis'       : 'log #frac{L_{2}}{#tau^{2}}',
}

varList['E_LLB'] = {
    'xaxis'       : '#ell(1), #ell(2), j(1) energy [GeV]',
    'yaxis'       : 'Events',
    'genbinning'  : [0, 190, 330, 500, 1000],
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
    'recobinning' : [0., 40., 75., 125., 180., 275., 1000.],
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
    'genbinning'  : [0, 150, 250, 350, 500, 1000],
    'recobinning' : [0, 100, 150, 200, 250, 300, 350, 400, 450, 500, 1000],
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
    'genbinning'  : [0, 50, 150, 250, 400, 1000],
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000],
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
    'genbinning'  : [0, 75, 175, 300, 1000],
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 1000],
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
    'genbinning'  : [0, 25, 75, 125, 175, 1000],
    'recobinning' : [0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 1000],
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
    'genbinning'  : [0, 25, 75, 125, 1000],
    'recobinning' : [0, 25, 50, 75, 100, 125, 150, 175, 1000],
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
    'recobinning' : [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi],
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
    'genbinning'  : [0, 75, 150, 500],
    'recobinning' : [0, 50, 90, 130, 175, 250, 500],
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
    'recobinning' : [-2.4, -1.75, -1.25, -.5, 0, .5, 1.6, 1.9, 2.4],
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
    'recobinning' : [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi],
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
    'genbinning'  : [0, 70, 190, 360, 1000],
    'recobinning' : [0, 30, 90, 140, 190, 250, 310, 400, 1000],
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
    'genbinning'  : [0, 30, 50, 110, 250, 1000],
    'recobinning' : [0, 50, 60, 70, 80, 90, 100, 120, 150, 250, 1000],
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
    'recobinning' : [-pi, -2.2, -1.5, -.75, 0, .75, 1.5, 2.2, pi],
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
    'genbinning'  : [0, 100, 200, 300, 400, 1000],
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000],
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
    'genbinning'  : [0, 100, 200, 300, 400, 1000],
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000],
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
    'recobinning' : [-pi, -2, -1.5, -.75, 0, .75, 1.5, 2, pi],
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
    'recobinning' : [0, 25, 50, 75, 100, 125, 150, 175, 500],
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
    'genbinning'  : [0, 50, 100, 150, 500],
    'recobinning' : [0, 20, 40, 60, 80, 100, 120, 140, 500],
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
    'genbinning'  : [0, 20, 40, 60, 80, 500],
    'recobinning' : [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 500],
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
    'genbinning'  : [0, 100, 200, 300, 400, 1000],
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000],
    'var'         : 'THTtot',
    'var_response': 'HTtot',
}
varList['HTtotuncertainties'] = {
    'xaxis'       : 'H_{T}(tot.) [GeV]',
    'yaxis'       : 'Relative uncertainty'
}
