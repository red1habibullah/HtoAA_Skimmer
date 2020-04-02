#include "HtoAA_Skimmer/Skimmer/interface/CandidateCollectionBranches.h"

template<typename T>
CandidateCollectionFunction<T>::CandidateCollectionFunction(TTree * tree, std::string functionName, std::string functionString, int maxCount):
  function_(functionString),
  functionString_(functionString),
  functionName_(functionName),
  vectorBranch_(tree->Branch(functionName.c_str(), &values_)),
  maxCount_(maxCount)
{
}

template<typename T>
JetConstituentCollectionFunction<T>::JetConstituentCollectionFunction(TTree * tree, std::string functionName, std::string functionString, int maxCount, int constituentMaxCount):
  function_(functionString),
  functionString_(functionString),
  functionName_(functionName),
  vectorBranch_(tree->Branch(functionName.c_str(), &values_)),
  maxCount_(maxCount),
  constituentMaxCount_(constituentMaxCount)
{
}

template<typename T>
void CandidateCollectionFunction<T>::evaluate(const reco::CandidateView& candidates)
{
  values_.clear();
  try {
    int i = 0;
    for (const auto& candidate: candidates) {
      if (maxCount_>0 && i>=maxCount_) break;
      values_.push_back(function_(candidate));
      i++;
    }
  } catch(cms::Exception& iException) {
    iException << "Caught exception in evaluating branch: "
    << functionName_ << " with formula: " << functionString_;
    throw;
  }
}

template<typename T>
void JetConstituentCollectionFunction<T>::evaluate(const edm::View<pat::Jet>& candidates)
{
  values_.clear();
  try {
    int i = 0;
    for (const auto& candidate: candidates) {
      if (maxCount_>0 && i>=maxCount_) break;
      std::vector<T> v;
      for (auto it : candidate.daughterPtrVector()) {
        if (constituentMaxCount_>0 && i>=constituentMaxCount_) break;
        v.push_back(function_(*it));
      }
      values_.push_back(v);
      i++;
    }
  } catch(cms::Exception& iException) {
    iException << "Caught exception in evaluating branch: "
    << functionName_ << " with formula: " << functionString_;
    throw;
  }
}

CandidateCollectionBranches::CandidateCollectionBranches(TTree * tree, std::string collectionName,  const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
  collectionToken_(cc.consumes<reco::CandidateView>(iConfig.getParameter<edm::InputTag>("collection"))),
  branches_(iConfig.getParameter<edm::ParameterSet>("branches")),
  collectionName_(collectionName),
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
      floatFunctions_.emplace_back(new CandidateCollectionFloatFunction(tree, branchName, functionString, maxCount_));
    }
    else if (functionType=='I') {
      intFunctions_.emplace_back(new CandidateCollectionIntFunction(tree, branchName, functionString, maxCount_));
    }
    allBranches.insert(branchName);
  }
}

JetCandidateCollectionBranches::JetCandidateCollectionBranches(TTree * tree, std::string collectionName, std::string constituentCollectionName,  const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
  collectionToken_(cc.consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("collection"))),
  branches_(iConfig.getParameter<edm::ParameterSet>("branches")),
  constituentBranches_(iConfig.getParameter<edm::ParameterSet>("constituentBranches")),
  collectionName_(collectionName),
  constituentCollectionName_(constituentCollectionName),
  minCount_(iConfig.exists("minCount") ? iConfig.getParameter<int>("minCount") : 0),
  maxCount_(iConfig.exists("maxCount") ? iConfig.getParameter<int>("maxCount") : 0),
  constituentMinCount_(iConfig.exists("constituentMinCount") ? iConfig.getParameter<int>("constituentMinCount") : 0),
  constituentMaxCount_(iConfig.exists("constituentMaxCount") ? iConfig.getParameter<int>("constituentMaxCount") : 0)
{
  // to verify no duplicate entries
  std::set<std::string> allBranches;
  std::set<std::string> allConstituentBranches;
  // the count
  std::string countBranch = collectionName + "_count";
  std::string constituentCountBranch = constituentCollectionName + "_count";
  allBranches.insert(countBranch);
  allBranches.insert(constituentCountBranch);
  collectionCountBranch_ = tree->Branch(countBranch.c_str(), &collectionCount_);
  constituentCollectionCountBranch_ = tree->Branch(constituentCountBranch.c_str(), &constituentCollectionCount_);
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
      floatFunctions_.emplace_back(new CandidateCollectionFloatFunction(tree, branchName, functionString, maxCount_));
    }
    else if (functionType=='I') {
      intFunctions_.emplace_back(new CandidateCollectionIntFunction(tree, branchName, functionString, maxCount_));
    }
    allBranches.insert(branchName);
  }
  // constituent functions
  for ( auto functionName : constituentBranches_.getParameterNames() ) {
    auto functionParams = constituentBranches_.getParameter<std::vector<std::string> >(functionName);
    auto functionString = functionParams[0];
    auto functionType = functionParams[1];
    auto branchName = collectionName + "_" + constituentCollectionName + "_" + functionName;
    if (allBranches.count(branchName)) {
        throw cms::Exception("DuplicatedBranch")
            << "Branch name \"" << branchName <<"\" already added to ntuple." << std::endl;
    }
    if (functionType=='F') {
      constituentFloatFunctions_.emplace_back(new JetConstituentCollectionFloatFunction(tree, branchName, functionString, maxCount_, constituentMaxCount_));
    }
    else if (functionType=='I') {
      constituentIntFunctions_.emplace_back(new JetConstituentCollectionIntFunction(tree, branchName, functionString, maxCount_, constituentMaxCount_));
    }
    allBranches.insert(branchName);
  }
}

void CandidateCollectionBranches::fill(const edm::Event& iEvent)
{
  edm::Handle<reco::CandidateView> candidates;
  iEvent.getByToken(collectionToken_, candidates);

  collectionCount_ = candidates->size();

  for ( auto& f : floatFunctions_ ) {
    f->evaluate(*candidates);
  }
  for ( auto& f : intFunctions_ ) {
    f->evaluate(*candidates);
  }
}

void JetCandidateCollectionBranches::fill(const edm::Event& iEvent)
{
  edm::Handle<edm::View<pat::Jet> > candidates;
  iEvent.getByToken(collectionToken_, candidates);

  collectionCount_ = candidates->size();

  for ( auto& f : floatFunctions_ ) {
    f->evaluate((reco::CandidateView&)*candidates);
  }
  for ( auto& f : intFunctions_ ) {
    f->evaluate((reco::CandidateView&)*candidates);
  }

  // the constiuents
  constituentCollectionCount_.clear();
  for (auto it : *candidates ) {
    constituentCollectionCount_.push_back(it.numberOfDaughters());
  }

  for ( auto& f : constituentFloatFunctions_ ) {
    f->evaluate(*candidates);
  }
  for ( auto& f : constituentIntFunctions_ ) {
    f->evaluate(*candidates);
  }

}
