#include "qCalEventAction.hh"
#include "qCalHit.hh"
#include "qCalUserEventInformation.hh"
#include "qCalTrajectory.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalAnalysis.hh"
#include "qCalRunAction.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>


/**************************************************************************************
 Constructor:  Need SiPM Hit Collection ID
 **************************************************************************************/
qCalEventAction::qCalEventAction():G4UserEventAction(),SDVolume(((qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetVolume()), fphotonCount{std::vector<G4double>(SDVolume, 0.)}, fSiPMCollID(-1),
fSiPMNums{std::vector<G4double>(SDVolume, -1)}
{
   //Set Print Progress to every event.
   G4RunManager::GetRunManager()->SetPrintProgress(1);
}

/**************************************************************************************
 Destructor:
 **************************************************************************************/
qCalEventAction::~qCalEventAction()
{
   
}

/**************************************************************************************
 BeginOfEventAction:
 First thing we have to do is grab the SiPM hit collection from the SDManager.
 The SiPMHit Collection is declared in qCalSD.hh
 **************************************************************************************/
void qCalEventAction::BeginOfEventAction(const G4Event*)
{
   //If fSiPMCollID not defined yet, define it.
   //We only need to do this once per run.
   if ( fSiPMCollID == -1 )
   {
      G4SDManager* sdManager = G4SDManager::GetSDMpointer();
      fSiPMCollID = sdManager->GetCollectionID("SiPM/SiPMHitCollection");
   }
}

/**************************************************************************************
 EndOfEventAction:
 At the end of the event, we have to grab the Hit Collection from the event.
 Then we can process the hits, and print out or save information about the hits.
 **************************************************************************************/
void qCalEventAction::EndOfEventAction(const G4Event* anEvent)
{
   // Grab the Hit Collection (hce) from the event
   G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();
   if ( !hce )
   {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found.\n";
      G4Exception("EventAction::EndOfEventAction()","Code001", JustWarning, msg);
      return;
   }
   
   // Grab specifically the SiPMHit collection from the general event Hit Collection.
   qCalSiPMHitsCollection* eventSiPMHitCollection = static_cast<qCalSiPMHitsCollection*>(hce->GetHC(fSiPMCollID));
   if ( (!eventSiPMHitCollection) )
   {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found.\n";
      G4Exception("EventAction::EndOfEventAction()","Code001", JustWarning, msg);
      return;
   }

   // Number of Hits
   G4int n_hit = eventSiPMHitCollection->entries();

   // Grab the verbosity level for deciding how often to print out info.
   G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
   
   if ( printModulo == 0 || anEvent ->GetEventID() % printModulo == 1 )
   {
      //Spit out info on the primary particle
      G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);
      G4cout   << G4endl
               << ">>> Event "      << anEvent->GetEventID() << " >>> Simulation truth : "
               << ">>> Particle: "  << primary->GetG4code()->GetParticleName()
               << ">>> Momentum: "  << primary->GetMomentum() << G4endl;
      G4cout << "The SiPMs collectively received: " << n_hit << " hits." << G4endl;

   }
   
   //Get the Analysis Manager
   auto analysisManager = G4AnalysisManager::Instance();
   
   //analysisManager->SetNtupleMerging(true);
   
   analysisManager->SetVerboseLevel(1);
   
   // Filling Histograms and ntuples
   for (G4int i = 0; i < SDVolume; ++i)
   {
       fSiPMNums[i] = i;
    }
   
   for ( G4int i = 0; i < n_hit; ++i )
   {
      qCalHit* hit = (*eventSiPMHitCollection)[i];
      int IDofHit = hit->GetSiPMNumber();
      
      //You can fill hit-dependent histograms here:
      //analysisManager->FillH1(IDofHit, hit->GetPhotonCount());
      //analysisManager->FillH1(IDofHit+SDVolume, hit->GetEnergy());

      fphotonCount[IDofHit] += hit->GetPhotonCount();

   }
   analysisManager->AddNtupleRow();

   // Reset the SiPM counts to zero for a new event.
   for (int i = 0; i < SDVolume; i++)
   {
      fphotonCount[i] = 0.;
   }
}
