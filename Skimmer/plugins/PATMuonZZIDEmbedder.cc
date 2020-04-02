//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//   PATMuonZZIDEmbedder.cc                                                 //
//                                                                          //
//   Embeds muon ID and isolation decisions as userfloats                   //
//       (1 for true, 0 for false), for use in other modules using          //
//       HZZ4l2015 definitions.                                             //
//                                                                          //
//   Author: Nate Woods, U. Wisconsin                                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


// system includes
#include <memory>
#include <vector>
#include <iostream>

// CMS includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


class PATMuonZZIDEmbedder : public edm::stream::EDProducer<>
{
public:
  explicit PATMuonZZIDEmbedder(const edm::ParameterSet&);
  ~PATMuonZZIDEmbedder() {}


private:
  // Methods
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);

  bool passKinematics(const edm::Ptr<pat::Muon>& mu) const;
  bool passVertex(const edm::Ptr<pat::Muon>& mu) const;
  bool passType(const edm::Ptr<pat::Muon>& mu) const;
  bool passTrackerHighPtID(const edm::Ptr<pat::Muon>& mu) const;

  // Data
  edm::EDGetTokenT<edm::View<pat::Muon> > muonCollectionToken_;
  std::string idLabel_; // label for the decision userfloat
  std::string isoLabel_;
  const edm::EDGetTokenT<reco::VertexCollection> vtxSrcToken_; // primary vertex (for veto PV and SIP cuts)
  edm::Handle<reco::VertexCollection> vertices;
  std::unique_ptr<std::vector<pat::Muon> > out; // Collection we'll output at the end

  double ptCut;
  double etaCut;
  double sipCut;
  double pvDXYCut;
  double pvDZCut;

};


// Constructors and destructors

PATMuonZZIDEmbedder::PATMuonZZIDEmbedder(const edm::ParameterSet& iConfig):
  muonCollectionToken_(consumes<edm::View<pat::Muon> >(iConfig.exists("src") ? 
						       iConfig.getParameter<edm::InputTag>("src") :
						       edm::InputTag("slimmedMuons"))),
  idLabel_(iConfig.exists("idLabel") ?
	   iConfig.getParameter<std::string>("idLabel") :
	   std::string("HZZ4lIDPass")),
  isoLabel_(iConfig.exists("isoLabel") ?
	   iConfig.getParameter<std::string>("isoLabel") :
	   std::string("HZZ4lIsoPass")),
  vtxSrcToken_(consumes<reco::VertexCollection>(iConfig.exists("vtxSrc") ? 
                                                iConfig.getParameter<edm::InputTag>("vtxSrc") : 
                                                edm::InputTag("selectedPrimaryVertex"))),
  ptCut(iConfig.exists("ptCut") ? iConfig.getParameter<double>("ptCut") : 5.),
  etaCut(iConfig.exists("etaCut") ? iConfig.getParameter<double>("etaCut") : 2.4),
  sipCut(iConfig.exists("sipCut") ? iConfig.getParameter<double>("sipCut") : 4.),
  pvDXYCut(iConfig.exists("pvDXYCut") ? iConfig.getParameter<double>("pvDXYCut") : 0.5),
  pvDZCut(iConfig.exists("pvDZCut") ? iConfig.getParameter<double>("pvDZCut") : 1.)
{
  produces<std::vector<pat::Muon> >();
}


void PATMuonZZIDEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  out = std::unique_ptr<std::vector<pat::Muon> >(new std::vector<pat::Muon>);

  edm::Handle<edm::View<pat::Muon> > muonsIn;
  iEvent.getByToken(muonCollectionToken_, muonsIn);

  iEvent.getByToken(vtxSrcToken_,vertices);


  for(edm::View<pat::Muon>::const_iterator mi = muonsIn->begin();
      mi != muonsIn->end(); mi++) // loop over muons
    {
      const edm::Ptr<pat::Muon> mptr(muonsIn, mi - muonsIn->begin());

      out->push_back(*mi); // copy muon to save correctly in event

      bool vtxResult = passVertex(mptr);
      bool kinResult = passKinematics(mptr);
      bool typeResult = passType(mptr);
      bool idResultNoVtx = kinResult && typeResult;
      bool idResult = idResultNoVtx && vtxResult;
      out->back().addUserInt(idLabel_, int(idResult)); // 1 for true, 0 for false
      out->back().addUserInt(idLabel_+"NoVtx", int(idResultNoVtx)); // 1 for true, 0 for false

      out->back().addUserInt(idLabel_+"PF", int(idResult && mi->isPFMuon())); // 1 for true, 0 for false
      out->back().addUserInt(idLabel_+"PFNoVtx", int(idResultNoVtx && mi->isPFMuon())); // 1 for true, 0 for false

      bool trackerHighPtID = passTrackerHighPtID(mptr);

      out->back().addUserInt(idLabel_+"HighPt", int(idResult && trackerHighPtID));
      out->back().addUserInt(idLabel_+"Tight", int(idResult && (mi->isPFMuon() || trackerHighPtID)));
      out->back().addUserInt(idLabel_+"HighPtNoVtx", int(idResultNoVtx && trackerHighPtID));
      out->back().addUserInt(idLabel_+"TightNoVtx", int(idResultNoVtx && (mi->isPFMuon() || trackerHighPtID)));
    }

  iEvent.put(std::move(out));
}


bool PATMuonZZIDEmbedder::passKinematics(const edm::Ptr<pat::Muon>& mu) const
{
  bool result = (mu->pt() > ptCut);
  result = (result && fabs(mu->eta()) < etaCut);
  return result;
}


bool PATMuonZZIDEmbedder::passVertex(const edm::Ptr<pat::Muon>& mu) const
{
  if(!vertices->size())
    return false;

  return (fabs(mu->dB(pat::Muon::PV3D))/mu->edB(pat::Muon::PV3D) < sipCut && 
	  fabs(mu->muonBestTrack()->dxy(vertices->at(0).position())) < pvDXYCut &&
	  fabs(mu->muonBestTrack()->dz(vertices->at(0).position())) < pvDZCut);
}


bool PATMuonZZIDEmbedder::passType(const edm::Ptr<pat::Muon>& mu) const
{
  // Global muon or (arbitrated) tracker muon
  return (mu->isGlobalMuon() || (mu->isTrackerMuon() && mu->numberOfMatchedStations() > 0)) && mu->muonBestTrackType() != 2;
}


bool PATMuonZZIDEmbedder::passTrackerHighPtID(const edm::Ptr<pat::Muon>& mu) const
{
  if(!vertices->size())
    return false;

  return (mu->pt() > 200. &&
          mu->isTrackerMuon() &&
          mu->numberOfMatchedStations() > 1 &&
          mu->dB() < 0.2 &&
          fabs(mu->muonBestTrack()->dz(vertices->at(0).position())) < 0.5 &&
          mu->innerTrack()->hitPattern().numberOfValidPixelHits() > 0 &&
          mu->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
          mu->muonBestTrack()->ptError() / mu->muonBestTrack()->pt() < 0.3);
}


//define this as a plug-in
DEFINE_FWK_MODULE(PATMuonZZIDEmbedder);









