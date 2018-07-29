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
qCalSD::qCalSD(G4String name, G4String hitsCollectionName, G4LogicalVolume* sipmLog)
: G4VSensitiveDetector(name)
{
   
}

//Destructor
qCalSD::~qCalSD()
{
   
}

//Process Hits (Required)
G4bool qCalSD::ProcessHits(G4Step* , G4TouchableHistory*)
{
   return false;
}

//Initialize (Required)
void qCalSD::Initialize(G4HCofThisEvent*)
{
   
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
