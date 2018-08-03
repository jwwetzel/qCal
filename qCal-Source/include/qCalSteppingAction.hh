#ifndef qCalSteppingAction_H
#define qCalSteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class qCalEventAction;
class qCalTrackingAction;
class qCalSteppingMessenger;

class qCalSteppingAction : public G4UserSteppingAction
{
public:
   
   qCalSteppingAction();
   virtual ~qCalSteppingAction();
   virtual void UserSteppingAction(const G4Step*);
   
   void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
   G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
   
private:
   
   G4bool fOneStepPrimaries;
   qCalSteppingMessenger* fSteppingMessenger;
   
   G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
