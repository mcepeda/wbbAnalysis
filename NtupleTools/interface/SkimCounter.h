#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Provenance/interface/RunID.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"

class TH1;
class TTree;
class TFileDirectory;
namespace edm {
  class LuminosityBlockBase;
}

class SkimCounter : public edm::BasicAnalyzer {
  public:
    SkimCounter(const edm::ParameterSet& pset, TFileDirectory& fs);
    virtual ~SkimCounter();
    void beginJob() {}
    void endJob();
    // Alias for filter with no return value
    bool filter(const edm::EventBase& evt);
    void analyze(const edm::EventBase& evt);
    // Do nothing at beginning
    void beginLuminosityBlock(const edm::LuminosityBlockBase& ls){};
    void endLuminosityBlock(const edm::LuminosityBlockBase& ls);

  private:
    edm::InputTag src_;
    std::string name_;
    TFileDirectory& fs_;
    edm::ParameterSet analysisCfg_;
    edm::InputTag evtSrc_;

    // For counting events
    TH1* eventCounter_;
    TH1* eventCounterWeighted_;
    TH1* eventWeights_;
    // For keeping track of the skimming
    edm::InputTag skimCounter_;
    TH1* skimEventCounter_;
    // For counting the luminosity
    edm::InputTag lumiProducer_;
    TH1* integratedLumi_;

    bool filter_;

    Int_t originalEvents;
};

