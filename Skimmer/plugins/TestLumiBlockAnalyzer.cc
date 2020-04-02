// -*- C++ -*-
//
// Package:    TestLumiBlockAnalyzer/TestLumiBlockAnalyzer
// Class:      TestLumiBlockAnalyzer
// 
/**\class TestLumiBlockAnalyzer TestLumiBlockAnalyzer.cc TestLumiBlockAnalyzer/TestLumiBlockAnalyzer/plugins/TestLumiBlockAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Devin Taylor
//         Created:  Fri, 27 Oct 2017 19:05:32 GMT
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class TestLumiBlockAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources,
                                                 edm::one::WatchLuminosityBlocks>  {
   public:
      explicit TestLumiBlockAnalyzer(const edm::ParameterSet&);
      ~TestLumiBlockAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void beginLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const&) override;
      virtual void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
      virtual void endLuminosityBlock(edm::LuminosityBlock const& iEvent, edm::EventSetup const&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<int> neventsToken_;
      edm::EDGetTokenT<float> summedWeightsToken_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TestLumiBlockAnalyzer::TestLumiBlockAnalyzer(const edm::ParameterSet& iConfig):
  neventsToken_(consumes<int, edm::InLumi>(iConfig.getParameter<edm::InputTag>("nevents"))),
  summedWeightsToken_(consumes<float, edm::InLumi>(iConfig.getParameter<edm::InputTag>("summedWeights")))
{
   //now do what ever initialization is needed

}


TestLumiBlockAnalyzer::~TestLumiBlockAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TestLumiBlockAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

}


// ------------ method called once each job just before starting event loop  ------------
void 
TestLumiBlockAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TestLumiBlockAnalyzer::endJob() 
{
}

void TestLumiBlockAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const& Lumi, edm::EventSetup const& iSetup) { }

void TestLumiBlockAnalyzer::endLuminosityBlock(edm::LuminosityBlock const& Lumi, edm::EventSetup const& iSetup) {
    edm::Handle<int> neventsHandle;
    Lumi.getByToken(neventsToken_, neventsHandle);
    edm::Handle<float> summedWeightsHandle;
    Lumi.getByToken(summedWeightsToken_, summedWeightsHandle);
    std::cout << "nevents: " << *neventsHandle << " summedWeights: " << *summedWeightsHandle << std::endl;

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestLumiBlockAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestLumiBlockAnalyzer);
