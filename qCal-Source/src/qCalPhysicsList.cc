/*
 This file and code are based heavily on the LXe Example https://github.com/Geant4/geant4/tree/master/examples/extended/optical/LXe
 */
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Geantino.hh"
#include "G4ChargedGeantino.hh"
#include "G4GenericIon.hh"
#include "G4Proton.hh"

#include "qCalPhysicsList.hh"
//#include "qCalEMPhysicsList.hh"
//#include "qCalGeneralPhysics.hh"
//#include "qCalMuonPhysicsList.hh"

qCalPhysicsList::qCalPhysicsList()
: G4VModularPhysicsList()
{
}


qCalPhysicsList::~qCalPhysicsList(){}


void qCalPhysicsList::SetCuts()
{
   SetCutsWithDefault();
}



