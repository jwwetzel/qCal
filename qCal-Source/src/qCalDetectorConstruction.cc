#include "qCalDetectorConstruction.hh"
#include "qCalSD.hh"

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
                                                   G4String sAbs,
                                                   G4float fCubeWidth)
: G4VUserDetectorConstruction()
{
   p_nXAxis = nXAxis;                  //Number of cubes in the X-Axis
   p_nYAxis = nYAxis;                  //Number of cubes in the Y-Axis
   p_nZAxis = nZAxis;                  //Number of cubes in the Z-Axis
   p_sAbs = sAbs;                      //Absorber element
   p_fCubeWidth = fCubeWidth * cm;     //Width of a single cube
   p_fQuartzSpacing = 0.15*cm;         //Width between x-cubes (circuit board + sipm)
   p_fWrapSize     = 0.015*cm;         //Width of the tyvek wrapping
   p_fAbsXDim = 0.5*((p_fCubeWidth+2*p_fWrapSize)*(p_nXAxis)+((p_fQuartzSpacing)*(p_nXAxis-1)))+p_fQuartzSpacing;     //Detector X coord center
   p_fAbsYDim = 0.5*(p_fCubeWidth+2*p_fWrapSize)*p_nYAxis+p_fQuartzSpacing;                                           //Detector Y coord Center
   p_SiPMDim = 0.5*cm;
}

qCalDetectorConstruction::~qCalDetectorConstruction()
{
   
}

G4VPhysicalVolume* qCalDetectorConstruction::Construct()
{
   // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();
   
   // Option to switch on/off checking of volumes overlaps
   G4bool checkOverlaps = false;

   
   /**************************************************************************************
    Material Definitions
    **************************************************************************************/
   //General Variables for Material definitions
   G4double a, z, density;
   G4int nelements;
   G4int ncomponents, natoms;
   const G4int nEntries = 32;
   
   //Define the User selected Absorber Material
   G4Material* absorberMat = nist->FindOrBuildMaterial("G4_"+p_sAbs);
   G4float fAbsRadLen = absorberMat->GetRadlen()*mm;
   
   G4cout << "RadL: " << fAbsRadLen << G4endl;
   
   //Photon Energies for Optical Photon calculations
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
   G4double world_sizeXY = 50*cm;
   G4double world_sizeZ  = 50*cm;
   
   G4Box* solidWorld =
   new G4Box("World",                                                   //its name
             0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);      //its size
   
   G4LogicalVolume* logicWorld =
   new G4LogicalVolume(solidWorld,                                      //its solid
                       airMat,                                          //its material
                       "World");                                        //its name
   
   G4VPhysicalVolume* physWorld =
   new G4PVPlacement(0,                      //no rotation
                     G4ThreeVector(),        //at (0,0,0)
                     logicWorld,             //its logical volume
                     "World",                //its name
                     nullptr,                //its mother  volume
                     false,                  //no boolean operation
                     0,                      //copy number
                     checkOverlaps);         //overlaps checking
   

   //Quartz Solid & Volume Definition:
   G4Box* solidQuartz =
   new G4Box("Quartz",                                                  //its name
             0.5*p_fCubeWidth, 0.5*p_fCubeWidth, 0.5*p_fCubeWidth);     //its size

   G4LogicalVolume* logicQuartz =
   new G4LogicalVolume(solidQuartz,                                     //its solid
                       quartzMat,                                       //its material
                       "Quartz");                                       //its name
   
   //SiPM Solid & Volume Definition:
   G4Box* solidSiPM =                                                   //its name
   new G4Box("SiPM", 0.5*p_SiPMDim, 0.5*p_SiPMDim, 0.001*cm);            //its size
   
   fLogicSiPM =
   new G4LogicalVolume(solidSiPM,
                       quartzMat,
                       "SiPM");
   
   
   //SiPM Sensitive Detector Declaration
   qCalSD* sipmSD = new qCalSD("SiPM");
   G4SDManager *sdMan = G4SDManager::GetSDMpointer();
   sdMan->AddNewDetector(sipmSD);
   
   //Absorber Solid & Volume Definition:
   G4float fAbsZDim = 0.5*fAbsRadLen;
   G4Box* solidAbsorber =
   new G4Box("Absorber",                                                //its names
             p_fAbsXDim, p_fAbsYDim, fAbsZDim);                             //its size
   
   G4LogicalVolume* logicAbsorber =
   new G4LogicalVolume(solidAbsorber,                                   //its solid
                       absorberMat,                                     //its material
                       "Absorber");                                     //its name
   
   //Quartz Physical Volume Placement
   G4int quartzIDNumber = 0;
   G4int absorberIDNumber = p_nXAxis*p_nYAxis*p_nZAxis+10;
   G4int sipmIDNumber = 2*p_nXAxis*p_nYAxis*p_nZAxis+10;
   G4int rowOffset = 0;
   for (G4int i = 0; i != p_nXAxis; ++i)
   {
      for ( G4int j = 0; j != p_nYAxis; ++j)
      {
         for ( G4int k = 0; k != p_nZAxis; ++k)
         {
            if (k % 2 == 0) rowOffset = 2;
            else rowOffset = 0;
            //Place the Quartz
            G4VPhysicalVolume* physicalQuartz =
            new G4PVPlacement(0,
                              G4ThreeVector((p_fCubeWidth+p_fQuartzSpacing+2*p_fWrapSize)*i+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,  //X
                                            (p_fCubeWidth+2*p_fWrapSize)*j+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,                   //Y
                                            (fAbsRadLen+p_fCubeWidth+2*p_fWrapSize)*k+0.5*p_fCubeWidth+p_fWrapSize),                                  //Z
                              logicQuartz,
                              "Quartz",
                              logicWorld,
                              false,
                              quartzIDNumber,
                              checkOverlaps
                              );
            
            //Place the SiPMs
            G4VPhysicalVolume* physicalSiPM =
            new G4PVPlacement(0,
                              G4ThreeVector((p_fCubeWidth+p_fQuartzSpacing+2*p_fWrapSize)*i+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,  //X
                                            (p_fCubeWidth+2*p_fWrapSize)*j+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,                   //Y
                                            (fAbsRadLen+p_fCubeWidth+2*p_fWrapSize)*k+p_fCubeWidth+0.001*cm+p_fWrapSize),                             //Z
                              fLogicSiPM,
                              "SiPM",
                              logicWorld,
                              false,
                              sipmIDNumber,
                              checkOverlaps
                              );
            fSiPMPositions.push_back(G4ThreeVector((p_fCubeWidth+p_fQuartzSpacing+2*p_fWrapSize)*i+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,
                                                   (p_fCubeWidth+2*p_fWrapSize)*j+0.5*p_fCubeWidth+p_fWrapSize+p_fQuartzSpacing*rowOffset,
                                                   (fAbsRadLen+p_fCubeWidth+2*p_fWrapSize)*k+p_fCubeWidth+0.001*cm+p_fWrapSize));

            //Set the Quartz Surface
            G4OpticalSurface* quartzWrap = new G4OpticalSurface("QuartzWrap");
            
            new G4LogicalBorderSurface("QuartzWrap",
                                       physicalQuartz,
                                       physWorld,
                                       quartzWrap);
            
            quartzWrap->SetType(dielectric_metal);
            quartzWrap->SetFinish(polished);
            quartzWrap->SetModel(glisur);
            
            G4double pp[2] = {2.0*eV, 3.5*eV};
            G4double reflectivity[2] = {1., 1.};
            G4double efficiency[2] = {0.0, 0.0};
            
            G4MaterialPropertiesTable* quartzWrapMPT = new G4MaterialPropertiesTable();
            
            quartzWrapMPT->AddProperty("REFLECTIVITY",pp,reflectivity,2);
            quartzWrapMPT->AddProperty("EFFICIENCY",pp,efficiency,2);
            quartzWrap->SetMaterialPropertiesTable(quartzWrapMPT);
            
            //Place the Absorber
            if (i == 0 && j == 0)
            {
               G4VPhysicalVolume* physicalAbsorber =
               new G4PVPlacement(0,
                                 G4ThreeVector(p_fAbsXDim,
                                               p_fAbsYDim,
                                               (p_fCubeWidth+0.5*fAbsRadLen+2*p_fWrapSize)+(p_fCubeWidth+fAbsRadLen+2*p_fWrapSize)*k),
                                 logicAbsorber,
                                 "Absorber",
                                 logicWorld,
                                 false,
                                 absorberIDNumber,
                                 checkOverlaps
                                 );
               absorberIDNumber++;
            }
            quartzIDNumber++;
            sipmIDNumber++;
         }
      }
   }

   return physWorld;
}


void qCalDetectorConstruction::ConstructSDandField()
{
   if (!fSiPM_SD.Get())
   {
      qCalSD* SiPM_SD = new qCalSD("/qCalDet/SiPMSD");
      fSiPM_SD.Put(SiPM_SD);
      SiPM_SD->InitSiPMs(p_nXAxis*p_nYAxis*p_nZAxis);
      SiPM_SD->SetSiPMPositions(fSiPMPositions);
   }

   G4SDManager::GetSDMpointer()->AddNewDetector(fSiPM_SD.Get());

   SetSensitiveDetector(fLogicSiPM,fSiPM_SD.Get());
}





























