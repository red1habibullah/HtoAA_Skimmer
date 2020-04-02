//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//   EffectiveAreaEmbedder.cc                                               //
//                                                                          //
//   Embeds effective areas using a file                                    //
//                                                                          //
//   Authors: Devin Taylor, U. Wisconsin                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


// system includes
#include <memory>
#include <vector>
#include <iostream>

// CMS includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

template<typename T>
class EffectiveAreaEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit EffectiveAreaEmbedder(const edm::ParameterSet&);
  ~EffectiveAreaEmbedder() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Methods
  void beginJob() {}
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  void endJob() {}

  float getEA(const T obj) const;

  // Data
  edm::EDGetTokenT<edm::View<T> > collectionToken_; // input collection
  const std::string label_;                         // label for the embedded userfloat
  const std::string filename_;                      // filename for effective area
  std::unique_ptr<std::vector<T> > out;               // Collection we'll output at the end
  EffectiveAreas effectiveAreas_;
};


// Constructors and destructors
template<typename T>
EffectiveAreaEmbedder<T>::EffectiveAreaEmbedder(const edm::ParameterSet& iConfig):
  collectionToken_(consumes<edm::View<T> >(iConfig.getParameter<edm::InputTag>("src"))),
  label_(iConfig.exists("label") ? iConfig.getParameter<std::string>("label") : std::string("EffectiveArea")),
  effectiveAreas_((iConfig.getParameter<edm::FileInPath>("configFile")).fullPath())
{
  produces<std::vector<T> >();
}

template<typename T>
void EffectiveAreaEmbedder<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<T> >(new std::vector<T>);

  edm::Handle<edm::View<T> > collection;
  iEvent.getByToken(collectionToken_, collection);

  for (size_t c = 0; c < collection->size(); ++c) {
    const auto obj = collection->at(c);
    T newObj = obj;

    float ea = getEA(obj);
	
    newObj.addUserFloat(label_, ea);
    out->push_back(newObj);
  }

  iEvent.put(std::move(out));
}

template<typename T>
float EffectiveAreaEmbedder<T>::getEA(const T obj) const
{
  float abseta = fabs(obj.eta());
  return effectiveAreas_.getEffectiveArea(abseta);
}

template<typename T>
void EffectiveAreaEmbedder<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
typedef EffectiveAreaEmbedder<pat::Electron> ElectronEffectiveAreaEmbedder;
typedef EffectiveAreaEmbedder<pat::Photon> PhotonEffectiveAreaEmbedder;

DEFINE_FWK_MODULE(ElectronEffectiveAreaEmbedder);
DEFINE_FWK_MODULE(PhotonEffectiveAreaEmbedder);
