import FWCore.ParameterSet.Config as cms

# load branches
from DevTools.Ntuplizer.branchTemplates import *

miniTree = cms.EDAnalyzer("MiniTree",
    isData = cms.bool(True),
    genEventInfo = cms.InputTag("generator"),
    nevents = cms.InputTag('lumiSummary','numberOfEvents'),
    summedWeights = cms.InputTag('lumiSummary','sumOfWeightedEvents'),
    lheEventProduct = cms.InputTag("externalLHEProducer"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    pileupSummaryInfo = cms.InputTag("slimmedAddPileupInfo"),
    triggerResults = cms.InputTag("TriggerResults","","HLT"),
    filterResults = cms.InputTag("TriggerResults","","PAT"),
    triggerObjects = cms.InputTag("selectedPatTrigger"),
    triggerPrescales = cms.InputTag("patTrigger"),
    triggerBranches = triggerBranches,
    filterBranches = filterBranches,
    customFilterBranches = customFilterBranches,
    vertexCollections = cms.PSet(
        vertices = cms.PSet(
            collection = cms.InputTag("slimmedOfflinePrimaryVertices"),
            branches = vertexBranches,
            maxCount = cms.int32(0),
        ),
    ),
    collections = cms.PSet(
        #electrons = cms.PSet(
        #    collection = cms.InputTag("slimmedElectrons"),
        #    branches = electronBranches,
        #    minCount = cms.int32(0),
        #    maxCount = cms.int32(0),
        #),
        #muons = cms.PSet(
        #    collection = cms.InputTag("slimmedMuons"),
        #    branches = muonBranches,
        #    minCount = cms.int32(0),
        #    maxCount = cms.int32(0),
        #),
        #taus = cms.PSet(
        #    collection = cms.InputTag("slimmedTaus"),
        #    branches = tauBranches,
        #    minCount = cms.int32(0),
        #    maxCount = cms.int32(0),
        #),
        #photons = cms.PSet(
        #    collection = cms.InputTag("slimmedPhotons"),
        #    branches = photonBranches,
        #    minCount = cms.int32(0),
        #    maxCount = cms.int32(0),
        #),
        jets = cms.PSet(
            collection = cms.InputTag("slimmedJets"),
            branches = jetBranches,
            minCount = cms.int32(0),
            maxCount = cms.int32(0),
        ),
        pfmet = cms.PSet(
            collection = cms.InputTag("slimmedMETs"),
            branches = metBranches,
            minCount = cms.int32(0),
            maxCount = cms.int32(0),
        ),
        kshort = cms.PSet(
            collection = cms.InputTag("slimmedKshortVertices"),
            branches = vertexCompositeBranches,
            minCount = cms.int32(0),
            maxCount = cms.int32(0),
        ),
        neutral = cms.PSet(
            collection = cms.InputTag("packedPFCandidates"),
            branches = packedBranches,
            minCount = cms.int32(0),
            maxCount = cms.int32(0),
        ),
    ),
)
