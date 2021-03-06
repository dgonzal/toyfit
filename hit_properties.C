#define hit_properties_cxx
#include "hit_properties.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void hit_properties::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L hit_properties.C
//      Root > hit_properties t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

hit_properties::hit_properties(TString fname)  
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
      if (!f || !f->IsOpen()) {
         f = new TFile(fname);
      }
      TDirectory * dir = (TDirectory*)f->Get(fname+":/demo");
      TTree* tree;
      dir->GetObject("FitParametersTree",tree);
      Init(tree);

}

hit_properties::~hit_properties()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t hit_properties::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t hit_properties::LoadTree(Long64_t entry)
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

void hit_properties::Init(TTree *tree)
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

Bool_t hit_properties::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void hit_properties::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t hit_properties::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
