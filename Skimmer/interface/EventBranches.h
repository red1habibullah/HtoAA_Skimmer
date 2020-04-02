#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"

class EventBranches {
  public:
    EventBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc);
    void fill(const edm::Event& iEvent);

  private:
    // branches
    Int_t     runBranch_;
    Int_t     lumiBranch_;
    ULong64_t eventBranch_;
    std::vector<std::string> provenanceBranch_;

};
