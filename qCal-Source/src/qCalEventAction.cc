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


#include <iomanip>

#include <algorithm>


/**************************************************************************************
 Constructor:  Need SiPM Hit Collection ID
 **************************************************************************************/
qCalEventAction::qCalEventAction():
   G4UserEventAction(),
   fSiPMCollID(-1),
   eventDetector((qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction()),
   SDVolume(eventDetector->GetVolume()),
   fphotonCount{std::vector<G4double>(SDVolume, 0)},
   fSiPMNums{std::vector<G4double>(SDVolume, -1)},
   fHitTimes{ std::vector<G4double>(SDVolume, -1)},
   fInitialHitTimes{ std::vector<G4double>(SDVolume, -1)},
   fFinalHitTimes{ std::vector<G4double>(SDVolume, -1)},
   iSiPMCoordsX{ std::vector<G4int>(SDVolume, -1)},
   iSiPMCoordsY{ std::vector<G4int>(SDVolume, -1)},
   iSiPMCoordsZ{ std::vector<G4int>(SDVolume, -1)}


{
   //Set Print Progress to every event.
   G4RunManager::GetRunManager()->SetPrintProgress(1);
}

/**************************************************************************************
 Destructor:
 **************************************************************************************/
qCalEventAction::~qCalEventAction() = default;
//{}

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
   auto eventSiPMHitCollection = dynamic_cast<qCalSiPMHitsCollection*>(hce->GetHC(fSiPMCollID));
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
               << ">>> Momentum: "  << primary->GetMomentum()/GeV << G4endl;
      G4cout << "The SiPMs collectively received: " << n_hit << " hits." << G4endl;

   }

   //Get the Analysis Manager
   auto analysisManager = G4AnalysisManager::Instance();
   analysisManager->SetVerboseLevel(1);

   // The SiPM Number and (x,y,z) SiPM coordinate Ntuples are filled first,
   // Center of the detector is at (0,0,0) for both even and odd nAxes,
   // this requires a shift along each axis by half the axis size rounded down,
   // Since each axis size is less than the total volume, coordinate components have to repeat as below:
   int nXAxis = eventDetector->GetnXAxis();
   int nYAxis = eventDetector->GetnYAxis();
   int nZAxis = eventDetector->GetnZAxis();

   for(int i = 0; i < SDVolume; i++){
      fSiPMNums[i] = i;
      // a given x-coordinate run recurs every nXAxis loops
      iSiPMCoordsX[i] = (int)floor(i)%nXAxis - (int)floor(0.5*nXAxis);
      // a given y-coordinate run recurs every nXAxis*nYAxis loops
      iSiPMCoordsY[i] = (int)floor(1.0*i/nXAxis)%nYAxis - (int)floor(0.5*nYAxis);
      // z-coordinate runs do not repeat:
      iSiPMCoordsZ[i] = (int)floor(1.0*i/(nXAxis*nYAxis)) - (int)floor(0.5*nZAxis);
   }


   // The hits are iterated to give them SiPMNumbers (IDs) and extract photon counts and hit times:
   for ( G4int i = 0; i < n_hit; ++i ) {
      qCalHit *hit = (*eventSiPMHitCollection)[i];
      // Set hit integer coordinates and SiPM number from scale factor:
      G4int IDofHit = eventDetector->RawCoordsToSiPMNumber(hit->GetSiPMCoords());
      hit->SetSiPMNumber(IDofHit);
      // Increment the hit count for the hit with the current SiPM number:
      fphotonCount[IDofHit] += hit->GetPhotonCount();
      // Set initial and final times for hits at the current SiPM number:

      if (fphotonCount[IDofHit] > 0) {
         if (hit->GetTime() >= fFinalHitTimes[IDofHit]) {
            fFinalHitTimes[IDofHit] = hit->GetTime();
         }
         if (hit->GetTime() < fInitialHitTimes[IDofHit] || fInitialHitTimes[IDofHit] == -1) {
            fInitialHitTimes[IDofHit] = hit->GetTime();
         }
     }
   }

   // Single hit times are calculated for each SiPM that fill the hit times ntuple:
   // Create histograms of hit times for each SiPM and extract the peak value which is the mode of hit times for that SiPM.
   // An initial estimate of the histogram bin width is 0.015 - 0.020 nsecs:
   G4int i_totalPhotonCount = 0;
   G4double timeBinWidth = 0.015;

   for (G4int id = 0; id < SDVolume ; ++id)
   {
      // Sum up all theq
      i_totalPhotonCount += (int)fphotonCount[id];

      G4double thi = fFinalHitTimes[id];
      G4double tlow = fInitialHitTimes[id];
      // Calculate the number of bins given the bin width and initial/final times:
      auto timeBins = (int)(ceil((thi-tlow) / timeBinWidth) + 1);
      //timeBins = timeBins <= 0.0 ? 1 : timeBins; // Force greater than 1
      if (thi > tlow && thi >= 0 && tlow >= 0)
      {
         analysisManager->SetH1(0, timeBins, tlow, thi); // Hist ID 0
         for (G4int i = 0; i < n_hit; ++i)
         {
            qCalHit *hit = (*eventSiPMHitCollection)[i];
            int IDofHit = hit->GetSiPMNumber();
            if (IDofHit == id)
            {
               analysisManager->FillH1(0, hit->GetTime());
           }
         }
         G4int nHistEntries = analysisManager->GetH1(0)->all_entries();
         auto maxModeLength = (int)analysisManager->GetH1(0)->max_bin_height();
         for (G4int i = 0; i < nHistEntries; ++i) {
            if (analysisManager->GetH1(0)->bin_height(i) == maxModeLength) {
               fHitTimes[id] = analysisManager->GetH1(0)->bin_center(i);
               break;
            }
         }
      }
      // If condition not met then the SiPM received only 1 hit:
      else{
         fHitTimes[id] = thi;
      }
      // Clear histogram of hit times:
      analysisManager->GetH1(0)->reset();
   }
   // Adds the events photon count to the histogram:
   analysisManager->FillH1(1, i_totalPhotonCount);

   // Update all the root ntuples:
   //analysisManager->AddNtupleRow();

   // Reset the SiPM counts and Hit Times to zero for a new event.
   for (int i = 0; i < SDVolume; i++){
      fphotonCount[i] = 0.;
      //fHitTimes[i] = -1.;
      //fFinalHitTimes[i] = -1.;
      //fInitialHitTimes[i] = -1.;
   }
}













