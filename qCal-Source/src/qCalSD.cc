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
qCalSD::qCalSD(G4String SDname, G4String hitsCollectionName, G4LogicalVolume* sipmLog)
: G4VSensitiveDetector(SDname)
{
   G4cout << "Creating SD with name: " << SDname << G4endl;
   collectionName.insert("qCalSipmHitCollection");
}

//Destructor
qCalSD::~qCalSD()
{
   
}

//Process Hits (Required)
G4bool qCalSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
//   qCalHit* aHit = new qCalHit();
//   collectionName.insert(aHit);
   
   G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
   G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
   G4cout << "This Detector is: " << copyNo << G4endl;
   return true;
}

//Initialize (Required)
void qCalSD::Initialize(G4HCofThisEvent* HCE)
{
//   hitCollection = new qCalSipmHitCollection(GetName(), collectionName[0]);
//   
//   static G4int HCID = -1;
//   if (HCID<0) HCID = GetCollectionID(0);
//   HCE->AddHitsCollection(HCID,hitCollection);
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
