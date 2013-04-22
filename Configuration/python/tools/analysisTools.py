
import FWCore.ParameterSet.Config as cms
from CommonTools.ParticleFlow.Isolation.tools_cfi import *

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.helpers import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *

from RecoBTag.ImpactParameter.impactParameter_cff import *
from RecoBTag.SecondaryVertex.secondaryVertex_cff import *



import sys


def defaultReconstructionSKIM(process,calibrateMET = False,calibrationScheme = "BothLegs"):
  process.load("UWAnalysis.Configuration.startUpSequence_cff")
  process.load("Configuration.StandardSequences.Geometry_cff")
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.StandardSequences.Services_cff")
  process.load("Configuration.StandardSequences.Reconstruction_cff")
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.load("DQMServices.Core.DQM_cfg")
  process.load("DQMServices.Components.DQMEnvironment_cfi")
  process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

  ##MET recoil calibration
  global metCalibration


  #make only what you need
  process.patDefaultSequence = cms.Sequence(
    process.makePatElectrons +
    process.makePatMuons     +
    process.makePatTaus      +
    process.patCandidateSummary
  )

  process.recoPAT = cms.Sequence(process.PFTau+process.patDefaultSequence)

  process.simpleSecondaryVertex = cms.ESProducer("SimpleSecondaryVertexESProducer",
      use3d = cms.bool(True),
      unBoost = cms.bool(False),
      useSignificance = cms.bool(True),
      minTracks = cms.uint32(2)
  )

  #remove MC matching
  removeMCMatching(process,['All'],"",False)
  #HPS
  switchToPFTauHPS(process)
  #apply particle based isolation
  switchToElePFIsolation(process,'gsfElectrons')
  switchToMuPFIsolation(process,'muons')
  runPFNoPileUp(process)
  muonOverloading(process,'patMuons')
  tauOverloading(process,'patTaus')
  electronOverloading(process,True,'patElectrons')
  jetOverloading(process)

  #add GOODCOLL criteria
  addSkimForDATA(process)

  #Apply default selections
  applyDefaultSelections(process)

  #Run FastJet 
  runRho(process)

def defaultTriggerOnlySKIM(process,triggerProcess = 'HLT',triggerPaths = ['HLT_Mu9','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v2','HLT_Mu15_v1','HLT_Mu15_v2']):
  process.load("UWAnalysis.Configuration.startUpSequence_cff")
  process.load("Configuration.StandardSequences.Geometry_cff")
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.StandardSequences.Services_cff")
  process.load("Configuration.StandardSequences.Reconstruction_cff")
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.load("DQMServices.Core.DQM_cfg")
  process.load("DQMServices.Components.DQMEnvironment_cfi")
  process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

  global TriggerPaths
  TriggerPaths= triggerPaths

  process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                        src = cms.InputTag('offlinePrimaryVertices'),
                                        cut = cms.string('ndof()>4&&position().rho()<2&&abs(z())<24'),
                                        filter = cms.bool(False)
  )

  process.createPV=cms.Path(process.primaryVertexFilter)

  process.vetoPatElectrons10 = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("convRejElectrons"),
    cut = cms.string("pt>10&&abs(eta)<2.5&&userFloat('wp95')==1&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )

  process.vetoPatElectrons20 = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("convRejElectrons"),
    cut = cms.string("pt>20&&abs(eta)<2.5&&userFloat('wp95')==1&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )

  process.vetoPatMuons10 = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsForAnalysis"),
    cut = cms.string("pt>10&&isGlobalMuon&&isTrackerMuon&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )
  process.vetoPatMuons20 = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsForAnalysis"),
    cut = cms.string("pt>20&&isGlobalMuon&&isTrackerMuon&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )


  process.addVetoLeptons=cms.Path(process.vetoPatElectrons10*process.vetoPatElectrons20*process.vetoPatMuons10*process.vetoPatMuons20)


def defaultConfCommonPatuples(process,mode="MC",triggerProcess = 'HLT',triggerPaths = ['HLT_Mu9','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v2','HLT_Mu15_v1','HLT_Mu15_v2']):
  process.load("UWAnalysis.Configuration.startUpSequence_cff")
  process.load("Configuration.StandardSequences.Geometry_cff")
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.StandardSequences.Services_cff")
  process.load("Configuration.StandardSequences.Reconstruction_cff")
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.load("DQMServices.Core.DQM_cfg")
  process.load("DQMServices.Components.DQMEnvironment_cfi")
  process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

  global TriggerPaths
  TriggerPaths= triggerPaths

  process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                        src = cms.InputTag('offlinePrimaryVertices'),
                                        cut = cms.string('ndof()>4&&position().rho()<2&&abs(z())<24'),
                                        filter = cms.bool(False)
  )

  process.createPV=cms.Path(process.primaryVertexFilter)

  process.source.inputCommands=cms.untracked.vstring(
                        'keep *', 'drop *_finalState*_*_*',
                        'drop *_patFinalStateEvent*_*_*'
  )


  process.patJetsForAnalysis = cms.EDProducer("PATJetCleaner",
    src = cms.InputTag("selectedPatJets"),

    # preselection (any string-based cut on pat::Jet)
    preselection = cms.string("pt>20&((abs(eta)<2.4&&chargedHadronEnergyFraction>0&neutralHadronEnergyFraction<0.99&chargedMultiplicity>0)||(abs(eta)>2.4&&neutralHadronEnergyFraction<0.99))&chargedEmEnergyFraction<0.99&neutralEmEnergyFraction<0.99&nConstituents>1&&userFloat('idLoose')"),

    # overlap checking configurables
    checkOverlaps = cms.PSet(
       muons = cms.PSet(
          src       = cms.InputTag("cleanPatMuons"),
          algorithm = cms.string("byDeltaR"),
          preselection        = cms.string("pt>10&&isGlobalMuon&&isTrackerMuon&&(chargedHadronIso()+max(photonIso+neutralHadronIso(),0.0))/pt()<0.3"),
          deltaR              = cms.double(0.3),
          checkRecoComponents = cms.bool(False),
          pairCut             = cms.string(""),
          requireNoOverlaps   = cms.bool(True),
       ),
       electrons = cms.PSet(
           src       = cms.InputTag("cleanPatElectrons"),
           algorithm = cms.string("byDeltaR"),
           preselection        = cms.string("pt>10&&(chargedHadronIso()+max(photonIso()+neutralHadronIso(),0.0))/pt()<0.3"),
           deltaR              = cms.double(0.3),
           checkRecoComponents = cms.bool(False),
           pairCut             = cms.string(""),
           requireNoOverlaps   = cms.bool(True),
       ),

    ),
    # finalCut (any string-based cut on pat::Jet)
    finalCut = cms.string('')
  )
  process.createCleanJets=cms.Path(process.patJetsForAnalysis)
 
  process.vetoPatElectrons10 = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("cleanPatElectrons"),
    cut = cms.string("pt>10&&abs(eta)<2.5&&userFloat('wp95V')==1&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )

  process.vetoPatElectrons20 = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("cleanPatElectrons"),
    cut = cms.string("pt>20&&abs(eta)<2.5&&userFloat('wp95V')==1&&(chargedHadronIso()+max(photonIso()+neutralHadronIso()-0.5*userIso(0),0.0))/pt()<0.2")
  )

  process.addVetoElectrons=cms.Path(process.vetoPatElectrons10*process.vetoPatElectrons20) 	 

  BTAGGING(process)
  mvaMet(process)

  if mode=="MC":
	 process.bhadrons = cms.EDProducer('MCBHadronProducer',
                                  quarkId = cms.uint32(5)
                                  )


	 process.cbarCands=cms.EDProducer(
	      "GenParticlePruner",
	      src = cms.InputTag("genParticles"),
	      select = cms.vstring(
                           "keep pdgId = -4",
	      )
	 )

	 process.cCands=cms.EDProducer(
	      "GenParticlePruner",
	      src = cms.InputTag("genParticles"),
	      select = cms.vstring(
 	                          "keep pdgId= 4 ",
	      )
	 )

	 process.genDaughters = cms.EDProducer(
	      "GenParticlePruner",
	      src = cms.InputTag("genParticles"),
	      select = cms.vstring(
                           "keep++ pdgId = {W+}",
                           "drop pdgId = {W+} & status = 2",
                           "drop pdgId = {W+} & status = 2",
                           "keep++ pdgId = {W-}",
                           "drop pdgId = {W-} & status = 2",
                           "drop pdgId = {W-} & status = 2",
                           "keep pdgId = {mu+}",
                           "keep pdgId = {nu_mu}",
                           "keep pdgId = {mu-}",
                           "keep pdgId = {e+}",
                           "keep pdgId = {e-}",
                           "keep pdgId = {nu_e}",
                           "keep pdgId = {tau+}",
                           "keep pdgId = {tau-}",
                           "keep pdgId = {nu_tau}",
                           "keep abs(pdgId) = 1",
                           "keep abs(pdgId) = 2",
                           "keep abs(pdgId) = 3",
                           "keep abs(pdgId) = 4",
                           "keep abs(pdgId) = 5",
                           "keep abs(pdgId) = 6",
                           "keep pdgId = 21",
     	  )
 	 )

         process.createSimCollections=cms.Path(process.bhadrons*process.cbarCands*process.cCands*process.genDaughters)



def BTAGGING(process):

	process.load('RecoVertex/AdaptiveVertexFinder/inclusiveVertexing_cff')
	process.load('RecoBTag/SecondaryVertex/bToCharmDecayVertexMerger_cfi')


	process.ak5JetTracksAssociatorAtVertex= cms.EDProducer("JetTracksAssociatorAtVertex",
   	  tracks       = cms.InputTag("generalTracks"),
     	  jets         = cms.InputTag("ak5PFJets"),
     	coneSize     = cms.double(0.5)
	)

	process.load('RecoBTag/ImpactParameter/impactParameter_cff')
	process.load('RecoBTag/SecondaryVertex/secondaryVertex_cff')


	process.btagging = cms.Sequence(
   	    (
     	      # impact parameters and IP-only algorithms
     	      impactParameterTagInfos *
	      ( trackCountingHighEffBJetTags +
	        trackCountingHighPurBJetTags +
	        jetProbabilityBJetTags +
                jetBProbabilityBJetTags +

	        # SV tag infos depending on IP tag infos, and SV (+IP) based algos
   	        secondaryVertexTagInfos *
     	        ( simpleSecondaryVertexHighEffBJetTags +
     	          simpleSecondaryVertexHighPurBJetTags +
	          combinedSecondaryVertexBJetTags +
	          combinedSecondaryVertexMVABJetTags
		
	        ) +
		secondaryVertexNegativeTagInfos*simpleSecondaryVertexNegativeHighEffBJetTags
		+
                ghostTrackVertexTagInfos *
                  ghostTrackBJetTags
	    )
	))

        process.patJetsForBTagging = cms.EDProducer("PATJetProducer",
	    addJetCharge = cms.bool(False),
   	    addGenJetMatch = cms.bool(False),
     	    embedPFCandidates = cms.bool(False),
     	    embedGenJetMatch = cms.bool(False),
	    addAssociatedTracks = cms.bool(False),
	    addGenPartonMatch = cms.bool(False),
	    genPartonMatch = cms.InputTag(""),
            addTagInfos = cms.bool(True),
            addPartonJetMatch = cms.bool(False),
	    embedGenPartonMatch = cms.bool(False),
   	    jetSource = cms.InputTag("ak5PFJets"),
     	    addEfficiencies = cms.bool(False),
     	    trackAssociationSource = cms.InputTag("ak5JetTracksAssociatorAtVertex"),
	    tagInfoSources = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"), cms.InputTag("secondaryVertexNegativeTagInfos"),
	# cms.InputTag("softMuonTagInfos")
	),
            discriminatorSources = cms.VInputTag(
               cms.InputTag("jetBProbabilityBJetTags"), cms.InputTag("jetProbabilityBJetTags"), cms.InputTag("trackCountingHighPurBJetTags"), cms.InputTag("trackCountingHighEffBJetTags"),
   	       cms.InputTag("simpleSecondaryVertexHighEffBJetTags"), cms.InputTag("simpleSecondaryVertexHighPurBJetTags"), cms.InputTag("combinedSecondaryVertexBJetTags"), cms.InputTag("combinedSecondaryVertexMVABJetTags"),
     	        cms.InputTag("simpleInclusiveSecondaryVertexHighEffBJetTags"), cms.InputTag("simpleInclusiveSecondaryVertexHighPurBJetTags"), cms.InputTag("doubleSecondaryVertexHighEffBJetTags"),
     	
		#cms.InputTag("softMuonBJetTags"),
	#        cms.InputTag("softMuonByPtBJetTags"), cms.InputTag("softMuonByIP3dBJetTags")
        ),
            addBTagInfo = cms.bool(True),
	    embedCaloTowers = cms.bool(False),
   	    addResolutions = cms.bool(False),
     	    getJetMCFlavour = cms.bool(False),
     	    addDiscriminators = cms.bool(True),
	    jetChargeSource = cms.InputTag("patJetCharge"),
	    addJetCorrFactors = cms.bool(False),
	    jetIDMap = cms.InputTag("ak5JetID"),
            addJetID = cms.bool(False)
        )
	
   	
     	
     	
	


	process.reDOBTAGGING = cms.Path(process.inclusiveVertexing *
	                         process.inclusiveMergedVerticesFiltered *
                                 process.bToCharmDecayVertexMerged
                                * process.ak5JetTracksAssociatorAtVertex
	                         *process.btagging
   	                         *process.inclusiveSecondaryVertexFinderTagInfosFiltered
     	                         *process.simpleInclusiveSecondaryVertexHighEffBJetTags *
     	                         process.simpleInclusiveSecondaryVertexHighPurBJetTags *
	                         process.doubleSecondaryVertexHighEffBJetTags
                    	         *process.patJetsForBTagging
	)



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
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>20&&userFloat("wp95V")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&!(userFloat("hasConversion")>0)&&userInt("missingHits")==0&&abs(userFloat("ipDXY"))<0.045&&abs(eta())<2.5'),
                                           filter = cms.bool(False)
  										)
  process.metMuons = cms.EDFilter("PATMuonSelector",
                                           src = cms.InputTag("cleanPatMuons"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>17&&userInt("tightID")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&abs(eta())<2.1&&abs(userFloat("ipDXY"))<0.045'),
                                           filter = cms.bool(False)
  										) 
  
  process.metTightMuons = cms.EDFilter("PATMuonSelector",
                                           src = cms.InputTag("cleanPatMuons"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>25&&userInt("WWID2011")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&abs(eta())<2.1&&abs(userFloat("ipDXY"))<0.02'),
                                           filter = cms.bool(False)
										)

  process.metTightElectrons = cms.EDFilter("PATElectronSelector",
                                           src = cms.InputTag("cleanPatElectrons"),
                                           cut = cms.string('abs(userFloat("dz"))<0.2&&pt>35&&userFloat("wp80")>0&&(userIso(0)+max(photonIso+neutralHadronIso()-0.5*userIso(2),0.0))/pt()<0.1&&!(userFloat("hasConversion")>0)&&userInt("missingHits")==0&&abs(eta())<2.5&&abs(userFloat("ipDXY"))<0.02'),
                                           filter = cms.bool(False)
                                                                                )


  process.pfMEtMVA.inputFileNames.U = cms.FileInPath('pharris/MVAMet/data/gbrmet_52.root')
  process.pfMEtMVA.inputFileNames.DPhi = cms.FileInPath('pharris/MVAMet/data/gbrmetphi_52.root')
  process.pfMEtMVA.inputFileNames.CovU1 = cms.FileInPath('pharris/MVAMet/data/gbrmetu1cov_52.root')
  process.pfMEtMVA.inputFileNames.CovU2 = cms.FileInPath('pharris/MVAMet/data/gbrmetu2cov_52.root')
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")

  process.mvaMetMuTau = process.pfMEtMVA.clone()
  process.mvaMetMuTau.srcLeptons = cms.VInputTag('metMuons', 'metMuTaus')  
  process.patMVAMetMuTau = process.patMETs.clone(
  	metSource = cms.InputTag('mvaMetMuTau'),
  	addMuonCorrections = cms.bool(False),
  	addGenMET = cms.bool(False)
  )
  
  process.mvaMetEleTau = process.pfMEtMVA.clone()
  process.mvaMetEleTau.srcLeptons = cms.VInputTag('metElectrons', 'metEleTaus')  
  process.patMVAMetEleTau = process.patMETs.clone(
  	metSource = cms.InputTag('mvaMetEleTau'),
  	addMuonCorrections = cms.bool(False),
  	addGenMET = cms.bool(False)
  )  
  
  process.mvaMetMu = process.pfMEtMVA.clone()                               
  process.mvaMetMu.srcLeptons = cms.VInputTag('metTightMuons')
  process.patMVAMetMu = process.patMETs.clone(
        metSource = cms.InputTag('mvaMetMu'),
        addMuonCorrections = cms.bool(False),
        addGenMET = cms.bool(False)        
  )


  process.mvaMetEle = process.pfMEtMVA.clone()
  process.mvaMetEle.srcLeptons = cms.VInputTag('metTightElectrons')
  process.patMVAMetEle = process.patMETs.clone(
        metSource = cms.InputTag('mvaMetEle'),
        addMuonCorrections = cms.bool(False),
        addGenMET = cms.bool(False)
  )


  process.mvaMetMuTauSequence = cms.Sequence(process.metMuTaus*process.metMuons*process.mvaMetMuTau*process.patMVAMetMuTau)
  process.mvaMetEleTauSequence = cms.Sequence(process.metEleTaus*process.metElectrons*process.mvaMetEleTau*process.patMVAMetEleTau)
  process.mvaMetMuSequence = cms.Sequence(process.metTightMuons*process.mvaMetMu*process.patMVAMetMu)
  process.mvaMetEleSequence = cms.Sequence(process.metTightElectrons*process.mvaMetEle*process.patMVAMetEle)
  process.MVAMET = cms.Path(process.calibratedAK5PFJetsForPFMEtMVA*process.mvaMetMuSequence*process.mvaMetEleSequence)






def defaultReconstruction(process,triggerProcess = 'HLT',triggerPaths = ['HLT_Mu9','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v1','HLT_Mu11_PFTau15_v2','HLT_Mu15_v1','HLT_Mu15_v2']):

  #Make PAT
  runRECO(process)




  global TriggerPaths
  TriggerPaths= triggerPaths

  #Use PF Jets
  switchJetCollection(process,cms.InputTag('ak5PFJets'),True,True,('AK5PF',['L1FastJet','L2Relative','L3Absolute','L2L3Residual']),False,cms.InputTag("ak5GenJets"),True,"ak5","","")

  #switchJetCollection(process,cms.InputTag('ak5PFJets'),True,True,('AK5PF',['L1FastJet','L2Relative','L3Absolute']),False,cms.InputTag("ak5GenJets"),True,"ak5","","")


  #remove MC matching
  removeMCMatching(process,['All'],"",False)

  #HPS
  switchToPFTauHPS(process)
  addTauDeltaBeta(process,'hpsPFTauProducer')
  #Add PF MET
  addPfMET(process,'')

  #apply particle based isolation
  switchToElePFIsolation(process,'gsfElectrons')
  switchToMuPFIsolation(process,'muons')
  runPFNoPileUp(process)

  #Add PAT Trigger

  switchOnTrigger( process,'patTrigger','patTriggerEvent','patDefaultSequence',triggerProcess,'') 
  muonTriggerMatch(process,triggerProcess)
  electronTriggerMatch(process,triggerProcess)
  tauTriggerMatch(process,triggerProcess)

  # Missing simpleSecondaryVertex producer
  process.simpleSecondaryVertex = cms.ESProducer("SimpleSecondaryVertexESProducer",
      use3d = cms.bool(True),
      unBoost = cms.bool(False),
      useSignificance = cms.bool(True),
      minTracks = cms.uint32(2)
  )

  #Muon Overloading
  muonOverloading(process,'triggeredPatMuons')
  tauOverloading(process,'triggeredPatTaus')
  electronOverloading(process,True,'triggeredPatElectrons')
  jetOverloading(process)

  #Add the tracks
  addTrackCandidates(process)

  #addSuperClusters
  addSuperClusterCandidates(process)

  #add GOODCOLL criteria
  addSkimForDATA(process)

  #Apply default selections
  applyDefaultSelections(process)

  #Run FastJet 
  runRho(process)

  #addCustomJEC(process)

  process.patJetCorrFactors.useRho = True

 # runPF2PAT(process)

  
def defaultReconstructionMC(process,triggerProcess = 'HLT',triggerPaths = ['HLT_Mu9'],calibrateMET = False,calibrationScheme = "BothLegs"):

  #Make PAT
  runRECO(process)

  global TriggerPaths
  TriggerPaths= triggerPaths


  ##MET recoil calibration
  global metCalibration

  #Use PF Jets
  switchJetCollection(process,cms.InputTag('ak5PFJets'),True,True,('AK5PF',['L1FastJet','L2Relative','L3Absolute']),False,cms.InputTag("ak5GenJets"),True,"ak5","","")
  process.patJetCorrFactors.useRho = True

  #make HPS Tau ID
  switchToPFTauHPS(process)
  addTauDeltaBeta(process,'hpsPFTauProducer')

  #Add PF MET
  addPfMET(process,'')

  #apply particle based isolation
  switchToElePFIsolation(process,'gsfElectrons')
  switchToMuPFIsolation(process,'muons')
  runPFNoPileUp(process)

  #Add PAT Trigger
  switchOnTrigger( process,'patTrigger','patTriggerEvent','patDefaultSequence',triggerProcess,'') 
  muonTriggerMatch(process,triggerProcess)
  electronTriggerMatch(process,triggerProcess)
  tauTriggerMatch(process,triggerProcess)

  # Missing simpleSecondaryVertex producer
  process.simpleSecondaryVertex = cms.ESProducer("SimpleSecondaryVertexESProducer",
      use3d = cms.bool(True),
      unBoost = cms.bool(False),
      useSignificance = cms.bool(True),
      minTracks = cms.uint32(2)
  )

  #Add the IP etc of the muon
  muonOverloading(process,'triggeredPatMuons')
  tauOverloading(process,'triggeredPatTaus')
  electronOverloading(process,False,'triggeredPatElectrons')
  jetOverloading(process)

  #Add the tracks
  addTrackCandidates(process)

  #addSuperClusters
  addSuperClusterCandidates(process)

  addSkimForDATA(process)

  #Run FastJet subtraction
  runRho(process)


  #addCustomJEC(process)


  process.patJetCorrFactors.useRho = True



  applyDefaultSelections(process)


def applyDefaultSelections(process):
  #DONT CHANGE THOSE HERE THEY ARE NOT USED FOR YOUR SELECTIONS!!!
  #ONLY FOR SYSTEMATICS . PLEASE CHANGE THEM in YOUR CFG FILE IF REALLY NEEDED

  #require jet selection over 15 GeV
#  process.selectedPatJets.cut = cms.string("pt>10&chargedHadronEnergyFraction>0&neutralHadronEnergyFraction<0.99&chargedEmEnergyFraction<0.99&neutralEmEnergyFraction<0.99&chargedMultiplicity>0&nConstituents>1")
  process.selectedPatJets.cut = cms.string("pt>10&((abs(eta)<2.4&&chargedHadronEnergyFraction>0&neutralHadronEnergyFraction<0.99&chargedMultiplicity>0)||(abs(eta)>2.4&&neutralHadronEnergyFraction<0.99))&chargedEmEnergyFraction<0.99&neutralEmEnergyFraction<0.99&nConstituents>1")

  #require muon selection
#  process.selectedPatMuons.cut = cms.string("pt>10&&isGlobalMuon&&(chargedHadronIso()+max(photonIso+neutralHadronIso()-userIso(0),0.0))/pt()<0.3")
  process.selectedPatMuons.cut = cms.string("pt>10&&isGlobalMuon&&(chargedHadronIso()+max(photonIso+neutralHadronIso(),0.0))/pt()<0.3")

  #require electron selection
#  process.selectedPatElectrons.cut = cms.string('pt>10&&userFloat("wp95")>0&&(chargedHadronIso()+photonIso()+neutralHadronIso())/pt()<0.3')
  process.selectedPatElectrons.cut = cms.string('pt>10&&(chargedHadronIso()+photonIso()+neutralHadronIso())/pt()<0.3')

  #require tau selection
  process.selectedPatTaus.cut = cms.string('pt>15&&tauID("decayModeFinding")&&tauID("byLooseIsolation")')




def runRECO(process):
  process.load("UWAnalysis.Configuration.startUpSequence_cff")
  process.load("Configuration.StandardSequences.Geometry_cff")
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.StandardSequences.Services_cff")
  process.load("Configuration.StandardSequences.Reconstruction_cff")
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.load("DQMServices.Core.DQM_cfg")
  process.load("DQMServices.Components.DQMEnvironment_cfi")
  process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
  process.patElectrons.addElectronID     = cms.bool( True )
  
  
  ##Add CIC Electron ID
  process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
  process.patElectronId = cms.Sequence(
	process.eidVeryLoose+
	process.eidLoose+
	process.eidMedium+
	process.eidTight+
	process.eidSuperTight+
	process.eidHyperTight1+
	process.eidHyperTight2+
	process.eidHyperTight3+
	process.eidHyperTight4
  )

  process.patElectrons.electronIDSources = cms.PSet(
	cicVeryLoose = cms.InputTag("eidVeryLoose"),
	cicLoose = cms.InputTag("eidLoose"),
	cicMedium = cms.InputTag("eidMedium"),
	cicTight = cms.InputTag("eidTight"),
	cicSuperTight = cms.InputTag("eidSuperTight"),
	cicHyperTight1 = cms.InputTag("eidHyperTight1"),
	cicHyperTight2 = cms.InputTag("eidHyperTight2"),
	cicHyperTight3 = cms.InputTag("eidHyperTight3"),
	cicHyperTight4 = cms.InputTag("eidHyperTight4")
  )

  process.recoPAT = cms.Path(process.PFTau+process.patElectronId*
        process.patDefaultSequence)


def runPF2PAT(process):
  process.load("PhysicsTools.PatAlgos.patSequences_cff")
  process.out = cms.OutputModule("PoolOutputModule",
                                 fileName = cms.untracked.string('patTuple.root'),
                                 # save only events passing the full path
                                 SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                                 # save PAT Layer 1 output; you need a '*' to
                                 # unpack the list of commands 'patEventContent'
                                 outputCommands = cms.untracked.vstring('keep *')#, *patEventContent )
                                 )
 
  postfix = "PFlow"
  usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix)
  process.pfPileUpPFlow.Enable = True
  process.pfPileUpPFlow.checkClosestZVertex = cms.bool(False)
  process.pfPileUpPFlow.Vertices = cms.InputTag('offlinePrimaryVertices')
  process.pfJetsPFlow.doAreaFastjet = True
  process.pfJetsPFlow.doRhoFastjet = False
  # Compute the mean pt per unit area (rho) from the
  # PFchs inputs
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJetsPFlow = kt4PFJets.clone(
      rParam = cms.double(0.6),
      src = cms.InputTag('pfNoElectron'+postfix),
      doAreaFastjet = cms.bool(True),
      doRhoFastjet = cms.bool(True),
      voronoiRfact = cms.double(0.9)
      )
  process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
 
 
  # Add the PV selector and KT6 producer to the sequence
  getattr(process,"patPF2PATSequence"+postfix).replace(
      getattr(process,"pfNoElectron"+postfix),
      getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow )
  
  ## let it run
  process.p = cms.Path(
      getattr(process,"patPF2PATSequence"+postfix)
      )
  
def runRho(process):
  process.load('RecoJets.JetProducers.kt4PFJets_cfi')
  process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doAreaFastjet = True,doRhoFastjet = cms.bool(True),voronoiRfact = cms.double(0.9) )
  process.kt6PFJets.Rho_EtaMax = cms.double(2.5)
  process.ak5PFJets.doAreaFastjet = True
  process.rhoSeq = cms.Sequence(process.ak5PFJets*process.kt6PFJets)
  process.patJSeq = process.patDefaultSequence
  process.patDefaultSequence = cms.Sequence(process.rhoSeq+process.patJSeq)



def addCustomJEC(process):
  process.load("CondCore.DBCommon.CondDBCommon_cfi")
  process.jec = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
      messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
            label  = cms.untracked.string('AK5PF')
            ),
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5Calo'),
            label  = cms.untracked.string('AK5Calo')
            )
      ),
      ## here you add as many jet types as you need (AK5PFchs, AK5Calo, AK5JPT, AK7PF, AK7Calo, KT4PF, KT4Calo)
      connect = cms.string('sqlite:Jec11V2.db')
  )
  # Add an es_prefer statement to get your new JEC constants from the sqlite file, rather than from the global tag
  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

def addTrigger(process,triggerProcess):
   process.patTrigger = cms.EDProducer( "PATTriggerProducer",
                                    processName    = cms.string(triggerProcess),
                                    )

   process.patDefaultSequence += process.patTrigger



def muonTriggerMatch(process,triggerProcess):

   process.triggeredPatMuons = cms.EDProducer("MuonTriggerMatcher",
                                            src = cms.InputTag("patMuons"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered12','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered15','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered24','',triggerProcess),
                                                cms.InputTag('hltDiMuonL3PreFiltered8','',triggerProcess),
                                                cms.InputTag('hltDiMuonL3PreFiltered7','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau15','',triggerProcess),
                                                cms.InputTag('hltL1Mu3EG5L3Filtered17','',triggerProcess),
                                                cms.InputTag('hltL1Mu7EG5L3MuFiltered17','',triggerProcess),
                                                cms.InputTag('hltL1MuOpenEG12L3Filtered8','',triggerProcess),
                                                cms.InputTag('hltL1MuOpenEG5L3Filtered8','',triggerProcess),
                                                cms.InputTag('hltL1MuOpenEG8L3Filtered8','',triggerProcess),
                                                cms.InputTag('hltL1Mu3EG5L3Filtered8','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu12IsoPFTau10','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL3IsoFiltered17','',triggerProcess),
                                                cms.InputTag('hltSingleMuL2QualIsoL3IsoFiltered17','',triggerProcess),
                                                cms.InputTag('hltSingleMuL2QualIsoL3IsoFiltered24','',triggerProcess),                                        
                                                cms.InputTag('hltSingleMu13L3Filtered13','',triggerProcess),
                                                cms.InputTag('hltSingleMuIsoL1s14L3IsoFiltered15eta2p1',"",triggerProcess),
                                                cms.InputTag('hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered','',triggerProcess),
                                                cms.InputTag('hltDiMuonL3p5PreFiltered8','',triggerProcess),
                                                cms.InputTag('hltSingleMu13L3Filtered17','',triggerProcess)
                                            ),
                                            pdgId = cms.int32(13)
  )

   process.patDefaultSequence=cms.Sequence(process.patDefaultSequence*process.triggeredPatMuons)





def electronTriggerMatch(process,triggerProcess):

   process.triggeredPatElectrons = cms.EDProducer("ElectronTriggerMatcher",
                                            src = cms.InputTag("patElectrons"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter','',triggerProcess),
                                                cms.InputTag('hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter','',triggerProcess),
                                                cms.InputTag('hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter','',triggerProcess),
                                                cms.InputTag('hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter','',triggerProcess),
                                                cms.InputTag('hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle18IsoPFTau20','',triggerProcess)

                                            ),
                                            pdgId = cms.int32(11)
  )


   process.patDefaultSequence=cms.Sequence(process.patDefaultSequence*process.triggeredPatElectrons)



def tauTriggerMatch(process,triggerProcess):
   strTrig=''
   for i in TriggerPaths:
    if i==TriggerPaths[0]:
      strTrig+='path(\"'+i+'\")'
    else:  
      strTrig+='|| path(\"'+i+'\")'


   #Match With The triggers
   process.triggeredPatTaus = cms.EDProducer("TauTriggerMatcher",
                                            src = cms.InputTag("patTaus"),
                                            trigEvent = cms.InputTag("hltTriggerSummaryAOD"),
                                            filters = cms.VInputTag(
                                                cms.InputTag('hltOverlapFilterIsoMu15IsoPFTau15','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoMu12IsoPFTau10','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle15IsoPFTau20','',triggerProcess),
                                                cms.InputTag('hltOverlapFilterIsoEle18IsoPFTau20','',triggerProcess)
                                            ),
                                            pdgId = cms.int32(15)
  )
                                            


   process.patDefaultSequence=cms.Sequence(process.patDefaultSequence*process.triggeredPatTaus)




def tauOverloading(process,src):


  process.patRhoTau = cms.EDProducer("TauRhoOverloader", 
                                          src = cms.InputTag(src),
                                          srcRho = cms.InputTag("kt6PFJets","rho")
  )
  process.patOverloadedTaus = cms.EDProducer('PATTauOverloader',
                                        src = cms.InputTag("patRhoTau")
  )                                        

  process.tauOverloading = cms.Sequence(process.patRhoTau+process.patOverloadedTaus)
  process.patDefaultSequence*=process.tauOverloading


def electronOverloading(process,isdata,src):

  process.electronsWP80 = cms.EDProducer('PATVBTFElectronEmbedder',
                                             src             = cms.InputTag(src),
                                             sigmaEtaEta     = cms.vdouble(0.01,0.03),
                                             deltaEta        = cms.vdouble(0.004,0.007),
                                             deltaPhi        = cms.vdouble(0.06,0.03),
                                             hoE             = cms.vdouble(0.04,0.025),
                                             id              = cms.string("wp80")

  )                                             

  process.electronsWP70 = cms.EDProducer('PATVBTFElectronEmbedder',
                                             src             = cms.InputTag("electronsWP80"),
                                             sigmaEtaEta     = cms.vdouble(0.01,0.03),
                                             deltaEta        = cms.vdouble(0.004,0.005),
                                             deltaPhi        = cms.vdouble(0.03,0.02),
                                             hoE             = cms.vdouble(0.025,0.025),
                                             id              = cms.string("wp70")

  )


  process.electronsWP90 = cms.EDProducer('PATVBTFElectronEmbedder',
                                             src             = cms.InputTag("electronsWP70"),
                                             sigmaEtaEta     = cms.vdouble(0.01,0.03),
                                             deltaEta        = cms.vdouble(0.007,0.009),
                                             deltaPhi        = cms.vdouble(0.8,0.7),
                                             hoE             = cms.vdouble(0.12,0.05),
                                             id              = cms.string("wp90")
  )   
  
  process.electronsWP95 = cms.EDProducer('PATVBTFElectronEmbedder',
                                             src             = cms.InputTag("electronsWP90"),
                                             sigmaEtaEta     = cms.vdouble(0.01,0.03),
                                             deltaEta        = cms.vdouble(0.007,0.01),
                                             deltaPhi        = cms.vdouble(0.8,0.7),
                                             hoE             = cms.vdouble(0.15,0.07),
                                             id              = cms.string("wp95")
  )                                             


  process.patRhoElectron = cms.EDProducer("ElectronRhoOverloader", 
                                          src = cms.InputTag("electronsWP95"),
                                          srcRho = cms.InputTag("kt6PFJets","rho")
  )


  process.convRejElectrons = cms.EDProducer('PATWWElectronEmbedder',
                                             src             = cms.InputTag("patRhoElectron"),
                                             srcVertices     = cms.InputTag("primaryVertexFilter"),
                                             sigmaEtaEta     = cms.vdouble(0.01,0.03,0.01,0.03),
                                             deltaEta        = cms.vdouble(0.004,0.005,0.004,0.007),
                                             deltaPhi        = cms.vdouble(0.03,0.02,0.06,0.03),
                                             hoE             = cms.vdouble(0.025,0.025,0.04,0.025),
                                             convDist        = cms.vdouble(0.02,0.02,0.02,0.02),
                                             convDCot        = cms.vdouble(0.02,0.02,0.02,0.02),
                                             id              = cms.string("WWID"),
                                             fbrem           = cms.double(0.15),
                                             EOP             = cms.double(0.95),
                                             d0              = cms.double(0.045),
                                             dz              = cms.double(0.2),
                                             )        

  process.preSelElectrons = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("convRejElectrons"),
    cut = cms.string("pt>20&&abs(eta)<2.5&&userFloat('wp95')==1&&(isEB||isEE)&&(dr03TkSumPt/p4.Pt <0.2)") 
  )


  process.electronOverloading=cms.Sequence(process.electronsWP80+process.electronsWP70+process.electronsWP90+process.electronsWP95+process.patRhoElectron+process.convRejElectrons+process.preSelElectrons)
  process.patDefaultSequence*=process.electronOverloading


def muonOverloading(process,src):
  #create the impact parameter

  process.patPFMuonMatch = cms.EDProducer("PATPFMuonEmbedder", #Saves the case where muon is matched to a PF Muon
                                          src = cms.InputTag(src),
                                          ref = cms.InputTag("pfAllMuons")
   )

  process.patVBTFMuonMatch = cms.EDProducer("PATVBTFMuonEmbedder", #Saves the case where muon is matched to a PF Muon
                                          src = cms.InputTag("patPFMuonMatch"),
                                          maxDxDy=cms.double(0.2),
                                          maxChi2=cms.double(10.),
                                          minTrackerHits=cms.int32(10),
                                          minPixelHits=cms.int32(1),
                                          minMuonHits = cms.int32(1),
                                          minMatches  = cms.int32(2),
                                          maxResol      = cms.double(0.1)
 
   )

  process.patWWMuonMatch = cms.EDProducer("PATWWMuonEmbedder", #Saves the case where muon is matched to a PF Muon
                                          src = cms.InputTag("patVBTFMuonMatch"),
                                          srcVertices = cms.InputTag("primaryVertexFilter"),
                                          #maxDxDy=cms.double(0.045),####REMOVED IP CUT
                                          maxDxDy=cms.double(0.02),
                                          maxChi2=cms.double(10.),
                                          minTrackerHits=cms.int32(10),
                                          minPixelHits=cms.int32(1),
                                          minMuonHits = cms.int32(1),
                                          minMatches  = cms.int32(2),
                                          maxResol      = cms.double(0.1),
	                                  #        dz            = cms.double(0.1)
                                          dz            = cms.double(10.)

  )

  process.patMuonsForAnalysis = cms.EDProducer("MuonRhoOverloader", 
                                          src = cms.InputTag("patWWMuonMatch"),
                                          srcRho = cms.InputTag("kt6PFJets","rho")
  )
 
  process.preSelMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsForAnalysis"),
    cut = cms.string("pt>10&&isGlobalMuon&&isTrackerMuon")
  )
 
  process.patDefaultSequence = cms.Sequence(process.patDefaultSequence*process.patPFMuonMatch*process.patVBTFMuonMatch*process.patWWMuonMatch*process.patMuonsForAnalysis*process.preSelMuons)

def jetOverloading(process,src = "selectedPatJets"):

  process.cleanPatJets = cms.EDProducer("PATJetCleaner",
    src = cms.InputTag(src),
#"patJetsForAnalysis"),

    # preselection (any string-based cut on pat::Jet)
    preselection = cms.string('pt>20&((abs(eta)<2.4&&chargedHadronEnergyFraction>0&neutralHadronEnergyFraction<0.99&chargedMultiplicity>0)||(abs(eta)>2.4&&neutralHadronEnergyFraction<0.99))&chargedEmEnergyFraction<0.99&neutralEmEnergyFraction<0.99&nConstituents>1'),

    # overlap checking configurables
    checkOverlaps = cms.PSet(
       muons = cms.PSet(
          src       = cms.InputTag("selectedPatMuons"),
          algorithm = cms.string("byDeltaR"),
          preselection        = cms.string("pt>10&&isGlobalMuon&&isTrackerMuon&&(chargedHadronIso()+max(photonIso+neutralHadronIso(),0.0))/pt()<0.3"),
          deltaR              = cms.double(0.3),
          checkRecoComponents = cms.bool(False), 
          pairCut             = cms.string(""),
          requireNoOverlaps   = cms.bool(True),
       ),
       electrons = cms.PSet(
           src       = cms.InputTag("selectedPatElectrons"),
           algorithm = cms.string("byDeltaR"),
           preselection        = cms.string("pt>10&&(chargedHadronIso()+max(photonIso()+neutralHadronIso(),0.0))/pt()<0.3"),
           deltaR              = cms.double(0.3),
           checkRecoComponents = cms.bool(False), 
           pairCut             = cms.string(""),
           requireNoOverlaps   = cms.bool(True), 
       )
    ),
    # finalCut (any string-based cut on pat::Jet)
    finalCut = cms.string('')
  )

  process.patRhoJets = cms.EDProducer("JetRhoOverloader",
                                          src = cms.InputTag("cleanPatJets"),
                                          srcRho = cms.InputTag("kt6PFJets","rho")
  )

  process.patSSVEmbeded = cms.EDProducer('PATSSVJetEmbedder',
                                        src = cms.InputTag("patRhoJets"),
  )


  process.patLeptonInJetEmbeded = cms.EDProducer('PATMuonInJetEmbedder',
                                        src = cms.InputTag("patSSVEmbeded"),
                                              srcVertex     = cms.InputTag("primaryVertexFilter"),
 )


  process.patJetsForAnalysis = cms.EDProducer('PATJetOverloader',
                                        src = cms.InputTag("patLeptonInJetEmbeded"),
  )

  process.jetOverloading = cms.Sequence(process.cleanPatJets*process.patRhoJets*process.patSSVEmbeded*process.patLeptonInJetEmbeded*process.patJetsForAnalysis)#*process.cleanPatJets)
  process.patDefaultSequence*=process.jetOverloading


   

def runPFNoPileUp(process):
  process.load("CommonTools.ParticleFlow.ParticleSelectors.pfCandsForIsolation_cff")
  process.pfPileUpCandidates = cms.EDProducer(
    "TPPFCandidatesOnPFCandidates",
    enable =  cms.bool( True ),
    verbose = cms.untracked.bool( False ),
    name = cms.untracked.string("pileUpCandidates"),
    topCollection = cms.InputTag("pfNoPileUp"),
    bottomCollection = cms.InputTag("particleFlow"),
    )



  #enable PF no Pile Up
  process.pfPileUp.Enable = cms.bool(True)
  process.pfPileUp.checkClosestZVertex = cms.bool(False)
  process.pfPileUp.Vertices = cms.InputTag('offlinePrimaryVertices')

  #Put all charged particles in charged hadron collection(electrons and muons)
  process.pfAllChargedHadrons.pdgId = cms.vint32(211,-211,321,-321,999211,2212,-2212,11,-11,13,-13)

  process.pileUpHadrons = cms.EDFilter("PdgIdPFCandidateSelector",
                                         src = cms.InputTag("pfPileUpCandidates"),
                                         pdgId = cms.vint32(211,-211,321,-321,999211,2212,-2212,11,-11,13,-13)
                                     )



  process.pfAllElectrons.src = cms.InputTag("pfNoPileUp")

  process.pfAllMuons = cms.EDFilter("PdgIdPFCandidateSelector",
                                         src = cms.InputTag("pfNoPileUp"),
                                         pdgId = cms.vint32(13,-13)
                                     )

  process.pfPostSequence = cms.Sequence(
    process.pfCandsForIsolationSequence+
    process.pfAllMuons+
    process.pfPileUpCandidates+
    process.pileUpHadrons
  )      

  process.patPreIsoSeq = process.pfPostSequence
  process.patDefaultSequence = cms.Sequence(process.patPreIsoSeq*process.patDefaultSequence)


def switchToMuPFIsolation(process,muons):

  ###Muon Isolation

  process.muPFIsoDepositAll     = isoDepositReplace(muons,cms.InputTag("pfNoPileUp"))
  process.muPFIsoDepositCharged = isoDepositReplace(muons,"pfAllChargedHadrons")
  process.muPFIsoDepositNeutral = isoDepositReplace(muons,"pfAllNeutralHadrons")
  process.muPFIsoDepositGamma = isoDepositReplace(muons,"pfAllPhotons")


  #Isodeposit from PileUp- For Vertex subtraction!!!!
  process.muPFIsoDepositPU = isoDepositReplace(muons,cms.InputTag("pileUpHadrons"))
  

  process.muPFIsoDeposits = cms.Sequence(
      process.muPFIsoDepositAll*
      process.muPFIsoDepositCharged*
      process.muPFIsoDepositPU*
      process.muPFIsoDepositNeutral*
      process.muPFIsoDepositGamma
  )

  
  #And Values
  process.muPFIsoValueAll = cms.EDProducer("CandIsolatorFromDeposits",
       deposits = cms.VPSet(
          cms.PSet(
          src = cms.InputTag("muPFIsoDepositAll"),
          deltaR = cms.double(0.4),
          weight = cms.string('1'),
          vetos = cms.vstring('0.001','Threshold(0.5)'),
          skipDefaultVeto = cms.bool(True),
          mode = cms.string('sum')
        )
      )
  )

  process.muPFIsoValueCharged = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("muPFIsoDepositCharged"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.0001','Threshold(0.0)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
 )

  process.muPFIsoValueNeutral = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
            src = cms.InputTag("muPFIsoDepositNeutral"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
  )

  process.muPFIsoValueGamma = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("muPFIsoDepositGamma"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.01','Threshold(0.5)'),
              skipDefaultVeto = cms.bool(True),
              mode = cms.string('sum')
         ) 
     )
  )

  process.muPFIsoValuePU = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("muPFIsoDepositPU"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.0001','Threshold(0.5)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
 )

  process.muPFIsoValuePULow = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("muPFIsoDepositPU"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.0001','Threshold(0.0)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
 )



  process.muPFIsoValues =  cms.Sequence( process.muPFIsoValueAll
                               * process.muPFIsoValueCharged
                               * process.muPFIsoValueNeutral
                               * process.muPFIsoValueGamma
                               * process.muPFIsoValuePU
                               * process.muPFIsoValuePULow
  )

  process.muisolationPrePat = cms.Sequence(
      process.muPFIsoDeposits*
      process.muPFIsoValues
  )



  process.patMuons.isoDeposits = cms.PSet(
        particle         = cms.InputTag("muPFIsoDepositAll"),
        pfChargedHadrons = cms.InputTag("muPFIsoDepositCharged"),
        pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutral"),
        pfPhotons        = cms.InputTag("muPFIsoDepositGamma")
  )

  process.patMuons.isolationValues = cms.PSet(
            particle         = cms.InputTag("muPFIsoValueAll"),
            pfChargedHadrons = cms.InputTag("muPFIsoValueCharged"),
            pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral"),
            pfPhotons        = cms.InputTag("muPFIsoValueGamma"),
            user = cms.VInputTag(
                         cms.InputTag("muPFIsoValuePU"),
                         cms.InputTag("muPFIsoValuePULow")
           )

  )

  process.patSeq = process.patDefaultSequence
  process.patDefaultSequence = cms.Sequence(process.muisolationPrePat*process.patSeq)






def addTauDeltaBeta(process,taus):

  ###Muon Isolation

  #Isodeposit from PileUp- For Vertex subtraction!!!!
  process.tauPFIsoDepositPU = isoDepositReplace(taus,cms.InputTag("pileUpHadrons"))
  
  process.tauPFIsoDeposits = cms.Sequence(
      process.tauPFIsoDepositPU
  )

  process.tauPFIsoValuePU = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("tauPFIsoDepositPU"),
             deltaR = cms.double(0.5),
             weight = cms.string('1'),
             vetos = cms.vstring('0.0001','Threshold(0.5)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
 )

  process.tauPFIsoValues =  cms.Sequence(
    process.tauPFIsoValuePU
  )

  process.tauisolationPrePat = cms.Sequence(
      process.tauPFIsoDeposits*
      process.tauPFIsoValues
  )




  process.patTaus.isolationValues = cms.PSet(
             particle         = cms.InputTag("tauPFIsoValuePU")
#            pfChargedHadrons = cms.InputTag("muPFIsoValueCharged"),
#            pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral"),
#            pfPhotons        = cms.InputTag("muPFIsoValueGamma"),
           )

  process.patSeqq = process.patDefaultSequence
  process.patDefaultSequence = cms.Sequence(process.tauisolationPrePat*process.patSeqq)


def switchToElePFIsolation(process,electrons):


  ###Electron Isolation

  process.elePFIsoDepositAll     = isoDepositReplace(electrons,cms.InputTag("pfNoPileUp"))
  process.elePFIsoDepositCharged = isoDepositReplace(electrons,"pfAllChargedHadrons")
  process.elePFIsoDepositNeutral = isoDepositReplace(electrons,"pfAllNeutralHadrons")
  process.elePFIsoDepositGamma = isoDepositReplace(electrons,"pfAllPhotons")
  process.elePFIsoDepositPU = isoDepositReplace(electrons,cms.InputTag("pileUpHadrons"))


  process.elePFIsoDeposits = cms.Sequence(
      process.elePFIsoDepositAll*
      process.elePFIsoDepositCharged*
      process.elePFIsoDepositNeutral*
      process.elePFIsoDepositGamma*
      process.elePFIsoDepositPU
  )

  
  #And Values
  process.elePFIsoValueAll = cms.EDProducer("CandIsolatorFromDeposits",
       deposits = cms.VPSet(
          cms.PSet(
          src = cms.InputTag("elePFIsoDepositAll"),
          deltaR = cms.double(0.4),
          weight = cms.string('1'),
          vetos = cms.vstring('0.03','Threshold(1.0)'),
          skipDefaultVeto = cms.bool(True),
          mode = cms.string('sum')
        )
      )
  )

  
  process.elePFIsoValueCharged = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("elePFIsoDepositCharged"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.03','Threshold(0.0)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
 )


  process.elePFIsoValueNeutral = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
            src = cms.InputTag("elePFIsoDepositNeutral"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.08','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
  )

  process.elePFIsoValueGamma = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("elePFIsoDepositGamma"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.05','Threshold(0.5)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
  )

  process.elePFIsoValuePU = cms.EDProducer("CandIsolatorFromDeposits",
     deposits = cms.VPSet(
         cms.PSet(
             src = cms.InputTag("elePFIsoDepositPU"),
             deltaR = cms.double(0.4),
             weight = cms.string('1'),
             vetos = cms.vstring('0.0','Threshold(0.5)'),
             skipDefaultVeto = cms.bool(True),
             mode = cms.string('sum')
         ) 
     )
  )




  process.elePFIsoValues =  cms.Sequence( process.elePFIsoValueAll
                               * process.elePFIsoValueCharged
                               * process.elePFIsoValueNeutral
                               * process.elePFIsoValueGamma
                               * process.elePFIsoValuePU
  )

  process.eleisolationPrePat = cms.Sequence(
      process.elePFIsoDeposits*
      process.elePFIsoValues
  )


#
#KLUDGE : Since PAT electron does not support custom iso deposits 
#put the pileup in the place of all candidates
#

  process.patElectrons.isoDeposits = cms.PSet(
        pfAllParticles   = cms.InputTag("elePFIsoDepositAll"),
        pfChargedHadrons = cms.InputTag("elePFIsoDepositCharged"),
        pfNeutralHadrons = cms.InputTag("elePFIsoDepositNeutral"),
        pfPhotons        = cms.InputTag("elePFIsoDepositGamma")
  )


###KLUDGE -> Add DB in UserIso
  process.patElectrons.isolationValues = cms.PSet(
            pfAllParticles   = cms.InputTag("elePFIsoValuePU"),
            pfChargedHadrons = cms.InputTag("elePFIsoValueCharged"),
            pfNeutralHadrons = cms.InputTag("elePFIsoValueNeutral"),
            pfPhotons        = cms.InputTag("elePFIsoValueGamma")
  )

  process.patEleIsoSeq = process.patDefaultSequence
  process.patDefaultSequence = cms.Sequence(process.eleisolationPrePat*process.patEleIsoSeq)




   

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
   

def addTrackCandidates(process):
  process.trackCandidates = cms.EDProducer("TrackViewCandidateProducer",
                                   src = cms.InputTag("generalTracks"),
                                   particleType = cms.string('mu+'),
                                   cut = cms.string('pt > 8')
  )

  process.gsfTrackCandidates = cms.EDProducer("GSFTrackCandidateProducer",
                                   src = cms.InputTag("electronGsfTracks"),
                                   threshold = cms.double(8.0)
  )



  process.patDefaultSequence*=process.trackCandidates
  process.patDefaultSequence*=process.gsfTrackCandidates




def addSuperClusterCandidates(process):


 process.superClusters = cms.EDProducer("SuperClusterMerger",
                                        src = cms.VInputTag(cms.InputTag("correctedHybridSuperClusters"), cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"))
                                       )

 process.superClusterCands = cms.EDProducer("ConcreteEcalCandidateProducer",
                                              src = cms.InputTag("superClusters"),
                                              particleType = cms.int32(11),
                                           )



 process.patDefaultSequence*=process.superClusters
 process.patDefaultSequence*=process.superClusterCands




def createGeneratedParticles(process,name,commands):


  refObjects = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
#    "drop  *  " 
    )
   )
  refObjects.select.extend(commands)
  setattr(process,name,refObjects)
  process.patDefaultSequence*= getattr(process,name)


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

  process.patDefaultSequence*= process.generatedTaus
  process.patDefaultSequence*= process.generatedTausInAcceptance



def addSkimForDATA(process):
  #good vertex
  process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                        src = cms.InputTag('offlinePrimaryVertices'),
                                        cut = cms.string('ndof()>4&&position().rho()<2&&abs(z())<24'),
                                        filter = cms.bool(False)
  )                                             
  process.preVertexSequence=process.patDefaultSequence
  process.patDefaultSequence=cms.Sequence(process.primaryVertexFilter*process.preVertexSequence)

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


def addMuonAnalyzer(process,src,ref,paths):
  process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )

  muonAnalysis = cms.EDAnalyzer('MuonEffAnalyzer',
                                        src = cms.InputTag(src),
                                        ref = cms.InputTag(ref),
                                        primaryVertices = cms.InputTag("primaryVertexFilter"),
                                        triggerPaths = cms.vstring(paths)
                                )
  setattr(process,'muonAnalysis'+src,muonAnalysis)
  p = cms.EndPath(getattr(process,'muonAnalysis'+src))
  setattr(process,'muonAnalysisPath'+src,p)



def addTauIDPlotter(process,name,src,ref,discriminators,ptNum = 0.0,ptDenom = 0.0):
  process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )

  muonAnalysis = cms.EDAnalyzer('TauIDPlotter',
                                src=cms.InputTag(src),
                                srcVertices=cms.InputTag("primaryVertexFilter"),
                                ref=cms.InputTag(ref),
                                id= cms.vstring(discriminators),
                                thresholdNum = cms.untracked.double(ptNum),
                                threshold = cms.untracked.double(ptDenom)
                                )                                
  
  setattr(process,'tauFakeRate'+name,muonAnalysis)
  p = cms.EndPath(getattr(process,'tauFakeRate'+name))
  setattr(process,'tauFakeRatePath'+name,p)


def addTagAndProbePlotter(process,type,name,src,ref,selections,methods,triggers,triggersProbe):

  process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )
  muonAnalysis = cms.EDAnalyzer(type,
                                src=cms.InputTag(src),
                                vertices=cms.InputTag("primaryVertexFilter"),
                                ref=cms.InputTag(ref),
                                patTrigger = cms.InputTag("patTrigger"),
                                id= cms.vstring(selections),
                                methods= cms.vstring(methods),
                                triggers = cms.vstring(triggers),
                                triggersProbe = cms.vstring(triggersProbe),
  )                                
  
  setattr(process,'tagAndProbe'+name,muonAnalysis)
  p = cms.EndPath(getattr(process,'tagAndProbe'+name))
  setattr(process,'tagAndProbePath'+name,p)


def addRunRangePlotter(process,triggers):
  process.runRangeSummary = cms.EDAnalyzer('RunRangeAnalyzer',
                           patTrigger = cms.InputTag("patTrigger"),
                           triggerRanges = cms.vstring(triggers)
  )                                
  process.p = cms.EndPath(process.runRangeSummary)


def createSystematics(process,sequence,postfix,muScale,eScale,tauScale,jetScale,unclusteredScale,metKind):

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
	  mod.smearMCParticle = cms.bool(False)
      if mod.label().find('smearedElectrons') !=-1 :
          mod.energyScale = cms.double(eScale)
      if mod.label().find('smearedJets') !=-1 :
 	  mod.energyScaleDB = cms.int32(jetScale)
      if mod.label().find('smearedMET') !=-1 :
          mod.unclusteredScale= cms.double(unclusteredScale)
          mod.metCorrection=cms.string(metKind)
#  print p
  return cms.Sequence(p) 





