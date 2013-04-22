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

int main (int argc, char* argv[]) 
{
   optutl::CommandLineParser parser ("Sets Event Weights in the ntuple");
   parser.addOption("branchHLTMT",optutl::CommandLineParser::kString,"HLTMT Weight","wtHLTMTEff");
   parser.addOption("branchIDISOHLT",optutl::CommandLineParser::kString,"IDISOHLT Weight","wtIDISOHLTEff");
   parser.parseArguments (argc, argv);
   
   TFile *f = new TFile(parser.stringValue("outputFile").c_str(),"UPDATE");
   printf("Now weighting: %s\n",parser.stringValue("outputFile").c_str());
   //int topType = parser.integerValue("typeTop");
   readdir(f,parser);

   f->Close();
} 


void readdir(TDirectory *dir,optutl::CommandLineParser parser) 
{
	TH1D* histoRATIODERATIO = new TH1D("histoRATIODERATIO","",50,-2.5,2.5);
   histoRATIODERATIO->SetBinContent(1,0.6348668);
   histoRATIODERATIO->SetBinContent(2,0.6984935);
   histoRATIODERATIO->SetBinContent(3,0.7515663);
   histoRATIODERATIO->SetBinContent(4,0.8101778);
   histoRATIODERATIO->SetBinContent(5,0.8562419);
   histoRATIODERATIO->SetBinContent(6,0.9143828);
   histoRATIODERATIO->SetBinContent(7,0.9238404);
   histoRATIODERATIO->SetBinContent(8,0.9508163);
   histoRATIODERATIO->SetBinContent(9,0.9211096);
   histoRATIODERATIO->SetBinContent(10,0.9900274);
   histoRATIODERATIO->SetBinContent(11,0.9997889);
   histoRATIODERATIO->SetBinContent(12,1.009455);
   histoRATIODERATIO->SetBinContent(13,0.9746005);
   histoRATIODERATIO->SetBinContent(14,1.008687);
   histoRATIODERATIO->SetBinContent(15,1.004348);
   histoRATIODERATIO->SetBinContent(16,0.9966097);
   histoRATIODERATIO->SetBinContent(17,0.9952458);
   histoRATIODERATIO->SetBinContent(18,1.000851);
   histoRATIODERATIO->SetBinContent(19,1.004353);
   histoRATIODERATIO->SetBinContent(20,1.008516);
   histoRATIODERATIO->SetBinContent(21,0.9975348);
   histoRATIODERATIO->SetBinContent(22,0.9882531);
   histoRATIODERATIO->SetBinContent(23,0.9886724);
   histoRATIODERATIO->SetBinContent(24,1.006006);
   histoRATIODERATIO->SetBinContent(25,1.000954);
   histoRATIODERATIO->SetBinContent(26,1.017708);
   histoRATIODERATIO->SetBinContent(27,0.9971436);
   histoRATIODERATIO->SetBinContent(28,0.9959989);
   histoRATIODERATIO->SetBinContent(29,0.9983351);
   histoRATIODERATIO->SetBinContent(30,0.9895101);
   histoRATIODERATIO->SetBinContent(31,0.9936764);
   histoRATIODERATIO->SetBinContent(32,1.000436);
   histoRATIODERATIO->SetBinContent(33,1.006346);
   histoRATIODERATIO->SetBinContent(34,0.972092);
   histoRATIODERATIO->SetBinContent(35,1.007151);
   histoRATIODERATIO->SetBinContent(36,0.9862933);
   histoRATIODERATIO->SetBinContent(37,0.9895663);
   histoRATIODERATIO->SetBinContent(38,0.9811242);
   histoRATIODERATIO->SetBinContent(39,1.011445);
   histoRATIODERATIO->SetBinContent(40,0.9660705);
   histoRATIODERATIO->SetBinContent(41,0.8887965);
   histoRATIODERATIO->SetBinContent(42,0.9125232);
   histoRATIODERATIO->SetBinContent(43,0.9381321);
   histoRATIODERATIO->SetBinContent(44,0.9148951);
   histoRATIODERATIO->SetBinContent(45,0.8739418);
   histoRATIODERATIO->SetBinContent(46,0.8321065);
   histoRATIODERATIO->SetBinContent(47,0.8234398);
   histoRATIODERATIO->SetBinContent(48,0.7331089);
   histoRATIODERATIO->SetBinContent(49,0.7106306);
   histoRATIODERATIO->SetBinContent(50,0.6373381);



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

      float weightIDISOHLT=-1;      
      float weightHLTMT=-1;

      t->SetBranchAddress("ptEle",&ptEle);
      t->SetBranchAddress("etaEle",&etaEle);
      TBranch *newBranchIDISOHLT = t->Branch(parser.stringValue("branchIDISOHLT").c_str(),&weightIDISOHLT,(parser.stringValue("branchIDISOHLT")+"/F").c_str());
      TBranch *newBranchHLTMT = t->Branch(parser.stringValue("branchHLTMT").c_str(),&weightHLTMT,(parser.stringValue("branchHLTMT")+"/F").c_str());

      printf("Found tree -> weighting\n");
      printf("Entries %i \n",(int)t->GetEntries());
     
      for(Int_t i=0;i<t->GetEntries();++i)
	{
	  t->GetEntry(i);
		  weightIDISOHLT = lookupScale(etaEle,ptEle);

                  int binMT=histoRATIODERATIO->FindBin(etaEle);
                  weightHLTMT=histoRATIODERATIO->GetBinContent(binMT);

	         newBranchIDISOHLT->Fill();
                 newBranchHLTMT->Fill();

          if(i%100000==1) printf("\t\t %4.0d Eta=%4.2f  Pt=%4.2f ---> T&P=%4.2f MT=%4.2f \n",i,etaEle,ptEle,weightIDISOHLT,weightHLTMT);

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






