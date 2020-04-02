#ifndef MiniTree_h
#define MiniTree_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "HtoAA_Skimmer/Skimmer/interface/CandidateCollectionBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/VertexCollectionBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/LumiSummaryBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/EventBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/MonteCarloBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/RhoBranches.h"
#include "HtoAA_Skimmer/Skimmer/interface/TriggerBranches.h"

class MiniTree : public edm::one::EDAnalyzer<edm::one::SharedResources,edm::one::WatchLuminosityBlocks> {
  public:
    explicit MiniTree(const edm::ParameterSet&);
    ~MiniTree();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual void beginLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const&) override;
    virtual void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
    virtual void endLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const&) override;
    virtual void endJob() override;

    edm::ParameterSet collections_;
    edm::ParameterSet vertexCollections_;

    // other configurations
    bool isData_;

    // trees
    TTree *tree_;
    TTree *lumitree_;

    // lumi summary
    std::unique_ptr<LumiSummaryBranches> lumiSummary_;

    // one off tree branches
    Int_t                 isDataBranch_;

    // event
    std::unique_ptr<EventBranches> event_;

    // monte carlo info
    std::unique_ptr<MonteCarloBranches> mcBranches_;

    // rho
    std::unique_ptr<RhoBranches> rho_;

    // trigger
    std::unique_ptr<TriggerBranches> trigger_;

    // collections
    std::vector<std::unique_ptr<CandidateCollectionBranches> > collectionBranches_;
    std::vector<std::unique_ptr<JetCandidateCollectionBranches> > jetCollectionBranches_;
    std::vector<std::unique_ptr<VertexCollectionBranches> > vertexCollectionBranches_;
};

void MiniTree::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


DEFINE_FWK_MODULE(MiniTree);

#endif
