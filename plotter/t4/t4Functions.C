
TString t4ChannelLabels = "2LSS, *, >= 3L, *, 2LSS + #tau, *, 2LOS+tau, * ";
Int_t GetChannel(Int_t TChannel){
  enum eChannel{iNoChannel, iElMu, iMuon, iElec, i2lss, iTriLep, iFourLep, iSS1tau, iOS1tau, i2lss_fake, iTriLep_fake, iElEl, iMuMu, i1Tau_emufakeOS ,i1Tau_emufakeSS, TotalDefinedChannels};

  Int_t ch = 0;
  if     (TChannel == i2lss)            ch = 1;
  else if(TChannel == i2lss_fake)       ch = 2;
  else if(TChannel == iTriLep)          ch = 3;
  else if(TChannel == iTriLep_fake)     ch = 4;
  else if(TChannel == iSS1tau)          ch = 5;
  else if(TChannel == i1Tau_emufakeSS)  ch = 6;
  else if(TChannel == iOS1tau)          ch = 7;
  else if(TChannel == i1Tau_emufakeOS)  ch = 8;
  return ch;
}
