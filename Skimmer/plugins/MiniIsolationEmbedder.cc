// MiniIsolationEmbedder.cc
// Embeds muons ids as userInts for later

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Math/interface/deltaR.h"

template<typename T>
class MiniIsolationEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit MiniIsolationEmbedder(const edm::ParameterSet&);
  ~MiniIsolationEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  std::tuple<double,double,double,double,double> getMiniIsolation(
    edm::Handle<pat::PackedCandidateCollection> pfcands, 
    const reco::Candidate& cand, 
    double r_iso_min, double r_iso_max, double kt_scale);

  // Data
  edm::EDGetTokenT<edm::View<T> > collectionToken_;               // input collection
  edm::EDGetTokenT<pat::PackedCandidateCollection> pfcandsToken_; // pfcands
  std::unique_ptr<std::vector<T> > out;                             // Collection we'll output at the end
};

// Constructors and destructors
template<typename T>
MiniIsolationEmbedder<T>::MiniIsolationEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  pfcandsToken_(consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("packedSrc")))
{
  produces<std::vector<T> >();
}

template<typename T>
void MiniIsolationEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByToken(pfcandsToken_, pfcands);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;

    double iso;
    double iso_ch;
    double iso_nh;
    double iso_ph;
    double iso_pu;
    std::tie(iso,iso_ch,iso_nh,iso_ph,iso_pu) = getMiniIsolation(pfcands,obj,0.05,0.2,10.);
    newObj.addUserFloat("MiniIsolation", (float)(iso));
    newObj.addUserFloat("MiniIsolationCharged", (float)(iso_ch));
    newObj.addUserFloat("MiniIsolationNeutral", (float)(iso_nh));
    newObj.addUserFloat("MiniIsolationPhoton", (float)(iso_ph));
    newObj.addUserFloat("MiniIsolationPileup", (float)(iso_pu));
    
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

// Isolation
// https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF
template<typename T>
std::tuple<double,double,double,double,double> MiniIsolationEmbedder<T>::getMiniIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands,
  const reco::Candidate& cand,  
  double r_iso_min, double r_iso_max, double kt_scale)
{
 
  //if (cand.pt()<5.) return 99999.;

  double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
  if (cand.isElectron()) {
    if (fabs(cand.eta())>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}
  } else if( cand.isMuon()) {
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;  
  } else {
    //deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // maybe use muon cones??
  }
  
  double iso_nh(0.); double iso_ch(0.); 
  double iso_ph(0.); double iso_pu(0.);
  double ptThresh(0.5);
  if (cand.isElectron()) ptThresh = 0;
  double r_iso = std::max(r_iso_min,std::min(r_iso_max, kt_scale/cand.pt()));
  for (const pat::PackedCandidate &pfc : *pfcands) {
    if (abs(pfc.pdgId())<7) continue;
    
    double dr = reco::deltaR(pfc, cand);
    if (dr > r_iso) continue;
    
    //////////////////  NEUTRALS  /////////////////////////
    if (pfc.charge()==0){
      if (pfc.pt()>ptThresh) {
	/////////// PHOTONS ////////////
	if (abs(pfc.pdgId())==22) {
	  if(dr < deadcone_ph) continue;
	  iso_ph += pfc.pt();
	  /////////// NEUTRAL HADRONS ////////////
	} else if (abs(pfc.pdgId())==130) {
	  if(dr < deadcone_nh) continue;
	  iso_nh += pfc.pt();
	}
      }
      //////////////////  CHARGED from PV  /////////////////////////
    } else if (pfc.fromPV()>1){
      if (abs(pfc.pdgId())==211) {
	if(dr < deadcone_ch) continue;
	iso_ch += pfc.pt();
      }
      //////////////////  CHARGED from PU  /////////////////////////
    } else {
      if (pfc.pt()>ptThresh){
	if(dr < deadcone_pu) continue;
	iso_pu += pfc.pt();
      }
    }
  }
  double iso(0.);
  iso = iso_ph + iso_nh;
  iso -= 0.5*iso_pu;
  if (iso>0) iso += iso_ch;
  else iso = iso_ch;
  if (cand.pt()) iso = iso/cand.pt();
  
  return std::make_tuple(iso,iso_ch,iso_nh,iso_ph,iso_pu);
}

template<typename T>
void MiniIsolationEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
typedef MiniIsolationEmbedder<pat::Electron> ElectronMiniIsolationEmbedder;
typedef MiniIsolationEmbedder<pat::Muon> MuonMiniIsolationEmbedder;

DEFINE_FWK_MODULE(ElectronMiniIsolationEmbedder);
DEFINE_FWK_MODULE(MuonMiniIsolationEmbedder);
