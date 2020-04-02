#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/PatCandidates/interface/Electron.h"


class ElectronMissingHitsEmbedder : public edm::stream::EDProducer<> {
  public:
    ElectronMissingHitsEmbedder(const edm::ParameterSet& pset);
    virtual ~ElectronMissingHitsEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::EDGetTokenT<edm::View<pat::Electron> > srcToken_;
};

ElectronMissingHitsEmbedder::ElectronMissingHitsEmbedder(const edm::ParameterSet& pset):
  srcToken_(consumes<edm::View<pat::Electron> >(pset.getParameter<edm::InputTag>("src")))
{
  produces<pat::ElectronCollection>();
}

void ElectronMissingHitsEmbedder::produce(edm::Event& evt, const edm::EventSetup& es) {
  std::unique_ptr<pat::ElectronCollection> output(new pat::ElectronCollection);

  edm::Handle<edm::View<pat::Electron> > input;
  evt.getByToken(srcToken_, input);

  output->reserve(input->size());
  for (size_t i = 0; i < input->size(); ++i) {
    pat::Electron electron = input->at(i);

    int missingHits = electron.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS);

    electron.addUserInt("missingHits", missingHits);
    output->push_back(electron);
  }

  evt.put(std::move(output));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ElectronMissingHitsEmbedder);
