#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TLorentzVector.h"

template<class T>
class JetEmbedder : public edm::stream::EDProducer<>
{
  public:
    explicit JetEmbedder(const edm::ParameterSet&);
    ~JetEmbedder() {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


  private:
    // Methods
    void beginJob() {}
    virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
    void endJob() {}

    edm::EDGetTokenT<edm::View<T> > collectionToken_;
    edm::EDGetTokenT<edm::View<pat::Jet> > jetSrcToken_;
    edm::EDGetTokenT<reco::JetCorrector> tagL1Corrector_;
    edm::EDGetTokenT<reco::JetCorrector> tagL1L2L3ResCorrector_;
    std::unique_ptr<std::vector<T> > out;
    double dRmax_;
};

// Constructors and destructors
template<class T>
JetEmbedder<T>::JetEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  jetSrcToken_(consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("jetSrc"))),
  tagL1Corrector_(consumes<reco::JetCorrector>(iConfig.getParameter<edm::InputTag>("L1Corrector"))),
  tagL1L2L3ResCorrector_(consumes<reco::JetCorrector>(iConfig.getParameter<edm::InputTag>("L1L2L3ResCorrector"))),
  dRmax_(iConfig.getParameter<double>("dRmax"))
{
  produces<std::vector<T> >();
}

template<class T>
void JetEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  bool verbose_ = false;

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByToken(jetSrcToken_, jets);

  edm::Handle<reco::JetCorrector> correctorL1L2L3Res;
  iEvent.getByToken(tagL1L2L3ResCorrector_, correctorL1L2L3Res);

  edm::Handle<reco::JetCorrector> correctorL1;
  iEvent.getByToken(tagL1Corrector_, correctorL1);

  if (verbose_) std::cout << "coll size: " << collection->size() << " jet size: " << jets->size() << std::endl;
  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;
    if (verbose_) std::cout << "  coll " << c << ": pt " << obj.pt() << " eta " << obj.eta() << " phi " << obj.phi() << std::endl;

    edm::Ptr<pat::Jet> closestJet;
    double closestDeltaR = std::numeric_limits<double>::infinity();

    for (size_t j = 0; j < jets->size(); ++j) {
      edm::Ptr<pat::Jet> jet = jets->ptrAt(j);
      if (verbose_) std::cout << "    jet " << j << ": pt " << jet->pt() << " eta " << jet->eta() << " phi " << jet->phi() << std::endl;
      double deltaR = reco::deltaR(obj,*jet);
      if (deltaR < closestDeltaR) {
        closestDeltaR = deltaR;
        closestJet = jet;
      }
    }
    if (verbose_) std::cout << "  closest dR " << closestDeltaR << std::endl;

    int nchdaughters = 0;
    float ptratio = 1;
    float ptrel = 0;
    float btagCSV = -1;
    float chMult = 0;

    if (closestJet.isNonnull()) {
      // https://github.com/cms-analysis/MuonAnalysis-TagAndProbe/blob/80X/plugins/AddLeptonJetRelatedVariables.cc
      if (verbose_) std::cout << "  number of daughters " << closestJet->numberOfDaughters() << std::endl;
      for (size_t jc = 0; jc < closestJet->numberOfDaughters(); ++jc) {
        const pat::PackedCandidate* pfcand = dynamic_cast<const pat::PackedCandidate*>(closestJet->daughter(jc));
        if (pfcand->charge()==0) continue;
        if (deltaR(obj,*pfcand) > 0.4) continue;	
        if (!pfcand->trackHighPurity()) continue;
        if (pfcand->pt()<1.) continue;
        if (pfcand->numberOfHits()<8) continue;
        if (pfcand->numberOfPixelHits()<2) continue;
        if (pfcand->pseudoTrack().normalizedChi2()>=5) continue;
        if (std::fabs(pfcand->dxy()) > 0.2) continue;
        if (std::fabs(pfcand->dz()) > 17) continue;
        nchdaughters++;
      }
      if (verbose_) std::cout << "  number of charged daughters " << nchdaughters << std::endl;

      if (verbose_) std::cout << "  lep aware jec" << std::endl;
      reco::Candidate::LorentzVector jet, lep;
      lep = obj.p4();
      jet = closestJet->correctedP4(0);

      double jecL1L2L3Res = correctorL1L2L3Res->correction(*closestJet);
      double jecL1 = correctorL1->correction(*closestJet);

      if ((jet-lep).Rho()<0.0001) 
        jet=lep; 
      else {
        jet -= lep/jecL1;
        jet *= jecL1L2L3Res;
        jet += lep;
      }
      ptratio = lep.pt()/jet.pt();
      if (verbose_) std::cout << "  pt ratio " << ptratio << std::endl;
      TLorentzVector tmp_lep, tmp_jet;
      tmp_lep.SetPxPyPzE(lep.px(),lep.py(),lep.pz(),lep.E());
      tmp_jet.SetPxPyPzE(jet.px(),jet.py(),jet.pz(),jet.E());
      if ((tmp_jet-tmp_lep).Rho()>=0.0001) ptrel = tmp_lep.Perp((tmp_jet-tmp_lep).Vect());
      if (verbose_) std::cout << "  ptrel " << ptrel << std::endl;

      btagCSV = closestJet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      if (verbose_) std::cout << "  btag " << btagCSV << std::endl;

      chMult = closestJet->chargedHadronMultiplicity();
    }

    newObj.addUserCand("jet", closestJet);
    newObj.addUserInt("jet_chargedHadronMultiplicity", chMult);
    newObj.addUserInt("jet_numberOfChargedDaughters", nchdaughters);
    newObj.addUserFloat("jet_ptRatio", ptratio);
    newObj.addUserFloat("jet_ptRel", ptrel);
    newObj.addUserFloat("jet_pfCombinedInclusiveSecondaryVertexV2BJetTags", btagCSV);

    if (verbose_) std::cout << "  put in event" << std::endl;
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<class T>
void JetEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
typedef JetEmbedder<pat::Tau> TauJetEmbedder;
typedef JetEmbedder<pat::Muon> MuonJetEmbedder;
typedef JetEmbedder<pat::Electron> ElectronJetEmbedder;
typedef JetEmbedder<pat::Photon> PhotonJetEmbedder;
DEFINE_FWK_MODULE(TauJetEmbedder);
DEFINE_FWK_MODULE(MuonJetEmbedder);
DEFINE_FWK_MODULE(ElectronJetEmbedder);
DEFINE_FWK_MODULE(PhotonJetEmbedder);
