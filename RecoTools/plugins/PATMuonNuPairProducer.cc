#include "UWAnalysis/RecoTools/interface/CompositePtrCandidateTMEtProducer.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include  "UWAnalysis/RecoTools/plugins/PATMuonIpProducer.h"

typedef CompositePtrCandidateTMEtProducer<pat::Electron> PATElectronNuPairProducer;
typedef CompositePtrCandidateTMEtProducer<pat::Muon> PATMuonNuPairProducer;
typedef CompositePtrCandidateTMEtProducer<reco::Candidate> PATCandNuPairProducer;
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronNuPairProducer);
DEFINE_FWK_MODULE(PATMuonNuPairProducer);
DEFINE_FWK_MODULE(PATCandNuPairProducer);
