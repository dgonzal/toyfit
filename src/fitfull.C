// c++ classes
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>  
#include <sstream>


//RooFit
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGamma.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooHist.h"
#include "RooMCStudy.h"
#include "RooFitResult.h"
#include "TMatrixDSym.h"
//#include "RooCmdArg.h"

//ROOT 
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TH2F.h"
#include "TString.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"


//MyCLasses
#include "hit_info.h"
#include "pion20_hits.h"

using namespace std;
using namespace RooFit;

void set_style();

int main(){


  RooRealVar zero("zero","",0,0,150); 

  //Number of Events and particles per shower
  //unsigned int events = 10;
  unsigned int particles = 10000; 

  //input parameters for the distribution
  double alpha_double = 2.38;
  double beta_double = 0.092;
  double lambda_double = 2.21;
  double theta_double = 1.12;
  double c_frac = 0.3;
  //RooRealVar fsig("fsig","signal fraction",0.3) ;

  TString epsname = "test2.ps";

  
  hit_info multi_shower("fullsim/example_pi20Gev.root");
  //hit_info multi_shower("fullsim/example_ele_20GeV.root");
  
 
  double beta_gflash_factor = 1.49;
  double theta_gflash_factor = 16.42;

  beta_double = beta_double/beta_gflash_factor;
  theta_double = theta_double/theta_gflash_factor;
  
  // --- Observable --- 
  RooRealVar depth("depth","depth",0,0,180); 
  
  // --- Weight ---
  RooRealVar* weight = new RooRealVar("weight","1/E dE/dr", 0.0, 1.0);
 
  // --- Gamma pdf --- 
  //if one of the Values is set to const you can not ask for it in RooFitModel, otherwise the program will crash
  RooRealVar alpha("alpha","alpha",alpha_double,0.001,10.);
  alpha.removeMax(); // set infinite range
  RooRealVar beta("beta","beta",1./beta_double,0.001,100.) ;  
  beta.removeMax(); // set infinite range

  RooRealVar mu("mu","mu",0); // no ranges means variable is fixed in fit
  
  RooRealVar lambda("lambda","lambda",lambda_double,0.001,10.);
  lambda.removeMax(); // set infinite range
  RooRealVar theta("theta","theta",1./theta_double,0.001,100.) ;  
  theta.removeMax(); // set infinite range

  RooGamma gamma_one("gamma_one","gamma_one pdf",depth,alpha,beta,mu) ; 
  RooGamma gamma_two("gamma_two","gamma_two pdf",depth,lambda,theta,mu) ; 

  RooRealVar fsig("fsig","signal fraction",c_frac,0.2,.8);
  //RooRealVar fsig("fsig","signal fraction",c_frac) ;

  RooAddPdf model("model","model",RooArgList(gamma_one,gamma_two),fsig) ;
   
  
  TCanvas * can = new TCanvas("can", "can", 600, 600); 
  can->cd();
  set_style();
  
  
  can->Print(epsname+"[");

  
  for(unsigned int p = 0; p<10 ; ++p){

    TLegend * legend = new TLegend(0.55,0.65,.9,0.9);
    legend->SetTextFont(72);
    legend->SetTextSize(0.04);
    legend->SetFillColor(kWhite);

    one_shower shower =  multi_shower.all_shower.at(p); 
  
    
    //DataSet I am looking at
    RooDataSet full_sim_data("full_sim_data","full_sim_data",RooArgSet(depth,weight),"weight");
   

    double sumHCAL=0;

    for(unsigned int i = 0; i < shower.mytype.size(); ++i){	
      sumHCAL += shower.myenergy.at(i);
    }

    for(unsigned int i = 0; i < shower.mytype.size(); ++i){	
	double x_val = shower.myx.at(i);
	double y_val = shower.myy.at(i);
	double z_val = shower.myz.at(i);

	z_val = z_val -shower.interactPoint;
	depth.setVal(sqrt(x_val*x_val+y_val*y_val+z_val*z_val)/10*1.642);
	full_sim_data.add(depth,shower.myenergy.at(i)/sumHCAL,0);
    }
	  

    model.fitTo(full_sim_data);
      
    
    //prepare to plot fits
    
    RooPlot * depthFrame = depth.frame(0,depth.getMax(),100);
    depthFrame->SetTitle("Global Fit");
    depthFrame->SetTitleOffset(1.2,"Y");
    depthFrame->SetLabelSize(0.02,"Y");
    depthFrame->SetYTitle("");
    zero.plotOn(depthFrame,LineStyle(kDashed),LineColor(kBlack));
    full_sim_data.plotOn(depthFrame);
    model.plotOn(depthFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
    model.plotOn(depthFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
    model.plotOn(depthFrame,Name("model"));
    
    stringstream gamma_one_text;
    gamma_one_text.precision(3);
    gamma_one_text << "#Gamma_{1} ("<<alpha.getVal()<<","<<(1/beta.getVal()*beta_gflash_factor)<<")"<<endl;
    stringstream gamma_two_text; 
    gamma_two_text.precision(3);
    gamma_two_text << "#Gamma_{2} (" <<lambda.getVal()<<","<<(1/theta.getVal()*theta_gflash_factor)<< ")"<<endl;
    
    stringstream gamma_text; 
    gamma_text.precision(3);
    gamma_text << "#Gamma_{1/2} c = " <<fsig.getVal() <<endl;
    
    legend->AddEntry(depthFrame->findObject("gamma_one"),gamma_one_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("gamma_two"),gamma_two_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("model"),gamma_text.str().c_str(),"L");
  
    cout << alpha.getVal() << " " << 1/beta.getVal()*beta_gflash_factor << std::endl;
    cout << lambda.getVal() << " " << 1/theta.getVal()*theta_gflash_factor << std::endl;
    cout << fsig.getVal() << " "<<depthFrame->chiSquare()<< std::endl;
    cout <<shower.startingPoint<<endl;
    
    //prepare to dump all the plots in the ps file
    
    
    // one data sample and its fit is drawn
    depthFrame->Draw();
    legend->Draw();
  
    can->Print(epsname);
  }
  
  can->Print(epsname+"]");
 

  return 0;
}


void set_style()
{
  // general appearance and style
  gROOT->SetStyle("Plain");
  
  //gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
    
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
    
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
    
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  
  gStyle->UseCurrentStyle();

}

