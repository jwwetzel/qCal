#ifndef qCalRunAction_h
#define qCalRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "qCalRunMessenger.hh"

class qCalEventAction;
class G4Run;

class qCalRunAction : public G4UserRunAction
{
public:
   qCalRunAction(qCalEventAction* eventAction);
   virtual ~qCalRunAction();
   
   virtual void BeginOfRunAction(const G4Run*);
   virtual void EndOfRunAction(const G4Run*);
   virtual void SetOutputFileName(G4String outputFileName);
   
private:
   G4String p_runActionOutputFileName;
   qCalRunMessenger* p_fRunMessenger;
   qCalEventAction* fEventAction;
   G4int SDVolume;
   
};

#endif
