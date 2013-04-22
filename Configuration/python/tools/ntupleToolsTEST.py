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
                                   )




   )
   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)

