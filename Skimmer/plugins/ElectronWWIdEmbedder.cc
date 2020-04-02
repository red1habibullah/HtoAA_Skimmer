#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


class ElectronWWIdEmbedder : public edm::stream::EDProducer<> {
  public:
    ElectronWWIdEmbedder(const edm::ParameterSet& pset);
    virtual ~ElectronWWIdEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::EDGetTokenT<edm::View<pat::Electron> > srcToken_;
    edm::EDGetTokenT<edm::View<reco::Vertex> > vertexToken_;
};

ElectronWWIdEmbedder::ElectronWWIdEmbedder(const edm::ParameterSet& pset):
  srcToken_(consumes<edm::View<pat::Electron> >(pset.getParameter<edm::InputTag>("src"))),
  vertexToken_(consumes<edm::View<reco::Vertex> >(pset.getParameter<edm::InputTag>("vertexSrc")))
{
  produces<pat::ElectronCollection>();
}

void ElectronWWIdEmbedder::produce(edm::Event& evt, const edm::EventSetup& es) {
  std::unique_ptr<pat::ElectronCollection> output(new pat::ElectronCollection);

  edm::Handle<edm::View<pat::Electron> > input;
  evt.getByToken(srcToken_, input);

  edm::Handle<edm::View<reco::Vertex> > vertices;
  evt.getByToken(vertexToken_, vertices);

  const reco::Vertex& thePV = *vertices->begin();

  output->reserve(input->size());
  for (size_t i = 0; i < input->size(); ++i) {
    pat::Electron electron = input->at(i);

    double pt = electron.pt();
    double dEtaIn = std::abs(electron.deltaEtaSuperClusterTrackAtVtx());
    double dPhiIn = std::abs(electron.deltaPhiSuperClusterTrackAtVtx());
    double sigmaIEtaIEta = electron.sigmaIetaIeta();
    double hOverE = electron.hcalOverEcal();
    double oneOverEMinusOneOverP = abs((1.-electron.eSuperClusterOverP())*1./electron.ecalEnergy());
    double ecalPFClusterIso = electron.ecalPFClusterIso();
    double hcalPFClusterIso = electron.hcalPFClusterIso();
    double trackIso = electron.dr03TkSumPt();
    int missingHits = electron.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS);
    double dxy = std::abs(electron.gsfTrack()->dxy(thePV.position()));
    double dz = std::abs(electron.gsfTrack()->dz(thePV.position()));
    bool passConversionVeto = electron.passConversionVeto();

    bool passLoose = true;
    if (electron.isEB()) {
      if (!(dEtaIn < 0.01))
        passLoose = false;
      if (!(dPhiIn < 0.04))
        passLoose = false;
      if (!(sigmaIEtaIEta < 0.011))
        passLoose = false;
      if (!(hOverE < 0.08))
        passLoose = false;
      if (!(oneOverEMinusOneOverP < 0.01))
        passLoose = false;
      if (!(ecalPFClusterIso/pt < 0.45))
        passLoose = false;
      if (!(hcalPFClusterIso/pt < 0.25))
        passLoose = false;
      if (!(trackIso/pt < 0.2))
        passLoose = false;
      if (!(missingHits <= 2))
        passLoose = false;
      if (!(dxy < 0.1))
        passLoose = false;
      if (!(dz < 0.373))
        passLoose = false;
      if (!passConversionVeto)
        passLoose = false;
    }
    else if (electron.isEE()) {
      if (!(dEtaIn < 0.01))
        passLoose = false;
      if (!(dPhiIn < 0.08))
        passLoose = false;
      if (!(sigmaIEtaIEta < 0.031))
        passLoose = false;
      if (!(hOverE < 0.08))
        passLoose = false;
      if (!(oneOverEMinusOneOverP < 0.01))
        passLoose = false;
      if (!(ecalPFClusterIso/pt < 0.45))
        passLoose = false;
      if (!(hcalPFClusterIso/pt < 0.25))
        passLoose = false;
      if (!(trackIso/pt < 0.2))
        passLoose = false;
      if (!(missingHits <= 1))
        passLoose = false;
      if (!(dxy < 0.2))
        passLoose = false;
      if (!(dz < 0.602))
        passLoose = false;
      if (!passConversionVeto)
        passLoose = false;
    }
    else {
      passLoose = false;
    }

    electron.addUserInt("WWLoose", passLoose);
    output->push_back(electron);
  }

  evt.put(std::move(output));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ElectronWWIdEmbedder);
