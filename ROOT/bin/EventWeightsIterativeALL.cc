#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "TFile.h"
#include "TROOT.h"
#include "TKey.h"
#include "TTree.h"
#include "TH1F.h"

//Distribution from Fall 2011

std::vector<float> data;
std::vector<float> mc;
edm::Lumi3DReWeighting *LumiWeights;
edm::LumiReWeighting *LumiWeights1D68;
edm::LumiReWeighting *LumiWeights1D73p5;
edm::Lumi3DReWeighting *LumiWeights3DRebe;

 Double_t weightsMC_[50] = {
    0.003388501,
    0.010357558,
    0.024724258,
    0.042348605,
    0.058279812,
    0.068851751,
    0.072914824,
    0.071579609,
    0.066811668,
    0.060672356,
    0.054528356,
    0.04919354,
    0.044886042,
    0.041341896,
    0.0384679,
    0.035871463,
    0.03341952,
    0.030915649,
    0.028395374,
    0.025798107,
    0.023237445,
    0.020602754,
    0.0180688,
    0.015559693,
    0.013211063,
    0.010964293,
    0.008920993,
    0.007080504,
    0.005499239,
    0.004187022,
    0.003096474,
    0.002237361,
    0.001566428,
    0.001074149,
    0.000721755,
    0.000470838,
    0.00030268,
    0.000184665,
    0.000112883,
    6.74043E-05,
    3.82178E-05,
    2.22847E-05,
    1.20933E-05,
    6.96173E-06,
    3.4689E-06,
    1.96172E-06,
    8.49283E-07,
    5.02393E-07,
    2.15311E-07,
    9.56938E-08
 };


 Double_t weights_[50] = {
    5.824840e-04,
    2.197489e-04,
    1.211629e-03,
    2.220454e-02,
    7.715328e-02,
    1.084481e-01,
    1.080546e-01,
    9.895186e-02,
    8.801444e-02,
    8.095987e-02,
    7.462586e-02,
    6.945955e-02,
    6.577173e-02,
    6.067065e-02,
    5.204067e-02,
    3.974540e-02,
    2.623719e-02,
    1.471457e-02,
    6.933530e-03,
    2.735890e-03,
    9.124539e-04,
    2.632320e-04,
    6.767414e-05,
    1.589458e-05,
    3.468433e-06,
    7.180819e-07,
    1.484517e-07,
    3.448207e-08,
    1.097762e-08,
    6.108292e-09,
    6.264500e-09,
    8.702501e-09,
    1.278183e-08,
    1.834045e-08,
    2.526965e-08,
    3.332641e-08,
    4.204924e-08,
    5.075478e-08,
    5.860588e-08,
    6.473668e-08,
    6.840763e-08,
    6.915181e-08,
    6.687248e-08,
    6.186376e-08,
    5.474822e-08,
    4.634989e-08,
    3.753809e-08,
    2.908310e-08,
    2.155530e-08,
    0.000000e+00
};

 Double_t weights73MB_[50] = {
    5.811923e-04,
    2.501019e-04,
    6.828335e-04,
    1.063119e-02,
    5.405826e-02,
    9.362939e-02,
    1.017907e-01,
    9.689960e-02,
    8.779542e-02,
    7.914933e-02,
    7.377273e-02,
    6.826762e-02,
    6.406011e-02,
    6.091800e-02,
    5.666883e-02,
    4.970319e-02,
    3.967595e-02,
    2.808457e-02,
    1.735659e-02,
    9.269154e-03,
    4.253005e-03,
    1.679823e-03,
    5.782058e-04,
    1.771840e-04,
    4.948797e-05,
    1.282945e-05,
    3.126782e-06,
    7.286161e-07,
    1.688181e-07,
    4.242548e-08,
    1.340582e-08,
    6.455727e-09,
    5.406465e-09,
    6.702613e-09,
    9.462957e-09,
    1.344846e-08,
    1.858163e-08,
    2.476510e-08,
    3.178771e-08,
    3.928438e-08,
    4.674150e-08,
    5.354322e-08,
    5.905073e-08,
    6.269961e-08,
    6.409485e-08,
    6.308125e-08,
    5.977179e-08,
    5.452693e-08,
    4.788997e-08,
    0.000000e+00
};





float weightTrue2011(float input){
  if(input>50) 
    return 1;

    
  float w[50];


w[0]= 0.212929;
w[1]= 0.0208114;
w[2]= 0.0584048;
w[3]= 0.538898;
w[4]= 1.357;
w[5]= 1.49913;
w[6]= 1.42247;
w[7]= 1.35904;
w[8]= 1.29946;
w[9]= 1.27925;
w[10]= 1.37845;
w[11]= 1.71246;
w[12]= 1.5291;
w[13]= 1.35234;
w[14]= 1.22215;
w[15]= 1.0155;
w[16]= 1.01137;
w[17]= 0.395465;
w[18]= 0.230984;
w[19]= 0.109883;
w[20]= 0.0433739;
w[21]= 0.0111497;
w[22]= 0.00408801;
w[23]= 0.00115678;
w[24]= 0.000365505;
w[25]= 0.000112391;
w[26]= 3.83894e-05;
w[27]= 1.60651e-05;
w[28]= 4.81412e-06;
w[29]= 1.39717e-06;
w[30]= 1.92368e-06;
w[31]= 4.10748e-06;
w[32]= 2.33157e-05;
w[33]= 4.0181e-05;
w[34]= 4.87786e-05;
w[35]= 0.00194128;
w[36]= 8.97414e-05;
w[37]= 1;
w[38]= 1;
w[39]= 0.000162709;
w[40]= 1;
w[41]= 1;
w[42]= 1;
w[43]= 1;
w[44]= 1;
w[45]= 1;
w[46]= 1;
w[47]= 1;
w[48]= 1;
w[49]= 1;


 TH1F h("boh","boh",50,0.,50.);
 
 for(int k=0;k<50;k++){
   h.SetBinContent(k+1,w[k]);
 }
 
 return h.GetBinContent(h.FindBin(input));

}



void readdir(TDirectory *dir,optutl::CommandLineParser parser,float ev,int doPU,bool doRho,TH1F* puWeight,TH1F* rhoWeight); 
void readdirALL(TDirectory *dir,optutl::CommandLineParser parser,float ev, TH1F* puWeight);



int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("histoName",optutl::CommandLineParser::kString,"Counter Histogram Name","EventSummary");
   parser.addOption("EVENTSMC",optutl::CommandLineParser::kDouble,"Events in MC Sample",-1.000);
   parser.addOption("weight",optutl::CommandLineParser::kDouble,"Weight to apply",1.0);
   parser.addOption("type",optutl::CommandLineParser::kInteger,"Type",0);
   parser.addOption("branch",optutl::CommandLineParser::kString,"Branch","__WEIGHT__");
   parser.addOption("doOneD",optutl::CommandLineParser::kInteger,"Do OneD",0);
   parser.addOption("doALL",optutl::CommandLineParser::kInteger,"Do ALL",0);

   
   parser.parseArguments (argc, argv);
   

   // Read File

   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");


   float eventsMC=parser.doubleValue("EVENTSMC"); 
   float ev=eventsMC; 	
   
   if(eventsMC==-1){	
   	TH1F* evC  = (TH1F*)f->Get(parser.stringValue("histoName").c_str());
   	ev = evC->GetBinContent(1)*1.0000;
   }	


   int DoALL=parser.integerValue("doALL");


   printf("Found  %f Events Counted\n",ev);
   printf("Base Weight %f \n", parser.doubleValue("weight")/(ev));




   //read PU info
   TH1F *puWeight=0;

   int doPU=0;
   TFile *fPU = new TFile("../puInfo.root");

   if(fPU!=0 && fPU->IsOpen()) {
     puWeight = (TH1F*)fPU->Get("weight");

     doPU=1;
     printf("ENABLING PU WEIGHTING USING VERTICES\n");

   }

   TFile *fPU3 = new TFile("../Weight3D.root");
   TFile *fPU4 = new TFile("Weight3D.root");

   if(fPU3!=0 && fPU3->IsOpen()) {
     doPU=2;
     printf("ENABLING PU WEIGHTING USING 3D with ready distribution\n");
     fPU3->Close();
     LumiWeights = new edm::Lumi3DReWeighting();//mc,data);
     LumiWeights->weight3D_init("../Weight3D.root");
   }
   else   if(fPU4!=0 && fPU4->IsOpen()) {

     //searxch in this folder
       doPU=2;
       printf("ENABLING PU WEIGHTING USING 3D with  distribution you just made\n");
       fPU4->Close();
       LumiWeights = new edm::Lumi3DReWeighting();//mc,data);
       LumiWeights->weight3D_init("Weight3D.root");

   }
   if(parser.integerValue("doOneD")||parser.integerValue("doALL")) {
     doPU=3;
     std::vector<float> mc;
     std::vector<float> data;
     std::vector<float> data73MB;

     for(int i=0;i<50;++i) {
       mc.push_back(weightsMC_[i]);
       data.push_back(weights_[i]);
       data73MB.push_back(weights73MB_[i]);	
     }

     LumiWeights1D68 = new edm::LumiReWeighting(mc,data);
     LumiWeights1D73p5 = new edm::LumiReWeighting(mc,data73MB);

     
   }
   

   TFile *fPUMCRebe = new TFile("../MC_Fall11.root");
   TFile *fPUDataRebe = new TFile("../RunAB.root");

   printf("ENABLING PU WEIGHTING USING 3D by Rebeca - tW analysis\n");
   	
   LumiWeights3DRebe = new edm::Lumi3DReWeighting("../MC_Fall11.root","../RunAB.root", "pileup", "pileup","Weights.root");//
   LumiWeights3DRebe->weight3D_init(1.0);


   //read PU info
   TH1F *rhoWeight=0;
   bool doRho=false;
   TFile *fRho = new TFile("../rhoInfo.root");

   if(fRho!=0 && fRho->IsOpen()) {
     rhoWeight = (TH1F*)fRho->Get("weight");
     doRho=true;
     printf("ENABLING Rho WEIGHTING\n");

   }

 
   if(DoALL==1) readdirALL(f,parser,ev,puWeight);
   else readdir(f,parser,ev,doPU,doRho,puWeight,rhoWeight);

   f->cd();
   f->Close();
   if(fPU!=0 && fPU->IsOpen())
     fPU->Close();


} 


void readdir(TDirectory *dir,optutl::CommandLineParser parser,float ev,int doPU,bool doRho,TH1F *puWeight,TH1F *rhoWeight) 
{
  TDirectory *dirsav = gDirectory;
  TIter next(dir->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    printf("Found key=%s \n",key->GetName());
    TObject *obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      dir->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      readdir(subdir,parser,ev,doPU,doRho,puWeight,rhoWeight);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())) {
      TTree *t = (TTree*)obj;
      float weight = parser.doubleValue("weight")/(ev);
      int   type = parser.integerValue("type");


      TBranch *newBranch = t->Branch(parser.stringValue("branch").c_str(),&weight,(parser.stringValue("branch")+"/F").c_str());

      TBranch *typeBranch = t->Branch("TYPE",&type,"TYPE/I");
      int vertices;
      float bxm=0;
      float bx=0;
      float bxp=0;

      if(doPU==1)
	t->SetBranchAddress("vertices",&vertices);
      else if(doPU==2 ||doPU==3) {
	t->SetBranchAddress("puBXminus",&bxm);
	t->SetBranchAddress("puBX0",&bx);
	t->SetBranchAddress("puBXplus",&bxp);
      }

      float rho=0.0;
      if(doRho)
	t->SetBranchAddress("Rho",&rho);

      printf("Found tree -> weighting\n");
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
	  weight = parser.doubleValue("weight")/(ev);
	  if(doPU==1) {
	    int bin=puWeight->FindBin(vertices);
	    if(bin>puWeight->GetNbinsX())
	      {
		printf("Overflow using max bin\n");
		bin = puWeight->GetNbinsX();
	      }
	    weight*=puWeight->GetBinContent(bin);
	    if(i==1)
	      printf("PU WEIGHT = %f\n",puWeight->GetBinContent(puWeight->FindBin(vertices)));

	  }
	  else if(doPU==2) {
	   float w = LumiWeights->weight3D( bxm,bx,bxp);
	    if(i==1)
	      printf("PU WEIGHT = %f\n",w);
	    weight*=w;
	  }
	  else if(doPU==3) {
	    weight*=LumiWeights1D68->weight(bx);
	  }
	  if(doRho) {
	    weight*=rhoWeight->GetBinContent(rhoWeight->FindBin(rho));
	    if(i==1)
	      printf("RHO WEIGHT = %f\n",rhoWeight->GetBinContent(rhoWeight->FindBin(rho)));
	  }

	  newBranch->Fill();
	  typeBranch->Fill();
	}
      t->Write("",TObject::kOverwrite);
    }
//     else if(obj->IsA()->InheritsFrom(TH1F::Class())) {
//       TH1F *h = (TH1F*)obj;
//       h->Sumw2();
//       printf("scaling histogram with %f entries\n",h->Integral());
//       float weight = parser.doubleValue("weight")/(ev);
//       h->Sumw2();
//       for( int i=1;i<=h->GetNbinsX();++i)
// 	h->SetBinContent(i,h->GetBinContent(i)*weight);
 
//       TDirectory *tmp = gDirectory;
//       h->SetDirectory(gDirectory);
//       h->Write("resultsWeighted");
//     }


  }

}



void readdirALL(TDirectory *dir,optutl::CommandLineParser parser,float ev,TH1F *puWeight) 
{

  printf("WEIGHTING!!!");

  TDirectory *dirsav = gDirectory;
  TIter next(dir->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    printf("Found key=%s \n",key->GetName());
    TObject *obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      dir->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      readdirALL(subdir,parser,ev,puWeight);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())) {
      TTree *t = (TTree*)obj;
      double weight = parser.doubleValue("weight")/(ev);

      float weightNOPU=-1;
      float weightPUV=-1;
      float weightPU3D=-1;
      float weightPU1D=-1;
      float weightPU1DSYS=-1;
      float weightPU1DIan=-1;

      float weightPUVSUM=0;
      float weightPU3DSUM=0;
      float weightPU1DSUM=0;
      float weightPU1DIanSUM=0;

      float weightPU3DRebe=-1;
      float weightPU3DRebeSUM=0;


      TBranch *newBranchNOPU = t->Branch("weightNOPU",&weightNOPU,"weightNOPU/F");
      TBranch *newBranch3D = t->Branch("weightPU3D",&weightPU3D,"weightPU3D/F");
      TBranch *newBranch1D = t->Branch("weightPU1D",&weightPU1D,"weightPU1D/F");
      TBranch *newBranchVertices = t->Branch("weightPUV",&weightPUV,"weightPUV/F");
      TBranch *newBranch1DSYS = t->Branch("weightPU1DSYS",&weightPU1DSYS,"weightPU1DSYS/F");
      TBranch *newBranch1DIan = t->Branch("weightPU1DIan",&weightPU1DIan,"weightPU1DIan/F");
      TBranch *newBranch3DRebe = t->Branch("weightPU3DRebe",&weightPU3DRebe,"weightPU3DRebe/F");

      int vertices;
      float bxm=0;
      float bx=0;
      float bxp=0;
      float btruth=0;


	t->SetBranchAddress("vertices",&vertices);
	t->SetBranchAddress("puBXminus",&bxm);
	t->SetBranchAddress("puBX0",&bx);
	t->SetBranchAddress("puBXplus",&bxp);
        t->SetBranchAddress("puTruth",&btruth);


          weightNOPU=weight;

	  printf("\t NOPU WEIGHT: %f\n",weightNOPU);

      float EVENTS=0;

      printf("Found tree -> weighting\n");
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  EVENTS++;

	  weight = parser.doubleValue("weight")/(ev);
	  weightNOPU=weight;

	  t->GetEntry(i);

	    if(i%100000==1){
	     printf("Entry %d \n",i);
	     printf("Vertices?  %d \n",vertices); 	
	     printf("PU? %f, %f, %f \n",bxm, bx, bxp);	
	     printf("True PU? %f (%f) \n", btruth, (bxm+bx+bxp)/3);	
	     }	

	    int bin=puWeight->FindBin(vertices);
	    if(bin>puWeight->GetNbinsX())
	      {
		//printf("Overflow using max bin\n");
		bin = puWeight->GetNbinsX();
	      }
	    weightPUV=weight*puWeight->GetBinContent(bin);
	    weightPUVSUM+=puWeight->GetBinContent(bin);

            if(i%100000==1)
	      printf("\t PU WEIGHT Vertices = %f (%d)\n",puWeight->GetBinContent(puWeight->FindBin(vertices))*weight,bin);

	   weightPU3D = weight*LumiWeights->weight3D( bxm,bx,bxp);
	   weightPU3DSUM+=LumiWeights->weight3D( bxm,bx,bxp);

   	   weightPU3DRebe = weight*LumiWeights3DRebe->weight3D( bxm,bx,bxp);
           weightPU3DRebeSUM+=LumiWeights3DRebe->weight3D( bxm,bx,bxp);



	    if(i%100000==1)
	      printf("\t PU WEIGHT 3D = %f (%4.0f,%4.0f,%4.0f)\n",weightPU3D,bxm,bx,bxp);

	   weightPU1D=weight*LumiWeights1D68->weight(btruth);
           weightPU1DSYS=weight*LumiWeights1D73p5->weight(btruth);
	   weightPU1DSUM+=LumiWeights1D68->weight(btruth);


            if(i%100000==1){
              printf("\t PU WEIGHT 1D (73.5) = %f   (%4.0f)\n",weightPU1DSYS,btruth);
              printf("\t PU WEIGHT 1D (68)   = %f   (%4.0f)\n",weightPU1D,btruth);
		}


	  weightPU1DIan=weight*weightTrue2011(btruth);
          weightPU1DIanSUM+=weightTrue2011(btruth);

            if(i%100000==1){
              printf("\t PU WEIGHT 1D (Ian) = %f   (%4.0f)\n",weightPU1DIan,btruth);
                }



	  newBranchNOPU->Fill();
          newBranch1D->Fill();
          newBranch3D->Fill();
          newBranchVertices->Fill();
          newBranch1DSYS->Fill();
          newBranch1DIan->Fill();
          newBranch3DRebe->Fill();

	}
        printf("EVENTS %f \n",EVENTS);
	printf("WEIGHTS PUV %f \n",weightPUVSUM);
        printf("WEIGHTS PU1D %f \n",weightPU1DSUM);
        printf("WEIGHTS PU1DIan %f \n",weightPU1DIanSUM);
        printf("WEIGHTS PU3D %f \n",weightPU3DSUM);
        printf("WEIGHTS PU3D %f \n",weightPU3DRebeSUM);


      t->Write("",TObject::kOverwrite);
    }
    }

}




