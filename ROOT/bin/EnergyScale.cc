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

   
void readdir(TDirectory *dir,optutl::CommandLineParser parser); 
float lookupScale(float etaEle,float ptEle);

int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("branchSMEARED_PT",optutl::CommandLineParser::kString,"Smeared Pt","ptEleCOR");
   parser.addOption("branchSMEARED_MT",optutl::CommandLineParser::kString,"Smeared Mt","mtEleCOR");
   parser.addOption("Gaussian",optutl::CommandLineParser::kDouble,"Gaussian?",1.0);
   parser.addOption("ScaleMC",optutl::CommandLineParser::kDouble,"ScaleMC?",1.0);

   parser.addOption("MC",optutl::CommandLineParser::kInteger,"MC?",0);
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
   double ScaleGaussian=parser.doubleValue("Gaussian");
   double scaleMC=parser.doubleValue("ScaleMC");

        TRandom rnd;


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
      if(NAME!="eleNuJetEventTree" && NAME!="eleNuJetJetEventTree") continue;
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
      float ptEle;
      float etaEle;
      float phiEle;
      float metphi;
      float met;
      UInt_t RUN;
      float r9;	

      float SMEARED_PT;	
      float SMEARED_MT;

      t->SetBranchAddress("ptEle",&ptEle);
      t->SetBranchAddress("etaEle",&etaEle);
      t->SetBranchAddress("phiEle",&phiEle);
      t->SetBranchAddress("metphi",&metphi);
      t->SetBranchAddress("met",&met);
      t->SetBranchAddress("RUN",&RUN);
      t->SetBranchAddress("r9",&r9);

      TBranch *newBranchSMEARED_PT = t->Branch(parser.stringValue("branchSMEARED_PT").c_str(),&SMEARED_PT,(parser.stringValue("branchSMEARED_PT")+"/F").c_str());
      TBranch *newBranchSMEARED_MT = t->Branch(parser.stringValue("branchSMEARED_MT").c_str(),&SMEARED_MT,(parser.stringValue("branchSMEARED_MT")+"/F").c_str());

      printf("Found tree -> weighting\n");
      printf("Entries %i \n",(int)t->GetEntries());
     
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);

                        int eta_bin_forScale=0;
                        double SCALE_PT=1.0;  double SMEAR_PT=0.0;
				double corr=0.00;


                        if(MC==0) {

		if(i==0) cout<<"Weighting Data!!!"<<endl;


      if (RUN>=160431 && RUN<=167784) {
	if (fabs(etaEle)<1.45) {
	  if (RUN>=160431 && RUN<=163869) {
            if (r9>=0.94) corr = +0.0047;
            if (r9<0.94) corr = -0.0025;
	  } else if (RUN>=165071 && RUN<=165970) {
            if (r9>=0.94) corr = +0.0007;
            if (r9<0.94) corr = -0.0049;
	  } else if (RUN>=165971 && RUN<=166502) {
            if (r9>=0.94) corr = -0.0003;
            if (r9<0.94) corr = -0.0067;
	  } else if (RUN>=166503 && RUN<=166861) {
            if (r9>=0.94) corr = -0.0011;
            if (r9<0.94) corr = -0.0063;
	  } else if (RUN>=166862 && RUN<=167784) {
            if (r9>=0.94) corr = -0.0014;
            if (r9<0.94) corr = -0.0074;
	  } 
	} else if (fabs(etaEle)>1.56) {
	  if (RUN>=160431 && RUN<=163869) {
            if (r9>=0.94) corr = -0.0058;
            if (r9<0.94) corr = +0.0010;
	  } else if (RUN>=165071 && RUN<=165970) {
            if (r9>=0.94) corr = -0.0249;
            if (r9<0.94) corr = -0.0062;
	  } else if (RUN>=165971 && RUN<=166502) {
            if (r9>=0.94) corr = -0.0376;
            if (r9<0.94) corr = -0.0133;
	  } else if (RUN>=166503 && RUN<=166861) {
            if (r9>=0.94) corr = -0.0450;
            if (r9<0.94) corr = -0.0178;
	  } else if (RUN>=166862 && RUN<=167784) {
            if (r9>=0.94) corr = -0.0561;
            if (r9<0.94) corr = -0.0273;
	  } 
	}  
  
      } 
        if (RUN>=170053 && RUN <=172619) {
	if (fabs(etaEle)<1.45) {
	  if (r9>=0.94) corr = -0.0011;
	  if (r9<0.94) corr = -0.0067;
	} else if (fabs(etaEle)>1.56) {
	  if (r9>=0.94) corr = +0.0009;
	  if (r9<0.94) corr = -0.0046;
	}  
      } else if (RUN>=172620 && RUN <=175770) {
	if (fabs(etaEle)<1.45) {
	  if (r9>=0.94) corr = -0.0046;
	  if (r9<0.94) corr = -0.0104;
	} else if (fabs(etaEle)>1.56) {
	  if (r9>=0.94) corr = +0.0337;
	  if (r9<0.94) corr = +0.0250;
        }  
      } else if (RUN>=175860 && RUN<=177139) {                      // prompt-v1 corrections for 2011B [ 175860 - 177139 ]
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9<0.94) corr = -0.0228;
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9>=0.94) corr = -0.0118;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9<0.94) corr = -0.0075;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9>=0.94) corr = -0.0034;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9<0.94) corr = -0.0041;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9>=0.94) corr = +0.0019;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9<0.94) corr = +0.0147;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9>=0.94) corr = +0.0168;
      } else if (RUN>=177140 && RUN<=178421) {                      // prompt-v1 corrections for 2011B [ 177140 - 178421 ]
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9<0.94) corr = -0.0239;
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9>=0.94) corr = -0.0129;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9<0.94) corr = -0.0079;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9>=0.94) corr = -0.0038;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9<0.94) corr = -0.0011;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9>=0.94) corr = +0.0049;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9<0.94) corr = +0.0236;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9>=0.94) corr = +0.0257;
      } else if (RUN>=178424 && RUN<=180252) {                      // prompt-v1 corrections for 2011B [ 178424 - 180252 ]
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9<0.94) corr = -0.0260;
        if (fabs(etaEle)<1.45 && fabs(etaEle)>=1 and r9>=0.94) corr = -0.0150;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9<0.94) corr = -0.0094;
        if (fabs(etaEle)<1.45 && fabs(etaEle)<1 and r9>=0.94) corr = -0.0052;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9<0.94) corr = -0.0050;
        if (fabs(etaEle)>1.56 && fabs(etaEle)>=2 and r9>=0.94) corr = +0.0009;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9<0.94) corr = +0.0331;
        if (fabs(etaEle)>1.56 && fabs(etaEle)<2 and r9>=0.94) corr = +0.0353;
      }

			}

                                SCALE_PT=1.00/(1+corr);


                        if(MC==1)       {

				if(i==1) cout<<"Is MC!!"<<endl;
				double GAUSSIAN_MC=0;
				if(r9>=0.94){
			        if (  fabs(etaEle)<1 ) GAUSSIAN_MC = ScaleGaussian*0.74;
    				else if (  fabs(etaEle)>=1&& fabs(etaEle)<1.445 ) GAUSSIAN_MC = ScaleGaussian*1.41;
    				else if (  fabs(etaEle)>1.55 && fabs(etaEle)<2 ) GAUSSIAN_MC = ScaleGaussian*2.68;
    				else if (  fabs(etaEle)>=2) GAUSSIAN_MC = ScaleGaussian*2.93;
				}else{
                                if (  fabs(etaEle)<1 ) GAUSSIAN_MC = ScaleGaussian*0.96;
                                else if (  fabs(etaEle)>=1&& fabs(etaEle)<1.445 ) GAUSSIAN_MC = ScaleGaussian*1.96;
                                else if (  fabs(etaEle)>1.55 && fabs(etaEle)<2 ) GAUSSIAN_MC = ScaleGaussian*2.79;
                                else if (  fabs(etaEle)>=2) GAUSSIAN_MC = ScaleGaussian*3.01;
				}

				   SCALE_PT=rnd.Gaus(1.,GAUSSIAN_MC/100);
			 }

                        SMEARED_PT=ptEle*SCALE_PT*scaleMC;

			SMEARED_MT=2*SMEARED_PT*met*(1-cos(phiEle-metphi));
			SMEARED_MT=SMEARED_MT>0?sqrt(SMEARED_MT):0;

			newBranchSMEARED_PT->Fill();
			newBranchSMEARED_MT->Fill();

          if(i%100000==1) printf("\t\t %4.0d %4.0d r9=%4.2f Eta=%4.2f  Pt=%4.2f Phi=%4.2f Met=%4.2f MetPhi=%4.2f---> %4.2f %4.2f (%4.4f %4.4f)\n",i,RUN, r9, etaEle,ptEle,phiEle,met,metphi,SMEARED_PT,SMEARED_MT,corr, SCALE_PT);


	}
      
      printf("The End!\n");
      
      t->Write("",TObject::kOverwrite);
      return;     
    }
  }
} 

