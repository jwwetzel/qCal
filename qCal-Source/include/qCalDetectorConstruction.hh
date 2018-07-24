#ifndef qCalDetectorConstruction_h
#define qCalDetectorConstruction_h 1

//#include "qCalSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4Cache.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

class G4PhysicalVolume;
class G4LogicalVolume;

class qCalDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   qCalDetectorConstruction();
   virtual ~qCalDetectorConstruction();
   
   virtual G4VPhysicalVolume* Construct();
   
   
private:

};
#endif
