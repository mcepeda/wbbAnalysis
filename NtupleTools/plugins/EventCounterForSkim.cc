#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "TH2D.h"
#include "FWCore/Common/interface/LuminosityBlockBase.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/FWLite/interface/LuminosityBlockBase.h"

class EventCounterForSkim : public edm::EDAnalyzer {

public:
  EventCounterForSkim (const edm::ParameterSet &);
  void analyze(const edm::Event & iEvent, const edm::EventSetup& EventSetup);
  virtual void beginJob() ;
  virtual void endJob() ;

  void beginLuminosityBlock(const edm::LuminosityBlockBase& ls);
  void endLuminosityBlock(edm::Event& iEvent,const edm::LuminosityBlockBase& ls);

private:
   std::map<std::string,TH1D*> h1_;
   Int_t originalEvents;
   Int_t originalEvents2;
   Int_t skimmedEvents;
   Int_t nevents;

   edm::InputTag skimEventCounter_;
   double checkLUMI;


};
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <map>
#include <memory>

using namespace edm;
using namespace std;

EventCounterForSkim::EventCounterForSkim( const ParameterSet & cfg ) :
	skimEventCounter_(cfg.getParameter<edm::InputTag>("skimCounter"))
{
}

void EventCounterForSkim::beginJob() {
      originalEvents=0;  originalEvents2=0;
      skimmedEvents=0;
      nevents=0; 	

      checkLUMI=-1;
      edm::Service<TFileService> fs_;
      h1_["eventCounter"] = fs_->make<TH1D>("eventCount", "Events Processed", 1, -0.5, 0.5);
      h1_["skimEventCounter"] = fs_->make<TH1D>("skimCounter", "Original Events Processed", 1, -0.5, 0.5);
      h1_["skimEventCounter2"] = fs_->make<TH1D>("skimCounter2", "Original Events Processed", 1, -0.5, 0.5);

}

void EventCounterForSkim::endJob() {
  std::cout << "Events     : " <<nevents<<std::endl;
//  std::cout << "Before Skim: " << originalEvents << std::endl;
  std::cout << "Before Skim: " << originalEvents2 << std::endl;
  std::cout << "After  Skim: " << skimmedEvents << std::endl;
}

void EventCounterForSkim::beginLuminosityBlock(const edm::LuminosityBlockBase& ls){}

void EventCounterForSkim::endLuminosityBlock(edm::Event& iEvent,
    const edm::LuminosityBlockBase& ls) {
  std::cout << "Analyzing lumisec: " << ls.id() << std::endl;

                 double       EVENT  = iEvent.id().event();
                 double        RUN    = iEvent.id().run();
                 double        LUMI   = iEvent.luminosityBlock();

  std::cout << "Analyzing lumisec: " <<RUN<<"-"<<LUMI<<"-"<<EVENT<< std::endl;


  edm::Handle<edm::MergeableCounter> skimmedEvents;
  ls.getByLabel(skimEventCounter_, skimmedEvents);
//	cout<<"I have it? "<<skimmedEvents<<"---> "<<skimmedEvents->value;
  h1_["skimEventCounter"]->Fill(0.0, skimmedEvents->value);   // This does not work, why?

  originalEvents+=skimmedEvents->value;
}

void EventCounterForSkim::analyze(const edm::Event& iEvent, const edm::EventSetup&) {

  nevents++;


  // Get the event weight
  double eventWeight = 1.0;

  // Count this event
  h1_["eventCounter"]->Fill(0.0);

  skimmedEvents++;



                 double       EVENT  = iEvent.id().event();
                 double        RUN    = iEvent.id().run();
                 double        LUMI   = iEvent.luminosityBlock();

  //std::cout << "Analyzing lumisec: " <<RUN<<"-"<<LUMI<<"-"<<EVENT<< std::endl;

  const edm::LuminosityBlockBase& ls=(const edm::LuminosityBlockBase&)iEvent.getLuminosityBlock();
    edm::Handle<edm::MergeableCounter> skimmedEvents;
    ls.getByLabel(skimEventCounter_, skimmedEvents);


    if(LUMI!=checkLUMI) {cout<<"EndLumi? "<<skimmedEvents->value;
        originalEvents2+=skimmedEvents->value;
	      h1_["skimEventCounter2"]->Fill(0.0, skimmedEvents->value);
	checkLUMI=LUMI;
    }
		

//  return 0;

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EventCounterForSkim);
