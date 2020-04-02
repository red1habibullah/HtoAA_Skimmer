# Setup parameters to validate ntuplizer

params = {
    'SingleMuon': {
        'type': 'data',
        'kwargs': {
            'inputFiles': '/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/284/035/00000/5849226D-569F-E611-B874-02163E011EAC.root',
            'isMC': 0,
            'numThreads': 4,
            'runH': 1,
        },
    },
    'DoubleMuon': {
        'type': 'data',
        'kwargs': {
            'inputFiles': '/store/data/Run2016H/DoubleMuon/AOD/PromptReco-v3/000/284/036/00000/001882B7-A39F-E611-B8DB-FA163E178004.root',
            'isMC': 0,
            'numThreads': 4,
            'runH': 1,
        },
    },
    'signal': {
        'type': 'mc',
        'kwargs': {
            'inputFiles': '/store/mc/RunIISummer16DR80Premix/SUSYGluGluToHToAA_AToMuMu_AToTauTau_M-15_TuneCUETP8M1_13TeV_madgraph_pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/1AE9BFA1-0BBB-E611-988C-00259019A43E.root',
            'isMC': 1,
            'numThreads': 4,
        },
    },
    'DY': {
        'type': 'mc',
        'kwargs': {
            'inputFiles': '/store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/001AC973-60E2-E611-B768-001E67586A2F.root',
            'isMC': 1,
            'numThreads': 4,
        },
    },
    'TTbar': {
        'type': 'mc',
        'kwargs': {
            'inputFiles': '/store/mc/RunIISummer16DR80Premix/TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/120001/9812BE1B-6E5B-E711-90CF-A0369F5BD91C.root',
            'isMC': 1,
            'numThreads': 4,
        },
    },
}
