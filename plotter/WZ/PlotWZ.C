#include "PlotWZ.h"

void PlotWZ(TString vName, TString varLabel, TString treeName, TString Selection, Float_t Min, Float_t Max, Int_t NBins){
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


	//WZ
	 sample WZ = sample("WZTo3LNu", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "signal", theSamples, PlottingSamples, kYellow, "WZ","WZ");

	//DY
	 sample DY1 = sample("DYJetsToLL_M50_MLM", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "DY","DY");
	 sample DY2 = sample("DYJetsToLL_M5to50_MLM", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "DY","DY");

	//WJets
	// sample WJets = sample("WJetsToLNu_MLM", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "WJets","WJets");

	//WW
	 sample WW1 = sample("WWTo2L2Nu", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "WW","WW");
	 sample WW2 = sample("WpWpJJ", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "WW","WW");
	 sample WW3 = sample("WWTo2L2Nu_DoubleScat", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "WW","WW");

	//VVV
	 sample VVV1 = sample("WWW", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "VVV","VVV");
	 sample VVV2 = sample("WWZ", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "VVV","VVV");
	 sample VVV3 = sample("WZZ", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "VVV","VVV");
	 sample VVV4 = sample("ZZZ", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "VVV","VVV");
	 sample VVV5 = sample("VHToNonbb_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "VVV","VVV");

	//XG
	 sample XG1 = sample("TGJets", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");
	 sample XG2 = sample("TTGJets", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");
	 sample XG3 = sample("WGToLNuG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");
	 sample XG4 = sample("ZGTo2LG", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");
	 sample XG5 = sample("WWG_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");//
	 sample XG6 = sample("WZG_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "XG","XG");//

	//ttX
	 sample ttX1 = sample("TTZToLL_M1to10", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ttX","ttX");
	 sample ttX2 = sample("TTZToLLNuNu", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ttX","ttX");
	 sample ttX3 = sample("TTWToLNu", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ttX","ttX");
	 sample ttX4 = sample("TTTT", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ttX","ttX");
	 sample ttX5 = sample("TTHNonbb", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ttX","ttX");

	//tt
	 sample tt = sample("TTbar_Powheg", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "tt","tt");

	//t
	 sample t1 = sample("TW_noFullyHadr", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "t","t");
	 sample t2 = sample("T_tch", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "t","t");
	 sample t3 = sample("Tbar_tch", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "t","t");
	 sample t4 = sample("TbarW_noFullyHadr", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "t","t");
	 sample t5 = sample("TToLeptons_sch_amcatnlo", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "t","t");
	
	//tZq
//	 sample tZq = sample("tZq_ll", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "tZq","tZq");
	//ZZ
	 sample ZZ = sample("ZZTo4L", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kYellow, "ZZ","ZZ");

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
	bool DrawLog = true;
	Int_t TotalBins = NBins;
	Float_t MinValue = Min;
	Float_t MaxValue = Max;
  Float_t TLep_Pt[3];
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
  hMC    =    new   TH1F( "hMC",           "hMC",           TotalBins, MinValue, MaxValue );
  hWZ    =    new   TH1F( "h_WZ",           "h_WZ",           TotalBins, MinValue, MaxValue );
	hDY    =    new   TH1F( "h_DY",           "h_DY",           TotalBins, MinValue, MaxValue );
  hZZ    =    new   TH1F( "h_ZZ",           "h_ZZ",           TotalBins, MinValue, MaxValue );
	htZq   =    new   TH1F( "h_tZq",           "h_tZq",           TotalBins, MinValue, MaxValue );
  httX   =    new   TH1F( "h_ttX",           "h_ttX",           TotalBins, MinValue, MaxValue );
	htt    =    new   TH1F( "h_tt",           "h_tt",           TotalBins, MinValue, MaxValue );
  hWJets =    new   TH1F( "h_WJets",           "h_WJets",           TotalBins, MinValue, MaxValue );
	hWW    =    new   TH1F( "h_WW",           "h_WW",           TotalBins, MinValue, MaxValue );
  hVVV   =    new   TH1F( "h_VVV",           "h_VVV",           TotalBins, MinValue, MaxValue );
	hXg    =    new   TH1F( "h_Xg",           "h_Xg",           TotalBins, MinValue, MaxValue );
  ht     =    new   TH1F( "h_t",           "h_t",           TotalBins, MinValue, MaxValue );
/*  hdata  =    new   TH1F( "hdata",           "hdata", 13, xbins);
  hWZ    =    new   TH1F( "h_WZ",           "h_WZ", 13, xbins);
	hDY    =    new   TH1F( "h_DY",           "h_DY", 13, xbins);
  hZZ    =    new   TH1F( "h_ZZ",           "h_ZZ", 13, xbins);
	htZq   =    new   TH1F( "h_tZq",           "h_tZq", 13, xbins);
  httX   =    new   TH1F( "h_ttX",           "h_ttX", 13, xbins);
	htt    =    new   TH1F( "h_tt",           "h_tt", 13, xbins);
  hWJets =    new   TH1F( "h_WJets",           "h_WJets", 13, xbins);
	hWW    =    new   TH1F( "h_WW",           "h_WW", 13, xbins);
  hVVV   =    new   TH1F( "h_VVV",           "h_VVV", 13, xbins);
	hXg    =    new   TH1F( "h_Xg",           "h_Xg", 13, xbins);
  ht     =    new   TH1F( "h_t",           "h_t", 13, xbins);*/

  std::cout << "HISTOGRAMS CREATED_________________________________" << std::endl;
	//Loop in samples
  Long64_t N1 = 2*3*5*11*13*17*19;
  Long64_t NCut = 37;
	for (std::vector<sample>::iterator it = PlottingSamples.begin(); it != PlottingSamples.end(); it++) {
        std::cout << it-> getdataCard() << std::endl;
				TString filename =  it->path();
				TFile f(filename);
				TTree* theTree = (TTree *) f.Get(treeName);
				//TTree* theTreeMVA = (TTree *) f.Get("nolepMVA");
				theTree->SetBranchAddress(varrr, &TPVariable);
				theTree->SetBranchAddress("TWeight", &TWeight);
				theTree->SetBranchAddress(Selection  , &TSelec);
        theTree->SetBranchAddress("TLep_Eta"  , &TLep_Pt);
        theTree->SetBranchAddress("TChannel"  , &TChannel);
        theTree->SetBranchAddress("TCutsId"  , &TCutsId);
				TH1F* h =  new TH1F( "h",         "h",         TotalBins, MinValue, MaxValue );
        //TH1F* h =  new TH1F( "h",         "h", 13, xbins);
				for (int i = 0; i < theTree->GetEntries(); i++){
					theTree->GetEntry(i);
					//theTreeMVA->GetEntry(i);
					if (TSelec != 0 && TLep_Pt[0] > 20 && TLep_Pt[1] > 20 && TLep_Pt[2] > 20 ){h->Fill(TPVariable, TWeight);};
          //h->Fill(TPVariable, TWeight);
				};
				h->SetName(it->getPlotName());
				h->SetDirectory(0);

				if (it->getdataCard() == TString("data")){
					hdata->Add(h);
				}
        else{
          h->Scale(Lumf);
          hMC->Add(h);
        }
				f.Close();
				//Signal
				if (it->getdataCard() == TString("WZ")){
					hWZ->Add(h);
				}
				//Background
				else if (it->getdataCard() == TString("DY")){
					hDY->Add(h);
				}
				else if (it->getdataCard() == TString("WJets")){
					hWJets->Add(h);
				}
				else if (it->getdataCard() == TString("WW")){
					hWW->Add(h);
				}
				else if (it->getdataCard() == TString("VVV")){
					hVVV->Add(h);
				}
				else if (it->getdataCard() == TString("XG")){
					hXg->Add(h);
				}
				else if (it->getdataCard() == TString("ttX")){
					httX->Add(h);
				}
				else if (it->getdataCard() == TString("tt")){
					htt->Add(h);
				}
				else if (it->getdataCard() == TString("t")){
					ht->Add(h);
				}
				else if (it->getdataCard() == TString("tZq")){
					htZq->Add(h);
				}
				else if (it->getdataCard() == TString("ZZ")){
					hZZ->Add(h);
				}
				delete h;
	}
	//Plotting

  hWW->SetFillColor(kBlue + 10);

	hs->Add(hWW);

  hVVV->SetFillColor(kGreen-9);

	hs->Add(hVVV);

  ht->SetFillColor(kGreen + 5);

	hs->Add(ht);

  hWJets->SetFillColor(kOrange-3);

	hs->Add(hWJets);

  hXg->SetFillColor(kViolet+2);

	hs->Add(hXg);

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


	l->AddEntry(hWZ, "WZ", "f");
	l->AddEntry(hZZ, "ZZ", "f");
	l->AddEntry(htZq, "tZq", "f");
	l->AddEntry(hDY, "DY", "f");
	l->AddEntry(httX, "tt+X", "f");
	l->AddEntry(htt, "tt", "f");
	l->AddEntry(hXg, "X + #gamma", "f");
	l->AddEntry(hWJets, "W + Jets", "f");
	l->AddEntry(ht, "Single t", "f");
	l->AddEntry(hVVV, "VVV/VH", "f");
	l->AddEntry(hWW, "WW", "f");


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


  hZZ->Sumw2();
	std::cout << "\n ZZ Integral: " << hZZ->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hZZ->Integral();
  yields_err += err*err;
  htZq->Sumw2();	
  std::cout << "\n tZq Integral: " << htZq->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += htZq->Integral();
  yields_err += err*err;
  httX->Sumw2();	
  std::cout << "\n ttX Integral: " << httX->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += httX->Integral();
  yields_err += err*err;
  hXg->Sumw2();	
  std::cout << "\n Xg Integral: " << hXg->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hXg->Integral();
  yields_err += err*err;
  hWJets->Sumw2();	
  std::cout << "\n WJets Integral: " << hWJets->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hWJets->Integral();
  yields_err += err*err;
  ht->Sumw2();	
  std::cout << "\n t Integral: " << ht->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += ht->Integral();
  yields_err += err*err;
  hVVV->Sumw2();	
  std::cout << "\n VVV Integral: " << hVVV->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hVVV->Integral();
  yields_err += err*err;
  hWW->Sumw2();	
  std::cout << "\n WW Integral: " << hWW->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hWW->Integral();
  yields_err += err*err;



	std::cout << "\n Data Integral: " << hdata->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  hDY->Sumw2();	
  std::cout << "\n DY Integral: " << hDY->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hDY->Integral();
  yields_err += err*err;
  htt->Sumw2();
	std::cout << "\n tt Integral: " << htt->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += htt->Integral();
  yields_err += err*err;
	std::cout << "\n BKG  Integral: " << yields_tot << "," << TMath::Sqrt(yields_err) << "\n";
  float B = yields_tot;
  float dB = TMath::Sqrt(yields_err);  
  hWZ->Sumw2();
	std::cout << "\n WZ Integral: " << hWZ->IntegralAndError(0,TotalBins, err)<< "," << err << "\n";
  yields_tot += hWZ->Integral();
  yields_err += err*err;
  std::cout << "\n S/B: " << hWZ->Integral()/B << " , " << dB*hWZ->Integral()/(B*B) + err/B << "\n";
	std::cout << "\n SM  Integral: " << yields_tot << "," << TMath::Sqrt(yields_err) << "\n";  

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

	//Quotient DATA/BG plot
	c1->cd(2);
	//gStyle->SetOptTitle(0);
	c1->GetPad(2)->SetPad(.005, .005, .995, 0.2475);
  gStyle->SetOptStat(0);
	quo = ((*hdata)/(*hMC));
	quo.SetMaximum(1.5);
	quo.SetMinimum(0.5);
	quo.SetTitle("");
	quo.GetYaxis()->SetTitle("Data/BG");
	quo.GetXaxis()->SetTitle(varName);
	quo.GetYaxis()->SetLabelSize(.10);
	quo.GetYaxis()->SetTitleSize(.10);
	quo.GetYaxis()->SetTitleOffset(.35);
	quo.GetXaxis()->SetTitleSize(.20);
	quo.GetYaxis()->SetNdivisions(510);

	quo.Draw();

	c1->Print("./plotsConvs/" + Selection + "/" + varrr + "_"  + NCut  + "_"+ treeName +  ".eps");
	c1->Print("./plotsConvs/" + Selection + "/" + varrr + "_"  + NCut  + "_"+ treeName + ".pdf");
	c1->Print("./plotsConvs/" + Selection + "/" + varrr + "_"  + NCut  + "_"+ treeName + ".png");
	
/*   TString savepath = "/mnt_pool/fanae105/user/carlosec/BDT/TMVA-v4.2.0/test/results";
   TFile *target  = new TFile( savepath + "/"+ methodName + ".root","RECREATE" );
   hsignal ->Write();
	hs -> Write();

   target->Close();
*/
}
