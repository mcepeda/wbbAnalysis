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


class FindResonances : public NtupleFillerBase {
 public:

    FindResonances(){
    }

    FindResonances(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src")),
    srcVertices_(iConfig.getParameter<edm::InputTag>("srcPrimaryVertices")),
    jetindex_(iConfig.getParameter<int>("jetindex"))
	{
//	   infoNUP=-1;
//            t->Branch("infoNUP",&infoNUP,"infoNUP/F");
//            t->Branch("CCandsPhi","std::vector<float>",&CCandsPhi);
		t->Branch("vertexMass",&vertexMass,"vertexMass/F");

                t->Branch("vertexMass_D0",&vertexMass_D0,"vertexMass_D0/F");
                t->Branch("vertexMass_DPM",&vertexMass_DPM,"vertexMass_DPM/F");

		t->Branch("vertexMass_D0_KP",&vertexMass_D0_KP,"vertexMass_D0_KP/F");
		t->Branch("vertexMass_D0_KM",&vertexMass_D0_KM,"vertexMass_D0_KM/F");
		t->Branch("vertexMass_D0Star",&vertexMass_D0Star,"vertexMass_D0Star/F");
		t->Branch("deltaM_D0Star_D0",&deltaM_D0Star_D0,"deltaM_D0Star_D0/F");

		t->Branch("chargeSV",&chargeSV,"chargeSV/F");
		t->Branch("nTracksSV",&nTracksSV,"nTracksSV/F");
	}
      
      ~FindResonances()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  edm::InputTag src_;
  edm::InputTag srcVertices_;
  int jetindex_;

	float vertexMass;
	float vertexMass_D0_KP; 
	float vertexMass_D0_KM; 
	float vertexMass_DPM;

	float vertexMass_D0;
	float vertexMass_D0Star;
	float deltaM_D0Star_D0;
	float chargeSV;
	float nTracksSV;
};


void FindResonances::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
    using namespace edm;
    using namespace reco;

	

    edm::Handle<reco::VertexCollection> vertexHandle;
    iEvent.getByLabel(srcVertices_, vertexHandle);
   std::auto_ptr<pat::JetCollection > out(new pat::JetCollection);
    edm::Handle<pat::JetCollection > cands;

    if(!iEvent.getByLabel(src_,cands)) LogError("")<<"Jet collection does not exist!!!"<<endl;

    int JETSIZE=cands->size();
  
        vertexMass=-777;        
        vertexMass_D0_KP=-777;        
        vertexMass_D0_KM=-777;        
        vertexMass_DPM=-777;
        vertexMass_D0=-777;
        vertexMass_D0Star=-777;
        deltaM_D0Star_D0=-777;
        chargeSV=-777;
        nTracksSV=-777;
	
 
	if(jetindex_<JETSIZE&&JETSIZE>0&&vertexHandle->size()>0){


//      for(unsigned int  i=0;i!=cands->size();++i){

        pat::Jet jet = cands->at(jetindex_);

        if(verbose_) cout<<"Reading Jet: "<<jet.pt()<<endl;

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

                vertexMass_D0=-1;
		// I just want 1 combination to be possible, not two

                if( fabs(vertexMass_D0_KP-1.87)<=0.05 && fabs(vertexMass_D0_KM-1.87)>0.05)  vertexMass_D0=vertexMass_D0_KP;
                else if( fabs(vertexMass_D0_KP-1.87)>0.05 && fabs(vertexMass_D0_KM-1.87)<=0.05)  vertexMass_D0=vertexMass_D0_KM;

		// Now for D0*


              vertexMass_D0Star=0;
	      deltaM_D0Star_D0 =0;

	      if(nTracksSV==2 &&chargeSV==0){	
		
              reco::Vertex::trackRef_iterator pion;

	      float angleMIN=9999;	

               for(reco::Vertex::trackRef_iterator track = vertexHandle->at(0).tracks_begin();
                    track != vertexHandle->at(0).tracks_end(); ++track) {
//			if((*track)->pt()<0.5) continue;
                       float angle=sqrt( (dir.x()-(*track)->px())*(dir.x()-(*track)->px())+(dir.y()-(*track)->py())*(dir.y()-(*track)->py())+(dir.z()-(*track)->pz())*(dir.z()-(*track)->pz()));
                         if(angle<angleMIN) {angleMIN=angle; pion=track;}
                }
                        cout<<angleMIN<<endl;

                        cout<<"PION --> "<<(*pion)->px()<<","<<(*pion)->py()<<","<<(*pion)->pz()<<endl;

                        ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float> > pionVEC;
                        pionVEC.SetPx((*pion)->px());
                        pionVEC.SetPy((*pion)->py());
                        pionVEC.SetPz((*pion)->pz());
                        pionVEC.SetM(mass_pion);      // pion mass
                        trackFourVectorSum_D0Star += pionVEC;
			cout<<"CHECK D0:"<<endl;
			cout<<pionVEC.M()<<" = ? "<<trackFourVectorSum_D0Star.M()<<endl;

			if((*pion)->charge()>0) trackFourVectorSum_D0Star+=trackFourVectorSum_D0_KM; 
			else if( (*pion)->charge()<0 ) trackFourVectorSum_D0Star+=trackFourVectorSum_D0_KP;
			cout<<"Added D0? "<<endl;
			cout<<(*pion)->charge()<<" "<<trackFourVectorSum_D0_KM.M()<<"  "<<trackFourVectorSum_D0_KP.M()<<endl;
			cout<<"---->"<<trackFourVectorSum_D0Star.M()<<endl;

		

			// D*(+-)->pion(+-) D0, with D0->kaon(-+) pion(+-)  	
			// Juan:
			//Lo mas seguro es seleccionar D*->D0 + "slow pion". Este slow pion es primario, no secundario. Por los tests que he estado haciendo esto se podria hacer casi automaticamente seleccionando trazas primarias (piones) con pt>0.5 GeV que difieran en angulo de la direccion del D0 en deltaR<0.05. La masa del D* menos la masa del D0, deltaM, tiene un picazo en unos 145 MeV. Aceptando todas las que tienen abs(deltaM-145)<0.015 MeV es suficiente, o si no guardas simplemente la masa que da menor deltaM. Asignar la carga es facil. El signo del pion slow es el mismo que el del D* y opuesto al signo del Kaon (el D*+ es un sistema "c antid", que se va a desintgar en "c antiu" (D0) mas "u antid" (pion slow positivo); lo mismo pero al reves para un D*-).
			//El D* del que hablamos es el que en el PDG tiene masa 2010 MeV.   


			vertexMass_D0Star = trackFourVectorSum_D0Star.M();
			deltaM_D0Star_D0=vertexMass_D0Star-vertexMass_D0_KM;
			if( (*pion)->charge()<0 ) deltaM_D0Star_D0=vertexMass_D0Star-vertexMass_D0_KP;

          }     

		if(verbose_){
			cout<<"MASS:  "<<jet.userFloat("mass_SSV")<<"     nTracks:  "<<nTracksSV<<"CHARGE:    "<<chargeSV<<endl;
			cout<<"2 Tracks Things: "<<vertexMass<<"    D0: "<<vertexMass_D0<<endl;
			cout<<"            D0*: "<<vertexMass_D0Star<<"  and DeltaM:  "<<deltaM_D0Star_D0<<endl;
                        cout<<"3 Tracks Things: "<<vertexMass<<"    DPM: "<<vertexMass_DPM<<endl;
		}					


	

   }}

	        if(verbose_) cout<<"Finished!: "<<endl;


}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, FindResonances, "FindResonances");




