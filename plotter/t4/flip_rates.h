#ifndef N_fliprate4t
#define N_fliprate4t 1

float flipRate(float pt, float eta) {
    float scale = 1.;
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.27436e-05*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000181807*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0013915*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 1.84387e-05*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000253821*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00191133*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.41058e-05*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000469437*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00283116*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 2.92427e-05*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000649611*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00411099*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 7.72003e-05*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000851706*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00448173*scale;
   if (pt>=200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 8.51165e-06*scale;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000804451*scale;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00451892*scale;
   return 0.*scale;
}

float flipRateError(float pt, float eta) {
    float scale = 1.;
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 5.22218e-06*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 1.62019e-05*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 4.95368e-05*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.99416e-06*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 1.909e-05*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 6.37224e-05*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 1.59923e-05*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 7.66692e-05*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000229318*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 1.53177e-05*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000168958*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000624686*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 4.91336e-05*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000215157*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000645208*scale;
   if (pt>=200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 8.51302e-06*scale;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000567781*scale;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00236755*scale;
   return 0.*scale;
}

#endif
