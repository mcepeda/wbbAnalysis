// This only works for W+1Jet
// T&P numbers are from Isobel, to be revised


#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include "TFile.h"
#include "TROOT.h"
#include "TKey.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h> 
#include "TMath.h" 
#include <limits>
#include "RooHist.h"
#include "TRandom.h"
#include <CLHEP/Random/RandGaussQ.h>
#include <CLHEP/Random/Random.h>
#include <TLorentzVector.h>
   
void readdir(TDirectory *dir,optutl::CommandLineParser parser); 
float lookupScale(float etaEle,float ptEle);

int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("branchSMEARED_PT_1",optutl::CommandLineParser::kString,"Smeared Pt 1","ptEle1Sm_1");
   parser.addOption("branchSMEARED_PT_2",optutl::CommandLineParser::kString,"Smeared Pt 2","ptEle2Sm_1");
   parser.addOption("branchSMEARED_MT",optutl::CommandLineParser::kString,"Smeared Mt (Lead)","MtLead_1");
   parser.addOption("branchSMEARED_Mass",optutl::CommandLineParser::kString,"Smeared Mass","MassSM_1");

   parser.addOption("MC",optutl::CommandLineParser::kInteger,"MC?",0);
   parser.addOption("Gaussian",optutl::CommandLineParser::kDouble,"Gaussian?",1.0000);
   parser.addOption("ScaleMC",optutl::CommandLineParser::kDouble,"ScaleMC?",1.02);

   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   printf("Now weighting: %s\n",parser.stringValue("outputFile").c_str());
   //int topType = parser.integerValue("typeTop");
   readdir(f,parser);

   f->Close();
} 


void readdir(TDirectory *dir,optutl::CommandLineParser parser) 
{
   int MC=parser.integerValue("MC");
        TRandom rnd;
   float GAUSSIAN_MC=parser.doubleValue("Gaussian");
   float scaleMC=parser.doubleValue("ScaleMC");

                        double REGION[13]={-2.5,-2.0,-1.5,-1.2,-0.8,-0.4,0.,0.4,0.8,1.2, 1.5,2, 2.5};
                        double GAUSSIAN_MC_BINS[12]={1.95,1.56,1.20,0.77,0.43,0.53,0.46,0.47,0.74,1.20,1.49,2.03};



  //read directory
  TDirectory *dirsav = gDirectory;
  TIter next(dir->GetListOfKeys());
  TKey *key;
  while((key = (TKey*)next())){
    printf("Found key=%s \n",key->GetName());
    TObject *obj = key->ReadObj();
    if(obj->IsA()->InheritsFrom(TDirectory::Class())){
      dir->cd(key->GetName());
      string NAME=key->GetName();
//      if(NAME!="eleNuJetEventTree" && NAME!="eleNuJetJetEventTree") continue;
          size_t trigPath = NAME.find("eleNu");
          if ( trigPath != 0) continue;
      printf("Found Electron Tree! %s \n",key->GetName());
      TDirectory *subdir = gDirectory;
      readdir(subdir,parser);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())){
      TTree *t = (TTree*)obj;
      
      //get muon pt,eta
      float ptEle1;
      float etaEle1;
      float phiEle1;
      float ptEle2;
      float etaEle2;
      float phiEle2;

      float metphi;
      float met;
      UInt_t RUN;

      float SMEARED_PT_1; float SMEARED_PT_2;	
      float SMEARED_MT;
      float SMEARED_Mass;

      t->SetBranchAddress("ptEle2",&ptEle2);
      t->SetBranchAddress("etaEle2",&etaEle2);
      t->SetBranchAddress("phiEle2",&phiEle2);

      t->SetBranchAddress("ptEle1",&ptEle1);
      t->SetBranchAddress("etaEle1",&etaEle1);
      t->SetBranchAddress("phiEle1",&phiEle1);

      t->SetBranchAddress("metphi",&metphi);
      t->SetBranchAddress("met",&met);
      t->SetBranchAddress("RUN",&RUN);

      TBranch *newBranchSMEARED_PT_1 = t->Branch(parser.stringValue("branchSMEARED_PT_1").c_str(),&SMEARED_PT_1,(parser.stringValue("branchSMEARED_PT_1")+"/F").c_str());
      TBranch *newBranchSMEARED_PT_2 = t->Branch(parser.stringValue("branchSMEARED_PT_2").c_str(),&SMEARED_PT_2,(parser.stringValue("branchSMEARED_PT_2")+"/F").c_str());
      TBranch *newBranchSMEARED_MT = t->Branch(parser.stringValue("branchSMEARED_MT").c_str(),&SMEARED_MT,(parser.stringValue("branchSMEARED_MT")+"/F").c_str());
      TBranch *newBranchSMEARED_Mass = t->Branch(parser.stringValue("branchSMEARED_Mass").c_str(),&SMEARED_Mass,(parser.stringValue("branchSMEARED_Mass")+"/F").c_str());

      printf("Found tree -> weighting\n");
      printf("Entries %i \n",(int)t->GetEntries());
     
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
	  SMEARED_PT_1=-1, SMEARED_PT_2=-1, SMEARED_MT=-1, SMEARED_Mass=-1;

	if(MC!=1){
	if(i==0) cout<<"Scaling Data"<<endl;


	double corr=0;


                                if (RUN>=160431 && RUN<=167784) {
    if (etaEle1<1.445) {
      if (RUN>=160431 && RUN<=163869) {
            corr = +0.0047;
      } else if (RUN>=165071 && RUN<=165970) {
            corr = +0.0007;
      } else if (RUN>=165971 && RUN<=166502) {
            corr = -0.0003;
      } else if (RUN>=166503 && RUN<=166861) {
            corr = -0.0011;
      } else if (RUN>=166862 && RUN<=167784) {
            corr = -0.0014;
      }
    } else {
      if (RUN>=160431 && RUN<=163869) {
             corr = -0.0058;
      } else if (RUN>=165071 && RUN<=165970) {
             corr = -0.0249;
      } else if (RUN>=165971 && RUN<=166502) {
             corr = -0.0376;
      } else if (RUN>=166503 && RUN<=166861) {
             corr = -0.0450;
      } else if (RUN>=166862 && RUN<=167784) {
             corr = -0.0561;
      }
    }
      } else if (RUN>=1700053 && RUN <=172619) {
    if (fabs(etaEle1)<1.445) {
       corr = -0.0011;
    } else {
       corr = +0.0009;
    }
      } else if (RUN>=172620 && RUN <=175770) {
    if (fabs(etaEle1)<1.445) {
       corr = -0.0046;
    } else {
       corr = +0.0337;
        }
      } else if (RUN>=175860 && RUN<=177139) { // prompt-v1 corrections for 2011B [ 175860 - 177139 ]
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)>=1 ) corr = -0.0118;
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)<1 ) corr = -0.0034;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)>=2 ) corr = +0.0019;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)<2 ) corr = +0.0168;
      } else if (RUN>=177140 && RUN<=178421) { // prompt-v1 corrections for 2011B [ 177140 - 178421 ]
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)>=1 ) corr = -0.0129;
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)<1 ) corr = -0.0038;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)>=2 ) corr = +0.0049;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)<2 ) corr = +0.0257;
      } else if (RUN>=178424 && RUN<=180252) { // prompt-v1 corrections for 2011B [ 178424 - 180252 ]
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)>=1 ) corr = -0.0150;
        if (fabs(etaEle1)<1.445 && fabs(etaEle1)<1 ) corr = -0.0052;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)>=2 ) corr = +0.0009;
        if (fabs(etaEle1)>1.55 && fabs(etaEle1)<2 ) corr = +0.0353;
      }

                                double SCALE_PT_1=1.00/(1+corr);
				                                SMEARED_PT_1=ptEle1*SCALE_PT_1;

	corr=0;
                                if (RUN>=160431 && RUN<=167784) {
    if (etaEle2<1.445) {
      if (RUN>=160431 && RUN<=163869) {
            corr = +0.0047;
      } else if (RUN>=165071 && RUN<=165970) {
            corr = +0.0007;
      } else if (RUN>=165971 && RUN<=166502) {
            corr = -0.0003;
      } else if (RUN>=166503 && RUN<=166861) {
            corr = -0.0011;
      } else if (RUN>=166862 && RUN<=167784) {
            corr = -0.0014;
      }
    } else {
      if (RUN>=160431 && RUN<=163869) {
             corr = -0.0058;
      } else if (RUN>=165071 && RUN<=165970) {
             corr = -0.0249;
      } else if (RUN>=165971 && RUN<=166502) {
             corr = -0.0376;
      } else if (RUN>=166503 && RUN<=166861) {
             corr = -0.0450;
      } else if (RUN>=166862 && RUN<=167784) {
             corr = -0.0561;
      }
    }
      } else if (RUN>=1700053 && RUN <=172619) {
    if (fabs(etaEle2)<1.445) {
       corr = -0.0011;
    } else {
       corr = +0.0009;
    }
      } else if (RUN>=172620 && RUN <=175770) {
    if (fabs(etaEle2)<1.445) {
       corr = -0.0046;
    } else {
       corr = +0.0337;
        }
      } else if (RUN>=175860 && RUN<=177139) { // prompt-v1 corrections for 2011B [ 175860 - 177139 ]
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)>=1 ) corr = -0.0118;
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)<1 ) corr = -0.0034;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)>=2 ) corr = +0.0019;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)<2 ) corr = +0.0168;
      } else if (RUN>=177140 && RUN<=178421) { // prompt-v1 corrections for 2011B [ 177140 - 178421 ]
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)>=1 ) corr = -0.0129;
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)<1 ) corr = -0.0038;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)>=2 ) corr = +0.0049;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)<2 ) corr = +0.0257;
      } else if (RUN>=178424 && RUN<=180252) { // prompt-v1 corrections for 2011B [ 178424 - 180252 ]
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)>=1 ) corr = -0.0150;
        if (fabs(etaEle2)<1.445 && fabs(etaEle2)<1 ) corr = -0.0052;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)>=2 ) corr = +0.0009;
        if (fabs(etaEle2)>1.55 && fabs(etaEle2)<2 ) corr = +0.0353;
      }

                                double SCALE_PT_2=1.00/(1+corr);
				SMEARED_PT_2=ptEle2*SCALE_PT_2;

          if(i%100000==1) printf("\t\t RUN %4.0d SCALE1=%4.2f  SCALE2=%4.2f \n", RUN, SCALE_PT_1, SCALE_PT_2);

                        }

			else if(MC==1){

      //                  int eta_bin_forScale=0;
     //                   double SCALE_PT_1=1.0;  double SCALE_PT_2=0.0;
				double smear=0;	
                                if (  fabs(etaEle1)<1 ) smear =GAUSSIAN_MC*0.74;
                                else if (  fabs(etaEle1)>=1&& fabs(etaEle1)<1.445 ) smear = GAUSSIAN_MC*1.41;
                                else if (  fabs(etaEle1)>1.55 && fabs(etaEle1)<2 ) smear = GAUSSIAN_MC*2.68;
                                else if (  fabs(etaEle1)>=2) smear = GAUSSIAN_MC*2.93;

				   SMEARED_PT_1=ptEle1*rnd.Gaus(1.,smear/100);

				   SMEARED_PT_1=SMEARED_PT_1*scaleMC; 				

				smear=0;
                                if (  fabs(etaEle2)<1 ) smear =GAUSSIAN_MC* 0.74;
                                else if (  fabs(etaEle2)>=1&& fabs(etaEle2)<1.445 ) smear = GAUSSIAN_MC*1.41;
                                else if (  fabs(etaEle2)>1.55 && fabs(etaEle2)<2 ) smear = GAUSSIAN_MC*2.68;
                                else if (  fabs(etaEle2)>=2) smear = GAUSSIAN_MC*2.93;

                                   SMEARED_PT_2=ptEle2*rnd.Gaus(1.,smear/100);
			
				   SMEARED_PT_2=SMEARED_PT_2*scaleMC;	


                        // Energy Scale Corrections!

/*
                        int eta_bin_forScale_1=0,eta_bin_forScale_2=0;
                        for(int i=0; i<13; i++){
                                        if(etaEle1>REGION[i] && etaEle1<=REGION[i]) eta_bin_forScale_1=i;
                                        if(etaEle2>REGION[i] && etaEle2<=REGION[i]) eta_bin_forScale_2=i;
                        }
                                   double SMEAR_PT_1=GAUSSIAN_MC_BINS[eta_bin_forScale_1]*rnd.Gaus()*GAUSSIAN_MC;
                                   double SMEAR_PT_2=GAUSSIAN_MC_BINS[eta_bin_forScale_2]*rnd.Gaus()*GAUSSIAN_MC;

                        SMEARED_PT_1=ptEle1+SMEAR_PT_1;
                        SMEARED_PT_2=ptEle2+SMEAR_PT_2;

*/



			}



			SMEARED_MT=2*SMEARED_PT_1*met*(1-cos(phiEle1-metphi));
			SMEARED_MT=SMEARED_MT>0?sqrt(SMEARED_MT):0;

			TLorentzVector A,B,C;
			A.SetPtEtaPhiM(SMEARED_PT_1,etaEle1,phiEle1,0.105);
                        B.SetPtEtaPhiM(SMEARED_PT_2,etaEle2,phiEle2,0.105);
			C=A+B;
			SMEARED_Mass=C.M();


//			SMEARED_Mass=2*SMEARED_PT_1*SMEARED_PT_2*(sinh(etaEle1)*sinh(etaEle2)+cos(phiEle1-phiEle2));
//			SMEARED_Mass=SMEARED_Mass>0?sqrt(SMEARED_Mass):0;

			newBranchSMEARED_PT_1->Fill();
                        newBranchSMEARED_PT_2->Fill();
			newBranchSMEARED_MT->Fill();
                        newBranchSMEARED_Mass->Fill();


          if(i%100000==1) printf("\t\t Ele1 %4.0d Eta=%4.2f  Pt=%4.2f Phi=%4.2f Met=%4.2f MetPhi=%4.2f---> %4.2f %4.2f \n",i,etaEle1,ptEle1,phiEle1,met,metphi,SMEARED_PT_1,SMEARED_MT);
          if(i%100000==1) printf("\t\t Ele2 %4.0d Eta=%4.2f  Pt=%4.2f Phi=%4.2f Met=%4.2f MetPhi=%4.2f---> %4.2f %4.2f \n",i,etaEle2,ptEle2,phiEle2,met,metphi,SMEARED_PT_2,SMEARED_Mass);

	}
      
      printf("The End!\n");
      
      t->Write("",TObject::kOverwrite);
      return;     
    }
  }
} 

