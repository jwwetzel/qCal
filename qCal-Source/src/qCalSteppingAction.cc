#include "qCalSteppingAction.hh"
#include "qCalEventAction.hh"
#include "qCalTrackingAction.hh"
#include "qCalTrajectory.hh"
#include "qCalSD.hh"
#include "qCalUserTrackInformation.hh"
#include "qCalUserEventInformation.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"


qCalSteppingAction::qCalSteppingAction()
{   
   fExpectedNextStatus = Undefined;
}


qCalSteppingAction::~qCalSteppingAction()
{
   
}


void qCalSteppingAction::UserSteppingAction(G4Step * theStep)
{
   G4Track* theTrack = theStep->GetTrack();
}
