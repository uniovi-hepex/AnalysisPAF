#include "CrossSection.h"

void CrossSection::SetMembers(){
    nBkgs = BkgYield.size();
    nSyst = SysVar.size();

    NBkg = 0;
    for(Int_t k = 0; k < nBkgs; k++) NBkg += BkgYield.at(k);      
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
    if(nSimulatedSignal != 0){
      TotalAcceptance = 0;
    }
    else TotalAcceptance = 0;
    if(nFiducialSignal != 0){
      acceptance = 0;
      acc_total_err = 0;
      acc_stat_err = 0;
      acc_syst_err = 0;
      acc_lumi_err = 0;
    }
    else acceptance = 0;
    if(BR != 0){
      efficiency = 0;
      eff_total_err = 0;
      eff_stat_err = 0;
      eff_syst_err = 0;
      eff_lumi_err = 0;
    }
    else efficiency = 0;
}

void CrossSection::PrintSystematicTable(TString options){
  SetMembers();
  Int_t nrows = nSyst+nBkgs+4;
  Int_t ncolumns = 2;
  TResultsTable t(nrows, ncolumns, false); cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetRowTitleHeader("Source");

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

void CrossSection::PrintCrossSection(TString options){
  cout << "The measured cross section is: " << endl;
  cout << Form(" >>> %1.1f +/- %1.1f (stat) +/- %1.1f (sys) +/- %1.1f (lumi)  pb", xsec, xsec_stat_err, xsec_syst_err, xsec_lumi_err) << endl;
  cout << " >>> " << xsec << " +/- " << xsec_total_err << " (" << xsec_total_err/xsec*100 << " %)" << endl;
}
