// c++ classes
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>  
#include <sstream>
#include <cmath>

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
#include "RooDataHist.h"
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
#include "TH1F.h"

//MyCLasses
#include "hit_info.h"
#include "pion20_hits.h"

using namespace std;
using namespace RooFit;

void set_style();

int main(){

  RooRealVar zero("zero","",0,0,150); 

  //input parameters for the distribution
  double alpha_double = 2.38;
  double beta_double = 0.092;
  double lambda_double = 2.21;
  double theta_double = 1.12;
  double c_frac = 0.5;
  //RooRealVar fsig("fsig","signal fraction",0.3) ;

  TString epsname = "test2.ps";

  
  hit_info multi_shower("fullsim/example_pi20Gev.root");
  //hit_info multi_shower("fullsim/example_ele_20GeV.root");
  
 
  double beta_gflash_factor = 1.49;
  double theta_gflash_factor = 16.42;

  beta_double = beta_double/beta_gflash_factor;
  theta_double = theta_double/theta_gflash_factor;
  
  // --- Observable --- 
  RooRealVar depth("depth","depth [cm]",0,800); 
  depth.removeMax(); // set infinite range
  // --- Weight ---
  RooRealVar* weight = new RooRealVar("weight","1/E dE/dr", 0.0, 1.0);
 
  // --- Gamma pdf --- 
  //if one of the Values is set to const you can not ask for it in RooFitModel, otherwise the program will crash
  //RooRealVar alpha("alpha","alpha",alpha_double,0.0,4);
  RooRealVar alpha("alpha","alpha",0.0,20.);
  //alpha.removeMax(); // set infinite range
  //RooRealVar beta("beta","beta",1./beta_double,0.0,100.);
  RooRealVar beta("beta","beta",0.1,.1,3.5);
  //beta.removeMax(); // set infinite range

  RooRealVar mu("mu","mu",0); // no ranges means variable is fixed in fit
  RooRealVar nu("nu","nu",0); // no ranges means variable is fixed in fit
  
  //RooRealVar lambda("lambda","lambda",lambda_double,0.0,6.);
  RooRealVar lambda("lambda","lambda",0.0,40.);
  //lambda.removeMax(); // set infinite range
  //RooRealVar theta("theta","theta",1./theta_double,0.0,100.) ;  
  RooRealVar theta("theta","theta",1.,47) ;  
  //theta.removeMax(); // set infinite range

  RooGamma gamma_one("gamma_one","gamma_one pdf",depth,alpha,beta,mu) ; 
  RooGamma gamma_two("gamma_two","gamma_two pdf",depth,lambda,theta,nu) ; 

  RooRealVar fsig("fsig","signal fraction",c_frac,0.0,1.);
  //RooRealVar fsig("fsig","signal fraction",c_frac);

  RooAddPdf model("model","model",RooArgList(gamma_two,gamma_one),fsig) ;
   
  RooDataSet mean("mean","mean",RooArgSet(depth,weight),"weight"); 
  TH1F* mean_fithist = new TH1F("mean_fithist"," mean fit hist",50,0,150);


  TH1F* log_alpha1 = new TH1F("log_alpha1"," log(#alpha_{1})",20,-1,3);
  TH1F* log_beta1  = new TH1F("log_beta1"," log(#beta_{1})",20,-1,3);
  TH2F* log_corr1  = new TH2F("log_corr1"," correlation #alpha_{1} #beta_{1}",20,-1,3,20,-1,3);
  TH1F* log_alpha2 = new TH1F("log_alpha2"," log(#alpha_{2})",20,-1,3);
  TH1F* log_beta2  = new TH1F("log_beta2"," log(#beta_{2})",20,-1,3);
  TH2F* log_corr2  = new TH2F("log_corr2"," correlation #alpha_{2} #beta_{2}",20,-1,3,20,-1,3);

  TH2F* mean_hist = new TH2F("mean_hist"," mean hist",100,0,150,100.,0.,.0015);
  
  TCanvas * can = new TCanvas("can", "can", 600, 600); 
  can->cd();
  set_style();
  
  
  can->Print(epsname+"[");

  double c_tot =0;

  unsigned int n_part =100;

  for(unsigned int p = 0; p<n_part ; ++p){

    TLegend * legend = new TLegend(0.5,0.65,.88,0.88);
    legend->SetTextFont(72);
    legend->SetTextSize(0.04);
    legend->SetFillColor(kWhite);
    legend->SetBorderSize(0);

    one_shower shower =  multi_shower.all_shower.at(p); 
   
    //DataSet I am looking at
    RooDataSet full_sim_data("full_sim_data","full_sim_data",RooArgSet(depth,weight),"weight");
    TH1F* datahisto = new TH1F("datahist","datahist",50,0,150);

    double sumHCAL=0;

    double deltar =-1; 
    for(unsigned int i = 0; i < shower.mytype.size(); ++i){	
      sumHCAL += shower.myenergy.at(i);
      double x_val = shower.myz.at(i);
      double y_val = shower.myy.at(i);
      double z_val = shower.myz.at(i);
      
      z_val = z_val - shower.interactPoint; 
      double distance=-1;
      if(sqrt(x_val*x_val+y_val*y_val+z_val*z_val)/10 < 200) distance = sqrt(x_val*x_val+y_val*y_val+z_val*z_val)/10;

      if(deltar < distance) deltar = z_val;
    }

    deltar=deltar/shower.mytype.size();
    
    c_tot += shower.ePi0first_energy/n_part;
    fsig.setVal(shower.ePi0first_energy);
    fsig.setRange(shower.ePi0first_energy-0.05 > 0 ? shower.ePi0first_energy-0.05: 0,shower.ePi0first_energy+0.05<1?shower.ePi0first_energy+0.05:1);
    //fsig.setRange(1-shower.ePi0first_energy-0.05 > 0 ? 1-shower.ePi0first_energy-0.05: 0,1-shower.ePi0first_energy+0.05<1 ? 1-shower.ePi0first_energy+0.05:1);

    for(unsigned int i = 0; i < shower.mytype.size(); ++i){	
      double x_val = shower.myx.at(i);
      double y_val = shower.myy.at(i);
      double z_val = shower.myz.at(i);

      // if(shower.mytype.at(i) !=2)continue;
     

      z_val = z_val - shower.interactPoint; 
      //if(z_val<=0) continue;
      depth.setVal(sqrt(x_val*x_val+y_val*y_val+z_val*z_val)/10);
      //depth.setVal(z_val);
      //cout <<depth.getVal()<<endl;
      datahisto->Fill(depth.getVal(),shower.myenergy.at(i)/sumHCAL/deltar);
      mean_fithist->Fill(depth.getVal(),shower.myenergy.at(i)/sumHCAL/deltar/n_part);
      mean_hist->Fill(depth.getVal(),shower.myenergy.at(i)/sumHCAL/deltar);
      full_sim_data.add(depth,shower.myenergy.at(i)/sumHCAL/deltar,0);
      mean.add(depth,shower.myenergy.at(i)/sumHCAL/deltar,0);
    }
  

    RooDataHist roodatahist("roodatahist","roodatahist",depth,Import(*datahisto)) ;

    model.fitTo(roodatahist, Strategy(2) );
    //model.fitTo(full_sim_data);
      
    RooDataHist* full_sim_binned = full_sim_data.binnedClone("full_sim_binned","full sim binned");
    //prepare to plot fits
    
    RooPlot * depthFrame = depth.frame(0,120,100);
    depthFrame->SetTitle("Global Fit");
    depthFrame->SetTitleOffset(1.2,"Y");
    depthFrame->SetLabelSize(0.02,"Y");
    depthFrame->SetYTitle("1/E dE/dr");
    //zero.plotOn(depthFrame,LineStyle(kDashed),LineColor(kBlack));
    //full_sim_data.plotOn(depthFrame);
    roodatahist.plotOn(depthFrame);
    //full_sim_binned->plotOn(depthFrame);
    /*
    model.plotOn(depthFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
    model.plotOn(depthFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
    model.plotOn(depthFrame,Name("model"));
    */
    stringstream gamma_one_text;
    gamma_one_text.precision(3);
    gamma_one_text << "#Gamma_{1} ("<<alpha.getVal()<<","<<(1/beta.getVal()*beta_gflash_factor)<<")"<<endl;
    stringstream gamma_two_text; 
    gamma_two_text.precision(3);
    gamma_two_text << "#Gamma_{2} (" <<lambda.getVal()<<","<<(1/theta.getVal()*theta_gflash_factor)<< ")"<<endl;
    
    stringstream gamma_text; 
    gamma_text.precision(2);
    gamma_text << "#Gamma_{1/2} c=" <<fsig.getVal() <<"("<<shower.ePi0first_energy<<")"<<endl;
    
    stringstream  chi2; 
    chi2.precision(2);
    chi2 << "#chi^{2} = " <<depthFrame->chiSquare() <<endl;

    legend->AddEntry(depthFrame->findObject("gamma_one"),gamma_one_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("gamma_two"),gamma_two_text.str().c_str(),"L");
    legend->AddEntry(depthFrame->findObject("model"),gamma_text.str().c_str(),"L");
    legend->AddEntry((TObject*)0, chi2.str().c_str(), "");
    cout << "alpha,  beta "<< alpha.getVal() << " " << 1/beta.getVal()*beta_gflash_factor << std::endl;
    cout << "lambda, theta "<<lambda.getVal() << " " << 1/theta.getVal()*theta_gflash_factor << std::endl;
    cout <<"c, chi2 "<< fsig.getVal() << " "<<depthFrame->chiSquare()<< std::endl;
    cout <<"starting , interacting Points "<<shower.startingPoint<<" "<<shower.interactPoint <<endl;
    cout<< "pi0first, pi0tot "<<shower.ePi0first_energy <<" "<<shower.ePi0tot_energy<<endl;
    
    /*
    log_alpha1->Fill(log10(alpha.getVal()));
    log_alpha2->Fill(log10(lambda.getVal()));
    //cout<<log(1/beta.getVal()*beta_gflash_factor)<<endl;
    log_beta1 ->Fill(log10(1/beta.getVal()*beta_gflash_factor));
    log_beta2 ->Fill(log10(1/theta.getVal()*theta_gflash_factor));
    log_corr1 ->Fill(log10(alpha.getVal()),log10(1/beta.getVal()*beta_gflash_factor));
    log_corr2 ->Fill(log10(lambda.getVal()),log10(1/theta.getVal()*theta_gflash_factor));
    */
    
    if(fsig.getVal()>0.1)log_alpha1->Fill(log(alpha.getVal()));
    if(fsig.getVal()<0.9)log_alpha2->Fill(log(lambda.getVal()));
    //cout<<log(1/beta.getVal()*beta_gflash_factor)<<endl;
    if(fsig.getVal()>0.1)log_beta1 ->Fill(log(1/beta.getVal()*beta_gflash_factor));
    if(fsig.getVal()<0.9)log_beta2 ->Fill(log(1/theta.getVal()*theta_gflash_factor));
    if(fsig.getVal()>0.1)log_corr1 ->Fill(log(alpha.getVal()),log(1/beta.getVal()*beta_gflash_factor));
    if(fsig.getVal()<0.9)log_corr2 ->Fill(log(lambda.getVal()),log(1/theta.getVal()*theta_gflash_factor));
    

    //prepare to dump all the plots in the ps file
    
    
    // one data sample and its fit is drawn
    /*
    datahisto->Draw();
    datahisto0->SetLineColor(kRed);
    datahisto0->Draw("same");
    datahisto1->SetLineColor(kGreen);
    datahisto1->Draw("same");
    datahisto2->SetLineColor(kBlue);
    datahisto2->Draw("same");
    can->Print(epsname);
    */
    //if(depthFrame->chiSquare()>5) continue;
    depthFrame->Draw("HIST p");
    //legend->Draw();
  
    can->Print(epsname);
  }

  RooDataHist roomeandatahist("roomeandatahist","roomeandatahist",depth,Import(*mean_fithist)) ;


  fsig.setVal(c_tot);
  alpha.removeMax(); // set infinite range
  beta.removeMax(); // set infinite range
  lambda.removeMax(); // set infinite range
  theta.removeMax(); // set infinite range

  //model.fitTo(roomeandatahist);
  //model.fitTo(mean,SumW2Error(kTRUE));




  TLegend * mean_legend = new TLegend(0.5,0.65,.9,0.9);
  mean_legend->SetTextFont(72);
  mean_legend->SetTextSize(0.04);
  mean_legend->SetFillColor(kWhite);
  mean_legend->SetBorderSize(0);


  stringstream  mean_gamma_one_text;
  mean_gamma_one_text.precision(3);
  mean_gamma_one_text << "#Gamma_{1} ("<<alpha.getVal()<<","<<(1/beta.getVal()*beta_gflash_factor)<<")"<<endl;
  stringstream mean_gamma_two_text; 
  mean_gamma_two_text.precision(3);
  mean_gamma_two_text << "#Gamma_{2} (" <<lambda.getVal()<<","<<(1/theta.getVal()*theta_gflash_factor)<< ")"<<endl;
  
  stringstream  mean_gamma_text; 
  mean_gamma_text.precision(2);
  mean_gamma_text << "#Gamma_{1/2} c=" <<fsig.getVal() <<"("<< c_tot<<")"<<endl;
  
  stringstream  mean_chi2; 
  mean_chi2.precision(2);
  //mean_chi2 << "#chi^{2} " <<depthFrame->chiSquare() <<endl;
  
  RooPlot * meanFrame = depth.frame(0,120,200);
  //meanFrame->SetMaximum(1);
  meanFrame->SetTitle("Mean Fit");
  meanFrame->SetYTitle("1/E dE/dr");
  model.plotOn(meanFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
  model.plotOn(meanFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
  model.plotOn(meanFrame,Name("model"));
  //mean.plotOn(meanFrame);
  //roomeandatahist.plotOn(meanFrame);

  mean_legend->AddEntry(meanFrame->findObject("gamma_one"),mean_gamma_one_text.str().c_str(),"L");
  mean_legend->AddEntry(meanFrame->findObject("gamma_two"),mean_gamma_two_text.str().c_str(),"L");
  mean_legend->AddEntry(meanFrame->findObject("model"),mean_gamma_text.str().c_str(),"L");
  
  meanFrame->Draw();
  mean_legend->Draw();
  //datahisto->Draw();
  //can->Print(epsname);

  //mean_hist->Draw("box");
  //can->Print(epsname);
 

  can->Print(epsname);
  log_alpha1->Draw();
  can->Print(epsname);
  log_beta1->Draw();
  can->Print(epsname);
  log_corr1->Draw("colz");
  can->Print(epsname);
  log_alpha2->Draw();
  can->Print(epsname);
  log_beta2->Draw();
  can->Print(epsname);
  log_corr2->Draw("colz");
  can->Print(epsname);



  can->Print(epsname+"]");
 

  cout<<log(.347)<<endl;

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

  //gPad->SetLogy();

}

