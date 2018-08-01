#include "G4VSensitiveDetector.hh"

#include "qCalSD.hh"
#include "qCalHit.hh"
#include "qCalDetectorConstruction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"


//Constructor
qCalSD::qCalSD(G4String SDname)
: G4VSensitiveDetector(SDname), fSiPMHitCollection(0),fSiPMPositionsX(0),fSiPMPositionsY(0),fSiPMPositionsZ(0)
{
   collectionName.insert("SiPMHitCollection");
}

//Destructor
qCalSD::~qCalSD()
{
   
}

void qCalSD::SetSiPMPositions(const std::vector<G4ThreeVector>& positions)
{
   for (G4int i=0; i<G4int(positions.size()); ++i) {
      if(fSiPMPositionsX)fSiPMPositionsX->push_back(positions[i].x());
      if(fSiPMPositionsY)fSiPMPositionsY->push_back(positions[i].y());
      if(fSiPMPositionsZ)fSiPMPositionsZ->push_back(positions[i].z());
   }
}

//Initialize (Required)
void qCalSD::Initialize(G4HCofThisEvent* hitsCE)
{
   fSiPMHitCollection = new qCalSiPMHitsCollection(SensitiveDetectorName,collectionName[0]);
   //Store collection with event and keep ID
   static G4int hitCID = -1;
   if(hitCID<0){
      hitCID = GetCollectionID(0);
   }
   hitsCE->AddHitsCollection( hitCID, fSiPMHitCollection );
}

//Process Hits (Required)
G4bool qCalSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
   return false;
}

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode
G4bool qCalSD::ProcessHits_constStep(const G4Step* aStep,
                                       G4TouchableHistory* ){
   
   //need to know if this is an optical photon
   if (aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
   {
      return false;
   }
   
   //User replica number 1 since photocathode is a daughter volume
   //to the pmt which was replicated
   G4int SiPMNumber = aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetCopyNo();
   G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();
   
   //Find the correct hit collection
   G4int n = fSiPMHitCollection->entries();
   qCalHit* hit=NULL;
   for ( G4int i = 0; i < n; i++ )
   {
      if ( (*fSiPMHitCollection)[i]->GetSiPMNumber() == SiPMNumber)
      {
         hit = (*fSiPMHitCollection)[i];
         break;
      }
   }
   
   if ( hit==NULL){//this pmt wasnt previously hit in this event
      hit = new qCalHit(); //so create new hit
      hit->SetSiPMNumber(SiPMNumber);
      hit->SetSiPMPhysVol(physVol);
      fSiPMHitCollection->insert(hit);
      hit->SetSiPMPos((*fSiPMPositionsX)[SiPMNumber],(*fSiPMPositionsY)[SiPMNumber],
                     (*fSiPMPositionsZ)[SiPMNumber]);
   }
   
   hit->IncPhotonCount(); //increment hit for the selected pmt
   
   hit->SetDrawit(true);
   
   return true;
}

//End event (Required)
void qCalSD::EndOfEvent(G4HCofThisEvent*)
{
   
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
