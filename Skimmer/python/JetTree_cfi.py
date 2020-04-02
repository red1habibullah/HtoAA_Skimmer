import FWCore.ParameterSet.Config as cms

# load branches
from DevTools.Ntuplizer.branchTemplates import *

miniTree = cms.EDAnalyzer("MiniTree",
    isData = cms.bool(True),
    genEventInfo = cms.InputTag("generator"),
    nevents = cms.InputTag('lumiSummary','numberOfEvents'),
    summedWeights = cms.InputTag('lumiSummary','sumOfWeightedEvents'),
    summedGenWeights = cms.InputTag('lumiSummary','sumOfGenWeightedEvents'),
    lheEventProduct = cms.InputTag("externalLHEProducer"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    pileupSummaryInfo = cms.InputTag("slimmedAddPileupInfo"),
    triggerResults = cms.InputTag("TriggerResults","","HLT"),
    filterResults = cms.InputTag("TriggerResults","","PAT"),
    triggerObjects = cms.InputTag("selectedPatTrigger"),
    triggerPrescales = cms.InputTag("patTrigger"),
    triggerBranches = cms.PSet(),
    filterBranches = cms.PSet(),
    customFilterBranches = cms.PSet(),
    vertexCollections = cms.PSet(
        vertices = cms.PSet(
            collection = cms.InputTag("slimmedOfflinePrimaryVertices"),
            branches = vertexBranches,
            maxCount = cms.int32(0),
        ),
    ),
    collections = cms.PSet(
        jets = cms.PSet(
            collection = cms.InputTag("slimmedJets"),
            branches = jetBranches,
            constituentBranches = constituentBranches,
            minCount = cms.int32(0),
            maxCount = cms.int32(0),
        ),
    ),
)
