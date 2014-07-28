#ifndef hit_info_h
#define hit_info_h

// c++ classes
#include <iostream>
#include <vector>
#include <string>

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

using namespace std;

//struct

class hit_info{
public: 
  hit_info(TString input_file);
  ~hit_info();

  std::vector<double>* mytype = 0;
  std::vector<double>* myx = 0;
  std::vector<double>* myy = 0;
  std::vector<double>* myz = 0;
  std::vector<double>* myenergy = 0;
  double energyGen;
  double phiGen;
  double etaGen;
  double startingPoint;
  double interactPoint;
  double ePi0first_energy;
  double ePi0tot_energy;

  TFile * input;
  TTree * theTree;
  bool GetEntry(int i){ return theTree->GetEntry(i)>0 ? true: false;}

};
#endif
