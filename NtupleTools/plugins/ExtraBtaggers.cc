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


class ExtraBTaggers : public NtupleFillerBase {
 public:

    ExtraBTaggers(){
    }

    ExtraBTaggers(const edm::ParameterSet& iConfig, TTree* t):
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    src_(iConfig.getParameter<edm::InputTag>("src")),
    srcBTAG_(iConfig.getParameter<edm::InputTag>("srcBTAG")),
    btagDisc_(iConfig.getParameter<std::string>("btagDisc")),
    tag_(iConfig.getParameter<std::string>("tag")),
    jetindex_(iConfig.getParameter<int>("jetindex"))
//    jet_(iConfig.getUntrackedParameter<pat::Jet>("Jet"))   :-( some library link missing (compiles but fails at the last step. shame.
	{
			btagDiscNew=-777;
		t->Branch(tag_.c_str(),&btagDiscNew,(tag_+"/F").c_str());
	}
      
      ~ExtraBTaggers()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  edm::InputTag src_;
  edm::InputTag srcBTAG_;
  edm::InputTag srcVertices_;
  int jetindex_;
  std::string tag_;
  std::string btagDisc_;

	float btagDiscNew;
};


void ExtraBTaggers::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
    using namespace edm;
    using namespace reco;

	
    edm::Handle<pat::JetCollection > cands;
    edm::Handle<pat::JetCollection > cands2;

    if(!iEvent.getByLabel(src_,cands)) LogError("")<<"Jet collection does not exist!!!"<<endl;
    if(!iEvent.getByLabel(srcBTAG_,cands2)) LogError("")<<"Jet BTAG collection does not exist!!!"<<endl;


    int JETSIZE=cands->size();
  
        btagDiscNew=-777;
 
	if(jetindex_<JETSIZE&&JETSIZE>0){


        pat::Jet jetORIGINAL = cands->at(jetindex_);


        if(verbose_) cout<<"Reading Jet: "<<jetORIGINAL.pt()<<endl;

	unsigned int indexTAG=999; 

	for (unsigned int  j=0;j!=cands2->size();++j){
		pat::Jet jetBTAG = cands2->at(j);
		double dR=sqrt( (jetORIGINAL.eta()-jetBTAG.eta())*(jetORIGINAL.eta()-jetBTAG.eta()) +  (jetORIGINAL.phi()-jetBTAG.phi())*(jetORIGINAL.phi()-jetBTAG.phi()) );
		if(dR==0) indexTAG=j; 
	}	

	if(indexTAG==999) LogError("")<<"This Jet does not match!!!"<<endl;
	else{
	pat::Jet jet=cands2->at(indexTAG);

		btagDiscNew=jet.bDiscriminator(btagDisc_);

   	 }

	}

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, ExtraBTaggers, "ExtraBTaggers");




