//
// Select the partons status 2 and 3 for MC Jet Flavour 
// Author: Attilio
// Date: 10.10.2007
//
    
//=======================================================================

// user include files
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"  
//#include "DataFormats/Candidate/interface/CandidateFwd.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
      
#include <memory>
#include <string>
#include <iostream>
#include <vector>
         
using namespace std;
using namespace reco;
using namespace edm;

class MyPartonSelector : public edm::EDProducer 
{
  public:
    MyPartonSelector( const edm::ParameterSet & );
    ~MyPartonSelector();

  private:

    virtual void produce(edm::Event&, const edm::EventSetup& ); 
    bool withStatus3;  // Optionally switch off 3
    bool withStatus2;  // Optionally switch off 2
    bool withLeptons;  // Optionally specify leptons
    bool withTop;      // Optionally include top quarks in the list 
    edm::InputTag   inputTagGenParticles_; // input collection
};
//=========================================================================

MyPartonSelector::MyPartonSelector( const edm::ParameterSet& iConfig )
{ 
    produces<reco::GenParticleRefVector>();
    withLeptons           = iConfig.getParameter<bool>("withLeptons"); 
    inputTagGenParticles_ = iConfig.getParameter<edm::InputTag>("src");
    if ( iConfig.exists("withTop") ) {
      withTop = iConfig.getParameter<bool>("withTop");
    } else {
      withTop = false;
    }
    if ( iConfig.exists("withStatus2") ) {
      withStatus2 = iConfig.getParameter<bool>("withStatus2");
    } else {
      withStatus2 = true;
    }
    if ( iConfig.exists("withStatus3") ) {
      withStatus3 = iConfig.getParameter<bool>("withStatus3");
    } else {
      withStatus3 = true;
    }


}

//=========================================================================

MyPartonSelector::~MyPartonSelector()
{
}

// ------------ method called to produce the data  ------------

void MyPartonSelector::produce( Event& iEvent, const EventSetup& iEs )
{
  
  //edm::Handle <reco::CandidateView> particles;
  edm::Handle <reco::GenParticleCollection> particles;
  iEvent.getByLabel (inputTagGenParticles_, particles );  
  edm::LogVerbatim("MyPartonSelector") << "=== GenParticle size:" << particles->size();
  int nPart=0; 

  auto_ptr<GenParticleRefVector> thePartons ( new GenParticleRefVector);  

  
  for (size_t m = 0; m < particles->size(); m++) {
    
    // Don't take into account first 6 particles in generator list
    if (m<6) continue;    

    const GenParticle & aParticle = (*particles)[ m ];

    bool isAParton = false;
    bool isALepton = false;
    int flavour = abs(aParticle.pdgId());
    if(flavour == 1 ||
       flavour == 2 ||
       flavour == 3 ||
       flavour == 4 ||
       flavour == 5 ||  
       (flavour == 6 && withTop) ||
       flavour == 21 ) isAParton = true;
    if(flavour == 11 ||
       flavour == 12 ||
       flavour == 13 ||
       flavour == 14 ||
       flavour == 15 ||
       flavour == 16 ) isALepton = true;


    //Add Partons status 3
	if( withStatus3==true){
    if( aParticle.status() == 3 && isAParton ) {
	 edm::LogVerbatim("MyPartonSelector")<<"Filling GEN PARTICLES  ---> "<<flavour<<"||"<< aParticle.status();
      thePartons->push_back( GenParticleRef( particles, m ) );
      nPart++;
    }
	}

    //Add Partons status 2
    int nparton_daughters = 0;
    if( withStatus2==true&&aParticle.numberOfDaughters() > 0 && isAParton ) {
      
      for (unsigned int i=0; i < aParticle.numberOfDaughters(); i++){
	
	int daughterFlavour = abs(aParticle.daughter(i)->pdgId());
	if( (daughterFlavour == 1 || daughterFlavour == 2 || daughterFlavour == 3 || 
	     daughterFlavour == 4 || daughterFlavour == 5 || daughterFlavour == 6 || daughterFlavour == 21)) {
          nparton_daughters++;
	}

      }
      if(nparton_daughters == 0){
         edm::LogVerbatim("MyPartonSelector")<<"Filling GEN PARTICLES  ---> "<<flavour<<"||"<< aParticle.status();
	  nPart++;
	  thePartons->push_back( GenParticleRef( particles, m ) );
      }

    }

    //Add Leptons
    // Here you have to decide what to do with taus....
    // Now all leptons, including e and mu from leptonic tau decays, are added
    if( withLeptons && aParticle.status() == 3 && isALepton ) {
      thePartons->push_back( GenParticleRef( particles, m ) );
      nPart++;
    }
  }
  
  edm::LogVerbatim("MyPartonSelector") << "=== GenParticle selected:" << nPart;  
  iEvent.put( thePartons );

}

DEFINE_FWK_MODULE(MyPartonSelector);

