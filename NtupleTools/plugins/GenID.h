// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


// Stuff for gen level ID (based on a W analysis...)


class GenID : public NtupleFillerBase {
 public:

    GenID(){
    }

    GenID(const edm::ParameterSet& iConfig, TTree* t):
    src_(iConfig.getParameter<edm::InputTag>("src"))
	{
	  genWLeptonID=0,genLeptonPT=0, genLeptonETA=0,genLeptonPHI=0;
          genWBs=-1; genWB_st3=-1; genWBbar_st3=-1;
          genWCs=-1;  genWC_st3=-1;genWCbar_st3=-1;
	  genWGs=-1;  genWGs_st3=-1;
          t->Branch("genWLeptonID",&genWLeptonID,"genWLeptonID/F");
	  t->Branch("genLeptonPT",&genLeptonPT,"genLeptonPT/F");
          t->Branch("genLeptonETA",&genLeptonETA,"genLeptonETA/F");
          t->Branch("genLeptonPHI",&genLeptonPHI,"genLeptonPHI/F");
          t->Branch("genWBs",&genWBs,"genWBs/F");
          t->Branch("genWCs",&genWCs,"genWCs/F");
          t->Branch("genWGs",&genWGs,"genWGs/F");

          t->Branch("genWBbar",&genWBbar,"genWBbar/F");
          t->Branch("genWB",&genWB,"genWB/F");

          t->Branch("genWBbar_st3",&genWBbar_st3,"genWBbar_st3/F");
          t->Branch("genWCbar_st3",&genWCbar_st3,"genWCbar_st3/F");
          t->Branch("genWB_st3",&genWB_st3,"genWB_st3/F");
          t->Branch("genWC_st3",&genWC_st3,"genWC_st3/F");
          t->Branch("genWGs_st3",&genWGs_st3,"genWGs_st3/F");

          t->Branch("genWBbar_AfterW",&genWBbar_AfterW,"genWBbar_AfterW/F");
          t->Branch("genWCbar_AfterW",&genWCbar_AfterW,"genWCbar_AfterW/F");
          t->Branch("genWB_AfterW",&genWB_AfterW,"genWB_AfterW/F");
          t->Branch("genWC_AfterW",&genWC_AfterW,"genWC_AfterW/F");
          t->Branch("genWGs_AfterW",&genWGs_AfterW,"genWGs_AfterW/F");

	}
      
      ~GenID()
	{ 
	  
	}
      


  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup)
  {
    using namespace std; 
    genWLeptonID=-777; 
    genWBs=-777;      
    genWBbar=-777;
    genWB=-777;
    genWCs=-777;     
    genWGs=-777;
    genWB_st3=-777;
    genWC_st3=-777;
    genWGs_st3=-777;
    genWBbar_st3=-777;
    genWCbar_st3=-777;
    genWB_AfterW=-777;
    genWC_AfterW=-777;
    genWGs_AfterW=-777;
    genWBbar_AfterW=-777;
    genWCbar_AfterW=-777;

   genLeptonPHI=-777;
   genLeptonPT=-777;
   genLeptonETA=-777;




  edm::Handle< vector<reco::GenParticle> >pGenPart;
  if(iEvent.getByLabel(src_, pGenPart)){

	  int muon=0, electron=0, tau=0, neutrino=0, Bs=0, Cs=0, Gs=0,EWK_B=0, EWK_C=0,EWK_Bbar=0, EWK_Cbar=0,EWK_Gs=0;
		int AfterW_B=0, AfterW_C=0,AfterW_Bbar=0, AfterW_Cbar=0,AfterW_Gs=0;
		int B=0, Bbar=0;

	  double charge=1;

	  bool lepton=false;  bool foundW=false;
	  for( size_t i = 0; i < pGenPart->size(); ++ i ) {
                const reco::GenParticle& genpart = (*pGenPart)[i];

		// To distinguish at ntuple level between WMuNu, WENu, WTauNu // ZMuMu, ZEE, ZTauTau
		if(genpart.status()==3){

				     if(abs(genpart.pdgId())==24) foundW=true;
                                     else if(genpart.numberOfMothers()!=0&&(abs(genpart.mother()->pdgId())==24 || genpart.mother()->pdgId()==23)){
				     if(abs(genpart.pdgId())==13){muon++; lepton=true;}
                                     else if (abs(genpart.pdgId())==11){electron++; lepton=true;}
                                     else if (abs(genpart.pdgId())==15){lepton=true; tau++; }
                                     else if (abs(genpart.pdgId())==14) {neutrino++; }


					if(abs(genpart.mother()->pdgId())==24 && (abs(genpart.pdgId())==11||abs(genpart.pdgId())==15||abs(genpart.pdgId())==13)) {
//					if((abs(genpart.pdgId())==11||abs(genpart.pdgId())==15||abs(genpart.pdgId())==13)) {
						genLeptonPT=genpart.pt();
						genLeptonPHI=genpart.phi();
						genLeptonETA=genpart.eta();
						if(genpart.mother()->pdgId()<0) charge=-1;
				//		if(genpart.pdgId()<0) charge=-1;
					}		

				     }	
		}

/*
		if(abs(genpart.pdgId())==6){
		cout<<"Found top!!!  "<<genpart.pdgId()<<"    --> St="<<genpart.status()<<"   ("<<genpart.numberOfDaughters()<<")"<<endl;
		bool topToWEleNuDecay=false, topToWTauNuDecay=false, topToWJetsDecay=false, topToWMuNuDecay=false;
		 for (unsigned int j=0; j<genpart.numberOfDaughters(); j++){
			const reco::Candidate* dau=genpart.daughter(j);
			cout<<"\t Dau "<<j<<"  ->"<<dau->pdgId()<<"    --> St="<<dau->status()<<"   ("<<dau->numberOfDaughters()<<")"<<endl;
			for (unsigned int jj=0; jj<dau->numberOfDaughters(); jj++){	
	                        const reco::Candidate* granddau=dau->daughter(jj);
                        	cout<<" \t \t GranDau "<<jj<<"  ->"<<granddau->pdgId()<<"    --> St="<<granddau->status()<<"   ("<<granddau->numberOfDaughters()<<")"<<endl;
				if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==11) topToWEleNuDecay=true;
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==13) topToWMuNuDecay=true;
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())==15) topToWTauNuDecay=true;
                                if(abs(dau->pdgId())==24 && abs(granddau->pdgId())<6 )  topToWJetsDecay=true;
//				for (unsigned int jjj=0; jjj<granddau->numberOfDaughters(); jjj++){
//					const reco::Candidate* bisdau=granddau->daughter(jjj);
//					cout<<" \t \t \t BisDau "<<jjj<<"  ->"<<bisdau->pdgId()<<"    --> St="<<bisdau->status()<<endl;
//				}
			}
		 }	
		cout<<"Found decay! Muon="<<topToWMuNuDecay<<"  Electron="<<topToWEleNuDecay<<"  Tau="<<topToWTauNuDecay<<"  Jets="<<topToWJetsDecay<<endl; 	
		}
*/
	
		// This is the easiest: if there are ANY B-Hadrons in the event (with status 2 or 3) --> This is a B-Event
		// Else, check if there is a C 
		// It does not differentiate gluon splitting (important to distinguish W+C and W+CC, not so for W+B)
//		if(genpart.pt()>10 && fabs(genpart.eta())<2.5){
	        		     if((abs(genpart.pdgId())==5)) Bs++;
                                     if((abs(genpart.pdgId())==4)) Cs++;
                                     if((abs(genpart.pdgId())==21)) Gs++;
				     if(genpart.pdgId()==-5) Bbar++;
                                     if(genpart.pdgId()==5) B++;  
	

//		}

		// Status 3 particles which are stored in the event after the W (only useful for W+C --> if there are C&CBAR --> CCBAR)
		if(foundW&&genpart.status()==3){//&&genpart.pt()>10 && fabs(genpart.eta())<2.5){
                                     if(genpart.pdgId()==5) AfterW_B++;
                                     if(genpart.pdgId()==4) AfterW_C++;
                                     if(genpart.pdgId()==-5) AfterW_Bbar++;
                                     if(genpart.pdgId()==-4) AfterW_Cbar++;
                                     if((abs(genpart.pdgId())==21)) AfterW_Gs++;
                }
                

		// Status 3 (initial partons) only. Since W+BBAR comes from gluon splitting, the B categories will be empty (the gluon one can help though)	
                if(genpart.status()==3){//&&genpart.pt()>10 && fabs(genpart.eta())<2.5){
                                     if(genpart.pdgId()==5) EWK_B++;
                                     if(genpart.pdgId()==4) EWK_C++;
                                     if(genpart.pdgId()==-5) EWK_Bbar++;
                                     if(genpart.pdgId()==-4) EWK_Cbar++;
                                     if((abs(genpart.pdgId())==21)) EWK_Gs++;
                }


		// To be studied a bit more in detail with a few printouts. 
		// It would be useful to modify the "status 2" sequence to NOT double count (if a B's mother is already accounted for --> Do not increase the counter)

           }
           if((tau>0&&muon>0) || (electron>0&&muon>0) || (tau>0&&electron>0)) genWLeptonID=100;
           else if(muon>0) genWLeptonID=13*charge;
           else if(electron>0) genWLeptonID=11*charge;
           else if(tau>0) genWLeptonID=15*charge;
           else if(!lepton) genWLeptonID=-1; 	
	
	   genWBs=Bs;
		genWBbar=Bbar;
		genWB=B;

	   genWCs=Cs;			
           genWGs=Gs;

           genWB_st3=EWK_B;
           genWC_st3=EWK_C;
           genWBbar_st3=EWK_Bbar;
           genWCbar_st3=EWK_Cbar;
           genWGs_st3=EWK_Gs;

           genWB_AfterW=AfterW_B;
           genWC_AfterW=AfterW_C;
           genWBbar_AfterW=AfterW_Bbar;
           genWCbar_AfterW=AfterW_Cbar;
           genWGs_AfterW=AfterW_Gs;


    }
   


    }
  
  

 protected:
  edm::InputTag src_;
  float  genWLeptonID;
  float  genWBs;
  float  genWCs;
	float genWGs;

  float  genWBbar;
  float  genWB;

	float    genLeptonPHI, genLeptonPT, genLeptonETA;


  float  genWB_st3;
  float  genWC_st3;
  float  genWBbar_st3;
  float  genWCbar_st3;
        float genWGs_st3;

  float  genWB_AfterW;
  float  genWC_AfterW;
  float  genWBbar_AfterW;
  float  genWCbar_AfterW;
        float genWGs_AfterW;

};






