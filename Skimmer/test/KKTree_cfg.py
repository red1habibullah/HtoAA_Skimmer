import os

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'kkTree.root'
#options.inputFiles= 'store/mc/RunIIFall17MiniAODv2/QCD_Pt-15to7000_TuneCP5_Flat2017_13TeV_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/00A8A0A7-3243-E811-B01E-0025905B8576.root'
options.inputFiles = '/store/data/Run2017F/JetHT/MINIAOD/31Mar2018-v1/00000/08EAF6E4-2437-E811-8424-001E67E6F86E.root'
options.maxEvents = -1
options.register('skipEvents', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Events to skip")
options.register('reportEvery', 100, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Report every")
options.register('isMC', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Sample is MC")
options.register('isREMINIAOD', 1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Sample is ReMiniAOD")
options.register('reHLT', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Sample is reHLT")
options.register('runMetFilter', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Run the recommended MET filters")
options.register('crab', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Make changes needed for crab")

options.parseArguments()

#####################
### setup process ###
#####################

process = cms.Process("MiniNtuple")

process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi") # Needed by EGamma energy correction

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True),
)

process.RandomNumberGeneratorService = cms.Service(
    "RandomNumberGeneratorService",
    calibratedPatElectrons = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName = cms.untracked.string('TRandom3')
    ),
    calibratedPatPhotons = cms.PSet(
        initialSeed = cms.untracked.uint32(2),
        engineName = cms.untracked.string('TRandom3')
    ),
    mRoch = cms.PSet(
        initialSeed = cms.untracked.uint32(3),
        engineName = cms.untracked.string('TRandom3')
    ),
)

#################
### GlobalTag ###
#################
envvar = 'mcgt' if options.isMC else 'datagt'
from Configuration.AlCa.GlobalTag import GlobalTag
#GT = {'mcgt': 'auto:run2_mc', 'datagt': 'auto:run2_data'}
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
# https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC
GT = {'mcgt': '94X_mc2017_realistic_v10', 'datagt': '94X_dataRun2_ReReco_EOY17_v2'}
process.GlobalTag = GlobalTag(process.GlobalTag, GT[envvar], '')

##################
### JEC source ###
##################
# this is if we need to override the jec in global tag
#sqfile = 'DevTools/Ntuplizer/data/{0}.db'.format('Summer16_23Sep2016V3_MC' if options.isMC else 'Summer16_23Sep2016AllV3_DATA')
#if options.crab: sqfile = 'src/{0}'.format(sqfile) # uncomment to submit to crab
#tag = 'JetCorrectorParametersCollection_Summer16_23Sep2016AllV3_DATA_AK4PFchs'
#if options.isMC: tag = 'JetCorrectorParametersCollection_Summer16_23Sep2016V3_MC_AK4PFchs' # MoriondMC
#process.load("CondCore.DBCommon.CondDBCommon_cfi")
#from CondCore.DBCommon.CondDBSetup_cfi import *
#process.jec = cms.ESSource("PoolDBESSource",
#    DBParameters = cms.PSet(
#        messageLevel = cms.untracked.int32(0)
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(
#        cms.PSet(
#            record = cms.string('JetCorrectionsRecord'),
#            tag    = cms.string(tag),
#            label  = cms.untracked.string('AK4PFchs')
#        ),
#    ), 
#    connect = cms.string('sqlite:{0}'.format(sqfile)),
#)
#process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

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

###########################
### Profiling utilities ###
###########################

#process.ProfilerService = cms.Service (
#      "ProfilerService",
#       firstEvent = cms.untracked.int32(2),
#       lastEvent = cms.untracked.int32(500),
#       paths = cms.untracked.vstring('schedule') 
#)

#process.SimpleMemoryCheck = cms.Service(
#    "SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1)
#)

### To use IgProf's neat memory profiling tools, uncomment the following 
### lines then run this cfg with igprof like so:
###      $ igprof -d -mp -z -o igprof.mp.gz cmsRun ... 
### this will create a memory profile every 250 events so you can track use
### Turn the profile into text with
###      $ igprof-analyse -d -v -g -r MEM_LIVE igprof.yourOutputFile.gz > igreport_live.res
### To do a performance profile instead of a memory profile, change -mp to -pp
### in the first command and remove  -r MEM_LIVE from the second
### For interpretation of the output, see http://igprof.org/text-output-format.html

#from IgTools.IgProf.IgProfTrigger import igprof
#process.load("IgTools.IgProf.IgProfTrigger")
#process.igprofPath = cms.Path(process.igprof)
#process.igprof.reportEventInterval     = cms.untracked.int32(250)
#process.igprof.reportToFileAtBeginJob  = cms.untracked.string("|gzip -c>igprof.begin-job.gz")
#process.igprof.reportToFileAtEvent = cms.untracked.string("|gzip -c>igprof.%I.%E.%L.%R.event.gz")
#process.schedule.append(process.igprofPath)

# first create collections to analyze
collections = {
    'genParticles' : 'prunedGenParticles',
    'electrons'    : 'slimmedElectrons',
    'muons'        : 'slimmedMuons',
    'taus'         : 'slimmedTaus',
    'photons'      : 'slimmedPhotons',
    'jets'         : 'slimmedJets',
    'pfmet'        : 'slimmedMETs',
    'rho'          : 'fixedGridRhoFastjetAll',
    'vertices'     : 'offlineSlimmedPrimaryVertices',
    'packed'       : 'packedPFCandidates',
    'neutral'      : 'packedPFCandidates',
    'kshort'       : 'slimmedKshortVertices',
}

# the selections for each object (to be included in ntuple)
# will always be the last thing done to the collection, so can use embedded things from previous steps
selections = {
    'electrons'   : 'pt>10 && abs(eta)<2.5',
    'muons'       : 'pt>10 && abs(eta)<2.4',
    'taus'        : 'pt>20 && abs(eta)<2.3',
    'photons'     : 'pt>10 && abs(eta)<3.0',
    'jets'        : 'pt>15 && abs(eta)<4.7',
    #'kshort'      : 'numberOfDaughters==2', # need to figure out how to implement
    'neutral'     : 'pt>10 && pdgId==130', # pf puts all neutrals at 130 K_L
}
if options.isMC:
    selections['genParticles'] = 'pt>4'

patTypes = {
    'neutral': 'PackedCandidate',
}


# requirements to store events
minCounts = {
    #'electrons' : 1,
    #'muons'     : 1,
    #'taus'      : 1,
    #'photons'   : 1,
    #'jets'      : 1,
    'kshort'    : 1,
}

# maximum candidates to store
# selects the first n in the collection
# patobjects are pt ordered
# vertices has pv first
maxCounts = {
    'vertices': 1,
}

# selection for cleaning (objects should match final selection)
# just do at analysis level
cleaning = {
    #'jets' : {
    #    'electrons' : {
    #        'cut' : 'pt>10 && abs(eta)<2.5 && userInt("cutBasedElectronID-Spring15-25ns-V1-standalone-medium")>0.5 && userInt("WWLoose")>0.5',
    #        'dr'  : 0.3,
    #    },
    #    'muons' : {
    #        'cut' : 'pt>10 && abs(eta)<2.4 && isMediumMuon>0.5 && trackIso/pt<0.4 && userFloat("dxy")<0.02 && userFloat("dz")<0.1 && (pfIsolationR04().sumChargedHadronPt+max(0.,pfIsolationR04().sumNeutralHadronEt+pfIsolationR04().sumPhotonEt-0.5*pfIsolationR04().sumPUPt))/pt<0.15',
    #        'dr'  : 0.3,
    #    },
    #    'taus' : {
    #        'cut' : 'pt>20 && abs(eta)<2.3 && tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5 && tauID("decayModeFinding")>0.5',
    #        'dr'  : 0.3,
    #    },
    #},
}

# filters
filters = []

# met filters
#if options.runMetFilter:
# run all the time and store result
print 'Preparing MET filters'
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
hltFilter = hltHighLevel.clone()
# PAT if miniaod by itself (MC) and RECO if at the same time as reco (data)
hltFilter.TriggerResultsTag = cms.InputTag('TriggerResults', '', 'PAT') if options.isMC else cms.InputTag('TriggerResults', '', 'RECO')
hltFilter.throw = cms.bool(True)
# ICHEP recommendation
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#MiniAOD_8011_ICHEP_dataset
for flag in ['HBHENoiseFilter','HBHENoiseIsoFilter','EcalDeadCellTriggerPrimitiveFilter','goodVertices','eeBadScFilter','globalTightHalo2016Filter']:
    mod = hltFilter.clone(HLTPaths=cms.vstring('Flag_{0}'.format(flag)))
    modName = 'filter{0}'.format(flag)
    setattr(process,modName,mod)
    filters += [getattr(process,modName)]
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
filters += [process.BadChargedCandidateFilter]
    

# now do any customization/cleaning
print 'Customizing jets'
from DevTools.Ntuplizer.customizeJets import customizeJets
collections = customizeJets(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

print 'Customizing electrons'
from DevTools.Ntuplizer.customizeElectrons import customizeElectrons
collections = customizeElectrons(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

print 'Customizing muons'
from DevTools.Ntuplizer.customizeMuons import customizeMuons
collections = customizeMuons(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

print 'Customizing taus'
from DevTools.Ntuplizer.customizeTaus import customizeTaus
collections = customizeTaus(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

print 'Customizing photons'
from DevTools.Ntuplizer.customizePhotons import customizePhotons
collections = customizePhotons(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

print 'Customizing METs'
from DevTools.Ntuplizer.customizeMets import customizeMets
collections = customizeMets(
    process,
    collections,
    isMC=bool(options.isMC),
    isREMINIAOD=bool(options.isREMINIAOD),
    reHLT=bool(options.reHLT),
)

# select desired objects
print 'Selecting objects'
from DevTools.Ntuplizer.objectTools import objectSelector, objectCleaner
for coll in selections:
    collections[coll] = objectSelector(process,coll,collections[coll],selections[coll],patType=patTypes.get(coll,None))
# TODO: memory problem
#for coll in cleaning:
#    collections[coll] = objectCleaner(process,coll,collections[coll],collections,cleaning[coll])

# add the analyzer
process.load("DevTools.Ntuplizer.KKTree_cfi")

process.miniTree.isData = not options.isMC
process.miniTree.filterResults = cms.InputTag('TriggerResults', '', 'PAT') if options.isMC else cms.InputTag('TriggerResults', '', 'RECO')
#process.miniTree.filterResults = cms.InputTag('TriggerResults', '', 'PAT')
process.miniTree.vertexCollections.vertices.collection = collections['vertices']
if options.isMC:
    from DevTools.Ntuplizer.branchTemplates import genParticleBranches 
    process.miniTree.collections.genParticles = cms.PSet(
        collection = cms.InputTag(collections['genParticles']),
        branches = genParticleBranches,
    )
#process.miniTree.collections.electrons.collection = collections['electrons']
#process.miniTree.collections.muons.collection = collections['muons']
#process.miniTree.collections.taus.collection = collections['taus']
#process.miniTree.collections.photons.collection = collections['photons']
process.miniTree.collections.jets.collection = collections['jets']
process.miniTree.collections.pfmet.collection = collections['pfmet']
process.miniTree.collections.kshort.collection = collections['kshort']
process.miniTree.collections.neutral.collection = collections['neutral']
process.miniTree.rho = collections['rho']
for coll, count in minCounts.iteritems():
    if  process.miniTree.vertexCollections.hasParameter(coll):
        getattr(process.miniTree.vertexCollections,coll).minCount = cms.int32(count)
    if process.miniTree.collections.hasParameter(coll):
        getattr(process.miniTree.collections,coll).minCount = cms.int32(count)
    else:
        print 'Unrecognized collection {0}'.format(coll)
for coll, count in maxCounts.iteritems():
    if process.miniTree.vertexCollections.hasParameter(coll):
        getattr(process.miniTree.vertexCollections,coll).maxCount = cms.int32(count)
    elif process.miniTree.collections.hasParameter(coll):
        getattr(process.miniTree.collections,coll).maxCount = cms.int32(count)
    else:
        print 'Unrecognized collection {0}'.format(coll)

process.miniTreePath = cms.Path()
for f in filters:
    process.miniTreePath += cms.ignore(f)
process.miniTreePath += process.miniTree
process.schedule.append(process.miniTreePath)
