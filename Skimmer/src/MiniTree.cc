#include "HtoAA_Skimmer/Skimmer/interface/MiniTree.h"

MiniTree::MiniTree(const edm::ParameterSet &iConfig) :
    collections_(iConfig.getParameter<edm::ParameterSet>("collections")),
    vertexCollections_(iConfig.getParameter<edm::ParameterSet>("vertexCollections")),
    isData_(iConfig.getParameter<bool>("isData"))
{
    // Declare use of TFileService
    usesResource("TFileService");

    edm::Service<TFileService> FS;

    // create lumitree_
    lumitree_ = FS->make<TTree>("LumiTree", "LumiTree");
    lumiSummary_ = std::unique_ptr<LumiSummaryBranches>(new LumiSummaryBranches(lumitree_, iConfig, consumesCollector()));

    // create tree_
    tree_ = FS->make<TTree>("MiniTree", "MiniTree");

    // now build the branches
    // event number
    event_ = std::unique_ptr<EventBranches>(new EventBranches(tree_, iConfig, consumesCollector()));

    // is data
    tree_->Branch("isData", &isDataBranch_, "isData/I");

    // mc info
    mcBranches_ = std::unique_ptr<MonteCarloBranches>(new MonteCarloBranches(tree_, iConfig, consumesCollector()));

    // rho
    rho_ = std::unique_ptr<RhoBranches>(new RhoBranches(tree_, iConfig, consumesCollector()));

    // trigger
    trigger_ = std::unique_ptr<TriggerBranches>(new TriggerBranches(tree_, iConfig, consumesCollector()));

    // add vertices
    auto vertexCollectionNames_ = vertexCollections_.getParameterNames();
    for (auto coll : vertexCollectionNames_) {
        vertexCollectionBranches_.emplace_back(new VertexCollectionBranches(tree_, coll, vertexCollections_.getParameter<edm::ParameterSet>(coll), consumesCollector()));
    }

    // add collections
    auto collectionNames_ = collections_.getParameterNames();
    for (auto coll : collectionNames_) {
        edm::ParameterSet pset = collections_.getParameter<edm::ParameterSet>(coll);
        if (pset.exists("constituentBranches")) {
            jetCollectionBranches_.emplace_back(new JetCandidateCollectionBranches(tree_, coll, "constituents", collections_.getParameter<edm::ParameterSet>(coll), consumesCollector()));
        }
        else {
            collectionBranches_.emplace_back(new CandidateCollectionBranches(tree_, coll, collections_.getParameter<edm::ParameterSet>(coll), consumesCollector()));
        }
    }
}

MiniTree::~MiniTree() { }

void MiniTree::beginJob() { }

void MiniTree::beginLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const& iSetup) {
    lumiSummary_->beginLumi(iEvent);
}

void MiniTree::endLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const& iSetup) {
    lumitree_->Fill();
}

void MiniTree::endJob() { }

void MiniTree::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    // first, the lumitree_
    lumiSummary_->fill(iEvent);

    // now the actual tree_
    isDataBranch_ = isData_;
    event_->fill(iEvent);
    mcBranches_->fill(iEvent);
    rho_->fill(iEvent);
    trigger_->fill(iEvent);

    // add vertices
    for ( auto& coll : vertexCollectionBranches_ ) {
        coll->fill(iEvent);
    }

    // add collections
    for ( auto& coll : collectionBranches_ ) {
        coll->fill(iEvent);
    }

    for ( auto& coll : jetCollectionBranches_ ) {
        coll->fill(iEvent);
    }

    // decide if we store it
    bool keep = false;
    for ( auto& coll : collectionBranches_ ) {
        keep = keep || coll->keep();
    }

    for ( auto& coll : jetCollectionBranches_ ) {
        keep = keep || coll->keep();
    }

    if (keep)
        tree_->Fill();
}
