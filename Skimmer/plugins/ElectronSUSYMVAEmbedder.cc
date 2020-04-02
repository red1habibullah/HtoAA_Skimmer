// ElectronSUSYMVAEmbedder.cc
// Embeds elons ids as userInts for later

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TLorentzVector.h"

class ElectronSUSYMVAEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit ElectronSUSYMVAEmbedder(const edm::ParameterSet&);
  ~ElectronSUSYMVAEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  bool isPreselection(const pat::Electron& el, const reco::Vertex& pv, bool passID, float miniiso);
  bool isTight(const pat::Electron & el, std::string nonTrigLabel);
  bool isVLoose(const pat::Electron & el, std::string nonTrigLabel);
  bool isVLooseFOIDEmu(const pat::Electron & el, std::string nonTrigLabel);
  bool isVLooseFOIDISOEmu(const pat::Electron & el, std::string nonTrigLabel);
  float getEA(const pat::Electron& el);
  float getMiniIsolation(const pat::Electron& el, double rho);
  void initialize();
  float getMVAValue(const pat::Electron & el, const reco::Vertex& pv, std::string nonTrigLabel);

  // Data
  edm::EDGetTokenT<edm::View<pat::Electron> > collectionToken_;  // input collection
  edm::EDGetTokenT<reco::VertexCollection> vertexToken_;         // vertices
  edm::EDGetTokenT<double> rhoToken_;                            // rho
  std::string nonTrigLabel_;                                     // embedded mva
  std::unique_ptr<std::vector<pat::Electron> > out;                // Collection we'll output at the end
  edm::FileInPath weightsfile_;                                  // MVA weights file
  TMVA::Reader* tmvaReader;                                      // TMVA reader

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
  Float_t LepGood_nontrigMVA;

};

// Constructors and destructors
ElectronSUSYMVAEmbedder::ElectronSUSYMVAEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("src"))),
  vertexToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoSrc"))),
  nonTrigLabel_(iConfig.getParameter<std::string>("mva")),
  weightsfile_(iConfig.getParameter<edm::FileInPath>("weights"))
{
  tmvaReader = new TMVA::Reader("!Color:!Silent:Error");
  initialize();
  produces<std::vector<pat::Electron> >();
}

void ElectronSUSYMVAEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<pat::Electron> >(new std::vector<pat::Electron>);

  edm::Handle<edm::View<pat::Electron> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vertexToken_, vertices);

  const reco::Vertex& pv = *vertices->begin();

  edm::Handle<double> rho;
  iEvent.getByToken(rhoToken_, rho);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    pat::Electron newObj = obj;

    newObj.addUserFloat("SUSYEA", getEA(obj));
    newObj.addUserFloat("SUSYRho", (float)(*rho));
    float miniiso = getMiniIsolation(obj,*rho);
    newObj.addUserFloat("SUSYMiniIsolationEA", miniiso);
    bool isTightVal = isTight(obj,nonTrigLabel_);
    bool isVLooseVal = isVLoose(obj,nonTrigLabel_);
    bool isVLooseFOIDEmuVal = isVLooseFOIDEmu(obj,nonTrigLabel_);
    bool isVLooseFOIDISOEmuVal = isVLooseFOIDISOEmu(obj,nonTrigLabel_);
    newObj.addUserInt("isSUSYTight",isTightVal);
    newObj.addUserInt("isSUSYVLoose",isVLooseVal);
    newObj.addUserInt("isSUSYVLooseFOIDEmu",isVLooseFOIDEmuVal);
    newObj.addUserInt("isSUSYVLooseFOIDISOEmu",isVLooseFOIDISOEmuVal);
    newObj.addUserInt("isSUSYMVAPreselection", isPreselection(obj,pv,isVLooseFOIDEmuVal,miniiso));
    newObj.addUserFloat("SUSYMVA", getMVAValue(obj,pv,nonTrigLabel_));
    
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

// Preselection
// https://twiki.cern.ch/twiki/bin/view/CMS/LeptonMVA
bool ElectronSUSYMVAEmbedder::isPreselection(const pat::Electron & el, const reco::Vertex& pv, bool passID, float miniiso)
  {
    bool pre = passID &&
               miniiso < 0.4 &&
               fabs(el.dB(pat::Electron::PV3D))/el.edB(pat::Electron::PV3D) < 8. &&
               fabs(el.gsfTrack()->dxy(pv.position())) < 0.05 &&
               fabs(el.gsfTrack()->dz(pv.position())) < 0.1;
    return pre;
  }

// MVA working points
bool ElectronSUSYMVAEmbedder::isTight(const pat::Electron & el, std::string nonTrigLabel) 
  {
    double abseta = fabs(el.eta());
    float mva = el.userFloat(nonTrigLabel);
    bool pass = false;
    if (abseta<0.8) {
      pass = (mva>0.87);
    }
    else if (abseta<1.479) {
      pass = (mva>0.6);
    }
    else if (abseta<2.5) {
      pass = (mva>0.17);
    }

    return pass;
  }

bool ElectronSUSYMVAEmbedder::isVLoose(const pat::Electron & el, std::string nonTrigLabel) 
  {
    double abseta = fabs(el.eta());
    float mva = el.userFloat(nonTrigLabel);
    bool pass = false;
    if (abseta<0.8) {
      pass = (mva>-0.16);
    }
    else if (abseta<1.479) {
      pass = (mva>-0.65);
    }
    else if (abseta<2.5) {
      pass = (mva>-0.74);
    }

    return pass;
  }

bool ElectronSUSYMVAEmbedder::isVLooseFOIDEmu(const pat::Electron & el, std::string nonTrigLabel) 
  {
    double abseta = fabs(el.eta());
    float mva = el.userFloat(nonTrigLabel);
    bool pass = false;
    if (abseta<0.8) {
      pass = (mva>-0.7);
    }
    else if (abseta<1.479) {
      pass = (mva>-0.83);
    }
    else if (abseta<2.5) {
      pass = (mva>-0.92);
    }

    return pass;
  }

bool ElectronSUSYMVAEmbedder::isVLooseFOIDISOEmu(const pat::Electron & el, std::string nonTrigLabel) 
  {
    double abseta = fabs(el.eta());
    float mva = el.userFloat(nonTrigLabel);
    bool pass = false;
    if (abseta<0.8) {
      pass = (mva>-0.155);
    }
    else if (abseta<1.479) {
      pass = (mva>-0.56);
    }
    else if (abseta<2.5) {
      pass = (mva>-0.76);
    }

    return pass;
  }

// Isolation and EA
// https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF
float ElectronSUSYMVAEmbedder::getEA(const pat::Electron & el)
  {
    float ea = 0.;
    if (std::abs(el.eta()) < 1.0)
      ea = 0.1752;
    if (std::abs(el.eta()) >= 1.0 && std::abs(el.eta()) < 1.479)
      ea = 0.1862;
    if (std::abs(el.eta()) >= 1.479 && std::abs(el.eta()) < 2.0)
      ea = 0.1411;
    if (std::abs(el.eta()) >= 2.0 && std::abs(el.eta()) < 2.2)
      ea = 0.1534;
    if (std::abs(el.eta()) >= 2.2 && std::abs(el.eta()) < 2.3)
      ea = 0.1903;
    if (std::abs(el.eta()) >= 2.3 && std::abs(el.eta()) < 2.4)
      ea = 0.2243;
    if (std::abs(el.eta()) >= 2.4 && std::abs(el.eta()) <= 2.5)
      ea = 0.2687;
    return ea;
  }

float ElectronSUSYMVAEmbedder::getMiniIsolation(const pat::Electron & el, double rho)
  {
    float ea = getEA(el);
    float chHad = el.userFloat("MiniIsolationCharged");
    float nHad = el.userFloat("MiniIsolationNeutral");
    float isoEA = (chHad + std::max(0.0, nHad - rho * ea * std::pow((10.0/std::min(std::max(el.pt(), 50.),200.))/0.3,2)));
    isoEA = (el.pt()>0 ? isoEA/el.pt() : isoEA);
    return isoEA;
  }

// MVA reader
void ElectronSUSYMVAEmbedder::initialize()
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
    if (nonTrigLabel_.find("HZZ") != std::string::npos)                 tmvaReader->AddVariable("LepGood_mvaIdSpring16HZZ",&LepGood_nontrigMVA);
    else if (nonTrigLabel_.find("GeneralPurpose") != std::string::npos) tmvaReader->AddVariable("LepGood_mvaIdSpring16GP", &LepGood_nontrigMVA);
    else if (nonTrigLabel_.find("Spring15") != std::string::npos)       tmvaReader->AddVariable("LepGood_mvaIdSpring15",   &LepGood_nontrigMVA);
    tmvaReader->BookMVA("BDTG",weightsfile_.fullPath());
  }

float ElectronSUSYMVAEmbedder::getMVAValue(const pat::Electron & el, const reco::Vertex& pv, std::string nonTrigLabel)
  {
    LepGood_pt =                   el.pt();
    LepGood_eta =                  el.eta();
    LepGood_JetNDauCharged =       el.userInt("jet_numberOfChargedDaughters");
    LepGood_miniRelIsoCharged =    el.userFloat("MiniIsolationCharged")/el.pt();
    LepGood_miniRelIsoNeutral =    el.userFloat("MiniIsolationNeutral")/el.pt();
    LepGood_JetPtRel =             el.userFloat("jet_ptRel");
    LepGood_JetPtRatio =           std::min(el.userFloat("jet_ptRatio"),(float)1.5);
    LepGood_JetBTagCSV =           std::max(el.userFloat("jet_pfCombinedInclusiveSecondaryVertexV2BJetTags"),(float)0.);
    LepGood_SIP =                  fabs(el.dB(pat::Electron::PV3D))/el.edB(pat::Electron::PV3D);
    //LepGood_dxyBS =                log(fabs(el.userFloat("dxy_beamspot")));
    LepGood_dxyBS =                log(fabs(el.userFloat("dxy")));
    LepGood_dzPV =                 log(fabs(el.userFloat("dz")));
    LepGood_nontrigMVA =           el.userFloat(nonTrigLabel);
    return tmvaReader->EvaluateMVA("BDTG");
  }
   


void ElectronSUSYMVAEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ElectronSUSYMVAEmbedder);
