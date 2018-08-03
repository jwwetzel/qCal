#include "qCalEventMessenger.hh"
#include "qCalEventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

qCalEventMessenger::qCalEventMessenger(qCalEventAction* event)
: fqCalEvent(event)
{
//   fSaveThresholdCmd = new G4UIcmdWithAnInteger("/qCal/saveThreshold",this);
//   fSaveThresholdCmd->SetGuidance("Set the photon count threshold for saving the random number seed");
//   fSaveThresholdCmd->SetParameterName("photons",true);
//   fSaveThresholdCmd->SetDefaultValue(4500);
//   fSaveThresholdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
//
//   fVerboseCmd = new G4UIcmdWithAnInteger("/qCal/eventVerbose",this);
//   fVerboseCmd->SetGuidance("Set the verbosity of event data.");
//   fVerboseCmd->SetParameterName("verbose",true);
//   fVerboseCmd->SetDefaultValue(1);
//
//   fSiPMThresholdCmd = new G4UIcmdWithAnInteger("/qCal/SiPMThreshold",this);
//   fSiPMThresholdCmd->SetGuidance("Set the SiPMThreshold (in # of photons)");
//
//   fForceDrawPhotonsCmd=new G4UIcmdWithABool("/qCal/forceDrawPhotons",this);
//   fForceDrawPhotonsCmd->SetGuidance("Force drawing of photons.");
//   fForceDrawPhotonsCmd->SetGuidance("(Higher priority than /qCal/forceDrawNoPhotons)");
//
//   fForceDrawNoPhotonsCmd=new G4UIcmdWithABool("/qCal/forceDrawNoPhotons",this);
//   fForceDrawNoPhotonsCmd->SetGuidance("Force no drawing of photons.");
//   fForceDrawNoPhotonsCmd->SetGuidance("(Lower priority than /qCal/forceDrawPhotons)");
}


qCalEventMessenger::~qCalEventMessenger(){
//   delete fSaveThresholdCmd;
//   delete fVerboseCmd;
//   delete fSiPMThresholdCmd;
//   delete fForceDrawPhotonsCmd;
//   delete fForceDrawNoPhotonsCmd;
}


void qCalEventMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
//   if( command == fSaveThresholdCmd ){
//      fqCalEvent->SetSaveThreshold(fSaveThresholdCmd->GetNewIntValue(newValue));
//   }
//   else if( command == fVerboseCmd ){
//      fqCalEvent->SetEventVerbose(fVerboseCmd->GetNewIntValue(newValue));
//   }
//   else if( command == fSiPMThresholdCmd ){
//      fqCalEvent->SetSiPMThreshold(fSiPMThresholdCmd->GetNewIntValue(newValue));
//   }
//   else if(command == fForceDrawPhotonsCmd){
//      fqCalEvent->SetForceDrawPhotons(fForceDrawPhotonsCmd
//                                     ->GetNewBoolValue(newValue));
//   }
//   else if(command == fForceDrawNoPhotonsCmd){
//      fqCalEvent->SetForceDrawNoPhotons(fForceDrawNoPhotonsCmd
//                                       ->GetNewBoolValue(newValue));
//      G4cout<<"TEST"<<G4endl;
//   }
}
