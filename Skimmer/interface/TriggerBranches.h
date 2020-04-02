#include <regex>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

#include "TTree.h"

class TriggerBranches {
  public:
    TriggerBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc);
    void fill(const edm::Event& iEvent);

  private:
    size_t GetTriggerBit(std::string trigName, const edm::TriggerNames& names);

    // tokens
    edm::EDGetTokenT<edm::TriggerResults> triggerBitsToken_;
    edm::EDGetTokenT<edm::TriggerResults> filterBitsToken_;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsToken_;
    edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescalesToken_;

    // handles
    edm::Handle<edm::TriggerResults> triggerBits_;
    edm::Handle<edm::TriggerResults> filterBits_;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects_;
    edm::Handle<pat::PackedTriggerPrescales> triggerPrescales_;

    // branch parameters
    edm::ParameterSet triggerBranches_;
    edm::ParameterSet filterBranches_;
    edm::ParameterSet customFilterBranches_;

    std::vector<std::string>           triggerNames_;
    std::vector<std::string>           filterNames_;
    std::vector<std::string>           customFilterNames_;
    std::vector<std::string>           triggerBranchStrings_;
    std::map<std::string, std::string> triggerNamingMap_;
    std::map<std::string, Int_t>       triggerIntMap_;
    std::map<std::string, edm::EDGetTokenT<bool>> customFilterMap_;
};
