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
   qCalDetectorConstruction(G4int nXAxis = 1,
                            G4int nYAxis = 1,
                            G4int nZAxis = 1,
                            G4String sAbs = "Iron",
                            G4float fCubeWidth = 1.0
                            );
   virtual ~qCalDetectorConstruction();
   
   virtual G4VPhysicalVolume* Construct();
   
   
private:
   G4int p_nXAxis;
   G4int p_nYAxis;
   G4int p_nZAxis;
   G4String p_sAbs;
   G4float p_fCubeWidth;
};
#endif
