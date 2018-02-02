R__LOAD_LIBRARY(TResultsTable.C+)

#include "../TResultsTable.h"
#include "TopHistoReader.C"
#include "GetDataDriven.C"
void PrintTableYields(TString level, Bool_t doDD = false);
void FiduAccUnc();
void FSR();

void PrintYields(){
  //GetDYDD("ElMu", "1btag", 0, 1);
  //GetNonWDD("ElMu", "1btag", 0, 1);
  //PrintSFwithLevel();
  //PrintTableYields("1btag");
  PrintTableYields("1btag", true);
  //FiduAccUnc();
  //FSR();
}


void PrintTableYields(TString level, Bool_t doDD){ 
  TopHistoReader *t = new TopHistoReader();
  TopHistoReader *s = new TopHistoReader();
  s->SetDoStat(1);

  TString ch;
  Float_t DY; Float_t nonW;
  Float_t DYerr; Float_t nonWerr;
  Float_t AllBkg; Float_t AllBkg_stat; Float_t AllBkg_syst;
 
  TResultsTable y(8, 3, 2); 
  y.SetFormatNum("%1.0f");
  y.SetRowTitleHeader("Source");
  y.AddVSeparation(4);  y.AddVSeparation(5);  y.AddVSeparation(6); 

  y.SetColumnTitle(0, "$e^{+}e^{-}$");
  y.SetColumnTitle(1, "$\\mu^{+}\\mu^{-}$");
  y.SetColumnTitle(2, "$e^{\\pm}\\mu^{\\mp}$");

  y.SetRowTitle(0, "Drell-Yan");
  y.SetRowTitle(1, "Non W/Z leptons");
  y.SetRowTitle(2, "Single top quark");
  y.SetRowTitle(3, "Dibosons");
  y.SetRowTitle(4, "ttV");
  y.SetRowTitle(5, "Total background");
  y.SetRowTitle(6, "\\ttbar dilepton");
  y.SetRowTitle(7, "Data");

  for(Int_t i = 0; i < 3; i++){

    if(i == 0) ch = "Elec"; 
    else if(i == 1) ch = "Muon";
    else ch = "ElMu";
    t->SetChan(ch); t->SetLevel(level);
    DY      = t->GetYield("DY");
    nonW    = t->GetYield("NonW");
    DYerr   = s->GetYield("DY");
    nonWerr = s->GetYield("NonW");
    if(doDD){
      DY = DY*GetDYDD(ch, level); 
      nonW = GetNonWDD(ch, level);
      DYerr = DY*GetDYDD(ch, level, 1); 
      nonWerr = GetNonWDD(ch, level, 1);
    }

    AllBkg = DY + nonWerr + t->GetYield("tW") + t->GetYield("VV") + t->GetYield("ttV");
    AllBkg_syst = TMath::Sqrt(DY*0.15*DY*0.15 + nonW*0.30*nonW*0.30 + t->GetYield("tW")*0.30*t->GetYield("tW")*0.30 + t->GetYield("VV")*0.30*t->GetYield("VV")*0.30 + t->GetYield("ttV")*0.30*t->GetYield("ttV")*0.30);
    AllBkg_stat = TMath::Sqrt(DYerr*DYerr + nonWerr*nonWerr + s->GetYield("tW")*s->GetYield("tW") + s->GetYield("VV")*s->GetYield("VV") + s->GetYield("ttV")*s->GetYield("ttV"));

    y[0][i].SetContent(DY);   y[0][i].SetError(DY*0.15);   y[0][i].SetStatError(DYerr); 
    y[1][i].SetContent(nonW); y[1][i].SetError(nonW*0.30); y[1][i].SetStatError(nonWerr); 
    y[2][i].SetContent(t->GetYield("tW")); y[2][i].SetError(t->GetYield("tW")*0.30); y[2][i].SetStatError(s->GetYield("tW")); 
    y[3][i].SetContent(t->GetYield("VV")); y[3][i].SetError(t->GetYield("VV")*0.30); y[3][i].SetStatError(s->GetYield("VV")); 
    y[4][i].SetContent(t->GetYield("ttV")); y[4][i].SetError(t->GetYield("ttV")*0.30); y[4][i].SetStatError(s->GetYield("ttV")); 
    y[5][i].SetContent(AllBkg); y[5][i].SetError(AllBkg_syst); y[5][i].SetStatError(AllBkg_stat); 
    y[6][i].SetContent(t->GetYield("ttbar")); y[6][i].SetError(t->GetYield("ttbar")*0.042); y[6][i].SetStatError(s->GetYield("ttbar")); 
    y[7][i].SetContent(t->GetYield("data")); y[7][i].SetStatError(TMath::Sqrt(t->GetYield("data"))); 
  }

  y.SetDrawHLines(true); y.SetDrawVLines(true); y.Print();
  TString name = doDD? "yieldsAllChan_DD" : "yieldsAllChan_MC";
  y.SaveAs(outputFolder + "/" + name + "_" + level + ".tex");
  y.SaveAs(outputFolder + "/" + name + "_" + level + ".html");
  y.SaveAs(outputFolder + "/" + name + "_" + level + ".txt");

}

void FiduAccUnc(){
  TopHistoReader *t = new TopHistoReader();
  t->SetLevel("dilepton");

  // Inclusive
  cout << "===== INCLUSIVE =====\n";
  cout<< "Hdamp: " << t->GetTopUnc("hdamp")*100 << endl;
  cout<< "UE: "    << t->GetTopUnc("ue")*100 << endl;
  cout<< "ISR: "   << t->GetTopUnc("isr")*100 << endl;
  cout<< "FSR: "   << t->GetTopUnc("fsr")*100/sqrt(2) << endl;
 
  // Fidu 
  t->SetIsFidu();
  t->SetPathFidu(t->GetPath()+"/Fiducial_ext/");
  cout << "===== FIDUCIAL =====\n";
  cout << "Hdamp: " << t->GetTopUnc("hdamp")*100 << endl;
  cout<< "UE: "    << t->GetTopUnc("ue")*100 << endl;
  cout<< "ISR: "   << t->GetTopUnc("isr")*100 << endl;
  cout<< "FSR: "   << t->GetTopUnc("fsr")*100/sqrt(2) << endl;

  delete t;
}

void FSR(){
  TopHistoReader *t = new TopHistoReader();
  //t->SetIsFidu();
  //t->SetPathFidu(t->GetPath()+"/Fiducial_ext/");
 
  TString levels[3] = {"dilepton", "2jets", "1btag"};  
  TString chans[3] = {"Elec", "Muon", "ElMu"};
  TString lev = ""; TString chan = "";

  TResultsTable y(3, 3, 0); 
  y.SetFormatNum("%1.2f");
  y.SetRowTitleHeader("FSR uncertainty");

  y.SetColumnTitle(0, "ee");//"$e^{+}e^{-}$");
  y.SetColumnTitle(1, "mumu");//"$\\mu^{+}\\mu^{-}$");
  y.SetColumnTitle(2, "emu");//"$e^{\\pm}\\mu^{\\mp}$");
  
  y.SetRowTitle(0, "Dilepton");
  y.SetRowTitle(1, "+ at least 2 jets");
  y.SetRowTitle(2, "+ at least 1 b-tagged");

  for(Int_t i = 0; i < 3; i++){
    t->SetLevel(levels[i]);
    for(Int_t j = 0; j < 3; j++){
      t->SetChan(chans[j]);
      cout << "Calculating FSR for " << chans[j] << " and " << levels[i] << endl;
      y[i][j].SetContent(t->GetTopUnc("fsr")*100/sqrt(2));
    }
  }

  TString name = "FSR_inclusive";
  TString outFolder = ".";

  y.SetDrawHLines(true); y.SetDrawVLines(true); y.Print();
  y.SaveAs(outFolder + "/" + name + ".tex");
  y.SaveAs(outFolder + "/" + name + ".html");
  y.SaveAs(outFolder + "/" + name + ".txt");
}
