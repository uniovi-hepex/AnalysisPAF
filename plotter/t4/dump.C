R__LOAD_LIBRARY(EventDump.C+)

//TString baseline = "TPassLowInvMass && TPassTrigger && TPassMETFilters && TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && !TIsOnZ";
TString baseline = "TPassLowInvMass && TPassTrigger && TMET > 50 && TNJets >= 2 && THT > 300 && TNBtags >= 2 && !TIsOnZ"; //TPassMETFilters
TString CRW      = baseline + " && TNSelLeps == 2 && TNBtags == 2 && TNJets <= 5";
// "TPassLowInvMass && TPassTrigger && TMET > 50 && TNJets >= 2 && THT > 300 && !TIsOnZ && TNBtags == 2 && TNJets <= 5"
//TString NoFake   = Form("TChannel == %i || TChannel == %i || TChannel == %i || TChannel == %i", i2lss, iTriLep, iSS1tau, iOS1tau);
TString NoFake   = Form("TChannel == %i || TChannel == %i || TChannel == %i", i2lss, iSS1tau, iOS1tau);

TString scan = "TPassLowInvMass : TPassTrigger : TPassMETFilters : TIsOnZ : TMET : THT : TNJets : TNBtags : TChannel : TNSelLeps : TNFakeableLeps : TNTaus";

TString cut = CRW;
TString chan = NoFake;
TString vars =   "TRun, TLumi, TEvent, TLep_Charge[0], TLep_Pt[0], TLep_Charge[1], TLep_Pt[1], TNJets, TNBtags, TMET, THT";
TString format = "%1.0f , %9.0f , %9.0f\t, %+2d , %5.1f\t, %+2d , %5.1f\t, %1.0f\t, %1.0f\t, %5.1f\t, %6.1f";

//TString cut = "1";
//TString chan = "1";
//TString vars =   "TEvent";
//TString format = "%9.0f";

void dump(){
  EventDump* e = new EventDump(vars, format, cut, chan);
  e->SetPath("/mnt_pool/ciencias_users/user/juanr/AnalysisPAF/tttt_temp/");
  e->SetSampleName("TTWToLNu");
  e->SetTreeName("tree");
  e->Print("dump");
}

//               std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\n",
//                        ss::run(), ss::lumi(), ss::event(),
//                        ss::lep1_id(), lep1_pt,
//                        ss::lep2_id(), lep2_pt,
//                        ss::njets(), ss::nbtags(), ss::met(), ss::ht()) << std::endl;
