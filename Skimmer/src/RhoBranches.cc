#include "HtoAA_Skimmer/Skimmer/interface/RhoBranches.h"

RhoBranches::RhoBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
    rhoToken_(cc.consumes<double>(iConfig.getParameter<edm::InputTag>("rho")))
{
  // add branches
  tree->Branch("rho", &rhoBranch_, "rho/F");
}

void RhoBranches::fill(const edm::Event& iEvent)
{
    edm::Handle<double> rho;
    iEvent.getByToken(rhoToken_,rho);

    rhoBranch_ = *rho;
}

