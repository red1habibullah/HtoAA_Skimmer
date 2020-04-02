// -*- C++ -*-
//
// Package:    ElectronCleanedJetProducer
// Class:      ElectronCleanedJetProducer
// 
/**\class ElectronCleanedJetProducer MuonCleanedJetProducer.cc
   
 1;5202;0c  
   Description: Removes PF Electrons from PFJet candidates and reconstructs the jets
	          Associates those Electrons to the jets from which they were removed
		  
		  Implementation:
		  [Notes on implementation]
*/
//
// Original Author:  Francesca Ricci-Tam,6 R-025,+41227672274,
//     Contributer:  Devin Taylor,
//         Contributor: Redwan Habibullah
//         Created:  Fri Aug 31 13:01:48 CEST 2012
//
//


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "TLorentzVector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"


#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "TMath.h"

//
// class declaration
//

class ElectronCleanedJetProducer : public edm::stream::EDProducer<>
{
   public:
      explicit ElectronCleanedJetProducer(const edm::ParameterSet&);
      ~ElectronCleanedJetProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
 
  typedef edm::AssociationMap<edm::OneToMany<std::vector<reco::PFJet>, std::vector<reco::PFCandidate>, unsigned int> >
  JetToPFCandidateAssociation;     
  typedef edm::AssociationMap<edm::OneToMany<std::vector<reco::PFJet>, std::vector<reco::PFCandidate>, unsigned int> >
  JetToElectronAssociation;

  // ----------member data ---------------------------

      // source of the jets to be cleaned of electrons
  edm::EDGetTokenT<reco::PFJetCollection> jetSrc_;
  
  // source of electrons that, if found within jet, should be removed
  //edm::EDGetTokenT<reco::MuonCollection> muonSrc_;
  edm::EDGetTokenT<reco::GsfElectronRefVector> electronSrc_;
  // source of PF candidates
  edm::EDGetTokenT<reco::PFCandidateCollection> pfCandSrc_;
  edm::EDGetTokenT<edm::View<reco::PFCandidate> > pfCandToken_;
  

  edm::ParameterSet* cfg_;

  
  //int Jet_Rem_count=0;
 
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
ElectronCleanedJetProducer::ElectronCleanedJetProducer(const edm::ParameterSet& iConfig):
  jetSrc_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetSrc"))),
  //muonSrc_(consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muonSrc"))),
  electronSrc_(consumes<reco::GsfElectronRefVector>(iConfig.getParameter<edm::InputTag>("electronSrc"))),
  //electronSrc_(consumes<edm::RefVector<reco::GsfElectronCollection> >(iConfig.getParameter<edm::InputTag>("electronSrc"))),
  pfCandSrc_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCandSrc")))
  //pfCandToken_(consumes<edm::View<reco::PFCandidate> >( iConfig.getParameter<edm::InputTag>("pfCandCollection")))
  
  
  //pfCandSrc_(consumes<edm::View<reco::PFCandidate> >( iConfig.getParameter<edm::InputTag>("pfCandSrc") ))
{
  cfg_ = const_cast<edm::ParameterSet*>(&iConfig);
  
  //register your products
  produces<reco::PFJetCollection>();
  produces<edm::ValueMap<bool> >( "jetCleanedValueMap" );
  produces<JetToPFCandidateAssociation>("pfCandAssocMapForIsolation");
  produces<JetToElectronAssociation>("pfCandAssocMapForElectron");
  produces<std::vector<int> >("ElectronsPassingID");
  produces<std::vector<int> >("ElectronsRemoved");
  produces<std::vector<int> >("NumJetsElectronsCleaned");
  //produces<std::vector<int> >("ElectronNumber");
}


ElectronCleanedJetProducer::~ElectronCleanedJetProducer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ElectronCleanedJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  edm::Handle<reco::PFJetCollection> pfJets;
  iEvent.getByToken(jetSrc_, pfJets);
  std::unique_ptr<reco::PFJetCollection> SetOfJets( new reco::PFJetCollection );
  std::unique_ptr<reco::PFJetCollection> cleanedJets = std::make_unique<reco::PFJetCollection>();
  
  edm::Handle<reco::GsfElectronRefVector> electrons;
  iEvent.getByToken(electronSrc_, electrons);
  
  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByToken(pfCandSrc_, pfCands);
  
  
  std::unique_ptr<reco::PFCandidateCollection> JetPFCand= std::make_unique<reco::PFCandidateCollection>();
  edm::RefProd<reco::PFJetCollection> selectedJetRefProd = iEvent.getRefBeforePut<reco::PFJetCollection>();
  

    auto selectedJetPFCandidateAssociationForIsolation =
      std::make_unique<JetToPFCandidateAssociation>(&iEvent.productGetter());
    
    auto selectedJetElectronAssociationForCount =
      std::make_unique<JetToElectronAssociation>(&iEvent.productGetter());


  std::unique_ptr<std::vector<int> > ElectronPID(new std::vector<int>);
  std::unique_ptr<std::vector<int> > ElectronRemoved(new std::vector<int>);
  std::unique_ptr<std::vector<int> > JetElectronCleaned(new std::vector<int>);
//fill an STL container with muon ref keys
  std::vector<unsigned int> electronRefKeys;
  std::vector<int> Count;
  Count.clear();
  int E_count=0;
  int Jet_Rem_count=0;
  if (electrons.isValid()) 
  {
      for (reco::GsfElectronRefVector::const_iterator iElectron = electrons->begin(); iElectron != electrons->end(); ++iElectron)
	{
	  
	  electronRefKeys.push_back(iElectron->key());
	 
	  ++E_count;
	}
  }
  ElectronPID->push_back(E_count);
  //vector of bools holding the signal electron tag decision for each jet
  std::vector<bool> electronTagDecisions;
  for (reco::PFJetCollection::const_iterator iJet = pfJets->begin(); iJet != pfJets->end(); ++iJet)
    {
  
      std::vector<reco::PFCandidatePtr> jetPFCands = iJet->getPFConstituents();
      reco::PFJet::Specific specs = iJet->getSpecific();
      math::XYZTLorentzVector pfmomentum;
      std::vector<edm::Ptr<reco::Candidate> > jetConstituents;
      jetConstituents.clear();
      
      bool taggedElectronForRemoval = false;
      
      
      for (std::vector<edm::Ptr<reco::PFCandidate> >::iterator i = jetPFCands.begin(); i != jetPFCands.end(); ++i)
	{
	  reco::PFCandidate pfCand = *i;
       
	  
	  
	  if (pfCand.particleId() == 2) //Reference: https://cmssdt.cern.ch/SDT/doxygen/CMSSW_7_1_17/doc/html/d8/d17/PFCandidate_8h_source.html
	    {
	      //std::cout << "Found a electron to check: "<< pfCand.gsfElectronRef().key() << " " << pfCand.pt() << " " << pfCand.eta() << " " << pfCand.phi() << std::endl;
	      //++PF_count;
	    reco::GsfElectronRef theRecoElectron = pfCand.gsfElectronRef();
	    
	    //does this muon pass the desired muon ID?
	    std::vector<unsigned int>::const_iterator iElectron = std::find(electronRefKeys.begin(), electronRefKeys.end(), theRecoElectron.key());
	    
	    if (iElectron != electronRefKeys.end()) 
	      {
		specs.mElectronEnergy -= pfCand.p4().e();
		specs.mElectronMultiplicity -= 1;
		specs.mChargedEmEnergy -= pfCand.p4().e();
		specs.mChargedMultiplicity -= 1;
		//save tag decision for this muon
		taggedElectronForRemoval = true;
		//++Jet_Rem_count;
		// add this muon ref to the vector of removed muons for this jet
		// iMuon - muonRefKeys.begin() is the index into muonRefKeys of the soft muon
		// since muonRefKeys was filled in order of muons, it is also the index into 
		// muons of the soft muon
		//removedMuons.push_back(muons->at(iMuon - muonRefKeys.begin())->masterRef());
		
		//std::cout << "Found an Electron to remove: "<< pfCand.gsfElectronRef().key() << " " << pfCand.pt() << " " << pfCand.eta() << " " << pfCand.phi() << std::endl;
		
		Count.push_back(pfCand.gsfElectronRef().key());
	      }
	    else
	      {
		pfmomentum += pfCand.p4(); // total p4()
		jetConstituents.push_back((*i));
	    
	      }
	  } //If its an electron->loop
	else // if it's not a muon
	  {
	    pfmomentum += pfCand.p4(); // total p4()
	    jetConstituents.push_back((*i));
	    //cout<<"No electron in pfCands but Jet const still pushed back"<<endl;
	  }
  
      } // loop over PF candidates
    
    // Build a new jet without the muon
    reco::PFJet electronfreePFJet(pfmomentum, specs, jetConstituents);
    SetOfJets->push_back( electronfreePFJet );
    cleanedJets->push_back( electronfreePFJet );
    //if at least 1 electron was tagged for removal, save a positive muon tag decision for this jet
    electronTagDecisions.push_back(taggedElectronForRemoval);
    
    edm::Ref<reco::PFJetCollection> jetRef(selectedJetRefProd, SetOfJets->size() - 1);
    //save the ref vector of removed muons
    //removedMuonMap.push_back(removedMuons);
    for (size_t i = 0; i < pfCands->size(); ++i) 
      {
	reco::PFCandidateRef pfCandRef(pfCands,i);
	bool ElectronFlag=false;
	// loop for electrons among PfCands
	if ((*pfCands)[i].particleId() == 2) {
	  reco::GsfElectronRef removedElRef = (*pfCands)[i].gsfElectronRef(); 
	  //Find if electons in PfCands matches with the pre-determined electron collection
	  std::vector<unsigned int>::const_iterator iElectron = std::find(electronRefKeys.begin(), electronRefKeys.end(), removedElRef.key());
	  if(iElectron != electronRefKeys.end())
	  {
	    ElectronFlag=true;
	  }
	  
	
	
	if(ElectronFlag==true)
	  {
	    selectedJetElectronAssociationForCount->insert(jetRef,pfCandRef);
	  }
	}
	//Put everything except the electron into the association map
	if(!(ElectronFlag==true))
	  {
	    selectedJetPFCandidateAssociationForIsolation->insert(jetRef, pfCandRef);
	  }
	
      }
    
    if(taggedElectronForRemoval==true)
      {
	++Jet_Rem_count;
      }
    
       } // loop over jets
  
  ElectronRemoved->push_back(Count.size());
  JetElectronCleaned->push_back(Jet_Rem_count);
  //fill an STL container of keys of removed muons
  //std::vector<unsigned int> removedMuRefKeys;
//for (std::vector<reco::MuonRefVector>::const_iterator iJet = removedMuonMap.begin(); iJet != removedMuonMap.end(); ++iJet)
//{
//  for (reco::MuonRefVector::const_iterator iRemovedMu = iJet->begin(); iRemovedMu != iJet->end(); ++iRemovedMu) 
//  {
//    removedMuRefKeys.push_back(iRemovedMu->key()); 
//  }
//}
 
// build a collection of PF candidates excluding muons
// we will still tag the jet as signal-like by the presence of a muon IN the jet, but this 
// ensures that such jets also cannot have the muon enter the isolation candidate collection
//unsigned int iCand=0; 

 

  
const edm::OrphanHandle<reco::PFJetCollection> cleanedJetsRefProd = iEvent.put(std::move(cleanedJets));

//fill the value map of muon tag decision for each cleaned jet
  std::unique_ptr<edm::ValueMap<bool> > valMap(new edm::ValueMap<bool>());
  edm::ValueMap<bool>::Filler filler(*valMap);
  filler.insert(cleanedJetsRefProd, electronTagDecisions.begin(), electronTagDecisions.end());
  filler.fill();
  iEvent.put(std::move(valMap), "jetCleanedValueMap" );
  //std::cout<< " Electrons in Loose Collection: " <<  E_count  << " Electrons found among the PFCands in Jet:  " <<  PF_count  << " Electrons removed : " << Rem_count <<std::endl;  
  std::cout<< " Electrons in ID Collection: " <<  E_count<<endl;
  std::cout<< "Electrons removed " << int(Count.size())<<endl;
  std::cout<<" Jet with Electron removed: " <<Jet_Rem_count<<endl;
  //std::cout<< " Jets With Electron Cleaned: " <<  Jet_Rem_count <<endl;

  
  //fill the value map of removed muon refs for each cleaned jet
  //std::unique_ptr<edm::ValueMap<reco::MuonRefVector> > muonValMap(new edm::ValueMap<reco::MuonRefVector>());
  //edm::ValueMap<reco::MuonRefVector>::Filler muonFiller(*muonValMap);
  //muonFiller.insert(cleanedJetsRefProd, removedMuonMap.begin(), removedMuonMap.end());
  //muonFiller.fill();
  //iEvent.put(std::move(muonValMap), "cleanedMuonsRefValueMap" );

  //fill the value map of old jet refs for each cleaned jet
  //std::unique_ptr<edm::ValueMap<reco::PFJetRef> > jetValMap(new edm::ValueMap<reco::PFJetRef>());
  //edm::ValueMap<reco::PFJetRef>::Filler jetFiller(*jetValMap);
  //jetFiller.insert(cleanedJetsRefProd, oldJets.begin(), oldJets.end());
  //jetFiller.fill();
  //iEvent.put(std::move(jetValMap), "uncleanedJetRefValueMap" );
  //std::cout<< " Muons in ID Collection: " <<  Mu_count<<endl;
  //std::cout<< " Jets With Muon Cleaned: " <<  Jet_Rem_count <<endl;


  //put the soft-muon-free PF cands into the event
  iEvent.put(std::move(SetOfJets));
  //iEvent.put(std::move(pfCandsExcludingElectrons), "particleFlowElectronCleaned");
  iEvent.put(std::move(selectedJetPFCandidateAssociationForIsolation), "pfCandAssocMapForIsolation");
  iEvent.put(std::move(selectedJetElectronAssociationForCount), "pfCandAssocMapForElectron");
  iEvent.put(std::move(ElectronPID),"ElectronsPassingID");
  iEvent.put(std::move(ElectronRemoved),"ElectronsRemoved");
  iEvent.put(std::move(JetElectronCleaned),"NumJetsElectronsCleaned");

}

// ------------ method called once each job just before starting event loop  ------------
void 
ElectronCleanedJetProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronCleanedJetProducer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronCleanedJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronCleanedJetProducer);
