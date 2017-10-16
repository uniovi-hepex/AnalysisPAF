R__LOAD_LIBRARY(Histo.C+)
R__LOAD_LIBRARY(Looper.C+)

TLegend l = new TLegend(0.65, 0.72, 0.9, 0.9);
l->AddEntry(h, "FastSim [225, 50]", "l");
l->SetTextSize(0.045);



