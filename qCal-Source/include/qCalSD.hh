#ifndef qCalSD_h
#define qCalSD_h 1

#include "qCalHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4LogicalVolume.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class qCalSD : public G4VSensitiveDetector
{
   public:
   
   qCalSD(G4String SDname);
   virtual ~qCalSD();

   //Required Functions
   virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* rohist);
   //A version of processHits that keeps aStep constant
   G4bool ProcessHits_constStep(const G4Step* ,
                                G4TouchableHistory* );
   virtual void Initialize(G4HCofThisEvent* hitCollection);
   virtual void EndOfEvent(G4HCofThisEvent* hitCollection );

   //User Functions
   void Clear();
   void DrawAll();
   void PrintAll();

   //Initialize the arrays to store pmt possitions
   inline void InitSiPMs(G4int nSiPMs){
      if(fSiPMPositionsX)delete fSiPMPositionsX;
      if(fSiPMPositionsY)delete fSiPMPositionsY;
      if(fSiPMPositionsZ)delete fSiPMPositionsZ;
      fSiPMPositionsX=new G4DataVector(nSiPMs);
      fSiPMPositionsY=new G4DataVector(nSiPMs);
      fSiPMPositionsZ=new G4DataVector(nSiPMs);
   }

   //Store a pmt position
   void SetSiPMPositions(const std::vector<G4ThreeVector>& positions);

   
   
   private:
   
   qCalSiPMHitsCollection* fSiPMHitCollection;

   G4DataVector* fSiPMPositionsX;
   G4DataVector* fSiPMPositionsY;
   G4DataVector* fSiPMPositionsZ;
};

#endif
