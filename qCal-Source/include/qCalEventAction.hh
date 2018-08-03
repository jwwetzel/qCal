#ifndef qCalEventAction_h
#define qCalEventAction_h 1

#include "qCalEventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;

class qCalEventAction : public G4UserEventAction
{
public:
   
   qCalEventAction();
   virtual ~qCalEventAction();
   
public:
   
   virtual void BeginOfEventAction(const G4Event*);
   virtual void EndOfEventAction(const G4Event*);
   
   void SetSaveThreshold(G4int );
   
   void SetEventVerbose(G4int v){fVerbose=v;}
   
   void SetSiPMThreshold(G4int t){fSiPMThreshold=t;}
   
   void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
   void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}
   
private:
   
   qCalEventMessenger* fEventMessenger;
   
   G4int              fSaveThreshold;
   
   G4int              fSiPMCollID;
   
   G4int              fVerbose;
   
   G4int              fSiPMThreshold;
   
   G4bool fForcedrawphotons;
   G4bool fForcenophotons;
   
};

#endif
