import FWCore.ParameterSet.Config as cms

from UWAnalysis.Configuration.tools.analysisTools import TriggerPaths


def addMuNuJetJetEventTree(process,name,src = 'wCandsSel', srcLL = 'diMuonsSorted',srcJets='patJetsForAnalysis',srcMuons='cleanPatMuons',srcDiJets='diJetsSel', srcElectrons='vetoPatElectrons10', srcWENUTIGHT='wCandsSelEleVeto', srcQuad='fourObject'):
   process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )
   eventTree = cms.EDAnalyzer('EventTreeMaker',
                              coreCollections = cms.VInputTag(
                                   cms.InputTag(src)
                              ),

			      # Number of Primary Vertices in the Event	
                              PVs = cms.PSet(
                                    pluginType = cms.string("VertexSizeFiller"),
                                    src        = cms.InputTag("primaryVertexFilter"),
                                    tag        = cms.string("vertices")
                              ),

			      # Pile-Up Information (for PU reweighting)	
                              pu = cms.PSet(
                                  pluginType = cms.string("PUFiller"),
                                  src        = cms.InputTag("addPileupInfo"),
                                  tag        = cms.string("pu"),
                                  ),


			     # Trigger Information (Saves booleans of each path to see if the event was fired or not)
                              trigger = cms.PSet(
                                  pluginType = cms.string("TriggerFiller"),
                                  src        = cms.InputTag("patTrigger"),
                                  paths      = cms.vstring(TriggerPaths)
                                  ),


			      # Muon Variables 	
                              ptMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("ptMu"),
                                  method     = cms.string("lepton.pt()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              etaMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("etaMu"),
                                  method     = cms.string("lepton.eta()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              phiMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("phiMu"),
                                  method     = cms.string("lepton.phi()"),
                                  leadingOnly=cms.untracked.bool(True)
                             ),
                              charge = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("charge"),
                                  method     = cms.string("lepton.charge()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
	

				muonID=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("muonID2011"),
				method=cms.string("lepton.userInt('WWID2011')"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                muonID2=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("muonID"),
                                method=cms.string("lepton.userInt('WWID')"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                muonID3=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("muonIDTight"),
                                method=cms.string("lepton.userInt('tightID')"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),


                                muonIPDXY=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("muonIPDXY"),
                                method=cms.string("lepton.userFloat('ipDXY') "),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

#                                muonID2=cms.PSet(
#                                  pluginType = cms.string("PATMuonNuPairFiller"),
#                                  src        = cms.InputTag(src),
#                                  tag        = cms.string("muonID2011old"),
#                                method=cms.string("lepton.userFloat('isWWMuon')"),
#                                  leadingOnly=cms.untracked.bool(True)
#                              ),
#                                muonIPDXY2=cms.PSet(
#                                  pluginType = cms.string("PATMuonNuPairFiller"),
#                                  src        = cms.InputTag(src),
#                                  tag        = cms.string("muonIPDXYold"),
#                                method=cms.string("lepton.userFloat('dxyVtx') "),
#                                  leadingOnly=cms.untracked.bool(True)
#                              ),
                                muonIPDZ=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("muonIPDZ"),
                                method=cms.string("lepton.userFloat('ipDZ') "),
                                  leadingOnly=cms.untracked.bool(True)
                              ),



			      # Muon Isolation. 3 kinds, corrected by PU, the default for analysis will be PFIsoRho (as starting point)	
                              isoMuPFIso = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIso"),
                                  method     = cms.string('(lepton.chargedHadronIso+lepton.photonIso()+lepton.neutralHadronIso())/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMu"),
                                  method     = cms.string('(lepton.isolationR03().sumPt+lepton.isolationR03().emEt+lepton.isolationR03().hadEt)/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMuRho = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuRho"),
                                  method     = cms.string('(lepton.isolationR03().sumPt+lepton.isolationR03().emEt+lepton.isolationR03().hadEt - 3.1416*0.3*0.3*lepton.userFloat("rho"))/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMuPFIsoDB = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIsoDB"),
                                  method     = cms.string('(lepton.chargedHadronIso+max(lepton.photonIso()+lepton.neutralHadronIso()-0.5*lepton.userIso(0),0.0))/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMuPFIsoDBISO = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIsoDBISO"),
                                  method     = cms.string('(lepton.userIso(0)+max(lepton.photonIso()+lepton.neutralHadronIso()-0.5*lepton.puChargedHadronIso,0.0))/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMuPFIsoRho = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIsoRho"),
                                  method     = cms.string('(lepton.chargedHadronIso+max(lepton.photonIso()+lepton.neutralHadronIso()-lepton.userFloat("rho")*3.14*0.4*0.4,0.0))/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

			     # MET or Neutrino information	
                             met = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("met"),
                                    method     = cms.string("calibratedMET().pt()"),  # "calibrated"  just means that we corrected the MET to account for detector effects ("RECOIL" corrections)
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              metphi = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metphi"),
                                    method     = cms.string("calibratedMET.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              unCorMet = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMet"),
                                    method     = cms.string("met.pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),



			     # W Boson		
                              pt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("pt"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              mt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("mt"),   # transverse mass of the muon-neutrino system. "Cor" since the met is corrected
                                    method     = cms.string("corMt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              unCorMt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMt"),
                                    method     = cms.string("mt"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              muNuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("muNuCands"), # Number of W Candidates in the event
                              ),


                              muMuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(srcLL),
                                    tag        = cms.string("muMuCands"), # Number of W Candidates in the event
                              ),

                              mass = cms.PSet(
                                    pluginType = cms.string("PATMuPairFiller"),
                                    src        = cms.InputTag(srcLL),
                                    tag        = cms.string("massMuMu"),   # transverse mass of the muon-neutrino system. "Cor" since the met is corrected
                                    method     = cms.string("mass()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),





                             # Number of Jets in the event
			    
                              nJetsSize = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets30"),
                                  method     = cms.string('pt()>30 && abs(eta())<2.4'),
                              ),
                              nJetsSize2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets20"),
				  method= cms.string('pt()>20 && abs(eta())<2.4'),
                              ),
                              nJetsSize3 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets40"),
                                  method     = cms.string('pt()>40 && abs(eta())<2.4'),
                              ),
                              nJetsSize4 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets25"),
                                  method     = cms.string('pt()>25 && abs(eta())<2.4'),
                              ),

                              nJetsBTAGBTAGSize = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsCSVM30"),
                                  method     = cms.string('pt()>30 && abs(eta())<2.4 && bDiscriminator("combinedSecondaryVertexBJetTags")>0.679 '),
                              ),
                              nJetsBTAGSize2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsBTAGCSVM20"),
                                  method= cms.string('pt()>20 && abs(eta())<2.4&&bDiscriminator("combinedSecondaryVertexBJetTags")>0.679'),
                              ),
                              nJetsBTAGSize3 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsBTAGCSVM40"),
                                  method     = cms.string('pt()>40 && abs(eta())<2.4 &&bDiscriminator("combinedSecondaryVertexBJetTags")>0.679'),
                              ),
                              nJetsBTAGSize4 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsBTAGCSVM25"),
                                  method     = cms.string('pt()>25 && abs(eta())<2.4 &&bDiscriminator("combinedSecondaryVertexBJetTags")>0.679'),
                              ),

                              nJetsSizeE = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA4p530"),
                                  method     = cms.string('pt()>30 && abs(eta())<4.5'),
                              ),
                              nJetsSizeE2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA4p520"),
                                  method= cms.string('pt()>20 && abs(eta())<4.5'),
                              ),
                              nJetsSizeE3 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA4p540"),
                                  method     = cms.string('pt()>40 && abs(eta())<4.5'),
                              ),
                              nJetsSizeE4 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA4p525"),
                                  method     = cms.string('pt()>25 && abs(eta())<4.5'),
                              ),


				# These lines are commented to give you a smaller tree to start. Uncommenting them will save the pt, eta and phi of all jets in the event 
				# will be useful for TOP background studies	

                              jetPUID = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetPUID"),
                                    method     = cms.string("userInt('fullIdLoose')"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                              jetID = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetID"),
                                    method     = cms.string("userFloat('idLoose')"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                              jetpt = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetpt"),
				    method     = cms.string("pt()"),
				    leadingOnly=cms.untracked.bool(False)		
                              ),
                              jeteta = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jeteta"),
                                    method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                              jetphi = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetphi"),
                                    method     = cms.string("phi()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),

                            jetflavour = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetflavour"),
                                    method     = cms.string("partonFlavour()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),

                              jetBTagALLCSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_CSV"),
                                    method     = cms.string('bDiscriminator("combinedSecondaryVertexBJetTags")'),
                                    leadingOnly=cms.untracked.bool(False)
                              ),





                              #jetpt2 = cms.PSet(
                              #      pluginType = cms.string("PATJetFiller"),
                              #      src        = cms.InputTag(srcJets),
                              #      tag        = cms.string("jetpt_uncorr"),
                              #      method     = cms.string("userCand('uncorr').pt()"),
                              #      leadingOnly=cms.untracked.bool(False)               
                              #),

                              #jetpt3 = cms.PSet(
                              #      pluginType = cms.string("PATJetFiller"),
                              #      src        = cms.InputTag(srcJets),
                              #      tag        = cms.string("jetpt_smeared"),
                              #      method     = cms.string("userCand('smeared').pt()"),
                              #      leadingOnly=cms.untracked.bool(False)
                              #),

			     # Di-Jet object

                              massDiJets = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("massDiJets"),
                                    method     = cms.string("mass"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              ptDiJets = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("ptDiJets"),
                                    method     = cms.string("pt"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetpt1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1pt"),
                                    method     = cms.string("leg1.pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetpt_uncorr = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2pt"),
                                    method     = cms.string("leg2.pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetPUID1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1PUID"),
                                    method     = cms.string("leg1.userInt('fullIdLoose')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetID1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1ID"),
                                    method     = cms.string("leg1.userFloat('idLoose')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             jetPUID2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2PUID"),
                                    method     = cms.string("leg2.userInt('fullIdLoose')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),    
                              jetID2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2ID"),
                                    method     = cms.string("leg2.userFloat('idLoose')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ), 
                              jeteta1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1eta"),
                                    method     = cms.string("leg1.eta()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jeteta2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2eta"),
                                    method     = cms.string("leg2.eta()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetphi1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1phi"),
                                    method     = cms.string("leg1.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetphi2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2phi"),
                                    method     = cms.string("leg2.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),


			      # BTagging discriminators
                              jetBTagALLHP1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_SSVHP"),
                                    method     = cms.string('leg1.bDiscriminator("simpleSecondaryVertexHighPurBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLHP2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_SSVHP"),
                                    method     = cms.string('leg2.bDiscriminator("simpleSecondaryVertexHighPurBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLHE1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_SSVHE"),
                                    method     = cms.string('leg1.bDiscriminator("simpleSecondaryVertexHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLHE2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_SSVHE"),
                                    method     = cms.string('leg2.bDiscriminator("simpleSecondaryVertexHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLTCHE1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_TCHE"),
                                    method     = cms.string('leg1.bDiscriminator("trackCountingHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLTCHE2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_TCHE"),
                                    method     = cms.string('leg2.bDiscriminator("trackCountingHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLCSV1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_CSV"),
                                    method     = cms.string('leg1.bDiscriminator("combinedSecondaryVertexBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLCSV2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_CSV"),
                                    method     = cms.string('leg2.bDiscriminator("combinedSecondaryVertexBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
				# Secondary Vertex Mass --> User Defined Variable, assumes that the tracks coming from the SSV are all pions and reconstructs the mass

                              jetBTagSSVMASS1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_SSVMass"),
                                    method     = cms.string('leg1.userFloat("massSSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASS2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_SSVMass"),
                                    method     = cms.string('leg2.userFloat("massSSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagSSVCharge1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_SSVCharge"),
                                    method     = cms.string('leg1.userFloat("chargeSSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVCharge2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_SSVCharge"),
                                    method     = cms.string('leg2.userFloat("chargeSSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),


			       # Flight Distance (associated to the SSV algo)

                              jetBTagFlightDistance1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1FlightDistance"),
                                    method     = cms.string('leg1.userFloat("flightDistance")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagFlightDistance2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2FlightDistance"),
                                    method     = cms.string('leg2.userFloat("flightDistance")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagFlightDistanceError1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1ErrorFlightDistance"),
                                    method     = cms.string("leg1.userFloat('errorFlightDistance')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagFlightDistanceError2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2ErrorFlightDistance"),
                                    method     = cms.string("leg2.userFloat('errorFlightDistance')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

			      # Number of tracks in the SV

                              jetBTagnTracksSSV1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1NumberOfTracksSSV"),
                                    method     = cms.string("leg1.userFloat('nTracksSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagnTracksSSV2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2NumberOfTracksSSV"),
                                    method     = cms.string("leg2.userFloat('nTracksSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

				# Negative solutions to the SV algo (lets ignore for now)

                              jetBTagSSVMASSNEG1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTag_SSVNEGMass"),
                                    method     = cms.string('leg1.userFloat("mass_SSVNEG")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASSNEG2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTag_SSVNEGMass"),
                                    method     = cms.string('leg2.userFloat("mass_SSVNEG")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagNSV1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1NumberOfSSV"),
                                    method     = cms.string("leg1.userFloat('nNegativeSSV')"),   # this is a stupid bug, the neg is the pos and viceversa
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNSV2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2NumberOfSSV"),
                                    method     = cms.string("leg2.userFloat('nNegativeSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),


                              jetBTagNegALL1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1BTagNeg"),
                                    method     = cms.string("leg1.userFloat('btagNEGSSVHE')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNegALL2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2BTagNeg"),
                                    method     = cms.string("leg2.userFloat('btagNEGSSVHE')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagNegFlightDistance1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1FlightDistanceNEG"),
                                    method     = cms.string('leg1.userFloat("flightDistanceNEG")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNegFlightDistance2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2FlightDistanceNEG"),
                                    method     = cms.string('leg2.userFloat("flightDistanceNEG")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNegnTracksSSV1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1NumberOfTracksNEGSSV"),
                                    method     = cms.string("leg1.userFloat('nTracksNEGSSV')"),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNegnTracksSSV2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2NumberOfTracksNEGSSV"),
                                    method     = cms.string("leg2.userFloat('nTracksNEGSSV')"),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

			
			
                              # Good Muon in Jet (not-isolated)
	
                              jetMuonInJet1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1MuonInJetPt"),
                                    method     = cms.string('leg1.userFloat("MuonInJetPt")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetMuonInJet2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2MuonInJetPt"),
                                    method     = cms.string('leg2.userFloat("MuonInJetPt")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetMuonInJetPtRel1 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet1MuonInJetPtRel"),
                                    method     = cms.string('leg1.userFloat("MuonInJetPtRel")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetMuonInJetPtRel2 = cms.PSet(
                                    pluginType = cms.string("PATJetPairFiller"),
                                    src        = cms.InputTag(srcDiJets),
                                    tag        = cms.string("jet2MuonInJetPtRel"),
                                    method     = cms.string('leg2.userFloat("MuonInJetPtRel")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              eleSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(srcElectrons),
                                    tag        = cms.string("electronsLoose10"), # Number of Electrons (WP95 & Iso)in the event
                              ),

                              eNuSize2 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(srcWENUTIGHT),
                                    tag        = cms.string("eNuCandsTight"), # Number of WENU Candidates in the event
                              ),

                              mtELENU = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(srcWENUTIGHT),
                                    tag        = cms.string("mtENU"),   # transverse mass of the electron-neutrino system. "Cor" since the met is corrected
                                    method     = cms.string("corMt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
			   


                              eleSize2 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("vetoPatElectrons20"),
                                    tag        = cms.string("electronsLoose20"), # Number of Electrons (WP95 & Iso)in the event
                              ),

			      # Neutrino, from Kalanand (revising the eqns wouldnt be amiss) 	
				neutrinoPz1=cms.PSet(
				  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("metPz1"),
                                  method     = cms.string("metPz()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                                neutrinoPz2=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("metPz2"),
                                  method     = cms.string("metPzB()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                wp4M=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("mW"),
                                  method     = cms.string("WP4().mass()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                                wp4eta=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("etaW"),
                                  method     = cms.string("WP4().eta()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                               ht=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("ht"),
                                  method     = cms.string("ht()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

	                       deltaPhi=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("DPhi"),
                                  method     = cms.string("dPhi()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
			
                               massTOP=cms.PSet(
                                  pluginType = cms.string("PATMuNuJetJetQuadFiller"),
                                  src        = cms.InputTag(srcQuad),
                                  tag        = cms.string("topMass"),
                                  method     = cms.string("sqrt((leg1.WP4().energy()+leg2.leg1().energy())*(leg1.WP4().energy()+leg2.leg1().energy())-( (leg1.px()+leg2.leg1().px())*(leg1.px()+leg2.leg1().px())+ (leg1.WP4().py()+leg2.leg1().py())*(leg1.WP4().py()+leg2.leg1().py() )+(leg1.WP4().pz()+leg2.leg1().pz())*(leg1.WP4().pz()+leg2.leg1().pz() )))"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                               massTOP2=cms.PSet(
                                  pluginType = cms.string("PATMuNuJetJetQuadFiller"),
                                  src        = cms.InputTag(srcQuad),
                                  tag        = cms.string("topMass2"),
                                  method     = cms.string("sqrt((leg1.WP4().energy()+leg2.leg2().energy())*(leg1.WP4().energy()+leg2.leg2().energy())- ( (leg1.px()+leg2.leg2().px())*(leg1.px()+leg2.leg2().px())+(leg1.WP4().py()+leg2.leg2().py())*(leg1.WP4().py()+leg2.leg2().py() )+ (leg1.WP4().pz()+leg2.leg2().pz())*(leg1.WP4().pz()+leg2.leg2().pz())))"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                               massTOPB=cms.PSet(
                                  pluginType = cms.string("PATMuNuJetJetQuadFiller"),
                                  src        = cms.InputTag(srcQuad),
                                  tag        = cms.string("topMassB"),
                                  method     = cms.string("sqrt((leg1.WP4B().energy()+leg2.leg1().energy())*(leg1.WP4B().energy()+leg2.leg1().energy())-( (leg1.px()+leg2.leg1().px())*(leg1.px()+leg2.leg1().px())+ (leg1.WP4B().py()+leg2.leg1().py())*(leg1.WP4B().py()+leg2.leg1().py() )+(leg1.WP4B().pz()+leg2.leg1().pz())*(leg1.WP4B().pz()+leg2.leg1().pz() )))"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                               massTOPB2=cms.PSet(
                                  pluginType = cms.string("PATMuNuJetJetQuadFiller"),
                                  src        = cms.InputTag(srcQuad),
                                  tag        = cms.string("topMassB2"),
                                  method     = cms.string("sqrt((leg1.WP4B().energy()+leg2.leg2().energy())*(leg1.WP4B().energy()+leg2.leg2().energy())- ( (leg1.px()+leg2.leg2().px())*(leg1.px()+leg2.leg2().px())+(leg1.WP4B().py()+leg2.leg2().py())*(leg1.WP4B().py()+leg2.leg2().py() )+ (leg1.WP4B().pz()+leg2.leg2().pz())*(leg1.WP4B().pz()+leg2.leg2().pz())))"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),





                               pzMu=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("pzMu"),
                                  method     = cms.string("lepton.pz()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                               pyMu=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("pyMu"),
                                  method     = cms.string("lepton.py()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                               pxMu=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("pxMu"),
                                  method     = cms.string("lepton.px()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                MetPx=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("METEx"),
                                  method     = cms.string("calibratedMET.px()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                MetPy=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("METEy"),
                                  method     = cms.string("calibratedMET.py()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              check= cms.PSet(
                                    pluginType=cms.string("FindResonancesExtra"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0)
                                ),

                              check2= cms.PSet(
                                    pluginType=cms.string("FindResonancesExtra"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(1)
                                ),




                               check3= cms.PSet(
                                    pluginType=cms.string("MatchJetWithVertex"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0),
                                    tag = cms.string("jetDXYToPV_0")
                                ),



                               check4= cms.PSet(
                                    pluginType=cms.string("MatchJetWithVertex"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(1),
                                    tag = cms.string("jetDXYToPV_1")
                                ),


   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)



