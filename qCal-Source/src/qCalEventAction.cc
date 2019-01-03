#include "qCalEventAction.hh"
#include "qCalHit.hh"
#include "qCalUserEventInformation.hh"
#include "qCalTrajectory.hh"

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
qCalEventAction::qCalEventAction()
:fSiPMCollID(-1)
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
void qCalEventAction::BeginOfEventAction(const G4Event* anEvent)
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
   
   // Grab the verbosity level for deciding how often to print out info.
   G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
   if ( printModulo == 0 || anEvent ->GetEventID() % printModulo != 0 )
   {
      return;
   }
   
   // Spit out info on the primary particle
   G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);
//   G4cout   << G4endl
//            << ">>> Event "      << anEvent->GetEventID() << " >>> Simulation truth : "
//            << ">>> Particle: "  << primary->GetG4code()->GetParticleName()
//            << ">>> Momentum: "  << primary->GetMomentum() << G4endl;
   
   // Get the number of
   G4int n_hit = eventSiPMHitCollection->entries();
   G4cout << "The SiPMs collectively received " << n_hit << " hits." << G4endl;
   
   //Get the Analysis Manager
   auto analysisManager = G4AnalysisManager::Instance();
   
   analysisManager->FillH1(1, n_hit);
   
   for ( G4int i = 0; i < n_hit; ++i )
   {
      qCalHit* hit = (*eventSiPMHitCollection)[i];
      analysisManager->FillH1(0, hit->GetEnergy());
   }
//   G4cout << "SiPM exposed to " << hHC->GetPhotonCount() << " Photons." << G4endl;
   
//   qCalUserEventInformation* eventInformation =(qCalUserEventInformation*)anEvent->GetUserInformation();
//
//   G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
//
//   G4int n_trajectories = 0;
//   if ( trajectoryContainer )
//   {
//      n_trajectories = trajectoryContainer->entries();
//   }
//
//   // extract the trajectories and draw them
//   if ( G4VVisManager::GetConcreteInstance() )
//   {
//      for ( G4int i=0; i<n_trajectories; i++ )
//      {
//         qCalTrajectory* trj = (qCalTrajectory*)((*(anEvent->GetTrajectoryContainer()))[i]);
//         if( trj->GetParticleName() == "opticalphoton" )
//         {
//            trj->SetForceDrawTrajectory(fForcedrawphotons);
//            trj->SetForceNoDrawTrajectory(fForcenophotons);
//         }
//         trj->DrawTrajectory();
//      }
//   }
//
//   qCalSiPMHitsCollection* SiPMHC = 0;
//   G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
//
//   //Get the hit collections
//   if( hitsCE )
//   {
//      if( fSiPMCollID >= 0 )
//      {
//         SiPMHC = (qCalSiPMHitsCollection*)(hitsCE->GetHC(fSiPMCollID));
//      }
//   }
//
//   if(SiPMHC)
//   {
//      G4ThreeVector reconPos(0.,0.,0.);
//      G4int SiPMs = SiPMHC->entries();
//      //Gather info from all SiPMs
//      for ( G4int i = 0 ; i < SiPMs; i++ )
//      {
//         eventInformation->IncHitCount((*SiPMHC)[i]->GetPhotonCount());
//         reconPos += (*SiPMHC)[i]->GetSiPMPos()*(*SiPMHC)[i]->GetPhotonCount();
//         if ( (*SiPMHC)[i]->GetPhotonCount() >= fSiPMThreshold )
//         {
//            eventInformation->IncSiPMSAboveThreshold();
//         }
//         else{
//            //wasnt above the threshold, turn it back off
//            (*SiPMHC)[i]->SetDrawit(false);
//         }
//      }
//
//      //dont bother unless there were hits
//      if ( eventInformation->GetHitCount() > 0 )
//      {
//         reconPos/=eventInformation->GetHitCount();
//         if(fVerbose>0){
//            G4cout << "\tReconstructed position of hits in qCal : "
//            << reconPos/mm << G4endl;
//         }
//         eventInformation->SetReconPos(reconPos);
//      }
//      SiPMHC->DrawAllHits();
//   }
//
//   if ( fVerbose > 0 )
//   {
//      //End of event output. later to be controlled by a verbose level
//      G4cout << "\tNumber of photons that hit SiPMs in this event : "
//      << eventInformation->GetHitCount() << G4endl;
//      G4cout << "\tNumber of SiPMs above threshold("<<fSiPMThreshold<<") : "
//      << eventInformation->GetSiPMSAboveThreshold() << G4endl;
//      G4cout << "\tNumber of photons produced by cerenkov in this event : "
//      << eventInformation->GetPhotonCount_Ceren() << G4endl;
//      G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
//      << eventInformation->GetAbsorptionCount() << G4endl;
//      G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
//      << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
//      << G4endl;
//      G4cout << "Unacounted for photons in this event : "
//      << (eventInformation->GetPhotonCount_Ceren() -
//          eventInformation->GetAbsorptionCount() -
//          eventInformation->GetHitCount() -
//          eventInformation->GetBoundaryAbsorptionCount())
//      << G4endl;
//   }
//   //If we have set the flag to save 'special' events, save here
//   if ( fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold )
//   {
//      G4RunManager::GetRunManager()->rndmSaveThisEvent();
//   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void qCalEventAction::SetSaveThreshold(G4int save){
//   /*Sets the save threshold for the random number seed. If the number of photons
//    generated in an event is lower than this, then save the seed for this event
//    in a file called run###evt###.rndm
//    */
//   fSaveThreshold = save;
//   G4RunManager::GetRunManager()->SetRandomNumberStore(true);
//   G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
//   //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
//}

