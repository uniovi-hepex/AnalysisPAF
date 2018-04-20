# Library of all the variables so everything is centralized and theres no confusions
# TODO: port the card producer to python so it can also use this

Lumi = 35.9

varList = {}

varList['LCurve'] = {
    'xaxis'       : 'log L_{1}',
    'yaxis'       : 'log #frac{L_{2}}{#tau^{2}}',
}

varList['LeadingJetPt'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'yaxis'       : 'Events',
    'recobinning' : [0, 50, 70, 90, 110, 130, 150, 170, 500], # binning in reco space
    'var'         : 'TLeadingJetPt',                          # name of the variable in the tree to make cards
    'var_response': 'LeadingJetPt',                           # name of the variable in the response matrix without the M
}

varList['LeadingJetPtuncertainties'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingJetEta'] = {
    'xaxis'       : 'Leading jet #eta',
    'yaxis'       : 'Events',
    'recobinning' : [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4], # binning in reco space
    'var'         : 'TLeadingJetEta',                          # name of the variable in the tree to make cards
    'var_response': 'LeadingJetEta',                           # name of the variable in the response matrix without the M
}

varList['LeadingJetEtauncertainties'] = {
    'xaxis'       : 'Leading jet #eta',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepPt'] = {
    'xaxis'       : 'Leading lep p_{T} [GeV]',
    'yaxis'       : 'Events',
    'recobinning' : [0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 1000], # binning in reco space
    'var'         : 'TLeadingLepPt',                          # name of the variable in the tree to make cards
    'var_response': 'LeadingLepPt',                           # name of the variable in the response matrix without the M
}

varList['LeadingLepPtLepuncertainties'] = {
    'xaxis'       : 'Leading lep p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LeadingLepEta'] = {
    'xaxis'       : 'Leading lep #eta',
    'yaxis'       : 'Events',
    'recobinning' : [-2.4, -1.75, -1.25, -.5, 0, .5, 1.25, 1.75, 2.4], # binning in reco space
    'var'         : 'TLeadingLepEta',                         # name of the variable in the tree to make cards
    'var_response': 'LeadingLepEta',                          # name of the variable in the response matrix without the M
}

varList['LeadingLepEtauncertainties'] = {
    'xaxis'       : 'Leading lep #eta',
    'yaxis'       : 'Relative uncertainty'
}
