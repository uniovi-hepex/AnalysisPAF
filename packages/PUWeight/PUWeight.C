///////////////////////////////////////////////////////////////////////
//
//    FILE: PUWeight.h
//   CLASS: PUWeight
// AUTHORS: I. Gonzalez Caballero
//    DATE: 09/03/2011
//
///////////////////////////////////////////////////////////////////////
#include "PUWeight.h"

// PAF includes
#include "PAF.h"

// ROOT includes
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"

// C++ includes
#include <iostream>
#include <math.h>
using namespace std;


//Set DEBUGPUWEIGHT to 1 to get some debug information. Set it to 2 for more
//detail debug information.
#define DEBUGPUWEIGHT 1

#ifdef DEBUG
#define DEBUGPUWEIGHT 1
#endif


PUWeight::PUWeight(float luminosity, EMCDistribution mcdistr, const char* year):
  fData(0),
  fMC(0),
  fWeight(0),
  fWeight3D(0),
  fIs3D(false) {


  //Set 3D to true if using 3D weight
  if (mcdistr == Summer113D)
    fIs3D = true;

  //Load Data histogram
  if (!LoadDataHistogram(luminosity, year))
    return;

  //No MC given. Take ideal MC
  IdealMCHistogram(mcdistr);


  //Calculate Weight
  if (fIs3D){
    //cout << ">> CalculateWeight3D() " << endl;
    CalculateWeight3D();
  }else{
    //cout << ">> CalculateWeight() " << endl;
    CalculateWeight();
  }
}




TH1D* PUWeight::LoadMCHistogram(const char* mcfolder, const char* mcproccess) {
#ifdef DEBUGPUWEIGHT
  PAF_DEBUG("PUWeight", Form("Getting pileup for the MC %s inside %s...",
			     mcproccess, mcfolder));
#endif
  
  TString dsfile;
  dsfile.Form("http://www.hep.uniovi.es/jfernan/PUhistos/%s/%s.root", 
        mcfolder, mcproccess);
#if (DEBUGPUWEIGHT > 1)
  PAF_DEBUG("PUWeight", Form("   + Opening %s", dsfile.Data()));
#endif
  
  TFile* fds = TFile::Open(dsfile);
  if (!fds) {
    cerr << "ERROR [PUWeight]: Could not open file " << dsfile << "!"  << endl
   << "                  Revise dataset name (" << mcproccess 
   << ") or internet connection" << endl;
    return 0;
  }
  
  //Read dataset histogram...
#if (DEBUGPUWEIGHT > 1)
  PAF_DEBUG("PUWeight", "   + Looking for histogram...");
#endif
  
  fMC = (TH1D*) fds->Get("htemp")->Clone("PU_MC");
  if (!fMC) {
    cerr << "ERROR [PUWeight]: Could not find histogram for dataset " << mcproccess << "!"
   << endl;
    return 0;
  }
  fMC->SetDirectory(0);

  if (fMC->Integral() != 1) {
    PAF_INFO("PUWeight", "MC histogram is not normalized to 1! Normalizing...");
    fMC->Scale(1./fMC->Integral());
  }

  fds->Close();
  return fMC;
  
}


TH1D* PUWeight::LoadDataHistogram(float luminosity, const char* year) {

#ifdef DEBUGPUWEIGHT
  PAF_DEBUG("PUWeight", Form("Getting pileup for the %s pb-1 of data...", 
			     luminosity));
#endif
  
  TString dtfile;
  TFile* fdt = 0;
  if (luminosity > 0) {
    if (fIs3D)
      dtfile.Form("http://www.hep.uniovi.es/jfernan/PUhistos/Data%s/3D/PUdata_%.1f.root", 
      year, luminosity);
    else
      //dtfile.Form("http://www.hep.uniovi.es/jfernan/PUhistos/Data%s/PUdata_%.1f.root", 
      dtfile.Form("http://www.hep.uniovi.es/palencia/PUhistos/Data%s/PUdata_%.1f.root", 
      year, luminosity);

  
#if (DEBUGPUWEIGHT > 1)
    PAF_DEBUG("PUWeight",Form("   + Opening %s", dtfile));
#endif

    fdt = TFile::Open(dtfile);
    if (!fdt) {
      cerr << "NOTE [PUWeight]: Could not find file " << dtfile << "!"  << endl;
      cerr << "                 Trying default PU profile for data..." << endl;
    }
  }

  if (!fdt) {
    dtfile="http://www.hep.uniovi.es/jfernan/PUhistos/Data2011A/PUdata.root";

#if (DEBUGPUWEIGHT > 1)
    PAF_DEBUG("PUWeight", Form("   + Opening %s", dtfile));
#endif

    fdt = TFile::Open(dtfile);
    if (!fdt) {
      cerr << "ERROR [PUWeight]: Could not find default profile in \"" 
     << dtfile << "\"!"  << endl
     << "                  Is your internet connection working?" << endl;
      return 0;
    }
  }
  
  //Read data histogram...
  fData = (TH1D*) fdt->Get("pileup")->Clone("PU_Data");
  if (!fData) {
    cerr << "ERROR [PUWeight]: Could not find histogram for data!" << endl;
    return 0;
  }
  
  fData->SetDirectory(0);
  
  if (fData->Integral() != 1) {
    PAF_INFO("PUWeight", "Data histogram is not normalized to 1! Normalizing...");
    fData->Scale(1./fData->Integral());
  }

  fdt->Close();

  return fData;
}


TH1D* PUWeight::CalculateWeight() {
#ifdef DEBUGPUWEIGHT
  PAF_DEBUG("PUWeight",  "Calculating weights...");
#endif
  if (fData && fMC) {
    unsigned int nbins = fData->GetXaxis()->GetNbins();
    float xmin  = fData->GetXaxis()->GetXmin();
    float xmax  = fData->GetXaxis()->GetXmax();
    fWeight = new TH1D("PUWeight", "PU Weight", nbins, xmin, xmax);
    fWeight->SetDirectory(0);
    fWeight->Divide(fData, fMC);
  }
  else {
    cerr << "ERROR [PUWeight]: Something weird happened when trying to calculate the weights."
   << endl 
   << "                  I could not find the data and/or mc histograms!"
   << endl;
  }

#ifdef DEBUGPUWEIGHT
  PAF_DEBUG("PUWeight", "Done weight calculation...");
#endif
  return fWeight;
}

TH1D* PUWeight::IdealMCHistogram(EMCDistribution mcdistr) {
#ifdef DEBUGPUWEIGHT
  PAF_DEBUG("PUWeight", "Building ideal MC profile... ");
#endif
  unsigned int ndbins = 25;
  float xmin = -0.5;
  float xmax = 24.5;

  if (fData) {
    ndbins = fData->GetXaxis()->GetNbins();
    xmin   = fData->GetXaxis()->GetXmin();
    xmax   = fData->GetXaxis()->GetXmax();
  }


#if (DEBUGPUWEIGHT > 1)
  PAF_DEBUG("PUWeight", Form(" + Data histogram has %d bins - [%f, %f]",
			     ndbins, xmin, xmax));
#endif

  fMC = new TH1D("PU_MC", "PU^{MC} Weight", ndbins, xmin, xmax);
  double bins[50] = {
    00, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49
  };

  unsigned int nbins = 25;
  
  if (mcdistr == Spring11 || mcdistr == Summer113D) {
    double idealpu[] = {0.0698146584, 0.0698146584, 0.0698146584, 
      0.0698146584, 0.0698146584, 0.0698146584,
      0.0698146584, 0.0698146584, 0.0698146584,
      0.0698146584, 0.0698146584, 0.0630151648,
      0.0526654164, 0.0402754482, 0.0292988928,
      0.0194384503, 0.0122016783, 0.007207042,
      0.004003637,  0.0020278322, 0.0010739954,
      0.0004595759, 0.0002229748, 0.0001028162,
      4.58337152809607E-05};
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer11) {
    double idealpu[] = {
      0.104109,
      0.0703573,
      0.0698445,
      0.0698254,
      0.0697054,
      0.0697907,
      0.0696751,
      0.0694486,
      0.0680332,
      0.0651044,
      0.0598036,
      0.0527395,
      0.0439513,
      0.0352202,
      0.0266714,
      0.019411,
      0.0133974,
      0.00898536,
      0.0057516,
      0.00351493,
      0.00212087,
      0.00122891,
      0.00070592,
      0.000384744,
      0.000219377     
    };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer11InTime) {
    double idealpu[] = {
      0.14551,
      0.0644453,
      0.0696412,
      0.0700311,
      0.0694257,
      0.0685655,
      0.0670929,
      0.0646049,
      0.0609383,
      0.0564597,
      0.0508014,
      0.0445226,
      0.0378796,
      0.0314746,
      0.0254139,
      0.0200091,
      0.0154191,
      0.0116242,
      0.00846857,
      0.00614328,
      0.00426355,
      0.00300632,
      0.00203485,
      0.00133045,
      0.000893794
    };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer11ITSmear) {
    nbins = 35;
    double idealpu[] = {
      1.45346E-01,
      6.42802E-02,
      6.95255E-02,
      6.96747E-02,
      6.92955E-02,
      6.84997E-02,
      6.69528E-02,
      6.45515E-02,
      6.09865E-02,
      5.63323E-02,
      5.07322E-02,
      4.44681E-02,
      3.79205E-02,
      3.15131E-02,
      2.54220E-02,
      2.00184E-02,
      1.53776E-02,
      1.15387E-02,
      8.47608E-03,
      6.08715E-03,
      4.28255E-03,
      2.97185E-03,
      2.01918E-03,
      1.34490E-03,
      8.81587E-04,
      5.69954E-04,
      3.61493E-04,
      2.28692E-04,
      1.40791E-04,
      8.44606E-05,
      5.10204E-05,
      3.07802E-05,
      1.81401E-05,
      1.00201E-05,
      5.80004E-06
    };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer11True) {
    double idealpu[] = {
      0.104109,
      0.0703573,
      0.0698445,
      0.0698254,
      0.0697054,
      0.0697907,
      0.0696751,
      0.0694486,
      0.0680332,
      0.0651044,
      0.0598036,
      0.0527395,
      0.0439513,
      0.0352202,
      0.0266714,
      0.019411,
      0.0133974,
      0.00898536,
      0.0057516,
      0.00351493,
      0.00212087,
      0.00122891,
      0.00070592,
      0.000384744,
      0.000219377 
    };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Fall11) {
    nbins = 50;
    double idealpu[] = {
      0.003388501,
      0.010357558,
      0.024724258,
      0.042348605,
      0.058279812,
      0.068851751,
      0.072914824,
      0.071579609,
      0.066811668,
      0.060672356,
      0.054528356,
      0.04919354,
      0.044886042,
      0.041341896,
      0.0384679,
      0.035871463,
      0.03341952,
      0.030915649,
      0.028395374,
      0.025798107,
      0.023237445,
      0.020602754,
      0.0180688,
      0.015559693,
      0.013211063,
      0.010964293,
      0.008920993,
      0.007080504,
      0.005499239,
      0.004187022,
      0.003096474,
      0.002237361,
      0.001566428,
      0.001074149,
      0.000721755,
      0.000470838,
      0.00030268,
      0.000184665,
      0.000112883,
      6.74043E-05,
      3.82178E-05,
      2.22847E-05,
      1.20933E-05,
      6.96173E-06,
      3.4689E-06,
      1.96172E-06,
      8.49283E-07,
      5.02393E-07,
      2.15311E-07,
      9.56938E-08     
    };
    fMC->FillN(nbins, bins, idealpu);
  }

  else if (mcdistr == Fall11True){
     nbins =50;
     double idealpu[]= {

  0.00875488,
  0.0188779,
  0.0314413,
  0.0439477,
  0.053954,
  0.060907,
  0.0629538,
  0.0614203,
  0.0601845,
  0.0573943,
  0.0528803,
  0.0490987,
  0.0453003,
  0.041755,
  0.0387835,
  0.0356998,
  0.0329536,
  0.0305912,
  0.0279545,
  0.025328,
  0.0229131,
  0.0205869,
  0.0182821,
  0.0160806,
  0.0140798,
  0.0121316,
  0.0103717,
  0.00875817,
  0.00729464,
  0.00606236,
  0.00501249,
  0.00404522,
  0.00322428,
  0.00257927,
  0.0020242,
  0.00159807,
  0.001214,
  0.000921139,
  0.000675573,
  0.000528562,
  0.00039809,
  0.000290443,
  0.000210844,
  0.00014498,
  0.000113643,
  8.07592e-05,
  6.04485e-05,
  4.23623e-05,
  2.74678e-05,
  1.73125e-05
  //  1.39273e-05
     };
     fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer12){
    nbins =60;
    double idealpu[]= {
                                  2.344E-05,
                                  2.344E-05,
                                  2.344E-05,
                                  2.344E-05,
                                  4.687E-04,
                                  4.687E-04,
                                  7.032E-04,
                                  9.414E-04,
                                  1.234E-03,
                                  1.603E-03,
                                  2.464E-03,
                                  3.250E-03,
                                  5.021E-03,
                                  6.644E-03,
                                  8.502E-03,
                                  1.121E-02,
                                  1.518E-02,
                                  2.033E-02,
                                  2.608E-02,
                                  3.171E-02,
                                  3.667E-02,
                                  4.060E-02,
                                  4.338E-02,
                                  4.520E-02,
                                  4.641E-02,
                                  4.735E-02,
                                  4.816E-02,
                                  4.881E-02,
                                  4.917E-02,
                                  4.909E-02,
                                  4.842E-02,
                                  4.707E-02,
                                  4.501E-02,
                                  4.228E-02,
                                  3.896E-02,
                                  3.521E-02,
                                  3.118E-02,
                                  2.702E-02,
                                  2.287E-02,
                                  1.885E-02,
                                  1.508E-02,
                                  1.166E-02,
                                  8.673E-03,
                                  6.190E-03,
                                  4.222E-03,
                                  2.746E-03,
                                  1.698E-03,
                                  9.971E-04,
                                  5.549E-04,
                                  2.924E-04,
                                  1.457E-04,
                                  6.864E-05,
                                  3.054E-05,
                                  1.282E-05,
                                  5.081E-06,
                                  1.898E-06,
                                  6.688E-07,
                                  2.221E-07,
                                  6.947E-08,
                                  2.047E-08
    };
  fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer12_53X){
    nbins =60;
    double idealpu[]= {
       2.560E-06,
                         5.239E-06,
                         1.420E-05,
                         5.005E-05,
                         1.001E-04,
                         2.705E-04,
                         1.999E-03,
                         6.097E-03,
                         1.046E-02,
                         1.383E-02,
                         1.685E-02,
                         2.055E-02,
                         2.572E-02,
                         3.262E-02,
                         4.121E-02,
                         4.977E-02,
                         5.539E-02,
                         5.725E-02,
                         5.607E-02,
                         5.312E-02,
                         5.008E-02,
                         4.763E-02,
                         4.558E-02,
                         4.363E-02,
                         4.159E-02,
                         3.933E-02,
                         3.681E-02,
                         3.406E-02,
                         3.116E-02,
                         2.818E-02,
                         2.519E-02,
                         2.226E-02,
                         1.946E-02,
                         1.682E-02,
                         1.437E-02,
                         1.215E-02,
                         1.016E-02,
                         8.400E-03,
                         6.873E-03,
                         5.564E-03,
                         4.457E-03,
                         3.533E-03,
                         2.772E-03,
                         2.154E-03,
                         1.656E-03,
                         1.261E-03,
                         9.513E-04,
                         7.107E-04,
                         5.259E-04,
                         3.856E-04,
                         2.801E-04,
                         2.017E-04,
                         1.439E-04,
                         1.017E-04,
                         7.126E-05,
                         4.948E-05,
                         3.405E-05,
                         2.322E-05,
                         1.570E-05,
                         5.005E-06
  };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer2015_50ns_poisson){
    //https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/SimGeneral/MixingModule/python/mix_2015_50ns_Startup_PoissonOOTPU_cfi.py
    nbins = 53;
    double idealpu[]= {4.71E-09, 2.86E-06, 4.85E-06, 1.53E-05, 3.14E-05, 6.28E-05, 1.26E-04, 
                       3.93E-04, 1.42E-03, 6.13E-03, 1.40E-02, 2.18E-02, 2.94E-02, 4.00E-02, 
           5.31E-02, 6.53E-02, 7.64E-02, 8.42E-02, 8.43E-02, 7.68E-02, 6.64E-02, 
           5.69E-02, 4.94E-02, 4.35E-02, 3.84E-02, 3.37E-02, 2.92E-02, 2.49E-02, 
           2.10E-02, 1.74E-02, 1.43E-02, 1.16E-02, 9.33E-03, 7.41E-03, 5.81E-03, 
           4.49E-03, 3.43E-03, 2.58E-03, 1.91E-03, 1.39E-03, 1.00E-03, 7.09E-04, 
           4.93E-04, 3.38E-04, 2.28E-04, 1.51E-04, 9.83E-05, 6.29E-05, 3.96E-05, 
           2.45E-05, 1.49E-05, 4.71E-06, 2.36E-06
  };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Summer2015_25ns_poisson){
    //https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/SimGeneral/MixingModule/python/mix_2015_25ns_Startup_PoissonOOTPU_cfi.py
    nbins = 53;
    double idealpu[]= {4.8551E-07 , 1.74806E-06, 3.30868E-06, 1.62972E-05, 4.95667E-05, 0.000606966,
                       0.003307249, 0.010340741, 0.022852296, 0.041948781, 0.058609363, 0.067475755,
                       0.072817826, 0.075931405, 0.076782504, 0.076202319, 0.074502547, 0.072355135,
                       0.069642102, 0.064920999, 0.05725576 , 0.047289348, 0.036528446, 0.026376131,
                       0.017806872, 0.011249422, 0.006643385, 0.003662904, 0.001899681, 0.00095614 ,
                       0.00050028 , 0.000297353, 0.000208717, 0.000165856, 0.000139974, 0.000120481,
                       0.000103826, 8.88868E-05, 7.53323E-05, 6.30863E-05, 5.21356E-05, 4.24754E-05,
                       3.40876E-05, 2.69282E-05, 2.09267E-05, 1.5989E-05 , 4.8551E-06 , 2.42755E-06, 
           4.8551E-07 , 2.42755E-07, 1.21378E-07, 4.8551E-08
  };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Fall2015_25ns_matchData_poisson){
    //https://github.com/cms-sw/cmssw/blob/CMSSW_7_6_X/SimGeneral/MixingModule/python/mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi.py
    nbins = 51;
    double idealpu[]= {0.000108643, 0.000388957, 0.000332882, 0.00038397 , 0.000549167, 0.00105412 ,
                       0.00459007 , 0.0210314  , 0.0573688  , 0.103986   , 0.142369   , 0.157729   ,
           0.147685   , 0.121027   , 0.08855    , 0.0582866  , 0.0348526  , 0.019457   ,
           0.0107907  , 0.00654313 , 0.00463195 , 0.00370927 , 0.0031137  , 0.00261141 , 
           0.00215499 , 0.00174491 , 0.00138268 , 0.00106731 , 0.000798828, 0.00057785 ,
                       0.00040336 , 0.00027161 , 0.000176535, 0.00011092 , 6.75502e-05, 4.00323e-05, 
           2.32123e-05, 1.32585e-05, 7.51611e-06, 4.25902e-06, 2.42513e-06, 1.39077e-06, 
           8.02452e-07, 4.64159e-07, 2.67845e-07, 1.5344e-07 , 8.68966e-08, 4.84931e-08, 
           2.6606e-08 , 1.433e-08
  };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Spring2016_25ns_poisson_OOTPU){
    //https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/SimGeneral/MixingModule/python/mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi.py
    nbins = 51;
    double idealpu[]= {0.000829312873542, 0.00124276120498 , 0.00339329181587, 0.00408224735376,  0.00383036590008, 0.00659159288946 ,
           0.00816022734493 , 0.00943640833116 , 0.0137777376066 , 0.017059392038  ,  0.0213193035468 , 0.0247343174676  ,
           0.0280848773878  , 0.0323308476564  , 0.0370394341409,  0.0456917721191 ,  0.0558762890594 , 0.0576956187107  ,
           0.0625325287017  , 0.0591603758776  , 0.0656650815128 , 0.0678329011676 ,  0.0625142146389 , 0.0548068448797  ,
           0.0503893295063  , 0.040209818868   , 0.0374446988111 , 0.0299661572042 ,  0.0272024759921 , 0.0219328403791  ,
           0.0179586571619  , 0.0142926728247  , 0.00839941654725, 0.00522366397213,  0.00224457976761, 0.000779274977993,
           0.000197066585944, 7.16031761328e-05, 0.0             , 0.0             ,  0.0             , 0.0,
           0.0              , 0.0      , 0.0       , 0.0             ,  0.0             , 0.0,
           0.0    , 0.0
  };
    fMC->FillN(nbins, bins, idealpu);
  }
  else if (mcdistr == Moriond17MC_PoissonOOTPU){
    // https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/SimGeneral/MixingModule/python/mix_2016_25ns_Moriond17MC_PoissonOOTPU_cfi.py
    nbins = 76;
    double idealpu[]= {1.78653e-05 , 2.56602e-05 , 5.27857e-05 , 8.88954e-05 , 0.000109362 , 0.000140973 ,
                       0.000240998 , 0.00071209  , 0.00130121  , 0.00245255  , 0.00502589  , 0.00919534  ,
           0.0146697   , 0.0204126   , 0.0267586   , 0.0337697   , 0.0401478   , 0.0450159   ,
           0.0490577   , 0.0524855   , 0.0548159   , 0.0559937   , 0.0554468   , 0.0537687   ,
           0.0512055   , 0.0476713   , 0.0435312   , 0.0393107   , 0.0349812   , 0.0307413   , 
           0.0272425   , 0.0237115   , 0.0208329   , 0.0182459   , 0.0160712   , 0.0142498   ,
                       0.012804    , 0.011571    , 0.010547    , 0.00959489  , 0.00891718  , 0.00829292  ,
           0.0076195   , 0.0069806   , 0.0062025   , 0.00546581  , 0.00484127  , 0.00407168  ,
           0.00337681  , 0.00269893  , 0.00212473  , 0.00160208  , 0.00117884  , 0.000859662 ,
           0.000569085 , 0.000365431 , 0.000243565 , 0.00015688  , 9.88128e-05 , 6.53783e-05 ,
                       3.73924e-05 , 2.61382e-05 , 2.0307e-05  , 1.73032e-05 , 1.435e-05   , 1.36486e-05 ,
           1.35555e-05 , 1.37491e-05 , 1.34255e-05 , 1.33987e-05 , 1.34061e-05 , 1.34211e-05 ,
           1.34177e-05 , 1.32959e-05 , 1.33287e-05
         

      };
    fMC->FillN(nbins, bins, idealpu);
  }
  
  /*
  else if (mcdistr == Summer2015_50ns_nvtx){
    //cout << "2222"<< endl;
    nbins =53;
    double idealpu[] = {1.737496e-05, 8.371571e-05, 0.0004027832, 0.001220986 , 0.003097481 ,  
                        0.006373451 , 0.01168387  , 0.01895292  , 0.02792946  , 0.03808275  ,
                        0.04887892  , 0.05797708  , 0.0664608   , 0.07182018  , 0.07402365  ,
      0.07342974  , 0.07107622  , 0.06575317  , 0.06042695  , 0.05360807  ,
                        0.0468271   , 0.03932111  , 0.03337888  , 0.02729132  , 0.02271065  ,   
      0.01831005  , 0.01442122  , 0.01134585  , 0.008933888 , 0.006831518 ,
                        0.005150886 , 0.003994661 , 0.002941107 , 0.002001279 , 0.001554269 , 
      0.001142009 , 0.0008071458, 0.0005544192, 0.0003632946, 0.0002527267,
                        0.0001927041, 0.0001216247, 7.107938e-05, 6.318167e-05, 4.580671e-05, 
      2.369313e-05, 1.737496e-05, 1.263633e-05, 4.738625e-06, 1.579542e-06,
                        7.897709e-06, 3.159084e-06, 1.579542e-06 };
    fMC->FillN(nbins, bins, idealpu);
  }
  */
  
  return fMC;
}


// Build 3D histogram with weights
TH3D* PUWeight::CalculateWeight3D(float ScaleFactor) {
  if (fWeight3D)
    delete fWeight3D;

  fWeight3D = new TH3D("fWeight3D","3D weights",50,-.5,49.5,50,-.5,49.5,50,-.5,49.5 );

  //////////////
  // From http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/Utilities/src/Lumi3DReWeighting.cc?view=markup

  TH3D* DHist = new TH3D("DHist","3D weights",50,-.5,49.5,50,-.5,49.5,50,-.5,49.5 );
  TH3D* MHist = new TH3D("MHist","3D weights",50,-.5,49.5,50,-.5,49.5,50,-.5,49.5 );

  // arrays for storing number of interactions

  double MC_ints[50][50][50];
  double Data_ints[50][50][50];
  double Weight3D_[50][50][50];

  for (int i=0; i<50; i++) {
    for(int j=0; j<50; j++) {
      for(int k=0; k<50; k++) {
  MC_ints[i][j][k] = 0.;
  Data_ints[i][j][k] = 0.;
      }
    }
  }

  double factorial[50];
  double PowerSer[50];
  double base = 1.;

  factorial[0] = 1.;
  PowerSer[0]  = 1.;

  for (int i = 1; i<50; i++) {
    base = base*i;
    factorial[i] = base;
  }

  double x;
  double xweight;
  double probi, probj, probk;
  double Expval, mean;
  int xi;

  // Get entries for Data, MC, fill arrays:
  
  int NMCbin = fMC->GetNbinsX();
  
  for (int jbin=1;jbin<NMCbin+1;jbin++) {       
    x =  fMC->GetBinCenter(jbin);
    xweight = fMC->GetBinContent(jbin); //use as weight for matrix
    
    //for Summer 11, we have this int feature:
    xi = int(x);
    
    // Generate Poisson distribution for each value of the mean
    
    mean = double(xi);
    
    if(mean<0.) {
      cerr << " Your histogram generates MC luminosity values less than zero!"
     << " Please Check.  Terminating." << endl;
      return 0;
    }
    
    
    if(mean==0.){
      Expval = 1.;
    }
    else {
      Expval = exp(-1.*mean);
    }
    
    base = 1.;
    
    for (int i = 1; i<50; ++i) {
      base = base*mean;
      PowerSer[i] = base; // PowerSer is mean^i
    }
    
    // compute poisson probability for each Nvtx in weight matrix
    
    for (int i=0; i<50; i++) {
      probi = PowerSer[i]/factorial[i]*Expval;
      for(int j=0; j<50; j++) {
  probj = PowerSer[j]/factorial[j]*Expval;
  for(int k=0; k<50; k++) {
    probk = PowerSer[k]/factorial[k]*Expval;
    // joint probability is product of event weights multiplied by weight of input distribution bin
    MC_ints[i][j][k] = MC_ints[i][j][k]+probi*probj*probk*xweight;
  }
      }
    }
    
  }
  

  int NDatabin = fData->GetNbinsX();
  
  for (int jbin=1;jbin<NDatabin+1;jbin++) {       
    mean =  (fData->GetBinCenter(jbin))*ScaleFactor;
    xweight = fData->GetBinContent(jbin);
    
    // Generate poisson distribution for each value of the mean
    
    if(mean<0.) {
      cerr << "ERROR: Your histogram generates Data luminosity values less than zero!"
     << " Please Check.  Terminating." << std::endl;
      return 0;
    }
    
    if(mean==0.){
      Expval = 1.;
    }
    else {
      Expval = exp(-1.*mean);
    }
    
    base = 1.;
    
    for (int i = 1; i<50; ++i) {
      base = base*mean;
      PowerSer[i] = base;
    }
    
    // compute poisson probability for each Nvtx in weight matrix                                                                  
    
    for (int i=0; i<50; i++) {
      probi = PowerSer[i]/factorial[i]*Expval;
      for(int j=0; j<50; j++) {
  probj = PowerSer[j]/factorial[j]*Expval;
  for(int k=0; k<50; k++) {
    probk = PowerSer[k]/factorial[k]*Expval;
    // joint probability is product of event weights multiplied by weight of input distribution bin
    Data_ints[i][j][k] = Data_ints[i][j][k]+probi*probj*probk*xweight;
  }
      }
    }
    
  }

  
  for (int i=0; i<50; i++) {  
    //if(i<5) std::cout << "i = " << i << std::endl;
    for(int j=0; j<50; j++) {
      for(int k=0; k<50; k++) {
  if( (MC_ints[i][j][k])>0.) {
    Weight3D_[i][j][k]  =  Data_ints[i][j][k]/MC_ints[i][j][k];
  }
  else {
    Weight3D_[i][j][k]  = 0.;
  }
  fWeight3D->SetBinContent( i+1,j+1,k+1,Weight3D_[i][j][k] );
  DHist->SetBinContent( i+1,j+1,k+1,Data_ints[i][j][k] );
  MHist->SetBinContent( i+1,j+1,k+1,MC_ints[i][j][k] );
  //  if(i<5 && j<5 && k<5) std::cout << Weight3D_[i][j][k] << " " ;
      }
      //      if(i<5 && j<5) std::cout << std::endl;
    }
  }
  
  
  //
  /////////////


  delete DHist, MHist;
  return fWeight3D;
}
