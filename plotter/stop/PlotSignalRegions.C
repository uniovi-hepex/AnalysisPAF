
int nBins = 14;
float bin0 = 1; float binN = 15;

TString var = "GetMT2orDeltaEtaBins(TMT2, TMET, TDeltaEta)";
TString level = BaselineCut;

// Bines 1 y  2: MET [150, 200, inf]
// Bines 3 a  8: DeltaEta [0, 0.4, 0.8, 1.2, 1.6, 2.0, inf (2.4)]
// Bines 9 a 14: MT2 [0, 20, 40, 60, 80, 100, inf]
TString binLabelsSR = "0, 150, > 200 | 0.0,  0.4, 0.8, 1.2, 1.6, 2.0, > 2.0 | 0.0, 20, 40, 60, 80, 100, > 100";

