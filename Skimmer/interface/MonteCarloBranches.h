#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TTree.h"

class MonteCarloBranches {
  public:
    MonteCarloBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc);
    void fill(const edm::Event& iEvent);

  private:
    // branches
    Float_t               genWeightBranch_;
    Int_t                 numWeightsBranch_;
    std::vector<Float_t>  genWeightsBranch_;
    Float_t               ptHatBranch_;
    Float_t               nTrueVerticesBranch_;
    Int_t                 nObservedVerticesBranch_;
    Int_t                 nupBranch_;
    Int_t                 numGenJetsBranch_;
    Float_t               genHTBranch_;

    // tokens
    edm::EDGetTokenT<LHEEventProduct> lheEventProductToken_;
    edm::EDGetTokenT<GenEventInfoProduct> genEventInfoToken_;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> > pileupSummaryInfoToken_;
};
