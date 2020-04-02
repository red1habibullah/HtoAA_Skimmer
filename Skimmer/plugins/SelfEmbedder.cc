#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

template<typename T>
class SelfEmbedder : public edm::stream::EDProducer<> {
  public:
    SelfEmbedder(const edm::ParameterSet& pset);
    virtual ~SelfEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::EDGetTokenT<edm::View<T> > srcToken_;
    std::string label_;
};

template<typename T>
SelfEmbedder<T>::SelfEmbedder(const edm::ParameterSet& pset):
  srcToken_(consumes<edm::View<T> >(pset.getParameter<edm::InputTag>("src"))),
  label_(pset.getParameter<std::string>("label"))
{
  produces<std::vector<T> >();
}

template<typename T>
void SelfEmbedder<T>::produce(edm::Event& evt, const edm::EventSetup& es) {
  std::unique_ptr<std::vector<T> > output = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > input;
  evt.getByToken(srcToken_, input);

  output->reserve(input->size());
  for (size_t i = 0; i < input->size(); ++i) {
    T obj = input->at(i);

    double pt = obj.pt();
    double eta = obj.eta();
    double phi = obj.phi();
    double energy = obj.energy();

    obj.addUserFloat(label_+"Pt", pt);
    obj.addUserFloat(label_+"Eta", eta);
    obj.addUserFloat(label_+"Phi", phi);
    obj.addUserFloat(label_+"Energy", energy);
    obj.addUserCand(label_, input->ptrAt(i));
    output->push_back(obj);
  }

  evt.put(std::move(output));
}

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef SelfEmbedder<pat::Muon> MuonSelfEmbedder;
typedef SelfEmbedder<pat::Electron> ElectronSelfEmbedder;
typedef SelfEmbedder<pat::Tau> TauSelfEmbedder;
typedef SelfEmbedder<pat::Jet> JetSelfEmbedder;
typedef SelfEmbedder<pat::Photon> PhotonSelfEmbedder;
typedef SelfEmbedder<pat::MET> METSelfEmbedder;

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MuonSelfEmbedder);
DEFINE_FWK_MODULE(ElectronSelfEmbedder);
DEFINE_FWK_MODULE(TauSelfEmbedder);
DEFINE_FWK_MODULE(JetSelfEmbedder);
DEFINE_FWK_MODULE(PhotonSelfEmbedder);
DEFINE_FWK_MODULE(METSelfEmbedder);
