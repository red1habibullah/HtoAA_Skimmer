#include "HtoAA_Skimmer/Skimmer/interface/TriggerBranches.h"

TriggerBranches::TriggerBranches(TTree * tree, const edm::ParameterSet& iConfig, edm::ConsumesCollector cc):
    triggerBitsToken_(cc.consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"))),
    filterBitsToken_(cc.consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("filterResults"))),
    triggerObjectsToken_(cc.consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
    triggerPrescalesToken_(cc.consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("triggerPrescales"))),
    triggerBranches_(iConfig.getParameter<edm::ParameterSet>("triggerBranches")),
    filterBranches_(iConfig.getParameter<edm::ParameterSet>("filterBranches")),
    customFilterBranches_(iConfig.getParameter<edm::ParameterSet>("customFilterBranches"))
{
    // get trigger parameters
    triggerBranchStrings_.push_back("Pass");
    triggerBranchStrings_.push_back("Prescale");
    triggerNames_ = triggerBranches_.getParameterNames();
    for (auto trig : triggerNames_) {
        edm::ParameterSet trigPSet = triggerBranches_.getParameter<edm::ParameterSet>(trig);
        std::string trigString = trigPSet.getParameter<std::string>("path");
        triggerNamingMap_.insert(std::pair<std::string, std::string>(trig,trigString));
    }
    // get filter parameters
    filterNames_ = filterBranches_.getParameterNames();
    for (auto trig : filterNames_) {
        edm::ParameterSet trigPSet = filterBranches_.getParameter<edm::ParameterSet>(trig);
        std::string trigString = trigPSet.getParameter<std::string>("path");
        triggerNamingMap_.insert(std::pair<std::string, std::string>(trig,trigString));
    }

    customFilterNames_ = customFilterBranches_.getParameterNames();
    for (auto trig : customFilterNames_) {
        edm::ParameterSet trigPSet = customFilterBranches_.getParameter<edm::ParameterSet>(trig);
        edm::EDGetTokenT<bool> trigToken = cc.consumes<bool>(trigPSet.getParameter<edm::InputTag>("inputTag"));
        customFilterMap_.insert(std::pair<std::string, edm::EDGetTokenT<bool>>(trig,trigToken));
    }

    // add triggers
    for (auto trigName : triggerNames_) {
        for (auto branch : triggerBranchStrings_) {
            std::string branchName = trigName + branch;
            Int_t branchVal;
            triggerIntMap_.insert(std::pair<std::string, Int_t>(branchName,branchVal));
            std::string branchLeaf = branchName + "/I";
            tree->Branch(branchName.c_str(), &triggerIntMap_[branchName], branchLeaf.c_str());
        }
    }

    // add filters
    for (auto trigName : filterNames_) {
        Int_t branchVal;
        triggerIntMap_.insert(std::pair<std::string, Int_t>(trigName,branchVal));
        std::string branchLeaf = trigName + "/I";
        tree->Branch(trigName.c_str(), &triggerIntMap_[trigName], branchLeaf.c_str());
    }

    for (auto trigName : customFilterNames_) {
        Int_t branchVal;
        triggerIntMap_.insert(std::pair<std::string, Int_t>(trigName,branchVal));
        std::string branchLeaf = trigName + "/I";
        tree->Branch(trigName.c_str(), &triggerIntMap_[trigName], branchLeaf.c_str());
    }
}

size_t TriggerBranches::GetTriggerBit(std::string trigName, const edm::TriggerNames& names) {
    std::string trigPathString = triggerNamingMap_[trigName];
    std::regex regexp(trigPathString);
    size_t trigBit = names.size();
    for (size_t i=0; i<names.size(); i++) {
        if (std::regex_match(names.triggerName(i), regexp)) {
            if (trigBit != names.size()) { // if we match more than one
                throw cms::Exception("DuplicateTrigger")
                    << "Second trigger matched for \"" << trigPathString
                    << "\". First: \"" << names.triggerName(trigBit)
                    << "\"; second: \"" << names.triggerName(i) << "\"." << std::endl;
            }
            trigBit = i;
        }
    }
    if (trigBit == names.size()) {
        return 9999;
        //throw cms::Exception("UnrecognizedTrigger")
        //    << "No trigger matched for \"" << trigPathString << "\"." << std::endl;
    }
    return trigBit;
}


void TriggerBranches::fill(const edm::Event& iEvent)
{
    iEvent.getByToken(triggerBitsToken_, triggerBits_);
    iEvent.getByToken(filterBitsToken_, filterBits_);
    iEvent.getByToken(triggerObjectsToken_, triggerObjects_);
    iEvent.getByToken(triggerPrescalesToken_, triggerPrescales_);

    // triggers
    const edm::TriggerNames& names = iEvent.triggerNames(*triggerBits_);

    for (auto trigName : triggerNames_) {
        size_t trigBit = TriggerBranches::GetTriggerBit(trigName,names);
        std::string passString = trigName + "Pass";
        std::string prescaleString = trigName + "Prescale";
        if (trigBit==9999) {
            triggerIntMap_[passString] = -1;
            triggerIntMap_[prescaleString] = -1;
        }
        else {
            triggerIntMap_[passString] = triggerBits_->accept(trigBit);
            triggerIntMap_[prescaleString] = triggerPrescales_->getPrescaleForIndex(trigBit);
        }
    }

    // filters
    const edm::TriggerNames& filters = iEvent.triggerNames(*filterBits_);

    for (auto trigName : filterNames_) {
        size_t trigBit = TriggerBranches::GetTriggerBit(trigName,filters);
        if (trigBit==9999) {
            triggerIntMap_[trigName] = -1;
        }
        else {
            triggerIntMap_[trigName] = filterBits_->accept(trigBit);
        }
    }

    for (auto trigName : customFilterNames_) {
        edm::Handle<bool> trigHandle;
        iEvent.getByToken(customFilterMap_[trigName],trigHandle);
        if (trigHandle.isValid()) {
            bool result = *trigHandle;
            triggerIntMap_[trigName] = (int)result;
        }
        else {
            triggerIntMap_[trigName] = -1;
        }
    }
}

