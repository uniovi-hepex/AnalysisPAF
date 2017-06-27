

/* These Efficiencies have been derived for Summer17ttbar events and should
be used only for the same MC samples or for events with similar topology */

// NOTE: these are the Summer17 values.
float BTagSFUtil::TagEfficiencyB(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.806989;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.813234;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.768685;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.681559;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.813813;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.814746;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.769026;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.73437;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.828204;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.824643;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.793567;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.752295;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.839489;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.847531;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.81012;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.7903;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.848491;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.856813;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.826937;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.798035;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.858923;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.866237;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.851316;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.817278;
    }
  }

  if (TaggerOP=="CSVv2M") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.611081;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.597251;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.543358;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.437334;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.645602;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.631008;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.577454;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.511005;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.664824;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.651416;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.604285;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.534258;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.676914;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.674082;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.619821;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.55459;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.685307;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.685411;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.619829;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.548674;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.691475;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.693542;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.633184;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.559326;
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.395391;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.356192;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.318148;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.233324;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.444827;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.414585;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.368564;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.303049;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.448746;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.42456;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.388152;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.322599;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.457241;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.439974;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.396907;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.333295;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.469317;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.461467;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.39137;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.325079;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.468426;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.461041;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.39514;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.328143;
    }
  }

   //cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2M") return 0.65;


  if (TaggerOP=="DeepCSVL") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVM") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVT") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  //cout << "BTagSFUtil-B: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}


float BTagSFUtil::TagEfficiencyC(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.350985;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.372822;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.349941;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.324773;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.341656;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.357105;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.348022;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.345883;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.354716;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.371542;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.364287;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.360898;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.362116;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.390369;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.377753;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.376062;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.35821;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.384801;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.387673;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.382912;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.377019;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.406578;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.416886;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.407547;
    }
  }

  if (TaggerOP=="CSVv2M") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.124928;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.119418;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.114293;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0899795;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.112334;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.107526;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.113746;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0994996;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.116726;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.114223;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.120296;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.101958;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.122586;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.123234;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.126938;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.107849;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.128375;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.1354;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.126352;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.113358;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.136771;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.141836;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.145007;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.124911;
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.025748;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0205919;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0223682;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0149293;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0219007;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0191232;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0210321;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0164719;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0196884;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0179063;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0215391;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.017257;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.02300097;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0192056;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0216996;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0182128;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0253434;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0259268;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0228025;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0197575;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.02542;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0263606;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0274619;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0214724;
    }  
  }
  
  //cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2M") return 0.16;


  if (TaggerOP=="DeepCSVL") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVM") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVT") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }  
  }
  

  cout << "BTagSFUtil-C: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}


float BTagSFUtil::TagEfficiencyLight(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0726942;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.101066;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.167475;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.198069;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0435688;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0614851;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0966173;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.143;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.03986;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0560322;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0971611;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.143578;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0404763;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0581904;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.109194;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.161019;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0387242;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.055215;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.115517;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.170317;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.0457645;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0650458;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.134577;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.200187;
    }
  }
  if (TaggerOP=="CSVv2M") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00880905;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.0105732;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0135471;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0155529;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00505528;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00649673;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0095147;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0141587;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00478881;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00606622;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.00854867;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0127676;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00502383;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00650635;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.00962279;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0133854;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00493782;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00657587;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.00928429;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0139263;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00572194;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00780104;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0120215;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.0180973;
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00126163;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00119452;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.0013293;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00170328;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.000653936;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.000689475;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.000729793;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00139157;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.000594018;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.000629331;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.000733599;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00108843;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.00067765;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.000719036;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.000905641;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00131573;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.000742326;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.000878643;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.000849321;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00118062;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.000964704;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.00115364;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.00130939;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.00175038;
    }
  }
  
  //cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2M") return 0.02;

  if (TaggerOP=="DeepCSVL") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVM") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  if (TaggerOP=="DeepCSVT") {
    if (JetPt > 20 && JetPt <= 40){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 40 && JetPt <= 60){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 60 && JetPt <= 80){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 80 && JetPt <= 100){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 100 && JetPt <= 120){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
    else if (JetPt > 120 && JetPt <= 3000){
      if      (fabs(JetEta) > 0   && fabs(JetEta) <= 0.6) return 0.;
      else if (fabs(JetEta) > 0.6 && fabs(JetEta) <= 1.2) return 0.;
      else if (fabs(JetEta) > 1.2 && fabs(JetEta) <= 1.8) return 0.;
      else if (fabs(JetEta) > 1.8 && fabs(JetEta) <= 2.4) return 0.;
    }
  }

  cout << "BTagSFUtil-Light: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}
