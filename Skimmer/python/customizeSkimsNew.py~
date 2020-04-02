import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.tools.helpers as configtools
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
#from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC, miniAOD_customizeAllData
# lower the pt threshold
def lowerTauPt(process,postfix='',tauPt=8, jetPt=5):
    from FWCore.ParameterSet.MassReplace import massSearchReplaceParam
    massSearchReplaceParam(getattr(process,'PATTauSequence'+postfix),'minJetPt',14,jetPt)
    #getattr(process,'selectedPatTaus'+postfix).cut = cms.string("pt > {} && tauID(\'decayModeFindingNewDMs\')> 0.5".format(tauPt))
    getattr(process,'selectedPatTaus'+postfix).cut = cms.string("pt > {}".format(tauPt))
def addMuMuTauTau(process,options,**kwargs):
    doMM = kwargs.pop('doMM',False)
    doMT = kwargs.pop('doMT',False)

    process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
    patAlgosToolsTask = configtools.getPatAlgosToolsTask(process)

    process.PATTauSequence = cms.Sequence(process.PFTau+process.makePatTaus+process.selectedPatTaus)
    #process.recoTauAK4PFJets08Region.pfCandAssocMapSrc = cms.InputTag("")

    #from RecoTauTag.Configuration.HPSPFTaus_cff import *
    
    #process.UnCleanedHPSPFTausTask=cms.Task(produceHPSPFTausTask)
    #patAlgosToolsTask.add(process.UnCleanedHPSPFTausTask)

     #########################
     ### UnCleaned Taus ###
     #########################
    #from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets 
    
    #process.ak4PFJetsUnCleaned=ak4PFJets.clone()
    #patAlgosToolsTask.add(process.ak4PFJetsUnCleaned)
    
    #jetSrc='ak4PFJetsUnCleaned'
    
    #process.PATTauSequenceUnCleaned = cloneProcessingSnippet(process,process.PATTauSequence,'UnCleaned', addToTask = True)
    #massSearchReplaceAnyInputTag(process.PATTauSequenceUnCleaned,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    #process.slimmedTausUnCleaned = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausUnCleaned'))
    #patAlgosToolsTask.add(process.slimmedTausUnCleaned)
    #lowerTauPt(process,'UnCleaned')
    




    #########################
    ### Muon Cleaned Taus ###
    #########################
    ############### Loose ###############
    process.recoMuonsForJetCleaning = cms.EDFilter('MuonRefSelector',
                                                   src = cms.InputTag('muons'),
                                                   cut = cms.string('pt > 3.0 && isPFMuon && (isGlobalMuon || isTrackerMuon)'),
                                                   #cut = cms.string('pt > 3.0 && selectors(reco::Muon::CutBasedIdMedium)'),
                                                   #cut = cms.string('pt > 3.0 && CutBasedIdMedium'),
                                                   
                                                   )

    process.ak4PFJetsMuonCleaned = cms.EDProducer(
        'MuonCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        muonSrc = cms.InputTag("recoMuonsForJetCleaning"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),

        
        )
    
    process.muonCleanedHPSPFTausTask = cms.Task(
        process.recoMuonsForJetCleaning,
        process.ak4PFJetsMuonCleaned
    )
    patAlgosToolsTask.add(process.muonCleanedHPSPFTausTask)
    
    jetSrc = 'ak4PFJetsMuonCleaned'
    pfAssocMap = cms.InputTag('ak4PFJetsMuonCleaned','pfCandAssocMapForIsolation')
    process.PATTauSequenceMuonCleaned = cloneProcessingSnippet(process,process.PATTauSequence, 'MuonCleaned', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceMuonCleaned,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
     



    process.recoTauAK4PFJets08RegionMuonCleaned.pfCandAssocMapSrc = pfAssocMap
    #process.recoTauAK4PFJets08RegionMuonCleaned.pfCandSrc = 
    process.slimmedTausMuonCleaned = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausMuonCleaned'))
    patAlgosToolsTask.add(process.slimmedTausMuonCleaned)
    lowerTauPt(process,'MuonCleaned')
    
    if options.isMC:
        process.tauGenJetsMuonCleaned.GenParticles = "prunedGenParticles"
        process.patTausMuonCleaned.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausMuonCleaned"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausMuonCleaned',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausMuonCleaned'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsMuonCleaned'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsMuonCleaned'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)
    ########## Medium ##############
    process.recoMuonsForJetCleaningMedium = cms.EDFilter('MuonRefSelector',
                                                   src = cms.InputTag('muons'),
                                                   #cut = cms.string('pt > 3.0 && isPFMuon && (isGlobalMuon || isTrackerMuon)'),
                                                   #cut = cms.string('pt > 3.0 && selectors(reco::Muon::CutBasedIdMedium)'),
                                                   #cut = cms.string('pt > 3.0 && CutBasedIdMedium'),
                                                   cut = cms.string('pt > 3.0 && passed("CutBasedIdMedium")'), 
                                                   )
    process.ak4PFJetsMuonCleanedMedium = cms.EDProducer(
        'MuonCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        muonSrc = cms.InputTag("recoMuonsForJetCleaningMedium"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),
        )
    
    process.muonCleanedMediumHPSPFTausTask = cms.Task(
        process.recoMuonsForJetCleaningMedium,
        process.ak4PFJetsMuonCleanedMedium
        )
    patAlgosToolsTask.add(process.muonCleanedMediumHPSPFTausTask)

    jetSrc = 'ak4PFJetsMuonCleanedMedium'
    pfAssocMap = cms.InputTag('ak4PFJetsMuonCleanedMedium','pfCandAssocMapForIsolation')
    process.PATTauSequenceMuonCleanedMedium = cloneProcessingSnippet(process,process.PATTauSequence, 'MuonCleanedMedium', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceMuonCleanedMedium,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    
    process.recoTauAK4PFJets08RegionMuonCleanedMedium.pfCandAssocMapSrc = pfAssocMap
    process.slimmedTausMuonCleanedMedium = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausMuonCleanedMedium'))
    patAlgosToolsTask.add(process.slimmedTausMuonCleanedMedium)
    lowerTauPt(process,'MuonCleanedMedium')
    
    if options.isMC:
        process.tauGenJetsMuonCleanedMedium.GenParticles = "prunedGenParticles"
        process.patTausMuonCleanedMedium.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausMuonCleanedMedium"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausMuonCleanedMedium',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausMuonCleanedMedium'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsMuonCleanedMedium'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsMuonCleanedMedium'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)

        ########## Tight ##############                                                                                                                                                                                                                                           
    process.recoMuonsForJetCleaningTight = cms.EDFilter('MuonRefSelector',
                                                   src = cms.InputTag('muons'),
                                                   
                                                   cut = cms.string('pt > 3.0 && passed("CutBasedIdTight")'),

                                                   )
    process.ak4PFJetsMuonCleanedTight = cms.EDProducer(
        'MuonCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        muonSrc = cms.InputTag("recoMuonsForJetCleaningTight"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),
        )

    process.muonCleanedTightHPSPFTausTask = cms.Task(
        process.recoMuonsForJetCleaningTight,
        process.ak4PFJetsMuonCleanedTight
        )
    patAlgosToolsTask.add(process.muonCleanedTightHPSPFTausTask)
    
    jetSrc = 'ak4PFJetsMuonCleanedTight'
    pfAssocMap = cms.InputTag('ak4PFJetsMuonCleanedTight','pfCandAssocMapForIsolation')
    process.PATTauSequenceMuonCleanedTight = cloneProcessingSnippet(process,process.PATTauSequence, 'MuonCleanedTight', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceMuonCleanedTight,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    
    process.recoTauAK4PFJets08RegionMuonCleanedTight.pfCandAssocMapSrc = pfAssocMap
    process.slimmedTausMuonCleanedTight = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausMuonCleanedTight'))
    patAlgosToolsTask.add(process.slimmedTausMuonCleanedTight)
    lowerTauPt(process,'MuonCleanedTight')
    

    if options.isMC:
        process.tauGenJetsMuonCleanedTight.GenParticles = "prunedGenParticles"
        process.patTausMuonCleanedTight.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausMuonCleanedTight"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausMuonCleanedTight',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausMuonCleanedTight'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsMuonCleanedTight'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsMuonCleanedTight'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)


    #############################
    ### Electron cleaned taus ###
    #############################
        ########### Loose ###################    
    process.recoElectronsForJetCleaning = cms.EDFilter('ElectronFilter',
                                                       vertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                                       Rho = cms.InputTag("fixedGridRhoFastjetAll"),
                                                       electrons = cms.InputTag("gedGsfElectrons"),
                                                       conv = cms.InputTag("conversions"),
                                                       BM = cms.InputTag("offlineBeamSpot"),
                                                       Tracks = cms.InputTag("electronGsfTracks"),
                                                       #Passcount =cms.uint32(1),
                                                       )
    process.ak4PFJetsElectronCleaned = cms.EDProducer(
        'ElectronCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        #electronSrc = cms.InputTag("recoElectronsForJetCleaning","MediumElectronRef"),  
        electronSrc = cms.InputTag("recoElectronsForJetCleaning","LooseElectronRef"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),
        )
    
    process.electronCleanedHPSPFTausTask = cms.Task(
        process.recoElectronsForJetCleaning,
        process.ak4PFJetsElectronCleaned
    )
    
    patAlgosToolsTask.add(process.electronCleanedHPSPFTausTask)
    
    jetSrc = 'ak4PFJetsElectronCleaned'
    
    pfAssocMaps = cms.InputTag('ak4PFJetsElectronCleaned','pfCandAssocMapForIsolation')
    #process.AssociationMapElectron=cms.InputTag('ak4PFJetsElectronCleaned','pfCandAssocMapForElectron')
   # patAlgosToolsTask.add(process.AssociationMapElectron)
    process.PATTauSequenceElectronCleaned = cloneProcessingSnippet(process,process.PATTauSequence, 'ElectronCleaned', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceElectronCleaned,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    process.recoTauAK4PFJets08RegionElectronCleaned.pfCandAssocMapSrc = pfAssocMaps
    process.slimmedTausElectronCleaned = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausElectronCleaned'))
    patAlgosToolsTask.add(process.slimmedTausElectronCleaned)
    lowerTauPt(process,'ElectronCleaned')

    




    
    if options.isMC:
        process.tauGenJetsElectronCleaned.GenParticles = "prunedGenParticles"
        process.patTausElectronCleaned.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausElectronCleaned"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausElectronCleaned',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausElectronCleaned'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsElectronCleaned'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsElectronCleaned'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)

    ########## Medium ############
            
    process.recoElectronsForJetCleaningMedium = cms.EDFilter('ElectronFilterMedium',
                                                       vertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                                       Rho = cms.InputTag("fixedGridRhoFastjetAll"),
                                                       electrons = cms.InputTag("gedGsfElectrons"),
                                                       conv = cms.InputTag("conversions"),
                                                       BM = cms.InputTag("offlineBeamSpot"),
                                                       Tracks = cms.InputTag("electronGsfTracks"),
                                                       #Passcount =cms.uint32(1),                                                             
                                                       )
    process.ak4PFJetsElectronCleanedMedium = cms.EDProducer(
        'ElectronCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        electronSrc = cms.InputTag("recoElectronsForJetCleaningMedium","MediumElectronRef"),  
        #electronSrc = cms.InputTag("recoElectronsForJetCleaning","LooseElectronRef"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),
        )
    process.electronCleanedMediumHPSPFTausTask = cms.Task(
        process.recoElectronsForJetCleaningMedium,
        process.ak4PFJetsElectronCleanedMedium
    )
    
    patAlgosToolsTask.add(process.electronCleanedMediumHPSPFTausTask)
    
    jetSrc = 'ak4PFJetsElectronCleanedMedium'
    
    pfAssocMaps = cms.InputTag('ak4PFJetsElectronCleanedMedium','pfCandAssocMapForIsolation')
    process.PATTauSequenceElectronCleanedMedium = cloneProcessingSnippet(process,process.PATTauSequence, 'ElectronCleanedMedium', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceElectronCleanedMedium,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    process.recoTauAK4PFJets08RegionElectronCleanedMedium.pfCandAssocMapSrc = pfAssocMaps
    process.slimmedTausElectronCleanedMedium = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausElectronCleanedMedium'))
    patAlgosToolsTask.add(process.slimmedTausElectronCleanedMedium)
    lowerTauPt(process,'ElectronCleanedMedium')


        
    if options.isMC:
        process.tauGenJetsElectronCleanedMedium.GenParticles = "prunedGenParticles"
        process.patTausElectronCleanedMedium.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausElectronCleanedMedium"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausElectronCleanedMedium',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausElectronCleanedMedium'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsElectronCleanedMedium'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsElectronCleanedMedium'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)


    ########## Tight ############  
    process.recoElectronsForJetCleaningTight = cms.EDFilter('ElectronFilterTight',
                                                       vertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                                       Rho = cms.InputTag("fixedGridRhoFastjetAll"),
                                                       electrons = cms.InputTag("gedGsfElectrons"),
                                                       conv = cms.InputTag("conversions"),
                                                       BM = cms.InputTag("offlineBeamSpot"),
                                                       Tracks = cms.InputTag("electronGsfTracks"),
                                                       #Passcount =cms.uint32(1),                                                             
                                                            )
    process.ak4PFJetsElectronCleanedTight = cms.EDProducer(
        'ElectronCleanedJetProducer',
        jetSrc = cms.InputTag("ak4PFJets"),
        electronSrc = cms.InputTag("recoElectronsForJetCleaningTight","TightElectronRef"),  
        #electronSrc = cms.InputTag("recoElectronsForJetCleaning","LooseElectronRef"),
        pfCandSrc = cms.InputTag("particleFlow"),
        pfCandCollection=cms.InputTag("particleFlow"),
        )
    process.electronCleanedTightHPSPFTausTask = cms.Task(
        process.recoElectronsForJetCleaningTight,
        process.ak4PFJetsElectronCleanedTight
    )
    
    patAlgosToolsTask.add(process.electronCleanedTightHPSPFTausTask)
    
    jetSrc = 'ak4PFJetsElectronCleanedTight'
    
    pfAssocMaps = cms.InputTag('ak4PFJetsElectronCleanedTight','pfCandAssocMapForIsolation')
    process.PATTauSequenceElectronCleanedTight = cloneProcessingSnippet(process,process.PATTauSequence, 'ElectronCleanedTight', addToTask = True)
    massSearchReplaceAnyInputTag(process.PATTauSequenceElectronCleanedTight,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))
    process.recoTauAK4PFJets08RegionElectronCleanedTight.pfCandAssocMapSrc = pfAssocMaps
    process.slimmedTausElectronCleanedTight = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausElectronCleanedTight'))
    patAlgosToolsTask.add(process.slimmedTausElectronCleanedTight)
    lowerTauPt(process,'ElectronCleanedTight')
    
    if options.isMC:
        process.tauGenJetsElectronCleanedTight.GenParticles = "prunedGenParticles"
        process.patTausElectronCleanedTight.embedGenMatch = False
    else:
        from PhysicsTools.PatAlgos.tools.coreTools import _removeMCMatchingForPATObject
        attrsToDelete = []
        postfix = ''
        print "removing MC dependencies for tausElectronCleanedTight"
        _removeMCMatchingForPATObject(process, 'tauMatch', 'patTausElectronCleanedTight',postfix)
        ## remove mc extra configs for taus
        tauProducer = getattr(process,'patTausElectronCleanedTight'+postfix)
        tauProducer.addGenJetMatch   = False
        tauProducer.embedGenJetMatch = False
        attrsToDelete += [tauProducer.genJetMatch.getModuleLabel()]
        tauProducer.genJetMatch      = ''
        attrsToDelete += ['tauGenJetsElectronCleanedTight'+postfix]
        attrsToDelete += ['tauGenJetsSelectorAllHadronsElectronCleanedTight'+postfix]
        for attr in attrsToDelete:
            if hasattr(process,attr): delattr(process,attr)

    
    #############################
    ### lower pt for nonclean ###
    #############################
    lowerTauPt(process)
    
    ############################
    ### lower pt for boosted ###
    ############################
    process.ca8PFJetsCHSprunedForBoostedTaus.jetPtMin = cms.double(20.0)
    process.ca8PFJetsCHSprunedForBoostedTaus.subjetPtMin = cms.double(8.0)
    lowerTauPt(process,'Boosted')

    #######################################
    ### Update btagging for cleaned jet ###
    #######################################
    #### Loose ####
    from RecoJets.JetAssociationProducers.j2tParametersVX_cfi import j2tParametersVX
    process.ak4PFJetsMuonCleanedTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
        j2tParametersVX,
        jets = cms.InputTag("ak4PFJetsMuonCleaned")
    )
    process.patJetMuonCleanedCharge = cms.EDProducer("JetChargeProducer",
        src = cms.InputTag("ak4PFJetsMuonCleanedTracksAssociatorAtVertex"),
        var = cms.string('Pt'),
        exp = cms.double(1.0)
    )
    
    from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
    addJetCollection(process, postfix   = "", labelName = 'MuonCleaned', jetSource = cms.InputTag('ak4PFJetsMuonCleaned'),
                    jetCorrections = ('AK4PF', ['L2Relative', 'L3Absolute'], ''),
                    algo= 'AK', rParam = 0.4, btagDiscriminators = map(lambda x: x.value() ,process.patJets.discriminatorSources)
                    )
    
    if options.isMC: process.patJetGenJetMatchMuonCleaned.matched = 'slimmedGenJets'
    process.patJetsMuonCleaned.jetChargeSource = cms.InputTag("patJetMuonCleanedCharge")
    
    process.slimmedJetsMuonCleaned = process.slimmedJets.clone(src = cms.InputTag("selectedPatJetsMuonCleaned"))
    
    #### Medium ####
    # from RecoJets.JetAssociationProducers.j2tParametersVX_cfi import j2tParametersVX
    # process.ak4PFJetsMuonCleanedTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
    #     j2tParametersVX,
    #     jets = cms.InputTag("ak4PFJetsMuonCleanedMedium")
    # )
    # process.patJetMuonCleanedCharge = cms.EDProducer("JetChargeProducer",
    #     src = cms.InputTag("ak4PFJetsMuonCleanedMediumTracksAssociatorAtVertex"),
    #     var = cms.string('Pt'),
    #     exp = cms.double(1.0)
    # )

    # from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
    # addJetCollection(process, postfix   = "", labelName = 'MuonCleanedMedium', jetSource = cms.InputTag('ak4PFJetsMuonCleaned'),
    #                 jetCorrections = ('AK4PF', ['L2Relative', 'L3Absolute'], ''),
    #                 algo= 'AK', rParam = 0.4, btagDiscriminators = map(lambda x: x.value() ,process.patJets.discriminatorSources)
    #                 )

    # if options.isMC: process.patJetGenJetMatchMuonCleaned.matched = 'slimmedGenJets'
    # process.patJetsMuonCleaned.jetChargeSource = cms.InputTag("patJetMuonCleanedCharge")

    # process.slimmedJetsMuonCleaned = process.slimmedJets.clone(src = cms.InputTag("selectedPatJetsMuonCleanedMedium"))

    #################
    ### Skim Path ###
    #################
    process.main_path = cms.Path()
    process.main_path_em = cms.Path()
    process.main_path_et = cms.Path()
    process.main_path_et_med=cms.Path()
    process.main_path_et_tht=cms.Path()
    process.main_path_mt = cms.Path()
    process.main_path_mt_med = cms.Path()
    process.main_path_mt_tht = cms.Path()
    #process.main_path_ut=cms.Path()
    process.main_path_tt = cms.Path()
    process.z_path = cms.Path()
    process.z_tau_eff_path = cms.Path()
    process.z_tau_eff_muclean_path = cms.Path()
    
    # currently set to be fast, only using collections in AOD
    # can switch back to using MINIAOD collections but they will need to be produced first
    
    # preskim
    
    ###############
    ### Trigger ###
    ###############
    process.HLT =cms.EDFilter("HLTHighLevel",
         TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
         #HLTPaths = cms.vstring("HLT_IsoMu27_v*", "HLT_IsoTkMu27_v*"), # 2017
         HLTPaths = cms.vstring("HLT_IsoMu24_v*", "HLT_IsoTkMu24_v*", "HLT_IsoMu27_v*", "HLT_IsoTkMu27_v*"),
         eventSetupPathsKey = cms.string(''),
         andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
         throw = cms.bool(False) # throw exception on unknown path names
    )
    process.main_path *= process.HLT
    process.main_path_em *= process.HLT
    process.main_path_et *= process.HLT
    process.main_path_et_med *=process.HLT
    process.main_path_et_tht *=process.HLT
    process.main_path_mt *= process.HLT
    process.main_path_mt_med *= process.HLT
    process.main_path_mt_tht *= process.HLT
    #process.main_path_ut *= process.HLT
    process.main_path_tt *= process.HLT
    process.z_path *= process.HLT
    process.z_tau_eff_path *= process.HLT
    process.z_tau_eff_muclean_path *= process.HLT
    
    #########################
    ### Muon ID embedding ###
    #########################
    #process.slimmedMuonsWithID = cms.EDProducer("MuonIdEmbedder",
    #    src = cms.InputTag('slimmedMuons'),
    #    vertexSrc = cms.InputTag('offlineSlimmedPrimaryVertices'),
    #)
    #process.main_path *= process.slimmedMuonsWithID
    #process.z_path *= process.slimmedMuonsWithID
    
    ###############
    ### Muon ID ###
    ###############
    #process.analysisMuonsNoIso = cms.EDFilter('PATMuonSelector',
    process.analysisMuonsNoIso = cms.EDFilter('MuonSelector',
        #src = cms.InputTag('slimmedMuonsWithID'),
        src = cms.InputTag('muons'),
        #cut = cms.string('pt > 3.0 && abs(eta)<2.4 && (isMediumMuon || userInt("isMediumMuonICHEP"))'),
        cut = cms.string('pt > 3.0 && abs(eta)<2.4 && isPFMuon && (isGlobalMuon || isTrackerMuon)'),
    )
    #process.analysisMuonsIso = cms.EDFilter('PATMuonSelector',
    process.analysisMuonsIso = cms.EDFilter('MuonSelector',
        src = cms.InputTag('analysisMuonsNoIso'),
        cut = cms.string('(pfIsolationR04().sumChargedHadronPt'
                         '+ max(0., pfIsolationR04().sumNeutralHadronEt'
                         '+ pfIsolationR04().sumPhotonEt'
                         '- 0.5*pfIsolationR04().sumPUPt))'
                         '/pt()<0.25'),
    )
    process.analysisMuonsNoIsoCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(2),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisMuonsNoIso'),
    )
    process.analysisMuonsIsoCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(2),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisMuonsIso'),
    )
    process.analysisMuonsNoIsoCountTauEff = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisMuonsNoIso'),
    )
    process.main_path *= process.analysisMuonsNoIso
    process.main_path *= process.analysisMuonsNoIsoCount
    process.main_path_em *= process.analysisMuonsNoIso
    process.main_path_em *= process.analysisMuonsNoIsoCount
    process.main_path_et *= process.analysisMuonsNoIso
    process.main_path_et *= process.analysisMuonsNoIsoCount
    process.main_path_et_med *= process.analysisMuonsNoIso
    process.main_path_et_med *= process.analysisMuonsNoIsoCount
    process.main_path_et_tht *= process.analysisMuonsNoIso
    process.main_path_et_tht *= process.analysisMuonsNoIsoCount
    process.main_path_mt *= process.analysisMuonsNoIso
    process.main_path_mt *= process.analysisMuonsNoIsoCount
    process.main_path_mt_med *= process.analysisMuonsNoIso
    process.main_path_mt_med *= process.analysisMuonsNoIsoCount
    process.main_path_mt_tht *= process.analysisMuonsNoIso
    process.main_path_mt_tht *= process.analysisMuonsNoIsoCount
    #process.main_path_ut *= process.analysisMuonsNoIso
    #process.main_path_ut *= process.analysisMuonsNoIsoCount
    process.main_path_tt *= process.analysisMuonsNoIso
    process.main_path_tt *= process.analysisMuonsNoIsoCount
    process.z_path *= process.analysisMuonsNoIso
    process.z_path *= process.analysisMuonsNoIsoCount
    process.z_tau_eff_path *= process.analysisMuonsNoIso
    process.z_tau_eff_path *= process.analysisMuonsNoIsoCountTauEff
    process.z_tau_eff_muclean_path *= process.analysisMuonsNoIso
    process.z_tau_eff_muclean_path *= process.analysisMuonsNoIsoCountTauEff
    
    #########################
    ### Trigger Threshold ###
    #########################
    #process.triggerMuon = cms.EDFilter('PATMuonSelector',
    process.triggerMuon = cms.EDFilter('MuonSelector',
        #src = cms.InputTag('secondMuon'),
        src = cms.InputTag('analysisMuonsNoIso'),
        #cut = cms.string('pt > 27.0'),
        cut = cms.string('pt > 24.0'),
    )
    process.triggerMuonCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('triggerMuon'),
    )
    process.main_path *= process.triggerMuon
    process.main_path *= process.triggerMuonCount
    process.main_path_em *= process.triggerMuon
    process.main_path_em *= process.triggerMuonCount
    process.main_path_et *= process.triggerMuon
    process.main_path_et *= process.triggerMuonCount
    process.main_path_et_med *= process.triggerMuon
    process.main_path_et_med *= process.triggerMuonCount
    process.main_path_et_tht *= process.triggerMuon
    process.main_path_et_tht *= process.triggerMuonCount
    process.main_path_mt *= process.triggerMuon
    process.main_path_mt *= process.triggerMuonCount
    process.main_path_mt_med *= process.triggerMuon
    process.main_path_mt_med *= process.triggerMuonCount
    process.main_path_mt_tht *= process.triggerMuon
    process.main_path_mt_tht *= process.triggerMuonCount
    #process.main_path_ut *= process.triggerMuon
    #process.main_path_ut *= process.triggerMuonCount
    process.main_path_tt *= process.triggerMuon
    process.main_path_tt *= process.triggerMuonCount
    process.z_path *= process.triggerMuon
    process.z_path *= process.triggerMuonCount
    process.z_tau_eff_path *= process.triggerMuon
    process.z_tau_eff_path *= process.triggerMuonCount
    process.z_tau_eff_muclean_path *= process.triggerMuon
    process.z_tau_eff_muclean_path *= process.triggerMuonCount
    
    ############################
    ### Require two OS muons ###
    ############################
    process.mumu = cms.EDProducer("CandViewShallowCloneCombiner",
        decay = cms.string("{0}@+ {0}@-".format('slimmedMuons')),
        #cut   = cms.string("deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi)<1.5 && mass<60"),
        cut   = cms.string("1<mass<65"),
    )
    process.mumuCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('mumu'),
    )
    process.main_path *= process.mumu
    process.main_path *= process.mumuCount
    process.main_path_em *= process.mumu
    process.main_path_em *= process.mumuCount
    process.main_path_et *= process.mumu
    process.main_path_et *= process.mumuCount
    process.main_path_et_med *= process.mumu
    process.main_path_et_med *= process.mumuCount
    process.main_path_mt *= process.mumu
    process.main_path_mt *= process.mumuCount
    process.main_path_mt_med *= process.mumu
    process.main_path_mt_med *= process.mumuCount
    process.main_path_mt_tht *= process.mumu
    process.main_path_mt_tht *= process.mumuCount
    #process.main_path_ut *= process.mumu
    #process.main_path_ut *= process.mumuCount
    process.main_path_tt *= process.mumu
    process.main_path_tt *= process.mumuCount
    
    process.mumuZ = cms.EDProducer("CandViewShallowCloneCombiner",
        decay = cms.string("{0}@+ {0}@-".format('slimmedMuons')),
        cut   = cms.string("60<mass<120"),
    )
    process.mumuZCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('mumuZ'),
    )
    process.z_path *= process.mumuZ
    process.z_path *= process.mumuZCount
    
    ########################
    ### Tau requirements ###
    ########################
    process.analysisTaus = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTaus'),
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    # process.analysisTausUnCleaned = cms.EDFilter('PATTauSelector',
    #     src = cms.InputTag('slimmedTausUnCleaned'),
    #     cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    #                                              )
    # process.analysisTausUnCleanedCount = cms.EDFilter("PATCandViewCountFilter",
    #      minNumber = cms.uint32(1),
    #      maxNumber = cms.uint32(999),
    #      src = cms.InputTag('analysisTaus'),
    #)

    process.analysisTausMuonCleaned = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausMuonCleaned'),
        #src = cms.InputTag('selectedPatTausMuonCleaned'),
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausMuonCleanedCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    process.analysisMuonsNoIsoMTCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(3),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisMuonsNoIso'),
    )
    process.analysisTausMuonCleanedMedium = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausMuonCleanedMedium'),
        #src = cms.InputTag('selectedPatTausMuonCleaned'),                                                                            
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausMuonCleanedMediumCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    
    process.analysisTausMuonCleanedTight = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausMuonCleanedTight'),
        #src = cms.InputTag('selectedPatTausMuonCleaned'),                                                                            
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausMuonCleanedTightCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    process.analysisTausElectronCleaned = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausElectronCleaned'),
        #src = cms.InputTag('selectedPatTausElectronCleaned'),
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausElectronCleanedCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    process.analysisTausElectronCleanedMedium = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausElectronCleanedMedium'),
        #src = cms.InputTag('selectedPatTausElectronCleaned'),
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausElectronCleanedMediumCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    process.analysisTausElectronCleanedTight = cms.EDFilter('PATTauSelector',
        src = cms.InputTag('slimmedTausElectronCleanedTight'),
        #src = cms.InputTag('selectedPatTausElectronCleaned'),
        cut = cms.string('pt > 8.0 && abs(eta)<2.3 && tauID(\'decayModeFinding\')> 0.5'),
    )
    process.analysisTausElectronCleanedTightCount = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('analysisTaus'),
    )
    process.main_path_mt *= process.analysisMuonsNoIsoMTCount
    process.main_path_mt *= process.analysisTausMuonCleaned
    process.main_path_mt *= process.analysisTausMuonCleanedCount
    process.main_path_mt_med *= process.analysisMuonsNoIsoMTCount
    process.main_path_mt_med *= process.analysisTausMuonCleanedMedium
    process.main_path_mt_med *= process.analysisTausMuonCleanedMediumCount
    process.main_path_mt_tht *= process.analysisMuonsNoIsoMTCount
    process.main_path_mt_tht *= process.analysisTausMuonCleanedTight
    process.main_path_mt_tht *= process.analysisTausMuonCleanedTightCount
    #process.main_path_ut *= process.analysisTausUnCleaned
    #process.main_path_ut *= process.analysisTausUnCleanedCount
    process.main_path_et *= process.analysisTausElectronCleaned
    process.main_path_et *= process.analysisTausElectronCleanedCount
    process.main_path_et_med *= process.analysisTausElectronCleanedMedium
    process.main_path_et_med *= process.analysisTausElectronCleanedMediumCount
    process.main_path_et_tht *= process.analysisTausElectronCleanedTight
    process.main_path_et_tht *= process.analysisTausElectronCleanedTightCount
    process.z_tau_eff_path *= process.analysisTaus
    process.z_tau_eff_path *= process.analysisTausCount
    process.z_tau_eff_muclean_path *= process.analysisTausMuonCleaned
    process.z_tau_eff_muclean_path *= process.analysisTausMuonCleanedCount

    # and for mt require third muon
    
    ############################
    ### Tau Eff requirements ###
    ############################
    process.mumuZTauEff = cms.EDProducer("CandViewShallowCloneCombiner",
        decay = cms.string("{0} {1}".format('slimmedMuons','analysisTaus')),
        checkCharge = cms.bool(False),
        cut   = cms.string("30<mass<210 && deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi)>0.5"),
    )
    process.mumuZCountTauEff = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('mumuZTauEff'),
    )
    process.mumuZMuonCleanedTauEff = cms.EDProducer("CandViewShallowCloneCombiner",
        decay = cms.string("{0} {1}".format('slimmedMuons','analysisTausMuonCleaned')),
        checkCharge = cms.bool(False),
        cut   = cms.string("30<mass<210 && deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi)>0.5"),
    )
    process.mumuZMuonCleanedCountTauEff = cms.EDFilter("PATCandViewCountFilter",
         minNumber = cms.uint32(1),
         maxNumber = cms.uint32(999),
         src = cms.InputTag('mumuZMuonCleanedTauEff'),
    )
    process.z_tau_eff_path *= process.mumuZTauEff
    process.z_tau_eff_path *= process.mumuZCountTauEff
    process.z_tau_eff_muclean_path *= process.mumuZMuonCleanedTauEff
    process.z_tau_eff_muclean_path *= process.mumuZMuonCleanedCountTauEff
    
    #################
    ### Finish up ###
    #################
    # add to schedule
    process.schedule.append(process.main_path)
    process.schedule.append(process.main_path_em)
    process.schedule.append(process.main_path_et)
    process.schedule.append(process.main_path_et_med)                                                              
    process.schedule.append(process.main_path_et_tht)
    process.schedule.append(process.main_path_mt)
    process.schedule.append(process.main_path_mt_med)
    process.schedule.append(process.main_path_mt_tht)
    #process.schedule.append(process.main_path_ut)
    process.schedule.append(process.main_path_tt)
    process.schedule.append(process.z_path)
    process.schedule.append(process.z_tau_eff_path)
    process.schedule.append(process.z_tau_eff_muclean_path)
    
    # lumi summary
    process.TFileService = cms.Service("TFileService",
        fileName = cms.string(options.outputFile.split('.root')[0]+'_lumi.root'),
    )
    
    process.lumiTree = cms.EDAnalyzer("LumiTree",
        genEventInfo = cms.InputTag("generator"),
        lheEventProduct = cms.InputTag("externalLHEProducer"),
    )
    process.lumi_step = cms.Path(process.lumiTree)
    process.schedule.append(process.lumi_step)
    
    process.lumiSummary = cms.EDProducer("LumiSummaryProducer",
        genEventInfo = cms.InputTag("generator"),
        lheEventProduct = cms.InputTag("externalLHEProducer"),
    )
    process.lumiSummary_step = cms.Path(process.lumiSummary)
    process.schedule.append(process.lumiSummary_step)
    
    
    # additional changes to standard MiniAOD content
    process.MINIAODoutput.outputCommands += [
        'keep *_slimmedTausElectronCleaned_*_*',
        'keep *_slimmedTausElectronCleanedMedium_*_*',
        'keep *_slimmedTausElectronCleanedTight_*_*',
        
        'keep *_slimmedTausMuonCleanedMedium_*_*',
        'keep *_slimmedTausMuonCleaned_*_*',
        'keep *_slimmedTausMuonCleanedTight_*_*',
       
        
        #'keep *_slimmedTausUnCleaned_*_*',
        'keep *_ak4PFJetsElectronCleaned_*_*',
        'keep *_ak4PFJetsElectronCleanedMedium_*_*',
        'keep *_ak4PFJetsElectronCleanedTight_*_*',
        
        'keep *_ak4PFJetsMuonCleaned_*_*',
        'keep *_ak4PFJetsMuonCleanedMedium_*_*',
        'keep *_ak4PFJetsMuonCleanedTight_*_*',
        'keep *_ak4PFJets_*_*',
        #'keep *_selectedPatTausMuonCleaned_*_*',
        #'keep *_selectedPatTausElectronCleaned_*_*',
        #'keep *_slimmedJetsMuonCleaned_*_*', # can't keep without warnings, can be recreated later anyway
        'keep *_lumiSummary_*_*',
    ]
    if not options.isMC:
        process.MINIAODoutput.outputCommands += [
            'drop *_ctppsLocalTrackLiteProducer_*_*', # Don't know what this is, but it prevents running in older releases
        ]

    process.MINIAODoutput.SelectEvents = cms.untracked.PSet(
        #SelectEvents = cms.vstring('main_path'),
        #SelectEvents = cms.vstring('main_path_em','main_path_et','main_path_mt','main_path_tt'),
        SelectEvents = cms.vstring('main_path_et','main_path_et_med','main_path_et_tht','main_path_mt','main_path_mt_med','main_path_mt_tht'),
        )
    
    # additional skims
    if doMM:
        process.MINIAODoutputZSKIM = process.MINIAODoutput.clone(
            SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('z_path'),
            ),
            fileName = cms.untracked.string(options.outputFile.split('.root')[0]+'_zskim.root'),
        )
        process.MINIAODoutputZSKIM_step = cms.EndPath(process.MINIAODoutputZSKIM)
        process.schedule.append(process.MINIAODoutputZSKIM_step)
    
    if doMT:
        process.MINIAODoutputZMUTAUSKIM = process.MINIAODoutput.clone(
            SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('z_tau_eff_path','z_tau_eff_muclean_path'),
            ),
            fileName = cms.untracked.string(options.outputFile.split('.root')[0]+'_zmutauskim.root'),
        )
        process.MINIAODoutputZMUTAUSKIM_step = cms.EndPath(process.MINIAODoutputZMUTAUSKIM)
        process.schedule.append(process.MINIAODoutputZMUTAUSKIM_step)
