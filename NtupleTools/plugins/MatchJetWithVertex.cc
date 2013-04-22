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
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "Math/GenVector/VectorUtil.h"


class MatchJetWithVertex : public NtupleFillerBase {
 public:

    MatchJetWithVertex(){
    }

    MatchJetWithVertex(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src")),
    srcVertices_(iConfig.getParameter<edm::InputTag>("srcPrimaryVertices")),
    jetindex_(iConfig.getParameter<int>("jetindex")),
    tag_(iConfig.getParameter<std::string>("tag"))
	{
          value = new std::vector<double>();
          singleValue=0;
          if(jetindex_==-1)
            vbranch = t->Branch(tag_.c_str(),"std::vector<double>",&value);
          else
            vbranch = t->Branch(tag_.c_str(),&singleValue,(tag_+"/F").c_str());
	}
      
      ~MatchJetWithVertex()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  edm::InputTag src_;
  edm::InputTag srcVertices_;
  int jetindex_;
  std::string tag_; 

  std::vector<double>* value;
  float singleValue;
  TBranch *vbranch;

};


void MatchJetWithVertex::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
    using namespace edm;
    using namespace reco;

    edm::Handle<reco::VertexCollection> vertexHandle;
    iEvent.getByLabel(srcVertices_, vertexHandle);
    std::auto_ptr<pat::JetCollection > out(new pat::JetCollection);
    edm::Handle<pat::JetCollection > cands;


    singleValue=-777;

    if(value->size()>0)
            value->clear();


    if(vertexHandle->size()>=1){ // Otherwise seg fault, even if events with 0 vertices should not exist... Mistery.

    if(!iEvent.getByLabel(src_,cands)) LogError("")<<"Jet collection does not exist!!!"<<endl;

    int JETSIZE=cands->size();
 
	if(jetindex_>JETSIZE) LogWarning("")<<"Careful, you are asking for a jet that does not exist"<<endl;
	else {

	for (int index=0; index<JETSIZE; index++){

        if(jetindex_!=-1 && jetindex_!=index) continue; // -1 will give us a vector of all jets, otherwise just use the one you picked	

        pat::Jet jet = cands->at(index);

                if(verbose_){
                        cout<<"Checking Matches? JET:  "<<jet.pt()<<endl;
  	                cout<<"     (PV-->"<<vertexHandle->at(0).position().x()<<"  " <<vertexHandle->at(0).position().y()<<" "<<vertexHandle->at(0).position().z()<<")"<<endl;
                }

        double minDistance=99999;

          int nConst=jet.nConstituents();

	  double AverageDXY=0, X0=0, Y0=0;
	  int ntracks=0;

          for(int i=0; i<nConst; i++){
                         reco::PFCandidate cand=jet.getPFConstituent(i);

			 if(cand.trackRef().isNull()) continue; // we do not want neutral constituents	}
			 				 
			  double candDXY=cand.trackRef()->dxy(vertexHandle->at(0).position());
                          if(verbose_) cout<<"PDGID "<<cand.pdgId()<<"--->"<<candDXY<<endl;

			  AverageDXY+=candDXY; ntracks++; // careful, this is not as useful as it sounds, it is not the real center of the jet
			  X0+=cand.trackRef()->vx();
                          Y0+=cand.trackRef()->vy();

			  if(fabs(candDXY)<minDistance) minDistance=candDXY;  
			
			 

          }     

		if(verbose_){
			cout<<"JET:  "<<jet.pt()<<"  min distance --> "<<minDistance<<"  (average"<<AverageDXY/ntracks<<")"<<endl;
		}					

          singleValue=minDistance;
          value->push_back(minDistance); 

	}

   }}

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, MatchJetWithVertex, "MatchJetWithVertex");




