import FWCore.ParameterSet.Config as cms

from UWAnalysis.Configuration.tools.analysisTools import TriggerPaths


def addMuNuJetSimpleEventTree(process,name,src = 'wCandsSel', srcLL = 'diMuonsSorted',srcJets='patJetsForAnalysis',srcMuons='preSelMuons'):
   process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )
   eventTree = cms.EDAnalyzer('EventTreeMaker',
                              coreCollections = cms.VInputTag(
                                   cms.InputTag(src)
                              ),
                              RHO = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("RHO"),
                                  method     = cms.string('lepton.userFloat("rho")'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                              PVs = cms.PSet(
                                    pluginType = cms.string("VertexSizeFiller"),
                                    src        = cms.InputTag("primaryVertexFilter"),
                                    tag        = cms.string("vertices")
                              ),
                              dxyMu2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("dxyMuVtx"),
                                  method     = cms.string('lepton.userFloat("ipDXY")'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              dzMu2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("dzMuVtx"),
                                  method     = cms.string('lepton.userFloat("dz")'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              trigger = cms.PSet(
                                  pluginType = cms.string("TriggerFiller"),
                                  src        = cms.InputTag("patTrigger"),
                                  paths      = cms.vstring(TriggerPaths)
                                  ),
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
                              idWWMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("idWWMu"),
                                  method     = cms.string("lepton.userFloat('WWID2011')"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              phiMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("phiMu"),
                                  method     = cms.string("lepton.phi()"),
                                  leadingOnly=cms.untracked.bool(True)
                             ),
                             uncormet = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMet"),
                                    method     = cms.string("met.pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             met = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("met"),
                                    method     = cms.string("calibratedMET().pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              metphi = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metphiUnCor"),
                                    method     = cms.string("met.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              metphiCor = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metphi"),
                                    method     = cms.string("calibratedMET.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              metSg = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metSg"),
                                    method     = cms.string("met.mEtSig()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                             sumEt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("sumEt"),
                                    method     = cms.string("met.sumEt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              charge = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("charge"),
                                  method     = cms.string("lepton.charge()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMu = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMu"),
                                  method     = cms.string("(lepton.isolationR03().sumPt+lepton.isolationR03().emEt+lepton.isolationR03().hadEt)/lepton.pt()"),
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
                              isoMuPFIsoRho = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIsoRho"),
                                  method     = cms.string('(lepton.chargedHadronIso+max(lepton.photonIso()+lepton.neutralHadronIso()-lepton.userFloat("rho")*3.14*0.4*0.4,0.0))/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              isoMuPFIso = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("isoMuPFIso"),
                                  method     = cms.string('(lepton.chargedHadronIso+lepton.photonIso()+lepton.neutralHadronIso())/lepton.pt()'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              pt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("pt"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              unCorMt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMt"),
                                    method     = cms.string("mt"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              mt = cms.PSet(
                                    pluginType = cms.string("PATMuonNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("mt"),
                                    method     = cms.string("corMt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              mumuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(srcLL),
                                    tag        = cms.string("diLeptons"),
                              ),
                              muNuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("nCands"),
                              ),
                              eleSize1 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("vetoPatElectrons10"),
                                    tag        = cms.string("electronsLoose10"), # Number of Electrons (WP95 & Iso)in the event
                              ),
                              eleSize2 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("vetoPatElectrons20"),
                                    tag        = cms.string("electronsLoose20"), # Number of Electrons (WP95 & Iso)in the event
                              ),
                              nJetsSize = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets"),
                                  method     = cms.string('pt()>30 && abs(eta())<2.1'),
                              ),
                              nJetsSize2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets20"),
				  method= cms.string('pt()>20 && abs(eta())<2.1'),
                              ),
                              nJetsSize3 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets40Eta21"),
                                  method     = cms.string('pt()>40 && abs(eta())<2.1'),
                              ),
                              nMuonsSize = cms.PSet(
                                  pluginType = cms.string("CollectionSizeFiller"),
                                  src        = cms.InputTag(srcMuons),
                                  tag        = cms.string("nMuons10"),
                              ),
                             diMuonMass = cms.PSet(
                                  pluginType = cms.string("PATMuPairFiller"),
                                  src        = cms.InputTag(srcLL),
                                  tag        = cms.string("dimuonMass"),
                                  method     = cms.string('mass'),
                                  leadingOnly=cms.untracked.bool(True)
                             ),
                              jetpt1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetpt"),
				    method     = cms.string("pt()"),
				    leadingOnly=cms.untracked.bool(True)		
                              ),
                              jetenergy1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetenergy"),
                                    method     = cms.string("energy()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetcharge1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetcharge"),
                                    method     = cms.string("charge()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jeteta1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jeteta"),
                                    method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetphi1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetphi"),
                                    method     = cms.string("phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLHP = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagHP"),
                                    method     = cms.string('bDiscriminator("simpleSecondaryVertexHighPurBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLIVF = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagIVF"),
                                    method     = cms.string('bDiscriminator("inclusiveSecondaryVertexFinderTagInfosFiltered")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagALLDOUBLE = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagDOUBLE"),
                                    method     = cms.string('bDiscriminator("doubleSecondaryVertexHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagALL2 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSecondSV2"),
                                    method     = cms.string('userFloat("btagSSVHE2")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASS = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASS"),
                                    method     = cms.string('userFloat("mass_SSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASSNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASSNEG"),
                                    method     = cms.string('userFloat("mass_SSVNEG")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagFlightDistance = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("flightDistance"),
                                    method     = cms.string('userFloat("flightDistance")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagFlightDistanceError = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("errorFlightDistance"),
                                    method     = cms.string("userFloat('errorFlightDistance')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagFlightDistanceErrorNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("errorFlightDistanceNEG"),
                                    method     = cms.string("userFloat('errorFlightDistanceNEG')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagnTracksSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfTracksSSV"),
                                    method     = cms.string("userFloat('nTracksSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfSSV"),
                                    method     = cms.string("userFloat('nNegativeSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNNSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfNegativeSSV"),
                                    method     = cms.string("userFloat('nSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              chargeSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("chargeSSV"),
                                    method     = cms.string("userFloat('chargeSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             chargeSSVNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("chargeSSVNEG"),
                                    method     = cms.string("userFloat('chargeSSVNEG')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNegALL = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagNeg"),
                                    method     = cms.string("userFloat('btagNEGSSVHE')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagNegFlightDistance = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("flightDistanceNEG"),
                                    method     = cms.string('userFloat("flightDistanceNEG")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNegnTracksSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfTracksNEGSSV"),
                                    method     = cms.string("userFloat('nTracksNEGSSV')"),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTag1b = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_TCHE"),
                                    method     = cms.string('bDiscriminator("trackCountingHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagControl = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag"),
                                    method     = cms.string('bDiscriminator("simpleSecondaryVertexHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTag1c = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_TCHP"),
                                    method     = cms.string('bDiscriminator("trackCountingHighPurBJetTags")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                        jetFlavour = cms.PSet(
                                  pluginType = cms.string("PATJetFiller"),
                                  src        = cms.InputTag(srcJets),
                                  tag        = cms.string("jetPartonFlavour"),
                                  method     = cms.string('partonFlavour()'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                        genID2 = cms.PSet(
                                pluginType = cms.string("GenID2"),
                                src        = cms.InputTag("genParticles"),
                                verbose    = cms.untracked.bool(True),
                                saveCs    = cms.untracked.bool(True),
			        ptMin    = cms.untracked.double(0),
                                etaMax    = cms.untracked.double(5),


                        ),

                              jetSecondMuon = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPt"),
                                    method     = cms.string('userFloat("MuonInJetPt")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),
                              jetSecondMuonPhi = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPhi"),
                                    method     = cms.string('userFloat("MuonInJetPhi")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetSecondMuonEta = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonEta"),
                                    method     = cms.string('userFloat("MuonInJetEta")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonPtRel = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPtRel"),
                                    method     = cms.string('userFloat("MuonInJetPtRel")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonDXYERR = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDXYERR"),
                                    method     = cms.string('userFloat("MuonInJetDXYERR")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonDZERR = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDZERR"),
                                    method     = cms.string('userFloat("MuonInJetDZERR")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetSecondMuonISO = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonIsoABS"),
                                    method     = cms.string('userFloat("MuonInJetIsoABS")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetMuonEnergy = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetMuonEnergy"),
				    method     = cms.string(" muonEnergy"),
                                    leadingOnly=cms.untracked.bool(True)

				),
                              jetMuonMultiplicity = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetMuonMultiplicity"),
                                    method     = cms.string("muonMultiplicity()"),
                                    leadingOnly=cms.untracked.bool(True)

				),
                              jetElectronEnergy = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetElectronEnergy"),
                                    method     = cms.string("electronEnergy()"),
                                    leadingOnly=cms.untracked.bool(True)

                                ),
                              jetElectronMultiplicity = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetElectronMultiplicity"),
                                    method     = cms.string("electronMultiplicity()"),
                                    leadingOnly=cms.untracked.bool(True)

                                ),

                             genWeights = cms.PSet(
                                  pluginType = cms.string("GenInfoFiller"),
                                  src        = cms.InputTag("generator"),
                               ),
                genID = cms.PSet(
                        pluginType = cms.string("GenID"),
                        src        = cms.InputTag("genParticles"),
                ),

		genID3=cms.PSet(
			pluginType = cms.string("PATJetFiller"),
			 src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetFlavour"),
                                    method     = cms.string("partonFlavour()"),
                                    leadingOnly=cms.untracked.bool(True)
                                ),

#                genID3=cms.PSet(
#                        pluginType = cms.string("PartonJetID"),
#                         src        = cms.InputTag("ak5PartonJets"),
#			 ptMin      = cms.double(10),
#			 etaMax     = cms.double(2.5)
#                                ),

                              pu = cms.PSet(
                                  pluginType = cms.string("PUFiller"),
                                  src        = cms.InputTag("addPileupInfo"),
                                  tag        = cms.string("pu"),
                                  ),
                              check= cms.PSet(
                                    pluginType=cms.string("FindResonancesExtra"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0)
                                ),

                                checkBTAGEXTRA= cms.PSet(
                                    pluginType=cms.string("ExtraBTaggers"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    jetindex=cms.int32(0),
                                    tag=cms.string("jet_DoubleTagging"),
                                    btagDisc=cms.string("doubleSecondaryVertexHighEffBJetTags")
                                ),

                                checkBTAGEXTRA2= cms.PSet(
                                    pluginType=cms.string("ExtraBTaggers"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    jetindex=cms.int32(0),
                                    tag=cms.string("jet_IVF"),
                                    btagDisc=cms.string("inclusiveSecondaryVertexFinderTagInfosFiltered")
                                ),



                               check2= cms.PSet(
                                    pluginType=cms.string("MatchJetWithVertex"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0),
                                    tag = cms.string("jetDXYToPV")

                                ),


                               check3= cms.PSet(
                                    pluginType=cms.string("GenDecayModes"),
                                src        = cms.InputTag("genParticles"),

                                    verbose   =cms.untracked.bool(False),
                                ),

   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)


def addEleNuJetSimpleEventTree(process,name,src = 'wCandsSelEle', srcLL = 'diElecsSorted',srcJets='patJetsForAnalysis',srcElectrons='preSelElectrons'):
   process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )
   eventTree = cms.EDAnalyzer('EventTreeMaker',
                              coreCollections = cms.VInputTag(
                                   cms.InputTag(src)
                              ),
                              PVs = cms.PSet(
                                    pluginType = cms.string("VertexSizeFiller"),
                                    src        = cms.InputTag("primaryVertexFilter"),
                                    tag        = cms.string("vertices")
                              ),
                              trigger = cms.PSet(
                                  pluginType = cms.string("TriggerFiller"),
                                  src        = cms.InputTag("patTrigger"),
                                  paths      = cms.vstring(TriggerPaths)
                                  ),
		#ele specific quantities
                eleDXY = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("ipDXY"),
                        method     = cms.string('lepton.userFloat("ipDXY")'),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleDZ = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("dz"),
                        method     = cms.string('lepton.userFloat("dz")'),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleRelIso03BRho = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleRelIso03BRho"),
                        method     = cms.string("(lepton.dr03TkSumPt()+max(lepton.dr03EcalRecHitSumEt()-1.0,0.0)+lepton.dr03HcalTowerSumEt()-lepton.userFloat('rho')*3.14*0.09)/lepton.pt()"),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleRelIso03ERho = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleRelIso03ERho"),
                        method     = cms.string("(lepton.dr03TkSumPt()+lepton.dr03EcalRecHitSumEt()+lepton.dr03HcalTowerSumEt()-lepton.userFloat('rho')*3.14*0.09)/lepton.pt()"),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleIsoECAL = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleIsoEcal"),
                        method     = cms.string("lepton.dr03EcalRecHitSumEt()"),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleIsoHCAL = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleIsoHcal"),
                        method     = cms.string("lepton.dr03HcalTowerSumEt()"),
                    leadingOnly=cms.untracked.bool(True)
                ),
                eleIsoTK = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleIsoTK"),
                        method     = cms.string("lepton.dr03TkSumPt()"),
                    leadingOnly=cms.untracked.bool(True)
                ),
		eleDCotTheta = cms.PSet(
			pluginType = cms.string("PATElectronNuPairFiller"),
			src        = cms.InputTag(src),
			tag        = cms.string("eleDcotTheta"),
			method     = cms.string('lepton.convDcot'),
			leadingOnly=cms.untracked.bool(True)
		),
		eleConvDist = cms.PSet(
			pluginType = cms.string("PATElectronNuPairFiller"),
			src        = cms.InputTag(src),
			tag        = cms.string("eleConvDistance"),
			method     = cms.string('lepton.convDist'),
			leadingOnly=cms.untracked.bool(True)
		),
		eleMissHits = cms.PSet(
			pluginType = cms.string("PATElectronNuPairFiller"),
			src        = cms.InputTag(src),
			tag        = cms.string("eleMissHits"),
			method     = cms.string('lepton.gsfTrack().trackerExpectedHitsInner().numberOfHits()'),
			leadingOnly=cms.untracked.bool(True)
		),
		elePFRelIso = cms.PSet(
			pluginType = cms.string("PATElectronNuPairFiller"),
			src        = cms.InputTag(src),
			tag        = cms.string("eleRelPFIso"),
			method     = cms.string('(lepton.chargedHadronIso+lepton.photonIso+lepton.neutralHadronIso)/lepton.pt()'),
			leadingOnly=cms.untracked.bool(True)
		),
                eleelePFRelIsoRho = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleRelPfIsoRho"),
                        method     = cms.string('(lepton.chargedHadronIso()+max(lepton.photonIso()+lepton.neutralHadronIso()-lepton.userFloat("rho")*3.14*0.4*0.4,0.0))/lepton.pt()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
		eleEleIP = cms.PSet(
			pluginType = cms.string("PATElectronNuPairFiller"),
			src        = cms.InputTag(src),
			tag        = cms.string("eleIP"),
			method     = cms.string('lepton.dB'),
			leadingOnly=cms.untracked.bool(True)
		),
                eleBarrel = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("isEleB"),
                        method     = cms.string('lepton.isEB()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleEndcap = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("isEleE"),
                        method     = cms.string('lepton.isEE()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleSigmaIEtaIEta = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleSigmaietaieta"),
                        method     = cms.string('lepton.sigmaIetaIeta()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleDeltaEta = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleDeltaEta"),
                        method     = cms.string('lepton.deltaEtaSuperClusterTrackAtVtx()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleDeltaPhi = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleDeltaPhi"),
                        method     = cms.string('lepton.deltaPhiSuperClusterTrackAtVtx()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleHoverE= cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("eleHoverE"),
                        method     = cms.string('lepton.hcalOverEcal()'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleWP95 = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("idWP95Ele"),
                        method     = cms.string('lepton.userFloat("wp95")'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                eleWWID = cms.PSet(
                        pluginType = cms.string("PATElectronNuPairFiller"),
                        src        = cms.InputTag(src),
                        tag        = cms.string("idWWEle"),
                        method     = cms.string('lepton.userFloat("WWID")'),
                        leadingOnly=cms.untracked.bool(True)
                ),
                              ptEle = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("ptEle"),
                                  method     = cms.string("lepton.pt()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              etaEle = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("etaEle"),
                                  method     = cms.string("lepton.eta()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                              phiEle = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("phiEle"),
                                  method     = cms.string("lepton.phi()"),
                                  leadingOnly=cms.untracked.bool(True)
                             ),
                             met = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMet"),
                                    method     = cms.string("met.pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             corMet = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("met"),
                                    method     = cms.string("calibratedMET().pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              metphi = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metphiUnCor"),
                                    method     = cms.string("met.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             sumEt = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("sumEt"),
                                    method     = cms.string("met.sumEt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ), 
                              metSg = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metSg"),
                                    method     = cms.string("met.mEtSig()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              charge = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("charge"),
                                  method     = cms.string("lepton.charge()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),
                              

			      pt = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorPt"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                            corPt = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("pt"),
                                    method     = cms.string("corPt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              unCorMt = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("unCorMt"),
                                    method     = cms.string("mt"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              mt = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("mt"),
                                    method     = cms.string("corMt()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              dPhi = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("dPhi"),
                                    method     = cms.string("dPhi"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              mumuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(srcLL),
                                    tag        = cms.string("diLeptons"),
                              ),
                              massDiLepton = cms.PSet(
                                    pluginType = cms.string("PATElePairFiller"),
                                    src        = cms.InputTag(srcLL),
                                    tag        = cms.string("dieleMass"),
				    method     =cms.string("mass"),
				    leadingOnly=cms.untracked.bool(True)
                              ),
                              muNuSize = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("nCands"),
                              ),
                              nJetsSize = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets"),
                                  method     = cms.string('pt()>30 && abs(eta)<2.1'),
                              ),
                              nJetsSize2 = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets20"),
				  method= cms.string('pt()>20 && abs(eta)<2.1'),
                              ),
                              nJetsSize3 = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJets40Eta21"),
                                  method= cms.string('pt()>40 && abs(eta)<2.1'),
                              ),
                              nJetsSizeSSV = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsBTAGSSV"),
                                  method     = cms.string('pt()>30 && abs(eta)<2.1&&bDiscriminator("simpleSecondaryVertexHighEffBJetTags")>1.19'),
                              ),
                              jetpt1 = cms.PSet( 
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetpt"),
                                    method     = cms.string("pt()"), 
  					leadingOnly=cms.untracked.bool(True) 
                              ),
                              jetenergy1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetenergy"),
                                    method     = cms.string("energy()"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetcharge1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetcharge"),
                                    method     = cms.string("charge()"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jeteta1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jeteta"),
                                    method     = cms.string("eta()"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetphi1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetphi"),
                                    method     = cms.string("phi()"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),

                              jetBTagALL = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag"),
                                    method     = cms.string("userFloat('btagSSVHE')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagALLHP = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagHP"),
                                    method     = cms.string('userFloat("btagSSVHP")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagALL2 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSV2"),
                                    method     = cms.string('userFloat("btagSSVHE2")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagSSVMASS = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASS"),
                                    method     = cms.string('userFloat("mass_SSV")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASSNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASSNEG"),
                                    method     = cms.string('userFloat("mass_SSVNEG")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),


                            jetBTagTRACK1 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track1_px"),
                                    method     = cms.string('userFloat("SSV_track1_px")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK1_py = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track1_py"),
                                    method     = cms.string('userFloat("SSV_track1_py")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK1_pz = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track1_pz"),
                                    method     = cms.string('userFloat("SSV_track1_pz")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK1_charge = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track1_charge"),
                                    method     = cms.string('userFloat("SSV_track1_charge")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),


                            jetBTagTRACK2 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track2_px"),
                                    method     = cms.string('userFloat("SSV_track2_px")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK2_py = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track2_py"),
                                    method     = cms.string('userFloat("SSV_track2_py")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK2_pz = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track2_pz"),
                                    method     = cms.string('userFloat("SSV_track2_pz")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK2_charge = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track2_charge"),
                                    method     = cms.string('userFloat("SSV_track2_charge")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),


                            jetBTagTRACK3 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track3_px"),
                                    method     = cms.string('userFloat("SSV_track3_px")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK3_py = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track3_py"),
                                    method     = cms.string('userFloat("SSV_track3_py")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK3_pz = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track3_pz"),
                                    method     = cms.string('userFloat("SSV_track3_pz")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                            jetBTagTRACK3_charge = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SVV_track3_charge"),
                                    method     = cms.string('userFloat("SSV_track3_charge")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),


                              jetBTagSSVMASSD0D0 = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASSD0"),
                                    method     = cms.string('userFloat("massD0_SSV")'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                              jetBTagSSVMASSD0NEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagSVVMASSD0NEG"),
                                    method     = cms.string('userFloat("massD0_SSVNEG")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),





                              jetBTagFlightDistance = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("flightDistance"),
                                    method     = cms.string('userFloat("flightDistance")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),

                              jetBTagFlightDistanceError = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("errorFlightDistance"),
                                    method     = cms.string("userFloat('errorFlightDistance')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagFlightDistanceErrorNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("errorFlightDistanceNEG"),
                                    method     = cms.string("userFloat('errorFlightDistanceNEG')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              jetBTagnTracksSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfTracksSSV"),
                                    method     = cms.string("userFloat('nTracksSSV')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),

                              jetBTagNSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfSSV"),
                                    method     = cms.string("userFloat('nNegativeSSV')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNNSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfNegativeSSV"),
                                    method     = cms.string("userFloat('nSSV')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNegALL = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTagNeg"),
                                    method     = cms.string("userFloat('btagNEGSSVHE')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNegFlightDistance = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("flightDistanceNEG"),
                                    method     = cms.string('userFloat("flightDistanceNEG")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTagNegnTracksSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("numberOfTracksNEGSSV"),
                                    method     = cms.string("userFloat('nTracksNEGSSV')"),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetBTag1b = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_TCHE"),
                                    method     = cms.string('bDiscriminator("trackCountingHighEffBJetTags")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),

                              jetBTag1c = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_TCHP"),
                                    method     = cms.string('bDiscriminator("trackCountingHighPurBJetTags")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),
                              nElectronsSize = cms.PSet(
                                  pluginType = cms.string("CollectionSizeFiller"),
                                  src        = cms.InputTag(srcElectrons),
                                  tag        = cms.string("nElectrons10"),
                              ),
                              jetSecondElectron = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondElectronPt"),
                                    method     = cms.string('userFloat("ElectronInJetPt")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetSecondElectronPtRel = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondElectronPtRel"),
                                    method     = cms.string('userFloat("ElectronInJetPtRel")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetElectronEnergy = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetElectronEnergy"),
                                    method     = cms.string("electronEnergy()"),
                                        leadingOnly=cms.untracked.bool(True)

                                ),
                              jetElectronMultiplicity = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetElectronMultiplicity"),
                                    method     = cms.string("electronMultiplicity()"),
                                        leadingOnly=cms.untracked.bool(True)

                                ),
                              jetSecondMuon = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPt"),
                                    method     = cms.string('userFloat("MuonInJetPt")'),
                                        leadingOnly=cms.untracked.bool(True)

                              ),
                              jetSecondMuonPtRel = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPtRel"),
                                    method     = cms.string('userFloat("MuonInJetPtRel")'),
                                        leadingOnly=cms.untracked.bool(True)
                              ),

                              jetSecondMuonPhi = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonPhi"),
                                    method     = cms.string('userFloat("MuonInJetPhi")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetSecondMuonEta = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonEta"),
                                    method     = cms.string('userFloat("MuonInJetEta")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonDXYERR = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDXYERR"),
                                    method     = cms.string('userFloat("MuonInJetDXYERR")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonDZERR = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDZERR"),
                                    method     = cms.string('userFloat("MuonInJetDZERR")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetMuonEnergy = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetMuonEnergy"),
                                    method     = cms.string(" muonEnergy"),
                                        leadingOnly=cms.untracked.bool(True)

                                ),
                              jetMuonMultiplicity = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetMuonMultiplicity"),
                                    method     = cms.string("muonMultiplicity()"),
                                        leadingOnly=cms.untracked.bool(True)

                                ),

                              jetSecondMuonDXY = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDXY"),
                                    method     = cms.string('userFloat("MuonInJetDXY")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetSecondMuonDZ = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonDZ"),
                                    method     = cms.string('userFloat("MuonInJetDZ")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),


                              jetSecondMuonISO = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetSecondMuonIsoABS"),
                                    method     = cms.string('userFloat("MuonInJetIsoABS")'),
                                    leadingOnly=cms.untracked.bool(True)

                              ),

                              jetFlavour = cms.PSet(
                                  pluginType = cms.string("PATJetFiller"),
                                  src        = cms.InputTag(srcJets),
                                  tag        = cms.string("jetPartonFlavour"),
                                  method     = cms.string('partonFlavour()'),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                             genWeights = cms.PSet(
                                  pluginType = cms.string("GenInfoFiller"),
                                  src        = cms.InputTag("generator"),
                               ),
                genID = cms.PSet(
                        pluginType = cms.string("GenID"),
                        src        = cms.InputTag("genParticles"),
                ),

                genID2=cms.PSet(
                        pluginType = cms.string("PATJetFiller"),
                         src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetFlavour"),
                                    method     = cms.string("partonFlavour()"),
                                    leadingOnly=cms.untracked.bool(True)
                                ),

                        genID3 = cms.PSet(
                                pluginType = cms.string("GenID2"),
                                src        = cms.InputTag("genParticles"),
                                verbose    = cms.untracked.bool(True),
                                saveCs    = cms.untracked.bool(True),
                                ptMin    = cms.untracked.double(0),
                                etaMax    = cms.untracked.double(5),

                        ),



                              metphiCor = cms.PSet(
                                    pluginType = cms.string("PATElectronNuPairFiller"),
                                    src        = cms.InputTag(src),
                                    tag        = cms.string("metphi"),
                                    method     = cms.string("calibratedMET.phi()"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),



                              pu = cms.PSet(
                                  pluginType = cms.string("PUFiller"),
                                  src        = cms.InputTag("addPileupInfo"),
                                  tag        = cms.string("pu"),
                                  ),


                              RHO = cms.PSet(
                                  pluginType = cms.string("PATElectronNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("RHO"),
                                  method     = cms.string('lepton.userFloat("rho")'),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                              chargeSSV = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("chargeSSV"),
                                    method     = cms.string("userFloat('chargeSSV')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),
                             chargeSSVNEG = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("chargeSSVNEG"),
                                    method     = cms.string("userFloat('chargeSSVNEG')"),
                                    leadingOnly=cms.untracked.bool(True)
                              ),

                              muSize1 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("vetoPatMuons10"),
                                    tag        = cms.string("muonsLoose10"), # Number of Muons (WP95 & Iso)in the event
                              ),
                              muSize2 = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("vetoPatMuons20"),
                                    tag        = cms.string("muonsLoose20"), # Number of Muons (WP95 & Iso)in the event
                              ),

                              check= cms.PSet(
                                    pluginType=cms.string("FindResonancesExtra"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0)
                                ),

			       check2= cms.PSet(
                                    pluginType=cms.string("MatchJetWithVertex"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcPrimaryVertices=cms.InputTag("primaryVertexFilter"),
                                    verbose   =cms.untracked.bool(False),
                                    jetindex=cms.int32(0),
				    tag = cms.string("jetDXYToPV")
                                ),

                               check3= cms.PSet(
                                    pluginType=cms.string("GenDecayModes"),
                                    verbose   =cms.untracked.bool(False),
                                src        = cms.InputTag("genParticles"),

                                ),

				checkBTAGEXTRA= cms.PSet(
                                    pluginType=cms.string("ExtraBTaggers"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    jetindex=cms.int32(0),
				    tag=cms.string("jet_DoubleTagging"),
				    btagDisc=cms.string("doubleSecondaryVertexHighEffBJetTags")	
                                ),

                                checkBTAGEXTRA2= cms.PSet(
                                    pluginType=cms.string("ExtraBTaggers"),
                                    src       = cms.InputTag("patJetsForAnalysis"),
                                    srcBTAG       = cms.InputTag("patJetsForBTagging"),
                                    jetindex=cms.int32(0),
                                    tag=cms.string("jet_IVF"),
                                    btagDisc=cms.string("inclusiveSecondaryVertexFinderTagInfosFiltered")
                                ),






   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)


