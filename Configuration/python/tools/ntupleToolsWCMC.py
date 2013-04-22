import FWCore.ParameterSet.Config as cms


def addMCEventTree(process,name,src="genParticles"):
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

                             genWeights = cms.PSet(
                                  pluginType = cms.string("GenInfoFiller"),
                                  src        = cms.InputTag("generator"),
                               ),
                genID = cms.PSet(
                        pluginType = cms.string("GenID"),
                        src        = cms.InputTag("genParticles"),
                ),

                        genID3 = cms.PSet(
                                pluginType = cms.string("GenID2"),
                                src        = cms.InputTag("genParticles"),
                                verbose    = cms.untracked.bool(False),
                                saveCs    = cms.untracked.bool(True),
                                saveBs    = cms.untracked.bool(True),
                                ptMin    = cms.untracked.double(0),
                                etaMax    = cms.untracked.double(5),

                        ),



                              pu = cms.PSet(
                                  pluginType = cms.string("PUFiller"),
                                  src        = cms.InputTag("addPileupInfo"),
                                  tag        = cms.string("pu"),
                                  ),


                               check3= cms.PSet(
                                    pluginType=cms.string("GenDecayModes"),
                                    verbose   =cms.untracked.bool(False),
                                src        = cms.InputTag("genParticles"),

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
    			    	PdfWeights= cms.untracked.string("pdfweight_cteq66"),
                                  PdfSetNames = cms.untracked.InputTag(
                                   "pdfWeights:cteq66"
                                 )
                            ),

#                              pdfs3= cms.PSet(
#                                  pluginType=cms.string("PDFReWeighting"),
 #                                 verbose   =cms.untracked.bool(True),
#                                  tag= cms.string("pdfweight_ct6ll"),
 #                                 PdfSetNames = cms.untracked.InputTag(
#                                    "pdfWeights:cteq6ll"
#                                  )
#                             ),


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

#                            jetflavour = cms.PSet(
#                                    pluginType = cms.string("GENJetFiller"),
#                                    src        = cms.InputTag("ak5GenJets"),
#                                    tag        = cms.string("genJetFlavour"),
#                                    method     = cms.string("partonFlavour()"),
#                                    leadingOnly=cms.untracked.bool(False)
#                              ),

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


   )

   setattr(process, name, eventTree)
   p = cms.Path(getattr(process,name))
   setattr(process, name+'Path', p)


