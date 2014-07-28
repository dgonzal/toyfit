#include "hit_info.h"


hit_info::hit_info(TString input_file){

  input = TFile::Open( input_file ); 
  TDirectory * dir = (TDirectory*)input->Get(input_file+":/demo");
  dir->GetObject("FitParametersTree",theTree);
  
  //TTree* theTree = (TTree*)input->Get("demo/FitParametersTree");
  theTree->SetBranchAddress("energyGen",&energyGen);
  theTree->SetBranchAddress("phiGen",&phiGen);
  theTree->SetBranchAddress("startingPoint",&startingPoint);
  theTree->SetBranchAddress("interactPoint",&interactPoint);
  theTree->SetBranchAddress("ePi0first",&ePi0first_energy);
  theTree->SetBranchAddress("ePi0tot",&ePi0tot_energy);
  theTree->SetBranchAddress("myx",&myx);
  theTree->SetBranchAddress("myy",&myy);
  theTree->SetBranchAddress("myz",&myz);
  theTree->SetBranchAddress("myenergy",&myenergy);
  theTree->SetBranchAddress("mytype",&mytype);
  
  theTree->GetEntry(1);
  input->Close();
}

hit_info::~hit_info(){
  //input->Close();
}
