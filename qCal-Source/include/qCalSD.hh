#ifndef qCalSD_h
#define qCalSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4LogicalVolume.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "qCalHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class qCalSD : public G4VSensitiveDetector
{
public:
   qCalSD(G4String SDname, G4String hitsCollectionName, G4LogicalVolume* sipmLog);
   virtual ~qCalSD();
   
   //Required Functions
   virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* rohist);
   virtual void Initialize(G4HCofThisEvent* hitCollection);
   virtual void EndOfEvent(G4HCofThisEvent* hitCollection );
   
   //User Functions
   void Clear();
   void DrawAll();
   void PrintAll();
   
      
private:
   
};
#endif
