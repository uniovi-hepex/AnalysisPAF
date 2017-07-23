#ifndef TopHistoReader_H 
#define TopHistoReader_H 11

const TString DefaultPathY    = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/jul7/";
const TString DefaultPathFidu = "/nfs/fanae/user/juanr/AnalysisPAF/TopTrees/jul7/Fiducial/";
const TString DefaultOutPath  = "./top/xsec/";
const Float_t DefaultLum     = 35.87;

class TopHistoReader{

  public:
    TopHistoReader(){
      path = DefaultPathY;
      pathFidu = DefaultPathFidu;
      outPath = DefaultOutPath;
      Lumi = DefaultLum;
      isSS = false; isFidu = false; doStatError = false;
      chan = "ElMu";
      level = "1btag";
      sys = "";
    }

    void SetPath(    TString p){ path = p;}
    void SetPathFidu(TString p){ pathFidu = p;}
    void SetOutPath( TString p){ outPath = p;}
    void SetChan(    TString p){ chan = p;}
    void SetLevel(   TString p){ level = p;}
    void SetSys(     TString p){ sys = p;}
    void SetDoStat(  Bool_t s = true){ doStatError  = s;}
    void SetIsSS(    Bool_t s = true){ isSS         = s;}
    void SetIsFidu(  Bool_t s = true){ isFidu       = s;}

    TString GetPath(){  return path;}
    TString GetPathFidu(){  return pathFidu;}
    TString GetOutPath(){  return outPath;}
    TString GetChan(){  return chan;}
    TString GetLevel(){ return level;}
    TString GetSys(){   return sys;}

    Float_t GetYield(     TString sample,              TString ch = "", TString Level = "", TString syst = "");
    //TH1F*   loadHistogram(TString sample, TString var, TString ch = "", TString Level = "", TString syst = "");
    Float_t GetTotalEntries(TString samplename);
    Float_t GetFiduEvents(TString sample);
    Float_t GetUnc(TString sample, TString ch = "", TString Level = "", TString syst = "");
    Float_t GetTopUnc(TString unc, TString ch = "", TString Level = "");

  private:
    TString path;
    TString pathFidu;
    TString outPath;
    Float_t Lumi;
    Bool_t isSS;
    Bool_t isFidu;
    Bool_t doStatError;

    TString chan;
    TString level;
    TString sys;
};


Float_t TopHistoReader::GetYield(TString sample, TString ch, TString Level, TString syst){
  syst = syst == ""? sys : syst; Level = Level == ""? level : Level; ch = ch == ""? chan : ch;
  if(ch == "All")   return GetYield(sample, "ElMu", Level, syst) + GetYield(sample, "Muon", Level, syst) + GetYield(sample, "Elec", Level, syst); 
  if(ch == "sameF") return GetYield(sample, "Muon", Level, syst) + GetYield(sample, "Elec", Level, syst); 

  TString process = sample; Float_t y = 0;
  if       (process == "ttbar"  ) y = GetYield("TTbar_Powheg", ch, Level, syst);
  else if  (process == "tW"     ) y = GetYield("TbarW", ch, Level, syst) + GetYield("TW", ch, Level, syst);
  else if  (process == "DY"     ) y = GetYield("DYJetsToLL_M50_aMCatNLO", ch, Level, syst);
  //else if  (process == "DY"     ) y = GetYield("DYJetsToLL_M10to50_aMCatNLO", ch, Level, syst) + GetYield("DYJetsToLL_M50_aMCatNLO", ch, Level, syst);
  else if  (process == "VV"     ) y = GetYield("WW", ch, Level, syst) + GetYield("WZ", ch, Level, syst) + GetYield("ZZ", ch, Level, syst);// + GetYield("ZZ", ch, Level, syst);
  else if  (process == "ttV"     ) y = GetYield("TTWToLNu", ch, Level, syst) + GetYield("TTWToQQ", ch, Level, syst) + GetYield("TTZToLLNuNu", ch, Level, syst) + GetYield("TTZToQQ", ch, Level, syst);// + GetYield("ZZ", ch, Level, syst);
  else if  (process == "fake"||process == "NonW/Z" ||process=="NonW")  y = GetYield("WJetsToLNu_MLM", ch, Level, syst) + GetYield("TTbar_Powheg_Semilep", ch, Level, syst); 
  else if  (process == "bkg"    ) {y = GetYield("tW", ch, Level, syst) + GetYield("DY", ch, Level, syst) + GetYield("VV", ch, Level, syst) + GetYield("fake", ch, Level, syst) + GetYield("ttV", ch, Level, syst); return y;}
  else if  (process == "data" || process == "Data"   ){ 
    if     (ch == "Elec") y = GetYield("SingleElec", ch, Level) + GetYield("DoubleEG", ch, Level);
    else if(ch == "Muon") y = GetYield("SingleMuon", ch, Level) + GetYield("DoubleMuon", ch, Level);
    else                  y = GetYield("MuonEG", ch, Level) + GetYield("SingleMuon", ch, Level) + GetYield("SingleElec", ch, Level);
  }

  else{
    int bin = 0;
    if(Level == "dilepton") bin = 1;
    if(Level == "ZVeto")    bin = 2;
    if(Level == "MET")      bin = 3;
    if(Level == "2jets")    bin = 4;
    if(Level == "1btag")    bin = 5;

    TH1F* h; TFile* inputfile;
    if(0)  inputfile = TFile::Open(pathFidu +    "/Tree_" + sample + ".root");
    else  inputfile = TFile::Open(path +    "/Tree_" + sample + ".root");
    TString    prefix = "H_Yields_";
    if(isSS)   prefix = "H_SSYields_";

    // Select nominal or variation
    if(syst == "")   inputfile->GetObject(prefix + ch, h);
    else             inputfile->GetObject(prefix + ch + "_" + syst, h);
    
    // Select GetYieldor stat error
    Float_t         yield = h->GetBinContent(bin);
    if(doStatError) yield = h->GetBinError(bin);

    delete inputfile;
    if(sample.Contains("aMCatNLO") && ( sample.Contains("TTbar") || sample.Contains("TTJets") ))  yield*= (1.08*0.9)*(1.08*0.9);

    if(isFidu) y = yield/GetFiduEvents(sample)*GetTotalEntries(sample);
    else if(!sample.Contains("Single") && !sample.Contains("Double") && !sample.Contains("MuonEG")) y = yield*Lumi*1000;
    else y = yield;
  }
  return y;
}

Float_t TopHistoReader::GetUnc(TString sample, TString ch = "", TString Level = "", TString syst = ""){
  syst = syst == ""? sys : syst; Level = Level == ""? level : Level; ch = ch == ""? chan : ch;
  Float_t nom   =  GetYield(sample, ch, Level);
  Float_t up    =  GetYield(sample, ch, Level+"Up");
  Float_t down  =  GetYield(sample, ch, Level+"Down");
  Float_t diff = TMath::Abs(up-nom) > TMath::Abs(down-nom)? TMath::Abs(up-nom) : TMath::Abs(down-nom);
  return diff/nom;
}
  
Float_t TopHistoReader::GetTopUnc(TString unc, TString ch = "", TString Level = ""){
  unc = unc == ""? sys : unc; Level = Level == ""? level : Level; ch = ch == ""? chan : ch;
  TString sample = "TTbar_Powheg";
  Float_t nom   =  GetYield(sample, ch, Level);
  Float_t up    =  GetYield(sample + "_" + unc + "Up", ch, Level);
  Float_t down  =  GetYield(sample + "_" + unc + "Down", ch, Level);
  Float_t diff = TMath::Abs(up-nom) > TMath::Abs(down-nom)? TMath::Abs(up-nom) : TMath::Abs(down-nom);
  return diff/nom;
}

Float_t TopHistoReader::GetTotalEntries(TString sample){
    TH1F* h; TFile* inputfile;
    inputfile = TFile::Open(pathFidu +    "/Tree_" + sample + ".root");
    h = (TH1F*) inputfile->Get("fhDummy");
    Float_t TotalEvents = h->GetEntries();
    return TotalEvents;
}

Float_t TopHistoReader::GetFiduEvents(TString sample){
  TString Level = level; TString ch = chan;
  int bin = 0;
  if(Level == "dilepton") bin = 1;
  if(Level == "ZVeto")    bin = 2;
  if(Level == "MET")      bin = 3;
  if(Level == "2jets")    bin = 4;
  if(Level == "1btag")    bin = 5;

  TH1F* h; TFile* inputfile;
  inputfile = TFile::Open(pathFidu +    "/Tree_" + sample + ".root");
  TString    prefix = "H_FiduYields_";

  inputfile->GetObject(prefix + ch, h);
  Float_t y = h->GetBinContent(bin);
  return y;
}

#endif 
