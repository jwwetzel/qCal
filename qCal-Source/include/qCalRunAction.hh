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
   explicit qCalRunAction(qCalEventAction* eventAction);
   ~qCalRunAction() override;
   
   void BeginOfRunAction(const G4Run*) override;
   void EndOfRunAction(const G4Run*) override;
   virtual void SetOutputFileName(const G4String& outputFileName);
   
private:
   G4String p_runActionOutputFileName;
   qCalRunMessenger* p_fRunMessenger;
   qCalEventAction* fEventAction;
   G4int SDVolume;
   
};

#endif
