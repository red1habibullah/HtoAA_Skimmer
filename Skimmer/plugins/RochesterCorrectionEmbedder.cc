// RochesterCorrectionEmbedder.cc

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"

//#include "DevTools/Ntuplizer/plugins/RoccoR.h"
#include "HtoAA_Skimmer/Skimmer/plugins/RoccoR.h"

#include "TLorentzVector.h"

class RochesterCorrectionEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit RochesterCorrectionEmbedder(const edm::ParameterSet&);
  ~RochesterCorrectionEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  // Data
  edm::EDGetTokenT<edm::View<pat::Muon> > collectionToken_; // input collection
  edm::FileInPath directory_;
  bool isData_;
  std::unique_ptr<std::vector<pat::Muon> > out;             // Collection we'll output at the end
  std::unique_ptr<RoccoR> rc;
};

// Constructors and destructors
RochesterCorrectionEmbedder::RochesterCorrectionEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<pat::Muon> >(iConfig.getParameter<edm::InputTag>("src"))),
  directory_(iConfig.getParameter<edm::FileInPath>("directory")),
  isData_(iConfig.getParameter<bool>("isData"))
{
  std::string rochCorrDataDirPath = directory_.fullPath();
  rochCorrDataDirPath.erase(rochCorrDataDirPath.length()-10);
  rc = std::unique_ptr<RoccoR>(new RoccoR(rochCorrDataDirPath));
  produces<std::vector<pat::Muon> >();
}

void RochesterCorrectionEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<pat::Muon> >(new std::vector<pat::Muon>);

  edm::Handle<edm::View<pat::Muon> > collection;
  iEvent.getByToken(collectionToken_, collection);

   // get random number generator
   edm::Service<edm::RandomNumberGenerator> rng;
   CLHEP::HepRandomEngine& engine = rng->getEngine(iEvent.streamID());

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    pat::Muon newObj = obj;

    //std::cout << "muon " << c << " " << obj.pt() << " " << obj.eta() << " " << obj.phi() << " " << obj.mass() << std::endl;
    
    if (!obj.mass() || !obj.pt()) { // something breaks when mass = 0 or pt = 0, just dont run
      newObj.addUserFloat("rochesterPt", obj.pt());
      newObj.addUserFloat("rochesterEta", obj.eta());
      newObj.addUserFloat("rochesterPhi", obj.phi());
      newObj.addUserFloat("rochesterEnergy", obj.energy());
      out->push_back(newObj);
      continue;
    }

    double sf = 1.0;
    int charge = obj.charge();
    double pt = obj.pt();
    double eta = obj.eta();
    double phi = obj.phi();
    if (isData_) {
      //for each data muon in the loop, use this function to get a scale factor for its momentum:
      sf = rc->kScaleDT(charge, pt, eta, phi, 0, 0);
    }
    else {
      //for MC, if matched gen-level muon (genPt) is available, use this function
      //sf = rc->kScaleFromGenMC(charge, pt, eta, phi, nl, genPt, u1, 0, 0);
    
      //if not, then:
      double u1 = engine.flat();
      double u2 = engine.flat();
      sf = rc->kScaleAndSmearMC(charge, pt, eta, phi, obj.bestTrack()->hitPattern().trackerLayersWithMeasurement(), u1, u2, 0, 0);
    }

    float sf_f = (float) sf;

    TLorentzVector p4;
    p4.SetPtEtaPhiM(std::max(0.0001,obj.pt()*sf_f),obj.eta(),obj.phi(),obj.mass()); //protect against negative pt from corrections in misreconstructed muons
    //std::cout << "scaled muon " << c << " " << p4.Pt() << " " << p4.Eta() << " " << p4.Phi() << " " << p4.Energy() << std::endl;

    newObj.addUserFloat("rochesterPt", p4.Pt());
    newObj.addUserFloat("rochesterEta", p4.Eta());
    newObj.addUserFloat("rochesterPhi", p4.Phi());
    newObj.addUserFloat("rochesterEnergy", p4.Energy());
    out->push_back(newObj);

    
  }

  iEvent.put(std::move(out));
}

void RochesterCorrectionEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(RochesterCorrectionEmbedder);
