// c++ classes
#include <iostream>
#include <vector>
#include <string>

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


using namespace std;
using namespace RooFit;

void set_style();

int main(){


  RooRealVar zero("zero","",0,0,150); 

  //Number of Events and particles per shower
  unsigned int events = 10;
  unsigned int particles = 10000; 

  //input parameters for the distribution
  double alpha_double = 2.38;
  double beta_double = 0.092;
  double lambda_double = 2.21;
  double theta_double = 1.12;
  double c_frac = 0.3;
  //RooRealVar fsig("fsig","signal fraction",0.3) ;

  TString epsname = "10_1000_50bins.ps";


  double beta_gflash_factor = 1.49;
  double theta_gflash_factor = 16.42;

  beta_double = beta_double/beta_gflash_factor;
  theta_double = theta_double/theta_gflash_factor;
  
  // --- Observable --- 
  RooRealVar depth("depth","depth (cm)",0,0,150) ; 
  depth.setBins(10);

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
  

  // Do the MonteCarlo study with RooMCStudy
  //RooMCStudy* study_model = new RooMCStudy(model,depth,Silence(),FitOptions(Strategy(1),Save(kTRUE),Hesse(kFALSE)));//,Minimizer("Minut","migradimproved")
  RooMCStudy* study_model = new RooMCStudy(model,depth,Silence(),FitOptions(Save(kTRUE)));
  study_model->generateAndFit(events,particles,kTRUE);


  //generate data for one histogram
  RooDataSet * data = model.generate(depth,particles);
  //RooDataSet * data = gamma_one.generate(depth,1000);
  //fsig.setVal(0.);
  model.fitTo(*data);
  //gamma_two.fitTo(*data);

  //get the correlation Matrix for the first generated and fitted histograms 
  TMatrixDSym correlationMatrix = study_model->fitResult(0)->correlationMatrix();

  correlationMatrix.Print();

  //prepare to plot pull histograms
  
  RooPlot* pull_alpha = study_model->plotPull(alpha,FrameRange(-5,5),Bins(40),FitGauss(kTRUE)) ;
  RooPlot* pull_beta = study_model->plotPull(beta,FrameRange(-5,5),Bins(40),FitGauss(kTRUE)) ;
  RooPlot* pull_lambda = study_model->plotPull(lambda,FrameRange(-5,5),Bins(40),FitGauss(kTRUE)) ;
  RooPlot* pull_theta = study_model->plotPull(theta,FrameRange(-5,5),Bins(40),FitGauss(kTRUE)) ;
  RooPlot* pull_c = study_model->plotPull(fsig,FrameRange(-5,5),Bins(40),FitGauss(kTRUE)) ;
  

  //prepare to plot one fit 
  
  RooPlot * onegammeFrame = depth.frame(mu.getVal(),150,100);
  data->plotOn(onegammeFrame);
  gamma_two.plotOn(onegammeFrame);
  

  RooPlot * depthFrame = depth.frame(mu.getVal(),150,100);
  zero.plotOn(depthFrame,LineStyle(kDashed),LineColor(kBlack));
  data->plotOn(depthFrame);
  model.plotOn(depthFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
  model.plotOn(depthFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
  model.plotOn(depthFrame,Name("model"));
  
  cout << alpha.getVal() << " " << 1/beta.getVal()*beta_gflash_factor << std::endl;
  cout << lambda.getVal() << " " << 1/theta.getVal()*theta_gflash_factor << std::endl;
  cout << fsig.getVal() << " "<<depthFrame->chiSquare()<< std::endl;


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
  

  //prepare to dump all the plots in the ps file
  TCanvas * can = new TCanvas("can", "can", 600, 600); 
  can->cd();
  set_style();
  
  TLegend * legend = new TLegend(0.55,0.65,.9,0.9);
  legend->SetTextFont(72);
  legend->SetTextSize(0.04);
  legend->SetFillColor(kWhite);
  legend->AddEntry(depthFrame->findObject("gamma_one"),"gamma one","L");
  legend->AddEntry(depthFrame->findObject("gamma_two"),"gamma two","L");
  legend->AddEntry(depthFrame->findObject("model")," gamma","L");
  
  can->Print(epsname+"[");
  
  //fill 2D-Histograms for the variables of the diagram and the correlation
  TString variables[] = {"fsig","alpha","beta","lambda","theta"};  
  RooArgList RooVar(fsig,alpha,beta,lambda,theta);  

  for(unsigned int i =0; i< 4; ++i){
    for(unsigned int p =1+i; p < 5;++p){	
     double a_max = 18;
     if(i==0) a_max = 1;
    
     double i_factor = 1.;
     double p_factor = 1.;

     if(i==2)
       i_factor = beta_gflash_factor;
     else if(i==4)
       i_factor = theta_gflash_factor;

     if(p==2)
       p_factor = beta_gflash_factor;
     else if(p==4)
       p_factor = theta_gflash_factor;

     TH2F* corre_hist = new TH2F(variables[i]+variables[p],variables[i]+' '+variables[p],50,0,a_max,50,0,18);
     TH2F* corre_hist_conv = new TH2F(variables[i]+variables[p]+"_conv",variables[i]+' '+variables[p]+" conv",250,0,a_max,250,0,50);
     study_model->fitParDataSet().fillHistogram(corre_hist_conv,RooArgList(RooVar[i],RooVar[p]));
     
     for(unsigned int m= 0; m <events; m++){
       if(i==2 || i==4){
	 if(p==2 || p==4){
	   corre_hist->Fill(i_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[i]))->getVal(), p_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[p]))->getVal());
	 }
	 else
	   corre_hist->Fill(i_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[i]))->getVal(), ((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[p]))->getVal());
       }
       else if(p==2 || p==4){
	 if(i==2 || i==4){
	   corre_hist->Fill(i_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[i]))->getVal(), p_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[p]))->getVal());
	 }
	 else
	   corre_hist->Fill(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[i]))->getVal(), p_factor/((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[p]))->getVal());
       }
       else 
	 corre_hist->Fill(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[i]))->getVal(),((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[p]))->getVal());
     }
     corre_hist->Draw("box");
     can->Print(epsname);
    
     //corre_hist_conv->Draw("box");
     //can->Print(epsname);

    } 
  }
  	  

  
  for(unsigned int m= 0; m <events; m++){
    fsig.setVal(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[0]))->getVal());
    alpha.setVal(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[1]))->getVal());
    beta.setVal(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[2]))->getVal());
    lambda.setVal(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[3]))->getVal());
    theta.setVal(((RooRealVar*)study_model->fitResult(m)->floatParsFinal().find(variables[4]))->getVal());

    RooPlot * fitFrame = depth.frame(mu.getVal(),50,100);
    zero.plotOn(fitFrame,LineStyle(kDashed),LineColor(kBlack));
    study_model->genData(m)->plotOn(fitFrame);
    model.plotOn(fitFrame,Components(gamma_one),LineStyle(kDashed),LineColor(kGreen),Name("gamma_one"));
    model.plotOn(fitFrame,Components(gamma_two),LineStyle(kDashed),LineColor(kRed),Name("gamma_two"));
    model.plotOn(fitFrame,Name("model"));


    TString legend_string_c = "c: ";
    legend_string_c += to_string(round(100*fsig.getVal())/100);
    TString legend_string_a = " a: ";
    legend_string_a += to_string(round(100*alpha.getVal())/100);
    legend_string_a += " b: ";
    legend_string_a += to_string(1/round(100*beta.getVal())*100*beta_gflash_factor);
    TString legend_string_b = " l: ";
    legend_string_b += to_string(round(100*lambda.getVal())/100);
    legend_string_b += " t: ";
    legend_string_b += to_string(1/round(100*theta.getVal())*100*theta_gflash_factor);

    TLegend * tmp_leg = new TLegend(0.45,0.5,.9,0.9);
    tmp_leg->SetTextFont(72);
    tmp_leg->SetTextSize(0.04);
    tmp_leg->SetFillColor(kWhite);
    tmp_leg->AddEntry(fitFrame->findObject("gamma_one"),"gamma one","L");
    tmp_leg->AddEntry(fitFrame->findObject("gamma_two"),"gamma two","L");
    tmp_leg->AddEntry(fitFrame->findObject("model")," gamma","L");
    tmp_leg->AddEntry((TObject*)0, legend_string_c, "");
    tmp_leg->AddEntry((TObject*)0, legend_string_a, "");
    tmp_leg->AddEntry((TObject*)0, legend_string_b, "");

    fitFrame->Draw();
    tmp_leg->Draw();
    can->Print(epsname);
  }



  //cout<<study_model->fitParDataSet().size()<<"/"<<events<<endl;

  //pull distibrutions are drawn
  pull_alpha->Draw();
  can->Print(epsname);

  pull_beta->Draw();
  can->Print(epsname);

  pull_lambda->Draw();
  can->Print(epsname);

  pull_theta->Draw();
  can->Print(epsname);

  pull_c->Draw();
  
  can->Print(epsname);


  // one data sample and its fit is drawn
  depthFrame->Draw();
  legend->Draw();
  can->Print(epsname);

  //pullhistFrame->Draw();
  //onegammeFrame->Draw();
  //can->Print(epsname);

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

