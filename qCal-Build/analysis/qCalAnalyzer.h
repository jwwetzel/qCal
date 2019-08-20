//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Feb  9 04:46:06 2019 by ROOT version 6.16/00
// from TTree qCal/Simulation Data
// found on file: ../muons_1-GeV_10k.root
//////////////////////////////////////////////////////////

#ifndef qCalAnalyzer_h
#define qCalAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class qCalAnalyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<double>  *SiPMNumbers;
   vector<double>  *PhotonCounts;

   // List of branches
   TBranch        *b_SiPMNumbers;   //!
   TBranch        *b_PhotonCounts;   //!

   qCalAnalyzer(TTree *tree=0);
   virtual ~qCalAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef qCalAnalyzer_cxx
qCalAnalyzer::qCalAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../muons_1-GeV_10k.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../muons_1-GeV_10k.root");
      }
      f->GetObject("qCal",tree);

   }
   Init(tree);
}

qCalAnalyzer::~qCalAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t qCalAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t qCalAnalyzer::LoadTree(Long64_t entry)
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

void qCalAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SiPMNumbers = 0;
   PhotonCounts = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SiPMNumbers", &SiPMNumbers, &b_SiPMNumbers);
   fChain->SetBranchAddress("PhotonCounts", &PhotonCounts, &b_PhotonCounts);
   Notify();
}

Bool_t qCalAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void qCalAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t qCalAnalyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef qCalAnalyzer_cxx
