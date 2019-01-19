#ifndef twogamma_H
#define twogamma_H

//RooFit
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGamma.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooHist.h"



class twogamma{
 public:
  twogamma(double range, double fraction, double alpha, double beta, double lambbda, double theta);
  ~twogamma();

  RooGamma gamma_one;
  RooGamma gamma_two;
  
  RooAddPdf model;

  double beta_gflash_factor;
  double theta_gflash_factor;
}
#endif
