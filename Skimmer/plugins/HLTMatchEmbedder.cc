// HLTMatchEmbedder.cc
// Embed trigger matching

#include <regex>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "DataFormats/Math/interface/deltaR.h"

template<typename T>
class HLTMatchEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit HLTMatchEmbedder(const edm::ParameterSet&);
  ~HLTMatchEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  size_t GetTriggerBit(std::string path, const edm::TriggerNames& names);
  int MatchToTriggerObject(T obj, std::string path, const edm::TriggerNames& names);

  // Data
  edm::EDGetTokenT<edm::View<T> > collectionToken_;                              // input collection
  edm::EDGetTokenT<edm::TriggerResults> triggerBitsToken_;                       // trigger decisions
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsToken_; // trigger objects
  edm::Handle<edm::TriggerResults> triggerBits_;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  double deltaR_;                                                                // deltaR for object match
  std::vector<std::string> labels_;                                              // labels for the embedded userfloat
  std::vector<std::string> paths_;                                               // paths for the matched object
  std::unique_ptr<std::vector<T> > out;                                            // Collection we'll output at the end
};

// Constructors and destructors
template<typename T>
HLTMatchEmbedder<T>::HLTMatchEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  triggerBitsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"))),
  triggerObjectsToken_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
  deltaR_(iConfig.getParameter<double>("deltaR")),
  labels_(iConfig.getParameter<std::vector<std::string> >("labels")),
  paths_(iConfig.getParameter<std::vector<std::string> >("paths"))
{
  if (labels_.size() != paths_.size()) {
    throw cms::Exception("SizeMismatch")
        << "Mismatch in number of labels (" << labels_.size()
        << ") and number of paths (" << paths_.size()
        << ")." << std::endl;
  }
  produces<std::vector<T> >();
}

template<typename T>
void HLTMatchEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  iEvent.getByToken(triggerBitsToken_, triggerBits_);
  iEvent.getByToken(triggerObjectsToken_, triggerObjects_);

  const edm::TriggerNames& names = iEvent.triggerNames(*triggerBits_);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;

    for (size_t i=0; i<labels_.size(); i++) {
      std::string label = labels_.at(i);
      std::string path = paths_.at(i);
      int match = MatchToTriggerObject(obj,path,names);
      newObj.addUserInt(label, match);
    }

    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<typename T>
size_t HLTMatchEmbedder<T>::GetTriggerBit(std::string path, const edm::TriggerNames& names) {
    std::regex regexp(path);
    size_t trigBit = names.size();
    for (size_t i=0; i<names.size(); i++) {
        if (std::regex_match(names.triggerName(i), regexp)) {
            if (trigBit != names.size()) { // if we match more than one
                throw cms::Exception("DuplicateTrigger")
                    << "Second trigger matched for \"" << path
                    << "\". First: \"" << names.triggerName(trigBit)
                    << "\"; second: \"" << names.triggerName(i) << "\"." << std::endl;
            }
            trigBit = i;
        }
    }
    if (trigBit == names.size()) {
        return 9999;
        //throw cms::Exception("UnrecognizedTrigger")
        //    << "No trigger matched for \"" << path << "\"." << std::endl;
    }
    return trigBit;
}

template<typename T>
int HLTMatchEmbedder<T>::MatchToTriggerObject(T obj, std::string path, const edm::TriggerNames& names) {
    int matched = 0;
    size_t trigBit = GetTriggerBit(path,names);
    if (trigBit==9999) {
        return -1;
    }
    std::string pathToMatch = names.triggerName(trigBit);
    for (auto trigObj : *triggerObjects_) {
       if (abs(trigObj.pdgId()) != abs(obj.pdgId())) continue;
       if (reco::deltaR(trigObj, obj) > deltaR_) continue;
       trigObj.unpackPathNames(names);
       std::vector<std::string> allPathNames = trigObj.pathNames(false);
       for (auto pathName : allPathNames) {
           if (pathName.compare(pathToMatch)==0) {
               matched = 1;
               return matched;
           }
       }
    }
    return matched;
}

template<typename T>
void HLTMatchEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
typedef HLTMatchEmbedder<pat::Electron> ElectronHLTMatchEmbedder;
typedef HLTMatchEmbedder<pat::Muon> MuonHLTMatchEmbedder;
typedef HLTMatchEmbedder<pat::Tau> TauHLTMatchEmbedder;
typedef HLTMatchEmbedder<pat::Photon> PhotonHLTMatchEmbedder;
typedef HLTMatchEmbedder<pat::Jet> JetHLTMatchEmbedder;

DEFINE_FWK_MODULE(ElectronHLTMatchEmbedder);
DEFINE_FWK_MODULE(MuonHLTMatchEmbedder);
DEFINE_FWK_MODULE(TauHLTMatchEmbedder);
DEFINE_FWK_MODULE(PhotonHLTMatchEmbedder);
DEFINE_FWK_MODULE(JetHLTMatchEmbedder);
