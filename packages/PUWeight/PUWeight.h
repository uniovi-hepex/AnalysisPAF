///////////////////////////////////////////////////////////////////////
//
//    FILE: PUWeight.h
//   CLASS: PUWeight
// AUTHORS: I. Gonzalez Caballero
//    DATE: 09/03/2011
//
// CONTENT: An utility class to weight the events according to the real
//          data pile up with respect to the generated pileup.
//          For more details see:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities
//
///////////////////////////////////////////////////////////////////////

#ifndef PUWEIGHT_H
#define PUWEIGHT_H 1


#include "TH1D.h"
#include "TH3D.h"

enum EMCDistribution {
  Spring11,       /*Flat10+Tail*/ 

  Summer11,       /*PU_S4 averaging the number of interactions in each beam
       * crossing
       */
  Summer113D,     /*Using Flat10 + tail since this is the "true" input for all
       * Summer11 samples.
       */
  Summer11InTime, /*PU_S4 obtained by only looking at the in-time crossing*/
  
  Summer11ITSmear,/*PU_S4 obtained by only looking at the in-time crossing.
       *This is the "spike+smear" distribution. RECOMENDED!
       */
  Summer11True,   /*PU_S4 obtained by averaging the number of interactions in
       * each beam crossing to estimate the true mean.
       */
  
  Fall11,         /*Fall11*/

  Fall11True,     /*Fall11 True from TTbar 2l2Nu Powheg sample by Matt*/
  
  Summer12,        /*Summer12 MC*/

  Summer12_53X,     /* Summer12 MC 53X*/

  Summer2015_50ns_poisson,     /* Summer15*/

  Summer2015_25ns_poisson,     /* Summer15*/
  
  Fall2015_25ns_matchData_poisson,
  
  Spring2016_25ns_poisson_OOTPU, /* Spring16 */
    
  Moriond17MC_PoissonOOTPU /* Summer16 */   
};

class PUWeight {
 public:
  /**************************************
   * Main Constructor: Builds the weights for the pileup from the Data 
   * PU profile for a given luminosity, and the MC PU Profile according
   * to the distribution specified in mcdistr
   * 
   * + luminosity: Determines the data profile to use. Set it to -1 to
   * use the default value
   * 
   * + mcdistr: MC PU Profile with which MC was generated. See the declaration
   * of EMCDistribution for details. It defaults to Spring11
   * 
   * + year: An string identifying the processing. It currently defaults to
   * the only possible value (2011A)
   * 
   */
  PUWeight(float luminosity, 
     EMCDistribution mcdistr = Spring11,
     const char* year="2011A");


  /**************************************
   * Destructor
   */
  ~PUWeight() {delete fWeight; delete fMC; delete fData;}


  /**************************************
   * Methods
   */

  // Tells if this object has loaded correctly the histograms
  bool IsValid() const { return fWeight;}


  // Returns the weight for a given PU value
  float GetWeight(unsigned int pu) const {
    return (fWeight? fWeight->GetBinContent(pu+1):0);
  }

  // Returns the weights for a given PU value using 3D algorithm
  float GetWeight3D(unsigned int puminus, 
        unsigned int pu0, 
        unsigned int puplus) {
    if (!fWeight3D)
      CalculateWeight3D();
    return fWeight3D->GetBinContent(puminus+1, pu0+1, puplus+1);
  }

  // Returns the MC only weight for a given PU value
  float GetPUMC(unsigned int pu) const {
    return (fMC? fMC->GetBinContent(pu+1):0);
  }
  // Returns the Data only weight for a given PU value
  float GetPUData(unsigned int pu) const {
    return (fData? fData->GetBinContent(pu+1):0);
  }

  // Get the histogram with the weights
  TH1D* GetWeightsHisto() const {return (TH1D*)fWeight->Clone();}

  // Get the histogram with the weights in 3D
  TH3D* GetWeightsHisto3D() {
    if (!fWeight3D)
      CalculateWeight3D();
    return (TH3D*)fWeight3D->Clone();
  }

  // Get the histogram with the profile for Data
  TH1D* GetDataHisto() const {return (TH1D*)fData->Clone();}

  // Get the histogram with the profile for MC
  TH1D* GetMCHisto() const {return (TH1D*)fMC->Clone();}

  // Rescale the 3D weight to study systematic errors
  TH3D* RescaleWeights3D(float ScaleFactor = 1.0) {return CalculateWeight3D(ScaleFactor);}

 protected:
  // Build the PU ideal profile for MC
  TH1D* IdealMCHistogram(EMCDistribution);
  // Helper class filling 

  // Load the PU profile for MC
  TH1D* LoadMCHistogram(const char* mcfolder, const char* mcproccess);
  // Load the PU profile for Data
  TH1D* LoadDataHistogram(float luminosity, const char* year="2011A");

  // Divide the Data profile by the MC profile
  TH1D* CalculateWeight();

  // Build 3D histogram with weights
  TH3D* CalculateWeight3D(float ScaleFactor = 1.0);

 protected:
  TH1D* fData;     //PU profile for data
  TH1D* fMC;       //PU profile for MC
  TH1D* fWeight;   //Histogram with the weight content
  TH3D* fWeight3D; //The 3D weights
  bool  fIs3D;     //True if 3D should be used
};


#endif

