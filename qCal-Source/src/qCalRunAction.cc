#include "qCalRunAction.hh"
#include "qCalAnalysis.hh"
#include "qCalRunMessenger.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalEventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"



qCalRunAction::qCalRunAction(qCalEventAction* eventAction)
: G4UserRunAction(), fEventAction(eventAction),
SDVolume(((qCalDetectorConstruction*)G4RunManager::GetRunManager()->
        GetUserDetectorConstruction())->GetVolume())
{
   p_runActionOutputFileName = "qCalOutputFile";
   G4RunManager::GetRunManager()->SetPrintProgress(1);
   
   auto analysisManager = G4AnalysisManager::Instance();
   G4cout << "Using " << analysisManager->GetType() << G4endl;
   
   // Create directories
   //analysisManager->SetHistoDirectoryName("histograms");
   //analysisManager->SetNtupleDirectoryName("ntuple");
   
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetNtupleMerging(true);
   // Creating histograms
    std::string id;

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Histogram Definitions
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //Proto Histogram examples:
   //analysisManager->CreateH1("0","Photon Wavelength Per SiPM Hit", 250, 0., 1000);
   //analysisManager->CreateH1("1","# of Photons per Event", 100, 0., 10000);
   //analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 1*m);
   //analysisManager->CreateH1("Lgap","trackL in gap", 100, 0., 50*cm);
   analysisManager->CreateH1("TimingHist", "HitTimesPerSiPM", 10, 0, 100);
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Extra Detector information to be stored in ntuples:
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   std::vector<G4double> detectorAxisValues = ((qCalDetectorConstruction*)G4RunManager::GetRunManager()->
         GetUserDetectorConstruction())->GetDetectorAxisValues();

   G4String detectorAbsMaterial = ((qCalDetectorConstruction*)G4RunManager::GetRunManager()->
         GetUserDetectorConstruction())->GetAbsMaterial();
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // nTuple Definitions
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   analysisManager->CreateNtuple("qCal", "Simulation Data");
   analysisManager->CreateNtupleIColumn("SiPMPositionX", fEventAction->GetSiPMCoordinatesX()); // ID 0
   analysisManager->CreateNtupleIColumn("SiPMPositionY", fEventAction->GetSiPMCoordinatesY()); // ID 1
   analysisManager->CreateNtupleIColumn("SiPMPositionZ", fEventAction->GetSiPMCoordinatesZ()); // ID 2
   analysisManager->CreateNtupleDColumn("SiPMNumbers", fEventAction->GetSiPMNums()); // ID 3
   analysisManager->CreateNtupleDColumn("PhotonCounts", fEventAction->GetPhotonCount()); // ID 4
   analysisManager->CreateNtupleDColumn("HitTimes", fEventAction->GetHitTimes()); // ID 5
   analysisManager->FinishNtuple();

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Filling Constant Ntuples:
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // analysisManager->FillNtupleSColumn(3, detectorAbsMaterial);

   p_fRunMessenger = new qCalRunMessenger(this);
}

qCalRunAction::~qCalRunAction()
{
   delete G4AnalysisManager::Instance();
}

void qCalRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
   //inform the runManager to save random number seed
   //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
   
   // Get analysis manager
   auto analysisManager = G4AnalysisManager::Instance();

   // Open an output file
   G4String fileName = "Muons";
   G4cout << "FileName " << p_runActionOutputFileName << G4endl;
   analysisManager->OpenFile(p_runActionOutputFileName);
}



void qCalRunAction::EndOfRunAction(const G4Run* /*run*/)
{
   auto analysisManager = G4AnalysisManager::Instance();

   // save histograms & ntuple
   analysisManager->Write();
   analysisManager->CloseFile();
}

void qCalRunAction::SetOutputFileName(G4String outputFileNameByCmd)
{
   p_runActionOutputFileName = outputFileNameByCmd;
}
