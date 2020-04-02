betterDAS.py dataset --primaryDatasets\
 "DYJetsToLL_M-50_Tune*pythia8" "DYJetsToLL_M-10to50_Tune*pythia8"\
 "SUSYGluGluToHToAA_AToMuMu_AToTauTau*"\
 "QCD_Pt*MuEnrichedPt5*"\
 "ST_s-*" "ST_t-*f_incl*" "ST_tW_*f_incl*"\
 "TTJets_SingleLeptFromT_Tune*" "TTJets_SingleLeptFromTbar_Tune*" "TTJets_DiLept_Tune*" "TTJets_Tune*"\
 "WJetsToLNu_Tune*"\
 "WGG*" "WWG*" "WWW*" "WWZ*" "WZG*" "WZZ*" "ZZZ*"\
 "WWTo2L2Nu_13TeV-powheg"\
 "WZTo3LNu_Tune*" "WZTo2L2Q_13TeV*" "WZTo3LNu_13TeV*"\
 "GluGluToContinToZZ*" "ZZTo4L_13TeV*" "ZZTo2L2Nu_13TeV*" "ZZTo2L2Q_13TeV*"\
 "tZq_ll_4f_13TeV-amcatnlo-pythia8"\
 "ttW*" "ttZ*"\
 "TTW*" "TTZ*"\
 "JPsiToMuMu*" "UpsilonMuMu*"\
 --dataTiers "AODSIM" --acquisitionEras "RunIIFall17DR*" --processNames "*94X_mc2017_realistic_v10*" > datasetList_MC_AOD_new.txt
