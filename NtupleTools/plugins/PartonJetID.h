// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"


class PartonJetID : public NtupleFillerBase {
 public:

    PartonJetID(){
    }

    PartonJetID(const edm::ParameterSet& iConfig, TTree* t):
    src_(iConfig.getParameter<edm::InputTag>("src")),
    jetPtMin_(iConfig.getParameter<double>("ptMin")),
    jetEtaMax_(iConfig.getParameter<double>("etaMax"))
	{
        BBbarPartonJets=-1;
        BbarPartonJets=-1;
        BPartonJets=-1;
        CCbarPartonJets=-1;
        CbarPartonJets=-1;
        CPartonJets=-1;

        t->Branch("BBbarPartonJets",&BBbarPartonJets,"BBbarPartonJets/F");
	t->Branch("BbarPartonJets",&BbarPartonJets,"BbarPartonJets/F");
        t->Branch("BPartonJets",&BPartonJets,"BPartonJets/F");
        t->Branch("CCbarPartonJets",&CCbarPartonJets,"CCbarPartonJets/F");
        t->Branch("CbarPartonJets",&CbarPartonJets,"CbarPartonJets/F");
        t->Branch("CPartonJets",&CPartonJets,"CPartonJets/F");

	}
      
      ~PartonJetID()
	{ 
	  
	}
      


  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup)
  {
    using namespace std; 
	BBbarPartonJets=0;
        BbarPartonJets=0;
        BPartonJets=0;

        CCbarPartonJets=0;
        CbarPartonJets=0;
        CPartonJets=0;

	edm::Handle<edm::View<reco::GenJet> > pGenJet;
  	if(iEvent.getByLabel(src_, pGenJet)){

	  for( size_t i = 0; i < pGenJet->size(); ++ i ) {
		int partonJB=0, partonJC=0, partonJBbar=0, partonJCbar=0, partonJCCbar=0, partonJBBbar;
                const reco::GenJet& genjet = (*pGenJet)[i];
		 if(genjet.pt()>jetPtMin_ && fabs(genjet.eta())<jetEtaMax_){
		       int nConstGEN=genjet.nConstituents();
                       for(int i=0; i<nConstGEN; i++){
                             const reco::Candidate* cand=genjet.getGenConstituent(i);
				if(cand->pdgId()==-5) partonJBbar++; 
                                if(cand->pdgId()==-4) partonJCbar++;
				if(cand->pdgId()==5) partonJB++;
                                if(cand->pdgId()==4) partonJC++;
			}

			if((partonJB-partonJBbar)!=0&&(partonJB-partonJBbar)%2==0) BBbarPartonJets++;
			else if ( (partonJB-partonJBbar)>0) BPartonJets++;
			else if ( (partonJB-partonJBbar)<0) BbarPartonJets++;

                        if((partonJC-partonJCbar)!=0&&(partonJC-partonJCbar)%2==0) CCbarPartonJets++;
                        else if ( (partonJC-partonJCbar)>0) CPartonJets++;
                        else if ( (partonJC-partonJCbar)<0) CbarPartonJets++;
           	  }
		
	
	}	  


    }
   


    }
  
  

 protected:
	edm::InputTag src_;
	double jetPtMin_;
	double jetEtaMax_;
	
	float BBbarPartonJets;
        float BbarPartonJets;
        float BPartonJets;
        float CCbarPartonJets;
        float CbarPartonJets;
        float CPartonJets;

};






