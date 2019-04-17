#include "G4VSensitiveDetector.hh"

#include "qCalSD.hh"
#include "qCalHit.hh"
#include "qCalDetectorConstruction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include <map>


//Constructor
qCalSD::qCalSD(G4String SDname, G4float absLen, G4double cubeSize, G4int noOfZ)
: G4VSensitiveDetector(SDname), fSiPMHitCollection(0),fSiPMPositionsX(0),fSiPMPositionsY(0),fSiPMPositionsZ(0)
{
   collectionName.insert("SiPMHitCollection");
   p_fAbsLen = absLen;
   p_fcubeSize = cubeSize;
   p_nZAxis = noOfZ;
}

//Destructor
qCalSD::~qCalSD()
{
   
}

//Initialize (Required)
void qCalSD::Initialize(G4HCofThisEvent* hitsCE)
{
   fSiPMHitCollection = new qCalSiPMHitsCollection(SensitiveDetectorName, collectionName[0]);
   //Store collection with event and keep ID
   static G4int hitCID = -1;
   if(hitCID<0)
   {
      hitCID = GetCollectionID(0);
   }
   hitsCE->AddHitsCollection( hitCID, fSiPMHitCollection );
}


//Process Hits (Required)
G4bool qCalSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
   if (step->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
   {
      return false;
   }
   
   G4StepPoint* preStepPoint = step->GetPreStepPoint();
   G4Track *aTrack = step->GetTrack() ;

   G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
   ///

   G4ThreeVector copyNoPos = touchable->GetTranslation();
   /////Storing the smallest vector(which will be (0,0,0), as well as the second smallest which will be (0,0,1);
   G4double tempX = copyNoPos.getX()/cm;
   G4double tempY = copyNoPos.getY()/cm;
   G4double tempZ = copyNoPos.getZ()/cm;
   G4ThreeVector posVector = G4ThreeVector(tempX, tempY, tempZ);
   auto it = mapOfHits.find(posVector);

   if (it != mapOfHits.end())
   {
      it->second++;
   }
   else
   {
      mapOfHits.insert(std::make_pair(posVector, 1));
   }

   ///
   G4int copyNo                  = touchable->GetVolume()->GetCopyNo();
   G4double hitTime              = preStepPoint->GetGlobalTime();
   G4double photonWavelength     = 4.15e-15*3e8/(step->GetTrack()->GetTotalEnergy()/eV)*1e9;  //nm
   qCalHit* hit = nullptr;
   G4int isHit = 0;
   
   // Simulating the Hamamatsu S13360-**75CS
   // https://www.hamamatsu.com/resources/pdf/ssd/s13360_series_kapd1052e.pdf
   
   if (photonWavelength >= 280 && photonWavelength < 300) isHit = (rand() % 100) < 15;
   else if (photonWavelength >= 300 && photonWavelength < 350) isHit = (rand() % 100) < 30*0.85;
   else if (photonWavelength >= 350 && photonWavelength < 400) isHit = (rand() % 100) < 40*0.85;
   else if (photonWavelength >= 400 && photonWavelength < 500) isHit = (rand() % 100) < 47*0.85;
   else if (photonWavelength >= 500 && photonWavelength < 550) isHit = (rand() % 100) < 45*0.85;
   else if (photonWavelength >= 550 && photonWavelength < 600) isHit = (rand() % 100) < 35*0.85;
   else if (photonWavelength >= 600 && photonWavelength < 700) isHit = (rand() % 100) < 25*0.85;
   else if (photonWavelength >= 700 && photonWavelength < 800) isHit = (rand() % 100) < 15*0.85;
   else if (photonWavelength >= 800 && photonWavelength < 900) isHit = (rand() % 100) < 5*0.85;
   
   
   if ( hit == nullptr && isHit == 1)
   {
      hit = new qCalHit(posVector, hitTime, photonWavelength);
      fSiPMHitCollection->insert(hit);
      aTrack->SetTrackStatus(fStopAndKill);
      hit->SetDrawit(true);
      hit->IncPhotonCount();


   }
   
   return false;
}

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode
//G4bool qCalSD::ProcessHits_constStep(const G4Step* aStep,
//                                       G4TouchableHistory* ){
//
//   //need to know if this is an optical photon
//   if (aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
//   {
//      return false;
//   }
//
//   //User replica number 1 since photocathode is a daughter volume
//   //to the pmt which was replicated
//   G4int SiPMNumber = aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetCopyNo();
//   G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();
//
//   //Find the correct hit collection
//   G4int n = fSiPMHitCollection->entries();
//   qCalHit* hit=NULL;
//   for ( G4int i = 0; i < n; i++ )
//   {
//      if ( (*fSiPMHitCollection)[i]->GetSiPMNumber() == SiPMNumber)
//      {
//         hit = (*fSiPMHitCollection)[i];
//         break;
//      }
//   }
//
//   if ( hit==NULL){//this pmt wasnt previously hit in this event
//      hit = new qCalHit(); //so create new hit
//      hit->SetSiPMNumber(SiPMNumber);
//      hit->SetSiPMPhysVol(physVol);
//      fSiPMHitCollection->insert(hit);
//      hit->SetSiPMPos((*fSiPMPositionsX)[SiPMNumber],(*fSiPMPositionsY)[SiPMNumber],
//                     (*fSiPMPositionsZ)[SiPMNumber]);
//   }
//
//   hit->IncPhotonCount(); //increment hit for the selected pmt
//
//   hit->SetDrawit(true);
//
//   return true;
//}

//End event (Required)



void qCalSD::EndOfEvent(G4HCofThisEvent*) {
   // The following iterative method below is used calculate the Z-offset that converts the raw
   // floating-point z component of the detector to an integer value, this value is constant with SiPM dimensions
   // but scales strongly with absorber material. If the SiPM X and Y dimensions change offsetX and offsetY need
   // have to be calculated. Direct formulas for the offsets may be needed in the future.

   G4double offsetX;
   G4double offsetY;
   G4double offsetZ;

   int count = 0;
   for (auto iter = mapOfHits.cbegin(); iter != mapOfHits.cend(); iter++) {

      G4ThreeVector posAt = iter->first;
      /*
      if (fabs(offsetX) > fabs(posAt.getX()) || count == 0) {
         offsetX = fabs(posAt.getX());
      }
      if (fabs(offsetY) > fabs(posAt.getY()) || count == 0) {
         offsetY = fabs(posAt.getY());
      }
      */
      if (fabs(offsetZ) > fabs(posAt.getZ()) || count == 0) {
         offsetZ = fabs(posAt.getZ());
      }

      count++;
   }
   //G4cout << "THE Z-OFFSET IS: " << offsetZ << G4endl;

   ((qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->SetCoordOffsetZ(offsetZ);

   mapOfHits.clear();

}

//Clear hit markings
void qCalSD::Clear()
{
   
}

//Draw hit markings
void qCalSD::DrawAll()
{
   
}

//Draw hit markings
void qCalSD::PrintAll()
{
   
}

