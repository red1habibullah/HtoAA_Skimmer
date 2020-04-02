import os

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'lumiTree.root'
#options.inputFiles= '/store/mc/RunIISummer16MiniAODv2/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/2E1C211C-05C2-E611-90D3-02163E01306F.root' # WZ
#options.inputFiles = '/store/mc/RunIISummer16MiniAODv2/HPlusPlusHMinusHTo3L_M-500_13TeV-calchep-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/08ECD723-E4CA-E611-8C93-0CC47A1E0DC2.root' # Hpp3l
#options.inputFiles = '/store/data/Run2016G/DoubleMuon/MINIAOD/23Sep2016-v1/100000/0A30F7A9-ED8F-E611-91F1-008CFA1C6564.root' # ReReco
options.inputFiles = '/store/mc/RunIISummer16DR80Premix/SUSYGluGluToHToAA_AToMuMu_AToTauTau_M-5_TuneCUETP8M1_13TeV_madgraph_pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/100000/FAD2C483-8FDB-E611-8984-A0369F3102F6.root'
#options.inputFiles = '/store/data/Run2016H/DoubleMuon/MINIAOD/PromptReco-v3/000/284/036/00000/64591DD7-A79F-E611-954C-FA163E5A1368.root' # PromptReco
options.maxEvents = -1
options.register('skipEvents', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Events to skip")
options.register('reportEvery', 1000, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Report every")
options.register('isMC', 1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Sample is MC")
options.register('crab', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Make changes needed for crab")

options.parseArguments()

#####################
### setup process ###
#####################

process = cms.Process("LumiNtuple")

process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.Services_cff')

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True),
)

#################
### GlobalTag ###
#################
envvar = 'mcgt' if options.isMC else 'datagt'
from Configuration.AlCa.GlobalTag import GlobalTag
#GT = {'mcgt': 'auto:run2_mc', 'datagt': 'auto:run2_data'}
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
# https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC
GT = {'mcgt': '80X_mcRun2_asymptotic_2016_TrancheIV_v8', 'datagt': '80X_dataRun2_2016SeptRepro_v7'}
process.GlobalTag = GlobalTag(process.GlobalTag, GT[envvar], '')


#############################
### Setup rest of running ###
#############################
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    skipEvents = cms.untracked.uint32(options.skipEvents),
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string(options.outputFile),
)

process.schedule = cms.Schedule()

# add the analyzer
process.lumiTree = cms.EDAnalyzer("LumiTree",
    genEventInfo = cms.InputTag("generator"),
    lheEventProduct = cms.InputTag("externalLHEProducer"),
    doGenWeights = cms.bool(bool(options.isMC)),
)

process.lumiTreePath = cms.Path()
process.lumiTreePath += process.lumiTree
process.schedule.append(process.lumiTreePath)
