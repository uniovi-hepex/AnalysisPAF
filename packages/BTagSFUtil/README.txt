/*************************************************************

Class Usage:

 - BTagSFUtil *BTagSF = new BTagSFUtil(MeasurementType, BTagAlgorithm, OperatingPoint, SystematicIndex);
   
      Where: MeasurementType is the type of measurement you want to use:
                             "mujets" -> combined b-jets SFs from methods based on QCD samples, recommended for dileptonic ttbar precision measurements
                             "comb"   -> combined b-jets SFs from QCD and ttbar datasets, generally recommended for b/c-jets except for the case above
                             "incl"   -> SFs for light jets
             BTagAlgorithm is the btag algorithm you want to use, ex: "CSVv2"
             OperatingPoint can be "Loose", "Medium" or "Tight"
             SystematicIndex +1 (-1) to scale up (down) the b-jets SF according to they total uncertainty
                             +3 (-3) to scale up (down) the light-jets SF according to they total uncertainty
                             Remember that these two uncertainties have to be treated as independent!

 - When running over FastSim MC, the dataset has to be specified to retrive special correction factors:
  
      BTagSFUtil *BTagSF = new BTagSFUtil(MeasurementType, BTagAlgorithm, OperatingPoint, SystematicIndex, FastSimDataset);
 
         Where the supported FastSim datasets for 8 TeV are T1bbbb, T1tttt, T2, T2bb, T2bwm T2tt and T3w
         SystematicIndex for FastSim SFs follow the same code as above +/-10 
	     e.g. to scale down the FastSim SF for light-jets by its uncertainty, set SystematicIndex to -13
             Note that for FastSim independent SFs for c-jets are used, with SystematicIndex +/-12
             Important: FastSim SF uncertainties should be treated as independet from standard SF ones
   
 - To get the SF for a given jet:

      GetJetSF(JetDiscriminant, JetFlavor, JetPt, JetEta);

 - For applying the SF to a jet according to the "flip a coin" method:

      IsTagged(JetDiscriminant, JetFlavor, JetPt, JetEta, Seed)

         Where: JetDiscriminant is the value for the discriminant of your choice for the jet to which the SF has being applied
         Seed initializes the random number generator for each jet. It is currently fixed as [event number] + [jet pT] + [systematic variation index] for reproducibility.
 
      Important note: this method to apply the SFs is designed in order to make a minimal use of absolute b-tag efficiencies. Nevertheless, 
      when the SFs are larger than 1, those efficiencies must be computed in the very MC samples on which the SFs have being applied. The 
      BTagEfficienciesTTbarSummer12.C file provide the efficiencies for a ttbar Summer12 samples only for example purposes only.

*************************************************************/
