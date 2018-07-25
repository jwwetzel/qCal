#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "qCalPrimaryGeneratorAction.hh"


qCalPrimaryGeneratorAction::qCalPrimaryGeneratorAction(const G4String& particleName,
                                                       G4double energy,
                                                       G4ThreeVector position,
                                                       G4ThreeVector momentumDirection
                                                       ) :  G4VUserPrimaryGeneratorAction(),
                                                            gun(nullptr)
{
   G4int numParticles = 1;
   gun = new G4ParticleGun(numParticles);
   gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle(particleName));
   gun->SetParticlePosition(position);
   gun->SetParticleEnergy(energy);
   gun->SetParticleMomentumDirection(momentumDirection);
}
    
// destructor
qCalPrimaryGeneratorAction::~qCalPrimaryGeneratorAction()
{
   delete gun;
}
    
//invoked once each event
void qCalPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    gun->GeneratePrimaryVertex(event);
}


