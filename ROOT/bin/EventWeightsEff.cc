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



void readdir(TDirectory *dir,TFile * fc, TFile * ft,optutl::CommandLineParser parser,int topType); 


int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   //parser.addOption("outputFile",optutl::CommandLineParser::kString,"File","corrections.root");
   //parser.addOption("eta",optutl::CommandLineParser::kString,"eta variable","eta1");
   parser.addOption("EffFile",optutl::CommandLineParser::kString,"Efficiencies File","~/MuonEfficiencies2011_42X_DataMC.root");
   parser.addOption("TrigEffFile",optutl::CommandLineParser::kString,"Trig Efficiency File","~/eff_mu.root");
   parser.addOption("brancheta",optutl::CommandLineParser::kString,"Tag and Probe Weight","effWEIGHTeta");
   parser.addOption("branchpt",optutl::CommandLineParser::kString,"Tag and Probe Weight","effWEIGHTpt");
   parser.addOption("branchtrigeta",optutl::CommandLineParser::kString,"Trigger Weight eta","trigWEIGHTeta");
   parser.addOption("branchtrigpt",optutl::CommandLineParser::kString,"Trigger Weight pt","trigWEIGHTpt");

   parser.addOption("csvm1Jet",optutl::CommandLineParser::kString,"1 medium CSV Jet weight","CSVM1Jet");
   parser.addOption("typeTop",optutl::CommandLineParser::kInteger,"Is this a top sample",0);


   parser.addOption("typept",optutl::CommandLineParser::kInteger,"Typept",0);
   parser.addOption("typeeta",optutl::CommandLineParser::kInteger,"Typeeta",2);
   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   printf("Now weighting: %s\n",parser.stringValue("outputFile").c_str());
   TFile *fc = new TFile(parser.stringValue("EffFile").c_str());
   TFile *ft = new TFile(parser.stringValue("TrigEffFile").c_str());
   int topType = parser.integerValue("typeTop");
   readdir(f,fc,ft,parser,topType);

   f->Close();
   fc->Close();
   ft->Close();
} 


void readdir(TDirectory *dir,TFile* fc, TFile* ft,optutl::CommandLineParser parser,int topType) 
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
      TDirectory *subdir = gDirectory;
      readdir(subdir,fc,ft,parser,topType);
      dirsav->cd();
    }
    else if(obj->IsA()->InheritsFrom(TTree::Class())){
      TTree *t = (TTree*)obj;
      
      //get muon pt,eta
      float pt;
      float eta;
      float J1Parton;
      float J2Parton;
      float J3Parton;
      int nJetsBTagCSV2Pt30;
      float weightpt = 1.0;
      float weighteta = 1.0;
      float weighttrigpt = 1.0;
      float weighttrigeta = 1.0;
      float effCSVM = 1;
      int typeeta = 0;
      int typept = 0;
      t->SetBranchAddress("muonPt",&pt);
      t->SetBranchAddress("muonEta",&eta);
      t->SetBranchAddress("nJetsBTagCSV2Pt30",&nJetsBTagCSV2Pt30);
      t->SetBranchAddress("J1JetParton",&J1Parton);
      t->SetBranchAddress("J2JetParton",&J2Parton);
      t->SetBranchAddress("J3JetParton",&J3Parton);
      
      //get branch and add in value
      TBranch *newBranchPt = t->Branch(parser.stringValue("branchpt").c_str(),&weightpt,(parser.stringValue("branchpt")+"/F").c_str());
      TBranch *typeBranchPt = t->Branch("TYPE",&typept,"TYPE/I");

      TBranch *newBranchEta = t->Branch(parser.stringValue("brancheta").c_str(),&weighteta,(parser.stringValue("brancheta")+"/F").c_str());
      TBranch *typeBranchEta = t->Branch("TYPE",&typeeta,"TYPE/I");

      TBranch *newBranchTrigEta = t->Branch(parser.stringValue("branchtrigeta").c_str(),&weighttrigeta,(parser.stringValue("branchtrigeta")+"/F").c_str());
      TBranch *newBranchTrigPt = t->Branch(parser.stringValue("branchtrigpt").c_str(),&weighttrigpt,(parser.stringValue("branchtrigpt")+"/F").c_str());

      TBranch *CSVMEff1Jet = t->Branch(parser.stringValue("csvm1Jet").c_str(),&effCSVM,(parser.stringValue("csvm1Jet")+"/F").c_str());
  
      ///////
      //t->SetBranchAddress("muonEta",&eta);
      //t->SetBranchAddress("",&pt);
      
      //std::vector<double> weights = parser.doubleVector("weights");
      //std::vector<std::string> correctors = parser.stringVector("correctors");

      RooHist *ptMCB_2011B = (RooHist*)fc->Get("MC_combRelPFISO12_2011B_pt__abseta<1.2");
      RooHist *ptMCE_2011B = (RooHist*)fc->Get("MC_combRelPFISO12_2011B_pt__abseta>1.2");

      RooHist *ptDataB_2011B = (RooHist*)fc->Get("DATA_combRelPFISO12_2011B_pt__abseta<1.2");
      RooHist *ptDataE_2011B = (RooHist*)fc->Get("DATA_combRelPFISO12_2011B_pt__abseta>1.2");

      RooHist *ptMCB_2011A = (RooHist*)fc->Get("MC_combRelPFISO12_2011A_pt__abseta<1.2");
      RooHist *ptMCE_2011A = (RooHist*)fc->Get("MC_combRelPFISO12_2011A_pt__abseta>1.2");

      RooHist *ptDataB_2011A = (RooHist*)fc->Get("DATA_combRelPFISO12_2011A_pt__abseta<1.2");
      RooHist *ptDataE_2011A = (RooHist*)fc->Get("DATA_combRelPFISO12_2011A_pt__abseta>1.2");


      //
      printf("Found tree -> weighting\n");

      printf("Entries %i \n",(int)t->GetEntries());

      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
	  weightpt=0.0;
	  if(fabs(eta)<1.2)//abs eta less than 1.2
	    {
	      
	      float Bcor= (float) ptDataB_2011B->Eval(pt)/(ptMCB_2011B->Eval(pt));
	      float Acor= (float) ptDataB_2011A->Eval(pt)/(ptMCB_2011A->Eval(pt));
	      weightpt= (Bcor*2.6549+Acor*2.3951)/5.05;

	      //printf("Gets Here1\n");
		}
	  else if(fabs(eta)>=1.2)//abs eta greater than = 1.2
	    {

	      float Bcor= (float) ptDataE_2011B->Eval(pt)/(ptMCE_2011B->Eval(pt));
	      float Acor= (float) ptDataE_2011A->Eval(pt)/(ptMCE_2011A->Eval(pt));
	      weightpt= (Bcor*2.6549+Acor*2.3951)/5.05;


	    }
	  if(nJetsBTagCSV2Pt30>0){/////////////////////////just a TRIAL NOT FINISHED!!!!!!!!!!!!!!!!!!!
	    if((fabs(J1Parton)== 5||fabs(J2Parton)==5||fabs(J3Parton)==5)||(fabs(J1Parton)==5&&fabs(J2Parton)==5)){
	      if((topType) == 1 ){
		effCSVM = 0.97;
	      }
	      else{
		effCSVM = 0.95;
	      }
	    }
	      else{effCSVM=1.1;}
	    

	  }
	  //printf("TPweight PT %f\n",weightpt);
	  CSVMEff1Jet->Fill();
	  newBranchPt->Fill();
	  typeBranchPt->Fill();
	  //printf("Gets Here3\n");
	}

      //t->Write("",TObject::kOverwrite);
      //printf("Gets Here4\n");
      
      RooHist *etaData_2011B = (RooHist*)fc->Get("DATA_combRelPFISO12_2011B_eta__pt>20");
      RooHist *etaData_2011A = (RooHist*)fc->Get("DATA_combRelPFISO12_2011A_eta__pt>20");
      
      RooHist *etaMC_2011B = (RooHist*)fc->Get("MC_combRelPFISO12_2011B_eta__pt>20");
      RooHist *etaMC_2011A = (RooHist*)fc->Get("MC_combRelPFISO12_2011A_eta__pt>20");

      TGraphAsymmErrors *trig2011A = (TGraphAsymmErrors*) ft->Get("Mu17_eta_2011A");
      TGraphAsymmErrors *trig2011B = (TGraphAsymmErrors*) ft->Get("Mu17_eta_2011B");

      
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  //printf("Gets Here1\n");
	  t->GetEntry(i);
	  weighteta = 0.0;
	  if(pt>20)//abs eta less than 1.2
	    {
	      //printf("Gets Here2\n");
	      //printf("eta: %f, pt: %f\n",eta,pt);
	      float Bcor = etaData_2011B->Eval(eta)/etaMC_2011B->Eval(eta);
	      //printf("Gets Here2.1\n");
	      float Acor = etaData_2011A->Eval(eta)/etaMC_2011A->Eval(eta);
	      weighteta = (Bcor*2.6549+Acor*2.3951)/5.05;
	      //printf("Gets Here3\n");

	      float Bcortrig= (float) trig2011B->Eval(eta);
	      float Acortrig= (float) trig2011A->Eval(eta);
	      //printf("Gets Here4\n");
	      weighttrigeta= (Bcortrig*2.6549+Acortrig*2.3951)/5.05;
	      //printf("TPweight %f Trig weight %f\n",weighteta,weighttrigeta);
	    }

	  newBranchTrigEta->Fill();
	  newBranchEta->Fill();
	  typeBranchEta->Fill();
	}



      t->Write("",TObject::kOverwrite);
      
    }
  }
} 
