#ifndef UWAnalysis_RecoTools_CompositePtrCandidateT1T2MEtAlgorithm_h
#define UWAnalysis_RecoTools_CompositePtrCandidateT1T2MEtAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/normalizedPhi.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "UWAnalysis/DataFormats/interface/CompositePtrCandidateT1T2MEt.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "UWAnalysis/RecoTools/interface/candidateAuxFunctions.h"
#include "UWAnalysis/RecoTools/interface/METCalibrator.h"
#include "UWAnalysis/RecoTools/interface/VBFMVA.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"



#include <TMath.h>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtAlgorithm 
{
  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::Ptr<T2> T2Ptr;
  typedef edm::Ptr<pat::Jet> JetPtr;
  typedef std::vector<JetPtr> JetPtrVector;


 public:
  CompositePtrCandidateT1T2MEtAlgorithm(const edm::ParameterSet& cfg)
  {
    recoMode_ = cfg.getParameter<std::string>("recoMode");
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
    edm::FileInPath thePath("UserCode/MitHtt/data/VBFMVA/MuTau/VBFMVA_BDTG.weights.xml");
    VBFMVA_.Initialize( thePath.fullPath() );
  }
  ~CompositePtrCandidateT1T2MEtAlgorithm() {}


  void setMETCalibrator(METCalibrator * calibrator) {calibrator_ = calibrator;}

  CompositePtrCandidateT1T2MEt<T1,T2> buildCompositePtrCandidate(const T1Ptr leg1, 
								 const T2Ptr leg2,
 								 const JetPtrVector& pfJets, 
								 const reco::CandidatePtr met,
								 const reco::GenParticleCollection* genParticles) {
    CompositePtrCandidateT1T2MEt<T1,T2> compositePtrCandidate(leg1, leg2, met);
    
    if ( leg1.isNull() || leg2.isNull() ) {
      edm::LogError ("CompositePtrCandidateT1T2MEtAlgorithm") << " Pointers to visible Decay products invalid !!";
      return compositePtrCandidate;
    }




//--- compute quantities that are independent of MET	
    compositePtrCandidate.setCharge(leg1->charge() + leg2->charge());
    compositePtrCandidate.setP4Vis(leg1->p4() + leg2->p4());
    compositePtrCandidate.setDR12(reco::deltaR(leg1->p4(), leg2->p4()));
    compositePtrCandidate.setDPhi12(TMath::Abs(normalizedPhi(leg1->phi() - leg2->phi())));
    compositePtrCandidate.setVisEtaMin(TMath::Min(leg1->eta(), leg2->eta()));
    compositePtrCandidate.setVisEtaMax(TMath::Max(leg1->eta(), leg2->eta()));



    ///Do Jet studies
    //cross clean jets from legs 
    JetPtrVector cleanedJets;

    //Use jets >30 for Phil's CALIBRATOR. Change it if you need different
    int njets30=0;
    for(unsigned int i=0;i<pfJets.size();++i) 
      if(reco::deltaR(pfJets.at(i)->p4(),leg1->p4())>0.5 && reco::deltaR(pfJets.at(i)->p4(),leg2->p4())>0.5) {
	cleanedJets.push_back(pfJets.at(i));
	if(pfJets.at(i)->pt()>30)
	  njets30++;
    }

    //sort them by Pt
    sortRefVectorByPt(cleanedJets);
    int nJets = cleanedJets.size();



    //calibrate the MET
    reco::Candidate::LorentzVector correctedMET = met->p4();
    edm::Ptr<reco::MET> metPtr(met);
    if(calibrator_!=0)
      correctedMET = calibrator_->calibrate(met->p4(),leg1->p4(),leg2->p4(),genParticles,njets30);
    
    
    //--- compute quantities that do dependent on MET
    if ( met.isNonnull() ) {
      compositePtrCandidate.setCalibratedMET(correctedMET);
      compCollinearApprox(compositePtrCandidate, leg1->p4(), leg2->p4(), correctedMET.px(), correctedMET.py());
      compositePtrCandidate.setP4CDFmethod(compP4CDFmethod(leg1->p4(), leg2->p4(), correctedMET.px(), correctedMET.py()));
      compositePtrCandidate.setMt12MET(compMt(leg1->p4(), leg2->p4(), correctedMET.px(), correctedMET.py()));    
      compositePtrCandidate.setMt1MET(compMt(leg1->p4(), correctedMET.px(), correctedMET.py()));
      compositePtrCandidate.setMt2MET(compMt(leg2->p4(), correctedMET.px(), correctedMET.py()));
      compositePtrCandidate.setDPhi1MET(TMath::Abs(normalizedPhi(leg1->phi() - correctedMET.phi())));
      compositePtrCandidate.setDPhi2MET(TMath::Abs(normalizedPhi(leg2->phi() - correctedMET.phi())));
      compositePtrCandidate.setCovMatrix( metPtr->getSignificanceMatrix() );
      compZeta(compositePtrCandidate, leg1->p4(), leg2->p4(), correctedMET.px(), correctedMET.py()); 
      compProjMET(compositePtrCandidate, leg1->p4(), leg2->p4(), correctedMET);
    } else {
      compositePtrCandidate.setCollinearApproxQuantities(reco::Candidate::LorentzVector(0,0,0,0), -1, -1, false);
    }
 
    //--- compute gen. level quantities
    if ( genParticles ) {
      compGenQuantities(compositePtrCandidate, genParticles);
    }
    
//--- set compositePtr four-momentum
//    (depending on recoMode configuration parameter)
    if ( recoMode_ == "collinearApprox" ) {
      if ( met.isNonnull() ) {
        compositePtrCandidate.setP4(compositePtrCandidate.p4CollinearApprox());
      } else {
        edm::LogError ("buildCompositePtrCandidate") << " Failed to set four-momentum:"
						     << " recoMode = " << recoMode_ << " requires MET pointer to be valid !!";
      }
    } else if ( recoMode_ == "cdfMethod" ) {
      if ( met.isNonnull() ) {
	compositePtrCandidate.setP4(compositePtrCandidate.p4CDFmethod());
      } else {
	edm::LogError ("buildCompositePtrCandidate") << " Failed to set four-momentum:"
						     << " recoMode = " << recoMode_ << " requires MET pointer to be valid !!";
      }
    } else if ( recoMode_ == "" ) {
      compositePtrCandidate.setP4(compositePtrCandidate.p4Vis());
    } else {
      edm::LogError ("buildCompositePtrCandidate") << " Failed to set four-momentum:"
						   << " recoMode = " << recoMode_ << " undefined !!";
    }  



    //Set recoil values
    reco::Candidate::LorentzVector recoilTMP = -leg1->p4() - leg2->p4() - correctedMET;
    reco::Candidate::LorentzVector recoil_(recoilTMP.px(),recoilTMP.py(),0.0,sqrt(recoilTMP.px()*recoilTMP.px()+recoilTMP.py()*recoilTMP.py()));
    compositePtrCandidate.setRecoil(recoil_);
    compositePtrCandidate.setRecoilDPhi(fabs(normalizedPhi(compositePtrCandidate.p4Vis().phi() - recoil_.phi())));
 
    


    //Calculate HT
    double ht = leg1->pt()+leg2->pt();
    for(int k=0;k<nJets;++k)
      ht+=cleanedJets.at(k)->pt();


    if(nJets>1) 
      compositePtrCandidate.setJJVariables((cleanedJets.at(0)->p4()+cleanedJets.at(1)->p4()).M(),(cleanedJets.at(0)->p4()+cleanedJets.at(1)->p4()).pt());
    else
      compositePtrCandidate.setJJVariables(0.0,0.0);

    compositePtrCandidate.setJetVariables(cleanedJets,ht);
    computeVBFVariables(compositePtrCandidate,leg1->p4(),leg2->p4(),cleanedJets,correctedMET);
    return compositePtrCandidate;
  }

 private: 
  
  void compGenQuantities(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate, const reco::GenParticleCollection* genParticles)
  {
    const reco::GenParticle* genLeg1 = findGenParticle(compositePtrCandidate.leg1()->p4(), *genParticles, 0.5, -1);
    if ( genLeg1 ) {
      //std::cout << "genLeg1: Pt = " << genLeg1->pt() << ", eta = " << genLeg1->eta() << "," 
      //	  << " phi = " << genLeg1->phi()*180./TMath::Pi() << std::endl;
      compositePtrCandidate.setP4Leg1gen(genLeg1->p4());
      compositePtrCandidate.setP4VisLeg1gen(getVisMomentum(genLeg1, genParticles));
      compositePtrCandidate.setPdg1(genLeg1->pdgId());

    }
    
    const reco::GenParticle* genLeg2 = findGenParticle(compositePtrCandidate.leg2()->p4(), *genParticles, 0.5, -1);
    if ( genLeg2 ) {
      //std::cout << "genLeg2: Pt = " << genLeg2->pt() << ", eta = " << genLeg2->eta() << "," 
      //	  << " phi = " << genLeg2->phi()*180./TMath::Pi() << std::endl;
      compositePtrCandidate.setP4Leg2gen(genLeg2->p4());
      compositePtrCandidate.setP4VisLeg2gen(getVisMomentum(genLeg2, genParticles));
      compositePtrCandidate.setPdg2(genLeg2->pdgId());
    }
  }
  void compCollinearApprox(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
			   const reco::Candidate::LorentzVector& leg1,
			   const reco::Candidate::LorentzVector& leg2,
			   double metPx, double metPy)
  {
    double x1_numerator = leg1.px()*leg2.py() - leg2.px()*leg1.py();
    double x1_denominator = leg2.py()*(leg1.px() + metPx) - leg2.px()*(leg1.py() + metPy);
    double x1 = ( x1_denominator != 0. ) ? x1_numerator/x1_denominator : -1.;
    //std::cout << "x1 = " << x1 << std::endl;
    bool isX1withinPhysRange = true;
    double x1phys = getPhysX(x1, isX1withinPhysRange);

    double x2_numerator = x1_numerator;
    double x2_denominator = leg1.px()*(leg2.py() + metPy) - leg1.py()*(leg2.px() + metPx);
    double x2 = ( x2_denominator != 0. ) ? x2_numerator/x2_denominator : -1.;
    //std::cout << "x2 = " << x2 << std::endl;
    bool isX2withinPhysRange = true;
    double x2phys = getPhysX(x2, isX2withinPhysRange);

    if ( x1phys != 0. && x2phys != 0. ) {
      reco::Candidate::LorentzVector p4 = leg1/x1phys + leg2/x2phys;
      compositePtrCandidate.setCollinearApproxQuantities(p4, x1, x2, isX1withinPhysRange && isX2withinPhysRange);
    } else {
      compositePtrCandidate.setCollinearApproxQuantities(reco::Candidate::LorentzVector(0,0,0,0), x1, x2, false);
    }
  }

  class refVectorPtSorter {
  public:
    refVectorPtSorter(const JetPtrVector vec)
      {
	vec_ = vec;
      }

    refVectorPtSorter()
      {
      }


    ~refVectorPtSorter()
      {}

    bool operator()(size_t a , size_t b) {
      return (vec_.at(a)->pt() > vec_.at(b)->pt());
    }
  private:
    JetPtrVector vec_;
  };


  void sortRefVectorByPt(JetPtrVector& vec)
  {
    std::vector<size_t> indices;
    indices.reserve(vec.size());
    for(unsigned int i=0;i<vec.size();++i)
      indices.push_back(i);
    
    refVectorPtSorter sorter(vec);
    std::sort(indices.begin(),indices.end(),sorter);
        
    JetPtrVector sorted;
    sorted.reserve(vec.size());
    
    for(unsigned int i=0;i<indices.size();++i)
      sorted.push_back(vec.at(indices.at(i)));

    vec = sorted;
  }



  void compZeta(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
		const reco::Candidate::LorentzVector& leg1,
		const reco::Candidate::LorentzVector& leg2,
		double metPx, double metPy)
  {
    //std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::compZeta>:" << std::endl;

    double leg1x = cos(leg1.phi());
    double leg1y = sin(leg1.phi());
    double leg2x = cos(leg2.phi());
    double leg2y = sin(leg2.phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    //std::cout << " leg1Phi = " << leg1.phi()*180./TMath::Pi() << std::endl;
    //std::cout << " leg2Phi = " << leg2.phi()*180./TMath::Pi() << std::endl;

    //std::cout << " zetaX = " << zetaX << std::endl;
    //std::cout << " zetaY = " << zetaY << std::endl;

    //std::cout << " zetaPhi = " << normalizedPhi(atan2(zetaY, zetaX))*180./TMath::Pi() << std::endl;

    double visPx = leg1.px() + leg2.px();
    double visPy = leg1.py() + leg2.py();
    double pZetaVis = visPx*zetaX + visPy*zetaY;

    //std::cout << " visPx = " << visPx << std::endl;
    //std::cout << " visPy = " << visPy << std::endl;

    double px = visPx + metPx;
    double py = visPy + metPy;
    double pZeta = px*zetaX + py*zetaY;

    //std::cout << " metPhi = " << normalizedPhi(atan2(metPy, metPx))*180./TMath::Pi() << std::endl;
    
    if ( verbosity_ ) {
      std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::compZeta>:" << std::endl;
      std::cout << " pZetaVis = " << pZetaVis << std::endl;
      std::cout << " pZeta = " << pZeta << std::endl;
    }

    //assert(pZetaVis >= 0.);

    compositePtrCandidate.setPzeta(pZeta);
    compositePtrCandidate.setPzetaVis(pZetaVis);
  }
  reco::Candidate::LorentzVector compP4CDFmethod(const reco::Candidate::LorentzVector& leg1, 
						 const reco::Candidate::LorentzVector& leg2, 
						 double metPx, double metPy)
  {
    double px = leg1.px() + leg2.px() + metPx;
    double py = leg1.py() + leg2.py() + metPy;
    double pz = leg1.pz() + leg2.pz();
    double e = leg1.energy() + leg2.energy() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    reco::Candidate::LorentzVector p4(px, py, pz, e);
    return p4;
  }
  double compMt(const reco::Candidate::LorentzVector& leg1, 
		const reco::Candidate::LorentzVector& leg2, 
		double metPx, double metPy)
  {
    double px = leg1.px() + leg2.px() + metPx;
    double py = leg1.py() + leg2.py() + metPy;
    double et = leg1.Et() + leg2.Et() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
//      edm::LogWarning ("compMt") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }
  double compMt(const reco::Candidate::LorentzVector& visParticle, 
		double metPx, double metPy)
  {
    double px = visParticle.px() + metPx;
    double py = visParticle.py() + metPy;
    double et = visParticle.Et() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
//      edm::LogWarning ("compMt") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }

  void compProjMET(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
  		const reco::Candidate::LorentzVector& leg1,
  		const reco::Candidate::LorentzVector& leg2, 
  		const reco::Candidate::LorentzVector& METP4)
  {
  	double pi = 3.1415926;
  	double PMET = 0;
  	double dPhi1 = leg1.phi() - METP4.phi();
  	double dPhi2 = leg2.phi() - METP4.phi();
  	if (dPhi1 < 0) dPhi1 = -dPhi1;
  	if (dPhi2 < 0) dPhi2 = -dPhi2;
  	if (dPhi1 > pi) {
		dPhi1 = 2 * pi - dPhi1;
  	}
  	if (dPhi2 > pi) {
		dPhi2 = 2 * pi - dPhi2;
  	}
  	if(dPhi1 < dPhi2 && dPhi1 < pi/2){
  		PMET = TMath::Sqrt(pow((METP4.px()*leg1.px()*leg1.px()+METP4.py()*leg1.py()*leg1.px())/(leg1.pt()*leg1.pt()),2)
  							+pow((METP4.px()*leg1.px()*leg1.py()+METP4.py()*leg1.py()*leg1.py())/(leg1.pt()*leg1.pt()),2));
  	}
  	else if(dPhi2 < dPhi1 && dPhi2 < pi/2){
  		PMET = TMath::Sqrt(pow((METP4.px()*leg2.px()*leg2.px()+METP4.py()*leg2.py()*leg2.px())/(leg2.pt()*leg2.pt()),2)
  							+pow((METP4.px()*leg2.px()*leg2.py()+METP4.py()*leg2.py()*leg2.py())/(leg2.pt()*leg2.pt()),2));
  	}
  	else
  	{
  		PMET = METP4.pt();
  	}
  	
  	compositePtrCandidate.setProjMET(PMET);
  
  }



  //Complicated approach
  //  void computeVBFVariables(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,const JetPtrVector& jets) {
  //    double deta=0.0;
  //    double mass=0.0;
  //    double gap=0.0;
  //    for( int i=0;i<(int)jets.size()-1;++i)
  //      for(int j=i+1;j<(int)jets.size();++j) {
  //	deta =fabs(jets.at(i)->eta()-jets.at(j)->eta()); 
  //	  if(deta>2.0&&jets.at(i)->eta()*jets.at(j)->eta()<0)
  //	    {
  //	      mass = (jets.at(i)->p4()+jets.at(j)->p4()).M();
  //	      float minEta = std::min(jets.at(i)->eta(),jets.at(j)->eta());
  //	      float maxEta = std::max(jets.at(i)->eta(),jets.at(j)->eta());
  //	      for(int k=0;k<(int)jets.size();++k)
  //		if(k!=i && k!=j)
  //		  if(jets.at(k)->eta()>minEta && jets.at(k)->eta()<maxEta)
  //		    gap+=jets.at(k)->pt();
  //	      break;
  //	    }
  //
  //	  
  //	  if(deta>2.0)
  //	    break;
  //	      
  //      }
  //
  //    if(deta<2.0) { deta=0.0; mass=0.0; gap=0.0;}
  //    compositePtrCandidate.setVBFVariables(mass,deta,gap);
  //  }


  //Simple approach -> Just highest 2 jets 

  void computeVBFVariables(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
  							const reco::Candidate::LorentzVector& leg1,
			   				const reco::Candidate::LorentzVector& leg2,
							const JetPtrVector& jets,
							const reco::Candidate::LorentzVector& METP4) {
    double deta=-1.0;
    double mass=-1.0;
    double dphi=-10.0;
    int jets20=0;
    int jets30=0;

    float pt1=-1.0;
    float pt2=-1.0;
    float dijetpt=-1.0;
    float eta1=-10.;
    float eta2=-10.;
    float ditaupt=-1.0;
    float dphihj=-10.;
    float c1=-1.0;
    float c2=-1.0;
    float mva=-10.0;
	reco::Candidate::LorentzVector diTau = leg1+leg2+METP4;
	reco::Candidate::LorentzVector diTauVis = leg1+leg2;
	reco::Candidate::LorentzVector diJet;
	
      if(jets.size()>1) {
		  diJet = jets.at(0)->p4()+jets.at(1)->p4() ;
		  deta =fabs(jets.at(0)->eta()-jets.at(1)->eta()); 
		  dphi = fabs(deltaPhi(jets.at(0)->phi(), jets.at(1)->phi()));
		  mass = (jets.at(0)->p4()+jets.at(1)->p4()).M();
		  dijetpt = diJet.pt();
		  ditaupt = diTau.pt();
		  dphihj = fabs(deltaPhi(diTau.phi(), diJet.phi()));
		  c1 = min(fabs(diTauVis.Eta() - jets.at(0)->eta()), fabs(diTauVis.Eta() - jets.at(1)->eta()));
		  c2 = diTauVis.pt();
		  float maxEta = std::max(jets.at(0)->eta(),jets.at(1)->eta());
		  float minEta = std::min(jets.at(0)->eta(),jets.at(1)->eta());
	
		  pt1 = jets.at(0)->pt();
		  pt2 = jets.at(1)->pt();
		  eta1 = jets.at(0)->eta();
		  eta2 = jets.at(1)->eta();
		  
		  
		  mva = VBFMVA_.calcVBFMVA(mass, deta, dphi, ditaupt, dijetpt, dphihj, c1, c2);
		  
	
		  for(int k=2;k<(int)jets.size();++k) {
			if(jets.at(k)->pt()>20&&jets.at(k)->eta()>minEta&&jets.at(k)->eta()<maxEta) 
			  jets20++;
	
			if(jets.at(k)->pt()>30&&jets.at(k)->eta()>minEta&&jets.at(k)->eta()<maxEta) 
			  jets30++;
	
		  }

	}
      compositePtrCandidate.setVBFVariables(mass,deta,dphi,jets20,jets30,pt1,pt2,dijetpt,ditaupt,eta1,eta2,dphihj,c1,c2,mva);
  }
  




  std::string recoMode_;
  int verbosity_;
  VBFMVA VBFMVA_;
  

  METCalibrator *calibrator_;



 


};

#endif 

