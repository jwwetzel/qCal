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


qCalDetectorConstruction::qCalDetectorConstruction(G4int nXAxis,                                   //Number of cubes in the X-Axis
                                                   G4int nYAxis,                                   //Number of cubes in the Y-Axis
                                                   G4int nZAxis,                                   //Number of cubes in the Z-Axis
                                                   G4String sAbs,                                  //Asborber Periodic Table symbol
                                                   G4float fAbsLen,                                //Absorber radiation length
                                                   G4float fCubeWidth)                             //Width of a single cube
      :G4VUserDetectorConstruction()
{
   p_nXAxis = nXAxis;                                                                              //Number of cubes in the X-Axis
   p_nYAxis = nYAxis;                                                                              //Number of cubes in the Y-Axis
   p_nZAxis = nZAxis;                                                                              //Number of cubes in the Z-Axis
   p_fAbsLen = fAbsLen;                                                                            //absorber radiation length
   p_sAbs = sAbs;                                                                                  //Absorber element
   p_fCubeWidth = fCubeWidth * cm;                                                                 //Width of a single cube
   p_fQuartzSpacing = 0.03*cm;                                                                     //Width between x-cubes (circuit board + sipm)
   p_fWrapSize = 0.015*cm;                                                                         //Width of the tyvek wrapping
   p_fAbsXDim = ((p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing) * p_nXAxis) / 2;              //Detector X coord center
   p_fAbsYDim = ((p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing) * p_nYAxis) / 2;              //Detector Y coord Center
   p_fAbsZDim = ((p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing + p_fAbsLen) * p_nZAxis) / 2;  //Detector Z coord Center
   p_SiPMDim = 0.5*cm;                                                                             //Dimension of SiPM
   p_sdCubeSize = (p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing);

}

qCalDetectorConstruction::~qCalDetectorConstruction()
{

}

G4VPhysicalVolume* qCalDetectorConstruction::Construct()
{
   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Creating the materials and initial values
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Get nist material manager
   G4NistManager* nist = G4NistManager::Instance();

   // Option to switch on/off checking of volumes overlaps
   G4bool checkOverlaps = true;

   //Creating The Materials for Quartz, and the air surrounding it.
   G4double atomicNumber, atomicWeight, density;
   G4int nElements, nAtoms;

   //Absorber Material
   const G4int nEntries = 32;

   //Photon Energies for Optical Photon calculations
   G4double PhotonEnergyQ[nEntries] = {
         0.44*eV, 0.623125*eV, 0.80625*eV, 0.989375*eV,
         1.1725*eV, 1.355625*eV, 1.53875*eV, 1.721875*eV,
         1.905*eV, 2.088125*eV, 2.27125*eV, 2.454375*eV,
         2.6375*eV, 2.820625*eV, 3.00375*eV, 3.186875*eV,
         3.37*eV, 3.553125*eV, 3.73625*eV, 3.919375*eV,
         4.1025*eV, 4.285625*eV, 4.46875*eV, 4.651875*eV,
         4.835*eV, 5.018125*eV, 5.20125*eV, 5.384375*eV,
         5.5675*eV, 5.750625*eV, 5.93375*eV, 6.3*eV };

   G4Element* N   = new G4Element("Nitrogen",   "N",  atomicNumber = 7,    atomicWeight = ((14.01*g) / mole));
   G4Element* O   = new G4Element("Oxygen",     "O",  atomicNumber = 8,    atomicWeight = ((16.00*g) / mole));
   G4Element* Ar  = new G4Element("Argon",      "Ar", atomicNumber = 18,   atomicWeight = ((39.95*g) / mole));
   G4Element* Si  = new G4Element("Silicon",    "Si", atomicNumber = 14,   atomicWeight = ((28.09*g) / mole));

   //Define Air Material
   G4Material* airMat = new G4Material("Air", density = ((1.29*mg) / cm3), nElements = 3);
   airMat->AddElement(N, 78.08*perCent);
   airMat->AddElement(O, 20.95*perCent);
   airMat->AddElement(Ar, 0.97*perCent);
   G4double RefractiveIndexAir[nEntries] =
         { 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
           1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
           1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
           1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293, 1.000293,
           1.000293, 1.000293, 1.000293, 1.000293 };

   G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
   airMPT->AddProperty("RINDEX", PhotonEnergyQ, RefractiveIndexAir, nEntries);
   airMat->SetMaterialPropertiesTable(airMPT);

   //Define Quartz Material
   G4Material* quartzMat = new G4Material("quartzCrystal", density = ((2.648*g) / cm3), nElements = 2);
   quartzMat->AddElement(Si, nAtoms = 1);

   quartzMat->AddElement(O, nAtoms = 4);

   G4double RefractiveIndexQ[nEntries] =
         { 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
           1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
           1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
           1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
           1.4585, 1.4585, 1.4585, 1.4585 };

   G4double AbsorptionQ[nEntries] =
         { 0.07*m, 0.069375*m, 0.06875*m, 0.068125*m,
           0.0675*m, 0.066875*m, 0.06625*m, 0.065625*m,
           0.065*m, 0.064375*m,  0.06375*m, 0.063125*m,
           0.0625*m, 0.061875*m, 0.06125*m, 0.060625*m,
           0.06*m, 0.059375*m, 0.05875*m, 0.058125*m,
           0.0575*m, 0.056875*m, 0.05625*m, 0.055625*m,
           0.055*m, 0.054375*m, 0.05375*m, 0.053125*m,
           0.0525*m, 0.051875*m, 0.05125*m, 0.05*m };

   G4MaterialPropertiesTable* quartzMPT = new G4MaterialPropertiesTable();
   quartzMPT->AddProperty("RINDEX", PhotonEnergyQ, RefractiveIndexQ, nEntries);
   quartzMPT->AddProperty("ABSLENGTH", PhotonEnergyQ, AbsorptionQ, nEntries);
   quartzMat->SetMaterialPropertiesTable(quartzMPT);

   //Define the User selected Absorber Material
   G4Material* absMat = nist->FindOrBuildMaterial("G4_" + p_sAbs);
   G4float fAbsRadLen = p_fAbsLen/2;

   //G4float fAbsZDim = 0.5*fAbsRadLen;
   G4double cubeSize       = (p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing)/2;
   G4double cubeSizeZ      = ((p_fCubeWidth + 2 * p_fWrapSize + p_fQuartzSpacing)/ 2) + (p_fAbsLen);
   G4Material* sipmMat     = quartzMat;

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Define the world (needs full detector + full absorber + extra space)
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4Box* solidWorld             = new G4Box("World",
                                             p_fAbsXDim + p_fCubeWidth,
                                             p_fAbsYDim + p_fCubeWidth,
                                             p_fAbsZDim + p_fCubeWidth);

   G4LogicalVolume* worldLog     = new G4LogicalVolume(solidWorld,
                                                       airMat,
                                                       "World");

   G4ThreeVector solidPos        = G4ThreeVector(0, 0, 0);

   G4VPhysicalVolume* physWorld  = new G4PVPlacement(0,               //no rotation
                                                     G4ThreeVector(), //at (0,0,0)
                                                     worldLog,        //its logical volume
                                                     "World",         //its name
                                                     nullptr,         //its mother  volume
                                                     false,           //no boolean operation
                                                     0,               //copy number
                                                     checkOverlaps);  //overlaps checking

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Create the Quartz
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4Box* solidQuartz = new G4Box("solidQuartz",
                                  0.5*p_fCubeWidth,
                                  0.5*p_fCubeWidth,
                                  0.5*p_fCubeWidth);

   G4LogicalVolume* logicQuartz = new G4LogicalVolume(solidQuartz,
                                                      quartzMat,
                                                      "logicQuartz");

   G4ThreeVector quartzPos = G4ThreeVector(0,
                                           0,
                                           0);

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Create the SiPM
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4Box* solidSiPM = new G4Box("solidSiPM",
                                0.5*p_SiPMDim,
                                0.5*p_SiPMDim,
                                0.001*cm);

   logicSiPM = new G4LogicalVolume(solidSiPM,
                                   sipmMat,
                                   "logicSiPM");

   G4ThreeVector SiPMPos = G4ThreeVector(0,
                                         0,
                                         -((p_fCubeWidth) / 2 + 0.001*cm));

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Define the whole detector(tyvec wrapping,quartz,SiPM) - "Pixel"
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4Box* solidDetector       = new G4Box("SingleDetectorTest",
                                          cubeSize,
                                          cubeSize,
                                          cubeSizeZ);

   G4Box* solidDetectorX      = new G4Box("DetectorXLayer",
                                          cubeSize*p_nXAxis,
                                          cubeSize,
                                          cubeSizeZ);

   G4Box* solidDetectorXY     = new G4Box("DetectorXYLayer",
                                          cubeSize*p_nXAxis,
                                          cubeSize*p_nYAxis,
                                          cubeSizeZ);

   G4LogicalVolume* logicDetector   = new G4LogicalVolume(solidDetector,   airMat, "logicDetector");
   G4LogicalVolume* logicDetectorX  = new G4LogicalVolume(solidDetectorX,  airMat, "logicDetectorX");
   G4LogicalVolume* logicDetectorXY = new G4LogicalVolume(solidDetectorXY, airMat, "logicDetectorXY");

   G4VPhysicalVolume* quartzPlace = new G4PVPlacement(0,                   //no rotation
                                                      quartzPos,           //at (0,0,0)
                                                      logicQuartz,         //its logical volume
                                                      "quartzOfDetector",  //its name
                                                      logicDetector,       //its mother  volume
                                                      false,               //no boolean operation
                                                      2,                   //copy number
                                                      checkOverlaps);      //overlaps checking

   new G4PVPlacement(0,                     //no rotation
                     SiPMPos,               //at (0,0,0)
                     logicSiPM,             //its logical volume
                     "sipmOfDetector",      //its name
                     logicDetector,         //its mother  volume
                     false,                 //no boolean operation
                     3,                     //copy number
                     checkOverlaps);        //overlaps checking

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Replicate pixel in XDir
   ////////////////////////////////////////////////////////////////////////////////////////////////
   new G4PVReplica("detectorLayerX", //Name
                   logicDetector,    //Logical Volume
                   logicDetectorX,   //Mother volume
                   kXAxis,           //Axis of replication
                   (p_nXAxis),       //Number of replica
                   cubeSize*2);      //Width of replica

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Replicate pixel in XYDir
   ////////////////////////////////////////////////////////////////////////////////////////////////
   new G4PVReplica("detecLayerXY",  //Name
                   logicDetectorX,  //Logical Volume
                   logicDetectorXY, //Mother volume
                   kYAxis,          //Axis of replication
                   (p_nYAxis),      //Number of replica
                   cubeSize*2);     //Width of replica

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Create an absorber behind the XYDir pixel
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4Box* solidAbsorber             = new G4Box("SingleAbsorber",
                                                (cubeSize)*(p_nXAxis),
                                                (cubeSize)*(p_nYAxis),
                                                fAbsRadLen);

   G4Box* solidFinalAbsorber        = new G4Box("FullAbs",
                                                (cubeSize)*(p_nXAxis),
                                                (cubeSize)*(p_nYAxis),
                                                (cubeSize + fAbsRadLen)*(p_nZAxis));

   G4LogicalVolume* logicAbsorber   = new G4LogicalVolume(solidAbsorber,
                                                          absMat,
                                                          "logicAbsorber");

   G4LogicalVolume* logicFinal      = new G4LogicalVolume(solidFinalAbsorber,
                                                          absMat,
                                                          "logicFinal");

   G4ThreeVector absPos             = G4ThreeVector(0,
                                                    0,
                                                    (cubeSize) + fAbsRadLen - 0.001*cm);

   new G4PVPlacement(0,                      //no rotation
                     absPos,                 //at (0,0,0)
                     logicDetectorXY,        //its logical volume
                     "absorberOfDetector",   //its name
                     logicAbsorber,          //its mother  volume
                     false,                  //no boolean operation
                     4,                      //copy number
                     checkOverlaps);         //overlaps checking

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Replicate pixel and absorber in XYZDir, and place it in the world
   ////////////////////////////////////////////////////////////////////////////////////////////////
   new G4PVReplica("detecLayerXYZ",             //Name
                   logicAbsorber,               //Logical Volume
                   logicFinal,                  //Mother volume
                   kZAxis,                      //Axis of replication
                   (p_nZAxis),                  //Number of replica
                   (cubeSize + fAbsRadLen)*2);  //Width of replica

   new G4PVPlacement(0,                         //no rotation
                     G4ThreeVector(0, 0, 0),    //at (0,0,0)
                     logicFinal,                //its logical volume
                     "DetectorAndAbs",          //its name
                     worldLog,                  //its mother  volume
                     false,                     //no boolean operation
                     5,                         //copy number
                     false);                    //overlaps checking

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //DEBUGGING: color/visible settings foe quartz, sipm, and absorber to check for collisions
   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Set some containers to invisible so we dont see their outlines
   logicFinal->SetVisAttributes(new G4VisAttributes(false));
   logicDetector->SetVisAttributes(new G4VisAttributes(false));
   logicDetectorX->SetVisAttributes(new G4VisAttributes(false));
   logicDetectorXY->SetVisAttributes(new G4VisAttributes(false));
   //logicAbsorber->SetVisAttributes(new G4VisAttributes(true));
   ////////////////////////////////////////////////////////////////////////////////////////////////
   //Set the Quartz Surface
   ////////////////////////////////////////////////////////////////////////////////////////////////
   G4OpticalSurface* quartzWrap = new G4OpticalSurface("QuartzWrap");
   quartzWrap->SetType(dielectric_metal);
   quartzWrap->SetFinish(polished);
   quartzWrap->SetModel(glisur);

   new G4LogicalBorderSurface("QuartzWrap",
                              quartzPlace,
                              physWorld,
                              quartzWrap);

   G4double pp[2]             = { 2.0*eV, 3.5*eV };
   G4double reflectivity[2]   = { 1., 1. };
   G4double efficiency[2]     = { 0.0, 0.0 };

   G4MaterialPropertiesTable* quartzWrapMPT = new G4MaterialPropertiesTable();
   quartzWrapMPT->AddProperty("REFLECTIVITY",   pp, reflectivity, 2);
   quartzWrapMPT->AddProperty("EFFICIENCY",     pp, efficiency,   2);

   quartzWrap->SetMaterialPropertiesTable(quartzWrapMPT);

   return physWorld;
}

//Defined the Sensitive Detectors
void qCalDetectorConstruction::ConstructSDandField()
{
   G4SDManager *sdMan = G4SDManager::GetSDMpointer();
   G4String SDname;

   G4VSensitiveDetector* sipm = new qCalSD(SDname="/SiPM");
   sdMan->AddNewDetector(sipm);
   logicSiPM->SetSensitiveDetector(sipm);
}
