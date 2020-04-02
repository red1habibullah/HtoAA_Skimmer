#include "HtoAA_Skimmer/Skimmer/interface/EventBranches.h"

EventBranches::EventBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc)
{
  // add branches
  tree->Branch("run", &runBranch_, "run/I");
  tree->Branch("lumi", &lumiBranch_, "lumi/I");
  tree->Branch("event", &eventBranch_, "event/l");
  tree->Branch("provenance", &provenanceBranch_);
}

void EventBranches::fill(const edm::Event& iEvent)
{
  eventBranch_ = iEvent.id().event();
  runBranch_   = iEvent.run();
  lumiBranch_  = iEvent.luminosityBlock();
  provenanceBranch_.clear();
  for (auto i = iEvent.processHistory().rbegin(); i!=iEvent.processHistory().rend(); i++) {
    provenanceBranch_.push_back(i->releaseVersion());
  }
}

