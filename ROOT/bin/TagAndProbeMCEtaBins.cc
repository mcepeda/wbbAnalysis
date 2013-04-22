#include "RooRealVar.h"
#include "RooVoigtian.h"
#include "RooPolynomial.h"
#include "RooKeysPdf.h"
#include "RooChebychev.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGlobalFunc.h" 
#include "RooChi2Var.h"
#include "RooRealVar.h"
#include "RooLandau.h"
#include "RooGaussian.h"
#include "RooNumConvPdf.h"
#include "RooMinuit.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooProdPdf.h"
#include "RooFFTConvPdf.h"
#include "TCanvas.h"
#include <TLegend.h>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1.h"
#include "TF1.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include<string>

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(700); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.1);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

  tdrStyle->SetOptTitle(1);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  //tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
   tdrStyle->SetPalette(1,0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}


using namespace RooFit; 
using namespace std;
struct fitResult {
  double pass;
  double passError;
  double fail;
  double failError;
  double efficiency;
  double efficiencyError;
};




void createWorspace(RooWorkspace *w,const std::string& signalFile,const std::string& treeDir,const std::string& cut);
RooDataHist *createDataSet(RooWorkspace *w,const std::string& prefix,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut);
void createHistPdfSig(const std::string& name,RooWorkspace *w,const std::string& var,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut);
void createHistPdfBkd(const std::string& name,RooWorkspace *w,const std::string& var,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut);
fitResult fitMC(RooWorkspace*,RooDataHist*,RooDataHist*);
fitResult fit(RooWorkspace*,RooDataHist*);
TH1F *createHist(RooWorkspace *w,const std::string& name,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut,float factor,float lumi);
float constraint(RooWorkspace *w,const std::string& name,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut,float factor,float lumi);
void run(const std::string& pt_lo,const std::string& pt_hi,const std::string& eta_lo,const std::string& eta_hi,const std::string& savefile,float factor,const std::string& dirMC,const std::string& dirData, const std::string& inTree,float effData,float effMC, const std::string& cut);

//specify binning here
int lo = 52;
int hi = 120;
int bin = 17;
int binmc = 17;
float effData = 0;
float effMC = 0;
float lumi = 5000; //inverse pico barns 

int main(int argc, char* argv[]){ 

  TFile *eff = new TFile("eff.root","RECREATE");
  eff->cd();
  TH1F *pteff =  new TH1F("pteff","",20,0,100); 
  string dirMC = "/scratch/ojalvo/sandbox_ZeeTP_5_18/";
  string dirData = "/scratch/ojalvo/sandbox_6_1_ZEE/ZEETPData.root";
  string inTree = "tagAndProbeelectronNuJetISOPFPOGRec/tagAndProbeTree";
  //string inTree = "tagAndProbeelectronNuJetISOPFWP70/tagAndProbeTree";
  //run("35","90","-2.4","-2.0","__neg2420_in",1,dirMC,dirData,inTree,effData,effMC,"nJets>-1");
  //run("35","90","-2.0","-1.6","__neg2016_in",1,dirMC,dirData,inTree,effData,effMC,"nJets>-1");
  //run("35","90","-1.6","-1.2","__neg1612_in",1,dirMC,dirData,inTree,effData,effMC,"nJets>-1");
  //run("35","90","-1.2","-0.8","__neg1208_in",1,dirMC,dirData,inTree,effData,effMC,"nJets>-1");
  run("35","90","-2.5","-2.1","__neg240_2j",.87,dirMC,dirData,inTree,effData,effMC,"nJets>-1&&");
  /*  run("35","90","2.1","2.5","__024_2j",1,dirMC,dirData,inTree,effData,effMC,"nJets>-1&&");

  run("35","90","-2.4","0.00","__neg240_1j",1,dirMC,dirData,inTree,effData,effMC,"nJets>0&&");
  run("35","90","0.00","2.4","__024_1j",1,dirMC,dirData,inTree,effData,effMC,"nJets>0&&");

  run("35","90","-2.4","0","__neg240_0j",1,dirMC,dirData,inTree,effData,effMC,"nJets==0&&");
  run("35","90","0.00","2.4","__024_0j",1,dirMC,dirData,inTree,effData,effMC,"nJets==0&&");

  */

}
void run(const std::string& pt_lo,const std::string& pt_hi,const std::string& eta_lo,const std::string& eta_hi,const std::string& savefile,float factor,const std::string& dirMC,const std::string& dirData, const std::string& inTree,float effData,float effMC,const std::string& cut){
  effData = 5;
  effMC = 5;
  FILE *file; 
  file = fopen("file.txt","a+");

  gROOT->SetBatch(kTRUE);

  //gROOT->SetStyle("Plain");
  setTDRStyle();

  string dir = "/scratch/ojalvo/sandbox_ZeeTP_5_18/";
  RooWorkspace *w = new RooWorkspace("w","workspace");
  string cutPassBarrel;
  string cutFailBarrel;
  string var = "IDLPass";


  printf("pt_lo: %s eta_lo:%s \n",pt_lo.c_str(),eta_lo.c_str());

  cutPassBarrel = "("+cut+"matchPass==1&&(eta>"+eta_lo+"&&eta<"+eta_hi+")&&"+var+"==1&&pt>"+pt_lo+"&&pt<"+pt_hi+")";
  cutFailBarrel = "("+cut+"matchPass==1&&(eta>"+eta_lo+"&&eta<"+eta_hi+")&&"+var+"!=1&&pt>"+pt_lo+"&&pt<"+pt_hi+")";

  printf("%s \n",cutPassBarrel.c_str());
  printf("%s \n",cutFailBarrel.c_str());


  createWorspace(w,dirMC+"ZeeTP_Data.root",inTree,cutPassBarrel);
  createHistPdfSig("DYee",w,"mcSample",dirMC+"DYee.root",inTree,"__WEIGHT__",cutPassBarrel);
  createHistPdfSig("DYtautau",w,"mcSample",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutPassBarrel);
  createHistPdfSig("TOP",w,"mcSample",dirMC+"TOP.root",inTree,"__WEIGHT__",cutPassBarrel);
  createHistPdfSig("QCDEM",w,"mcSample",dirMC+"QCDEM30.root",inTree,"__WEIGHT__",cutPassBarrel);
  createHistPdfSig("W",w,"mcSample",dirMC+"W.root",inTree,"__WEIGHT__",cutPassBarrel);

  createHistPdfSig("DYeeFail",w,"mcSample",dirMC+"DYee.root",inTree,"__WEIGHT__",cutFailBarrel);
  createHistPdfSig("DYtautauFail",w,"mcSample",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutFailBarrel);
  createHistPdfSig("TOPFail",w,"mcSample",dirMC+"TOP.root",inTree,"__WEIGHT__",cutFailBarrel);
  createHistPdfSig("QCDEMFail",w,"mcSample",dirMC+"QCDEM30.root",inTree,"__WEIGHT__",cutFailBarrel);
  createHistPdfSig("WFail",w,"mcSample",dirMC+"W.root",inTree,"__WEIGHT__",cutFailBarrel);  

  TFile *tmp = new TFile("tmp.root","RECREATE");
  TFile *Data = new TFile(dirData.c_str());
  TTree* tree = (TTree*)Data->Get(inTree.c_str());
  TH1F *hd = new TH1F("hd","hd", bin, lo, hi);
  tree->Draw("mass>>hd",cutPassBarrel.c_str());
  printf("Passed Events in Data %f\n",hd->Integral());  

  float nData = hd->Integral();
  tmp->cd();
  RooDataHist *data = new RooDataHist("data","dataset with mass",*w->var("mass"),hd);

  TH1F *hdFail = new TH1F("hdFail","hdFail", bin, lo, hi);
  tree->Draw("mass>>hdFail",cutFailBarrel.c_str());
  printf("Failed Events in Data %f\n",hdFail->Integral());
  RooDataHist *dataFail = new RooDataHist("dataFail","Fail dataset with mass",*w->var("mass"),hdFail);

  delete hd;
  delete hdFail;

  /////////////////Constraints

  double DYeeYield = constraint(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  double bkgrdYield =constraint(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",("(mass<70&&"+cutPassBarrel+")").c_str(),factor,lumi);

  w->factory(TString::Format("nExp[%f,0,%f]",bkgrdYield,bkgrdYield*1.33));
 
  double DYtautauYield = constraint(w,"DYtautau",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  w->factory(TString::Format("nTauTau[%f,%f,%f]",DYtautauYield,DYtautauYield*0.66,DYtautauYield*1.33)); 

  
  double TOPYield = constraint(w,"TOP",dirMC+"TOP.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  w->factory(TString::Format("nTOP[%f,%f,%f]",TOPYield,TOPYield*0.66,TOPYield*1.33)); 
  
  double WYield = constraint(w,"W",dirMC+"W.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  w->factory(TString::Format("nW[%f,0,%f]",WYield,WYield*1.33)); 
  
  double DYeeFailYield = constraint(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);


  double bkgrdFailYield = constraint(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",("(mass<70&&"+cutFailBarrel+")").c_str(),factor,lumi);
  w->factory("nExpFail[0,0,10000]"); 
  printf("Background Fail Region Yield %f\n",bkgrdFailYield);
  double DYtautauFailYield = constraint(w,"DYtautau",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
  w->factory(TString::Format("nTauTauFail[%f,%f,%f]",DYtautauFailYield,DYtautauFailYield*0.66,DYtautauFailYield*1.33)); 
  
  double TOPFailYield = constraint(w,"TOP",dirMC+"TOP.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
  w->factory(TString::Format("nTOPFail[%f,%f,%f]",TOPFailYield,TOPFailYield*0.66,TOPFailYield*1.33)); 
  double WFailYield = constraint(w,"W",dirMC+"W.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
    w->factory(TString::Format("nWFail[%f,1,%f]",WFailYield,WFailYield*1.5)); 

    printf("Yields for Z->tautau: %f Yield for Background: %f\n",DYtautauYield,bkgrdYield);


  ////////end Yield estimates

  //---------------------
  w->factory("Gaussian::gauss(mass,mg[3,0,6],sigmaGaus[10,0.5,15])") ;
  w->factory("eff[0.5,0.3,1]");
  w->factory(TString::Format("nTotal[%f,%f,100000000]",1.2*DYeeYield,1.1*DYeeYield));

  w->factory("FCONV::SigGaus(mass,DYee,gauss)") ;
  w->factory("prod::nSig(eff,nTotal)");

  w->factory("CBShape::CB(mass,m2[-2,-5,1],s[5,0,10],a[5,0,10],n[0.5,0,0.9])");

  w->factory(TString::Format("nWFail[%f,%f,%f]",WFailYield,WFailYield*0.66,WFailYield*1.33));
  //w->factory("SUM::sum(nSig*SigGaus,nTauTau*DYtautau,nTOP*TOP,nExp*background1,nW*W)") ;
  ///---------------------SUM Pdfs used for fitting Pass Region here--------------------------- (if fits fail to converge it may be necessary to get rid of some pdfs)
  w->factory("SUM::sum(nSig*SigGaus,nTauTau*DYtautau,nTOP*TOP,nExp*background1,nW*W)") ;
  //w->factory("SUM::sum(nTotal*DYee,nTauTau[1,0,1000000]*DYtautau,nTOP[1,0,1000000]*TOP,nExp[1,1,10000000]*background1)") ;

  RooFormulaVar nSigFail("nSigFail","(1-eff)*nTotal",RooArgSet(*w->var("eff"),*w->var("nTotal")));

  w->import(nSigFail);

  w->factory("Gaussian::gaussFail(mass,mgFail[0,-5,5],sigmaGausFail[1.5,1,3.5])") ;
  w->factory("FCONV::SigGausFail(mass,DYeeFail,gaussFail)") ;
  w->factory("CBShape::CBFail(mass,m2[-2,-5,1],s[5,0,10],a[5,0,10],n[0.5,0,0.9])");

  ///---------------------SUM Pdfs used for fitting Fail Region here--------------------------- (if fits fail to converge it may be necessary to get rid of some pdfs)
  w->factory("SUM::sumFail(nSigFail*SigGausFail,nTauTauFail*DYtautauFail,nTOPFail*TOPFail,nExpFail*background1Fail,nWFail*WFail)") ;

  tmp->cd();

  //-----Define Categories for a COMBINED Fit
  RooDataHist *combData = new RooDataHist("combData","combined data",*w->var("mass"),Index(*w->cat("sample")),Import("Pass",*data),Import("Fail",*dataFail)) ;
  w->factory("SIMUL::smodel(sample,Pass=sum,Fail=sumFail)");

  fitResult result =  fit(w,combData); ///fit done in "fit" function
  printf("--------------Efficiency = %f +- %f-----------------\n",w->var("eff")->getVal(),w->var("eff")->getError());

  //write Data fit to 
  fprintf(file,"%s eta %s Pt Range %s to %s: Efficiency Data = %f +- %f \n",eta_lo.c_str(),eta_hi.c_str(),pt_lo.c_str(),pt_hi.c_str(),w->var("eff")->getVal(),w->var("eff")->getError()); 

 //------- Plot Pdfs------ 
 RooPlot *frameDhistP = w->var("mass")->frame();
 w->pdf("TOP")->plotOn(frameDhistP,LineColor(kMagenta));
 w->pdf("DYee")->plotOn(frameDhistP,LineColor(kRed));
 w->pdf("DYtautau")->plotOn(frameDhistP,LineColor(kBlue));
 TCanvas *Tcan = new TCanvas("Tcan","CCCC");
 frameDhistP->GetXaxis()->SetTitle(TString::Format("PASS Barrel %s>eta>%s  Probe Pt%s      Mass",eta_lo.c_str(),eta_hi.c_str(),savefile.c_str()));
 frameDhistP->Draw();
 Tcan->SaveAs(TString::Format("./FitPlots/Pdfs_%sPASS.png",savefile.c_str()));
 delete Tcan;

 RooPlot *frameDhistF = w->var("mass")->frame();
 w->pdf("TOPFail")->plotOn(frameDhistF,LineColor(kMagenta));
 w->pdf("DYeeFail")->plotOn(frameDhistF,LineColor(kRed));
 w->pdf("DYtautauFail")->plotOn(frameDhistF,LineColor(kBlue));
 TCanvas *Tcan2 = new TCanvas("Tcan2","CCCC");

 frameDhistF->GetXaxis()->SetTitle(TString::Format("FAIL Barrel   Probe Pt%s      Mass",savefile.c_str()));
 frameDhistF->Draw();
 Tcan2->SaveAs(TString::Format("./FitPlots/Pdfs_%sFAIL.png",savefile.c_str()));
 delete Tcan2;

  ////-----Finish Plotting PDFs-------------
  ////-----Begin Plotting Pass Region--------
  RooPlot *frameDP = w->var("mass")->frame();
  *combData->plotOn(frameDP,Cut("sample==sample::Pass"));
  w->cat("sample=Pass");
  w->pdf("smodel")->plotOn(frameDP,Slice(*w->cat("sample"),"Pass"),ProjWData(*combData),DrawOption("L9"));
  w->pdf("smodel")->plotOn(frameDP,Slice(*w->cat("sample"),"Pass"),Components("background1"),ProjWData(*combData),LineColor(kRed),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDP,Components("SigGaus"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combData),LineColor(kOrange),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDP,Components("TOP"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combData),LineColor(kMagenta),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDP,Components("DYtautau"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combData),LineColor(kCyan),LineStyle(kDashed)) ;

  TCanvas *cDP = new TCanvas("cDP","CCCC");

  frameDP->GetXaxis()->SetTitle(TString::Format("PASS   Probe %s      Mass",savefile.c_str()));
  frameDP->Draw();
  cDP->SaveAs(TString::Format("./FitPlots/Pass_%s.png",savefile.c_str()));
  delete cDP;
  delete frameDP;

  ////-----Begin Plotting Fail Region--------
  RooPlot *frameDF = w->var("mass")->frame();
  *combData->plotOn(frameDF,Cut("sample==sample::Fail"));
  w->pdf("smodel")->plotOn(frameDF,Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),DrawOption("C"));
  w->pdf("smodel")->plotOn(frameDF,Components("gaussFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kSpring),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDF,Components("background1Fail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kRed),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDF,Components("SigGausFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kOrange),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDF,Components("DYtautauFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kCyan),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDF,Components("TOPFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kMagenta),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameDF,Components("WFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combData),LineColor(kCyan),LineStyle(kDashed)) ;
  TCanvas *cDF = new TCanvas("cDF","CCCC");
  frameDF->GetXaxis()->SetTitle(TString::Format("Fail Probe %s    Mass",savefile.c_str()));
  frameDF->Draw();
  cDF->SaveAs(TString::Format("./FitPlots/Fail_%s.png",savefile.c_str()));
  delete cDF;
  delete frameDF;
  delete tmp;

  ////////Create MC to Fit
  //w->factory("eff2[0.5,0.001,1]");
  TFile *tmp1 = new TFile("tmp1.root","RECREATE");
  TH1F *MCfit = new TH1F("MCfit","MCfit", bin, lo, hi);
  TH1F *MCfitFail = new TH1F("MCfitFail","MCfit", bin, lo, hi);

  printf("-----%f------- \n",MCfit->Integral(0,60));

  TH1F *DYeeHist = createHist(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  TH1F *DYtautauHist = createHist(w,"DYtautau",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  TH1F *TOPHist = createHist(w,"TOP",dirMC+"TOP.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);
  TH1F *WHist = createHist(w,"W",dirMC+"W.root",inTree,"__WEIGHT__",cutPassBarrel,factor,lumi);

  MCfit->Add(DYeeHist);
  MCfit->Add(DYtautauHist);
  MCfit->Add(TOPHist);
  MCfit->Add(WHist);

  TH1F *DYeeHistFail = createHist(w,"DYee",dirMC+"DYee.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
  TH1F *DYtautauHistFail = createHist(w,"DYtautau",dirMC+"DYtautau.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
  TH1F *TOPHistFail = createHist(w,"TOP",dirMC+"TOP.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);
  TH1F *WHistFail = createHist(w,"W",dirMC+"W.root",inTree,"__WEIGHT__",cutFailBarrel,factor,lumi);

  MCfitFail->Add(DYeeHistFail);
  MCfitFail->Add(DYtautauHistFail);
  MCfitFail->Add(TOPHistFail);
  MCfitFail->Add(WHistFail);

  RooDataHist *MCFail = new RooDataHist("MCfail","MC Fail",*w->var("mass"),MCfitFail);
  RooDataHist *MCPass = new RooDataHist("MCHistfit","Weighted MC with mass",*w->var("mass"),MCfit);

  RooDataHist *combDataMC = new RooDataHist("combDataMC","combined dataMC",*w->var("mass"),Index(*w->cat("sample")),Import("Pass",*MCPass),Import("Fail",*MCFail)) ;

  fitResult resultMC  =  fit(w,combDataMC);

  fprintf(file,"%s eta %s Pt Range %s to %s: Efficiency MC = %f +- %f \n",eta_lo.c_str(),eta_hi.c_str(),pt_lo.c_str(),pt_hi.c_str(),w->var("eff")->getVal(),w->var("eff")->getError()); 


  ////////////////////////////////////////////////

  RooPlot *frameMCPass = w->var("mass")->frame();

  *combDataMC->plotOn(frameMCPass,Cut("sample==sample::Pass"));
  

  w->cat("sample=Pass");
  printf("getsHere Line466 \n");
  w->pdf("smodel")->plotOn(frameMCPass,Slice(*w->cat("sample"),"Pass"),ProjWData(*combDataMC),DrawOption("L9"));
  w->pdf("smodel")->plotOn(frameMCPass,Slice(*w->cat("sample"),"Pass"),Components("background1"),ProjWData(*combDataMC),LineColor(kRed),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameMCPass,Components("SigGaus"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combDataMC),LineColor(kOrange),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameMCPass,Components("TOP"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combDataMC),LineColor(kMagenta),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameMCPass,Components("DYtautau"),Slice(*w->cat("sample"),"Pass"),ProjWData(*combDataMC),LineColor(kCyan),LineStyle(kDashed)) ;


  TCanvas *cMCP = new TCanvas("cMCP","CCCC");


  frameMCPass->GetXaxis()->SetTitle(TString::Format("MC PASS  Probe %s      Mass",savefile.c_str()));
  frameMCPass->Draw();
  cMCP->SaveAs(TString::Format("./FitPlots/PassMC_%s.png",savefile.c_str()));

  delete cMCP;
  delete frameMCPass;


  ////////////////////////////////////////////////


  RooPlot *frameMCFail = w->var("mass")->frame();
  *combDataMC->plotOn(frameMCFail,Cut("sample==sample::Fail"));
  printf("Passes to Here Line 498 ----\n");
  //RooAbsPdf *sumFail = w->pdf("sumFail");
  w->cat("sample=Fail");
  w->pdf("smodel")->plotOn(frameMCFail,Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),DrawOption("C"),Name("test"));
  w->pdf("smodel")->plotOn(frameMCFail,Components("gaussFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kSpring),LineStyle(kDashed),Name("h_gaussFail")) ;
  w->pdf("smodel")->plotOn(frameMCFail,Components("background1Fail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kRed),LineStyle(kDashed),Name("h_background1Fail")) ;
  w->pdf("smodel")->plotOn(frameMCFail,Components("SigGausFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kOrange),LineStyle(kDashed),Name("h_SigGausFail")) ;
  w->pdf("smodel")->plotOn(frameMCFail,Components("DYtautauFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kCyan),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameMCFail,Components("TOPFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kMagenta),LineStyle(kDashed)) ;
  w->pdf("smodel")->plotOn(frameMCFail,Components("WFail"),Slice(*w->cat("sample"),"Fail"),ProjWData(*combDataMC),LineColor(kCyan),LineStyle(kDashed)) ;

  TCanvas *cMCF = new TCanvas("cMCF","CCCC");





    frameMCFail->GetXaxis()->SetTitle(TString::Format("Fail   Probe %s      Mass",savefile.c_str()));
    frameMCFail->Draw();

   TLegend* legend = new TLegend(0.2,0.9,0.5,0.5);
   RooCurve* testhist = frameMCFail->getCurve("test");
   legend->AddEntry( testhist,  "data", "P"); // For whatever reason, legend does not like RooHist?

   RooCurve* gausshist = frameMCFail->getCurve("h_gaussFail");
   legend->AddEntry( gausshist,  "Gauss", "L");
   RooCurve* bckghist = frameMCFail->getCurve("h_background1Fail");
   legend->AddEntry( bckghist,  "Bckg1", "L");
   RooCurve* sigGaushist = frameMCFail->getCurve("h_SigGausFail");
   legend->AddEntry( sigGaushist,  "sigGaus", "L");
   legend->SetFillColor(0);
        legend->SetFillStyle(0);
   legend->Draw();




    cMCF->SaveAs(TString::Format("./FitPlots/FailMC_%s.png",savefile.c_str()));



  delete cMCF;
  delete frameMCFail;
  delete tmp;
  //delete hd;
  //delete hdFail;  
  


  printf("factor : %f\n",nData/DYeeYield);


  fclose(file); /*done!*/ 
  //getchar(); /* pause and wait for key */ 
  //return 0; 

  ///////////////////////////////////////////////

  //delete tmp1;


}

void createWorspace(RooWorkspace *w,const std::string& signalFile,const std::string& treeDir,const std::string& cut) {
  using namespace RooFit;
  //Create a common variable for the invariant mass                                                                                                                                                            
  RooRealVar mass("mass","mass",91,lo,hi);
  mass.setBins(bin); //needed to correctly do fit
  w->import(mass);
  w->var("mass")->setPlotLabel("l^{+} l^{-} Mass [GeV/c^{2}]");
  w->factory("sample[Pass=1,Fail=0]");
  //CREATE THE CATEGORY                                                                                                                                                                                        
  w->factory(TString::Format("%s[pass=1,fail=0]","prefix"));
  w->factory("__WEIGHT__[0.5,0.,100]");

  w->factory("b1[0,-7,0]");
  w->factory("b2[0,-7,0]");
  w->factory("NpassB[1,1,10000000.]");
  w->factory("NfailB[1,1,100000.]");
  RooRealVar bkrdRange("bkrdRange","bkrdRange",lo,hi);
  w->import(bkrdRange);
  //declare background PDFS
  w->factory("Exponential::background1(mass,b1)");
  w->factory("Exponential::background1Fail(mass,b2)");


  printf("Background pdfs declared\n");


}



fitResult fitMC(RooWorkspace* w, RooDataHist* combData,  RooDataHist* dataFail )
{ 

   TFile *ff = new TFile("tmp2.root","RECREATE");
   ff->cd();
   //RooRealVar *mass = w->var("mass");
   w->pdf("smodel")->fitTo(*combData,SumW2Error(kFALSE));
   fitResult result;
   result.efficiency = w->var("eff")->getVal();
   result.efficiencyError = w->var("eff")->getError();
  //int stats;
  //result.status = stats;
  //printf("--------------Efficiency = %f +- %f-----------------\n",w->var("eff")->getVal(),w->var("eff")->getError());
  //fprintf(file,"%s","This is just an example :)"); /*writes*/ 

  ff->Close();
  return result;

}


fitResult fit(RooWorkspace* w , RooDataHist* combData)
{ 

   TFile *ff = new TFile("tmp2.root","RECREATE");
   ff->cd();

   //RooRealVar *mass = w->var("mass");
   *w->pdf("smodel")->fitTo(*combData,SumW2Error(kFALSE));
   fitResult result;
   
   //int stats;
   //stats = result.status();
   //printf("the result status = %i\n",stats);


   result.efficiency = w->var("eff")->getVal();
   result.efficiencyError = w->var("eff")->getError();
   printf("--------------Efficiency = %f +- %f-----------------\n",w->var("eff")->getVal(),w->var("eff")->getError());
   ff->Close();
   return result;

}



///create a function that calls create dataset and creates a roohistPDF
void createHistPdfSig(const std::string& name,RooWorkspace *w,const std::string& var,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut)
{
  TFile *ff = new TFile("tmp2.root","RECREATE");
  ff->cd();
  TFile *f = new TFile(file.c_str());
  TTree *t = (TTree*)f->Get(treeDir.c_str());
  //t->Print();
  TH1F *he = new TH1F("he","he",binmc,lo,hi);
  //RooRealVar *mass = w->var("mass");

  t->Draw("mass>>he",cut.c_str());
  

  RooDataHist *mc = new RooDataHist((name+"mc").c_str(),"mc",*w->var("mass"),he);//binned dat
  //w->factory("RooHistPdf::mcPdf(mass,*mc,he)");
  RooHistPdf *MCPdf = new RooHistPdf(name.c_str(),name.c_str(),*w->var("mass"),*mc,2);//set inerpolation here
  int x = MCPdf->getInterpolationOrder();

  w->import(*MCPdf,RenameConflictNodes(name.c_str())); 
  
  delete MCPdf;
  delete mc;
  delete t;
  delete he;
  ff->Close();
  delete ff;


}

void createHistPdfBkd(const std::string& name,RooWorkspace *w,const std::string& var,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut)
{
  TFile *ff = new TFile("tmp2.root","RECREATE");
  ff->cd();
  TFile *f = new TFile(file.c_str());
  TTree *t = (TTree*)f->Get(treeDir.c_str());
  //t->Print();
  TH1F *he = new TH1F("he","he",binmc,lo,hi);

  t->Draw("mass>>he",cut.c_str());
  

  RooDataHist *mc = new RooDataHist((name+"mc").c_str(),"mc",*w->var("bkrdRange"),he);//binned dat

  RooHistPdf *MCBkdPdf = new RooHistPdf(name.c_str(),name.c_str(),*w->var("bkrdRange"),*mc);

  w->import(*MCBkdPdf,RenameConflictNodes(name.c_str())); 
  
  delete MCBkdPdf;
  delete mc;
  delete t;
  delete he;
 ff->Close();
  delete ff;


}


RooDataHist *createDataSet(RooWorkspace *w,const std::string& var,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut)
{
  TFile *f = new TFile(file.c_str());
  TFile* tmp = new TFile("tmp.root","RECREATE");

  tmp->cd();
  TTree *t = (TTree*)f->Get(treeDir.c_str());
  TTree *t2 = t->CopyTree(cut.c_str());


  RooRealVar * mass = w->var("mass");
  RooRealVar NSig("NSig","NSig",50000,0,1000000);
  TH1F *dat = new TH1F("dat","dat",600,lo,hi);

  RooDataHist *data = new RooDataHist("data", "dsd",*mass,dat);
  RooDataHist *data1 = new RooDataHist("data1", "dsd",*mass,dat);

  
  data->Print();
  printf("Gets Here\n");
  w->import(*data1);
  
  return data;
  delete t2;
  //ff->Close();
}

TH1F *createHist(RooWorkspace *w,const std::string& name,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut, float factor, float lumi)
{
  TFile *f = new TFile(file.c_str());
  TFile* tmp = new TFile("tmp.root","RECREATE");


  TTree *t = (TTree*)f->Get(treeDir.c_str());
  TH1F *DYee = new TH1F(name.c_str(),name.c_str(), bin, lo, hi);

  t->Draw(("mass>>"+name).c_str(),TString::Format("(%s)*__WEIGHT__*%f*%f",cut.c_str(),lumi,factor));
  printf("Line 617-%s----------- %f\n",name.c_str(),DYee->Integral());


  return DYee;
  delete t; 
  delete f;
  delete tmp;

}


float constraint(RooWorkspace *w,const std::string& name,const std::string& file,const std::string& treeDir,const std::string& weightString,const std::string& cut, float factor, float lumi)
{
  TFile *f = new TFile(file.c_str());
  TFile* tmp = new TFile("tmp.root","RECREATE");


  TTree *t = (TTree*)f->Get(treeDir.c_str());
  
  TH1F *DYee = new TH1F(name.c_str(),name.c_str(), bin, lo, hi);

  t->Draw(("mass>>"+name).c_str(),TString::Format("(%s)*__WEIGHT__*%f*%f",cut.c_str(),lumi,factor));  


  printf("Integral of-%s----------- %f\n",name.c_str(),DYee->Integral());
  float Val = DYee->Integral();

  return Val;
  delete t; 
  delete f;
  delete tmp;

}
