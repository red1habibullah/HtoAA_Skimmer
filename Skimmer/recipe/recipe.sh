#!/usr/bin/env bash

# CMSSW packages
pushd $CMSSW_BASE/src

# EGamma
# https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2
git cms-merge-topic cms-egamma:EgammaPostRecoTools
git cms-merge-topic cms-egamma:PhotonIDValueMapSpeedup1029 # 102X
git cms-merge-topic cms-egamma:slava77-btvDictFix_10210 # 102X
git cms-addpkg EgammaAnalysis/ElectronTools
rm EgammaAnalysis/ElectronTools/data -rf
git clone git@github.com:cms-data/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data

popd

