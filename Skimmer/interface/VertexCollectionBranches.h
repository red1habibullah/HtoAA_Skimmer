#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TTree.h"

template<typename T>
class VertexCollectionFunction {
  public:
    VertexCollectionFunction(TTree * tree, std::string functionName, std::string functionString, int maxCount);
    void evaluate(const reco::VertexCollection& candidates);

  private:
    StringObjectFunction<reco::Vertex, true> function_;
    std::string functionString_;
    std::string functionName_;
    TBranch * vectorBranch_;
    std::vector<T> values_;
    int maxCount_;
};

typedef VertexCollectionFunction<int> VertexCollectionIntFunction;
typedef VertexCollectionFunction<float> VertexCollectionFloatFunction;

class VertexCollectionBranches {
  public:
    VertexCollectionBranches(TTree * tree, std::string collectionName,  const edm::ParameterSet& iConfig, edm::ConsumesCollector cc);
    void fill(const edm::Event& iEvent);
    int keep() { return minCount_ > 0 ? collectionCount_ >= minCount_ : false; }

  private:
    edm::EDGetTokenT<reco::VertexCollection> collectionToken_;
    edm::ParameterSet branches_;
    std::vector<std::unique_ptr<VertexCollectionFloatFunction> > floatFunctions_;
    std::vector<std::unique_ptr<VertexCollectionIntFunction> > intFunctions_;
    TBranch * collectionCountBranch_;
    int collectionCount_;
    int minCount_;
    int maxCount_;
};
