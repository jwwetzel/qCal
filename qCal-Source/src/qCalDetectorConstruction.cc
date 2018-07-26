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
#include "G4PVReplica.hh"
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

qCalDetectorConstruction::qCalDetectorConstruction(G4int nXAxis,
                                                   G4int nYAxis,
                                                   G4int nZAxis,
                                                   G4int nAbsZ,
                                                   G4float fCubeWidth)
: G4VUserDetectorConstruction()
{
   p_nXAxis = nXAxis;
   p_nYAxis = nYAxis;
   p_nZAxis = nZAxis;
   p_nAbsZ = nAbsZ;
   p_fCubeWidth = fCubeWidth * cm;
   
}

qCalDetectorConstruction::~qCalDetectorConstruction()
{
   
}

G4VPhysicalVolume* qCalDetectorConstruction::Construct()
{
   G4cout << p_nXAxis << G4endl;
   // Get nist material manager
   G4NistManager* nist = G4NistManager::Instance();
   
   // Option to switch on/off checking of volumes overlaps
   //
   G4bool checkOverlaps = true;
   
   /**************************************************************************************
                  World Solid, Logical Volume, and Physical Volume Definition
    **************************************************************************************/
   G4double world_sizeXY = 500*cm;
   G4double world_sizeZ  = 500*cm;
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
                     nullptr,                     //its mother  volume
                     false,                 //no boolean operation
                     0,                     //copy number
                     checkOverlaps);        //overlaps checking
   
   /**************************************************************************************
    Envelope Solid, Logical Volume, and Physical Volume Definition
    **************************************************************************************/
   G4Material* envelope_mat = nist->FindOrBuildMaterial("G4_AIR");
   
   G4Box* solidEnvelope =
   new G4Box("Envelope",                       //its name
             0.5*p_fCubeWidth * p_nXAxis, 0.5*p_fCubeWidth * p_nYAxis, 0.5*p_fCubeWidth * p_nZAxis);     //its size
   
   G4LogicalVolume* logicEnvelope =
   new G4LogicalVolume(solidEnvelope,          //its solid
                       envelope_mat,           //its material
                       "Envelope");            //its name
   
   G4VPhysicalVolume* physEnvelope =
   new G4PVPlacement(0,                      //no rotation
                     G4ThreeVector(),        //at (0,0,0)
                     logicEnvelope,          //its logical volume
                     "Envelope",             //its name
                     logicWorld,             //its mother  volume
                     false,                  //no boolean operation
                     0,                      //copy number
                     checkOverlaps);         //overlaps checking
   
   /**************************************************************************************
    Quartz Solid, Logical Volume, and Physical Volume Definition
    **************************************************************************************/
   //Quartz Material Definition:
   const G4int NUMENTRIES = 3;
   G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
   G4Element* Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
   G4Material* quartz_mat = new G4Material("quartz", 2.200*g/cm3, 2);
   quartz_mat->AddElement(Si, 1);
   quartz_mat->AddElement(O , 2);

   //Quartz Material Properties Definition
   G4double quartz_PP[NUMENTRIES]   = { 5.0*eV, 6.69*eV, 7.50*eV }; // lambda range 4 ri
   G4double quartz_RIND[NUMENTRIES] = { 1.45, 1.51, 1.54 };     // ref index
   G4double quartz_ABSL[NUMENTRIES] = { 3.0*cm, 3.0*cm, 3.0*cm };// atten length
   G4MaterialPropertiesTable *quartz_mt = new G4MaterialPropertiesTable();
   quartz_mt->AddProperty("RINDEX", quartz_PP, quartz_RIND, NUMENTRIES);
   quartz_mt->AddProperty("ABSLENGTH", quartz_PP, quartz_ABSL, NUMENTRIES);
   quartz_mat->SetMaterialPropertiesTable(quartz_mt);
//
//   //Quartz Optical Surfaces & Refractive Index
//   G4double ephoton[] = {7.0*eV,7.14*eV};
//   G4double quartz_EFF[] = {0.5,0.5};
//   G4double quartz_RINDEX[] = {1.543,1.554};
//   const G4int num = sizeof(ephoton)/sizeof(G4double);
//
//   quartz_mt->AddProperty("EFFICIENCY",ephoton,quartz_EFF,num);
//   quartz_mt->AddProperty("RINDEX",ephoton,quartz_RINDEX,num); //add quartz refractive index
//   G4OpticalSurface* quartzSurface = new G4OpticalSurface("quartz_opsurf", glisur, polishedtyvekair, dielectric_metal); //ADD TYVEK here
//   quartzSurface->SetMaterialPropertiesTable(quartz_mt);
   

   
   //Quartz Solid Definition:
   G4Box* solidQuartz =
   new G4Box("Quartz",                       //its name
             0.5*p_fCubeWidth, 0.5*p_fCubeWidth, 0.5*p_fCubeWidth);     //its size

   G4LogicalVolume* logicQuartz =
   new G4LogicalVolume(solidQuartz,          //its solid
                       quartz_mat,           //its material
                       "Quartz");            //its name
   
   G4PVReplica* quartzXReplica =
   new G4PVReplica("Quartz",
               logicQuartz,
               logicEnvelope,
               kZAxis,
               p_nZAxis,
               p_fCubeWidth
               );

   
   return physWorld;
}



































