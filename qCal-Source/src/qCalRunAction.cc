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
   // Note: these for-loops have the same counter but should not be combined given that root creates IDs sequentially
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
   //for (int i = 0; i < SDVolume; i++){
   //    id = std::to_string(i);
   //    analysisManager->CreateH1("PhotonsPerEvent_ID"+id, "# of Photons per Event: SiPMNumber"+ std::to_string(i), 10000, 0., 10000);
   //}

   //for (int i = 0; i < SDVolume; i++){
   //    id = std::to_string(i + SDVolume);
   //    analysisManager->CreateH1("WavelengthPerHit_ID"+id, "Photon Wavelength per SiPM Hit: SiPMNumber"+ std::to_string(i), 1000, 0., 1000);
   //}

   //Proto Histogram examples:
   //analysisManager->CreateH1("0","Photon Wavelength Per SiPM Hit", 250, 0., 1000);
   //analysisManager->CreateH1("1","# of Photons per Event", 100, 0., 10000);
   //analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 1*m);
   //analysisManager->CreateH1("Lgap","trackL in gap", 100, 0., 50*cm);

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // nTuple Definitions
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   analysisManager->CreateNtuple("qCal", "Simulation Data");
   analysisManager->CreateNtupleDColumn("SiPMNumbers", fEventAction->GetSiPMNums()); // ID 0
   analysisManager->CreateNtupleDColumn("PhotonCounts", fEventAction->GetPhotonCount()); // ID 1
   analysisManager->FinishNtuple();
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
