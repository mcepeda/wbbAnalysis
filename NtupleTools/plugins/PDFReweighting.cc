// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class PDFReWeighting : public NtupleFillerBase {
 public:

    PDFReWeighting(){
    }

 //   virtual void beginJob() ;

    PDFReWeighting(const edm::ParameterSet& iConfig, TTree* t):

    genTag_(iConfig.getUntrackedParameter<edm::InputTag> ("GenTag", edm::InputTag("genParticles"))),
    pdfInfoTag_(iConfig.getUntrackedParameter<edm::InputTag> ("PdfInfoTag", edm::InputTag("generator"))),
    pdfSetNames_(iConfig.getUntrackedParameter<edm::InputTag> ("PdfSetNames")),
    verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
    tag_(iConfig.getUntrackedParameter<std::string> ("PdfWeights","PdfWeights"))
	{
            weight=1;
            t->Branch(tag_.c_str(),&weight,(tag_+"/F").c_str());

	}
      
      ~PDFReWeighting()
	{ 
	  
	}
      

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);


 protected:
  bool verbose_;
  edm::InputTag src_;


  edm::InputTag genTag_;
  edm::InputTag pdfInfoTag_;
  edm::InputTag pdfSetNames_;
  std::vector<std::string> pdfShortNames_;
  
  std::string tag_;

  float weight;  


};

/*
void PDFReWeighting::beginJob() {
      for (unsigned int k=1; k<=pdfSetNames_.size(); k++) {
            LHAPDF::initPDFSet(k,pdfSetNames_[k-1]);
	    if(verbose_){
			std::cout<<"Weighting by: "<<pdfSetNames_.at(k-1)<<std::endl;
	     }
      }
}
*/

void PDFReWeighting::fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
    using namespace std; 
/*

    if(!setPDFs){

    for (unsigned int k=1; k<=pdfSetNames_.size(); k++) {
                    LHAPDF::initPDFSet(k,pdfSetNames_[k-1]);
                         if(verbose_){
                        std::cout<<"Weighting by: "<<pdfSetNames_.at(k-1)<<std::endl;
                }
    }

    setPDFs=true;
    }	

*/
      weight=1;


      if (iEvent.isRealData()) return;

      edm::Handle<std::vector<double> > weightHandle;
            if (iEvent.getByLabel(pdfSetNames_, weightHandle)) {
//	    cout<<"Weighting by "<<pdfSetNames_[0]<<endl;
            std::vector<double> pdfweights = (*weightHandle);
            unsigned int nmembers = pdfweights.size();
//	    cout<<nmembers<<" members in this pdf set"<<endl;
	    weight=pdfweights[0];
//	    cout<<"Central weight?"<<pdfweights[0]<<endl;	
	    }
	    else{ 
//	    cout<<"This Pdf does not exist!!!"<<endl;
	    } 	


}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_EDM_PLUGIN(NtupleFillerFactory, PDFReWeighting, "PDFReWeighting");




