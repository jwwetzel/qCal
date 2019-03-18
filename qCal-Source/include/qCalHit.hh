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
   qCalHit(G4ThreeVector i, G4double t, G4double E);
   virtual ~qCalHit();
   
   inline void *operator new(size_t);
   inline void operator delete(void *aHit);
   
   virtual void Print();
   
   inline G4int GetSiPMNumber() { return fSiPMNumber; }
   inline G4ThreeVector GetSiPMCoords() { return fCoord; }
   
   void SetTime(G4double val) { fTime = val; }
   G4double GetTime() const { return fTime; } 

   inline void SetEnergy(G4double E) {fEnergy = E;}
   inline G4double GetEnergy(){return fEnergy;}
   
   // From LXe:
   virtual void Draw();
   
   inline void SetSiPMNumber(G4int n) { fSiPMNumber = n; }
   
   inline void SetSiPMPhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
   inline G4VPhysicalVolume* GetSiPMPhysVol(){return fPhysVol;}
   
   inline void IncPhotonCount(){fPhotons++;}
   inline G4int GetPhotonCount(){return fPhotons;}
   
   inline void SetDrawit(G4bool b){fDrawit=b;}
   inline G4bool GetDrawit(){return fDrawit;}

   inline void NormalizeZCoord(G4double offset, G4double norm){fCoord = G4ThreeVector(fCoord.getX(), fCoord.getY(), (fCoord.getZ() - offset)/norm);}

private:
   G4double fTime;
   G4int fSiPMNumber;
   G4double fEnergy;
   G4VPhysicalVolume* fPhysVol;
   G4ThreeVector fCoord;
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
