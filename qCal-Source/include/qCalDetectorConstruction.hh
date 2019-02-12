#ifndef qCalDetectorConstruction_h
#define qCalDetectorConstruction_h 1

#include "qCalSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
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
                            G4String sAbs = "Fe",
                            G4float fAbsLen = 1.0,
                            G4float fCubeWidth = 1.0
   );
   virtual ~qCalDetectorConstruction();

   virtual G4VPhysicalVolume* Construct();
   virtual void ConstructSDandField();

   G4double GetAbsXdim(){return p_fAbsXDim;};
   G4double GetAbsYdim(){return p_fAbsYDim;};
   G4double GetAbsZdim(){return p_fAbsZDim;};
   G4int GetVolume(){return p_nXAxis * p_nYAxis * p_nZAxis;};
private:
   G4int p_nXAxis;
   G4int p_nYAxis;
   G4int p_nZAxis;
   G4String p_sAbs;
   G4float p_fCubeWidth;
   G4float p_fAbsXDim;
   G4float p_fAbsYDim;
   G4float p_fAbsZDim;
   G4float p_fQuartzSpacing;
   G4float p_fWrapSize;
   G4float p_SiPMDim;
   G4float p_fAbsLen;

   G4LogicalVolume* logicSiPM;

   //The SiPM SDs:
   G4Cache<qCalSD*> fSiPM_SD;
   std::vector<G4ThreeVector> fSiPMPositions;
};
#endif