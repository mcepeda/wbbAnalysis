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


class GenFindDs : public NtupleFillerBase {
 public:

    GenFindDs(){
    }

    GenFindDs(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    saveCs_(iConfig.getUntrackedParameter<bool>("saveCs",false)),
    saveBs_(iConfig.getUntrackedParameter<bool>("saveBs",false)),
    PTMIN_(iConfig.getUntrackedParameter<double>("ptMin",10)),
    ETAMAX_(iConfig.getUntrackedParameter<double>("etaMax",2.5)),
    src_(iConfig.getParameter<edm::InputTag>("src"))
	{
	   infoNUP=-1;
            t->Branch("infoNUP",&infoNUP,"infoNUP/F");

	  foundB=-1, foundBbar=-1, foundC=-1, foundCbar=-1, gluonMadgraphForB=-1, gluonMadgraphForC=-1,gluonPythiaForB=-1, gluonPythiaForC=-1; 	
            t->Branch("foundB",&foundB,"foundB/F");
            t->Branch("foundBbar",&foundBbar,"foundBbar/F");
            t->Branch("foundC",&foundC,"foundC/F");
            t->Branch("foundCbar",&foundCbar,"foundCbar/F");
            t->Branch("gluonPythiaForB",&gluonPythiaForB,"gluonPythiaForB/F");
            t->Branch("gluonPythiaForC",&gluonPythiaForC,"gluonPythiaForC/F");
            t->Branch("gluonMadgraphForB",&gluonMadgraphForB,"gluonMadgraphForB/F");
            t->Branch("gluonMadgraphForC",&gluonMadgraphForC,"gluonMadgraphForC/F");


          BCandsPt = new std::vector<double>();
          BCandsPhi = new std::vector<double>();
          BCandsEta = new std::vector<double>();
          BCandsPDGID = new std::vector<int>();
          BCandsStatus = new std::vector<int>();
          BCandsMothers = new std::vector<int>();

          BCandsDaughters = new std::vector<int>();
          BMotherPt = new std::vector<double>();
          BMotherPhi = new std::vector<double>();
          BMotherEta = new std::vector<double>();
          BMotherPDGID = new std::vector<int>();
          BMotherStatus = new std::vector<int>();
          BMother2Eta = new std::vector<double>();
          BMother2Phi = new std::vector<double>();
          BMother2Pt = new std::vector<double>();
          BMother2Status = new std::vector<int>();
          BMother2PDGID = new std::vector<int>();


	  if(saveBs_){
            t->Branch("BCandsPt","std::vector<double>",&BCandsPt);
            t->Branch("BCandsPhi","std::vector<double>",&BCandsPhi);
            t->Branch("BCandsEta","std::vector<double>",&BCandsEta);
            t->Branch("BCandsPDGID","std::vector<int>",&BCandsPDGID);
            t->Branch("BCandsStatus","std::vector<int>",&BCandsStatus);
            t->Branch("BCandsMothers","std::vector<int>",&BCandsMothers);
		  if(verbose_){ 
            		t->Branch("BCandsDaughters","std::vector<int>",&BCandsDaughters);
	                t->Branch("BMotherPt","std::vector<double>",&BMotherPt);
            		t->Branch("BMotherPhi","std::vector<double>",&BMotherPhi);
	                t->Branch("BMotherEta","std::vector<double>",&BMotherEta);
            		t->Branch("BMotherPDGID","std::vector<int>",&BMotherPDGID);
            		t->Branch("BMotherStatus","std::vector<int>",&BMotherStatus);
            		t->Branch("BMother2Pt","std::vector<double>",&BMother2Pt);
            		t->Branch("BMother2Phi","std::vector<double>",&BMother2Phi);
            		t->Branch("BMother2Eta","std::vector<double>",&BMother2Eta);
            		t->Branch("BMother2PDGID","std::vector<int>",&BMother2PDGID);
            		t->Branch("BMother2Status","std::vector<int>",&BMother2Status);
	  	}
	  }

          CCandsPt = new std::vector<double>();
          CCandsPhi = new std::vector<double>();
          CCandsEta = new std::vector<double>();
          CCandsPDGID = new std::vector<int>();
          CCandsStatus = new std::vector<int>();
          CCandsMothers = new std::vector<int>();
          CCandsDaughters = new std::vector<int>();
          CMotherPt = new std::vector<double>();
          CMotherPhi = new std::vector<double>();
          CMotherEta = new std::vector<double>();
          CMotherPDGID = new std::vector<int>();
          CMotherStatus = new std::vector<int>();
          CMother2Pt = new std::vector<double>();
          CMother2Phi = new std::vector<double>();
          CMother2Eta = new std::vector<double>();
          CMother2PDGID = new std::vector<int>();
          CMother2Status = new std::vector<int>();


          if(saveCs_){
            t->Branch("CCandsPt","std::vector<double>",&CCandsPt);
            t->Branch("CCandsPhi","std::vector<double>",&CCandsPhi);
            t->Branch("CCandsEta","std::vector<double>",&CCandsEta);
            t->Branch("CCandsPDGID","std::vector<int>",&CCandsPDGID);
            t->Branch("CCandsStatus","std::vector<int>",&CCandsStatus);
            t->Branch("CCandsMothers","std::vector<int>",&CCandsMothers);
          if(verbose_){
            t->Branch("CCandsDaughters","std::vector<int>",&CCandsDaughters);
            t->Branch("CMotherPt","std::vector<double>",&CMotherPt);
            t->Branch("CMotherPhi","std::vector<double>",&CMotherPhi);
            t->Branch("CMotherEta","std::vector<double>",&CMotherEta);
            t->Branch("CMotherPDGID","std::vector<int>",&CMotherPDGID);
            t->Branch("CMotherStatus","std::vector<int>",&CMotherStatus);
            t->Branch("CMother2Pt","std::vector<double>",&CMother2Pt);
            t->Branch("CMother2Phi","std::vector<double>",&CMother2Phi);
            t->Branch("CMother2Eta","std::vector<double>",&CMother2Eta);
            t->Branch("CMother2PDGID","std::vector<int>",&CMother2PDGID);
            t->Branch("CMother2Status","std::vector<int>",&CMother2Status);
          }
          }



 	

	}
      
      ~GenFindDs()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  bool saveCs_;  bool saveBs_;
  edm::InputTag src_;

	std::vector<double> *CCandsPt;              std::vector<double> *BCandsPt;     
        std::vector<double> *CCandsEta;             std::vector<double> *BCandsEta;
        std::vector<double> *CCandsPhi;             std::vector<double> *BCandsPhi;
        std::vector<int> *CCandsPDGID;              std::vector<int> *BCandsPDGID;
        std::vector<int> *CCandsStatus;             std::vector<int> *BCandsStatus;
        std::vector<int> *CCandsMothers;            std::vector<int> *BCandsMothers;
        std::vector<int> *CCandsDaughters;          std::vector<int> *BCandsDaughters;
        std::vector<double> *CMotherPt;             std::vector<double> *BMotherPt;
        std::vector<double> *CMotherEta;            std::vector<double> *BMotherEta;
        std::vector<double> *CMotherPhi;            std::vector<double> *BMotherPhi;
        std::vector<int> *CMotherPDGID;             std::vector<int> *BMotherPDGID;
        std::vector<int> *CMotherStatus;            std::vector<int> *BMotherStatus;
        std::vector<double> *CMother2Pt;            std::vector<double> *BMother2Pt;
        std::vector<double> *CMother2Eta;           std::vector<double> *BMother2Eta;
        std::vector<double> *CMother2Phi;           std::vector<double> *BMother2Phi;
        std::vector<int> *CMother2PDGID;            std::vector<int> *BMother2PDGID;
        std::vector<int> *CMother2Status;           std::vector<int> *BMother2Status;

       float   foundB;
       float   foundBbar;
       float   foundC;
       float   foundCbar;
       float   gluonPythiaForC;
       float   gluonPythiaForB;
       float   gluonMadgraphForC;
       float   gluonMadgraphForB;


	double PTMIN_; double ETAMAX_;

       float   infoNUP;



};


void GenFindDs::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 

            CCandsPt->clear();                   BCandsPt->clear();
            CCandsPhi->clear();                  BCandsPhi->clear();
            CCandsEta->clear();                  BCandsEta->clear();
            CCandsStatus->clear();               BCandsStatus->clear();
            CCandsMothers->clear();              BCandsMothers->clear();
            CCandsDaughters->clear();            BCandsDaughters->clear();
            CCandsPDGID->clear();                BCandsPDGID->clear();

            CMotherPt->clear();                  BMotherPt->clear();
            CMotherPhi->clear();                 BMotherPhi->clear();
            CMotherEta->clear();                 BMotherEta->clear();
            CMotherStatus->clear();              BMotherStatus->clear();
            CMotherPDGID->clear();               BMotherPDGID->clear();

            CMother2Pt->clear();                 BMother2Pt->clear();
            CMother2Phi->clear();                BMother2Phi->clear();
            CMother2Eta->clear();                BMother2Eta->clear();
            CMother2Status->clear();             BMother2Status->clear();
            CMother2PDGID->clear();              BMother2PDGID->clear();

            foundB=0;
            foundBbar=0;
            foundC=0;
            foundCbar=0;
            gluonPythiaForC=0;
            gluonPythiaForB=0;
            gluonMadgraphForC=0;
            gluonMadgraphForB=0;



  edm::Handle< vector<reco::GenParticle> >pGenPart;
  if(iEvent.getByLabel(src_, pGenPart)){

	  for( size_t i = 0; i < pGenPart->size(); ++ i ) {
                const reco::GenParticle& genpart = (*pGenPart)[i];

		if(abs(genpart.pdgId())!=5 && abs(genpart.pdgId())!=4) continue;

//                        cout<<"Found B/C ("<<genpart.pdgId()<<") with Status : "<<genpart.status()<<"   "<<genpart.pt()<<endl;
//                        cout<<"    Number of Mothers: "<<genpart.numberOfMothers()<<endl;
 
			bool sameDecay=false; 
                        for(unsigned int imom=0; imom<genpart.numberOfMothers(); imom++){  
				const reco::Candidate *Mom =genpart.mother(imom);
				if(Mom->pdgId()==genpart.pdgId()) sameDecay=true;	// I dont want to save N times the same Hadron. I will just save the original 
											// Warning: this might affect the pt cuts on it 
				
			}
			if(sameDecay) {/*cout<<"..... Already accounted for!!!!"<<endl;*/ continue;}

                        int gluons=0; int protons=0;
                        for(unsigned int imom=0; imom<genpart.numberOfMothers(); imom++){
                                const reco::Candidate *Mom =genpart.mother(imom);
                                if(Mom->pdgId()==21) gluons++;
                                if(Mom->pdgId()==2212) protons++;

                        }


	
                        if(abs(genpart.pdgId())==5){  // This is a B / Bbar

			if(genpart.pdgId()==5&&genpart.pt()>PTMIN_&&fabs(genpart.eta())<ETAMAX_)foundB++;
			if(genpart.pdgId()==-5&&genpart.pt()>PTMIN_&&fabs(genpart.eta())<ETAMAX_) foundBbar++;
			if(gluons>0) gluonPythiaForB++;
                        if(protons>0) gluonMadgraphForB++;


			BCandsPt->push_back(genpart.pt());
			BCandsPhi->push_back(genpart.phi());
			BCandsEta->push_back(genpart.eta());
			BCandsPDGID->push_back(genpart.pdgId());
                        BCandsStatus->push_back(genpart.status());
                        BCandsMothers->push_back(genpart.numberOfMothers());
                        BCandsDaughters->push_back(genpart.numberOfDaughters());

                                const reco::Candidate *mom =genpart.mother(0);
	                        BMotherPt->push_back(mom->pt());
        	                BMotherPhi->push_back(mom->phi());
                	        BMotherEta->push_back(mom->eta());
                        	BMotherPDGID->push_back(mom->pdgId());
                        	BMotherStatus->push_back(mom->status());
				if(genpart.numberOfMothers()>1){   // Sometimes the particles have *many* mothers (pythia strings). Lets simplify...
                                const reco::Candidate *mom2 =genpart.mother(1);
                                BMother2Pt->push_back(mom2->pt());
                                BMother2Phi->push_back(mom2->phi());
                                BMother2Eta->push_back(mom2->eta());
                                BMother2PDGID->push_back(mom2->pdgId());
                                BMother2Status->push_back(mom2->status());
				}
				else{
				BMother2Pt->push_back(-777);
				BMother2Phi->push_back(-777);	
				BMother2Eta->push_back(-777);
				BMother2PDGID->push_back(-777);
				BMother2Status->push_back(-777);
				}

		 }
		 else {  // This is a C / Cbar

                        if(genpart.pdgId()==4&&genpart.pt()>PTMIN_&&fabs(genpart.eta())<ETAMAX_)foundC++;
                        if(genpart.pdgId()==-4&&genpart.pt()>PTMIN_&&fabs(genpart.eta())<ETAMAX_)foundCbar++;
                        if(gluons>0) gluonPythiaForC++;
                        if(protons>0) gluonMadgraphForC++;

                        CCandsPt->push_back(genpart.pt());
                        CCandsPhi->push_back(genpart.phi());
                        CCandsEta->push_back(genpart.eta());
                        CCandsPDGID->push_back(genpart.pdgId());
                        CCandsStatus->push_back(genpart.status());
                        CCandsMothers->push_back(genpart.numberOfMothers());
                        CCandsDaughters->push_back(genpart.numberOfDaughters());

                                const reco::Candidate *mom =genpart.mother(0);
                                CMotherPt->push_back(mom->pt());
                                CMotherPhi->push_back(mom->phi());
                                CMotherEta->push_back(mom->eta());
                                CMotherPDGID->push_back(mom->pdgId());
                                CMotherStatus->push_back(mom->status());
                                if(genpart.numberOfMothers()>1){   // Sometimes the particles have *many* mothers (pythia strings). Lets simplify...
                                const reco::Candidate *mom2 =genpart.mother(1);
                                CMother2Pt->push_back(mom2->pt());
                                CMother2Phi->push_back(mom2->phi());
                                CMother2Eta->push_back(mom2->eta());
                                CMother2PDGID->push_back(mom2->pdgId());
                                CMother2Status->push_back(mom2->status());
                                }
                                else{
                                CMother2Pt->push_back(-777);
                                CMother2Phi->push_back(-777);
                                CMother2Eta->push_back(-777);
                                CMother2PDGID->push_back(-777);
                                CMother2Status->push_back(-777);
                                }
		}	

   }



  }


  edm::Handle<LHEEventProduct> lheeventinfo;
  if(iEvent.getByLabel("source", lheeventinfo)){

  infoNUP=lheeventinfo->hepeup().NUP;


	}


}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, GenFindDs, "GenFindDs");




