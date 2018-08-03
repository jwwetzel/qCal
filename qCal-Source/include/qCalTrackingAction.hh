#ifndef qCalTrackingAction_h
#define qCalTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class qCalTrackingAction : public G4UserTrackingAction {
   
public:
   
   qCalTrackingAction();
   virtual ~qCalTrackingAction() {};
   
   virtual void PreUserTrackingAction(const G4Track*);
   virtual void PostUserTrackingAction(const G4Track*);
   
private:
   
};

#endif

