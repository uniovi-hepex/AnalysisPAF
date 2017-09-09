Histo
====
This is just a TH1F with some other parameters...
It has a type (the style depends on the type):
 
    enum types{itBkg, itSignal, itData, itCompare, itSys, nTypes};

If has a color, a name (whatever, for example: "TTbar_Powheg_histo"),
a process (for example, "ttbar" or "VV"), a systematic (example SysTag = "JES"),
and a tag that it is usually tag = process_SysTag 

Looper
====

    Looper *L = new  Looper(TString pathToTree, TString NameOfTree, TString _var = "TMET", TString _cut = "1", TString _chan = "ElMu", Int_t nb = 30, Float_t b0 = 0, Float_t bN = 300)

- var: a variable or formula using branches in the mini tree.
- cut: defines the selection. Uses branches in the mini tree. 
- chan: use a pre-defined channel or define one yourself.
- nb, b0, bN: number of bins, first bin, last bin.
- You can also use b0 as an array defining a not uniform binning.

You can change the default weight (TWeight) and pass a systematic tag
so Looper can craft formulas with the substitution:

    TVar    --> TVarSyst
    TWeight --> TWeight_Syst

And you get the histogram:

    L->GetHisto("NameOfTheSample", "Syst");

Plotter
====
Uses Looper and Histo to create beautiful plots.
    
    Plot *p = new Plot("TVariable", "Cut1 && Cut2", "TChannel == 3", nbins, bin0, binN, "SomeTitle", "Variable [GeV]");
    p->SetPath("path_to_trees");
    p->SetPathSignal("path_to_signal_trees"); // only if it's different from path_to_trees...
    p->SetPathData("path_to_data_trees"); // only if it's different from path_to_trees...
    p->SetTreeName("MiniTreeName");

Then you must add the samples for background (mandatory for stack plots!), signal and data (if you don't use data, do: p->doData = false).

    p->AddSample("WZ, WW, ZZ", "VV", itBkg, kYellow);
    p->AddSample("MuonEG, DoubleMuon", itData);

And add systematics...

    p->AddSystematics("JES, LepEff, PU");

Or from another samples...

    p->AddSample("TTbar_Powheg_SomeSystUp", "ttbar", itSys, kColor, "SomeSystUp");

You can always add histograms that you get from wherever (as signal, systematics, background...).

    Histo* RamdomHisto;
    RandomHisto->SetType(itSignal); RandomHisto->SetColor(kRed); RandomHisto->SetProcess("MySignal");
    p->AddToHistos(RandomHisto);

You have options to set legend, axis, labels... search for them! If you don't find an option.. DO IT YOURSELF!! (implement it carefully, don't be invasive and make it universal!).

    p->DrawStack("name", 1); // 1 --> save it!

To compare samples, add everything as itSignal and use:

    p->DrawComp("name", 1, 1); // Last 1 is for normalizing everything

You can extract yield or systematic tables:

    p->PrintYields("new_cuts", "lables", "channels", "txt, tex, html");

Important to later make datacards:
  
    p->SaveHistograms();

TResultTable
====
Make cute tables in txt, tex, html. It's used in other classes.

Datacards.C
====
It uses a .root created by Plot and generate datacards! Example:

    TString signalName = "T2tt_250_50";
    TString Bkg =      "ttV,  VV,   DY,   tW,   Nonprompt, ttbar";
    TString NormSyst = "1.30, 1.30, 1.15, 1.30, 2.00,      1.053, 1.10"; // the last one is for the signal
    TString Sys = "ue, isr, hdamp, Scale, pdf, JES, Btag, MisTag, LepEff, PU, JER, MET";

    Datacard *d = new Datacard(signal, Bkg, Sys, chan);

    d->AddNuis("TriggerEff"); // You can add another nuisance
    d->SetShape("TriggerEff", false); // You can say if it's a shape or not

    // Load the .root!!!
    d->SetPathToFile("path_to_rootfile");
    d->SetRootFileName("rootfileName.root");
    d->GetParamsFormFile();

    d->SetNuisancesToAllProcesses("TriggerEff", 1.02); // For those that aren't on the rootfile
    d->SetNormUnc(NormSyst); // defined above
    d->SetLumiUnc(1 + 0.024); // For example

    d->PrintDatacard("datacard_nombre.txt"); // Here you have your datacard!

EventDump
====
You know what this is!

    EventDump* e = new EventDump("variable1, variable2, ...", "%1.0f, %9.0f, ...", "TVar > X && TVar2 == Z || ...", "ElMu_or_other_channel");
    e->SetPath("Path_to_mini_tree"); e->SetSampleName("Name_of_sample"); e->SetTreeName("MiniTreeName");
    e->Print("someName"); // It save the output as someName.txt

CrossSection
====
You can obtain cross sections from cut-and-count experiments using this class.
It needs the imputs.. most of them can come from a Plot! It also produces tables with yields and systematic uncertainties on the cross section. 

PDFunc
====
To calculate pdf and scale ME uncertainties using weights. You need to have the histograms with the sum of LHE weights on the mini tree file. There is a really easy way to do so using RunAnalyzer.C.

    PDFunc *pdf = new PDFunc("pathToTree", "sampleName", "miniTreeName", "TCut", "TChannel"); // Inclusive on the region defined by the cut and the channel
    PDFunc *pdf = new PDFunc("pathToTree", "sampleName", "miniTreeName", "TCut", "TChannel", "TVar", nbins, bin0, binN); // Shape

    pdf->GetSystHisto("up",   "pdf"); 
    pdf->GetSystHisto("down", "scale"); 

    for(Int_t i = 0; i < nbins; i++) pdf->PrintLHE(i);
