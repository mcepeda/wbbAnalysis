import FWCore.ParameterSet.Config as cms

def addMCEventTree(process,name,src="genParticles"):
   process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root") )
   eventTree = cms.EDAnalyzer('EventTreeMaker',
                              coreCollections = cms.VInputTag(
                                   cms.InputTag(src)
                              ),
			     # gen Level identification
	                genID = cms.PSet(
                	        pluginType = cms.string("GenID"),
                        	src        = cms.InputTag("genParticles"),
        	        ),

                genID2 = cms.PSet(
                        pluginType = cms.string("GenID2"),
                         src        = cms.InputTag("genParticles"),
       		        verbose    = cms.untracked.bool(False),
                        saveCs     = cms.untracked.bool(True),
                        saveBs     = cms.untracked.bool(True),
       		        ptMin      = cms.untracked.double(0),
                        etaMax     = cms.untracked.double(10),
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

                               genDecay= cms.PSet(
                                    pluginType=cms.string("GenDecayModes"),
                                    verbose   =cms.untracked.bool(False),
                                src        = cms.InputTag("genParticles"),
                               ),

                        ToyDecays= cms.PSet(
                                pluginType = cms.string("TopDecays"),
                                src        = cms.InputTag("genParticles"),
                        ), 


                           jetPT = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5GenJets"),
                                   tag        = cms.string("genJetPt"),
                                   method     = cms.string("pt()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPHI = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5GenJets"),
                                   tag        = cms.string("genJetPhi"),
                                   method     = cms.string("phi()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetETA = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5GenJets"),
                                   tag        = cms.string("genJetEta"),
                                   method     = cms.string("eta()"),
                                    leadingOnly=cms.untracked.bool(False)
                              ),


                           jetPartonPT = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJets"),
                                   tag        = cms.string("genPartonJetPt"),
                                   method     = cms.string("pt()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonETA = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJets"),
                                   tag        = cms.string("genPartonJetEta"),
                                   method     = cms.string("eta()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonPHI = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJets"),
                                   tag        = cms.string("genPartonJetPhi"),
                                   method     = cms.string("phi()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2PT = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJetsV2"),
                                   tag        = cms.string("genPartonJetV2Pt"),
                                   method     = cms.string("pt()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2ETA = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJetsV2"),
                                   tag        = cms.string("genPartonJetV2Eta"),
                                   method     = cms.string("eta()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2PHI = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("ak5PartonJetsV2"),
                                   tag        = cms.string("genPartonJetV2Phi"),
                                   method     = cms.string("phi()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2CandsSt2 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2Matched"),
                                   method     = cms.string("userFloat('matchedBsSt2')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),
                           jetPartonV2CandsSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2MatchedSt3"),
                                   method     = cms.string("userFloat('matchedBsSt3')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonCandsSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt3"),
                                   tag        = cms.string("genPartonJetMatchedSt3"),
                                   method     = cms.string("userFloat('matchedBsSt3')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2CandsdRSt2 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2mindRSt2"),
                                   method     = cms.string("userFloat('minDRSt2')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),
                           jetPartonV2CandsdRSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2mindRSt3"),
                                   method     = cms.string("userFloat('minDRSt3')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),
                           jetPartonCandsdRSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt3"),
                                   tag        = cms.string("genPartonJetmindRSt3"),
                                   method     = cms.string("userFloat('minDRSt3')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2CandsleadSt2 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2LeadPtSt2"),
                                   method     = cms.string("userFloat('leadJetSt2Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),
                           jetPartonV2CandsleadSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2LeadPtSt3"),
                                   method     = cms.string("userFloat('leadJetSt3Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),
                           jetPartonCandsleadSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt3"),
                                   tag        = cms.string("genPartonJetLeadPtSt3"),
                                   method     = cms.string("userFloat('leadJetSt3Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV2CandssecondSt2 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2SecondPtSt2"),
                                   method     = cms.string("userFloat('secondJetSt2Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),    
                           jetPartonV2CandssecondSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt2"),
                                   tag        = cms.string("genPartonJetV2SecondPtSt3"),
                                   method     = cms.string("userFloat('secondJetSt3Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonCandssecondSt3 = cms.PSet(
                                   pluginType = cms.string("PATJetFiller"),
                                   src        = cms.InputTag("btagPartonJetsSt3"),
                                   tag        = cms.string("genPartonJetSecondPtSt3"),
                                   method     = cms.string("userFloat('secondJetSt3Pt')"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),


                           jetPartonV3PT = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("k10PartonJets"),
                                   tag        = cms.string("genPartonJetk10Pt"),
                                   method     = cms.string("pt()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV3ETA = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("k10PartonJets"),
                                   tag        = cms.string("genPartonJetk10Eta"),
                                   method     = cms.string("eta()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),

                           jetPartonV3PHI = cms.PSet(
                                   pluginType = cms.string("GENJetFiller"),
                                   src        = cms.InputTag("k10PartonJets"),
                                   tag        = cms.string("genPartonJetk10Phi"),
                                   method     = cms.string("phi()"),
                                   leadingOnly=cms.untracked.bool(False)
                             ),



                             partonsSt3SIZE = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("myPartons"),
                                    tag        = cms.string("partonsSt3SIZE"),
                              ),

                             partonsSt2St3SIZE = cms.PSet(
                                    pluginType = cms.string("CollectionSizeFiller"),
                                    src        = cms.InputTag("myPartonsV2"),
                                    tag        = cms.string("partonsSt3St2SIZE"),
                              ),

                              pdfs= cms.PSet(
                                  pluginType=cms.string("PDFReWeighting"),
                                  verbose   =cms.untracked.bool(False),
                                  PdfWeights= cms.untracked.string("pdfweight_mstw2008nnlo"),
                                     PdfSetNames = cms.untracked.InputTag(
                              "pdfWeights:MSTW2008nnlo68cl"
                                    )
                             ),

                             pdfs2= cms.PSet(
                                 pluginType=cms.string("PDFReWeighting"),
                                 verbose   =cms.untracked.bool(False),
                                PdfWeights= cms.untracked.string("pdfweight_CT10"),
                                  PdfSetNames = cms.untracked.InputTag(
                                   "pdfWeights:CT10"
                                 )
                            ),

   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)



