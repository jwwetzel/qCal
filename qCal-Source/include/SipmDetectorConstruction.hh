#ifndef SipmDetectorConstruction_h
#define SipmDetectorConstruction_h 1

#include "SipmSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4Cache.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

class G4PhysicalVolume;
class G4LogicalVolume;

class SipmDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   SipmDetectorConstruction(const G4Int nXAxis, const G4Int nYAxis, const G4Int nZAxis, const G4Int zAbs);
   virtual ~SipmDetectorConstruction();
   
   virtual G4VPhysicalVolume* Construct();
   G4VPhysicalVolume* ConstructDetector();
   
private:
   const G4Int nXAxis = nXAxis;
}
