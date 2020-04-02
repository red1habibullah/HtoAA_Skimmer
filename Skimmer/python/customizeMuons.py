import FWCore.ParameterSet.Config as cms

def customizeMuons(process,coll,srcLabel='muons',postfix='',**kwargs):
    '''Customize muons'''
    reHLT = kwargs.pop('reHLT',False)
    isMC = kwargs.pop('isMC',False)
    mSrc = coll[srcLabel]
    jSrc = coll['jets']
    rhoSrc = coll['rho']
    pvSrc = coll['vertices']
    pfSrc = coll['packed']

    # customization path
    pathName = 'muonCustomization{0}'.format(postfix)
    setattr(process,pathName,cms.Path())
    path = getattr(process,pathName)

    #########################
    ### embed nearest jet ###
    #########################
    # TODO: reenable
    #module = cms.EDProducer(
    #    "MuonJetEmbedder",
    #    src = cms.InputTag(mSrc),
    #    jetSrc = cms.InputTag(jSrc),
    #    dRmax = cms.double(0.4),
    #    L1Corrector = cms.InputTag("ak4PFCHSL1FastjetCorrector"),
    #    L1L2L3ResCorrector= cms.InputTag("ak4PFCHSL1FastL2L3Corrector"),
    #)
    #modName = 'mJet{0}'.format(postfix)
    #setattr(process,modName,module)
    #mSrc = modName

    #path *= getattr(process,modName)

    ###################################
    ### embed rochester corrections ###
    ###################################
    # TODO: reenable with 2017
    #module = cms.EDProducer(
    #    "RochesterCorrectionEmbedder",
    #    src = cms.InputTag(mSrc),
    #    isData = cms.bool(not isMC),
    #    directory = cms.FileInPath("DevTools/Ntuplizer/data/rcdata.2016.v3/config.txt"),
    #)
    #modName = 'mRoch{0}'.format(postfix)
    #setattr(process,modName,module)
    #mSrc = modName

    #path *= getattr(process,modName)

    #####################
    ### embed muon id ###
    #####################
    module = cms.EDProducer(
        "MuonIdEmbedder",
        src = cms.InputTag(mSrc),
        vertexSrc = cms.InputTag(pvSrc),
    )
    modName = 'mID{0}'.format(postfix)
    setattr(process,modName,module)
    mSrc = modName

    path *= getattr(process,modName)

    #################
    ### embed rho ###
    #################
    module = cms.EDProducer(
        "MuonRhoEmbedder",
        src = cms.InputTag(mSrc),
        rhoSrc = cms.InputTag(rhoSrc),
        label = cms.string("rho"),
    )
    modName = 'mRho{0}'.format(postfix)
    setattr(process,modName,module)
    mSrc = modName

    path *= getattr(process,modName)

    ################
    ### embed pv ###
    ################
    module = cms.EDProducer(
        "MuonIpEmbedder",
        src = cms.InputTag(mSrc),
        vertexSrc = cms.InputTag(pvSrc),
        beamspotSrc = cms.InputTag("offlineBeamSpot"),
    )
    modName = 'mPV{0}'.format(postfix)
    setattr(process,modName,module)
    mSrc = modName

    path *= getattr(process,modName)

    ##############################
    ### embed trigger matching ###
    ##############################
    labels = []
    paths = []
    from triggers import triggerMap
    for trigger in triggerMap:
        if 'muon' in triggerMap[trigger]['objects']:
            labels += ['matches_{0}'.format(trigger)]
            paths += [triggerMap[trigger]['path']]
    module = cms.EDProducer(
        "MuonHLTMatchEmbedder",
        src = cms.InputTag(mSrc),
        #triggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
        triggerResults = cms.InputTag('TriggerResults', '', 'HLT2') if reHLT else cms.InputTag('TriggerResults', '', 'HLT'),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        deltaR = cms.double(0.5),
        labels = cms.vstring(*labels),
        paths = cms.vstring(*paths),
    )
    modName = 'mTrig{0}'.format(postfix)
    setattr(process,modName,module)
    mSrc = modName

    path *= getattr(process,modName)

    # TODO: update if needed
    ######################
    #### embed HZZ IDs ###
    ######################
    ## https://github.com/nwoods/UWVV/blob/ichep/AnalysisTools/python/templates/ZZID.py
    ## https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsZZ4l2016
    #module = cms.EDProducer(
    #    "PATMuonZZIDEmbedder",
    #    src = cms.InputTag(mSrc),
    #    vtxSrc = cms.InputTag(pvSrc),
    #    ptCut = cms.double(5.),
    #)
    #modName = 'mHZZEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #mSrc = modName

    #path *= getattr(process,modName)

    #######################
    #### embed SUSY IDs ###
    #######################
    ## https://twiki.cern.ch/twiki/bin/view/CMS/LeptonMVA
    #module = cms.EDProducer(
    #    "MuonMiniIsolationEmbedder",
    #    src = cms.InputTag(mSrc),
    #    packedSrc = cms.InputTag(pfSrc),
    #)
    #modName = 'mMiniIsoEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #mSrc = modName

    #path *= getattr(process,modName)

    #module = cms.EDProducer(
    #    "MuonSUSYMVAEmbedder",
    #    src = cms.InputTag(mSrc),
    #    vertexSrc = cms.InputTag(pvSrc),
    #    rhoSrc = cms.InputTag('fixedGridRhoFastjetCentralNeutral'),
    #    weights = cms.FileInPath('DevTools/Ntuplizer/data/susy_mu_BDTG.weights.xml'), # https://github.com/CERN-PH-CMG/cmgtools-lite/blob/80X/TTHAnalysis/data/leptonMVA/tth
    #)
    #modName = 'mSUSYEmbedder{0}'.format(postfix)
    #setattr(process,modName,module)
    #mSrc = modName

    #path *= getattr(process,modName)

    # add to schedule
    process.schedule.append(path)

    coll[srcLabel] = mSrc

    return coll
