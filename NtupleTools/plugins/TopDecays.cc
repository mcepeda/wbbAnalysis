// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


// Stuff for gen level ID (based on a W analysis...)


class TopDecays : public NtupleFillerBase {
 public:

    TopDecays(){
    }

    TopDecays(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src"))
	{
          topToWEleNuDecay=-1, topToWMuNuDecay=-1, topToWTauNuDecay=-1, topToWJetsDecay=-1;
            t->Branch("topToWEleNuDecay",&topToWEleNuDecay,"topToWEleNuDecay/F");
            t->Branch("topToWMuNuDecay",&topToWMuNuDecay,"topToWMuNuDecay/F");
            t->Branch("topToWTauNuDecay",&topToWTauNuDecay,"topToWTauNuDecay/F");
            t->Branch("topToWJetsDecay",&topToWJetsDecay,"topToWJetsDecay/F");
            t->Branch("topToWTauNuDecayTauToMu",&topToWTauNuDecayTauToMu,"topToWTauNuDecayTauToMu/F");
            t->Branch("topToWTauNuDecayTauToEle",&topToWTauNuDecayTauToEle,"topToWTauNuDecayTauToEle/F");

	  numberOfExtraNonBJetsFromW=-1, numberOfExtraGluonSt3=-1, numberOfExtraJetsSt3=-1;
	    t->Branch("numberOfExtraNonBJetsFromW",&numberOfExtraNonBJetsFromW,"numberOfExtraNonBJetsFromW/F");	
            t->Branch("numberOfExtraGluonSt3",&numberOfExtraGluonSt3,"numberOfExtraGluonSt3/F");
            t->Branch("numberOfExtraJetsSt3",&numberOfExtraJetsSt3,"numberOfExtraJetsSt3/F");
	}
     
 
      ~TopDecays()
	{ 
	  
	}
      
  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);

 protected:
  bool verbose_;
  edm::InputTag src_;

                float topToWEleNuDecay, topToWTauNuDecay, topToWJetsDecay, topToWMuNuDecay;
                float numberOfExtraNonBJetsFromW;
                float numberOfExtraGluonSt3;
                float numberOfExtraJetsSt3;
		float topToWTauNuDecayTauToMu, topToWTauNuDecayTauToEle;


};

void TopDecays::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup)
  {
    using namespace std;

                topToWEleNuDecay=0, topToWTauNuDecay=0, topToWJetsDecay=0, topToWMuNuDecay=0;
		numberOfExtraNonBJetsFromW=0;
		numberOfExtraGluonSt3=0;	
		numberOfExtraJetsSt3=0;			
		topToWTauNuDecayTauToEle=0; topToWTauNuDecayTauToMu=0;

  edm::Handle< vector<reco::GenParticle> >pGenPart;
  if(iEvent.getByLabel(src_, pGenPart)){

          for( size_t i = 0; i < pGenPart->size(); ++ i ) {
                const reco::GenParticle& genpart = (*pGenPart)[i];

		if(genpart.status()==3) {//cout<<genpart.pdgId()<<"    --> St="<<genpart.status()<<endl;	
					if(genpart.pdgId()==21) numberOfExtraGluonSt3++;
					if(abs(genpart.pdgId())<5) numberOfExtraJetsSt3++;
					}

                if(abs(genpart.pdgId())==6){
		if(verbose_)
                cout<<"Found top!!!  "<<genpart.pdgId()<<"    --> St="<<genpart.status()<<"   ("<<genpart.numberOfDaughters()<<")"<<endl;

                 for (unsigned int j=0; j<genpart.numberOfDaughters(); j++){
                        const reco::Candidate* dau=genpart.daughter(j);
                        if(verbose_)
			cout<<"\t Dau "<<j<<"  ->"<<dau->pdgId()<<"    --> St="<<dau->status()<<"   ("<<dau->numberOfDaughters()<<")"<<endl;
			
			if(abs(dau->pdgId())<5 && abs(dau->pdgId())==21) numberOfExtraNonBJetsFromW++;  

                        for (unsigned int jj=0; jj<dau->numberOfDaughters(); jj++){
                                const reco::Candidate* granddau=dau->daughter(jj);
                                if(verbose_) 
				cout<<" \t \t GranDau "<<jj<<"  ->"<<granddau->pdgId()<<"    --> St="<<granddau->status()<<"   ("<<granddau->numberOfDaughters()<<")"<<endl;
                                
				if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==11) topToWEleNuDecay++;
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==13) topToWMuNuDecay++;
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==15) { topToWTauNuDecay++;
					bool muon=false, electron=false;
					if(verbose_) cout<<"\t \t Tau!!!!!"<<jj<<" --->"<<granddau->pdgId()<<"  --> St="<< granddau->status()<<"   ("<<granddau->numberOfDaughters()<<")"<<endl;
					for (unsigned int jjj=0; jjj<granddau->numberOfDaughters(); jjj++){
					const reco::Candidate* bisdau=granddau->daughter(jjj);
							for (unsigned int jjjj=0; jjjj<bisdau->numberOfDaughters(); jjjj++){
							const reco::Candidate* tataradau=bisdau->daughter(jjjj);
							if(verbose_) cout<<"\t \t \t \t Second Tau Decay? "<<tataradau->pdgId()<<"  --> St="<<tataradau->status()<<endl;
							if(abs(tataradau->pdgId())==13) muon=true;
							if(abs(tataradau->pdgId())==11) electron=true;
								for(unsigned int kk=0; kk<tataradau->numberOfDaughters(); kk++){
									if(verbose_) cout<<"\t \t \t \t \t "<<tataradau->daughter(kk)->pdgId()<<endl;
								}
							}
					}
					if(muon) topToWTauNuDecayTauToMu++;
					if(electron) topToWTauNuDecayTauToEle++; 
				}
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())<6 )  topToWJetsDecay++;
                        }
                 }
                }
    }
		if(verbose_){
                cout<<"Found decay! Muon="<<topToWMuNuDecay<<"  Electron="<<topToWEleNuDecay<<"  Tau="<<topToWTauNuDecay<<"  Jets="<<topToWJetsDecay<<endl;
		cout<<"And Extra Jets St3? "<<numberOfExtraGluonSt3<<"(G)  "<<numberOfExtraJetsSt3-topToWJetsDecay<<"(Q)"<<endl;
		}	
    }

}




DEFINE_EDM_PLUGIN(NtupleFillerFactory, TopDecays, "TopDecays");





