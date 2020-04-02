#include "HtoAA_Skimmer/Skimmer/interface/VertexCollectionBranches.h"

template<typename T>
VertexCollectionFunction<T>::VertexCollectionFunction(TTree * tree, std::string functionName, std::string functionString, int maxCount):
  function_(functionString),
  functionString_(functionString),
  functionName_(functionName),
  vectorBranch_(tree->Branch(functionName.c_str(), &values_)),
  maxCount_(maxCount)
{
}

template<typename T>
void VertexCollectionFunction<T>::evaluate(const reco::VertexCollection& candidates)
{
  values_.clear();
  try {
    int i = 0;
    for (const auto& candidate: candidates) {
      if (maxCount_>0 and i>=maxCount_) break;
      values_.push_back(function_(candidate));
      i++;
    }
  } catch(cms::Exception& iException) {
    iException << "Caught exception in evaluating branch: "
    << functionName_ << " with formula: " << functionString_;
    throw;
  }
}


VertexCollectionBranches::VertexCollectionBranches(TTree * tree, std::string collectionName,  const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
  collectionToken_(cc.consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("collection"))),
  branches_(iConfig.getParameter<edm::ParameterSet>("branches")),
  minCount_(iConfig.exists("minCount") ? iConfig.getParameter<int>("minCount") : 0),
  maxCount_(iConfig.exists("maxCount") ? iConfig.getParameter<int>("maxCount") : 0)
{
  // to verify no duplicate entries
  std::set<std::string> allBranches;
  // the count
  std::string countBranch = collectionName + "_count";
  allBranches.insert(countBranch);
  collectionCountBranch_ = tree->Branch(countBranch.c_str(), &collectionCount_);
  // the functions
  for ( auto functionName : branches_.getParameterNames() ) {
    auto functionParams = branches_.getParameter<std::vector<std::string> >(functionName);
    auto functionString = functionParams[0];
    auto functionType = functionParams[1];
    auto branchName = collectionName + "_" + functionName;
    if (allBranches.count(branchName)) {
        throw cms::Exception("DuplicatedBranch")
            << "Branch name \"" << branchName <<"\" already added to ntuple." << std::endl;
    }
    if (functionType=='F') {
      floatFunctions_.emplace_back(new VertexCollectionFloatFunction(tree, branchName, functionString, maxCount_));
    }
    else if (functionType=='I') {
      intFunctions_.emplace_back(new VertexCollectionIntFunction(tree, branchName, functionString, maxCount_));
    }
    allBranches.insert(branchName);
  }
}

void VertexCollectionBranches::fill(const edm::Event& iEvent)
{
  edm::Handle<reco::VertexCollection> candidates;
  iEvent.getByToken(collectionToken_, candidates);

  collectionCount_ = candidates->size();

  for ( auto& f : floatFunctions_ ) {
    f->evaluate(*candidates);
  }
  for ( auto& f : intFunctions_ ) {
    f->evaluate(*candidates);
  }
}
