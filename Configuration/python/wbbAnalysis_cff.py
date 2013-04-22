import FWCore.ParameterSet.Config as cms

#Import tool that creates the cut sequence
from UWAnalysis.Configuration.tools.CutSequenceProducer import *
from UWAnalysis.Configuration.tools.analysisTools import *


############### Define Cuts #######################################
DiMuonPreSel='leg1.isGlobalMuon && leg1.isTrackerMuon && leg2.isGlobalMuon && leg2.isTrackerMuon && leg1.pt()>20 && leg2.pt()>20'
DiMuonPreSel2='(leg1.isolationR03().sumPt+leg1.isolationR03().emEt+leg1.isolationR03().hadEt)/leg1.pt()<0.3 '
DiMuonPreSel3='(leg2.isolationR03().sumPt+leg2.isolationR03().emEt+leg2.isolationR03().hadEt)/leg2.pt()<0.3 '

munuKIN=''#lepton.pt()>25 && abs(lepton.eta())<2.1&&lepton.isGlobalMuon &&lepton.isTrackerMuon'
munuSel='lepton.userFloat("isWWMuon")==1'#&&(lepton.isolationR03().sumPt+lepton.isolationR03().emEt+lepton.isolationR03().hadEt)/lepton.pt()<0.3&&lepton.userFloat("dxyVtx")<0.02&&lepton.userFloat("dxyVtx")<0.2'
#&&met.pt()>30&mt>40&&lepton.userFloat("isWWMuon")==1'

elenuKIN='lepton.pt()>35 && abs(lepton.eta())<2.5&&(abs(lepton.eta)<1.4442||abs(lepton.eta)>1.5666)'
elenuSelA='((lepton.isEB &&(lepton.sigmaIetaIeta<0.01)&&(-0.8<lepton.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<lepton.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (lepton.isEE &&(lepton.sigmaIetaIeta<0.03)&&(-0.7<lepton.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<lepton.deltaEtaSuperClusterTrackAtVtx<0.01) ) )'
elenuSelB='lepton.userFloat("dxyVtx")<0.02&&lepton.userFloat("dxyVtx")<0.2&&lepton.gsfTrack().trackerExpectedHitsInner().numberOfHits()==0&&!( abs(lepton.convDcot)<0.02&&abs(lepton.convDist)<0.02)' 
elenuSel=elenuSelA+'&&'+elenuSelB

DiElePreSel='leg1.pt>20&&leg2.pt>20'
DiElePreSel2='(leg1.isEB &&(leg1.sigmaIetaIeta<0.01)&&(-0.8<leg1.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<leg1.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (leg1.isEE &&(leg1.sigmaIetaIeta<0.03)&&(-0.7<leg1.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<leg1.deltaEtaSuperClusterTrackAtVtx<0.01) )'
DiElePreSel2='(leg2.isEB &&(leg2.sigmaIetaIeta<0.01)&&(-0.8<leg2.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<leg2.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (leg2.isEE &&(leg2.sigmaIetaIeta<0.03)&&(-0.7<leg2.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<leg2.deltaEtaSuperClusterTrackAtVtx<0.01) )'
DiElePreSel3='(leg1.chargedHadronIso+leg1.photonIso()+leg1.neutralHadronIso())/leg1.pt()<0.3&&(leg2.chargedHadronIso+leg2.photonIso()+leg2.neutralHadronIso())/leg2.pt()<0.3'


jetKIN='leg1.pt>20 && leg2.pt> 20 && abs(leg1.eta)<2.4 && abs(leg2.eta)<2.4'


######################__________________________________MNBB_____________________________________##############################

MNBBanalysisConfigurator = CutSequenceProducer(initialCounter  = 'initialEventsMNBB',
                                  pyModuleName = __name__,
                                  pyNameSpace  = locals())




#Make Di Muons to VETO DY
MNBBanalysisConfigurator.addDiCandidateModule('diMuons','PATMuPairProducer', 'patMuonsForAnalysis','patMuonsForAnalysis','patMETs','patJetsForAnalysis',0,9999,text = '',dR=0.5,recoMode = "",genParticles = "genDaughters")
MNBBanalysisConfigurator.addSelector('diMuonsSel','PATMuPairSelector',DiMuonPreSel+'&&'+DiMuonPreSel2+'&&'+DiMuonPreSel3,'diMuonSel',0,999)
MNBBanalysisConfigurator.addSorter('diMuonsSorted','PATMuPairSorter')

#Make Muons+MET
MNBBanalysisConfigurator.addCandidateMETModule('wCands','PATMuonNuPairProducer','patMuonsForAnalysis','patMETs','patJetsForAnalysis',1,9999,'AtLeastOneWCandidate',genParticles="genDaughters")
#MNBBanalysisConfigurator.addSelector('wCandsTight','PATMuonNuPairSelector',munuTight,'tightID',1)
MNBBanalysisConfigurator.addSelector('wCandsKIN','PATMuonNuPairSelector',munuKIN,'wCandsKIN',1)
MNBBanalysisConfigurator.addSelector('wCandsSel','PATMuonNuPairSelector',munuSel,'wCandsSel',1)

#Make Di Jets
MNBBanalysisConfigurator.addDiCandidateModule('diJets','PATJetPairProducer', 'patJetsForAnalysis','patJetsForAnalysis','patMETs','patJetsForAnalysis',1,9999,leadingObjectsOnly = True,dR = 0.5,recoMode = "",genParticles = "genDaughters")
MNBBanalysisConfigurator.addSelector('diJetsKIN','PATJetPairSelector',jetKIN,'JetEtKIN',1,999)
MNBBanalysisConfigurator.addSelector('diJetsSel','PATJetPairSelector','','JetEtSel',1,999)
MNBBanalysisConfigurator.addSorter('diJetsSorted','PATJetPairSorter')




#create the sequence
MNBBselectionSequence =MNBBanalysisConfigurator.returnSequence()

######################__________________________________ENBB_____________________________________##############################

ENBBanalysisConfigurator = CutSequenceProducer(initialCounter  = 'initialEventsENBB',
                                  pyModuleName = __name__,                                  pyNameSpace  = locals())

#Make Di Muons to VETO DY
ENBBanalysisConfigurator.addDiCandidateModule('diElecs','PATElePairProducer', 'convRejElectrons','convRejElectrons','patMETs','patJetsForAnalysis',0,9999,text = '',dR=0.5,recoMode = "")
ENBBanalysisConfigurator.addSelector('diElecsSel','PATElePairSelector',DiElePreSel+'&&'+DiElePreSel2+'&&'+DiElePreSel3,'diElecsSel',0,9999)
ENBBanalysisConfigurator.addSorter('diElecsSorted','PATElePairSorter')

#Make Elecs+MET
ENBBanalysisConfigurator.addCandidateMETModule('wCandsEle','PATElectronNuPairProducer','convRejElectrons','patMETs','patJetsForAnalysis',1,9999,'AtLeastOneWCandidateELE',genParticles="genDaughters")
ENBBanalysisConfigurator.addSelector('wCandsKINEle','PATElectronNuPairSelector',elenuKIN,'wCandsKINEle',1)
ENBBanalysisConfigurator.addSelector('wCandsSelEle','PATElectronNuPairSelector',elenuSel,'wCandsSelEle',1)

#Make Di Jets
ENBBanalysisConfigurator.addDiCandidateModule('diJetsEle','PATJetPairProducer', 'patJetsForAnalysis','patJetsForAnalysis','patMETs','patJetsForAnalysis',1,9999,leadingObjectsOnly = True,dR = 0.5,recoMode = "")
ENBBanalysisConfigurator.addSorter('diJetsSortedEle','PATJetPairSorter')
ENBBanalysisConfigurator.addSelector('diJetsEtEle','PATJetPairSelector',jetKIN,'JetEtKIN',1,999)
ENBBanalysisConfigurator.addSelector('diJetsSelEle','PATJetPairSelector','','JetEtSel',1,999)

#create the sequence
ENBBselectionSequence =ENBBanalysisConfigurator.returnSequence()



