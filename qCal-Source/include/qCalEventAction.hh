#ifndef qCalEventAction_h
#define qCalEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "qCalHit.hh"
#include "qCalSD.hh"
#include "qCalDetectorConstruction.hh"

class G4Event;

class qCalEventAction : public G4UserEventAction
{
public:
   
   qCalEventAction();
   virtual ~qCalEventAction();
   
   virtual void BeginOfEventAction(const G4Event*);
   virtual void EndOfEventAction(const G4Event*);
   //   virtual void SetSaveThreshold(G4int save);
   std::vector<G4double>& GetPhotonCount() { return fphotonCount;}
   std::vector<G4double>& GetSiPMNums() {return fSiPMNums;}
   std::vector<G4double>& GetHitTimes() {return fHitTimes;}

   std::vector<G4int>& GetSiPMCoordinatesX() { return fSiPMCordsX; }
   std::vector<G4int>& GetSiPMCoordinatesY() { return fSiPMCordsY; }
   std::vector<G4int>& GetSiPMCoordinatesZ() { return fSiPMCordsZ; }


private:
   G4int fSiPMCollID;
   qCalDetectorConstruction* eventDetector;
   G4int SDVolume;
   std::vector<G4double> fphotonCount;
   std::vector<G4double> fSiPMNums;
   std::vector<G4double> fHitTimes;

   std::vector<G4int> fSiPMCordsX;
   std::vector<G4int> fSiPMCordsY;
   std::vector<G4int> fSiPMCordsZ;

};

#endif
