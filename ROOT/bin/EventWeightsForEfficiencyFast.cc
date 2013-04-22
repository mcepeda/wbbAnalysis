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




double efficiency(double m, double m0, double sigma, double alpha, double n, double norm)
 { 
   const double sqrtPiOver2 = 1.2533141373;
   const double sqrt2 = 1.4142135624;

   double sig = fabs((double) sigma);
   
   double t = (m - m0)/sig ;
   
   if (alpha < 0)
     t = -t;

   double absAlpha = fabs(alpha / sig);
   double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
   double b = absAlpha - n/absAlpha;

   //   if (a>=std::numeric_limits<double>::max()) return -1. ;

   double ApproxErf ;
   double arg = absAlpha / sqrt2 ;
   if (arg > 5.) ApproxErf = 1 ;
   else if (arg < -5.) ApproxErf = -1 ;
   else ApproxErf = TMath::Erf(arg) ;

   double leftArea = (1 + ApproxErf) * sqrtPiOver2 ;
   double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
   double area = leftArea + rightArea;

   if ( t <= absAlpha ){
     arg = t / sqrt2 ;
     if (arg > 5.) ApproxErf = 1 ;
     else if (arg < -5.) ApproxErf = -1 ;
     else ApproxErf = TMath::Erf(arg) ;
     return norm * (1 + ApproxErf) * sqrtPiOver2 / area ;
   }
   else{
     return norm * (leftArea +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area ;
   }
  
 }


void readdir(TDirectory *dir,optutl::CommandLineParser parser); 


float weightEMu(float pt1,float pt2) {
  float m0 =14.4751; float sigma = 0.120967; float alpha = 0.0226; float n = 4.3709; float norm=0.874294;


  m0=2.90901; sigma=22.4641;alpha=74.3622;n=3.72143;norm=0.976318;
  float dataMUID = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=1.74118; sigma=22.5399;alpha=52.1416;n=6.59594;norm=0.980176;
  float mcMUID = efficiency(pt2,m0,sigma,alpha,n,norm);
  

  m0=2.67899; sigma=21.9374;alpha=35.4;n=155.359;norm=0.977301;
  float eleid = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=-3.1459; sigma=27.0568;alpha=81.9479;n=2.21511;norm=0.974106;
  float mceleid = efficiency(pt1,m0,sigma,alpha,n,norm);



  return dataMUID*0.997841*eleid/(mcMUID*mceleid);
 

}


float weightMuTau(float pt1,float pt2,float eta1,float eta2) {

  float m0 =15.06; float sigma = 0.55278; float alpha = 1.34236; float n = 1.003; float norm = 3.36767;
  float muBdata2 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0 =15.03175; sigma = 0.866114; alpha = 1.25009; n = 1.63711; norm = 0.844906;
  float muEdata2 = efficiency(pt1,m0,sigma,alpha,n,norm);

  float muBmc = 0.91677;
  float muBdata1 = 0.901186;

  float muEmc = 0.835749;
  float muEdata1 = 0.862932;


  m0 =13.6046;sigma = 1.66291;alpha = 1.71551;n = 141.929;norm=0.910686;
  float data1B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=13.9694; sigma=0.084835;alpha=0.057743;n=1.50674;norm=0.984976;
  float data2B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.2102; sigma=1.26519;alpha=2.48994;n=1.04699;norm=1.3492;
  float data3B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.4601; sigma=0.0485272;alpha=0.03849;n=1.48324;norm=0.965257;
  float mcTauB = efficiency(pt2,m0,sigma,alpha,n,norm);


  float weighttauB=(0.034*data1B+0.399*data2B+0.567*data3B);

  m0 =-0.392211;sigma = 7.90467;alpha = 5.48228;n = 134.599;norm=0.925858;
  float data1E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.435; sigma=1.34952;alpha=2.43996;n=1.03631;norm=1.79081;
  float data2E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.2438; sigma=1.37298;alpha=1.76448;n=1.73935;norm=0.901291;
  float data3E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.4451; sigma=0.0790573;alpha=0.0732472;n=1.47046;norm=0.942028;
  float mcTauE = efficiency(pt2,m0,sigma,alpha,n,norm);


  float weighttauE=(0.034*data1E+0.399*data2E+0.567*data3E);


  float dataIdB = 0.958046;
  float mcIdB = 0.968006;
  
  float dataIdE = 0.937338;
  float mcIdE = 0.960542;
  if(fabs(eta2)<1.479){
	  if(pt1 > 17 && pt1 < 20){
		  if(fabs(eta1)<1.4)
			return weighttauB*0.678*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauB*0.705*mcIdB*muBmc);
		  else
			return weighttauB*0.699*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauB*0.734*mcIdE*muEmc);
	  }
	  else if(pt1 > 20 && pt1 < 25){
		  if(fabs(eta1)<1.4)
			return weighttauB*0.768*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauB*0.786*mcIdB*muBmc);
		  else
			return weighttauB*0.783*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauB*0.810*mcIdE*muEmc);  	  
	  }
	  else{
		  if(fabs(eta1)<1.4)
			return weighttauB*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauB*mcIdB*muBmc);
		  else
			return weighttauB*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauB*mcIdE*muEmc);  	  
	  }
  }
  else{
	  if(pt1 > 17 && pt1 < 20){
		  if(fabs(eta1)<1.4)
			return weighttauE*0.678*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauE*0.705*mcIdB*muBmc);
		  else
			return weighttauE*0.699*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauE*0.734*mcIdE*muEmc);
	  }
	  else if(pt1 > 20 && pt1 < 25){
		  if(fabs(eta1)<1.4)
			return weighttauE*0.768*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauE*0.786*mcIdB*muBmc);
		  else
			return weighttauE*0.783*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauE*0.810*mcIdE*muEmc);  	  
	  }
	  else{
		  if(fabs(eta1)<1.4)
			return weighttauE*dataIdB*(0.433*muBdata1+0.567*muBdata2)/(mcTauE*mcIdB*muBmc);
		  else
			return weighttauE*dataIdE*(0.433*muEdata1+0.567*muEdata2)/(mcTauE*mcIdE*muEmc);  	  
	  }  
  }
}


float weightMuTauEmbedded(float pt1,float pt2,float eta1,float eta2) {
  float m0 =15.06; float sigma = 0.55278; float alpha = 1.34236; float n = 1.003; float norm = 3.36767;
  float muBdata2 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0 =15.03175; sigma = 0.866114; alpha = 1.25009; n = 1.63711; norm = 0.844906;
  float muEdata2 = efficiency(pt1,m0,sigma,alpha,n,norm);

  float muBmc = 0.91677;
  float muBdata1 = 0.901186;

  float muEmc = 0.835749;
  float muEdata1 = 0.862932;


  m0 =13.6046;sigma = 1.66291;alpha = 1.71551;n = 141.929;norm=0.910686;
  float data1B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=13.9694; sigma=0.084835;alpha=0.057743;n=1.50674;norm=0.984976;
  float data2B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.2102; sigma=1.26519;alpha=2.48994;n=1.04699;norm=1.3492;
  float data3B = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.4601; sigma=0.0485272;alpha=0.03849;n=1.48324;norm=0.965257;
  float mcTauB = efficiency(pt2,m0,sigma,alpha,n,norm);


  float weighttauB=(0.034*data1B+0.399*data2B+0.567*data3B);

  m0 =-0.392211;sigma = 7.90467;alpha = 5.48228;n = 134.599;norm=0.925858;
  float data1E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.435; sigma=1.34952;alpha=2.43996;n=1.03631;norm=1.79081;
  float data2E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.2438; sigma=1.37298;alpha=1.76448;n=1.73935;norm=0.901291;
  float data3E = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=14.4451; sigma=0.0790573;alpha=0.0732472;n=1.47046;norm=0.942028;
  float mcTauE = efficiency(pt2,m0,sigma,alpha,n,norm);


  float weighttauE=(0.034*data1E+0.399*data2E+0.567*data3E);

  float dataIdB = 0.958046;
  float mcIdB = 0.968006;
  
  float dataIdE = 0.937338;
  float mcIdE = 0.960542;


	if(fabs(eta2)<1.479){
	  if(fabs(eta1)<1.4)
		return weighttauB*(0.433*muBdata1+0.567*muBdata2);
	  else
		return weighttauB*(0.433*muEdata1+0.567*muEdata2);
	}
	else{
	  if(fabs(eta1)<1.4)
		return weighttauE*(0.433*muBdata1+0.567*muBdata2);
	  else
		return weighttauE*(0.433*muEdata1+0.567*muEdata2);
	}

}



float weightETauEmbedded(float pt1,float pt2,float eta1,float eta2) {

  float m0 =14.8772; float sigma = 0.311255; float alpha = 0.221021; float n = 1.87734; float norm=0.986665;
  float dataeleEB15 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=15.6629; sigma=0.759192;alpha=0.47756;n=2.02154;norm=0.998816;
  float dataeleEE15 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=17.0006; sigma=0.0230878;alpha=0.00569008;n=1.6388;norm=1.00235;
  float mceleEB = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=16.993; sigma=0.0693958;alpha=0.00695096;n=1.9566;norm=1.00632;
  float mceleEE = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=18.3193; sigma=0.443703;alpha=0.385554;n=1.86523;norm=0.986514;
  float dataeleEB18 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=19.6586; sigma=-0.682633;alpha=0.279486;n=2.66423;norm=0.973455;
  float dataeleEE18 = efficiency(pt1,m0,sigma,alpha,n,norm);


  m0=20.554; sigma=0.683776;alpha=0.855573;n=1.45917;norm=1.03957;
  float dataeleEB20 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=23.6386; sigma=-1.60775;alpha=1.72093;n=1.4131;norm=1.13962;
  float dataeleEE20 = efficiency(pt1,m0,sigma,alpha,n,norm);


  m0=19.3916; sigma=0.996964;alpha=1.70131;n=1.38002;norm=0.994953;
  float datatauLooseB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.5667; sigma=1.15203;alpha=1.68126;n=1.40025;norm=0.848033;
  float datatauMediumB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.6013; sigma=0.987317;alpha=1.08015;n=1.88592;norm=0.776894;
  float datatauTightB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.468; sigma=0.0615381;alpha=0.0349325;n=1.59349;norm=0.860096;
  float mctauB = efficiency(pt2,m0,sigma,alpha,n,norm);


  m0=18.8166; sigma=0.526632;alpha=0.20666;n=6.80392;norm=0.903245;
  float datatauLooseE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=18.8476; sigma=0.528963;alpha=0.16717;n=3.65814;norm=0.749759;
  float datatauMediumE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=18.8859; sigma=0.271301;alpha=0.128008;n=1.50993;norm=0.825122;
  float datatauTightE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.3862; sigma=0.247148;alpha=0.123187;n=2.87108;norm=0.790894;
  float mctauE = efficiency(pt2,m0,sigma,alpha,n,norm);

  if(fabs(eta2)<1.479){
	  if(fabs(eta1)<1.442)
		return (0.226*dataeleEB15*datatauLooseB+0.157*dataeleEB15*datatauTightB+0.417*dataeleEB18*datatauMediumB+0.2*dataeleEB20*datatauMediumB);
	  else
		return (0.226*dataeleEE15*datatauLooseB+0.157*dataeleEE15*datatauTightB+0.417*dataeleEE18*datatauMediumB+0.2*dataeleEE20*datatauMediumB);
  }
  else{
      if(fabs(eta1)<1.442)
		return (0.226*dataeleEB15*datatauLooseE+0.157*dataeleEB15*datatauTightE+0.417*dataeleEB18*datatauMediumE+0.2*dataeleEB20*datatauMediumE);
	  else
		return (0.226*dataeleEE15*datatauLooseE+0.157*dataeleEE15*datatauTightE+0.417*dataeleEE18*datatauMediumE+0.2*dataeleEE20*datatauMediumE);
  }


}




float weightETau(float pt1,float pt2,float eta1,float eta2) {

  float m0 =14.8772; float sigma = 0.311255; float alpha = 0.221021; float n = 1.87734; float norm=0.986665;
  float dataeleEB15 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=15.6629; sigma=0.759192;alpha=0.47756;n=2.02154;norm=0.998816;
  float dataeleEE15 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=17.0006; sigma=0.0230878;alpha=0.00569008;n=1.6388;norm=1.00235;
  float mceleEB = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=16.993; sigma=0.0693958;alpha=0.00695096;n=1.9566;norm=1.00632;
  float mceleEE = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=18.3193; sigma=0.443703;alpha=0.385554;n=1.86523;norm=0.986514;
  float dataeleEB18 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=19.6586; sigma=-0.682633;alpha=0.279486;n=2.66423;norm=0.973455;
  float dataeleEE18 = efficiency(pt1,m0,sigma,alpha,n,norm);


  m0=20.554; sigma=0.683776;alpha=0.855573;n=1.45917;norm=1.03957;
  float dataeleEB20 = efficiency(pt1,m0,sigma,alpha,n,norm);

  m0=23.6386; sigma=-1.60775;alpha=1.72093;n=1.4131;norm=1.13962;
  float dataeleEE20 = efficiency(pt1,m0,sigma,alpha,n,norm);


  m0=19.3916; sigma=0.996964;alpha=1.70131;n=1.38002;norm=0.994953;
  float datatauLooseB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.5667; sigma=1.15203;alpha=1.68126;n=1.40025;norm=0.848033;
  float datatauMediumB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.6013; sigma=0.987317;alpha=1.08015;n=1.88592;norm=0.776894;
  float datatauTightB = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.468; sigma=0.0615381;alpha=0.0349325;n=1.59349;norm=0.860096;
  float mctauB = efficiency(pt2,m0,sigma,alpha,n,norm);


  m0=18.8166; sigma=0.526632;alpha=0.20666;n=6.80392;norm=0.903245;
  float datatauLooseE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=18.8476; sigma=0.528963;alpha=0.16717;n=3.65814;norm=0.749759;
  float datatauMediumE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=18.8859; sigma=0.271301;alpha=0.128008;n=1.50993;norm=0.825122;
  float datatauTightE = efficiency(pt2,m0,sigma,alpha,n,norm);

  m0=19.3862; sigma=0.247148;alpha=0.123187;n=2.87108;norm=0.790894;
  float mctauE = efficiency(pt2,m0,sigma,alpha,n,norm);

  if(fabs(eta2)<1.479){
	  if(fabs(eta1)<1.442)
		return (0.226*dataeleEB15*datatauLooseB+0.157*dataeleEB15*datatauTightB+0.417*dataeleEB18*datatauMediumB+0.2*dataeleEB20*datatauMediumB)/(mctauB*mceleEB);
	  else
		return (0.226*dataeleEE15*datatauLooseB+0.157*dataeleEE15*datatauTightB+0.417*dataeleEE18*datatauMediumB+0.2*dataeleEE20*datatauMediumB)/(mctauB*mceleEE);
  }
  else{
	  if(fabs(eta1)<1.442)
		return (0.226*dataeleEB15*datatauLooseE+0.157*dataeleEB15*datatauTightE+0.417*dataeleEB18*datatauMediumE+0.2*dataeleEB20*datatauMediumE)/(mctauE*mceleEB);
	  else
		return (0.226*dataeleEE15*datatauLooseE+0.157*dataeleEE15*datatauTightE+0.417*dataeleEE18*datatauMediumE+0.2*dataeleEE20*datatauMediumE)/(mctauE*mceleEE);
  }


}



int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("branch",optutl::CommandLineParser::kString,"Branch","__HLT__");
   parser.addOption("finalState",optutl::CommandLineParser::kString,"Final state","mutau");
   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   readdir(f,parser);
   f->Close();

} 


void readdir(TDirectory *dir,optutl::CommandLineParser parser) 
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
      readdir(subdir,parser);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())) {
      TTree *t = (TTree*)obj;
      float weight = 1.0;
      TBranch *newBranch = t->Branch(parser.stringValue("branch").c_str(),&weight,(parser.stringValue("branch")+"/F").c_str());

      float pt1;
      float eta1;
      float pt2;
      float eta2;


      t->SetBranchAddress("pt1",&pt1);
      t->SetBranchAddress("eta1",&eta1);
      t->SetBranchAddress("pt2",&pt2);
      t->SetBranchAddress("eta2",&eta2);
      
      std::string finalState = parser.stringValue("finalState");

      printf("Found tree -> weighting\n");
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
	  weight=0;
	  if(finalState=="muTau")
	    weight=weightMuTau(pt1,pt2,eta1,eta2);
	  else if (finalState=="eleTau")
	    weight=weightETau(pt1,pt2,eta1,eta2);
	  else if (finalState=="eleMu")
	    weight=weightEMu(pt1,pt2);
	  else if (finalState=="muTauEmbedded")
	    weight=weightMuTauEmbedded(pt1,pt2,eta1,eta2);
	  else if (finalState=="eleTauEmbedded")
	    weight=weightETauEmbedded(pt1,pt2,eta1,eta2);

	  newBranch->Fill();
	}

      t->Write("",TObject::kOverwrite);
    }
  }
}
