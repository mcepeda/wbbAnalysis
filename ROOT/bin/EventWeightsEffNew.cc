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

// RooHist *stuff = _file0->Get("MC_combRelPFISO20_2011B_pt__abseta>1.2")



void readdir(TDirectory *dir,TFile * fc, TFile * ft,optutl::CommandLineParser parser); 
float lookupScale(float eta);

int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   //parser.addOption("outputFile",optutl::CommandLineParser::kString,"File","corrections.root");
   //parser.addOption("eta",optutl::CommandLineParser::kString,"eta variable","eta1");
   parser.addOption("EffFile",optutl::CommandLineParser::kString,"Efficiencies File","../MuonEfficiencies2011_42X_DataMC.root");
   parser.addOption("TrigEffFile",optutl::CommandLineParser::kString,"Trig Efficiency File","../eff_mu.root");
   parser.addOption("brancheta",optutl::CommandLineParser::kString,"Tag and Probe Weight","wtEffIsotComb");
   //parser.addOption("branchpt",optutl::CommandLineParser::kString,"Tag and Probe Weight","effWEIGHTpt");

   //parser.addOption("typept",optutl::CommandLineParser::kInteger,"Typept",0);
   parser.addOption("typeeta",optutl::CommandLineParser::kInteger,"Typeeta",2);
   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   printf("Now weighting: %s\n",parser.stringValue("outputFile").c_str());
   TFile *fc = new TFile(parser.stringValue("EffFile").c_str());
   TFile *ft = new TFile(parser.stringValue("TrigEffFile").c_str());
   //int topType = parser.integerValue("typeTop");
   readdir(f,fc,ft,parser);

   f->Close();
   fc->Close();
   ft->Close();
} 


void readdir(TDirectory *dir,TFile* fc, TFile* ft,optutl::CommandLineParser parser) 
{

  //read directory
  TDirectory *dirsav = gDirectory;
  TIter next(dir->GetListOfKeys());
  TKey *key;
  while((key = (TKey*)next())){
    printf("Found key=%s \n",key->GetName());
    TObject *obj = key->ReadObj();
    if(obj->IsA()->InheritsFrom(TDirectory::Class())){
      string NAME=key->GetName();
//      if(NAME!="muNuJetEventTree" && NAME!="muNuJetJetEventTree") continue;
          size_t trigPath = NAME.find("muNu");
          if ( trigPath != 0) continue;
      printf("Found Muon Tree! %s \n",key->GetName());
	
      dir->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      readdir(subdir,fc,ft,parser);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())){
      TTree *t = (TTree*)obj;
      
      //get muon pt,eta
      float pt;
      float eta;

      float weighteta = 1.0;
      float weightetaIsoComb;
      float weightetaEff;      
      float weightetaIsoTight;

      int typeeta = 0;
      int typept = 0;
      t->SetBranchAddress("ptMu",&pt);
      t->SetBranchAddress("etaMu",&eta);
   
      RooHist *etaData_2011Bcomb = (RooHist*)fc->Get("DATA_combRelPFISO12_2011B_eta__pt>20");
      RooHist *etaData_2011Acomb = (RooHist*)fc->Get("DATA_combRelPFISO12_2011A_eta__pt>20");
      RooHist *etaMC_2011Bcomb = (RooHist*)fc->Get("MC_combRelPFISO12_2011B_eta__pt>20");
      RooHist *etaMC_2011Acomb = (RooHist*)fc->Get("MC_combRelPFISO12_2011A_eta__pt>20");
printf("tom was here\n");
      RooHist *etaData_2011Btight = (RooHist*)fc->Get("DATA_TIGHT_nH10_2011B_eta__pt>20");
      RooHist *etaData_2011Atight = (RooHist*)fc->Get("DATA_TIGHT_nH10_2011A_eta__pt>20");
      RooHist *etaMC_2011Btight = (RooHist*)fc->Get("MC_TIGHT_nH10_2011B_eta__pt>20");
      RooHist *etaMC_2011Atight = (RooHist*)fc->Get("MC_TIGHT_nH10_2011A_eta__pt>20");
printf("also here\n");
//      TGraphAsymmErrors *trig2011A = (TGraphAsymmErrors*) ft->Get("Mu24_eta_2011A");
//      TGraphAsymmErrors *trig2011B = (TGraphAsymmErrors*) ft->Get("Mu24_eta_2011B");
 
      //get branch and add in value
      TBranch *newBranchEta = t->Branch(parser.stringValue("brancheta").c_str(),&weighteta,(parser.stringValue("brancheta")+"/F").c_str());
      TBranch *typeBranchEta = t->Branch("TYPE",&typeeta,"TYPE/I");
      printf("Found tree -> weighting\n");
      printf("Entries %i \n",(int)t->GetEntries());

      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
	  weighteta=0.0;
	  if(pt>24&&eta>-2.4&&eta<2.4){////NOTE THESE EFFICIENCIES ARE ONLY GOOD FOR >30GeV!!
	  	//A,Bcor are for isolation correction
                float BcorComb = etaData_2011Bcomb->Eval(eta)/etaMC_2011Bcomb->Eval(eta);
                float AcorComb = etaData_2011Acomb->Eval(eta)/etaMC_2011Acomb->Eval(eta);
		weightetaIsoComb = (BcorComb*2.6549+AcorComb*2.3951)/5.05;
		  weightetaEff = lookupScale(eta);
                float BcorTight = etaData_2011Btight->Eval(eta)/etaMC_2011Btight->Eval(eta);
                float AcorTight = etaData_2011Atight->Eval(eta)/etaMC_2011Atight->Eval(eta);
		weightetaIsoTight = (BcorTight*2.6549+AcorTight*2.3951)/5.05;
//		  float Bcortrig= (float) trig2011B->Eval(etaMu);	
//		  float Acortrig= (float) trig2011A->Eval(etaMu);
//		  weightetaTrig= (Bcortrig*2.6549+Acortrig*2.3951)/5.05;
	      weighteta = weightetaIsoComb*weightetaIsoTight*weightetaEff;//*weightetaTrig;
	  }
	  else{
	    weighteta = 1;
	  }
      newBranchEta->Fill();
      typeBranchEta->Fill();
	}
      
      printf("Got It\n");
      
      t->Write("",TObject::kOverwrite);
      return;     
    }
  }
} 


float lookupScale(float eta){
  if(eta>-2.4&&eta<=-2.1){
    return (float) (0.582/0.456);
  }
  else if(eta>-2.1&&eta<=-1.6){
    return (float) (0.811/0.784);
  }
  else if(eta>-1.6&&eta<=-1.2){
    return (float) (0.844/0.800);
  }
  else if(eta>-1.2&&eta<=-0.9){
    return (float) (0.831/0.843);
  }
  else if(eta>-0.9&&eta<=-0.6){
    return (float) (0.875/0.886);
  }
  else if(eta>-0.6&&eta<=-0.3){
    return (float) (0.904/0.919);
  }
  else if(eta>-0.3&&eta<=-0.2){
    return (float) (0.857/0.893);
  }
  else if(eta>-0.2&&eta<=0.2){
    return (float) (0.904/0.921);
  }
  else if(eta>0.2&&eta<=0.3){
    return (float) (0.859/0.892);
  }
  else if(eta>0.3&&eta<=0.6){
    return (float) (0.904/0.918);
  }
  else if(eta>0.6&&eta<=0.9){
    return (float) (0.877/0.883);
  }
  else if(eta>0.9&&eta<=1.2){
    return (float) (0.828/0.847);
  }
  else if(eta>1.2&&eta<=1.6){
    return (float) (0.828/0.796);
  }
  else if(eta>1.6&&eta<=2.1){
    return (float) (0.818/0.770);
  }
  else if(eta>2.1&&eta<=2.4){
    return (float) (0.589/0.443);
  }

  else return 1;

}
