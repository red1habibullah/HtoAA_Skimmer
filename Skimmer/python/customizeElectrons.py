import FWCore.ParameterSet.Config as cms

def customizeElectrons(process,coll,srcLabel='electrons',postfix='',**kwargs):
    '''Customize electrons'''
    reHLT = kwargs.pop('reHLT',False)
    isREMINIAOD = kwargs.pop('isREMINIAOD',False)
    isMC = kwargs.pop('isMC',False)
    eSrc = coll[srcLabel]
    pSrc = coll['photons']
    jSrc = coll['jets']
    rhoSrc = coll['rho']
    pvSrc = coll['vertices']
    pfSrc = coll['packed']

    # customization path
    pathName = 'electronCustomization{0}'.format(postfix)
    setattr(process,pathName,cms.Path())
    path = getattr(process,pathName)


    #######################
    ### ECAL Regression ###
    #######################
    # when using reminiaod, dont need: data 31Mar2018, MC: 12Apr2018
    #from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
    #setupEgammaPostRecoSeq(process,applyEnergyCorrections=False,
    #                       applyVIDOnCorrectedEgamma=False,
    #                       isMiniAOD=True,
    #                       era='2017-Nov17ReReco') # eras: 2017-Nov17ReReco, 2016-Legacy, 2016-Feb17ReMiniAOD. Will need to add option to configure
    #path *= process.egammaPostRecoSeq

    # Previous recipe
    # TODO: verify no longer needed
    #from EgammaAnalysis.ElectronTools.regressionWeights_cfi import regressionWeights
    #process = regressionWeights(process)

    ## note: also brings in photons, customize in customizePhotons.py
    #process.load('EgammaAnalysis.ElectronTools.regressionApplication_cff')
    #process.electronCustomization *= process.regressionApplication

    # embed the uncorrected stuff
    module = cms.EDProducer(
        "ShiftedElectronEmbedder",
        src=cms.InputTag(eSrc),
        label=cms.string('uncorrected'),
        shiftedSrc=cms.InputTag('slimmedElectrons::{0}'.format('PAT' if isMC or isREMINIAOD else 'RECO')),
    )
    modName = 'uncorElec{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    ###################################
    ### scale and smear corrections ###
    ###################################

    # Not needed after reminiaod
    # first need to add a manual protection for the corrections
    #module = cms.EDFilter(
    #    "PATElectronSelector",
    #    src = cms.InputTag(eSrc),
    #    cut = cms.string("pt > 5 && abs(eta)<2.5")
    #)
    #modName = 'selectedElectrons{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    ## TODO: Note, postfix doesn't work on electrons yet
    #process.load('EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi')
    #process.calibratedPatElectrons.electrons = eSrc
    #process.calibratedPatElectrons.isMC = isMC
    #process.electronCustomization *= process.calibratedPatElectrons
    #eSrc = 'calibratedPatElectrons'

    #################
    ### embed VID ###
    #################
    from PhysicsTools.SelectorUtils.tools.vid_id_tools import switchOnVIDElectronIdProducer, setupAllVIDIdsInModule, DataFormat, setupVIDElectronSelection
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
    
    # define which IDs we want to produce
    my_id_modules = [
        'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V1_cff',
        'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff',
        'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff',
        #'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff',
    ]
    
    # add them to the VID producer
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

    # update the collection
    process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag(eSrc)
    process.electronMVAValueMapProducer.srcMiniAOD = cms.InputTag(eSrc)
    process.electronMVAValueMapProducer.src = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles
    # missing? 
    #process.electronRegressionValueMapProducer.srcMiniAOD = cms.InputTag(eSrc)
    #process.electronRegressionValueMapProducer.src = cms.InputTag("") # disable AOD in case we are running with secondaryInputFiles

    idDecisionLabels = [
        'cutBasedElectronID-Fall17-94X-V1-veto',
        'cutBasedElectronID-Fall17-94X-V1-loose',
        'cutBasedElectronID-Fall17-94X-V1-medium',
        'cutBasedElectronID-Fall17-94X-V1-tight',
        'mvaEleID-Fall17-noIso-V1-wp90',
        'mvaEleID-Fall17-noIso-V1-wp80',
        'mvaEleID-Fall17-noIso-V1-wpLoose',
        'mvaEleID-Fall17-iso-V1-wp90',
        'mvaEleID-Fall17-iso-V1-wp80',
        'mvaEleID-Fall17-iso-V1-wpLoose',
        #'heepElectronID-HEEPV70',
    ]
    idDecisionTags = [
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-veto'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-loose'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-medium'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-tight'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-noIso-V1-wp90'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-noIso-V1-wp80'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-noIso-V1-wpLoose'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-iso-V1-wp90'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-iso-V1-wp80'),
        cms.InputTag('egmGsfElectronIDs:mvaEleID-Fall17-iso-V1-wpLoose'),
        #cms.InputTag('egmGsfElectronIDs:heepElectronID-HEEPV70'),
    ]
    fullIDDecisionLabels = [
        'cutBasedElectronID-Fall17-94X-V1-veto',
        'cutBasedElectronID-Fall17-94X-V1-loose',
        'cutBasedElectronID-Fall17-94X-V1-medium',
        'cutBasedElectronID-Fall17-94X-V1-tight',
    ]
    fullIDDecisionTags = [
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-veto'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-loose'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-medium'),
        cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-tight'),
    ]
    nMinusOneIDNames = [
        'GsfEleEffAreaPFIsoCut_0',
    ]
    nMinusOneIDLabels = [
        'NoIso',
    ]
    mvaValueLabels = [
        'ElectronMVAEstimatorRun2Fall17IsoV1Values',
        'ElectronMVAEstimatorRun2Fall17NoIsoV1Values',
    ]
    mvaValueTags = [
        cms.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV1Values'),
        cms.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV1Values'),
    ]
    mvaCategoryLabels = [
        'ElectronMVAEstimatorRun2Fall17IsoV1Categories',
        'ElectronMVAEstimatorRun2Fall17NoIsoV1Categories',
    ]
    mvaCategoryTags = [
        cms.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV1Categories'),
        cms.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV1Categories'),
    ]

    module = cms.EDProducer(
        "ElectronVIDEmbedder",
        src=cms.InputTag(eSrc),
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
    modName = 'eidEmbedder{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= process.egmGsfElectronIDSequence
    path *= getattr(process,modName)



    #########################
    ### embed nearest jet ###
    #########################
    # TODO reenable
    #module = cms.EDProducer(
    #    "ElectronJetEmbedder",
    #    src = cms.InputTag(eSrc),
    #    jetSrc = cms.InputTag(jSrc),
    #    dRmax = cms.double(0.4),
    #    L1Corrector = cms.InputTag("ak4PFCHSL1FastjetCorrector"),
    #    L1L2L3ResCorrector= cms.InputTag("ak4PFCHSL1FastL2L3Corrector"),
    #)
    #modName = 'eJet{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    ##########################
    ### embed missing hits ###
    ##########################
    module = cms.EDProducer(
        "ElectronMissingHitsEmbedder",
        src = cms.InputTag(eSrc),
    )
    modName = 'eMissingHits{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    ###################
    ### embed ww id ###
    ###################
    # TODO: enable if needed
    #module = cms.EDProducer(
    #    "ElectronWWIdEmbedder",
    #    src = cms.InputTag(eSrc),
    #    vertexSrc = cms.InputTag(pvSrc),
    #)
    #modName = 'eWW{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    #############################
    ### embed effective areas ###
    #############################
    eaFile = 'RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt'
    module = cms.EDProducer(
        "ElectronEffectiveAreaEmbedder",
        src = cms.InputTag(eSrc),
        label = cms.string("EffectiveArea"), # embeds a user float with this name
        configFile = cms.FileInPath(eaFile), # the effective areas file
    )
    modName = 'eEffArea{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    #################
    ### embed rho ###
    #################
    module = cms.EDProducer(
        "ElectronRhoEmbedder",
        src = cms.InputTag(eSrc),
        rhoSrc = cms.InputTag(rhoSrc),
        label = cms.string("rho"),
    )
    modName = 'eRho{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    ################
    ### embed pv ###
    ################
    module = cms.EDProducer(
        "ElectronIpEmbedder",
        src = cms.InputTag(eSrc),
        vertexSrc = cms.InputTag(pvSrc),
        beamspotSrc = cms.InputTag("offlineBeamSpot"),
    )
    modName = 'ePV{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    ##############################
    ### embed trigger matching ###
    ##############################
    labels = []
    paths = []
    from triggers import triggerMap
    for trigger in triggerMap:
        if 'electron' in triggerMap[trigger]['objects']:
            labels += ['matches_{0}'.format(trigger)]
            paths += [triggerMap[trigger]['path']]
    module = cms.EDProducer(
        "ElectronHLTMatchEmbedder",
        src = cms.InputTag(eSrc),
        #triggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
        triggerResults = cms.InputTag('TriggerResults', '', 'HLT2') if reHLT else cms.InputTag('TriggerResults', '', 'HLT'),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        deltaR = cms.double(0.5),
        labels = cms.vstring(*labels),
        paths = cms.vstring(*paths),
    )
    modName = 'eTrig{0}'.format(postfix)
    setattr(process,modName,module)
    eSrc = modName

    path *= getattr(process,modName)

    # TODO: update if needed
    ######################
    #### embed HZZ IDs ###
    ######################
    ## https://github.com/nwoods/UWVV/blob/ichep/AnalysisTools/python/templates/ZZID.py
    ## https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsZZ4l2016
    #module = cms.EDProducer(
    #    "PATElectronZZIDEmbedder",
    #    src = cms.InputTag(eSrc),
    #    vtxSrc = cms.InputTag(pvSrc),
    #    bdtLabel = cms.string('ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values'),
    #    idCutLowPtLowEta = cms.double(-.211),
    #    idCutLowPtMedEta = cms.double(-.396),
    #    idCutLowPtHighEta = cms.double(-.215),
    #    idCutHighPtLowEta = cms.double(-.870),
    #    idCutHighPtMedEta = cms.double(-.838),
    #    idCutHighPtHighEta = cms.double(-.763),
    #    missingHitsCut = cms.int32(999),
    #    ptCut = cms.double(7.), 
    #)
    #modName = 'eHZZEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    #######################
    #### embed SUSY IDs ###
    #######################
    ## https://twiki.cern.ch/twiki/bin/view/CMS/LeptonMVA
    #module = cms.EDProducer(
    #    "ElectronMiniIsolationEmbedder",
    #    src = cms.InputTag(eSrc),
    #    packedSrc = cms.InputTag(pfSrc),
    #)
    #modName = 'eMiniIsoEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    #module = cms.EDProducer(
    #    "ElectronSUSYMVAEmbedder",
    #    src = cms.InputTag(eSrc),
    #    vertexSrc = cms.InputTag(pvSrc),
    #    rhoSrc = cms.InputTag('fixedGridRhoFastjetCentralNeutral'),
    #    mva = cms.string('ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values'),
    #    weights = cms.FileInPath('DevTools/Ntuplizer/data/susy_el_BDTG.weights.xml'), # https://github.com/CERN-PH-CMG/cmgtools-lite/blob/80X/TTHAnalysis/data/leptonMVA/tth
    #)
    #modName = 'eSUSYEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #eSrc = modName

    #path *= getattr(process,modName)

    # add to schedule
    process.schedule.append(path)

    coll[srcLabel] = eSrc

    return coll
