// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"


// Stuff for gen level ID (based on a W analysis...)


class GenDecayModes : public NtupleFillerBase {
 public:

    GenDecayModes(){
    }

    GenDecayModes(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src"))
	{
	   weight=-1;
            t->Branch("weight",&weight,"weight/F");

	    foundDPM=0,foundD0=0, foundD0Star=0, foundDPMall=0;
	    identifiedCDecay=0;

            t->Branch("identifiedCDecay",&identifiedCDecay,"identifiedCDecay/O");
            t->Branch("foundDPM",&foundDPM,"foundDPM/O");
            t->Branch("foundD0",&foundD0,"foundD0/O");
            t->Branch("foundD0Star",&foundD0Star,"foundD0Star/O");
            t->Branch("foundDPMall",&foundDPMall,"foundDPMall/O");


	}
      
      ~GenDecayModes()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  bool saveCs_;  bool saveBs_;
  edm::InputTag src_;

  float   weight;

  bool foundDPM,foundD0, foundD0Star,foundDPMall;
  bool identifiedCDecay;




};


void GenDecayModes::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
    weight=1;

    foundDPM=false,foundD0=false, foundD0Star=false,foundDPMall=false;
    identifiedCDecay=false;


  edm::Handle< vector<reco::GenParticle> >pGenPart;
  if(iEvent.getByLabel(src_, pGenPart)){

	  for( size_t i = 0; i < pGenPart->size(); ++ i ) {
                const reco::GenParticle& genpart = (*pGenPart)[i];

		if(abs(genpart.pdgId())==421||abs(genpart.pdgId())==411) {
		        if(verbose_){	
			cout<<"Found DPM/D0 ("<<genpart.pdgId()<<") with Status : "<<genpart.status()<<"   "<<genpart.pt()<<endl;
			cout<<"    Number of Mothers: "<<genpart.numberOfMothers()<<endl;
			                        if(genpart.numberOfMothers()!=0) cout<<" ---> "<<genpart.mother(0)->pdgId()<<endl;
			}
                        int k_plus=0, k_minus=0, pi_minus=0, pi_plus=0;

			if(abs(genpart.pdgId())==411) foundDPMall=true;

                                for(unsigned int k=0; k<genpart.numberOfDaughters(); k++){
                                        const reco::Candidate* grandau=genpart.daughter(k);
		//			if(verbose_) cout<<"\t-->"<<grandau->pdgId()<<" ";
                                        if(grandau->pdgId()==211) pi_plus++;
                                        else if(grandau->pdgId()==-211) pi_minus++;
                                        else if(grandau->pdgId()==321) k_plus++;
                                        else if(grandau->pdgId()==-321) k_minus++;
	                              }


                        if(verbose_&&(k_plus+k_minus+pi_minus+pi_plus)>0) cout<<endl<<"Found:  K+"<<k_plus<<" K-"<<k_minus<<" Pi+:"<<pi_plus<<" Pi-:"<<pi_minus<<endl;

			if(abs(genpart.pdgId())==411&&genpart.numberOfDaughters()==3&&((k_plus==1&&pi_minus==2)||(k_minus==1&&pi_plus==2))) foundDPM=true;
			 
			if(abs(genpart.pdgId())==421&& genpart.numberOfDaughters()==2&&(k_plus+k_minus)==1&&(pi_minus+pi_plus)==1) foundD0=true;
			if(foundD0&&(genpart.mother(0)->pdgId()==413)) { foundD0=false; foundD0Star=true;}
			
 	
		}

		if(abs(genpart.pdgId())!=4) continue;

			if(verbose_){
			cout<<endl;
                        cout<<"Found B/C ("<<genpart.pdgId()<<") with Status : "<<genpart.status()<<"   "<<genpart.pt()<<endl;
                        cout<<"    Number of Mothers: "<<genpart.numberOfMothers();
			if(genpart.numberOfMothers()!=0) cout<<" ---> "<<genpart.mother(0)->pdgId()<<endl;
				else cout<<endl;
			cout<<endl;
			} 

			for(unsigned int j=0; j<genpart.numberOfDaughters(); j++){
				const reco::Candidate* dau=genpart.daughter(j);
				if(verbose_)			
				cout<<"Dau : "<<j<<" -->"<<dau->pdgId()<<endl;
				bool unique=true; 
				while(dau->pdgId()==92){
			//		cout<<"----> "<<dau->pdgId()<<"READING DAUGHTERS: "<<dau->numberOfDaughters()<<endl;
					if(dau->numberOfDaughters()==1) dau=dau->daughter(0);
					else {
						unique=false; int found=0; int count=0;
						for(unsigned int k=0; k<dau->numberOfDaughters(); k++){
			//			 cout<<dau->daughter(k)->pdgId()<<endl;
						 if(abs(dau->daughter(k)->pdgId())==411 || abs(dau->daughter(k)->pdgId())==413||
							abs(dau->daughter(k)->pdgId())==421) {count++; found=k;
							if(verbose_)	cout<<"found!"<<endl;
							}
						 }
						dau=dau->daughter(found);
						//if(count>1) cout<<"Warning! too many found"<<endl;
					}
				}

				if(abs(dau->pdgId())==411 || abs(dau->pdgId())==413||
                                                        abs(dau->pdgId())==421)identifiedCDecay=true;	
			}
  }

 }

	if(identifiedCDecay&&verbose_){
			if(foundDPM) weight=1.3; if(foundD0Star) weight=0.8;
		  if(verbose_){ 
		  cout<<"FOUND DPM? "<<foundDPM<<endl;  
		  cout<<"FOUND D0? "<<foundD0<<endl;
		  cout<<"FOUND D0STAR? "<<foundD0Star<<endl;
		  }
	}




}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, GenDecayModes, "GenDecayModes");




