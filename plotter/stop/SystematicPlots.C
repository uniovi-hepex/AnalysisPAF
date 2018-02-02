//##################################################################################################################    
// 
// Print plots with nominal and systematic variations for ttbar sample
//    root -l -b -1 'SystematicPlots("ElMu")'
//
//##################################################################################################################    
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "PDFunc.C"

//=== Functions
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name, Int_t pl);
void DrawComp(TString systName, Bool_t VariedSample = false);

//=== Constants
TString pathToTree = "/pool/ciencias/userstorage/juanr/stop/jan5/";
TString NameOfTree = "tree";
TString outputFolder = "./output/";
TString BaselineCut = "TNJets >= 2 && TNBtags >= 1 && !TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
TString Dilepton    = "!TIsSS && TPassTrigger && TPassMETfilters && TNSelLeps == 2 && TLep0_Pt >= 25";
const Int_t ngbins = 26;
const Int_t ngbins21 = 21;
Float_t gbins[ngbins+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,140,200};
Float_t gbins21[ngbins21+1] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,200};
const TString ttbar = "TTbar_Powheg";
Plot* p = nullptr;
PDFunc *pdf = nullptr;

//=== Systematics
enum Syst{kMuon, kTrig, kJES, kBtag, kElec, kPU, kJER, kUE, kISR, khdamp, kME, kPDF, kCR, kMass, kFSR, kTopPt, kUnclustMET, kMuScale, kElScale, nSyst}; 

//=== Main function
void SystematicPlots(TString chan = "ElMu"){
  //DrawPlot("TMT2", BaselineCut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2", kTopPt);
  //DrawPlot("TMT2", BaselineCut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2", kMass);
  //DrawPlot("TMT2", BaselineCut, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2", kMass);
  //for(Int_t pl = 0; pl < nSyst; pl++) DrawPlot("TMT2", Dilepton, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2_dilep", kBtag);
  //for(Int_t pl = 0; pl < nSyst; pl++) DrawPlot("TMT2", Dilepton, chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2_dilep", pl);
//for(Int_t pl = 0; pl < nSyst; pl++) DrawPlot("TMT2", BaselineCut, chan, ngbins21, 0, 0, gbins21, "M_{T2} [GeV]", "MT2", pl);
  //DrawPlot("TMT2", BaselineCut, chan, ngbins21, 0, 0, gbins21, "M_{T2} [GeV]", "MT2", kUnclustMET);
  DrawPlot("TMET", BaselineCut, chan, 20, 0, 200, 0, "MET [GeV]", "MET", kUnclustMET);
  //for(Int_t pl = 0; pl < nSyst; pl++) DrawPlot("TMT2", BaselineCut + " && TMT2 > 0", chan, ngbins, 0, 0, gbins, "M_{T2} [GeV]", "MT2no0", pl);
}

//=== Plotting function
void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, Float_t* bins, TString Xtitle, TString name, Int_t pl){
  if(bin0 == binN) p = new Plot(var, cut, chan, nbins, bins,       "Title", Xtitle);
  else             p = new Plot(var, cut, chan, nbins, bin0, binN, "Title", Xtitle);
  p->SetPath(pathToTree); p->SetTreeName(NameOfTree);
  p->SetOutputName("");
  p->SetPlotFolder("output/Systematics/");
  p->verbose = false;  p->doYieldsInLeg = false;
  p->SetLegendTextSize(0.045); p->SetLegendPosition(0.60, 0.70, 0.93, 0.93);
  p->SetVarName(name);

  //>>> Ratio range for each syst variation
  Float_t RatioMax = 1.2; Float_t RatioMin = 0.8;
  if(pl == kCR || pl == kTrig || pl == kElec || pl == kMuon || pl == kJER || pl == kPDF){ RatioMax = 1.03; RatioMin = 0.97;} 
  if(pl == kBtag){ RatioMax = 1.05; RatioMin = 0.95;} 
  if(pl == kJES || pl == kPU || pl == kUE || pl == khdamp || pl == kUnclustMET){ RatioMax = 1.1; RatioMin = 0.9;} 
  if(pl == kMuScale || pl == kElScale){ RatioMax = 1.02; RatioMin = 0.98;}
  p->SetRatioMin(RatioMin); p->SetRatioMax(RatioMax);

  //>>> Add nominal samples
  p->AddSample("TTbar_Powheg", "Nominal", itSignal, 1);

  //>>> The different systematics...
  if(pl == kTrig)   DrawComp("Trig");
  if(pl == kJES)    DrawComp("JES");
  if(pl == kElec)   DrawComp("ElecEff");
  if(pl == kPU)     DrawComp("PU");
  if(pl == kMuScale)     DrawComp("MuScale");
  if(pl == kElScale)     DrawComp("ElScale");

  if(pl == khdamp)  DrawComp("hdamp", true);
  if(pl == kUE)     DrawComp("ue", true);
  if(pl == kISR)    DrawComp("isr", true);

  if(pl == kUnclustMET){
    p->SetWeight("TWeight*GetMetUnclWeightUp(TMET)");
    p->AddSample(ttbar, "ttbarUnclMETUp",   itSignal, kGreen+1, "Uncl MET Up");
    p->SetWeight("TWeight*GetMetUnclWeightDown(TMET)");
    p->AddSample(ttbar, "ttbarUnclMETDown", itSignal, kRed+1, "Uncl MET Down");
    p->doSetLogy = false;
    p->DrawComp("UnclMET");
  }

  if(pl == kMuon){
    p->AddSample(ttbar, "ttbarMUP", itSignal, kGreen+1, "MuonEffUp");
    p->AddSample(ttbar, "ttbarMDO", itSignal, kRed+1,   "MuonEffDown");
    float fac = sqrt(2 + 0.25 + 0.25);
    p->ScaleProcess("ttbarMUP",  1 + fac/100);
    p->ScaleProcess("ttbarMDO",  1 - fac/100);
    p->DrawComp("MuonEff");
  }

  //>>> Btag includes mistag
  if(pl == kBtag){
    p->AddSample("TTbar_Powheg", "ttbar", itSignal, kGreen+1, "BtagUp");
    p->AddSample("TTbar_Powheg", "ttbar", itSignal, kRed+1,  "BtagDown");
    p->AddSample("TTbar_Powheg", "ttbar", itSignal, kBlue+1, "MisTagUp");
    p->AddSample("TTbar_Powheg", "ttbar", itSignal, kPink, "MisTagDown");
    p->DrawComp("Btag");
  }

  // JER is only one variation... and the symmetric
  if(pl == kJER){
    p->GetHisto("Nominal")->SetProcess("ttbar");
    p->AddSample("TTbar_Powheg", "ttbar", itSignal, kGreen+1, "JERUp");
    Histo* hJERDown = (Histo*) p->GetSymmetricHisto("ttbar", "JERUp")->CloneHisto("ttbar_JERDown");
    //hJERDown->SetTag("ttbar_JERDown"); hJERDown->SetSysTag("JERDown");
    hJERDown->SetColor(kRed+1);
    //hJERDown->SetStyle();
    p->AddToHistos(hJERDown);
    p->DrawComp("JER");
  }

  if(pl == kFSR){
    TString pathToFSRcorrSF = "/nfs/fanae/user/juanr/AnalysisPAF/plotter/top/FSRcorr/SF/";
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Up",   "FSR_btagSF_Up");
    AddFRhisto1D(pathToFSRcorrSF + "FSR_BtagSF.root" , "FSR_btagSF_Down", "FSR_btagSF_Down");
    p->SetWeight("TWeight*GetFSR_BtagSF_Up(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrUp",     "ttbar FSR Up", itSignal, kGreen+1, "fsrUp"    );
    p->SetWeight("TWeight*GetFSR_BtagSF_Down(TBtagPt)");
    p->AddSample("TTbar_Powheg_fsrDown",   "ttbar FSR Down", itSignal, kRed+1, "fsrDown"  );

    Float_t n  = p->GetYield("Nominal"); 
    Float_t vu = p->GetYield("ttbar FSR Up");
    Float_t vd = p->GetYield("ttbar FSR Down");
    p->ScaleProcess("ttbar FSR Up",    GetFactorFSR(n, vu));
    p->ScaleProcess("ttbar FSR Down",  GetFactorFSR(n, vd));
    p->DrawComp("FSR");
  }

  if(pl == kTopPt){
    p->SetWeight("GetTopPtWeight(TgenTop1Pt, TgenTop2Pt)*TWeight");
    p->AddSample("TTbar_Powheg","ttbar TopPt reweighted", itSignal, kRed+1, "TopPtUp");
    p->SetWeight("TWeight");
    p->DrawComp("TopPt");
  }

  if(pl == kMass){
    p->AddSample("TTbar_Powheg_mtop1715", "ttbar m_{t} = 171.5 GeV", itSignal, kRed+1, "topMassDown");
    p->AddSample("TTbar_Powheg_mtop1735", "ttbar m_{t} = 173.5 GeV", itSignal, kGreen+1, "topMassUp");
    p->DrawComp("TopMass");
  }
  
  //>>> Color Reconnection
  if(pl == kCR){
    p->AddSample("TTbar_Powheg_erdON", "ttbar", itSys, 1, "Powheg_erdON");
    p->AddSample("TTbar_QCDbasedCRTune_erdON", "ttbar", itSys, 1, "QCDbasedCRTune_erdON");
    p->AddSample("TTbar_GluonMoveCRTune", "ttbar", itSys, 1, "GluonMoveCRTune");
    p->AddSample("TTbar_GluonMoveCRTune_erdON", "ttbar", itSys, 1, "GluonMoveCRTune_erdON");
    p->GetHisto("Nominal")->SetProcess("ttbar"); 

    p->UseEnvelope("ttbar", "Powheg_erdON, QCDbasedCRTune_erdON, GluonMoveCRTune, GluonMoveCRTune_erdON", "CR");
    Histo* hCRUp = p->GetHisto("ttbar", "CRUp")->CloneHisto("ttbar_CRUp");
    Histo* hCRDo = p->GetHisto("ttbar", "CRDown")->CloneHisto("ttbar_CRDo");
    hCRUp->SetType(itSignal); hCRDo->SetType(itSignal);
    hCRUp->SetStyle(); hCRDo->SetStyle();
    hCRUp->SetColor(kGreen+1); hCRDo->SetColor(kRed+1);
    p->AddToHistos(hCRUp); p->AddToHistos(hCRDo);
    p->DrawComp("CR");
  }


  // The PDF and Scale ME variations
  if(pl == kPDF || pl == kME){
    if(bin0 == binN) pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bins, "");
    else             pdf = new PDFunc(pathToTree, "TTbar_Powheg", NameOfTree, cut, chan, var, nbins, bin0, binN, ""); 

    Histo* hPDFUp   =  pdf->GetSystHisto("up",   "pdf")->CloneHisto("ttbar_PDFUp");
    Histo* hPDFDown =  pdf->GetSystHisto("down", "pdf")->CloneHisto("ttbar_PDFDown");
    hPDFUp->SetDirectory(0); hPDFDown->SetDirectory(0);
    Histo* hMEUp    =  pdf->GetSystHisto("up",   "ME")->CloneHisto("ttbar_Scale_ME_Up");
    Histo* hMEDown  =  pdf->GetSystHisto("down", "ME")->CloneHisto("ttbar_Scale_ME_Down");
    hPDFUp->SetProcess("ttbar"); hPDFDown->SetProcess("ttbar"); hMEUp->SetProcess("ttbar"); hMEDown->SetProcess("ttbar");
    hPDFUp->SetSysTag("PDFUp"); hPDFDown->SetSysTag("PDFDown"); hMEUp->SetSysTag("Scale_ME_Up");  hMEDown->SetSysTag("Scale_ME_Down");
    hPDFUp->SetTag("ttbar_PDFUp"); hPDFDown->SetTag("ttbar_PDFDown"); hMEUp->SetTag("ttbar_Scale_ME_Up"); hMEDown->SetTag("ttbar_Scale_ME_Down");
    hPDFUp->SetName("ttbar_PDFUp"); hPDFDown->SetName("ttbar_PDFDown"); hMEUp->SetName("ttbar_Scale_ME_Up"); hMEDown->SetName("ttbar_Scale_ME_Down");
    hPDFUp->SetType(itSignal); hPDFDown->SetType(itSignal); hMEUp->SetType(itSignal); hMEDown->SetType(itSignal);
    hPDFUp->SetStyle(); hPDFDown->SetStyle(); hMEUp->SetStyle(); hMEDown->SetStyle();
    hPDFUp->SetColor(kGreen+1); hPDFDown->SetColor(kRed+1); hMEUp->SetColor(kGreen+1); hMEDown->SetColor(kRed+1);

    if(pl == kPDF){p->AddToHistos(hPDFUp); p->AddToHistos(hPDFDown); p->DrawComp("PDF");     }
    else{          p->AddToHistos(hMEUp);  p->AddToHistos(hMEDown);  p->DrawComp("ScaleME"); }
  }

  if(p) delete p;
  return;
}

void DrawComp(TString systName, Bool_t VariedSample){
  TString samplename = ttbar;
  p->SetWeight("TWeight");
  if(VariedSample){
    p->AddSample(ttbar + "_" + systName + "Up",   systName + "Up",   itSignal, kGreen+1);
    p->AddSample(ttbar + "_" + systName + "Down", systName + "Down", itSignal, kRed+1);
  }
  else{
    p->AddSample(ttbar, "ttbar", itSignal, kGreen+1, systName+"Up");
    p->AddSample(ttbar, "ttbar", itSignal, kRed+1,   systName+"Down");
  }
  p->DrawComp(systName,1);
}


