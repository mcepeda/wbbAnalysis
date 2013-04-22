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


// HLT efficiencies 1 Jet 
   
void readdir(TDirectory *dir,optutl::CommandLineParser parser); 
float lookupScale(float etaEle,float ptEle);
float lookupSCtoRECO(float etaEle,float ptEle);
float lookupRECOtoWP(float etaEle,float ptEle);
float lookupSingleEle(float etaEle,float ptEle);
float lookupMT(float mt, float etaEle);

int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("branchHLTMT",optutl::CommandLineParser::kString,"HLTMT Weight","wtHLTMTEffKal");
   parser.addOption("branchHLT",optutl::CommandLineParser::kString,"HLTMT Weight","wtHLTEffKal");
   parser.addOption("branchIDISO",optutl::CommandLineParser::kString,"IDISOHLT Weight","wtIDISOEffKal");
   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   printf("Now weighting: %s\n",parser.stringValue("outputFile").c_str());
   //int topType = parser.integerValue("typeTop");
   readdir(f,parser);

   f->Close();
} 


void readdir(TDirectory *dir,optutl::CommandLineParser parser) 
{


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
      float ptEle;
      float etaEle;
      float mt;

      float weightIDISO=-1;      
      float weightHLTMT=-1;
      float weightHLT=-1;

      float isobel=-1;	

      t->SetBranchAddress("ptEle",&ptEle);
      t->SetBranchAddress("etaEle",&etaEle);
      t->SetBranchAddress("mt",&mt);

      TBranch *newBranchIDISO = t->Branch(parser.stringValue("branchIDISO").c_str(),&weightIDISO,(parser.stringValue("branchIDISO")+"/F").c_str());
      TBranch *newBranchHLTMT = t->Branch(parser.stringValue("branchHLTMT").c_str(),&weightHLTMT,(parser.stringValue("branchHLTMT")+"/F").c_str());
      TBranch *newBranchHLT = t->Branch(parser.stringValue("branchHLT").c_str(),&weightHLT,(parser.stringValue("branchHLT")+"/F").c_str());

      printf("Found tree -> weighting\n");
      printf("Entries %i \n",(int)t->GetEntries());
     
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
		  isobel = lookupScale(etaEle,ptEle);
	          weightIDISO=lookupRECOtoWP(etaEle,ptEle)*lookupSCtoRECO(etaEle,ptEle);		
		  weightHLT=lookupSingleEle(etaEle,ptEle);
		  weightHLTMT=lookupMT(mt,etaEle); 

	         newBranchHLT->Fill();
                 newBranchHLTMT->Fill();
                 newBranchIDISO->Fill();

          if(i%100000==1) printf("\t\t %4.0d Eta=%4.2f  Pt=%4.2f MT=%4.2f---> T&P=%4.2f HLT=%4.2f MT=%4.2f (Iso=%4.2f) \n",i,etaEle,ptEle,mt, weightIDISO,weightHLT,weightHLTMT,isobel);

	}
      
      printf("The End!\n");
      
      t->Write("",TObject::kOverwrite);
      return;     
    }
  }
} 


float lookupScale(float etaEle,float ptEle){

	double weightISOBEL=1;

                        if(ptEle<35){
                                if(fabs(etaEle)<1.4442) weightISOBEL= 0.821050/0.833064 ;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL= 0.518092/0.511194;
				 else weightISOBEL=0;
                        }
                        else if(ptEle<40) {
                                if(fabs(etaEle)<1.4442) weightISOBEL= 0.848462/0.862365;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL= 0.596356/0.602840;
                                 else weightISOBEL=0;
                        }
                        if(ptEle<45) {
                                if(fabs(etaEle)<1.4442) weightISOBEL=0.871605/0.887630;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL = 0.677178/0.697510 ;
                                 else weightISOBEL=0;
                        }
                        else if(ptEle<50) {
                                if(fabs(etaEle)<1.4442) weightISOBEL= 0.879113/0.893840;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL= 0.726756/0.755125 ;//
                                 else weightISOBEL=0;
                        }
                        else if (ptEle<55){
                                 if(fabs(etaEle)<1.4442) weightISOBEL= 0.88024/0.898515;
                                 else if(fabs(etaEle)>1.5660)  weightISOBEL = 0.750264/0.772811;
                                 else weightISOBEL=0;
                        }
                        else if(ptEle<75){
                                if(fabs(etaEle)<1.4442) weightISOBEL=0.885933/0.910065;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL=0.771490/0.773492;
                                 else weightISOBEL=0;
                        }else{
                                if(fabs(etaEle)<1.4442) weightISOBEL=0.883917/0.910876;
                                 else if(fabs(etaEle)>1.5660) weightISOBEL= 0.779009/0.782698;
                                 else weightISOBEL=0;
                        }

	return weightISOBEL;
}

float lookupSCtoRECO(float etaEle,float ptEle){

        double weight=1;

	if (ptEle<35){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=1.0096;
		if(etaEle>-1.5&&etaEle<=0)    weight=1.0060;
		if(etaEle>0&&etaEle<1.5)      weight=1.0021;
		if(etaEle>1.5&&etaEle<2.5)    weight=1.0094;	
	}
	else if (ptEle<40){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=1.0038;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9987;
                if(etaEle>0&&etaEle<1.5)      weight=0.9935;
                if(etaEle>1.5&&etaEle<2.5)    weight=1.0135;  
        }
        else if (ptEle<45){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=1.0002;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9951;
                if(etaEle>0&&etaEle<1.5)      weight=0.9941;
                if(etaEle>1.5&&etaEle<2.5)    weight=1.0111;
        }
        else if (ptEle<50){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=1.0202;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9941;
                if(etaEle>0&&etaEle<1.5)      weight=0.9967;
                if(etaEle>1.5&&etaEle<2.5)    weight=1.0170;
        }
        else if (ptEle<200){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=1.0287;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9805;
                if(etaEle>0&&etaEle<1.5)      weight=0.9989;
                if(etaEle>1.5&&etaEle<2.5)    weight=1.0421;
        }
	else weight=1.0;

return weight;

} 	

float lookupRECOtoWP(float etaEle,float ptEle){

        double weight=1;

        if (ptEle<35){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9337;
                if(etaEle>-1.5&&etaEle<=0)    weight=1.0018;
                if(etaEle>0&&etaEle<1.5)      weight=0.9958;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9372;  
        }
        else if (ptEle<40){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9545;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9910;
                if(etaEle>0&&etaEle<1.5)      weight=0.9960;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9607;
        }
        else if (ptEle<45){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9661;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9946;
                if(etaEle>0&&etaEle<1.5)      weight=0.9892;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9648;
        }
        else if (ptEle<50){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9672;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9938;
                if(etaEle>0&&etaEle<1.5)      weight=0.9917;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9729;
        }
        else if (ptEle<200){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9836;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9915;
                if(etaEle>0&&etaEle<1.5)      weight=0.9857;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9813;
        }
        else weight=1.0;

return weight;

}

float lookupSingleEle(float etaEle,float ptEle){

        double weight=1;

        if (ptEle<35){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8676;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9132;
                if(etaEle>0&&etaEle<1.5)      weight=0.9148;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8753;
        }
        else if (ptEle<40){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9258;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9938;
                if(etaEle>0&&etaEle<1.5)      weight=0.9938;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9145;
        }
        else if (ptEle<45){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9656;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9945;
                if(etaEle>0&&etaEle<1.5)      weight=0.9944;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9603;
        }
        else if (ptEle<50){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9698;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9934;
                if(etaEle>0&&etaEle<1.5)      weight=0.9926;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9664;
        }
        else if (ptEle<200){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9290;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9796;
                if(etaEle>0&&etaEle<1.5)      weight=0.9796;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9255;
        }
        else weight=1.0;

return weight;

}

float lookupMT(float mt, float etaEle){

	float weight=1.000;

	if(mt<55){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.3580;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.7315;
                if(etaEle>0&&etaEle<1.5)      weight=0.7315;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.3580;
	}
	else if(mt<60){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.4796;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.8151;
                if(etaEle>0&&etaEle<1.5)      weight=0.8151;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.4796;
        }
        else if(mt<65){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.6073;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9035;
                if(etaEle>0&&etaEle<1.5)      weight=0.9035;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.6073;
        }
        else if(mt<70){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.7473;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9548;
                if(etaEle>0&&etaEle<1.5)      weight=0.9548;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.7473;
        }       
        else if(mt<75){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8256;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9756;
                if(etaEle>0&&etaEle<1.5)      weight=0.9756;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8256;
        }
	else if(mt<80){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8711;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9866;
                if(etaEle>0&&etaEle<1.5)      weight=0.9866;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8711;
	} else if(mt<85){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9047;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9934;
                if(etaEle>0&&etaEle<1.5)      weight=0.9934;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9047;
        }
	else if (mt<90){
	        if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9308;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9958;
                if(etaEle>0&&etaEle<1.5)      weight=0.9958;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9308;
        }
        else if (mt<95){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9415;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9975;
                if(etaEle>0&&etaEle<1.5)      weight=0.9975;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9415;
        }
	else if(mt<100){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9441;
		if(etaEle>-1.5&&etaEle<=0)    weight=0.9973;
		if(etaEle>0&&etaEle<1.5)      weight=0.9973;
		if(etaEle>1.5&&etaEle<2.5)    weight=0.9441;
	}
	else if(mt<110){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9358;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9980;
                if(etaEle>0&&etaEle<1.5)      weight=0.9980;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9358;
        }
	else if(mt<120){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9120 ;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9963 ;
                if(etaEle>0&&etaEle<1.5)      weight=0.9963 ;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.9120 ;
        }
        else if(mt<120){
		if(etaEle>-2.5&&etaEle<=-1.5) weight=0.9120 ;   
        	if(etaEle>-1.5&&etaEle<=0)    weight=0.9963 ;
        	if(etaEle>0&&etaEle<1.5)      weight=0.9963 ;
        	if(etaEle>1.5&&etaEle<2.5)    weight=0.9120 ;
	}
	
        else if(mt<140){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8721 ;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9950 ;
                if(etaEle>0&&etaEle<1.5)      weight=0.9950 ;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8721 ;
        }
        else if(mt<180){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8311  ;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9899  ;
                if(etaEle>0&&etaEle<1.5)      weight=0.9899  ;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8311  ;
        }
        else if(mt<240){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8011 ;
                if(etaEle>-1.5&&etaEle<=0)    weight=0.9915 ;
                if(etaEle>0&&etaEle<1.5)      weight=0.9915 ;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8011 ;
        }
        else if(mt<300){
                if(etaEle>-2.5&&etaEle<=-1.5) weight=0.8110 ;
                if(etaEle>-1.5&&etaEle<=0)    weight=1.0000 ;
                if(etaEle>0&&etaEle<1.5)      weight=1.0000 ;
                if(etaEle>1.5&&etaEle<2.5)    weight=0.8110 ;
        }

	return weight;

}



