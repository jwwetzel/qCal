#ifndef qCalEventMessenger_h
#define qCalEventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class qCalEventAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class qCalEventMessenger: public G4UImessenger
{
public:
   
   qCalEventMessenger(qCalEventAction*);
   virtual ~qCalEventMessenger();
   
   virtual void SetNewValue(G4UIcommand*, G4String);
   
private:
   
//   qCalEventAction*        fqCalEvent;
   G4UIcmdWithAnInteger*   fSaveThresholdCmd;
//   G4UIcmdWithAnInteger*   fVerboseCmd;
//   G4UIcmdWithAnInteger*   fSiPMThresholdCmd;
//   G4UIcmdWithABool*       fForceDrawPhotonsCmd;
//   G4UIcmdWithABool*       fForceDrawNoPhotonsCmd;
};

#endif

