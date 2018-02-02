#include "FunctionsPAF.C"

TString path = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/FSR_JECSF.root";
TString var = "pt";

void GetFSR_JECSF_Func(){
  GetFunctionFromTH1F(path, "FSR_JECSF_Up",   "GeFSR_JECSF_Up",   var);
  GetFunctionFromTH1F(path, "FSR_JECSF_Down", "GeFSR_JECSF_Down", var);
}
