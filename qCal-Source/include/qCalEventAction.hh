#ifndef qCalEventAction_h
#define qCalEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4SystemOfUnits.hh"
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

   std::vector<G4int>& GetSiPMCoordsX() { return iSiPMCoordsX; }
   std::vector<G4int>& GetSiPMCoordsY() { return iSiPMCoordsY; }
   std::vector<G4int>& GetSiPMCoordsZ() { return iSiPMCoordsZ; }

private:
   G4int fSiPMCollID;
   qCalDetectorConstruction* eventDetector;
   G4int SDVolume;
   std::vector<G4double> fphotonCount;
   std::vector<G4double> fSiPMNums;
   std::vector<G4double> fHitTimes;
   std::vector<G4double> fInitialHitTimes;
   std::vector<G4double> fFinalHitTimes;

   std::vector<G4int> iSiPMCoordsX;
   std::vector<G4int> iSiPMCoordsY;
   std::vector<G4int> iSiPMCoordsZ;

};

#endif
