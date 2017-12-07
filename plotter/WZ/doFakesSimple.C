#include "PlotWZ.h"

void doFakesSimple(){
  TString treeName = "medium";
  Int_t TotalBins = 10;
  Float_t MinValue = 0;
  Float_t MaxValue = 300;
	sample dataMuMu = sample("DoubleMuon", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");
    
  std::cout << "LOADED_________________________________" << std::endl;
	//Initialize
  gStyle->SetOptStat(111111);
	int NLeg = PlottingSamples.size();
  std::cout << "SETTING LEGEND_________________________________" << std::endl;
  TLegend *l = new TLegend(0.85,0.88,0.65,0.9-0.075*4);
  l->SetFillStyle(1001);
  l->SetFillColor(kWhite);
  l->SetLineColor(kWhite);
  l->SetLineWidth(2);
  std::cout << "SETTING CANVAS_________________________________" << std::endl;
  TCanvas *c1 = new TCanvas("c","c",600,750);
  std::cout << "DIVIDE CANVAS_________________________________" << std::endl;
	c1->Divide(1,2);
  std::cout << "CD CANVAS_________________________________" << std::endl;
	c1->cd(1);
  std::cout << "CANVAS SET_________________________________" << std::endl;
	c1->GetPad(1)->SetLogy(true);
	c1->GetPad(1)->SetPad(.005, .2525, .995, .995);
  std::cout << "PAD SET_________________________________" << std::endl;
	THStack * hs = new THStack("hs","");
  std::cout << "STACK SET_________________________________" << std::endl;
	// Set lumi
	float Lumf = 35851; // 1pb⁻¹ = 1
	
	//Plotting Variables
  Float_t TLep_Pt[3];

	float MaximunY = 10000;
	float MinimunY = 10;

  std::cout << "SETTING HISTOS_________________________________" << std::endl;
	//What to plot
	TString varrr = "TMET";
	TString varName = "E_T^{miss}";

	// Weight and cut variables
	float TPVariable;
	float TWeight;
  Int_t TNTight;

  float TLep_PtZ1;
  float TLep_EtaZ1;
  Float_t TLep_IsTightZ1;

  float TLep_PtZ2;
  float TLep_EtaZ2;
  Float_t TLep_IsTightZ2;

  float TLep_PtW;
  float TLep_EtaW;
  Float_t TLep_IsTightW;

  Int_t TChannel;
	char TSelec;

	//Creating histos
	TH1F* hData  =  new TH1F( "hd",         "hd",         TotalBins, MinValue, MaxValue );
	TH1F* hFakes =  new TH1F( "hf",         "hf",         TotalBins, MinValue, MaxValue );

  std::cout << "HISTOGRAMS CREATED_________________________________" << std::endl;
	//Loop in samples

	for (std::vector<sample>::iterator it = PlottingSamples.begin(); it != PlottingSamples.end(); it++) {
        std::cout << it-> getdataCard() << std::endl;
				TString filename =  it->path();
        TFile g("/nfs/fanae/user/carlosec/AnalysisPAF/plotter/FRmaps/fakerate_light_EWKino_Moriond2017_36p5fb.root");
        TH2F* hFR = (TH2F *) g.Get("FR_susy_wpM_el_data_comb");

				TFile f(filename);
				TTree* theTree = (TTree *) f.Get(treeName);
				//TTree* theTreeMVA = (TTree *) f.Get("nolepMVA");
				theTree->SetBranchAddress(varrr, &TPVariable);
				theTree->SetBranchAddress("TWeight", &TWeight);
				theTree->SetBranchAddress("TIsSR"  , &TSelec);
				theTree->SetBranchAddress("TNTightLeps"  , &TNTight);
				theTree->SetBranchAddress("TLep_PtZ1"  , &TLep_PtZ1);
				theTree->SetBranchAddress("TLep_EtaZ1"  , &TLep_EtaZ1);
				theTree->SetBranchAddress("TLep_IsTightZ1"  , &TLep_IsTightZ1);
				theTree->SetBranchAddress("TLep_PtZ2"  , &TLep_PtZ2);
				theTree->SetBranchAddress("TLep_EtaZ2"  , &TLep_EtaZ2);
				theTree->SetBranchAddress("TLep_IsTightZ2"  , &TLep_IsTightZ2);
				theTree->SetBranchAddress("TLep_PtW"  , &TLep_PtW);
				theTree->SetBranchAddress("TLep_EtaW"  , &TLep_EtaW);
				theTree->SetBranchAddress("TLep_IsTightW"  , &TLep_IsTightW);


        //TH1F* h =  new TH1F( "h",         "h", 13, xbins);
				for (int i = 0; i < theTree->GetEntries(); i++){
					theTree->GetEntry(i);
					if (TSelec != 0 && TNTight == 3){hData->Fill(TPVariable, TWeight);}
          else if (TSelec != 0 && TNTight == 2){
            Float_t newWeight = 0;
            if (!TLep_IsTightZ1){
              newWeight = hFR->GetBinContent(hFR->FindBin(TLep_PtZ1*,TMath::Abs(TLep_EtaZ1)));
              std::cout << "Z1, " << TLep_PtZ1 << ", " << TLep_EtaZ1 << std::endl;
            }
            if (!TLep_IsTightZ2){
              newWeight = hFR->GetBinContent(hFR->FindBin(TLep_PtZ2,TMath::Abs(TLep_EtaZ2)));
              std::cout << "Z2, " << TLep_PtZ2 << ", " << TLep_EtaZ2 << std::endl;              
            }
            if (!TLep_IsTightW){
              newWeight = hFR->GetBinContent(hFR->FindBin(TLep_PtW,TMath::Abs(TLep_EtaW)));
              std::cout << "W, " << TLep_PtW << ", " << TLep_EtaW << std::endl;    
            }
            std::cout << newWeight <<std::endl;
            newWeight =  newWeight/(1-newWeight);
            hFakes->Fill(TPVariable, newWeight*TWeight);
          }
				};
        f.Close();
        g.Close();
	}

  std::cout << hFakes->Integral() << "," << hData->Integral();
}
