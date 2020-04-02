#include "HtoAA_Skimmer/Skimmer/interface/MonteCarloBranches.h"

MonteCarloBranches::MonteCarloBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
    lheEventProductToken_(cc.consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lheEventProduct"))),
    genEventInfoToken_(cc.consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfo"))),
    pileupSummaryInfoToken_(cc.consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileupSummaryInfo")))
{
  // add branches
  tree->Branch("genWeight", &genWeightBranch_, "genWeight/F");
  tree->Branch("numWeights", &numWeightsBranch_, "numWeights/I");
  tree->Branch("genWeights", &genWeightsBranch_);
  tree->Branch("ptHat", &ptHatBranch_, "ptHat/F");
  tree->Branch("nTrueVertices", &nTrueVerticesBranch_, "nTrueVertices/F");
  tree->Branch("nObservedVertices", &nObservedVerticesBranch_, "nTrueVertices/I");
  tree->Branch("NUP", &nupBranch_, "NUP/I");
  tree->Branch("numGenJets", &numGenJetsBranch_, "numGenJets/I");
  tree->Branch("genHT", &genHTBranch_, "genHT/I");
}

void MonteCarloBranches::fill(const edm::Event& iEvent)
{
    // Get products
    edm::Handle<std::vector<PileupSummaryInfo> > pileupSummaryInfo;
    iEvent.getByToken(pileupSummaryInfoToken_, pileupSummaryInfo);

    edm::Handle<GenEventInfoProduct> genEventInfo;
    iEvent.getByToken(genEventInfoToken_, genEventInfo);

    edm::Handle<LHEEventProduct> lheInfo;
    iEvent.getByToken(lheEventProductToken_, lheInfo);

    // fill branches
    genWeightBranch_ = 0.;
    if (genEventInfo.isValid()) {
        genWeightBranch_ = genEventInfo->weight();
        ptHatBranch_ = genEventInfo->hasBinningValues() ? genEventInfo->binningValues()[0] : 0;
    }

    nTrueVerticesBranch_ = 0;
    nObservedVerticesBranch_ = 0;
    if (pileupSummaryInfo.isValid() && pileupSummaryInfo->size()>0) {
        for( auto & pu : *pileupSummaryInfo ) {
            if( pu.getBunchCrossing() == 0 ) {
                nTrueVerticesBranch_ = pu.getTrueNumInteractions();
                nObservedVerticesBranch_ = pu.getPU_NumInteractions();
                break;
            }
        }
        //nTrueVerticesBranch_ = pileupSummaryInfo->at(1).getTrueNumInteractions();
    }

    // calculate lhe related information
    nupBranch_ = 0;
    numGenJetsBranch_ = 0;
    genHTBranch_ = 0;
    genWeightsBranch_.clear();
    if (lheInfo.isValid()) {
        nupBranch_ = lheInfo->hepeup().NUP;
        for (int i =0; i<lheInfo->hepeup().NUP; ++i) {
            int absPdgId = TMath::Abs(lheInfo->hepeup().IDUP[i]);
            int status = lheInfo->hepeup().ISTUP[i];
            if (status == 1 && ((absPdgId >= 1 && absPdgId <= 6) || absPdgId == 21)) { // quarks/gluons
                ++numGenJetsBranch_;
            }
            if (lheInfo->hepeup().ISTUP[i] <0 || (absPdgId>5 && absPdgId!=21))  continue;
            double px=lheInfo->hepeup().PUP.at(i)[0];
            double py=lheInfo->hepeup().PUP.at(i)[1];
            double pt=sqrt(px*px+py*py);
            genHTBranch_ += (Float_t)pt;
        }
        for (size_t i=0; i<lheInfo->weights().size(); ++i) {
            genWeightsBranch_.push_back(lheInfo->weights()[i].wgt);
        }
    }
    numWeightsBranch_ = genWeightsBranch_.size();
}

