/*
 This file and code are based heavily on the LXe Example https://github.com/Geant4/geant4/tree/master/examples/extended/optical/LXe
 */
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "qCalPrimaryGeneratorAction.hh"


qCalPrimaryGeneratorAction::qCalPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), gun(nullptr), muonP(G4ParticleTable::GetParticleTable()->FindParticle("mu+"))
{
    G4int numParticles = 1;
    gun = new G4ParticleGun(numParticles);
    gun->SetParticleDefinition(muonP);
    gun->SetParticlePosition(G4ThreeVector(-5.0,1.0,0.8));
    gun->SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
}
    
// destructor
qCalPrimaryGeneratorAction::~qCalPrimaryGeneratorAction()
{
    delete gun;
    delete muonP;
}
    
//invoked once each event
void qCalPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    gun->GeneratePrimaryVertex(event);
}


