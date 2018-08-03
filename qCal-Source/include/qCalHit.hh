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

class G4VTouchable;

class qCalHit : public G4VHit
{
public:
   qCalHit();
   virtual ~qCalHit();
   
   virtual void Draw();
   virtual void Print();
   
   inline void *operator new(size_t);
   inline void operator delete(void *aHit);
   
   inline void SetSiPMNumber(G4int n) { fSiPMNumber = n; }
   inline G4int GetSiPMNumber() { return fSiPMNumber; }
   
   inline void SetSiPMPhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
   inline G4VPhysicalVolume* GetSiPMPhysVol(){return fPhysVol;}
   
   inline void IncPhotonCount(){fPhotons++;}
   inline G4int GetPhotonCount(){return fPhotons;}
   
   inline void SetDrawit(G4bool b){fDrawit=b;}
   inline G4bool GetDrawit(){return fDrawit;}
   
   inline void SetSiPMPos(G4double x,G4double y,G4double z)
   {
      fPos=G4ThreeVector(x,y,z);
   }
   
       inline G4ThreeVector GetSiPMPos(){return fPos;}
   
private:
   G4int fSiPMNumber;
   G4VPhysicalVolume* fPhysVol;
   G4ThreeVector fPos;
   G4int fPhotons;
   G4bool fDrawit;
   
};

typedef G4THitsCollection<qCalHit> qCalSiPMHitsCollection;

extern G4ThreadLocal G4Allocator<qCalHit>* qCalSiPMHitAllocator;

inline void* qCalHit::operator new(size_t)
{
   if(!qCalSiPMHitAllocator)
      qCalSiPMHitAllocator = new G4Allocator<qCalHit>;
   return (void *) qCalSiPMHitAllocator->MallocSingle();
}

inline void qCalHit::operator delete(void *aHit)
{
   qCalSiPMHitAllocator->FreeSingle((qCalHit*) aHit);
}

#endif
