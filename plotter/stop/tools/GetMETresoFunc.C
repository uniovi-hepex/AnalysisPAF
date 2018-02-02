#include "FunctionsPAF.C"

TString path = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/stop/METresolution/METresoSF.root";
TString var = "met";

void GetMETresoFunc(){
  GetFunctionFromTH1F(path, "SF",   "GetMetResolutionSF",   var);
}
