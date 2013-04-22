import FWCore.ParameterSet.Config as cms

from UWAnalysis.Configuration.tools.analysisTools import TriggerPaths


def addMuNuJetsEventTree(process,name,src = 'wCandsSel', srcLL = 'diMuonsSorted',srcJets='patJetsForAnalysis',srcMuons='cleanPatMuons',srcElectrons='vetoPatElectrons10', srcWENUTIGHT='wCandsSelEleVeto'):
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

                              nJetsSizeE = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA530"),
                                  method     = cms.string('pt()>30 && abs(eta())<5'),
                              ),
                              nJetsSizeE2 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA520"),
                                  method= cms.string('pt()>20 && abs(eta())<5'),
                              ),
                              nJetsSizeE3 = cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairJetCountFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nJetsETA540"),
                                  method     = cms.string('pt()>40 && abs(eta())<5'),
                              ),


				# These lines are commented to give you a smaller tree to start. Uncommenting them will save the pt, eta and phi of all jets in the event 
				# will be useful for TOP background studies	

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

                              jetBTagALLSSVHE = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SSVHE"),
                                    method     = cms.string('bDiscriminator("simpleSecondaryVertexHighPurBJetTags")'),
                                    leadingOnly=cms.untracked.bool(False)
                              ),

                              jetBTagALLSSVHP = cms.PSet(
                                    pluginType = cms.string("PATJetFiller"),
                                    src        = cms.InputTag(srcJets),
                                    tag        = cms.string("jetBTag_SSVHP"),
                                    method     = cms.string('bDiscriminator("simpleSecondaryVertexHighEffBJetTags")'),
                                    leadingOnly=cms.untracked.bool(False)
                              ),



			     # gen Level identification
	                genID = cms.PSet(
                	        pluginType = cms.string("GenID"),
                        	src        = cms.InputTag("genParticles"),
        	        ),


                        genID2 = cms.PSet(
                                pluginType = cms.string("GenID2"),
                                src        = cms.InputTag("genParticles"),
				verbose    = cms.untracked.bool(True),
                                saveCs    = cms.untracked.bool(True),

                        ),


#                             genID3=cms.PSet(
#                                  pluginType = cms.string("PartonJetID"),
#                                  src        = cms.InputTag("ak5PartonJets"),
#                                  ptMin      = cms.double(10),
#                                  etaMax     = cms.double(2.5)
#                                ),


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

                                neutrinoTruePz=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nuPz"),
                                  method     = cms.string("trueMEz()"),
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


                                neutrinoTruePx=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nuPx"),
                                  method     = cms.string("trueMEx()"),
                                  leadingOnly=cms.untracked.bool(True)
                              ),

                                neutrinoTruePy=cms.PSet(
                                  pluginType = cms.string("PATMuonNuPairFiller"),
                                  src        = cms.InputTag(src),
                                  tag        = cms.string("nuPy"),
                                  method     = cms.string("trueMEy()"),
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

		 	     simBHadrons = cms.PSet(
                                    pluginType = cms.string("SimBHadronsFiller"),
                                    src        = cms.InputTag("bhadrons"),
                                    tag        = cms.string("bHadronsID"),
				    method     = cms.string("pdgId()"),
				    leadingOnly=cms.untracked.bool(False)	
                              ),

                             simBHadronsPT = cms.PSet(
                                    pluginType = cms.string("SimBHadronsFiller"),
                                    src        = cms.InputTag("bhadrons"),
                                    tag        = cms.string("bHadronsPT"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),

                             simBHadronsETA = cms.PSet(
                                    pluginType = cms.string("SimBHadronsFiller"),
                                    src        = cms.InputTag("bhadrons"),
                                    tag        = cms.string("bHadronsETA"),
                                    method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),

                             simBHadronsPHI = cms.PSet(
                                    pluginType = cms.string("SimBHadronsFiller"),
                                    src        = cms.InputTag("bhadrons"),
                                    tag        = cms.string("bHadronsPHI"),
                                    method     = cms.string("phi()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),



                             simBHadronsSIZE = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("bhadrons"),
                                    tag        = cms.string("bHadronsSIZE"),
                              ),


                             cSIZE2= cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("cbarCands"),
                                    tag        = cms.string("cbarCandsSIZE"),
                              ),


                             cSIZE = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("cCands"),
                                    tag        = cms.string("cCandsSIZE"),
                              ),

                             cstatus = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cCands"),
                                    tag        = cms.string("cCandsStatus"),
                                    method     = cms.string("status()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cbarStatus2 = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cbarCands"),
                                    tag        = cms.string("cbarCandsStatus"),
                                    method     = cms.string("status()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cPT = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cCands"),
                                    tag        = cms.string("cCandsPT"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cbarPT2 = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cbarCands"),
                                    tag        = cms.string("cbarCandsPT"),
                                    method     = cms.string("pt()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cPHI = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cCands"),
                                    tag        = cms.string("cCandsPHI"),
                                    method     = cms.string("phi()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cbarPHI2 = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cbarCands"),
                                    tag        = cms.string("cbarCandsPHI"),
                                    method     = cms.string("phi()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cETA = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cCands"),
                                    tag        = cms.string("cCandsETA"),
                                    method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),
                             cbarETA2 = cms.PSet(
                                    pluginType = cms.string("PATGenParticleFiller"),
                                    src        = cms.InputTag("cbarCands"),
                                    tag        = cms.string("cbarCandsETA"),
                                    method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(False)
                              )

   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)



