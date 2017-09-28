//#######################################################
//
// Class to draw 2D plots with exclusion and numbers...
//
//#######################################################

class Exclusion2D{
  public:
  Exclusion2D(){InitParams();};
  Exclusion2D(Int_t nx, Float_t bx0, Float_t bxn, Int_t ny, Float_t by0, Float_t byn){
    InitParams();
    nbX = nx;
    nbY = ny;
    FirstBinX = bx0;
    LastBinX = bxn;
    FirstBinY = by0;
    LastBinY = byn;
  };
  Exclusion2D(Int_t nx, Float_t* x, Int_t ny, Float_t* y){
    InitParams();
    nbX = nx;
    nbY = ny;
    bX = x;
    bY = y;
  }
  ~Exclusion2D(){
    if(h) delete h;
  };

  public:
  std::vector<Int_t>   TStringToVectorInt(TString s, char separator = ',');
  std::vector<Float_t> TStringToVectorFloat(TString s, char separator = ',');
  void SetNBinsX(Int_t n){nbX = n;}
  void SetNBinsY(Int_t n){nbY = n;}
  void SetXBins(Float_t *x){ bX = x;}
  void SetYBins(Float_t *y){ bY = y;}
  void SetFirstBinX(Float_t b){ FirstBinX = b;}
  void SetLastBinX(Float_t b){ LastBinX = b;}
  void SetFirstBinY(Float_t b){ FirstBinY = b;}
  void SetLastBinY(Float_t b){ LastBinY = b;}
  void SetZmin(Float_t z){ zMin = z;}
  void SetZmax(Float_t z){ zMax = z;}

  void SetLspM(std::vector<Int_t> m){ mLsp = m;};
  void SetLspM(Float_t m){ mLsp.push_back(m);};
  void SetLspM(TString s){mLsp = TStringToVectorInt(s);};
  
  void SetStopM(std::vector<Int_t> m){ mStop = m;};
  void SetStopM(Float_t m){ mStop.push_back(m);};
  void SetStopM(TString s){ mStop = TStringToVectorInt(s);};

  void SetVal(std::vector<Float_t> m){ r = m;};
  void SetVal(Float_t m){ r.push_back(m);};
  void SetVal(TString s){ r = TStringToVectorFloat(s);};

  void SetOutDir(TString o){ OutDir = o;}
  void SetPlotName(TString o){ PlotName = o;}
  void SetDrawOptions(TString o){DrawOptions = o;}
  void SetSaveOptions(TString o){SaveOptions = o;}

  TH2F* GetH(){ return h;}

  void InitParams();
  void InitTH2F();
  void Fill();
  void Draw();

  protected:
  TH2F* h;
  TCanvas* c;

  std::vector<Float_t> r;
  std::vector<Int_t> mLsp;
  std::vector<Int_t> mStop;

  Float_t *bX;
  Float_t *bY;
  Int_t nbX;
  Int_t nbY;
  Float_t FirstBinX;
  Float_t LastBinX;
  Float_t FirstBinY;
  Float_t LastBinY;

  Float_t zMin;
  Float_t zMax;

  TString OutDir;
  TString PlotName;
  TString DrawOptions;
  TString SaveOptions;
};

void Exclusion2D::InitParams(){
  bX = nullptr;
  bY = nullptr;
  nbX = 0;
  nbY = 0;
  FirstBinX = 0;
  LastBinX = 0;
  FirstBinY = 0;
  LastBinY = 0;

  zMin = -99;
  zMax = -99;

  OutDir = "./";
  PlotName = "Exclusion2D";
  DrawOptions = "colz,text";
  SaveOptions = "png,pdf";
} 

void Exclusion2D::InitTH2F(){
  if( (FirstBinX == LastBinX) || (FirstBinY == LastBinY) ){
    h = new TH2F("histo2D", "Exclusion plot", nbX, bX, nbY, bY);
  }
  else 
    h = new TH2F("histo2D", "Exclusion plot", nbX, FirstBinX, LastBinX, nbY, FirstBinY, LastBinY);
}

void Exclusion2D::Fill(){
  InitTH2F();
  Int_t np = mLsp.size();
  Int_t mx = 0; Int_t my = 0; Float_t val = 0;
  if(np == 0 || np != mLsp.size() || np != r.size()){
    cout << "[Exclusion2D] Error: no Stop-Lsp mass points!" << endl;
    return;
  }
  for(Int_t k = 0; k < np; k++){
    mx = mStop.at(k); my = mLsp.at(k); val = r.at(k);
    //cout << "Point [mStop, mLsp, val] = [" << mx << ", " << my << ", " << val << "]" << endl;
    h->SetBinContent(h->FindBin(mx, my, 1), val);
  }
}

void Exclusion2D::Draw(){
  Int_t np = mLsp.size();
  if(np == 0 || np != mLsp.size() || np != r.size()){
    cout << "[Exclusion2D] Error: no Stop-Lsp mass points!" << endl;
    return;
  }
  else Fill();

  c = new TCanvas("c","c",10,10,850,600);

  if(zMin != -99) h->SetMinimum(zMin);
  if(zMax != -99) h->SetMaximum(zMax);
  h->GetXaxis()->SetTitle("Stop mass [GeV]");
  h->GetYaxis()->SetTitle("LSP mass [GeV]");
  h->GetZaxis()->SetTitle("Expected limit on signal strength");
  h->SetStats(0);
  h->SetTitle("");

  h->Draw(DrawOptions);

  gSystem->mkdir(OutDir, kTRUE);
  if(SaveOptions.Contains("pdf")) c->SaveAs(OutDir + "/" + PlotName + ".pdf", "pdf");
  if(SaveOptions.Contains("png")) c->SaveAs(OutDir + "/" + PlotName + ".png", "png");
  delete c;
}

//=== Extra functions
std::vector<Int_t> Exclusion2D::TStringToVectorInt(TString t, char separator){
  std::vector<Int_t> v;
  t.ReplaceAll(" ", "");
  Int_t n = t.CountChar(separator);
  TString element;
  for(Int_t i = 0; i < n; i++){
    element = t(0, t.First(separator));
    t = t(t.First(separator)+1, t.Sizeof());
    v.push_back(element.Atoi());
  }
  v.push_back(t.Atoi());
  return v;
}

std::vector<Float_t> Exclusion2D::TStringToVectorFloat(TString t, char separator){
  std::vector<Float_t> v;
  t.ReplaceAll(" ", "");
  Int_t n = t.CountChar(separator);
  TString element;
  for(Int_t i = 0; i < n; i++){
    element = t(0, t.First(separator));
    t = t(t.First(separator)+1, t.Sizeof());
    v.push_back(element.Atof());
  }
  v.push_back(t.Atof());
  return v;
}
