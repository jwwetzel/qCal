#ifndef qCalTrajectory_h
#define qCalTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class qCalTrajectory : public G4Trajectory
{
public:
   
   qCalTrajectory();
   qCalTrajectory(const G4Track* aTrack);
   qCalTrajectory(qCalTrajectory &);
   virtual ~qCalTrajectory();
   
   virtual void DrawTrajectory() const;
   
   inline void* operator new(size_t);
   inline void  operator delete(void*);
   
   void SetDrawTrajectory(G4bool b){fDrawit=b;}
   void WLS(){fWls=true;}
   void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
   void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}
   
private:
   
   G4bool fWls;
   G4bool fDrawit;
   G4bool fForceNoDraw;
   G4bool fForceDraw;
   G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<qCalTrajectory>* qCalTrajectoryAllocator;

inline void* qCalTrajectory::operator new(size_t)
{
   if(!qCalTrajectoryAllocator)
      qCalTrajectoryAllocator = new G4Allocator<qCalTrajectory>;
   return (void*)qCalTrajectoryAllocator->MallocSingle();
}

inline void qCalTrajectory::operator delete(void* aTrajectory)
{
   qCalTrajectoryAllocator->FreeSingle((qCalTrajectory*)aTrajectory);
}

#endif

