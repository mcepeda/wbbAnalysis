import FWCore.ParameterSet.Config as cms

#Import tool that creates the cut sequence
from UWAnalysis.Configuration.tools.CutSequenceProducer import *
from UWAnalysis.Configuration.tools.analysisTools import *


############### Define Cuts #######################################
DiMuonPreSel='leg1.isGlobalMuon && leg1.isTrackerMuon && leg2.isGlobalMuon && leg2.isTrackerMuon && leg1.pt()>20 && leg2.pt()>20'
DiMuonPreSel2='(leg1.isolationR03().sumPt+leg1.isolationR03().emEt+leg1.isolationR03().hadEt)/leg1.pt()<0.3 '
DiMuonPreSel3='(leg2.isolationR03().sumPt+leg2.isolationR03().emEt+leg2.isolationR03().hadEt)/leg2.pt()<0.3 '

munuKIN='lepton.pt()>25 && abs(lepton.eta())<2.1&&!lepton.pfCandidateRef().isNull()'
munuSel='lepton.userInt("WWID2011")==1&&abs(lepton.userFloat("ipDXY"))<0.02'#(lepton.isolationR03().sumPt+lepton.isolationR03().emEt+lepton.isolationR03().hadEt)/lepton.pt()<0.3&&met.pt()>30&mt>40&&lepton.userFloat("isWWMuon")==1'

elenuKIN='lepton.pt()>30 && abs(lepton.eta())<2.5&&(abs(lepton.eta)<1.4442||abs(lepton.eta)>1.5666)'
elenuSel='(lepton.isEB &&(lepton.sigmaIetaIeta<0.01)&&(-0.8<lepton.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<lepton.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (lepton.isEE &&(lepton.sigmaIetaIeta<0.03)&&(-0.7<lepton.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<lepton.deltaEtaSuperClusterTrackAtVtx<0.01) )'

DiElePreSel='leg1.pt>20&&leg2.pt>20'
DiElePreSel2='(leg1.isEB &&(leg1.sigmaIetaIeta<0.01)&&(-0.8<leg1.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<leg1.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (leg1.isEE &&(leg1.sigmaIetaIeta<0.03)&&(-0.7<leg1.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<leg1.deltaEtaSuperClusterTrackAtVtx<0.01) )'
DiElePreSel2='(leg2.isEB &&(leg2.sigmaIetaIeta<0.01)&&(-0.8<leg2.deltaPhiSuperClusterTrackAtVtx<0.8 ) && ( -0.007<leg2.deltaEtaSuperClusterTrackAtVtx<0.007) ) || (leg2.isEE &&(leg2.sigmaIetaIeta<0.03)&&(-0.7<leg2.deltaPhiSuperClusterTrackAtVtx<0.7 ) && ( -0.01<leg2.deltaEtaSuperClusterTrackAtVtx<0.01) )'
DiElePreSel3='(leg1.chargedHadronIso+leg1.photonIso()+leg1.neutralHadronIso())/leg1.pt()<0.3&&(leg2.chargedHadronIso+leg2.photonIso()+leg2.neutralHadronIso())/leg2.pt()<0.3'


######################__________________________________MNJMVAMET_____________________________________##############################

MNJMVAMETanalysisConfigurator = CutSequenceProducer(initialCounter  = 'initialEventsMNJMVAMET',
                                  pyModuleName = __name__,
                                  pyNameSpace  = locals())

MNJMVAMETanalysisConfigurator.addSmearing('cleanPatTaus','cleanPatMuons','cleanPatElectrons','patJetsForAnalysis','patMVAMetMu','default','MNJMVAMET')


#Make Muons+MET
MNJMVAMETanalysisConfigurator.addCandidateMETModule('wCandsMVAMET','PATMuonNuPairProducer','smearedMuonsMNJMVAMET','smearedMETMNJMVAMET','smearedJetsMNJMVAMET',1,9999,'AtLeastOneWCandidate',genParticles="genDaughters")
#MNJMVAMETanalysisConfigurator.addSelector('wCandsTight','PATMuonNuPairSelector',munuTight,'tightID',1)
MNJMVAMETanalysisConfigurator.addSelector('wCandsKINMVAMET','PATMuonNuPairSelector',munuKIN,'wCandsKIN',1)
MNJMVAMETanalysisConfigurator.addSelector('wCandsSelMVAMET','PATMuonNuPairSelector',munuSel,'wCandsSel',1)

MNJMVAMETanalysisConfigurator.addDiCandidateModule('diMuonsMVAMET','PATMuPairProducer', 'smearedMuonsMNJMVAMET','cleanPatMuons','smearedMETMNJMVAMET','smearedJetsMNJMVAMET',0,9999,text = '',dR=0.5,recoMode = "")
MNJMVAMETanalysisConfigurator.addSelector('diMuonsSelMVAMET','PATMuPairSelector',DiMuonPreSel+'&&'+DiMuonPreSel2+'&&'+DiMuonPreSel3,'diMuonSel',0,999)
MNJMVAMETanalysisConfigurator.addSorter('diMuonsSortedMVAMET','PATMuPairSorter')


#create the sequence
MNJMVAMETselectionSequence =MNJMVAMETanalysisConfigurator.returnSequence()

######################__________________________________ENJMVAMET_____________________________________##############################

ENJMVAMETanalysisConfigurator = CutSequenceProducer(initialCounter  = 'initialEventsENJMVAMET',
                                  pyModuleName = __name__,                                  pyNameSpace  = locals())

ENJMVAMETanalysisConfigurator.addSmearing('cleanPatTaus','cleanPatMuons','cleanPatElectrons','patJetsForAnalysis','patMVAMetEle','default','ENJMVAMET')

#Make Di Muons to VETO DY
ENJMVAMETanalysisConfigurator.addDiCandidateModule('diElecsMVAMET','PATElePairProducer', 'smearedElectronsENJMVAMET','smearedElectronsENJMVAMET','smearedMETENJMVAMET','smearedJetsENJMVAMET',0,9999,text = '',dR=0.5,recoMode = "")
ENJMVAMETanalysisConfigurator.addSelector('diElecsSelMVAMET','PATElePairSelector',DiElePreSel+'&&'+DiElePreSel2+'&&'+DiElePreSel3,'diElecsSel',0,9999)
ENJMVAMETanalysisConfigurator.addSorter('diElecsSortedMVAMET','PATElePairSorter')

#Make Elecs+MET
ENJMVAMETanalysisConfigurator.addCandidateMETModule('wCandsEleMVAMET','PATElectronNuPairProducer','smearedElectronsENJMVAMET','smearedMETENJMVAMET','smearedJetsENJMVAMET',1,9999,'AtLeastOneWCandidateELE',genParticles="genDaughters")
ENJMVAMETanalysisConfigurator.addSelector('wCandsKINEleMVAMET','PATElectronNuPairSelector',elenuKIN,'wCandsKINEle',1)
ENJMVAMETanalysisConfigurator.addSelector('wCandsSelEleMVAMET','PATElectronNuPairSelector',elenuSel,'wCandsSelEle',1)

#create the sequence
ENJMVAMETselectionSequence =ENJMVAMETanalysisConfigurator.returnSequence()



