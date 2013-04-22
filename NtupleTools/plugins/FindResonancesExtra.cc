// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "UWAnalysis/RecoTools/plugins/PATSSVJetEmbedder.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "Math/GenVector/VectorUtil.h"


class FindResonancesExtra : public NtupleFillerBase {
 public:

    FindResonancesExtra(){
    }

    FindResonancesExtra(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src")),
    srcBTAG_(iConfig.getParameter<edm::InputTag>("srcBTAG")),
    srcVertices_(iConfig.getParameter<edm::InputTag>("srcPrimaryVertices")),
    jetindex_(iConfig.getParameter<int>("jetindex"))
//    jet_(iConfig.getUntrackedParameter<pat::Jet>("Jet"))   :-( some library link missing (compiles but fails at the last step. shame.
	{
//	   infoNUP=-1;
//            t->Branch("infoNUP",&infoNUP,"infoNUP/F");
//            t->Branch("CCandsPhi","std::vector<float>",&CCandsPhi);
		TString whichJet;
			whichJet=Form("_%d",jetindex_);

                t->Branch("phiVertexFlight"+whichJet,&phiVertexFlight,"phiVertexFlight"+whichJet+"/F");
                t->Branch("etaVertexFlight"+whichJet,&etaVertexFlight,"etaVertexFlight"+whichJet+"/F");

		t->Branch("vertexMass"+whichJet,&vertexMass,"vertexMass"+whichJet+"/F");

                t->Branch("vertexMass_D0"+whichJet,&vertexMass_D0,"vertexMass_D0"+whichJet+"/F");
                t->Branch("vertexMass_DPM"+whichJet,&vertexMass_DPM,"vertexMass_DPM"+whichJet+"/F");
		t->Branch("vertexMass_D0_KP"+whichJet,&vertexMass_D0_KP,"vertexMass_D0_KP"+whichJet+"/F");
		t->Branch("vertexMass_D0_KM"+whichJet,&vertexMass_D0_KM,"vertexMass_D0_KM"+whichJet+"/F");
		t->Branch("vertexMass_D0Star"+whichJet,&vertexMass_D0Star,"vertexMass_D0Star"+whichJet+"/F");
                t->Branch("vertexMass_D0Star_KM"+whichJet,&vertexMass_D0Star_KM,"vertexMass_D0Star_KM"+whichJet+"/F");
                t->Branch("vertexMass_D0Star_KP"+whichJet,&vertexMass_D0Star_KP,"vertexMass_D0Star_KP"+whichJet+"/F");

                t->Branch("vertexPT_D0"+whichJet,&vertexPT_D0,"vertexPT_D0"+whichJet+"/F");
                t->Branch("vertexPT_DPM"+whichJet,&vertexPT_DPM,"vertexPT_DPM"+whichJet+"/F");
                t->Branch("vertexPT_D0_KP"+whichJet,&vertexPT_D0_KP,"vertexPT_D0_KP"+whichJet+"/F");
                t->Branch("vertexPT_D0_KM"+whichJet,&vertexPT_D0_KM,"vertexPT_D0_KM"+whichJet+"/F");
                t->Branch("vertexPT_D0Star"+whichJet,&vertexPT_D0Star,"vertexPT_D0Star"+whichJet+"/F");
                t->Branch("vertexPT_D0Star_KP"+whichJet,&vertexPT_D0Star_KP,"vertexPT_D0Star_KP"+whichJet+"/F");
                t->Branch("vertexPT_D0Star_KM"+whichJet,&vertexPT_D0Star_KM,"vertexPT_D0Star_KM"+whichJet+"/F");

		t->Branch("deltaM_D0Star_D0"+whichJet,&deltaM_D0Star_D0,"deltaM_D0Star_D0"+whichJet+"/F");
                t->Branch("chargePION"+whichJet,&chargePION,"chargePION"+whichJet+"/F");
                t->Branch("ptPION"+whichJet,&ptPION,"ptPION"+whichJet+"/F");
                t->Branch("anglePION"+whichJet,&anglePION,"anglePION"+whichJet+"/F");

		t->Branch("chargeSV"+whichJet,&chargeSV,"chargeSV"+whichJet+"/F");
		t->Branch("nTracksSV"+whichJet,&nTracksSV,"nTracksSV"+whichJet+"/F");

		 t->Branch("JetPTCHECK"+whichJet,&JetPTCHECK,"JetPTCHECK"+whichJet+"/F");
	}
      
      ~FindResonancesExtra()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  edm::InputTag src_;
  edm::InputTag srcBTAG_;
  edm::InputTag srcVertices_;
  int jetindex_;
//  pat::Jet jet_;

	float vertexMass;
	float vertexMass_D0_KP; 
	float vertexMass_D0_KM; 
	float vertexMass_DPM;
	float vertexMass_D0;
	float vertexMass_D0Star;
        float vertexMass_D0Star_KP;
        float vertexMass_D0Star_KM;

        float vertexPT;
        float vertexPT_D0_KP;
        float vertexPT_D0_KM;
        float vertexPT_DPM;
        float vertexPT_D0;
        float vertexPT_D0Star;
        float vertexPT_D0Star_KM;
        float vertexPT_D0Star_KP;



	float deltaM_D0Star_D0;
        float chargePION;
        float ptPION;
        float anglePION;

	float chargeSV;
	float nTracksSV;

	float JetPTCHECK;

	float phiVertexFlight;
        float etaVertexFlight;

};


void FindResonancesExtra::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
    using namespace edm;
    using namespace reco;

	

    edm::Handle<reco::VertexCollection> vertexHandle;
    iEvent.getByLabel(srcVertices_, vertexHandle);
    edm::Handle<pat::JetCollection > cands;
    edm::Handle<pat::JetCollection > cands2;

    if(!iEvent.getByLabel(src_,cands)) LogError("")<<"Jet collection does not exist!!!"<<endl;
    if(!iEvent.getByLabel(srcBTAG_,cands2)) LogError("")<<"Jet BTAG collection does not exist!!!"<<endl;


    int JETSIZE=cands->size();
  
        vertexMass=-777;        
        vertexMass_D0_KP=-777;        
        vertexMass_D0_KM=-777;        
        vertexMass_DPM=-777;
        vertexMass_D0=-777;
        vertexMass_D0Star=-777;
        deltaM_D0Star_D0=-777;
        chargeSV=-777;
        chargePION=-777;
        nTracksSV=-777;
        vertexPT=-777;
        vertexPT_D0_KP=-777;
        vertexPT_D0_KM=-777;
        vertexPT_DPM=-777;
        vertexPT_D0=-777;
        vertexMass_D0Star_KP=-777;
        vertexMass_D0Star_KM=-777;
        vertexPT_D0Star_KP=-777;
        vertexPT_D0Star_KM=-777;
        vertexPT_D0Star=-777;
        ptPION=-777;
        anglePION=-777;
	etaVertexFlight=-777;
	phiVertexFlight=-777;
 
	if(jetindex_<JETSIZE&&JETSIZE>0&&vertexHandle->size()>0){


//      for(unsigned int  i=0;i!=cands->size();++i){

        pat::Jet jetORIGINAL = cands->at(jetindex_);


        if(verbose_) cout<<"Reading Jet: "<<jetORIGINAL.pt()<<endl;

	unsigned int indexTAG=999; 

        double dRMin=1000000;

	for (unsigned int  j=0;j!=cands2->size();++j){
		pat::Jet jetBTAG = cands2->at(j);
		double dR=sqrt( (jetORIGINAL.eta()-jetBTAG.eta())*(jetORIGINAL.eta()-jetBTAG.eta()) +  (jetORIGINAL.phi()-jetBTAG.phi())*(jetORIGINAL.phi()-jetBTAG.phi()) );
		if(dRMin>dR) {dRMin=dR; indexTAG=j;} 
	}	

	if(indexTAG==999||dRMin>0.01) LogError("")<<"This Jet does not match!!!"<<endl;
	else{
	pat::Jet jet=cands2->at(indexTAG);
		JetPTCHECK=jet.pt();

                           const reco::SecondaryVertexTagInfo& secInfo = *jet.tagInfoSecondaryVertex("secondaryVertex");
	if(verbose_) {cout<<"Got SV! "<<endl;
	cout<<"Really?"<< secInfo.nVertices()<<endl;
	}
                 if (secInfo.nVertices() >= 1) {
	        if(verbose_) cout<<"Check SV!"<<endl;

              const reco::Vertex &sv = secInfo.secondaryVertex(0);
	      if(verbose_) cout<<"SV 0 "<<endl;	
              // the precomputed direction with respect to the primary vertex
              GlobalVector dir = secInfo.flightDirection(0);
              // unfortunately CMSSW hsa all kinds of vectors,
              // and sometimes we need to convert them *sigh*   
              math::XYZVector dir2(dir.x(), dir.y(), dir.z());


	      if(verbose_){
	      cout<<"-----------------------------------------------"<<endl;
              cout<<"FOUND SV-->"<<sv.x()<<"  " <<sv.y()<<" "<<sv.z()<<endl;
              cout<<"     (PV-->"<<vertexHandle->at(0).position().x()<<"  " <<vertexHandle->at(0).position().y()<<" "<<vertexHandle->at(0).position().z()<<")"<<endl;
	      cout<<"     (dir->"<<dir.x()<<" "<<dir.y()<<" "<<dir.z()<<")"<<endl;	
	      }	

              float mass_pion=0.13957;    float     mass_kaon=0.493677;


                // compute the invariant mass from a four-vector sum
                math::XYZTLorentzVector trackFourVectorSum;
                math::XYZTLorentzVector trackFourVectorSum_D0_KP;
                math::XYZTLorentzVector trackFourVectorSum_D0_KM;
		math::XYZTLorentzVector trackFourVectorSum_DPM;
                math::XYZTLorentzVector trackFourVectorSum_D0Star;
                math::XYZTLorentzVector trackFourVectorSum_D0Star_KM;
                math::XYZTLorentzVector trackFourVectorSum_D0Star_KP;

                nTracksSV=sv.tracksSize();

		chargeSV=0;

		for(reco::Vertex::trackRef_iterator track = sv.tracks_begin();track != sv.tracks_end(); ++track) {
					chargeSV+=(*track)->charge();
		}


                // loop over all tracks in the vertex
                for(reco::Vertex::trackRef_iterator track = sv.tracks_begin();
                    track != sv.tracks_end(); ++track) {
                        ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float> > vec;
                        vec.SetPx((*track)->px());
                        vec.SetPy((*track)->py());
                        vec.SetPz((*track)->pz());
                        vec.SetM(mass_pion);      // pion mass
                        trackFourVectorSum += vec;

			if(verbose_) {
			cout<<"   SV track --> ("<<(*track)->px()<<" "<<(*track)->py()<<" "<<(*track)->pz()<<"), "<<(*track)->charge()<<endl;
			}

			// For D0
			if(nTracksSV==2 && chargeSV==0){ 
				if( (*track)->charge()<0) vec.SetM(mass_pion);
				else 			     vec.SetM(mass_kaon);
				trackFourVectorSum_D0_KP+=vec;

                                if( (*track)->charge()>0) vec.SetM(mass_pion);
                                else                         vec.SetM(mass_kaon);
                                trackFourVectorSum_D0_KM+=vec;
			}


			if(nTracksSV==3 && abs(chargeSV)==1){
				if( (*track)->charge() !=chargeSV) vec.SetM(mass_kaon);
				else vec.SetM(mass_pion);
				trackFourVectorSum_DPM+=vec;
			}


                }

                // get the invariant mass: sqrt(E?~B² - px?~B² - py?~B² - pz?~B²)
                vertexMass = trackFourVectorSum.M();
		vertexMass_D0_KP = trackFourVectorSum_D0_KP.M();
                vertexMass_D0_KM = trackFourVectorSum_D0_KM.M();
		vertexMass_DPM  =trackFourVectorSum_DPM.M();

                vertexPT = trackFourVectorSum.pt();
                vertexPT_D0_KP = trackFourVectorSum_D0_KP.pt();
                vertexPT_D0_KM = trackFourVectorSum_D0_KM.pt();
                vertexPT_DPM  =trackFourVectorSum_DPM.pt();

                vertexMass_D0=-1;
		// I just want 1 combination to be possible, not two

                if( fabs(vertexMass_D0_KP-1.87)<=fabs(vertexMass_D0_KM-1.87))  {vertexMass_D0=vertexMass_D0_KP; vertexPT_D0=vertexPT_D0_KP;}
                else { vertexMass_D0=vertexMass_D0_KM; vertexPT_D0=vertexPT_DPM;}



	      phiVertexFlight=trackFourVectorSum.phi(); 	
              etaVertexFlight=trackFourVectorSum.eta();
		

		// Now for D0*


              vertexMass_D0Star=0;
	      deltaM_D0Star_D0 =0;

	      if(nTracksSV==2 &&chargeSV==0){	
		
              reco::Vertex::trackRef_iterator pion;

	      float angleMIN=1;	
	      bool foundPION=false; 

               for(reco::Vertex::trackRef_iterator track = vertexHandle->at(0).tracks_begin();
                    track != vertexHandle->at(0).tracks_end(); ++track) {
//			if((*track)->pt()<0.5) continue;
			bool sameTrack=false;
			for(reco::Vertex::trackRef_iterator track2 = sv.tracks_begin(); track2 != sv.tracks_end(); ++track2) {
			        float angleComp=sqrt( ((*track2)->px()-(*track)->px())*((*track2)->px()-(*track)->px())+((*track2)->py()-(*track)->py())*((*track2)->py()-(*track)->py())+((*track2)->pz()-(*track)->pz())*((*track2)->pz()-(*track)->pz()));
				if (angleComp==0) sameTrack=true;
			}		
			if (sameTrack) continue;

//                       float angle=sqrt( (dir.x()-(*track)->px())*(dir.x()-(*track)->px())+(dir.y()-(*track)->py())*(dir.y()-(*track)->py())+(dir.z()-(*track)->pz())*(dir.z()-(*track)->pz()));
			 float angle=sqrt( ((*track)->eta()-etaVertexFlight)*((*track)->eta()-etaVertexFlight) + ((*track)->phi()-phiVertexFlight)*((*track)->phi()-phiVertexFlight)); 

                         if(angle<angleMIN) {angleMIN=angle; pion=track; foundPION=true;}
                }

			if(foundPION){
			anglePION=angleMIN;

                        if(verbose_) cout<<"PION --> "<<(*pion)->px()<<","<<(*pion)->py()<<","<<(*pion)->pz()<<" ---> "<<(*pion)->pt()<<endl;

                        ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float> > pionVEC;
                        pionVEC.SetPx((*pion)->px());
                        pionVEC.SetPy((*pion)->py());
                        pionVEC.SetPz((*pion)->pz());
                        pionVEC.SetM(mass_pion);      // pion mass
                        trackFourVectorSum_D0Star_KM += pionVEC;
                        trackFourVectorSum_D0Star_KP += pionVEC;

			trackFourVectorSum_D0Star_KM+=trackFourVectorSum_D0_KM;
			trackFourVectorSum_D0Star_KP+=trackFourVectorSum_D0_KP;

			if((*pion)->charge()>0) trackFourVectorSum_D0Star+=trackFourVectorSum_D0Star_KM; 
			else if( (*pion)->charge()<0 ) trackFourVectorSum_D0Star+=trackFourVectorSum_D0Star_KP;
		//	cout<<(*pion)->charge()<<" "<<trackFourVectorSum_D0_KM.M()<<"  "<<trackFourVectorSum_D0_KP.M()<<endl;

			chargePION=(*pion)->charge();
                        ptPION=(*pion)->pt();

			// test:

			double massD0Star=0, ptD0Star=0;

			if((*pion)->charge()>0) {			
                                double pxD0star=pionVEC.px()+trackFourVectorSum_D0_KM.px();
                                double pyD0star=pionVEC.py()+trackFourVectorSum_D0_KM.py();	
                                double pzD0star=pionVEC.pz()+trackFourVectorSum_D0_KM.pz();
				ptD0Star=sqrt(pxD0star*pxD0star+pyD0star*pyD0star);
			
				double ED0Star=pionVEC.energy()+trackFourVectorSum_D0_KM.energy();

				double MD0Star=ED0Star*ED0Star-(pxD0star*pxD0star+pyD0star*pyD0star+pzD0star*pzD0star);
				massD0Star=MD0Star>0?sqrt(MD0Star):0;
			}
			else{
				double pxD0star=pionVEC.px()+trackFourVectorSum_D0_KP.px();
                                double pyD0star=pionVEC.py()+trackFourVectorSum_D0_KP.py();
                                double pzD0star=pionVEC.pz()+trackFourVectorSum_D0_KP.pz();
                                ptD0Star=sqrt(pxD0star*pxD0star+pyD0star*pyD0star);

                                double ED0Star=pionVEC.energy()+trackFourVectorSum_D0_KP.energy();

                                double MD0Star=ED0Star*ED0Star-(pxD0star*pxD0star+pyD0star*pyD0star+pzD0star*pzD0star);
                                massD0Star=MD0Star>0?sqrt(MD0Star):0;
			}





			// D*(+-)->pion(+-) D0, with D0->kaon(-+) pion(+-)  	
			// Juan:
			//Lo mas seguro es seleccionar D*->D0 + "slow pion". Este slow pion es primario, no secundario. Por los tests que he estado haciendo esto se podria hacer casi automaticamente seleccionando trazas primarias (piones) con pt>0.5 GeV que difieran en angulo de la direccion del D0 en deltaR<0.05. La masa del D* menos la masa del D0, deltaM, tiene un picazo en unos 145 MeV. Aceptando todas las que tienen abs(deltaM-145)<0.015 MeV es suficiente, o si no guardas simplemente la masa que da menor deltaM. Asignar la carga es facil. El signo del pion slow es el mismo que el del D* y opuesto al signo del Kaon (el D*+ es un sistema "c antid", que se va a desintgar en "c antiu" (D0) mas "u antid" (pion slow positivo); lo mismo pero al reves para un D*-).
			//El D* del que hablamos es el que en el PDG tiene masa 2010 MeV.   

                        chargePION=(*pion)->charge();
			vertexMass_D0Star = trackFourVectorSum_D0Star.M();
                        vertexPT_D0Star = trackFourVectorSum_D0Star.pt();

                        vertexMass_D0Star_KM = trackFourVectorSum_D0Star_KM.M();
                        vertexPT_D0Star_KM = trackFourVectorSum_D0Star_KM.pt();

                        vertexMass_D0Star_KP = trackFourVectorSum_D0Star_KP.M();
                        vertexPT_D0Star_KP = trackFourVectorSum_D0Star_KP.pt();


			deltaM_D0Star_D0=vertexMass_D0Star-vertexMass_D0_KM;
			if( (*pion)->charge()<0 ) deltaM_D0Star_D0=vertexMass_D0Star-vertexMass_D0_KP;

		        if(verbose_){	
			cout<<"V1---> : "<<massD0Star<<"  V2-->"<<vertexMass_D0Star<<endl;
			cout<<"V1---> : "<<ptD0Star<<"  V2-->"<<vertexPT_D0Star<<endl;
			}	
          }    } 

		if(verbose_){
			cout<<"MASS:  "<<jet.userFloat("mass_SSV")<<"     nTracks:  "<<nTracksSV<<"CHARGE:    "<<chargeSV<<endl;
			cout<<"2 Tracks Things: "<<vertexMass<<"    D0: "<<vertexMass_D0<<endl;
			cout<<"            D0*: "<<vertexMass_D0Star<<"  and DeltaM:  "<<deltaM_D0Star_D0<<endl;
                        cout<<"3 Tracks Things: "<<vertexMass<<"    DPM: "<<vertexMass_DPM<<endl;
		}					

	

   }}

	        if(verbose_) cout<<"Finished!: "<<endl;
    }

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, FindResonancesExtra, "FindResonancesExtra");




