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

  TString epsname = "test.ps";

  
  //hit_info multi_shower("fullsim/example_pi20Gev.root");
  hit_info multi_shower("fullsim/example_ele_20GeV.root");
  
  /*
  for(unsigned int i = 0; i<10; ++i){
      cout<<multi_shower.all_shower.at(0).myx.at(i)<< " " <<multi_shower.all_shower.at(0).myy.at(i)<< " " <<multi_shower.all_shower.at(0).myz.at(i) <<" "<<multi_shower.all_shower.at(1).myx.at(i)<< " " <<multi_shower.all_shower.at(1).myy.at(i)<< " " <<multi_shower.all_shower.at(1).myz.at(i) <<endl;
  }
  */
  //pion20_hits shower;

  double beta_gflash_factor = 1.49;
  double theta_gflash_factor = 16.42;

  beta_double = beta_double/beta_gflash_factor;
  theta_double = theta_double/theta_gflash_factor;
  
  // --- Observable --- 
  RooRealVar depth("depth","depth [cm]",0,0,300); 
  
  // --- Weight ---
  RooRealVar* weight = new RooRealVar("weight","1/E dE/dr", 0.0, 1.0);
 
  // --- Gamma pdf --- 
  //if one of the Values is set to const you can not ask for it in RooFitModel, otherwise the program will crash
  RooRealVar alpha("alpha","alpha",alpha_double,0.,100.);
  alpha.removeMax(); // set infinite range
  RooRealVar beta("beta","beta",1./beta_double,0,100.) ;  
  beta.removeMax(); // set infinite range
  //RooRealVar mu("mu","mu",1.49); // no ranges means variable is fixed in fit
  RooRealVar mu("mu","mu",0); // no ranges means variable is fixed in fit

  RooRealVar lambda("lambda","lambda",lambda_double,0.,100.);
  lambda.removeMax(); // set infinite range
  RooRealVar theta("theta","theta",1./theta_double,0.,100.) ;  
  theta.removeMax(); // set infinite range

  RooGamma gamma_one("gamma_one","gamma_one pdf",depth,alpha,beta,mu) ; 
  RooGamma gamma_two("gamma_two","gamma_two pdf",depth,lambda,theta,mu) ; 

  RooRealVar fsig("fsig","signal fraction",c_frac,0.2,.8);
  //RooRealVar fsig("fsig","signal fraction",c_frac) ;

  RooAddPdf model("model","model",RooArgList(gamma_one,gamma_two),fsig) ;
  
  //generate data for one histogram
  RooDataSet * toydata = model.generate(depth,particles);
  
  
  TCanvas * can = new TCanvas("can", "can", 600, 600); 
  can->cd();
  set_style();
  
  
  can->Print(epsname+"[");

  //shower.GetEntry(0);
  
  //cout<<shower.mytype->size()<<" "<<shower.myx->size()<<" "<<shower.myy->size()<<" "<<shower.energyGen<<endl;
  //cout<<inv_energy<endl;
  //cout<<shower.energyGen<<endl;
  
  for(unsigned int p = 0; p<100 ; ++p){

    TLegend * legend = new TLegend(0.55,0.65,.9,0.9);
    legend->SetTextFont(72);
    legend->SetTextSize(0.04);
    legend->SetFillColor(kWhite);

    one_shower shower =  multi_shower.all_shower.at(p); 
    double inv_energy = 1/shower.energyGen;
    
    //DataSet I am looking at
    RooDataSet full_sim_data("full_sim_data","full_sim_data",RooArgSet(depth,weight),"weight");

    for(unsigned int i = 0; i < shower.mytype.size(); ++i){	
	double x_val = shower.myx.at(i);
	double y_val = shower.myy.at(i);
	double z_val = shower.myz.at(i);
	if(z_val>1750){
	  double dE = shower.energyGen-shower.myenergy.at(i);
	  //z_val-=1750;
	  depth.setVal(sqrt(x_val*x_val+y_val*y_val+z_val*z_val)/10);
	  double dr = depth.getVal(); 	 
	  //cout<< x_val<< " "<<y_val << " "<<z_val <<" "<<depth.getVal()<<endl;
	  //cout<<shower.myenergy.at(i)<<endl;
	  full_sim_data.add(depth,1/shower.myenergy.at(i),0);
	  //cout<<full_sim_data.isWeighted()<<endl;
	}
      }
	  
    //RooDataSet * data = gamma_one.generate(depth,1000);
    //fsig.setVal(0.);
    //model.fitTo(*toydata);
    model.fitTo(full_sim_data);

    //gamma_two.fitTo(*toydata);  
    
    //prepare to plot one fit 
    
    RooPlot * onegammeFrame = depth.frame(mu.getVal(),150,100);
    toydata->plotOn(onegammeFrame);
    //gamma_two.plotOn(onegammeFrame);
  

    RooPlot * depthFrame = depth.frame(150,depth.getMax(),100);
    depthFrame->SetYTitle("1/E");
    zero.plotOn(depthFrame,LineStyle(kDashed),LineColor(kBlack));
    //toydata->plotOn(depthFrame);
    full_sim_data.plotOn(depthFrame);
    model.plotOn(depthFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
    model.plotOn(depthFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
    model.plotOn(depthFrame,Name("model"));
  
    
    stringstream gamma_one_text;
    gamma_one_text.precision(4);
    gamma_one_text << "#Gamma_{1} ("<<alpha.getVal()<<","<<(1/beta.getVal()*beta_gflash_factor)<<")"<<endl;
    stringstream gamma_two_text; 
    gamma_two_text.precision(4);
    gamma_two_text << "#Gamma_{2} (" <<lambda.getVal()<<","<<(1/theta.getVal()*theta_gflash_factor)<< ")"<<endl;
    
    stringstream gamma_text; 
    gamma_text.precision(3);
    gamma_text << "#Gamma_{1/2} c = " <<fsig.getVal()<<endl;
    
    legend->AddEntry(depthFrame->findObject("gamma_one"),gamma_one_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("gamma_two"),gamma_two_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("model"),gamma_text.str().c_str(),"L");
  
    cout << alpha.getVal() << " " << 1/beta.getVal()*beta_gflash_factor << std::endl;
    cout << lambda.getVal() << " " << 1/theta.getVal()*theta_gflash_factor << std::endl;
    cout << fsig.getVal() << " "<<depthFrame->chiSquare()<< std::endl;

    /*
      RooHist* hpull = depthFrame->pullHist();
      hpull->SetLineColor(0);
      hpull->SetMarkerColor(kRed);
      hpull->SetMarkerStyle(21);
      
      depthFrame->addPlotable(hpull);
      depthFrame->SetMinimum(-5);
      

      RooPlot * pullhistFrame = depth.frame(Title("Pull Distribution"));
      pullhistFrame->addPlotable(hpull);
      pullhistFrame->SetMaximum(5);
      pullhistFrame->SetMinimum(-5);
    */
    
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

