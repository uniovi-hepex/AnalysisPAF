# Library of all the variables so everything is centralized and theres no confusions
# TODO: port the card producer to python so it can also use this

varList = {}

varList['LeadingJetPt'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'recobinning' : [0,50,70,90,110,130,150,170,500], # binning in reco space
    'var'         : 'TTJet1_pt',                      # name of the variable in the tree to make cards
    'var_response': 'LeadingJetPt',                   # name of the variable in the response matrix 
    'yaxis'       : 'Events',
}


varList['LeadingJetPtuncertainties'] = {
    'xaxis'       : 'Leading jet p_{T} [GeV]',
    'yaxis'       : 'Relative uncertainty'
}

varList['LCurve'] = {
    'xaxis'       : 'log L_{1}',
    'yaxis'       : 'log #frac{L_{2}}{#tau^{2}}',
}


Lumi = 35.9
