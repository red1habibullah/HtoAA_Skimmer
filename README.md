# HtoAASkimmerThis tool is used to Skim the AOD samples to MiniAOD. We do the Electron/Muon Cleaning in this step and store results for Taus cleaned using different Muon/Electron IDs.# Introduction for setting up the environment:```bash$ export SCRAM_ARCH=slc6_amd64_gcc700$ cmsrel CMSSW_10_2_18$ cd CMSSW_10_2_18/src$ git cms-init$ scram b  $ git clone git@github.com:red1habibullah/HtoAA_Skimmer.git$ ./Skimmer/recipe/recipe.h $ scram b -j8```