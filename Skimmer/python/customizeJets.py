import FWCore.ParameterSet.Config as cms

def customizeJets(process,coll,srcLabel='jets',postfix='',**kwargs):
    '''Customize jets'''
    isMC = kwargs.pop('isMC',False)
    reHLT = kwargs.pop('reHLT',False)
    jSrc = coll[srcLabel]
    rhoSrc = coll['rho']
    pvSrc = coll['vertices']

    # customization path
    pathName = 'jetCustomization{0}'.format(postfix)
    setattr(process,pathName,cms.Path())
    path = getattr(process,pathName)

    #################################
    ### add updated pileup jet id ###
    #################################
    # TODO: why is this here?
    #process.load("RecoJets.JetProducers.PileupJetID_cfi")
    #module = process.pileupJetId.clone(
    #    jets=cms.InputTag(jSrc),
    #    inputIsCorrected=True,
    #    applyJec=True,
    #    vertexes=cms.InputTag(pvSrc),
    #)
    #modName = 'pileupJetIdUpdated{0}'.format(postfix)
    #setattr(process,modName,module)

    #path *= getattr(process,modName)

    ######################
    ### recorrect jets ###
    ######################
    # TODO: reenable when we have a new recipe
    #from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

    #jetCorr = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')
    #if isMC:
    #    jetCorr = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None')
    #updateJetCollection(
    #    process,
    #    jetSource = cms.InputTag(jSrc),
    #    jetCorrections = jetCorr,
    #    postfix=postfix,
    #)
    #modName = 'updatedPatJets{0}'.format(postfix)
    #getattr(process,modName).userData.userFloats.src += ['pileupJetIdUpdated{0}:fullDiscriminant'.format(postfix)]
    #jSrc = modName

    #################
    ### embed ids ###
    #################
    module = cms.EDProducer('DeepDiTauProducer',
        src = cms.InputTag(jSrc),
        DeepDiTauConfiguration = cms.PSet(
            memmapped = cms.bool(False),
            graphDefinitions = cms.VPSet(
                cms.PSet(
                    name = cms.string('ditau2017v1'),
                    path = cms.FileInPath('DevTools/Ntuplizer/data/ditau_2017_v1.pb'),
                    means = cms.FileInPath('DevTools/Ntuplizer/data/ditau_2017_v1_means_sigmas.txt'),
                ),
                cms.PSet(
                    name = cms.string('ditau2017MDv1'),
                    path = cms.FileInPath('DevTools/Ntuplizer/data/ditau_2017_md_v1.pb'),
                    means = cms.FileInPath('DevTools/Ntuplizer/data/ditau_2017_md_v1_means_sigmas.txt'),
                ),
            ),
        ),
    )
    modName = 'deepDiTau{0}'.format(postfix)
    setattr(process,modName,module)
    path *= getattr(process,modName)

    module = cms.EDProducer(
        "JetIdEmbedder",
        src = cms.InputTag(jSrc),
        discriminator = cms.string('pileupJetId:fullDiscriminant'),
        ditau2017v1 = cms.InputTag("deepDiTau"+postfix,"ditau2017v1"),
        ditau2017MDv1 = cms.InputTag("deepDiTau"+postfix,"ditau2017MDv1"),
    )
    modName = 'jID{0}'.format(postfix)
    setattr(process,modName,module)
    jSrc = modName

    path *= getattr(process,modName)

    ###################
    ### embed truth ###
    ###################
    module = cms.EDProducer(
        "JetMCTruthEmbedder",
        src = cms.InputTag(jSrc),
        genSrc = cms.InputTag('prunedGenParticles'),
        packedGenSrc = cms.InputTag('packedGenParticles'),
    )
    modName = 'jTruth{0}'.format(postfix)
    setattr(process,modName,module)
    jSrc = modName

    path *= getattr(process,modName)

    #################
    ### embed rho ###
    #################
    module = cms.EDProducer(
        "JetRhoEmbedder",
        src = cms.InputTag(jSrc),
        rhoSrc = cms.InputTag(rhoSrc),
        label = cms.string("rho"),
    )
    modName = 'jRho{0}'.format(postfix)
    setattr(process,modName,module)
    jSrc = modName

    path *= getattr(process,modName)

    ##########################
    ### embed jet gen jets ###
    ##########################
    if isMC:
        module = cms.EDProducer(
            "JetGenJetEmbedder",
            src = cms.InputTag(jSrc),
            genJets = cms.InputTag("slimmedGenJets"),
            excludeLeptons = cms.bool(False),
            deltaR = cms.double(0.5),
        )
        modName = 'jGenJetMatching{0}'.format(postfix)
        setattr(process,modName,module)
        jSrc = modName

        path *= getattr(process,modName)

    ##############################
    ### embed trigger matching ###
    ##############################
    labels = []
    paths = []
    from triggers import triggerMap
    for trigger in triggerMap:
        if 'jet' in triggerMap[trigger]['objects']:
            labels += ['matches_{0}'.format(trigger)]
            paths += [triggerMap[trigger]['path']]
    module = cms.EDProducer(
        "JetHLTMatchEmbedder",
        src = cms.InputTag(jSrc),
        #triggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
        triggerResults = cms.InputTag('TriggerResults', '', 'HLT2') if reHLT else cms.InputTag('TriggerResults', '', 'HLT'),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        deltaR = cms.double(0.4),
        labels = cms.vstring(*labels),
        paths = cms.vstring(*paths),
    )
    modName = 'jTrig{0}'.format(postfix)
    setattr(process,modName,module)
    jSrc = modName

    path *= getattr(process,modName)

    # add to schedule
    process.schedule.append(path)

    coll[srcLabel] = jSrc

    return coll
