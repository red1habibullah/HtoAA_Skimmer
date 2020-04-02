// RhoEmbedder.cc
// Embed the rho for each event as a user float

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

template<typename T>
class RhoEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit RhoEmbedder(const edm::ParameterSet&);
  ~RhoEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  // Data
  edm::EDGetTokenT<edm::View<T> > collectionToken_; // input collection
  edm::EDGetTokenT<double> rhoToken_;               // rho
  const std::string label_;                         // label for the embedded userfloat
  std::unique_ptr<std::vector<T> > out;               // Collection we'll output at the end
};

// Constructors and destructors
template<typename T>
RhoEmbedder<T>::RhoEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoSrc"))),
  label_(iConfig.exists("label") ? iConfig.getParameter<std::string>("label") : std::string("rho"))
{
  produces<std::vector<T> >();
}

template<typename T>
void RhoEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<double> rho;
  iEvent.getByToken(rhoToken_, rho);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;

    newObj.addUserFloat(label_, *rho);
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<typename T>
void RhoEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
typedef RhoEmbedder<pat::Electron> ElectronRhoEmbedder;
typedef RhoEmbedder<pat::Muon> MuonRhoEmbedder;
typedef RhoEmbedder<pat::Tau> TauRhoEmbedder;
typedef RhoEmbedder<pat::Photon> PhotonRhoEmbedder;
typedef RhoEmbedder<pat::Jet> JetRhoEmbedder;

DEFINE_FWK_MODULE(ElectronRhoEmbedder);
DEFINE_FWK_MODULE(MuonRhoEmbedder);
DEFINE_FWK_MODULE(TauRhoEmbedder);
DEFINE_FWK_MODULE(PhotonRhoEmbedder);
DEFINE_FWK_MODULE(JetRhoEmbedder);
