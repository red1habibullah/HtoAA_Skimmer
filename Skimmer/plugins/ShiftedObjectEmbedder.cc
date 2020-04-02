// ShiftedObjectEmbedder.cc
// Embed shifts for each event as a user cand

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"

template<typename T>
class ShiftedObjectEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit ShiftedObjectEmbedder(const edm::ParameterSet&);
  ~ShiftedObjectEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  // Data
  edm::EDGetTokenT<edm::View<T> > srcToken_;        // input collection
  edm::EDGetTokenT<edm::View<reco::Candidate> > shiftedSrcToken_; // shifted collection
  std::string label_;                               // label for embedding
  std::unique_ptr<std::vector<T> > out;               // Collection we'll output at the end
};

// Constructors and destructors
template<typename T>
ShiftedObjectEmbedder<T>::ShiftedObjectEmbedder(const edm::ParameterSet& iConfig):
  srcToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  shiftedSrcToken_(consumes<edm::View<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("shiftedSrc"))),
  label_(iConfig.getParameter<std::string>("label"))
{
  produces<std::vector<T> >();
}

template<typename T>
void ShiftedObjectEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > src;
  iEvent.getByToken(srcToken_, src);

  edm::Handle<edm::View<reco::Candidate> > shiftedSrc;
  iEvent.getByToken(shiftedSrcToken_, shiftedSrc);

  for (size_t o = 0; o < src->size(); ++o) {
    const auto obj = src->at(o);
    T newObj = obj;

    if (o<shiftedSrc->size()){
      reco::CandidatePtr shiftObj = shiftedSrc->ptrAt(o);
      newObj.addUserCand(label_, shiftObj);
    }
    // Don't add nulll ptr, just check later if userCand exists
    //else {
    //  newObj.addUserCand(label_, edm::Ptr<T>());
    //}

    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<typename T>
void ShiftedObjectEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef ShiftedObjectEmbedder<pat::Muon> ShiftedMuonEmbedder;
typedef ShiftedObjectEmbedder<pat::Electron> ShiftedElectronEmbedder;
typedef ShiftedObjectEmbedder<pat::Tau> ShiftedTauEmbedder;
typedef ShiftedObjectEmbedder<pat::Jet> ShiftedJetEmbedder;
typedef ShiftedObjectEmbedder<pat::Photon> ShiftedPhotonEmbedder;
typedef ShiftedObjectEmbedder<pat::MET> ShiftedMETEmbedder;

//define this as a plug-in
DEFINE_FWK_MODULE(ShiftedMuonEmbedder);
DEFINE_FWK_MODULE(ShiftedElectronEmbedder);
DEFINE_FWK_MODULE(ShiftedTauEmbedder);
DEFINE_FWK_MODULE(ShiftedJetEmbedder);
DEFINE_FWK_MODULE(ShiftedPhotonEmbedder);
DEFINE_FWK_MODULE(ShiftedMETEmbedder);
