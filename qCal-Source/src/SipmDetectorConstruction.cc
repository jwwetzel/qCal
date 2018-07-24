#include "SipmDetectorConstruction.hh"
#include "SipmSD.hh"

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

SipmDetectorConstruction::SipmDetectorConstruction(const G4Int nXAxis, const G4Int nYAxis, const G4Int nZAxis, const G4Int zAbs)
: G4VUserDetectorConstruction(), numSipmsOnEachSide(numSipmsOnSide), numberOfSipms(std::pow(numSipmsOnSide,3))
{}
