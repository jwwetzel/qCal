#ifndef qCalRunMessenger_h
#define qCalRunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"


class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class qCalRunAction;

class qCalRunMessenger: public G4UImessenger
{
public:
   
   qCalRunMessenger(qCalRunAction*);
   virtual ~qCalRunMessenger();
   
   virtual void SetNewValue(G4UIcommand*, G4String);
   
private:
   qCalRunAction*          p_fqCalRunAction;
   
   G4UIdirectory*          p_qCalDirectory;
   G4UIcmdWithAString*     p_fOutputFileNameCmd;
};

#endif
