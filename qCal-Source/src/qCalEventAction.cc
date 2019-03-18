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

#include <algorithm>


/**************************************************************************************
 Constructor:  Need SiPM Hit Collection ID
 **************************************************************************************/
qCalEventAction::qCalEventAction():
   G4UserEventAction(),
   fSiPMCollID(-1),
   eventDetector((qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction()),
   SDVolume(eventDetector->GetVolume()),
   fphotonCount{std::vector<G4double>(SDVolume, 0.)},
   fSiPMNums{std::vector<G4double>(SDVolume, -1)},
   fHitTimes{ std::vector<G4double>(SDVolume, -1)},
   fSiPMCordsX{ std::vector<G4int>(SDVolume, -1)},
   fSiPMCordsY{ std::vector<G4int>(SDVolume, -1)},
   fSiPMCordsZ{ std::vector<G4int>(SDVolume, -1)}


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
   analysisManager->SetVerboseLevel(1);
   
   // Filling The SiPM number ntuple (contains IDs of the SiPMs):
   for (G4int i = 0; i < SDVolume; ++i){
      fSiPMNums[i] = i;
   }

   // Filling the X,Y,Z coordinate ntuples:

   std::map<G4ThreeVector, int> mapOfHits = qCalSD::returnMap();
   G4int counter = 0;
   for (auto iter3 = mapOfHits.cbegin(); iter3 != mapOfHits.cend(); iter3++)
   {
      G4ThreeVector tempPos = iter3->first;
      G4int tempX = tempPos.getX();
      G4int tempY = tempPos.getY();
      G4int tempZ = tempPos.getZ();

      fSiPMCordsX[counter] = tempX;
      fSiPMCordsY[counter] = tempY;
      fSiPMCordsZ[counter] = tempZ;
      counter = counter+1;
   }


   G4int nZAxis = eventDetector->GetnZAxis();
   G4float absLen =  eventDetector->GetAbsLen();
   G4double cubeSize = eventDetector->GetCubeSize();
   G4double offsetZ = eventDetector->GetOffsetZ();
   G4double scaleZ;

   // Calculate scale factor that normalizes SiPM Z-coordinate to integer:
   if (nZAxis% 2 == 0){
      scaleZ = ((absLen)+(cubeSize)) / cm;
   }
   else{
      scaleZ = (((absLen/ 2) + (cubeSize / 2)) *2) / cm;
   }

   std::vector<G4double> initialHitTimes(SDVolume, -1);
   std::vector<G4double> finalHitTimes(SDVolume, -1);

   // Iterate through hits to add hit coordinates and hit counts:
   for ( G4int i = 0; i < n_hit; ++i ) {
      qCalHit *hit = (*eventSiPMHitCollection)[i];
      // Set hit integer coordinates and SiPM number from scale factor:
      hit->NormalizeZCoord(offsetZ, scaleZ);
      hit->SetSiPMNumber(eventDetector->GetSiPMIDfromPosition(hit->GetSiPMCoords()));

      int IDofHit = hit->GetSiPMNumber();
      // Increment the hit count for the hit with the current SiPM number:
      fphotonCount[IDofHit] += hit->GetPhotonCount();

      // Set initial and final times for hits at the current SiPM number:
      if (fphotonCount[IDofHit] > 0) {
         if (hit->GetTime() >= finalHitTimes[IDofHit]) {
            finalHitTimes[IDofHit] = hit->GetTime();
         }
         if (hit->GetTime() < initialHitTimes[IDofHit] || initialHitTimes[IDofHit] == -1) {
            initialHitTimes[IDofHit] = hit->GetTime();
         }
      }
   }

   // Fill the hit times ntuple:
   // Create histograms of hit times for each SiPM and extract the peak value which is the mode of hit times for that SiPM.
   // An initial estimate of the histogram bin width is 0.015 - 0.020 nsecs:

   G4double timeBinWidth = 0.015;

   for (G4int id = 0; id < SDVolume ; ++id){
      G4double thi = finalHitTimes[id];
      G4double tlow = initialHitTimes[id];

      // Calculate the number of bins given the bin width and initial/final times:
      G4int timeBins = ceil((thi-tlow) / timeBinWidth) + 1;

      if (thi != -1 && tlow != -1 && thi > tlow) {
         analysisManager->SetH1(0, timeBins, tlow, thi); // Hist ID 0
         for (G4int i = 0; i < n_hit; ++i) {
            qCalHit *hit = (*eventSiPMHitCollection)[i];
            int IDofHit = hit->GetSiPMNumber();

            if (IDofHit == id) {
               analysisManager->FillH1(0, hit->GetTime());
            }
         }

         G4int nHistEntries = analysisManager->GetH1(0)->all_entries();
         G4int maxModeLength = analysisManager->GetH1(0)->max_bin_height();

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

   // Update all the root ntuples:
   analysisManager->AddNtupleRow();

   // Reset the SiPM counts and Hit Times to zero for a new event.
   for (int i = 0; i < SDVolume; i++){
      fphotonCount[i] = 0.;
      fHitTimes[i] = -1.;
   }
}













