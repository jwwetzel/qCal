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

std::map<G4ThreeVector, int> qCalSD::normalizedMap = {}; // ?

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
   std::map<G4ThreeVector, int>::iterator it = mapOfHits.find(posVector);

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
   
   qCalHit* hit = NULL;
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
   
   
   if ( hit == NULL && isHit == 1)
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
   G4double offsetX = 10;
   G4double offsetY = 10;
   G4double offsetZ = 10;
   G4double normOffsetZ;
   G4double normOffsetXY;
   bool newZfound = false;
   std::map<G4ThreeVector, int> finalMap;
   G4ThreeVector newVector;
   G4double newZ;

   for (auto iter = mapOfHits.cbegin(); iter != mapOfHits.cend(); iter++) {

      G4ThreeVector posAt = iter->first;
      G4double newXtemp = (posAt.getX()) - (offsetX);
      G4double newYtemp = (posAt.getY()) - (offsetY);
      G4double newZtemp = (posAt.getZ()) - (offsetZ);
      if (abs(offsetX) >= abs(posAt.getX())) {
      }
      if (abs(offsetY) >= abs(posAt.getY())) {
      }
      if (abs(offsetZ) >= abs(posAt.getZ())) {
         offsetZ = posAt.getZ();
      }
   }

   ((qCalDetectorConstruction *) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->SetOffsetZ(offsetZ);

   normOffsetXY = 1 / cm; //(p_fcubeSize / cm);
   if (p_nZAxis % 2 == 0) {
      normOffsetZ = ((p_fAbsLen) + (p_fcubeSize)) / cm;
   } else {
      normOffsetZ = (((p_fAbsLen / 2) + (p_fcubeSize / 2)) * 2) / cm;
   }

   for (auto iter2 = mapOfHits.cbegin(); iter2 != mapOfHits.cend(); iter2++) {
      G4ThreeVector posAt = iter2->first;
      G4int hitNo = iter2->second;
      G4double newX = (posAt.getX());// - (offsetX);
      G4double newY = (posAt.getY());// -(offsetY);
      G4double newZ = (posAt.getZ()) - (offsetZ);
      newVector = G4ThreeVector(newX, newY, newZ / normOffsetZ);
      finalMap.insert(std::make_pair(newVector, hitNo));
   }

   setNormalizedMap(finalMap);
   finalMap.clear();
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

