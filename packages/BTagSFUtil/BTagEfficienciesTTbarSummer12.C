/* These Efficiencies have been derived for Summer12 ttbar events and should
be used only for the same MC samples or for events with similar topology */

float BTagSFUtil::TagEfficiencyB(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=2.2) return 0.809008; // 3312.87 0.682939
      else if (JetEta>=2) return 0.852787; // 3060.58 0.64046
      else if (JetEta>=1.8) return 0.858175; // 3707.54 0.572956
      else if (JetEta>=1.6) return 0.848624; // 4706.23 0.522456
      else if (JetEta>=1.4) return 0.846757; // 5615.71 0.480692
      else if (JetEta>=1.2) return 0.860785; // 6459.96 0.430701
      else if (JetEta>=1) return 0.838642; // 7239.39 0.432347
      else if (JetEta>=0.8) return 0.856607; // 8095.13 0.389532
      else if (JetEta>=0.6) return 0.858616; // 8676.53 0.374048
      else if (JetEta>=0.4) return 0.856543; // 9048.09 0.368517
      else if (JetEta>=0.2) return 0.850265; // 9378.28 0.368449
      else if (JetEta>=0) return 0.837786; // 9619.12 0.375875
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=2.2) return 0.787633; // 2567.98 0.807067
      else if (JetEta>=2) return 0.83771; // 2598.2 0.723363
      else if (JetEta>=1.8) return 0.832037; // 3464.23 0.635147
      else if (JetEta>=1.6) return 0.833712; // 4794.31 0.537744
      else if (JetEta>=1.4) return 0.833563; // 6163.37 0.474444
      else if (JetEta>=1.2) return 0.841055; // 7730.32 0.41585
      else if (JetEta>=1) return 0.842563; // 9031.9 0.383235
      else if (JetEta>=0.8) return 0.846668; // 10456.4 0.352356
      else if (JetEta>=0.6) return 0.855472; // 11673.4 0.325448
      else if (JetEta>=0.4) return 0.85725; // 12383.7 0.314352
      else if (JetEta>=0.2) return 0.85215; // 13135.8 0.309699
      else if (JetEta>=0) return 0.837702; // 13479.1 0.317593
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=2) return 0.769223; // 2105.09 0.918305
      else if (JetEta>=1.8) return 0.816982; // 1671.31 0.945855
      else if (JetEta>=1.6) return 0.828224; // 2412.84 0.767874
      else if (JetEta>=1.4) return 0.831575; // 3437.13 0.638345
      else if (JetEta>=1.2) return 0.837462; // 4820.41 0.531396
      else if (JetEta>=1) return 0.842355; // 5947.54 0.472519
      else if (JetEta>=0.8) return 0.852549; // 7018.78 0.423207
      else if (JetEta>=0.6) return 0.865364; // 8107.63 0.379081
      else if (JetEta>=0.4) return 0.863202; // 8724.63 0.367893
      else if (JetEta>=0.2) return 0.857301; // 9481.23 0.359206
      else if (JetEta>=0) return 0.833508; // 9576.16 0.380677
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=1.4) return 0.796303; // 2478.95 0.808905
      else if (JetEta>=1.2) return 0.815799; // 1581.29 0.974838
      else if (JetEta>=1) return 0.829592; // 2047.94 0.830843
      else if (JetEta>=0.8) return 0.853015; // 2451.91 0.715093
      else if (JetEta>=0.6) return 0.849798; // 2855.79 0.668548
      else if (JetEta>=0.4) return 0.870126; // 3139.45 0.599964
      else if (JetEta>=0.2) return 0.850251; // 3397.08 0.612213
      else if (JetEta>=0) return 0.835425; // 3555.59 0.621842
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=1) return 0.793887; // 1792.65 0.9554
      else if (JetEta>=0.6) return 0.835242; // 1816.48 0.87039
      else if (JetEta>=0.2) return 0.852439; // 2407.34 0.722851
      else if (JetEta>=-0.2) return 0.838569; // 1332.46 1.00794
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=0.4) return 0.778819; // 1749.14 0.992385
      else if (JetEta>=-0.2) return 0.806058; // 1133.46 1.1744
    }
  }

  if (TaggerOP=="CSVv2M") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=2.2) return 0.457318; // 3312.87 0.865525
      else if (JetEta>=2) return 0.581291; // 3060.58 0.891767
      else if (JetEta>=1.8) return 0.621905; // 3707.54 0.796378
      else if (JetEta>=1.6) return 0.608839; // 4706.23 0.711365
      else if (JetEta>=1.4) return 0.609522; // 5615.71 0.651015
      else if (JetEta>=1.2) return 0.615207; // 6459.96 0.605354
      else if (JetEta>=1) return 0.636686; // 7239.39 0.565266
      else if (JetEta>=0.8) return 0.676667; // 8095.13 0.519877
      else if (JetEta>=0.6) return 0.678029; // 8676.53 0.501602
      else if (JetEta>=0.4) return 0.68552; // 9048.09 0.488122
      else if (JetEta>=0.2) return 0.686303; // 9378.28 0.479128
      else if (JetEta>=0) return 0.665883; // 9619.12 0.480929
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=2.2) return 0.468871; // 2567.98 0.984761
      else if (JetEta>=2) return 0.610662; // 2598.2 0.956594
      else if (JetEta>=1.8) return 0.648496; // 3464.23 0.811176
      else if (JetEta>=1.6) return 0.651667; // 4794.31 0.688093
      else if (JetEta>=1.4) return 0.645824; // 6163.37 0.609197
      else if (JetEta>=1.2) return 0.666929; // 7730.32 0.536055
      else if (JetEta>=1) return 0.692456; // 9031.9 0.485579
      else if (JetEta>=0.8) return 0.720003; // 10456.4 0.439089
      else if (JetEta>=0.6) return 0.737833; // 11673.4 0.407071
      else if (JetEta>=0.4) return 0.741772; // 12383.7 0.393289
      else if (JetEta>=0.2) return 0.731069; // 13135.8 0.386875
      else if (JetEta>=0) return 0.723047; // 13479.1 0.385439
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=1.8) return 0.59996; // 3778.31 0.797011
      else if (JetEta>=1.6) return 0.681846; // 2412.84 0.948194
      else if (JetEta>=1.4) return 0.680889; // 3437.13 0.79508
      else if (JetEta>=1.2) return 0.696981; // 4820.41 0.661916
      else if (JetEta>=1) return 0.730068; // 5947.54 0.575626
      else if (JetEta>=0.8) return 0.7469; // 7018.78 0.518975
      else if (JetEta>=0.6) return 0.77531; // 8107.63 0.463535
      else if (JetEta>=0.4) return 0.765666; // 8724.63 0.453486
      else if (JetEta>=0.2) return 0.766997; // 9481.23 0.434155
      else if (JetEta>=0) return 0.741348; // 9576.16 0.447479
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=1.4) return 0.639172; // 2478.95 0.964551
      else if (JetEta>=1) return 0.701581; // 3629.46 0.759506
      else if (JetEta>=0.8) return 0.758373; // 2451.91 0.864494
      else if (JetEta>=0.6) return 0.74889; // 2855.79 0.811479
      else if (JetEta>=0.4) return 0.77955; // 3139.45 0.739861
      else if (JetEta>=0.2) return 0.753272; // 3397.08 0.73966
      else if (JetEta>=0) return 0.736507; // 3555.59 0.738783
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=0.8) return 0.66771; // 2630.44 0.918414
      else if (JetEta>=0.4) return 0.740332; // 2140.21 0.947752
      else if (JetEta>=0) return 0.739091; // 2579.1 0.864687
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=0.2) return 0.64097; // 2312.74 0.997519
      else if (JetEta>=-0.2) return 0.704848; // 569.568 1.91116
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=1.8) return 0.387812; // 10077.4 0.485376 0.490232
      else if (JetEta>=1.4) return 0.45759; // 10326.1 0.490268 0.499275
      else if (JetEta>=1) return 0.474506; // 13698.8 0.426642 0.433902
      else if (JetEta>=0.6) return 0.534178; // 16772.7 0.385169 0.391619
      else if (JetEta>=0.2) return 0.547121; // 18426.4 0.366701 0.373442
      else if (JetEta>=-0.2) return 0.524589; // 9619.12 0.509186 0.514535
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=1.6) return 0.4476; // 13425.6 0.429146 0.435385
      else if (JetEta>=1.2) return 0.516362; // 13893.6 0.423965 0.429079
      else if (JetEta>=0.8) return 0.556665; // 19488.3 0.355857 0.362944
      else if (JetEta>=0.6) return 0.596254; // 11673.4 0.454121 0.466361
      else if (JetEta>=0.4) return 0.601248; // 12383.7 0.440001 0.445847
      else if (JetEta>=0.2) return 0.588598; // 13135.8 0.429353 0.434044
      else if (JetEta>=0) return 0.577497; // 13479.1 0.425461 0.434786
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=1.2) return 0.477814; // 14438.7 0.415698 0.422847
      else if (JetEta>=0.8) return 0.555712; // 12966.6 0.436359 0.443159
      else if (JetEta>=0.4) return 0.603197; // 16832.3 0.37709 0.384097
      else if (JetEta>=0) return 0.583599; // 19057.6 0.357092 0.362001
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=0.6) return 0.51441; // 11410.5 0.467882 0.474973
      else if (JetEta>=0) return 0.56342; // 10092 0.493696 0.504266
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=-0.2) return 0.438987; // 7346.99 0.578972 0.589266
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=-0.2) return 0.371781; // 2881.77 0.900264 0.921969
    }
  }

  // cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2L") return 0.80;
  if (TaggerOP=="CSVv2M") return 0.65;

  cout << "BTagSFUtil: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}

float BTagSFUtil::TagEfficiencyC(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=0) return 0.473677; // 2671.38 0.966049
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=-0.2) return 0.392501; // 1795.58 1.15237
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=-0.2) return 0.359924; // 1061.43 1.47324
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=-0.2) return 0.37042; // 484.451 2.19406
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=-0.2) return 0.359324; // 302.053 2.76071
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=-0.2) return 0.345628; // 276.093 2.86213
    }
  }

  if (TaggerOP=="CSVv2M") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=1) return 0.137101; // 1367.57 0.93009
      else if (JetEta>=-0.2) return 0.172955; // 1302.01 1.04815
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=0.4) return 0.163397; // 1413.84 0.983288
      else if (JetEta>=-0.2) return 0.173224; // 381.629 1.93721
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=-0.2) return 0.18961; // 1061.43 1.20318
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=-0.2) return 0.188993; // 484.451 1.77873
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=-0.2) return 0.197597; // 302.053 2.2911
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=-0.2) return 0.204649; // 276.093 2.42804
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=1.2) return 0.038948; // 1103.6 0.582384 0.649
      else if (JetEta>=0) return 0.0730094; // 1565.53 0.657502 0.677727
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=-0.2) return 0.0594221; // 1795.58 0.557916 0.604515
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=-0.2) return 0.0597343; // 1061.43 0.727429 0.768528
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=-0.2) return 0.0298142; // 484.451 0.772706 0.912494
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=-0.2) return 0.0225716; // 302.053 0.854637 1.02596
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=-0.2) return 0.0136672; // 276.093 0.698752 0.945059
    }
  }
  
  // cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2L") return 0.35;
  if (TaggerOP=="CSVv2M") return 0.16;

  cout << "BTagSFUtil: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}

float BTagSFUtil::TagEfficiencyLight(float JetPt, float JetEta) {

  if (TaggerOP=="CSVv2L") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=2.2)      return 0.420442; // 3926.52 0.787767
      else if (JetEta>=2)   return 0.338665; // 2983.58 0.866418
      else if (JetEta>=1.8) return 0.247388; // 3171.73 0.766173
      else if (JetEta>=1.6) return 0.238397; // 3493.16 0.720951
      else if (JetEta>=1.4) return 0.238432; // 3989.68 0.674632
      else if (JetEta>=1.2) return 0.240449; // 4269.08 0.654067
      else if (JetEta>=1)   return 0.148722; // 4592.34 0.525057
      else if (JetEta>=0.8) return 0.165212; // 4739.99 0.539412
      else if (JetEta>=0.6) return 0.144527; // 5046.85 0.494956
      else if (JetEta>=0.4) return 0.148472; // 5155.2 0.495221
      else if (JetEta>=0.2) return 0.127898; // 4976.4 0.473432
      else if (JetEta>=0)   return 0.127603; // 5197.03 0.462818
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=2.2)      return 0.321751; // 2480.17 0.938024
      else if (JetEta>=2)   return 0.238807; // 1867.7 0.986545
      else if (JetEta>=1.8) return 0.137103; // 1896.63 0.78979
      else if (JetEta>=1.6) return 0.123762; // 2209.89 0.700518
      else if (JetEta>=1.4) return 0.14158; // 2504.66 0.696588
      else if (JetEta>=1.2) return 0.113777; // 2764.43 0.603942
      else if (JetEta>=1)   return 0.0912294; // 2834.32 0.540841
      else if (JetEta>=0.8) return 0.0903294; // 2960.95 0.526794
      else if (JetEta>=0.6) return 0.101207; // 2992.92 0.5513
      else if (JetEta>=0.4) return 0.0799342; // 3249.78 0.475717
      else if (JetEta>=0.2) return 0.0784796; // 3148.24 0.479288
      else if (JetEta>=0)   return 0.0772237; // 3205.01 0.471529
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=2)        return 0.252252; // 2326.08 0.900497
      else if (JetEta>=1.6) return 0.114915; // 2269.35 0.669469
      else if (JetEta>=1.4) return 0.110387; // 1359.65 0.849856
      else if (JetEta>=1.2) return 0.0830715; // 1533.19 0.704848
      else if (JetEta>=1) return 0.0778623; // 1585.36 0.672974
      else if (JetEta>=0.8) return 0.0768658; // 1608.92 0.664097
      else if (JetEta>=0.6) return 0.0676013; // 1712.71 0.606647
      else if (JetEta>=0.4) return 0.0830071; // 1676.9 0.673732
      else if (JetEta>=0.2) return 0.073603; // 1714.09 0.63071
      else if (JetEta>=0) return 0.0620921; // 1822.07 0.565348
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=1.6) return 0.181298; // 2011.89 0.858929
      else if (JetEta>=1.2) return 0.0864793; // 1317.64 0.774315
      else if (JetEta>=1) return 0.0666236; // 781.072 0.892271
      else if (JetEta>=0.8) return 0.0800225; // 756.503 0.986482
      else if (JetEta>=0.6) return 0.0664807; // 814.199 0.87306
      else if (JetEta>=0.4) return 0.0665918; // 817.127 0.872171
      else if (JetEta>=0.2) return 0.0609753; // 725.88 0.888143
      else if (JetEta>=0) return 0.0751063; // 808.285 0.927047
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=1.4) return 0.164064; // 1524.31 0.948543
      else if (JetEta>=1) return 0.0921369; // 838.988 0.998502
      else if (JetEta>=0.6) return 0.0978261; // 908.33 0.985714
      else if (JetEta>=0.2) return 0.0780631; // 955.181 0.868022
      else if (JetEta>=-0.2) return 0.0832897; // 501.078 1.23441
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=1.2) return 0.129322; // 1701.89 0.81339
      else if (JetEta>=0.8) return 0.0753734; // 962.893 0.850753
      else if (JetEta>=0.4) return 0.0895906; // 923.429 0.939827
      else if (JetEta>=0) return 0.0755069; // 1017.49 0.828285
    }
  }
  
  if (TaggerOP=="CSVv2M") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=2.2) return 0.0218702; // 3926.52 0.233411
      else if (JetEta>=2) return 0.0252778; // 2983.58 0.28737
      else if (JetEta>=1.8) return 0.0187535; // 3171.73 0.24087
      else if (JetEta>=1.6) return 0.0207562; // 3493.16 0.241219
      else if (JetEta>=1.4) return 0.0172815; // 3989.68 0.206318
      else if (JetEta>=1.2) return 0.0127035; // 4269.08 0.171403
      else if (JetEta>=1) return 0.0157108; // 4592.34 0.183503
      else if (JetEta>=0.8) return 0.0164909; // 4739.99 0.184979
      else if (JetEta>=0.6) return 0.0132922; // 5046.85 0.161207
      else if (JetEta>=0.4) return 0.0145052; // 5155.2 0.16652
      else if (JetEta>=0.2) return 0.0105695; // 4976.4 0.144964
      else if (JetEta>=0) return 0.0133768; // 5197.03 0.159358
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=2.2) return 0.0121253; // 2480.17 0.219764
      else if (JetEta>=2) return 0.0195525; // 1867.7 0.320375
      else if (JetEta>=1.8) return 0.0159969; // 1896.63 0.288088
      else if (JetEta>=1.6) return 0.0163013; // 2209.89 0.269375
      else if (JetEta>=1.4) return 0.014961; // 2504.66 0.242567
      else if (JetEta>=1.2) return 0.0082208; // 2764.43 0.171736
      else if (JetEta>=1) return 0.0112669; // 2834.32 0.198252
      else if (JetEta>=0.8) return 0.0136138; // 2960.95 0.21296
      else if (JetEta>=0.6) return 0.0154717; // 2992.92 0.225598
      else if (JetEta>=0.4) return 0.00991515; // 3249.78 0.173804
      else if (JetEta>=0.2) return 0.0119935; // 3148.24 0.194008
      else if (JetEta>=0) return 0.0114284; // 3205.01 0.187751
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=2.2) return 0.0171949; // 1340.69 0.355034
      else if (JetEta>=2) return 0.0208581; // 985.459 0.45524
      else if (JetEta>=1.8) return 0.0210796; // 1078.96 0.437323
      else if (JetEta>=1.6) return 0.00915257; // 1190.28 0.276026
      else if (JetEta>=1.4) return 0.0140617; // 1359.65 0.319322
      else if (JetEta>=1.2) return 0.00958679; // 1533.19 0.248855
      else if (JetEta>=1) return 0.0140639; // 1585.36 0.295742
      else if (JetEta>=0.8) return 0.0137454; // 1608.92 0.290273
      else if (JetEta>=0.6) return 0.0116382; // 1712.71 0.259154
      else if (JetEta>=0.4) return 0.0115792; // 1676.9 0.261251
      else if (JetEta>=0.2) return 0.01354; // 1714.09 0.279147
      else if (JetEta>=0) return 0.00933232; // 1822.07 0.225256
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=2.2) return 0.0113618; // 548.511 0.452532
      else if (JetEta>=2) return 0.0362423; // 448.5 0.882492
      else if (JetEta>=1.8) return 0.0201319; // 471.407 0.646886
      else if (JetEta>=1.6) return 0.0141313; // 543.408 0.506334
      else if (JetEta>=1.4) return 0.018753; // 651.027 0.53165
      else if (JetEta>=1.2) return 0.0215288; // 666.74 0.56209
      else if (JetEta>=1) return 0.00903814; // 781.072 0.338628
      else if (JetEta>=0.8) return 0.0245823; // 756.503 0.562991
      else if (JetEta>=0.6) return 0.0149224; // 814.199 0.424902
      else if (JetEta>=0.4) return 0.00705541; // 817.127 0.292805
      else if (JetEta>=0.2) return 0.0195207; // 725.88 0.513493
      else if (JetEta>=0) return 0.0126196; // 808.285 0.392629
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=2.2) return 0.00722232; // 305.489 0.484469
      else if (JetEta>=2) return 0.0057698; // 254.765 0.474519
      else if (JetEta>=1.8) return 0.0139492; // 284.348 0.695503
      else if (JetEta>=1.6) return 0.0255133; // 315.103 0.88827
      else if (JetEta>=1.4) return 0.00946272; // 364.607 0.507027
      else if (JetEta>=1.2) return 0.00377937; // 400.467 0.306622
      else if (JetEta>=1) return 0.0125736; // 438.595 0.532047
      else if (JetEta>=0.8) return 0.017102; // 468.029 0.599296
      else if (JetEta>=0.6) return 0.0240959; // 440.421 0.730703
      else if (JetEta>=0.4) return 0.0137613; // 472.588 0.535894
      else if (JetEta>=0.2) return 0.0218971; // 482.462 0.666275
      else if (JetEta>=0) return 0.0172466; // 501.078 0.581596
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=2) return 0.0246663; // 358.693 0.818968
      else if (JetEta>=1.6) return 0.0373008; // 519.881 0.831098
      else if (JetEta>=1.4) return 0.0338009; // 383.362 0.92298
      else if (JetEta>=1.2) return 0.0128993; // 441.087 0.53728
      else if (JetEta>=1) return 0.0189693; // 454.311 0.640015
      else if (JetEta>=0.8) return 0.0177886; // 508.606 0.586115
      else if (JetEta>=0.6) return 0.0139127; // 474.895 0.537483
      else if (JetEta>=0.4) return 0.0145154; // 448.404 0.564813
      else if (JetEta>=0.2) return 0.0210017; // 501.926 0.640028
      else if (JetEta>=0) return 0.0180327; // 515.528 0.586074
    }
  }

  if (TaggerOP=="CSVv2T") {
    if (30<=JetPt && JetPt<50) {
      if (JetEta>=1) return 0.00326464; // 26428.4 0.0350891 0.037114
      else if (JetEta>=0) return 0.00355733; // 25115.4 0.037568 0.0382523
    } else if (50<=JetPt && JetPt<80) {
      if (JetEta>=1) return 0.00169019; // 16558.8 0.0319217 0.0354968
      else if (JetEta>=-0.2) return 0.00225288; // 15556.8 0.0380119 0.0425972
    } else if (80<=JetPt && JetPt<120) {
      if (JetEta>=1) return 0.000915307; // 9073.96 0.0317458 0.0398403
      else if (JetEta>=-0.2) return 0.00101768; // 8535.11 0.0345128 0.041528
    } else if (120<=JetPt && JetPt<160) {
      if (JetEta>=-0.2) return 0.00131949; // 8031.1 0.0405068 0.0509324
    } else if (160<=JetPt && JetPt<210) {
      if (JetEta>=-0.2) return 0.00114043; // 4727.13 0.0490893 0.0713493
    } else if (210<=JetPt && JetPt<100000) {
      if (JetEta>=0) return 0.000390689; // 4602.99 0.029128 0.0364063
    }
  }
  
  // cout << "BTagSFUtil: Jet pt (" << JetPt << ") or jet eta (" << JetEta << ") out of range!" << endl;

  if (TaggerOP=="CSVv2L") return 0.10;
  if (TaggerOP=="CSVv2M") return 0.02;

  cout << "BTagSFUtil: Tagger (" << TaggerOP << ") not valid!" << endl;

  return 0.0;

}
