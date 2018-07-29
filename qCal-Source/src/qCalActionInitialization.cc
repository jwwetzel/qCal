#include "qCalActionInitialization.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
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
{
   //Grab the Detector construction to aim the gun at its center.
   p_theDetector = (qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
   p_gunXLocation = p_theDetector->GetAbsXdim();
   p_gunYLocation = p_theDetector->GetAbsYdim();
}


qCalActionInitialization::~qCalActionInitialization()
{}



void qCalActionInitialization::Build()const
{
   G4ThreeVector gunPosition = G4ThreeVector(p_gunXLocation, p_gunYLocation, -10*cm);
   G4ThreeVector gunMomentum = G4ThreeVector(0,0,1);
   SetUserAction(new qCalPrimaryGeneratorAction("mu-",120.0*GeV, gunPosition, gunMomentum));
}



void qCalActionInitialization::BuildForMaster() const
{

}
