#include "qCalDetectorConstruction.hh"
//#include "qCalSD.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4Material.hh"
#include "G4Threading.hh"
#include <vector>

qCalDetectorConstruction::qCalDetectorConstruction()
: G4VUserDetectorConstruction()
{

}

qCalDetectorConstruction::~qCalDetectorConstruction()
{
   
}

G4VPhysicalVolume* qCalDetectorConstruction::Construct()
{
   // Get nist material manager
   G4NistManager* nist = G4NistManager::Instance();
   
   // Envelope parameters
   //
   G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
   G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
   // Option to switch on/off checking of volumes overlaps
   //
   G4bool checkOverlaps = true;
   
   //
   // World
   //
   G4double world_sizeXY = 1.2*env_sizeXY;
   G4double world_sizeZ  = 1.2*env_sizeZ;
   G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
   
   G4Box* solidWorld =
   new G4Box("World",                       //its name
             0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
   
   G4LogicalVolume* logicWorld =
   new G4LogicalVolume(solidWorld,          //its solid
                       world_mat,           //its material
                       "World");            //its name
   
   G4VPhysicalVolume* physWorld =
   new G4PVPlacement(0,                     //no rotation
                     G4ThreeVector(),       //at (0,0,0)
                     logicWorld,            //its logical volume
                     "World",               //its name
                     0,                     //its mother  volume
                     false,                 //no boolean operation
                     0,                     //copy number
                     checkOverlaps);        //overlaps checking
   
   return physWorld;
}
