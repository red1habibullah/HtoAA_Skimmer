#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/Math/interface/deltaR.h"

template<typename T>
class GenJetEmbedder : public edm::stream::EDProducer<> {
  public:
    GenJetEmbedder(const edm::ParameterSet& pset);
    virtual ~GenJetEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::EDGetTokenT<edm::View<T> > srcToken_;
    edm::EDGetTokenT<edm::View<reco::GenJet> > genJetToken_;
    std::unique_ptr<std::vector<T> > out;
    double deltaR_;
    bool excludeLeptons_;
};

template<typename T>
GenJetEmbedder<T>::GenJetEmbedder(const edm::ParameterSet& pset):
  srcToken_(consumes<edm::View<T> >(pset.getParameter<edm::InputTag>("src"))),
  genJetToken_(consumes<edm::View<reco::GenJet> >(pset.getParameter<edm::InputTag>("genJets"))),
  deltaR_(pset.getParameter<double>("deltaR")),
  excludeLeptons_(pset.getParameter<bool>("excludeLeptons"))
{
  produces<std::vector<T> >();
}

template<typename T>
void GenJetEmbedder<T>::produce(edm::Event& evt, const edm::EventSetup& es) {
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > input;
  evt.getByToken(srcToken_, input);

  edm::Handle<edm::View<reco::GenJet> > genJets;
  evt.getByToken(genJetToken_, genJets);

  for (size_t i = 0; i < input->size(); ++i) {
    T obj = input->at(i);

    edm::Ptr<reco::GenJet> genJet;
    double closest = 999.;
    for (size_t j = 0; j < genJets->size(); ++j) {
      if (excludeLeptons_) {
        bool exclude = false;
        for (auto jetConstituent: genJets->at(j).getGenConstituents()) {
          if (abs(jetConstituent->pdgId())==11 
              || abs(jetConstituent->pdgId())==13
              || abs(jetConstituent->pdgId())==15) {
            exclude = true;
          }
        }
        if (exclude) continue;
      }
      double thisDR = reco::deltaR(obj,genJets->at(j));
      if ((thisDR < deltaR_) && (thisDR < closest)) {
        genJet = genJets->ptrAt(j);
        closest = thisDR;
      }
    }

    obj.addUserCand("genJet", genJet);
    out->push_back(obj);
  }

  evt.put(std::move(out));
}

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
typedef GenJetEmbedder<pat::Tau> TauGenJetEmbedder;
typedef GenJetEmbedder<pat::Jet> JetGenJetEmbedder;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauGenJetEmbedder);
DEFINE_FWK_MODULE(JetGenJetEmbedder);
