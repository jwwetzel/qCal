#ifndef qCalActionInitialization_h
#define qCalActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "qCalDetectorConstruction.hh"

class qCalDetectorConstruction;
//class qCalRecorderBase;

class qCalActionInitialization : public G4VUserActionInitialization
{
public:
    qCalActionInitialization(G4String sParticle, G4int sEnergy);
    virtual ~qCalActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
   qCalDetectorConstruction* p_theDetector;
   G4float p_gunXLocation;
   G4float p_gunYLocation;
   G4float p_gunZLocation;
   G4String startingParticle;
   G4int startingEnergy;
};
#endif

