#include "HtoAA_Skimmer/Skimmer/interface/GenMiniTree.h"

GenMiniTree::GenMiniTree(const edm::ParameterSet &iConfig) :
    lheEventProductToken_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lheEventProduct"))),
    genEventInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfo"))),
    pileupSummaryInfoToken_(consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileupSummaryInfo"))),
    collections_(iConfig.getParameter<edm::ParameterSet>("collections"))
{
    // Declare use of TFileService
    usesResource("TFileService");

    // now build the branches

    edm::Service<TFileService> FS;

    // create lumitree_
    lumitree_ = FS->make<TTree>("LumiTree", "LumiTree");
    lumiSummary_ = std::unique_ptr<LumiSummaryBranches>(new LumiSummaryBranches(lumitree_, iConfig, consumesCollector()));

    // create tree_
    tree_ = FS->make<TTree>("GenMiniTree", "GenMiniTree");

    // now build the branches
    // event number
    event_ = std::unique_ptr<EventBranches>(new EventBranches(tree_, iConfig, consumesCollector()));

    // is data
    tree_->Branch("isData", &isDataBranch_, "isData/I");

    // mc info
    mcBranches_ = std::unique_ptr<MonteCarloBranches>(new MonteCarloBranches(tree_, iConfig, consumesCollector()));

    // add collections
    auto collectionNames_ = collections_.getParameterNames();
    for (auto coll : collectionNames_) {
        collectionBranches_.emplace_back(new CandidateCollectionBranches(tree_, coll, collections_.getParameter<edm::ParameterSet>(coll), consumesCollector()));
    }
}

GenMiniTree::~GenMiniTree() { }

void GenMiniTree::beginJob() { }

void GenMiniTree::beginLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const& iSetup) {
    lumiSummary_->beginLumi(iEvent);
}

void GenMiniTree::endLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const& iSetup) {
    lumitree_->Fill();
}

void GenMiniTree::endJob() { }

void GenMiniTree::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    // first, the lumitree_
    lumiSummary_->fill(iEvent);

    // now the actual tree_
    isDataBranch_ = isData_;
    event_->fill(iEvent);
    mcBranches_->fill(iEvent);

    // add collections
    for ( auto& coll : collectionBranches_ ) {
        coll->fill(iEvent);
    }

    // decide if we store it
    // for now, keep everything
    bool keep = true;

    if (keep)
        tree_->Fill();
}

