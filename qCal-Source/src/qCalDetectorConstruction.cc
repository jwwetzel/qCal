#include "qCalDetectorConstruction.hh"
//#include "qCalSD.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include <vector>

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4OpticalSurface.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"




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
//   G4NistManager* nist = G4NistManager::Instance();
   
   // Option to switch on/off checking of volumes overlaps
   G4bool checkOverlaps = true;
   
   /**************************************************************************************
    Material Definitions
    **************************************************************************************/
   //General Variables for Material definitions
   G4double a, z, density;
   G4int nelements;
   G4int ncomponents, natoms;
   const G4int nEntries = 32;
   
   //Photon Energies
   G4double PhotonEnergyQ[nEntries] ={
      0.44*eV, 0.623125*eV, 0.80625*eV, 0.989375*eV,
      1.1725*eV, 1.355625*eV, 1.53875*eV, 1.721875*eV,
      1.905*eV, 2.088125*eV, 2.27125*eV, 2.454375*eV,
      2.6375*eV, 2.820625*eV, 3.00375*eV, 3.186875*eV,
      3.37*eV, 3.553125*eV, 3.73625*eV, 3.919375*eV,
      4.1025*eV, 4.285625*eV, 4.46875*eV, 4.651875*eV,
      4.835*eV, 5.018125*eV, 5.20125*eV, 5.384375*eV,
      5.5675*eV, 5.750625*eV, 5.93375*eV, 6.3*eV};
   
   //Air Definition
   G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
   G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
   
   G4Material* airMat = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
   airMat->AddElement(N, 70.*perCent);
   airMat->AddElement(O, 30.*perCent);
   
   G4double RefractiveIndexAir[nEntries] =
   { 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
      1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
      1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
      1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
      1.000293, 1.000293, 1.000293, 1.000293 };
   
   G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
   airMPT->AddProperty("RINDEX",       PhotonEnergyQ, RefractiveIndexAir,nEntries);
   airMat->SetMaterialPropertiesTable(airMPT);
   
   //Quartz Material Definition:
   G4Element* Si = new G4Element("Silicon", "Si", z=14., a=28.09*g/mole);  // Silicon Definition
   
   G4Material* quartzMat = new G4Material("Quartz",density= 2.32*g/cm3, ncomponents=2);
   quartzMat->AddElement(Si, natoms=1);
   quartzMat->AddElement(O , natoms=2);
   
   G4double RefractiveIndexQ[nEntries] =
   { 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
      1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
      1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
      1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
      1.4585, 1.4585, 1.4585, 1.4585 };
   
   G4double AbsorptionQ[nEntries] =
   {0.07*m, 0.069375*m, 0.06875*m, 0.068125*m,
      0.0675*m, 0.066875*m, 0.06625*m, 0.065625*m,
      0.065*m, 0.064375*m,  0.06375*m, 0.063125*m,
      0.0625*m, 0.061875*m, 0.06125*m, 0.060625*m,
      0.06*m, 0.059375*m, 0.05875*m, 0.058125*m,
      0.0575*m, 0.056875*m, 0.05625*m, 0.055625*m,
      0.055*m, 0.054375*m, 0.05375*m, 0.053125*m,
      0.0525*m, 0.051875*m, 0.05125*m, 0.05*m};
   
   G4MaterialPropertiesTable* quartzMPT = new G4MaterialPropertiesTable();
   quartzMPT->AddProperty("RINDEX",       PhotonEnergyQ, RefractiveIndexQ,nEntries);
   quartzMPT->AddProperty("ABSLENGTH",    PhotonEnergyQ, AbsorptionQ,     nEntries);
   
   quartzMat->SetMaterialPropertiesTable(quartzMPT);
   
   
   /**************************************************************************************
    Solid, Logical Volume, and Physical Volume Definitions
    **************************************************************************************/
   
   //World Definition
   G4double world_sizeXY = 500*cm;
   G4double world_sizeZ  = 500*cm;
   
   G4Box* solidWorld =
   new G4Box("World",                       //its name
             0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
   
   G4LogicalVolume* logicWorld =
   new G4LogicalVolume(solidWorld,          //its solid
                       airMat,           //its material
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
   

   //Quartz Solid Definition:
   G4Box* solidQuartz =
   new G4Box("Quartz",                       //its name
             0.5*p_fCubeWidth, 0.5*p_fCubeWidth, 0.5*p_fCubeWidth);     //its size

   G4LogicalVolume* logicQuartz =
   new G4LogicalVolume(solidQuartz,          //its solid
                       quartzMat,           //its material
                       "Quartz");            //its name
   
   G4VPhysicalVolume* physicalQuartz =
   new G4PVPlacement(0,
                     G4ThreeVector(),
                     logicQuartz,
                     "Quartz",
                     logicWorld,
                     false,
                     0,
                     checkOverlaps
               );
   
   return physWorld;
}



































