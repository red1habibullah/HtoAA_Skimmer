// METShiftEmbedder.cc
// Embed met shifts for each event as a user float

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/MET.h"

class METShiftEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit METShiftEmbedder(const edm::ParameterSet&);
  ~METShiftEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  // Data
  edm::EDGetTokenT<edm::View<pat::MET> > metToken_; // input collection
  std::unique_ptr<std::vector<pat::MET> > out;        // Collection we'll output at the end
};

// Constructors and destructors
METShiftEmbedder::METShiftEmbedder(const edm::ParameterSet& iConfig):
  metToken_(consumes<edm::View<pat::MET> >(iConfig.getParameter<edm::InputTag>("src")))
{
  produces<std::vector<pat::MET> >();
}

void METShiftEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<pat::MET> >(new std::vector<pat::MET>);

  edm::Handle<edm::View<pat::MET> > met;
  iEvent.getByToken(metToken_, met);

  for (size_t m = 0; m < met->size(); ++m) {
    const auto obj = met->at(m);
    pat::MET newObj = obj;

    newObj.addUserFloat("shiftedPtJetResUp",          obj.shiftedPt(pat::MET::JetResUp));
    newObj.addUserFloat("shiftedPtJetResDown",        obj.shiftedPt(pat::MET::JetResDown));
    newObj.addUserFloat("shiftedPtJetEnUp",           obj.shiftedPt(pat::MET::JetEnUp));
    newObj.addUserFloat("shiftedPtJetEnDown",         obj.shiftedPt(pat::MET::JetEnDown));
    newObj.addUserFloat("shiftedPtMuonEnUp",          obj.shiftedPt(pat::MET::MuonEnUp));
    newObj.addUserFloat("shiftedPtMuonEnDown",        obj.shiftedPt(pat::MET::MuonEnDown));
    newObj.addUserFloat("shiftedPtElectronEnUp",      obj.shiftedPt(pat::MET::ElectronEnUp));
    newObj.addUserFloat("shiftedPtElectronEnDown",    obj.shiftedPt(pat::MET::ElectronEnDown));
    newObj.addUserFloat("shiftedPtTauEnUp",           obj.shiftedPt(pat::MET::TauEnUp));
    newObj.addUserFloat("shiftedPtTauEnDown",         obj.shiftedPt(pat::MET::TauEnDown));
    newObj.addUserFloat("shiftedPtUnclusteredEnUp",   obj.shiftedPt(pat::MET::UnclusteredEnUp));
    newObj.addUserFloat("shiftedPtUnclusteredEnDown", obj.shiftedPt(pat::MET::UnclusteredEnDown));
    newObj.addUserFloat("shiftedPtPhotonEnUp",        obj.shiftedPt(pat::MET::PhotonEnUp));
    newObj.addUserFloat("shiftedPtPhotonEnDown",      obj.shiftedPt(pat::MET::PhotonEnDown));

    newObj.addUserFloat("shiftedPhiJetResUp",          obj.shiftedPhi(pat::MET::JetResUp));
    newObj.addUserFloat("shiftedPhiJetResDown",        obj.shiftedPhi(pat::MET::JetResDown));
    newObj.addUserFloat("shiftedPhiJetEnUp",           obj.shiftedPhi(pat::MET::JetEnUp));
    newObj.addUserFloat("shiftedPhiJetEnDown",         obj.shiftedPhi(pat::MET::JetEnDown));
    newObj.addUserFloat("shiftedPhiMuonEnUp",          obj.shiftedPhi(pat::MET::MuonEnUp));
    newObj.addUserFloat("shiftedPhiMuonEnDown",        obj.shiftedPhi(pat::MET::MuonEnDown));
    newObj.addUserFloat("shiftedPhiElectronEnUp",      obj.shiftedPhi(pat::MET::ElectronEnUp));
    newObj.addUserFloat("shiftedPhiElectronEnDown",    obj.shiftedPhi(pat::MET::ElectronEnDown));
    newObj.addUserFloat("shiftedPhiTauEnUp",           obj.shiftedPhi(pat::MET::TauEnUp));
    newObj.addUserFloat("shiftedPhiTauEnDown",         obj.shiftedPhi(pat::MET::TauEnDown));
    newObj.addUserFloat("shiftedPhiUnclusteredEnUp",   obj.shiftedPhi(pat::MET::UnclusteredEnUp));
    newObj.addUserFloat("shiftedPhiUnclusteredEnDown", obj.shiftedPhi(pat::MET::UnclusteredEnDown));
    newObj.addUserFloat("shiftedPhiPhotonEnUp",        obj.shiftedPhi(pat::MET::PhotonEnUp));
    newObj.addUserFloat("shiftedPhiPhotonEnDown",      obj.shiftedPhi(pat::MET::PhotonEnDown));

    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

void METShiftEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METShiftEmbedder);
