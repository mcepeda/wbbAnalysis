import FWCore.ParameterSet.Config as cms


from DQMServices.Core.DQM_cfg import *
from DQMServices.Components.DQMEnvironment_cfi import *
from DQMServices.Components.MEtoEDMConverter_cfi import *
from DQMServices.Components.EDMtoMEConverter_cfi import *

EDMtoMEConverter.Verbosity = cms.untracked.int32(1)
EDMtoMEConverter.convertOnEndLumi = cms.untracked.bool(True)
EDMtoMEConverter.convertOnEndRun = cms.untracked.bool(False)
EDMtoMEConverter.Frequency = cms.untracked.int32(1)

saveHistos = cms.Sequence(MEtoEDMConverter)
loadHistos = cms.Sequence(EDMtoMEConverter)


initialCounter = cms.EDProducer('EventCounter',
                         name = cms.string("initialEvents")
)

startupSequence = cms.Sequence(initialCounter)


startupSequenceFromSkim = cms.Sequence(loadHistos)

