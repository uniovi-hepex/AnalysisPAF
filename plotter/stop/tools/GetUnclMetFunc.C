#include "FunctionsPAF.C"

void GetUnclMetFunc(){

  TString path = "InputFiles/UnclMETratios.root";

  TString hup = "UncMetUpRatio";
  TString hdo = "UncMetDownRatio";

  GetFunctionFromTH1F(path, hup, "GetMetUnclWeightUp",   "met");
  GetFunctionFromTH1F(path, hdo, "GetMetUnclWeightDown", "met");
}
