// system include files
#include <memory>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include <TTree.h>
#include "UWAnalysis/NtupleTools/interface/NtupleFillerBase.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
//
// class decleration
//

class GenInfoFiller : public NtupleFillerBase {
 public:

    GenInfoFiller(){
    }

    GenInfoFiller(const edm::ParameterSet& iConfig, TTree* t):
      src_(iConfig.getParameter<edm::InputTag>("src"))
	{
	  genWeight=0;
          t->Branch("genWeight",&genWeight,"genWeight/F");
	}
      
      ~GenInfoFiller()
	{ 
	  
	}
       

  void fill(const edm::Event& iEvent,const edm::EventSetup& iSetup)
  {
    using namespace std; 
    genWeight=-777; 
    edm::Handle<GenEventInfoProduct> genInfo;
    if(iEvent.getByLabel(src_, genInfo)) {
		      genWeight = genInfo->weight();
      }
//	cout<<genWeight<<endl;

    }
  
  

 protected:
  edm::InputTag src_;
  float  genWeight;
};






