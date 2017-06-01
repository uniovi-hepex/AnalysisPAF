double Ele23_Ele12_LeadingLeg_SF(double pt, double eta){
  double abseta =abs(eta);
  if(pt<25) return 0.;
  if(abseta<0) return 0.;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.99494;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.993074;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.993714;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.951615;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.988346;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.995542;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 0.995095;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 0.981974;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.98789;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.993678;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.995426;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 0.991292;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.986247;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 0.993639;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 0.997055;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.995389;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.985234;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.990968;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 0.996402;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 0.994802;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.98497;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.989587;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.997632;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 0.996236;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.98513;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.987974;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.997332;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.994494;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.978954;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.988632;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 0.999226;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 0.997519;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.977597;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 0.988665;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 1.00744;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.977597;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 0.988665;
  if(pt >500&& abseta>1.479&& abseta<=2) return 1.00744;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
}

double Ele23_Ele12_TrailingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<15) return 0.;
  if(abseta<0) return 0.;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.955448;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.479) return 0.976347;
  if(pt >15&& pt<=20&& abseta>1.479&& abseta<=2) return 0.962857;
  if(pt >15&& pt<=20&& abseta>2&& abseta<=2.5) return 0.881956;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.986712;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.479) return 0.990492;
  if(pt >20&& pt<=25&& abseta>1.479&& abseta<=2) return 0.989844;
  if(pt >20&& pt<=25&& abseta>2&& abseta<=2.5) return 0.932416;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.995855;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.996545;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.996394;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.965732;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.988417;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.995617;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 0.994888;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 0.982279;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.987882;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.993693;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.995439;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 0.991451;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.986248;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 0.993635;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 0.997055;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.995465;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.985234;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.990966;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 0.996415;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 0.994838;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.98497;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.989587;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.997632;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 0.996236;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.98513;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.987974;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.997332;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.994494;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.978954;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.988632;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 0.999226;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 0.997519;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.977597;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 0.988665;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 1.00744;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.977597;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 0.988665;
  if(pt >500&& abseta>1.479&& abseta<=2) return 1.00744;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
  
}

double Ele23_Ele12_DZLeg(double eta1, double eta2){

  if(eta1 < eta2) {
    double buf =eta1;
    eta1=eta2;
    eta2=buf;
  }

  if(abs(eta1)>2.5) return 0.;
  if(abs(eta2)>2.5) return 0.;
  if(eta1 >-2.5&& eta1<=-2&& eta2>-2.5&& eta2<=-2) return 0.972408;
  if(eta1 >-2&& eta1<=-1.5&& eta2>-2.5&& eta2<=-2) return 0.976071;
  if(eta1 >-2&& eta1<=-1.5&& eta2>-2&& eta2<=-1.5) return 0.979875;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-2.5&& eta2<=-2) return 0.982069;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-2&& eta2<=-1.5) return 0.986621;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-1.5&& eta2<=-1) return 0.99242;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-2.5&& eta2<=-2) return 0.983559;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-2&& eta2<=-1.5) return 0.987892;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-1.5&& eta2<=-1) return 0.994891;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-1&& eta2<=-0.5) return 0.996419;
  if(eta1 >-0.5&& eta1<=0&& eta2>-2.5&& eta2<=-2) return 0.981451;
  if(eta1 >-0.5&& eta1<=0&& eta2>-2&& eta2<=-1.5) return 0.987607;
  if(eta1 >-0.5&& eta1<=0&& eta2>-1.5&& eta2<=-1) return 0.994967;
  if(eta1 >-0.5&& eta1<=0&& eta2>-1&& eta2<=-0.5) return 0.996985;
  if(eta1 >-0.5&& eta1<=0&& eta2>-0.5&& eta2<=0) return 0.997191;
  if(eta1 >0&& eta1<=0.5&& eta2>-2.5&& eta2<=-2) return 0.984523;
  if(eta1 >0&& eta1<=0.5&& eta2>-2&& eta2<=-1.5) return 0.987602;
  if(eta1 >0&& eta1<=0.5&& eta2>-1.5&& eta2<=-1) return 0.994738;
  if(eta1 >0&& eta1<=0.5&& eta2>-1&& eta2<=-0.5) return 0.996844;
  if(eta1 >0&& eta1<=0.5&& eta2>-0.5&& eta2<=0) return 0.997149;
  if(eta1 >0&& eta1<=0.5&& eta2>0&& eta2<=0.5) return 0.997427;
  if(eta1 >0.5&& eta1<=1&& eta2>-2.5&& eta2<=-2) return 0.988543;
  if(eta1 >0.5&& eta1<=1&& eta2>-2&& eta2<=-1.5) return 0.988847;
  if(eta1 >0.5&& eta1<=1&& eta2>-1.5&& eta2<=-1) return 0.99475;
  if(eta1 >0.5&& eta1<=1&& eta2>-1&& eta2<=-0.5) return 0.997263;
  if(eta1 >0.5&& eta1<=1&& eta2>-0.5&& eta2<=0) return 0.997527;
  if(eta1 >0.5&& eta1<=1&& eta2>0&& eta2<=0.5) return 0.997728;
  if(eta1 >0.5&& eta1<=1&& eta2>0.5&& eta2<=1) return 0.998355;
  if(eta1 >1&& eta1<=1.5&& eta2>-2.5&& eta2<=-2) return 0.963636;
  if(eta1 >1&& eta1<=1.5&& eta2>-2&& eta2<=-1.5) return 0.987676;
  if(eta1 >1&& eta1<=1.5&& eta2>-1.5&& eta2<=-1) return 0.993714;
  if(eta1 >1&& eta1<=1.5&& eta2>-1&& eta2<=-0.5) return 0.995661;
  if(eta1 >1&& eta1<=1.5&& eta2>-0.5&& eta2<=0) return 0.995514;
  if(eta1 >1&& eta1<=1.5&& eta2>0&& eta2<=0.5) return 0.996184;
  if(eta1 >1&& eta1<=1.5&& eta2>0.5&& eta2<=1) return 0.996245;
  if(eta1 >1&& eta1<=1.5&& eta2>1&& eta2<=1.5) return 0.994036;
  if(eta1 >1.5&& eta1<=2&& eta2>-2.5&& eta2<=-2) return 0.972222;
  if(eta1 >1.5&& eta1<=2&& eta2>-2&& eta2<=-1.5) return 0.968254;
  if(eta1 >1.5&& eta1<=2&& eta2>-1.5&& eta2<=-1) return 0.987764;
  if(eta1 >1.5&& eta1<=2&& eta2>-1&& eta2<=-0.5) return 0.989606;
  if(eta1 >1.5&& eta1<=2&& eta2>-0.5&& eta2<=0) return 0.990423;
  if(eta1 >1.5&& eta1<=2&& eta2>0&& eta2<=0.5) return 0.989735;
  if(eta1 >1.5&& eta1<=2&& eta2>0.5&& eta2<=1) return 0.990882;
  if(eta1 >1.5&& eta1<=2&& eta2>1&& eta2<=1.5) return 0.987764;
  if(eta1 >1.5&& eta1<=2&& eta2>1.5&& eta2<=2) return 0.983886;
  if(eta1 >2&& eta1<=2.5&& eta2>-2.5&& eta2<=-2) return 1.;
  if(eta1 >2&& eta1<=2.5&& eta2>-2&& eta2<=-1.5) return 0.935484;
  if(eta1 >2&& eta1<=2.5&& eta2>-1.5&& eta2<=-1) return 1.;
  if(eta1 >2&& eta1<=2.5&& eta2>-1&& eta2<=-0.5) return 0.984919;
  if(eta1 >2&& eta1<=2.5&& eta2>-0.5&& eta2<=0) return 0.982533;
  if(eta1 >2&& eta1<=2.5&& eta2>0&& eta2<=0.5) return 0.985398;
  if(eta1 >2&& eta1<=2.5&& eta2>0.5&& eta2<=1) return 0.985532;
  if(eta1 >2&& eta1<=2.5&& eta2>1&& eta2<=1.5) return 0.984224;
  if(eta1 >2&& eta1<=2.5&& eta2>1.5&& eta2<=2) return 0.978444;
  if(eta1 >2&& eta1<=2.5&& eta2>2&& eta2<=2.5) return 0.970051;
  return 0.;
}

  
double Mu17_Mu8orTkMu8_LeadingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<25) return 0.;
  if(abseta<0) return 0.;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.975885;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.976515;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 0.998967;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.976116;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.940206;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.975675;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.975927;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 0.998962;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.975763;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 0.943915;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.975037;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.973992;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 0.998333;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.976402;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 0.950738;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.974536;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.974686;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 0.998161;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.976752;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.953636;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.974515;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.974129;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 0.998451;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.97733;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.95698;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.974183;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.976537;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 0.998544;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.976648;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 0.964094;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.972806;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.974981;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 0.989306;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.974255;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.958616;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.966852;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.971836;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 0.96698;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 0.971697;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.98064;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.968627;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.941135;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 0.975559;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 0.975005;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 1.14123;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.968627;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.941135;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 0.975559;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 0.975005;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 1.14123;
  return 0.;
  
}
double Mu17_Mu8orTkMu8_TrailingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<10) return 0.;
  if(abseta<0) return 0.;
  if(pt >10&& pt<=15&& abseta>0&& abseta<=0.8) return 0.970129;
  if(pt >10&& pt<=15&& abseta>0.8&& abseta<=1.25) return 0.985147;
  if(pt >10&& pt<=15&& abseta>1.25&& abseta<=1.6) return 1.00669;
  if(pt >10&& pt<=15&& abseta>1.6&& abseta<=2.1) return 0.984445;
  if(pt >10&& pt<=15&& abseta>2.1&& abseta<=2.4) return 0.987653;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.973804;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.25) return 0.987493;
  if(pt >15&& pt<=20&& abseta>1.25&& abseta<=1.6) return 1.00677;
  if(pt >15&& pt<=20&& abseta>1.6&& abseta<=2.1) return 0.983937;
  if(pt >15&& pt<=20&& abseta>2.1&& abseta<=2.4) return 0.985672;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.977614;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.25) return 0.987096;
  if(pt >20&& pt<=25&& abseta>1.25&& abseta<=1.6) return 1.00741;
  if(pt >20&& pt<=25&& abseta>1.6&& abseta<=2.1) return 0.985597;
  if(pt >20&& pt<=25&& abseta>2.1&& abseta<=2.4) return 0.985495;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.979366;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.988048;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 1.00886;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.988311;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.987578;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.978944;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.98732;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 1.00867;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.986726;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 0.986462;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.978393;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.985674;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 1.00801;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.986386;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 0.987331;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.977731;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.986233;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 1.00816;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.985049;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.987037;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.977879;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.985104;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 1.00816;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.985156;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.985738;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.978618;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.986432;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 1.00971;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.984776;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 0.986632;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.97712;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.986563;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 1.00593;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.983988;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.981772;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.970541;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.993899;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 0.998965;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 0.981334;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.9877;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.971879;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.964478;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 1.01378;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 0.993002;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 1.10365;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.971879;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.964478;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 1.01378;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 0.993002;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 1.10365;
  return 0.;
  
}

double Mu17_Mu8orTkMu8_DZLeg(double eta1, double eta2){

  if(eta1 < eta2) {
    double buf =eta1;
    eta1=eta2;
    eta2=buf;
  }
  if(abs(eta1)>2.5) return 0.;
  if(abs(eta2)>2.5) return 0.;
  
  if(eta1 >-2.5&& eta1<=-2&& eta2>-2.5&& eta2<=-2) return 0.972707;
  if(eta1 >-2&& eta1<=-1.5&& eta2>-2.5&& eta2<=-2) return 0.975146;
  if(eta1 >-2&& eta1<=-1.5&& eta2>-2&& eta2<=-1.5) return 0.972411;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-2.5&& eta2<=-2) return 0.973371;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-2&& eta2<=-1.5) return 0.972947;
  if(eta1 >-1.5&& eta1<=-1&& eta2>-1.5&& eta2<=-1) return 0.978426;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-2.5&& eta2<=-2) return 0.97995;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-2&& eta2<=-1.5) return 0.981893;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-1.5&& eta2<=-1) return 0.9832;
  if(eta1 >-1&& eta1<=-0.5&& eta2>-1&& eta2<=-0.5) return 0.987504;
  if(eta1 >-0.5&& eta1<=0&& eta2>-2.5&& eta2<=-2) return 0.983911;
  if(eta1 >-0.5&& eta1<=0&& eta2>-2&& eta2<=-1.5) return 0.983727;
  if(eta1 >-0.5&& eta1<=0&& eta2>-1.5&& eta2<=-1) return 0.983577;
  if(eta1 >-0.5&& eta1<=0&& eta2>-1&& eta2<=-0.5) return 0.988804;
  if(eta1 >-0.5&& eta1<=0&& eta2>-0.5&& eta2<=0) return 0.987586;
  if(eta1 >0&& eta1<=0.5&& eta2>-2.5&& eta2<=-2) return 0.977957;
  if(eta1 >0&& eta1<=0.5&& eta2>-2&& eta2<=-1.5) return 0.976627;
  if(eta1 >0&& eta1<=0.5&& eta2>-1.5&& eta2<=-1) return 0.97841;
  if(eta1 >0&& eta1<=0.5&& eta2>-1&& eta2<=-0.5) return 0.982161;
  if(eta1 >0&& eta1<=0.5&& eta2>-0.5&& eta2<=0) return 0.982768;
  if(eta1 >0&& eta1<=0.5&& eta2>0&& eta2<=0.5) return 0.976456;
  if(eta1 >0.5&& eta1<=1&& eta2>-2.5&& eta2<=-2) return 0.981454;
  if(eta1 >0.5&& eta1<=1&& eta2>-2&& eta2<=-1.5) return 0.974367;
  if(eta1 >0.5&& eta1<=1&& eta2>-1.5&& eta2<=-1) return 0.980125;
  if(eta1 >0.5&& eta1<=1&& eta2>-1&& eta2<=-0.5) return 0.983967;
  if(eta1 >0.5&& eta1<=1&& eta2>-0.5&& eta2<=0) return 0.985229;
  if(eta1 >0.5&& eta1<=1&& eta2>0&& eta2<=0.5) return 0.979161;
  if(eta1 >0.5&& eta1<=1&& eta2>0.5&& eta2<=1) return 0.979705;
  if(eta1 >1&& eta1<=1.5&& eta2>-2.5&& eta2<=-2) return 0.9875;
  if(eta1 >1&& eta1<=1.5&& eta2>-2&& eta2<=-1.5) return 0.983796;
  if(eta1 >1&& eta1<=1.5&& eta2>-1.5&& eta2<=-1) return 0.977081;
  if(eta1 >1&& eta1<=1.5&& eta2>-1&& eta2<=-0.5) return 0.984437;
  if(eta1 >1&& eta1<=1.5&& eta2>-0.5&& eta2<=0) return 0.980074;
  if(eta1 >1&& eta1<=1.5&& eta2>0&& eta2<=0.5) return 0.975956;
  if(eta1 >1&& eta1<=1.5&& eta2>0.5&& eta2<=1) return 0.975936;
  if(eta1 >1&& eta1<=1.5&& eta2>1&& eta2<=1.5) return 0.975914;
  if(eta1 >1.5&& eta1<=2&& eta2>-2.5&& eta2<=-2) return 1.;
  if(eta1 >1.5&& eta1<=2&& eta2>-2&& eta2<=-1.5) return 0.984211;
  if(eta1 >1.5&& eta1<=2&& eta2>-1.5&& eta2<=-1) return 0.984632;
  if(eta1 >1.5&& eta1<=2&& eta2>-1&& eta2<=-0.5) return 0.984092;
  if(eta1 >1.5&& eta1<=2&& eta2>-0.5&& eta2<=0) return 0.980701;
  if(eta1 >1.5&& eta1<=2&& eta2>0&& eta2<=0.5) return 0.976597;
  if(eta1 >1.5&& eta1<=2&& eta2>0.5&& eta2<=1) return 0.978571;
  if(eta1 >1.5&& eta1<=2&& eta2>1&& eta2<=1.5) return 0.975928;
  if(eta1 >1.5&& eta1<=2&& eta2>1.5&& eta2<=2) return 0.979104;
  if(eta1 >2&& eta1<=2.5&& eta2>-2.5&& eta2<=-2) return 0.8;
  if(eta1 >2&& eta1<=2.5&& eta2>-2&& eta2<=-1.5) return 0.979167;
  if(eta1 >2&& eta1<=2.5&& eta2>-1.5&& eta2<=-1) return 0.977823;
  if(eta1 >2&& eta1<=2.5&& eta2>-1&& eta2<=-0.5) return 0.988174;
  if(eta1 >2&& eta1<=2.5&& eta2>-0.5&& eta2<=0) return 0.983944;
  if(eta1 >2&& eta1<=2.5&& eta2>0&& eta2<=0.5) return 0.97293;
  if(eta1 >2&& eta1<=2.5&& eta2>0.5&& eta2<=1) return 0.976471;
  if(eta1 >2&& eta1<=2.5&& eta2>1&& eta2<=1.5) return 0.974463;
  if(eta1 >2&& eta1<=2.5&& eta2>1.5&& eta2<=2) return 0.976945;
  if(eta1 >2&& eta1<=2.5&& eta2>2&& eta2<=2.5) return 0.977016;
  if(eta1 >2.5&& eta2>-2.5&& eta2<=-2) return 0.8;
  if(eta1 >2.5&& eta2>-2&& eta2<=-1.5) return 0.979167;
  if(eta1 >2.5&& eta2>-1.5&& eta2<=-1) return 0.977823;
  if(eta1 >2.5&& eta2>-1&& eta2<=-0.5) return 0.988174;
  if(eta1 >2.5&& eta2>-0.5&& eta2<=0) return 0.983944;
  if(eta1 >2.5&& eta2>0&& eta2<=0.5) return 0.97293;
  if(eta1 >2.5&& eta2>0.5&& eta2<=1) return 0.976471;
  if(eta1 >2.5&& eta2>1&& eta2<=1.5) return 0.974463;
  if(eta1 >2.5&& eta2>1.5&& eta2<=2) return 0.976945;
  if(eta1 >2.5&& eta2>2&& eta2<=2.5) return 0.977016;
  return 0.;

}


double Mu23_Ele8_LeadingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<25) return 0.;
  if(abseta<0) return 0.;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.983791;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.958447;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 1.04024;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.971534;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.866128;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.974403;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.954707;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 1.00923;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.961557;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 0.912399;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.978558;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.964189;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 1.03854;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.973187;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 0.981393;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.983556;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.963628;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 1.01934;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.98513;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.926148;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.974924;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.964003;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 1.00468;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.973593;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.967389;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.974253;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.95519;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 0.963482;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.974624;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 0.958616;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.977996;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.951171;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 0.931739;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.971829;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.946525;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.949255;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.919107;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 0.913977;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 1.00784;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.766897;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.950913;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.843843;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 0.762691;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 1.03728;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 0.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.950913;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.843843;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 0.762691;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 1.03728;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 0.;
  return 0.;
}

double Mu23_Ele8_TrailingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<15) return 0.;
  if(abseta<0) return 0.;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.962296;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.479) return 0.97406;
  if(pt >15&& pt<=20&& abseta>1.479&& abseta<=2) return 1.00345;
  if(pt >15&& pt<=20&& abseta>2&& abseta<=2.5) return 0.886592;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.982786;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.479) return 0.989526;
  if(pt >20&& pt<=25&& abseta>1.479&& abseta<=2) return 0.995562;
  if(pt >20&& pt<=25&& abseta>2&& abseta<=2.5) return 0.958201;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.983073;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.989045;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.994472;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.974094;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.97675;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.988575;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 0.999878;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 0.977327;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.982799;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.987438;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.989576;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 1.00861;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.982149;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 0.998018;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 1.04788;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.992378;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.988253;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.992617;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 1.00104;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 0.999735;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.985853;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.988278;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.998418;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 0.998114;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.990736;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.989388;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.996331;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.994522;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.977036;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.980719;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 1.00015;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 0.983871;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.968791;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 0.973439;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 1.00055;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.968791;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 0.973439;
  if(pt >500&& abseta>1.479&& abseta<=2) return 1.00055;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
}


double Mu8_Ele23_LeadingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<25) return 0.;
  if(abseta<0) return 0.;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.996752;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.977666;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.96467;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.950112;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 1.00261;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.996632;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 0.991572;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 0.994238;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.999469;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.998148;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.987747;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 0.9944;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.98889;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 1.00266;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 1.04457;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.983896;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.987807;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.991549;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 0.999183;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 0.99376;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.98497;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.98699;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.996359;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 0.992529;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.989804;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.988907;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.994665;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.993534;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.975237;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.980719;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 1.00015;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 0.983871;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.968791;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 0.973439;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 1.00055;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.968791;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 0.973439;
  if(pt >500&& abseta>1.479&& abseta<=2) return 1.00055;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
  
}
double Mu8_Ele23_TrailingLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<10) return 0.;
  if(abseta<0) return 0.;
  if(pt >10&& pt<=15&& abseta>0&& abseta<=0.8) return 0.969544;
  if(pt >10&& pt<=15&& abseta>0.8&& abseta<=1.25) return 0.987007;
  if(pt >10&& pt<=15&& abseta>1.25&& abseta<=1.6) return 1.06007;
  if(pt >10&& pt<=15&& abseta>1.6&& abseta<=2.1) return 0.973832;
  if(pt >10&& pt<=15&& abseta>2.1&& abseta<=2.4) return 0.939214;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.976972;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.25) return 0.969411;
  if(pt >15&& pt<=20&& abseta>1.25&& abseta<=1.6) return 1.03445;
  if(pt >15&& pt<=20&& abseta>1.6&& abseta<=2.1) return 0.968881;
  if(pt >15&& pt<=20&& abseta>2.1&& abseta<=2.4) return 0.931032;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.975506;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.25) return 0.972312;
  if(pt >20&& pt<=25&& abseta>1.25&& abseta<=1.6) return 1.03867;
  if(pt >20&& pt<=25&& abseta>1.6&& abseta<=2.1) return 0.97625;
  if(pt >20&& pt<=25&& abseta>2.1&& abseta<=2.4) return 1.00142;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.977014;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.968861;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 1.05818;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.986134;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.947736;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.970503;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.959503;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 1.02155;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.971289;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 0.977242;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.974486;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.971089;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 1.0438;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.976107;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 1.00675;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.978317;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.972809;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 1.02242;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.990935;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.957972;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.971896;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.97416;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 1.01162;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.973841;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.986678;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.968619;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.964946;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 0.969441;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.975715;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 0.977714;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.973962;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.962435;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 0.942494;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.981258;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.972996;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.952872;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.928972;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 0.892012;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 1.01145;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.85275;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.934789;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.850056;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 0.858468;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 1.04048;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 0.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.934789;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.850056;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 0.858468;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 1.04048;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 0.;
  return 0.;
}


double MuX_EleY_DZLeg(double eta_e, double eta_mu){
  if(abs(eta_e)>2.5) return 0.;
  if(abs(eta_mu)>2.5) return 0.;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>-2.5&& eta_mu<=-2) return 0.987342;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>-2&& eta_mu<=-1.5) return 0.994118;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>-1.5&& eta_mu<=-1) return 0.994012;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>-1&& eta_mu<=-0.5) return 0.993378;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>-0.5&& eta_mu<=0) return 0.993007;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>0&& eta_mu<=0.5) return 0.988889;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>0.5&& eta_mu<=1) return 0.983333;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>1&& eta_mu<=1.5) return 0.964286;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>1.5&& eta_mu<=2) return 1.;
  if(eta_e >-2.5&& eta_e<=-2&& eta_mu>2&& eta_mu<=2.5) return 1.;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>-2.5&& eta_mu<=-2) return 0.987179;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>-2&& eta_mu<=-1.5) return 0.986885;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>-1.5&& eta_mu<=-1) return 0.992084;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>-1&& eta_mu<=-0.5) return 0.997783;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>-0.5&& eta_mu<=0) return 0.99726;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>0&& eta_mu<=0.5) return 0.996377;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>0.5&& eta_mu<=1) return 0.975;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>1&& eta_mu<=1.5) return 1.;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>1.5&& eta_mu<=2) return 0.979167;
  if(eta_e >-2&& eta_e<=-1.5&& eta_mu>2&& eta_mu<=2.5) return 1.;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>-2.5&& eta_mu<=-2) return 0.992933;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>-2&& eta_mu<=-1.5) return 0.998185;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>-1.5&& eta_mu<=-1) return 0.991549;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>-1&& eta_mu<=-0.5) return 0.997519;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>-0.5&& eta_mu<=0) return 0.996241;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>0&& eta_mu<=0.5) return 1.;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>0.5&& eta_mu<=1) return 0.99789;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>1&& eta_mu<=1.5) return 0.986799;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>1.5&& eta_mu<=2) return 0.993197;
  if(eta_e >-1.5&& eta_e<=-1&& eta_mu>2&& eta_mu<=2.5) return 1.;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>-2&& eta_mu<=-1.5) return 0.998299;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>-1.5&& eta_mu<=-1) return 0.995749;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>-1&& eta_mu<=-0.5) return 0.993833;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>-0.5&& eta_mu<=0) return 0.99654;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>0&& eta_mu<=0.5) return 0.995353;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>0.5&& eta_mu<=1) return 0.997436;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>1&& eta_mu<=1.5) return 0.993068;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>1.5&& eta_mu<=2) return 0.996667;
  if(eta_e >-1&& eta_e<=-0.5&& eta_mu>2&& eta_mu<=2.5) return 0.980392;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>-1.5&& eta_mu<=-1) return 0.992891;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>-1&& eta_mu<=-0.5) return 0.998296;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>-0.5&& eta_mu<=0) return 0.998324;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>0&& eta_mu<=0.5) return 0.998315;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>0.5&& eta_mu<=1) return 0.998131;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>1&& eta_mu<=1.5) return 0.994723;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>1.5&& eta_mu<=2) return 1.;
  if(eta_e >-0.5&& eta_e<=0&& eta_mu>2&& eta_mu<=2.5) return 1.;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>-1.5&& eta_mu<=-1) return 0.991881;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>-1&& eta_mu<=-0.5) return 0.998049;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>-0.5&& eta_mu<=0) return 0.998329;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>0&& eta_mu<=0.5) return 0.998379;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>0.5&& eta_mu<=1) return 0.997396;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>1&& eta_mu<=1.5) return 0.996591;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>1.5&& eta_mu<=2) return 0.998024;
  if(eta_e >0&& eta_e<=0.5&& eta_mu>2&& eta_mu<=2.5) return 1.;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>-1.5&& eta_mu<=-1) return 0.984183;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>-1&& eta_mu<=-0.5) return 0.996355;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>-0.5&& eta_mu<=0) return 0.998016;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>0&& eta_mu<=0.5) return 0.999122;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>0.5&& eta_mu<=1) return 0.998282;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>1&& eta_mu<=1.5) return 0.994919;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>1.5&& eta_mu<=2) return 0.997041;
  if(eta_e >0.5&& eta_e<=1&& eta_mu>2&& eta_mu<=2.5) return 0.996324;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>-1.5&& eta_mu<=-1) return 0.996743;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>-1&& eta_mu<=-0.5) return 0.997912;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>-0.5&& eta_mu<=0) return 0.998496;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>0&& eta_mu<=0.5) return 0.996099;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>0.5&& eta_mu<=1) return 0.990885;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>1&& eta_mu<=1.5) return 0.997059;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>1.5&& eta_mu<=2) return 0.99211;
  if(eta_e >1&& eta_e<=1.5&& eta_mu>2&& eta_mu<=2.5) return 0.991701;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>-1.5&& eta_mu<=-1) return 0.99115;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>-1&& eta_mu<=-0.5) return 0.993902;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>-0.5&& eta_mu<=0) return 0.992218;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>0&& eta_mu<=0.5) return 0.990909;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>0.5&& eta_mu<=1) return 0.988208;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>1&& eta_mu<=1.5) return 0.994709;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>1.5&& eta_mu<=2) return 0.996377;
  if(eta_e >1.5&& eta_e<=2&& eta_mu>2&& eta_mu<=2.5) return 0.993939;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>-1.5&& eta_mu<=-1) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>-1&& eta_mu<=-0.5) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>-0.5&& eta_mu<=0) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>0&& eta_mu<=0.5) return 0.981982;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>0.5&& eta_mu<=1) return 0.994118;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>1&& eta_mu<=1.5) return 0.993902;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>1.5&& eta_mu<=2) return 1.;
  if(eta_e >2&& eta_e<=2.5&& eta_mu>2&& eta_mu<=2.5) return 0.986111;
  if(eta_e >2.5&& eta_mu>-2.5&& eta_mu<=-2) return 1.;
  if(eta_e >2.5&& eta_mu>-2&& eta_mu<=-1.5) return 1.;
  if(eta_e >2.5&& eta_mu>-1.5&& eta_mu<=-1) return 1.;
  if(eta_e >2.5&& eta_mu>-1&& eta_mu<=-0.5) return 1.;
  if(eta_e >2.5&& eta_mu>-0.5&& eta_mu<=0) return 1.;
  if(eta_e >2.5&& eta_mu>0&& eta_mu<=0.5) return 0.981982;
  if(eta_e >2.5&& eta_mu>0.5&& eta_mu<=1) return 0.994118;
  if(eta_e >2.5&& eta_mu>1&& eta_mu<=1.5) return 0.993902;
  if(eta_e >2.5&& eta_mu>1.5&& eta_mu<=2) return 1.;
  if(eta_e >2.5&& eta_mu>2&& eta_mu<=2.5) return 0.986111;
  return 0.;
  
}






double DoubleMu8_PFHT300_MuLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<10) return 0.;
  if(abseta<0) return 0.;
  if(pt >10&& pt<=15&& abseta>0&& abseta<=0.8) return 0.960731;
  if(pt >10&& pt<=15&& abseta>0.8&& abseta<=1.25) return 0.964223;
  if(pt >10&& pt<=15&& abseta>1.25&& abseta<=1.6) return 1.01611;
  if(pt >10&& pt<=15&& abseta>1.6&& abseta<=2.1) return 0.965723;
  if(pt >10&& pt<=15&& abseta>2.1&& abseta<=2.4) return 0.994383;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.971327;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.25) return 0.967724;
  if(pt >15&& pt<=20&& abseta>1.25&& abseta<=1.6) return 1.03275;
  if(pt >15&& pt<=20&& abseta>1.6&& abseta<=2.1) return 0.980079;
  if(pt >15&& pt<=20&& abseta>2.1&& abseta<=2.4) return 0.959287;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.976318;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.25) return 0.96481;
  if(pt >20&& pt<=25&& abseta>1.25&& abseta<=1.6) return 1.0274;
  if(pt >20&& pt<=25&& abseta>1.6&& abseta<=2.1) return 0.983683;
  if(pt >20&& pt<=25&& abseta>2.1&& abseta<=2.4) return 0.961744;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.971433;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.96158;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 1.03122;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.980313;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.981128;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.970851;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.962689;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 1.02291;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.976409;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 0.967978;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.975345;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.961161;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 1.04575;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.97135;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 0.97425;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.972125;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.964279;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 1.02987;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.975114;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.966507;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.972766;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.959135;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 1.03277;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.97756;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.974533;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.966165;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.95999;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 1.02942;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.972821;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 0.983244;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.972998;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.95753;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 1.01928;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.973425;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.987247;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.960615;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.95321;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 1.02657;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 0.937892;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.882355;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.963306;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.929107;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 1.00224;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 0.898576;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 0.701376;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.963306;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.929107;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 1.00224;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 0.898576;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 0.701376;
  return 0.;
  
}

double DoubleEle8_PFHT300_EleLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<15) return 0.;
  if(abseta<0) return 0.;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.928866;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.479) return 0.958101;
  if(pt >15&& pt<=20&& abseta>1.479&& abseta<=2) return 0.960211;
  if(pt >15&& pt<=20&& abseta>2&& abseta<=2.5) return 0.993113;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.950342;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.479) return 0.94888;
  if(pt >20&& pt<=25&& abseta>1.479&& abseta<=2) return 0.991671;
  if(pt >20&& pt<=25&& abseta>2&& abseta<=2.5) return 0.910769;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.956532;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.975738;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.949567;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.975092;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.963795;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.986395;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 0.966564;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 1.01287;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.959385;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.981712;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.99787;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 1.00917;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.977643;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 0.968181;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 0.973399;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.986176;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.968383;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.979696;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 0.98717;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 0.974876;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.969643;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.968729;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.973011;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 1.0056;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.954194;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.956753;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.967387;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.979007;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.933972;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.934365;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 0.914686;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 0.925;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.954449;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 0.939678;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 0.935965;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.954449;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 0.939678;
  if(pt >500&& abseta>1.479&& abseta<=2) return 0.935965;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
  
}

double Mu8Ele8_PFHT300_MuLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<10) return 0.;
  if(abseta<0) return 0.;
  if(pt >10&& pt<=15&& abseta>0&& abseta<=0.8) return 0.99158;
  if(pt >10&& pt<=15&& abseta>0.8&& abseta<=1.25) return 0.976316;
  if(pt >10&& pt<=15&& abseta>1.25&& abseta<=1.6) return 1.12067;
  if(pt >10&& pt<=15&& abseta>1.6&& abseta<=2.1) return 0.873183;
  if(pt >10&& pt<=15&& abseta>2.1&& abseta<=2.4) return 0.948684;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.983393;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.25) return 0.994127;
  if(pt >15&& pt<=20&& abseta>1.25&& abseta<=1.6) return 1.04964;
  if(pt >15&& pt<=20&& abseta>1.6&& abseta<=2.1) return 0.940447;
  if(pt >15&& pt<=20&& abseta>2.1&& abseta<=2.4) return 0.854399;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.968239;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.25) return 0.973682;
  if(pt >20&& pt<=25&& abseta>1.25&& abseta<=1.6) return 1.13711;
  if(pt >20&& pt<=25&& abseta>1.6&& abseta<=2.1) return 0.942133;
  if(pt >20&& pt<=25&& abseta>2.1&& abseta<=2.4) return 1.0606;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.959451;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.25) return 0.965017;
  if(pt >25&& pt<=30&& abseta>1.25&& abseta<=1.6) return 1.00259;
  if(pt >25&& pt<=30&& abseta>1.6&& abseta<=2.1) return 0.907263;
  if(pt >25&& pt<=30&& abseta>2.1&& abseta<=2.4) return 0.914754;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.9693;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.25) return 0.939411;
  if(pt >30&& pt<=35&& abseta>1.25&& abseta<=1.6) return 0.96045;
  if(pt >30&& pt<=35&& abseta>1.6&& abseta<=2.1) return 0.966374;
  if(pt >30&& pt<=35&& abseta>2.1&& abseta<=2.4) return 1.04058;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.96276;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.25) return 0.946155;
  if(pt >35&& pt<=40&& abseta>1.25&& abseta<=1.6) return 1.00522;
  if(pt >35&& pt<=40&& abseta>1.6&& abseta<=2.1) return 0.978777;
  if(pt >35&& pt<=40&& abseta>2.1&& abseta<=2.4) return 0.909806;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.956252;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.25) return 0.967728;
  if(pt >40&& pt<=50&& abseta>1.25&& abseta<=1.6) return 0.991337;
  if(pt >40&& pt<=50&& abseta>1.6&& abseta<=2.1) return 0.9742;
  if(pt >40&& pt<=50&& abseta>2.1&& abseta<=2.4) return 0.979083;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.969156;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.25) return 0.963124;
  if(pt >50&& pt<=75&& abseta>1.25&& abseta<=1.6) return 0.98751;
  if(pt >50&& pt<=75&& abseta>1.6&& abseta<=2.1) return 0.959941;
  if(pt >50&& pt<=75&& abseta>2.1&& abseta<=2.4) return 0.961788;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.94923;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.25) return 0.929378;
  if(pt >75&& pt<=100&& abseta>1.25&& abseta<=1.6) return 0.92597;
  if(pt >75&& pt<=100&& abseta>1.6&& abseta<=2.1) return 0.956913;
  if(pt >75&& pt<=100&& abseta>2.1&& abseta<=2.4) return 1.02718;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.953872;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.25) return 0.940895;
  if(pt >100&& pt<=200&& abseta>1.25&& abseta<=1.6) return 0.932601;
  if(pt >100&& pt<=200&& abseta>1.6&& abseta<=2.1) return 0.960032;
  if(pt >100&& pt<=200&& abseta>2.1&& abseta<=2.4) return 0.878376;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.944908;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.25) return 0.889961;
  if(pt >200&& pt<=300&& abseta>1.25&& abseta<=1.6) return 0.861393;
  if(pt >200&& pt<=300&& abseta>1.6&& abseta<=2.1) return 1.22419;
  if(pt >200&& pt<=300&& abseta>2.1&& abseta<=2.4) return 0.980087;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.874154;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.25) return 0.78291;
  if(pt >300&& pt<=500&& abseta>1.25&& abseta<=1.6) return 0.745323;
  if(pt >300&& pt<=500&& abseta>1.6&& abseta<=2.1) return 1.08538;
  if(pt >300&& pt<=500&& abseta>2.1&& abseta<=2.4) return 0.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.874154;
  if(pt >500&& abseta>0.8&& abseta<=1.25) return 0.78291;
  if(pt >500&& abseta>1.25&& abseta<=1.6) return 0.745323;
  if(pt >500&& abseta>1.6&& abseta<=2.1) return 1.08538;
  if(pt >500&& abseta>2.1&& abseta<=2.4) return 0.;
  return 0.;
  
}

double Mu8Ele8_PFHT300_EleLeg_SF(double pt, double eta){
  double abseta=abs(eta);
  if(pt<15) return 0.;
  if(abseta<0) return 0.;
  if(pt >15&& pt<=20&& abseta>0&& abseta<=0.8) return 0.953626;
  if(pt >15&& pt<=20&& abseta>0.8&& abseta<=1.479) return 0.965579;
  if(pt >15&& pt<=20&& abseta>1.479&& abseta<=2) return 0.921426;
  if(pt >15&& pt<=20&& abseta>2&& abseta<=2.5) return 0.927653;
  if(pt >20&& pt<=25&& abseta>0&& abseta<=0.8) return 0.976504;
  if(pt >20&& pt<=25&& abseta>0.8&& abseta<=1.479) return 0.998459;
  if(pt >20&& pt<=25&& abseta>1.479&& abseta<=2) return 0.954646;
  if(pt >20&& pt<=25&& abseta>2&& abseta<=2.5) return 0.98396;
  if(pt >25&& pt<=30&& abseta>0&& abseta<=0.8) return 0.972863;
  if(pt >25&& pt<=30&& abseta>0.8&& abseta<=1.479) return 0.9883;
  if(pt >25&& pt<=30&& abseta>1.479&& abseta<=2) return 0.968179;
  if(pt >25&& pt<=30&& abseta>2&& abseta<=2.5) return 0.985852;
  if(pt >30&& pt<=35&& abseta>0&& abseta<=0.8) return 0.978949;
  if(pt >30&& pt<=35&& abseta>0.8&& abseta<=1.479) return 0.984177;
  if(pt >30&& pt<=35&& abseta>1.479&& abseta<=2) return 1.0002;
  if(pt >30&& pt<=35&& abseta>2&& abseta<=2.5) return 0.973056;
  if(pt >35&& pt<=40&& abseta>0&& abseta<=0.8) return 0.98523;
  if(pt >35&& pt<=40&& abseta>0.8&& abseta<=1.479) return 0.984871;
  if(pt >35&& pt<=40&& abseta>1.479&& abseta<=2) return 0.977685;
  if(pt >35&& pt<=40&& abseta>2&& abseta<=2.5) return 0.997637;
  if(pt >40&& pt<=50&& abseta>0&& abseta<=0.8) return 0.985967;
  if(pt >40&& pt<=50&& abseta>0.8&& abseta<=1.479) return 0.997491;
  if(pt >40&& pt<=50&& abseta>1.479&& abseta<=2) return 0.993795;
  if(pt >40&& pt<=50&& abseta>2&& abseta<=2.5) return 0.991517;
  if(pt >50&& pt<=75&& abseta>0&& abseta<=0.8) return 0.988038;
  if(pt >50&& pt<=75&& abseta>0.8&& abseta<=1.479) return 0.992215;
  if(pt >50&& pt<=75&& abseta>1.479&& abseta<=2) return 0.998005;
  if(pt >50&& pt<=75&& abseta>2&& abseta<=2.5) return 1.00157;
  if(pt >75&& pt<=100&& abseta>0&& abseta<=0.8) return 0.987354;
  if(pt >75&& pt<=100&& abseta>0.8&& abseta<=1.479) return 0.988789;
  if(pt >75&& pt<=100&& abseta>1.479&& abseta<=2) return 0.999343;
  if(pt >75&& pt<=100&& abseta>2&& abseta<=2.5) return 0.998783;
  if(pt >100&& pt<=200&& abseta>0&& abseta<=0.8) return 0.988044;
  if(pt >100&& pt<=200&& abseta>0.8&& abseta<=1.479) return 0.991275;
  if(pt >100&& pt<=200&& abseta>1.479&& abseta<=2) return 0.994685;
  if(pt >100&& pt<=200&& abseta>2&& abseta<=2.5) return 0.994669;
  if(pt >200&& pt<=300&& abseta>0&& abseta<=0.8) return 0.983503;
  if(pt >200&& pt<=300&& abseta>0.8&& abseta<=1.479) return 0.98558;
  if(pt >200&& pt<=300&& abseta>1.479&& abseta<=2) return 1.00308;
  if(pt >200&& pt<=300&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >300&& pt<=500&& abseta>0&& abseta<=0.8) return 0.975322;
  if(pt >300&& pt<=500&& abseta>0.8&& abseta<=1.479) return 1.00021;
  if(pt >300&& pt<=500&& abseta>1.479&& abseta<=2) return 1.00107;
  if(pt >300&& pt<=500&& abseta>2&& abseta<=2.5) return 1.;
  if(pt >500&& abseta>0&& abseta<=0.8) return 0.975322;
  if(pt >500&& abseta>0.8&& abseta<=1.479) return 1.00021;
  if(pt >500&& abseta>1.479&& abseta<=2) return 1.00107;
  if(pt >500&& abseta>2&& abseta<=2.5) return 1.;
  return 0.;
  
}



double HTLeg_SF(int lep1_pdgId, double lep1_pt, int lep2_pdgId, double lep2_pt,  double HT){
  if(abs(lep1_pdgId)!=13 || abs(lep2_pdgId) !=13) return 1 ;// HT ineffcy  only measured for dimuon+HT. 
  if(HT<300) return 1.;
  if(HT>1000) return 1.;
  if(HT<1000&&HT>350) return 0.992;//Effect due to the L1 loss of efficiency in Run H  
  if(HT<350 && lep1_pt<27&&lep2_pt<27) return 0.990; //HLT SF in that region  is below 1.
  return 1.;

  
}




double TotalTriggerSF(int lep1_pdgId, double lep1_pt, double lep1_eta, int lep2_pdgId, double lep2_pt, double lep2_eta, double HT){

  double fracrunH =  1-27.22/36.46;
  // Make sure lep 1 is the leading lepton
  if(lep1_pt<lep2_pt){
    int buf_pdgId = lep1_pdgId;
    double buf_pt = lep1_pt;
    double buf_eta = lep1_eta;
    lep1_pdgId =  lep2_pdgId;
    lep1_pt = lep2_pt;
    lep1_eta = lep2_eta;
    lep2_pdgId =  buf_pdgId;
    lep2_pt = buf_pt;
    lep2_eta = buf_eta;
    
  }



  if(abs(lep1_pdgId)+ abs(lep2_pdgId) ==22) {//ele-Ele
    if(HT<300) return Ele23_Ele12_LeadingLeg_SF(lep1_pt,lep1_eta)*Ele23_Ele12_TrailingLeg_SF(lep2_pt,lep2_eta)*Ele23_Ele12_DZLeg(lep1_eta,lep2_eta);
    else  return DoubleEle8_PFHT300_EleLeg_SF(lep1_pt,lep1_eta)*DoubleEle8_PFHT300_EleLeg_SF(lep2_pt,lep2_eta) ; //HT SF is one here. 
  }
  else if(abs(lep1_pdgId)+ abs(lep2_pdgId) ==26) {//mu-mu
    if(HT<300) return  Mu17_Mu8orTkMu8_LeadingLeg_SF(lep1_pt,lep1_eta)*Mu17_Mu8orTkMu8_TrailingLeg_SF(lep2_pt,lep2_eta)*(fracrunH*Mu17_Mu8orTkMu8_DZLeg(lep1_eta,lep2_eta)+(1-fracrunH));
    //The last factor applies the DZ effcy on the fraction of lumi corresponding to Run H
        else return DoubleMu8_PFHT300_MuLeg_SF(lep1_pt,lep1_eta)*DoubleMu8_PFHT300_MuLeg_SF(lep2_pt,lep2_eta)*HTLeg_SF(lep1_pdgId,lep1_pt,lep2_pdgId,lep2_pt,HT);
          }
  else if(abs(lep1_pdgId)+ abs(lep2_pdgId) ==24) {//ele-mu
    if(abs(lep1_pdgId)==11){//leading ele
      if(HT<300) return Mu8_Ele23_LeadingLeg_SF(lep1_pt,lep1_eta)*Mu8_Ele23_TrailingLeg_SF(lep2_pt,lep2_eta)*(fracrunH*MuX_EleY_DZLeg(lep1_eta,lep2_eta)+(1-fracrunH)); 
      else return Mu8Ele8_PFHT300_EleLeg_SF(lep1_pt,lep1_eta)*Mu8Ele8_PFHT300_MuLeg_SF(lep2_pt,lep2_eta);
    }
    else{//leading mu
      if(HT<300) return Mu23_Ele8_LeadingLeg_SF(lep1_pt,lep1_eta)*Mu23_Ele8_TrailingLeg_SF(lep2_pt,lep2_eta)*(fracrunH*MuX_EleY_DZLeg(lep2_eta,lep1_eta)+(1-fracrunH));
      else return Mu8Ele8_PFHT300_EleLeg_SF(lep2_pt,lep2_eta)*Mu8Ele8_PFHT300_MuLeg_SF(lep1_pt,lep1_eta);
    } 
  }
  
  else return 0.; 
  
}

