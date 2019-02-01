#include "qCalRunMessenger.hh"
#include "qCalRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4RunManager.hh"

#include "G4UIcmdWithAString.hh"

qCalRunMessenger::qCalRunMessenger(qCalRunAction* run)
: p_fqCalRunAction(run)
{
   p_fOutputFileNameCmd = new G4UIcmdWithAString("/qCal/outputFileName", this);
   p_fOutputFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
   p_fOutputFileNameCmd->SetDefaultValue("outputFile");
}

qCalRunMessenger::~qCalRunMessenger()
{
   delete p_fOutputFileNameCmd;
   delete p_qCalDirectory;
}

void qCalRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
   if ( command == p_fOutputFileNameCmd )
   {
      p_fqCalRunAction->SetOutputFileName(newValue);
   }
}
