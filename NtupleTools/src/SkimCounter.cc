#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "UWAnalysis/NtupleTools/interface/SkimCounter.h"
#include "FWCore/Common/interface/LuminosityBlockBase.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "TH1F.h"
#include "TTree.h"

#include <sstream>

SkimCounter::SkimCounter(
    const edm::ParameterSet& pset, TFileDirectory& fs):
  BasicAnalyzer(pset, fs),fs_(fs) {
  filter_ = pset.exists("filter") ? pset.getParameter<bool>("filter") : false;
  skimCounter_ = pset.getParameter<edm::InputTag>("skimCounter");

  originalEvents=0;  

  // Build the event counter histos.

  eventCounter_ = fs_.make<TH1F>("eventCount", "Events Processed", 1, -0.5, 0.5);
  eventCounterWeighted_ = fs_.make<TH1F>(
      "eventCountWeighted", "Events Processed (weighted)", 1, -0.5, 0.5);
  eventWeights_ = fs_.make<TH1F>(
      "eventWeights", "Events Weights", 100, 0, 5);
  skimEventCounter_ = fs_.make<TH1F>(
      "skimCounter", "Original Events Processed", 1, -0.5, 0.5);
}

SkimCounter::~SkimCounter() { }

void SkimCounter::endLuminosityBlock(
    const edm::LuminosityBlockBase& ls) {
  //std::cout << "Analyzing lumisec: " << ls.id() << std::endl;

  edm::Handle<edm::MergeableCounter> skimmedEvents;
  ls.getByLabel(skimCounter_, skimmedEvents);
  skimEventCounter_->Fill(0.0, skimmedEvents->value);

  originalEvents+=skimmedEvents->value;

}

bool SkimCounter::filter(const edm::EventBase& evt) {
  // Get the event weight
  double eventWeight = 1.0;

  // Count this event
  eventCounter_->Fill(0.0);
  eventCounterWeighted_->Fill(0.0, eventWeight);
  eventWeights_->Fill(eventWeight);

  // Hack workarounds into ntuple here
  // bool mustCleanupFinalStates = true;
  // do something

  return true;

}

void SkimCounter::analyze(const edm::EventBase& evt) {
  filter(evt);
}

void SkimCounter::endJob() {
  std::cout << "Before Skim: " << originalEvents << std::endl;
}
