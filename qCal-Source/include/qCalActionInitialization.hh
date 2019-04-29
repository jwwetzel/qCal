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
    ~qCalActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

private:
   qCalDetectorConstruction* p_theDetector;
   G4double p_gunXLocation;
   G4double p_gunYLocation;
   G4double p_gunZLocation;
   G4String startingParticle;
   G4int startingEnergy;
};
#endif

