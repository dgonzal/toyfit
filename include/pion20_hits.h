//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 25 14:41:23 2014 by ROOT version 5.34/18
// from TTree FitParametersTree/FitParametersTree
// found on file: fullsim/example_pi20Gev.root
//////////////////////////////////////////////////////////

#ifndef pion20_hits_h
#define pion20_hits_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>


using namespace std;
// Fixed size dimensions of array or collections stored in the TTree if any.

class pion20_hits {
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
   TBranch        *b_startingPoint;   //!
   TBranch        *b_interactPoint;   //!
   TBranch        *b_ePi0first;   //!
   TBranch        *b_ePi0tot;   //!
   TBranch        *b_myx;   //!
   TBranch        *b_myy;   //!
   TBranch        *b_myz;   //!
   TBranch        *b_myenergy;   //!
   TBranch        *b_mytype;   //!

   pion20_hits(TTree *tree=0);
   virtual ~pion20_hits();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pion20_hits_cxx
pion20_hits::pion20_hits(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("fullsim/example_pi20Gev.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("fullsim/example_pi20Gev.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("fullsim/example_pi20Gev.root:/demo");
      dir->GetObject("FitParametersTree",tree);
      //f->Close();
   }
   Init(tree);
   
}

pion20_hits::~pion20_hits()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pion20_hits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pion20_hits::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pion20_hits::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   myx = 0;
   myy = 0;
   myz = 0;
   myenergy = 0;
   mytype = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("energyGen", &energyGen, &b_energyGen);
   fChain->SetBranchAddress("phiGen", &phiGen, &b_phiGen);
   fChain->SetBranchAddress("etaGen", &etaGen, &b_phiGen);
   fChain->SetBranchAddress("startingPoint", &startingPoint, &b_startingPoint);
   fChain->SetBranchAddress("interactPoint", &interactPoint, &b_interactPoint);
   fChain->SetBranchAddress("ePi0first", &ePi0first, &b_ePi0first);
   fChain->SetBranchAddress("ePi0tot", &ePi0tot, &b_ePi0tot);
   fChain->SetBranchAddress("myx", &myx, &b_myx);
   fChain->SetBranchAddress("myy", &myy, &b_myy);
   fChain->SetBranchAddress("myz", &myz, &b_myz);
   fChain->SetBranchAddress("myenergy", &myenergy, &b_myenergy);
   fChain->SetBranchAddress("mytype", &mytype, &b_mytype);
   Notify();
}

Bool_t pion20_hits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pion20_hits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pion20_hits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pion20_hits_cxx
