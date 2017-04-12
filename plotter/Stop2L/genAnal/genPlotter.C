#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"

const TString path = "./genVars.root";
const TString outputfolder = "genPlots/";
const TString process[]     = {"ttbar", "SFS_225_50", "S_233_50", "SFS_250_50"};
const int nVars = 13;
const TString vars[nVars] = {"NJets","NBJets","Lep0Pt","Lep1Pt","Jet0Pt","Jet1Pt","HT","MET", "Mll","MT2", "MT2no0", "DilepPt", "DeltaPhill"};//, "DeltaPhijj", "DeltaPhilj", "DeltaPhilm", "DeltaPhijm","MT2", "MT2lblb", "MT2bb", "Meff"};



TH1F* httbar; TH1F* httbarMLM; TH1F* hstop1; TH1F* hstop2; TH1F* httbarMLMFS;
TH1F* hstop3; TH1F* hstop4;
TFile* file = TFile::Open(path);

void plotvar(TString var){
  cout << "Plotting " << var << ": loading files!" << endl;
  file->GetObject( var + "_" + "ttbar_Powheg",  httbar);
  file->GetObject( var + "_" + "ttbar_MLM",     httbarMLM);
  //file->GetObject( var + "_" + "ttbar_MLM_fastSim",  httbarMLMFS);
  file->GetObject( var + "_" + "SFS_225_50", hstop1);
  file->GetObject( var + "_" + "SFS_250_50", hstop2);
  file->GetObject( var + "_" + "S_233_50", hstop3);
  file->GetObject( var + "_" + "SFS_200_50", hstop4);

  cout << "Setting histograms..." << endl;
  httbar->SetLineColor(1);
  httbar->SetMarkerColor(1);
  httbarMLM->SetLineColor(kGray+1);
  httbarMLM->SetMarkerColor(kGray+1);
//  httbarMLMFS->SetLineColor(kViolet);
//  httbarMLMFS->SetMarkerColor(kViolet);
  hstop1->SetLineColor(kBlue);
  hstop1->SetMarkerColor(kBlue);
  hstop2->SetLineColor(kRed);
  hstop2->SetMarkerColor(kRed);
  hstop3->SetLineColor(kGreen+2);
  hstop3->SetMarkerColor(kGreen+2);
  hstop4->SetLineColor(kViolet+1);
  hstop4->SetMarkerColor(kViolet+1);


  httbar->SetLineWidth(2); hstop1->SetLineWidth(2); hstop2->SetLineWidth(2); hstop3->SetLineWidth(2);
  hstop4->SetLineWidth(2);
  httbarMLM->SetLineWidth(2); 
//  httbarMLMFS->SetLineWidth(2);
  httbar->SetStats(0); hstop1->SetStats(0); hstop2->SetStats(0);  hstop3->SetStats(0); 
  httbarMLM->SetStats(0);
//  httbarMLMFS->SetStats(0);
  httbar->Scale(1/httbar->Integral()); 
  hstop1->Scale(1/hstop1->Integral()); hstop2->Scale(1/hstop2->Integral()); 
  hstop3->Scale(1/hstop3->Integral());
  hstop4->Scale(1/hstop4->Integral());
//  httbarMLMFS->Scale(1/httbarMLMFS->Integral()); 
  httbarMLM->Scale(1/httbarMLM->Integral()); 
  httbar->SetMarkerStyle(21);
 httbarMLM->SetMarkerStyle(21); //httbarMLMFS->SetMarkerStyle(21);
hstop1->SetMarkerStyle(21); hstop2->SetMarkerStyle(21); 
hstop3->SetMarkerStyle(21);  hstop4->SetMarkerStyle(21); 
  httbar->SetMarkerSize(2.0); hstop1->SetMarkerSize(2.0); hstop2->SetMarkerSize(2.0); 
hstop3->SetMarkerSize(2.0);  hstop4->SetMarkerSize(2.0); 
httbarMLM->SetMarkerSize(2.0); // httbarMLMFS->SetMarkerSize(21);


  httbar->SetTitle(var);
  httbar->GetXaxis()->SetTitle(var);

  cout << "Setting leg..." << endl;
  TLegend* leg;
  leg = new TLegend(0.75,0.79,0.93,0.93);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(httbar, "ttbar powheg", "l");
  leg->AddEntry(httbarMLM, "ttbar MLM", "l");
//  leg->AddEntry(httbarMLMFS, "ttbar MLM FastSim", "l");
  leg->AddEntry(hstop4, "stop_200_50", "l");
  leg->AddEntry(hstop1, "stop_225_50", "l");
  leg->AddEntry(hstop3, "stop_233_50", "l");
  leg->AddEntry(hstop2, "stop_250_50", "l");

  cout << "New Camvas..." << endl;
  TCanvas* c= new TCanvas("c","c",10,10,800,600);
  
  httbar->SetMaximum(httbar->GetMaximum()*1.4);
  httbar->Draw("");
 httbarMLM->Draw("same");
//  httbarMLMFS->Draw("same");
  hstop1->Draw("same");
  hstop3->Draw("same");
  hstop2->Draw("same");
  hstop4->Draw("same");
  leg->Draw("same");
 
  gSystem->mkdir(outputfolder, kTRUE); 
  c->Print(outputfolder + var + ".png", "png");
  c->SetLogy();
  c->Print(outputfolder + var + "_log.png", "png");
  //file->Close();
  delete c; 
}


void genPlotter(){
		//plotvar("MT2");
	//	plotvar("MT2no0");
	for(int i = 0; i < nVars; i++){
		plotvar(vars[i]);
	}
}
