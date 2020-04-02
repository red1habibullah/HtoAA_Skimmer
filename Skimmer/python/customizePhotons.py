import FWCore.ParameterSet.Config as cms

def customizePhotons(process,coll,srcLabel='photons',postfix='',**kwargs):
    '''Customize photons'''
    reHLT = kwargs.pop('reHLT',False)
    isREMINIAOD = kwargs.pop('isREMINIAOD',False)
    isMC = kwargs.pop('isMC',False)
    pSrc = coll[srcLabel]
    rhoSrc = coll['rho']

    # customization path
    pathName = 'photonCustomization{0}'.format(postfix)
    setattr(process,pathName,cms.Path())
    path = getattr(process,pathName)

    ###################################
    ### scale and smear corrections ###
    ###################################
    # embed the uncorrected stuff
    module = cms.EDProducer(
        "ShiftedPhotonEmbedder",
        src=cms.InputTag(pSrc),
        label=cms.string('uncorrected'),
        shiftedSrc=cms.InputTag('slimmedPhotons::{0}'.format('PAT' if isMC or isREMINIAOD else 'RECO')),
    )
    modName = 'uncorPho{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    # TODO: postfix doesnt work for photons
    # Not needed after reminiaod
    #process.load('EgammaAnalysis.ElectronTools.calibratedPatPhotonsRun2_cfi')
    #process.calibratedPatPhotons.photons = pSrc
    #process.calibratedPatPhotons.isMC = isMC
    #path *= process.calibratedPatPhotons
    #pSrc = 'calibratedPatPhotons'

    #######################
    ### embed Isolation ###
    #######################
    # missing?
    #process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")

    #path *= process.photonIDValueMapProducer

    #################
    ### embed VID ###
    #################
    from PhysicsTools.SelectorUtils.tools.vid_id_tools import switchOnVIDPhotonIdProducer, setupAllVIDIdsInModule, DataFormat, setupVIDPhotonSelection
    switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)
    
    # define which IDs we want to produce
    my_id_modules = [
        'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V1_cff',
        'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Fall17_94X_V1p1_cff',
    ]
    
    # add them to the VID producer
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

    # update the collection
    process.egmPhotonIDs.physicsObjectSrc = cms.InputTag(pSrc)
    # missing?
    #process.egmPhotonIsolation.srcToIsolate = cms.InputTag(pSrc)
    #process.photonIDValueMapProducer.srcMiniAOD = cms.InputTag(pSrc)
    #process.photonIDValueMapProducer.src = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles
    #process.photonIDValueMapProducer.pfCandidates = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles
    #process.photonIDValueMapProducer.vertices = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles
    #process.photonMVAValueMapProducer.srcMiniAOD = cms.InputTag(pSrc)
    #process.photonMVAValueMapProducer.src = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles
    #process.photonRegressionValueMapProducer.srcMiniAOD = cms.InputTag(pSrc)
    #process.photonRegressionValueMapProducer.src = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles

    idDecisionLabels = [
        'cutBasedPhotonID-Fall17-94X-V1-loose',
        'cutBasedPhotonID-Fall17-94X-V1-medium',
        'cutBasedPhotonID-Fall17-94X-V1-tight',
        'mvaPhoID-RunIIFall17-v1p1-wp80', # EGM recommends not to use
        'mvaPhoID-RunIIFall17-v1p1-wp90',
    ]
    idDecisionTags = [
        cms.InputTag('egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V1-loose'),
        cms.InputTag('egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V1-medium'),
        cms.InputTag('egmPhotonIDs:cutBasedPhotonID-Fall17-94X-V1-tight'),
        cms.InputTag('egmPhotonIDs:mvaPhoID-RunIIFall17-v1p1-wp80'), # EGM recommends not to use
        cms.InputTag('egmPhotonIDs:mvaPhoID-RunIIFall17-v1p1-wp90'),
    ]
    fullIDDecisionLabels = [
    ]
    fullIDDecisionTags = [
    ]
    nMinusOneIDNames = [
    ]
    nMinusOneIDLabels = [
    ]
    mvaValueLabels = [
        'PhotonMVAEstimatorRunIIFall17v1p1Values',
        'gammaDR030',
        "phoWorstChargedIsolationWithConeVeto",
        "phoESEffSigmaRR",
        "phoFull5x5E1x3",
        "phoFull5x5E2x2",
        "phoFull5x5E2x5Max",
        "phoFull5x5E5x5",
        "phoFull5x5SigmaIEtaIEta",
        "phoFull5x5SigmaIEtaIPhi",
        "phoChargedIsolation",
        "phoNeutralHadronIsolation",
        "phoPhotonIsolation",
    ]
    mvaValueTags = [
        cms.InputTag('photonMVAValueMapProducer:PhotonMVAEstimatorRunIIFall17v1p1Values'),
        cms.InputTag("egmPhotonIsolation","gamma-DR030-"),
        cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolationWithConeVeto"),
        cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
        cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
        cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
        cms.InputTag("photonIDValueMapProducer","phoNeutralHadronIsolation"),
        cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
    ]
    mvaCategoryLabels = [
        'PhotonMVAEstimatorRunIIFall17v1p1Categories',
    ]
    mvaCategoryTags = [
        cms.InputTag('photonMVAValueMapProducer:PhotonMVAEstimatorRunIIFall17v1p1Categories'),
    ]

    module = cms.EDProducer(
        "PhotonVIDEmbedder",
        src=cms.InputTag(pSrc),
        idLabels = cms.vstring(*idDecisionLabels),          # labels for bool maps
        ids = cms.VInputTag(*idDecisionTags),               # bool maps
        fullIDLabels = cms.vstring(*fullIDDecisionLabels),  # labels for bool maps for n-1
        fullIDs = cms.VInputTag(*fullIDDecisionTags),       # bool maps for n-1
        nMinusOneIDNames = cms.vstring(*nMinusOneIDNames),  # n-1 cut names
        nMinusOneIDLabels = cms.vstring(*nMinusOneIDLabels),# n-1 cut labels
        valueLabels = cms.vstring(*mvaValueLabels),         # labels for float maps
        values = cms.VInputTag(*mvaValueTags),              # float maps
        categoryLabels = cms.vstring(*mvaCategoryLabels),   # labels for int maps
        categories = cms.VInputTag(*mvaCategoryTags),       # int maps
    )
    modName = 'pidEmbedder{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    process.photonCustomization *= process.egmPhotonIDSequence
    path *= getattr(process,modName)

    #################
    ### embed rho ###
    #################
    module = cms.EDProducer(
        "PhotonRhoEmbedder",
        src = cms.InputTag(pSrc),
        rhoSrc = cms.InputTag(rhoSrc),
        label = cms.string("rho"),
    )
    modName = 'pRho{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    #############################
    ### embed effective areas ###
    #############################
    eaChargedHadronsFile = 'RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt'
    eaNeutralHadronsFile = 'RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt'
    #eaPhotonsFile = 'RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased_3bins.txt'
    eaPhotonsFile = 'RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'

    module = cms.EDProducer(
        "PhotonEffectiveAreaEmbedder",
        src = cms.InputTag(pSrc),
        label = cms.string("EffectiveAreaChargedHadrons"), # embeds a user float with this name
        configFile = cms.FileInPath(eaChargedHadronsFile), # the effective areas file
    )
    modName = 'pChargedHadronsEffArea{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    module = cms.EDProducer(
        "PhotonEffectiveAreaEmbedder",
        src = cms.InputTag(pSrc),
        label = cms.string("EffectiveAreaNeutralHadrons"), # embeds a user float with this name
        configFile = cms.FileInPath(eaNeutralHadronsFile), # the effective areas file
    )
    modName = 'pNeutralHadronsEffArea{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    module = cms.EDProducer(
        "PhotonEffectiveAreaEmbedder",
        src = cms.InputTag(pSrc),
        label = cms.string("EffectiveAreaPhotons"), # embeds a user float with this name
        configFile = cms.FileInPath(eaPhotonsFile), # the effective areas file
    )
    modName = 'pPhotonsEffArea{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    ##############################
    ### embed trigger matching ###
    ##############################
    labels = []
    paths = []
    from triggers import triggerMap
    for trigger in triggerMap:
        if 'photon' in triggerMap[trigger]['objects']:
            labels += ['matches_{0}'.format(trigger)]
            paths += [triggerMap[trigger]['path']]
    module = cms.EDProducer(
        "PhotonHLTMatchEmbedder",
        src = cms.InputTag(pSrc),
        #triggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
        triggerResults = cms.InputTag('TriggerResults', '', 'HLT2') if reHLT else cms.InputTag('TriggerResults', '', 'HLT'),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        deltaR = cms.double(0.5),
        labels = cms.vstring(*labels),
        paths = cms.vstring(*paths),
    )
    modName = 'pTrig{0}'.format(postfix)
    setattr(process,modName,module)
    pSrc = modName

    path *= getattr(process,modName)

    # add to schedule
    process.schedule.append(path)

    coll[srcLabel] = pSrc

    return coll
