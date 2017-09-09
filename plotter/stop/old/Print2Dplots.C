#include "Exclusion2D.C"

Float_t *bx; Float_t *by;
Int_t nx; Int_t ny;
TString mStop; TString mLsp;

TString val; TString name;
Float_t minval; Float_t maxval;

TString mStop_inc   = " 175,  183,  192,  200,  208,  217,  242,  250,  200,  225,  250";
TString mLsp_inc    = "   1,    1,   25,   25,   25,   50,   75,   75,   50,   50,   50";
TString val_inc_mt2 = "0.29, 0.42, 0.53, 0.57, 0.56, 0.73, 0.88, 1.43, 0.87, 0.90, 0.81";
TString val_inc_cc  = "0.56, 0.67, 1.10, 1.06, 1.18, 1.88, 3.17, 2.90, 2.08, 1.80, 2.91";

TString mStop_dil   = " 160,  160,  160,  170,  170,  170,  175,  175,  175,  180,  180,  180,  190,  190,  190,  200,  200,  200,  210,  210,  210";
TString mLsp_dil    = "   1,   10,   20,    1,   10,   20,    1,   10,   20,    1,   10,   20,    1,   10,   20,    1,   10,   20,    1,   10,   20";
TString val_dil_cc  = "0.68, -1.0, 1.00, 0.66, 0.85, 1.02, 0.56, 0.85, 1.07, 0.61, 0.76, 1.03, 0.76, 0.76, 0.94, 1.02, 1.21, 1.02, 1.21, 1.21, 1.21";
TString val_dil_mt2 = "0.48, -1.0, 0.58, 0.51, 0.58, 0.53, 0.35, 0.58, 0.58, 0.37, 0.57, 0.58, 0.48, 0.47, 0.66, 0.63, 0.63, 0.63, 0.60, 0.63, 0.68";

void Print2Dplots(TString name = ""){
  if(name == ""){
    Print2Dplots("Dilep_MT2");
    Print2Dplots("Dilep_CutAndCount");
    Print2Dplots("Incl_MT2");
    Print2Dplots("Incl_CutAndCount");
    Print2Dplots("All_MT2");
    Print2Dplots("All_CutAndCount");
    return;
  }
  else if(name.Contains("Dil")){
    Float_t _bx[] = {155, 165, 173, 178, 185, 195, 205, 215, 225}; 
    Float_t _by[] = {0, 5, 15, 25}; 
    bx = _bx; by = _by; nx = 8; ny = 3;
    mStop = mStop_dil;
    mLsp  = mLsp_dil;
    if(name.Contains("MT2")){
      val = val_dil_mt2; 
      minval = 0.3; maxval = 1.0;
    }
    else{ // CutAndCount
      val = val_dil_cc; 
      minval = 0.5; maxval = 1.2;
    }
  }
  else if(name.Contains("Incl")){
    Float_t _bx[] = {170, 180, 185, 190, 195, 200, 205, 210, 220, 230, 240, 245, 255};
    Float_t _by[] = {0, 5, 20, 30, 45, 55, 70, 80}; 
    bx = _bx; by = _by; nx = 12; ny = 7;
    mStop = mStop_inc;
    mLsp  = mLsp_inc;
    if(name.Contains("MT2")){
      val = val_inc_mt2; 
      minval = 0.3; maxval = 3;
    }
    else{ // CutAndCount
      val = val_inc_cc; 
      minval = 0.3; maxval = 1.5;
    }
  }
  else if(name.Contains("All")){
    Float_t _bx[] = {155, 165, 173, 178, 181, 185, 190, 195, 200, 205, 210, 215, 220, 230, 240, 245, 255};
    Float_t _by[] = {0, 5, 15, 20, 25, 30, 45, 55, 70, 80}; 
    bx = _bx; by = _by; nx = 16; ny = 9;
    mStop = mStop_inc + "," + mStop_dil;
    mLsp  = mLsp_inc  + "," + mLsp_dil;
    if(name.Contains("MT2")){
      TString val_inc   = "";
      TString val_dil   = "";
      val = val_inc_mt2 + ", " + val_dil_mt2;
      minval = 0.3;
      maxval = 1.5;
    }
    else{ // CutAndCount
      TString val_inc   = "";
      TString val_dil   = "";
      val = val_inc_cc + ", " + val_dil_cc;
      minval = 0.3;
      maxval = 3;
    }
  }

  Exclusion2D *p = new Exclusion2D(nx, bx, ny, by);
  p->SetStopM(mStop); p->SetLspM(mLsp); p->SetVal(val);
  p->SetZmin(minval);
  p->SetZmax(maxval);
  p->SetPlotName(name);
  p->SetOutDir("Plots2D");
  p->Draw();
  delete p;
}








//TString val_inc_mt2 = "0.32, 0.42, 0.63, 0.63, 0.65, 0.68, 0.92, 1.46, 0.68, 0.83, 0.66";
//TString val_inc_cc  = "0.59, 0.67, 1.07, 1.08, 1.16, 1.82, 2.96, 2.85, 1.96, 1.76, 2.62";
//TString val_dil_cc  = "-1.0, -1.0, 1.00, 0.68, 0.82, 1.02, 0.59, 0.86, 1.06, 0.64, 0.78, 1.02, 0.78, 0.79, 0.95, 1.00, 1.01, 1.03, 1.17, 1.18, 1.18";
//TString val_dil_mt2 = "-1.0, -1.0, 0.70, 0.56, 0.58, 0.63, 0.37, 0.65, 0.68, 0.42, 0.67, 0.68, 0.58, 0.54, 0.82, 0.80, 0.80, 0.74, 0.79, 0.77, 0.87";
