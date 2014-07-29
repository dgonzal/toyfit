#include "hit_info.h"


hit_info::hit_info(TString input_file){

  TFile * input = TFile::Open( input_file ); 
  TDirectory * dir = (TDirectory*)input->Get(input_file+":/demo");
  TTree * theTree;
  dir->GetObject("FitParametersTree",theTree);
  one_shower help_oneShower;

  std::vector<double>* mytype=0;
  std::vector<double>* myx=0;
  std::vector<double>* myy=0;
  std::vector<double>* myz=0;
  std::vector<double>* myenergy=0;

  //TTree* theTree = (TTree*)input->Get("demo/FitParametersTree");
  theTree->SetBranchAddress("energyGen",&help_oneShower.energyGen);
  theTree->SetBranchAddress("phiGen",&help_oneShower.phiGen);
  theTree->SetBranchAddress("startingPoint",&help_oneShower.startingPoint);
  theTree->SetBranchAddress("interactPoint",&help_oneShower.interactPoint);
  theTree->SetBranchAddress("ePi0first",&help_oneShower.ePi0first_energy);
  theTree->SetBranchAddress("ePi0tot",&help_oneShower.ePi0tot_energy);
  theTree->SetBranchAddress("myx",&myx);
  theTree->SetBranchAddress("myy",&myy);
  theTree->SetBranchAddress("myz",&myz);
  theTree->SetBranchAddress("myenergy",&myenergy);
  theTree->SetBranchAddress("mytype",&mytype);
  
  for(unsigned int i =0; i<theTree->GetEntries(); ++i){
    theTree->GetEntry(i);
  
    for(unsigned int p = 0; p<mytype->size(); ++p){
      help_oneShower.mytype.push_back(mytype->at(p)); 
      help_oneShower.myenergy.push_back(myenergy->at(p)); 
      help_oneShower.myx.push_back(myx->at(p)); 
      help_oneShower.myy.push_back(myy->at(p)); 
      help_oneShower.myz.push_back(myz->at(p)); 
    }
    all_shower.push_back(help_oneShower);

    help_oneShower.mytype.clear();
    help_oneShower.myenergy.clear();
    help_oneShower.myx.clear();
    help_oneShower.myy.clear();
    help_oneShower.myz.clear();

  }

  input->Close();
}

hit_info::~hit_info(){
  //input->Close();
}
