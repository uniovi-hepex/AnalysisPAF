#include "CrossSection.h"

void CrossSection::SetMembers(){
    notSet = false;
    nBkgs = BkgYield.size();
    nSyst = SysVar.size();

    NBkg = 0; NBkg_err = 0; y_err = 0; NData_err = TMath::Sqrt(NData);
    for(Int_t k = 0; k < nBkgs; k++) NBkg      += BkgYield.at(k); 
    for(Int_t k = 0; k < nBkgs; k++) NBkg_err  += ( BkgYield.at(k)*BkgUnc.at(k) )*( BkgYield.at(k)*BkgUnc.at(k) ); 
    for(Int_t k = 0; k < nSyst; k++) y_err     += (y - SysVar.at(k)) * (y - SysVar.at(k));
    NBkg_err = TMath::Sqrt(NBkg_err);
    y_err    = TMath::Sqrt(   y_err);
    
    xsec = GetXSec(NData, NBkg, y);
    xsec_stat_err = fabs(xsec - GetXSec(NData+TMath::Sqrt(NData), NBkg, y));
    xsec_lumi_err = fabs(xsec*LumiUnc);

    xsec_syst_err = 0;
    for(Int_t k = 0; k < nSyst; k++){
      xsec_syst_err += fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)*fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec);
    }
    xsec_syst_err  = TMath::Sqrt(xsec_syst_err);
    xsec_total_err = TMath::Sqrt(xsec_syst_err*xsec_syst_err + xsec_lumi_err*xsec_lumi_err + xsec_stat_err*xsec_stat_err);

    // For acceptance and efficiency...
    TotalAcceptance = 0; acceptance = 0; efficiency = 0;
    eff_err = 0; acc_err = 0;
    if(BR != 0){
      TotalAcceptance = y/(Lumi*thxsec)/BR;
      if(nFiducialSignal != 0 && nSimulatedSignal != 0){
        acceptance = nFiducialSignal/nSimulatedSignal/BR;
        efficiency = TotalAcceptance/acceptance;
      }
    }
}

void CrossSection::PrintSystematicTable(TString options){
  if(notSet) SetMembers();
  Int_t nrows = nSyst+nBkgs+4;
  Int_t ncolumns = 2;
  TResultsTable t(nrows, ncolumns, false); //cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetFormatNum("%1.2f");
  t.SetRowTitleHeader("Source");
  //t.AddVSeparation(Form("%i, %i, %i", nSyst-1, nSyst + nBkgs - 1, nSyst+nBkgs+4 - 2));
  t.AddVSeparation(nSyst-1);  t.AddVSeparation(nSyst + nBkgs - 1); t.AddVSeparation(nSyst+nBkgs+4 - 2);

  // Set row titles
  for(Int_t i = 0; i < nSyst; i++) t.SetRowTitle(i,       SysTags.at(i));
  for(Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i+nSyst, BkgTags.at(i));
  t.SetRowTitle(nBkgs+nSyst+0, "Total Systematic");
  t.SetRowTitle(nBkgs+nSyst+1, "Integrated Luminosity");
  t.SetRowTitle(nBkgs+nSyst+2, "Statistical");
  t.SetRowTitle(nBkgs+nSyst+3, "Total");

  // Set column titles
  t.SetColumnTitle(0, "Delta sigma");
  t.SetColumnTitle(1, "(%)");

  // Fill numbers
  for(Int_t i = 0; i < nSyst; i++){
    t[i][0] = fabs(GetXSec(NData, NBkg, SysVar.at(i)) - xsec);
    t[i][1] = t[i][0]/xsec*100;
  }
  for(Int_t i = 0; i < nBkgs; i++){
    t[i+nSyst][0] = fabs(GetXSec(NData + BkgUnc.at(i)*BkgYield.at(i), NBkg, y) - xsec);
    t[i+nSyst][1] = t[i+nSyst][0]/xsec*100;
  }
  t[nBkgs+nSyst+0][0] = xsec_syst_err; 
  t[nBkgs+nSyst+1][0] = xsec_lumi_err;
  t[nBkgs+nSyst+2][0] = xsec_stat_err;
  t[nBkgs+nSyst+3][0] = xsec_total_err;
  t[nBkgs+nSyst+0][1] = t[nBkgs+nSyst+0][0]/xsec*100;
  t[nBkgs+nSyst+1][1] = t[nBkgs+nSyst+1][0]/xsec*100;
  t[nBkgs+nSyst+2][1] = t[nBkgs+nSyst+2][0]/xsec*100;
  t[nBkgs+nSyst+3][1] = t[nBkgs+nSyst+3][0]/xsec*100;

  // Print and save
  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  if(options.Contains("tex"))  t.SaveAs(outputFolder + "/" + tableName + ".tex");
  if(options.Contains("html")) t.SaveAs(outputFolder + "/" + tableName + ".html");
  if(options.Contains("txt"))  t.SaveAs(outputFolder + "/" + tableName + ".txt");
}

void CrossSection::PrintYieldsTable(TString options){
  if(notSet) SetMembers();
  Int_t nrows = nBkgs+3;
  Int_t ncolumns = 1;
  TResultsTable t(nrows, ncolumns, true); 
  t.SetFormatNum("%1.2f");
  t.AddVSeparation(nBkgs-1); t.AddVSeparation(nBkgs); t.AddVSeparation(nBkgs+1);
  for(Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i, BkgTags.at(i));
  t.SetRowTitle(nBkgs  , "Total Background");
  t.SetRowTitle(nBkgs+1, "Expected signal");
  t.SetRowTitle(nBkgs+2, "Observed data");
  t.SetRowTitleHeader("Source");
  t.SetColumnTitle(0, "Yield");
  for(Int_t i = 0; i < nBkgs; i++){
    t[i][0] = BkgYield.at(i);
    t[i][0].SetError(BkgUnc.at(i)*BkgYield.at(i));
  }
    t[nBkgs][0]   = NBkg;  t[nBkgs][0].SetError(NBkg_err);
    t[nBkgs+1][0] = y;     t[nBkgs+1][0].SetError(y_err);
    t[nBkgs+2][0] = NData; t[nBkgs+2][0].SetError(NData);

    t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
    if(options.Contains("tex"))  t.SaveAs(outputFolder + "/CrossSection_yields.tex");
    if(options.Contains("html")) t.SaveAs(outputFolder + "/CrossSection_yields.html");
    if(options.Contains("txt"))  t.SaveAs(outputFolder + "/CrossSection_yields.txt");
}

void CrossSection::PrintCrossSection(TString options){
  cout << "\033[1;31m >>> The measured cross section is: \033[0m\n";
  cout << Form("\033[1;36m     - %1.1f +/- %1.1f (stat) +/- %1.1f (sys) +/- %1.1f (lumi)  pb\033[0m\n", xsec, xsec_stat_err, xsec_syst_err, xsec_lumi_err);
  cout << Form("\033[1;34m     - %1.1f +/- %1.1f (total, %1.2f percent) \033[0m\n", xsec, xsec_total_err, xsec_total_err/xsec*100);
  cout << Form("\033[1;32m     - BR         = %1.4f \033[0m\n", BR);
  cout << Form("\033[1;32m     - Acceptance = %1.2f +/- %1.2f \033[0m\n", acceptance, acc_err);
  cout << Form("\033[1;32m     - Efficiency = %1.2f +/- %1.2f \033[0m\n", efficiency, eff_err);
}

void CrossSection::SwitchLabel(TString oldLabel, TString newLabel){
  if(notSet) SetMembers();
  for(Int_t i = 0; i < nBkgs; i++) if(BkgTags.at(i) == oldLabel) BkgTags.at(i) = newLabel;
  for(Int_t i = 0; i < nSyst; i++) if(SysTags.at(i) == oldLabel) SysTags.at(i) = newLabel;
}
