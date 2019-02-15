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
   
private:
   
   G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
