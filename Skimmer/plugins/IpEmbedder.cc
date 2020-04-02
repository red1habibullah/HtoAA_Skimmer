// IpEmbedder.cc
// Embed the rho for each event as a user float

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef math::XYZPoint Point;

template<typename T>
class IpEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit IpEmbedder(const edm::ParameterSet&);
  ~IpEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  double dz(T obj, const Point&) { return 0.; }
  double dxy(T obj, const Point&) { return 0.; }
  double dB2D(T obj) { return 0.; }
  double dB3D(T obj) { return 0.; }
  double edB2D(T obj) { return 0.; }
  double edB3D(T obj) { return 0.; }

  // Data
  edm::EDGetTokenT<edm::View<T> > collectionToken_;      // input collection
  edm::EDGetTokenT<reco::VertexCollection> vertexToken_; // pv collection
  edm::EDGetTokenT<reco::BeamSpot> beamspotToken_;       // the beamspot
  std::unique_ptr<std::vector<T> > out;                    // Collection we'll output at the end
};

// Constructors and destructors
template<typename T>
IpEmbedder<T>::IpEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  vertexToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  beamspotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamspotSrc")))
{
  produces<std::vector<T> >();
}

template<typename T>
void IpEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vertexToken_, vertices);

  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByToken(beamspotToken_, beamspot);

  const reco::Vertex& pv = *vertices->begin();

  Point zero = Point(0,0,0);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;

    newObj.addUserFloat("dz", dz(obj, pv.position()));
    newObj.addUserFloat("dxy", dxy(obj, pv.position()));
    newObj.addUserFloat("dz_beamspot", dz(obj, beamspot->position()));
    newObj.addUserFloat("dxy_beamspot", dxy(obj, beamspot->position()));
    newObj.addUserFloat("dz_zero", dz(obj, zero));
    newObj.addUserFloat("dxy_zero", dxy(obj, zero));
    newObj.addUserFloat("dB2D", dB2D(obj));
    newObj.addUserFloat("dB3D", dB3D(obj));
    newObj.addUserFloat("edB2D", edB2D(obj));
    newObj.addUserFloat("edB3D", edB3D(obj));
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<>
double IpEmbedder<pat::Electron>::dz(pat::Electron obj, const Point& p) {
  return obj.gsfTrack()->dz(p);
}

template<>
double IpEmbedder<pat::Muon>::dz(pat::Muon obj, const Point& p) {
  return obj.muonBestTrack()->dz(p);
}

template<>
double IpEmbedder<pat::Tau>::dz(pat::Tau obj, const Point& p) {
  try {
    pat::PackedCandidate const* packedLeadTauCand = dynamic_cast<pat::PackedCandidate const*>(obj.leadChargedHadrCand().get());
    return packedLeadTauCand->dz(p);
  }
  catch (const std::exception &e) {
    return 0.;
  }
}

template<>
double IpEmbedder<pat::Electron>::dxy(pat::Electron obj, const Point& p) {
  return obj.gsfTrack()->dxy(p);
}

template<>
double IpEmbedder<pat::Muon>::dxy(pat::Muon obj, const Point& p) {
  return obj.muonBestTrack()->dxy(p);
}

template<>
double IpEmbedder<pat::Tau>::dxy(pat::Tau obj, const Point& p) {
  try {
    pat::PackedCandidate const* packedLeadTauCand = dynamic_cast<pat::PackedCandidate const*>(obj.leadChargedHadrCand().get());
    return packedLeadTauCand->dxy(p);
  }
  catch (const std::exception &e) {
    return 0.;
  }
}

template<>
double IpEmbedder<pat::Electron>::dB2D(pat::Electron obj) {
    return obj.dB(pat::Electron::PV2D);
}

template<>
double IpEmbedder<pat::Muon>::dB2D(pat::Muon obj) {
    return obj.dB(pat::Muon::PV2D);
}

template<>
double IpEmbedder<pat::Electron>::dB3D(pat::Electron obj) {
    return obj.dB(pat::Electron::PV3D);
}

template<>
double IpEmbedder<pat::Muon>::dB3D(pat::Muon obj) {
    return obj.dB(pat::Muon::PV3D);
}

template<>
double IpEmbedder<pat::Electron>::edB2D(pat::Electron obj) {
    return obj.edB(pat::Electron::PV2D);
}

template<>
double IpEmbedder<pat::Muon>::edB2D(pat::Muon obj) {
    return obj.edB(pat::Muon::PV2D);
}

template<>
double IpEmbedder<pat::Electron>::edB3D(pat::Electron obj) {
    return obj.edB(pat::Electron::PV3D);
}

template<>
double IpEmbedder<pat::Muon>::edB3D(pat::Muon obj) {
    return obj.edB(pat::Muon::PV3D);
}

template<typename T>
void IpEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
typedef IpEmbedder<pat::Electron> ElectronIpEmbedder;
typedef IpEmbedder<pat::Muon> MuonIpEmbedder;
typedef IpEmbedder<pat::Tau> TauIpEmbedder;

DEFINE_FWK_MODULE(ElectronIpEmbedder);
DEFINE_FWK_MODULE(MuonIpEmbedder);
DEFINE_FWK_MODULE(TauIpEmbedder);
