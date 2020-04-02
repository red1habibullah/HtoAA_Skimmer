import FWCore.ParameterSet.Config as cms

def customizeMets(process,coll,**kwargs):
    '''Customize METs'''
    isMC = kwargs.pop('isMC',False)
    metSrc = coll['pfmet']
    jSrc = coll['jets']
    pSrc = coll['photons']
    eSrc = coll['electrons']
    mSrc = coll['muons']
    tSrc = coll['taus']
    pfSrc = coll['packed']
    pvSrc = coll['vertices']

    process.metCustomization = cms.Path()

    #################################
    ### recompute met uncertainty ###
    #################################
    # TODO: find recipe for 94X
    #print '... Preparing MET uncertainties'
    #postfix = "New"
    #from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
    #runMetCorAndUncFromMiniAOD(process,
    #                           jetCollUnskimmed="slimmedJets",
    #                           #jetCollUnskimmed=jSrc,
    #                           photonColl=pSrc,
    #                           electronColl=eSrc,
    #                           muonColl=mSrc,
    #                           tauColl=tSrc,
    #                           pfCandColl=pfSrc,
    #                           isData=not isMC,
    #                           #jetFlav="AK4PFchs",
    #                           postfix=postfix)

    ## this works now, but leave in case they break it again
    ### correct things to make it work
    ##del getattr(process,'slimmedMETs{0}'.format(postfix)).caloMET
    ##getattr(process,'shiftedPatJetEnUp{0}'.format(postfix)).src = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'shiftedPatJetEnDown{0}'.format(postfix)).src = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'shiftedPatMETCorrJetEnUp{0}'.format(postfix)).srcOriginal = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'shiftedPatMETCorrJetEnDown{0}'.format(postfix)).srcOriginal = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'shiftedPatMETCorrJetResUp{0}'.format(postfix)).srcOriginal = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'shiftedPatMETCorrJetResDown{0}'.format(postfix)).srcOriginal = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'pfCandsNoJets{0}'.format(postfix)).veto = cms.InputTag("cleanedPatJets{0}".format(postfix))
    ##getattr(process,'patPFMetTxyCorr{0}'.format(postfix)).srcPFlow = cms.InputTag(pfSrc)
    ##getattr(process,'patPFMetTxyCorr{0}'.format(postfix)).vertexCollection = cms.InputTag(pvSrc)

    ## switch to uncleaned jets
    #getattr(process,'shiftedPatJetEnUp{0}'.format(postfix)).src = cms.InputTag(jSrc)
    #getattr(process,'shiftedPatJetEnDown{0}'.format(postfix)).src = cms.InputTag(jSrc)

    #metSrc = "slimmedMETs{0}".format(postfix) # for now, use pre created mets
    ##metSrc = "slimmedMETs"
    #
    #####################
    #### embed shifts ###
    #####################
    #print '... Embedding shifts'
    #for shift in ['JetRes','JetEn','MuonEn','ElectronEn','TauEn','UnclusteredEn','PhotonEn']:
    #    for sign in ['Up','Down']:
    #        mod = cms.EDProducer(
    #            "ShiftedMETEmbedder",
    #            src = cms.InputTag(metSrc),
    #            label = cms.string('{0}{1}'.format(shift,sign)),
    #            shiftedSrc = cms.InputTag('patPFMetT1{0}{1}{2}'.format(shift,sign,postfix)),
    #        )
    #        modName = 'embed{0}{1}'.format(shift,sign)
    #        setattr(process,modName,mod)
    #        metSrc = modName
    #        process.metCustomization += getattr(process,modName)

    #for sign in ['Up','Down']:
    #    # electrons
    #    shift = 'ElectronEn'
    #    mod = cms.EDProducer(
    #        "ShiftedElectronEmbedder",
    #        src = cms.InputTag(eSrc),
    #        label = cms.string('{0}{1}'.format(shift,sign)),
    #        shiftedSrc = cms.InputTag('shiftedPat{0}{1}{2}'.format(shift,sign,postfix)),
    #    )
    #    modName = 'electronEmbed{0}{1}'.format(shift,sign)
    #    setattr(process,modName,mod)
    #    eSrc = modName
    #    process.metCustomization += getattr(process,modName)
    #    # muons
    #    shift = 'MuonEn'
    #    mod = cms.EDProducer(
    #        "ShiftedMuonEmbedder",
    #        src = cms.InputTag(mSrc),
    #        label = cms.string('{0}{1}'.format(shift,sign)),
    #        shiftedSrc = cms.InputTag('shiftedPat{0}{1}{2}'.format(shift,sign,postfix)),
    #    )
    #    modName = 'muonEmbed{0}{1}'.format(shift,sign)
    #    setattr(process,modName,mod)
    #    mSrc = modName
    #    process.metCustomization += getattr(process,modName)
    #    # taus
    #    shift = 'TauEn'
    #    mod = cms.EDProducer(
    #        "ShiftedTauEmbedder",
    #        src = cms.InputTag(tSrc),
    #        label = cms.string('{0}{1}'.format(shift,sign)),
    #        shiftedSrc = cms.InputTag('shiftedPat{0}{1}{2}'.format(shift,sign,postfix)),
    #    )
    #    modName = 'tauEmbed{0}{1}'.format(shift,sign)
    #    setattr(process,modName,mod)
    #    tSrc = modName
    #    process.metCustomization += getattr(process,modName)
    #    # jets
    #    shift = 'JetEn'
    #    mod = cms.EDProducer(
    #        "ShiftedJetEmbedder",
    #        src = cms.InputTag(jSrc),
    #        label = cms.string('{0}{1}'.format(shift,sign)),
    #        shiftedSrc = cms.InputTag('shiftedPat{0}{1}{2}'.format(shift,sign,postfix)),
    #    )
    #    modName = 'jetEmbed{0}{1}'.format(shift,sign)
    #    setattr(process,modName,mod)
    #    jSrc = modName
    #    process.metCustomization += getattr(process,modName)

    process.schedule.append(process.metCustomization)

    coll['pfmet'] = metSrc
    coll['muons'] = mSrc
    coll['electrons'] = eSrc
    coll['taus'] = tSrc
    coll['jets'] = jSrc
    coll['packed'] = pfSrc

    return coll
