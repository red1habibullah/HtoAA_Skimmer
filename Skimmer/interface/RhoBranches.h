#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"

class RhoBranches {
  public:
    RhoBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc);
    void fill(const edm::Event& iEvent);

  private:
    // branches
    Float_t rhoBranch_;

    // tokens
    edm::EDGetTokenT<double> rhoToken_;
};
