#ifndef qCalPrimaryGeneratorAction_h
#define qCalPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"


class G4ParticleDefinition;
class G4Event;
class G4ParticleGun;

class qCalPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
   qCalPrimaryGeneratorAction();
   virtual ~qCalPrimaryGeneratorAction();
   virtual void GeneratePrimaries(G4Event* event);
   
private:
   G4ParticleGun* gun;
   G4ParticleDefinition* muonP;

};
#endif

