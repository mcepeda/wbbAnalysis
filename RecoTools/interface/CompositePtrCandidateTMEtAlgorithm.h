#ifndef UWAnalysis_RecoTools_CompositePtrCandidateTMEtAlgorithm_h
#define UWAnalysis_RecoTools_CompositePtrCandidateTMEtAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "UWAnalysis/DataFormats/interface/CompositePtrCandidateTMEt.h"

#include "UWAnalysis/RecoTools/interface/candidateAuxFunctions.h"
#include "UWAnalysis/RecoTools/interface/METCalibrator.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 

#include <TMath.h>

template<typename T>
class CompositePtrCandidateTMEtAlgorithm 
{
  typedef edm::Ptr<T> TPtr;
  typedef edm::Ptr<pat::MET> METPtr;
  typedef edm::Ptr<pat::Jet> JetPtr;
  typedef std::vector<JetPtr> JetPtrVector;

 public:

  CompositePtrCandidateTMEtAlgorithm()
  {
    verbosity_ = 0;
  }
  ~CompositePtrCandidateTMEtAlgorithm() {}

  void setMETCalibrator(METCalibrator * calibrator) {calibrator_ = calibrator;}

  CompositePtrCandidateTMEt<T> buildCompositePtrCandidate(const TPtr visDecayProducts, 
							  METPtr met,
							  JetPtrVector pfJets,
							  edm::View<T> visProductCollection,
							  const reco::GenParticleCollection* genParticles)
  {
    CompositePtrCandidateTMEt<T> compositePtrCandidate(visDecayProducts, met);
  
    if ( visDecayProducts.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to visible Decay products invalid !!";
      return compositePtrCandidate;
    }

    if ( met.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to missing transverse momentum invalid !!";
      return compositePtrCandidate;
    }

    reco::Candidate::LorentzVector recoil(-met->px()-visDecayProducts->px(),-met->py()-visDecayProducts->py(),0.0,sqrt(pow(met->px()+visDecayProducts->px(),2)+pow(met->py()+visDecayProducts->py(),2)));

    compositePtrCandidate.setRecoil(recoil);
    compositePtrCandidate.setRecoilDPhi(deltaPhi(recoil.phi(),visDecayProducts->phi()));
    compositePtrCandidate.setCharge(visDecayProducts->charge());
    compositePtrCandidate.setMt(compMt(visDecayProducts->p4(), met->px(), met->py()));
    compositePtrCandidate.setDPhi(TMath::Abs(normalizedPhi(visDecayProducts->phi() - met->phi())));
    compositePtrCandidate.setPx(visDecayProducts->px() + met->px());
    compositePtrCandidate.setPy(visDecayProducts->py() + met->py());
    compositePtrCandidate.setPt(sqrt( (visDecayProducts->py() + met->py())*(visDecayProducts->py() + met->py()) + (visDecayProducts->px() + met->px())*(visDecayProducts->px() + met->px())));


    //calibrate the MET
    reco::Candidate::LorentzVector correctedMET = met->p4();
    if(calibrator_!=0)
      correctedMET = calibrator_->calibrate(met->p4(),visDecayProducts->p4(),visDecayProducts->p4(),genParticles);

      compositePtrCandidate.setCalibratedMET(correctedMET);
      compositePtrCandidate.setCorMt(compMt(visDecayProducts->p4(), correctedMET.px(), correctedMET.py()));
      compositePtrCandidate.setCorPt(sqrt( (visDecayProducts->py() + correctedMET.py())*(visDecayProducts->py() + correctedMET.py()) + (visDecayProducts->px() + correctedMET.px())*(visDecayProducts->px() + correctedMET.px())));
      compositePtrCandidate.setCorPx(visDecayProducts->px() + correctedMET.px());
      compositePtrCandidate.setCorPy(visDecayProducts->py() + correctedMET.py());
    //Jets
    JetPtrVector cleanedJets;
    for(unsigned int i=0;i<pfJets.size();++i)
      if(reco::deltaR(pfJets.at(i)->p4(),visDecayProducts->p4())>0.15 )
	cleanedJets.push_back(pfJets.at(i));


   // Try to get the neutrino pz        
    double pznu_=-1; double otherSol_=-1;
    bool isComplex_=false;
//    computeMETPz(compositePtrCandidate, met->px(), met->py(),pznu_,otherSol_,isComplex_);
     computeMETPz(visDecayProducts->p4(), correctedMET.px(), correctedMET.py(),pznu_,otherSol_,isComplex_);
    compositePtrCandidate.setMetPz(pznu_);
    compositePtrCandidate.setMetPzB(otherSol_);
    compositePtrCandidate.setIsComplex(isComplex_);
//      cout<<pznu_<<endl;
    double PX_TOTAL=visDecayProducts->px() + met->px();
    double PY_TOTAL=visDecayProducts->py() + met->py();
    double PZ_TOTAL=visDecayProducts->pz() + pznu_;
    double PZ_TOTAL_2=visDecayProducts->pz() + otherSol_;
    double EN_TOTAL=sqrt(80.4*80.4+PX_TOTAL*PX_TOTAL+PY_TOTAL*PY_TOTAL+PZ_TOTAL*PZ_TOTAL);
    double EN_TOTAL_2=sqrt(80.4*80.4+PX_TOTAL*PX_TOTAL+PY_TOTAL*PY_TOTAL+PZ_TOTAL_2*PZ_TOTAL_2);
    compositePtrCandidate.setWP4(math::XYZTLorentzVector(PX_TOTAL,PY_TOTAL,PZ_TOTAL,EN_TOTAL));
    compositePtrCandidate.setWP4B(math::XYZTLorentzVector(PX_TOTAL,PY_TOTAL,PZ_TOTAL_2,EN_TOTAL_2));

    //sort them by Pt
    sortRefVectorByPt(cleanedJets);
    unsigned int nJets = cleanedJets.size();
 
   //find the nearest jet to leg1
    double ht = visDecayProducts->pt();
    for(unsigned int k=0;k<nJets;++k)
      ht+=cleanedJets.at(k)->pt();


    compositePtrCandidate.setJetValues(cleanedJets,ht);


    //--- compute gen. level quantities
    if ( genParticles ) {
      compGenQuantities(compositePtrCandidate, genParticles);
      compTrueMet(compositePtrCandidate, genParticles);
      compositePtrCandidate.setGenMt(compMt(compositePtrCandidate.p4Leptongen(), compositePtrCandidate.trueMEx(),compositePtrCandidate.trueMEy()));
    }


    return compositePtrCandidate;
  }

 private: 

 void computeMETPz(const reco::Candidate::LorentzVector& visParticle, double MetPx, double MetPy, double& pznu, double& otherSol, bool& isComplex){
	// WARNING: This assumes that the lepton+MET is a W boson
	// For coherence, this should be moved out of this general class (I am lazy right now) 
        double M_W  = 80.4;
	double M_mu =  0.105;
	
        double pxmu = visParticle.px();
        double pymu = visParticle.py();
        double pzmu = visParticle.pz();
        double emu = visParticle.energy();
        double pmu = visParticle.M();
/*        double a = M_W*M_W - 2.0*pxmu*MetPx - 2.0*pymu*MetPy;
	double A=4*pzmu*pzmu - 4*emu*emu;
 	double B=4*a*pzmu;
	double C=a*a-4*emu*emu*(MetPx*MetPx+MetPy*MetPy); 	
*/

	double alpha=(M_W*M_W-M_mu*M_mu)/2/emu+(pxmu*MetPx+pymu*MetPy)/emu;
	double A=pzmu*pzmu/emu/emu-1;
	double B=2*alpha*pzmu/emu;
	double C=alpha*alpha-(MetPx*MetPx+MetPy*MetPy);

	// EQUIVALENT TO:
	// pz=(Landa*pzmu/ptmu/ptmu) +(-) 1/ptmu/ptmu*sqrt( landa*landa*pzmu - ptmu*ptmu*(emu*emu*met*met-landa*landa))
	// landa = M_W*M_W/2+ ptmu x met (scalar product) = alpha*emu;
	// From AN-11-229

        double tmproot = B*B - 4.0*A*C;

        if (tmproot<0) {
                isComplex= true;
                pznu = - B/(2*A); // take real part of complex roots
                otherSol = pznu;
                }
        else {
                        isComplex = false;
                        double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
                        double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);

                          //if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2; otherSol = tmpsol1;}
                          //else { pznu = tmpsol1; otherSol = tmpsol2; }
                                  if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pznu = tmpsol1; otherSol = tmpsol2; }
                                  else { pznu = tmpsol2; otherSol = tmpsol1; }
                        }


   }
 

  void compGenQuantities(CompositePtrCandidateTMEt<T>& compositePtrCandidate, const reco::GenParticleCollection* genParticles)
  {
    const reco::GenParticle* genLepton = findGenParticle(compositePtrCandidate.lepton()->p4(), *genParticles, 0.5, -1);
    if ( genLepton ) {
      compositePtrCandidate.setP4Leptongen(genLepton->p4());
      compositePtrCandidate.setP4VisLeptongen(getVisMomentum(genLepton, genParticles));
      compositePtrCandidate.setPdg(genLepton->pdgId());
    }
  }     

  void compTrueMet(CompositePtrCandidateTMEt<T>& compositePtrCandidate, const reco::GenParticleCollection* genParticles){
    double trueMEY  = 0.0;
    double trueMEX  = 0.0;;
    double trueMEZ  = 0.0;;
 
    for( unsigned i = 0; i < genParticles->size(); i++ ) {
    const reco::GenParticle& genpart = (*genParticles)[i];
      if( genpart.status() == 1 && fabs(genpart.eta()) < 5.0 ) { 
        if( std::abs(genpart.pdgId()) == 12 ||
            std::abs(genpart.pdgId()) == 14 ||
            std::abs(genpart.pdgId()) == 16  
          //  ||std::abs(genpart.pdgId()) < 7   || 
          //  std::abs(genpart.pdgId()) == 21 
		) {
          trueMEX += genpart.px();
          trueMEY += genpart.py();
          trueMEZ += genpart.pz();

        } 
      }
    }
    double true_met = sqrt( trueMEX*trueMEX + trueMEY*trueMEY );
    double true_phi = atan2(trueMEY,trueMEX);	

   compositePtrCandidate.setTrueMet(true_met);
   compositePtrCandidate.setTrueMetPhi(true_phi);
   compositePtrCandidate.setTrueMEy(trueMEY);
   compositePtrCandidate.setTrueMEx(trueMEX);
   compositePtrCandidate.setTrueMEz(trueMEZ);
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

  METCalibrator *calibrator_;


  int verbosity_;
};

#endif 

