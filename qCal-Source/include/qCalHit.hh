#ifndef qCalHit_h
#define qCalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4Allocator.hh"
#include "tls.hh"

class qCalHit : public G4VHit
{
public:
   qCalHit();
   virtual ~qCalHit();
   
   virtual void DrawHit();
   virtual void PrintHit();
   
};
#endif

typedef G4THitsCollection<qCalHit> qCalSipmHitCollection;
