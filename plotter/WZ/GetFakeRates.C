#include "DrawPlotsClosure.h"

void GetFakeRates(TString what){
  // Path to base trees--Outdated
  //const TString dir = "/mnt_pool/fanae105/user/carlosec/BDT/StopTOP/feb03/";
  //const TString oPath = "./";
  
  // Method identifier and path

  std::cout << "START_________________________________" << std::endl;

  //DY
  if (what == "DY"){
    sample DY1 = sample("DYJetsToLL_M50_MLM_0", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY2 = sample("DYJetsToLL_M50_MLM_1", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY3 = sample("DYJetsToLL_M50_MLM_2", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY4 = sample("DYJetsToLL_M50_MLM_3", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY5 = sample("DYJetsToLL_M50_MLM_4", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY6 = sample("DYJetsToLL_M50_MLM_5", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY7 = sample("DYJetsToLL_M50_MLM_6", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY8 = sample("DYJetsToLL_M50_MLM_7", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample DY9 = sample("DYJetsToLL_M5to50_MLM", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
/*    sample tZq0 = sample("tZq_ll_0", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample tZq1 = sample("tZq_ll_1", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample tZq2 = sample("tZq_ll_2", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");
    sample tZq3 = sample("tZq_ll_3", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "DY","DY");*/
 

  }
  else if (what == "TT"){
    sample TT = sample("TTbar_Powheg", "/mnt_pool/ciencias_users/user/carlosec/AnalysisPAF/WZ_temp/", "Tree_", ".root" , "back", theSamples, PlottingSamples, kRed, "tt","tt");
  }

  std::cout << "LOADED_________________________________" << std::endl;
  //Initialize
  gStyle->SetOptStat(0);
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
  c1->GetPad(1)->SetLogy(false);
//  c1->GetPad(1)->SetPad(.005, .2525, .995, .995);
  std::cout << "PAD SET_________________________________" << std::endl;


  // Set lumi

  float Lumf = 35900; // 1pb⁻¹ = 1
  
  //Plotting Variables
  bool DrawLog = true;

  //Selection variables
  Float_t TLep_PtW;
  Float_t TLep_PtZ1;
  Float_t TLep_PtZ2;

  Float_t TLep_EtaW;
  Float_t TLep_EtaZ1;
  Float_t TLep_EtaZ2;

  Float_t TLep_IsTightW;
  Float_t TLep_IsTightZ1;
  Float_t TLep_IsTightZ2;

  Float_t TLep_IsPromptW;
  Float_t TLep_IsPromptZ1;
  Float_t TLep_IsPromptZ2;

  Float_t TLep_pdgIdW;
  Float_t TLep_pdgIdZ1;
  Float_t TLep_pdgIdZ2;

  Int_t TNTightLeps;
  Int_t TFakeNumber;
  Int_t TConvNumber;
  Float_t TMll;
  Float_t TM3l;
  Float_t TMinMll;
  Float_t TMET;
  Int_t TNBtags;
  Float_t TMtW;
  
  Long64_t TCutsId[3];
  float MaximunY = 20;
  float MinimunY = 0;

  std::cout << "SETTING HISTOS_________________________________" << std::endl;

  // Weight and cut variables
  float TPVariable;
  float TWeight;
  Int_t TChannel;
  char TSelec;
  std::cout << "CREATING HISTOGRAMS_________________________________" << std::endl;
  //Creating histos
  Double_t xbins[6] = {10,15,20,30,45,100};
  Double_t ybinsEl[3] = {0, 1.4456, 2.5};
  Double_t ybinsMu[3] = {0, 1.2, 2.4};

  hFRMuTi   =    new   TH2D( "hMuTi",           "",  5,  xbins,2,  ybinsMu);
  hFRElTi   =    new   TH2D( "hElTi",           "",  5,  xbins,2,  ybinsEl);

  hFRMuFO   =    new   TH2D( "hMuFO",           "",  5,  xbins,2,  ybinsMu);
  hFRElFO   =    new   TH2D( "hElFO",           "",  5,  xbins,2,  ybinsEl);

  std::cout << "HISTOGRAMS CREATED_________________________________" << std::endl;
  //Loop in samples
  Int_t n0 = 0; Int_t n1= 0;
  for (std::vector<sample>::iterator it = PlottingSamples.begin(); it != PlottingSamples.end(); it++) {
      std::cout << it-> getdataCard() << std::endl;
      TString filename =  it->path();
      TFile f(filename);
      TTree* theTree = (TTree *) f.Get("veryTight");
      //TTree* theTreeMVA = (TTree *) f.Get("nolepMVA");
      theTree->SetBranchAddress("TLep_PtW", &TLep_PtW);
      theTree->SetBranchAddress("TLep_PtZ1", &TLep_PtZ1);
      theTree->SetBranchAddress("TLep_PtZ2", &TLep_PtZ2);
      theTree->SetBranchAddress("TLep_EtaW", &TLep_EtaW);
      theTree->SetBranchAddress("TLep_EtaZ1", &TLep_EtaZ1);
      theTree->SetBranchAddress("TLep_EtaZ2", &TLep_EtaZ2);
      theTree->SetBranchAddress("TLep_IsTightW", &TLep_IsTightW);
      theTree->SetBranchAddress("TLep_IsTightZ1", &TLep_IsTightZ1);
      theTree->SetBranchAddress("TLep_IsTightZ2", &TLep_IsTightZ2);
      theTree->SetBranchAddress("TLep_IsPromptW", &TLep_IsPromptW);
      theTree->SetBranchAddress("TLep_IsPromptZ1", &TLep_IsPromptZ1);
      theTree->SetBranchAddress("TLep_IsPromptZ2", &TLep_IsPromptZ2);
      theTree->SetBranchAddress("TLep_pdgIdW", &TLep_pdgIdW);
      theTree->SetBranchAddress("TLep_pdgIdZ1", &TLep_pdgIdZ1);
      theTree->SetBranchAddress("TLep_pdgIdZ2", &TLep_pdgIdZ2);
      theTree->SetBranchAddress("TNTightLeps", &TNTightLeps);
      theTree->SetBranchAddress("TFakeNumber", &TFakeNumber);
      theTree->SetBranchAddress("TConvNumber", &TConvNumber);
      theTree->SetBranchAddress("TMll", &TMll);
      theTree->SetBranchAddress("TM3l", &TM3l);
      theTree->SetBranchAddress("TMinMll", &TMinMll);
      theTree->SetBranchAddress("TNBtags", &TNBtags);
      theTree->SetBranchAddress("TMET", &TMET);
      theTree->SetBranchAddress("TWeight", &TWeight);

      for (int i = 0; i < theTree->GetEntries(); i++){
        theTree->GetEntry(i);
        //theTreeMVA->GetEntry(i);
        if (TMath::Abs(TMll - 91.1876)< 15.  && TM3l > 100 && TMinMll > 4 && TNBtags == 0 && TMET < 30){//CRDY && TLep_PtZ1 > 25 && TLep_PtZ2 > 15 && TLep_PtW > 20
//        if (TMath::Abs(TMll - 91.1876)> 15. && TLep_PtZ1 > 20 && TLep_PtZ2 > 10 && TLep_PtW > 10 && TMinMll > 12 && TMET > 50){
          Int_t nFakes = 0;
          Float_t pt;
          if (!TLep_IsPromptW){
            pt = (TLep_PtW > 100) ? 99 : TLep_PtW; //std::cout << pt << std::endl;
            if (TLep_IsTightW == 1){
              TLep_pdgIdW ? hFRElTi->Fill(pt, TMath::Abs(TLep_EtaW), TWeight) : hFRMuTi->Fill(pt, TMath::Abs(TLep_EtaW), TWeight);
              //std::cout << TLep_PtW << std::endl;
              n0++;
            }
            TLep_pdgIdW ? hFRElFO->Fill(pt, TMath::Abs(TLep_EtaW), TWeight) : hFRMuFO->Fill(pt, TMath::Abs(TLep_EtaW), TWeight);
            n1++;
          }
          if (!TLep_IsPromptZ1){         
            pt = (TLep_PtZ1 > 100) ? 99 : TLep_PtZ1; //std::cout << pt << std::endl;
            if (TLep_IsTightZ1 == 1){
              TLep_pdgIdZ1 ? hFRElTi->Fill(pt, TMath::Abs(TLep_EtaZ1), TWeight) : hFRMuTi->Fill(pt, TMath::Abs(TLep_EtaZ1), TWeight);
              //std::cout << TLep_PtW << std::endl;
              n0++;
            }
            TLep_pdgIdZ1 ? hFRElFO->Fill(pt, TMath::Abs(TLep_EtaZ1), TWeight) : hFRMuFO->Fill(pt, TMath::Abs(TLep_EtaZ1), TWeight);
            n1++;
          }
          if (!TLep_IsPromptZ2){
            pt = (TLep_PtZ2 > 100) ? 99 : TLep_PtZ2; //std::cout << pt << std::endl;
            if (TLep_IsTightZ2 == 1){
              TLep_pdgIdZ2 ? hFRElTi->Fill(pt, TMath::Abs(TLep_EtaZ2), TWeight) : hFRMuTi->Fill(pt, TMath::Abs(TLep_EtaZ2), TWeight);
              //std::cout << TLep_PtW << std::endl;
              n0++;
            }
            TLep_pdgIdZ2 ? hFRElFO->Fill(pt, TMath::Abs(TLep_EtaZ2), TWeight) : hFRMuFO->Fill(pt, TMath::Abs(TLep_EtaZ2), TWeight);
            n1++;
          }
        }
      }
  }
  std::cout << n0 << ", " << n1 << "\n";
  TFile f("./plotsFR/FR.root", "RECREATE");
  f.cd();
  hFRMuTi->Write(); hFRElTi->Write(); hFRMuFO->Write(); hFRElFO->Write();
  hFRMuTi->Sumw2();hFRElTi->Sumw2();hFRMuFO->Sumw2();hFRElFO->Sumw2();

  TH2D qMu = (*hFRMuTi)/(*hFRMuFO);   TH2D qEl = (*hFRElTi)/(*hFRElFO);
  qMu.SetName("qMu"); qEl.SetName("qEl");
  qMu.Write(); qEl.Write();

  f.Close();


  //Plotting
/*
  hTruth->Sumw2();
  hEst->Sumw2();

  hTruth->Scale(Lumf);
  hEst->Scale(Lumf);
  hEst->SetLineColor(kBlack);

  std::cout << hTruth->Integral() << ", " << hEst->Integral()<< std::endl;
  std::cout << "KS - test: " << hTruth->KolmogorovTest(hEst, "XDN") << std::endl;



  hTruth->Draw("p");
  hTruth->GetYaxis()->SetTitle("Events");
  hTruth->GetXaxis()->SetTitle(varrLabel);
  hTruth->GetYaxis()->SetTitleOffset(1.35);
  hTruth->SetMaximum(MaximunY);
  hTruth->SetMinimum(MinimunY);
  
  hEst->SetFillColorAlpha(kRed, 0.15);

  hEst->DrawCopy("histsame");
  hEst->SetFillColor(kBlack);
  hEst->SetFillStyle(3644);
  hEst->DrawCopy("e2same");
  hEst->SetFillColorAlpha(kRed, 0.15);
  hEst->SetFillStyle(1001);

  l->AddEntry(hTruth, "Truth (" +what+ ")", "lep");
  l->AddEntry(hEst, "Estimated (" +what+ ")", "f");
  l->AddEntry("", Form("KS-test: %.3f",hTruth->KolmogorovTest(hEst, "XD")), "");
  l->Draw("same");


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
  texPrelim->Draw();*/

  //Quotient DATA/BG plot
/*  c1->cd(2);
  //gStyle->SetOptTitle(0);
  c1->GetPad(2)->SetPad(.005, .005, .995, 0.2475);
  gStyle->SetOptStat(0);
  quo = ((*hTruth)/(*hEst));
  quo.SetMaximum(1.5);
  quo.SetMinimum(0.5);
  quo.SetTitle("");
  quo.GetYaxis()->SetTitle("MC Truth / Estim.");
  quo.GetXaxis()->SetTitle(varrLabel);
  quo.GetYaxis()->SetLabelSize(.10);
  quo.GetYaxis()->SetTitleSize(.10);
  quo.GetYaxis()->SetTitleOffset(.35);
  quo.GetXaxis()->SetTitleSize(.20);
  quo.GetYaxis()->SetNdivisions(510);
  quo.Draw();
  
  TH1F* one = (TH1F*) quo.Clone();
  for (int i = 0; i < one->GetNbinsX()+1 ; i ++){
    one->SetBinContent(i,1);
  }
  one->SetLineColor(kBlack);
  one->Draw("histsame");*/
//  c1->Print("./plotsFR/"+ varr + ".eps");
//  c1->Print("./plotsFR/"+ varr + ".pdf");
//  c1->Print("./plotsFR/"+ varr + ".png");
  
/*   TString savepath = "/mnt_pool/fanae105/user/carlosec/BDT/TMVA-v4.2.0/test/results";
   TFile *target  = new TFile( savepath + "/"+ methodName + ".root","RECREATE" );
   hsignal ->Write();
  hs -> Write();

   target->Close();
*/
}
