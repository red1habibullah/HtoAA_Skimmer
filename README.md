# HtoAASkimmer @2017This tool is used to Skim the AOD samples to MiniAOD. We do the Electron/Muon Cleaning in this step and store results for Taus cleaned using different Muon/Electron IDs.# Introduction for setting up the environment:```bash$ export SCRAM_ARCH=slc6_amd64_gcc630 (or if not bash setenv SCRAM_ARCH slc6_amd64_gcc630)$ cmsrel CMSSW_9_4_13$ cd CMSSW_9_4_13/src$ git cms-init$ cmsenv$ scram b  #needed for the EGamma recipes$git cms-merge-topic cms-egamma:EgammaPostRecoTools$ git clone --recursive  git@github.com:red1habibullah/HtoAA_Skimmer.git  -b  2016_Skim $ scram b -j8```