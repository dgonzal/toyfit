//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 24 18:06:32 2014 by ROOT version 5.34/18
// from TTree FitParametersTree/FitParametersTree
// found on file: fullsim/example_pi20Gev.root
//////////////////////////////////////////////////////////

#ifndef hit_properties_h
#define hit_properties_h

#include <TROOT.h>
#include <TChain.h>
#include "TTree.h"
#include <TFile.h>
#include "TString.h"

// Header file for the classes stored in the TTree if any.
#include <vector>

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

class hit_properties {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        energyGen;
   Double_t        phiGen;
   Double_t        etaGen;
   Double_t        startingPoint;
   Double_t        interactPoint;
   Double_t        ePi0first;
   Double_t        ePi0tot;
   vector<double>  *myx;
   vector<double>  *myy;
   vector<double>  *myz;
   vector<double>  *myenergy;
   vector<double>  *mytype;

   // List of branches
   TBranch        *b_energyGen;   //!
   TBranch        *b_phiGen;   //!
   TBranch        *b_interactPoint;   //!
   TBranch        *b_ePi0first;   //!
   TBranch        *b_ePi0tot;   //!
   TBranch        *b_myx;   //!
   TBranch        *b_myy;   //!
   TBranch        *b_myz;   //!
   TBranch        *b_myenergy;   //!
   TBranch        *b_mytype;   //!

   hit_properties(TString fname);
   virtual ~hit_properties();

   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif


