#include "qCalActionInitialization.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalPrimaryGeneratorAction.hh"
//#include "qCalEventAction.hh"
//#include "qCalRunAction.hh"
//#include "qCalStackingAction.hh"
//#include "qCalSteppingAction.hh"
//#include "qCalSteppingVerbose.hh"
//#include "qCalRecorderBase.hh"
//#include "qCalTrackingAction.hh"

//constructor & destructor
qCalActionInitialization::qCalActionInitialization()
: G4VUserActionInitialization()
{}


qCalActionInitialization::~qCalActionInitialization()
{}



void qCalActionInitialization::Build()const
{
   SetUserAction(new qCalPrimaryGeneratorAction);
}



void qCalActionInitialization::BuildForMaster() const
{

}
