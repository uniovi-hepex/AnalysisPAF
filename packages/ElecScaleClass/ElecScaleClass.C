#include "ElecScaleClass.h"

ElecScaleVal GetElecScaleVal(unsigned category, unsigned runMin, unsigned runMax, float unc){
  ElecScaleVal s;
  s.runMin = runMin;
  s.runMax = runMax;
  s.category = category;
  s.uncertainty = unc;
  return s;
}

unsigned ElecScaleClass::GetCategory(float eta, float pt, float R9){
  unsigned etaCat = GetEtaCat(eta);
  unsigned R9Cat  = GetR9Cat(R9);
  unsigned ptCat  = GetPtCat(pt, etaCat, R9Cat);

  return GetCategory(etaCat, ptCat, R9Cat);
}

unsigned ElecScaleClass::GetCategory(const unsigned &etaCat, unsigned &ptCat, const unsigned &R9Cat){
  if(etaCat >= 4) ptCat = 0;
  unsigned cat = etaCat*100 + ptCat*10 + R9Cat;
  return cat;
}

unsigned ElecScaleClass::GetEtaCat(float eta){
  eta = fabs(eta);
  if(eta <= 1)      return 1;
  if(eta <= 1.4442) return 2;
  if(eta <= 1.566 ) return 3;
  if(eta <= 2)      return 4;
  return 5;
}


unsigned ElecScaleClass::GetPtCat( float pt, float eta, float R9){
  unsigned etaCat = GetEtaCat(eta);
  unsigned R9Cat = GetR9Cat(R9);
  return GetPtCat(pt, etaCat, R9Cat);
}

unsigned ElecScaleClass::GetPtCat( float pt, unsigned etaCat, unsigned R9Cat){
  if(etaCat >= 3) return 0;
  else if(R9Cat == 0){
    if(     pt <= 20 ) return 1;
    else if(pt <= 33 ) return 2;
    else if(pt <= 39 ) return 3;
    else if(pt <= 45 ) return 4;
    else if(pt <= 50 ) return 5;
    else if(pt <= 58 ) return 6;
    else if(pt <= 100) return 7;
    else               return 8;
  }
  else if(R9Cat == 1){
    if(etaCat == 1){
      if(     pt <= 20 ) return 1;
      else if(pt <= 35 ) return 2;
      else if(pt <= 43 ) return 3;
      else if(pt <= 50 ) return 4;
      else if(pt <= 55 ) return 5;
      else if(pt <= 100) return 6;
      else               return 7;
    }
    else if(etaCat == 2){
      if(     pt <= 20 ) return 1;
      else if(pt <= 40 ) return 2;
      else if(pt <= 50 ) return 3;
      else if(pt <= 100) return 4;
      else               return 5;
    }
  }
  cout << Form("[ElecScaleClass::GetPtCat] ERROR: Category not found for pt = %f, etaCat = %u, R9Cat = %u\n", pt, etaCat, R9Cat);
  return 0;
}

unsigned ElecScaleClass::GetR9Cat( float R9){
  if(R9 >= -1 && R9 < 0.94) return 0;
  if(R9 >= 0.94) return 1;
  cout << "[ElecScaleClass::GetR9Cat] ERROR: R9 out of range (" << R9 << ")" << endl;
  return 0;
}

/*
const ElecScaleVal &FindValues(unsigned cat, unsigned run){
  for(auto it = ElecCorrCategories.begin(); it != ElecCorrCategories.end(); it++){
    if(it->category != cat) continue;
    if(run > it->runMax || run < it->runMin) continue;
    return it->uncertainty;
  }
  cout 
}*/

float ElecScaleClass::GetUnc(unsigned cat, unsigned run){
  for(auto it = ElecCorrCategories.begin(); it != ElecCorrCategories.end(); it++){
    if(it->category != cat) continue;
    if(run != 1) if(run > it->runMax || run < it->runMin) continue;
    return it->uncertainty;
  }
  cout << Form("[ElecScaleClass::GetUnc] ERROR: not found category %u or run %u\n", cat, run);
  return 0.0;
}

float ElecScaleClass::GetUnc(float pt, float eta, float R9, unsigned run){
  unsigned cat = GetCategory(pt, eta, R9);
  return GetUnc(cat, run);
}


void ElecScaleClass::loadCategories(){
  string lin; TString line;
  ifstream file(filename);
  if(!file.is_open()){
    cout << "[ElecScaleClass] ERROR: could not open file " << filename << endl;
    return;
  }
  else{
    while( getline(file, lin)){
      line = TString(lin);
      AddCategory(line);
    }
    file.close();
  }
}

void ElecScaleClass::AddCategory(TString line){
  unsigned category = GetCategoryFromLine(line);
  unsigned runMin   = GetRunMinFromLine(line);
  unsigned runMax   = GetRunMaxFromLine(line);
  float    unc      = GetUncFromLine(line);
  ElecCorrCategories.push_back(GetElecScaleVal(category, runMin, runMax, unc));
  if(verbose) cout << Form("Adding category with numbers: %u, %u, %u, %f || %s\n", category, runMin, runMax, unc, line.Data());
}

unsigned ElecScaleClass::GetCategoryFromLine(const TString &line){
  TString l = line(line.First("absEta_")+7, line.First("\t")-7);
  vector<TString> veta = TStringToVector(l(0, l.First("-")), '_');
  if(veta.size() < 2) cout << "[ElecScaleClass::GetCategoryFromLine] WARNING: veta.size < 2" << endl;
  float eta = (veta.at(1).Atof() + veta.at(0).Atof())/2;
  unsigned etaCat = GetEtaCat(eta);
  unsigned R9Cat = (l.Contains("gold")) ? 1 : 0;
  unsigned ptCat = 0;
  if(etaCat >= 4) ptCat = 0;
  else{
    vector<TString> vpt = TStringToVector(l(l.Index("Et_")+3, l.Sizeof()), '_');
    if(vpt.size() < 2) cout << "[ElecScaleClass::GetCategoryFromLine] WARNING: vpt.size < 2" << endl;
    float pt = (vpt.at(1).Atof() + vpt.at(0).Atof())/2;
    ptCat = GetPtCat(pt, etaCat, R9Cat);
  }

  return GetCategory(etaCat, ptCat, R9Cat);
}

unsigned ElecScaleClass::GetRunMinFromLine(  const TString &line){
  vector<TString> v = TStringToVector(line, '\t');
  if(v.size() < 8) cout << "[ElecScaleClass::GetRunMinFromLine] WARNING: v.size() < 8" << endl;
  unsigned d = v.at(2).Atoi();
  return d;
}

unsigned ElecScaleClass::GetRunMaxFromLine(  const TString &line){
  vector<TString> v = TStringToVector(line, '\t');
  if(v.size() < 8) cout << "[ElecScaleClass::GetRunMaxFromLine] WARNING: v.size() < 8" << endl;
  unsigned d = v.at(3).Atoi();
  return d;
}

float    ElecScaleClass::GetUncFromLine(     const TString &line){
  vector<TString> v = TStringToVector(line, '\t');
  if(v.size() < 8) cout << "[ElecScaleClass::GetUncFromLine] WARNING: v.size() < 8" << endl;
  double syst = 0; double x = 0;
  for(int i = 5; i <= 8; i++){
    x = v.at(i).Atof();
    syst += x*x;
  }
  return TMath::Sqrt(syst);
}

std::vector<TString> TStringToVector(const TString &_t, char separator){
  std::vector<TString> v;
  TString t = _t;
  t.ReplaceAll(" ", "");
  Int_t n = t.CountChar(separator);
  TString element;
  for(Int_t i = 0; i < n; i++){
    element = t(0, t.First(separator));
    t = t(t.First(separator)+1, t.Sizeof());
    v.push_back(element);
  }
  v.push_back(t);
  return v;
}

