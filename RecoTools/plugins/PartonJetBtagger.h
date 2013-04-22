// -*- C++ -*-
//
// Package:    PATMuonTrackVetoSelector
// Class:      PATMuonTrackVetoSelector
// 
/**\class PATMuonTrackVetoSelector PATMuonTrackVetoSelector.cc UWAnalysis/PATMuonTrackVetoSelector/src/PATMuonTrackVetoSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Michail Bachtis
//         Created:  Sun Jan 31 15:04:57 CST 2010
// $Id: PartonJetBtagger.h,v 1.1 2011/06/18 00:41:29 bachtis Exp $
//
//
#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "Math/GenVector/VectorUtil.h"
//
// class decleration
#include <boost/foreach.hpp>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/getRef.h" 
//#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

class PartonJetBtagger : public edm::EDProducer {
   public:
  
  

  explicit PartonJetBtagger(const edm::ParameterSet& iConfig):
  src_(iConfig.getParameter<edm::InputTag>("src")),
  srcPartonsSt2_(iConfig.getParameter<edm::InputTag>("srcPartonsSt2")),
  srcPartonsSt3_(iConfig.getParameter<edm::InputTag>("srcPartonsSt3"))

  {
    produces<pat::JetCollection>();
  }
  
  ~PartonJetBtagger() {}
   private:

  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    using namespace edm;
    using namespace reco;
    using namespace std;
    std::auto_ptr<pat::JetCollection> jets(new pat::JetCollection);
    Handle<reco::GenJetCollection > cands;
    edm::Handle <GenParticleRefVector> partonsSt3;
    edm::Handle <GenParticleRefVector> partonsSt2;;
	bool st2NotFound=false, st3NotFound=false;
    if(!iEvent.getByLabel(srcPartonsSt2_, partonsSt2)) st2NotFound=true;
    if(!iEvent.getByLabel(srcPartonsSt3_, partonsSt3)) st3NotFound=true; 

	

    if(iEvent.getByLabel(src_,cands)) 
      for(unsigned int  i=0;i!=cands->size();++i){
	pat::Jet jet = cands->at(i);
	float dRSt3=1e6;
	float dRSt2=1e6;
        std::vector<double> partonsSt2_PT;
        std::vector<double> partonsSt2_ETA;
        std::vector<double> partonsSt2_PHI;
        double matchedSt2=0;
	double secondJetSt2Pt=-1;
	double leadJetSt2Pt=-1;

        std::vector<double> partonsSt3_PT;
        std::vector<double> partonsSt3_ETA;
        std::vector<double> partonsSt3_PHI;
        double matchedSt3=0;	
        double secondJetSt3Pt=-1;
        double leadJetSt3Pt=-1;

	
	  if(!st2NotFound)
              for( size_t m = 0; m != partonsSt2->size(); ++ m ) {
                const GenParticle & genpart = *(partonsSt2->at(m).get());
//   	        cout<<"found?"<<genpart.pdgId()<<endl;
		if(abs(genpart.pdgId())!=5)  continue;
		if(genpart.pt()<5) continue;
		double dR=deltaR(jet.eta(), jet.phi(), genpart.eta(), genpart.phi());
		if(dR<0.3) {
				matchedSt2++;
				partonsSt2_PT.push_back(genpart.pt());
				partonsSt2_ETA.push_back(genpart.eta());
				partonsSt2_PHI.push_back(genpart.phi());
				if(genpart.pt()>leadJetSt2Pt) {secondJetSt2Pt=leadJetSt2Pt; leadJetSt2Pt=genpart.pt();}
				else if(genpart.pt()>secondJetSt2Pt) secondJetSt2Pt=genpart.pt();
		}
		if(dR<dRSt2) dRSt2=dR;
	  }

          if(!st3NotFound)
          for( size_t i = 0; i < partonsSt3->size(); ++ i ) {
                const reco::GenParticle& genpart = *(partonsSt3->at(i).get());
		
	                if(abs(genpart.pdgId())!=5)  continue;
                if(genpart.pt()<5) continue;
                double dR=deltaR(jet.eta(), jet.phi(), genpart.eta(), genpart.phi());
                if(dR<0.3) {
                                matchedSt3++;
                                partonsSt3_PT.push_back(genpart.pt());
                                partonsSt3_ETA.push_back(genpart.eta());
                                partonsSt3_PHI.push_back(genpart.phi());
                                if(genpart.pt()>leadJetSt3Pt) {secondJetSt3Pt=leadJetSt3Pt; leadJetSt3Pt=genpart.pt(); }
                                else if(genpart.pt()>secondJetSt3Pt) secondJetSt3Pt=genpart.pt();
                }
                if(dR<dRSt3) dRSt3=dR;
          }
	/*if(matchedSt2>0){
	std::cout<<"Btagged Gen Jet!"<<std::endl;
        std::cout<<"Pt "<<jet.pt()<<"   "<<matchedSt2<<std::endl;
	}
	*/
	jet.addUserFloat("matchedBsSt2",matchedSt2);
	jet.addUserFloat("minDRSt2",dRSt2);
	jet.addUserFloat("leadJetSt2Pt",leadJetSt2Pt);
        jet.addUserFloat("secondJetSt2Pt",secondJetSt2Pt);

//	jet.addUserData<std::vector<double> >("partonSt2Pt",partonsSt2_PT);
        jet.addUserFloat("matchedBsSt3",matchedSt3);
        jet.addUserFloat("minDRSt3",dRSt3);
  //      jet.addUserData<std::vector<double> >("partonSt3Pt",partonsSt3_PT);
        jet.addUserFloat("leadJetSt3Pt",leadJetSt3Pt);
        jet.addUserFloat("secondJetSt3Pt",secondJetSt3Pt);


	jets->push_back(jet);

      }  
    
    iEvent.put(jets);

  } 

      // ----------member data ---------------------------
      edm::InputTag src_;
      edm::InputTag srcPartonsSt2_;
      edm::InputTag srcPartonsSt3_;

};

