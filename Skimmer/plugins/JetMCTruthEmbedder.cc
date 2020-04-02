#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

class JetMCTruthEmbedder : public edm::stream::EDProducer<> {
  public:
    JetMCTruthEmbedder(const edm::ParameterSet& pset);
    virtual ~JetMCTruthEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::EDGetTokenT<edm::View<pat::Jet> > srcToken_;
    edm::EDGetTokenT<edm::View<reco::GenParticle> > genToken_;
    edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;
};

JetMCTruthEmbedder::JetMCTruthEmbedder(const edm::ParameterSet& pset):
  srcToken_(consumes<edm::View<pat::Jet> >(pset.getParameter<edm::InputTag>("src"))),
  genToken_(consumes<edm::View<reco::GenParticle> >(pset.getParameter<edm::InputTag>("genSrc"))),
  packedGenToken_(consumes<edm::View<pat::PackedGenParticle> >(pset.getParameter<edm::InputTag>("packedGenSrc")))
{
  produces<pat::JetCollection>();
}

void JetMCTruthEmbedder::produce(edm::Event& evt, const edm::EventSetup& es) {
  std::unique_ptr<pat::JetCollection> output(new pat::JetCollection);

  edm::Handle<edm::View<pat::Jet> > input;
  evt.getByToken(srcToken_, input);

  edm::Handle<edm::View<reco::GenParticle> > gen;
  evt.getByToken(genToken_, gen);

  edm::Handle<edm::View<pat::PackedGenParticle> > packedGen;
  evt.getByToken(packedGenToken_, packedGen);

  float dR = 0.4;

  output->reserve(input->size());
  for (size_t i = 0; i < input->size(); ++i) {
    pat::Jet jet = input->at(i);

    // check Jets
    int hflav = abs(jet.hadronFlavour());
    int pflav = abs(jet.partonFlavour());
    int physflav = jet.genParton() ? abs(jet.genParton()->pdgId()) : 0;
    std::size_t nbs = jet.jetFlavourInfo().getbHadrons().size();
    std::size_t ncs = jet.jetFlavourInfo().getcHadrons().size();

    // check taus
    int ntaumu = 0;
    int ntaue = 0;
    int ntaudm0 = 0;
    int ntaudm1 = 0;
    int ntaudm10 = 0;
    for (const auto& g : *gen) {
      // check for tau
      if (std::abs(g.pdgId()) == 15) {
        // find constituents
        int nch = 0;
        int nneu = 0;
        for (size_t d=0; d<g.numberOfDaughters(); d++) {
          if (reco::deltaR(*g.daughter(d),jet) > dR) continue;
          int did = std::abs(g.daughter(d)->pdgId());
          int dch = g.daughter(d)->charge();
          if (did == 13) {
            ntaumu++;
          }
          else if (did == 11) {
            ntaue++;
          }
          else if (did != 12 && did != 14 && did != 16) {
            if (dch) {
              nch++;
            }
            else {
              nneu++;
            }
          }
        }
        // count up the dms
        if (nch==1) {
          if (nneu) {
            ntaudm1++;
          }
          else {
            ntaudm0++;
          }
        }
        else if (nch>1) {
          ntaudm10++;
        }
      }
    }

    int ntauhad = ntaudm0 + ntaudm1 + ntaudm10;
    int ntau = ntaue + ntaumu + ntauhad;

    // store
    int isB=0, isBB=0, isC=0, isCC=0;
    int isG=0, isS=0, isUD=0;
    int isGPhys=0, isSPhys=0, isUDPhys=0;
    int isTau=0, isTauTau=0;
    int isTauE=0, isTauM=0, isTauH=0;
    int isTauDM0=0, isTauDM1=0, isTauDM10=0;
    int isTauETauE=0, isTauETauM=0, isTauETauH=0;
    int isTauMTauM=0, isTauMTauH=0, isTauHTauH=0;
    int isTauETauDM0=0, isTauETauDM1=0, isTauETauDM10=0;
    int isTauMTauDM0=0, isTauMTauDM1=0, isTauMTauDM10=0;
    int isTauDM0TauDM0=0, isTauDM0TauDM1=0, isTauDM0TauDM10=0;
    int isTauDM1TauDM1=0, isTauDM1TauDM10=0, isTauDM10TauDM10=0;

    // prioritize by:
    //   b tagged
    //   c tagged
    //   tau tagged
    //   light
    // TODO separate leptonic bs
    // TODO separete GBB/GCC and BB/CC
    if (hflav==5) {
      isB = (hflav==5 && nbs==1);
      isBB = (hflav==5 && nbs>1);
    }
    else if (hflav==4) {
      isC = (hflav==4 && ncs==1);
      isCC = (hflav==4 && ncs>1);
    }
    else if (ntau>0) {
      isTau    = ntau==1;
      isTauTau = ntau==2;
      // by decay mode
      isTauE    = ntaue==1 && ntau==1;
      isTauM    = ntaumu==1 && ntau==1;
      isTauH    = ntauhad==1 && ntau==1;
      isTauDM0  = ntaudm0==1 && ntau==1;
      isTauDM1  = ntaudm1==1 && ntau==1;
      isTauDM10 = ntaudm10==1 && ntau==1;
      // ditau
      isTauETauE = ntaue==2 && ntau==2;
      isTauETauM = ntaue==1 && ntaumu==1 && ntau==2;
      isTauETauH = ntaue==1 && ntauhad==1 && ntau==2;
      isTauMTauM = ntaumu==2 && ntau==2;
      isTauMTauH = ntaumu==1 && ntauhad==1 && ntau==2;
      isTauHTauH = ntauhad==2 && ntau==2;
      // by dm
      isTauETauDM0     = ntaue==1 && ntaudm0==1 && ntau==2;
      isTauETauDM1     = ntaue==1 && ntaudm1==1 && ntau==2;
      isTauETauDM10    = ntaue==1 && ntaudm10==1 && ntau==2;
      isTauMTauDM0     = ntaumu==1 && ntaudm0==1 && ntau==2;
      isTauMTauDM1     = ntaumu==1 && ntaudm1==1 && ntau==2;
      isTauMTauDM10    = ntaumu==1 && ntaudm10==1 && ntau==2;
      isTauDM0TauDM0   = ntaudm0==2 && ntau==2;
      isTauDM0TauDM1   = ntaudm0==1 && ntaudm1==1 && ntau==2;
      isTauDM0TauDM10  = ntaudm0==1 && ntaudm10==1 && ntau==2;
      isTauDM1TauDM1   = ntaudm1==2 && ntau==2;
      isTauDM1TauDM10  = ntaudm1==1 && ntaudm10==1 && ntau==2;
      isTauDM10TauDM10 = ntaudm10==2 && ntau==2;
    }
    else {
      isG = (pflav==21);
      isS = (pflav==3);
      isUD = (pflav==2 || pflav==1);
      isGPhys = (physflav==21);
      isSPhys = (physflav==3);
      isUDPhys = (physflav==2 || physflav==1);
    }


    // fill jet
    jet.addUserInt("isB", isB);
    jet.addUserInt("isBB", isBB);
    jet.addUserInt("isC", isC);
    jet.addUserInt("isCC", isCC);
    jet.addUserInt("isG", isG);
    jet.addUserInt("isS", isS);
    jet.addUserInt("isUD", isUD);
    jet.addUserInt("isGPhys", isGPhys);
    jet.addUserInt("isSPhys", isSPhys);
    jet.addUserInt("isUDPhys", isUDPhys);

    // fill tau
    // global
    jet.addUserInt("isTau", isTau);
    jet.addUserInt("isTauTau", isTauTau);
    // by decay mode
    jet.addUserInt("isTauE", isTauE);
    jet.addUserInt("isTauM", isTauM);
    jet.addUserInt("isTauH", isTauH);
    jet.addUserInt("isTauDM0", isTauDM0);
    jet.addUserInt("isTauDM1", isTauDM1);
    jet.addUserInt("isTauDM10", isTauDM10);
    // ditau
    jet.addUserInt("isTauETauE", isTauETauE);
    jet.addUserInt("isTauETauM", isTauETauM);
    jet.addUserInt("isTauETauH", isTauETauH);
    jet.addUserInt("isTauMTauM", isTauMTauM);
    jet.addUserInt("isTauMTauH", isTauMTauH);
    jet.addUserInt("isTauHTauH", isTauHTauH);
    // by dm
    jet.addUserInt("isTauETauDM0",     isTauETauDM0);
    jet.addUserInt("isTauETauDM1",     isTauETauDM1);
    jet.addUserInt("isTauETauDM10",    isTauETauDM10);
    jet.addUserInt("isTauMTauDM0",     isTauMTauDM0);
    jet.addUserInt("isTauMTauDM1",     isTauMTauDM1);
    jet.addUserInt("isTauMTauDM10",    isTauMTauDM10);
    jet.addUserInt("isTauDM0TauDM0",   isTauDM0TauDM0);
    jet.addUserInt("isTauDM0TauDM1",   isTauDM0TauDM1);
    jet.addUserInt("isTauDM0TauDM10",  isTauDM0TauDM10);
    jet.addUserInt("isTauDM1TauDM1",   isTauDM1TauDM1);
    jet.addUserInt("isTauDM1TauDM10",  isTauDM1TauDM10);
    jet.addUserInt("isTauDM10TauDM10", isTauDM10TauDM10);
    output->push_back(jet);
  }

  evt.put(std::move(output));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(JetMCTruthEmbedder);
