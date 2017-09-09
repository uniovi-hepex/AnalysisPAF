#include "CrossSection.h"

void CrossSection::SetMembers(){
  notSet = false;
  nBkgs = BkgYield.size();
  nSyst = SysVar.size();

  NBkg = 0; NBkg_err = 0; NBkg_staterr = 0; y_err = 0; NData_err = TMath::Sqrt(NData);
  for(Int_t k = 0; k < nBkgs; k++) NBkg      += BkgYield.at(k); 
  for(Int_t k = 0; k < nBkgs; k++) NBkg_err  += ( BkgYield.at(k)*BkgUnc.at(k) )*( BkgYield.at(k)*BkgUnc.at(k) ); 
  for(Int_t k = 0; k < nBkgs; k++) NBkg_staterr  += ( BkgYield.at(k)*BkgStatUnc.at(k) )*( BkgYield.at(k)*BkgStatUnc.at(k) ); 
  for(Int_t k = 0; k < nSyst; k++) y_err     += (y - SysVar.at(k)) * (y - SysVar.at(k));
  NBkg_err = TMath::Sqrt(NBkg_err);
  NBkg_staterr = TMath::Sqrt(NBkg_staterr);
  y_err    = TMath::Sqrt(   y_err);

  xsec = GetXSec(NData, NBkg, y);
  xsec_stat_err = fabs(xsec - GetXSec(NData+TMath::Sqrt(NData), NBkg, y));
  xsec_lumi_err = fabs(xsec*(LumiUnc/(1+LumiUnc)));

  xsec_syst_err = 0;
  for(Int_t k = 0; k < nSyst; k++) xsec_syst_err += fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)*fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec);
  for(Int_t k = 0; k < nBkgs; k++) xsec_syst_err += fabs(GetXSec(NData, NBkg + BkgYield.at(k)*BkgUnc.at(k), y) - xsec)*fabs(GetXSec(NData, NBkg + BkgYield.at(k)*BkgUnc.at(k), y) - xsec);
  xsec_syst_err  = TMath::Sqrt(xsec_syst_err);
  xsec_total_err = TMath::Sqrt(xsec_syst_err*xsec_syst_err + xsec_lumi_err*xsec_lumi_err + xsec_stat_err*xsec_stat_err);

  // For acceptance and efficiency...
  TotalAcceptance = 0; acceptance = 0; efficiency = 0; eff_err = 0; acc_err = 0;
  if(BR != 0 && nFiducialSignal != 0 && nSimulatedSignal != 0){
    TotalAcceptance = y/(Lumi*1000*thxsec)/BR;
    acceptance = nFiducialSignal/nSimulatedSignal/BR;
    efficiency = TotalAcceptance/acceptance;
    for(Int_t k = 0; k < nSyst; k++){
      if(IsEffic.at(k)) eff_err += fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)/xsec*fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)/xsec;
      else              acc_err += fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)/xsec*fabs(GetXSec(NData, NBkg, SysVar.at(k)) - xsec)/xsec;
    }
    eff_err = TMath::Sqrt(eff_err)*efficiency;
    acc_err = TMath::Sqrt(acc_err)*acceptance;
  }
}

void CrossSection::SetEfficiencySyst(TString s){
  s.ReplaceAll(" ", "");
  if(s.Contains(",")){
    TString OneSyst = TString(s(0, s.First(",")));
    TString TheRest = TString(s(s.First(",")+1, s.Sizeof()));
    SetEfficiencySyst(OneSyst);
    SetEfficiencySyst(TheRest);
    return;
  }
  else{
    nSyst = SysVar.size();
    while((Int_t) IsEffic.size() < nSyst) IsEffic.push_back(false);
    for(Int_t i = 0; i < nSyst; i++) if(SysTags.at(i) == s) IsEffic.at(i) = true;
  }
}

void CrossSection::SetAcceptanceSyst(TString s){
  s.ReplaceAll(" ", "");
  if(s.Contains(",")){
    TString OneSyst = TString(s(0, s.First(",")));
    TString TheRest = TString(s(s.First(",")+1, s.Sizeof()));
    SetAcceptanceSyst(OneSyst);
    SetAcceptanceSyst(TheRest);
    return;
  }
  else{
    nSyst = SysVar.size();
    while((Int_t) IsEffic.size() < nSyst) IsEffic.push_back(false);
    for(Int_t i = 0; i < nSyst; i++) if(SysTags.at(i) == s) IsEffic.at(i) = false;
  }
}

void CrossSection::PrintSystematicTable(TString options){
  if(notSet) SetMembers();
  Int_t nrows = nSyst+nBkgs+4;
  Int_t ncolumns = 2;
  TResultsTable t(nrows, ncolumns, false); //cout << Form("Creating table with [rows, columns] = [%i, %i]\n", nrows, ncolumns);
  t.SetFormatNum("%1.2f");
  if(IsFiducial) t.SetFormatNum("%1.4f");
  t.SetRowTitleHeader("Source");
  //t.AddVSeparation(Form("%i, %i, %i", nSyst-1, nSyst + nBkgs - 1, nSyst+nBkgs+4 - 2));
  t.AddVSeparation(nSyst-1);  t.AddVSeparation(nSyst + nBkgs - 1); t.AddVSeparation(nSyst+nBkgs+4 - 2);

  Int_t nEff = 0; Int_t nAcc = 0;
  for(Int_t i = 0; i < nSyst; i++){
    if(IsEffic.at(i)) nEff++;
    else              nAcc++;
  }
  cout << "nEff = " << nEff << ", nAcc = " << nAcc << endl;

  // Set row titles
  Int_t index = 0;
  if(nEff != 0 && nAcc != 0){
    t.AddVSeparation(nEff-1);
    for(Int_t i = 0; i < nSyst; i++){ 
      if(IsEffic.at(i) ){
        t.SetRowTitle(index,       SysTags.at(i));
        index++;
      }
    }
    for(Int_t i = 0; i < nSyst; i++){
      if(!IsEffic.at(i)){
        t.SetRowTitle(index,       SysTags.at(i));
        index++;
      }
    }
  }
  else for(Int_t i = 0; i < nSyst; i++) t.SetRowTitle(i,       SysTags.at(i));
  for     (Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i+nSyst, BkgTags.at(i));
  t.SetRowTitle(nBkgs+nSyst+0, "Total Systematic");
  t.SetRowTitle(nBkgs+nSyst+1, "Integrated Luminosity");
  t.SetRowTitle(nBkgs+nSyst+2, "Statistical");
  t.SetRowTitle(nBkgs+nSyst+3, "Total");

  // Set column titles
  t.SetColumnTitle(0, "Delta sigma");
  t.SetColumnTitle(1, "(%)");

  // Fill numbers
  if(nEff != 0 && nAcc != 0){
    index = 0;
    for(Int_t i = 0; i < nSyst; i++){ 
      if(IsEffic.at(i) ){
        t[index][0] = fabs(GetXSec(NData, NBkg, SysVar.at(i)) - xsec);
        t[index][1] = t[index][0]/xsec*100;
        index++;
      }
    }
    for(Int_t i = 0; i < nSyst; i++){
      if(!IsEffic.at(i)){
        t[index][0] = fabs(GetXSec(NData, NBkg, SysVar.at(i)) - xsec);
        t[index][1] = t[index][0]/xsec*100;
        index++;
      }
    }
  }
  else{
    for(Int_t i = 0; i < nSyst; i++){
      t[i][0] = fabs(GetXSec(NData, NBkg, SysVar.at(i)) - xsec);
      t[i][1] = t[i][0]/xsec*100;
    }
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
  TResultsTable t(nrows, ncolumns, 2); 
  t.SetFormatNum("%1.2f");
  t.AddVSeparation(nBkgs-1); t.AddVSeparation(nBkgs); t.AddVSeparation(nBkgs+1);
  for(Int_t i = 0; i < nBkgs; i++) t.SetRowTitle(i, BkgTags.at(i));
  t.SetRowTitle(nBkgs  , "Total Background");
  t.SetRowTitle(nBkgs+1, "Expected signal");
  t.SetRowTitle(nBkgs+2, "Observed data");
  t.SetRowTitleHeader("Source");
  t.SetColumnTitle(0, "Yield");
  for(Int_t i = 0; i < nBkgs; i++){
    t[i][0].SetContent(BkgYield.at(i));
    t[i][0].SetError(BkgUnc.at(i)*BkgYield.at(i));
    t[i][0].SetStatError(BkgStatUnc.at(i)*BkgYield.at(i));
  }
    t[nBkgs][0].SetContent(   NBkg);  t[nBkgs][0].SetError(NBkg_err);
    t[nBkgs+1][0].SetContent( y);     t[nBkgs+1][0].SetError(y_err);
    t[nBkgs+2][0].SetContent( NData); t[nBkgs+2][0].SetError(TMath::Sqrt(NData));

    t[nBkgs][0].SetContent(   NBkg);  t[nBkgs][0].SetStatError(NBkg_staterr);
    t[nBkgs+1][0].SetContent( y);     t[nBkgs+1][0].SetStatError(y_staterr);

    t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
    if(options.Contains("tex"))  t.SaveAs(outputFolder + "/" + yieldsTableName + ".tex");
    if(options.Contains("html")) t.SaveAs(outputFolder + "/" + yieldsTableName + ".html");
    if(options.Contains("txt"))  t.SaveAs(outputFolder + "/" + yieldsTableName + ".txt");
}

void CrossSection::PrintCrossSection(TString options){
  cout << "\033[1;31m >>> The measured cross section is: \033[0m\n";
  cout << Form("\033[1;36m     - xsec = %1.5f +/- %1.5f (stat) +/- %1.5f (sys) +/- %1.5f (lumi)  pb\033[0m\n", xsec, xsec_stat_err, xsec_syst_err, xsec_lumi_err);
  cout << Form("\033[1;34m            = %1.5f +/- %1.5f (total, %1.5f percent) \033[0m\n", xsec, xsec_total_err, xsec_total_err/xsec*100);
  cout << Form("\033[1;32m     - BR         = %1.4f \033[0m\n", BR);
  cout << Form("\033[1;32m     - Acceptance = %1.5f +/- %1.5f (%1.3f percent) \033[0m\n", acceptance, acc_err, acc_err/acceptance*100);
  cout << Form("\033[1;32m     - Efficiency = %1.5f +/- %1.5f (%1.3f percent) \033[0m\n", efficiency, eff_err, eff_err/efficiency*100);

  if(notSet) SetMembers();
  Int_t nrows = 5;
  Int_t ncolumns = 5;
  TResultsTable t(nrows, ncolumns, false); 
  t.SetFormatNum("%1.5f");
  t.AddVSeparation(0); t.AddVSeparation(3); 

  t.SetRowTitleHeader(" ");
  t.SetColumnTitle(0, "Value");
  t.SetColumnTitle(1, "Stat");
  t.SetColumnTitle(2, "Syst");
  t.SetColumnTitle(3, "Lumi");
  t.SetColumnTitle(4, "Total");
  t.SetRowTitle(0, "Inclusive Cross Section");
  t.SetRowTitle(1, "Branching ratio");
  t.SetRowTitle(2, "Efficiency");
  t.SetRowTitle(3, "Acceptance");
  t.SetRowTitle(4, "Fiducial Cross Section");

  t[0][0] = xsec; t[0][1] = xsec_stat_err; t[0][2] = xsec_syst_err; t[0][3] = xsec_lumi_err; t[0][4] = xsec_total_err;
  t[1][0] = BR;
  t[2][0] = efficiency; t[2][4] = acc_err;
  t[3][0] = acceptance; t[3][4] = eff_err;
  t[4][0] = xsec*acceptance*BR;
  t[4][1] = xsec_stat_err*acceptance*BR; //stat
  t[4][2] = xsec_syst_err*acceptance*BR; //syst
  t[4][3] = xsec_lumi_err*acceptance*BR; //lumi
  t[4][4] = xsec_total_err*acceptance*BR; //total


  t.SetDrawHLines(true); t.SetDrawVLines(true); t.Print();
  if(options.Contains("tex"))  t.SaveAs(outputFolder + "/" + xsecvalTableName + ".tex");
  if(options.Contains("html")) t.SaveAs(outputFolder + "/" + xsecvalTableName + ".html");
  if(options.Contains("txt"))  t.SaveAs(outputFolder + "/" + xsecvalTableName + ".txt");
}

void CrossSection::SwitchLabel(TString oldLabel, TString newLabel){
  if(notSet) SetMembers();
  for(Int_t i = 0; i < nBkgs; i++) if(BkgTags.at(i) == oldLabel) BkgTags.at(i) = newLabel;
  for(Int_t i = 0; i < nSyst; i++) if(SysTags.at(i) == oldLabel) SysTags.at(i) = newLabel;
}

void CrossSection::SetUnc(TString source, Float_t value){
  if(notSet) SetMembers();
  Int_t nSyst = SysTags.size();
  Int_t nBkg  = BkgTags.size();
  for(Int_t i = 0; i < nSyst; i ++) if(source == SysTags.at(i)) SysVar.at(i) = value;
  for(Int_t i = 0; i < nBkg ; i ++) if(source == BkgTags.at(i)) BkgUnc.at(i) = value;
}

void CrossSection::SetYield(TString bkg, Float_t value){
  if(notSet) SetMembers();
  if(bkg == signalTag) y = value;
  Int_t nBkg  = BkgTags.size();
  for(Int_t i = 0; i < nBkg ; i ++) if(bkg == BkgTags.at(i)) BkgYield.at(i) = value;
}

Float_t CrossSection::GetYield(TString bkg){
  if(bkg == signalTag) return y;
  Int_t nBkg  = BkgTags.size();
  for(Int_t i = 0; i < nBkg ; i ++) if(bkg == BkgTags.at(i)) return BkgYield.at(i);
  cout << "[CrossSection::GetYield] Error, label not found: " << bkg << endl;
  return 0;
}

Float_t CrossSection::GetUnc(TString source){
  Int_t nSyst = SysTags.size();
  Int_t nBkg  = BkgTags.size();
  for(Int_t i = 0; i < nSyst; i ++) if(source == SysTags.at(i)) return SysVar.at(i);
  for(Int_t i = 0; i < nBkg ; i ++) if(source == BkgTags.at(i)) return BkgUnc.at(i);
  cout << "[CrossSection::GetUnc] Error, label not found: " << source << endl;
  return 0;
}

