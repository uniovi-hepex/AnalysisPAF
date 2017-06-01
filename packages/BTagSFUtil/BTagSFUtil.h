#include <string>
#include "BTagCalibrationStandalone.h"
#include <Riostream.h>
#include "TRandom3.h"
#include "TMath.h"
#include "TF1.h"

using namespace std;

class BTagSFUtil{

 public:
    
  BTagSFUtil(const string& MeasurementType, 
	     const TString& BTagSFPath, const string& BTagAlgorithm, 
	     const TString& OperatingPoint, int SystematicIndex = 0, TString FastSimDataset = "");
  ~BTagSFUtil();

  float GetJetSF(float JetDiscriminant, int JetFlavor, float JetPt, float JetEta);
  bool IsTagged(float JetDiscriminant, int JetFlavor, float JetPt, float JetEta, UInt_t Seed = 0);

 private:

  BTagCalibrationReader *reader_b, *reader_c, *reader_l;

  void GetBTagPayload(TString BTagAlgorithm, TString DataPeriod);
  void GetFastSimPayload(TString BTagAlgorithm, TString FastSimDataset);

  float ScaleFactorB(float JetPt, int SystematicFlag);
  float ScaleFactorLight(float JetPt, float JetEta, int SystematicFlag);
  float ScaleFactorJet(int JetFlavor, float JetPt, float JetEta, int SystematicFlag);

  float JetTagEfficiency(int JetFlavor, float JetPt, float JetEta);
  float TagEfficiencyB(float JetPt, float JetEta);
  float TagEfficiencyC(float JetPt, float JetEta);
  float TagEfficiencyLight(float JetPt, float JetEta);

  float FastSimCorrectionFactor(int JetFlavor, float JetPt, float JetEta);
  
  TRandom3* rand_;

  TString TaggerName, TaggerOP;
  float TaggerCut;

  TF1 *funSFb, *funSFlight[4][3];

  int nBTagPtBins;
  float BTagPtBinEdge[50];
  float SFb_error[50];

  int nBTagEtaBins;
  float BTagEtaBinEdge[50];

  bool IsFastSimDataset;
  int nFastSimPtBins, nFastSimEtaBins[3];
  float FastSimPtBinEdge[50], FastSimEtaBinEdge[50][3];
  float FastSimCF[50][2][3];
  float FastSimCF_error[50][2][3]; int FastSimSystematic;

  string SystematicFlagBC, SystematicFlagL;
    
};

