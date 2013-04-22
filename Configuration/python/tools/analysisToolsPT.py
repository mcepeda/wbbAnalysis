
import FWCore.ParameterSet.Config as cms
from CommonTools.ParticleFlow.Isolation.tools_cfi import *

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.helpers import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
import sys


def defaultReconstructionPT(process,triggerProcess = 'HLT',triggerPaths = ['HLT_Mu9','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v2','HLT_Mu15_v1','HLT_Mu15_v2']):
  process.load("UWAnalysis.Configuration.startUpSequence_cff")
  process.load("Configuration.StandardSequences.Geometry_cff")
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.StandardSequences.Services_cff")
  process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.load("DQMServices.Core.DQM_cfg")
  process.load("DQMServices.Components.DQMEnvironment_cfi")

  #Make the TriggerPaths Global variable to be accesed by the ntuples
  global TriggerPaths
  TriggerPaths= triggerPaths
  process.analysisSequence = cms.Sequence()
  
  #Add trigger Matching
  muonTriggerMatchPT(process,triggerProcess)
  electronTriggerMatchPT(process,triggerProcess)
  tauTriggerMatchPT(process,triggerProcess)
  
  #Build good vertex collection
  goodVertexFilter(process)
  
  #Default selections for systematics
  applyDefaultSelectionsPT(process)
  
  #Build MVA MET
  mvaMet(process)
  
  process.runAnalysisSequence = cms.Path(process.analysisSequence)


def mvaMet(process):
  
  process.load("RecoMET.METProducers.mvaPFMET_cff")
  process.load("PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi")
  
  process.metEleTaus = cms.EDFilter("PATTauSelector",
                                           src = cms.InputTag("cleanPatTaus"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>20&&tauID("byLooseIsolationMVA")&&tauID("againstElectronMedium")&&tauID("againstElectronMVA")&&tauID("againstMuonLoose")&&abs(eta())<2.3'),
                                           filter = cms.bool(False)
  										)  
  process.metMuTaus = cms.EDFilter("PATTauSelector",
                                           src = cms.InputTag("cleanPatTaus"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>20&&tauID("byLooseIsolationMVA")&&tauID("againstElectronLoose")&&tauID("againstMuonTight")&&abs(eta())<2.3'),
                                           filter = cms.bool(False)
  										)    										
  process.metElectrons = cms.EDFilter("PATElectronSelector",
                                           src = cms.InputTag("cleanPatElectrons"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>20&&userFloat("wp95V")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&!(userFloat("hasConversion")>0)&&userInt("missingHits")==0&&abs(userFloat("ipDXY"))<0.045&&abs(eta())<2.1'),
                                           filter = cms.bool(False)
  										)
  process.metMuons = cms.EDFilter("PATMuonSelector",
                                           src = cms.InputTag("cleanPatMuons"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>17&&userInt("tightID")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&abs(eta())<2.1&&abs(userFloat("ipDXY"))<0.045'),
                                           filter = cms.bool(False)
  										) 
  
  process.pfMEtMVA2.inputFileNames.U = cms.FileInPath('pharris/MVAMet/data/gbrmet_42.root')
  process.pfMEtMVA2.inputFileNames.DPhi = cms.FileInPath('pharris/MVAMet/data/gbrmetphi_42.root')
  process.pfMEtMVA2.inputFileNames.CovU1 = cms.FileInPath('pharris/MVAMet/data/gbrmetu1cov_42.root')
  process.pfMEtMVA2.inputFileNames.CovU2 = cms.FileInPath('pharris/MVAMet/data/gbrmetu2cov_42.root')
  

  process.mvaMetMuTau = process.pfMEtMVA2.clone()
  process.mvaMetMuTau.srcLeptons = cms.VInputTag('metMuons', 'metMuTaus')  
  process.patMVAMetMuTau = process.patMETs.clone(
  	metSource = cms.InputTag('mvaMetMuTau'),
  	addMuonCorrections = cms.bool(False),
  	addGenMET = cms.bool(False)
  )
  
  process.mvaMetEleTau = process.pfMEtMVA2.clone()
  process.mvaMetEleTau.srcLeptons = cms.VInputTag('metElectrons', 'metEleTaus')  
  process.patMVAMetEleTau = process.patMETs.clone(
  	metSource = cms.InputTag('mvaMetEleTau'),
  	addMuonCorrections = cms.bool(False),
  	addGenMET = cms.bool(False)
  )  
  
  process.mvaMetMuTauSequence = cms.Sequence(process.metMuTaus*process.metMuons*process.mvaMetMuTau*process.patMVAMetMuTau) 
  process.mvaMetEleTauSequence = cms.Sequence(process.metEleTaus*process.metElectrons*process.mvaMetEleTau*process.patMVAMetEleTau)
  process.analysisSequence = cms.Sequence(process.analysisSequence*process.mvaMetMuTauSequence*process.mvaMetEleTauSequence)
  



def applyDefaultSelectionsPT(process):
  #DONT CHANGE THOSE HERE:: THEY ARE NOT USED FOR YOUR SELECTIONS!!!
  #ONLY FOR SYSTEMATICS . PLEASE CHANGE THEM in YOUR CFG FILE IF REALLY NEEDED
  process.selectedPatTaus = cms.EDFilter("PATTauSelector",
                                           src = cms.InputTag("cleanPatTaus"),
                                           cut = cms.string('pt>15&&tauID("byLooseIsolationMVA")&&tauID("againstElectronLoose")&&tauID("againstMuonLoose")'),
                                           filter = cms.bool(False)
  										)  
  process.selectedPatElectrons = cms.EDFilter("PATElectronSelector",
                                           src = cms.InputTag("cleanPatElectrons"),
                                           cut = cms.string('pt>10&&userFloat("wp95")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.3'),
                                           filter = cms.bool(False)
  										)
  process.selectedPatMuons = cms.EDFilter("PATMuonSelector",
                                           src = cms.InputTag("cleanPatMuons"),
                                           cut = cms.string('pt>10&&userInt("tightID")&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.3'),
                                           filter = cms.bool(False)
  										) 
  process.cleanPatJets = cms.EDProducer("PATJetCleaner",
  										   src = cms.InputTag("selectedPatJets"),
  										   preselection = cms.string('abs(eta)<5.0&&userFloat("idLoose")>0&&pt>10&&userInt("fullIdLoose")>0'),
  										   checkOverlaps = cms.PSet(),
  										   finalCut = cms.string('')
  										)								 									  
  process.selectedObjectsForSyst = cms.Sequence(process.selectedPatTaus+process.selectedPatElectrons+process.selectedPatMuons+process.cleanPatJets)
  process.analysisSequence = cms.Sequence(process.analysisSequence*process.selectedObjectsForSyst)


def muonTriggerMatchPT(process,triggerProcess):

   process.triggeredPatMuons = cms.EDProducer("MuonTriggerMatcher",
                                            src = cms.InputTag("cleanPatMuons"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered12','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered15','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered24','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau15','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu12IsoPFTau10','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered17','',triggerProcess),                                               
                                                cms.InputTag('hltSingleMuIsoL1s14L3IsoFiltered15eta2p1',"",triggerProcess),
                                                cms.InputTag('hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10','',triggerProcess),
                                                cms.InputTag('hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10','',triggerProcess)
                                            ),
                                            pdgId = cms.int32(13)
   )
  
   process.analysisSequence*= process.triggeredPatMuons

def electronTriggerMatchPT(process,triggerProcess):

   process.triggeredPatElectronsL = cms.EDProducer("ElectronTriggerMatcher",
                                            src = cms.InputTag("cleanPatElectrons"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltEle17CaloIdLCaloIsoVLPixelMatchFilterDoubleEG125','',triggerProcess),
                                            ),
                                            pdgId = cms.int32(0)
   )
   process.triggeredPatElectrons = cms.EDProducer("ElectronTriggerMatcher",
                                            src = cms.InputTag("triggeredPatElectronsL"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltOverlapFilterIsoEle15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle15TightIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle18MediumIsoPFTau20','',triggerProcess),                                                
                                                cms.InputTag('hltOverlapFilterIsoEle18TightIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle18IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle20MediumIsoPFTau20','',triggerProcess)
                                            ),
                                            pdgId = cms.int32(11)
   )
   
   process.analysisSequence=cms.Sequence(process.analysisSequence*process.triggeredPatElectronsL*process.triggeredPatElectrons)

def tauTriggerMatchPT(process,triggerProcess):
   strTrig=''
   for i in TriggerPaths:
    if i==TriggerPaths[0]:
      strTrig+='path(\"'+i+'\")'
    else:  
      strTrig+='|| path(\"'+i+'\")'


   #Match With The triggers
   process.preTriggeredPatTaus = cms.EDProducer("TauTriggerMatcher",
                                            src = cms.InputTag("cleanPatTaus"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltFilterDoubleIsoPFTau20Trk5LeadTrack5IsolationL1HLTMatched','',triggerProcess)
                                            ),
                                            pdgId = cms.int32(0)
   )

   process.triggeredPatTaus = cms.EDProducer("TauTriggerMatcher",
                                            src = cms.InputTag("preTriggeredPatTaus"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau15','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15MediumIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15TightIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterMu15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu12IsoPFTau10','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle15TightIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle18MediumIsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle20MediumIsoPFTau20','',triggerProcess),                                              
                                            ),

                                            pdgId = cms.int32(15)
   )
                                            
   process.analysisSequence=cms.Sequence(process.analysisSequence*process.preTriggeredPatTaus*process.triggeredPatTaus)
  

def getAllEventCounters(process,path,onSkim = False):
        stringList = []
        if onSkim:
          stringList.append('processedEvents')

        modules = listModules(path)
    
        for mod in modules:
            if(hasattr(mod,'label')):
                if mod.label().find('Counter') !=-1 :
                    stringList.append(mod.name.value())
        print 'List Of Filters'        
        print stringList
        
        return cms.untracked.vstring(stringList)

def addEventSummary(process,onSkim = False,name = 'summary',path = 'eventSelection'):
    
   process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )

   summary = cms.EDAnalyzer('EventSummary',
                            src =getAllEventCounters(process,getattr(process,path),onSkim)
   )

   setattr(process,name,summary)
   if onSkim:
        process.EDMtoMEConverter = cms.EDAnalyzer("EDMtoMEConverter",
                                               Name = cms.untracked.string('EDMtoMEConverter'),
                                               Verbosity = cms.untracked.int32(1), # 0 provides no output
                                               # 1 provides basic output
                                               Frequency = cms.untracked.int32(50),
                                               convertOnEndLumi = cms.untracked.bool(True),
                                               convertOnEndRun = cms.untracked.bool(True)
                                               )
        eventSummaryPath=cms.EndPath(process.EDMtoMEConverter+getattr(process,name))
        setattr(process,name+"Path",eventSummaryPath)
   else:
        eventSummaryPath=cms.EndPath(getattr(process,name))
        setattr(process,name+"Path",eventSummaryPath)



def createGeneratedParticles(process,name,commands):


  refObjects = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "drop  *  " 
    )
   )
  refObjects.select.extend(commands)
  setattr(process,name,refObjects)
  process.analysisSequence*= getattr(process,name)


def createGeneratedTaus(process,decayMode,fiducialCuts):
  process.generatedTaus = cms.EDFilter("TauGenJetDecayModeSelector",
                                       src = cms.InputTag("tauGenJets"),
                                       select = cms.vstring(decayMode),
                                       filter = cms.bool(False)
                                       )
  process.generatedTausInAcceptance = cms.EDFilter("GenJetSelector",
                                           src = cms.InputTag("generatedTaus"),
                                           cut = cms.string(fiducialCuts),
                                           filter = cms.bool(False)
                                           )

  process.analysisSequence*= process.generatedTaus
  process.analysisSequence*= process.generatedTausInAcceptance


def goodVertexFilter(process):

  process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                        src = cms.InputTag('offlinePrimaryVertices'),
                                        cut = cms.string('ndof()>4&&position().rho()<2&&abs(z())<24'),
                                        filter = cms.bool(False)
  )   
  process.analysisSequence*= process.primaryVertexFilter  


def cloneAndReplaceInputTag(process,sequence,oldValue,newValue,postfix):
  #First Clone the sequence
  p = cloneProcessingSnippet(process, sequence, postfix)
  massSearchReplaceAnyInputTag(p,oldValue,newValue )
  modules = listModules(p)

  #Change the labels of the counters
  for mod in modules:
    if(hasattr(mod,'label')):
      if mod.label().find('Counter') !=-1 :
        if(hasattr(mod,'name')):
          newValue = mod.name.value()+postfix
          mod.name=cms.string(newValue)
  return p



def createSystematics(process,sequence,postfix,muScale,eScale,tauScale,jetScale,unclusteredScale,electronresb = 0.0, electronrese = 0.0):

  #First Clone the sequence
  p = cloneProcessingSnippet(process, sequence, postfix)
  modules = listModules(p)

  #Change the labels of the counters and the smearign modules
  for mod in modules:
    if(hasattr(mod,'label')):
      if mod.label().find('Counter') !=-1 :
        if(hasattr(mod,'name')):
          newValue = mod.name.value()+postfix
          mod.name=cms.string(newValue)
      if mod.label().find('smearedMuons') !=-1 :
          mod.energyScale = cms.double(muScale)
      if mod.label().find('smearedTaus') !=-1 :
          mod.energyScale = cms.double(tauScale)
      if mod.label().find('smearedElectrons') !=-1 :
          mod.energyScale = cms.double(eScale)
          mod.deltaPtB = cms.double(electronresb)
          mod.deltaPtE = cms.double(electronrese)
      if mod.label().find('smearedJets') !=-1 :
          mod.energyScaleDB = cms.int32(jetScale)
      if mod.label().find('smearedMET') !=-1 :
          mod.unclusteredScale= cms.double(unclusteredScale)
  return cms.Path(p) 





def createRecoilSystematics(process,sequence,postfix,metScale,metResolution):

  #First Clone the sequence
  p = cloneProcessingSnippet(process, sequence, postfix)
  modules = listModules(p)

  #Change the labels of the counters and the smearign modules
  for mod in modules:
    if(hasattr(mod,'label')):
      if mod.label().find('Counter') !=-1 :
        if(hasattr(mod,'name')):
          newValue = mod.name.value()+postfix
          mod.name=cms.string(newValue)
    if(hasattr(mod,'metCalibration')):
          mod.metCalibration.shiftScale = cms.untracked.double(metScale)
          mod.metCalibration.shiftRes   = cms.untracked.double(metResolution)
      
  return cms.Path(p) 







   

