// MuonSUSYMVAEmbedder.cc
// Embeds muons ids as userInts for later

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TLorentzVector.h"

class MuonSUSYMVAEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit MuonSUSYMVAEmbedder(const edm::ParameterSet&);
  ~MuonSUSYMVAEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  bool isPreselection(const pat::Muon& mu, const reco::Vertex& pv, float miniiso);
  float getEA(const pat::Muon& mu);
  float getMiniIsolation(const pat::Muon& mu, double rho);
  void initialize();
  float getMVAValue(const pat::Muon & mu, const reco::Vertex& pv);

  // Data
  edm::EDGetTokenT<edm::View<pat::Muon> > collectionToken_;       // input collection
  edm::EDGetTokenT<reco::VertexCollection> vertexToken_;          // vertices
  edm::EDGetTokenT<double> rhoToken_;                             // rho
  std::unique_ptr<std::vector<pat::Muon> > out;                     // Collection we'll output at the end
  edm::FileInPath weightsfile_;                                   // MVA weights file
  TMVA::Reader* tmvaReader;                                       // TMVA reader

  /// MVA VAriables:
  Float_t LepGood_pt;
  Float_t LepGood_eta;
  Float_t LepGood_JetNDauCharged;
  Float_t LepGood_miniRelIsoCharged;
  Float_t LepGood_miniRelIsoNeutral;
  Float_t LepGood_JetPtRel;
  Float_t LepGood_JetPtRatio;
  Float_t LepGood_JetBTagCSV;
  Float_t LepGood_SIP;
  Float_t LepGood_dxyBS; 
  Float_t LepGood_dzPV;            
  Float_t LepGood_segmentCompatibility;
};

// Constructors and destructors
MuonSUSYMVAEmbedder::MuonSUSYMVAEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<pat::Muon> >(iConfig.getParameter<edm::InputTag>("src"))),
  vertexToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoSrc"))),
  weightsfile_(iConfig.getParameter<edm::FileInPath>("weights"))
{
  tmvaReader = new TMVA::Reader("!Color:!Silent:Error");
  initialize();
  produces<std::vector<pat::Muon> >();
}

void MuonSUSYMVAEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<pat::Muon> >(new std::vector<pat::Muon>);

  edm::Handle<edm::View<pat::Muon> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vertexToken_, vertices);

  const reco::Vertex& pv = *vertices->begin();

  edm::Handle<double> rho;
  iEvent.getByToken(rhoToken_, rho);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    pat::Muon newObj = obj;

    newObj.addUserFloat("SUSYEA", getEA(obj));
    newObj.addUserFloat("SUSYRho", (float)(*rho));
    float miniiso = getMiniIsolation(obj,*rho);
    newObj.addUserFloat("SUSYMiniIsolationEA", miniiso);
    newObj.addUserInt("isSUSYMVAPreselection", isPreselection(obj,pv,miniiso));
    newObj.addUserFloat("SUSYMVA", getMVAValue(obj,pv));
    
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

// Preselection
// https://twiki.cern.ch/twiki/bin/view/CMS/LeptonMVA
bool MuonSUSYMVAEmbedder::isPreselection(const pat::Muon & mu, const reco::Vertex& pv, float miniiso) 
  {
    bool pre = muon::isLooseMuon(mu) &&
               miniiso < 0.4 &&
               fabs(mu.dB(pat::Muon::PV3D))/mu.edB(pat::Muon::PV3D) < 8. &&
               fabs(mu.innerTrack()->dxy(pv.position())) < 0.05 &&
               fabs(mu.innerTrack()->dz(pv.position())) < 0.1;
    return pre;
  }

// Isolation and EA
// https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF
float MuonSUSYMVAEmbedder::getEA(const pat::Muon & mu)
  {
    float ea = 0.;
    if (std::abs(mu.eta()) < 0.8)
      ea = 0.0735;
    if (std::abs(mu.eta()) >= 0.8 && std::abs(mu.eta()) < 1.3)
      ea = 0.0619;
    if (std::abs(mu.eta()) >= 1.3 && std::abs(mu.eta()) < 2.0)
      ea = 0.0465;
    if (std::abs(mu.eta()) >= 2.0 && std::abs(mu.eta()) < 2.2)
      ea = 0.0433;
    if (std::abs(mu.eta()) >= 2.2 && std::abs(mu.eta()) <= 2.5)
      ea = 0.0577;
    return ea;
  }

float MuonSUSYMVAEmbedder::getMiniIsolation(const pat::Muon & mu, double rho)
  {
    float ea = getEA(mu);
    float chHad = mu.userFloat("MiniIsolationCharged");
    float nHad = mu.userFloat("MiniIsolationNeutral");
    float isoEA = (chHad + std::max(0.0, nHad - rho * ea * std::pow((10.0/std::min(std::max(mu.pt(), 50.),200.))/0.3,2)));
    isoEA = (mu.pt()>0 ? isoEA/mu.pt() : isoEA);
    return isoEA;
  }

// MVA reader
void MuonSUSYMVAEmbedder::initialize()
  {
    tmvaReader->AddVariable("LepGood_pt",                   &LepGood_pt                  );
    tmvaReader->AddVariable("LepGood_eta",                  &LepGood_eta                 );
    tmvaReader->AddVariable("LepGood_jetNDauChargedMVASel", &LepGood_JetNDauCharged      );
    tmvaReader->AddVariable("LepGood_miniRelIsoCharged",    &LepGood_miniRelIsoCharged   );
    tmvaReader->AddVariable("LepGood_miniRelIsoNeutral",    &LepGood_miniRelIsoNeutral   );
    tmvaReader->AddVariable("LepGood_jetPtRelv2",           &LepGood_JetPtRel            );
    tmvaReader->AddVariable("min(LepGood_jetPtRatiov2,1.5)",&LepGood_JetPtRatio          );
    tmvaReader->AddVariable("max(LepGood_jetBTagCSV,0)",    &LepGood_JetBTagCSV          );
    tmvaReader->AddVariable("LepGood_sip3d",                &LepGood_SIP                 );
    tmvaReader->AddVariable("log(abs(LepGood_dxy))",        &LepGood_dxyBS               ); 
    tmvaReader->AddVariable("log(abs(LepGood_dz))",         &LepGood_dzPV                );
    tmvaReader->AddVariable("LepGood_segmentCompatibility", &LepGood_segmentCompatibility);
    tmvaReader->BookMVA("BDTG",weightsfile_.fullPath());
  }

float MuonSUSYMVAEmbedder::getMVAValue(const pat::Muon & mu, const reco::Vertex& pv)
  {
    LepGood_pt =                   mu.pt();
    LepGood_eta =                  mu.eta();
    LepGood_JetNDauCharged =       mu.userInt("jet_numberOfChargedDaughters");
    LepGood_miniRelIsoCharged =    mu.userFloat("MiniIsolationCharged")/mu.pt();
    LepGood_miniRelIsoNeutral =    mu.userFloat("MiniIsolationNeutral")/mu.pt();
    LepGood_JetPtRel =             mu.userFloat("jet_ptRel");
    LepGood_JetPtRatio =           std::min(mu.userFloat("jet_ptRatio"),(float)1.5);
    LepGood_JetBTagCSV =           std::max(mu.userFloat("jet_pfCombinedInclusiveSecondaryVertexV2BJetTags"),(float)0.);
    LepGood_SIP =                  fabs(mu.dB(pat::Muon::PV3D))/mu.edB(pat::Muon::PV3D);
    //LepGood_dxyBS =                log(fabs(mu.userFloat("dxy_beamspot")));
    LepGood_dxyBS =                log(fabs(mu.userFloat("dB2D")));
    LepGood_dzPV =                 log(fabs(mu.userFloat("dz")));
    LepGood_segmentCompatibility = mu.segmentCompatibility();
    return tmvaReader->EvaluateMVA("BDTG");
    
  }

void MuonSUSYMVAEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(MuonSUSYMVAEmbedder);
