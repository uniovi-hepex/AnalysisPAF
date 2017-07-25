#define DEBUGFLAG 
R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)
R__LOAD_LIBRARY(Plot.C+)
R__LOAD_LIBRARY(Datacard.C+)
R__LOAD_LIBRARY(TW/AdditionalStuff.C+)
#include "Histo.h"
#include "Looper.h"
#include "Plot.h"
#include "Datacard.h"
#include "TW/AdditionalStuff.C"

void DrawPlot(TString var, TString cut, TString chan, Int_t nbins, Float_t bin0, Float_t binN, TString Xtitle);
TString pathToTree = "/pool/cienciasrw/userstorage/sscruz/TW/may28/";
TString NameOfTree = "MiniTree";


void doTheCard(){

  // Datacard *d1 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, Mistag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER,pdf,Scale,tWMe_s_cale", "ElMu"); 
  // d1->SetRootFileName("Datacards/forCards_YieldsOnly.root");  

  // d1->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d1->SetNormUnc("NonWZ", 1.50);
  // d1->SetNormUnc("DY"   , 1.10);
  // d1->SetNormUnc("VV"   , 1.10);
  // d1->SetNormUnc("ttV"  , 1.10);

  // d1->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d1->PrintDatacard("datacard_YieldsOnly_ElMu.txt");
  
  // delete d1;


  // Datacard *d2 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, Mistag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER,pdf,Scale,tWMe_s_cale", "ElMu"); 
  // d2->SetRootFileName("Datacards/forCards_With1j1t.root");  

  // d2->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d2->SetNormUnc("NonWZ", 1.50);
  // d2->SetNormUnc("DY"   , 1.10);
  // d2->SetNormUnc("VV"   , 1.10);
  // d2->SetNormUnc("ttV"  , 1.10);

  // d2->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d2->PrintDatacard("datacard_With1j1t_ElMu.txt");
  
  // delete d2;

  // Datacard *d3 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, Mistag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER,pdf,Scale,tWMe_s_cale", "ElMu"); 
  // d3->SetRootFileName("Datacards/forCards_withBothBDTs.root");  

  // d3->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d3->SetNormUnc("NonWZ", 1.50);
  // d3->SetNormUnc("DY"   , 1.10);
  // d3->SetNormUnc("VV"   , 1.10);
  // d3->SetNormUnc("ttV"  , 1.10);

  // d3->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d3->PrintDatacard("datacard_withBothBDTs_ElMu.txt");
  
  // delete d3;


  Datacard *d4 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, Mistag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER,pdf,Scale,tWMe_s_cale", "ElMu"); 
  d4->SetRootFileName("Datacards/forCards_WithEverything.root");  

  d4->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  d4->SetNormUnc("NonWZ", 1.50);
  d4->SetNormUnc("DY"   , 1.10);
  d4->SetNormUnc("VV"   , 1.10);
  d4->SetNormUnc("ttV"  , 1.10);

  d4->SetLumiUnc(1.025); // la de luminosidad, que ahora tampoco esta en Plot.C
  d4->PrintDatacard("datacard_WithEverything_ElMu.txt");
  
  delete d4;



  // Datacard *d_1 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, MEscale, JES, Btag, MisTag, PU,ElecEff,MuonEff,DiagramSubtraction", "ElMu"); 
  // d_1->SetRootFileName("Datacards/forCards_FitWithNJets.root");  

  // d_1->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d_1->SetNormUnc("NonWZ", 1.50);
  // d_1->SetNormUnc("DY"   , 1.50);
  // d_1->SetNormUnc("VV"   , 1.50);
  // d_1->SetNormUnc("ttV"  , 1.50);

  // d_1->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d_1->PrintDatacard("datacard_FitWithPt_ElMu.txt");

  // return;


  // Datacard *d0 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, MEscale, JES, Btag, MisTag, PU,ElecEff,MuonEff", "ElMu"); 
  // d0->SetRootFileName("Datacards/forCards_SimpleFit.root");  

  // d0->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d0->SetNormUnc("NonWZ", 1.50);
  // d0->SetNormUnc("DY"   , 1.50);
  // d0->SetNormUnc("VV"   , 1.50);
  // d0->SetNormUnc("ttV"  , 1.50);

  // d0->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d0->PrintDatacard("datacard_NoBDT_ElMu.txt");

  // return;

  // Datacard *d1 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, MisTag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER", "ElMu"); 
  // d1->SetRootFileName("Datacards/forCards_Nothing.root");  

  // d1->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d1->SetNormUnc("NonWZ", 1.50);
  // d1->SetNormUnc("DY"   , 1.10);
  // d1->SetNormUnc("VV"   , 1.10);
  // d1->SetNormUnc("ttV"  , 1.10);

  // d1->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d1->PrintDatacard("datacard_nothing_ElMu.txt");
  
  // delete d1;


  // Datacard *d2 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, MisTag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER", "ElMu"); 
  // d2->SetRootFileName("Datacards/forCards_Nothing.root");  

  // d2->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d2->SetNormUnc("NonWZ", 1.50);
  // d2->SetNormUnc("DY"   , 1.10);
  // d2->SetNormUnc("VV"   , 1.10);
  // d2->SetNormUnc("ttV"  , 1.10);

  // d2->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d2->PrintDatacard("datacard_Nothing_ElMu.txt");
  
  // delete d2;


  // Datacard *d3 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, JES, Btag, MisTag, PU,ElecEff,MuonEff,DiagramSubtraction,Trig,JER", "ElMu"); 
  // d3->SetRootFileName("Datacards/forCards_SimpleFit.root");  

  // d3->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d3->SetNormUnc("NonWZ", 1.50);
  // d3->SetNormUnc("DY"   , 1.10);
  // d3->SetNormUnc("VV"   , 1.10);
  // d3->SetNormUnc("ttV"  , 1.10);

  // d3->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d3->PrintDatacard("datacard_SimpleFit_ElMu.txt");
  
  // delete d3;
  // Datacard *d2 = new Datacard("tW", "ttV, VV, DY, ttbar,NonWZ", "ue, isr, fsr, hdamp, MEscale, JES, Btag, MisTag, PU,ElecEff,MuonEff", "ElMu"); 
  // d2->SetRootFileName("Datacards/forCards_BDTada.root");  

  // d2->GetParamsFormFile(); // Obtiene la info a partir de los histogramas en el .root
  // d2->SetNormUnc("NonWZ", 1.50);
  // d2->SetNormUnc("DY"   , 1.50);
  // d2->SetNormUnc("VV"   , 1.50);
  // d2->SetNormUnc("ttV"  , 1.50);

  // d2->SetLumiUnc(1.026); // la de luminosidad, que ahora tampoco esta en Plot.C
  // d2->PrintDatacard("datacard_ada_ElMu.txt");


}
