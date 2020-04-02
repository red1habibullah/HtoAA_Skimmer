import FWCore.ParameterSet.Config as cms

def customizeTaus(process,coll,srcLabel='taus',postfix='',**kwargs):
    '''Customize taus'''
    reHLT = kwargs.pop('reHLT',False)
    isMC = kwargs.pop('isMC',False)
    tSrc = coll[srcLabel]
    rhoSrc = coll['rho']
    pvSrc = coll['vertices']
    genSrc = coll['genParticles']

    # customization path
    pathName = 'tauCustomization{0}'.format(postfix)
    setattr(process,pathName,cms.Path())
    path = getattr(process,pathName)

    #################
    ### embed rho ###
    #################
    module = cms.EDProducer(
        "TauRhoEmbedder",
        src = cms.InputTag(tSrc),
        rhoSrc = cms.InputTag(rhoSrc),
        label = cms.string("rho"),
    )
    modName = 'tRho{0}'.format(postfix)
    setattr(process,modName,module)
    tSrc = modName

    path *= getattr(process,modName)

    ################
    ### embed pv ###
    ################
    module = cms.EDProducer(
        "TauIpEmbedder",
        src = cms.InputTag(tSrc),
        vertexSrc = cms.InputTag(pvSrc),
        beamspotSrc = cms.InputTag("offlineBeamSpot"),
    )
    modName = 'tPV{0}'.format(postfix)
    setattr(process,modName,module)
    tSrc = modName

    path *= getattr(process,modName)

    ##############################
    ### embed trigger matching ###
    ##############################
    labels = []
    paths = []
    from triggers import triggerMap
    for trigger in triggerMap:
        if 'tau' in triggerMap[trigger]['objects']:
            labels += ['matches_{0}'.format(trigger)]
            paths += [triggerMap[trigger]['path']]
    module = cms.EDProducer(
        "TauHLTMatchEmbedder",
        src = cms.InputTag(tSrc),
        #triggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
        triggerResults = cms.InputTag('TriggerResults', '', 'HLT2') if reHLT else cms.InputTag('TriggerResults', '', 'HLT'),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        deltaR = cms.double(0.5),
        labels = cms.vstring(*labels),
        paths = cms.vstring(*paths),
    )
    modName = 'tTrig{0}'.format(postfix)
    setattr(process,modName,module)
    tSrc = modName

    path *= getattr(process,modName)

    ##########################
    ### embed tau gen jets ###
    ##########################
    if isMC:
        from PhysicsTools.JetMCAlgos.TauGenJets_cfi import tauGenJets
        process.tauGenJetsNew = tauGenJets.clone(GenParticles = cms.InputTag(genSrc))
        process.tauCustomization *= process.tauGenJetsNew

        module = cms.EDProducer(
            "TauGenJetEmbedder",
            src = cms.InputTag(tSrc),
            genJets = cms.InputTag("tauGenJetsNew"),
            excludeLeptons = cms.bool(True),
            deltaR = cms.double(0.5),
        )
        modName = 'tGenJetMatching{0}'.format(postfix)
        setattr(process,modName,module)
        tSrc = modName

        path *= getattr(process,modName)


    # add to schedule
    process.schedule.append(path)

    coll[srcLabel] = tSrc

    return coll
