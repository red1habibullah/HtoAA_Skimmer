import FWCore.ParameterSet.Config as cms

from triggers import triggerMap

commonCandidates = cms.PSet(
    pt     = cms.vstring('pt()','F'),
    eta    = cms.vstring('eta()','F'),
    phi    = cms.vstring('phi()','F'),
    energy = cms.vstring('energy()','F'),
    charge = cms.vstring('charge()','F'),
    mass   = cms.vstring('mass()','F'),
    pdgId  = cms.vstring('pdgId()','I'),
)

constituentBranches = commonCandidates.clone(
    px                      = cms.vstring('px','F'),
    py                      = cms.vstring('py','F'),
    pz                      = cms.vstring('pz','F'),
    ptTrk                   = cms.vstring('ptTrk','F'),
    etaAtVtx                = cms.vstring('etaAtVtx','F'),
    phiAtVtx                = cms.vstring('phiAtVtx','F'),
    dxy                     = cms.vstring('dxy','F'),
    dxyError                = cms.vstring('? hasTrackDetails ? dxyError : -1','F'),
    dz                      = cms.vstring('dz','F'),
    dzError                 = cms.vstring('? hasTrackDetails ? dzError : -1','F'),
    hcalFration             = cms.vstring('hcalFraction','F'),
    caloFration             = cms.vstring('caloFraction','F'),
    isIsolatedChargedHadron = cms.vstring('isIsolatedChargedHadron','I'),
    isConvertedPhoton       = cms.vstring('isConvertedPhoton','I'),
    isPhoton                = cms.vstring('isPhoton','I'),
    isElectron              = cms.vstring('isElectron','I'),
    isMuon                  = cms.vstring('isMuon','I'),
    isGlobalMuon            = cms.vstring('isGlobalMuon','I'),
    isStandAloneMuon        = cms.vstring('isStandAloneMuon','I'),
    isTrackerMuon           = cms.vstring('isTrackerMuon','I'),
    numberOfHits            = cms.vstring('numberOfHits','I'),
    numberOfPixelHits       = cms.vstring('numberOfPixelHits','I'),
    pixelLayersWithMeasurements   = cms.vstring('pixelLayersWithMeasurement','I'),
    stripLayersWithMeasurements   = cms.vstring('stripLayersWithMeasurement','I'),
    trackerLayersWithMeasurements = cms.vstring('trackerLayersWithMeasurement','I'),
    puppiWeight             = cms.vstring('puppiWeight','F'),
    puppiWeightNoLep        = cms.vstring('puppiWeightNoLep','F'),
    trackHighPurity         = cms.vstring('trackHighPurity','I'),
    vertexNdof              = cms.vstring('vertexNdof','I'),
    vertexNormalizedChi2    = cms.vstring('vertexNormalizedChi2','F'),
    vx                      = cms.vstring('vx','F'),
    vy                      = cms.vstring('vy','F'),
    vz                      = cms.vstring('vz','F'),
)

commonGenCandidates = commonCandidates.clone(
    status                 = cms.vstring('status()','I'),
    numberOfDaughters      = cms.vstring('numberOfDaughters()','I'),
    daughter_1             = cms.vstring('? numberOfDaughters()>0 ? daughter(0).pdgId() : 0','I'),
    daughter_2             = cms.vstring('? numberOfDaughters()>1 ? daughter(1).pdgId() : 0','I'),
    numberOfMothers        = cms.vstring('numberOfMothers()','I'),
    mother_1               = cms.vstring('? numberOfMothers()>0 ? mother(0).pdgId() : 0','I'),
    mother_2               = cms.vstring('? numberOfMothers()>1 ? mother(1).pdgId() : 0','I'),
    isPrompt               = cms.vstring('isPromptFinalState()','I'),
    isFromTau              = cms.vstring('isDirectPromptTauDecayProductFinalState()','I'),
    isPromptDecayed        = cms.vstring('isPromptDecayed()','I'),
    isFromHadron           = cms.vstring('statusFlags().isDirectHadronDecayProduct()','I'),
    fromHardProcess        = cms.vstring('fromHardProcessFinalState()','I'),
    fromHardProcessDecayed = cms.vstring('fromHardProcessDecayed()','I'),
    fromHardProcessTau     = cms.vstring('isDirectHardProcessTauDecayProductFinalState()','I'),
)

commonPatCandidates = commonCandidates.clone(
    genMatch                  = cms.vstring('genParticleRef.isNonnull()','I'),
    genPdgId                  = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().pdgId() : 0', 'I'),
    genPt                     = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().pt() : 0', 'F'),
    genEta                    = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().eta() : 0', 'F'),
    genPhi                    = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().phi() : 0', 'F'),
    genMass                   = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().mass() : 0', 'F'),
    genEnergy                 = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().energy() : 0', 'F'),
    genCharge                 = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().charge() : 0', 'F'),
    genVZ                     = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().vz() : 0', 'F'),
    genStatus                 = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().status() : 0', 'I'),
    genIsPrompt               = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().isPromptFinalState() : 0', 'I'),
    genIsFromTau              = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().isDirectPromptTauDecayProductFinalState() : 0', 'I'),
    genIsPromptDecayed        = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().isPromptDecayed() : 0', 'I'),
    genIsFromHadron           = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().statusFlags().isDirectHadronDecayProduct() : 0', 'I'),
    genFromHardProcess        = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().fromHardProcessFinalState() : 0', 'I'),
    genFromHardProcessDecayed = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().fromHardProcessDecayed() : 0', 'I'),
    genFromHardProcessTau     = cms.vstring('? (genParticleRef.isNonnull() ) ? genParticleRef().isDirectHardProcessTauDecayProductFinalState() : 0', 'I'),
)

commonJetCandidates = commonPatCandidates.clone(
    genJetMatch               = cms.vstring('userCand("genJet").isNonnull()','I'),
    genJetPdgId               = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").pdgId() : 0', 'I'),
    genJetPt                  = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").pt() : 0', 'F'),
    genJetEta                 = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").eta() : 0', 'F'),
    genJetPhi                 = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").phi() : 0', 'F'),
    genJetMass                = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").mass() : 0', 'F'),
    genJetEnergy              = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").energy() : 0', 'F'),
    genJetCharge              = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").charge() : 0', 'F'),
    genJetVZ                  = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").vz() : 0', 'F'),
    genJetStatus              = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").status() : 0', 'I'),
    genJetEMEnergy            = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").emEnergy() : 0', 'F'),
    genJetHadEnergy           = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").hadEnergy() : 0', 'F'),
    genJetInvisibleEnergy     = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").invisibleEnergy() : 0', 'F'),
    genJetNConstituents       = cms.vstring('? (userCand("genJet").isNonnull() ) ? userCand("genJet").nConstituents() : 0', 'I'),
)

#statusOneCandidates = commonGenCandidates.clone(
#    isPrompt               = cms.vstring('isPromptFinalState()','I'),
#    isFromTau              = cms.vstring('isDirectPromptTauDecayProductFinalState()','I'),
#    isPromptDecayed        = cms.vstring('isPromptDecayed()','I'),
#    fromHardProcess        = cms.vstring('fromHardProcessFinalState()','I'),
#    fromHardProcessDecayed = cms.vstring('fromHardProcessDecayed()','I'),
#    fromHardProcessTau     = cms.vstring('isDirectHardProcessTauDecayProductFinalState()','I'),
#)

commonGenJetCandidates = commonCandidates.clone(
    status           = cms.vstring('status()','I'),
    emEnergy         = cms.vstring('emEnergy()','F'),
    hadEnergy        = cms.vstring('hadEnergy()','F'),
    invisibileEnergy = cms.vstring('invisibleEnergy()','F'),
    nConstituents    = cms.vstring('nConstituents','I'),
    
)

commonMet = cms.PSet(
    et  = cms.vstring('pt()','F'),
    phi = cms.vstring('phi()','F'),
)

commonVertex = cms.PSet(
    x              = cms.vstring('x','F'),
    y              = cms.vstring('y','F'),
    z              = cms.vstring('z','F'),
    xError         = cms.vstring('xError','F'),
    yError         = cms.vstring('yError','F'),
    zError         = cms.vstring('zError','F'),
    chi2           = cms.vstring('chi2','F'),
    ndof           = cms.vstring('ndof','F'),
    normalizedChi2 = cms.vstring('normalizedChi2','F'),
    isValid        = cms.vstring('isValid', 'I'),
    isFake         = cms.vstring('isFake', 'I'),
    rho            = cms.vstring('position.Rho','F'),
)

# trigger
triggerBranches = cms.PSet()
for trigger in triggerMap:
    setattr(triggerBranches,trigger,cms.PSet( path = cms.string(triggerMap[trigger]['path']) ))


# filters
filterBranches = cms.PSet(
    HBHENoiseFilter                    = cms.PSet( path  = cms.string('Flag_HBHENoiseFilter') ),
    HBHENoiseIsoFilter                 = cms.PSet( path  = cms.string('Flag_HBHENoiseIsoFilter') ),
    globalTightHalo2016Filter          = cms.PSet( path  = cms.string('Flag_globalTightHalo2016Filter') ),
    EcalDeadCellTriggerPrimitiveFilter = cms.PSet( path  = cms.string('Flag_EcalDeadCellTriggerPrimitiveFilter') ),
    goodVertices                       = cms.PSet( path  = cms.string('Flag_goodVertices') ),
    eeBadScFilter                      = cms.PSet( path  = cms.string('Flag_eeBadScFilter') ),
    duplicateMuons                     = cms.PSet( path  = cms.string('Flag_duplicateMuons') ),
    badMuons                           = cms.PSet( path  = cms.string('Flag_badMuons') ),
    noBadMuons                         = cms.PSet( path  = cms.string('Flag_noBadMuons') ),
)

customFilterBranches = cms.PSet(
    BadChargedCandidateFilter          = cms.PSet( inputTag = cms.InputTag('BadChargedCandidateFilter') ),
)

# vertices
vertexBranches = commonVertex.clone()

# vertexComposite

vertexCompositeBranches = commonCandidates.clone(
    x                      = cms.vstring('position().x','F'),
    y                      = cms.vstring('position().y','F'),
    z                      = cms.vstring('position().z','F'),
    chi2                   = cms.vstring('vertexChi2','F'),
    ndof                   = cms.vstring('vertexNdof','F'),
    normalizedChi2         = cms.vstring('vertexNormalizedChi2','F'),
    numberOfDaughters      = cms.vstring('numberOfDaughters()','I'),
    daughter1_pdgId        = cms.vstring('? numberOfDaughters()>0 ? daughter(0).pdgId() : 0','I'),
    daughter1_pt           = cms.vstring('? numberOfDaughters()>0 ? daughter(0).pt() : 0','F'),
    daughter1_eta          = cms.vstring('? numberOfDaughters()>0 ? daughter(0).eta() : 0','F'),
    daughter1_phi          = cms.vstring('? numberOfDaughters()>0 ? daughter(0).phi() : 0','F'),
    daughter1_mass         = cms.vstring('? numberOfDaughters()>0 ? daughter(0).mass() : 0','F'),
    daughter1_energy       = cms.vstring('? numberOfDaughters()>0 ? daughter(0).energy() : 0','F'),
    daughter1_charge       = cms.vstring('? numberOfDaughters()>0 ? daughter(0).charge() : 0','F'),
    daughter2_pdgId        = cms.vstring('? numberOfDaughters()>1 ? daughter(1).pdgId() : 0','I'),
    daughter2_pt           = cms.vstring('? numberOfDaughters()>1 ? daughter(1).pt() : 0','F'),
    daughter2_eta          = cms.vstring('? numberOfDaughters()>1 ? daughter(1).eta() : 0','F'),
    daughter2_phi          = cms.vstring('? numberOfDaughters()>1 ? daughter(1).phi() : 0','F'),
    daughter2_mass         = cms.vstring('? numberOfDaughters()>1 ? daughter(1).mass() : 0','F'),
    daughter2_energy       = cms.vstring('? numberOfDaughters()>1 ? daughter(1).energy() : 0','F'),
    daughter2_charge       = cms.vstring('? numberOfDaughters()>1 ? daughter(1).charge() : 0','F'),
)

# packed
packedBranches = commonCandidates.clone()

# genParticles
genParticleBranches = commonGenCandidates.clone()
genJetBranches = commonGenJetCandidates.clone()

# electrons
electronBranches = commonPatCandidates.clone(
    # supercluster
    superClusterEta                = cms.vstring('superCluster().eta','F'),
    superClusterPhi                = cms.vstring('superCluster().phi','F'),
    superClusterEnergy             = cms.vstring('superCluster().energy','F'),
    superClusterRawEnergy          = cms.vstring('superCluster().rawEnergy','F'),
    superClusterPreshowerEnergy    = cms.vstring('superCluster().preshowerEnergy','F'),
    superClusterPhiWidth           = cms.vstring('superCluster().phiWidth','F'),
    superClusterEtaWidth           = cms.vstring('superCluster().etaWidth','F'),
    # isolation
    pfChargedHadronIso             = cms.vstring('userIsolation("PfChargedHadronIso")','F'),
    pfNeutralHadronIso             = cms.vstring('userIsolation("PfNeutralHadronIso")','F'),
    pfGammaIso                     = cms.vstring('userIsolation("PfGammaIso")','F'),
    pfPUChargedHadronIso           = cms.vstring('userIsolation("PfPUChargedHadronIso")','F'),
    dr03TkSumPt                    = cms.vstring('dr03TkSumPt()','F'),
    dr03EcalRecHitSumEt            = cms.vstring('dr03EcalRecHitSumEt()','F'),
    dr03HcalTowerSumEt             = cms.vstring('dr03HcalTowerSumEt()','F'),
    effectiveArea                  = cms.vstring('userFloat("EffectiveArea")','F'),
    relPFIsoDeltaBetaR03           = cms.vstring(
        '(userIsolation("PfChargedHadronIso")'
        '+max(userIsolation("PfNeutralHadronIso")'
        '+userIsolation("PfGammaIso")'
        '-0.5*userIsolation("PfPUChargedHadronIso"),0.0))'
        '/pt()',
        'F'
    ),
    relPFIsoRhoR03                 = cms.vstring(
        '(chargedHadronIso()'
        '+max(0.0,neutralHadronIso()'
        '+photonIso()'
        '-userFloat("rho")*userFloat("EffectiveArea")))'
        '/pt()',
        'F'
    ),
    # shower shape / ID variables
    passConversionVeto             = cms.vstring('passConversionVeto()','I'),
    hcalOverEcal                   = cms.vstring('hcalOverEcal','F'),
    hcalDepth1OverEcal             = cms.vstring('hcalDepth1OverEcal','F'),
    hcalDepth2OverEcal             = cms.vstring('hcalDepth2OverEcal','F'),
    sigmaIetaIeta                  = cms.vstring('sigmaIetaIeta','F'),
    deltaEtaSuperClusterTrackAtVtx = cms.vstring('deltaEtaSuperClusterTrackAtVtx','F'),
    deltaPhiSuperClusterTrackAtVtx = cms.vstring('deltaPhiSuperClusterTrackAtVtx','F'),
    deltaEtaSeedClusterTrackAtVtx  = cms.vstring('deltaEtaSeedClusterTrackAtVtx','F'),
    fbrem                          = cms.vstring('fbrem','F'),
    eSuperClusterOverP             = cms.vstring('eSuperClusterOverP','F'),
    ecalEnergy                     = cms.vstring('ecalEnergy','F'),
    scE1x5                         = cms.vstring('scE1x5','F'),
    scE2x5Max                      = cms.vstring('scE2x5Max','F'),
    scE5x5                         = cms.vstring('scE5x5','F'),
    missingHits                    = cms.vstring('userInt("missingHits")','I'),
    # charge id
    isGsfCtfScPixChargeConsistent  = cms.vstring('isGsfCtfScPixChargeConsistent','I'),
    isGsfScPixChargeConsistent     = cms.vstring('isGsfScPixChargeConsistent','I'),
    isGsfCtfChargeConsistent       = cms.vstring('isGsfCtfChargeConsistent','I'),
    # ID
    cutBasedVeto                   = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-veto")','I'),
    cutBasedLoose                  = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-loose")','I'),
    cutBasedMedium                 = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-medium")','I'),
    cutBasedTight                  = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-tight")','I'),
    #cutBasedHLTPreselection        = cms.vstring('userInt("cutBasedElectronHLTPreselection-Summer16-V1")','I'),
    #heepV70                        = cms.vstring('userInt("heepElectronID-HEEPV70")','I'),
    mvaWP90                        = cms.vstring('userInt("mvaEleID-Fall17-iso-V1-wp90")','I'),
    mvaWP80                        = cms.vstring('userInt("mvaEleID-Fall17-iso-V1-wp80")','I'),
    mvaWPLoose                     = cms.vstring('userInt("mvaEleID-Fall17-iso-V1-wpLoose")','I'),
    mvaWP90NoIso                   = cms.vstring('userInt("mvaEleID-Fall17-noIso-V1-wp90")','I'),
    mvaWP80NoIso                   = cms.vstring('userInt("mvaEleID-Fall17-noIso-V1-wp80")','I'),
    mvaWPLooseNoIso                = cms.vstring('userInt("mvaEleID-Fall17-noIso-V1-wpLoose")','I'),
    cutBasedVetoNoIso              = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-vetoNoIso")','I'),
    cutBasedLooseNoIso             = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-looseNoIso")','I'),
    cutBasedMediumNoIso            = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-mediumNoIso")','I'),
    cutBasedTightNoIso             = cms.vstring('userInt("cutBasedElectronID-Fall17-94X-V1-tightNoIso")','I'),
    #wwLoose                        = cms.vstring('userInt("WWLoose")','I'),
    mvaValues                      = cms.vstring('userFloat("ElectronMVAEstimatorRun2Fall17IsoV1Values")','F'),
    mvaValuesNoIso                 = cms.vstring('userFloat("ElectronMVAEstimatorRun2Fall17NoIsoV1Values")','F'),
    mvaCategories                  = cms.vstring('userInt("ElectronMVAEstimatorRun2Fall17IsoV1Categories")','I'),
    mvaCategoriesNoIso             = cms.vstring('userInt("ElectronMVAEstimatorRun2Fall17NoIsoV1Categories")','I'),
    #mvaHZZWPLoose                  = cms.vstring('userInt("mvaEleID-Spring16-HZZ-V1-wpLoose")','I'),
    #mvaHZZValues                   = cms.vstring('userFloat("ElectronMVAEstimatorRun2Spring16HZZV1Values")','F'),
    #mvaHZZCategories               = cms.vstring('userInt("ElectronMVAEstimatorRun2Spring16HZZV1Categories")','I'),
    #hzzLoose                       = cms.vstring('userInt("HZZ4lIDPass")','I'),
    #hzzTight                       = cms.vstring('userInt("HZZ4lIDPassTight")','I'),
    #miniIsolation                  = cms.vstring('userFloat("MiniIsolation")','F'),
    #miniIsolationCharged           = cms.vstring('userFloat("MiniIsolationCharged")','F'),
    #miniIsolationNeutral           = cms.vstring('userFloat("MiniIsolationNeutral")','F'),
    #miniIsolationPhoton            = cms.vstring('userFloat("MiniIsolationPileup")','F'),
    #miniIsolationPileup            = cms.vstring('userFloat("MiniIsolationPhoton")','F'),
    #susyEA                         = cms.vstring('userFloat("SUSYEA")','F'),
    #susyRho                        = cms.vstring('userFloat("SUSYRho")','F'),
    #susyMiniIsolationEA            = cms.vstring('userFloat("SUSYMiniIsolationEA")','F'),
    #isSUSYTight                    = cms.vstring('userInt("isSUSYTight")','I'),
    #isSUSYVLoose                   = cms.vstring('userInt("isSUSYVLoose")','I'),
    #isSUSYVLooseFOIDEmu            = cms.vstring('userInt("isSUSYVLooseFOIDEmu")','I'),
    #isSUSYVLooseFOIDISOEmu         = cms.vstring('userInt("isSUSYVLooseFOIDISOEmu")','I'),
    #isSUSYMVAPreselection          = cms.vstring('userInt("isSUSYMVAPreselection")','I'),
    #susyMVA                        = cms.vstring('userFloat("SUSYMVA")','F'),
    #jetPtRatio                     = cms.vstring('userFloat("jet_ptRatio")','F'),
    #jetPtRel                       = cms.vstring('userFloat("jet_ptRel")','F'),
    #jetNumberOfChargedDaughters    = cms.vstring('userInt("jet_numberOfChargedDaughters")','I'),
    #jetBtagCSV                     = cms.vstring('userFloat("jet_pfCombinedInclusiveSecondaryVertexV2BJetTags")','F'),
    # pv
    dz                             = cms.vstring('userFloat("dz")','F'),
    dxy                            = cms.vstring('userFloat("dxy")','F'),
    dz_beamspot                    = cms.vstring('userFloat("dz_beamspot")','F'),
    dxy_beamspot                   = cms.vstring('userFloat("dxy_beamspot")','F'),
    dz_zero                        = cms.vstring('userFloat("dz_zero")','F'),
    dxy_zero                       = cms.vstring('userFloat("dxy_zero")','F'),
    dB2D                           = cms.vstring('userFloat("dB2D")','F'),
    dB3D                           = cms.vstring('userFloat("dB3D")','F'),
    edB2D                          = cms.vstring('userFloat("edB2D")','F'),
    edB3D                          = cms.vstring('userFloat("edB3D")','F'),
    # energy shifts
    pt_electronEnUp                = cms.vstring('? hasUserCand("ElectronEnUp") ? userCand("ElectronEnUp").pt() : 0','F'),
    energy_electronEnUp            = cms.vstring('? hasUserCand("ElectronEnUp") ? userCand("ElectronEnUp").energy() : 0','F'),
    pt_electronEnDown              = cms.vstring('? hasUserCand("ElectronEnDown") ? userCand("ElectronEnDown").pt() : 0','F'),
    energy_electronEnDown          = cms.vstring('? hasUserCand("ElectronEnDown") ? userCand("ElectronEnDown").energy() : 0','F'),
    # uncorrected objects
    pt_uncorrected                 = cms.vstring('? hasUserCand("uncorrected") ? userCand("uncorrected").pt() : 0','F'),
    energy_uncorrected             = cms.vstring('? hasUserCand("uncorrected") ? userCand("uncorrected").energy() : 0','F'),
    
)

# muons
muonBranches = commonPatCandidates.clone(
    # type
    isPFMuon                = cms.vstring('isPFMuon','I'),
    isGlobalMuon            = cms.vstring('isGlobalMuon','I'),
    isTrackerMuon           = cms.vstring('isTrackerMuon','I'),
    muonBestTrackType       = cms.vstring('muonBestTrackType','I'),
    pt_tuneP                = cms.vstring('? tunePMuonBestTrack.isNonnull ? tunePMuonBestTrack().pt : -1','F'),
    muonBestTrackType_tuneP = cms.vstring('tunePMuonBestTrackType','I'),
    # isolation
    sumChargedHadronPtR03   = cms.vstring('pfIsolationR03().sumChargedHadronPt','F'),
    sumNeutralHadronEtR03   = cms.vstring('pfIsolationR03().sumNeutralHadronEt','F'),
    sumPhotonEtR03          = cms.vstring('pfIsolationR03().sumPhotonEt','F'),
    sumPUPtR03              = cms.vstring('pfIsolationR03().sumPUPt','F'),
    trackIso                = cms.vstring('trackIso()','F'),
    ecalIso                 = cms.vstring('ecalIso()','F'),
    hcalIso                 = cms.vstring('hcalIso()','F'),
    sumChargedHadronPtR04   = cms.vstring('pfIsolationR04().sumChargedHadronPt','F'),
    sumNeutralHadronEtR04   = cms.vstring('pfIsolationR04().sumNeutralHadronEt','F'),
    sumPhotonEtR04          = cms.vstring('pfIsolationR04().sumPhotonEt','F'),
    sumPUPtR04              = cms.vstring('pfIsolationR04().sumPUPt','F'),
    relPFIsoDeltaBetaR04    = cms.vstring(
        '(pfIsolationR04().sumChargedHadronPt'
        '+ max(0., pfIsolationR04().sumNeutralHadronEt'
        '+ pfIsolationR04().sumPhotonEt'
        '- 0.5*pfIsolationR04().sumPUPt))'
        '/pt()',
        'F'
    ),
    relPFIsoDeltaBetaR03    = cms.vstring(
        '(pfIsolationR03().sumChargedHadronPt'
        '+ max(0., pfIsolationR03().sumNeutralHadronEt'
        '+ pfIsolationR03().sumPhotonEt'
        '- 0.5*pfIsolationR03().sumPUPt))'
        '/pt()',
        'F'
    ),
    # ID
    isTightMuon                 = cms.vstring('userInt("isTightMuon")','I'),
    isHighPtMuon                = cms.vstring('userInt("isHighPtMuon")','I'),
    isSoftMuon                  = cms.vstring('userInt("isSoftMuon")','I'),
    isSoftMuonICHEP             = cms.vstring('userInt("isSoftMuonICHEP")','I'),
    isMediumMuon                = cms.vstring('isMediumMuon','I'),
    isMediumMuonICHEP           = cms.vstring('userInt("isMediumMuonICHEP")','I'),
    isLooseMuon                 = cms.vstring('isLooseMuon','I'),
    segmentCompatibility        = cms.vstring('userFloat("segmentCompatibility")','F'),
    isGoodMuon                  = cms.vstring('userInt("isGoodMuon")','I'),
    highPurityTrack             = cms.vstring('userInt("highPurityTrack")','I'),
    matchedStations             = cms.vstring('numberOfMatchedStations','I'),
    validMuonHits               = cms.vstring('? globalTrack.isNonnull ? globalTrack().hitPattern().numberOfValidMuonHits : -1','I'),
    normalizedChi2              = cms.vstring('? globalTrack.isNonnull ? globalTrack().normalizedChi2 : -1','F'),
    validPixelHits              = cms.vstring('? innerTrack.isNonnull ? innerTrack().hitPattern().numberOfValidPixelHits : -1','I'),
    trackerLayers               = cms.vstring('? innerTrack.isNonnull ? innerTrack().hitPattern().trackerLayersWithMeasurement : -1','I'),
    pixelLayers                 = cms.vstring('? innerTrack.isNonnull ? innerTrack().hitPattern().pixelLayersWithMeasurement : -1','I'),
    validTrackerFraction        = cms.vstring('? innerTrack.isNonnull ? innerTrack().validFraction : -1','F'),
    bestTrackPtError            = cms.vstring('? muonBestTrack.isNonnull ? muonBestTrack().ptError : -1','F'),
    bestTrackPt                 = cms.vstring('? muonBestTrack.isNonnull ? muonBestTrack().pt : -1','F'),
    trackerStandaloneMatch      = cms.vstring('combinedQuality().chi2LocalPosition','F'),
    trackKink                   = cms.vstring('combinedQuality().trkKink','F'),
    #hzzLoose                    = cms.vstring('userInt("HZZ4lIDPass")','I'),
    #hzzTight                    = cms.vstring('userInt("HZZ4lIDPassTight")','I'),
    #miniIsolation               = cms.vstring('userFloat("MiniIsolation")','F'),
    #miniIsolationCharged        = cms.vstring('userFloat("MiniIsolationCharged")','F'),
    #miniIsolationNeutral        = cms.vstring('userFloat("MiniIsolationNeutral")','F'),
    #miniIsolationPhoton         = cms.vstring('userFloat("MiniIsolationPileup")','F'),
    #miniIsolationPileup         = cms.vstring('userFloat("MiniIsolationPhoton")','F'),
    #susyEA                      = cms.vstring('userFloat("SUSYEA")','F'),
    #susyRho                     = cms.vstring('userFloat("SUSYRho")','F'),
    #susyMiniIsolationEA         = cms.vstring('userFloat("SUSYMiniIsolationEA")','F'),
    #isSUSYMVAPreselection       = cms.vstring('userInt("isSUSYMVAPreselection")','I'),
    #susyMVA                     = cms.vstring('userFloat("SUSYMVA")','F'),
    #jetPtRatio                  = cms.vstring('userFloat("jet_ptRatio")','F'),
    #jetPtRel                    = cms.vstring('userFloat("jet_ptRel")','F'),
    #jetNumberOfChargedDaughters = cms.vstring('userInt("jet_numberOfChargedDaughters")','I'),
    #jetBtagCSV                  = cms.vstring('userFloat("jet_pfCombinedInclusiveSecondaryVertexV2BJetTags")','F'),
    # pv
    dz                          = cms.vstring('userFloat("dz")','F'),
    dxy                         = cms.vstring('userFloat("dxy")','F'),
    dz_beamspot                 = cms.vstring('userFloat("dz_beamspot")','F'),
    dxy_beamspot                = cms.vstring('userFloat("dxy_beamspot")','F'),
    dz_zero                     = cms.vstring('userFloat("dz_zero")','F'),
    dxy_zero                    = cms.vstring('userFloat("dxy_zero")','F'),
    dB2D                        = cms.vstring('userFloat("dB2D")','F'),
    dB3D                        = cms.vstring('userFloat("dB3D")','F'),
    edB2D                       = cms.vstring('userFloat("edB2D")','F'),
    edB3D                       = cms.vstring('userFloat("edB3D")','F'),
    # corrections
    #rochesterPt                 = cms.vstring('userFloat("rochesterPt")','F'),
    #rochesterEnergy             = cms.vstring('userFloat("rochesterEnergy")','F'),
    #rochesterError              = cms.vstring('userFloat("rochesterError")','F'),
    # energy shifts
    pt_muonEnUp                 = cms.vstring('? hasUserCand("MuonEnUp") ? userCand("MuonEnUp").pt() : 0','F'),
    energy_muonEnUp             = cms.vstring('? hasUserCand("MuonEnUp") ? userCand("MuonEnUp").energy() : 0','F'),
    pt_muonEnDown               = cms.vstring('? hasUserCand("MuonEnDown") ? userCand("MuonEnDown").pt() : 0','F'),
    energy_muonEnDown           = cms.vstring('? hasUserCand("MuonEnDown") ? userCand("MuonEnDown").energy() : 0','F'),

)

# taus
tauBranches = commonJetCandidates.clone(
    againstElectronVLooseMVA6                        = cms.vstring('tauID("againstElectronVLooseMVA6")','I'),
    againstElectronLooseMVA6                         = cms.vstring('tauID("againstElectronLooseMVA6")','I'),
    againstElectronMediumMVA6                        = cms.vstring('tauID("againstElectronMediumMVA6")','I'),
    againstElectronTightMVA6                         = cms.vstring('tauID("againstElectronTightMVA6")','I'),
    againstElectronVTightMVA6                        = cms.vstring('tauID("againstElectronVTightMVA6")','I'),
    againstElectronMVA6category                      = cms.vstring('tauID("againstElectronMVA6category")','I'),
    againstElectronMVA6Raw                           = cms.vstring('tauID("againstElectronMVA6Raw")','F'),

    againstMuonLoose3                                = cms.vstring('tauID("againstMuonLoose3")','I'),
    againstMuonTight3                                = cms.vstring('tauID("againstMuonTight3")','I'),

    byCombinedIsolationDeltaBetaCorrRaw3Hits         = cms.vstring('tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")','I'),

    byVLooseIsolationMVArun2v1DBdR03oldDMwLT         = cms.vstring('tauID("byVLooseIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byLooseIsolationMVArun2v1DBdR03oldDMwLT          = cms.vstring('tauID("byLooseIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byMediumIsolationMVArun2v1DBdR03oldDMwLT         = cms.vstring('tauID("byMediumIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byTightIsolationMVArun2v1DBdR03oldDMwLT          = cms.vstring('tauID("byTightIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byVTightIsolationMVArun2v1DBdR03oldDMwLT         = cms.vstring('tauID("byVTightIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byVVTightIsolationMVArun2v1DBdR03oldDMwLT        = cms.vstring('tauID("byVVTightIsolationMVArun2v1DBdR03oldDMwLT")','I'),
    byIsolationMVArun2v1DBdR03oldDMwLTraw            = cms.vstring('tauID("byIsolationMVArun2v1DBdR03oldDMwLTraw")','F'),

    byVLooseIsolationMVArun2v1DBnewDMwLT             = cms.vstring('tauID("byVLooseIsolationMVArun2v1DBnewDMwLT")','I'),
    byLooseIsolationMVArun2v1DBnewDMwLT              = cms.vstring('tauID("byLooseIsolationMVArun2v1DBnewDMwLT")','I'),
    byMediumIsolationMVArun2v1DBnewDMwLT             = cms.vstring('tauID("byMediumIsolationMVArun2v1DBnewDMwLT")','I'),
    byTightIsolationMVArun2v1DBnewDMwLT              = cms.vstring('tauID("byTightIsolationMVArun2v1DBnewDMwLT")','I'),
    byVTightIsolationMVArun2v1DBnewDMwLT             = cms.vstring('tauID("byVTightIsolationMVArun2v1DBnewDMwLT")','I'),
    byVVTightIsolationMVArun2v1DBnewDMwLT            = cms.vstring('tauID("byVVTightIsolationMVArun2v1DBnewDMwLT")','I'),
    byIsolationMVArun2v1DBnewDMwLTraw                = cms.vstring('tauID("byIsolationMVArun2v1DBnewDMwLTraw")','F'),

    byVLooseIsolationMVArun2v1DBoldDMwLT             = cms.vstring('tauID("byVLooseIsolationMVArun2v1DBoldDMwLT")','I'),
    byLooseIsolationMVArun2v1DBoldDMwLT              = cms.vstring('tauID("byLooseIsolationMVArun2v1DBoldDMwLT")','I'),
    byMediumIsolationMVArun2v1DBoldDMwLT             = cms.vstring('tauID("byMediumIsolationMVArun2v1DBoldDMwLT")','I'),
    byTightIsolationMVArun2v1DBoldDMwLT              = cms.vstring('tauID("byTightIsolationMVArun2v1DBoldDMwLT")','I'),
    byVTightIsolationMVArun2v1DBoldDMwLT             = cms.vstring('tauID("byVTightIsolationMVArun2v1DBoldDMwLT")','I'),
    byVVTightIsolationMVArun2v1DBoldDMwLT            = cms.vstring('tauID("byVVTightIsolationMVArun2v1DBoldDMwLT")','I'),
    byIsolationMVArun2v1DBoldDMwLTraw                = cms.vstring('tauID("byIsolationMVArun2v1DBoldDMwLTraw")','F'),
    
    byVLooseIsolationMVArun2v1PWdR03oldDMwLT         = cms.vstring('tauID("byVLooseIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byLooseIsolationMVArun2v1PWdR03oldDMwLT          = cms.vstring('tauID("byLooseIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byMediumIsolationMVArun2v1PWdR03oldDMwLT         = cms.vstring('tauID("byMediumIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byTightIsolationMVArun2v1PWdR03oldDMwLT          = cms.vstring('tauID("byTightIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byVTightIsolationMVArun2v1PWdR03oldDMwLT         = cms.vstring('tauID("byVTightIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byVVTightIsolationMVArun2v1PWdR03oldDMwLT        = cms.vstring('tauID("byVVTightIsolationMVArun2v1PWdR03oldDMwLT")','I'),
    byIsolationMVArun2v1PWdR03oldDMwLTraw            = cms.vstring('tauID("byIsolationMVArun2v1PWdR03oldDMwLTraw")','F'),
    
    byVLooseIsolationMVArun2v1PWnewDMwLT             = cms.vstring('tauID("byVLooseIsolationMVArun2v1PWnewDMwLT")','I'),
    byLooseIsolationMVArun2v1PWnewDMwLT              = cms.vstring('tauID("byLooseIsolationMVArun2v1PWnewDMwLT")','I'),
    byMediumIsolationMVArun2v1PWnewDMwLT             = cms.vstring('tauID("byMediumIsolationMVArun2v1PWnewDMwLT")','I'),
    byTightIsolationMVArun2v1PWnewDMwLT              = cms.vstring('tauID("byTightIsolationMVArun2v1PWnewDMwLT")','I'),
    byVTightIsolationMVArun2v1PWnewDMwLT             = cms.vstring('tauID("byVTightIsolationMVArun2v1PWnewDMwLT")','I'),
    byVVTightIsolationMVArun2v1PWnewDMwLT            = cms.vstring('tauID("byVVTightIsolationMVArun2v1PWnewDMwLT")','I'),
    byIsolationMVArun2v1PWnewDMwLTraw                = cms.vstring('tauID("byIsolationMVArun2v1PWnewDMwLTraw")','F'),

    byVLooseIsolationMVArun2v1PWoldDMwLT             = cms.vstring('tauID("byVLooseIsolationMVArun2v1PWoldDMwLT")','I'),
    byLooseIsolationMVArun2v1PWoldDMwLT              = cms.vstring('tauID("byLooseIsolationMVArun2v1PWoldDMwLT")','I'),
    byMediumIsolationMVArun2v1PWoldDMwLT             = cms.vstring('tauID("byMediumIsolationMVArun2v1PWoldDMwLT")','I'),
    byTightIsolationMVArun2v1PWoldDMwLT              = cms.vstring('tauID("byTightIsolationMVArun2v1PWoldDMwLT")','I'),
    byVTightIsolationMVArun2v1PWoldDMwLT             = cms.vstring('tauID("byVTightIsolationMVArun2v1PWoldDMwLT")','I'),
    byVVTightIsolationMVArun2v1PWoldDMwLT            = cms.vstring('tauID("byVVTightIsolationMVArun2v1PWoldDMwLT")','I'),
    byIsolationMVArun2v1PWoldDMwLTraw                = cms.vstring('tauID("byIsolationMVArun2v1PWoldDMwLTraw")','F'),

    chargedIsoPtSum                                  = cms.vstring('tauID("chargedIsoPtSum")','F'),
    chargedIsoPtSumdR03                              = cms.vstring('tauID("chargedIsoPtSumdR03")','F'),
    neutralIsoPtSum                                  = cms.vstring('tauID("neutralIsoPtSum")','F'),
    neutralIsoPtSumWeight                            = cms.vstring('tauID("neutralIsoPtSumWeight")','F'),
    neutralIsoPtSumWeightdR03                        = cms.vstring('tauID("neutralIsoPtSumWeightdR03")','F'),
    neutralIsoPtSumdR03                              = cms.vstring('tauID("neutralIsoPtSumdR03")','F'),
    footprintCorrection                              = cms.vstring('tauID("footprintCorrection")','F'),
    footprintCorrectiondR03                          = cms.vstring('tauID("footprintCorrectiondR03")','F'),
    puCorrPtSum                                      = cms.vstring('tauID("puCorrPtSum")','F'),

    byPhotonPtSumOutsideSignalCone                   = cms.vstring('tauID("byPhotonPtSumOutsideSignalCone")','I'),
    photonPtSumOutsideSignalCone                     = cms.vstring('tauID("photonPtSumOutsideSignalCone")','F'),
    photonPtSumOutsideSignalConedR03                 = cms.vstring('tauID("photonPtSumOutsideSignalConedR03")','F'),

    decayMode                                        = cms.vstring('decayMode','I'),
    decayModeFinding                                 = cms.vstring('tauID("decayModeFinding")','I'),
    decayModeFindingNewDMs                           = cms.vstring('tauID("decayModeFindingNewDMs")','I'),
    # pv
    dz                                               = cms.vstring('userFloat("dz")','F'),
    dxy                                              = cms.vstring('userFloat("dxy")','F'),
    dz_beamspot                                      = cms.vstring('userFloat("dz_beamspot")','F'),
    dxy_beamspot                                     = cms.vstring('userFloat("dxy_beamspot")','F'),
    dz_zero                                          = cms.vstring('userFloat("dz_zero")','F'),
    dxy_zero                                         = cms.vstring('userFloat("dxy_zero")','F'),
    # energy shifts
    pt_tauEnUp                                       = cms.vstring('? hasUserCand("TauEnUp") ? userCand("TauEnUp").pt() : 0','F'),
    energy_tauEnUp                                   = cms.vstring('? hasUserCand("TauEnUp") ? userCand("TauEnUp").energy() : 0','F'),
    pt_tauEnDown                                     = cms.vstring('? hasUserCand("TauEnDown") ? userCand("TauEnDown").pt() : 0','F'),
    energy_tauEnDown                                 = cms.vstring('? hasUserCand("TauEnDown") ? userCand("TauEnDown").energy() : 0','F'),
)

# photons
photonBranches = commonPatCandidates.clone(
    # supercluster
    superClusterEta                = cms.vstring('superCluster().eta','F'),
    superClusterPhi                = cms.vstring('superCluster().phi','F'),
    superClusterEnergy             = cms.vstring('superCluster().energy','F'),
    superClusterRawEnergy          = cms.vstring('superCluster().rawEnergy','F'),
    superClusterPreshowerEnergy    = cms.vstring('superCluster().preshowerEnergy','F'),
    superClusterPhiWidth           = cms.vstring('superCluster().phiWidth','F'),
    superClusterEtaWidth           = cms.vstring('superCluster().etaWidth','F'),
    # isolation
    gammaDR030                     = cms.vstring('userFloat("gammaDR030")','F'),
    phoWorstChargedIsolationWithConeVeto = cms.vstring('userFloat("phoWorstChargedIsolationWithConeVeto")','F'),
    phoESEffSigmaRR                = cms.vstring('userFloat("phoESEffSigmaRR")','F'),
    phoFull5x5E1x3                 = cms.vstring('userFloat("phoFull5x5E1x3")','F'),
    phoFull5x5E2x2                 = cms.vstring('userFloat("phoFull5x5E2x2")','F'),
    phoFull5x5E2x5Max              = cms.vstring('userFloat("phoFull5x5E2x5Max")','F'),
    phoFull5x5E5x5                 = cms.vstring('userFloat("phoFull5x5E5x5")','F'),
    phoFull5x5SigmaIEtaIEta        = cms.vstring('userFloat("phoFull5x5SigmaIEtaIEta")','F'),
    phoFull5x5SigmaIEtaIPhi        = cms.vstring('userFloat("phoFull5x5SigmaIEtaIPhi")','F'),
    phoChargedIsolation            = cms.vstring('userFloat("phoChargedIsolation")','F'),
    phoNeutralHadronIsolation      = cms.vstring('userFloat("phoNeutralHadronIsolation")','F'),
    phoPhotonIsolation             = cms.vstring('userFloat("phoPhotonIsolation")','F'),
    effectiveAreaChargedHadrons    = cms.vstring('userFloat("EffectiveAreaChargedHadrons")','F'),
    effectiveAreaNeutralHadrons    = cms.vstring('userFloat("EffectiveAreaNeutralHadrons")','F'),
    effectiveAreaPhotons           = cms.vstring('userFloat("EffectiveAreaPhotons")','F'),
    trackIso                       = cms.vstring('trackIso','F'),
    # type
    passElectronVeto               = cms.vstring('passElectronVeto','I'),
    hasPixelSeed                   = cms.vstring('hasPixelSeed','I'),
    isPFlowPhoton                  = cms.vstring('isPFlowPhoton','I'),
    isStandardPhoton               = cms.vstring('isStandardPhoton','I'),
    # ID
    cutBasedLoose                  = cms.vstring('userInt("cutBasedPhotonID-Fall17-94X-V1-loose")','I'),
    cutBasedMedium                 = cms.vstring('userInt("cutBasedPhotonID-Fall17-94X-V1-medium")','I'),
    cutBasedTight                  = cms.vstring('userInt("cutBasedPhotonID-Fall17-94X-V1-tight")','I'),
    mvaWP80                        = cms.vstring('userInt("mvaPhoID-RunIIFall17-v1p1-wp80")','I'),
    mvaWP90                        = cms.vstring('userInt("mvaPhoID-RunIIFall17-v1p1-wp90")','I'),
    mvaValues                      = cms.vstring('userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values")','F'),
    mvaCategories                  = cms.vstring('userInt("PhotonMVAEstimatorRunIIFall17v1p1Categories")','I'),
    # ID variables
    hadronicOverEM                 = cms.vstring('hadronicOverEm','F'),
    hadronicDepth1OverEm           = cms.vstring('hadronicDepth1OverEm','F'),
    hadronicDepth2OverEm           = cms.vstring('hadronicDepth2OverEm','F'),
    sigmaIEtaIEta                  = cms.vstring('sigmaIetaIeta','F'),
    e1x5                           = cms.vstring('e1x5','F'),
    e2x5                           = cms.vstring('e2x5','F'),
    e3x3                           = cms.vstring('e3x3','F'),
    e5x5                           = cms.vstring('e5x5','F'),
    maxEnergyXtal                  = cms.vstring('maxEnergyXtal','F'),
    r1x5                           = cms.vstring('r1x5','F'),
    r2x5                           = cms.vstring('r2x5','F'),
    r9                             = cms.vstring('r9','F'),
    # location
    isEB                           = cms.vstring('isEB','I'),
    isEE                           = cms.vstring('isEE','I'),
    isEBGap                        = cms.vstring('isEBGap','I'),
    isEBEtaGap                     = cms.vstring('isEBEtaGap','I'),
    isEBPhiGap                     = cms.vstring('isEBPhiGap','I'),
    isEEGap                        = cms.vstring('isEEGap','I'),
    isEERingGap                    = cms.vstring('isEERingGap','I'),
    isEEDeeGap                     = cms.vstring('isEEDeeGap','I'),
    isEBEEGap                      = cms.vstring('isEBEEGap','I'),
    # uncorrected objects
    pt_uncorrected                 = cms.vstring('? hasUserCand("uncorrected") ? userCand("uncorrected").pt() : 0','F'),
    energy_uncorrected             = cms.vstring('? hasUserCand("uncorrected") ? userCand("uncorrected").energy() : 0','F'),
)

# jets
jetBranches = commonJetCandidates.clone(
    # btagging
    #pfCombinedInclusiveSecondaryVertexV2BJetTags = cms.vstring('bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")','F'),
    #pfCombinedMVAV2BJetTags                      = cms.vstring('bDiscriminator("pfCombinedMVAV2BJetTags")','F'),
    #passCSVv2L                                   = cms.vstring('? bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")>0.5426 ? 1 : 0','I'),
    #passCSVv2M                                   = cms.vstring('? bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")>0.8484 ? 1 : 0','I'),
    #passCSVv2T                                   = cms.vstring('? bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")>0.9535 ? 1 : 0','I'),
    #passCMVAv2L                                  = cms.vstring('? bDiscriminator("pfCombinedMVAV2BJetTags")>-0.5884 ? 1 : 0','I'),
    #passCMVAv2M                                  = cms.vstring('? bDiscriminator("pfCombinedMVAV2BJetTags")>0.4432 ? 1 : 0','I'),
    #passCMVAv2T                                  = cms.vstring('? bDiscriminator("pfCombinedMVAV2BJetTags")>0.9432 ? 1 : 0','I'),
    pfJetBProbabilityBJetTags                             = cms.vstring('bDiscriminator("pfJetBProbabilityBJetTags")','F'), 
    pfJetProbabilityBJetTags                              = cms.vstring('bDiscriminator("pfJetProbabilityBJetTags")','F'), 
    pfTrackCountingHighEffBJetTags                        = cms.vstring('bDiscriminator("pfTrackCountingHighEffBJetTags")','F'), 
    pfSimpleSecondaryVertexHighEffBJetTags                = cms.vstring('bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags")','F'), 
    pfSimpleInclusiveSecondaryVertexHighEffBJetTags       = cms.vstring('bDiscriminator("pfSimpleInclusiveSecondaryVertexHighEffBJetTags")','F'), 
    pfCombinedSecondaryVertexV2BJetTags                   = cms.vstring('bDiscriminator("pfCombinedSecondaryVertexV2BJetTags")','F'), 
    pfCombinedInclusiveSecondaryVertexV2BJetTags          = cms.vstring('bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")','F'), 
    softPFMuonBJetTags                                    = cms.vstring('bDiscriminator("softPFMuonBJetTags")','F'), 
    softPFElectronBJetTags                                = cms.vstring('bDiscriminator("softPFElectronBJetTags")','F'), 
    pfCombinedMVAV2BJetTags                               = cms.vstring('bDiscriminator("pfCombinedMVAV2BJetTags")','F'), 
    pfCombinedCvsLJetTags                                 = cms.vstring('bDiscriminator("pfCombinedCvsLJetTags")','F'), 
    pfCombinedCvsBJetTags                                 = cms.vstring('bDiscriminator("pfCombinedCvsBJetTags")','F'), 
    pfDeepCSVJetTags_probb                                = cms.vstring('bDiscriminator("pfDeepCSVJetTags:probb")','F'), 
    pfDeepCSVJetTags_probc                                = cms.vstring('bDiscriminator("pfDeepCSVJetTags:probc")','F'), 
    pfDeepCSVJetTags_probudsg                             = cms.vstring('bDiscriminator("pfDeepCSVJetTags:probudsg")','F'), 
    pfDeepCSVJetTags_probbb                               = cms.vstring('bDiscriminator("pfDeepCSVJetTags:probbb")','F'),
    # flavor
    partonFlavour                                = cms.vstring('partonFlavour','I'),
    # id variables
    neutralHadronEnergyFraction                  = cms.vstring('neutralHadronEnergyFraction','F'),
    neutralEmEnergyFraction                      = cms.vstring('neutralEmEnergyFraction','F'),
    chargedHadronEnergyFraction                  = cms.vstring('chargedHadronEnergyFraction','F'),
    muonEnergyFraction                           = cms.vstring('muonEnergyFraction','F'),
    chargedEmEnergyFraction                      = cms.vstring('chargedEmEnergyFraction','F'),
    chargedMultiplicity                          = cms.vstring('chargedMultiplicity','I'),
    neutralMultiplicity                          = cms.vstring('neutralMultiplicity','I'),
    # ids
    isLoose                                      = cms.vstring('userInt("idLoose")','I'),
    isTight                                      = cms.vstring('userInt("idTight")','I'),
    isTightLepVeto                               = cms.vstring('userInt("idTightLepVeto")','I'),
    puID                                         = cms.vstring('userInt("puID")','I'),
    #pileupJetIdDiscriminant                      = cms.vstring('userFloat("pileupJetIdUpdated:fullDiscriminant")','F'),
    pileupJetIdDiscriminant                      = cms.vstring('userFloat("pileupJetId:fullDiscriminant")','F'),
    caloJetMap_pt                                = cms.vstring('userFloat("caloJetMap:pt")','F'),
    caloJetMap_emEnergyFraction                  = cms.vstring('userFloat("caloJetMap:emEnergyFraction")','F'),
    # energy shifts
    pt_jetEnUp                                   = cms.vstring('? hasUserCand("JetEnUp") ? userCand("JetEnUp").pt() : 0','F'),
    energy_jetEnUp                               = cms.vstring('? hasUserCand("JetEnUp") ? userCand("JetEnUp").energy() : 0','F'),
    pt_jetEnDown                                 = cms.vstring('? hasUserCand("JetEnDown") ? userCand("JetEnDown").pt() : 0','F'),
    energy_jetEnDown                             = cms.vstring('? hasUserCand("JetEnDown") ? userCand("JetEnDown").energy() : 0 ','F'),
    # deep ditau
    deepDiTau_ditau2017v1                        = cms.vstring('? hasUserFloat("ditau2017v1") ? userFloat("ditau2017v1") : 0', 'F'),
    deepDiTau_ditau2017DMv1                      = cms.vstring('? hasUserFloat("ditau2017DMv1") ? userFloat("ditau2017DMv1") : 0', 'F'),
)

jetTruths = [
    'isTau','isTauTau',
    'isB','isBB',
    'isC','isCC',
    'isG','isS','isUD',
    #'isGPhys','isSPhys','isUDPhys',
]
tauModes = ['TauE','TauM','TauH']
if False: tauModes += ['TauDM0','TauDM1','TauDM10']
for i, tmi in enumerate(tauModes):
    jetTruths += ['is{}'.format(tmi)]
    for j, tmj in enumerate(tauModes):
        if j<i: continue
        jetTruths += ['is{}{}'.format(tmi,tmj)]
        
for truth in jetTruths:
    setattr(jetBranches,truth,cms.vstring('userInt("{}")'.format(truth),'I'))

# mets
metBranches = commonMet.clone(
    # covariance
    cov00                 = cms.vstring('getSignificanceMatrix[0][0]','F'),
    cov01                 = cms.vstring('getSignificanceMatrix[0][1]','F'),
    cov10                 = cms.vstring('getSignificanceMatrix[1][0]','F'),
    cov11                 = cms.vstring('getSignificanceMatrix[1][1]','F'),
    # uncor
    uncorEt               = cms.vstring('uncorPt','F'),
    uncorPhi              = cms.vstring('uncorPhi','F'),
    # shifts
    et_jetResUp           = cms.vstring('? hasUserCand("JetResUp") ? userCand("JetResUp").pt() : 0','F'),
    et_jetResDown         = cms.vstring('? hasUserCand("JetResDown") ? userCand("JetResDown").pt() : 0','F'),
    et_jetEnUp            = cms.vstring('? hasUserCand("JetEnUp") ? userCand("JetEnUp").pt() : 0','F'),
    et_jetEnDown          = cms.vstring('? hasUserCand("JetEnDown") ? userCand("JetEnDown").pt() : 0','F'),
    et_muonEnUp           = cms.vstring('? hasUserCand("MuonEnUp") ? userCand("MuonEnUp").pt() : 0','F'),
    et_muonEnDown         = cms.vstring('? hasUserCand("MuonEnDown") ? userCand("MuonEnDown").pt() : 0','F'),
    et_electronEnUp       = cms.vstring('? hasUserCand("ElectronEnUp") ? userCand("ElectronEnUp").pt() : 0','F'),
    et_electronEnDown     = cms.vstring('? hasUserCand("ElectronEnDown") ? userCand("ElectronEnDown").pt() : 0','F'),
    et_tauEnUp            = cms.vstring('? hasUserCand("TauEnUp") ? userCand("TauEnUp").pt() : 0','F'),
    et_tauEnDown          = cms.vstring('? hasUserCand("TauEnDown") ? userCand("TauEnDown").pt() : 0','F'),
    et_unclusteredEnUp    = cms.vstring('? hasUserCand("UnclusteredEnUp") ? userCand("UnclusteredEnUp").pt() : 0','F'),
    et_unclusteredEnDown  = cms.vstring('? hasUserCand("UnclusteredEnDown") ? userCand("UnclusteredEnDown").pt() : 0','F'),
    #et_photonEnUp         = cms.vstring('? hasUserCand("PhotonEnUp") ? userCand("PhotonEnUp").pt() : 0','F'),
    #et_photonEnDown       = cms.vstring('? hasUserCand("PhotonEnDown") ? userCand("PhotonEnDown").pt() : 0','F'),
    phi_jetResUp          = cms.vstring('? hasUserCand("JetResUp") ? userCand("JetResUp").phi() : 0','F'),
    phi_jetResDown        = cms.vstring('? hasUserCand("JetResDown") ? userCand("JetResDown").phi() : 0','F'),
    phi_jetEnUp           = cms.vstring('? hasUserCand("JetEnUp") ? userCand("JetEnUp").phi() : 0','F'),
    phi_jetEnDown         = cms.vstring('? hasUserCand("JetEnDown") ? userCand("JetEnDown").phi() : 0','F'),
    phi_muonEnUp          = cms.vstring('? hasUserCand("MuonEnUp") ? userCand("MuonEnUp").phi() : 0','F'),
    phi_muonEnDown        = cms.vstring('? hasUserCand("MuonEnDown") ? userCand("MuonEnDown").phi() : 0','F'),
    phi_electronEnUp      = cms.vstring('? hasUserCand("ElectronEnUp") ? userCand("ElectronEnUp").phi() : 0','F'),
    phi_electronEnDown    = cms.vstring('? hasUserCand("ElectronEnDown") ? userCand("ElectronEnDown").phi() : 0','F'),
    phi_tauEnUp           = cms.vstring('? hasUserCand("TauEnUp") ? userCand("TauEnUp").phi() : 0','F'),
    phi_tauEnDown         = cms.vstring('? hasUserCand("TauEnDown") ? userCand("TauEnDown").phi() : 0','F'),
    phi_unclusteredEnUp   = cms.vstring('? hasUserCand("UnclusteredEnUp") ? userCand("UnclusteredEnUp").phi() : 0','F'),
    phi_unclusteredEnDown = cms.vstring('? hasUserCand("UnclusteredEnDown") ? userCand("UnclusteredEnDown").phi() : 0','F'),
    #phi_photonEnUp        = cms.vstring('? hasUserCand("PhotonEnUp") ? userCand("PhotonEnUp").phi() : 0','F'),
    #phi_photonEnDown      = cms.vstring('? hasUserCand("PhotonEnDown") ? userCand("PhotonEnDown").phi() : 0','F'),
)

for trigger in triggerMap:
    if 'electron' in triggerMap[trigger]['objects']: setattr(electronBranches,'matches_{0}'.format(trigger),cms.vstring('userInt("matches_{0}")'.format(trigger),'I'))
    if 'muon' in triggerMap[trigger]['objects']:     setattr(muonBranches,    'matches_{0}'.format(trigger),cms.vstring('userInt("matches_{0}")'.format(trigger),'I'))
    if 'tau' in triggerMap[trigger]['objects']:      setattr(tauBranches,     'matches_{0}'.format(trigger),cms.vstring('userInt("matches_{0}")'.format(trigger),'I'))
    if 'photon' in triggerMap[trigger]['objects']:   setattr(photonBranches,  'matches_{0}'.format(trigger),cms.vstring('userInt("matches_{0}")'.format(trigger),'I'))
    if 'jet' in triggerMap[trigger]['objects']:      setattr(jetBranches,     'matches_{0}'.format(trigger),cms.vstring('userInt("matches_{0}")'.format(trigger),'I'))
