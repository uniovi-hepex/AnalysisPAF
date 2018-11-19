#include "PlotWZ.h"

void PlotWZConvs(TString vName, TString varLabel, TString treeName, TString Selection, Float_t Min, Float_t Max, Int_t NBins){
	// Path to base trees--Outdated
	//const TString dir = "/mnt_pool/fanae105/user/carlosec/BDT/StopTOP/feb03/";
	//const TString oPath = "./";
	
	// Method identifier and path

  std::cout << "START_________________________________" << std::endl;
	//WZ
	 sample dataMuMu = sample("DoubleEG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");
	 sample dataElEl = sample("DoubleMuon", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");
	 sample dataElMu = sample("MuonEG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");
 	 sample dataMu   = sample("SingleMuon", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");
	 sample dataEl   = sample("SingleElec", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "data", theSamples, PlottingSamples, kBlack, "data","data");

	//XG
	 sample XG1 = sample("TGJets", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG1","XG1");
	 sample XG2 = sample("TTGJets", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG2","XG2");
	 sample XG3 = sample("WGToLNuG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG3","XG3");
	 sample XG4 = sample("ZGTo2LG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG4","XG4");
	 sample XG5 = sample("WWG_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG5","XG5");//
	 sample XG6 = sample("WZG_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG6","XG6");//


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
	c1->Divide(1,1);
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
	bool DrawLog = true;
	Int_t TotalBins = NBins;
	Float_t MinValue = Min;
	Float_t MaxValue = Max;
  Float_t TLep_Eta[3];
  Long64_t TCutsId[3];
	float MaximunY = 10000;
	float MinimunY = 10;

  std::cout << "SETTING HISTOS_________________________________" << std::endl;
	//What to plot
	TString varrr = vName;
	TString varName = varLabel;

	// Weight and cut variables
	float TPVariable;
	float TWeight;
  Int_t TChannel;
	char TSelec;
  Float_t xbins[] = {0,100,200,300,400,500,600,700,800,900,1000,1200,1500,2000};
  std::cout << "CREATING HISTOGRAMS_________________________________" << std::endl;
	//Creating histos
  hdata  =    new   TH1F( "hdata",           "hdata",           TotalBins, MinValue, MaxValue );
  hWZ    =    new   TH1F( "h_WZ",           "h_WZ",           TotalBins, MinValue, MaxValue );
	hDY    =    new   TH1F( "h_DY",           "h_DY",           TotalBins, MinValue, MaxValue );
  hZZ    =    new   TH1F( "h_ZZ",           "h_ZZ",           TotalBins, MinValue, MaxValue );
	htZq   =    new   TH1F( "h_tZq",           "h_tZq",           TotalBins, MinValue, MaxValue );
  httX   =    new   TH1F( "h_ttX",           "h_ttX",           TotalBins, MinValue, MaxValue );
	htt    =    new   TH1F( "h_tt",           "h_tt",           TotalBins, MinValue, MaxValue );

  std::cout << "HISTOGRAMS CREATED_________________________________" << std::endl;
	//Loop in samples
	for (std::vector<sample>::iterator it = PlottingSamples.begin(); it != PlottingSamples.end(); it++) {
        Long64_t cutN = 37;
        std::cout << it-> getdataCard() << std::endl;
				TString filename =  it->path();
				TFile f(filename);
				TTree* theTree = (TTree *) f.Get(treeName);
				theTree->SetBranchAddress(varrr, &TPVariable);
				theTree->SetBranchAddress("TWeight", &TWeight);
				theTree->SetBranchAddress(Selection  , &TSelec);
        theTree->SetBranchAddress("TCutsId"  , &TCutsId);
        theTree->SetBranchAddress("TChannel"  , &TChannel);
				TH1F* h =  new TH1F( "h",         "h",         TotalBins, MinValue, MaxValue );
				for (int i = 0; i < theTree->GetEntries(); i++){
					theTree->GetEntry(i);
					if (TSelec != 0 && ((TCutsId[0]< 0) || (TCutsId[0]%cutN == 0)) && ((TCutsId[1]< 0) || (TCutsId[1]%cutN == 0)) && ((TCutsId[2]< 0) || (TCutsId[2]%cutN == 0))){h->Fill(TPVariable, TWeight);};
				};
				h->SetName(it->getPlotName());
				h->SetDirectory(0);

				if (it->getdataCard() == TString("data")){
					hdata->Add(h);
				}
        else{
          h->Scale(Lumf);
        }
				f.Close();
				//Signal
				if (it->getdataCard() == TString("XG1")){
					hWZ->Add(h);
				}
				//Background
				else if (it->getdataCard() == TString("XG2")){
					hDY->Add(h);
				}
				else if (it->getdataCard() == TString("XG3")){
					hZZ->Add(h);
				}
				else if (it->getdataCard() == TString("XG4")){
					htZq->Add(h);
				}
				else if (it->getdataCard() == TString("XG5")){
					httX->Add(h);
				}
				else if (it->getdataCard() == TString("XG6")){
					htt->Add(h);
				}
				delete h;
	}
	//Plotting

  htt->SetFillColor(kRed);

	hs->Add(htt);

  httX->SetFillColor(kRed-10);

	hs->Add(httX);
	
  hDY->SetFillColor(kOrange);

	hs->Add(hDY);


  htZq->SetFillColor(kGreen);

	hs->Add(htZq);

  hZZ->SetFillColor(kCyan-5);

	hs->Add(hZZ);

  hWZ->SetFillColor(kYellow);
	hs->Add(hWZ);


	l->AddEntry(hWZ, "tG", "f");
	l->AddEntry(hZZ, "WG", "f");
	l->AddEntry(htZq, "ZG", "f");
	l->AddEntry(hDY, "ttG", "f");
	l->AddEntry(httX, "WWG", "f");
	l->AddEntry(htt, "WZG", "f");


  hs->Draw("hist");
	hs->GetYaxis()->SetTitle("Events");
	hs->GetXaxis()->SetTitle(varName);
	hs->GetYaxis()->SetTitleOffset(1.35);
	hs->SetMaximum(MaximunY);
	hs->SetMinimum(MinimunY);

  l->AddEntry(hdata, "Data (All)", "p");
  hdata->Sumw2();
	hdata->Draw("psame");	

	l->Draw("same");
  Double_t  err;
  Double_t  yields_tot = 0;
  Double_t  yields_err = 0;

	std::cout << "\n Data Integral: " << hdata->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";


  hWZ->Sumw2();
	std::cout << "\n tG Integral: " << hWZ->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hWZ->Integral();
  yields_err += err*err;


  hDY->Sumw2();	
  std::cout << "\n ttG Integral: " << hDY->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hDY->Integral();
  yields_err += err*err;


  hZZ->Sumw2();
	std::cout << "\n WG Integral: " << hZZ->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hZZ->Integral();
  yields_err += err*err;

  htZq->Sumw2();	
  std::cout << "\n ZG Integral: " << htZq->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += htZq->Integral();
  yields_err += err*err;
  httX->Sumw2();	

  std::cout << "\n WWG Integral: " << httX->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += httX->Integral();
  yields_err += err*err;

  htt->Sumw2();

	std::cout << "\n WZG Integral: " << htt->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += htt->Integral();
  yields_err += err*err;




  hDY->Sumw2();	
	std::cout << "\n XG  Integral: " << yields_tot << "," << TMath::Sqrt(yields_err) << "\n";
/*
  TLatex * texlumi = new TLatex(-20.,50., Form("%2.1f fb^{-1}, #sqrt{s} = 13 TeV", Lumf/1000.));
  texlumi->SetNDC();
  texlumi->SetTextAlign(12);
  texlumi->SetX(0.62);
  texlumi->SetY(0.92);
  texlumi->SetTextFont(42);
  texlumi->SetTextSize(0.035);
  texlumi->SetTextSizePixels(12);
  texlumi->Draw();  

  TLatex * texcms = new TLatex(0.,0., "CMS");
  texcms->SetNDC();
  texcms->SetTextAlign(12);
  texcms->SetX(0.12);
  texcms->SetY(0.93);
  //texcms->SetTextFont(42);
  texcms->SetTextSize(0.06);
  texcms->SetTextSizePixels(13);
  texcms->Draw();

  TLatex * texPrelim  = new TLatex(0.,0., "Preliminary"); 
  texPrelim->SetNDC();
  texPrelim->SetTextAlign(12);
  texPrelim->SetX(0.245);
  texPrelim->SetY(0.925);
  texPrelim->SetTextFont(52);
  texPrelim->SetTextSize(0.05);
  texPrelim->SetTextSizePixels(15);
  texPrelim->Draw();
*/

/*	c1->Print("./plots/" + Selection + "/" + varrr + "_"  + "_"+ treeName +  ".eps");
	c1->Print("./plots/" + Selection + "/" + varrr + "_"  + "_"+ treeName + ".pdf");
	c1->Print("./plots/" + Selection + "/" + varrr + "_"  + "_"+ treeName + ".png");
	*/
/*   TString savepath = "/mnt_pool/fanae105/user/carlosec/BDT/TMVA-v4.2.0/test/results";
   TFile *target  = new TFile( savepath + "/"+ methodName + ".root","RECREATE" );
   hsignal ->Write();
	hs -> Write();

   target->Close();
*/
}
