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
                            G4double fAbsLen = 1.0,
                            G4double fCubeWidth = 1.0,
                            G4double fQuartzDepth = 1.0,
                            G4double fPMTDepth = 1.0,
                            G4double fDetecWidth = 1.0
   );
   virtual ~qCalDetectorConstruction();

   virtual G4VPhysicalVolume* Construct();
   virtual void ConstructSDandField();

   G4double GetAbsXdim(){return p_fAbsXDim;};
   G4double GetAbsYdim(){return p_fAbsYDim;};
   G4double GetAbsZdim(){return p_fAbsZDim;};
   G4double GetHalfDetSize() {return negHalfDetSize;}
   G4int GetnXAxis(){return p_nXAxis;};
   G4int GetnYAxis(){return p_nYAxis;};
   G4int GetnZAxis(){return p_nZAxis;};
   G4double GetAbsLen(){return p_fAbsLen;};
   G4double GetCubeSize(){return p_sdCubeSize;};
   G4int GetVolume(){return p_nXAxis * p_nYAxis * p_nZAxis;};
   std::vector<G4double>& GetDetectorAxisValues(){return p_vDetectorAxisValues;}
   G4String GetAbsMaterial(){return p_sAbs;}
   G4double GetCoordScaleZ(){return (p_fAbsLen + p_sdCubeSize)/cm;}
   G4double GetCoordScaleXY(){return p_sdCubeSize/cm;}
   G4double GetCoordOffsetZ(){return p_foffsetZ;}
   void SetCoordOffsetZ(G4double newOffsetZ){p_foffsetZ = newOffsetZ;}
   G4int RawCoordsToSiPMNumber(const G4ThreeVector &raw);


private:
   G4int p_nXAxis;
   G4int p_nYAxis;
   G4int p_nZAxis;
   G4String p_sAbs;
   G4double p_fCubeWidth;
   G4double p_fQuartzDepth;
   G4double p_fAbsXDim;
   G4double p_fAbsYDim;
   G4double p_fAbsZDim;
   G4double p_fQuartzSpacing;
   G4double p_fWrapSize;
   G4double p_SiPMDim;
   G4double p_fAbsLen;
   std::vector<G4double> p_vDetectorAxisValues;
   G4double p_sdCubeSize;
   G4double p_foffsetZ;
   G4double p_fscaleZ;
   G4double p_PMTBackDim;
   G4LogicalVolume* logicSiPM;
   G4double negHalfDetSize;
   //The SiPM SDs:
   G4Cache<qCalSD*> fSiPM_SD;
   std::vector<G4ThreeVector> fSiPMPositions;
};
#endif
