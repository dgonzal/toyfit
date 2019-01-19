#include "twogamma.h"

twogamma::twogamma(double range, double fraction, double alpha, double beta, double lambbda, double theta){

  beta_gflash_factor = 1.49;
  theta_gflash_factor = 16.42;

  double beta_double = beta/beta_gflash_factor;
  double theta_double = theta/theta_gflash_factor;

  // --- Observable --- 
  RooRealVar depth("depth","depth (cm)",0,0,range); 
   // --- Gamma pdf --- 
  RooRealVar alpha("alpha","alpha",alpha_double,0.,100.);
  alpha.removeMax(); // set infinite range
  RooRealVar beta("beta","beta",1./beta_double,0,100.) ;  
  beta.removeMax(); // set infinite range
  RooRealVar mu("mu","mu",0); // no ranges means variable is fixed in fit

  RooRealVar lambda("lambda","lambda",lambda_double,0.,100.);
  lambda.removeMax(); // set infinite range
  RooRealVar theta("theta","theta",1./theta_double,0.,100.) ;  
  theta.removeMax(); // set infinite range
  RooRealVar nu("nu","nu",16.42); // no ranges means variable is fixed in fit

  RooGamma gamma_one("gamma_one","gamma_one pdf",depth,alpha,beta,mu) ; 
  RooGamma gamma_two("gamma_two","gamma_two pdf",depth,lambda,theta,mu) ;

    
}
