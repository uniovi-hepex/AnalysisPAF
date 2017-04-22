
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagCalibration https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco
#include "BTagSFUtil.h"
#include "BTagCalibrationStandalone.cc"
//#include "BTagEfficienciesTTbarSummer12.C" // Change this to your sample efficiency
#include "BTagEfficienciesTTbarSummer15.C" // Change this to your sample efficiency
#include "FastSimCorrectionFactorsSummer12.C" // Change this to your sample efficiency
#include "TSystem.h"

using namespace std;

BTagSFUtil::BTagSFUtil(string MeasurementType, string BTagAlgorithm, TString OperatingPoint, int SystematicIndex, TString FastSimDataset) {

  //rand_ = new TRandom3(Seed);

  string CSVFileName = (string)gSystem->ExpandPathName("$PWD") + "/packages/BTagSFUtil/" + BTagAlgorithm + ".csv";
  const BTagCalibration calib(BTagAlgorithm, CSVFileName);

  SystematicFlagBC = "central";
  SystematicFlagL  = "central";
  if (abs(SystematicIndex)<10) {
    if (SystematicIndex==-1 || SystematicIndex==-2) SystematicFlagBC = "down";
    if (SystematicIndex==+1 || SystematicIndex==+2) SystematicFlagBC = "up";
    if (SystematicIndex==-3) SystematicFlagL = "down";
    if (SystematicIndex==+3) SystematicFlagL = "up";
  }
  vector<string> sysTypes;
  sysTypes.push_back("up");
  sysTypes.push_back("down");

  TaggerCut = -1;
  TaggerName = BTagAlgorithm;
  TaggerOP = BTagAlgorithm;

  if (OperatingPoint=="Loose")  {
    TaggerOP += "L";
    if (TaggerName=="CSV") TaggerCut = 0.244;
    //if (TaggerName=="CSVv2") TaggerCut = 0.605;
    if (TaggerName=="CSVv2") TaggerCut = 0.5426; // for Moriond17
    reader_b = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", sysTypes);
    reader_b -> load(calib, BTagEntry::FLAV_B, MeasurementType);
    reader_c = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", sysTypes);
    reader_c -> load(calib, BTagEntry::FLAV_C, MeasurementType);
    reader_l = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", sysTypes);
    reader_l -> load(calib, BTagEntry::FLAV_UDSG, "incl");
  } else if (OperatingPoint=="Medium")  {
    TaggerOP += "M";
    if (TaggerName=="CSV") TaggerCut = 0.679;
    //if (TaggerName=="CSVv2") TaggerCut = 0.890; // for 74X
    //if (TaggerName=="CSVv2") TaggerCut = 0.800; // for 76X
    if (TaggerName=="CSVv2") TaggerCut = 0.8484; // for Moriond17
    reader_b = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", sysTypes);
    reader_b -> load(calib, BTagEntry::FLAV_B, MeasurementType);
    reader_c = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", sysTypes);
    reader_c -> load(calib, BTagEntry::FLAV_C, MeasurementType);
    reader_l = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", sysTypes);
    reader_l -> load(calib, BTagEntry::FLAV_UDSG, "incl");
  } else if (OperatingPoint=="Tight")  {
    TaggerOP += "T";
    if (TaggerName=="CSV") TaggerCut = 0.898;
    if (TaggerName=="TCHP") TaggerCut = 3.41;
    //if (TaggerName=="CSVv2") TaggerCut = 0.970;
    if (TaggerName=="CSVv2") TaggerCut = 0.9535; // for Moriond17
    reader_b = new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", sysTypes);
    reader_b -> load(calib, BTagEntry::FLAV_B, MeasurementType);
    reader_c = new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", sysTypes);
    reader_c -> load(calib, BTagEntry::FLAV_C, MeasurementType);
    reader_l = new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", sysTypes);
    reader_l -> load(calib, BTagEntry::FLAV_UDSG, "incl");
  } 

  if (TaggerCut<0) 
    cout << " " << TaggerName << " not supported for " << OperatingPoint << " WP" << endl;

  FastSimSystematic = 0;
  if (abs(SystematicIndex)>10) FastSimSystematic = SystematicIndex%10;
  GetFastSimPayload(BTagAlgorithm, FastSimDataset);

  if (TaggerCut<0.) 
    cout << "BTagSFUtil: " << BTagAlgorithm << " not a supported b-tagging algorithm" << endl;

}

BTagSFUtil::~BTagSFUtil() {

  delete reader_b, reader_c, reader_l;

}

float BTagSFUtil::FastSimCorrectionFactor(int JetFlavor, float JetPt, float JetEta) {

  float CF = 1.;
 
  if (JetPt<FastSimPtBinEdge[0]) { cout << "CF is not available for jet pt<" << FastSimPtBinEdge[0] << " GeV" << endl; return -1.; }
  if (fabs(JetEta)>2.4) { cout << "CF is not available for jet |eta|>2.4" << endl; return -1.; }

  int JetFlavorFlag = 2;
  if (abs(JetFlavor)==4) JetFlavorFlag = 1;
  else if (abs(JetFlavor)==5) JetFlavorFlag = 0;

  int ThisFastSimSystematic = 0;
  if (abs(FastSimSystematic)==JetFlavorFlag+1) 
    ThisFastSimSystematic = FastSimSystematic/abs(FastSimSystematic);
 
  int JetPtBin = -1;
  for (int ptbin = 0; ptbin<nFastSimPtBins; ptbin++) 
    if (JetPt>=FastSimPtBinEdge[ptbin]) JetPtBin++;

  if (JetPt>=FastSimPtBinEdge[nFastSimPtBins]) ThisFastSimSystematic *= 2;

  int JetEtaBin = -1;  
  for (int etabin = 0; etabin<nFastSimEtaBins[JetFlavorFlag]; etabin++) 
    if (fabs(JetEta)>=FastSimEtaBinEdge[etabin][JetFlavorFlag]) JetEtaBin++;
    
  CF = FastSimCF[JetPtBin][JetEtaBin][JetFlavorFlag] + ThisFastSimSystematic*FastSimCF_error[JetPtBin][JetEtaBin][JetFlavorFlag];

  if (CF<0.) CF = 0.; // Effect of large uncertainties on light CFs!

  return CF;

}

float BTagSFUtil::JetTagEfficiency(int JetFlavor, float JetPt, float JetEta) {

  if (abs(JetFlavor)==5) return TagEfficiencyB(JetPt, JetEta);
  else if (abs(JetFlavor)==4) return TagEfficiencyC(JetPt, JetEta);
  else return TagEfficiencyLight(JetPt, JetEta);

}

float BTagSFUtil::GetJetSF(float JetDiscriminant, int JetFlavor, float JetPt, float JetEta) {

  float Btag_SF;

  if (JetPt < 20. && abs(JetEta) < 2.4 && JetDiscriminant > 0. && JetDiscriminant < 1.)  
    cout << "WARNING: BTagSFUtil: Found jet with pT " << JetPt << " GeV, smaller than 20 GeV. From BTagRecommendation80XReReco: do NOT go lower than 20 GeV." << endl << "Please check your jet pT thresholds. BTagSF for 20 GeV with double uncertainty has been applied." << endl;

  if (abs(JetFlavor)==5) 
    Btag_SF = reader_b->eval_auto_bounds(SystematicFlagBC, BTagEntry::FLAV_B, JetEta, JetPt, JetDiscriminant);
  else if (abs(JetFlavor)==4) 
    Btag_SF = reader_c->eval_auto_bounds(SystematicFlagBC, BTagEntry::FLAV_C, JetEta, JetPt, JetDiscriminant);
  else Btag_SF = reader_l->eval_auto_bounds(SystematicFlagL, BTagEntry::FLAV_UDSG, JetEta, JetPt, JetDiscriminant);
  
  if (IsFastSimDataset)
    Btag_SF *= FastSimCorrectionFactor(JetFlavor, JetPt, JetEta);
  
  return Btag_SF;

}

bool BTagSFUtil::IsTagged(float JetDiscriminant, int JetFlavor, float JetPt, float JetEta, UInt_t Seed) {
  bool isBTagged = JetDiscriminant>TaggerCut;
  if (JetFlavor==-999999) return isBTagged; // Data: no correction needed
  bool newBTag = isBTagged;
  float Btag_SF = GetJetSF(JetDiscriminant, JetFlavor, JetPt, JetEta);
  if (Btag_SF == 1) return newBTag; //no correction needed 

  //throw die
  //Seed = 0; // Uncomment for using random seed.
  rand_ = new TRandom3(Seed); // Fixed seeds, dependent on event number, jet pT and systematic variation, for reproducibility. Use Seed=0 for testing that seed dependence is negligible.
  float coin = rand_->Uniform(1.);    
 
  if(Btag_SF > 1){  // use this if SF>1

    if( !isBTagged ) {

      float Btag_eff = JetTagEfficiency(JetFlavor, JetPt, fabs(JetEta));

      //fraction of jets that need to be upgraded
      float mistagPercent = (1.0 - Btag_SF) / (1.0 - (1./Btag_eff) );
      
      //upgrade to tagged
      if( coin < mistagPercent ) {newBTag = true;}
    }

  }else{  // use this if SF<1
      
    //downgrade tagged to untagged
    if( isBTagged && coin > Btag_SF ) {newBTag = false;}

  }

  delete rand_;
  return newBTag;

}

