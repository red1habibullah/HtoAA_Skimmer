import FWCore.ParameterSet.Config as cms

# load branches
from DevTools.Ntuplizer.branchTemplates import *

genMiniTree = cms.EDAnalyzer("GenMiniTree",
    isData = cms.bool(True),
    genEventInfo = cms.InputTag("generator"),
    lheEventProduct = cms.InputTag("externalLHEProducer"),
    pileupSummaryInfo = cms.InputTag("slimmedAddPileupInfo"),
    collections = cms.PSet(),
)
