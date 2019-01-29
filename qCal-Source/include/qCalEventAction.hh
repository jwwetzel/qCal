#ifndef qCalEventAction_h
#define qCalEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
class G4Event;

class qCalEventAction : public G4UserEventAction
{
public:
   
   qCalEventAction();
   virtual ~qCalEventAction();
   
   virtual void BeginOfEventAction(const G4Event*);
   virtual void EndOfEventAction(const G4Event*);
//   virtual void SetSaveThreshold(G4int save);
   std::vector<G4double>& GetPhotonCount() { return fphotonCount; }
   std::vector<G4double>& GetSiPMNums() { return fSiPMNums; }
   
private:
   G4int              fSiPMCollID;
   G4int SDVolume;
   std::vector<G4double> fphotonCount;
   std::vector<G4double> fSiPMNums;

   
};

#endif
