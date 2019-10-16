#include "qCalActionInitialization.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalPrimaryGeneratorAction.hh"
#include "qCalEventAction.hh"
#include "qCalRunAction.hh"
#include "qCalTrackingAction.hh"
#include "qCalSteppingAction.hh"

#include "G4ThreeVector.hh"
#include "G4RunManager.hh"


//constructor
qCalActionInitialization::qCalActionInitialization(G4String sParticle, G4int sEnergy)
: G4VUserActionInitialization()
{
   // Grab the Detector construction to aim the gun at its center.
   // You can get access to the detector dimensions for positioning the gun with:
   // p_theDetector->GetAbsXdim(), or GetAbsYdim(), or GetAbsZdim();
   
   p_theDetector  = (qCalDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
   p_gunXLocation = 0; //(1 - p_theDetector->GetnXAxis()%2 ) * 0.5*p_theDetector->GetCubeSize();
   p_gunYLocation = 0; //(1 - p_theDetector->GetnYAxis()%2 )* 0.5*p_theDetector->GetCubeSize();
   p_gunZLocation = p_theDetector->GetHalfDetSize();//p_theDetector->GetHalfDetSize();
   //G4cout << "AbsZDim = " << p_theDetector->GetAbsZdim() << G4endl;
   //G4cout << "neg = " << p_theDetector->GetHalfDetSize() << G4endl;
   //p_gunZLocation = p_theDetector->GetAbsZdim()+1*cm;

   startingParticle = sParticle;
   startingEnergy = sEnergy;
   G4cout << "************************************"   << G4endl;
   G4cout << "***    Action Initialization     ***"   << G4endl;
   G4cout << "************************************"   << G4endl;
   G4cout << "The Gun is positions at: "              << G4endl;
   G4cout << "X: " << p_gunXLocation/cm               << G4endl;
   G4cout << "Y: " << p_gunYLocation/cm               << G4endl;
   G4cout << "Z: " << p_gunZLocation                  << G4endl;
   G4cout << "************************************"   << G4endl;
   G4cout << "************************************"   << G4endl;
}

//destructor
qCalActionInitialization::~qCalActionInitialization() = default;

//Build method
void qCalActionInitialization::Build()const
{
   const G4ThreeVector gunPosition = G4ThreeVector(p_gunXLocation, p_gunYLocation, p_gunZLocation);
   G4ThreeVector gunMomentum = G4ThreeVector(0,0,-1);
   SetUserAction(new qCalPrimaryGeneratorAction(startingParticle,startingEnergy, gunPosition, gunMomentum));

   auto eventAction = new qCalEventAction;
   SetUserAction(eventAction);
   SetUserAction(new qCalRunAction(eventAction));
   
   G4cout << "The Gun's Config is: " << G4endl;
   G4cout << "Particle: " << startingParticle             << G4endl;
   G4cout << "Energy: " << startingEnergy               << G4endl;

}

//Build for Master
void qCalActionInitialization::BuildForMaster() const
{
   auto eventAction = new qCalEventAction;
   SetUserAction(new qCalRunAction(eventAction));
}
